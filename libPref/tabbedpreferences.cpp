#include "tabbedpreferences.h"
#include <QList>
#include <QTabWidget>
#include <QBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QListWidget>
#include <QPushButton>
#include <QIcon>
#include "fileutil.h"
#include <QMetaObject>
#include "jseparator.h"
#include "preferencessubpanel.h"
#include "propertychangeevent.h"
#include <QMetaType>
#include "connectionspreferencespanel.h"
#include "programmerconfigpane.h"
#include "createbuttonmodel.h"
#include "performactionmodel.h"
#include "managerdefaultsconfigpane.h"
#include "filelocationpane.h"
#include "guilafconfigpane.h"
#include "throttlespreferencespane.h"
#include "usermessagepreferencespane.h"
#include <performfilepanel.h>
#include "rosterconfigpane.h"
#include "guilocalepreferencespanel.h"
#include "railroadnamepreferencespanel.h"
#include "profilepreferencespanel.h"
#include "signalheadtableaction.h"
#include "turnouttableaction.h"
#include "metatypes.h"
#include "loggerfactory.h"
#include "class.h"
#include "instancemanager.h"
#include "webserverpreferencespanel.h"
#include <QSplitter>

//TabbedPreferences::TabbedPreferences(QObject *parent) :
//    AppConfigBase(parent)
//{
//}
/**
 * Provide access to preferences via a tabbed pane.
 *
 * Preferences panels listed in the PreferencesPanel property of the
 * apps.AppsStructureBundle ResourceBundle will be automatically loaded if they
 * implement the {@link jmri.swing.PreferencesPanel} interface.
 *
 * Other Preferences Panels will need to be manually added to this file in a
 * manner similar to the WiThrottlePrefsPanel.
 *
 * @author Bob Jacobsen Copyright 2010
 * @author Randall Wood 2012
 * @version $Revision: 28419 $
 */
// /*public*/ class TabbedPreferences extends AppConfigBase {

bool preferencesCompare(QObject* o1, QObject* o2)
{
 PreferencesCatItems*  item1 = (PreferencesCatItems*)o1;
 PreferencesCatItems*  item2 = (PreferencesCatItems*)o2;
 //int comparison = item1->getSortOrder() - item2->getSortOrder();
 int comparison;
 if(item1->getSortOrder() < item2->getSortOrder())
  comparison = -1;
 else if(item1->getSortOrder() == item2->getSortOrder()) comparison = 0;
 else comparison = 1;

 //return (comparison != 0) ? comparison : item1->getPrefItem().compareTo(item2->getPrefItem());
 int ret = (comparison != 0) ? comparison : QString::compare(item1->getPrefItem(), item2->getPrefItem());
 return ret < 0;
}

bool tabDetailsCompare(QObject* o1, QObject* o2)
{
 TabDetails*  item1 = (TabDetails*)o1;
 TabDetails*  item2 = (TabDetails*)o2;
 //int comparison = item1->getSortOrder() - item2->getSortOrder();
 int comparison;
 if(item1->getSortOrder() < item2->getSortOrder())
  comparison = -1;
 else if(item1->getSortOrder() == item2->getSortOrder()) comparison = 0;
 else comparison = 1;
 //return (comparison != 0) ? comparison : item1->getPrefItem().compareTo(item2->getPrefItem());
 int ret = (comparison != 0) ? comparison : QString::compare(item1->getTitle(), item2->getTitle());
 return ret < 0;
}



//@Override
/*public*/ QString TabbedPreferences::getHelpTarget() {
    return "package.apps.TabbedPreferences";
}

//@Override
/*public*/ QString TabbedPreferences::getTitle() {
    return tr("Preferences");
}

//@Override
/*public*/ bool TabbedPreferences::isMultipleInstances() {
    return false;
} // only one of these!

// only one of these!
/*public*/ /*static*/ /*final*/ QString TabbedPreferences::INITIALIZATION = "PROP_INITIALIZATION";


