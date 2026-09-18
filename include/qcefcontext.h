#ifndef QCEFCONTEXT_H
#define QCEFCONTEXT_H
#pragma once

#include <qcefium_global.h>

#pragma region qt_headers
#include <QCoreApplication>
#include <QScopedPointer>
#pragma endregion

#include <qcefconfig.h>

class QCefContextPrivate;

class QCEFIUM_EXPORT QCefContext : public QObject
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(QCefContext)
  QScopedPointer<QCefContextPrivate> d_ptr;

  friend class QCefWidget;

public:
  QCefContext(QCoreApplication* app, int argc, char* argv[], const QCefConfig* config);

  /// Destructs the CEF context.
  ~QCefContext();

  /// Gets the unique default instance.
  static QCefContext* instance();

  const QCefConfig* cefConfig() const;

protected:
  bool init(const QCefConfig* config);

  void uninit();

private:
  static QCefContext* s_self;
};

#endif // CEFCONTEXT_H