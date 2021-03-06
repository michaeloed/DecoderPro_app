#ifndef ROSTERCONFIGMANAGER_H
#define ROSTERCONFIGMANAGER_H
#include "abstractpreferencesmanager.h"
#include "propertychangeevent.h"
#include <QHash>

class Logger;
class Roster;
class RosterConfigManager : public AbstractPreferencesManager
{
 Q_OBJECT
public:
 Q_INVOKABLE RosterConfigManager();
 ~RosterConfigManager() {}
 RosterConfigManager(const RosterConfigManager&): AbstractPreferencesManager() {}
 /*public*/ static /*final*/ QString DIRECTORY;// = "directory";
 /*public*/ static /*final*/ QString DEFAULT_OWNER;// = "defaultOwner";
 /*public*/ void initialize(Profile* profile)throw (InitializationException);
 /*public*/ void savePreferences(Profile* profile);
 /*public*/ /*Set<Class<? extends PreferencesManager>>*/ QSet<QString>* getRequires();
 /*public*/ QString getDefaultOwner();
 /*public*/ QString getDefaultOwner(/*@CheckForNull*/ Profile* profile);
 /*public*/ void setDefaultOwner(Profile *profile, QString defaultOwner);
 /*public*/ QString getDirectory();
 /*public*/ QString getDirectory(/*@CheckForNull*/ Profile* profile);
 /*public*/ void setDirectory(/*@CheckForNull*/ Profile* profile, /*@CheckForNull*/ QString directory);
 /*public*/ Roster* getRoster(/*@CheckForNull*/ Profile* profile);
 /*public*/ Roster* setRoster(/*@CheckForNull*/ Profile* profile, /*@Nonnull*/ Roster* roster);

public slots:
 void propertyChange(PropertyChangeEvent*);

private:
 Logger* log;
// /*private*/ QString directory;// = FileUtil.PREFERENCES;
// /*private*/ QString defaultOwner;// = "";
 /*private*/ /*final*/ QHash<Profile*, QString> directories;// = new HashMap<>();
 /*private*/ /*final*/ QHash<Profile*, QString> defaultOwners;// = new HashMap<>();
 /*private*/ /*final*/ QHash<Profile*, Roster*> rosters;// = new HashMap<>();
};
Q_DECLARE_METATYPE(RosterConfigManager)

#endif // ROSTERCONFIGMANAGER_H