/*public*/ TabbedPreferences::TabbedPreferences(QWidget *parent)
    : AppConfigBase(parent)
{
 preferencesElements =  QList<QDomElement>();
 preferencesArray = QList<PreferencesCatItems*>();
 initialisationState = UNINITIALISED;
 log->setDebugEnabled(true);
 widgetIndexes = QMap<QString, int>();

 /*
  * Adds the place holders for the menu items so that any items add by
  * third party code is added to the end
  */
  preferencesArray.append(new PreferencesCatItems("CONNECTIONS", tr("Connections"), 100, this));

  preferencesArray.append(new PreferencesCatItems("DEFAULTS", tr("Defaults"), 200, this));

  preferencesArray.append(new PreferencesCatItems("FILELOCATIONS", tr("File Location"), 300, this));

  preferencesArray.append(new PreferencesCatItems("STARTUP", tr("StartUp"), 400, this));

  preferencesArray.append(new PreferencesCatItems("DISPLAY", tr("Display"), 500, this));

  preferencesArray.append(new PreferencesCatItems("MESSAGES", tr("Messages"), 600, this));

  preferencesArray.append(new PreferencesCatItems("ROSTER", tr("Roster"), 700, this));

  preferencesArray.append(new PreferencesCatItems("THROTTLE", tr("Throttle"), 800, this));

  preferencesArray.append(new PreferencesCatItems("WITHROTTLE",tr("WiThrottle"), 900, this));
  new Metatypes();
  //init();  // can't do this here.
}

