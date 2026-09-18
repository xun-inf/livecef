#pragma once

#pragma region qt_headers
#include <QCoreApplication>
#include <QObject>
#include <QTimer>
#pragma endregion

#include <qcefconfig_p.h>

class QCefContextPrivate : public QObject
{
  Q_OBJECT

private:
  int argc_;

  char** argv_;

  const QCefConfig* config_;

  QTimer cefWorkerTimer_;

#if defined(Q_OS_WINDOWS)
  HANDLE windowsJobHandle_ = nullptr;

  QString windowsJobName_;
#endif

public:
  QCefContextPrivate(QCoreApplication* app, int argc, char** argv);

  ~QCefContextPrivate();

  bool initialize(const QCefConfig* config);

  const QCefConfig* cefConfig() const;

  void uninitialize();

  void scheduleCefLoopWork(int64_t delayMs);

public slots:
  void onAboutToQuit();

  void performCefLoopWork();

protected:
  bool initializeCef(const QCefConfig* config);

  void uninitializeCef();
};