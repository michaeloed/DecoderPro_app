#ifndef INSTANCEMANAGER_H
#define INSTANCEMANAGER_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QList>
#include <QVector>
#include "logger.h"
#include "timebase.h"
#include "powermanager.h"
#include "sensormanager.h"
#include "configuremanager.h"
#include "programmermanager.h"
#include "clockcontrol.h"
#include "turnoutmanager.h"
//#include "lightmanager.h"
#include "reportermanager.h"
//#include "signalheadmanager.h"
//#ifdef BLOCKS_AND_SECTIONS
#include "../LayoutEditor/sectionmanager.h"
#include "blockmanager.h"
//#endif
//#ifdef LAYOUTS
//#include "Layout/layoutblockmanager.h"
//#endif
//#include "transitmanager.h"
//#include "catalogtreemanager.h"
#include "audiomanager.h"
#include "memorymanager.h"
//#include "rostericonfactory.h"
#include "conditionalmanager.h"
#include "shutdownmanager.h"
#include "logixmanager.h"
#include "consistmanager.h"
#include "dccconsistmanager.h"
#include "instanceinitializer.h"
#include "commandstation.h"
#include "abstractproxymanager.h"
#include "proxyreportermanager.h"
#include "proxysensormanager.h"
#include "throttlemanager.h"
#include "proxyturnoutmanager.h"
#include "proxylightmanager.h"
//#ifdef BLOCKS_AND_SECTIONS
#include "defaultinstanceinitializer.h"
//#endif
#include "routemanager.h"
#include "lightmanager.h"
/**
 * Provides methods for locating various interface implementations.
 * These form the base for locating JMRI objects, including the key managers.
 *<p>
 * The structural goal is to have the jmri package not depend on the
 * lower jmri.jmrit and jmri.jmrix packages, with the implementations
 * still available at run-time through the InstanceManager.
 *<p>
 * To retrieve the default object of a specific type, do
 * {@link    InstanceManager#getDefault}
 * where the argument is e.g. "SensorManager.class".
 * In other words, you ask for the default object of a particular type.
 *<p>
 * Multiple items can be held, and are retrieved as a list with
 * {@link    InstanceManager#getList}.
 *<p>
 * If a specific item is needed, e.g. one that has been constructed via
 * a complex process during startup, it should be installed with
 * {@link     InstanceManager#store}.
 * If it's OK for the InstanceManager to create an object on first
 * request, have that object's class implement the
 * {@link     InstanceManagerAutoDefault}
 * flag interface. The InstanceManager will then construct a default
 * object via the no-argument constructor when one is first needed.
 *<p>
 * For initialization of more complex objects, see the
 * {@link InstanceInitializer} mechanism and it's default implementation
 * in {@link jmri.managers.DefaultInstanceInitializer}.
 *
 * <hr>
 * This file is part of JMRI.
 * <P>
 * JMRI is free software; you can redistribute it and/or modify it under
 * the terms of version 2 of the GNU General Public License as published
 * by the Free Software Foundation. See the "COPYING" file for a copy
 * of this license.
 * <P>
 * JMRI is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 * <P>
 * @author			Bob Jacobsen Copyright (C) 2001, 2008, 2013
 * @author          Matthew Harris copyright (c) 2009
 * @version			$Revision: 22528 $
 */
