#include "jmriconfigurationmanager.h"
#include "profilemanager.h"
#include "configxmlmanager.h"
#include "profile.h"
#include "instancemanager.h"
#include "fileutil.h"
#include "file.h"
#include "preferencesmanager.h"
#include "serviceloader.h"
#include "jlist.h"
#include <QMessageBox>
#include <QApplication>
#include <QLayout>
#include <QLabel>
#include "configxmlmanager.h"
#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include "appsbase.h"
#include "editconnectionpreferencesdialog.h"
#include "joptionpane.h"
#include "tabbedpreferencesaction.h"
#include "vptr.h"
#include "loggerfactory.h"
#include "preferencespanel.h"

//JmriConfigurationManager::JmriConfigurationManager()
//{

//}
/**
 *
 * @author Randall Wood
 */
// /*public*/ class JmriConfigurationManager implements ConfigureManager {

/*private*/ /*final*/ /*static*/ Logger* JmriConfigurationManager::log = LoggerFactory::getLogger("JmriConfigurationManager");

//@SuppressWarnings("unchecked") // For types in InstanceManager.store()
/*public*/ JmriConfigurationManager::JmriConfigurationManager(QObject* parent) : ConfigXmlManager(parent)
{
 setObjectName("JmriConfigurationManager");
 legacy = new ConfigXmlManager();
 initializationExceptions = new QHash<PreferencesManager*, InitializationException*>();
 initialized = new QList<PreferencesManager*>();

 //ServiceLoader<PreferencesManager> sl = ServiceLoader.load(PreferencesManager.class);
#if 0
    ServiceLoader<PreferencesManager>* sl = ServiceLoader<PreferencesManager>::load("PreferencesManager");

    for (PreferencesManager pp : sl) {
        InstanceManager.store(pp, PreferencesManager.class);
        for (Class provided : pp.getProvides()) { // use raw class so next line can compile
            InstanceManager.store(provided.cast(pp), provided);
        }
    }
#else
 QList<QString> lst = QList<QString>()
  << "StartupActionsManager" << "GuiLafPreferencesManager" << "SystemConsolePreferencesManager"
     << "FileLocationsPreferences" << "ManagerDefaultSelector" << "WarrantPreferences" << "RosterConfigManager"
     << "ProgrammerConfigManager" << "ConnectionConfigManager" << "JmriJTablePersistenceManager";

 foreach (QString name, lst)
 {
  PreferencesManager* pp = (PreferencesManager*)InstanceManager::getNullableDefault(name);
//  QList<PreferencesManager*> l = pp->getProvides();
//  foreach (PreferencesManager* provided, *pp->getProvides())
//  {
//   InstanceManager::store(provided, name);
//  }
  if(pp == NULL)
   continue;
  InstanceManager::store(pp, "PreferencesManager");
 }
#endif
// if (ProfileManager::getDefault() == NULL)
//  ProfileManagerHolder::manager = new ProfileManager();
    if (ProfileManager::getDefault()->getActiveProfile() != NULL) {
        this->legacy->setPrefsLocation(new File(ProfileManager::getDefault()->getActiveProfile()->getPath(), /*Profile::CONFIG_FILENAME*/"ProfileConfig.xml"));
    }
#if 0
    if (!GraphicsEnvironment.isHeadless()) {
        ConfigXmlManager.setErrorHandler(new DialogErrorHandler());
    }
#endif
}

//@Override
/*public*/ void JmriConfigurationManager::registerPref(QObject* o) {
    //if ((o instanceof PreferencesManager)) {
 if(qobject_cast<PreferencesPanel*>(o))
 {
  this->legacy->registerPref(o);
  return;
 }
    if(qobject_cast<PreferencesManager*>(o) != NULL)
    {
        InstanceManager::store((PreferencesManager*) o, "PreferencesManager");
    }
    this->legacy->registerPref(o);
}

//@Override
/*public*/ void JmriConfigurationManager::removePrefItems() {
    this->legacy->removePrefItems();
}

//@Override
/*public*/ void JmriConfigurationManager::registerConfig(QObject* o) {
    this->legacy->registerConfig(o);
}

//@Override
/*public*/ void JmriConfigurationManager::registerConfig(QObject* o, int x) {
    this->legacy->registerConfig(o, x);
}

//@Override
/*public*/ void JmriConfigurationManager::registerTool(QObject* o) {
    this->legacy->registerTool(o);
}

//@Override
/*public*/ void JmriConfigurationManager::registerUser(QObject* o) {
    this->legacy->registerUser(o);
}

//@Override
/*public*/ void JmriConfigurationManager::registerUserPrefs(QObject* o) {
    this->legacy->registerUserPrefs(o);
}

