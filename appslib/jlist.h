#ifndef JLIST_H
#define JLIST_H

#include <QListView>
#include "javaqt_global.h"
#include "abstracttablemodel.h"

class JAVAQTSHARED_EXPORT JList : public QListView
{
public:
 JList( QWidget* parent = 0);
 JList(QAbstractListModel* model, QWidget* parent = 0);
 JList(QList<QString>, QWidget* parent = 0);
 QModelIndexList getSelectedValues();
 QModelIndex locationToIndex(QPoint);
 /*public*/ int getSelectedIndex();
 /*public*/ void setSelectedValue(QVariant anObject, bool shouldScroll);
 /*public*/ QVariant getSelectedValue();
 /*public*/ void setSelectedIndex(int index);
 /*public*/ void ensureIndexIsVisible(int index);

private:
 void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
};

class JAVAQTSHARED_EXPORT JListTableModel : public AbstractTableModel
{
 Q_OBJECT
 QList<QString> list;
public:
 JListTableModel(QList<QString>);
 int rowCount(const QModelIndex &parent) const;
 int columnCount(const QModelIndex &parent) const;
 QVariant headerData(int section, Qt::Orientation orientation, int role) const;
 Qt::ItemFlags flags(const QModelIndex &index) const;
 QVariant data(const QModelIndex &index, int role) const;
 /*public*/ int getRowCount() {return rowCount(QModelIndex());}
 /*public*/ int getColumnCount() {return columnCount(QModelIndex());}

};

#endif // JLIST_H
