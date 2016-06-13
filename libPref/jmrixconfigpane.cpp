#include "jmrixconfigpane.h"
#include "connectionconfig.h"
#include "instancemanager.h"
#include <QBoxLayout>
#include <QComboBox>
#include "dccmanufacturerlist.h"
#include <QVector>
#include "defaultusermessagepreferences.h"
#include <QScrollArea>
#include "jtitledseparator.h"

//JmrixConfigPane::JmrixConfigPane(QWidget *parent) :
//    QWidget(parent)
//{
//}
/**
 * Provide GUI to configure communications links.
 * <P>
 * This is really just a catalog of connections to classes within the systems.
 * Reflection is used to reduce coupling at load time.
 * <P>
 * Objects of this class are based on an underlying ConnectionConfig
 * implementation, which in turn is obtained from the InstanceManager. Those
 * must be created at load time by the ConfigXml process, or in some Application
 * class.
 * <P>
 * The classes referenced are the specific subclasses of
 * {@link jmri.jmrix.ConnectionConfig} which provides the methods providing data
 * to the configuration GUI, and responding to its changes.
 * <p>
 *
 * @author Bob Jacobsen Copyright (C) 2001, 2003, 2004, 2010
 * @version	$Revision: 28113 $
 */
// /*public*/ class JmrixConfigPane extends QWidget* implements PreferencesPanel {

    /**
     *
     */
//    private static final long serialVersionUID = -6184977238513337292L;
//    private static final ResourceBundle acb = ResourceBundle.getBundle("apps.AppsConfigBundle");

/**
 * Get access to a pane describing existing configuration information, or
 * create one if needed.
 * <P>
 * The index argument is used to connect the new pane to the right
 * communications info. A value of "1" means the first (primary) port, 2 is
 * the second, etc.
 *
 * @param index 1-N based index of the communications object to configure.
 * @return a configuration panel for the specified communications object.
 */
/*public*/ /*static*/ JmrixConfigPane* JmrixConfigPane::instance(int index)
{
 JmrixConfigPane* retval = configPaneTable.value(index);
 if (retval != NULL)
 {
  return retval;
 }
 return createPanel(index);
}

/*public*/ /*static*/ JmrixConfigPane* JmrixConfigPane::instance(ConnectionConfig* config)
{
 foreach (int key, configPaneTable.keys())
 {
  if (configPaneTable.value(key)->ccCurrent == config)
  {
   return configPaneTable.value(key);
  }
 }
 return NULL;
}

/*
 * Create panel is separated off from the instance and synchronized, so that only
 * one connection can be configured at once, this prevents multiple threads from
 * trying to create the same panel at the same time.
 */
/*private*/ /*static*/ /*synchronized*/ JmrixConfigPane* JmrixConfigPane::createPanel(int index)
{
 Logger log("JmrixConfigPane");
 JmrixConfigPane* retval = configPaneTable.value((index));
 if (retval != NULL)
 {
  return retval;
 }
 ConfigureManager* mgr = InstanceManager::configureManagerInstance();
 QObject* c = mgr->findInstance("ConnectionConfig", index);
 if(c == NULL)
  log.debug("findInstance returned  NULL");
 else
  log.debug("findInstance returned " + QString(c->metaObject()->className()));
 retval = new JmrixConfigPane((ConnectionConfig*) c);
 configPaneTable.insert(index, retval);
 return retval;
}

/**
 * Get access to a new pane for creating new connections.
 *
 * @return a new configuration panel
 */
/*public*/ /*static*/ JmrixConfigPane* JmrixConfigPane::createNewPanel()
{
 int lastIndex = -1;
 QObjectList conlist = InstanceManager::configureManagerInstance()->getInstanceList("ConnectionConfig");

 if (!conlist.isEmpty())
 {
  lastIndex = conlist.size();
 }
 foreach (int key, configPaneTable.keys())
 {
  if (key > lastIndex)
  {
   lastIndex = key;
  }
 }
 lastIndex++;
 return createPanel(lastIndex);
}

