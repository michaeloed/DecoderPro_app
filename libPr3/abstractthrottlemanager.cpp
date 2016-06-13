#include "abstractthrottlemanager.h"
#include "locoaddress.h"
#include "Roster/rosterentry.h"
#include "loconetthrottle.h"
#include "Throttle/throttlewindow.h"
#include "addresspanel.h"
#include "speedprofilepanel.h"
#include "abstractautomaton.h"

/**
 * Abstract implementation of a ThrottleManager.
 * <P>
 * Based on Glen Oberhauser's original LnThrottleManager implementation.
 *
 * @author	Bob Jacobsen  Copyright (C) 2001
 * @version     $Revision: 22484 $
 */

AbstractThrottleManager::AbstractThrottleManager(QObject *parent)
    : ThrottleManager(parent)
{
    this->parent = parent;
    log = new Logger("AbstractThrottleManager");
    log->setDebugEnabled(true);
    throttleListeners = new QHash<DccLocoAddress*,  QList<WaitingThrottle*>* >();
    listenerOnly = new QHash<DccLocoAddress*,QList<WaitingThrottle*>* >();
    addressThrottles = new QHash<DccLocoAddress*,Addresses*>();
    userName = "Internal";
}
//abstract public class AbstractThrottleManager implements ThrottleManager {


    //public AbstractThrottleManager(){}

/*public*/ AbstractThrottleManager::AbstractThrottleManager(SystemConnectionMemo* memo,QObject* parent) : ThrottleManager(parent)
{
    this->parent = parent;
    log = new Logger("AbstractThrottleManager");
    log->setDebugEnabled(true);
    adapterMemo = memo;
    userName = "Internal";
    throttleListeners = new QHash<DccLocoAddress*,QList<WaitingThrottle*>* >();
    listenerOnly = new QHash<DccLocoAddress*,QList<WaitingThrottle*>* >();
    addressThrottles = new QHash<DccLocoAddress*,Addresses*>();
}


/*public*/ QString AbstractThrottleManager::getUserName(){
    if(adapterMemo!=NULL)
        return adapterMemo->getUserName();
    return userName;
}

/**
 * By default, only DCC in this implementation
 */
/*public*/ QStringList AbstractThrottleManager::getAddressTypes(){
QStringList list;
list << "dcc" <<"dcc_short" << "dcc_long";
 return list;
}

/**
 * By default, only DCC in this implementation
 */
/*public*/ QString AbstractThrottleManager::getAddressTypeString(LocoAddress::Protocol prot){
    return LocoAddress::getPeopleName(prot);
}

/*public*/ QList<LocoAddress::Protocol> AbstractThrottleManager::getAddressProtocolTypes(){
    QList<LocoAddress::Protocol> list;
    list << LocoAddress::DCC<< LocoAddress::DCC_SHORT<< LocoAddress::DCC_LONG;
    return list;
}

/*public*/ LocoAddress* AbstractThrottleManager::getAddress(QString value, LocoAddress::Protocol protocol)
{
 if (value == NULL) return NULL;
 //if (protocol == NULL) return NULL;
 int num = value.toInt();

 // if DCC long and can't be, or short and can't be, fix
 if ((LocoAddress::DCC == protocol || LocoAddress::DCC_SHORT == protocol) && !canBeShortAddress(num))
  protocol = LocoAddress::DCC_LONG;
 if ((LocoAddress::DCC == protocol || LocoAddress::DCC_LONG == protocol) && !canBeLongAddress(num))
  protocol = LocoAddress::DCC_SHORT;

 // if still ambiguous, prefer short
 if (protocol == LocoAddress::DCC)
  protocol = LocoAddress::DCC_SHORT;

 return new DccLocoAddress(num,protocol);
}
/*public*/ LocoAddress* AbstractThrottleManager::getAddress(QString value, QString protocol) {
    if (value == NULL) return NULL;
    if (protocol == NULL) return NULL;
    LocoAddress::Protocol p = getProtocolFromString(protocol);

    return getAddress(value, p);
}

/*public*/ LocoAddress::Protocol AbstractThrottleManager::getProtocolFromString(QString selection){
    return  LocoAddress::getByPeopleName(selection);
}