class RouteManager;
class BlockManager;
class SectionManager;
class TabbedPreferences;
class TransitManager;
class LightManager;
class SignalHeadManager;
class SignalMastManager;
class SignalSystemManager;
class SignalGroupManager;
class SignalMastLogicManager;
class LayoutBlockManager;
class CatalogTreeManager;
class LIBPR3SHARED_EXPORT InstanceManager : public QObject
{
    Q_OBJECT
public:
    Q_DECL_DEPRECATED InstanceManager(QObject *parent = 0);
/**
 * Store an object of a particular type for later
 * retrieval via {@link #getDefault} or {@link #getList}.
 * @param item The object of type T to be stored
 * @param type The class Object for the item's type.  This will be used
 *               as the key to retrieve the object later.
 */
//    static public <T> void store(T item, Class<T> type);
static void store(QObject* item, QString type);
/**
 * Retrieve a list of all objects of type T that were
 * registered with {@link #store}.
 * @param type The class Object for the items' type.
 */
//static public <T> List<Object> getList(Class<T> type)
static QObjectList* getList(QString type);
/**
 * Deregister all objects of a particular type.
 * @param type The class Object for the items to be removed.
 */
//static public <T> void reset(Class<T> type) {
static void reset(QString type);
/**
 * Remove an object of a particular type
 * that had earlier been registered with {@link #store}.
 * @param item The object of type T to be deregistered
 * @param type The class Object for the item's type.
 */
// static public <T> void deregister(T item, Class<T> type)
static void deregister(QObject* item, QString type);
/**
 * Retrieve the last object of type T that was
 * registered with {@link #store}.
 * <p>
 * Someday, we may provide another way to set the default
 * but for now it's the last one stored, see the
 * {@link #setDefault} method.
 */
//@SuppressWarnings("unchecked")   // checked by construction
// static public <T> T getDefault(Class<T> type) {
static QObject* getDefault(QString type);
/**
 * Set an object of type T as the default for that type.
 *<p>
 * Also registers (stores) the object if not already present.
 *<p>
 * Now, we do that moving the item to the back of the list;
 * see the {@link #getDefault} method
 */
//static public <T> void setDefault(Class<T> type, T val)
static void setDefault(QString type, QObject* val);
/**
 * Dump generic content of InstanceManager
 * by type.
 */
static QString contentsToString();
static PowerManager* powerManagerInstance();
Q_DECL_DEPRECATED static void setPowerManager(PowerManager* p);
static Timebase* timebaseInstance();
static ConfigureManager* configureManagerInstance();
Q_DECL_DEPRECATED static void setConfigureManager(ConfigureManager* p);

static SensorManager* sensorManagerInstance();
static TurnoutManager* turnoutManagerInstance();
static LightManager* lightManagerInstance();
Q_DECL_DEPRECATED static ProgrammerManager* programmerManagerInstance();
static void setProgrammerManager(ProgrammerManager* p);
static ThrottleManager* throttleManagerInstance();
static void setTurnoutManager(TurnoutManager* p);

static SignalHeadManager* signalHeadManagerInstance();
static SignalMastManager* signalMastManagerInstance();
static void setSignalMastManager(SignalMastManager* p);
static SignalSystemManager* signalSystemManagerInstance();
static void setSignalSystemManager(SignalSystemManager* p);
static SignalGroupManager* signalGroupManagerInstance() ;
static void setSignalGroupManager(SignalGroupManager* p);
static ConsistManager* consistManagerInstance();
static void setConsistManager(ConsistManager* p);
Q_DECL_DEPRECATED static BlockManager* blockManagerInstance();
static SectionManager* sectionManagerInstance();
static SensorManager* sensorManager;
static InstanceManager* instance();
static SignalMastLogicManager* signalMastLogicManagerInstance();

static RouteManager* routeManagerInstance();

static /*public*/ LayoutBlockManager* layoutBlockManagerInstance();
static ConditionalManager* conditionalManagerInstance();
static LogixManager* logixManagerInstance();

static TransitManager* transitManagerInstance();
static ShutDownManager* shutDownManagerInstance();
static TabbedPreferences* tabbedPreferencesInstance();
static ClockControl* clockControlInstance();
Q_DECL_DEPRECATED static void addClockControl(ClockControl* cc);
static CommandStation* commandStationInstance();

static ReporterManager* reporterManagerInstance();
static MemoryManager* memoryManagerInstance();
static AudioManager* audioManagerInstance();
//static RosterIconFactory* rosterIconFactoryInstance();
static CatalogTreeManager* catalogTreeManagerInstance();
static void setSensorManager(SensorManager* p);
static void setLightManager(LightManager* p);
static void setThrottleManager(ThrottleManager* p);
Q_DECL_DEPRECATED static void setSignalHeadManager(SignalHeadManager* p);
static /*public*/ void setLayoutBlockManager(LayoutBlockManager* p);
static void setConditionalManager(ConditionalManager* p);
static void setLogixManager(LogixManager* p);
Q_DECL_DEPRECATED static void setShutDownManager(ShutDownManager* p);
static void setTabbedPreferences(TabbedPreferences* p);
static void setCommandStation(CommandStation* p);
static void setReporterManager(ReporterManager* p);
static void removePropertyChangeListener(PropertyChangeListener* l);


signals:
  void propertyChange(PropertyChangeEvent*);
public slots:
private:
//    static private HashMap<Class<?>,ArrayList<Object>> managerLists;
 static QHash<QString,QObjectList*>* managerLists;
 //static Logger log;
 Logger* log;
 static void setRootInstance();
 static ConfigureManager* configureManager;
 /*private*/ ClockControl* clockControl;// = NULL;
 SectionManager* sectionManager;
 TransitManager* transitManager;
 TurnoutManager* turnoutManager;
 LightManager* lightManager;
 // data members to hold contact with the property listeners
 static QVector<PropertyChangeListener*> listeners;
 CatalogTreeManager* catalogTreeManager;
 AudioManager* audioManager;
 MemoryManager* memoryManager;
 //RosterIconFactory* rosterIconFactory;
 Timebase* timebase;
 SignalHeadManager* signalHeadManager;
 ConditionalManager* conditionalManager;
 ReporterManager* reporterManager;
 ShutDownManager* shutdownManager;
 LogixManager* logixManager;

 TabbedPreferences* tabbedPreferencesManager;
 //VSDecoderManager vsdecoderManager;
 static InstanceInitializer* initializer;// = new jmri.managers.DefaultInstanceInitializer();
 static QMutex mutex;
protected:

 /**
  * The "root" object is the instance manager that's answering
  * requests for other instances. Protected access to allow
  * changes during JUnit testing.
  */
// @edu.umd.cs.findbugs.annotations.SuppressWarnings(
//     value="MS_PKGPROTECT",
//     justification="Protected access to allow changes during JUnit testing.")
 static InstanceManager* root;

 void addConfigureManager(ConfigureManager* p);
 void addTurnoutManager(TurnoutManager* p);
 void notifyPropertyChangeListener(QString property, QVariant oldValue, QVariant newValue);
 void init();
 void addSensorManager(SensorManager* p);
 void addLightManager(LightManager* p);
 void addSignalHeadManager(SignalHeadManager* p);
 void addConditionalManager(ConditionalManager* p);
 void addLogixManager(LogixManager* p);
 void addShutdownManager(ShutDownManager* p);
 void addTabbedPreferences(TabbedPreferences* p);
 void addReporterManager(ReporterManager* p);
 /*public*/ static /*synchronized*/ void addPropertyChangeListener(PropertyChangeListener* l);
 friend class RosterFrame;
};

#endif // INSTANCEMANAGER_H