/*public*/ /*static*/ int JmrixConfigPane::getNumberOfInstances() {
    return configPaneTable.size();
}

/*public*/ /*static*/ void JmrixConfigPane::dispose(int index)
{
 Logger log("JmrixConfigPane");
 JmrixConfigPane* retval = configPaneTable.value((index));
 if (retval == NULL)
 {
  log.debug("no instance found therefore can not dispose of it!");
  return;
 }
 dispose(retval);
}

/*public*/ /*static*/ void JmrixConfigPane::dispose(JmrixConfigPane* confPane)
{
 Logger log("JmrixConfigPane");

    if (confPane == NULL) {
        log.debug("no instance found therefore can not dispose of it!");
        return;
    }

    if (confPane->ccCurrent != NULL) {
        try {
            confPane->ccCurrent->dispose();
        } catch (Exception ex) {
            log.error("Error Occured while disposing connection "+ ex.getMessage());
        }
    }
    InstanceManager::configureManagerInstance()->deregister(confPane);
    InstanceManager::configureManagerInstance()->deregister(confPane->ccCurrent);

    configPaneTable.remove(getInstanceNumber(confPane));
}

/*public*/ /*static*/ int JmrixConfigPane::getInstanceNumber(JmrixConfigPane* confPane)
{
 foreach (int key, configPaneTable.keys())
 {
  if (configPaneTable.value(key)==(confPane))
  {
   return key;
  }
 }
 return -1;
}

/*public*/ /*static*/ QList<JmrixConfigPane*> JmrixConfigPane::getListOfConfigPanes()
{
 return configPaneTable.values();
}

/*static*/ /*final*/ QMap<int, JmrixConfigPane*> JmrixConfigPane::configPaneTable = QMap<int, JmrixConfigPane*>();

/*public*/ /*static*/ /*final*/ QString JmrixConfigPane::NONE_SELECTED = tr("(none selected)");
/*public*/ /*static*/ /*final*/ QString JmrixConfigPane::NO_PORTS_FOUND = tr("(no ports found!)");
/*public*/ /*static*/ /*final*/ QString JmrixConfigPane::NONE = tr("(none)");



/**
 * Use "instance" to get one of these. That allows it to reconnect to
 * existing information in an existing ConnectionConfig object. It's
 * permitted to call this with a NULL argument, e.g. for when first
 * configuring the system.
 */
