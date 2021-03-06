#include "abstractproxyturnoutmanager.h"
#include "exceptions.h"
#include <QSet>
#include "proxyreportermanager.h"
#include "proxyturnoutmanager.h"
#include "proxysensormanager.h"
#include "proxylightmanager.h"
#include "abstractturnoutmanager.h"
#include "abstractsensormanager.h"
#include "abstractlightmanager.h"
#include "abstractreportermanager.h"
#include "lnturnoutmanager.h"
#include "loggerfactory.h"
#include "namedbeancomparator.h"
#include "defaultsystemconnectionmemo.h"

// NOTE: This class is a repklacement for AbstractProxyManager that
// implements TurnoutManager instead of AbstractManager

AbstractProxyTurnoutManager::AbstractProxyTurnoutManager(QObject *parent)
    : TurnoutManager(new DefaultSystemConnectionMemo(), parent)
{
 mgrs = QList<Manager*>();
 internalManager = nullptr;
 defaultManager = nullptr;
 addedOrderList = QStringList();
 //log->setDebugEnabled(true);
 propertyListenerList = QVector<PropertyChangeListener*>();
 namedPropertyListenerMap = QMap<QString, QVector<PropertyChangeListener*>*>();
 propertyVetoListenerList = QVector<VetoableChangeListener*>();
 namedPropertyVetoListenerMap = QMap<QString, QVector<VetoableChangeListener*>*>();

 if(log == nullptr)
  log = LoggerFactory::getLogger("AbstractProxyManager");
 //registerSelf();
}
/**
 * Implementation of a Manager that can serves as a proxy
 * for multiple system-specific implementations.
 * <p>
 * Automatically includes an Internal system, which
 * need not be separately added any more.
 * <p>
 * Encapsulates access to the "Primary" manager,
 * used by default.
 * <p>
 * Internally, this is done by using a list of all
 * non-Internal managers, plus a separate reference to the
 * internal manager.
 *
 * @author	Bob Jacobsen Copyright (C) 2003, 2010
 * @version	$Revision: 18422 $
 */
//abstract public class AbstractProxyManager implements Manager {

/**
 * Number of managers available through
 * getManager(i) and getManagerList(),
 * including the Internal manager
 */
/*protected*/ int AbstractProxyTurnoutManager::nMgrs() const
{
 // make sure internal present
 initInternal();
 return mgrs.size();
}

/*protected*/ Manager* AbstractProxyTurnoutManager::getMgr(int index) const
{
 // make sure internal present
 initInternal();

 if (index < mgrs.size())
  return mgrs.value(index);
 else
 {
  throw  IllegalArgumentException("illegal index "+QString::number(index));
 }
}

/**
 * Returns a list of all managers, including the
 * internal manager.  This is not a live list.
 */
/*public*/ QList<Manager*> AbstractProxyTurnoutManager::getManagerList() const
{
 // make sure internal present
 initInternal();
 QList<Manager*> retval = QList<Manager*>(mgrs);
 return retval;
}

/**
 * Returns a list of all managers, with the default
 * at the start and internal default at the end.
 *
 * @return the list of managers
 */
/*public*/ QList<Manager*> AbstractProxyTurnoutManager::getDisplayOrderManagerList() const {
    // make sure internal present
    initInternal();

    QList<Manager*> retval =  QList<Manager*>();
    if (defaultManager != nullptr) { retval.append(defaultManager); }
    foreach (Manager* manager, mgrs) {
        if (manager != defaultManager && manager != internalManager) {
            retval.append(manager);
        }
    }
    if (internalManager != nullptr && internalManager != defaultManager) {
        retval.append(internalManager);
    }
    return retval;
}

/*public*/ Manager* AbstractProxyTurnoutManager::getInternalManager() const {
    initInternal();
    return internalManager;
}

/**
 * Returns the set default or, if not present, the internal manager as defacto default
 */
/*public*/ Manager* AbstractProxyTurnoutManager::getDefaultManager() const {
    if (defaultManager != nullptr) return defaultManager;

    return getInternalManager();
}