//@SuppressWarnings("rawtypes")
/*public*/ /*synchronized*/ int TabbedPreferences::init()
{
 if (initialisationState == INITIALISED)
 {
  return INITIALISED;
 }
 if (initialisationState != UNINITIALISED)
 {
  return initialisationState;
 }
// return UNINITIALISED;
//}
//int TabbedPreferences::startInit()
//{
 this->setInitalisationState(INITIALISING);
 setWindowTitle(getTitle());

 list = new QListWidget();
//    listScroller = new JScrollPane(list);
//    listScroller.setPreferredSize(new Dimension(100, 100, this));

 buttonpanel = new QWidget();
 buttonpanel->setMaximumWidth(200);
 QVBoxLayout* buttonPanelLayout;
 buttonpanel->setLayout(buttonPanelLayout = new QVBoxLayout); //(buttonpanel, BoxLayout.Y_AXIS));
//    buttonpanel.setBorder(BorderFactory.createEmptyBorder(6, 6, 6, 3));

//    save = new QPushButton(
//            tr("ButtonSave"),
//            new ImageIcon(
//                    FileUtil::findExternalFilename("program:resources/icons/misc/gui3/SaveIcon.png")));
 save = new QPushButton( QIcon(FileUtil::getExternalFilename(":/resources/icons/misc/gui3/SaveIcon.png")),tr("Save"));
//    save.addActionListener((ActionEvent e) -> {
//        savePressed(invokeSaveOptions());
//    });
 connect(save, SIGNAL(clicked()), this, SLOT(On_save_clicked()));
 setMinimumSize(400, 300);
 splitter = new QSplitter();

 // panels that are dependent upon another panel being added first
         QSet<PreferencesPanel*> delayed = QSet<PreferencesPanel*>();

//    try
//    {
//      QStringList classNames = (new ObjectMapper()).readValue(
//                ResourceBundle.getBundle("apps.AppsStructureBundle").getString("PreferencesPanels"),
//                new TypeReference<List<String>>() {
//                });
#if 0
 [ "jmri.jmrix.swing.ConnectionsPreferencesPanel", "apps.ManagerDefaultsConfigPane",
         "apps.FileLocationPane", "apps.PerformActionPanel", /*"apps.CreateButtonPanel",*/
         "apps.PerformFilePanel", "apps.PerformScriptPanel", "apps.GuiLafConfigPane",
         "apps.GuiLocalePreferencesPanel", "apps.SystemConsoleConfigPanel",
         "jmri.jmrit.beantable.usermessagepreferences.UserMessagePreferencesPane",
         "jmri.jmrit.symbolicprog.ProgrammerConfigPane", "jmri.jmrit.roster.RosterConfigPane",
         "jmri.jmrit.throttle.ThrottlesPreferencesPane", "jmri.jmrit.withrottle.WiThrottlePrefsPanel",
         "jmri.profile.ProfilePreferencesPanel", "jmri.jmris.json.JsonServerPreferencesPanel",
         "jmri.web.server.RailroadNamePreferencesPanel", "jmri.web.server.WebServerPreferencesPanel",
         "jmri.jmrit.logix.WarrantPreferencesPanel" ]
#endif
 QStringList classNames = QStringList()
  << QString("jmri.jmrix.swing.ConnectionsPreferencesPanel") << QString( "apps.ManagerDefaultsConfigPane")
  << QString( "apps.FileLocationPane" )
  << QString( "apps.PerformFilePanel") << QString("apps.GuiLafConfigPane")
  << QString("apps.GuiLocalePreferencesPanel") << QString( "apps.SystemConsoleConfigPanel")
  << QString("jmri.jmrit.beantable.usermessagepreferences.UserMessagePreferencesPane")
  << QString( "jmri.jmrit.symbolicprog.ProgrammerConfigPane")
  << QString("jmri.web.server.WebServerPreferencesPanel")<< QString("jmri.jmrit.roster.RosterConfigPane")
  << QString( "jmri.jmrit.throttle.ThrottlesPreferencesPane") << QString("jmri.jmrit.throttle.WiThrottlePrefsPanel")
  << QString( "jmri.profile.ProfilePreferencesPanel") << QString("apps.startup.StartupActionsPreferencesPanel")
  << QString("jmri.jmrix.loconet.LnTcpPreferencesPanel") << QString("jmri.server.json.JsonServerPreferencesPanel")
  << QString( "jmri.web.server.RailroadNamePreferencesPanel")   << QString("jmri.jmrit.logix.WarrantPreferencesPanel")
  ;

 // add preference panels registered with the Instance Manager
 for (QObject* panel : *InstanceManager::getList("PreferencesPanel"))
 {
  if (qobject_cast<PreferencesSubPanel*>(panel))
  {
   QString parent = ((PreferencesSubPanel*) panel)->getParentClassName();
   if (!this->getPreferencesPanels()->contains(parent))
   {
    delayed.insert((PreferencesPanel*)panel);
   }
   else
   {
    ((PreferencesSubPanel*) panel)->setParent(this->getPreferencesPanels()->value(parent));
   }
  }
  if (!delayed.contains((PreferencesPanel*)panel))
  {
   this->addPreferencesPanel((PreferencesPanel*)panel);
  }
 }

 foreach (QString className,classNames)
 {
  try
  {
   QObject* panel = NULL;

   //PreferencesPanel
    panel = (QObject*) Class::forName(className)->newInstance();
   //if (panel instanceof PreferencesSubPanel)
   if(qobject_cast<PreferencesSubPanel*>(panel)!=NULL)
   {
    QString parent = qobject_cast<PreferencesSubPanel*>( panel)->getParentClassName();
    if (!this->getPreferencesPanels()->contains(parent))
    {
     delayed.insert(qobject_cast<PreferencesPanel*>(panel));
    }
    else
    {
     qobject_cast<PreferencesSubPanel*>(panel)->setParent(this->getPreferencesPanels()->value(parent));
    }
   }
   if (!delayed.contains(qobject_cast<PreferencesPanel*>(panel))) {
       this->addPreferencesPanel(qobject_cast<PreferencesPanel*>(panel));
   }
  }
  //catch (ClassNotFoundException | InstantiationException | IllegalAccessException e) {
  catch(ClassNotFoundException e)
  {
    log->error("Unable to add preferences class (" + className + ")"/*, e*/);
  }
  catch(InstantiationException e)
  {
    log->error("Unable to add preferences class (" + className + ")"/*, e*/);
  }
 }

 while (!delayed.isEmpty())
 {
  QSet<PreferencesPanel*> iterated = QSet<PreferencesPanel*>(delayed);
  //iterated.stream().filter((panel) -> (panel instanceof PreferencesSubPanel)).forEach((panel) ->
  foreach(PreferencesPanel* panel, iterated)
  {
      QString parent = qobject_cast<PreferencesSubPanel*>(panel->self())->getParentClassName();
      if (this->getPreferencesPanels()->contains(parent)) {
          qobject_cast<PreferencesSubPanel*>( (QObject*)panel)->setParent(this->getPreferencesPanels()->value(parent));
          delayed.remove(panel);
          this->addPreferencesPanel(panel);
      }
  }//);
 }

 QVBoxLayout* thisLayout = new QVBoxLayout(this);
 this->layout()->addWidget(splitter);
 splitter->addWidget(buttonpanel); //widgetIndex 0.

 int widgetIndex = 1;
 foreach (PreferencesCatItems* preferences, preferencesArray)
 {
  splitter->addWidget(preferences->getPanel());
  log->debug(tr("add panel %1 class %2 index %3").arg(preferences->getPrefItem()).arg(preferences->getPanel()->metaObject()->className()).arg(widgetIndex));
  widgetIndexes.insert(preferences->getPrefItem(), widgetIndex);
  splitter->setStretchFactor(widgetIndex++,2);
  preferences->getPanel()->hide();
  preferences->getPanel()->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
 }

// preferencesArray.sort((PreferencesCatItems o1, PreferencesCatItems o2) -> {
//     int comparison = Integer.compare(o1.sortOrder, o2.sortOrder);
//     return (comparison != 0) ? comparison : o1.getPrefItem().compareTo(o2.getPrefItem());
// });
 qSort(preferencesArray.begin(), preferencesArray.end(), preferencesCompare);

 updateJList();
 QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
 sizePolicy.setHorizontalStretch(1);
 sizePolicy.setVerticalStretch(1);
 sizePolicy.setHeightForWidth(false); //this->sizePolicy().hasHeightForWidth());
 this->setSizePolicy(sizePolicy);

 list->setCurrentRow(0);
// selection(preferencesArray.get(0).getPrefItem());
 this->setInitalisationState(INITIALISED);
 return initialisationState;
}