/**
 * Does this DCC system allow a Throttle (e.g. an address) to be used
 * by only one user at a time?
 */
/*protected*/ bool AbstractThrottleManager::singleUse() { return true; }

/*public*/ bool AbstractThrottleManager::requestThrottle(BasicRosterEntry* re, ThrottleListener* l){
    return requestThrottle(((RosterEntry*)re)->getDccLocoAddress(), re, l);
}

/*public*/ bool AbstractThrottleManager::requestThrottle(int address, bool isLongAddress, ThrottleListener* l) {
    DccLocoAddress* la = new DccLocoAddress(address, isLongAddress);
    return requestThrottle(la, NULL, l);
}

/*public*/ bool AbstractThrottleManager::requestThrottle(DccLocoAddress* la, ThrottleListener* l){
    return requestThrottle(la, NULL, l);
}

/**
 * Request a throttle, given a decoder address. When the decoder address
 * is located, the ThrottleListener gets a callback via the ThrottleListener.notifyThrottleFound
 * method.
 * @param la DccLocoAddress of the decoder desired.
 * @param l The ThrottleListener awaiting notification of a found Throttle::
 * @return True if the request will continue, false if the request will not
 * be made. False may be returned if a the throttle is already in use.
 */
/*public*/ bool AbstractThrottleManager::requestThrottle(DccLocoAddress* la, BasicRosterEntry* re, ThrottleListener* l)
{
 bool throttleFree = true;

 // put the list in if not present
 if (!throttleListeners->contains(la))
  throttleListeners->insert(la,  new QList<WaitingThrottle*>());
 // get the corresponding list to check length
 QList<WaitingThrottle*>* a = throttleListeners->value(la);

 if (addressThrottles->contains(la))
 {
  log->debug("A throttle to address " + QString("%1").arg(la->getNumber()) + " already exists, so will return that throttle");
  a->append(new WaitingThrottle(l, re));
  notifyThrottleKnown(addressThrottles->value(la)->getThrottle(), la);
  return throttleFree;
 }
 else
 {
  log->debug(QString("%1").arg(la->getNumber()) + " has not been created before");
 }

 if (log->isDebugEnabled()) log->debug("After request in ATM: "+QString::number(a->size()));
 // check length

 if (singleUse() && (a->size()>0))
 {
  throttleFree= false;
  if (log->isDebugEnabled()) log->debug("case 1");
 }
 else if (a->size() == 0)
 {
  a->append(new WaitingThrottle(l, re));
  if (log->isDebugEnabled()) log->debug("case 2: "+la->toString()+";"/*+a*/);
  requestThrottleSetup(la, true);
 }
 else
 {
  a->append(new WaitingThrottle(l, re));
  if (log->isDebugEnabled()) log->debug("case 3");
 }
 return throttleFree;
}

/**
 * Request a throttle, given a decoder address. When the decoder address
 * is located, the ThrottleListener gets a callback via the ThrottleListener.notifyThrottleFound
 * method.
 * <P>
 * This is a convenience version of the call, which uses system-specific
 * logic to tell whether the address is a short or long form.
 * @param address The decoder address desired.
 * @param l The ThrottleListener awaiting notification of a found Throttle::
 * @return True if the request will continue, false if the request will not
 * be made. False may be returned if a the throttle is already in use.
 */
/*public*/ bool AbstractThrottleManager::requestThrottle(int address, ThrottleListener* l) {
    bool isLong = true;
    //if (canBeShortAddress(address)) isLong = false;
    return requestThrottle(address, isLong, l);
}

/**
 * Abstract member to actually do the work of configuring a new throttle,
 * usually via interaction with the DCC system
 */
///*abstract public*/ void AbstractThrottleManager::requestThrottleSetup(LocoAddress* a, bool control);

 /**
 * Abstract member to actually do the work of configuring a new throttle,
 * usually via interaction with the DCC system
 */
/*public*/ void AbstractThrottleManager::requestThrottleSetup(LocoAddress* a)
{
 requestThrottleSetup(a, true);
}

/**
 * Cancel a request for a throttle
 * @param address The decoder address desired.
 * @param isLong True if this is a request for a DCC long (extended) address.
 * @param l The ThrottleListener cancelling request for a Throttle::
 */
