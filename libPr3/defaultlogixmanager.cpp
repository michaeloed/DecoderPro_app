#include "defaultlogixmanager.h"
#include "decimalformat.h"
#include "conditional.h"
#include "logix.h"
#include "defaultlogix.h"
#include "instancemanager.h"
#include "entryexitpairs.h"
#include "oblockmanager.h"
#include "warrantmanager.h"
#include "signalheadmanager.h"
#include "signalmastmanager.h"

/*static*/ DefaultLogixManager* DefaultLogixManager::_instance = nullptr;

DefaultLogixManager::DefaultLogixManager(QObject *parent) :
    LogixManager(parent)
{
 setObjectName("DefaultLogixManager");
 setProperty("JavaClassName", "jmri.managers.DefaultLogixManager");

 log = new Logger("DefaultLogixManager");
 paddedNumber = new DecimalFormat("0000");
 registerSelf();

 //jmri.InstanceManager.turnoutManagerInstance().addVetoableChangeListener(this);
 connect(InstanceManager::turnoutManagerInstance(), SIGNAL(vetoablePropertyChange(PropertyChangeEvent*)), this, SLOT(vetoableChange(PropertyChangeEvent*)));
 //jmri.InstanceManager.sensorManagerInstance().addVetoableChangeListener(this);
 connect(InstanceManager::sensorManagerInstance(), SIGNAL(vetoablePropertyChange(PropertyChangeEvent*)), this, SLOT(vetoableChange(PropertyChangeEvent*)));
 //jmri.InstanceManager.memoryManagerInstance().addVetoableChangeListener(this);
 connect(InstanceManager::memoryManagerInstance(), SIGNAL(vetoablePropertyChange(PropertyChangeEvent*)), this, SLOT(vetoableChange(PropertyChangeEvent*)));
 //jmri.InstanceManager.getDefault(jmri.SignalHeadManager.class).addVetoableChangeListener(this);
 connect(static_cast<SignalHeadManager*>(InstanceManager::getDefault("SignalHeadManager")), SIGNAL(vetoablePropertyChange(PropertyChangeEvent*)), this, SLOT(vetoableChange(PropertyChangeEvent*)));
 //jmri.InstanceManager.getDefault(jmri.SignalMastManager.class).addVetoableChangeListener(this);
 connect(static_cast<SignalMastManager*>(InstanceManager::getDefault("SignalMastManager")), SIGNAL(vetoablePropertyChange(PropertyChangeEvent*)), this, SLOT(vetoableChange(PropertyChangeEvent*)));
 //jmri.InstanceManager.getDefault(jmri.BlockManager.class).addVetoableChangeListener(this);
 connect(static_cast<BlockManager*>(InstanceManager::getDefault("BlockManager")), SIGNAL(vetoablePropertyChange(PropertyChangeEvent*)), this, SLOT(vetoableChange(PropertyChangeEvent*)));
 //jmri.InstanceManager.lightManagerInstance().addVetoableChangeListener(this);
 connect(InstanceManager::lightManagerInstance(), SIGNAL(vetoablePropertyChange(PropertyChangeEvent*)), this, SLOT(vetoableChange(PropertyChangeEvent*)));
 //jmri.InstanceManager.getDefault(jmri.ConditionalManager.class).addVetoableChangeListener(this);
 connect(static_cast<ConditionalManager*>(InstanceManager::getDefault("ConditionalManager")), SIGNAL(vetoablePropertyChange(PropertyChangeEvent*)), this, SLOT(vetoableChange(PropertyChangeEvent*)));
 //InstanceManager.getDefault(jmri.jmrit.logix.WarrantManager.class).addVetoableChangeListener(this);
 connect(static_cast<WarrantManager*>(InstanceManager::getDefault("WarrantManager")), SIGNAL(vetoablePropertyChange(PropertyChangeEvent*)), this, SLOT(vetoableChange(PropertyChangeEvent*)));
 //InstanceManager.getDefault(jmri.jmrit.logix.OBlockManager.class).addVetoableChangeListener(this);
 connect(static_cast<OBlockManager*>(InstanceManager::getDefault("OBlockManager")), SIGNAL(vetoablePropertyChange(PropertyChangeEvent*)), this, SLOT(vetoableChange(PropertyChangeEvent*)));
 //InstanceManager.getDefault(jmri.jmrit.entryexit.EntryExitPairs.class).addVetoableChangeListener(this);
 connect(static_cast<EntryExitPairs*>(InstanceManager::getDefault("EntryExitPairs")), SIGNAL(vetoablePropertyChange(PropertyChangeEvent*)), this, SLOT(vetoableChange(PropertyChangeEvent*)));
}
/**
 * Basic Implementation of a LogixManager.
 * <P>
 * Note that Logix system names must begin with IX, and be followed by a
 * string, usually, but not always, a number. All alphabetic characters
 * in a Logix system name must be upper case. This is enforced when a Logix
 * is created.
 * <P>
 * The system names of Conditionals belonging to a Logix begin with the
 * Logix's system name, then there is a capital C and a number.
 *
 * @author      Dave Duchamp Copyright (C) 2007
 * @version	$Revision: 18102 $
 */
///*public*/ class DefaultLogixManager extends AbstractManager
//    implements LogixManager, java.beans.PropertyChangeListener {

///*public*/ DefaultLogixManager() {
//    super();
//}

