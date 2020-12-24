#include "oblocktableframe.h"
#include "oblocktablepanel.h"
#include <QBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include "storemenu.h"

/**
 * Override to place the four OBlock tables in tabbed interface.
 * @see jmri.jmrit.beantable.oblock.TableFrames
 *
 * @author Bob Jacobsen Copyright (C) 2003
 * @author Matthew Harris copyright (c) 2009
 * @author Egbert Broerse copyright (c) 2020
 */
// /*public*/ class OBlockTableFrame extends BeanTableFrame<OBlock> {


    /*public*/ OBlockTableFrame::OBlockTableFrame(OBlockTablePanel* panel, QString helpTarget, QWidget *parent) : BeanTableFrame(parent) {

        //super();

        oblockPanel = panel;

        // general GUI config
        getContentPane()->setLayout(new QVBoxLayout());//(getContentPane(), BoxLayout.Y_AXIS));
#if 1
        // add save menu item
        QMenuBar* menuBar = new QMenuBar();
        QMenu* fileMenu = new QMenu(tr("File"));
        menuBar->addMenu(fileMenu);
        fileMenu->addMenu(new StoreMenu());
        fileMenu->addAction(panel->getPrintItem());

        menuBar->addMenu(panel->getOptionMenu()); // configure to show up on oblock.TableFrames when called from Tools menu
        menuBar->addMenu(panel->getTablesMenu());

        setMenuBar(menuBar); // this menubar shows when table opened as unlisted/startup action

        addHelpMenu(helpTarget, true);

        // install items in GUI
        getContentPane()->layout()->addWidget(oblockPanel);
        bottomBox = Box::createHorizontalBox();
        bottomBox->layout()->addWidget(Box::createHorizontalGlue()); // stays at end of box
        bottomBoxIndex = 0;

        getContentPane()->layout()->addWidget(bottomBox);

        // add extras, if desired by subclass
        extras();
#endif
    }

    //@Override
    /*public*/ void OBlockTableFrame::dispose() {
        if (oblockPanel != nullptr) {
            oblockPanel->dispose();
        }
        BeanTableFrame::dispose();
    }
