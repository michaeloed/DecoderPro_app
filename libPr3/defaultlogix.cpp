#include "defaultlogix.h"
#include "logger.h"
#include "exceptions.h"
#include "instancemanager.h"
#include "jmrisimplepropertylistener.h"
#include "logix.h"
#include "conditional.h"
#include "conditionalvariable.h"
#include "signalhead.h"
#include "abstractlightmanager.h"
#include "signalmast.h"
#include "defaultsignalmastmanager.h"
#include "abstractmemorymanager.h"
#include "jmritwostatepropertylistener.h"
#include "jmriclockpropertylistener.h"
#include "jmrimultistatepropertylistener.h"
#include "defaultconditional.h"
#include "defaultconditionalmanager.h"
#include "../Tables/lroutetableaction.h"
#include "abstractsignalheadmanager.h"
#include "simpletimebase.h"
#include "oblock.h"
#include "oblockmanager.h"
#include "entryexitpairs.h"
#include "warrantmanager.h"
#include "warrant.h"
#include "abstractnamedbean.h"

//DefaultLogix::DefaultLogix(QObject *parent) :
//    AbstractNamedBean(parent)
//{
//}
/**
 * Class providing the basic logic of the Logix interface.
 *
 * @author	Dave Duchamp Copyright (C) 2007
 * @version     $Revision: 21308 $
 * @author Pete Cressman Copyright (C) 2009
 */
///*public*/ class DefaultLogix extends AbstractNamedBean
//    implements Logix, java.io.Serializable {

/*public*/ DefaultLogix::DefaultLogix(QString systemName, QString userName, QObject *parent) :
    Logix(systemName, userName, parent)
{
 //super(systemName, userName);
 log = new Logger("DefaultLogix");
 _conditionalSystemNames = new QStringList();
 _listeners = new QList<JmriSimplePropertyListener*>();

 /**
  *  Operational instance variables (not saved between runs)
  */
 mEnabled = true;

 _isActivated = false;
}

///*public*/ DefaultLogix(String systemName) {
//    super(systemName);
//}


/**
 * Get number of Conditionals for this Logix
 */
/*public*/ int DefaultLogix::getNumConditionals() {
    return _conditionalSystemNames->size();
}

/**
 * Move 'row' to 'nextInOrder' and shift all between 'row' and 'nextInOrder'
 * up one position   ( row > nextInOrder )
 */
/*public*/ void DefaultLogix::swapConditional(int nextInOrder, int row) {
    if (row <= nextInOrder) {
        return;
    }
    QString temp = _conditionalSystemNames->at(row);
    for (int i = row; i > nextInOrder; i--)
    {
        _conditionalSystemNames->replace(i, _conditionalSystemNames->at(i-1));
    }
    _conditionalSystemNames->replace(nextInOrder, temp);
}

/**
 * Returns the system name of the conditional that will calculate in the
 * specified order. This is also the order the Conditional is listed in
 * the Add/Edit Logix dialog->
 * If 'order' is greater than the number of Conditionals for this Logix,
 * and empty String is returned.
 * @param order - order in which the Conditional calculates.
 */
/*public*/ QString DefaultLogix::getConditionalByNumberOrder(int order) {
    try {
        return _conditionalSystemNames->at(order);
    }
    catch (IndexOutOfBoundsException ioob)
    {
        return "";
    }
}

/**
 * Add a Conditional to this Logix
 * Returns true if Conditional was successfully added, returns false
 * if the maximum number of conditionals has been exceeded.
 * @param systemName The Conditional system name
 * @param order - the order this conditional should calculate in
 *                 if order is negative, the conditional is added
 *				   at the end of current group of conditionals
 */
/*public*/ bool DefaultLogix::addConditional(QString systemName,int /*order*/) {
    _conditionalSystemNames->append(systemName);
    return (true);
}

/**
 * Set enabled status.  Enabled is a bound property
 *   All conditionals are set to UNKNOWN state and recalculated
 *		when the Logix is enabled, provided the Logix has been
 *		previously activated.
 */
/*public*/ void DefaultLogix::setEnabled(bool state) {

    bool old = mEnabled;
    mEnabled = state;
    if (old != state) {
        bool active = _isActivated;
        deActivateLogix();
        activateLogix();
        _isActivated = active;
        for (int i=_listeners->size()-1; i>=0; i--)
        {
            _listeners->at(i)->setEnabled(state);
        }
        firePropertyChange("Enabled", QVariant(old), QVariant(state));
    }
}

/**
 * Get enabled status
*/
/*public*/ bool DefaultLogix::getEnabled() {
    return mEnabled;
}

/**
 * Delete a Conditional and remove it from this Logix
 * <P>
 * Note: Since each Logix must have at least one Conditional to
 *	do anything, the user is warned in Logix Table Action when the
 *  last Conditional is deleted.
 * <P>
 * Returns true if Conditional was successfully deleted, otherwise
 *  returns false.
 * @param systemName The Conditional system name
 */
/*public*/ QStringList* DefaultLogix::deleteConditional(QString systemName) {
    if (_conditionalSystemNames->size()<=0) {
        return (NULL);
    }
    // check other Logix(es) for use of this conditional (systemName) for use as a
    // variable in one of their conditionals
    QStringListIterator iter1 (InstanceManager::logixManagerInstance()->getSystemNameList());
    while (iter1.hasNext()) {
        QString sNameLogix = iter1.next();
        if (sNameLogix!=(getSystemName()) ) {
            Logix* x = InstanceManager::logixManagerInstance()->getBySystemName(sNameLogix);
            int numCond = x->getNumConditionals();
            for (int i=0; i<numCond; i++) {
                QString sNameCond = x->getConditionalByNumberOrder(i);
                Conditional* c = InstanceManager::conditionalManagerInstance()->getBySystemName(sNameCond);
                QList <ConditionalVariable*>* varList = c->getCopyOfStateVariables();
                for (int k=0; k<varList->size(); k++)  {
                    ConditionalVariable* v = varList->at(k);
                    if ( (v->getType()==Conditional::TYPE_CONDITIONAL_TRUE) ||
                         (v->getType()==Conditional::TYPE_CONDITIONAL_FALSE) )
                    {
                        QString name = v->getName();
                        Conditional* c1 = InstanceManager::conditionalManagerInstance()->getConditional(name);
                        if (c1 == NULL) {
                            log->error("\""+name+"\" is a non-existent Conditional variable in Conditional \""
                                      +c->getUserName()+"\" in Logix \""+x->getUserName()+"\" ("+sNameLogix+")");
                        } else {
                            if ( systemName==(c1->getSystemName()) ) {
                                QStringList* result = new QStringList();
                                        *result << name << systemName << c->getUserName() <<
                                                        sNameCond << x->getUserName() << sNameLogix;
                                return result;
                            }
                        }
                    }
                }
            }
        }
    }
    // Remove Conditional from this logix
    int ix = _conditionalSystemNames->indexOf(systemName);
    if (ix < 0) {
        log->error("attempt to delete Conditional not in Logix: "+systemName);
        return NULL;
    }
    _conditionalSystemNames->removeAt(ix);
    // delete the Conditional object
    Conditional* c = ((ConditionalManager*)InstanceManager::conditionalManagerInstance())->getBySystemName(systemName);
    if (c == NULL) {
        log->error("attempt to delete non-existant Conditional - "+systemName);
        return NULL;
    }
    ((ConditionalManager*)InstanceManager::conditionalManagerInstance())->deleteConditional(c);
    return (NULL);
}