/*public*/ void AbstractProxyTurnoutManager::addManager(Manager* m)
{
 // check for already present
 if (mgrs.contains(m))
 {
  // already present, complain and skip
  log->warn(tr("Manager already present: %1").arg(m->metaObject()->className()));
  return;
 }
 mgrs.append(static_cast<AbstractManager*>(m));

 if (defaultManager == nullptr) defaultManager = m;  // 1st one is default

 //propertyVetoListenerList.stream().forEach((l) ->
 foreach(VetoableChangeListener* l, propertyVetoListenerList)
 {
     m->addVetoableChangeListener(l);
 }//);
 //propertyListenerList.stream().forEach((l) ->
 foreach(PropertyChangeListener* l, propertyListenerList)
 {
     m->addPropertyChangeListener(l);
 }//);
 //namedPropertyVetoListenerMap.entrySet().forEach((e) ->
 QMapIterator<QString, QVector<VetoableChangeListener*>*> e(namedPropertyVetoListenerMap);
 while(e.hasNext())
 {
  e.next();
//     e.getValue().forEach((l) ->
  foreach(VetoableChangeListener* l, *e.value())
  {
      m->addVetoableChangeListener(e.key(), l);
  }//);
 }//);
 //namedPropertyListenerMap.entrySet().forEach((e) ->
 QMapIterator<QString, QVector<PropertyChangeListener*>*> e1(namedPropertyListenerMap);
 while(e1.hasNext())
 {
  e1.next();
     //e.getValue().forEach((l) ->
  foreach(PropertyChangeListener* l, *e1.value())
     {
         m->addPropertyChangeListener(e1.key(), l);
     }//);
 }//);

 m->addDataListener(this);
 recomputeNamedBeanSet();

 if (log->isDebugEnabled())
 {
     log->debug(QString("added manager ") + QString(m->metaObject()->className()));
 }
}

/*private*/ Manager* AbstractProxyTurnoutManager::initInternal() const
{
 if (internalManager == nullptr) {
  if(log)
     log->debug("create internal manager when first requested");
  internalManager = makeInternalManager();
 }
 return internalManager;
}

/*abstract protected*/ Manager* AbstractProxyTurnoutManager::makeInternalManager() const
{
 return nullptr;
}

/**
 * Locate via user name, then system name if needed.
 * Subclasses use this to provide e.g. getSensor, getTurnout, etc
 * via casts.
 *
 * @param name
 * @return Null if nothing by that name exists
 */
/*public*/ NamedBean* AbstractProxyTurnoutManager::getNamedBean(QString name) const
{
 NamedBean* t = getByUserName(name);
 if (t != nullptr) return t;
 return getBySystemName(name);
}

/**
 * Enforces, and as a user convenience converts to, the standard form for a system name
 * for the NamedBeans handled by this manager and its submanagers.
 * <p>
 * Attempts to match by system prefix first.
 * <p>
 *
 * @param inputName System name to be normalized
 * @throws NamedBean.BadSystemNameException If the inputName can't be converted to normalized form
 * @return A system name in standard normalized form
 */
//@Override
//@CheckReturnValue
/*public*/ /*@Nonnull*/ QString AbstractProxyTurnoutManager::normalizeSystemName(/*@Nonnull*/ QString inputName) const /*throw (NamedBean::BadSystemNameException)*/ {
    int index = matchTentative(inputName);
    if (index >= 0) {
        return getMgr(index)->normalizeSystemName(inputName);
    }
    log->debug("normalizeSystemName did not find manager for name " + inputName + ", defer to default");
    return getMgr(0)->normalizeSystemName(inputName);
}
#if 1
/**
 * Locate via user name, then system name if needed.
 * If that fails, create a new NamedBean: If the name
 * is a valid system name, it will be used for the new
 * NamedBean.  Otherwise, the makeSystemName method
 * will attempt to turn it into a valid system name.
 * Subclasses use this to provide e.g. getSensor, getTurnout, etc
 * via casts.
 *
 * @param name
 * @return Never nullptr under normal circumstances
 */
