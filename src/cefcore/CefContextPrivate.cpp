#include "CefContextPrivate.h"

#include <CefContext.h>

const int64_t kCefWorkerIntervalMs = (1000 / 60); // 60 fps

CefContextPrivate::CefContextPrivate(QCoreApplication* app, int argc, char** argv)
  : argc_(argc)
  , argv_(argv)
  , config_(nullptr)
{
  cefWorkerTimer_.setTimerType(Qt::PreciseTimer);
  connect(&cefWorkerTimer_, SIGNAL(timeout()), this, SLOT(performCefLoopWork()));
  connect(app, SIGNAL(aboutToQuit()), this, SLOT(onAboutToQuit()));
}

CefContextPrivate::~CefContextPrivate()
{
  disconnect(&cefWorkerTimer_, SIGNAL(timeout()), this, SLOT(performCefLoopWork()));
  disconnect(SIGNAL(aboutToQuit()), this, SLOT(onAboutToQuit()));
}

bool CefContextPrivate::initialize(const CefConfig* config)
{
  config_ = config;

  // initialize CEF
  if (!initializeCef(config)) {
    return false;
  }

  // start message pump timer
  if (!config_->standaloneMessageLoopEnabled()) {
    cefWorkerTimer_.start(kCefWorkerIntervalMs);
  }

  return true;
}

const CefConfig* CefContextPrivate::cefConfig() const
{
  return config_;
}

void CefContextPrivate::uninitialize()
{
  // cleanup CEF
  uninitializeCef();
}

void CefContextPrivate::scheduleCefLoopWork(int64_t delayMs)
{
  // calculate the effective delay number
  auto delay = qMax((int64_t)0, qMin(delayMs, kCefWorkerIntervalMs));
  QTimer::singleShot(static_cast<int>(delay), this, SLOT(performCefLoopWork()));
}

void CefContextPrivate::onAboutToQuit()
{
  if (!pApp_) {
    return;
  }

  // close all live browsers
  QCefViewPrivate::destroyAllInstance();

  // check whether can exit now
  if (!pApp_->IsSafeToExit()) {
    // launch an event loop to wait for the clean process
    // of CEF browsers resource
    QEventLoop exitCleanLoop;

    // create an idle timer to check whether the CEF resource has been
    // cleaned up. If yes then exit the event loop to continue the
    // application exit process
    QTimer exitCheckTimer;
    connect(&exitCheckTimer, &QTimer::timeout, [&]() {
      // if all browser were closed and there is only one reference to the
      // CefBrowserClient object (only referred by QCefContextPrivate instance),
      // we can quit safely
      if (pApp_->IsSafeToExit())
        exitCleanLoop.quit();
    });

    // start the timer
    exitCheckTimer.start(0);

    // enter the event loop
    exitCleanLoop.exec();
  }
}

void CefContextPrivate::performCefLoopWork()
{
  // process cef work
  CefDoMessageLoopWork();
}