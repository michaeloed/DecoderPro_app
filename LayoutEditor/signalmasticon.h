#ifndef SIGNALMASTICON_H
#define SIGNALMASTICON_H
#include "positionableicon.h"
#include "QSignalMapper"
#include "namedbeanhandle.h"
#include <QHash>
#include "signalheadicon.h"

class SignalHeadIcon;
class SignalMastItemPanel;
class SignalMast;
//class NamedBeanHandle;
class NamedBean;
class PropertyChangeEvent;
class LIBLAYOUTEDITORSHARED_EXPORT SignalMastIcon : public PositionableIcon
{
    Q_OBJECT
public:
    explicit SignalMastIcon(QWidget *parent = 0);
    /*public*/ SignalMastIcon(Editor* editor, Positionable* parent = 0);
 ~SignalMastIcon() {}
 SignalMastIcon(const SignalMastIcon&) : PositionableIcon() {}
    /*public*/ void setShowAutoText(bool state);
    /*public*/ Positionable* deepClone();
    /*public*/ Positionable* finishClone(Positionable* p);
    /*public*/ void setSignalMast(NamedBeanHandle <SignalMast*>* sh);
    /*public*/ void setSignalMast(QString pName);
    /*public*/ NamedBeanHandle<SignalMast*>* getNamedSignalMast();
    /*public*/ SignalMast* getSignalMast();
    /*public*/ NamedBean* getNamedBean();
    /*public*/ QString mastState();
    // update icon as state of turnout changes
//    /*public*/ void propertyChange(PropertyChangeEvent* e); // is slot!
    /*public*/ QString getPName();
    /*public*/ QString getNameString();
    /*public*/ bool showPopUp(QMenu* popup);
    /*public*/ bool setRotateOrthogonalMenu(QMenu* popup);
    /*public*/ bool setEditItemMenu(QMenu* popup);
    void updateItem();
    /*public*/ void doMouseClicked(QGraphicsSceneMouseEvent* e) ;
    /*public*/ void performMouseClicked(QGraphicsSceneMouseEvent* e);
//    /*public*/ void useIconSet(QString icon);
    /*public*/ QString useIconSet();
    /*public*/ void displayState(int s);
    /*public*/ void displayState(QString state);
    /*public*/ bool setEditIconMenu(QMenu* popup);
    /*public*/ void rotate(int deg);
    /*public*/ void setScale(double s);
    /*public*/ void setClickMode(int mode);
    /*public*/ int getClickMode();
    /*public*/ void setLitMode(bool mode);
    /*public*/ bool getLitMode();
    /*public*/ void dispose();
    /*public*/ bool updateScene();

signals:

public slots:
    void setClickMode0();
    void setClickMode1();
    void setClickMode2();
    void setAspect(int);
    void propertyChange(PropertyChangeEvent *e);
    /*public*/ void useIconSet(QString icon);
    void onWhenNotLit();
    void onShowDarkIcon();

private:
    /*private*/ SignalMast* mMast;
    /*private*/ NamedBeanHandle<SignalMast*>* namedMast;
    /*private*/ bool debug;
    Logger* log;
    /*private*/ void getIcons();
    /*private*/ bool loadIcons(QString aspect);
    QString pName;
    /*private*/ void setImageTypeList(QSignalMapper* iconTypeGroup, QMenu* iconSetMenu, /*final*/ QString item);
    SignalMastItemPanel* _itemPanel;
    QActionGroup* litButtonGroup;
protected:
//    /*protected*/ void editItem();
    QString _useIconSet;// = "default";
    /*protected*/ void rotateOrthogonal();
    /**
     * What to do on click? 0 means
     * sequence through aspects; 1 means
     * alternate the "lit" aspect; 2 means
     * alternate the DefaultSignalAppearanceMap.HELD aspect.
     */
    /*protected*/ int clickMode;// = 0;
    /*protected*/ bool litMode;// = false;
    class AddIconActionListener : public ActionListener
    {
     SignalMastIcon* parent;
    public:
     AddIconActionListener(SignalMastIcon* parent)
     {
      this->parent = parent;
     }
     void actionPerformed(ActionEvent */*e*/ = 0)
     {
      parent->updateItem();
     }
    };
protected slots:
    /*protected*/ void editItem();

};
Q_DECLARE_METATYPE(SignalMastIcon)
#endif // SIGNALMASTICON_H