/*protected*/ NamedBean* AbstractProxyTurnoutManager::provideNamedBean(QString name) const
{
 NamedBean* t = getNamedBean(name);
 if (t!=nullptr) return t;
 // Doesn't exist. If the systemName was specified, find that system
 int index = matchTentative(name);
 if (index >= 0)
  return makeBean(index, name, "");
 log->debug("Did not find manager for name "+name+", defer to default");
//    int iI = nMgrs()-1;
//    return makeBean(iI,getMgr(iI)->makeSystemName(name), "");
 QString newSysName = getMgr(0)->makeSystemName(name);
 return makeBean(0,newSysName,"");
}
#else
/**
 * Locate via user name, then system name if needed. If that fails, create a
 * new NamedBean: If the name is a valid system name, it will be used for
 * the new NamedBean. Otherwise, the makeSystemName method will attempt to
 * turn it into a valid system name. Subclasses use this to create provider methods such as
 * getSensor or getTurnout via casts.
 *
 * @param name the user name or system name of the bean
 * @return an existing or new NamedBean
 * @throws IllegalArgumentException if name is not usable in a bean
 */
/*protected*/ NamedBean* AbstractProxyTurnoutManager::provideNamedBean(QString name) throw IllegalArgumentException {
    // make sure internal present
    initInternal();

    NamedBean* t = getNamedBean(name);
    if (t != nullptr) {
        return t;
    }
    // Doesn't exist. If the systemName was specified, find that system
    int index = matchTentative(name);
    if (index >= 0) {
        return makeBean(index, name, "");
    }
    log->debug(tr("provideNamedBean did not find manager for name %1, defer to default").arg(name)); // NOI18N
    return makeBean(mgrs.entryIndex(getDefaultManager()), getDefaultManager()->makeSystemName(name), null);
}
#endif
/**
 * Defer creation of the proper type to the subclass
 * @param index Which manager to invoke
 */
/*abstract protected*/ NamedBean* AbstractProxyTurnoutManager::makeBean(int /*index*/, QString /*systemName*/, QString /*userName*/) const
{
// Q_UNUSED(index);
// Q_UNUSED(systemName);
// Q_UNUSED(userName);
//// if(qobject_cast<ProxyReporterManager*>(this)!=NULL)
////  return ProxyReporterManager::makeBean(index, systemName, userName);

 return nullptr; // Should not get here! this should be overriden by the actual class
}
/** {@inheritDoc} */
//@Override
//@CheckReturnValue
//@CheckForNull
/*public*/ Turnout* AbstractProxyTurnoutManager::getBySystemName(/*@Nonnull*/ QString systemName) const {
    // System names can be matched to managers by system and type at front of name
    int index = matchTentative(systemName);
    if (index >= 0) {
        Manager* m = getMgr(index);
        return (Turnout*)((AbstractTurnoutManager*)m)->getBySystemName(systemName);
    }
    log->debug(tr("getBySystemName did not find manager from name %1, defer to default manager").arg(systemName.isEmpty()?"(NULL)":systemName)); // NOI18N
    return (Turnout*)((AbstractTurnoutManager*)getDefaultManager())->getBySystemName(systemName);
}

/** {@inheritDoc} */
//@Override
//@CheckReturnValue
//@CheckForNull
/*public*/ Turnout *AbstractProxyTurnoutManager::getByUserName(/*@Nonnull*/ QString userName) const{
    for (Manager* m : this->mgrs) {
        Turnout* b = ((AbstractTurnoutManager*)m)->getByUserName(userName);
        if (b != nullptr) {
            return b;
        }
    }
    return nullptr;
}
//@Override
/*public*/ NamedBean* AbstractProxyTurnoutManager::getBeanBySystemName(QString systemName) const
{
 //NamedBean* t = nullptr;
 foreach (Manager* m, this->mgrs) {
     NamedBean* b = m->getBeanBySystemName(systemName);
     if (b != nullptr) {
         return b;
     }
 }
 return nullptr;
}

//@Override
/*public*/ NamedBean* AbstractProxyTurnoutManager::getBeanByUserName(QString userName) const
{
 foreach (Manager* m, this->mgrs) {
     NamedBean* b = m->getBeanByUserName(userName);
     if (b != nullptr) {
         return b;
     }
 }
 return nullptr;
}