/*public*/ void AbstractThrottleManager::cancelThrottleRequest(int address, bool isLong, ThrottleListener* l) {
    if(throttleListeners!=NULL){
        DccLocoAddress* la = new DccLocoAddress(address, isLong);
        cancelThrottleRequest(la, l);
    }
    /*if (addressThrottles.contains(la)){
        addressThrottles.get(la).decrementUse();
    }*/
}

/*public*/ void AbstractThrottleManager::cancelThrottleRequest(BasicRosterEntry* re, ThrottleListener* l){
    if(throttleListeners!=NULL){
        cancelThrottleRequest(re->getDccLocoAddress(), l);
    }
}

/*private*/ void AbstractThrottleManager::cancelThrottleRequest(DccLocoAddress* la, ThrottleListener* l)
{
 if (throttleListeners != NULL)
 {
#if 1 // TODO:
  QList<WaitingThrottle*>* a = throttleListeners->value(la);
  if (a->isEmpty()) return;
  for (int i = 0; i<a->size(); i++)
  {
   if (l == a->value(i)->getListener())
    a->removeAt(i);
  }
#endif
 }
}

/**
 * Cancel a request for a Throttle::
 * <P>
 * This is a convenience version of the call, which uses system-specific
 * logic to tell whether the address is a short or long form.
 * @param address The decoder address desired.
 * @param l The ThrottleListener cancelling request for a Throttle::
 */
/*public*/ void AbstractThrottleManager::cancelThrottleRequest(int address, ThrottleListener* l) {
    bool isLong = true;
   // if (canBeShortAddress(address)) isLong = false;
    cancelThrottleRequest(address, isLong, l);
}

/**
 * If the system-specific ThrottleManager has been unable to create the DCC
 * throttle then it needs to be removed from the throttleListeners, otherwise
 * any subsequent request for that address results in the address being reported
 * as already in use, if singleUse is set->
 * This also sends a notification message back to the requestor with a string
 * reason as to why the request has failed.
 * @param address The DCC Loco Address that the request failed on.
 * @param reason A text string passed by the ThrottleManae as to why
 */
/*public*/ void AbstractThrottleManager::failedThrottleRequest(DccLocoAddress* address, QString reason)
{
 QList<WaitingThrottle*>* a = throttleListeners->value(address);
 if (a->isEmpty())
 {
     log->warn("notifyThrottleKnown with zero-length listeners: "+address->toString());
 }
 else
 {
  for (int i = 0; i<a->size(); i++)
  {
   ThrottleListener* l = a->at(i)->getListener();
   l->notifyFailedThrottleRequest(address, reason);
  }
 }
 throttleListeners->remove(address);
 QList<WaitingThrottle*>* p = listenerOnly->value(address);
 if (p->isEmpty())
 {
  log->debug("notifyThrottleKnown with zero-length PropertyChange listeners: "+address->toString());
 }
 else
 {
  for (int i = 0; i<p->size(); i++)
  {
   PropertyChangeListener* l = p->value(i)->getPropertyChangeListener();
   l->propertyChange(new PropertyChangeEvent(this, "attachFailed", address, NULL));
  }
 }
 listenerOnly->remove(address);
}

/**
 * Handle throttle information when it's finally available, e.g. when
 * a new Throttle object has been created.
 * <P>
 * This method creates a throttle for all ThrottleListeners of that address
 * and notifies them via the ThrottleListener.notifyThrottleFound method.
 */
