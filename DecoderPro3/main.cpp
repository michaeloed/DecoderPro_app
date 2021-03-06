//#include "mainwindow.h"
#include <QApplication>
#include "myapplication.h"
#ifdef SCRIPTING_ENABLED
#include "PythonQt.h"
#include "PythonQt_QtAll.h"
#endif
#include "decoderpro3.h"

int main(int argc, char *argv[])
{
 QCoreApplication::setApplicationName("DecoderPro3");
 QCoreApplication::setOrganizationName("ACK Software");

 MyApplication a(argc, argv);
#ifdef SCRIPTING_ENABLED
 PythonQt::init(PythonQt::IgnoreSiteModule | PythonQt::RedirectStdOut);
 PythonQt_QtAll::init();
#endif
//    MainWindow w;
//    w.show();
 QStringList args;
 if(argc > 1)
  for(int i=1; i < argc; i++)
  {
   args.append(QString(argv[i]));
  }
 DecoderPro3::main(args);

 return a.exec();
}
