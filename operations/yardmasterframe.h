#ifndef YARDMASTERFRAME_H
#define YARDMASTERFRAME_H

#include <QObject>
#include <operationsframe.h>
#include "appslib_global.h"

namespace Operations
{
 class Location;
 class APPSLIBSHARED_EXPORT YardmasterFrame : public OperationsFrame
 {
  Q_OBJECT
 public:
  YardmasterFrame(Location *location, QWidget* parent = 0);
  /*public*/ QString getClassName();

 private:
  /*private*/ void initComponents(Location* location);

 };
}
#endif // YARDMASTERFRAME_H