/*public*/ bool TabbedPreferences::isPreferencesValid() {
//    return this->getPreferencesPanels().values().stream().allMatch((panel) -> (panel.isPreferencesValid()));
 bool ret = true;
 foreach(PreferencesPanel* panel, this->getPreferencesPanels()->values())
 {
  if(! panel->isPreferencesValid())
     ret = false;
 }
 return ret;
}

void TabbedPreferences::On_save_clicked()
{
 savePressed(invokeSaveOptions());
}

/*private*/ void TabbedPreferences::setInitalisationState(int state)
{
    int old = this->initialisationState;
    this->initialisationState = state;
    //this->firePropertyChange(INITIALIZATION, old, state);
    emit propertyChange(new PropertyChangeEvent(this,INITIALIZATION, old, state));
}

/*public*/ int TabbedPreferences::getInitialisationState() {
    return this->initialisationState;
}

/*public*/ bool TabbedPreferences::isInitialised() {
    return (this->getInitialisationState() == INITIALISED);
}

// package only - for TabbedPreferencesFrame
bool TabbedPreferences::isDirty()
{
 foreach (PreferencesPanel* panel, this->getPreferencesPanels()->values())
 {
  if (log->isDebugEnabled())
  {
   log->debug(tr("PreferencesPanel %1 (%2) is %3.").arg(
           panel->className()).arg(
           (panel->getTabbedPreferencesTitle() != "") ? panel->getTabbedPreferencesTitle() : panel->getPreferencesItemText()).arg(
           (panel->isDirty() ? "dirty" : "clean")));
  }
  if (panel->isDirty()) {
      return true;
  }
 }
 return false;
}

