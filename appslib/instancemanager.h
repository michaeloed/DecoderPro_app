#ifndef INSTANCEMANAGER_H
#define INSTANCEMANAGER_H

#include "appslib_global.h"
#include <QObject>
#include <QHash>
#include <QString>
#include <QList>
#include <QVector>
#include "logger.h"
//#include "timebase.h"
#include "powermanager.h"
#include "sensormanager.h"
//#include "configuremanager.h"
#include "programmermanager.h"
//#include "clockcontrol.h"
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
//#include "consistmanager.h"
#include "dccconsistmanager.h"
#include "instanceinitializer.h"
//#include "commandstation.h"
#include "abstractproxymanager.h"
#include "proxyreportermanager.h"
#include "proxysensormanager.h"
#include "throttlemanager.h"
#include "proxyturnoutmanager.h"
#include "proxylightmanager.h"
#include "defaultinstanceinitializer.h"
#include "../libPr3/routemanager.h"
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
/*private*/ enum InitializationState {
    NOTSET, // synonymous with no value for this stored
    NOTSTARTED,
    STARTED,
    FAILED,
    DONE
};

/*static*/ /*private*/ /*final*/ class StateHolder {

   InitializationState state;
   Exception* exception;
public:
   StateHolder(InitializationState state, Exception* exception) {
       this->state = state;
       this->exception = exception;
   }
   friend class InstanceManager;
};

class SignalSpeedMap;
class PrintWriter;
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
class APPSLIBSHARED_EXPORT InstanceManager : public QObject
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
 //template<class T>
 static /*public*/  void store(/*@Nonnull*/ QObject* item, /*@Nonnull Class<T> */ QString type);
//static void storeBefore( int index, QObject* item, QString type);

/**
 * Retrieve a list of all objects of type T that were
 * registered with {@link #store}.
 * @param type The class Object for the items' type.
 */
//static public <T> List<Object> getList(Class<T> type)
//template<class T>
static QObjectList *getList(QString type);
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
//template<class T>
//static <T> T getDefault(class<T> type);
static /*public*/ QObject* getNullableDefault(QString type);
static /*public*/ QObject* getOptionalDefault(QString type);
/*public*/ static QString getDefaultsPropertyName(QString clazz);
/*public*/ static QString getListPropertyName(/*Class<?>*/QString clazz);
/*public*/ static InstanceManager* getDefault();
/*public*/ QObject* getInstance(/*@Nonnull Class<T>*/QString type);


/**
 * Set an object of type T as the default for that type.
 *<p>
 * Also registers (stores) the object if not already present.
 *<p>
 * Now, we do that moving the item to the back of the list;
 * see the {@link #getDefault} method
 */
//static public <T> void setDefault(Class<T> type, T val)
static QObject* setDefault(QString type, QObject* val);
/**
 * Dump generic content of InstanceManager
 * by type.
 */
static QString contentsToString();
Q_DECL_DEPRECATED static PowerManager* powerManagerInstance();
//Q_DECL_DEPRECATED static void setPowerManager(PowerManager* p);
//Q_DECL_DEPRECATED static Timebase* timebaseInstance();
//Q_DECL_DEPRECATED static ConfigureManager* configureManagerInstance();
//Q_DECL_DEPRECATED static void setConfigureManager(ConfigureManager* p);

static SensorManager* sensorManagerInstance();
static TurnoutManager* turnoutManagerInstance();
static LightManager* lightManagerInstance();
Q_DECL_DEPRECATED static ProgrammerManager* programmerManagerInstance();
//static void setProgrammerManager(ProgrammerManager* p);
static ThrottleManager* throttleManagerInstance();
static void setTurnoutManager(TurnoutManager* p);

//Q_DECL_DEPRECATED static SignalHeadManager* signalHeadManagerInstance();
//Q_DECL_DEPRECATED static SignalMastManager* signalMastManagerInstance();
//static void setSignalMastManager(SignalMastManager* p);
//Q_DECL_DEPRECATED static SignalSystemManager* signalSystemManagerInstance();
//static void setSignalSystemManager(SignalSystemManager* p);
//Q_DECL_DEPRECATED static SignalGroupManager* signalGroupManagerInstance() ;
//static void setSignalGroupManager(SignalGroupManager* p);
//Q_DECL_DEPRECATED static ConsistManager* consistManagerInstance();
static void setConsistManager(ConsistManager* p);
Q_DECL_DEPRECATED static BlockManager* blockManagerInstance();
Q_DECL_DEPRECATED static SectionManager* sectionManagerInstance();
//static SensorManager* sensorManager;
//static InstanceManager* instance();
static SignalMastLogicManager* signalMastLogicManagerInstance();
static /*public*/  bool containsDefault(/*@Nonnull*/ QString type);

Q_DECL_DEPRECATED static RouteManager* routeManagerInstance();

//static /*public*/ LayoutBlockManager* layoutBlockManagerInstance();
//Q_DECL_DEPRECATED static ConditionalManager* conditionalManagerInstance();
//Q_DECL_DEPRECATED static LogixManager* logixManagerInstance();

//Q_DECL_DEPRECATED static TransitManager* transitManagerInstance();
//Q_DECL_DEPRECATED static ShutDownManager* shutDownManagerInstance();
//Q_DECL_DEPRECATED static TabbedPreferences* tabbedPreferencesInstance();
//Q_DECL_DEPRECATED static ClockControl* clockControlInstance();
//Q_DECL_DEPRECATED static void addClockControl(ClockControl* cc);
//Q_DECL_DEPRECATED static CommandStation* commandStationInstance();