/**
 * Calculate all Conditionals, triggering action if the user specified
 *   conditions are met, and the Logix is enabled.
 */
/*public*/ void DefaultLogix::calculateConditionals()
{
 // are there Conditionals to calculate?
     // There are conditionals to calculate
 QString cName = "";
 Conditional* c = NULL;
 for (int i=0; i<_conditionalSystemNames->size(); i++)
 {
  cName = _conditionalSystemNames->at(i);
  c = ((DefaultConditionalManager*)InstanceManager::conditionalManagerInstance())->getBySystemName(cName);
  if (c==NULL)
  {
   log->error("Invalid conditional system name when calculating Logix - "+cName);
  }
  else
  {
   // calculate without taking any action unless Logix is enabled
   ((DefaultConditional*)c)->calculate(mEnabled, NULL);
  }
 }
}

/**
 * Activate the Logix, starts Logix processing by connecting all
 *    inputs that are included the Conditionals in this Logix->
 * <P>
 * A Logix must be activated before it will calculate any of its
 *    Conditionals.
 */
/*public*/ void DefaultLogix::activateLogix() {
    // if the Logix is already busy, simply return
    if (_isActivated) return;
    // set the state of all Conditionals to UNKNOWN
    resetConditionals();
    // assemble a list of needed listeners
    assembleListenerList();
    // create and attach the needed property change listeners
    // start a minute Listener if needed
    for (int i=0; i<_listeners->size(); i++) {
        startListener(_listeners->at(i));
    }
    // mark this Logix as busy
    _isActivated = true;
    // calculate this Logix to set initial state of Conditionals
    calculateConditionals();
}

/*private*/ void DefaultLogix::resetConditionals() {
    ConditionalManager* cm = InstanceManager::conditionalManagerInstance();
    for (int i=0; i<_conditionalSystemNames->size(); i++) {
        Conditional* conditional = ((DefaultConditionalManager*)cm)->getBySystemName(_conditionalSystemNames->at(i));
        if (conditional!=NULL) {
            try {
                ((DefaultConditional*)conditional)->setState(Conditional::UNKNOWN);
            } catch ( JmriException e) {
                  // ignore
            }
        }
    }
}

/**
 * Assembles a list of Listeners needed to activate this Logix
 */
