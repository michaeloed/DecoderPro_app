#ifndef DEFAULTCOMBOBOXMODEL_H
#define DEFAULTCOMBOBOXMODEL_H

#include <QObject>
#include <QVector>
#include "abstractlistmodel.h"
#include "javaqt_global.h"
#include "comboboxmodel.h"

template <class E>
class JAVAQTSHARED_EXPORT DefaultComboBoxModel : public AbstractListModel, public ComboBoxModel
{
    //Q_OBJECT
  Q_INTERFACES(ComboBoxModel)
public:
    //explicit DefaultComboBoxModel(QObject *parent = 0);
    /**
     * Constructs an empty DefaultComboBoxModel object.
     */
//    /*public*/ DefaultComboBoxModel<E>(QObject *parent = 0);
    /**
     * Constructs a DefaultComboBoxModel object initialized with
     * an array of objects.
     *
     * @param items  an array of Object objects
     */
    /*public*/ DefaultComboBoxModel<E>(/*final*/ QList<E> items,QObject *parent = 0);
    /*public*/ DefaultComboBoxModel<E>(QVector<E> v, QObject *parent = 0);
    /*public*/ void setSelectedItem(QObject* anObject);
    /*public*/ QObject* getSelectedItem();
    /*public*/ int getSize();
//    /*public*/ QObject* getElementAt(int index);
    /*public*/ int getIndexOf(QObject* anObject) ;
    /*public*/ void addElement(E anObject);
    /*public*/ void insertElementAt(E anObject,int index);
    /*public*/ void removeElementAt(int index);
    /*public*/ void removeElement(QObject* anObject);
    /**
     * Empties the list.
     */
    /*public*/ void removeAllElements();
  /*public*/ int rowCount(const QModelIndex &parent) const {return 0;}
  /*public*/ QVariant data(const QModelIndex &index, int role) const {return QVariant();}
  QObject* self() {return (QObject*)this;}

signals:

public slots:
private:
    QVector<E> objects;
    QObject* selectedObject;


};

#endif // DEFAULTCOMBOBOXMODEL_H
