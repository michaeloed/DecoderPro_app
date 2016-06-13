#ifndef FAMILYITEMPANEL_H
#define FAMILYITEMPANEL_H
#include "itempanel.h"
#include <QPushButton>
#include "actionlistener.h"
#include "logger.h"

class DragJLabel;
class DataFlavor;
class NamedIcon;
class FamilyItemPanel : public ItemPanel
{
    Q_OBJECT
public:
    //explicit FamilyItemPanel(QWidget *parent = 0);
    /*public*/ FamilyItemPanel(JmriJFrame* parentFrame, QString type, QString family, Editor* editor, QWidget* parent = 0);
    /*public*/ void init() ;
    /*public*/ void init(ActionListener* doneAction, QHash<QString, NamedIcon*>* iconMap) ;
    /*public*/ void virtual init(ActionListener* doneAction);
    /*public*/ virtual QHash<QString, NamedIcon*>* getIconMap();

signals:

public slots:
    void on_showIconsButton_clicked();
    void on_newFamilyButton_clicked();
private:
    QWidget*    _bottom1Panel;  // Typically _showIconsButton and editIconsButton
    QWidget*    _bottom2Panel;  // createIconFamilyButton - when all families deleted
    QPushButton*   _showIconsButton;
    QPushButton*   _updateButton;
    /*private*/ void checkCurrentMap(QHash<QString, NamedIcon*>* iconMap);
Logger* log;
private slots:
    /*private*/ void createNewFamilySet(QString type) ;

protected:
    /*protected*/ QWidget*    _iconFamilyPanel;
    /*protected*/ QWidget*    _iconPanel;     // panel contained in _iconFamilyPanel - all icons in family
    /*protected*/ QWidget*    _dragIconPanel; // contained in _iconFamilyPanel - sample to drag to control panel
    /*protected*/ QHash<QString, NamedIcon*>* _currentIconMap;
    /*protected*/ void makeBottomPanel(ActionListener* doneAction);
    /*protected*/ virtual void initIconFamiliesPanel();
    /*protected*/ QWidget* makeFamilyButtons (QStringListIterator it, bool setDefault);
    /*protected*/ void addFamilyPanels(QWidget* familyPanel);
    /*protected*/ void addCreatePanels() ;
    /*protected*/ void addIconsToPanel(QHash<QString, NamedIcon*>* iconMap);
    /*abstract*/ virtual /*protected*/ DragJLabel* getDragger(DataFlavor* /*flavor*/, QHash <QString, NamedIcon*>* /*map*/) {return NULL;}
    /*protected*/ virtual void makeDndIconPanel(QHash<QString, NamedIcon*>* iconMap, QString displayKey);
    /*protected*/ virtual QWidget* makeBottom1Panel();
    /*protected*/ void hideIcons();
    /*protected*/ void showIcons();
    /*protected*/ virtual QWidget* makeBottom2Panel();
    /*protected*/ virtual QWidget* makeBottom3Panel(ActionListener* doneAction, QWidget* bottom1Panel) ;
    /*protected*/ void removeIconFamiliesPanel();
    /*protected*/ virtual void setFamily(QString family) ;
protected slots:
    /*protected*/ void openEditDialog();
    /*protected*/ void updateFamiliesPanel();

friend class ItemPalette;
friend class TableItemPanel;
friend class SignalHeadItemPanel;
friend class ButtonListener;
friend class SignalMastItemPanel;
friend class MemoryItemPanel;
friend class ReporterItemPanel;
friend class IndicatorTOItemPanel;
friend class IconDialog;
};
class ButtonListener : public QObject
{
 Q_OBJECT
 QString family;
 FamilyItemPanel* self;
public slots:
 /*public*/ void actionPerformed();
 ButtonListener* init(QString f, FamilyItemPanel* self);
};

#endif // FAMILYITEMPANEL_H
