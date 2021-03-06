#ifndef LOCATIONMANAGERXML_H
#define LOCATIONMANAGERXML_H
#include "operationsxml.h"
#include "logger.h"
#include <QtXml>
#include "appslib_global.h"

namespace Operations
{
 class APPSLIBSHARED_EXPORT LocationManagerXml : public OperationsXml
 {
  Q_OBJECT
 public:
  explicit LocationManagerXml(QObject *parent = 0);
  QT_DEPRECATED/*public*/ static /*synchronized*/ LocationManagerXml* instance();
  /*public*/ void writeFile(QString name) throw (FileNotFoundException, IOException);
  /*public*/ void readFile(QString name) throw (JDOMException, IOException);
  /*public*/ void setOperationsFileName(QString name);
  /*public*/ QString getOperationsFileName() ;
  Q_INVOKABLE /*public*/ void initialize();
  /*public*/ void dispose();

 signals:

 public slots:

 private:
  // /*private*/ static LocationManagerXml* _instance;// = null;
  /*private*/ QString operationsFileName;// = "OperationsLocationRoster.xml"; // NOI18N
  Logger* log;

 };
}
#endif // LOCATIONMANAGERXML_H
