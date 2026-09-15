#pragma once
#include <string>
#include <unordered_map>
#include <memory>

#include "../CefCoreGlobal.h"

class CefContextPrivate;

class CefAppDelegate
{
  typedef std::unordered_map<std::string, std::string> CommandLineArgs;

public:
  typedef std::shared_ptr<CefAppDelegate> RefPtr;

  typedef std::weak_ptr<CefAppDelegate> WeakPtr;

  CefAppDelegate(CefContextPrivate* context, CommandLineArgs args);

  ~CefAppDelegate();

  void onBeforeCommandLineProcessing(const CefString& process_type,
                                             CefRefPtr<CefCommandLine> command_line);

  void onBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line);

  void onScheduleMessageLoopWork(int64_t delay_ms);

private:
  CefContextPrivate* pContext_;

  CommandLineArgs commandLineArgs_;
};