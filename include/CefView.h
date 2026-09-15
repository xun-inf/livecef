#ifndef CEFVIEW_H
#define CEFVIEW_H
#pragma once

#include <CefView_global.h>
#include <QString>
#include <QWidget>

/// A QWidget embedding a CEF browser (native child window rendering).
class CefViewPrivate;

class LIVECEF_EXPORT CefView : public QWidget
{
  Q_OBJECT
public:
  explicit CefView(const QString& url = QString(), QWidget* parent = nullptr);
  ~CefView() override;

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

private:
  CefViewPrivate* d;
};

#endif // CEFVIEW_H