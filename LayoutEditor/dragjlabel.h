#ifndef DRAGJLABEL_H
#define DRAGJLABEL_H
#include "jlabel.h"
#include "exceptions.h"
#include "logger.h"
#include "namedicon.h"
#include <QLabel>

class QDrag;
class DataFlavor;
class DragJLabel : public JLabel
{
    Q_OBJECT
public:
    explicit DragJLabel(QString s, QWidget *parent = 0);
    /*public*/ DragJLabel(DataFlavor* flavor, QWidget *parent = 0);
    /*public*/ DragJLabel(DataFlavor* flavor, NamedIcon* icon, QWidget *parent = 0);

    ~DragJLabel();
    /*public*/ QList<DataFlavor*> getTransferDataFlavors();
    /*public*/ bool isDataFlavorSupported(DataFlavor* flavor);
    /*public*/ QVariant getTransferData(DataFlavor* flavor) throw (UnsupportedFlavorException,IOException);
//    void dragEnterEvent(QDragEnterEvent *);
//    void dragMoveEvent(QDragMoveEvent *);
    /*public*/ void mousePressEvent(QMouseEvent *e) override;
    virtual QByteArray mimeData();
    void setIcon(NamedIcon* icon);
    NamedIcon* getIcon();
    void setName(QString name);
    void setOpaque(bool) {}
    /*public*/ void setPixmap(QPixmap pixmap);
    /*public*/ void setMargin(int);
    /*public*/ void setText(QString txt);

signals:
 void showPopUp(NamedIcon*);

public slots:
private:
  Logger* log;
  NamedIcon* icon;
  QString name;
  QDrag *dr;
  JLabel* internalLabel;
  void common();

protected:
    /*protected*/ DataFlavor* _dataFlavor;
  virtual /*protected*/ bool okToDrag();

friend class IndicatorDragJLabel;
friend class TIconDragJLabel;
friend class RPSIconDragJLabel;
friend class ClockItemPanel;
};

#endif // DRAGJLABEL_H
