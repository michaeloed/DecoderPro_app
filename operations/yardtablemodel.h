#ifndef YARDTABLEMODEL_H
#define YARDTABLEMODEL_H
#include "tracktablemodel.h"
#include "appslib_global.h"

namespace Operations
{
 class APPSLIBSHARED_EXPORT YardTableModel : public TrackTableModel
 {
  Q_OBJECT
 public:
  /*public*/ YardTableModel(QObject* parent = 0);
  /*public*/ void initTable(JTable* table, Location* location);
  /*public*/ QVariant headerData(int section, Qt::Orientation orientation, int role) const;
  /*public*/ int getRowCount() {return rowCount(QModelIndex());}
  /*public*/ int getColumnCount() {return columnCount(QModelIndex());}

 public slots:
  /*public*/ void propertyChange(PropertyChangeEvent* e);

 private:
  Logger* log;
 protected:
  /*protected*/ void editTrack(int row);

 };
}
#endif // YARDTABLEMODEL_H