/*public*/ int DefaultLogixManager::getXMLOrder(){
    return Manager::LOGIXS;
}

/*public*/ QString DefaultLogixManager::getSystemPrefix() { return "I"; }
/*public*/ char DefaultLogixManager::typeLetter() { return 'X'; }

/**
 * Method to create a new Logix if the Logix does not exist
 *   Returns nullptr if a Logix with the same systemName or userName
 *       already exists, or if there is trouble creating a new Logix.
 */
/*public*/ Logix* DefaultLogixManager::createNewLogix(QString systemName, QString userName) {
    // Check that Logix does not already exist
    Logix* x;
    if (userName!= "" ) {
        x = getByUserName(userName);
        if (x!=nullptr) return nullptr;
    }
    x = getBySystemName(systemName);
    if (x==nullptr) x = getBySystemName(systemName.toUpper());   // for compatibility?
    if (x!=nullptr) return nullptr;
    // Logix does not exist, create a new Logix
    x = (Logix*)new DefaultLogix(systemName,userName);
    // save in the maps
    Register(x);
    //emit newLogixCreated(x);
    emit propertyChange(new PropertyChangeEvent(this, "length", QVariant(), QVariant(_tsys->count())));

    /*The following keeps trace of the last created auto system name.
    currently we do not reuse numbers, although there is nothing to stop the
    user from manually recreating them*/
    if (systemName.startsWith("IX:AUTO:"))
    {
     bool bOk;
     int autoNumber = systemName.mid(8).toInt(&bOk);
     if (autoNumber > lastAutoLogixRef)
     {
      lastAutoLogixRef = autoNumber;
     }
     if(!bOk)
       log->warn("Auto generated SystemName "+ systemName + " is not in the correct format");

 }
 return x;
}

/*public*/ Logix* DefaultLogixManager::createNewLogix(QString userName) {
    int nextAutoLogixRef = lastAutoLogixRef+1;
    QString b = QString("IX:AUTO:");
    QString nextNumber = paddedNumber->format(nextAutoLogixRef);
    b.append(nextNumber);
    return createNewLogix(b, userName);
}

/**
 * Remove an existing Logix and delete all its conditionals.
 * Logix must have been deactivated
 * before invoking this.
 */
/*public*/ void DefaultLogixManager::deleteLogix(Logix* x) {
    // delete conditionals if there are any
    int numConditionals = x->getNumConditionals();
    if (numConditionals>0) {
        Conditional* c = nullptr;
        for (int i = 0;i<numConditionals;i++) {
            c = static_cast<ConditionalManager*>(InstanceManager::getDefault("ConditionalManager"))->getBySystemName(
                    x->getConditionalByNumberOrder(i));
            static_cast<ConditionalManager*>(InstanceManager::getDefault("ConditionalManager"))->deleteConditional(c);
        }
    }
    // delete the Logix
    deregister(x);
    x->dispose();
}

/**
 * Activate all Logixs that are not currently active
 * This method is called after a configuration file is loaded.
 */
/*public*/ void DefaultLogixManager::activateAllLogixs() {
    // Guarantee Initializer executes first.
    Logix* x = getBySystemName(/*LRouteTableAction::LOGIX_INITIALIZER*/"RTXINITIALIZER");
    if (x!=nullptr) {
        x->activateLogix();
    }
    // iterate thru all Logixs that exist
    QStringListIterator iter(getSystemNameList());
    while (iter.hasNext()) {
        // get the next Logix
        QString sysName = iter.next();
        if (sysName==nullptr) {
            log->error("System name nullptr when activating Logixs");
            break;
        }
        if (sysName==(/*LRouteTableAction.LOGIX_INITIALIZER*/"RTXINITIALIZER")) {
            continue;
        }
        x = getBySystemName(sysName);
        if (x==nullptr) {
            log->error("Error getting Logix *"+sysName+"* when activating Logixs");
            break;
        }
        if (loadDisabled) {
            // user has requested that Logixs be loaded disabled
            log->warn("load disabled set - will not activate logic for: " + x->getDisplayName());
            x->setEnabled(false);
        }
        if (x->getEnabled()){
            //System.out.println("logix set enabled");
            x->activateLogix();
        }
    }
    // reset the load switch
    loadDisabled = false;
}

/**
 * Method to get an existing Logix.  First looks up assuming that
 *      name is a User Name.  If this fails looks up assuming
 *      that name is a System Name.  If both fail, returns nullptr.
 */
/*public*/ Logix* DefaultLogixManager::getLogix(QString name) {
    Logix* x = getByUserName(name);
    if (x!=nullptr) return x;
    return getBySystemName(name);
}

/*public*/ Logix* DefaultLogixManager::getBySystemName(QString name) {
    return (Logix*)_tsys->value(name);
}

/*public*/ Logix* DefaultLogixManager::getByUserName(QString key) {
    return (Logix*)_tuser->value(key);
}

/**
 * Support for loading Logixs in a disabled state to debug loops
 */
/*public*/ void DefaultLogixManager::setLoadDisabled(bool s) {loadDisabled = s;}

/*static*/ /*public*/ DefaultLogixManager* DefaultLogixManager::instance() {
    if (_instance == nullptr) {
        _instance = new DefaultLogixManager();
    }
    return (_instance);
}

//    static org.apache.log4j.Logger log = org.apache.log4j.Logger.getLogger(DefaultLogixManager.class.getName());
//}
