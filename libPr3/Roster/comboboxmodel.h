#ifndef COMBOBOXMODEL_H
#define COMBOBOXMODEL_H
#include "listmodel.h"
#include <QObject>

//template <class E>
class ComboBoxModel : public ListModel
{
    /**
       * Set the selected item. The implementation of this  method should notify
       * all registered <code>ListDataListener</code>s that the contents
       * have changed.
       *
       * @param anItem the list object to select or <code>null</code>
       *        to clear the selection
       */
      virtual void setSelectedItem(QObject* anItem);

      /**
       * Returns the selected item
       * @return The selected item or <code>null</code> if there is no selection
       */
      virtual QObject* getSelectedItem();
};
#endif // COMBOBOXMODEL_H