/*public*/ void AbstractThrottleManager::notifyThrottleKnown(DccThrottle* throttle, LocoAddress* addr)
{
#if 1 // TODO:
 log->debug("notifyThrottleKnown for "+((DccLocoAddress*)addr)->toString());
 DccLocoAddress* dla = (DccLocoAddress*) addr;
 int nbr = dla->getNumber();
// QHashIterator<DccLocoAddress*,Addresses*> it(*addressThrottles);
// while(it.hasNext())
// {
//  it.next();
//  DccLocoAddress* key = it.key();
//  if(key->getNumber() == dla->getNumber()) =
//  {
//   dla = key;
//   break;
//  }
// }
 Addresses* ads = NULL;
 if (!addressThrottles->contains(dla))
 {
  log->debug("Address " + dla->toString() + "doesn't already exists so will add");
  ads = new Addresses(throttle);
  addressThrottles->insert(dla, ads);
 }
 else
 {
  addressThrottles->value(dla)->setThrottle(throttle);
 }

//QList<WaitingThrottle*>* a = throttleListeners->value(dla);
 QList<WaitingThrottle*>* a = NULL;
 QHashIterator<DccLocoAddress*, QList<WaitingThrottle*>* > it(*throttleListeners);
 while(it.hasNext())
 {
  it.next();
  DccLocoAddress* addr = it.key();
  if((addr->getNumber() == dla->getNumber()) && (addr->getProtocol() == dla->getProtocol()))
  {
   a = it.value();
   break;
  }
 }
 if (a->isEmpty())
 {
  log->debug("notifyThrottleKnown with zero-length listeners: "+dla->toString());
 }
 else
 {
  for (int i = 0; i<a->size(); i++)
  {
   ThrottleListener* l = a->at(i)->getListener();
   log->debug("Notify listener");
   //((ThrottleWindow*)l)->notifyThrottleFound(throttle);
   addressThrottles->value(dla)->incrementUse();
   addressThrottles->value(dla)->addListener(l);
   if(ads!=NULL && a->at(i)->getRosterEntry()!=NULL && ((LocoNetThrottle*)throttle)->getRosterEntry()==NULL)
   {
    ((LocoNetThrottle*)throttle)->setRosterEntry(a->at(i)->getRosterEntry());
   }
   if(qobject_cast<ThrottleWindow*>(l)!=NULL)
    ((ThrottleWindow*)l)->notifyThrottleFound(throttle);
   else if(qobject_cast<AddressPanel*>(l)!=NULL)
    ((AddressPanel*)l)->notifyThrottleFound(throttle);
   else if(qobject_cast<SpeedProfilePanel*>(l) != NULL)
    ((SpeedProfilePanel*)l)->notifyThrottleFound(throttle);
   else if(qobject_cast<AbstractAutomaton*>(l) != NULL)
    ((AbstractAutomaton*)l)->notifyThrottleFound(throttle);
   else
   {
    log->error(tr("unknown Throttle listener %1").arg(l->metaObject()->className()));
    Q_ASSERT(false);
   }
  }
  throttleListeners->remove(dla);
 }


 QList<WaitingThrottle*>* p = listenerOnly->value(dla);
 if (p==NULL || p->isEmpty())
 {
  log->debug("notifyThrottleKnown with zero-length propertyChangeListeners: "+dla->toString());
 }
 else
 {
  for (int i = 0; i<p->size(); i++)
  {
   PropertyChangeListener* l = p->at(i)->getPropertyChangeListener();
   log->debug("Notify propertyChangeListener");
   l->propertyChange(new PropertyChangeEvent(this, "throttleAssigned", NULL, dla));
   if(ads!=NULL && p->value(i)->getRosterEntry()!=NULL && throttle->getRosterEntry()==NULL)
   {
    ((LocoNetThrottle*)throttle)->setRosterEntry(p->value(i)->getRosterEntry());
   }
  ((LocoNetThrottle*)throttle)->addPropertyChangeListener(l);
 }
 listenerOnly->remove(dla);
 }
#endif
}

/**
 * Check to see if the Dispatch Button should be enabled or not
 * Default to true, override if necessary
 **/
/*public*/ bool AbstractThrottleManager::hasDispatchFunction() { return true; }

/**
 * What speed modes are supported by this system?
 * value should be xor of possible modes specifed by the
 * DccThrottle interface
 */
/*public*/ int AbstractThrottleManager::supportedSpeedModes() {
    return(DccThrottle::SpeedStepMode128);
}

/*public*/ void AbstractThrottleManager::attachListener(BasicRosterEntry* re, PropertyChangeListener* p){
    attachListener(re->getDccLocoAddress(), re, p);
}

/*public*/ void AbstractThrottleManager::attachListener(DccLocoAddress* la, PropertyChangeListener* p){
    attachListener(la, NULL, p);
}

