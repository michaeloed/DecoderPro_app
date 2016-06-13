#ifndef CONNECTIONPREFERENCESPANEL_H
#define CONNECTIONPREFERENCESPANEL_H
#include "managingpreferencespanel.h"
#include "logger.h"
#include "actionlistener.h"
#include "libpref_global.h"
class QToolButton;
class MyTabWidget;
class QPushButton;
class QCheckBox;
class TabbedPreferences;
class ImageIcon;
class JmrixConfigPane;
class LIBPREFSHARED_EXPORT ConnectionsPreferencesPanel : public ManagingPreferencesPanel
{
    Q_OBJECT
public:
    explicit ConnectionsPreferencesPanel(QWidget *parent = 0);
    /*public*/ ConnectionsPreferencesPanel(TabbedPreferences* preference, QWidget* parent=0);
    ~ConnectionsPreferencesPanel() {}
    ConnectionsPreferencesPanel(const ConnectionsPreferencesPanel &); //: PreferencesPanel(parent)
    virtual
    /*public*/ QString getPreferencesItem();
    virtual
    /*public*/ QString getPreferencesItemText();
    virtual
    /*public*/ QString getTabbedPreferencesTitle();
    virtual
    /*public*/ QString getLabelKey();
    virtual
    /*public*/ QWidget* getPreferencesComponent() ;
    virtual
    /*public*/ bool isPersistant();
    virtual
    /*public*/ QString getPreferencesTooltip() ;
    virtual
    /*public*/ void savePreferences() ;
    virtual
    /*public*/ bool isDirty() ;
    virtual
    /*public*/ bool isRestartRequired();
    /*public*/ bool isPreferencesValid() ;
    /*public*/ QList<PreferencesPanel*>* getPreferencesPanels();

signals:

public slots:
    void On_currentChanged(int);
private:
    MyTabWidget* tabWidget;
    /*private*/ /*final*/ TabbedPreferences* preferences;
    /*private*/ /*final*/ QIcon deleteIcon;
    /*private*/ /*final*/ QIcon deleteIconRollOver;
    /*private*/ /*final*/ QSize deleteButtonSize;
    /*private*/ QIcon addIcon;
    /*private*/ bool restartRequired;// = false;

    /*private*/ QList<JmrixConfigPane*> configPanes;// = QList<JmrixConfigPane*>();
    void common(TabbedPreferences* preferences);
    /*private*/ void activeTab();
    /*private*/ void addConnection(int tabPosition, /*final*/ JmrixConfigPane* configPane);
    void addConnectionTab();
    /*private*/ void newConnectionTab() ;
    /*private*/ void removeTab(/*ActionEvent* e,*/ int x);
Logger* log;
    int indexOfTab(QString text);
    bool bDeleteFlag;
 friend class CloseButtonListener;
};
class DisableCheckboxListener : public ActionListener
{
    Q_OBJECT
    JmrixConfigPane* configPane;
    QCheckBox* checkBox;
public:
    DisableCheckboxListener(JmrixConfigPane* configPane, QCheckBox* checkBox);
public slots:
    void actionPerformed(ActionEvent * = 0);
};
class CloseButtonListener : public ActionListener
{
    Q_OBJECT
    int index;
    ConnectionsPreferencesPanel* parent;
public:
    CloseButtonListener(int index, ConnectionsPreferencesPanel* parent);
public slots:
    void actionPerformed(ActionEvent * = 0);
};
class MyTabWidget: public QTabWidget
{
 Q_OBJECT

 public:
  MyTabWidget(QWidget *parent = 0);
  void setTabButton(int, QToolButton*);
};
#endif // CONNECTIONPREFERENCESPANEL_H
