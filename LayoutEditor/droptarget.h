#ifndef DROPTARGET_H
#define DROPTARGET_H

#include <QObject>
#include "exceptions.h"
#include "dndconstants.h"


class DropTargetDropEvent;
class DropTargetEvent;
class ComponentPeer;
class DropTargetContext;
class Component;
class FlavorMap;
class DropTargetListener;
class DropTarget : public QObject
{
    Q_OBJECT
public:
    //explicit DropTarget(QObject *parent = 0);
//    /*public*/ DropTarget(QWidget* c, DropTargetListener dtl, QObject *parent)
//throw (HeadlessException);
    /*public*/ DropTarget(Component* c, int ops, DropTargetListener* dtl, bool act = DnDConstants::ACTION_NONE, FlavorMap* fm = NULL) throw (HeadlessException);
//throw (HeadlessException);
    /*public*/ /*synchronized*/ Component* getComponent();
    /*public*/ /*synchronized*/ void addDropTargetListener(DropTargetListener* dtl) throw (TooManyListenersException);
    /*public*/ /*synchronized*/ void removeDropTargetListener(DropTargetListener* dtl);
    /*public*/ /*synchronized*/ void setActive(bool isActive);
    /*public*/ bool isActive();
    /*public*/ void setDefaultActions(int ops);
    void doSetDefaultActions(int ops);
    /*public*/ int getDefaultActions();
    /*public*/ DropTargetContext* getDropTargetContext();
    /*public*/ void removeNotify(ComponentPeer* peer);
    /*public*/ /*synchronized*/ void dragExit(DropTargetEvent* dte);
    /*public*/ /*synchronized*/ void drop(DropTargetDropEvent* dtde);
    /*public*/ FlavorMap* getFlavorMap();
    /*public*/ void setFlavorMap(FlavorMap* fm);
    /*public*/ void addNotify(ComponentPeer* peer);
    /*public*/ /*synchronized*/ void setComponent(Component* c);

signals:

public slots:
private:
    Component* component;
    FlavorMap* flavorMap;
    bool active;
    DropTargetContext* dropTargetContext;
    DropTargetListener* dtListener;
    ComponentPeer* componentPeer;
    ComponentPeer* nativePeer;
    bool isDraggingInside;
protected:
    /*protected*/ DropTargetContext* createDropTargetContext();
    int actions;
};

#endif // DROPTARGET_H
