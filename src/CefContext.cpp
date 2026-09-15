#include <CefContext.h>

#pragma region qt_headers
#include <QDebug>
#pragma endregion

#include "cefcore/CefContextPrivate.h"

CefContext* CefContext::s_self = nullptr;

CefContext::CefContext(QCoreApplication* app, int argc, char* argv[], const CefConfig* config)
  : QObject(app)
  , d_ptr(new CefContextPrivate(app, argc, argv))
{
  init(config);
}

CefContext* CefContext::instance()
{
  return s_self;
}

CefContext::~CefContext()
{
  uninit();
}

bool CefContext::init(const CefConfig* config)
{
  Q_ASSERT_X(!s_self, "CefContext::init()", "There can be only one CefContext instance");
  s_self = this;

  Q_D(CefContext);
  d->initialize(config);

  return true;
}

void CefContext::uninit()
{
  Q_D(CefContext);
  d->uninitialize();

  s_self = nullptr;
}