/**
 * Return an instance with the specified system and user names.
 * Note that two calls with the same arguments will get the same instance;
 * there is only one Sensor object representing a given physical turnout
 * and therefore only one with a specific system or user name.
 *<P>
 * This will always return a valid object reference for a valid request;
 * a new object will be
 * created if necessary. In that case:
 *<UL>
 *<LI>If a NULL reference is given for user name, no user name will be associated
 *    with the NamedBean object created; a valid system name must be provided
 *<LI>If a NULL reference is given for the system name, a system name
 *    will _somehow_ be inferred from the user name.  How this is done
 *    is system specific.  Note: a future extension of this interface
 *    will add an exception to signal that this was not possible.
 *<LI>If both names are provided, the system name defines the
 *    hardware access of the desired turnout, and the user address
 *    is associated with it.
 *</UL>
 * Note that it is possible to make an inconsistent request if both
 * addresses are provided, but the given values are associated with
 * different objects.  This is a problem, and we don't have a
 * good solution except to issue warnings.
 * This will mostly happen if you're creating NamedBean when you should
 * be looking them up.
 * @return requested NamedBean object (never NULL)
 */
/*public*/ NamedBean* AbstractProxyTurnoutManager::newNamedBean(QString systemName, QString userName) const {
 // make sure internal present
 initInternal();

    // if the systemName is specified, find that system
    int i = matchTentative(systemName);
    if (i >= 0)
        return makeBean(i, systemName, userName);

    // did not find a manager, allow it to default to the primary
    log->debug("Did not find manager for system name "+systemName+", delegate to primary");
    int iI = nMgrs()-1;
    return makeBean(iI, systemName, userName);
    //return makeBean(mgrs.entryIndex(getDefaultManager()), systemName, userName);

}

/*public*/ void AbstractProxyTurnoutManager::dispose() {
    for (int i=0; i<mgrs.size(); i++)
        mgrs.value(i)->dispose();
    mgrs.clear();
    if (internalManager != nullptr) internalManager->dispose(); // don't make if not made yet
}

/**
 * Find the index of a matching manager.
 * Returns -1 if there is no match, which is not considered an
 * error
 */
/*protected*/ int AbstractProxyTurnoutManager::matchTentative(QString systemname) const
{
 //Q_ASSERT(mgrs->count()> 0);
 for (int i = 0; i<nMgrs(); i++)
 {
  Manager* mgr = getMgr(i);
#ifdef QT_DEBUG
  QString pfx = ((AbstractManager*)mgr)->getSystemPrefix();
  char tl = ((AbstractProxyTurnoutManager*)mgr)->typeLetter();
#endif
  if ( systemname.startsWith(((AbstractManager*)mgr)->getSystemPrefix()+((AbstractProxyTurnoutManager*)mgr)->typeLetter()))
  {
   return i;
  }
 }
 return -1;
}

/**
 * Find the index of a matching manager.
 * Throws IllegalArgumentException if there is no match,
 * here considered to be an error that must be reported.
 */
/*protected*/ int AbstractProxyTurnoutManager::match(QString systemname) const{
    int index = matchTentative(systemname);
    if (index < 0) throw  IllegalArgumentException("System name "+systemname+" failed to match");
    return index;
}

/**
 * Shared method to create a systemName based on the address base, the prefix and manager class.
 *
 * @param curAddress base address to use
 * @param prefix system prefix to use
 * @param managerType BeanType manager (method is used for Turnout and Sensor Managers)
 * @return a valid system name for this connection
 * @throws JmriException if systemName cannot be created
 */
QString AbstractProxyTurnoutManager::createSystemName(QString curAddress, QString prefix, QString managerType)const throw (JmriException)
{
 for (Manager* m : mgrs)
 {
  if (prefix == (m->getSystemPrefix()) && managerType == (/*m->getClass()*/ "Turnout")) {
      try {
          if (managerType == "TurnoutManager") {
           return ((TurnoutManager*) m)->createSystemName(curAddress, prefix);
       } else if (managerType == "SensorManager") {
           return ((SensorManager*) m)->createSystemName(curAddress, prefix);
       } else {
           log->warn("createSystemName requested for incompatible Manager");
       }
   } catch (JmriException ex) {
       throw ex;
   }
  }
 }
 throw Exception("Manager could not be found for System Prefix " + prefix);
}

