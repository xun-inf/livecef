// #include "LiveCefSchemeHandlerFactory.h"

// #include <CefViewBrowserClient.h>
// #include <CefViewBrowserClientDelegate.h>

// #include "LiveCefSchemeHandler.h"

// LiveCefSchemeHandlerFactory::LiveCefSchemeHandlerFactory(CefRefPtr<CefBrowserApp> app)
//   : app_(app)
// {
// }

// LiveCefSchemeHandlerFactory::~LiveCefSchemeHandlerFactory() {}

// CefRefPtr<CefResourceHandler> LiveCefSchemeHandlerFactory::Create(CefRefPtr<CefBrowser> browser,
//                                     CefRefPtr<CefFrame> frame,
//                                     const CefString& scheme_name,
//                                     CefRefPtr<CefRequest> request)
// {
//   if (!app_) {
//     return nullptr;
//   }

//   auto client = browser->GetHost()->GetClient();
//   if (!client) {
//     return nullptr;
//   }

//   auto handler = app_->GetClientHandler(client.get());
//   if (!handler) {
//     return nullptr;
//   }
//   return new CefViewSchemeHandler(browser, frame, handler);
// }
