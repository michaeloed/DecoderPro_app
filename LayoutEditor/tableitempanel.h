#ifndef TABLEITEMPANEL_H
#define TABLEITEMPANEL_H
#include "familyitempanel.h"
#include "dragjlabel.h"

class JDialog;
class QItemSelection;
class TIconDragJLabel;
class JTable;
class JTextField;
class PickListModel;
class ActionListener;
class NamedBean;
class TableItemPanel : public FamilyItemPanel
{
    Q_OBJECT
public:
    //explicit TableItemPanel(QWidget *parent = 0);
    /*public*/ TableItemPanel(JmriJFrame* parentFrame, QString  type, QString family, PickListModel* model, Editor* editor, QWidget *parent = 0);
    /*public*/ void init();
    /*public*/ void init(ActionListener* doneAction, QHash<QString, NamedIcon*>* iconMap);
    /*public*/ NamedBean* getTableSelection();
    /*public*/ void setSelection(NamedBean* bean) ;
    /*public*/ void valueChanged(/*ListSelectionEvent* e*/);

signals:

public slots:
    void OnSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected );
private:
    int ROW_HEIGHT;
    //JScrollPane _scrollPane;
    JDialog*     _addTableDialog;
    JTextField*  _sysNametext;// = new JTextField();
    JTextField*  _userNametext;// = new JTextField();
    QPushButton* _addTableButton;
private:
    Logger* log;
protected:
    /*protected*/ JTable*    _table;
    /*protected*/ PickListModel* _model;
    /*protected*/ virtual QWidget* initTablePanel(PickListModel* model, Editor* editor);
    /*protected*/ void addToTable();
    /*protected*/ NamedBean* getNamedBean();
protected slots:
    /*protected*/ void makeAddToTableWindow();
    /*protected*/ DragJLabel* getDragger(DataFlavor* flavor, QHash<QString, NamedIcon*>* map);

friend class SignalHeadItemPanel;
friend class TIconDragJLabel;
friend class SignalMastItemPanel;
friend class SMIconDragJLabel;
friend class MultiSensorItemPanel;
friend class MSIconDragJLabel;
friend class ITOIconDragJLabel;
friend class AddTableActionListener;
};
/*protected*/ class TIconDragJLabel :public DragJLabel
{
 Q_OBJECT
 TableItemPanel* self;
 QHash<QString, NamedIcon*>* iconMap;
public:
 /*public*/ TIconDragJLabel(DataFlavor* flavor, QHash <QString, NamedIcon*>* map, TableItemPanel* self);
 /*public*/ bool isDataFlavorSupported(DataFlavor* flavor) ;
 /*public*/ QObject* getTransferData(DataFlavor* flavor) throw (UnsupportedFlavorException,IOException) ;
 QString mimeData();
};
class AddTableActionListener : public ActionListener
{
    Q_OBJECT
    TableItemPanel* parent;
public:
    AddTableActionListener(TableItemPanel* parent);
    void actionPerformed(ActionEvent *e = 0);
};

#endif // TABLEITEMPANEL_H
