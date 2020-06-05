#ifndef ICONDIALOG_H
#define ICONDIALOG_H
#include <itemdialog.h>
#include "logger.h"

class JPanel;
class QLabel;
class ImagePanel;
class JTextField;
class CatalogPanel;
class NamedIcon;
class IconDialog : public ItemDialog
{
    Q_OBJECT
public:
    //explicit IconDialog(QWidget *parent = 0);
    /*public*/ IconDialog(QString type, QString family, FamilyItemPanel *parent, QMap <QString, NamedIcon*>* iconMap );
signals:

public slots:
    void doneAction();
    void addFamilyButtonAction();
    void deleteButtonAction();
    /*public*/ void dispose();


private:
    /*private*/ bool		_newIconSet;// = false;
    Logger* log;
    /*private*/ CatalogPanel* makeCatalog();
    void checkIconSizes();

protected:
    /*protected*/ QMap <QString, NamedIcon*>*  _iconMap;
//    /*protected*/ QWidget*       _iconPanel;
    /*protected*/ ImagePanel*    _iconEditPanel;
    /*protected*/ CatalogPanel*  _catalog;
    /*private*/ /*final*/ QLabel* _nameLabel;
    /*protected*/ JTextField*    _familyName;
    /*protected*/ QPushButton*   _addFamilyButton;
    /*protected*/ QPushButton*   _deleteButton;
    /*protected*/ void makeAddSetButtonPanel(QWidget* buttonPanel);
    // Only multiSensor adds and deletes icons
    /*protected*/ virtual void makeAddIconButtonPanel(JPanel* buttonPanel, QString addTip, QString deleteTip);
    /*protected*/ virtual bool doDoneAction();
    /*protected*/ void addFamilySet();
    /*protected*/ void deleteFamilySet();
    /*protected*/ virtual void makeDoneButtonPanel(QWidget* buttonPanel, QMap<QString, NamedIcon *> *iconMap);
    /*protected*/ void makeDoneButtonPanel(QWidget* buttonPanel, QString text);
    /*protected*/ void makeIconPanel(QMap<QString, NamedIcon*>* iconMap, ImagePanel* iconPanel);
    /*protected*/ QMap<QString, NamedIcon *> *clone(QMap<QString, NamedIcon *> *map);
    /*protected*/ ImagePanel* getIconEditPanel() ;
    /*protected*/ ImagePanel* getCatalogPreviewPanel();

protected slots:
    /*protected*/ void renameFamily();

    friend class FamilyItemPanel;
};

#endif // ICONDIALOG_H
