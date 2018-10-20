#include "managerdefaultselector.h"
#include "systemconnectionmemo.h"
#include "instancemanager.h"
#include "loggerfactory.h"
#include "internalsystemconnectionmemo.h"
#include "preferences.h"
#include "profileutils.h"
#include "class.h"
#include "configuremanager.h"

/**
 * Records and executes a desired set of defaults for the JMRI InstanceManager
 * and ProxyManagers
 * <hr>
 * This file is part of JMRI.
 * <P>
 * JMRI is free software; you can redistribute it and/or modify it under the
 * terms of version 2 of the GNU General Public License as published by the Free
 * Software Foundation. See the "COPYING" file for a copy of this license.
 * <P>
 * JMRI is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * <P>
 * @author	Bob Jacobsen Copyright (C) 2010
 * @author Randall Wood Copyright (C) 2015
 * @since 2.9.4
 */
///*public*/ class ManagerDefaultSelector extends AbstractPreferencesManager {

/*private*/ /*final*/ /*static*/ Logger* ManagerDefaultSelector::log = LoggerFactory::getLogger("ManagerDefaultSelector");


/*public*/ ManagerDefaultSelector::ManagerDefaultSelector(QObject *parent) : AbstractPreferencesManager(parent)
{
 defaults = QMap<QString, QString>();
 // Define set of items that we remember defaults for, manually maintained because
 // there are lots of JMRI-internal types of no interest to the user and/or not system-specific.
 // This grows if you add something to the SystemConnectionMemo system
 knownManagers = QList<Item1>() <<
      Item1("Throttles", "ThrottleManager")<<
      Item1("<html>Power<br>Control</html>", "PowerManager") <<
      Item1("<html>Command<br>Station</html>", "CommandStation")<<
      Item1("<html>Service<br>Programmer</html>", "GlobalProgrammerManager") <<
      Item1("<html>Ops Mode<br>Programmer</html>",  "AddressedProgrammerManager") <<
      Item1("Consists ", "ConsistManager");
 if(log == NULL)
  log = LoggerFactory::getLogger("ManagerDefaultSelector");

 SystemConnectionMemo::addPropertyChangeListener((PropertyChangeListener*)this);
 connect(SystemConnectionMemo::instance(), SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));
}

//    SystemConnectionMemo::addPropertyChangeListener((PropertyChangeEvent e) ->
/*public*/ void ManagerDefaultSelector::propertyChange(PropertyChangeEvent *e)
{
 if (e->getPropertyName()== "ConnectionNameChanged")
 {
  QString oldName =  e->getOldValue().toString();
  QString newName =  e->getNewValue().toString();
  log->debug(tr("ConnectionNameChanged from \"%1\" to \"%2\"").arg(oldName).arg( newName));
  //defaults.keySet().stream().forEach((c) ->
  foreach(QString c, defaults.keys())
  {
   QString connectionName = this->defaults.value(c);
   if (connectionName == (oldName))
   {
    this->defaults.insert(c, newName);
   }
  }
 }
 else if (e->getPropertyName()=="ConnectionDisabled")
 {
  bool newState = e->getNewValue().toBool();
  if (newState)
  {
   SystemConnectionMemo* memo = (SystemConnectionMemo*) e->getSource();
   QString disabledName = memo->getUserName();
   log->debug(tr("ConnectionDisabled true: \"%1\"").arg(disabledName));
   removeConnectionAsDefault(disabledName);
  }
 }
 else if (e->getPropertyName()== "ConnectionRemoved")
 {
  QString removedName =  e->getOldValue().toString();
  log->debug(tr("ConnectionRemoved for \"%1\"").arg(removedName));
  removeConnectionAsDefault(removedName);
 }
 else if (e->getPropertyName()=="ConnectionAdded")
 {
  // check for special case of anything else then Internal
  QObjectList* list = InstanceManager::getList("SystemConnectionMemo");
  if (list->size() == 2 && qobject_cast<InternalSystemConnectionMemo*>(list->at(0)) != 0 ) // ACK change from 1 to 0; Internal is last!
  {
   log->debug("First real system added, reset defaults");
   QString name = ((SystemConnectionMemo*)list->at(1))->getUserName();
   removeConnectionAsDefault(name);
  }
 }
 else
 {
  log->debug(tr("ignoring notification of \"%1\"").arg(e->getPropertyName()));
 }
 this->firePropertyChange("Updated", QVariant(), QVariant());
}

