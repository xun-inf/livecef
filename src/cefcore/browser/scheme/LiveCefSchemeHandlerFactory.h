// #pragma once

// #include "../../CefCoreGlobal.h"
// #include "../CefBrowserApp.h"

// class LiveCefSchemeHandlerFactory : public CefSchemeHandlerFactory
// {
//   IMPLEMENT_REFCOUNTING(LiveCefSchemeHandlerFactory);

// private:
//   CefRefPtr<CefBrowserApp> app_;

// public:
//   LiveCefSchemeHandlerFactory(CefRefPtr<CefBrowserApp> app);

//   /// <summary>
//   ///
//   /// </summary>
//   ~LiveCefSchemeHandlerFactory();

//   /// <summary>
//   ///
//   /// </summary>
//   /// <param name="browser"></param>
//   /// <param name="frame"></param>
//   /// <param name="scheme_name"></param>
//   /// <param name="request"></param>
//   /// <returns></returns>
//   virtual CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser> browser,
//                                                CefRefPtr<CefFrame> frame,
//                                                const CefString& scheme_name,
//                                                CefRefPtr<CefRequest> request) override;
// };
