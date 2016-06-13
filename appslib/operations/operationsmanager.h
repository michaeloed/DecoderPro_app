#ifndef OPERATIONSMANAGER_H
#define OPERATIONSMANAGER_H

#include "quietshutdowntask.h"
#include <QObject>
#include "appslib_global.h"

class File;
class Logger;
namespace Operations
{
 class APPSLIBSHARED_EXPORT OperationsManager : public QObject
 {
  Q_OBJECT
 public:
  /*public*/ ShutDownTask* getDefaultShutDownTask();
  /*public*/ static OperationsManager* getInstance();
  /*public*/ QString getPath();
  /*public*/ QString getPath(QString name) ;
  /*public*/ File* getFile(QString name);
  /*public*/ void setShutDownTask(ShutDownTask* shutDownTask);
  /*public*/ QString getOperationsFolderName();

 signals:

 public slots:
 private:
  explicit OperationsManager(QObject *parent = 0);
  /*private*/ ShutDownTask* shutDownTask;// = null;
  static /*private*/ OperationsManager* instance;// = null;
  /*private*/ OperationsManager(QString operationsFolderName, QObject *parent = 0);
  void common();
  Logger* log;

  class MyQuietShutdownTask : public QuietShutDownTask
  {
  public:
   MyQuietShutdownTask(QString name);
   /*public*/ bool execute();
  };

 protected:
  /*protected*/ /*final*/ QString operationsFolderName;
  /*protected*/ static void setInstance(QString operationsFolderName);

 };
}
#endif // OPERATIONSMANAGER_H
