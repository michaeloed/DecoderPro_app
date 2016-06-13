#include "beantableframe.h"
#include "beantabledatamodel.h"
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include "savemenu.h"
#include "jtable.h"

BeanTableFrame::BeanTableFrame(QWidget *parent) :
    JmriJFrame(parent)
{
 //common();
}
/**
 * Provide a JFrame to display a table of NamedBeans.
 * <P>
 * This frame includes the table itself at the top,
 * plus a "bottom area" for things like an Add... button
 * and checkboxes that control display options.
 * <p>
 * The usual menus are also provided here.
 * <p>
 * Specific uses are customized via the BeanTableDataModel
 * implementation they provide, and by
 * providing a {@link #extras} implementation
 * that can in turn invoke {@link #addToBottomBox} as needed.
 *
 * @author	Bob Jacobsen   Copyright (C) 2003
 * @version	$Revision: 20803 $
 */
// /*public*/ class BeanTableFrame extends jmri.util.JmriJFrame {

/*static*/ /*final*/ int BeanTableFrame::bottomStrutWidth = 20;

//    ResourceBundle rb = ResourceBundle.getBundle("jmri.jmrit.beantable.BeanTableBundle");
//    ResourceBundle rbapps = ResourceBundle.getBundle("apps.AppsBundle");

//    /*public*/ BeanTableFrame(){
//        super();
//    }

/*public*/ BeanTableFrame::BeanTableFrame(QString s, QWidget *parent) : JmriJFrame(s, parent)
{
 //super(s);
 //common();
}

/*public*/ BeanTableFrame::BeanTableFrame(BeanTableDataModel* model, QString helpTarget, JTable* dataTab, QWidget *parent) : JmriJFrame(parent)
{

 //super();
 //common(helpTarget);
 dataModel 	= model;
 this->dataTable = dataTab;

 //   dataScroll	= new JScrollPane(dataTable);

    // give system name column as smarter sorter and use it initially
//    try {
//        TableSorter tmodel = ((TableSorter)dataTable.getModel());
//        tmodel.setColumnComparator(String.class, new jmri.util.SystemNameComparator());
//        tmodel.setSortingStatus(BeanTableDataModel.SYSNAMECOL, TableSorter.ASCENDING);
//    } catch (java.lang.ClassCastException e) {}  // happens if not sortable table

 // configure items for GUI
 dataModel->configureTable(dataTable);

 // general GUI config
 //getContentPane().setLayout(new BoxLayout(getContentPane(), BoxLayout.Y_AXIS));
 QVBoxLayout* contentPaneLayout = new QVBoxLayout;
 getContentPane()->setLayout(contentPaneLayout);

 // add save menu item
 QMenuBar* menuBar = new QMenuBar();
 QMenu* fileMenu = new QMenu(tr("File"));
 menuBar->addMenu(fileMenu);
 fileMenu->addMenu(new SaveMenu());

 QAction* printItem = new QAction(tr("Print Table"),this);
 fileMenu->addAction(printItem);
//    printItem.addActionListener(new ActionListener() {
//            /*public*/ void actionPerformed(ActionEvent e) {
//                try {
//                    // MessageFormat headerFormat = new MessageFormat(getTitle());  // not used below
//                    MessageFormat footerFormat = new MessageFormat(getTitle()+" page {0,number}");
//                    dataTable.print(JTable.PrintMode.FIT_WIDTH , null, footerFormat);
//                } catch (java.awt.print.PrinterException e1) {
//                    log.warn("error printing: "+e1,e1);
//                }
//            }
//    });

 setMenuBar(menuBar);

 addHelpMenu(helpTarget,true);

 // install items in GUI
 //getContentPane().add(dataScroll);
 contentPaneLayout->addWidget(dataTable);
 //bottomBox = Box.createHorizontalBox();
 bottomBox = new QWidget();
 bottomBox->setLayout(new QHBoxLayout);
 //bottomBox.add(Box.createHorizontalGlue());	// stays at end of box
 bottomBoxIndex = 0;

 contentPaneLayout->addWidget(bottomBox);

 // add extras, if desired by subclass
 extras();

 // set Viewport preferred size from size of table
 QSize dataTableSize = dataTable->sizeHint();
 // width is right, but if table is empty, it's not high
 // enough to reserve much space.
 dataTableSize.setHeight(qMax(dataTableSize.height(), 400));
//    dataScroll.getViewport().setPreferredSize(dataTableSize);

 // set preferred scrolling options
//    dataScroll.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
//    dataScroll.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
// dataModel->loadTableColumnDetails(dataTable);
}

/**
 * Hook to allow sub-types to install more items in GUI
 */
void BeanTableFrame::extras() {}

/*protected*/ QWidget* BeanTableFrame::getBottomBox() { return bottomBox; }
/**
 * Add a component to the bottom box. Takes care of organising glue, struts etc
 * @param comp
 * @param c
 */
//@edu.umd.cs.findbugs.annotations.SuppressWarnings(value="UUF_UNUSED_FIELD",
//      justification="param c is required in the listedtableframe")
/*protected*/ void BeanTableFrame::addToBottomBox(QWidget* comp, QString c)
{
    QHBoxLayout* bottomBoxLayout;
    if(bottomBox->layout() == NULL)
     bottomBox->setLayout(bottomBoxLayout = new QHBoxLayout);
    else
     bottomBoxLayout = (QHBoxLayout*)bottomBox->layout();
    //bottomBox.add(Box.createHorizontalStrut(bottomStrutWidth), bottomBoxIndex);
    bottomBoxLayout->addStrut(bottomStrutWidth);
    ++bottomBoxIndex;
    //bottomBox.add(comp, bottomBoxIndex);
    bottomBoxLayout->addWidget(comp);
    ++bottomBoxIndex;
}

/*public*/ void BeanTableFrame::dispose() {
    if (dataModel != NULL){
//        dataModel->saveTableColumnDetails(dataTable);
        dataModel->dispose();
    }
    dataModel = NULL;
    dataTable = NULL;
//    dataScroll = NULL;
    //super.dispose();
    close();
}
