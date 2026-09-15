#pragma once

#include "../CefCoreGlobal.h"
#include "CefAppDelegate.h"

class CefBrowserApp
  : public CefApp
  , public CefBrowserProcessHandler
{
  IMPLEMENT_REFCOUNTING(CefBrowserApp);

private:
  CefString builtinSchemeName_;

  CefAppDelegate::WeakPtr appDelegate_;

public:
  CefBrowserApp(const CefString& schemeName, CefAppDelegate::RefPtr delegate);
  ~CefBrowserApp();

private:
#pragma region CefApp

  void OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line) override;

  void OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar) override;

  CefRefPtr<CefResourceBundleHandler> GetResourceBundleHandler() override;

  CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override;

  CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;

#pragma endregion

#pragma region CefBrowserProcessHandler

  void OnContextInitialized() override;

  void OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line) override;

  void OnScheduleMessagePumpWork(int64_t delay_ms) override;

  CefRefPtr<CefClient> GetDefaultClient() override;

#pragma endregion
};