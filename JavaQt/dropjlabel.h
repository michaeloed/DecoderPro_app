#ifndef DROPJLABEL_H
#define DROPJLABEL_H
#include <QLabel>
#include "logger.h"

class DropTargetDropEvent;
class DataFlavor;
class DropJLabel : public QLabel
{
    Q_OBJECT
public:
    explicit DropJLabel(QWidget *parent = 0);
    DropJLabel (NamedIcon* _icon, QWidget *parent = 0);
    DropJLabel (NamedIcon* _icon,  QHash <QString, NamedIcon*>* iconMap, bool update, QWidget *parent = 0);
//    /*public*/ void dragExit(DropTargetEvent dte);
//    /*public*/ void dragEnter(DropTargetDragEvent dtde);
    void dragEnterEvent(QDragEnterEvent *event);
//    /*public*/ void dragOver(DropTargetDragEvent dtde);
    void dragMoveEvent(QDragMoveEvent *de);
//    /*public*/ void dropActionChanged(DropTargetDragEvent dtde);
    /*public*/ void dropEvent(QDropEvent *);
    /*public*/ void drop(DropTargetDropEvent* e);
    void mouseMoveEvent(QMouseEvent *ev);
    void setName(QString name);
    QString getName();
    void setIcon(NamedIcon* _icon);
    NamedIcon* icon();
    void setIconFn(QString, QSize);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    bool bRightClick;
signals:
    void fileNameChanged(QString);
    void rightClick(DropJLabel*);

public slots:
private:
    /*private*/ DataFlavor* _dataFlavor;
    /*private*/ QHash <QString, NamedIcon*>*   _iconMap;
    /*private*/ bool 		_update;
    /*private*/ void accept(DropTargetDropEvent* e, NamedIcon* newIcon);
 Logger* log;
 QString name;
 NamedIcon* _icon;
 void init(NamedIcon* icon);
 friend class FunctionLabelsPane;
};

#endif // DROPJLABEL_H