Q_DECL_DEPRECATED static ReporterManager* reporterManagerInstance();
static MemoryManager* memoryManagerInstance();
//Q_DECL_DEPRECATED static AudioManager* audioManagerInstance();
//static RosterIconFactory* rosterIconFactoryInstance();
//Q_DECL_DEPRECATED static CatalogTreeManager* catalogTreeManagerInstance();
static void setSensorManager(SensorManager* p);
static void setLightManager(LightManager* p);
static void setThrottleManager(ThrottleManager* p);
Q_DECL_DEPRECATED static void setSignalHeadManager(SignalHeadManager* p);
//static /*public*/ void setLayoutBlockManager(LayoutBlockManager* p);
//static void setConditionalManager(ConditionalManager* p);
//static void setLogixManager(LogixManager* p);
//Q_DECL_DEPRECATED static void setShutDownManager(ShutDownManager* p);
//static void setTabbedPreferences(TabbedPreferences* p);
QT_DEPRECATED static void setCommandStation(CommandStation* p);
QT_DEPRECATED static /*public*/ void setAddressedProgrammerManager(AddressedProgrammerManager* p);
static void setReporterManager(ReporterManager* p);
static void removePropertyChangeListener(PropertyChangeListener* l);
//template<class T>
/*public*/ /*<T>*/ QObjectList* getInstances(/*@Nonnull Class<T>*/ QString type);
//template<class T>
/*public*/  void clear(/*@Nonnull*/ /*Class<T>*/QString type);
/*public*/ /*<T>*/ void remove(/*@Nonnull T*/QObject* item, /*@Nonnull Class<T>*/QString type);


signals:
  void propertyChange(PropertyChangeEvent*);

public slots:

private:
 QHash<QString,QObjectList*> managerLists;
 /*private*/ /*final*/ QMap</*Class<?>*/QString, QObject*> initializers;// = new HashMap<>();
 PropertyChangeSupport* pcs;
 /*private*/ /*final*/ QMap</*Class<?>*/QString, StateHolder*> initState;// = new HashMap<>();
 //Logger* log;
 /*private*/ /*final*/ static Logger* log;// = LoggerFactory::getLogger("InstanceManager");
 //static void setRootInstance();
 //static ConfigureManager* configureManager;
 ///*private*/ ClockControl* clockControl;// = NULL;
 //SectionManager* sectionManager;
 //TransitManager* transitManager;
 //TurnoutManager* turnoutManager;
 //LightManager* lightManager;
 // data members to hold contact with the property listeners
 static QVector<PropertyChangeListener*> listeners;
 //CatalogTreeManager* catalogTreeManager;
 //AudioManager* audioManager;
 //MemoryManager* memoryManager;
 //RosterIconFactory* rosterIconFactory;
 //Timebase* timebase;
 //SignalHeadManager* signalHeadManager;
 //ConditionalManager* conditionalManager;
 //ReporterManager* reporterManager;
 //ShutDownManager* shutdownManager;
 //LogixManager* logixManager;

 //TabbedPreferences* tabbedPreferencesManager;
 //VSDecoderManager vsdecoderManager;
 //static InstanceInitializer* initializer;// = new jmri.managers.DefaultInstanceInitializer();
 static QMutex mutex;
 // support checking for overlapping intialization
 /*private*/ void setInitializationState(QString type, InitializationState state);
 /*private*/ InitializationState getInitializationState(QString type) ;
 /*private*/ Exception* getInitializationException(QString type);
 // support creating a file with initialization summary information
 /*private*/ static /*final*/ bool traceFileActive;// = log.isTraceEnabled(); // or manually force true
 /*private*/ static /*final*/ bool traceFileAppend;// = false; // append from run to run
 /*private*/ int traceFileIndent = 1; // used to track overlap, but note that threads are parallel
 /*private*/ static /*final*/ QString traceFileName;// = "instanceManagerSequence.txt";  // use a standalone name
 /*private*/ static PrintWriter* traceFileWriter;
 /*private*/ void traceFilePrint(QString msg);

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

 //void addConfigureManager(ConfigureManager* p);
 //void addTurnoutManager(TurnoutManager* p);
 void notifyPropertyChangeListener(QString property, QVariant oldValue, QVariant newValue);
 //void init();
 //void addSensorManager(SensorManager* p);
 //void addLightManager(LightManager* p);
// void addSignalHeadManager(SignalHeadManager* p);
// void addConditionalManager(ConditionalManager* p);
// void addLogixManager(LogixManager* p);
// void addShutdownManager(ShutDownManager* p);
// void addTabbedPreferences(TabbedPreferences* p);
 //void addReporterManager(ReporterManager* p);
 // /*public*/ static /*synchronized*/ void addPropertyChangeListener(PropertyChangeListener* l);
 friend class RosterFrame;
 friend class SignalSpeedMap;
};

/**
 * A class for lazy initialization of the singleton class InstanceManager.
 * https://www.ibm.com/developerworks/library/j-jtp03304/
 */
/*private*/ /*static*/ class LazyInstanceManager {
public:
    /*public*/ static InstanceManager* instanceManager;// = new InstanceManager();
};
#endif // INSTANCEMANAGER_H