/*private*/ void DefaultLogix::assembleListenerList() {
    // initialize
    for (int i=_listeners->size()-1; i>=0; i--)
    {
        removeListener(_listeners->at(i));
    }
    _listeners = new QList<JmriSimplePropertyListener*>();
    // cycle thru Conditionals to find objects to listen to
    ConditionalManager* cm = InstanceManager::conditionalManagerInstance();
    for (int i=0; i<_conditionalSystemNames->size(); i++) {
        Conditional* conditional = NULL;
        conditional = ((DefaultConditionalManager*)cm)->getBySystemName(_conditionalSystemNames->at(i));
        if (conditional!=NULL) {
            QList<ConditionalVariable*>* variableList = ((DefaultConditional*)conditional)->getCopyOfStateVariables();
            for (int k = 0; k<variableList->size(); k++) {
                ConditionalVariable* variable = variableList->at(k);
                // check if listening for a change has been suppressed
                int varListenerType = 0;
                QString varName = variable->getName();
                NamedBeanHandle<NamedBean*>* namedBean = variable->getNamedBean();
                int varType = variable->getType();
                int signalAspect = -1;
                // Get Listener type from varible type
                switch(varType) {
                    case Conditional::TYPE_SENSOR_ACTIVE:
                    case Conditional::TYPE_SENSOR_INACTIVE:
                        varListenerType = Logix::LISTENER_TYPE_SENSOR;
                        break;
                    case Conditional::TYPE_TURNOUT_THROWN:
                    case Conditional::TYPE_TURNOUT_CLOSED:
                        varListenerType = Logix::LISTENER_TYPE_TURNOUT;
                        break;
                    case Conditional::TYPE_CONDITIONAL_TRUE:
                    case Conditional::TYPE_CONDITIONAL_FALSE:
                        varListenerType = Logix::LISTENER_TYPE_CONDITIONAL;
                        break;
                    case Conditional::TYPE_LIGHT_ON:
                    case Conditional::TYPE_LIGHT_OFF:
                        varListenerType = Logix::LISTENER_TYPE_LIGHT;
                        break;
                    case Conditional::TYPE_MEMORY_EQUALS:
                    case Conditional::TYPE_MEMORY_COMPARE:
                    case Conditional::TYPE_MEMORY_EQUALS_INSENSITIVE:
                    case Conditional::TYPE_MEMORY_COMPARE_INSENSITIVE:
                        varListenerType = Logix::LISTENER_TYPE_MEMORY;
                        break;
                    case Conditional::TYPE_ROUTE_FREE:
                    case Conditional::TYPE_ROUTE_OCCUPIED:
                    case Conditional::TYPE_ROUTE_ALLOCATED:
                    case Conditional::TYPE_ROUTE_SET:
                    case Conditional::TYPE_TRAIN_RUNNING:
                        varListenerType = Logix::LISTENER_TYPE_WARRANT;
                        break;
                    case Conditional::TYPE_FAST_CLOCK_RANGE:
                        varListenerType = Logix::LISTENER_TYPE_FASTCLOCK;
                        varName = "clock";
                        break;
                    case Conditional::TYPE_SIGNAL_HEAD_RED:
                        varListenerType = Logix::LISTENER_TYPE_SIGNALHEAD;
                        signalAspect = SignalHead::RED;
                        break;
                    case Conditional::TYPE_SIGNAL_HEAD_YELLOW:
                        varListenerType = Logix::LISTENER_TYPE_SIGNALHEAD;
                        signalAspect = SignalHead::YELLOW;
                        break;
                    case Conditional::TYPE_SIGNAL_HEAD_GREEN:
                        varListenerType = Logix::LISTENER_TYPE_SIGNALHEAD;
                        signalAspect = SignalHead::GREEN;
                        break;
                    case Conditional::TYPE_SIGNAL_HEAD_DARK:
                        varListenerType = Logix::LISTENER_TYPE_SIGNALHEAD;
                        signalAspect = SignalHead::DARK;
                        break;
                    case Conditional::TYPE_SIGNAL_HEAD_FLASHRED:
                        varListenerType = Logix::LISTENER_TYPE_SIGNALHEAD;
                        signalAspect = SignalHead::FLASHRED;
                        break;
                    case Conditional::TYPE_SIGNAL_HEAD_FLASHYELLOW:
                        varListenerType = Logix::LISTENER_TYPE_SIGNALHEAD;
                        signalAspect = SignalHead::FLASHYELLOW;
                        break;
                    case Conditional::TYPE_SIGNAL_HEAD_FLASHGREEN:
                        varListenerType = Logix::LISTENER_TYPE_SIGNALHEAD;
                        signalAspect = SignalHead::FLASHGREEN;
                        break;
                    case Conditional::TYPE_SIGNAL_HEAD_LIT:
                    case Conditional::TYPE_SIGNAL_HEAD_HELD:
                        varListenerType = Logix::LISTENER_TYPE_SIGNALHEAD;
                        break;
                    case Conditional::TYPE_SIGNAL_MAST_ASPECT_EQUALS:
                    case Conditional::TYPE_SIGNAL_MAST_LIT:
                    case Conditional::TYPE_SIGNAL_MAST_HELD:
                        varListenerType = Logix::LISTENER_TYPE_SIGNALMAST;
                        break;
                    case Conditional::TYPE_BLOCK_STATUS_EQUALS:
                        varListenerType = Logix::LISTENER_TYPE_OBLOCK;
                        break;
                    case Conditional::TYPE_ENTRYEXIT_ACTIVE:
                    case Conditional::TYPE_ENTRYEXIT_INACTIVE:
                        varListenerType = Logix::LISTENER_TYPE_ENTRYEXIT;
                        break;
                }
                int positionOfListener = getPositionOfListener(varListenerType, varType, varName);
                // add to list if new
                JmriSimplePropertyListener* listener = NULL;
                if (positionOfListener == -1) {
                    switch (varListenerType) {
                        case Logix::LISTENER_TYPE_SENSOR:
                            listener = new JmriTwoStatePropertyListener("KnownState", Logix::LISTENER_TYPE_SENSOR, namedBean, varType, conditional);
                            break;
                        case Logix::LISTENER_TYPE_TURNOUT:
                            listener = new JmriTwoStatePropertyListener("KnownState", Logix::LISTENER_TYPE_TURNOUT,namedBean, varType, conditional);
                            break;
                        case Logix::LISTENER_TYPE_CONDITIONAL:
                            listener = new JmriTwoStatePropertyListener("KnownState", Logix::LISTENER_TYPE_CONDITIONAL, varName, varType, conditional);
                            break;
                        case Logix::LISTENER_TYPE_LIGHT:
                            listener = new JmriTwoStatePropertyListener("KnownState", Logix::LISTENER_TYPE_LIGHT, varName, varType, conditional);
                            break;
                        case Logix::LISTENER_TYPE_MEMORY:
                            listener = new JmriTwoStatePropertyListener("value", Logix::LISTENER_TYPE_MEMORY, namedBean, varType, conditional);
                            break;
                        case Logix::LISTENER_TYPE_WARRANT:
                            listener = new JmriSimplePropertyListener(NULL, Logix::LISTENER_TYPE_WARRANT,
                                                                      varName, varType, conditional);
                            break;
                        case Logix::LISTENER_TYPE_FASTCLOCK:
                            listener = new JmriClockPropertyListener("minutes", Logix::LISTENER_TYPE_FASTCLOCK,
                                                                     varName, varType, conditional,
                                                                variable->getNum1(), variable->getNum2());
                            break;
                        case Logix::LISTENER_TYPE_SIGNALHEAD:
                            if (signalAspect <0) {
                                if (varType == Conditional::TYPE_SIGNAL_HEAD_LIT) {
                                    listener = new JmriTwoStatePropertyListener("Lit", Logix::LISTENER_TYPE_SIGNALHEAD,
                                                                        varName, varType, conditional);
                                } else { // varType == Conditional::TYPE_SIGNAL_HEAD_HELD
                                    listener = new JmriTwoStatePropertyListener("Held", Logix::LISTENER_TYPE_SIGNALHEAD,
                                                                        varName, varType, conditional);
                                }
                            } else {
                                listener = new JmriMultiStatePropertyListener("Appearance", Logix::LISTENER_TYPE_SIGNALHEAD,
                                                                    varName, varType, conditional, signalAspect);
                            }
                            break;
                        case Logix::LISTENER_TYPE_SIGNALMAST:
                            listener = new JmriTwoStatePropertyListener("Aspect", Logix::LISTENER_TYPE_SIGNALMAST,
                                                                varName, varType, conditional);
                            break;
                        case Logix::LISTENER_TYPE_OBLOCK:
                            listener = new JmriTwoStatePropertyListener("state", Logix::LISTENER_TYPE_OBLOCK,
                                                                varName, varType, conditional);
                            break;
                        case Logix::LISTENER_TYPE_ENTRYEXIT:
                            listener = new JmriTwoStatePropertyListener("active", Logix::LISTENER_TYPE_ENTRYEXIT,
                                namedBean, varType, conditional);
                            break;
                        default:
                            if (LRouteTableAction::LOGIX_INITIALIZER !=(varName)) {
                                log->error("Unknown (new) Variable Listener type= "+QString::number(varListenerType)+", for varName= "
                                          +varName+", varType= "+QString::number(varType)+" in Conditional, "+
                                          _conditionalSystemNames->at(i));
                            }
                            continue;
                    }
                    _listeners->append(listener);
                    //log->debug("Add listener for "+varName);
                }
                else {
                    switch (varListenerType) {
                        case Logix::LISTENER_TYPE_SENSOR:
                        case Logix::LISTENER_TYPE_TURNOUT:
                        case Logix::LISTENER_TYPE_CONDITIONAL:
                        case Logix::LISTENER_TYPE_LIGHT:
                        case Logix::LISTENER_TYPE_MEMORY:
                        case Logix::LISTENER_TYPE_WARRANT:
                        case Logix::LISTENER_TYPE_SIGNALMAST:
                        case Logix::LISTENER_TYPE_OBLOCK:
                        case Logix::LISTENER_TYPE_ENTRYEXIT:
                            listener = _listeners->at(positionOfListener);
                            listener->addConditional(conditional);
                            break;

                        case Logix::LISTENER_TYPE_FASTCLOCK:
                    {
                            JmriClockPropertyListener* cpl =
                                    (JmriClockPropertyListener*)_listeners->at(positionOfListener);
                            cpl->setRange(variable->getNum1(), variable->getNum2());
                            cpl->addConditional(conditional);
                            break;
}
                        case Logix::LISTENER_TYPE_SIGNALHEAD:
                    {
                            if (signalAspect < 0) {
                                listener = _listeners->at(positionOfListener);
                                listener->addConditional(conditional);
                            } else {
                                JmriMultiStatePropertyListener* mpl =
                                    (JmriMultiStatePropertyListener*)_listeners->at(positionOfListener);
                                mpl->addConditional(conditional);
                                mpl->setState(signalAspect);
                            }
                            break;
                    }
                        default:
                            log->error("Unknown (old) Variable Listener type= "+QString::number(varListenerType)+", for varName= "
                                      +varName+", varType= "+QString::number(varType)+" in Conditional, "+
                                      _conditionalSystemNames->at(i));
                    }
                }
                // addition listeners needed for memory compare
                if (varType==Conditional::TYPE_MEMORY_COMPARE ||
                    varType==Conditional::TYPE_MEMORY_COMPARE_INSENSITIVE) {
                    positionOfListener = getPositionOfListener(varListenerType, varType,
                                                                   variable->getDataString());
                    if (positionOfListener == -1) {
                        QString name = variable->getDataString();
                        Memory* my = ((AbstractMemoryManager*)InstanceManager::memoryManagerInstance())->provideMemory(name);
                        if (my == NULL) {
                            log->error("invalid memory name= \""+name+"\" in state variable");
                            break;
                        }
                        NamedBeanHandle<NamedBean*>* nb = (NamedBeanHandle<NamedBean*>*)((NamedBeanHandleManager*)InstanceManager::getDefault(
                                    "NamedBeanHandleManager"))->getNamedBeanHandle(name, my);

                        listener = new JmriTwoStatePropertyListener("value", Logix::LISTENER_TYPE_MEMORY,
                                                                  nb, varType,
                                                                  conditional);
                        _listeners->append(listener);
                    } else {
                        listener = _listeners->at(positionOfListener);
                        listener->addConditional(conditional);
                    }
                }
            }
        }
        else {
            log->error("invalid conditional system name in Logix \""+getSystemName()+
                      "\" assembleListenerList DELETING "+
                      _conditionalSystemNames->at(i)+ " from Conditional list." );
            _conditionalSystemNames->removeAt(i);

        }
    }
}

