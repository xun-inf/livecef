#ifndef CEFCONTEXT_H
#define CEFCONTEXT_H
#pragma once

#include <CefView_global.h>

#pragma region qt_headers
#include <QCoreApplication>
#include <QScopedPointer>
#pragma endregion

#include <CefConfig.h>

class CefContextPrivate;

class LIVECEF_EXPORT CefContext : public QObject
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(CefContext)
  QScopedPointer<CefContextPrivate> d_ptr;

  friend class CefView;

public:
  CefContext(QCoreApplication* app, int argc, char* argv[], const CefConfig* config);

  /// Destructs the CEF context.
  ~CefContext();

  /// Gets the unique default instance.
  static CefContext* instance();

  const CefConfig* cefConfig() const;

protected:
  bool init(const CefConfig* config);

  void uninit();

private:
  static CefContext* s_self;
};

#endif // CEFCONTEXT_H