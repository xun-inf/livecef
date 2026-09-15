#pragma once

#include "qtimer.h"
#pragma region qt_headers
#include <QCoreApplication>
#include <QObject>
#include <QTimer>
#pragma endregion

#include "CefConfigPrivate.h"

class CefContextPrivate : public QObject
{
  Q_OBJECT

private:
  int argc_;

  char** argv_;

  const CefConfig* config_;

  QTimer cefWorkerTimer_;

#if defined(Q_OS_WINDOWS)
  HANDLE windowsJobHandle_ = nullptr;

  QString windowsJobName_;
#endif

public:
  CefContextPrivate(QCoreApplication* app, int argc, char** argv);

  ~CefContextPrivate();

  bool initialize(const CefConfig* config);

  const CefConfig* cefConfig() const;

  void uninitialize();

  void scheduleCefLoopWork(int64_t delayMs);

public slots:
  void onAboutToQuit();

  void performCefLoopWork();

protected:
  bool initializeCef(const CefConfig* config);

  void uninitializeCef();
};