// remove connection's record
void ManagerDefaultSelector::removeConnectionAsDefault(QString removedName)
{
 QVector<QString> tmpArray = QVector<QString>();
  //defaults.keys().stream().forEach((c) ->
 foreach(QString c, defaults.keys())
 {
  QString connectionName = this->defaults.value(c);
  if (connectionName==(removedName))
  {
   log->debug("Connection " + removedName + " has been removed as the default for " + c);
   tmpArray.append(c);
  }
 }//);
  //tmpArray.stream().forEach((tmpArray1) ->
 foreach(QString tmpArray1, tmpArray)
 {
  this->defaults.remove(tmpArray1);
 }//);
}

/**
 * Return the userName of the system that provides the default instance for
 * a specific class.
 *
 * @param managerClass the specific type, e.g. TurnoutManager, for which a
 *                     default system is desired
 * @return userName of the system, or  NULL if none set
 */
/*public*/ QString ManagerDefaultSelector::getDefault(QString managerClass) {
    return defaults.value(managerClass);
}

/**
 * Record the userName of the system that provides the default instance for
 * a specific class.
 *
 * To ensure compatibility of different preference versions, only classes
 * that are current registered are preserved. This way, reading in an old
 * file will just have irrelevant items ignored.
 *
 * @param managerClass the specific type, e.g. TurnoutManager, for which a
 *                     default system is desired
 * @param userName     of the system, or  NULL if none set
 */
/*public*/ void ManagerDefaultSelector::setDefault(QString managerClass, QString userName)
{
 foreach (Item1 item, knownManagers)
 {
  if (item.managerClass == (managerClass))
  {
   log->debug(tr("   setting default for \"%1\" to \"%2\" by request").arg( managerClass).arg(userName));
   defaults.insert(managerClass, userName);
   return;
  }
 }
 log->warn(tr("Ignoring preference for class %1 with name %2").arg(managerClass).arg(userName));
}

/**
 * Load into InstanceManager
 *
 * @return an exception that can be passed to the user or  NULL if no errors
 *         occur
 */
//@CheckForNull
/*public*/ InitializationException* ManagerDefaultSelector::configure()
{
 InitializationException* error =  NULL;
 QObjectList* connList = InstanceManager::getList("SystemConnectionMemo");
 log->debug(tr("configure defaults into InstanceManager from %1 memos, %2 defaults").arg(connList->size()).arg(defaults.keys().size()));
 foreach (QString c, defaults.keys())
 {
  // 'c' is the class to load
  QString connectionName = this->defaults.value(c);
  // have to find object of that type from proper connection
  bool found = false;
  foreach (QObject* memo, *connList)
  {
   QString testName = ((SystemConnectionMemo*)memo)->getUserName();
   if (testName == (connectionName))
   {
    found = true;
    // match, store
    try
    {
     if(((SystemConnectionMemo*)memo)->provides(c))
     {
      log->debug(tr("   setting default for \"%1\" to \"%2\" in configure").arg(c).arg(((SystemConnectionMemo*)memo)->get(c)->metaObject()->className()));
      InstanceManager::setDefault(c, ((SystemConnectionMemo*)memo)->get(c));
     }
    }
    catch (NullPointerException ex)
    {
     QString englishMsg = QString("System connection %1 provides a  NULL manager for %2").arg(((SystemConnectionMemo*)memo)->getUserName()).arg( c); // NOI18N
     QString localizedMsg = tr("System connection %1 provides a  NULL manager for %2").arg(((SystemConnectionMemo*)memo)->getUserName()).arg( c); // NOI18N
     error = new InitializationException(englishMsg, localizedMsg,NULL);
     log->warn(tr("SystemConnectionMemo for %1 (%2) provides a  NULL %3 instance").arg(((SystemConnectionMemo*)memo)->getUserName()).arg(((SystemConnectionMemo*)memo)->metaObject()->className()).arg(c));
    }
    break;
   }
   else
   {
    log->debug(tr("   memo name didn't match: %1 vs %2").arg(testName).arg(connectionName));
   }
  }
  /*
   * If the set connection can not be found then we shall set the manager default to use what
   * has currently been set.
   */
  if (!found) {
      log->debug("!found, so resetting");
      QString currentName =  NULL;
      if (c == "ThrottleManager" && InstanceManager::getNullableDefault("ThrottleManager") !=  NULL) {
       currentName = ((ThrottleManager*)InstanceManager::throttleManagerInstance())->getUserName();
      }
      else if (c == "PowerManager" && InstanceManager::getNullableDefault("PowerManager") !=  NULL) {
       currentName =((PowerManager*) InstanceManager::getDefault("PowerManager"))->getUserName();
      }
      else if (c == "ProgrammerManager" && InstanceManager::getNullableDefault("ProgrammerManager") !=  NULL)
      {
       currentName = ((ProgrammerManager*) InstanceManager::getDefault("ProgrammerManager"))->getUserName();
      }
      if (currentName !=  NULL)
      {
       log->warn("The configured " + connectionName + " for " + c + " can not be found so will use the default " + currentName);
       this->defaults.insert(c, currentName);
      }
  }
 }
 return error;
}


