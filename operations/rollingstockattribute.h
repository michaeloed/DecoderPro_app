#ifndef ROLLINGSTOCKATTRIBUTE_H
#define ROLLINGSTOCKATTRIBUTE_H

#include <QObject>
#include <QStringList>
#include <QtXml>
#include "appslib_global.h"
#include "propertychangesupport.h"

class QDomElement;
class JComboBox;
class Logger;
namespace Operations
{
 class APPSLIBSHARED_EXPORT RollingStockAttribute : public PropertyChangeSupport
 {
  Q_OBJECT
 public:
  explicit RollingStockAttribute(QObject *parent = 0);
  PropertyChangeSupport* pcs;// = new java.beans.PropertyChangeSupport(this);
  /*public*/ /*synchronized*/ void dispose() ;
     /*public*/ QStringList getNames();
  /*public*/ void setNames(QStringList names);
  /*public*/ void setValues(QStringList lengths);
  /*public*/ void addName(QString name);
  /*public*/ void deleteName(QString name);
  /*public*/ bool containsName(QString name);
  /*public*/ JComboBox* getComboBox();
  /*public*/ void updateComboBox(JComboBox* box);
  /*public*/ int getMaxNameLength();
  /*public*/ void store(QDomElement root, QString eNames, QString eName, QString oldName, QDomDocument doc);
  /*public*/ void load(QDomElement root, QString eNames, QString eName, QString oldName);

 signals:

 public slots:
 private:
  Logger* log;
 protected:
  /*protected*/ static /*final*/ int MIN_NAME_LENGTH;// = 4;
   /*protected*/ QStringList list;// = new ArrayList<String>();
  /*protected*/ void firePropertyChange(QString p, QVariant old, QVariant n);
  /*protected*/ virtual QString getDefaultNames();
  /*protected*/ int maxNameLength;// = 0;

 };
}
#endif // ROLLINGSTOCKATTRIBUTE_H
