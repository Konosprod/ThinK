#include "mainwindow.h"

int main(int argc, char* argv[])
{
   QApplication app(argc, argv);
   //Q_INIT_RESOURCE(rc);
   app.setApplicationName("ThinK");
   mainWindow win;
   win.setWindowTitle("ThinK");
   win.setWindowIcon(QIcon(":/ressources/icon"));
   win.show();
   return app.exec();
}
