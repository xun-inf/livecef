#include "CefViewPrivate.h"

#include "cefcore/CefBrowserClient.h"

#include <QElapsedTimer>
#include <QResizeEvent>
#include <QThread>

#if defined(OS_WINDOWS)
#include <windows.h>
#endif

QSet<CefViewPrivate*> CefViewPrivate::sLiveInstances;

CefViewPrivate::CefViewPrivate(CefView* q)
  : q_(q)
{}

CefViewPrivate::~CefViewPrivate()
{
  sLiveInstances.remove(this);
}

CefRefPtr<CefBrowser>
CefViewPrivate::browser() const
{
  return client_ ? client_->browser() : nullptr;
}

void
CefViewPrivate::createBrowser(const QString& url)
{
  if (client_)
    return;

  client_ = new CefBrowserClient(this);
  sLiveInstances.insert(this);

  CefWindowInfo windowInfo;
  windowInfo.SetAsChild(reinterpret_cast<cef_window_handle_t>(q_->winId()), CefRect(0, 0, q_->width(), q_->height()));

  CefBrowserSettings browserSettings;
  CefBrowserHost::CreateBrowser(windowInfo, client_, url.toStdString(), browserSettings, nullptr, nullptr);
}

void
CefViewPrivate::destroyBrowser()
{
  if (!client_)
    return;

  // the view is going away, stop forwarding CEF callbacks to it
  client_->resetDelegate();
  client_->closeBrowser();
}

void
CefViewPrivate::destroyAllInstance()
{
  const auto instances = sLiveInstances.values();

  // ask every live browser to close
  for (CefViewPrivate* instance : instances) {
    if (instance->client_ && instance->client_->browser())
      instance->client_->closeBrowser();
  }

  // pump the CEF message loop until all of them are gone, because
  // browser closing is asynchronous and CefShutdown() asserts on
  // any browser still alive
  QElapsedTimer timeout;
  timeout.start();
  while (timeout.elapsed() < 3000) {
    bool anyOpen = false;
    for (CefViewPrivate* instance : instances) {
      if (instance->client_ && instance->client_->browser()) {
        anyOpen = true;
        break;
      }
    }
    if (!anyOpen)
      break;

    CefDoMessageLoopWork();
    QThread::msleep(5);
  }
}

void
CefViewPrivate::syncBrowserGeometry()
{
  CefRefPtr<CefBrowser> b = browser();
  if (!b)
    return;

#if defined(OS_WINDOWS)
  HWND hwnd = b->GetHost()->GetWindowHandle();
  if (hwnd)
    ::SetWindowPos(hwnd, nullptr, 0, 0, q_->width(), q_->height(), SWP_NOZORDER | SWP_NOACTIVATE);
#endif
  b->GetHost()->NotifyMoveOrResizeStarted();
}

void
CefViewPrivate::onBrowserCreated()
{
  // the browser is created asynchronously with the geometry the widget had
  // at construction time (usually 0x0), so sync the real size once it exists
  QMetaObject::invokeMethod(q_, [this]() { syncBrowserGeometry(); }, Qt::QueuedConnection);
}

void
CefViewPrivate::onLoadingStateChanged(bool isLoading, bool canGoBack, bool canGoForward)
{
  QMetaObject::invokeMethod(
    q_,
    [this, isLoading, canGoBack, canGoForward]() {
      loading_ = isLoading;
      canGoBack_ = canGoBack;
      canGoForward_ = canGoForward;
      Q_EMIT q_->loadingStateChanged(isLoading, canGoBack, canGoForward);
    },
    Qt::QueuedConnection);
}

void
CefViewPrivate::onLoadEnd(int httpStatusCode)
{
  QMetaObject::invokeMethod(
    q_, [this, httpStatusCode]() { Q_EMIT q_->loadFinished(httpStatusCode); }, Qt::QueuedConnection);
}

void
CefViewPrivate::onTitleChanged(const QString& title)
{
  QMetaObject::invokeMethod(q_, [this, title]() { Q_EMIT q_->titleChanged(title); }, Qt::QueuedConnection);
}

void
CefViewPrivate::onAddressChanged(const QString& url)
{
  QMetaObject::invokeMethod(
    q_,
    [this, url]() {
      url_ = url;
      Q_EMIT q_->urlChanged(url);
    },
    Qt::QueuedConnection);
}

CefView::CefView(const QString& url, QWidget* parent)
  : QWidget(parent)
  , d(new CefViewPrivate(this))
{
  d->url_ = url;
  setAttribute(Qt::WA_NativeWindow);
  setAttribute(Qt::WA_DontCreateNativeAncestors);
  d->createBrowser(url);
}

CefView::~CefView()
{
  d->destroyBrowser();
  delete d;
}

void
CefView::loadUrl(const QString& url)
{
  d->url_ = url;
  if (CefRefPtr<CefBrowser> b = d->browser())
    b->GetMainFrame()->LoadURL(url.toStdString());
}

QString
CefView::url() const
{
  return d->url_;
}

void
CefView::goBack()
{
  if (CefRefPtr<CefBrowser> b = d->browser())
    b->GoBack();
}

void
CefView::goForward()
{
  if (CefRefPtr<CefBrowser> b = d->browser())
    b->GoForward();
}

bool
CefView::canGoBack() const
{
  return d->canGoBack_;
}

bool
CefView::canGoForward() const
{
  return d->canGoForward_;
}

void
CefView::reload()
{
  if (CefRefPtr<CefBrowser> b = d->browser())
    b->Reload();
}

void
CefView::stopLoad()
{
  if (CefRefPtr<CefBrowser> b = d->browser())
    b->StopLoad();
}

bool
CefView::isLoading() const
{
  return d->loading_;
}

void
CefView::resizeEvent(QResizeEvent* event)
{
  QWidget::resizeEvent(event);
  d->syncBrowserGeometry();
}