/*private*/ int DefaultLogix::getPositionOfListener(int varListenerType, int varType, QString varName)
{
 // check if already in list
 for (int j=0; (j<_listeners->size()); j++)
 {
  if (varListenerType==_listeners->at(j)->getType() )
  {
   if (varName==(_listeners->at(j)->getDevName()))
   {
    if (varListenerType == Logix::LISTENER_TYPE_SIGNALHEAD)
    {
     if (varType == Conditional::TYPE_SIGNAL_HEAD_LIT ||
                 varType == Conditional::TYPE_SIGNAL_HEAD_HELD )
     {
      if (varType == _listeners->at(j)->getVarType() )
      {
       return j;
      }
     } else if ("Appearance"==(_listeners->at(j)->getPropertyName()))
     {
      // the Appearance Listener can handle all aspects
      return j;
     }
    }
    else
    {
     return j;
    }
   }
  }
 }
 return -1;
}

/**
 * Assembles and returns a list of state variables that are used by conditionals
 *   of this Logix including the number of occurances of each variable that
 *   trigger a calculation, and the number of occurances where the triggering
 *   has been suppressed.
 * The main use of this method is to return information that can be used to test
 *   for inconsistency in suppressing triggering of a calculation among multiple
 *   occurances of the same state variable.
 * Caller provides an ArrayList of the variables to check and and empty Array list
 *   to return the counts for triggering or suppressing calculation.  The first
 *   index is a count that the correspondeing variable triggers calculation and
 *   second is a count that the correspondeing variable suppresses Calculation.
 * Note this method must not modify the supplied variable list in any way.
 *
public void getStateVariableList(ArrayList <ConditionalVariable> varList, ArrayList <int[]> triggerPair) {
    // initialize
    Conditional c = NULL;
    String testSystemName = "";
    String testUserName = "";
    String testVarName = "";
    // cycle thru Conditionals to find state variables
    ConditionalManager cm = InstanceManager::conditionalManagerInstance();
    for (int i=0; i<_conditionalSystemNames.size(); i++) {
        c = cm.getBySystemName(_conditionalSystemNames.get(i));
        if (c!=NULL) {
            ArrayList variableList = c.getCopyOfStateVariables();
            for (int k = 0; k<variableList.size(); k++) {
                ConditionalVariable variable = (ConditionalVariable)variableList.get(k);
                testVarName = variable.getName();
                testSystemName = "";
                testUserName = "";
                // initialize this state variable
                switch (variable.getType()) {
                    case Conditional::TYPE_SENSOR_ACTIVE:
                    case Conditional::TYPE_SENSOR_INACTIVE:
                        Sensor s = InstanceManager::sensorManagerInstance().
                                            getSensor(testVarName);
                        if (s!=NULL) {
                            testSystemName = s.getSystemName();
                            testUserName = s.getUserName();
                        }
                        break;
                    case Conditional::TYPE_TURNOUT_THROWN:
                    case Conditional::TYPE_TURNOUT_CLOSED:
                        Turnout t = InstanceManager::turnoutManagerInstance().
                                            getTurnout(testVarName);
                        if (t!=NULL) {
                            testSystemName = t.getSystemName();
                            testUserName = t.getUserName();
                        }
                        break;
                    case Conditional::TYPE_CONDITIONAL_TRUE:
                    case Conditional::TYPE_CONDITIONAL_FALSE:
                        Conditional cx = InstanceManager::conditionalManagerInstance().
                                            getConditional(this,testVarName);
                        if (cx==NULL) {
                            cx = InstanceManager::conditionalManagerInstance().
                                            getBySystemName(testVarName);
                        }
                        if (cx!=NULL) {
                            testSystemName = cx->getSystemName();
                            testUserName = cx->getUserName();
                        }
                        break;
                    case Conditional::TYPE_LIGHT_ON:
                    case Conditional::TYPE_LIGHT_OFF:
                        Light lgt = InstanceManager::lightManagerInstance().
                                            getLight(testVarName);
                        if (lgt!=NULL) {
                            testSystemName = lgt.getSystemName();
                            testUserName = lgt.getUserName();
                        }
                        break;
                    case Conditional::TYPE_MEMORY_EQUALS:
                        Memory m = InstanceManager::memoryManagerInstance().
                                            getMemory(testVarName);
                        if (m!=NULL) {
                            testSystemName = m.getSystemName();
                            testUserName = m.getUserName();
                        }
                        break;
                    case Conditional::TYPE_SIGNAL_HEAD_RED:
                    case Conditional::TYPE_SIGNAL_HEAD_YELLOW:
                    case Conditional::TYPE_SIGNAL_HEAD_GREEN:
                    case Conditional::TYPE_SIGNAL_HEAD_DARK:
                    case Conditional::TYPE_SIGNAL_HEAD_FLASHRED:
                    case Conditional::TYPE_SIGNAL_HEAD_FLASHYELLOW:
                    case Conditional::TYPE_SIGNAL_HEAD_FLASHGREEN:
                        SignalHead h = InstanceManager::signalHeadManagerInstance().
                                            getSignalHead(testVarName);
                        if (h!=NULL) {
                            testSystemName = h.getSystemName();
                            testUserName = h.getUserName();
                        }
                        break;
                    case Conditional::TYPE_SIGNAL_HEAD_LIT:
                        SignalHead hx = InstanceManager::signalHeadManagerInstance().
                                            getSignalHead(testVarName);
                        if (hx!=NULL) {
                            testSystemName = hx->getSystemName();
                            testUserName = hx->getUserName();
                        }
                        break;
                    case Conditional::TYPE_SIGNAL_HEAD_HELD:
                        SignalHead hy = InstanceManager::signalHeadManagerInstance().
                                            getSignalHead(testVarName);
                        if (hy!=NULL) {
                            testSystemName = hy.getSystemName();
                            testUserName = hy.getUserName();
                        }
                        break;
                    default:
                        testSystemName = "";
                }
                // check if this state variable is already in the list to be returned
                boolean inList = false;
                int indexOfRepeat = -1;
                if (testSystemName!="") {
                    // getXXXXXX succeeded, process this state variable
                    for (int j=0; j<varList.size(); j++)  {
                        ConditionalVariable v = varList.get(j);
                        if ( v.getName()==(testSystemName) || v.getName()==(testUserName) ) {
                            inList = true;
                            indexOfRepeat = j;
                            break;
                        }
                    }
                    // add to list if new and if there is room
                    if ( inList ) {
                        int[] trigs = triggerPair.get(indexOfRepeat);
                        if ( variable.doCalculation() ) {
                            trigs[0]++;
                        }
                        else {
                            trigs[1]++;

                        }
                    }
                }
            }
        }
        else {
            log->error("invalid conditional system name in Logix getStateVariableList - "+
                                                        _conditionalSystemNames.get(i));

        }
    }
}       // getStateVariableList
*/
/**
 * Deactivate the Logix-> This method disconnects the Logix from
 *    all input objects and stops it from being triggered to calculate.
 * <P>
 * A Logix must be deactivated before it's Conditionals are
 *   changed.
 */