/*public*/ void AbstractThrottleManager::attachListener(DccLocoAddress* la, BasicRosterEntry* re, PropertyChangeListener* p)
{

 if (addressThrottles->contains(la))
 {
  addressThrottles->value(la)->getThrottle()->addPropertyChangeListener(p);
  p->propertyChange(new PropertyChangeEvent(this, "throttleAssigned", NULL, la));
  return;
 }
 else
 {
  if (!listenerOnly->contains(la))
   listenerOnly->insert(la, new QList<WaitingThrottle*>());

  // get the corresponding list to check length
  QList<WaitingThrottle*>* a = listenerOnly->value(la);
  a->append(new WaitingThrottle(p, re));
  //Only request that the throttle is set up if it hasn't already been
  //requested.
  if ((!throttleListeners->contains(la)) && (a->size()==1))
  {
   requestThrottleSetup(la, false);
  }
 }

}

/*public*/ void AbstractThrottleManager::removeListener(DccLocoAddress* la, PropertyChangeListener* p)
{
#if 1 // TODO:
 if (addressThrottles->contains(la))
 {
  addressThrottles->value(la)->getThrottle()->removePropertyChangeListener(p);
  p->propertyChange(new PropertyChangeEvent(this, "throttleRemoved", la, NULL));
  return;
 }
 p->propertyChange(new PropertyChangeEvent(this, "throttleNotFoundInRemoval", la, NULL));
#endif
}

/*public*/ bool AbstractThrottleManager::addressStillRequired(DccLocoAddress* la)
{
#if 1 // TODO:
 if (addressThrottles->contains(la))
 {
  log->debug("usage count is " + QString("%1").arg(addressThrottles->value(la)->getUseCount()));
  if(addressThrottles->value(la)->getUseCount()>0)
  {
   return true;
  }
 }
#endif
    return false;
}

/*public*/ void AbstractThrottleManager::releaseThrottle(DccThrottle* t, ThrottleListener* l)
{
 disposeThrottle(t, l);
}

/*public*/ bool AbstractThrottleManager::disposeThrottle(DccThrottle* t, ThrottleListener* l)
{
//        if (!active) log->error("Dispose called when not active");  <-- might need to control this in the sub class
 if(t == NULL) return false;
#if 1 // TODO:
 DccLocoAddress* la = (DccLocoAddress*) t->getLocoAddress();
 if (addressReleased(la, l))
 {
  log->debug("Address " + QString::number(((DccLocoAddress*)t->getLocoAddress())->getNumber()) + " still has active users");
  return false;
 }
 if(t->getListeners()->size()>0)
 {
  log->debug("Throttle " + QString::number(((DccLocoAddress*)t->getLocoAddress())->getNumber()) + " still has active propertyChangeListeners registered to the throttle");
  return false;
 }
 if (addressThrottles->contains(la))
 {
  addressThrottles->remove(la);
  log->debug("Loco Address removed from the stack " + QString::number(la->getNumber()));
 }
 else
 {
  log->debug("Loco Address not found in the stack " + QString::number(la->getNumber()));
 }
#endif
 return true;
}

/*public*/ void AbstractThrottleManager::dispatchThrottle(DccThrottle* t, ThrottleListener* l){
    releaseThrottle(t, l);
}

/*protected*/ bool AbstractThrottleManager::addressReleased(DccLocoAddress* la, ThrottleListener* l)
{
#if 1 // TODO:

 if (addressThrottles->contains(la))
 {
  if(addressThrottles->value(la)->containsListener(l))
  {
   log->debug("decrementUse called with listener " + l->objectName());
   addressThrottles->value(la)->decrementUse();
   addressThrottles->value(la)->removeListener(l);
  }
  else if (l==NULL)
  {
   log->debug("decrementUse called withOUT listener");
            /*The release release has been called, but as no listener has
            been specified, we can only decrement the use flag*/
   addressThrottles->value(la)->decrementUse();
  }
 }
 if (addressThrottles->contains(la))
 {
  if(addressThrottles->value(la)->getUseCount()>0)
  {
   return true;
  }
 }
#endif
 return false;
}

