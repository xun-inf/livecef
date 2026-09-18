#ifndef QCEFWIDGET_H
#define QCEFWIDGET_H
#pragma once

#include <qcefium_global.h>
#include <QString>
#include <QWidget>

/// A QWidget embedding a CEF browser (native child window rendering).
class QCefWidgetPrivate;

class QCEFIUM_EXPORT QCefWidget : public QWidget
{
  Q_OBJECT
  Q_DECLARE_PRIVATE(QCefWidget)
  QScopedPointer<QCefViewPrivate> d_ptr;

public:
  explicit QCefWidget(const QString& url = QString(), QWidget* parent = nullptr);
  ~QCefWidget() override;

  void loadUrl(const QString& url);
  QString url() const;

  void goBack();
  void goForward();
  bool canGoBack() const;
  bool canGoForward() const;

  void reload();
  void stopLoad();
  bool isLoading() const;

Q_SIGNALS:
  void titleChanged(const QString& title);
  void urlChanged(const QString& url);
  void loadingStateChanged(bool isLoading, bool canGoBack, bool canGoForward);
  void loadFinished(int httpStatusCode);

protected:
  void resizeEvent(QResizeEvent* event) override;
  
};

#endif // QCEFWIDGET_H