/*public*/ void DefaultLogix::deActivateLogix() {
    if (_isActivated) {
        // Logix is active, deactivate it and all listeners
        _isActivated = false;
        // remove listeners if there are any
        for (int i=_listeners->size()-1; i>=0; i--) {
            removeListener(_listeners->at(i));
        }
    }
}
#if 0
/**
     * Creates a listener of the required type and starts it
     */
    private void startListener(JmriSimplePropertyListener listener) {
        String msg = "(unknown type number " + listener.getType() + ")";
        NamedBean nb;
        NamedBeanHandle<?> namedBeanHandle;

        switch (listener.getType()) {
            case LISTENER_TYPE_FASTCLOCK:
                Timebase tb = InstanceManager.timebaseInstance();
                tb.addMinuteChangeListener(listener);
                return;
            default:
                namedBeanHandle = listener.getNamedBean();
                if (namedBeanHandle == null) {
                    switch (listener.getType()) {
                        case LISTENER_TYPE_SENSOR:
                            msg = "sensor";
                            break;
                        case LISTENER_TYPE_TURNOUT:
                            msg = "turnout";
                            break;
                        case LISTENER_TYPE_LIGHT:
                            msg = "light";
                            break;
                        case LISTENER_TYPE_CONDITIONAL:
                            msg = "conditional";
                            break;
                        case LISTENER_TYPE_SIGNALHEAD:
                            msg = "signalhead";
                            break;
                        case LISTENER_TYPE_SIGNALMAST:
                            msg = "signalmast";
                            break;
                        case LISTENER_TYPE_MEMORY:
                            msg = "memory";
                            break;
                        case LISTENER_TYPE_WARRANT:
                            msg = "warrant";
                            break;
                        case LISTENER_TYPE_OBLOCK:
                            msg = "oblock";
                            break;
                        case LISTENER_TYPE_ENTRYEXIT:
                            msg = "entry exit";
                            break;
                        default:
                            msg = "unknown";
                    }
                    break;
                }
                nb = (NamedBean) namedBeanHandle.getBean();
                nb.addPropertyChangeListener(listener, namedBeanHandle.getName(), "Logix " + getDisplayName());
                return;
        }
        log.error("Bad name for " + msg + " \"" + listener.getDevName()
                + "\" when setting up Logix listener");
    }
