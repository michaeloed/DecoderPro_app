#include "rostergroupcombobox.h"

//RosterGroupComboBox::RosterGroupComboBox(QObject *parent) :
//    QComboBox(parent)
//{
// allEntriesEnabled = true;
//}
/**
 * A JComboBox of Roster Groups.
 *
 * @author  Randall Wood Copyright (C) 2011
 * @version	$Revision: $
 * @see         jmri.jmrit.roster.Roster
 */
///*public*/ class RosterGroupComboBox extends JComboBox implements RosterGroupSelector {


    //private static ResourceBundle resources = ResourceBundle.getBundle("jmri.jmrit.roster.JmritRosterBundle");

/**
 * Create a RosterGroupComboBox with an arbitrary Roster instead of the
 * default Roster instance.
 *
 * @param roster
 */
// needed for unit tests
/*public*/ RosterGroupComboBox::RosterGroupComboBox(Roster* roster, QWidget *parent)
{
 //RosterGroupComboBox(roster, roster->getDefaultRosterGroup(), parent);
 _roster = roster;
 this->parent = parent;
 update("");

}

/**
 * Create a RosterGroupComboBox with an arbitrary selection.
 *
 * @param selection
 */
/*public*/ RosterGroupComboBox::RosterGroupComboBox(QString selection, QWidget *parent)
{
    RosterGroupComboBox(Roster::instance(), selection, parent);
}

/**
 * Create a RosterGroupComboBox with arbitrary selection and Roster.
 *
 * @param roster
 * @param selection
 */
/*public*/ RosterGroupComboBox::RosterGroupComboBox(Roster* roster, QString selection, QWidget *parent) : QComboBox(parent)
{
 //super();
 _roster = roster;
 update(selection);
//    roster->addPropertyChangeListener(new PropertyChangeListener()
//    {

//        @Override
//        /*public*/ void propertyChange(PropertyChangeEvent pce) {
//            if (pce.getPropertyName()==("RosterGroupAdded")) {
//                update();
//            } else if (pce.getPropertyName()==("RosterGroupRemoved")
//                || pce.getPropertyName()==("RosterGroupRenamed")) {
//                if (getSelectedItem()==(pce.getOldValue())) {
//                    update((String)pce.getNewValue());
//                } else {
//                    update();
//                }
//            }
//        }
//    });
 connect(_roster, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));
}

/**
 * Create a RosterGroupComboBox with the default Roster instance and the
 * default roster group.
 */
/*public*/ RosterGroupComboBox::RosterGroupComboBox(QWidget *parent)
{
 //RosterGroupComboBox(Roster::instance(), Roster::instance()->getDefaultRosterGroup(), parent);
    _roster = Roster::instance();
    this->parent = parent;
    update(Roster::instance()->getDefaultRosterGroup());

}

/**
 * Update the combo box and reselect the current selection.
 */
/*public*/ /*final*/ void RosterGroupComboBox::update()
{
    update((QString) this->currentText());
}

/**
 * Update the combo box and select given String.
 *
 * @param selection
 */
/*public*/ /*final*/ void RosterGroupComboBox::update(QString selection)
{
  clear();
  QVector<QString> l = _roster->getRosterGroupList();
  //Collections.sort(l);
  foreach (QString g, l)
  {
   addItem(g);
  }
  if (allEntriesEnabled)
  {
   insertItem(0, Roster::ALLENTRIES, 0);
   if (selection == NULL)
   {
    selection = Roster::ALLENTRIES;
   }
   this->setToolTip("");
  }
  else
  {
   if (this->count() == 0)
   {
    this->addItem(tr("No Groups"));
    this->setToolTip(tr("No roster groups have been defined."));
   }
   else
   {
    this->setToolTip(NULL);
   }
  }
  //setSelectedItem(selection);
  this->setCurrentIndex(this->findText(selection));
  if (this->count() == 1)
  {
   this->setCurrentIndex(0);
   this->setEnabled(false);
   this->setVisible(false);
  }
  else
  {
   this->setEnabled(true);
   this->setVisible(true);
  }
}

//@Override
/*public*/ QString RosterGroupComboBox::getSelectedRosterGroup()
{
 if (currentText() == "")
 {
  return NULL;
 }
 else if (currentText()==(Roster::ALLENTRIES))
 {
  return "";
 }
 else
 {
  return currentText();
 }
}

/**
 * @return the allEntriesEnabled
 */
/*public*/ bool RosterGroupComboBox::isAllEntriesEnabled()
{
    return allEntriesEnabled;
}

/**
 * @param allEntriesEnabled the allEntriesEnabled to set
 */
/*public*/ void RosterGroupComboBox::setAllEntriesEnabled(bool allEntriesEnabled)
{
 this->allEntriesEnabled = allEntriesEnabled;
 this->update();
}
/*public*/ void RosterGroupComboBox::propertyChange(PropertyChangeEvent* pce)
{
 if (pce->getPropertyName()==("RosterGroupAdded"))
 {
  update();
 }
 else if (pce->getPropertyName()==("RosterGroupRemoved")
     || pce->getPropertyName()==("RosterGroupRenamed"))
 {
  if (currentText()==(pce->getOldValue()))
  {
   update(pce->getNewValue().toString());
  }
  else
  {
   update();
  }
 }
}