/*private*/ JmrixConfigPane::JmrixConfigPane(ConnectionConfig* original, QWidget* parent) : QWidget(parent)
{
 log = new  Logger("JmrixConfigPane");
 setObjectName("JmrixConfigPane");

 modeBox = new QComboBox();
 manuBox = new QComboBox();
 details = new QWidget();
 details->setObjectName("details");
 p = (UserPreferencesManager*)InstanceManager::getDefault("UserPreferencesManager");

 ccCurrent = original;
 QVBoxLayout* layout;
 setLayout(layout = new QVBoxLayout());
 //this->setBorder(BorderFactory.createEmptyBorder(0, 8, 0, 8));

  manuBox->addItem(NONE_SELECTED);

  manufactureNameList = DCCManufacturerList::getSystemNames();
  foreach (QString manuName, *manufactureNameList)
  {
   if (original != NULL && original->getManufacturer() != NULL
                && original->getManufacturer()==(manuName))
   {
    manuBox->addItem(manuName);
    manuBox->setCurrentIndex(manuBox->findText(manuName));
    updateComboConnection();
   }
   else
   {
    manuBox->addItem(manuName);
   }
  }
//  manuBox.addActionListener((ActionEvent evt) -> {
//        updateComboConnection();
//    });

  connect(manuBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateComboConnection()));
  // get the list of ConnectionConfig items into a selection box
  classConnectionNameList = DCCManufacturerList::getConnectionList( manuBox->currentText());
  QVector<ConnectionConfig*> v = QVector<ConnectionConfig*>(classConnectionNameList.length() + 1);
  classConnectionList =  v.toList(); //[classConnectionNameList.length + 1];
  modeBox->clear();
  modeBox->addItem(NONE_SELECTED);
  if (manuBox->currentIndex() != 0)
  {
   modeBox->setEnabled(true);
  }
  else
  {
   modeBox->setCurrentIndex(0);
   modeBox->setEnabled(false);
  }
  int n = 1;
  if (manuBox->currentIndex() != 0)
  {
   foreach (QString className, classConnectionNameList)
   {
    try
    {
     ConnectionConfig* config;
     if (original != NULL && original->metaObject()->className()==(className))
     {
      config = original;
      log->debug("matched existing config object");
       modeBox->addItem(config->name());
       modeBox->setCurrentIndex(modeBox->findText(config->name()));
       if (classConnectionNameList.length() == 1)
       {
        modeBox->setCurrentIndex(1);
       }
      }
     else
     {
//      Class<?> cl = Class.forName(className);
//      config = (ConnectionConfig) cl.newInstance();
      int id = QMetaType::type(className.toLocal8Bit());
      if(id != 0)
      {
#if QT_VERSION < 0x050000
       config = (ConnectionConfig*)QMetaType::construct(id);
#else
       config = (ConnectionConfig*)QMetaType::create(id);
#endif

       modeBox->addItem(config->name());
      }
      else
       Logger::error("JmrixConfigPane: Can't construct " + className);
     }
     //classConnectionList[n++] = config;
     classConnectionList.replace(n++,config);
    }
    catch (NullPointerException e)
    {
     Logger::error(QString("Attempt to load %1 failed.").arg(className));
    }
//    catch (ClassNotFoundException | InstantiationException | IllegalAccessException e)
//    {
//     log.debug("Attempt to load {} failed: {}.", className, e);
//    }
   }
   if ((modeBox->currentIndex() == 0) && (p->getComboBoxLastSelection( manuBox->currentText()) != NULL))
   {
    modeBox->setCurrentIndex(modeBox->findText(p->getComboBoxLastSelection( manuBox->currentText())));
   }
  }
//    modeBox.addActionListener((ActionEvent a) -> {
//        if ((String) modeBox.getSelectedItem() != NULL) {
//            if (!((String) modeBox.getSelectedItem())==(NONE_SELECTED)) {
//                p.addComboBoxLastSelection((String) manuBox.getSelectedItem(), (String) modeBox.getSelectedItem());
//            }
//        }
//        selection();
//    });
 connect(modeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(On_modeBox_currentIndexChanged(int)));
 //QWidget* manufacturerPanel = new QWidget();
 QHBoxLayout* manufacturerPanelLayout= new QHBoxLayout;
 //manufacturerPanel->setLayout(manufLayout = new QHBoxLayout);
 manufacturerPanelLayout->addWidget(manuBox);
 //QWidget* connectionPanel = new QWidget();
 QHBoxLayout* connectionPanelLayout = new QHBoxLayout;
 //connectionPanel->setLayout(connectionPanelLayout = new QHBoxLayout);
 connectionPanelLayout->addWidget(modeBox);
 //QWidget* initialPanel = new QWidget();
 QVBoxLayout* initialPanelLayout = new QVBoxLayout;
 initialPanelLayout->setAlignment(Qt::AlignTop);
 //initialPanel->setLayout(initialPanelLayout = new QVBoxLayout); //(initialPanel, BoxLayout.Y_AXIS));
 initialPanelLayout->addWidget(new JTitledSeparator(tr("System Manufacturer"))); // NOI18N
 initialPanelLayout->addLayout(manufacturerPanelLayout);
 initialPanelLayout->addWidget(new JTitledSeparator(tr("System Connection"))); // NOI18N
 initialPanelLayout->addLayout(connectionPanelLayout);
 layout->addLayout(initialPanelLayout,0); //, Qt::AlignTop);
 initialPanelLayout->addWidget(new JTitledSeparator(tr("Settings"))); // NOI18N
 QScrollArea* scroll = new QScrollArea(/*details*/);
 scroll->setMinimumSize( 300, 200);
 scroll->setWidget(details);
 scroll->setWidgetResizable(true);
 QGridLayout* gbLayout;
 details->setLayout(gbLayout = new QGridLayout);
 gbLayout->setObjectName("detailsLayout");
 //scroll.setBorder(BorderFactory.createEmptyBorder());
 layout->addWidget(scroll, 0, Qt::AlignCenter);

 selection();  // first time through, pretend we've selected a value
 // to load the rest of the GUI
}
void JmrixConfigPane::On_modeBox_currentIndexChanged(int)
{
 if (modeBox->currentText() != NULL)
 {
  if ( modeBox->currentText()!=(NONE_SELECTED))
  {
   p->addComboBoxLastSelection( manuBox->currentText(), modeBox->currentText());
  }
 }
 selection();
}