//@Override
/*public*/ void ManagerDefaultSelector::initialize(Profile* profile) throw (InitializationException)
{
 if (!this->isInitialized(profile))
 {
  Preferences* settings = ProfileUtils::getPreferences(profile, "jmri/managers/ManagerDefaultSelector", true)->node("defaults"); // NOI18N
  try
  {
   foreach (QString name, settings->keys())
   {
    QString connection = settings->get(name,  NULL);
    QString cls = this->classForName(name);
    log->debug(tr("Loading default %1 for %2").arg(connection).arg(name));
    if (cls !=  NULL)
    {
     this->defaults.insert(cls, connection);
     log->debug(tr("Loaded default %1 for %2").arg(connection).arg(cls));
    }
   }
  } catch (BackingStoreException ex) {
      log->info("Unable to read preferences for Default Selector.");
  }
  InitializationException* ex = this->configure();
  ConfigureManager* manager =(ConfigureManager*) InstanceManager::getNullableDefault("ConfigureManager");
  if (manager !=  NULL) {
      manager->registerPref(this); // allow ProfileConfig.xml to be written correctly
  }
  this->setInitialized(profile, true);
  if (ex !=  NULL) {
      throw ex;
  }
 }
}

//@Override
/*public*/ void ManagerDefaultSelector::savePreferences(Profile* profile)
{
 Preferences* settings = ProfileUtils::getPreferences(profile, "jmri/managers/ManagerDefaultSelector", true)->node("defaults"); // NOI18N
 try
 {
  //this->defaults.keySet().stream().forEach((cls) ->
  foreach(QString cls, defaults.keys())
  {
   settings->put(this->nameForClass(cls), this->defaults.value(cls));
  }//);
  settings->sync();
 }
 catch (BackingStoreException ex)
 {
  log->error("Unable to save preferences for Default Selector.", ex.getMessage());
 }
}

///*public*/ static class Item {

//    /*public*/ String typeName;
//    /*public*/ Class<?> managerClass;

    ManagerDefaultSelector::Item1::Item1(QString typeName, QString managerClass) {
        this->typeName = typeName;
        this->managerClass = managerClass;
    }
//}

/*private*/ QString ManagerDefaultSelector::nameForClass(/*@Non NULL*/ QString cls)
{
 return cls; //.getCanonicalName().replace('.', '-');
}

/*private*/ QString ManagerDefaultSelector::classForName(/*@Non NULL*/ QString name)
{
 try
 {
  return Class::forName(name/*.replace('-', '.')*/)->metaObject()->className();
 }
 catch (ClassNotFoundException ex)
 {
  log->error(tr("Could not find class for %1").arg(name));
  return  NULL;
 }
}