/*public*/ QString AbstractProxyTurnoutManager::getNextValidAddress(/*@Nonnull*/ QString curAddress, /*@Nonnull*/ QString prefix, char typeLetter) throw (JmriException) {
    for (Manager* m : mgrs) {
        log->debug(tr("NextValidAddress requested for %1").arg(curAddress));
        if (prefix == (m->getSystemPrefix()) && typeLetter == m->typeLetter()) {
            try {
                switch (typeLetter) { // use #getDefaultManager() instead?
                    case 'T':
                        return ((TurnoutManager*) m)->getNextValidAddress(curAddress, prefix);
                    case 'S':
                        return ((SensorManager*) m)->getNextValidAddress(curAddress, prefix);
                    case 'R':
                        return ((ReporterManager*) m)->getNextValidAddress(curAddress, prefix);
                    default:
                        return QString();
                }
            } catch (JmriException ex) {
                throw ex;
            }
        }
    }
    return QString();
}

/** {@inheritDoc} */
//@Override
/*public*/ void AbstractProxyTurnoutManager::deleteBean(/*@Nonnull*/ NamedBean* s, /*@Nonnull*/ QString property) throw (PropertyVetoException) {
    QString systemName = s->getSystemName();
    try {
        getMgr(match(systemName))->deleteBean(s, property);
    } catch (PropertyVetoException e) {
        throw e;
    }
}

/**
 * Remember a NamedBean Object created outside the manager.
 * <P>
 * Forwards the register request to the matching system
 */
/*public*/ void AbstractProxyTurnoutManager::Register(NamedBean* s) const {
    QString systemName = s->getSystemName();
    getMgr(match(systemName))->Register(s);
}

/**
 * Forget a NamedBean Object created outside the manager.
 * <P>
 * Forwards the deregister request to the matching system
 */
/*public*/ void AbstractProxyTurnoutManager::deregister(NamedBean* s) const {
    QString systemName = s->getSystemName();
    getMgr(match(systemName))->deregister(s);
}

/*public synchronized*/ void AbstractProxyTurnoutManager::addPropertyChangeListener(PropertyChangeListener* l)
{
 if (!propertyListenerList.contains(l)) {
     propertyListenerList.append(l);
 }
 for (int i = 0; i<nMgrs(); i++)
 {
  Manager* mgr = getMgr(i);
  mgr->addPropertyChangeListener(l);
  connect(((AbstractManager*)mgr)->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), l, SLOT(propertyChange(PropertyChangeEvent*)));
 }
}
/*public synchronized*/ void AbstractProxyTurnoutManager::removePropertyChangeListener(PropertyChangeListener* l)
{
 if (propertyListenerList.contains(l)) {
     propertyListenerList.removeOne(l);
 }
 for (int i = 0; i<nMgrs(); i++)
 {
  Manager* mgr = getMgr(i);
  mgr->addPropertyChangeListener(l);
  disconnect(((AbstractManager*)mgr)->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), l, SLOT(propertyChange(PropertyChangeEvent*)));
 }
}

/** {@inheritDoc} */
//@Override
//@OverridingMethodsMustInvokeSuper
/*public*/ void AbstractProxyTurnoutManager::addPropertyChangeListener(QString propertyName, PropertyChangeListener* listener) {
    if (!namedPropertyListenerMap.contains(propertyName)) {
        namedPropertyListenerMap.insert(propertyName, new QVector<PropertyChangeListener*>());
    }
    if (!namedPropertyListenerMap.value(propertyName)->contains(listener)) {
        namedPropertyListenerMap.value(propertyName)->append(listener);
    }
    for (Manager/*<E>*/* m : mgrs) {
        m->addPropertyChangeListener(propertyName, listener);
    }
}