/*private*/ bool TabbedPreferences::invokeSaveOptions()
{
 bool restartRequired = false;
 foreach (PreferencesPanel* panel, this->preferencesPanels->values())
 {
  if (log->isDebugEnabled())
  {
   log->debug(QString("PreferencesPanel %1 (%2) is %3.").arg(
                    panel->className()).arg(
                    (panel->getTabbedPreferencesTitle() != "") ? panel->getTabbedPreferencesTitle() : panel->getPreferencesItemText()).arg(
                    (panel->isDirty())? "dirty" : "clean"));
  }
  panel->savePreferences();
  if (log->isDebugEnabled())
  {
   log->debug(QString("PreferencesPanel %1 (%2) restart is %3 required.").arg(
                    panel->className()).arg(
                    (panel->getTabbedPreferencesTitle() != "") ? panel->getTabbedPreferencesTitle() : panel->getPreferencesItemText()).arg(
                    (panel->isRestartRequired()) ? "" : "not "));
  }
  if (!restartRequired)
  {
   restartRequired = panel->isRestartRequired();
  }
 }
 return restartRequired;
}

/*public*/ void TabbedPreferences::setUIFontSize(float /*size*/) {
#if 0
    Enumeration<Object> keys = UIManager.getDefaults().keys();
    Font f;
    while (keys.hasMoreElements()) {
        Object key = keys.nextElement();
        Object value = UIManager.get(key);

        if (value instanceof FontUIResource) {
            f = UIManager.getFont(key).deriveFont(((Font) value).getStyle(), size);
            UIManager.put(key, f);
        }
    }
#endif
}
#if 0
/*public*/ void TabbedPreferences::setUIFont(FontUIResource f) {
    Enumeration<Object> keys = UIManager.getDefaults().keys();
    while (keys.hasMoreElements()) {
        Object key = keys.nextElement();
        Object value = UIManager.get(key);
        if (value instanceof FontUIResource) {
            UIManager.put(key, f);
        }
    }
}
#endif
void TabbedPreferences::selection(QString view)
{
//    CardLayout cl = (CardLayout) (detailpanel.getLayout());
//    cl.show(detailpanel, View);
// for(int i=0; i < detailpanel->count(); i++)
// for(int i=0; i < preferencesArray.count(); i++)
// {
//  if(preferencesArray.at(i)->getPrefItem() == view)
//  {
//   //detailpanel->setCurrentIndex(i);
//   widget(currSplitterWidget +1)->hide();
//   widget(i+1)->show();
//   currSplitterWidget = i;
//   return;
//  }
// }
 if(widgetIndexes.contains(view))
 {
     splitter->widget(currSplitterWidget)->hide();
     currSplitterWidget = widgetIndexes.value(view);
     splitter->widget(currSplitterWidget)->show();
 }
}

/*public*/ void TabbedPreferences::addPreferencesPanel(PreferencesPanel* panel)
{
 this->preferencesPanels->insert(panel->className(), panel);

 QString preferenceItem = panel->getPreferencesItem();
 Q_UNUSED(preferenceItem);
 QString preferencesItemText = panel->getPreferencesItemText();
 Q_UNUSED(preferencesItemText);
 QString tabbedPreferencesTitle = panel->getTabbedPreferencesTitle();
 Q_UNUSED(tabbedPreferencesTitle);
 QString labelKey = panel ->getLabelKey();
 Q_UNUSED(labelKey);
 bool bPersistent = panel->isPersistant();
 Q_UNUSED(bPersistent);
 QString toolTip = panel->getPreferencesTooltip();
 Q_UNUSED(toolTip);

 addItem(panel->getPreferencesItem(),
            panel->getPreferencesItemText(),
            panel->getTabbedPreferencesTitle(),
            panel->getLabelKey(),
            panel,
            panel->getPreferencesTooltip(),
            panel->getSortOrder(),
            panel->className()
 );
}