/*public*/ QVariant AbstractThrottleManager::getThrottleInfo(DccLocoAddress* la, QString item)
{
 DccThrottle* t;
 LocoNetThrottle* ln;
 if (addressThrottles->contains(la))
 {
  t = addressThrottles->value(la)->getThrottle();
  ln = (LocoNetThrottle*)t;
 }
 else
 {
     return QVariant();
 }
 if (item==("IsForward"))
 {
  bool b;
  b = ln->getIsForward();
  return b;
 }
 else if(item.startsWith("Speed"))
 {
  if (item==("SpeedSetting"))
  {
   float f = ln->getSpeedSetting();
   return f;
  }
  else
  {
   if (item==("SpeedIncrement"))
   {
    float f = ln->getSpeedIncrement();
    return f;
   }
   else if (item==("SpeedStepMode"))
   {
    int i =  ln->getSpeedStepMode();
    return i;
   }
  }
 }
 else if (item==(Throttle::F0))
 {
  bool b = ln->getF0();
  return b;
 }
 else if(item.startsWith(Throttle::F1))
 {
  if (item==(Throttle::F1))
  {
   bool b = ln->getF1();\
   return b;
  }
  else if (item==(Throttle::F10))
  {
   bool b = ln->getF10();\
   return b;
  }
  else if (item==(Throttle::F11))
  {
   bool b = ln->getF11();\
   return b;
  }
  else if (item==(Throttle::F12))
  {
   bool b = ln->getF12();\
   return b;
  }
  else if (item==(Throttle::F13))
  {
   bool b = ln->getF13();\
   return b;
  }
  else if (item==(Throttle::F14))
  {
   bool b = ln->getF14();\
   return b;
  }
  else if (item==(Throttle::F15))
  {
   bool b = ln->getF15();\
   return b;
  }
  else if (item==(Throttle::F16))
  {
   bool b = ln->getF16();\
   return b;
  }
  else if (item==(Throttle::F17))
  {
   bool b = ln->getF17();\
   return b;
  }
  else if (item==(Throttle::F18))
  {
   bool b = ln->getF18();\
   return b;
  }
  else if (item==(Throttle::F19))
  {
   bool b = ln->getF19();\
   return b;
  }
 }
 else if(item.startsWith(Throttle::F2))
 {
  if (item==(Throttle::F2))
  {
   bool b = ln->getF2();\
   return b;
  }
  else if (item==(Throttle::F20))
  {
   bool b = ln->getF20();\
   return b;
  }
  else if (item==(Throttle::F21))
  {
   bool b = ln->getF21();\
   return b;
  }
  else if (item==(Throttle::F22))
  {
   bool b = ln->getF22();\
   return b;
  }
  else if (item==(Throttle::F23))
  {
   bool b = ln->getF23();\
   return b;
  }
  else if (item==(Throttle::F24))
  {
   bool b = ln->getF24();\
   return b;
  }
  else if (item==(Throttle::F25))
  {
   bool b = ln->getF25();\
   return b;
  }
  else if (item==(Throttle::F26))
  {
   bool b = ln->getF26();\
   return b;
  }
  else if (item==(Throttle::F27))
  {
   bool b = ln->getF27();\
   return b;
  }
  else if (item==(Throttle::F28))
  {
   bool b = ln->getF28();\
   return b;
  }
 }
 else if (item==(Throttle::F3))
 {
  bool b =  ln->getF3();
  return b;
 }
  else if (item==(Throttle::F4))
  {
   bool b =  ln->getF4();
   return b;
  }
  else if (item==(Throttle::F5))
  {
   bool b =  ln->getF5();
   return b;
  }
  else if (item==(Throttle::F6))
  {
   bool b =  ln->getF6();
   return b;
  }
  else if (item==(Throttle::F7))
  {
   bool b =  ln->getF7();
   return b;
  }
  else if (item==(Throttle::F8))
  {
   bool b =  ln->getF8();
   return b;
  }
  else if (item==(Throttle::F9))
  {
   bool b =  ln->getF9();
   return b;
  }
 return QVariant();
}

 WaitingThrottle::WaitingThrottle(ThrottleListener* _l, BasicRosterEntry* _re)
 {
  setObjectName("WaitingThrottle");
  pl=NULL;
  l = _l;
  re = _re;
 }
 WaitingThrottle::WaitingThrottle(PropertyChangeListener* _pl, BasicRosterEntry* _re)
 {
  setObjectName("WaitingThrottle");
  l=NULL;
  pl = _pl;
  re = _re;
 }

 PropertyChangeListener* WaitingThrottle::getPropertyChangeListener()
 {
  return pl;
 }

 ThrottleListener* WaitingThrottle::getListener()
 {
  return l;
 }

 BasicRosterEntry* WaitingThrottle::getRosterEntry()
 {
  return re;
 }