/** {@inheritDoc} */
//@Override
//@OverridingMethodsMustInvokeSuper
/*public*/ QVector<PropertyChangeListener*> AbstractProxyTurnoutManager::getPropertyChangeListeners() {
    QList<PropertyChangeListener*> listeners = QList<PropertyChangeListener*>(propertyListenerList.toList());
    for (QVector<PropertyChangeListener*>* list : namedPropertyListenerMap.values()) {
        //listeners.addAll(list);
     foreach(PropertyChangeListener* listener, *list)
      listeners.append(listener);
    }
    return listeners.toVector();//(new PropertyChangeListener[listeners.size()]);
}

/** {@inheritDoc} */
//@Override
//@OverridingMethodsMustInvokeSuper
/*public*/ QVector<PropertyChangeListener*> AbstractProxyTurnoutManager::getPropertyChangeListeners(QString propertyName) {
    if (!namedPropertyListenerMap.contains(propertyName)) {
        namedPropertyListenerMap.insert(propertyName, new QVector<PropertyChangeListener*>());
    }
    QVector<PropertyChangeListener*>* listeners = namedPropertyListenerMap.value(propertyName);
    return *listeners;//(new PropertyChangeListener[listeners.size()]);
}

/** {@inheritDoc} */
//@Override
//@OverridingMethodsMustInvokeSuper
/*public*/ void AbstractProxyTurnoutManager::removePropertyChangeListener(QString propertyName, PropertyChangeListener* listener) {
    if (!namedPropertyListenerMap.contains(propertyName)) {
        namedPropertyListenerMap.insert(propertyName, new QVector<PropertyChangeListener*>());
    }
    namedPropertyListenerMap.value(propertyName)->removeOne(listener);
    for (Manager/*<E>*/* m : mgrs) {
        m->removePropertyChangeListener(propertyName, listener);
    }
}

/**
 * @return The system-specific prefix letter for the primary implementation
 */
/*public*/ QString AbstractProxyTurnoutManager::getSystemPrefix() const {
//    try {
      return getMgr(0)->getSystemPrefix();
//        } catch(IndexOutOfBoundsException* ie) {
//          return "?";
//        }
}

/**
 * @return The type letter for turnouts
 */
/*public*/ char AbstractProxyTurnoutManager::typeLetter() const
{
 return getDefaultManager()->typeLetter();
}

/**
 * @return A system name from a user input, typically a number,
 * from the primary system.
 */
/*public*/ QString AbstractProxyTurnoutManager::makeSystemName(QString s) const {
    return getMgr(0)->makeSystemName(s);
}

/** {@inheritDoc} */
//@CheckReturnValue
//@Override
/*public*/ int AbstractProxyTurnoutManager::getObjectCount() {
    int count = 0;
    for (Manager/*<E>*/* m : mgrs) { count += m->getObjectCount(); }
    return count;
}

/*public*/ QStringList AbstractProxyTurnoutManager::getSystemNameArray() {
    //TreeSet<QString> ts = new TreeSet<String>(new SystemNameComparator());
    QSet<QString>* ts = new QSet<QString>();
    for (int i = 0; i<nMgrs(); i++) {
        //ts->add( getMgr(i).getSystemNameList() );
        QStringList snl = getMgr(i)->getSystemNameList();
        foreach(QString s, snl)
         ts->insert(s);
    }
//        QStringList* arr;// = new String[ts.size()];
//        Iterator<QString> it = ts.iterator();
//        int i=0;
//        while(it.hasNext()) {
//            arr[i++] = it.next();
//        }
    QStringList arr = (ts->toList());
    return arr;
}

/**
 * Get a list of all system names.
 */
/*public*/ QStringList AbstractProxyTurnoutManager::getSystemNameList() {
    //TreeSet<QString> ts = new TreeSet<String>(new SystemNameComparator());
    QSet<QString>* ts = new QSet<QString>();
    for (int i = 0; i<nMgrs(); i++) {
        //ts.addAll(getMgr(i).getSystemNameList());
        QStringList snl = getMgr(i)->getSystemNameList();
        foreach(QString s, snl)
         ts->insert(s);
    }
    //return new ArrayList<String>(ts);
    QStringList arr = ts->toList();
    return arr;
}