/*private*/ void TabbedPreferences::addItem(QString prefItem, QString itemText, QString tabtitle, QString labelKey,
                                            PreferencesPanel* item, QString tooltip, int sortOrder, QString className)
{
 PreferencesCatItems* itemBeingAdded = NULL;
 foreach (PreferencesCatItems* preferences, preferencesArray)
 {
  if (preferences->getPrefItem()==(prefItem))
  {
   itemBeingAdded = preferences;
   // the lowest sort order of any panel sets the sort order for
   // the preferences category
   if (sortOrder < preferences->getSortOrder()) {
       preferences->setSortOrder(sortOrder);
   }
   break;
  }
 }
 if (itemBeingAdded == NULL)
 {
  itemBeingAdded = new PreferencesCatItems(prefItem, itemText, sortOrder, this);
  preferencesArray.append(itemBeingAdded);
  // As this is a new item in the selection list, we need to update
  // the JList.
  if (initialisationState == INITIALISED)
  {
   updateJList();
  }
 }
 if (tabtitle == NULL)
 {
     tabtitle = itemText;
 }
 itemBeingAdded->addPreferenceItem(tabtitle, labelKey, item->getPreferencesComponent(), tooltip, sortOrder,className);
}

/* Method allows for the preference to goto a specific list item */
/*public*/ void TabbedPreferences::gotoPreferenceItem(QString _selection, QString subCategory)
{
 selection(_selection);
 //list->setSelectedIndex(getCategoryIndexFromString(_selection));
 list->setCurrentRow(getCategoryIndexFromString(_selection));
 if (subCategory == "" || subCategory==(""))
 {
  return;
 }
 preferencesArray.value(getCategoryIndexFromString(_selection))
            ->gotoSubCategory(subCategory);
}

/*
 * Returns a List of existing Preference Categories.
 */
/*public*/ QStringList TabbedPreferences::getPreferenceMenuList()
{
 QStringList choices =  QStringList();
 foreach (PreferencesCatItems* preferences, preferencesArray)
 {
  choices.append(preferences->getPrefItem());
 }
 return choices;
}

/*
 * Returns a list of Sub Category Items for a give category
 */
/*public*/ QStringList TabbedPreferences::getPreferenceSubCategory(QString category) {
 int index = getCategoryIndexFromString(category);
 return preferencesArray.value(index)->getSubCategoriesList();
}

int TabbedPreferences::getCategoryIndexFromString(QString category)
{
 for (int x = 0; x < preferencesArray.size(); x++)
 {
  if (preferencesArray.at(x)->getPrefItem()==(category))
  {
   return (x);
  }
 }
 return -1;
}

/*public*/ void TabbedPreferences::disablePreferenceItem(QString selection, QString subCategory)
{
 if (subCategory == "" || subCategory==(""))
 {
  // need to do something here like just disable the item

 }
 else
 {
  preferencesArray.at(getCategoryIndexFromString(selection))->disableSubCategory(subCategory);
 }
}

/*protected*/ QStringList TabbedPreferences::getChoices()
{
 QStringList choices = QStringList();
 foreach (PreferencesCatItems* preferences, preferencesArray)
 {
  choices.append(preferences->getItemString());
 }
 return choices;
}

