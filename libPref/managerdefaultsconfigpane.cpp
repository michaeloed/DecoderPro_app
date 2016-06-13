#include "managerdefaultsconfigpane.h"
#include <QBoxLayout>
#include <QButtonGroup>
#include <QBoxLayout>
#include "managerdefaultselector.h"
#include "systemconnectionmemo.h"
#include <QLabel>
#include <QGridLayout>
#include <QRadioButton>
#include "instancemanager.h"

//ManagerDefaultsPane::ManagerDefaultsPane(QWidget *parent) :
//    PreferencesPanel(parent)
//{
//}

/**
 * Provide GUI to configure InstanceManager defaults.
 * <P>
 *
 * @author Bob Jacobsen Copyright (C) 2010
 * @version	$Revision: 28430 $
 * @since 2.9.5
 */
// /*public*/ class ManagerDefaultsConfigPane extends JmriPanel implements PreferencesPanel {

/**
 *
 */
//private static final long serialVersionUID = 4382220076212974325L;
//private static final ResourceBundle rb = ResourceBundle.getBundle("apps.AppsConfigBundle");

/*public*/ ManagerDefaultsConfigPane::ManagerDefaultsConfigPane(QWidget *parent)
    : PreferencesPanel(parent)
{
 setObjectName("ManagerDefaultsConfigFile");
 dirty = false;
 setLayout(thisLayout = new QVBoxLayout);//(this, BoxLayout.Y_AXIS));

 matrix = new QWidget();
 matrix->setObjectName("matrix");
 matrixLayout = NULL;
 thisLayout->addWidget(matrix);
// ManagerDefaultSelector::instance.addPropertyChangeListener((PropertyChangeEvent e) -> {
//        if (e.getPropertyName().equals("Updated")) {
//            update();
//        }
//    });
 ManagerDefaultSelector* mgr = ManagerDefaultSelector::instance;
 connect(mgr, SIGNAL(notifyPropertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));
 update();
}

ManagerDefaultsConfigPane::~ManagerDefaultsConfigPane() {}

ManagerDefaultsConfigPane::ManagerDefaultsConfigPane(const ManagerDefaultsConfigPane&) : PreferencesPanel() {}

void ManagerDefaultsConfigPane::propertyChange(PropertyChangeEvent * e)
{
 if (e->getPropertyName()==("Updated"))
 {
  update();
 }
}

/**
 * Invoke when first displayed to load and present options
 */
/*public*/ void ManagerDefaultsConfigPane::update()
{
 //matrix.removeAll();
 if(matrixLayout != NULL)
 {
  QObjectList ol = matrixLayout->children();
  foreach(QObject* o, ol)
  {
   if(qobject_cast<QWidget*>(o) != NULL)
   {
    matrix->layout()->removeWidget((QWidget*)o);
    o->deleteLater();
   }
  }
  matrixLayout->deleteLater();
  matrixLayout = NULL;
 }

 // this doesn't find non-migrated systems, how do we handle that eventually?
 QList<SystemConnectionMemo*>* connList = (QList<SystemConnectionMemo*>*)InstanceManager::getList("SystemConnectionMemo");
 if (connList != NULL)
 {
  reloadConnections(connList);
 }
 else
 {
  matrixLayout->addWidget(new QLabel("No new-form system connections configured"));
 }
}