//@Override
/*public*/ void JmriConfigurationManager::deregister(QObject* o) {
    this->legacy->deregister(o);
}

//@Override
/*public*/ QObject* JmriConfigurationManager::findInstance(QString c, int index) {
    return this->legacy->findInstance(c, index);
}

//@Override
/*public*/ QList<QObject*> JmriConfigurationManager::getInstanceList(QString c) {
    return this->legacy->getInstanceList(c);
}

//@Override
/*public*/ bool JmriConfigurationManager::storeAll(File* file) {
    return this->legacy->storeAll(file);
}

/**
 * Save preferences. Preferences are saved using either the
 * {@link jmri.util.prefs.JmriConfigurationProvider} or
 * {@link jmri.util.prefs.JmriPreferencesProvider} as appropriate to the
 * register preferences handler.
 */
//@Override
/*public*/ void JmriConfigurationManager::storePrefs()
{
 log->debug("Saving preferences...");
 Profile* profile = ProfileManager::getDefault()->getActiveProfile();
//    InstanceManager::getList(PreferencesManager.class).stream().forEach((o) -> {
//        log.debug("Saving preferences for {}", o.getClass().getName());
//        o.savePreferences(profile);
//    });
 foreach (QObject* o, *InstanceManager::getList("PreferencesManager"))
 {
  log->debug(tr("Saving preferences for %1").arg(o->metaObject()->className()));
  ((PreferencesManager*)o)->savePreferences(profile);
 }
}

/**
 * Save preferences. This method calls {@link #storePrefs() }.
 *
 * @param file Ignored.
 */
//@Override
/*public*/ void JmriConfigurationManager::storePrefs(File* file) {
    this->storePrefs();
}

//@Override
/*public*/ void JmriConfigurationManager::storeUserPrefs(File* file) {
    this->legacy->storeUserPrefs(file);
}

//@Override
/*public*/ bool JmriConfigurationManager::storeConfig(File* file) {
    return this->legacy->storeConfig(file);
}

//@Override
/*public*/ bool JmriConfigurationManager::storeUser(File* file)
{
    return this->legacy->storeUser(file);
}

//@Override
/*public*/ bool JmriConfigurationManager::load(File* file)  throw (JmriException)
{
    return this->load(file, false);
}

//@Override
/*public*/ bool JmriConfigurationManager::load( QUrl file)  throw (JmriException)
{
    return this->load(file, false);
}

//@Override
/*public*/ bool JmriConfigurationManager::load(File* file, bool registerDeferred)  throw (JmriException)
{
    return this->load(FileUtil::fileToURL(file), registerDeferred);
}