void TabbedPreferences::updateJList()
{
 //buttonpanel.removeAll();
 QObjectList oList = buttonpanel->children();
 foreach(QObject* o, oList)
 {
  if(qobject_cast<QWidget*>(o) != NULL)
  {
   buttonpanel->layout()->removeWidget((QWidget*)o);
   o->deleteLater();
  }
 }
// if (list->getListSelectionListeners().length() > 0)
// {
//  list->removeListSelectionListener(list->getListSelectionListeners()[0]);
// }
 list =  new QListWidget(); //new Vector<String>(getChoices()));
 //list->append(getChoices());
 list->addItems(getChoices());

//    listScroller = new QScrollArea(list);
//    listScroller->resize( QSize(100, 100, this));
 list->setSelectionMode(/*ListSelectionModel::SINGLE_INTERVAL_SELECTION*/QAbstractItemView::SingleSelection);
// list->setLayoutOrientation(JList.VERTICAL);
//    list.addListSelectionListener((ListSelectionEvent e) -> {
//        PreferencesCatItems item = preferencesArray.get(list.getSelectedIndex());
//        selection(item.getPrefItem());
//    });
 connect(list, SIGNAL(currentRowChanged(int)), this, SLOT(On_list_selection(int)));
 buttonpanel->layout()->addWidget(list);
 buttonpanel->layout()->addWidget(save);
}

void TabbedPreferences::On_list_selection(int row)
{
 if(row < 0)
  return;
 PreferencesCatItems* item = preferencesArray.at(list->currentRow());
 selection(item->getPrefItem());

}

PreferencesCatItems::PreferencesCatItems(QString pref, QString title, int sortOrder, TabbedPreferences *tabbedPreferences)
{
 tabbedPane = new JTabbedPane();
 disableItemsList = QStringList();
 tabDetailsArray = QList<TabDetails*>();
 prefItem = pref;
 itemText = title;
 this->sortOrder = sortOrder;
 this->tabbedPreferences = tabbedPreferences;
}

void PreferencesCatItems::addPreferenceItem(QString title, QString labelkey, QWidget* item, QString tooltip, int sortOrder, QString className)
{
  foreach (TabDetails* tabDetails, tabDetailsArray)
  {
   if (tabDetails->getTitle()==(title))
   {
    // If we have a match then we do not need to add it back in.
    return;
   }
  }
  TabDetails* tab = new TabDetails(labelkey, title, item, tooltip, sortOrder, className);
  tabDetailsArray.append(tab);
  qSort(tabDetailsArray.begin(), tabDetailsArray.end(), tabDetailsCompare);
  QScrollArea* scroller = new QScrollArea(/*tab->getPanel()*/);
  scroller->setWidget(tab->getPanel());
  scroller->setWidgetResizable(true);
  //scroller.setBorder(BorderFactory.createEmptyBorder());
  tabbedPane->addTab(tab->getTitle(), QIcon(), scroller, tab->getToolTip());

  foreach (QString disableItem, disableItemsList)
  {
   if (QString(item->metaObject()->className()) == disableItem)
   {
    tabbedPane->setTabEnabled(tabbedPane->indexOf(tab->getPanel()),false);
    return;
   }
  }
}

QString PreferencesCatItems::getPrefItem() {
 return prefItem;
}

QString PreferencesCatItems::getItemString() {
 return itemText;
}

QList<QString> PreferencesCatItems::getSubCategoriesList() {
 QList<QString> choices =  QList<QString>();
 foreach (TabDetails* tabDetails, tabDetailsArray) {
     choices.append(tabDetails->getTitle());
 }
 return choices;
}

/*
* This returns a JPanel if only one item is configured for a menu item
* or it returns a JTabbedFrame if there are multiple managedPreferences for the menu
*/
QWidget* PreferencesCatItems::getPanel()
{
 if (tabDetailsArray.size() == 1)
 {
  //return tabDetailsArray.at(0)->getPanel();
  return tabDetailsArray.at(0)->getItem();
 }
 else
 {
  if (tabbedPane->count() == 0)
  {
   for (TabDetails* tab : tabDetailsArray)
   {
    //ThreadingUtil.runOnGUI(() ->
    {
     QScrollArea* scroller = new QScrollArea();// tab.getPanel());
     scroller->setWidget(tab->getPanel());
     scroller->setWidgetResizable(true);
     //scroller.setBorder(BorderFactory.createEmptyBorder());

     tabbedPane->addTab(tab->getTitle(), QIcon(), scroller, tab->getToolTip());

     for (QString disableItem : disableItemsList)
     {
      if (tab->className() == (disableItem))
      {
       tabbedPane->setEnabledAt(tabbedPane->indexOfTab(tab->getTitle()), false);
       break;
      }
     }
    }//);
   }
  }
  return tabbedPane;
 }
}

