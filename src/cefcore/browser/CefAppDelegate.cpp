#include "CefAppDelegate.h"

#include <QDebug>

#include "../CefContextPrivate.h"

CefAppDelegate::CefAppDelegate(CefContextPrivate* context, CommandLineArgs args)
  : pContext_(context)
  , commandLineArgs_(args)
{
}

CefAppDelegate::~CefAppDelegate()
{
  qDebug() << "CCefAppDelegate is being destructed";
}

void CefAppDelegate::onBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
{
  for (auto& kv : commandLineArgs_) {
    if (!kv.first.empty()) {
      if (!kv.second.empty())
        command_line->AppendSwitchWithValue(kv.first, kv.second);
      else
        command_line->AppendSwitch(kv.first);
    }
  }
}

void CefAppDelegate::onBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line)
{
  for (auto& kv : commandLineArgs_) {
    if (!kv.first.empty()) {
      if (!kv.second.empty())
        command_line->AppendSwitchWithValue(kv.first, kv.second);
      else
        command_line->AppendSwitch(kv.first);
    }
  }
}

void CefAppDelegate::onScheduleMessageLoopWork(int64_t delay_ms)
{
  if (pContext_) {
    pContext_->scheduleCefLoopWork(delay_ms);
  }
}
