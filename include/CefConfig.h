#ifndef CEFCONFIG_H
#define CEFCONFIG_H
#pragma once

#include <CefView_global.h>

#pragma region qt_headers
#include <QString>
#include <QScopedPointer>

class CefConfigPrivate;

class LIVECEF_EXPORT CefConfig
{
  Q_DECLARE_PRIVATE(CefConfig)
  QScopedPointer<CefConfigPrivate> d_ptr;

public:
  enum LogLevel
  {
    LOGLEVEL_DEFAULT,
    LOGLEVEL_VERBOSE,
    LOGLEVEL_DEBUG,
    LOGLEVEL_INFO,
    LOGLEVEL_WARNING,
    LOGLEVEL_ERROR,
    LOGLEVEL_FATAL,
    LOGLEVEL_DISABLE
  };

public:
  CefConfig();

  CefConfig(const CefConfig& other);

  CefConfig& operator=(const CefConfig& other);

  ~CefConfig();

  void addCommandLineSwitch(const QString& smitch);

  void addCommandLineSwitchWithValue(const QString& smitch, const QString& value);

  void setStandaloneMessageLoopEnabled(const bool enabled);
  const bool standaloneMessageLoopEnabled() const;

  void setSandboxDisabled(const bool disabled);
  const bool sandboxDisabled() const;

  void setLogLevel(const LogLevel level);
  const CefConfig::LogLevel logLevel() const;

  void setUserAgent(const QString& agent);
  const QString userAgent() const;

  void setCachePath(const QString& path);
  const QString cachePath() const;

  void setRemoteDebuggingPort(short port);
  const short remoteDebuggingPort() const;
};

Q_DECLARE_METATYPE(CefConfig);

#endif