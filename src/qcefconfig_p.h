#pragma once

#pragma region stl_headers
#include <string>
#include <unordered_map>
#pragma endregion

#pragma region cef_headers
#include <include/cef_app.h>

#include <qcefconfig.h>

class QCefConfigPrivate
{
  typedef std::unordered_map<std::string, std::string> ArgsMap;

public:
  explicit QCefConfigPrivate();

  static void CopyToCefSettings(const QCefConfig* config, CefSettings* settings);

  static ArgsMap& GetCommandLineArgs(const QCefConfig* config);

public:
  std::string userAgent_;
  std::string cachePath_;

  int logLevel_ = QCefConfig::LOGLEVEL_DEFAULT;

#if defined(Q_OS_MACOS)
  bool standaloneMessgeLoopEnabled_ = false;
#else
  bool standaloneMessgeLoopEnabled_ = true;
#endif

#if defined(CEF_USE_SANDBOX)
  bool sandboxDisabled_ = false;
#else
  bool sandboxDisabled_ = true;
#endif

  short remoteDebuggingport_;

  ArgsMap commandLineArgs_;
};