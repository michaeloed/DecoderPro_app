#include "internalsystemconnectionmemo.h"
#include "instancemanager.h"
#include "loconet/HexFile/debugprogrammermanager.h"
#include "defaultpowermanager.h"
#include "loconet/HexFile/debugthrottlemanager.h"
#include "internalturnoutmanager.h"
#include "internalsensormanager.h"
#include "internalreportermanager.h"
#include "internallightmanager.h"
#include "loggerfactory.h"
#include "internalconsistmanager.h"

//InternalSystemConnectionMemo::InternalSystemConnectionMemo()
//{

//}
/**
 * Lightweight class to denote that a system is active, and provide general
 * information.
 * <p>
 * Things this needed to do:
 * <ul>
 * <li>One of these must be automatically, transparently available - this is done by
 *      inheriting from InstanceManagerAutoDefault
 * <li>It must be possible to have more than one of these, so you can have
 *      multiple internal systems defined - each one keeps internal references
 *      to its objects
 * <li>It must make sure that its objects are available individually through the instance manager.
 * <li>But it also has to handle the ProxyManager special cases in the InstanceManager
 * </ul>
 * <p>
 *
 * @author	Bob Jacobsen Copyright (C) 2010, 2016
 */
// /*public*/ class InternalSystemConnectionMemo extends jmrix.SystemConnectionMemo implements InstanceManagerAutoDefault {

/*public*/ InternalSystemConnectionMemo::InternalSystemConnectionMemo(QObject *parent) : SystemConnectionMemo("I", "Internal", parent)
{
 setObjectName("InternalSystemConnectionMemo");

 configured = false;
 turnoutManager = nullptr;
 lightManager = nullptr;
 sensorManager = nullptr;
 reporterManager = nullptr;
 throttleManager = nullptr;
 powerManager = nullptr;
 programManager = nullptr;

 //super("I", "Internal");
 InstanceManager::store(this, "InternalSystemConnectionMemo"); // also register as specific type
 QObjectList* l = InstanceManager::getList("InternalSystemConnectionMemo");

 _register();
}


/**
 * Configure the common managers for Internal connections. This puts the
 * common manager config in one place.
 * <p> Note: The Proxy system can cause some managers to be created early.
 * We don't call configureManagers in that case, as it causes an infinite loop.
 */
/*public*/ void InternalSystemConnectionMemo::configureManagers() {
    log->debug("Do configureManagers - doesn't pre-build anything");
    if (configured) log->warn("configureManagers called for a second time"/*, new Exception("traceback")*/);
    configured = true;

}

//@Override
/*public*/ InternalConsistManager* InternalSystemConnectionMemo::getConsistManager()
{
    if (consistManager == nullptr) {
        log->debug("Create InternalConsistManager by request");
        consistManager = new InternalConsistManager();
        // special due to ProxyManager support
        InstanceManager::store(consistManager,"ConsistManager");
    }
    return consistManager;
}

/*public*/ InternalLightManager* InternalSystemConnectionMemo::getLightManager()
{
 if (lightManager == nullptr)
 {
     log->debug("Create InternalLightManager by request");
     lightManager = new InternalLightManager();
     // special due to ProxyManager support
     InstanceManager::setLightManager(lightManager);
 }
 return lightManager;
}

/*public*/ InternalSensorManager* InternalSystemConnectionMemo::getSensorManager()
{
 if (sensorManager == nullptr)
 {
  log->debug(tr("Create InternalSensorManager \"%1\" by request").arg(getSystemPrefix()));
  sensorManager = new InternalSensorManager(getSystemPrefix());
  // special due to ProxyManager support
  InstanceManager::setSensorManager(sensorManager);
 }
 return sensorManager;
}

/*public*/ InternalReporterManager* InternalSystemConnectionMemo::getReporterManager() {
    if (reporterManager == nullptr) {
        log->debug("Create InternalReporterManager by request");
        reporterManager = new InternalReporterManager();
        // special due to ProxyManager support
        InstanceManager::setReporterManager(reporterManager);
    }
    return reporterManager;
}

/*public*/ InternalTurnoutManager* InternalSystemConnectionMemo::getTurnoutManager() {
    if (turnoutManager == nullptr) {
        log->debug(tr("Create InternalTurnoutManager \"%1\" by request").arg( getSystemPrefix()));
        turnoutManager = new InternalTurnoutManager(getSystemPrefix());
        // special due to ProxyManager support
        InstanceManager::setTurnoutManager(turnoutManager);
    }
    return turnoutManager;
}