#endif
/**
 * Creates a listener of the required type and starts it
 */
/*private*/ void DefaultLogix::startListener(JmriSimplePropertyListener* listener)
{
 QString msg = "(unknown type number "+QString::number(listener->getType())+")";
 /* If all are converted to NamedBeanHandles, do we have to go through this switch,
 the only reason for doing so would be the error message */
 NamedBean* nb;
 NamedBeanHandle<NamedBean*>* namedBeanHandle;
 switch (listener->getType())
 {
  case Logix::LISTENER_TYPE_SENSOR:
   namedBeanHandle = listener->getNamedBean();
   if (namedBeanHandle==NULL)
   {
    msg = "sensor";
    break;
   }
   nb = (NamedBean*) namedBeanHandle->getBean();
   nb->addPropertyChangeListener (listener, namedBeanHandle->getName(), "Logix " + getDisplayName());
   connect(((AbstractNamedBean*)nb)->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), listener, SLOT(propertyChange(PropertyChangeEvent*)));
   return;
  case Logix::LISTENER_TYPE_TURNOUT:
   namedBeanHandle = listener->getNamedBean();
   if (namedBeanHandle==NULL)
   {
    msg = "turnout";
    break;
   }
   nb = (NamedBean*) namedBeanHandle->getBean();
   nb->addPropertyChangeListener (listener, namedBeanHandle->getName(), "Logix " + getDisplayName());
   connect(((AbstractNamedBean*)nb)->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), listener, SLOT(propertyChange(PropertyChangeEvent*)));
   return;
  case Logix::LISTENER_TYPE_LIGHT:
  {
   Light* lgt = (Light*)((AbstractLightManager*)InstanceManager::lightManagerInstance())->
                           getLight(listener->getDevName());
   if (lgt==NULL)
   {
    msg = "light";
    break;
   }
   lgt->addPropertyChangeListener (listener);
   connect(((AbstractNamedBean*)lgt)->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), listener, SLOT(propertyChange(PropertyChangeEvent*)));
   return;
  }
  case Logix::LISTENER_TYPE_CONDITIONAL:
  {
   Conditional* c = ((ConditionalManager*)InstanceManager::conditionalManagerInstance())->
                           getConditional(listener->getDevName());
   if (c==NULL) {
       msg = "conditional";
       break;
   }
   c->addPropertyChangeListener (listener);
   connect(((AbstractNamedBean*)c)->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), listener, SLOT(propertyChange(PropertyChangeEvent*)));
   return;
  }
  case Logix::LISTENER_TYPE_SIGNALHEAD:
  {
   SignalHead* h = ((AbstractSignalHeadManager*)InstanceManager::signalHeadManagerInstance())->
                           getSignalHead(listener->getDevName());
   if (h==NULL) {
       msg = "signal head";
       break;
   }
   h->addPropertyChangeListener (listener);
   connect(((AbstractNamedBean*)h)->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), listener, SLOT(propertyChange(PropertyChangeEvent*)));

   return;
  }
  case Logix::LISTENER_TYPE_SIGNALMAST:
  {
   SignalMast* f = ((DefaultSignalMastManager*)InstanceManager::signalMastManagerInstance())->
                           provideSignalMast(listener->getDevName());
   if (f==NULL)
   {
    msg = "signal mast";
    break;
   }
   f->addPropertyChangeListener (listener);
   connect(((AbstractNamedBean*)f)->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), listener, SLOT(propertyChange(PropertyChangeEvent*)));
   return;
  }
  case Logix::LISTENER_TYPE_MEMORY:
  {
   namedBeanHandle = listener->getNamedBean();
   if (namedBeanHandle==NULL) {
       msg = "memory";
       break;
   }
   nb = (NamedBean*) namedBeanHandle->getBean();
   nb->addPropertyChangeListener (listener, namedBeanHandle->getName(), "Logix " + getDisplayName());
   connect(((AbstractNamedBean*)nb)->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), listener, SLOT(propertyChange(PropertyChangeEvent*)));
   return;
  }
#if 1
  case Logix::LISTENER_TYPE_WARRANT:
  {
   msg= "warrant";
   break;
  }
#endif
  case Logix::LISTENER_TYPE_FASTCLOCK:
  {
   Timebase* tb = InstanceManager::timebaseInstance();
   ((SimpleTimebase*)tb)->addMinuteChangeListener (listener);
   return;
  }
#if 1
  case Logix::LISTENER_TYPE_OBLOCK:
  {
   OBlock* b = ((OBlockManager*)InstanceManager::getDefault("OBlockManager"))->
                                 provideOBlock(listener->getDevName());
   if (b==NULL)
   {
    msg= "oblock";
    break;
   }
   b->addPropertyChangeListener (listener);
   connect(((AbstractNamedBean*)b)->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), listener, SLOT(propertyChange(PropertyChangeEvent*)));
   return;
  }
  case Logix::LISTENER_TYPE_ENTRYEXIT:
  {
   NamedBean* ex = ((EntryExitPairs*)InstanceManager::getDefault("EntryExitPairs"))->getBySystemName(listener->getDevName());
   if (ex==NULL)
   {
    msg= "entryexit";
    break;
   }
   ex->addPropertyChangeListener (listener);
   connect(((AbstractNamedBean*)ex)->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), listener, SLOT(propertyChange(PropertyChangeEvent*)));
  return;
  }
#endif
 }
 log->error("Bad name for " +msg+" \""+listener->getDevName()+
                    "\" when setting up Logix listener");
}

/**
 * Removes a listener of the required type
 */