/**
 * This subClass, keeps track of which loco address have been requested and
 * by whom, it primarily uses a increment count to keep track of all the the
 * Addresses in use as not all external code will have been refactored over
 * to use the new disposeThrottle::
 */
  /*protected*/ Addresses::Addresses(DccThrottle* throttle)
  {
   setObjectName("Addresses");
   this->throttle = throttle;
   re = NULL;
   useActiveCount = 0;
   listeners = new QList<ThrottleListener*>();
  }
  void Addresses::incrementUse()
  {
   useActiveCount++;
   if(qobject_cast<LocoNetThrottle*>(throttle)!= NULL)
   {
    LocoNetThrottle* lnThrottle = (LocoNetThrottle*)throttle;
    log.debug(QString::number(((DccLocoAddress*) lnThrottle->getLocoAddress())->getNumber()) + " increased Use Size to " + QString::number(useActiveCount));
   }
  }

  void Addresses::decrementUse()
  {
   //Do want to go below 0 on the usage front!
   if (useActiveCount >0)
    useActiveCount--;
   if(qobject_cast<LocoNetThrottle*>(throttle)!= NULL)
   {
    LocoNetThrottle* lnThrottle = (LocoNetThrottle*)throttle;
    log.debug(QString::number(((DccLocoAddress*) lnThrottle->getLocoAddress())->getNumber()) + " decreased Use Size to " + QString::number(useActiveCount));
   }
  }

  int Addresses::getUseCount() { return useActiveCount; }

  DccThrottle* Addresses::getThrottle()
  {
   return throttle;
  }

  void Addresses::setThrottle(DccThrottle* throttle)
  {
   DccThrottle* old = this->throttle;
   this->throttle = throttle;
   if ((old==NULL) || (old==throttle))
   {
    return;
   }

   //As the throttle has changed, we need to inform the listeners
   //However if a throttle hasn't used the new code, it will not have been
   //removed and will get a notification.
   if(qobject_cast<LocoNetThrottle*>(throttle)!= NULL)
   {
    LocoNetThrottle* lnThrottle = (LocoNetThrottle*)throttle;
    log.debug(QString::number(((DccLocoAddress*) lnThrottle->getLocoAddress())->getNumber()) + " throttle assigned " +
                 "has been changed need to notify throttle users");
   }
   this->throttle = throttle;
   for (int i = 0; i<listeners->size(); i++)
   {
    ThrottleListener* l = listeners->value(i);
    if(qobject_cast<ThrottleWindow*>(l)!= NULL)
     ((ThrottleWindow*)l)->notifyThrottleFound(throttle);
   }
   //This handles moving the listeners from the old throttle to the new one
   DccLocoAddress* la = dynamic_cast<DccLocoAddress*>(this->throttle->getLocoAddress());
   QVector<PropertyChangeListener*>* v = old->getListeners();
   foreach (PropertyChangeListener* prop , *v)
   {
    if(qobject_cast<LocoNetThrottle*>(throttle)!= NULL)
     ((LocoNetThrottle*)this->throttle)->addPropertyChangeListener(prop);
#if 1 //TODO:
     prop->propertyChange(new PropertyChangeEvent((QObject*)this, "throttleAssignmentChanged", QString(""), VPtr<DccLocoAddress>::asQVariant(la)));
#endif
   }
  }

  void Addresses::setRosterEntry(BasicRosterEntry* _re)
  {
   re = _re;
  }

  BasicRosterEntry* Addresses::getRosterEntry()
  {
   return re;
  }

  void Addresses::addListener(ThrottleListener* l){
         // Will need to do a check for duplication here
         listeners->append(l);
  }

  void Addresses::removeListener(ThrottleListener* l){

         listeners->removeAt(listeners->indexOf(l));
  }

  bool Addresses::containsListener(ThrottleListener* l){
         return listeners->contains(l);
  }

//static org.apache.log4j.Logger log = org.apache.log4j.Logger.getLogger(AbstractThrottleManager.class.getName());
