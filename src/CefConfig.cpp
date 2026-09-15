#include <CefConfig.h>

#include "cefcore/CefConfigPrivate.h"

REGISTER_METATYPE(CefConfig);

CefConfig::CefConfig()
  : d_ptr(new CefConfigPrivate)
{
}

CefConfig::CefConfig(const CefConfig& other)
  : d_ptr(new CefConfigPrivate)
{
  *d_ptr = *(other.d_ptr);
}

CefConfig&
CefConfig::operator=(const CefConfig& other)
{
  *d_ptr = *(other.d_ptr);
  return *this;
}

CefConfig::~CefConfig() {}

void
CefConfig::addCommandLineSwitch(const QString& smitch)
{
  if (smitch.isEmpty())
    return;

  Q_D(CefConfig);
  d->commandLineArgs_[smitch.toStdString()] = std::string();
}

void
CefConfig::addCommandLineSwitchWithValue(const QString& smitch, const QString& v)
{
  if (smitch.isEmpty() || v.isEmpty())
    return;

  Q_D(CefConfig);
  d->commandLineArgs_[smitch.toStdString()] = v.toStdString();
}

void CefConfig::setStandaloneMessageLoopEnabled(const bool enabled)
{
  Q_D(CefConfig);

  #if defined(Q_OS_MACOS)
    qWarning() << "StandaloneMessgeLoop is not supported on macOS";
  #else
    d->standaloneMessgeLoopEnabled_ = enabled;
  #endif
}

const bool CefConfig::standaloneMessageLoopEnabled() const
{
  Q_D(const CefConfig);
  return d->standaloneMessgeLoopEnabled_;
}

void CefConfig::setSandboxDisabled(const bool disabled)
{
  Q_D(CefConfig);

#if defined(CEF_USE_SANDBOX)
  d->sandboxDisabled_ = disabled;
#else
  qWarning() << "Sandbox status is not configurable when compile switch CEF_USE_SANDBOX is OFF";
#endif
}

const bool CefConfig::sandboxDisabled() const
{
  Q_D(const CefConfig);
  return d->sandboxDisabled_;
}

void CefConfig::setLogLevel(const LogLevel level)
{
  Q_D(CefConfig);
  d->logLevel_ = level;
}

const CefConfig::LogLevel CefConfig::logLevel() const
{
  Q_D(const CefConfig);
  return static_cast<LogLevel>(d->logLevel_);
}

void CefConfig::setUserAgent(const QString& agent)
{
  Q_D(CefConfig);
  d->userAgent_ = agent.toStdString();
}

const QString CefConfig::userAgent() const
{
  Q_D(const CefConfig);
  return QString::fromStdString(d->userAgent_);
}

void CefConfig::setCachePath(const QString& path)
{
  Q_D(CefConfig);
  d->cachePath_ = path.toStdString();
}

const QString CefConfig::cachePath() const
{
  Q_D(const CefConfig);
  return QString::fromStdString(d->cachePath_);
}

void CefConfig::setRemoteDebuggingPort(short port)
{
  Q_D(CefConfig);
  d->remoteDebuggingport_ = port;
}

const short CefConfig::remoteDebuggingPort() const
{
  Q_D(const CefConfig);
  return d->remoteDebuggingport_;
}