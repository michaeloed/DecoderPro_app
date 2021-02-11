#include "oblocktableactiontest.h"
#include "assert1.h"
#include "guilafpreferencesmanager.h"
#include "instancemanager.h"
#include "jframeoperator.h"
#include "oblockmanager.h"
#include "oblock.h"
#include "jemmyutil.h"
#include "path.h"
#include "junitappender.h"
#include "portal.h"
#include "portalmanager.h"
#include "signalmastmanager.h"
#include "virtualsignalmast.h"
#include "threadingutil.h"
#include "jdesktoppane.h"
#include "jinternalframe.h"
#include "opath.h"
#include <QApplication>

OBlockTableActionTest::OBlockTableActionTest(QObject *parent) : QObject(parent)
{

}
/**
 * Swing tests for the OBlock etc tables.
 * Includes many of the jmri.jmrit.beantable.ablock classes via Jemmy UI tests.
 *
 * @author Pete Cressman Copyright 2016
 * @author Egbert Broerse Copyright 2020
 */
///*public*/ class OBlockTableActionTest {


    //@Test
    /*public*/ void OBlockTableActionTest::testCreate() {
        Assert::assertNotNull(a, __FILE__, __LINE__);
        Assert::assertNull(a->f, __FILE__, __LINE__); // frame should be null until action invoked
    }

    //@Test
    /*public*/ void OBlockTableActionTest::testGetClassDescription() {
        Assert::assertEquals("OBlock Table Action class description", "Occupancy Block Table", a->getClassDescription(), __FILE__, __LINE__);
    }

    /**
     * Check the return value of includeAddButton. The table generated by this
     * action includes an Add Button.
     */
    //@Test
    /*public*/ void OBlockTableActionTest::testIncludeAddButton() {
        Assert::assertFalse("Default include add button", a->includeAddButton(), __FILE__, __LINE__);
    }

    //@Test
    /*public*/ void OBlockTableActionTest::testInvoke() {
        //Assume.assumeFalse(GraphicsEnvironment.isHeadless());
        // use original _desktop interface
        ((GuiLafPreferencesManager*)InstanceManager::getDefault("GuiLafPreferencesManager"))->setOblockEditTabbed(false);

        // ask for the window to open
        a->actionPerformed(/*(QAction::ActionEvent*)*/new JActionEvent(a, 1, ""));

        // Find new table window by name
        JmriJFrame* doc = JmriJFrame::getFrame(tr("Occupancy Blocks, their Portals and Paths"));
        Assert::assertNotNull("Occupancy window", doc, __FILE__, __LINE__);
        //new QueueTool().waitEmpty();


        JDesktopPane* dt = (JDesktopPane*) doc->getContentPane();
        QList<JInternalFrame*> fob = dt->getAllFrames();
        Assert::assertNotNull("OBlock window", fob.count(), __FILE__, __LINE__);

        Assert::assertEquals(4, fob.length(), __FILE__, __LINE__);
        //new QueueTool().waitEmpty();
        qApp->processEvents();
        // Ask to close add window
//        ThreadingUtil.runOnGUI(() -> {
            doc->setVisible(false);
            JUnitUtil::dispose(doc);
//        });
    }

    //@Test
    /*public*/ void OBlockTableActionTest::testInvokeTabbed() {
        //Assume.assumeFalse(GraphicsEnvironment.isHeadless());
        // use _tabbed interface
        ((GuiLafPreferencesManager*)InstanceManager::getDefault("GuiLafPreferencesManager"))->setOblockEditTabbed(true);

        // ask for the window to open
        a->actionPerformed(/*(QAction::ActionEvent*)*/new JActionEvent(a, 1, ""));

        // Find new table window by name
        JmriJFrame* doc = JmriJFrame::getFrame(tr("Occupancy Block Tables"));
        Assert::assertNotNull("Occupancy Blocks tabbed window", doc, __FILE__, __LINE__);
        //new QueueTool().waitEmpty();
        qApp->processEvents();

        // Ask to close add window
//        ThreadingUtil.runOnGUI(() -> {
            doc->setVisible(false);
            JUnitUtil::dispose(doc);
//        });
    }

    //@Test
    /*public*/ void OBlockTableActionTest::testAddOBlock() {
        //Assume.assumeFalse(GraphicsEnvironment.isHeadless());
        // use _tabbed interface
        ((GuiLafPreferencesManager*)InstanceManager::getDefault("GuiLafPreferencesManager"))->setOblockEditTabbed(true);
        a->actionPerformed(/*(QAction::ActionEvent*)*/new JActionEvent(a, 2, "")); // show table
        JFrame* f = JFrameOperator::waitJFrame(tr("Occupancy Block Tables"), true, true);
        Assert::assertNotNull(f, __FILE__, __LINE__);

        a->addOBlockPressed(/*null*/);
        JFrameOperator* addFrame = new JFrameOperator(tr("Add Occupancy Block"));  // NOI18N
        Assert::assertNotNull("Found Add OBlock Frame", addFrame, __FILE__, __LINE__);  // NOI18N

        (new JTextFieldOperator(addFrame, "0"))->setText("105");  // NOI18N
        (new JTextFieldOperator(addFrame, "2"))->setText("OBlock 105");  // NOI18N
        (new JButtonOperator(addFrame, tr("Create")))->push();  // NOI18N
        (new JButtonOperator(addFrame, tr("Cancel")))->push();  // NOI18N

        Block* chk105 = ((OBlockManager*)InstanceManager::getDefault("OBlockManager"))->getOBlock("OBlock 105");  // NOI18N
        Assert::assertNotNull("Verify OB105 Added", chk105, __FILE__, __LINE__);  // NOI18N
        Assert::assertEquals("Verify system name prefix", "OB105", chk105->getSystemName(), __FILE__, __LINE__);  // NOI18N

        (new JFrameOperator(f))->requestClose();
        JUnitUtil::dispose(f);
    }

    //@Test
    /*public*/ void OBlockTableActionTest::testRenameOBlock() {
        //Assume.assumeFalse(GraphicsEnvironment.isHeadless());
        // use _tabbed interface
        ((GuiLafPreferencesManager*)InstanceManager::getDefault("GuiLafPreferencesManager"))->setOblockEditTabbed(true);

        OBlockManager* obm = (OBlockManager*)InstanceManager::getDefault("OBlockManager");
        OBlock* ob = obm->createNewOBlock("OB999", "OBlock Name");  // NOI18N
        Assert::assertNotNull(ob, __FILE__, __LINE__);
        Assert::assertEquals("OBlock Name", ob->getUserName(), __FILE__, __LINE__);  // NOI18N

        OPath* path = new OPath(ob, "Path");

        // Open the OBlock table
        a->actionPerformed(/*null*/); // show table
        JFrameOperator* jfo = new JFrameOperator(tr("Occupancy Block Tables"));  // NOI18N
        Assert::assertNotNull(jfo, __FILE__, __LINE__);

        JTabbedPaneOperator* tbp = new JTabbedPaneOperator(jfo);
        tbp->selectPage("Occupancy Blocks");
        Assert::assertNotNull(tbp, __FILE__, __LINE__);

        JTableOperator* tbo = new JTableOperator(tbp);
        Assert::assertNotNull(tbo, __FILE__, __LINE__);

        // Click on the edit button, set the user name to empty for remove
        tbo->clickOnCell(0, 5);
        JFrameOperator* jfoEdit = new JFrameOperator("Edit Occupancy Block OBlock Name");  // NOI18N
        JTextFieldOperator* jtxt = new JTextFieldOperator(jfoEdit, 0);
        jtxt->clickMouse();
        jtxt->setText("");

        // Prepare the dialog thread and click on OK //
        QThread* remove = JemmyUtil::createModalDialogOperatorThread("Update usage to system name", tr("Yes"));  // NOI18N
        (new JButtonOperator(jfoEdit, "OK"))->doClick();  // NOI18N
        OBTWaitFor* condition = new OBTWaitFor( remove);
        //JUnitUtil::waitFor(() -> !(remove.isAlive()), "remove finished");  // NOI18N
        JUnitUtil::waitFor(condition, "remove finished", __FILE__, __LINE__);
        tbo->clickOnCell(0, 0);  // deselect the edit button

        // Click on the edit button, set the user name to a new value
        tbo->clickOnCell(0, 5);
        jfoEdit = new JFrameOperator("Edit Occupancy Block OB999");  // NOI18N
        jtxt = new JTextFieldOperator(jfoEdit, 0);
        jtxt->clickMouse();
        jtxt->setText("New OBlock Name");  // NOI18N

        // Prepare the dialog thread and click on OK
        //Thread rename = JemmyUtil.createModalDialogOperatorThread("Update usage to system name", Bundle.getMessage("ButtonYes"));  // NOI18N
        (new JButtonOperator(jfoEdit, "OK"))->doClick();  // NOI18N
        //JUnitUtil::waitFor(() -> !(rename.isAlive()), "rename finished");  // NOI18N
        tbo->clickOnCell(0, 0);  // deselect the edit button

        // Confirm the portal toBlock user name change
        Assert::assertEquals("New OBlock Name", path->getBlock()->getUserName(), __FILE__, __LINE__);

        jfo->requestClose();
    }

    //@Test
    /*public*/ void OBlockTableActionTest::testAddPortal() {
        //Assume.assumeFalse(GraphicsEnvironment.isHeadless());
        // use _tabbed interface
        ((GuiLafPreferencesManager*)InstanceManager::getDefault("GuiLafPreferencesManager"))->setOblockEditTabbed(true);
        a->actionPerformed(/*(QAction::ActionEvent*)*/new JActionEvent(a, 2, "")); // show table
        JFrame* f = JFrameOperator::waitJFrame(tr("Occupancy Block Tables"), true, true);
        Assert::assertNotNull(f, __FILE__, __LINE__);

        a->addPortalPressed(/*null*/);

        JFrameOperator* addFrame = new JFrameOperator(tr("Add Portal"));  // NOI18N
        Assert::assertNotNull("Found Add Portal Frame", addFrame, __FILE__, __LINE__);  // NOI18N

        (new JButtonOperator(addFrame, ("OK")))->push();  // NOI18N
        Assert::assertNotNull("Add Portal Frame still open after empty entry", addFrame, __FILE__, __LINE__);  // NOI18N

        PortalManager* pm = (PortalManager*)InstanceManager::getDefault("PortalManager");
        OBlockManager* obm = (OBlockManager*)InstanceManager::getDefault("OBlockManager");
        obm->createNewOBlock("OB1", "ob1");  // NOI18N
        obm->createNewOBlock("OB2", "ob2");  // NOI18N

        (new JTextFieldOperator(addFrame, 0))->setText("portal1");  // NOI18N
        (new JComboBoxOperator(addFrame, 0))->setSelectedIndex(1);  // NOI18N
        (new JComboBoxOperator(addFrame, 1))->setSelectedIndex(2);  // NOI18N
        (new JButtonOperator(addFrame, tr("OK")))->push();  // NOI18N

        JUnitAppender::assertWarnMessage("Portal portal1 needs an OBlock on each side"); // TODO prevent this from showing up when oblocks are defined
        Assert::assertEquals("Verify Portal Added", 1, pm->getPortalCount(), __FILE__, __LINE__);

        // TODO add Edit Portal test
        (new JFrameOperator(f))->requestClose();
        JUnitUtil::dispose(f);
    }

    //@Test
    /*public*/ void OBlockTableActionTest::testAddSignal() {
        //Assume.assumeFalse(GraphicsEnvironment.isHeadless());
        // use _tabbed interface
        ((GuiLafPreferencesManager*)InstanceManager::getDefault("GuiLafPreferencesManager"))->setOblockEditTabbed(true);
        a->actionPerformed(/*(QAction::ActionEvent*)*/new JActionEvent(a, 2, "")); // show table
        JFrame* f = JFrameOperator::waitJFrame(tr("Occupancy Block Tables"), true, true);
        Assert::assertNotNull(f, __FILE__, __LINE__);

        OBlockManager* obm = (OBlockManager*)InstanceManager::getDefault("OBlockManager");
        OBlock* ob1 = obm->createNewOBlock("OB1", "ob1");  // NOI18N
        OBlock* ob2 = obm->createNewOBlock("OB2", "ob2");  // NOI18N

        PortalManager* pm = (PortalManager*)InstanceManager::getDefault("PortalManager");
        Portal* port1 = pm->createNewPortal("IP1");  // NOI18N
        port1->setFromBlock(ob1, false);
        port1->setToBlock(ob2, false);

        SignalMastManager* l = (SignalMastManager*)InstanceManager::getDefault("SignalMastManager");
        VirtualSignalMast* sm1 = (VirtualSignalMast*) l->provideSignalMast("IF$vsm:basic:one-searchlight($1)");

        a->addSignalPressed(/*null*/);
        JUnitAppender::assertWarnMessage("Portal IP1 needs an OBlock on each side");

        JFrameOperator* addFrame = new JFrameOperator(tr("Add Signal"));  // NOI18N
        Assert::assertNotNull("Found Add Signal Frame", addFrame, __FILE__, __LINE__);  // NOI18N

        (new JButtonOperator(addFrame, tr("OK")))->push();  // NOI18N
        Assert::assertNotNull("Add Signal Frame still open after empty entry", addFrame, __FILE__, __LINE__);  // NOI18N

        (new JComboBoxOperator(addFrame, 0))->setSelectedItem(sm1->getDisplayName());  // NOI18N select sm1
        (new JComboBoxOperator(addFrame, 2))->setSelectedIndex(1);  // NOI18N select port1
        (new JButtonOperator(addFrame, "Flip OBlocks"))->push();  // NOI18N
        (new JTextFieldOperator(addFrame, 0))->setText("19.05");  // NOI18N
        (new JButtonOperator(addFrame, tr("OK")))->push();  // NOI18N

        Assert::assertNotNull("Verify Signal Added", port1->getToSignal(), __FILE__, __LINE__);  // NOI18N
        Assert::assertEquals("Check signal name", "IF$vsm:basic:one-searchlight($1)", port1->getToSignal()->getSystemName(), __FILE__, __LINE__);  // NOI18N
        // TODO add Edit Signal test
        (new JFrameOperator(f))->requestClose();
        JUnitUtil::dispose(f);
    }

        //@BeforeEach
    /*public*/ void OBlockTableActionTest::setUp() throw (Exception) {
        JUnitUtil::setUp();
        JUnitUtil::resetInstanceManager();
        JUnitUtil::resetProfileManager();
        JUnitUtil::initDefaultUserMessagePreferences();
        InstanceManager::setDefault("OBlockManager", new OBlockManager());
        a = new OBlockTableAction();
    }

    //@AfterEach
    /*public*/ void OBlockTableActionTest::tearDown() throw (Exception) {
        a = nullptr;
        JUnitUtil::resetWindows(false,false);
        JUnitUtil::deregisterBlockManagerShutdownTask(); // perhaps invoked via super, must be cleaned up
        JUnitUtil::tearDown();
    }