/*public*/ DebugThrottleManager* InternalSystemConnectionMemo::getThrottleManager() {
    if (throttleManager == nullptr) {
        log->debug("Create DebugThrottleManager by request");
        // Install a debug throttle manager
        throttleManager = new DebugThrottleManager(this);
        InstanceManager::setThrottleManager(throttleManager);
    }
    return throttleManager;
}

/*public*/ DefaultPowerManager* InternalSystemConnectionMemo::getPowerManager() {
    if (powerManager == nullptr) {
        log->debug("Create DefaultPowerManager by request");
        powerManager = new DefaultPowerManager();
        InstanceManager::store(powerManager, "PowerManager");
    }
    return powerManager;
}

/*public*/ DebugProgrammerManager* InternalSystemConnectionMemo::getProgrammerManager() {
    if (programManager == nullptr) {
        log->debug("Create DebugProgrammerManager by request");
        // Install a debug programmer
        programManager = new DebugProgrammerManager(this);
        // Don't auto-enter, as that messes up selection in Single CV programmer
        //InstanceManager.setProgrammerManager(programManager);
    }
    return programManager;
}

//@Override
/*public*/ bool InternalSystemConnectionMemo::provides(/*Class<?>*/QString type) {
    if (getDisabled()) {
        return false;
    }

    if (!configured) configureManagers();

    if (type == ("GlobalProgrammerManager")) {
        return getProgrammerManager()->isGlobalProgrammerAvailable();
    }
    if (type == ("AddressedProgrammerManager")) {
        return getProgrammerManager()->isAddressedModePossible();
    }

    if (type == ("ThrottleManager")) {
        return true;
    }
    if (type == ("PowerManager")) {
        return true;
    }
    if (type == ("SensorManager")) {
        return true;
    }
    if (type == ("LightManager")) {
        return true;
    }
    if (type == ("ReporterManager")) {
        return true;
    }
    if (type == ("TurnoutManager")) {
        return true;
    }
    if (type == ("ConsistManager")) {
      return true;
    }
    return false; // nothing, by default
}

//@SuppressWarnings("unchecked")
//@Override
/*public*/ QObject* InternalSystemConnectionMemo::get(/*Class<?>*/ QString className) {
    if (getDisabled()) {
        return nullptr;
    }

    if (!configured) configureManagers();

    if (className == ("ProgrammerManager")) {
        return (ProgrammerManager*) getProgrammerManager();
    }
    if (className== ("GlobalProgrammerManager")) {
        return (GlobalProgrammerManager*) getProgrammerManager();
    }
    if (className == ("AddressedProgrammerManager")) {
        return (AddressedProgrammerManager*) getProgrammerManager();
    }

    if (className == ("ThrottleManager")) {
        return (ThrottleManager*) getThrottleManager();
    }
    if (className == ("PowerManager")) {
        return (PowerManager*) getPowerManager();
    }
    if (className == ("LightManager")) {
        return (LightManager*) getLightManager();
    }
    if (className == ("SensorManager")) {
        return (SensorManager*) getSensorManager();
    }
    if (className == ("ReporterManager")) {
        return (ReporterManager*) getReporterManager();
    }
    if (className == ("TurnoutManager")) {
        return (TurnoutManager*) getTurnoutManager();
    }
    if (className == ("ConsistManager")) {
        return (ConsistManager*) getConsistManager();
    }

    return nullptr; // nothing, by default
}

//@Override
///*protected*/ ResourceBundle getActionModelResourceBundle() {
//    //No actions to add at start up
//    return nullptr;
//}

//@Override
/*public*/ void InternalSystemConnectionMemo::dispose() {
    if (sensorManager != nullptr) {
        sensorManager->dispose();
        sensorManager = nullptr;
    }
    if (turnoutManager != nullptr) {
        turnoutManager->dispose();
        turnoutManager = nullptr;
    }
    SystemConnectionMemo::dispose();
}

/*private*/ /*static*/ /*final*/ Logger* InternalSystemConnectionMemo::log = LoggerFactory::getLogger("InternalSystemConnectionMemo");