/*public*/ void JmrixConfigPane::updateComboConnection()
{
 modeBox->clear();
 modeBox->addItem(NONE_SELECTED);
 classConnectionNameList = DCCManufacturerList::getConnectionList((QString) manuBox->currentText());
 //classConnectionList =  ConnectionConfig(); //[classConnectionNameList.length + 1];
 QVector<ConnectionConfig*> v = QVector<ConnectionConfig*>(classConnectionNameList.length() + 1);
 classConnectionList = v.toList();
 if (manuBox->currentIndex() != 0)
 {
  modeBox->setEnabled(true);
 }
 else
 {
  modeBox->setCurrentIndex(0);
  modeBox->setEnabled(false);
 }

 int n = 1;
 if (manuBox->currentIndex() != 0)
 {
  foreach (QString classConnectionNameList1, classConnectionNameList)
  {
//      try
//      {
   ConnectionConfig* config = NULL;
//       Class<?> cl = Class.forName(classConnectionNameList1);
//       config = (ConnectionConfig*) cl.newInstance();
//       modeBox.addItem(config.name());
  int id = QMetaType::type(classConnectionNameList1.toLocal8Bit());
  if(id != 0)
  {
#if QT_VERSION < 0x050000
   config = (ConnectionConfig*)QMetaType::construct(id);
#else
   config = (ConnectionConfig*)QMetaType::create(id);
#endif
    modeBox->addItem(config->name());
  }
  else
  {
   log->warn(tr("Attempt to load %1 failed").arg(classConnectionNameList1));
  }
  classConnectionList.replace(n++, config);
  if (classConnectionNameList.length() == 1)
  {
   modeBox->setCurrentIndex(1);
  }
//      }
//      catch (NullPointerException | ClassNotFoundException | InstantiationException | IllegalAccessException e)
//      {
//        log.warn("Attempt to load {} failed: {}", classConnectionNameList1, e);
//      }
  }
  if (p->getComboBoxLastSelection(manuBox->currentText()) != NULL)
  {
   modeBox->setCurrentIndex(modeBox->findText(p->getComboBoxLastSelection( manuBox->currentText())));
  }
 }
 else
 {
  if (ccCurrent != NULL)
  {
   ccCurrent->dispose();
  }
 }
}