void ManagerDefaultsConfigPane::reloadConnections(QList<SystemConnectionMemo*>* connList)
{
 int row = 0;
 int col = 0;
 if(matrixLayout == NULL)
  matrix->setLayout(matrixLayout = new QGridLayout); //(connList.size() + 1, ManagerDefaultSelector.instance.knownManagers.length + 1));
 matrixLayout->addWidget(new QLabel(""), row, col++);


 foreach (ManagerDefaultSelector::Item* item, ManagerDefaultSelector::instance->knownManagers)
 {
  matrixLayout->addWidget(new QLabel(item->typeName), row, col++);
 }
 row++;
 col = 1;
 groups = QVector<QButtonGroup*>(ManagerDefaultSelector::instance->knownManagers.length());
 for (int i = 0; i < ManagerDefaultSelector::instance->knownManagers.length(); i++)
 {
  groups.replace(i, new QButtonGroup());
 }
 for (int x = 0; x < connList->size(); x++)
 {
  SystemConnectionMemo* memo = connList->at(x);
  QString name = memo->getUserName();
  matrixLayout->addWidget(new QLabel(name));
  int i = 0;
  foreach (ManagerDefaultSelector::Item* item, ManagerDefaultSelector::instance->knownManagers)
  {
   if (memo->provides(item->managerClass))
   {
    QRadioButton* r = new SelectionButton(name, item->managerClass, this);
    matrixLayout->addWidget(r, row, col++);
    groups[i]->addButton(r);
    if (x == connList->size() - 1 && ManagerDefaultSelector::instance->getDefault(item->managerClass) == NULL) {
     r->setChecked(true);
    }
   }
   else
   {
    // leave a blank
    QRadioButton* r = new QRadioButton();
    r->setEnabled(false);
    matrixLayout->addWidget(r, row, col++);
   }
   i++; //we need to increment 'i' as we are going onto the next group even if we added a blank button
  }
  row++;
  col = 1;
 }
 //revalidate();
 //update();
}



//@Override
/*public*/ QString ManagerDefaultsConfigPane::getPreferencesItem() {
    return "DEFAULTS"; // NOI18N
}

//@Override
/*public*/ QString ManagerDefaultsConfigPane::getPreferencesItemText() {
    return tr("Defaults"); // NOI18N
}

//@Override
/*public*/ QString ManagerDefaultsConfigPane::getTabbedPreferencesTitle() {
    return tr("TabbedLayoutDefaults"); // NOI18N
}

//@Override
/*public*/ QString ManagerDefaultsConfigPane::getLabelKey() {
    return tr("Set connection defaults here; may have to save and restart to see all choices"); // NOI18N
}

//@Override
/*public*/ QWidget* ManagerDefaultsConfigPane::getPreferencesComponent() {
    return this;
}

//@Override
/*public*/ bool ManagerDefaultsConfigPane::isPersistant() {
    return true;
}

//@Override
/*public*/ QString ManagerDefaultsConfigPane::getPreferencesTooltip() {
    return "";
}

//@Override
/*public*/ void ManagerDefaultsConfigPane::savePreferences() {
    // do nothing - the persistant manager will take care of this
}

//@Override
/*public*/ bool ManagerDefaultsConfigPane::isDirty() {
    return this->dirty;
}

//@Override
/*public*/ bool ManagerDefaultsConfigPane::isRestartRequired() {
    return this->isDirty();
}

/**
 * Captive class to track changes
 */
//static class SelectionButton extends JRadioButton {

//    /**
//     *
//     */
//    private static final long serialVersionUID = -2572336492673634333L;

    SelectionButton::SelectionButton(QString name, QString managerClass, ManagerDefaultsConfigPane* pane)
    {
     //super();
     this->managerClass = managerClass;
     this->name = name;
     this->pane = pane;

     if (name == (ManagerDefaultSelector::instance->getDefault(managerClass)))
     {
      QRadioButton::setChecked(true);
     }

//        addActionListener((ActionEvent e) -> {
//            if (isSelected()) {
//                ManagerDefaultSelector.instance.setDefault(SelectionButton.this.managerClass, SelectionButton.this.name);
//                pane.dirty = true;
//            }
//        });
     connect(this, SIGNAL(toggled(bool)), this, SLOT(On_toggled(bool)));
    }

    //@Override
    /*public*/ void SelectionButton::setSelected(bool t) {
        QRadioButton::setChecked(t);
        if (t) {
            ManagerDefaultSelector::instance->setDefault(this->managerClass, this->name);
        }
    }
    void SelectionButton::On_toggled(bool t)
    {
     if (t)
     {
      ManagerDefaultSelector::instance->setDefault(this->managerClass, this->name);
      pane->dirty = true;
     }
    }
//};
