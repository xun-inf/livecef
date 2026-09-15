#include "CefBrowserApp.h"
#include "../CefCoreLog.h"
#include "../CefCoreProtocol.h"

CefBrowserApp::CefBrowserApp(const CefString& schemeName, CefAppDelegate::RefPtr delegate)
  : builtinSchemeName_(schemeName.empty() ? kDefaultBuiltinSchemaName : schemeName)
  , appDelegate_(delegate)
{
}

CefBrowserApp::~CefBrowserApp()
{
  logD("CefViewBrowserApp::~CefBrowserApp");
}

// CefApp
void CefBrowserApp::OnBeforeCommandLineProcessing(const CefString& process_type, CefRefPtr<CefCommandLine> command_line)
{
  auto delegate = appDelegate_.lock();
  if (delegate)
    delegate->onBeforeCommandLineProcessing(process_type, command_line);
}

void CefBrowserApp::OnRegisterCustomSchemes(CefRawPtr<CefSchemeRegistrar> registrar)
{
  if (registrar) {
    // register custom scheme
    int options = 0                                 //
                  | CEF_SCHEME_OPTION_STANDARD      //
                  | CEF_SCHEME_OPTION_SECURE        //
                  | CEF_SCHEME_OPTION_CORS_ENABLED  //
                  | CEF_SCHEME_OPTION_FETCH_ENABLED //
                  | 0;
    if (!registrar->AddCustomScheme(builtinSchemeName_, options)) {
      logE("faield to add built-in scheme: %s", builtinSchemeName_.c_str());
    }
  }
}

CefRefPtr<CefResourceBundleHandler> CefBrowserApp::GetResourceBundleHandler()
{
  return nullptr;
}

CefRefPtr<CefBrowserProcessHandler> CefBrowserApp::GetBrowserProcessHandler()
{
  return this;
}

CefRefPtr<CefRenderProcessHandler> CefBrowserApp::GetRenderProcessHandler()
{
  return nullptr;
}

// browser process handler
void CefBrowserApp::OnContextInitialized()
{
  CEF_REQUIRE_UI_THREAD();

  // register custom scheme and handler
//   CefRegisterSchemeHandlerFactory(builtinSchemeName_, "", new CefSchemeHandlerFactory(this));
}

void
CefBrowserApp::OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line)
{
  auto delegate = appDelegate_.lock();
  if (delegate)
    delegate->onBeforeChildProcessLaunch(command_line);

//   if (bridge_object_name_.empty())
//     bridge_object_name_ = kCefViewDefaultBridgeObjectName;
//   command_line->AppendSwitchWithValue(kCefViewBridgeObjectNameKey, bridge_object_name_);

//   if (builtin_scheme_name_.empty())
//     builtin_scheme_name_ = kCefViewBuiltinSchemeNameKey;
//   command_line->AppendSwitchWithValue(kCefViewBuiltinSchemeNameKey, bridge_object_name_);
}

void CefBrowserApp::OnScheduleMessagePumpWork(int64_t delay_ms)
{
  auto delegate = appDelegate_.lock();

  if (delegate)
    delegate->onScheduleMessageLoopWork(delay_ms);
}

CefRefPtr<CefClient> CefBrowserApp::GetDefaultClient()
{
  return nullptr;
}