//@Override
/*public*/ bool JmriConfigurationManager::load(QUrl file, bool registerDeferred)  throw (JmriException)
{
 log->debug(tr("loading %1 ...").arg(file.path()));
 try {
  if (file.isEmpty()
          || (File(file.toDisplayString()).getName() == ("ProfileConfig.xml")) //NOI18N
          || (File(file.toDisplayString()).getName() == (/*Profile::CONFIG*/"profile.xml")))
  {
   Profile* profile = ProfileManager::getDefault()->getActiveProfile();
   QObjectList* providers =  InstanceManager::getList("PreferencesManager");
   //providers.stream().forEach((provider) -> {
   foreach(QObject* provider, *providers)
   {
    this->initializeProvider((PreferencesManager*)provider, profile);
   }//);
   //foreach(QObject* provider, *providers)
   if (!this->initializationExceptions->isEmpty())
   {
//                if (!GraphicsEnvironment.isHeadless()) {

    bool isUnableToConnect = false;

    QStringList* errors = new QStringList();
    //this.initialized.forEach((provider) ->
    foreach(PreferencesManager* provider, *this->initialized)
    {
     QList<Exception*>* exceptions = provider->getInitializationExceptions(profile);
     if (!exceptions->isEmpty()) {
         //exceptions.forEach((exception) ->
      foreach(Exception* exception, *exceptions)
      {
          errors->append(exception->getLocalizedMessage());
      } //);
     } else if (this->initializationExceptions->value(provider) != NULL) {
         errors->append(this->initializationExceptions->value(provider)->getLocalizedMessage());
     }
    } //);
    JList* list = NULL;
    if (errors->size() == 1) {
//                        list = *errors->at(0);
    } else {
        list = new JList(*errors);
    }
    QStringList* errorList = errors;

    if (isUnableToConnect)
    {
     if (errors->size() > 1)
     {
         errorList->insert(0, tr("The following errors occurred in the order listed:"));
     }
     errorList->append("");
     errorList->append(tr("Please check the logs for more details.")); // NOI18N

     ErrorDialog* dialog = new ErrorDialog(*errorList);

     switch (dialog->result)
     {
      case ErrorDialog::NEW_PROFILE:
      {
          AddProfileDialog* apd = new AddProfileDialog((QFrame*)nullptr, true, false);
          apd->setLocationRelativeTo(nullptr);
          apd->setVisible(true);
          // Restart program
          AppsBase::handleRestart();
          break;
      }
      case ErrorDialog::EDIT_CONNECTIONS:
         if (EditConnectionPreferencesDialog::showDialog()) {
              // Restart program
              AppsBase::handleRestart();
              break;
          } else {
              // Quit program
              AppsBase::handleQuit();
              break;
          }

      case ErrorDialog::RESTART_PROGRAM:
          // Restart program
          AppsBase::handleRestart();
          break;

      case ErrorDialog::EXIT_PROGRAM:
      default:
          // Exit program
             AppsBase::handleQuit();
     }
    }
    QVariantList vl = QVariantList() << (list!=nullptr? tr("The following errors occurred in the order listed:"): "") << VPtr<JList>::asQVariant(list) << "<html><br></html>" << tr("Please check the logs for more details.") << tr("The Preferences window will open so this can be fixed.");
    JOptionPane::showMessageDialog(nullptr,
//         new Object[]{
//             (list instanceof JList) ? tr("InitExMessageListHeader") : nullptr,
//             list,
//             "<html><br></html>", // Add a visual break between list of errors and notes // NOI18N
//             tr("InitExMessageLogs"), // NOI18N
//             tr("InitExMessagePrefs"), // NOI18N
//         },
         vl,
         tr("Error initializing %1").arg(QApplication::applicationDisplayName()), // NOI18N
         JOptionPane::ERROR_MESSAGE);
 (new TabbedPreferencesAction())->actionPerformed();

   }
   if (!file.isEmpty() && (File(file.toDisplayString())).getName() == ("ProfileConfig.xml"))
   { // NOI18N
    log->debug("Loading legacy configuration...");
    return this->legacy->load(file, registerDeferred);
   }
   return this->initializationExceptions->isEmpty();
  }
 }
 catch (/*URISyntax*/Exception ex)
 {
     log->error(tr("Unable to get File* for %1").arg(file.toDisplayString()));
     throw JmriException(ex.getMessage()+ex.getLocalizedMessage());
 }
 return this->legacy->load(file, registerDeferred);
 // return true; // always return true once legacy support is dropped
}

//@Override
/*public*/ bool JmriConfigurationManager::loadDeferred(File* file)  throw (JmriException)
{
    return this->legacy->loadDeferred(file);
}

//@Override
/*public*/ bool JmriConfigurationManager::loadDeferred( QUrl file)  throw (JmriException)
{
    return this->legacy->loadDeferred(file);
}

//@Override
/*public*/  QUrl JmriConfigurationManager::find(QString filename) {
    return this->legacy->find(filename);
}

//@Override
/*public*/ bool JmriConfigurationManager::makeBackup(File* file) {
    return this->legacy->makeBackup(file);
}

/*private*/ void JmriConfigurationManager::initializeProvider( PreferencesManager* provider, Profile* profile)
{
 if (!provider->isInitialized(profile) && !provider->isInitializedWithExceptions(profile))
 {
  log->debug(tr("Initializing provider %1").arg(provider->metaObject()->className()));
//        foreach (QString extends PreferencesManager> c : provider.getRequires()) {
//            InstanceManager.getList(c).stream().forEach((p) -> {
//                this.initializeProvider(p, profile);
//            });
//        }
  foreach(QString classname, *provider->getRequires())
  {
   PreferencesManager* p = (PreferencesManager*) InstanceManager::getDefault(classname);
   if(p != NULL)
   this->initializeProvider(p,profile);
  }
  try {
      provider->initialize(profile);
  }
  catch (InitializationException ex)
  {
   // log all initialization exceptions, but only retain for GUI display the
   // first initialization exception for a provider
   //            InitializationException put = this->initializationExceptions->putIfAbsent(provider, ex);
   bool bPut = this->initializationExceptions->contains(provider);
   if(!bPut)
    this->initializationExceptions->insert((PreferencesManager*)provider, &ex);
   log->error(tr("Exception initializing %1: %2").arg(provider->metaObject()->className()).arg(ex.getMessage()));
   if (bPut)
   {
       log->error(tr("Additional exception initializing %1: %1").arg(provider->metaObject()->className()).arg(ex.getMessage()));
   }
  }
  this->initialized->append(provider);
  log->debug(tr("Initialized provider %1").arg(provider->metaObject()->className()));
 }
}

/*public*/ QHash<PreferencesManager*, InitializationException*> JmriConfigurationManager::getInitializationExceptions() {
    return QHash<PreferencesManager*, InitializationException*> (*initializationExceptions);
}

