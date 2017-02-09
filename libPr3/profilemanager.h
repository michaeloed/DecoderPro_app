#ifndef PROFILEMANAGER_H
#define PROFILEMANAGER_H
#include <QObject>
#include "logger.h"
#include "filefilter.h"
#include "filenamefilter.h"
#include "indexedpropertychangeevent.h"
#include "addprofiledialog.h"
#include "libpref_global.h"

class QDir;
class QuaZip;
class QuaZipFile;
class PropertyChangeEvent;
class File;
class Profile;
class LIBPREFSHARED_EXPORT ProfileManager : public QObject
{
    Q_OBJECT
public:
    explicit ProfileManager(QObject *parent = 0);
    QT_DEPRECATED/*public*/  static ProfileManager* defaultManager();
    /*public*/ static ProfileManager* getDefault();
    /*public*/ File* getConfigFile();
    /*public*/ void setConfigFile(File* configFile);
    /*public*/ static QString createUniqueId();
    /*public*/ Profile* getActiveProfile();
    /*public*/ static Profile* getStartingProfile() /*throws IOException*/;
    /*public*/ bool isAutoStartActiveProfile();
    /*public*/ void saveActiveProfile() /*throws IOException */;
    /*public*/ void readActiveProfile() /*throws IOException */;
    /*public*/ QList<Profile*> getProfiles();
    /*public*/ QList<Profile*> getAllProfiles();
    /*public*/ Profile* getProfiles(int index);
    /*public*/ void setProfiles(Profile* profile, int index);
    /*public*/ bool migrateToProfiles(QString configFilename) /*throws IllegalArgumentException, IOException*/;
    /*public*/ void _export(Profile* profile, File* target, bool exportExternalUserFiles, bool exportExternalRoster); //throws IOException, JDOMException
    /*public*/ void setAutoStartActiveProfile(bool autoStartActiveProfile);
    /*public*/ Profile* createDefaultProfile() /*throws IllegalArgumentException, IOException*/;
    /*public*/ Profile* migrateConfigToProfile(File* config, QString name) /*throws IllegalArgumentException, IOException*/;
    /*public*/ static /*final*/ QString SEARCH_PATHS;// = "searchPaths"; // NOI18N
    /*public*/ static /*final*/ QString SYSTEM_PROPERTY;// = "org.jmri.profile"; // NOI18N
    /*public*/ static /*final*/ QString ACTIVE_PROFILE;// = "activeProfile"; // NOI18N
    /*public*/ static /*final*/ QString NEXT_PROFILE;// = "nextProfile"; // NOI18N
    /*public*/ static /*final*/ QString PROFILES;// = "profiles"; // NOI18N
    /*public*/ static /*final*/ QString DEFAULT_SEARCH_PATH;// = "defaultSearchPath"; // NOI18N

    /*public*/ QList<File*> getSearchPaths();
    /*public*/ File* getSearchPaths(int index);
    /*public*/ void setActiveProfile(QString id);
    /*public*/ void setActiveProfile(Profile* profile);


signals:
    void indexedPropertyChange(IndexedPropertyChangeEvent*);
    void propertyChange(PropertyChangeEvent*);
public slots:
private:
    /*private*/ /*static*/ class ProfileManagerHolder
    {
     /**
      * Default instance of the ProfileManager
      */
     /*public*/ static ProfileManager* manager;// = new ProfileManager();
     friend class ProfileManager;
    };
    /*private*/ /*final*/ QList<Profile*> profiles;// = new QList<Profile>();
    /*private*/ /*final*/ QList<File*> searchPaths;// = new QList<File>();
    /*private*/ Profile* activeProfile;// = NULL;
    /*private*/ Profile* nextActiveProfile;// = NULL;
    /*private*/ /*final*/ File* catalog;
    /*private*/ File* configFile;// = NULL;
    /*private*/ bool readingProfiles;// = false;
    /*private*/ bool autoStartActiveProfile;// = false;
    /*private*/ static ProfileManager* instance;// = NULL;
    /*private*/ static /*final*/ QString AUTO_START;// = "autoStart"; // NOI18N
    /*private*/ static /*final*/ QString CATALOG;// = "profiles.xml"; // NOI18N
    /*private*/ static /*final*/ QString PROFILE;// = "profile"; // NOI18N
    /*private*/ static /*final*/ QString PROFILECONFIG;// = "profileConfig"; // NOI18N
    Logger* log;
    void profileNameChange(Profile* profile, QString oldName);
    /*private*/ void readProfiles() /*throws JDOMException, IOException*/;
    /*private*/ void writeProfiles() /*throws IOException*/;
    /*private*/ void findProfiles();
    /*private*/ void findProfiles(File* searchPath);
    QuaZip* zip;
    /*private*/ bool exportDirectory(QuaZipFile* zip, File* source, QString root); //throws IOException
    /*private*/ bool exportFile(QuaZipFile* zip, File* source, QString root); //throws IOException
    /*private*/ QString relativeName(File* file, QString root);

    class FileFilter1 : public FileFilter
    {
     File* pathname;
    public:
     /*public*/ bool accept(File* pathname);
     /*public*/ QString getDescription();

     friend class ProfileManager;
    };
    /*private*/ File* defaultSearchPath;// = new File(FileUtil.getPreferencesPath());
protected:
    /*protected*/ void addProfile(Profile* profile);
    /*protected*/ void addSearchPath(File* path) /*throws IOException*/;
    /*protected*/ void saveActiveProfile(Profile* profile, bool autoStart) /*throws IOException*/;
    /*protected*/ Profile* getNextActiveProfile();
    /*protected*/ void setNextActiveProfile(Profile* profile);
    /*protected*/ void removeProfile(Profile* profile);
    /*protected*/ void removeSearchPath(File* path) /*throws IOException*/;
    /*protected*/ File* getDefaultSearchPath();
    /*protected*/ void setDefaultSearchPath(/*@Nonnull*/ File* defaultSearchPath) /*throws IOException */;

    friend class Profile;
    friend class FileFilter1;
    friend class ProfileManagerDialog;
    friend class ProfilePreferencesPanel;
    friend class ProfileTableModel;
    friend class AppsBase;
    friend class Apps;
    friend class Apps3;
    friend class AddProfileDialog;
};

#endif // PROFILEMANAGER_H