/*private*/ void DefaultLogix::removeListener(JmriSimplePropertyListener* listener) {
    QString msg = NULL;
    try {
        switch (listener->getType()) {
            case Logix::LISTENER_TYPE_SENSOR:
        {
                Sensor* s = ((ProxySensorManager*)InstanceManager::sensorManagerInstance())->
                                        provideSensor(listener->getDevName());
                if (s==NULL) {
                    msg = "sensor";
                    break;
                }
                // remove listener for this Sensor
                s->removePropertyChangeListener(listener);
                return;
        }
            case Logix::LISTENER_TYPE_TURNOUT:
        {
                Turnout* t = ((ProxyTurnoutManager*)InstanceManager::turnoutManagerInstance())->
                                        provideTurnout(listener->getDevName());
                if (t==NULL) {
                    msg = "turnout";
                    break;
                }
                // remove listener for this Turnout
                t->removePropertyChangeListener(listener);
                return;
        }
            case Logix::LISTENER_TYPE_LIGHT:
        {
                Light* lgt = (Light*)((AbstractLightManager*)InstanceManager::lightManagerInstance())->
                                        getLight(listener->getDevName());
                if (lgt==NULL) {
                    msg = "light";
                    break;
                }
                // remove listener for this Light
                lgt->removePropertyChangeListener(listener);
                return;
        }
            case Logix::LISTENER_TYPE_CONDITIONAL:
        {
                Conditional* c = ((ConditionalManager*)InstanceManager::conditionalManagerInstance())->
                                        getConditional(listener->getDevName());
                if (c==NULL) {
                    msg = "conditional";
                    break;
                }
                // remove listener for this Conditional
                c->removePropertyChangeListener(listener);
                return;
        }
            case Logix::LISTENER_TYPE_SIGNALHEAD:
        {
                SignalHead* h = ((AbstractSignalHeadManager*)InstanceManager::signalHeadManagerInstance())->
                                        getSignalHead(listener->getDevName());
                if (h==NULL) {
                    msg = "signal head";
                    break;
                }
                // remove listener for this Signal Head
                h->removePropertyChangeListener(listener);
                return;
        }
            case Logix::LISTENER_TYPE_SIGNALMAST:
        {
                SignalMast* f = ((DefaultSignalMastManager*)InstanceManager::signalMastManagerInstance())->
                                        provideSignalMast(listener->getDevName());
                if (f==NULL) {
                    msg = "signal mast";
                    break;
                }
                // remove listener for this Signal Head
                f->removePropertyChangeListener(listener);
                return;
        }
            case Logix::LISTENER_TYPE_MEMORY:
        {
                Memory* m = ((AbstractMemoryManager*)InstanceManager::memoryManagerInstance())->
                                        provideMemory(listener->getDevName());
                if (m==NULL) {
                    msg= "memory";
                    break;
                }
                // remove listener for this Memory
                m->removePropertyChangeListener(listener);
                return;
        }
#if 1
            case Logix::LISTENER_TYPE_WARRANT:
        {
         Warrant* w = ((WarrantManager*)InstanceManager::getDefault("WarrantManager"))->
                                        provideWarrant(listener->getDevName());
                if (w==NULL) {
                    msg= "warrant";
                    break;
                }
                // remove listener for this Memory
                w->removePropertyChangeListener(listener);
                return;
        }
#endif
            case Logix::LISTENER_TYPE_FASTCLOCK:
        {
                Timebase* tb = InstanceManager::timebaseInstance();
                ((SimpleTimebase*)tb)->removeMinuteChangeListener (listener);
                return;
        }
#if 1
   case Logix::LISTENER_TYPE_OBLOCK:
   {
    //OBlock* b = InstanceManager::oBlockManagerInstance().
    OBlock* b = ((OBlockManager*)InstanceManager::getDefault("OBlockManager"))->
                                   provideOBlock(listener->getDevName());
    if (b==NULL)
    {
     msg= "oblock";
     break;
    }
    // remove listener for this Memory
    b->removePropertyChangeListener(listener);
    return;
   }
   case Logix::LISTENER_TYPE_ENTRYEXIT:
   {
    NamedBean* ex = ((EntryExitPairs*)InstanceManager::getDefault("EntryExitPairs"))->getBySystemName(listener->getDevName());
    if (ex==NULL)
    {
     msg= "entryexit";
     break;
    }
    ex->removePropertyChangeListener (listener);
    return;
   }
#endif
  }
 }
 catch (Throwable t)
 {
  log->error("Bad name for listener on \""+listener->getDevName()+"\": "+t.getMessage());
 }
 log->error("Bad name for "+msg+" listener on \""+listener->getDevName()+
                    "\" when removing");
}

