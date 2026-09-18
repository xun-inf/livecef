#include <QApplication>
#include <QMainWindow>

#include <CefContext.h>
#include <CefView.h>

int
main(int argc, char* argv[])
{
  QApplication app(argc, argv);

  CefSetting setting;
  setting.remoteDebuggingPort = 9222;
  CefContext context(argc, argv, setting);
  if (!context.initialized())
    return -1;

  QMainWindow window;
  CefView* view = new CefView(QStringLiteral("https://www.bing.com"), &window);
  window.setCentralWidget(view);
  window.resize(1280, 800);
  window.show();

  QObject::connect(view, &CefView::titleChanged, &window, &QMainWindow::setWindowTitle);

  return app.exec();
}