//@Override
/*public*/ void JmriConfigurationManager::setValidate(Validate v) {
    legacy->setValidate(v);
}

//@Override
/*public*/ XmlFile::Validate JmriConfigurationManager::getValidate() {
    return legacy->getValidate();
}



//private static final class ErrorDialog extends JDialog {

//    enum Result {
//        EXIT_PROGRAM,
//        RESTART_PROGRAM,
//        NEW_PROFILE,
//        EDIT_CONNECTIONS,
//    }


//    Result result = Result.EXIT_PROGRAM;

ErrorDialog::ErrorDialog(QStringList list) : JDialog()
{
    //super();
    result = Result::EXIT_PROGRAM;

    setTitle(tr("JMRI is unable to connect"));
    setModal(true);
    QWidget* contentPanel = new QWidget();
    //contentPanel.setLayout(new BoxLayout(contentPanel, BoxLayout.Y_AXIS));
    QVBoxLayout* contentPanelLayout = new QVBoxLayout(contentPanel);
    QWidget* panel = new QWidget();
    QHBoxLayout* panelLayout = new QHBoxLayout(panel);
    panelLayout->addWidget(new QLabel(tr("The following errors occurred in the order listed:")));
    contentPanelLayout->addWidget(panel);

    QWidget* marginPanel = new QWidget();
    QVBoxLayout* marginPanelLayout = new QVBoxLayout(marginPanel);
//    marginPanel.setAlignmentX(java.awt.Component.CENTER_ALIGNMENT);
//    marginPanel.setBorder(javax.swing.BorderFactory.createEmptyBorder(5,5,5,5));
    contentPanelLayout->addWidget(marginPanel);
    QFrame* borderPanel = new QFrame();
    QVBoxLayout* borderPanelLayout= new QVBoxLayout(borderPanel);
//    borderPanel.setAlignmentX(java.awt.Component.CENTER_ALIGNMENT);
//    borderPanel.setBorder(javax.swing.BorderFactory.createLineBorder(java.awt.Color.black));
    marginPanelLayout->addWidget(borderPanel);
    panel = new QWidget();
//    panel.setAlignmentX(java.awt.Component.CENTER_ALIGNMENT);
//    panel.setLayout(new BoxLayout(panel, BoxLayout.Y_AXIS));
//    panel.setBorder(javax.swing.BorderFactory.createEmptyBorder(5,5,5,5));
    for (QString s : list) {
        // Remove html
        s = s.replace("\\<html\\>.*\\<\\/html\\>", "");
        QLabel* label = new QLabel(s);
        //label.setAlignmentX(java.awt.Component.CENTER_ALIGNMENT);
        panelLayout->addWidget(label, 0, Qt::AlignHCenter);
    }
    borderPanelLayout->addWidget(panel);

    panel = new QWidget();
    QPushButton* button = new QPushButton(tr("Exit program"));
//    button.addActionListener((ActionEvent a) -> {
//        result = Result.EXIT_PROGRAM;
//        dispose();
//    });
    connect(button, SIGNAL(clicked()), this, SLOT(onExitProgram()));
    panelLayout->addWidget(button);

    button = new QPushButton(tr("Restart program"));
//    button.addActionListener((ActionEvent a) -> {
//        result = Result.RESTART_PROGRAM;
//        dispose();
//    });
    connect(button, SIGNAL(clicked()), this, SLOT(onRestartProgram()));
    panelLayout->addWidget(button);

    button = new QPushButton(tr("Start with new profile"));
//    button.addActionListener((ActionEvent a) -> {
//        result = Result.NEW_PROFILE;
//        dispose();
//    });
    connect(button, SIGNAL(clicked()), this, SLOT(onRestartProgram()));
    panelLayout->addWidget(button);

    button = new QPushButton(tr("Edit connections"));
//    button.addActionListener((ActionEvent a) -> {
//        result = Result.EDIT_CONNECTIONS;
//        dispose();
//    });
    connect(button, SIGNAL(clicked()), this, SLOT(onRestartProgram()));
    panelLayout->addWidget(button);

    contentPanelLayout->addWidget(panel);

    //setContentPane(contentPanel);
    setLayout(new QVBoxLayout());
    layout()->addWidget(contentPanel);
    pack();

    // Center dialog on screen
    setLocationRelativeTo(nullptr);
    setVisible(true);
}

void ErrorDialog::onExitProgram()
{
 result = Result::EXIT_PROGRAM;
 dispose();
}

void ErrorDialog::onRestartProgram()
{
 result = Result::RESTART_PROGRAM;
 dispose();
}

void ErrorDialog::onNewProfile()
{
 result = Result::NEW_PROFILE;
 dispose();
}
void ErrorDialog::onEditConnections()
{
 result = Result::EDIT_CONNECTIONS;
 dispose();
}