void PreferencesCatItems::gotoSubCategory(QString sub) {
 if (tabDetailsArray.size() == 1) {
     return;
 }
 for (int i = 0; i < tabDetailsArray.size(); i++) {
     if (tabDetailsArray.at(i)->getTitle()==(sub)) {
         tabbedPane->setCurrentIndex(i);
         return;
     }
 }
}

void PreferencesCatItems::disableSubCategory(QString sub) {
 if (tabDetailsArray.isEmpty()) {
     // So the tab preferences might not have been initialised when
     // the call to disable an item is called therefore store it for
     // later on
     disableItemsList.append(sub);
     return;
 }
 for (int i = 0; i < tabDetailsArray.size(); i++) {
     if ((tabDetailsArray.at(i)->getItem())->metaObject()->className()
             == (sub)) {
         tabbedPane->setTabEnabled(i, false);
         return;
     }
 }
}
int PreferencesCatItems::getSortOrder() {return sortOrder;}
void PreferencesCatItems::setSortOrder(int sortOrder) { this->sortOrder = sortOrder;}

//        static class TabDetails implements java.io.Serializable {

//            /**
//             *
//             */
//            private static final long serialVersionUID = -7077354592762639878L;
//            /* This contains all the JPanels that make up a preferences menus */
//            JComponent tabItem;
//            String tabTooltip;
//            String tabTitle;
//            JPanel tabPanel = new JPanel();
//            //boolean store;

TabDetails::TabDetails(QString labelkey, QString tabTit, QWidget* item,
        QString tooltip, int sortOrder, QString className)
{
    tabItem = item;
    tabTitle = tabTit;
    tabTooltip = tooltip;
    tabPanel = new QWidget;
    this->sortOrder = sortOrder;
    this->_className = className;

    QWidget* p = new QWidget();
    //p.setLayout(new BorderLayout());
    QVBoxLayout* pLayout = new QVBoxLayout(p);
    if (labelkey != NULL)
    {
        // insert label at top
        // As this can be multi-line, embed the text within <html>
        // tags and replace newlines with <br> tag
        QLabel* t = new QLabel("<html>"
                + labelkey.replace(('\n'), "<br>")
                + "</html>");
        //t.setHorizontalAlignment(JLabel.CENTER);
        t->setAlignment(Qt::AlignHCenter);
        //t.setAlignmentX(0.5f);
        t->resize(t->sizeHint());
        t->setMaximumSize(t->sizeHint());
//                    t->setOpaque(false);
        pLayout->addWidget(t, /*BorderLayout.NORTH*/0,Qt::AlignTop);
    }
    pLayout->addWidget(item, /*BorderLayout.CENTER*/0, Qt::AlignVCenter);
    //tabPanel.setLayout(new BorderLayout());
    QVBoxLayout* tabPanelLayout = new QVBoxLayout(tabPanel);
    tabPanelLayout->addWidget(p, /*BorderLayout.CENTER*/0, Qt::AlignVCenter);
}

QString TabDetails::getToolTip() {
    return tabTooltip;
}

QString TabDetails::getTitle() {
    return tabTitle;
}

QWidget* TabDetails::getPanel() {
    return tabPanel;
}

QWidget* TabDetails::getItem() {
    return tabItem;
}
void TabDetails::setSortOrder(int sortOrder) { this->sortOrder = sortOrder;}
int TabDetails::getSortOrder() { return this->sortOrder;}

QString TabDetails::className() {return _className;}
//        }
//    }
/*private*/ /*final*/ /*static*/ Logger* TabbedPreferences::log = LoggerFactory::getLogger("TabbedPreferences");
