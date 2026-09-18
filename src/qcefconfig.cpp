#include <qcefconfig.h>
#include "qcefconfig_p.h"

// QCefConfigPrivate
QCefConfigPrivate::QCefConfigPrivate() {}

void QCefConfigPrivate::CopyToCefSettings(const QCefConfig* config, CefSettings* settings)
{
  if (!settings) {
    return;
  }

  if (!config) {
    QCefConfigPrivate cfg;

    // copy the mandatory fields
    settings->no_sandbox = static_cast<int>(cfg.sandboxDisabled_);
    // settings->windowless_rendering_enabled = cfg.windowlessRenderingEnabled_.toInt();
    // settings->command_line_args_disabled = cfg.commandLinePassthroughDisabled_.toInt();
    settings->multi_threaded_message_loop = static_cast<int>(cfg.standaloneMessgeLoopEnabled_);

#if defined(Q_OS_WINDOWS)
    // CefString(&settings->browser_subprocess_path) = cfg.browserSubProcessPath_;
    // CefString(&settings->resources_dir_path) = cfg.resourceDirectoryPath_;
    // CefString(&settings->locales_dir_path) = cfg.localesDirectoryPath_;
#endif
  } else {
    settings->no_sandbox = static_cast<int>(config->d_ptr->sandboxDisabled_);
    // settings->windowless_rendering_enabled = config->d_ptr->windowlessRenderingEnabled_.toInt();
    // settings->command_line_args_disabled = config->d_ptr->commandLinePassthroughDisabled_.toInt();
    settings->multi_threaded_message_loop = static_cast<int>(config->d_ptr->standaloneMessgeLoopEnabled_);

#if defined(Q_OS_WINDOWS)
    // if (!config->d_ptr->browserSubProcessPath_.empty())
    //   CefString(&settings->browser_subprocess_path) = config->d_ptr->browserSubProcessPath_;

    // if (!config->d_ptr->resourceDirectoryPath_.empty())
    //   CefString(&settings->resources_dir_path) = config->d_ptr->resourceDirectoryPath_;

    // if (!config->d_ptr->localesDirectoryPath_.empty())
    //   CefString(&settings->locales_dir_path) = config->d_ptr->localesDirectoryPath_;
#endif

    if (!config->d_ptr->userAgent_.empty())
      CefString(&settings->user_agent) = config->d_ptr->userAgent_;

    if (!config->d_ptr->cachePath_.empty())
      CefString(&settings->cache_path) = config->d_ptr->cachePath_;

#if CEF_VERSION_MAJOR < 115
    if (!config->d_ptr->userDataPath_.empty())
      CefString(&settings->user_data_path) = config->d_ptr->userDataPath_;
#else
    // if (!config->d_ptr->rootCachePath_.empty())
    //   CefString(&settings->root_cache_path) = config->d_ptr->rootCachePath_;
#endif

    // if (!config->d_ptr->locale_.empty())
    //   CefString(&settings->locale) = config->d_ptr->locale_;

    // if (!config->d_ptr->acceptLanguageList_.empty())
    //   CefString(&settings->accept_language_list) = config->d_ptr->acceptLanguageList_;

    // if (config->d_ptr->persistSessionCookies_.canConvert<int>())
    //   settings->persist_session_cookies = config->d_ptr->persistSessionCookies_.toInt();

#if CEF_VERSION_MAJOR < 128
    if (config->d_ptr->persistUserPreferences_.canConvert<int>())
      settings->persist_user_preferences = config->d_ptr->persistUserPreferences_.toInt();
#endif

    // if (config->d_ptr->backgroundColor_.canConvert<QColor>())
    //   settings->background_color = config->d_ptr->backgroundColor_.value<QColor>().rgba();

    if (config->d_ptr->remoteDebuggingport_)
      settings->remote_debugging_port = static_cast<int>(config->d_ptr->remoteDebuggingport_);

    settings->log_severity = (cef_log_severity_t)config->d_ptr->logLevel_;
  }
}

QCefConfigPrivate::ArgsMap& QCefConfigPrivate::GetCommandLineArgs(const CefConfig* config)
{
  if (!config) {
    static ArgsMap emptyArgs;
    return emptyArgs;
  }

  return config->d_ptr->commandLineArgs_;
}


// QCefConfig
REGISTER_METATYPE(QCefConfig);

QCefConfig::QCefConfig()
  : d_ptr(new QCefConfigPrivate)
{
}

QCefConfig::QCefConfig(const QCefConfig& other)
  : d_ptr(new QCefConfigPrivate)
{
  *d_ptr = *(other.d_ptr);
}

QCefConfig& QCefConfig::operator=(const QCefConfig& other)
{
  *d_ptr = *(other.d_ptr);
  return *this;
}

QCefConfig::~QCefConfig() {}

void QCefConfig::addCommandLineSwitch(const QString& smitch)
{
  if (smitch.isEmpty())
    return;

  Q_D(QCefConfig);
  d->commandLineArgs_[smitch.toStdString()] = std::string();
}

void QCefConfig::addCommandLineSwitchWithValue(const QString& smitch, const QString& v)
{
  if (smitch.isEmpty() || v.isEmpty())
    return;

  Q_D(QCefConfig);
  d->commandLineArgs_[smitch.toStdString()] = v.toStdString();
}

void QCefConfig::setStandaloneMessageLoopEnabled(const bool enabled)
{
  Q_D(QCefConfig);

  #if defined(Q_OS_MACOS)
    qWarning() << "StandaloneMessgeLoop is not supported on macOS";
  #else
    d->standaloneMessgeLoopEnabled_ = enabled;
  #endif
}

const bool QCefConfig::standaloneMessageLoopEnabled() const
{
  Q_D(const QCefConfig);
  return d->standaloneMessgeLoopEnabled_;
}

void QCefConfig::setSandboxDisabled(const bool disabled)
{
  Q_D(QCefConfig);

#if defined(CEF_USE_SANDBOX)
  d->sandboxDisabled_ = disabled;
#else
  qWarning() << "Sandbox status is not configurable when compile switch CEF_USE_SANDBOX is OFF";
#endif
}

const bool QCefConfig::sandboxDisabled() const
{
  Q_D(const QCefConfig);
  return d->sandboxDisabled_;
}

void QCefConfig::setLogLevel(const LogLevel level)
{
  Q_D(QCefConfig);
  d->logLevel_ = level;
}

const QCefConfig::LogLevel QCefConfig::logLevel() const
{
  Q_D(const QCefConfig);
  return static_cast<LogLevel>(d->logLevel_);
}

void QCefConfig::setUserAgent(const QString& agent)
{
  Q_D(QCefConfig);
  d->userAgent_ = agent.toStdString();
}

const QString QCefConfig::userAgent() const
{
  Q_D(const QCefConfig);
  return QString::fromStdString(d->userAgent_);
}

void QCefConfig::setCachePath(const QString& path)
{
  Q_D(QCefConfig);
  d->cachePath_ = path.toStdString();
}

const QString QCefConfig::cachePath() const
{
  Q_D(const QCefConfig);
  return QString::fromStdString(d->cachePath_);
}

void QCefConfig::setRemoteDebuggingPort(short port)
{
  Q_D(QCefConfig);
  d->remoteDebuggingport_ = port;
}

const short QCefConfig::remoteDebuggingPort() const
{
  Q_D(const QCefConfig);
  return d->remoteDebuggingport_;
}