/*protected*/ void AbstractProxyTurnoutManager::updateOrderList() {
    if (addedOrderList.isEmpty()) return; // only maintain if requested
    addedOrderList.clear();
    for (Manager* m : mgrs) {
        //addedOrderList.addAll(m.getSystemNameAddedOrderList());
     for(QString name : m->getSystemNameAddedOrderList() )
      addedOrderList.append(name);
    }
}

/** {@inheritDoc} */
//@Override
//@Deprecated  // will be removed when Manager method is removed due to @Override
/*public*/ QStringList AbstractProxyTurnoutManager::getSystemNameAddedOrderList() {
    addedOrderList = QStringList();  // need to start maintaining it
    updateOrderList();
    //return Collections.unmodifiableList(addedOrderList);
    return addedOrderList;
}

/** {@inheritDoc} */
//@Override
//@Deprecated  // will be removed when Manager method is removed due to @Override
//@Nonnull
/*public*/ QList<NamedBean*>* AbstractProxyTurnoutManager::getNamedBeanList() {
    // by doing this in order by manager and from each managers ordered sets, its finally in order
    QList<NamedBean*>* tl = new QList<NamedBean*>();
    for (Manager*  m : mgrs) {
        //tl.addAll(m.getNamedBeanSet());
     foreach(NamedBean* bean, m->getNamedBeanSet())
      tl->append(bean);
    }
    //return Collections.unmodifiableList(tl);
    return tl;
}

/*protected*/ void AbstractProxyTurnoutManager::recomputeNamedBeanSet() {
 if (namedBeanSet == nullptr)
  return; // only maintain if requested
 namedBeanSet->clear();
 for (Manager/*<E>*/* m : mgrs) {
     namedBeanSet->unite(m->getNamedBeanSet());
 }
}

NamedBeanComparator<NamedBean*> comparator2 = NamedBeanComparator<NamedBean*>();

bool sortLessThanconst2( NamedBean* s1,  NamedBean* s2)
{
 return comparator2.compare(s1, s2) < 0;
}

/** {@inheritDoc} */
//@Override
//@Nonnull
/*public*/ /*SortedSet<E>*/QSet<NamedBean*> AbstractProxyTurnoutManager::getNamedBeanSet() {
 if (namedBeanSet == nullptr) {
     namedBeanSet = new QSet<NamedBean*>();//new TreeSet<>(new NamedBeanComparator<>());
     recomputeNamedBeanSet();
 }

 //return Collections.unmodifiableSortedSet(namedBeanSet);
 QList<NamedBean*> list = namedBeanSet->toList();
 qSort(list.begin(), list.end(), sortLessThanconst2); //NamedBeanComparator<NamedBean*>::compare);
 return list.toSet();
}

/**
 * Get a list of all user names.
 */
///*public*/ QStringList AbstractProxyTurnoutManager::getUserNameList()
//{
// //TreeSet<QString> ts = new TreeSet<String>(new SystemNameComparator());
// QSet<QString>* ts = new QSet<QString>();
// for (int i = 0; i<nMgrs(); i++)
// {
//  //ts.addAll(getMgr(i).getSystemNameList());
//  QStringList snl = getMgr(i)->getSystemNameList();
//  foreach(QString s, snl)
//  {
//   AbstractNamedBean* bean = (AbstractNamedBean*)getMgr(i)->getBeanBySystemName(s);
//   QString userName = bean->getUserName();
//   if(userName != "")
//    ts->insert(userName);
//  }
// }
// //return new ArrayList<String>(ts);
// QStringList arr = ts->toList();
// return arr;
//}

//void AbstractProxyTurnoutManager::propertyChange(PropertyChangeEvent */*e*/)
//{
// //emit this->propertyChange(e);
//}
// initialize logging
/*private*/ /*final*/ /*static*/ Logger* AbstractProxyTurnoutManager::log = LoggerFactory::getLogger("AbstractProxyTurnoutManager");