void JmrixConfigPane::selection()
{
 Logger log("JmrixConfigPane");

 int current = modeBox->currentIndex();
 //details.removeAll();
 if(details->layout() != NULL)
 {
  QObjectList objects = details->children();
  foreach (QObject* o, objects)
  {
   if(qobject_cast<QWidget*>(o)!= NULL)
   {
    QWidget* w = (QWidget*)o;
    details->layout()->removeWidget(w);
    w->hide();
    //delete w;
   }
   //delete details->layout();
  }
 }
 // first choice is -no- protocol chosen
 log.debug("new selection is  "+ QString::number(current)+ " " + modeBox->currentText());
 if ((current != 0) && (current != -1))
 {
  if ((ccCurrent != NULL) && (ccCurrent != classConnectionList[current]))
  {
   ccCurrent->dispose();
  }
  ccCurrent = classConnectionList[current];
  classConnectionList[current]->loadDetails(details);
  classConnectionList[current]->setManufacturer( manuBox->currentText());
 }
 else
 {
  if (ccCurrent != NULL)
  {
   ccCurrent->dispose();
  }
 }
 //validate();

 //repaint();
 update();
}

/*public*/ QString JmrixConfigPane::getConnectionName() {
    int current = modeBox->currentIndex();
    if (current == 0) {
        return "";
    }
    return classConnectionList[current]->getConnectionName();
}

/*public*/ QString JmrixConfigPane::getCurrentManufacturerName() {
    int current = modeBox->currentIndex();
    if (current == 0) {
        return NONE;
    }
    return classConnectionList[current]->getManufacturer();
}

/*public*/ QString JmrixConfigPane::getCurrentProtocolName() {
    int current = modeBox->currentIndex();
    if (current == 0) {
        return NONE;
    }
    return classConnectionList[current]->name();
}

/*public*/ QString JmrixConfigPane::getCurrentProtocolInfo() {
    int current = modeBox->currentIndex();
    if (current == 0) {
        return NONE;
    }
    return classConnectionList[current]->getInfo();
}

/*public*/ QObject* JmrixConfigPane::getCurrentObject() {
    int current = modeBox->currentIndex();
    if (current != 0) {
        return classConnectionList[current];
    }
    return NULL;
}

/*public*/ bool JmrixConfigPane::getDisabled() {
    int current = modeBox->currentIndex();
    if (current == 0) {
        return false;
    }
    return classConnectionList[current]->getDisabled();
}

/*public*/ void JmrixConfigPane::setDisabled(bool disabled) {
    int current = modeBox->currentIndex();
    if (current == 0) {
        return;
    }
    classConnectionList[current]->setDisabled(disabled);
}

// initialize logging
//static Logger log = LoggerFactory.getLogger(JmrixConfigPane.class);

//@Override
/*public*/ QString JmrixConfigPane::getPreferencesItem() {
    return "CONNECTIONS"; // NOI18N
}

//@Override
/*public*/ QString JmrixConfigPane::getPreferencesItemText() {
    return tr("Connections"); // NOI18N
}

//@Override
/*public*/ QString JmrixConfigPane::getTabbedPreferencesTitle() {
    QString title = this->getConnectionName();
    if (title == NULL
            && this->getCurrentProtocolName() != NULL
            && this->getCurrentProtocolName()!=(JmrixConfigPane::NONE)) {
        title = this->getCurrentProtocolName();
    }
    if (title != NULL && !this->getDisabled()) {
        title = "(" + title + ")";
    }
    return title;
}

//@Override
/*public*/ QString JmrixConfigPane::getLabelKey() {
    return NULL;
}

//@Override
/*public*/ QWidget* JmrixConfigPane::getPreferencesComponent() {
    return this;
}

//@Override
/*public*/ bool JmrixConfigPane::isPersistant() {
    return true;
}

//@Override
/*public*/ QString JmrixConfigPane::getPreferencesTooltip() {
    return this->getTabbedPreferencesTitle();
}

//@Override
/*public*/ void JmrixConfigPane::savePreferences() {
    // do nothing - the persistant manager will take care of this
}

//@Override
/*public*/ bool JmrixConfigPane::isDirty() {
    return this->ccCurrent->isDirty();
}

//@Override
/*public*/ bool JmrixConfigPane::isRestartRequired() {
    return this->ccCurrent->isRestartRequired();
}

//@Override
/*public*/ bool isPreferencesValid() {
    return true; // no validity checking performed
}
