#ifndef CEFVIEWPRIVATE_H
#define CEFVIEWPRIVATE_H
#pragma once

#include <CefView.h>
#include <include/cef_app.h>
#include <include/cef_browser.h>

#include <QSet>

#include "cefcore/CefViewDelegate.h"

class CefBrowserClient;

class CefViewPrivate : public QObject, public CefViewDelegate
{
  Q_OBJECT
public:
  explicit CefViewPrivate(CefView* q);
  ~CefViewPrivate() override;

  void createBrowser(const QString& url);
  void destroyBrowser();
  void syncBrowserGeometry();
  CefRefPtr<CefBrowser> browser() const;

  /// Closes every live browser and pumps the CEF message loop until all
  /// of them are actually closed. Must run on the Qt UI thread before
  /// CefShutdown().
  static void destroyAllInstance();

  // CefViewDelegate (called on the CEF UI thread)
  void onBrowserCreated() override;
  void onLoadingStateChanged(bool isLoading, bool canGoBack, bool canGoForward) override;
  void onLoadEnd(int httpStatusCode) override;
  void onTitleChanged(const QString& title) override;
  void onAddressChanged(const QString& url) override;

  CefView* q_;
  CefRefPtr<CefBrowserClient> client_;
  QString url_;
  bool loading_ = false;
  bool canGoBack_ = false;
  bool canGoForward_ = false;

private:
  static QSet<CefViewPrivate*> sLiveInstances;
};

#endif // CEFVIEWPRIVATE_H