/**
 * Assembles a list of state variables that both trigger the Logix, and are
 *   changed by it.  Returns true if any such variables were found.  Returns false
 *   otherwise.
 * Can be called when Logix is enabled.
 *
public boolean checkLoopCondition() {
    loopGremlins = new ArrayList<String[]>();
    if (!_isActivated) {
        // Prepare a list of all variables used in conditionals
        java.util.HashSet <ConditionalVariable> variableList = new java.util.HashSet<ConditionalVariable>();
        ConditionalManager cm = InstanceManager::conditionalManagerInstance();
        for (int i=0; i<_conditionalSystemNames.size(); i++) {
            Conditional c = NULL;
            c = cm.getBySystemName(_conditionalSystemNames.get(i));
            if (c!=NULL) {
                // Not necesary to modify methods, equals and hashcode. Redundacy checked in addGremlin
                variableList.addAll(c.getCopyOfStateVariables());
            }
        }
        java.util.HashSet <ConditionalVariable> variableList = new java.util.HashSet<ConditionalVariable>();
        ConditionalVariable v = NULL;
            // check conditional action items
        Conditional c = NULL;
        for (int i=0; i<_conditionalSystemNames.size(); i++) {
            // get next conditional
            c = cm.getBySystemName(_conditionalSystemNames.get(i));
            if (c!=NULL) {
                ArrayList <ConditionalAction> actionList = c.getCopyOfActions();
                for (int j = 0; j < actionList.size(); j++) {
                    ConditionalAction action = actionList.get(j);
                    String sName = "";
                    String uName = "";
                    switch (action.getType()) {
                        case Conditional::ACTION_NONE:
                            break;
                        case Conditional::ACTION_SET_TURNOUT:
                        case Conditional::ACTION_DELAYED_TURNOUT:
                        case Conditional::ACTION_RESET_DELAYED_TURNOUT:
                        case Conditional::ACTION_CANCEL_TURNOUT_TIMERS:
                            Turnout t = InstanceManager::turnoutManagerInstance().
                                        provideTurnout(action.getDeviceName());
                            if (t!=NULL) {
                                sName = t.getSystemName();
                                uName = t.getUserName();
                                // check for action on the same turnout
                                Iterator <ConditionalVariable>it= variableList.iterator();
                                while(it.hasNext()) {
                                    v = it.next();
                                    if (v.getType() == Conditional::TYPE_TURNOUT_CLOSED ||
                                        v.getType() == Conditional::TYPE_TURNOUT_THROWN) {
                                        if ( (v.getName()==(sName)) ||
                                                (v.getName()==(uName)) ) {
                                            // possible conflict found
                                            addGremlin("Turnout", sName, uName);
                                        }
                                    }
                                }
                            }
                            break;
                        case Conditional::ACTION_SET_SIGNAL_APPEARANCE:
                        case Conditional::ACTION_SET_SIGNAL_HELD:
                        case Conditional::ACTION_CLEAR_SIGNAL_HELD:
                        case Conditional::ACTION_SET_SIGNAL_DARK:
                        case Conditional::ACTION_SET_SIGNAL_LIT:
                            SignalHead h = InstanceManager::signalHeadManagerInstance().
                                            getSignalHead(action.getDeviceName());
                            if (h!=NULL) {
                                sName = h.getSystemName();
                                uName = h.getUserName();
                                // check for action on the same signal head
                                Iterator <ConditionalVariable>it= variableList.iterator();
                                while(it.hasNext()) {
                                    v = it.next();
                                    if (v.getType() >= Conditional::TYPE_SIGNAL_HEAD_RED ||
                                        v.getType() <= Conditional::TYPE_SIGNAL_HEAD_HELD) {
                                        if ( (v.getName()==(sName)) ||
                                                (v.getName()==(uName)) ) {
                                            // possible conflict found
                                            addGremlin("SignalHead", sName, uName);
                                        }
                                    }
                                }
                            }
                            break;
                        case Conditional::ACTION_SET_SENSOR:
                        case Conditional::ACTION_DELAYED_SENSOR:
                        case Conditional::ACTION_RESET_DELAYED_SENSOR:
                        case Conditional::ACTION_CANCEL_SENSOR_TIMERS:
                            Sensor s = InstanceManager::sensorManagerInstance().
                                        provideSensor(action.getDeviceName());
                            if (s!=NULL) {
                                sName = s.getSystemName();
                                uName = s.getUserName();
                                // check for action on the same sensor
                                Iterator <ConditionalVariable>it= variableList.iterator();
                                while(it.hasNext()) {
                                    v = it.next();
                                    if (v.getType() == Conditional::TYPE_SENSOR_ACTIVE ||
                                        v.getType() == Conditional::TYPE_SENSOR_INACTIVE) {

                                        if ( (v.getName()==(sName)) ||
                                                (v.getName()==(uName)) ) {
                                            // possible conflict found
                                            addGremlin("Sensor",sName, uName);
                                        }
                                    }
                                }
                            }
                            break;
                        case Conditional::ACTION_SET_LIGHT:
                        case Conditional::ACTION_SET_LIGHT_TRANSITION_TIME:
                        case Conditional::ACTION_SET_LIGHT_INTENSITY:
                            Light lgt = InstanceManager::lightManagerInstance().
                                            getLight(action.getDeviceName());
                            if (lgt!=NULL) {
                                sName = lgt.getSystemName();
                                uName = lgt.getUserName();
                                // check for listener on the same light
                                Iterator <ConditionalVariable>it= variableList.iterator();
                                while(it.hasNext()) {
                                    v = it.next();
                                    if (v.getType() == Conditional::TYPE_LIGHT_ON ||
                                        v.getType() == Conditional::TYPE_LIGHT_OFF) {
                                        if ( (v.getName()==(sName)) ||
                                                (v.getName()==(uName)) ) {
                                            // possible conflict found
                                            addGremlin("Light", sName, uName);
                                        }
                                    }
                                }
                            }
                            break;
                        case Conditional::ACTION_SET_MEMORY:
                        case Conditional::ACTION_COPY_MEMORY:
                            Memory m = InstanceManager::memoryManagerInstance().
                                        provideMemory(action.getDeviceName());
                            if (m!=NULL) {
                                sName = m.getSystemName();
                                uName = m.getUserName();
                                // check for variable on the same memory
                                Iterator <ConditionalVariable>it= variableList.iterator();
                                while(it.hasNext()) {
                                    v = it.next();
                                    if (v.getType() == Conditional::TYPE_MEMORY_EQUALS) {
                                        if ( (v.getName()==(sName)) ||
                                                (v.getName()==(uName)) ) {
                                            // possible conflict found
                                            addGremlin("Memory", sName, uName);
                                        }
                                    }
                                }
                            }
                            break;
                        case Conditional::ACTION_SET_FAST_CLOCK_TIME:
                        case Conditional::ACTION_START_FAST_CLOCK:
                        case Conditional::ACTION_STOP_FAST_CLOCK:
                            Iterator <ConditionalVariable>it= variableList.iterator();
                            while(it.hasNext()) {
                                v = it.next();
                                if (v.getType() == Conditional::TYPE_FAST_CLOCK_RANGE) {
                                        addGremlin("FastClock", NULL, v.getName());
                                }
                            }
                            break;
                        default:
                    }
                }
            }
        }
    }
    return (loopGremlins.size()>0);
}

private void addGremlin(String type, String sName, String uName) {
    // check for redundancy
    String names = uName+ (sName == NULL ? "" : " ("+sName+")");
    for (int i=0; i<loopGremlins.size(); i++)
    {
        String[] str = loopGremlins.get(i);
        if (str[0]==(type) && str[1]==(names)) {
            return;
        }
    }
    String[] item = new String[2];
    item[0] = type;
    item[1] = names;
    loopGremlins.add(item);
}

ArrayList <String[]> loopGremlins = NULL;

/**
 * Returns a string listing state variables that might result in a loop.
 *    Returns an empty string if there are none, probably because
 *    "checkLoopCondition" was not invoked before the call, or returned false.
 *
public ArrayList <String[]> getLoopGremlins() {return(loopGremlins);}
*/
/**
 * Not needed for Logixs - included to complete implementation of the NamedBean interface.
 */
/*public*/ int DefaultLogix::getState() {
    log->warn("Unexpected call to getState in DefaultLogix->");
    return UNKNOWN;
}

/**
 * Not needed for Logixs - included to complete implementation of the NamedBean interface.
 */
/*public*/ void DefaultLogix::setState(int /*state*/) {
    log->warn("Unexpected call to setState in DefaultLogix->");
    return;
}

//    static final org.apache.log4j.Logger log = org.apache.log4j.Logger.getLogger(DefaultLogix->class.getName());
//}

/* @(#)DefaultLogix->java */
