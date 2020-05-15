#include "layouteditortools.h"
#include "jtextfield.h"
#include <QMessageBox>
#include "signalheadmanager.h"
#include "signalheadmanager.h"
#include "signalheadicon.h"
#include "signalmast.h"
#include "layoutturnout.h"
#include "blockbosslogic.h"
#include "defaultsignalmastmanager.h"
#include "../Tables/jmribeancombobox.h"
#include "sensoricon.h"
#include "tracksegment.h"
#include "signalmasticon.h"
#include "signalmastlogicmanager.h"
#include "defaultsignalmastlogicmanager.h"
#include "signallingguitools.h"
#include "inputdialog.h"
#include "jmrijframe.h"
#include "jseparator.h"
#include "defaultconditionalaction.h"
#include "logix.h"
#include "defaultconditionalaction.h"
#include "conditionalvariable.h"
#include <QCompleter>
#include <QGroupBox>
#include "borderlayout.h"
#include "jmribeancombobox.h"
#include "joptionpane.h"
#include <QVariant>
#include "layoutslip.h"
#include "mathutil.h"
#include "layouteditorfinditems.h"
#include "path.h"

//LayoutEditorTools::LayoutEditorTools(QObject *parent) :
//    QObject(parent)
//{
//}
/**
 * Layout Editor Tools provides tools making use of layout connectivity available
 *	in Layout Editor panels.
 * <P>
 * The tools in this module are accessed via the Tools menu in Layout Editor.
 * <P>
 * @author Dave Duchamp Copyright (c) 2007
 * @version $Revision: 21247 $
 */

///*public*/ class LayoutEditorTools
//{

//	// Defined text resource
//	static final ResourceBundle rb = ResourceBundle.getBundle("jmri.jmrit.display.layoutEditor->LayoutEditorBundle");
//    static final ResourceBundle rbean = ResourceBundle.getBundle("jmri.NamedBeanBundle");


// constructor method
/*public*/ LayoutEditorTools::LayoutEditorTools(LayoutEditor* thePanel, QObject* parent) :QObject(parent)
{
    // constants
 NONE = 0;  // Signal at Turnout Positions
 A1 = 1;
 A2 = 2;
 A3 = 3;
 B1 = 4;
 B2 = 5;
 C1 = 6;
 C2 = 7;
 D1 = 8;
 D2 = 9;
 // operational instance variables shared between tools
 layoutEditor = NULL;
 signalIconEditor = NULL;
 signalFrame = NULL;
 needRedraw = false;
 logic = NULL;
 auxSignal = NULL;
 hitEndBumper = false;

 layoutEditor = thePanel;

 //operational variables for Set Signals at Double Crossover Turnout tool
 turnout1ComboBox = new NamedBeanComboBox(
         InstanceManager::turnoutManagerInstance(),
         nullptr, NamedBean::DisplayOptions::DISPLAYNAME);
 turnout2ComboBox = new NamedBeanComboBox(
         InstanceManager::turnoutManagerInstance(),
         nullptr, NamedBean::DisplayOptions::DISPLAYNAME);
 a1TToTSignalHeadComboBox = new NamedBeanComboBox(
         (SignalHeadManager*)InstanceManager::getDefault("SignalHeadManager"
         ),
         nullptr, NamedBean::DisplayOptions::DISPLAYNAME);
 a2TToTSignalHeadComboBox = new NamedBeanComboBox(
         (SignalHeadManager*)InstanceManager::getDefault("SignalHeadManager"
         ),
         nullptr, NamedBean::DisplayOptions::DISPLAYNAME);
 b1TToTSignalHeadComboBox = new NamedBeanComboBox(
         (SignalHeadManager*)InstanceManager::getDefault("SignalHeadManager"
         ),
         nullptr, NamedBean::DisplayOptions::DISPLAYNAME);
 b2TToTSignalHeadComboBox = new NamedBeanComboBox(
         (SignalHeadManager*)InstanceManager::getDefault("SignalHeadManager"
         ),
         nullptr, NamedBean::DisplayOptions::DISPLAYNAME);
 c1TToTSignalHeadComboBox = new NamedBeanComboBox(
         (SignalHeadManager*)InstanceManager::getDefault("SignalHeadManager"
         ),
         nullptr, NamedBean::DisplayOptions::DISPLAYNAME);
 c2TToTSignalHeadComboBox = new NamedBeanComboBox(
         (SignalHeadManager*)InstanceManager::getDefault("SignalHeadManager"
         ),
         nullptr, NamedBean::DisplayOptions::DISPLAYNAME);
 d1TToTSignalHeadComboBox = new NamedBeanComboBox(
         (SignalHeadManager*)InstanceManager::getDefault("SignalHeadManager"
         ),
         nullptr, NamedBean::DisplayOptions::DISPLAYNAME);
  d2TToTSignalHeadComboBox = new NamedBeanComboBox(
         (SignalHeadManager*)InstanceManager::getDefault("SignalHeadManager"
         ),
         nullptr, NamedBean::DisplayOptions::DISPLAYNAME);
  setA2TToTHead = new QCheckBox(tr("Add the above Signal Head Icon to Panel"));
  setB1TToTHead = new QCheckBox(tr("Add the above Signal Head Icon to Panel"));
  setB2TToTHead = new QCheckBox(tr("Add the above Signal Head Icon to Panel"));
  setC1TToTHead = new QCheckBox(tr("Add the above Signal Head Icon to Panel"));
  setC2TToTHead = new QCheckBox(tr("Add the above Signal Head Icon to Panel"));
  setD1TToTHead = new QCheckBox(tr("Add the above Signal Head Icon to Panel"));
  setD2TToTHead = new QCheckBox(tr("Add the above Signal Head Icon to Panel"));

  setupA1TToTLogic = new QCheckBox(tr("Set up SSL"));
  setupA2TToTLogic = new QCheckBox(tr("Set up SSL"));
  setupB1TToTLogic = new QCheckBox(tr("Set up SSL"));
  setupB2TToTLogic = new QCheckBox(tr("Set up SSL"));
  setupC1TToTLogic = new QCheckBox(tr("Set up SSL"));
  setupC2TToTLogic = new QCheckBox(tr("Set up SSL"));
  setupD1TToTLogic = new QCheckBox(tr("Set up SSL"));
  setupD2TToTLogic = new QCheckBox(tr("Set up SSL"));


 // operational variables for Set Signals at Turnout tool
 setSignalsFrame = NULL;
 setSignalsOpen = false;
 turnoutComboBox = new NamedBeanComboBox(
         InstanceManager::turnoutManagerInstance(),
         nullptr, NamedBean::DisplayOptions::DISPLAYNAME);
 turnoutNameField = new JTextField(16);
 throatContinuingField = new JTextField(16);
 connect(throatContinuingField, SIGNAL(textChanged(QString)), this, SLOT(throatContinuingField_textChanged(QString)));
 throatDivergingField = new JTextField(16);
 connect(throatDivergingField, SIGNAL(textChanged(QString)), this, SLOT(throatDivergingField_textChanged(QString)));
 continuingField = new JTextField(16);
 connect(continuingField, SIGNAL(textChanged(QString)), this, SLOT(continuingField_textChanged(QString)));
 divergingField = new JTextField(16);
 connect(divergingField, SIGNAL(textChanged(QString)), this, SLOT(divergingField_textChanged(QString)));
 setThroatContinuing = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupLogicThroatContinuing = new QCheckBox(tr("Set up Logic"));
 setThroatDiverging = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupLogicThroatDiverging = new QCheckBox(tr("Set up Logic"));
 setContinuing = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupLogicContinuing = new QCheckBox(tr("Set up Logic"));
 setDiverging = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupLogicDiverging = new QCheckBox(tr("Set up Logic"));
 getSavedSignalHeads = NULL;
 changeSignalIcon = NULL;
 setSignalsDone = NULL;
 setSignalsCancel = NULL;
 layoutTurnout = NULL;
 layoutTurnoutHorizontal = false;
 layoutTurnoutVertical = false;
 layoutTurnoutThroatLeft = false;
 layoutTurnoutThroatUp = false;
 layoutTurnoutBUp = false;
 layoutTurnoutBLeft = false;
 turnoutFromMenu = false;
 turnout = NULL;
 throatContinuingHead = NULL;
 throatDivergingHead = NULL;
 continuingHead = NULL;
 divergingHead = NULL;

 //operational variables for Set Signals at 3-Way Turnout tool
 turnoutAComboBox = new NamedBeanComboBox(
         InstanceManager::turnoutManagerInstance(),
         nullptr, NamedBean::DisplayOptions::DISPLAYNAME);
 turnoutBComboBox = new NamedBeanComboBox(
         InstanceManager::turnoutManagerInstance(),
         nullptr, NamedBean::DisplayOptions::DISPLAYNAME);
 a1_3WaySignalHeadComboBox = new NamedBeanComboBox(
         (SignalHeadManager*)InstanceManager::getDefault("SignalHeadManager"),
         nullptr, NamedBean::DisplayOptions::DISPLAYNAME);
 a2_3WaySignalHeadComboBox = new NamedBeanComboBox(
         (SignalHeadManager*)InstanceManager::getDefault("SignalHeadManager"),
         nullptr, NamedBean::DisplayOptions::DISPLAYNAME);
 a3_3WaySignalHeadComboBox = new NamedBeanComboBox(
         (SignalHeadManager*)InstanceManager::getDefault("SignalHeadManager"),
         nullptr, NamedBean::DisplayOptions::DISPLAYNAME);
 b_3WaySignalHeadComboBox = new NamedBeanComboBox(
         (SignalHeadManager*)InstanceManager::getDefault("SignalHeadManager"),
         nullptr, NamedBean::DisplayOptions::DISPLAYNAME);
 c_3WaySignalHeadComboBox = new NamedBeanComboBox(
         (SignalHeadManager*)InstanceManager::getDefault("SignalHeadManager"),
         nullptr, NamedBean::DisplayOptions::DISPLAYNAME);
 d_3WaySignalHeadComboBox = new NamedBeanComboBox(
         (SignalHeadManager*)InstanceManager::getDefault("SignalHeadManager"),
         nullptr, NamedBean::DisplayOptions::DISPLAYNAME);


 // operational variables for Set Signals at Block Boundary tool
 setSignalsAtBoundaryFrame = NULL;
 setSignalsAtBoundaryOpen = false;
 block1NameField = new JTextField(16);
 connect(block1NameField, SIGNAL(textChanged(QString)), this, SLOT(block1NameField_textChanged(QString)));
 block2NameField = new JTextField(16);
 connect(block2NameField, SIGNAL(textChanged(QString)), this, SLOT(block2NameField_textChanged(QString)));
 eastBoundField = new JTextField(16);
 connect(eastBoundField, SIGNAL(textChanged(QString)), this, SLOT(eastBoundField_textChanged(QString)));
 westBoundField = new JTextField(16);
 connect(westBoundField, SIGNAL(textChanged(QString)), this, SLOT(westBoundField_textChanged(QString)));
 setEastBound = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupLogicEastBound = new QCheckBox(tr("Set up Logic"));
 setWestBound = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupLogicWestBound = new QCheckBox(tr("Set up Logic"));
 getAnchorSavedSignalHeads = NULL;
 changeSignalAtBoundaryIcon = NULL;
 setSignalsAtBoundaryDone = NULL;
 setSignalsAtBoundaryCancel = NULL;
 block1 = NULL;
 block2 = NULL;
 eastTrack = NULL;
 westTrack = NULL;
 trackHorizontal = false;
 trackVertical = false;
 boundaryFromMenu = false;
 boundary = NULL;
 eastBoundHead = NULL;
 westBoundHead = NULL;
 testIcon = NULL;

 // operational variables for Set Signals at Double Crossover Turnout tool
 setSignalsAtXoverFrame = NULL;
 setSignalsAtXoverOpen = false;
 a1Field = new JTextField(16);
 a2Field = new JTextField(16);
 b1Field = new JTextField(16);
 b2Field = new JTextField(16);
 c1Field = new JTextField(16);
 c2Field = new JTextField(16);
 d1Field = new JTextField(16);
 d2Field = new JTextField(16);
 setA1Head = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupA1Logic = new QCheckBox(tr("Set up Logic"));
 setA2Head = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupA2Logic = new QCheckBox(tr("Set up Logic"));
 setB1Head = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupB1Logic = new QCheckBox(tr("Set up Logic"));
 setB2Head = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupB2Logic = new QCheckBox(tr("Set up Logic"));
 setC1Head = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupC1Logic = new QCheckBox(tr("Set up Logic"));
 setC2Head = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupC2Logic = new QCheckBox(tr("Set up Logic"));
 setD1Head = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupD1Logic = new QCheckBox(tr("Set up Logic"));
 setD2Head = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupD2Logic = new QCheckBox(tr("Set up Logic"));
 getSavedXoverSignalHeads = NULL;
 changeXoverSignalIcon = NULL;
 setXoverSignalsDone = NULL;
 setXoverSignalsCancel = NULL;
 xoverFromMenu = false;
 a1Head = NULL;
 a2Head = NULL;
 b1Head = NULL;
 b2Head = NULL;
 c1Head = NULL;
 c2Head = NULL;
 d1Head = NULL;
 d2Head = NULL;
 xoverType = LayoutTurnout::DOUBLE_XOVER;  // changes to RH_XOVER or LH_XOVER as required
 xoverTurnoutName = "";
 xoverTurnoutNameLabel = new QLabel("");

 setSignalMastsOpen =false;
 turnoutMastFromMenu = false;
 signalMastsJmriFrame = NULL;

 turnoutMastNameField = new JTextField(16);
 signalMastTurnoutPanel = new QWidget();
 turnoutBlocks =  QVector<QString>(4);
 usedMasts = QList<NamedBean*>();

 // operational variables for Set Signals at Double Crossover Turnout tool
 setSignalsAtXoverFrame = NULL;
 setSignalsAtXoverOpen = false;
 ///*private*/ JTextField* xoverTurnoutNameField = new JTextField(16);
 a1Field = new JTextField(16);
 a2Field = new JTextField(16);
 b1Field = new JTextField(16);
 b2Field = new JTextField(16);
 c1Field = new JTextField(16);
 c2Field = new JTextField(16);
 d1Field = new JTextField(16);
 d2Field = new JTextField(16);
 setA1Head = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupA1Logic = new QCheckBox(tr("Set up Logic"));
 setA2Head = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupA2Logic = new QCheckBox(tr("Set up Logic"));
 setB1Head = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupB1Logic = new QCheckBox(tr("Set up Logic"));
 setB2Head = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupB2Logic = new QCheckBox(tr("Set up Logic"));
 setC1Head = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupC1Logic = new QCheckBox(tr("Set up Logic"));
 setC2Head = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupC2Logic = new QCheckBox(tr("Set up Logic"));
 setD1Head = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupD1Logic = new QCheckBox(tr("Set up Logic"));
 setD2Head = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupD2Logic = new QCheckBox(tr("Set up Logic"));
 getSavedXoverSignalHeads = NULL;
 changeXoverSignalIcon = NULL;
 setXoverSignalsDone = NULL;
 setXoverSignalsCancel = NULL;
 xoverFromMenu = false;
 a1Head = NULL;
 a2Head = NULL;
 b1Head = NULL;
 b2Head = NULL;
 c1Head = NULL;
 c2Head = NULL;
 d1Head = NULL;
 d2Head = NULL;
 xoverType = LayoutTurnout::DOUBLE_XOVER;  // changes to RH_XOVER or LH_XOVER as required
 QString xoverTurnoutName = "";
 xoverTurnoutNameLabel = new QLabel("");

 turnoutComboBox = new NamedBeanComboBox(
            InstanceManager::turnoutManagerInstance(),
            nullptr, NamedBean::DisplayOptions::DISPLAYNAME);

 throatContinuingSignalHeadComboBox = new NamedBeanComboBox(
            (SignalHeadManager*)InstanceManager::getDefault("SignalHeadManager"),
            nullptr, NamedBean::DisplayOptions::DISPLAYNAME);
 throatDivergingSignalHeadComboBox = new NamedBeanComboBox(
            (SignalHeadManager*)InstanceManager::getDefault("SignalHeadManager"),
            nullptr, NamedBean::DisplayOptions::DISPLAYNAME);
 continuingSignalHeadComboBox = new NamedBeanComboBox(
            (SignalHeadManager*)InstanceManager::getDefault("SignalHeadManager"),
            nullptr, NamedBean::DisplayOptions::DISPLAYNAME);
 divergingSignalHeadComboBox = new NamedBeanComboBox(
            (SignalHeadManager*)InstanceManager::getDefault("SignalHeadManager"),
            nullptr, NamedBean::DisplayOptions::DISPLAYNAME);

 setPlaceAllHeads = new QCheckBox(tr("Place All Signal Head Icons"));
 setupAllLogic = new QCheckBox(tr("Set up all SSL's"));


 setSensorsOpen =false;
 turnoutSensorFromMenu = false;
 setSensorsFrame = NULL;
 turnoutSensorFrame = NULL;

 //turnoutSensorNameField = new JTextField(16);
 sensorsTurnoutComboBox = new JmriBeanComboBox(
    InstanceManager::turnoutManagerInstance(),
    nullptr, JmriBeanComboBox::DisplayOptions::DISPLAYNAME);

 changeSensorIcon = NULL;

 turnoutSenBlocks = QVector<QString>(4);

 sensorTurnoutPanel = new QWidget();
 getAnchorSavedSensors = NULL;
 changeSensorAtBoundaryIcon = NULL;
 setSensorsAtBoundaryDone = NULL;
 setSensorsAtBoundaryCancel = NULL;
 setSensorsAtBoundaryOpen = false;
 setSensorsAtBoundaryFrame = NULL;

 sensorFrame = NULL;

 getAnchorSavedSignalMasts = NULL;
 setSignalMastsAtBoundaryDone = NULL;
 setSignalMastsAtBoundaryCancel = NULL;
 setSignalMastsAtBoundaryOpen = false;
 setSignalMastsAtBoundaryFrame = NULL;
 adjust = (5.0/90.0);
 awayright = 5;
 offSetFromPoint = 5;

 setSignalsAtTToTFrame = NULL;
 turnout1NameField = new JTextField(16);
 turnout2NameField = new JTextField(16);
 a1TToTField = new JTextField(16);
 a2TToTField = new JTextField(16);
 b1TToTField = new JTextField(16);
 b2TToTField = new JTextField(16);
 c1TToTField = new JTextField(16);
 c2TToTField = new JTextField(16);
 d1TToTField = new JTextField(16);
 d2TToTField = new JTextField(16);
 setA1TToTHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupA1TToTLogic = new QCheckBox(tr("Set up Logic"));
 setA2TToTHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupA2TToTLogic = new QCheckBox(tr("Set up Logic"));
 setB1TToTHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupB1TToTLogic = new QCheckBox(tr("Set up Logic"));
 setB2TToTHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupB2TToTLogic = new QCheckBox(tr("Set up Logic"));
 setC1TToTHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupC1TToTLogic = new QCheckBox(tr("Set up Logic"));
 setC2TToTHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupC2TToTLogic = new QCheckBox(tr("Set up Logic"));
 setD1TToTHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupD1TToTLogic = new QCheckBox(tr("Set up Logic"));
 setD2TToTHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupD2TToTLogic = new QCheckBox(tr("Set up Logic"));
 getSavedTToTSignalHeads = NULL;
 changeTToTSignalIcon = NULL;
 setTToTSignalsDone = NULL;
 setTToTSignalsCancel = NULL;
 layoutTurnout1 = NULL;
 layoutTurnout2 = NULL;
 turnout1 = NULL;
 turnout2 = NULL;
 connectorTrack = NULL;
 a1TToTHead = NULL;
 a2TToTHead = NULL;
 b1TToTHead = NULL;
 b2TToTHead = NULL;
 c1TToTHead = NULL;
 c2TToTHead = NULL;
 d1TToTHead = NULL;
 d2TToTHead = NULL;
 layoutTurnout1Horizontal = false;
 layoutTurnout1Vertical = false;
 layoutTurnout2Horizontal = false;
 layoutTurnout2Vertical = false;
 layoutTurnout1ThroatLeft = false;
 layoutTurnout1ThroatUp = false;
 layoutTurnout2ThroatLeft = false;
 layoutTurnout2ThroatUp = false;
 layoutTurnout1BUp = false;
 layoutTurnout1BLeft = false;
 layoutTurnout2BUp = false;
 layoutTurnout2BLeft = false;

 // operational variables for Set Sensors at Level Crossing tool
 sensorsAtSlipFrame = NULL;
 setSensorsAtSlipOpen = false;

// getSavedSlipSensors = NULL;
// setSlipSensorsDone = NULL;
// setSlipSensorsCancel = NULL;
// changeSensorSlipIcon = NULL;
 sensorSlipFrame = NULL;

 slipSensorFromMenu = false;
 slipSensorBlocks = new QVector<QString>(4);

 sensorSlipPanel = new QWidget();

 // operational variables for Set Signals at Double Crossover Turnout tool
 setSignalsAtSlipFrame = NULL;
 setSignalsAtSlipOpen = false;
 slipNameCombo = new QComboBox();
 a1SlipField = new JTextField(16);
 a2SlipField = new JTextField(16);
 b1SlipField = new JTextField(16);
 b2SlipField = new JTextField(16);
 c1SlipField = new JTextField(16);
 c2SlipField = new JTextField(16);
 d1SlipField = new JTextField(16);
 d2SlipField = new JTextField(16);
 setA1SlipHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupA1SlipLogic = new QCheckBox(tr("Set up Logic"));
 setA2SlipHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupA2SlipLogic = new QCheckBox(tr("Set up Logic"));
 setB1SlipHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupB1SlipLogic = new QCheckBox(tr("Set up Logic"));
 setB2SlipHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupB2SlipLogic = new QCheckBox(tr("Set up Logic"));
 setC1SlipHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupC1SlipLogic = new QCheckBox(tr("Set up Logic"));
 setC2SlipHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupC2SlipLogic = new QCheckBox(tr("Set up Logic"));
 setD1SlipHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupD1SlipLogic = new QCheckBox(tr("Set up Logic"));
 setD2SlipHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupD2SlipLogic = new QCheckBox(tr("Set up Logic"));

 setSlipSignalsDone = NULL;
 setSlipSignalsCancel = NULL;
 layoutSlip = NULL;

 a1SlipHead = NULL;
 a2SlipHead = NULL;
 b1SlipHead = NULL;
 b2SlipHead = NULL;
 c1SlipHead = NULL;
 c2SlipHead = NULL;
 d1SlipHead = NULL;
 d2SlipHead = NULL;

 slipSignalFromMenu = false;

 // operational variables for Set Signals at Level Crossing tool
 setSignalsAtXingFrame = NULL;
 setSignalsAtXingOpen = false;
 blockANameField = new JTextField(16);
 blockCNameField = new JTextField(16);
 aField = new JTextField(16);
 bField = new JTextField(16);
 cField = new JTextField(16);
 dField = new JTextField(16);
 setAHead = new QCheckBox(tr("Place Head"));
 setupALogic = new QCheckBox(tr("Set Logic"));
 setBHead = new QCheckBox(tr("Place Head"));
 setupBLogic = new QCheckBox(tr("Set Logic"));
 setCHead = new QCheckBox(tr("Place Head"));
 setupCLogic = new QCheckBox(tr("Set Logic"));
 setDHead = new QCheckBox(tr("Place Head"));
 setupDLogic = new QCheckBox(tr("Set Logic"));
 getSavedXingSignalHeads = NULL;
 changeXingSignalIcon = NULL;
 setXingSignalsDone = NULL;
 setXingSignalsCancel = NULL;
 levelXingACHorizontal = false;
 levelXingACVertical = false;
 levelXingALeft = false;
 levelXingAUp = false;
 levelXingBUp = false;
 levelXingBLeft = false;
 xingFromMenu = false;
 levelXing = NULL;
 aHead = NULL;
 bHead = NULL;
 cHead = NULL;
 dHead = NULL;

 // operational variables for Set Sensors at Level Crossing tool
 sensorsAtXingFrame = NULL;
 setSensorsAtXingOpen = false;
 blockANameSensorField = new JTextField(16);
 blockCNameSensorField = new JTextField(16);

 getSavedXingSensors = NULL;
 setXingSensorsDone = NULL;
 setXingSensorsCancel = NULL;
 changeSensorXingIcon = NULL;
 sensorXingFrame = NULL;

 xingSensorFromMenu = false;
 xingSensorBlocks = new QVector<QString>(4);

 sensorXingPanel = new QWidget();

 // operational variables for Set Signals at Double Crossover Turnout tool
 setSignalsAtSlipFrame = NULL;
 setSignalsAtSlipOpen = false;
 slipNameCombo = new QComboBox();
 a1SlipField = new JTextField(16);
 a2SlipField = new JTextField(16);
 b1SlipField = new JTextField(16);
 b2SlipField = new JTextField(16);
 c1SlipField = new JTextField(16);
 c2SlipField = new JTextField(16);
 d1SlipField = new JTextField(16);
 d2SlipField = new JTextField(16);
 setA1SlipHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupA1SlipLogic = new QCheckBox(tr("Set up Logic"));
 setA2SlipHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupA2SlipLogic = new QCheckBox(tr("Set up Logic"));
 setB1SlipHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupB1SlipLogic = new QCheckBox(tr("Set up Logic"));
 setB2SlipHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupB2SlipLogic = new QCheckBox(tr("Set up Logic"));
 setC1SlipHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupC1SlipLogic = new QCheckBox(tr("Set up Logic"));
 setC2SlipHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupC2SlipLogic = new QCheckBox(tr("Set up Logic"));
 setD1SlipHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupD1SlipLogic = new QCheckBox(tr("Set up Logic"));
 setD2SlipHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupD2SlipLogic = new QCheckBox(tr("Set up Logic"));

 setSlipSignalsDone = NULL;
 setSlipSignalsCancel = NULL;
 layoutSlip = NULL;

 a1SlipHead = NULL;
 a2SlipHead = NULL;
 b1SlipHead = NULL;
 b2SlipHead = NULL;
 c1SlipHead = NULL;
 c2SlipHead = NULL;
 d1SlipHead = NULL;
 d2SlipHead = NULL;

 slipSignalFromMenu = false;

 // operational variables for Set SignalMast at Slip tool
 signalMastsAtSlipFrame = NULL;
 setSignalMastsAtSlipOpen = false;

 getSavedSlipSignalMasts = NULL;
 setSlipSignalMastsDone = NULL;
 setSlipSignalMastsCancel = NULL;

 slipMastFromMenu = false;
 slipBlocks = new QVector<QString>(4);


 signalMastLayoutSlipPanel = new QWidget();

 // operational variables for Set SignalMast at Level Crossing tool
 signalMastsAtXingFrame = NULL;
 setSignalMastsAtXingOpen = false;
 blockANameMastField = new JTextField(16);
 blockCNameMastField = new JTextField(16);

 getSavedXingSignalMasts = NULL;
 setXingSignalMastsDone = NULL;
 setXingSignalMastsCancel = NULL;

 xingMastFromMenu = false;
 xingBlocks = new QVector<QString>(4);

 signalMastLevelXingPanel = new QWidget();

 // operational variables for Set Signals at 3-Way Turnout tool
 setSignalsAt3WayTurnoutFrame = nullptr;
 setSignalsAt3WayOpen = false;
 turnoutANameField = new JTextField(16);
 turnoutBNameField = new JTextField(16);
 a13WayField = new JTextField(16);
 a23WayField = new JTextField(16);
 a33WayField = new JTextField(16);
 b3WayField = new JTextField(16);
 c3WayField = new JTextField(16);
 d3WayField = new JTextField(16);
 setA13WayHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupA13WayLogic = new QCheckBox(tr("Set up Logic"));
 setA23WayHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupA23WayLogic = new QCheckBox(tr("Set up Logic"));
 setA33WayHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupA33WayLogic = new QCheckBox(tr("Set up Logic"));
 setB3WayHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupB3WayLogic = new QCheckBox(tr("Set up Logic"));
 setC3WayHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupC3WayLogic = new QCheckBox(tr("Set up Logic"));
 setD3WayHead = new QCheckBox(tr("Add Signal Head Icon to Panel"));
 setupD3WayLogic = new QCheckBox(tr("Set up Logic"));
 getSaved3WaySignalHeads = NULL;
 change3WaySignalIcon = NULL;
 set3WaySignalsDone = NULL;
 set3WaySignalsCancel = NULL;
 layoutTurnoutA = NULL;
 layoutTurnoutB = NULL;
 turnoutA = NULL;
 turnoutB = NULL;
 ///*private*/ TrackSegment* conTrack = NULL;
 a13WayHead = NULL;   // saved in A1 of Turnout A - Throat - continuing
 a23WayHead = NULL;   // saved in A2 of Turnout A - Throat - diverging A (optional)
 a33WayHead = NULL;   // saved in A3 of Turnout A - Throat - diverging B (optional)
 b3WayHead = NULL;    // saved in C1 of Turnout A - at diverging A
 c3WayHead = NULL;    // saved in B1 of Turnout B - at continuing
 d3WayHead = NULL;    // saved in C1 of Turnout B - at diverging B
 layoutTurnoutAHorizontal = false;
 layoutTurnoutAVertical = false;
 layoutTurnoutBHorizontal = false;
 layoutTurnoutBVertical = false;
 layoutTurnoutAThroatLeft = false;
 layoutTurnoutAThroatUp = false;
 layoutTurnoutBThroatLeft = false;
 layoutTurnoutBThroatUp = false;
 layoutTurnoutABUp = false;
 layoutTurnoutABLeft = false;
 layoutTurnoutBBUp = false;
 layoutTurnoutBBLeft = false;
 //Border blackline = BorderFactory.createLineBorder(Color.black);

 signalMastsTurnoutComboBox = new JmriBeanComboBox(
             InstanceManager::turnoutManagerInstance(), nullptr,
             JmriBeanComboBox::DisplayOptions::DISPLAYNAME);
}
/*public*/ void LayoutEditorTools::setSignalsAtThroatToThroatTurnoutsFromMenu(
        /*@Nonnull*/ LayoutTurnout* to, /*@Nonnull*/ QString linkedTurnoutName,
        /*@Nonnull*/ MultiIconEditor* theEditor, /*@Nonnull*/ JFrame* theFrame) {
    ttotTurnoutName1 = to->getTurnoutName();
    ttotTurnoutName2 = linkedTurnoutName;

    turnout1ComboBox->setSelectedItem(to->getTurnout());
    turnout2ComboBox->setSelectedItem(to->getSecondTurnout());

    a1TToTSignalHeadComboBox->setSelectedItem(nullptr);
    a2TToTSignalHeadComboBox->setSelectedItem(nullptr);
    b1TToTSignalHeadComboBox->setSelectedItem(nullptr);
    b2TToTSignalHeadComboBox->setSelectedItem(nullptr);
    c1TToTSignalHeadComboBox->setSelectedItem(nullptr);
    c2TToTSignalHeadComboBox->setSelectedItem(nullptr);
    d1TToTSignalHeadComboBox->setSelectedItem(nullptr);
    d2TToTSignalHeadComboBox->setSelectedItem(nullptr);

    setSignalsAtThroatToThroatTurnoutsFromMenuFlag = true;
    setSignalsAtThroatToThroatTurnouts(theEditor, theFrame);
    setSignalsAtThroatToThroatTurnoutsFromMenuFlag = false;
}

/*public*/ void LayoutEditorTools::setSignalsAtThroatToThroatTurnouts(
        /*@Nonnull*/ MultiIconEditor* theEditor, /*@Nonnull*/ JFrame* theFrame) {
    signalIconEditor = theEditor;
    signalFrame = theFrame;

    //Initialize if needed
    if (setSignalsAtThroatToThroatTurnoutsFrame == nullptr) {
        setSignalsAtThroatToThroatTurnoutsOpenFlag = false;
        setSignalsAtThroatToThroatTurnoutsFrame = new JmriJFrameX(tr("Set Signal Heads at Throat-to-Throat Turnouts"), false, true);
        oneFrameToRuleThemAll(setSignalsAtThroatToThroatTurnoutsFrame);
        setSignalsAtThroatToThroatTurnoutsFrame->setDefaultCloseOperation(JFrame::DISPOSE_ON_CLOSE);
        setSignalsAtThroatToThroatTurnoutsFrame->addHelpMenu("package.jmri.jmrit.display.SetSignalsAtTToTTurnout", true);
        setSignalsAtThroatToThroatTurnoutsFrame->setLocation(70, 30);
        QWidget* theContentPane = setSignalsAtThroatToThroatTurnoutsFrame->getContentPane();
        QVBoxLayout* theContentPaneLayout;
        theContentPane->setLayout(theContentPaneLayout = new QVBoxLayout()); //theContentPane, BoxLayout.Y_AXIS));

        JPanel* panel1a = new JPanel(new FlowLayout());
        ttotTurnoutName1Label = new JLabel(tr("Turnout") + " 1 "
                + tr("Name"));
        panel1a->layout()->addWidget(ttotTurnoutName1Label);
        panel1a->layout()->addWidget(turnout1ComboBox);
        turnout1ComboBox->setToolTip(tr("Enter name (system or user) of turnout where signals are needed."));
        theContentPaneLayout->addWidget(panel1a);

        JPanel* panel1b = new JPanel(new FlowLayout());
        ttotTurnoutName2Label = new JLabel(tr("Turnout") + " 2 "
                + tr("Name"));
        panel1b->layout()->addWidget(ttotTurnoutName2Label);
        panel1b->layout()->addWidget(turnout2ComboBox);
        turnout2ComboBox->setToolTip(tr("Enter name (system or user) of turnout where signals are needed."));
        theContentPaneLayout->addWidget(panel1b);
        theContentPaneLayout->addWidget(new JSeparator(JSeparator::HORIZONTAL));
        //Provide for retrieval of names of previously saved signal heads

        JPanel* panel20 = new JPanel(new FlowLayout());
        JLabel* shTitle = new JLabel(tr("SignalHeads"));
        panel20->layout()->addWidget(shTitle);
        panel20->layout()->addWidget(new JLabel("		"));
        panel20->layout()->addWidget(getSavedTToTSignalHeads = new QPushButton(tr("GetSaved")));
        //getSavedTToTSignalHeads.addActionListener((ActionEvent e) -> {
        connect(getSavedTToTSignalHeads, &QPushButton::clicked, [=] {
            setSignalsAtTToTTurnoutsGetSaved();
        });
        getSavedTToTSignalHeads->setToolTip(tr("Select to retrieve signal heads previously stored."));
        theContentPaneLayout->addWidget(panel20);
        theContentPaneLayout->addWidget(new JSeparator(JSeparator::HORIZONTAL));

        JPanel* panel2a = new JPanel(new FlowLayout());
        panel2a->layout()->addWidget(new JLabel("   "));
        panel2a->layout()->addWidget(setPlaceAllHeads);
        setPlaceAllHeads->setToolTip(tr("Check to place all signal head icons"));
        //setPlaceAllHeads.addActionListener((ActionEvent e) -> {
        connect(setPlaceAllHeads, &QCheckBox::clicked, [=]{
            bool isSelected = setPlaceAllHeads->isChecked();
            //(de)select all checkboxes
            setA1TToTHead->setChecked(isSelected);
            setA2TToTHead->setChecked(isSelected);
            setB1TToTHead->setChecked(isSelected);
            setB2TToTHead->setChecked(isSelected);
            setC1TToTHead->setChecked(isSelected);
            setC2TToTHead->setChecked(isSelected);
            setD1TToTHead->setChecked(isSelected);
            setD2TToTHead->setChecked(isSelected);
        });
        panel2a->layout()->addWidget(new JLabel("  "));
        panel2a->layout()->addWidget(setupAllLogic);
        setupAllLogic->setToolTip(tr("Check to set up Simple Signal Logic for all Signal Heads"));
        //setupAllLogic.addActionListener((ActionEvent e) -> {
        connect(setupAllLogic, &QCheckBox::clicked, [=]{
            bool isSelected = setupAllLogic->isChecked();
            //(de)select all checkboxes
            setupA1TToTLogic->setChecked(isSelected);
            setupA2TToTLogic->setChecked(isSelected);
            setupB1TToTLogic->setChecked(isSelected);
            setupB2TToTLogic->setChecked(isSelected);
            setupC1TToTLogic->setChecked(isSelected);
            setupC2TToTLogic->setChecked(isSelected);
            setupD1TToTLogic->setChecked(isSelected);
            setupD2TToTLogic->setChecked(isSelected);
        });
        theContentPaneLayout->addWidget(panel2a);
        theContentPaneLayout->addWidget(new JSeparator(JSeparator::HORIZONTAL));

        //Signal heads located at turnout 1
        JPanel* panel20a = new JPanel(new FlowLayout());
        panel20a->layout()->addWidget(new JLabel(tr("Signal located at")
                + " " + tr("Turnout") + " 1 - "
                + tr("ContinuingTrack")));
        theContentPaneLayout->addWidget(panel20a);

        JPanel* panel21 = new JPanel(new FlowLayout());
        panel21->layout()->addWidget(new JLabel(tr("%1").arg(
                tr("ProtectsTurnout") + " 2 - "
                + tr("ContinuingTrack"))));
        panel21->layout()->addWidget(a1TToTSignalHeadComboBox);
        theContentPaneLayout->addWidget(panel21);
        a1TToTSignalHeadComboBox->setToolTip(tr("Enter name (system or user) of signal head located here."));

        JPanel* panel22 = new JPanel(new FlowLayout());
        panel22->layout()->addWidget(new JLabel(tr("OrBoth") + " 2 " + tr("Tracks)") + "	  "));
        panel22->layout()->addWidget(setA1TToTHead);
        setA1TToTHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel22->layout()->addWidget(new JLabel("  "));
        panel22->layout()->addWidget(setupA1TToTLogic);
        setupA1TToTLogic->setToolTip(tr("Check to set up Simple Signal Logic for the above signal head."));
        theContentPaneLayout->addWidget(panel22);

        JPanel* panel23 = new JPanel(new FlowLayout());
        panel23->layout()->addWidget(new JLabel(tr("%1").arg(
                tr("ProtectsTurnout") + " 2 - "
                + tr("DivergingTrack"))));
        panel23->layout()->addWidget(a2TToTSignalHeadComboBox);
        theContentPaneLayout->addWidget(panel23);
        a2TToTSignalHeadComboBox->setToolTip(tr("Enter name (system or user) of signal head located here."));

        JPanel* panel24 = new JPanel(new FlowLayout());
        panel24->layout()->addWidget(new JLabel("				"));
        panel24->layout()->addWidget(setA2TToTHead);
        setA2TToTHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel24->layout()->addWidget(new JLabel("  "));
        panel24->layout()->addWidget(setupA2TToTLogic);
        setupA2TToTLogic->setToolTip(tr("Check to set up Simple Signal Logic for the above signal head."));
        theContentPaneLayout->addWidget(panel24);

        JPanel* panel31x = new JPanel(new FlowLayout());
        panel31x->layout()->addWidget(new JLabel(tr("Signal located at")
                + " " + tr("Turnout") + " 1 - "
                + tr("DivergingTrack")));
        theContentPaneLayout->addWidget(panel31x);

        JPanel* panel31 = new JPanel(new FlowLayout());
        panel31->layout()->addWidget(new JLabel(tr("%1").arg(
                tr("ProtectsTurnout") + " 2 - "
                + tr("ContinuingTrack"))));
        panel31->layout()->addWidget(b1TToTSignalHeadComboBox);
        theContentPaneLayout->addWidget(panel31);
        b1TToTSignalHeadComboBox->setToolTip(tr("Enter name (system or user) of signal head located here."));

        JPanel* panel32 = new JPanel(new FlowLayout());
        panel32->layout()->addWidget(new JLabel(tr("OrBoth") + " 2 " + tr("Tracks)") + "	  "));
        panel32->layout()->addWidget(setB1TToTHead);
        setB1TToTHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel32->layout()->addWidget(new JLabel("  "));
        panel32->layout()->addWidget(setupB1TToTLogic);
        setupB1TToTLogic->setToolTip(tr("Check to set up Simple Signal Logic for the above signal head."));
        theContentPaneLayout->addWidget(panel32);

        JPanel* panel33 = new JPanel(new FlowLayout());
        panel33->layout()->addWidget(new JLabel(tr("%1").arg(
                tr("ProtectsTurnout") + " 2 - "
                + tr("DivergingTrack"))));
        panel33->layout()->addWidget(b2TToTSignalHeadComboBox);
        theContentPaneLayout->addWidget(panel33);
        b2TToTSignalHeadComboBox->setToolTip(tr("Enter name (system or user) of signal head located here."));

        JPanel* panel34 = new JPanel(new FlowLayout());
        panel34->layout()->addWidget(new JLabel("				"));
        panel34->layout()->addWidget(setB2TToTHead);
        setB2TToTHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel34->layout()->addWidget(new JLabel("  "));
        panel34->layout()->addWidget(setupB2TToTLogic);
        setupB2TToTLogic->setToolTip(tr("Check to set up Simple Signal Logic for the above signal head."));
        theContentPaneLayout->addWidget(panel34);
        theContentPaneLayout->addWidget(new JSeparator(JSeparator::HORIZONTAL));
        //Signal heads located at turnout 2

        JPanel* panel41x = new JPanel(new FlowLayout());
        panel41x->layout()->addWidget(new JLabel(tr("Signal located at")
                + " " + tr("Turnout") + " 2 - "
                + tr("ContinuingTrack")));
        theContentPaneLayout->addWidget(panel41x);

        JPanel* panel41 = new JPanel(new FlowLayout());
        panel33->layout()->addWidget(new JLabel(tr("%1").arg(
                tr("ProtectsTurnout") + " 1 - "
                + tr("ContinuingTrack"))));
        panel41->layout()->addWidget(c1TToTSignalHeadComboBox);
        theContentPaneLayout->addWidget(panel41);
        c1TToTSignalHeadComboBox->setToolTip(tr("Enter name (system or user) of signal head located here."));

        JPanel* panel42 = new JPanel(new FlowLayout());
        panel42->layout()->addWidget(new JLabel(tr("OrBoth") + " 1 " + tr("Tracks)") + "	  "));
        panel42->layout()->addWidget(setC1TToTHead);
        setC1TToTHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel42->layout()->addWidget(new JLabel("  "));
        panel42->layout()->addWidget(setupC1TToTLogic);
        setupC1TToTLogic->setToolTip(tr("Check to set up Simple Signal Logic for the above signal head."));
        theContentPaneLayout->addWidget(panel42);

        JPanel* panel43 = new JPanel(new FlowLayout());
        panel43->layout()->addWidget(new JLabel(tr("%1").arg(
                tr("ProtectsTurnout") + " 1 - "
                + tr("DivergingTrack"))));
        panel43->layout()->addWidget(c2TToTSignalHeadComboBox);
        theContentPaneLayout->addWidget(panel43);
        c2TToTSignalHeadComboBox->setToolTip(tr("Enter name (system or user) of signal head located here."));

        JPanel* panel44 = new JPanel(new FlowLayout());
        panel44->layout()->addWidget(new JLabel("				"));
        panel44->layout()->addWidget(setC2TToTHead);
        setC2TToTHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel44->layout()->addWidget(new JLabel("  "));
        panel44->layout()->addWidget(setupC2TToTLogic);
        setupC2TToTLogic->setToolTip(tr("Check to set up Simple Signal Logic for the above signal head."));
        theContentPaneLayout->addWidget(panel44);

        JPanel* panel51x = new JPanel(new FlowLayout());
        panel51x->layout()->addWidget(new JLabel(tr("Signal located at")
                + " " + tr("Turnout") + " 2 - "
                + tr("DivergingTrack")));
        theContentPaneLayout->addWidget(panel51x);

        JPanel* panel51 = new JPanel(new FlowLayout());
        panel51->layout()->addWidget(new JLabel(tr("%1").arg(
                tr("ProtectsTurnout") + " 1 - "
                + tr("ContinuingTrack"))));
        panel51->layout()->addWidget(d1TToTSignalHeadComboBox);
        theContentPaneLayout->addWidget(panel51);
        d1TToTSignalHeadComboBox->setToolTip(tr("Enter name (system or user) of signal head located here."));

        JPanel* panel52 = new JPanel(new FlowLayout());
        panel52->layout()->addWidget(new JLabel(tr("OrBoth") + " 1 " + tr("Tracks)") + "	  "));
        panel52->layout()->addWidget(setD1TToTHead);
        setD1TToTHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel52->layout()->addWidget(new JLabel("  "));
        panel52->layout()->addWidget(setupD1TToTLogic);
        setupD1TToTLogic->setToolTip(tr("Check to set up Simple Signal Logic for the above signal head."));
        theContentPaneLayout->addWidget(panel52);

        JPanel* panel53 = new JPanel(new FlowLayout());
        panel53->layout()->addWidget(new JLabel(tr("%1").arg(
                tr("ProtectsTurnout") + " 1 - "
                + tr("DivergingTrack"))));
        panel53->layout()->addWidget(d2TToTSignalHeadComboBox);
        theContentPaneLayout->addWidget(panel53);
        d2TToTSignalHeadComboBox->setToolTip(tr("Enter name (system or user) of signal head located here."));

        JPanel* panel54 = new JPanel(new FlowLayout());
        panel54->layout()->addWidget(new JLabel("				"));
        panel54->layout()->addWidget(setD2TToTHead);
        setD2TToTHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel54->layout()->addWidget(new JLabel("  "));
        panel54->layout()->addWidget(setupD2TToTLogic);
        setupD2TToTLogic->setToolTip(tr("Check to set up Simple Signal Logic for the above signal head."));
        theContentPaneLayout->addWidget(panel54);
        theContentPaneLayout->addWidget(new JSeparator(JSeparator::HORIZONTAL));

        JPanel* panel6 = new JPanel(new FlowLayout());
        panel6->layout()->addWidget(changeTToTSignalIcon = new QPushButton(tr("Change Signal Head Icon")));
        //changeTToTSignalIcon.addActionListener((ActionEvent e) -> {
        connect(changeTToTSignalIcon, &QPushButton::clicked, [=]{
            signalFrame->setVisible(true);
        });
        changeTToTSignalIcon->setToolTip(tr("Select to change icons used to represent signal heads (Use left-facing icons)"));
        panel6->layout()->addWidget(new JLabel("	 "));
        panel6->layout()->addWidget(setTToTSignalsDone = new QPushButton(tr("Done")));
        //setTToTSignalsDone.addActionListener((ActionEvent e) -> {
        connect(setTToTSignalsDone, &QPushButton::clicked, [=]{
            setTToTSignalsDonePressed();
        });
        setTToTSignalsDone->setToolTip(tr("Click [%1] to accept any changes made above and close this dialog.eHint").arg(tr("Done")));

        //make this button the default button (return or enter activates)
        //Note: We have to invoke this later because we don't currently have a root pane
#if 0
        SwingUtilities.invokeLater(() -> {
            JRootPane rootPane = SwingUtilities.getRootPane(setTToTSignalsDone);
            rootPane.setDefaultButton(setTToTSignalsDone);
        });
#else
        setTToTSignalsDone->setDefault(true);
#endif
        panel6->layout()->addWidget(setTToTSignalsCancel = new QPushButton(tr("Cancel")));
        //setTToTSignalsCancel.addActionListener((ActionEvent e) -> {
        connect(setTToTSignalsCancel, &QPushButton::clicked, [=]{
            setTToTSignalsCancelPressed();
        });
        setTToTSignalsCancel->setToolTip(tr("Click [%1] to dismiss this dialog without making changes.").arg(tr("Cancel")));
        theContentPaneLayout->addWidget(panel6);
#if 0
        setSignalsAtThroatToThroatTurnoutsFrame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                setTToTSignalsCancelPressed(null);
            }
        });
#else
        class MyWindowListener : public WindowListener
        {
         LayoutEditorTools* let;
        public:
         MyWindowListener(LayoutEditorTools* let) {this->let = let;}
         void windowClosing(QCloseEvent *)
         {
          let->setTToTSignalsCancelPressed();
         }
        };
#endif
    }
    setPlaceAllHeads->setChecked(false);
    setupAllLogic->setChecked(false);

    turnout1ComboBox->setVisible(!setSignalsAtThroatToThroatTurnoutsFromMenuFlag);
    turnout2ComboBox->setVisible(!setSignalsAtThroatToThroatTurnoutsFromMenuFlag);

    if (setSignalsAtThroatToThroatTurnoutsFromMenuFlag) {
        ttotTurnoutName1Label->setText(tr("%1").arg(
                tr("Turnout") + " 1 "
                + tr("Name")) + ttotTurnoutName1);
        ttotTurnoutName2Label->setText(tr("%1").arg(
                tr("Turnout") + " 2 "
                + tr("Name")) + ttotTurnoutName2);

//        SwingUtilities.invokeLater(() -> {
            setSignalsAtTToTTurnoutsGetSaved();
//        });
    } else {
        ttotTurnoutName1Label->setText(
                tr("Turnout") + " 1 "
                + tr("Name"));
        ttotTurnoutName2Label->setText(
                tr("Turnout") + " 2 "
                + tr("Name"));
    }

    if (!setSignalsAtThroatToThroatTurnoutsOpenFlag) {
        setSignalsAtThroatToThroatTurnoutsFrame->resize(QSize());
        setSignalsAtThroatToThroatTurnoutsFrame->pack();
        setSignalsAtThroatToThroatTurnoutsOpenFlag = true;
    }
    setSignalsAtThroatToThroatTurnoutsFrame->setVisible(true);
}   //setSignalsAtTToTTurnouts

/*private*/ void LayoutEditorTools::setSignalsAtTToTTurnoutsGetSaved(/*ActionEvent* a*/) {
    if (!getTToTTurnoutInformation()) {
        return;
    }
    a1TToTSignalHeadComboBox->setSelectedItem(layoutTurnout1->getSignalB1());
    a2TToTSignalHeadComboBox->setSelectedItem(layoutTurnout1->getSignalB2());
    b1TToTSignalHeadComboBox->setSelectedItem(layoutTurnout1->getSignalC1());
    b2TToTSignalHeadComboBox->setSelectedItem(layoutTurnout1->getSignalC2());
    c1TToTSignalHeadComboBox->setSelectedItem(layoutTurnout2->getSignalB1());
    c2TToTSignalHeadComboBox->setSelectedItem(layoutTurnout2->getSignalB2());
    d1TToTSignalHeadComboBox->setSelectedItem(layoutTurnout2->getSignalC1());
    d2TToTSignalHeadComboBox->setSelectedItem(layoutTurnout2->getSignalC2());
}

/*private*/ void LayoutEditorTools::setTToTSignalsCancelPressed(/*ActionEvent a*/) {
    setSignalsAtThroatToThroatTurnoutsOpenFlag = false;
    setSignalsAtThroatToThroatTurnoutsFrame->setVisible(false);
}

/**
 * Tool to set signals at a turnout, including placing the signal icons and
 *		optionally setup of Simple Signal Logic for each signal head
 * <P>
 * This tool assumes left facing signal head icons have been selected, and
 *		will rotate the signal head icons accordingly.
 * <P>
 * This tool will place throat icons on the right side of the track, and
 *		continuing and diverging icons on the outside edge of the turnout.
 * <P>
 * This tool only places signal icons if the turnout is either mostly vertical
 *		or mostly horizontal. Some user adjustment may be needed.
 */

// display dialog for Set Signals at Turnout tool
/*public*/ void LayoutEditorTools::setSignalsAtTurnoutFromMenu( LayoutTurnout* to,
                MultiIconEditor* theEditor, JFrame* theFrame )
{
    turnoutFromMenu = true;
    layoutTurnout = to;
    turnout = to->getTurnout();
    turnoutNameField->setText(to->getTurnoutName());
    setSignalsAtTurnout(theEditor,theFrame);
}
/*public*/ void LayoutEditorTools::setSignalsAtTurnout( MultiIconEditor* theEditor, JFrame* theFrame )
{
 signalIconEditor = theEditor;
 signalFrame = theFrame;
 if (setSignalsOpen)
 {
  setSignalsFrame->setVisible(true);
  return;
 }
 // Initialize if needed
 if (setSignalsFrame == nullptr)
 {
  setSignalsFrame = new JmriJFrameX(tr("Signals At Turnout"), false, true);
  setSignalsFrame->addHelpMenu("package.jmri.jmrit.display.SetSignalsAtTurnout", true);
  setSignalsFrame->setLocation(70,30);
  //Container theContentPane = setSignalsFrame.getContentPane();
  QFont font;
  font.setPointSize(8);
  setSignalsFrame->setFont(font);
  QWidget* theContentPane = new QWidget();
  QVBoxLayout* centralWidgetLayout;
  theContentPane->setLayout(centralWidgetLayout = new QVBoxLayout(theContentPane/*, BoxLayout.Y_AXIS)*/));
  setSignalsFrame->setCentralWidget(theContentPane);
  QWidget* panel1 = new QWidget();
  panel1->setLayout(new QHBoxLayout());
  if (turnoutFromMenu)
  {
   QLabel* turnoutNameLabel = new QLabel( tr("Turnout")+" "+
                tr("Name")+" : "+layoutTurnout->getTurnoutName());
            panel1->layout()->addWidget(turnoutNameLabel);
  }
  else
  {
   QLabel* turnoutNameLabel = new QLabel( tr("Turnout")+" "+
                                                    tr("Name") );
   panel1->layout()->addWidget(turnoutNameLabel);
   panel1->layout()->addWidget(turnoutNameField);
   turnoutNameField->setToolTip(tr("Enter name (system or user) of turnout where signals are needed."));
   connect(turnoutNameField, SIGNAL(textChanged(QString)),this, SLOT(turnoutNameField_textChanged(QString)));
  }
  centralWidgetLayout->addWidget(panel1);
  QFrame* line = new QFrame();
  line->setObjectName(QString::fromUtf8("line"));
  line->setGeometry(QRect(10, 30, 571, 16));
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  centralWidgetLayout->addWidget(line);
  QWidget* panel2 = new QWidget();
  panel2->setLayout(new QHBoxLayout());
  QLabel* shTitle = new QLabel(tr("Signal Heads"));
  panel2->layout()->addWidget(shTitle);
  panel2->layout()->addWidget(new QLabel("   "));
  panel2->layout()->addWidget(getSavedSignalHeads = new QPushButton(tr("Get Saved")));
//        getSavedSignalHeads->layout()->addWidgetActionListener(new ActionListener() {
//                /*public*/ void actionPerformed(ActionEvent e) {
//                    turnoutSignalsGetSaved(e);
//                }
//            });
  connect(getSavedSignalHeads, SIGNAL(clicked()), this, SLOT(turnoutSignalsGetSaved()));
  getSavedSignalHeads->setToolTip( tr("Click to retrieve signal heads previously stored.") );
  centralWidgetLayout->addWidget(panel2);
  QWidget* panel21 = new QWidget();
  panel21->setLayout(new QHBoxLayout());
  QLabel* throatContinuingLabel = new QLabel(tr("Throat - Continuing")+" : ");
  panel21->layout()->addWidget(throatContinuingLabel);
  panel21->layout()->addWidget(throatContinuingField);
  centralWidgetLayout->addWidget(panel21);
  throatContinuingField->setToolTip(tr("Enter name (system or user) of signal head located here."));
  QWidget* panel22 = new QWidget();
  panel22->setLayout(new QHBoxLayout());
  panel22->layout()->addWidget(new QLabel("   "));
  panel22->layout()->addWidget(setThroatContinuing);
  setThroatContinuing->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
  panel22->layout()->addWidget(new QLabel("  "));
  panel22->layout()->addWidget(setupLogicThroatContinuing);
  setupLogicThroatContinuing->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
  centralWidgetLayout->addWidget(panel22);
  QWidget* panel31 = new QWidget();
  panel31->setLayout(new QHBoxLayout());
  QLabel* throatDivergingLabel = new QLabel(tr("Throat - Diverging")+" : ");
  panel31->layout()->addWidget(throatDivergingLabel);
  panel31->layout()->addWidget(throatDivergingField);
  centralWidgetLayout->addWidget(panel31);
  throatDivergingField->setToolTip(tr("Enter name (system or user) of signal head located here."));
  QWidget* panel32 = new QWidget();
  panel32->setLayout(new QHBoxLayout());
  panel32->layout()->addWidget(new QLabel("   "));
  panel32->layout()->addWidget(setThroatDiverging);
  setThroatDiverging->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
  panel32->layout()->addWidget(new QLabel("  "));
  panel32->layout()->addWidget(setupLogicThroatDiverging);
  setupLogicThroatDiverging->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
  centralWidgetLayout->addWidget(panel32);
  QWidget* panel41 = new QWidget();
  panel41->setLayout(new QHBoxLayout());
  QLabel* continuingLabel = new QLabel(tr("Continuing")+" : ");
  panel41->layout()->addWidget(continuingLabel);
  panel41->layout()->addWidget(continuingField);
  centralWidgetLayout->addWidget(panel41);
  continuingField->setToolTip(tr("Enter name (system or user) of signal head located here."));
  QWidget* panel42 = new QWidget();
  panel42->setLayout(new QHBoxLayout());
  panel42->layout()->addWidget(new QLabel("   "));
  panel42->layout()->addWidget(setContinuing);
  setContinuing->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
  panel42->layout()->addWidget(new QLabel("  "));
  panel42->layout()->addWidget(setupLogicContinuing);
  setupLogicContinuing->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
  centralWidgetLayout->addWidget(panel42);
  QWidget* panel51 = new QWidget();
  panel51->setLayout(new QHBoxLayout());
  QLabel* divergingLabel = new QLabel(tr("Diverging")+" : ");
  panel51->layout()->addWidget(divergingLabel);
  panel51->layout()->addWidget(divergingField);
  centralWidgetLayout->addWidget(panel51);
  divergingField->setToolTip(tr("Enter name (system or user) of signal head located here."));
  QWidget* panel52 = new QWidget();
  panel52->setLayout(new QHBoxLayout());
  panel52->layout()->addWidget(new QLabel("   "));
  panel52->layout()->addWidget(setDiverging);
  setDiverging->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
  panel52->layout()->addWidget(new QLabel("  "));
  panel52->layout()->addWidget(setupLogicDiverging);
  setupLogicDiverging->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
  centralWidgetLayout->addWidget(panel52);
  QFrame* line2 = new QFrame();
  line2->setObjectName(QString::fromUtf8("line"));
  line2->setGeometry(QRect(10, 30, 571, 16));
  line2->setFrameShape(QFrame::HLine);
  line2->setFrameShadow(QFrame::Sunken);
  centralWidgetLayout->addWidget(line2);
  QWidget* panel6 = new QWidget();
  panel6->setLayout(new QHBoxLayout());
  panel6->layout()->addWidget(changeSignalIcon = new QPushButton(tr("Change Signal Head Icon")));
//    changeSignalIcon->layout()->addWidgetActionListener(new ActionListener() {
//            /*public*/ void actionPerformed(ActionEvent e) {
//                signalFrame->setVisible(true);
//            }
//        });
  changeSignalIcon->setToolTip( tr("Click to change icons used to represent signal heads. (Use left-facing icons.)") );
  connect(changeSignalIcon, SIGNAL(clicked()), this, SLOT(on_changeSignalIcon_clicked()));
  panel6->layout()->addWidget(new QLabel("  "));
  panel6->layout()->addWidget(setSignalsDone = new QPushButton(tr("Done")));
//    setSignalsDone->layout()->addWidgetActionListener(new ActionListener() {
//        /*public*/ void actionPerformed(ActionEvent e) {
//            setSignalsDonePressed(e);
//        }
//    });
  connect(setSignalsDone, SIGNAL(clicked()), this, SLOT(setSignalsDonePressed()));
  setSignalsDone->setToolTip( tr("Click Done to perform tasks requested above and dismiss this dialog.") );
  panel6->layout()->addWidget(setSignalsCancel = new QPushButton(tr("Cancel")));
//    setSignalsCancel->layout()->addWidgetActionListener(new ActionListener() {
//        /*public*/ void actionPerformed(ActionEvent e) {
//            setSignalsCancelPressed(e);
//        }
//    });
 connect(setSignalsCancel, SIGNAL(clicked()), this, SLOT(setSignalsCancelPressed()));
 setSignalsCancel->setToolTip( tr("Click Cancel to dismiss this dialog without making changes.") );
 centralWidgetLayout->addWidget(panel6);
//    setSignalsFrame->addWindowListener(new java.awt.event.WindowAdapter() {
//        /*public*/ void windowClosing(java.awt.event.WindowEvent e) {
//            setSignalsCancelPressed(nullptr);
//        }
//    });
 setSignalsFrame->addWindowListener(new SSFWindowListener(this));
 if (turnoutFromMenu) turnoutSignalsGetSaved(nullptr);
 }
 //setSignalsFrame.pack();
 setSignalsFrame->setVisible(true);
 setSignalsOpen = true;
}
void LayoutEditorTools::windowClosing(QCloseEvent* /*e*/)
{
 if(setSignalsCancel !=NULL && setSignalsOpen)
  setSignalsCancelPressed();

}
void LayoutEditorTools::turnoutNameField_textChanged(QString text)
{
 turnoutNameField->setCompleter((static_cast<LayoutBlockManager*>(InstanceManager::getDefault("LayutBlockManager"))->getCompleter(text, true)));
}
void LayoutEditorTools::throatContinuingField_textChanged(QString text)
{
throatContinuingField->setCompleter(((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->getCompleter(text, true));
}
void LayoutEditorTools::throatDivergingField_textChanged(QString text)
{
  throatDivergingField->setCompleter(((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->getCompleter(text, true));
}

void LayoutEditorTools::continuingField_textChanged(QString text)
{
 continuingField->setCompleter(((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->getCompleter(text, true));
}

void LayoutEditorTools::divergingField_textChanged(QString text )
{
 divergingField->setCompleter(((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->getCompleter(text, true));
}

/*private*/ void LayoutEditorTools::turnoutSignalsGetSaved (ActionEvent* /*a*/) // SLOT[]
{
 if ( !getTurnoutInformation(false) ) return;
 throatContinuingField->setText(layoutTurnout->getSignalA1Name());
 throatDivergingField->setText(layoutTurnout->getSignalA2Name());
 continuingField->setText(layoutTurnout->getSignalB1Name());
 divergingField->setText(layoutTurnout->getSignalC1Name());
}
void LayoutEditorTools::on_changeSignalIcon_clicked() // SLOT[]
{
 signalFrame->setVisible(true);
}

/*private*/ void LayoutEditorTools::setSignalsCancelPressed (ActionEvent* /*a*/) // SLOT[]
{
 setSignalsOpen = false;
 turnoutFromMenu = false;
 setSignalsFrame->setVisible(false);
}

/*private*/ void LayoutEditorTools::setSignalsDonePressed (ActionEvent* /*a*/) // SLOT[]
{
 // process turnout name
 if ( !getTurnoutInformation(false) ) return;
 // process signal head names
 if ( !getTurnoutSignalHeadInformation() ) return;
 // place signals as requested
 if (setThroatContinuing->isChecked())
 {
  if (isHeadOnPanel(throatContinuingHead) &&
    (throatContinuingHead!=getHeadFromName(layoutTurnout->getSignalA1Name())))
  {
//    JOptionPane.showMessageDialog(setSignalsFrame,
//        java.text.MessageFormat.format(tr("SignalsError6"),
//            new Object[]{throatContinuingField->text().trimmed()}),
//                tr("Error"),JOptionPane.ERROR_MESSAGE);
      QMessageBox::critical(0, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\nbecause it is already on the panel.").arg(throatContinuingField->text().trimmed()));
    return;
  }
  else if ( (!layoutTurnoutHorizontal) && (!layoutTurnoutVertical) )
  {
//    JOptionPane.showMessageDialog(setSignalsFrame,
//        tr("InfoMessage2"),"",JOptionPane.INFORMATION_MESSAGE);
   QMessageBox::critical(0, tr("Information"), tr("Sorry, cannot place signal heads at turnouts\nthat are not oriented vertical or horizontal."));

   if (throatContinuingHead!=getHeadFromName(layoutTurnout->getSignalA1Name()))
   {
    removeSignalHeadFromPanel(layoutTurnout->getSignalA1Name());
    removeAssignment(throatContinuingHead);
    layoutTurnout->setSignalA1Name(throatContinuingField->text().trimmed());
   }
  }
  else
  {
   removeSignalHeadFromPanel(layoutTurnout->getSignalA1Name());
   placeThroatContinuing();
   removeAssignment(throatContinuingHead);
   layoutTurnout->setSignalA1Name(throatContinuingField->text().trimmed());
   needRedraw = true;
  }
 }
 else
 {
  int assigned = isHeadAssignedHere(throatContinuingHead,layoutTurnout);
  if (assigned == NONE)
  {
   if ( isHeadOnPanel(throatContinuingHead) &&
                        isHeadAssignedAnywhere(throatContinuingHead) )
   {
//        JOptionPane.showMessageDialog(setSignalsFrame,
//            java.text.MessageFormat.format(tr("SignalsError8"),
//                new Object[]{throatContinuingField->text().trimmed()}),
//                    tr("Error"),JOptionPane.ERROR_MESSAGE);
    QMessageBox::critical(0, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\nbecause it is already on the panel at a different place.").arg(throatContinuingField->text().trimmed()));
    return;
   }
   else
   {
    removeSignalHeadFromPanel(layoutTurnout->getSignalA1Name());
    removeAssignment(throatContinuingHead);
    layoutTurnout->setSignalA1Name(throatContinuingField->text().trimmed());
   }
  }
  else if (assigned!=A1)
  {
   // need to figure out what to do in this case.
  }
 }
 if ( (setThroatDiverging->isChecked()) && (throatDivergingHead!=nullptr) )
 {
  if (isHeadOnPanel(throatDivergingHead) &&
    (throatDivergingHead!=getHeadFromName(layoutTurnout->getSignalA2Name())))
  {
//    JOptionPane.showMessageDialog(setSignalsFrame,
//        java.text.MessageFormat.format(tr("SignalsError6"),
//            new Object[]{}),
//                tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(0, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\nbecause it is already on the panel.").arg(throatDivergingField->text().trimmed()));
   return;
  }
  else if ( (!layoutTurnoutHorizontal) && (!layoutTurnoutVertical) )
  {
//    JOptionPane.showMessageDialog(setSignalsFrame,
//        tr("InfoMessage2"),"",JOptionPane.INFORMATION_MESSAGE);
   QMessageBox::critical(0, tr("Information"), tr("Sorry, cannot place signal heads at turnouts\nthat are not oriented vertical or horizontal."));

  if (throatDivergingHead!=getHeadFromName(layoutTurnout->getSignalA2Name()))
  {
   removeSignalHeadFromPanel(layoutTurnout->getSignalA2Name());
   removeAssignment(throatDivergingHead);
   layoutTurnout->setSignalA2Name(throatDivergingField->text().trimmed());
  }
 }
 else
  {
   removeSignalHeadFromPanel(layoutTurnout->getSignalA2Name());
   placeThroatDiverging();
   removeAssignment(throatDivergingHead);
   layoutTurnout->setSignalA2Name(throatDivergingField->text().trimmed());
   needRedraw = true;
  }
 }
 else if (throatDivergingHead!=nullptr) {
 int assigned = isHeadAssignedHere(throatDivergingHead,layoutTurnout);
 if (assigned == NONE)
 {
  if (isHeadOnPanel(throatDivergingHead) &&
                        isHeadAssignedAnywhere(throatDivergingHead) ) {
//   JOptionPane.showMessageDialog(setSignalsFrame,
//            java.text.MessageFormat.format(tr("SignalsError8"),
//                new Object[]{throatDivergingField->text().trimmed()}),
//                    tr("Error"),JOptionPane.ERROR_MESSAGE);
      QMessageBox::critical(0, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\nbecause it is already on the panel at a different place.").arg(throatDivergingField->text().trimmed()));
  return;
 }
 else
  {
   removeSignalHeadFromPanel(layoutTurnout->getSignalA2Name());
   removeAssignment(throatDivergingHead);
   layoutTurnout->setSignalA2Name(throatDivergingField->text().trimmed());
  }
 }
 else if (assigned!=A2)
 {
  // need to figure out what to do in this case.
 }
}
else if (throatDivergingHead==nullptr)
 {
  removeSignalHeadFromPanel(layoutTurnout->getSignalA2Name());
  layoutTurnout->setSignalA2Name("");
 }
 if (setContinuing->isChecked())
 {
  if (isHeadOnPanel(continuingHead) &&
    (continuingHead!=getHeadFromName(layoutTurnout->getSignalB1Name())))
  {
//    JOptionPane.showMessageDialog(setSignalsFrame,
//        java.text.MessageFormat.format(tr("SignalsError6"),
//            new Object[]{continuingField->text().trimmed()}),
//                tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(0, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\nbecause it is already on the panel.").arg(continuingField->text().trimmed()));
   return;
  }
  else if ( (!layoutTurnoutHorizontal) && (!layoutTurnoutVertical) )
  {
//    JOptionPane.showMessageDialog(setSignalsFrame,
//        tr("InfoMessage2"),"",JOptionPane.INFORMATION_MESSAGE);
   QMessageBox::critical(0, tr("Information"), tr("Sorry, cannot place signal heads at turnouts\nthat are not oriented vertical or horizontal."));

   if (continuingHead!=getHeadFromName(layoutTurnout->getSignalB1Name()))
   {
    removeSignalHeadFromPanel(layoutTurnout->getSignalB1Name());
    removeAssignment(continuingHead);
    layoutTurnout->setSignalB1Name(continuingField->text().trimmed());
   }
  }
  else
  {
   removeSignalHeadFromPanel(layoutTurnout->getSignalB1Name());
    if (layoutTurnout->getContinuingSense()==Turnout::CLOSED)
        placeContinuing(continuingField->text().trimmed());
    else
        placeDiverging(continuingField->text().trimmed());
    removeAssignment(continuingHead);
    layoutTurnout->setSignalB1Name(continuingField->text().trimmed());
    needRedraw = true;
  }
 }
 else
 {
  int assigned = isHeadAssignedHere(continuingHead,layoutTurnout);
  if (assigned == NONE)
  {
   if (isHeadOnPanel(continuingHead)  &&
                        isHeadAssignedAnywhere(continuingHead) )
   {
//        JOptionPane.showMessageDialog(setSignalsFrame,
//            java.text.MessageFormat.format(tr("SignalsError8"),
//                new Object[]{continuingField->text().trimmed()}),
//                    tr("Error"),JOptionPane.ERROR_MESSAGE);
    QMessageBox::critical(0, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\nbecause it is already on the panel at a different place.").arg(continuingField->text().trimmed()));

    return;
   }
   else
   {
    removeSignalHeadFromPanel(layoutTurnout->getSignalB1Name());
    removeAssignment(continuingHead);
    layoutTurnout->setSignalB1Name(continuingField->text().trimmed());
   }
  }
  else if (assigned!=B1)
  {
   // need to figure out what to do in this case.
  }
 }
 if (setDiverging->isChecked())
 {
  if (isHeadOnPanel(divergingHead) &&
    (divergingHead!=getHeadFromName(layoutTurnout->getSignalC1Name())))
  {
//    JOptionPane.showMessageDialog(setSignalsFrame,
//        java.text.MessageFormat.format(tr("SignalsError6"),
//            new Object[]{divergingField->text().trimmed()}),
//                tr("Error"),JOptionPane.ERROR_MESSAGE);
      QMessageBox::critical(0, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\n because it is already on the panel.").arg(divergingField->text().trimmed()));
   return;
  }
  else if ( (!layoutTurnoutHorizontal) && (!layoutTurnoutVertical) )
  {
//    JOptionPane.showMessageDialog(setSignalsFrame,
//        tr("InfoMessage2"),"",JOptionPane.INFORMATION_MESSAGE);
   QMessageBox::information(0, tr("Information"), tr("Sorry, cannot place signal heads at turnouts\n                                                   that are not oriented vertical or horizontal."));

   if (divergingHead!=getHeadFromName(layoutTurnout->getSignalC1Name()))
   {
    removeSignalHeadFromPanel(layoutTurnout->getSignalC1Name());
    removeAssignment(divergingHead);
    layoutTurnout->setSignalC1Name(divergingField->text().trimmed());
   }
  }
  else
  {
   removeSignalHeadFromPanel(layoutTurnout->getSignalC1Name());
    if (layoutTurnout->getContinuingSense()==Turnout::CLOSED)
        placeDiverging(divergingField->text().trimmed());
    else
        placeContinuing(divergingField->text().trimmed());
    removeAssignment(divergingHead);
    layoutTurnout->setSignalC1Name(divergingField->text().trimmed());
    needRedraw = true;
  }
 }
 else
 {
  int assigned = isHeadAssignedHere(divergingHead,layoutTurnout);
  if (assigned == NONE)
  {
   if (isHeadOnPanel(divergingHead) &&
                        isHeadAssignedAnywhere(divergingHead) )
   {
//        JOptionPane.showMessageDialog(setSignalsFrame,
//            java.text.MessageFormat.format(tr("SignalsError8"),
//                new Object[]{divergingField->text().trimmed()}),
//                    tr("Error"),JOptionPane.ERROR_MESSAGE);
    QMessageBox::critical(0, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\nbecause it is already on the panel at a different place.").arg(divergingField->text().trimmed()));

    return;
   }
   else
   {
    removeSignalHeadFromPanel(layoutTurnout->getSignalC1Name());
    removeAssignment(divergingHead);
    layoutTurnout->setSignalC1Name(divergingField->text().trimmed());
   }
  }
  else if (assigned!=C1)
  {
   // need to figure out what to do in this case.
  }
 }
 // setup Logic if requested and enough information is available
 if (setupLogicThroatContinuing->isChecked())
 {
  setLogicThroatContinuing();
 }
 if ( (throatDivergingHead!=nullptr) && setupLogicThroatDiverging->isChecked() )
 {
  setLogicThroatDiverging();
 }
 if (setupLogicContinuing->isChecked())
 {
  setLogicContinuing();
 }
 if ( setupLogicDiverging->isChecked() )
 {
  setLogicDiverging();
 }
 // make sure this layout turnout is not linked to another
 layoutTurnout->setLinkType(LayoutTurnout::NO_LINK);
 layoutTurnout->setLinkedTurnoutName("");
 // finish up
 setSignalsOpen = false;
 turnoutFromMenu = false;
 setSignalsFrame->setVisible(false);
 if (needRedraw) {
 layoutEditor->redrawPanel();
 needRedraw = false;
 layoutEditor->setDirty();
 }
}
/*private*/ bool LayoutEditorTools::getTurnoutInformation(bool isCrossover)
{
 QString str = "";
 if ((!setSignalsAtTurnoutFromMenuFlag && !isCrossover)
         || (!setSignalsAtXoverTurnoutFromMenuFlag && isCrossover)) {
     turnout = nullptr;
     layoutTurnout = nullptr;
     if (isCrossover) {
         str = NamedBean::normalizeUserName(xoverTurnoutName);
     } else {
         str = turnoutComboBox->currentText();
     }
     if ((str == "") || str.isEmpty()) {
         JOptionPane::showMessageDialog(layoutEditor, tr("Error - No turnout name was entered. Please enter a turnout name or cancel."),
                 tr("Error"), JOptionPane::ERROR_MESSAGE);
         return false;
     }
     turnout = InstanceManager::turnoutManagerInstance()->getTurnout(str);
     if (turnout == nullptr) {
         JOptionPane::showMessageDialog(layoutEditor,
                 tr("Error - No turnout is defined for \"%1\". Please enter\na turnout name in the Turnout Table and on the panel.").arg(str),
                         tr("ErrorTitle"),
                 JOptionPane::ERROR_MESSAGE);
         return false;
     } else {
         QString uname = turnout->getUserName();
         if ((uname == nullptr) || uname.isEmpty() || uname!=(str)) {
             str = str.toUpper();
             if (isCrossover) {
                 xoverTurnoutName = str;
             } else {
                 turnoutComboBox->setCurrentText(str);
             }
         }
     }
     for (LayoutTrack* lt : layoutEditor->getLayoutTurnouts()) {
      LayoutTurnout* t = (LayoutTurnout*)lt;
         if (t->getTurnout() == turnout) {
             layoutTurnout = t;
             if (((t->getTurnoutType() == LayoutTurnout::DOUBLE_XOVER)
                     || (t->getTurnoutType() == LayoutTurnout::RH_XOVER)
                     || (t->getTurnoutType() == LayoutTurnout::LH_XOVER))
                     && (!isCrossover)) {
                 JOptionPane::showMessageDialog(layoutEditor,
                         tr("Sorry, this tool does\nnot work with crossover turnouts."), "",
                         JOptionPane::INFORMATION_MESSAGE);
                 setSignalsCancelPressed(nullptr);
                 return false;
             }
             if ((!((t->getTurnoutType() == LayoutTurnout::DOUBLE_XOVER)
                     || (t->getTurnoutType() == LayoutTurnout::RH_XOVER)
                     || (t->getTurnoutType() == LayoutTurnout::LH_XOVER)))
                     && isCrossover) {
                 JOptionPane::showMessageDialog(layoutEditor,
                         tr("Sorry, Set Signal Heads at Crossover does\nnot work with other types of turnouts."), "",
                         JOptionPane::INFORMATION_MESSAGE);
                 setXoverSignalsCancelPressed(nullptr);
                 return false;
             }
         }
     }
 }

 if (layoutTurnout != nullptr) {
     if (isCrossover) {
         QPointF coordsA = layoutTurnout->getCoordsA();
         QPointF coordsB = layoutTurnout->getCoordsB();
         placeSignalDirectionDEG = MathUtil::wrap360(90.0 - MathUtil::computeAngleDEG(coordsB, coordsA));
     } else {
         QPointF coordsA = layoutTurnout->getCoordsA();
         QPointF coordsCenter = layoutTurnout->getCoordsCenter();
         placeSignalDirectionDEG = MathUtil::wrap360(90.0 - MathUtil::computeAngleDEG(coordsCenter, coordsA));
     }
     return true;
 }
 JOptionPane::showMessageDialog(layoutEditor,
         tr("Error - Turnout \"%1\" is not drawn on the panel.\nPlease enter the name of a drawn turnout.").arg(str),
                  tr("Error"),
         JOptionPane::ERROR_MESSAGE);
 return false;
}   // getTurnoutInformation}

/*private*/ bool LayoutEditorTools::getTurnoutSignalHeadInformation()
{
 throatContinuingHead = getSignalHeadFromEntry(throatContinuingField,true,setSignalsFrame);
 if (throatContinuingHead==nullptr) return false;
 throatDivergingHead = getSignalHeadFromEntry(throatDivergingField,false,setSignalsFrame);
 continuingHead = getSignalHeadFromEntry(continuingField,true,setSignalsFrame);
 if (continuingHead==nullptr) return false;
 divergingHead = getSignalHeadFromEntry(divergingField,true,setSignalsFrame);
 if (divergingHead==nullptr) return false;
  return true;
}

/*private*/ void LayoutEditorTools::placeThroatContinuing()
{
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    if( layoutTurnoutHorizontal && layoutTurnoutThroatLeft ) {
        setSignalHeadOnPanel(2,throatContinuingField->text().trimmed(),
            (int)(layoutTurnout->getCoordsA().x()-testIcon->getIconWidth()),
            (int)(layoutTurnout->getCoordsA().y()+4) );
    }
    else if( layoutTurnoutHorizontal && (!layoutTurnoutThroatLeft) ) {
        setSignalHeadOnPanel(0,throatContinuingField->text().trimmed(),
            (int)(layoutTurnout->getCoordsA().x()),
            (int)(layoutTurnout->getCoordsA().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnoutVertical && layoutTurnoutThroatUp ) {
        setSignalHeadOnPanel(1,throatContinuingField->text().trimmed(),
            (int)(layoutTurnout->getCoordsA().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout->getCoordsA().y()-testIcon->getIconHeight()) );
    }
    else if( layoutTurnoutVertical && (!layoutTurnoutThroatUp) ) {
        setSignalHeadOnPanel(3,throatContinuingField->text().trimmed(),
            (int)(layoutTurnout->getCoordsA().x()+4),
            (int)(layoutTurnout->getCoordsA().y()) );
    }
}
/*private*/ void LayoutEditorTools::placeThroatDiverging() {
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    if( layoutTurnoutHorizontal && layoutTurnoutThroatLeft ) {
        setSignalHeadOnPanel(2,throatDivergingField->text().trimmed(),
            (int)(layoutTurnout->getCoordsA().x()-4-(2*testIcon->getIconWidth())),
            (int)(layoutTurnout->getCoordsA().y()+4) );
    }
    else if( layoutTurnoutHorizontal && (!layoutTurnoutThroatLeft) ) {
        setSignalHeadOnPanel(0,throatDivergingField->text().trimmed(),
            (int)(layoutTurnout->getCoordsA().x()+4+testIcon->getIconWidth()),
            (int)(layoutTurnout->getCoordsA().y()-4-testIcon->getIconHeight()));
    }
    else if( layoutTurnoutVertical && layoutTurnoutThroatUp ) {
        setSignalHeadOnPanel(1,throatDivergingField->text().trimmed(),
            (int)(layoutTurnout->getCoordsA().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout->getCoordsA().y()-4-(2*testIcon->getIconHeight())));
    }
    else if( layoutTurnoutVertical && (!layoutTurnoutThroatUp) ) {
        setSignalHeadOnPanel(3,throatDivergingField->text().trimmed(),
            (int)(layoutTurnout->getCoordsA().x()+4),
            (int)(layoutTurnout->getCoordsA().y()+4+testIcon->getIconHeight()));
    }
}
/*private*/ void LayoutEditorTools::placeContinuing(QString signalHeadName)
{
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    if( layoutTurnoutHorizontal && layoutTurnoutThroatLeft && layoutTurnoutBUp) {
        setSignalHeadOnPanel(0,signalHeadName,
            (int)(layoutTurnout->getCoordsB().x()),
            (int)(layoutTurnout->getCoordsB().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnoutHorizontal && layoutTurnoutThroatLeft && (!layoutTurnoutBUp) ) {
        setSignalHeadOnPanel(0,signalHeadName,
            (int)(layoutTurnout->getCoordsB().x()),
            (int)(layoutTurnout->getCoordsB().y()+4) );
    }
    else if( layoutTurnoutHorizontal && (!layoutTurnoutThroatLeft) && layoutTurnoutBUp ) {
        setSignalHeadOnPanel(2,signalHeadName,
            (int)(layoutTurnout->getCoordsB().x()-testIcon->getIconWidth()),
            (int)(layoutTurnout->getCoordsB().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnoutHorizontal && (!layoutTurnoutThroatLeft) && (!layoutTurnoutBUp) ) {
        setSignalHeadOnPanel(2,signalHeadName,
            (int)(layoutTurnout->getCoordsB().x()-testIcon->getIconWidth()),
            (int)(layoutTurnout->getCoordsB().y()+4) );
    }
    else if( layoutTurnoutVertical && layoutTurnoutThroatUp && layoutTurnoutBLeft ) {
        setSignalHeadOnPanel(3,signalHeadName,
            (int)(layoutTurnout->getCoordsB().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout->getCoordsB().y()) );
    }
    else if( layoutTurnoutVertical && layoutTurnoutThroatUp && (!layoutTurnoutBLeft) ) {
        setSignalHeadOnPanel(3,signalHeadName,
            (int)(layoutTurnout->getCoordsB().x()+4),
            (int)(layoutTurnout->getCoordsB().y()) );
    }
    else if( layoutTurnoutVertical && (!layoutTurnoutThroatUp) && layoutTurnoutBLeft ) {
        setSignalHeadOnPanel(1,signalHeadName,
            (int)(layoutTurnout->getCoordsB().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout->getCoordsB().y()-testIcon->getIconHeight()) );
    }
    else if( layoutTurnoutVertical && (!layoutTurnoutThroatUp) && (!layoutTurnoutBLeft) ) {
        setSignalHeadOnPanel(1,signalHeadName,
            (int)(layoutTurnout->getCoordsB().x()+4),
            (int)(layoutTurnout->getCoordsB().y()-testIcon->getIconHeight()) );
    }
}
/*private*/ void LayoutEditorTools::placeDiverging(QString signalHeadName) {
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    if( layoutTurnoutHorizontal && layoutTurnoutThroatLeft && layoutTurnoutBUp) {
        setSignalHeadOnPanel(0,signalHeadName,
            (int)(layoutTurnout->getCoordsC().x()),
            (int)(layoutTurnout->getCoordsC().y()+4) );
    }
    else if( layoutTurnoutHorizontal && layoutTurnoutThroatLeft && (!layoutTurnoutBUp) ) {
        setSignalHeadOnPanel(0,signalHeadName,
            (int)(layoutTurnout->getCoordsC().x()),
            (int)(layoutTurnout->getCoordsC().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnoutHorizontal && (!layoutTurnoutThroatLeft) && layoutTurnoutBUp ) {
        setSignalHeadOnPanel(2,signalHeadName,
            (int)(layoutTurnout->getCoordsC().x()-testIcon->getIconWidth()),
            (int)(layoutTurnout->getCoordsC().y()+4) );
    }
    else if( layoutTurnoutHorizontal && (!layoutTurnoutThroatLeft) && (!layoutTurnoutBUp) ) {
        setSignalHeadOnPanel(2,signalHeadName,
            (int)(layoutTurnout->getCoordsC().x()-testIcon->getIconWidth()),
            (int)(layoutTurnout->getCoordsC().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnoutVertical && layoutTurnoutThroatUp && layoutTurnoutBLeft ) {
        setSignalHeadOnPanel(3,signalHeadName,
            (int)(layoutTurnout->getCoordsC().x()+4),
            (int)(layoutTurnout->getCoordsC().y()) );
    }
    else if( layoutTurnoutVertical && layoutTurnoutThroatUp && (!layoutTurnoutBLeft) ) {
        setSignalHeadOnPanel(3,signalHeadName,
            (int)(layoutTurnout->getCoordsC().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout->getCoordsC().y()) );
    }
    else if( layoutTurnoutVertical && (!layoutTurnoutThroatUp) && layoutTurnoutBLeft ) {
        setSignalHeadOnPanel(1,signalHeadName,
            (int)(layoutTurnout->getCoordsC().x()+4),
            (int)(layoutTurnout->getCoordsC().y()-testIcon->getIconHeight()) );
    }
    else if( layoutTurnoutVertical && (!layoutTurnoutThroatUp) && (!layoutTurnoutBLeft) ) {
        setSignalHeadOnPanel(1,signalHeadName,
            (int)(layoutTurnout->getCoordsC().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout->getCoordsC().y()-testIcon->getIconHeight()) );
    }
}
/*private*/ void LayoutEditorTools::setLogicThroatContinuing() {
    TrackSegment* track = NULL;
    if (layoutTurnout->getContinuingSense()==Turnout::CLOSED)
        track = (TrackSegment*)layoutTurnout->getConnectB();
    else
        track = (TrackSegment*)layoutTurnout->getConnectC();
    if (track==nullptr)
    {
//        JOptionPane.showMessageDialog(setSignalsFrame,
//                tr("InfoMessage7"),"",JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsFrame, tr("Information"), tr("Cannot set up logic because all connections\nhave not been defined around this item."));
     return;
    }
    LayoutBlock* block = track->getLayoutBlock();
    if (block==nullptr)
    {
//        JOptionPane.showMessageDialog(setSignalsFrame,
//                tr("InfoMessage6"),"",JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsFrame, tr("Information"), tr("Cannot set up logic because blocks have\nnot been defined around this item."));
     return;
    }
    Sensor* occupancy = block->getOccupancySensor();
    if (occupancy==nullptr)
    {
//        JOptionPane.showMessageDialog(setSignalsFrame,
//            java.text.MessageFormat.format(tr("InfoMessage4"),
//                new Object[]{block->getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsFrame, tr("Information"), tr("Cannot set up logic because block \"%1\"\ndoesn''t have an occupancy sensor.").arg(block->getUserName()));

     return;
    }
    SignalHead* nextHead = getNextSignalFromObject(track,
            layoutTurnout, throatContinuingField->text().trimmed(), setSignalsFrame);
    if ( (nextHead==nullptr) && (!reachedEndBumper()) )
    {
//        JOptionPane.showMessageDialog(setSignalsFrame,
//            java.text.MessageFormat.format(tr("InfoMessage5"),
//                new Object[]{block->getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsFrame, tr("Information"), tr("Cannot set up logic because the next signal (in or\nat the end of block \"%1\") apparently is not yet defined.").arg(block->getUserName()));
     return;
    }
    if (throatDivergingHead!=nullptr)
    {
     if (!initializeBlockBossLogic(throatContinuingField->text().trimmed())) return;
        logic->setMode(BlockBossLogic::TRAILINGMAIN);
        logic->setTurnout(turnout->getSystemName());
        logic->setSensor1(occupancy->getSystemName());
        if (nextHead!=nullptr) {
            logic->setWatchedSignal1(nextHead->getSystemName(),false);
        }
        if (auxSignal!=nullptr) {
            logic->setWatchedSignal1Alt(auxSignal->getSystemName());
        }
        finalizeBlockBossLogic();
        return;
    }
    SignalHead* savedAuxSignal = auxSignal;
    TrackSegment* track2 = NULL;
    if (layoutTurnout->getContinuingSense()==Turnout::CLOSED)
        track2 = (TrackSegment*)layoutTurnout->getConnectC();
    else
        track2 = (TrackSegment*)layoutTurnout->getConnectB();
    if (track2==nullptr)
    {
//        JOptionPane.showMessageDialog(setSignalsFrame,
//                tr("InfoMessage7"),"",JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsFrame, tr("Information"), tr("Cannot set up logic because all connections\nhave not been defined around this item."));

     return;
    }
    LayoutBlock* block2 = track2->getLayoutBlock();
    if (block2==nullptr)
    {
//        JOptionPane.showMessageDialog(setSignalsFrame,
//                tr("InfoMessage6"),"",JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsFrame, tr("Information"), tr("Cannot set up logic because blocks have\nnot been defined around this item."));

     return;
    }
    Sensor* occupancy2 = block2->getOccupancySensor();
    if (occupancy2==nullptr)
    {
//        JOptionPane.showMessageDialog(setSignalsFrame,
//            java.text.MessageFormat.format(tr("InfoMessage4"),
//                new Object[]{block2.getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsFrame, tr("Information"), tr("Cannot set up logic because block \"%1\"\ndoesn''t have an occupancy sensor.").arg(block2->getUserName()));

     return;
    }
    SignalHead* nextHead2 = getNextSignalFromObject(track2,
            layoutTurnout, throatContinuingField->text().trimmed(), setSignalsFrame);
    if ( (nextHead2==nullptr) && (!reachedEndBumper()) )
    {
//        JOptionPane.showMessageDialog(setSignalsFrame,
//            java.text.MessageFormat.format(tr("InfoMessage5"),
//                new Object[]{block2.getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
        QMessageBox::information(setSignalsFrame, tr("Information"), tr("Cannot set up logic because the next signal (in or\nat the end of block \"%1\") apparently is not yet defined.").arg(block2->getUserName()));

        return;
    }
    if (!initializeBlockBossLogic(throatContinuingField->text().trimmed())) return;
    logic->setMode(BlockBossLogic::FACING);
    logic->setTurnout(turnout->getSystemName());
    logic->setWatchedSensor1(occupancy->getSystemName());
    logic->setWatchedSensor2(occupancy2->getSystemName());
    if (nextHead!=nullptr) {
        logic->setWatchedSignal1(nextHead->getSystemName(),false);
    }
    if (savedAuxSignal!=nullptr) {
        logic->setWatchedSignal1Alt(savedAuxSignal->getSystemName());
    }
    if (nextHead2!=nullptr) {
        logic->setWatchedSignal2(nextHead2->getSystemName());
    }
    if (auxSignal!=nullptr) {
        logic->setWatchedSignal2Alt(auxSignal->getSystemName());
    }
    if (!layoutTurnout->isMainlineC())
        logic->setLimitSpeed2(true);
    finalizeBlockBossLogic();
}
/*private*/ void LayoutEditorTools::setLogicThroatDiverging()
{
    TrackSegment* track = NULL;
    if (layoutTurnout->getContinuingSense()==Turnout::CLOSED)
        track = (TrackSegment*)layoutTurnout->getConnectC();
    else
        track = (TrackSegment*)layoutTurnout->getConnectB();
    if (track==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsFrame,
//                tr("InfoMessage7"),"",JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsFrame, tr("Information"), tr("Cannot set up logic because all connections\nhave not been defined around this item."));
     return;
    }
    LayoutBlock* block = track->getLayoutBlock();
    if (block==nullptr)
    {
//        JOptionPane.showMessageDialog(setSignalsFrame,
//                tr("InfoMessage6"),"",JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsFrame, tr("Information"), tr("Cannot set up logic because blocks have\nnot been defined around this item."));
     return;
    }
    Sensor* occupancy = block->getOccupancySensor();
    if (occupancy==nullptr)
    {
//        JOptionPane.showMessageDialog(setSignalsFrame,
//            java.text.MessageFormat.format(tr("InfoMessage4"),
//                new Object[]{block->getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsFrame, tr("Information"), tr("Cannot set up logic because block \"%1\"\ndoesn''t have an occupancy sensor."));
     return;
    }
    SignalHead* nextHead = getNextSignalFromObject(track,
            layoutTurnout, throatDivergingField->text().trimmed(), setSignalsFrame);
    if ( (nextHead==nullptr) && (!reachedEndBumper()) )
    {
//        JOptionPane.showMessageDialog(setSignalsFrame,
//            java.text.MessageFormat.format(tr("InfoMessage5"),
//                new Object[]{block->getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsFrame, tr("Information"), tr("Cannot set up logic because the next signal (in or\nat the end of block \"%1\") apparently is not yet defined.").arg(block->getUserName()));
     return;
    }
    if (!initializeBlockBossLogic(throatDivergingField->text().trimmed())) return;
    logic->setMode(BlockBossLogic::TRAILINGDIVERGING);
    logic->setTurnout(turnout->getSystemName());
    logic->setSensor1(occupancy->getSystemName());
    if (nextHead!=nullptr)
        logic->setWatchedSignal1(nextHead->getSystemName(),false);
    if (auxSignal!=nullptr) {
        logic->setWatchedSignal1Alt(auxSignal->getSystemName());
    }
    if (!layoutTurnout->isMainlineC())
        logic->setLimitSpeed2(true);
    finalizeBlockBossLogic();
}
/*private*/ void LayoutEditorTools::setLogicContinuing()
{
 TrackSegment* track = (TrackSegment*)layoutTurnout->getConnectA();
 if (track==nullptr)
 {
//        JOptionPane.showMessageDialog(setSignalsFrame,
//                tr("InfoMessage7"),"",JOptionPane.INFORMATION_MESSAGE);
  QMessageBox::information(setSignalsFrame, tr("Information"), tr("Cannot set up logic because all connections\nhave not been defined around this item."));
  return;
 }
 LayoutBlock* block = track->getLayoutBlock();
 if (block==nullptr)
 {
//        JOptionPane.showMessageDialog(setSignalsFrame,
//                tr("InfoMessage6"),"",JOptionPane.INFORMATION_MESSAGE);
  QMessageBox::information(setSignalsFrame, tr("Information"), tr("Cannot set up logic because blocks have\n                                                                     not been defined around this item."));
  return;
 }
 Sensor* occupancy = block->getOccupancySensor();
 if (occupancy==nullptr)
 {
//        JOptionPane.showMessageDialog(setSignalsFrame,
//            java.text.MessageFormat.format(tr("InfoMessage4"),
//                new Object[]{block->getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
  QMessageBox::information(setSignalsFrame, tr("Information"), tr("Cannot set up logic because block \"%1\"\ndoesn''t have an occupancy sensor.").arg(block->getUserName()));
  return;
 }
 SignalHead* nextHead = getNextSignalFromObject(track,
                layoutTurnout, continuingField->text().trimmed(), setSignalsFrame);
 if ( (nextHead==nullptr) && (!reachedEndBumper()) )
 {
//        JOptionPane.showMessageDialog(setSignalsFrame,
//            java.text.MessageFormat.format(tr("InfoMessage5"),
//                new Object[]{block->getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
  QMessageBox::information(setSignalsFrame, tr("Information"), tr("Cannot set up logic because the next signal (in or\nat the end of block \"%1\") apparently is not yet defined.").arg(block->getUserName()));
  return;
 }
 if (!initializeBlockBossLogic(continuingField->text().trimmed())) return;
 logic->setMode(BlockBossLogic::TRAILINGMAIN);
 logic->setTurnout(turnout->getSystemName());
 logic->setSensor1(occupancy->getSystemName());
 if (nextHead!=nullptr)
 {
  logic->setWatchedSignal1(nextHead->getSystemName(),false);
 }
 if (auxSignal!=nullptr)
 {
  logic->setWatchedSignal1Alt(auxSignal->getSystemName());
 }
 finalizeBlockBossLogic();
}
/*private*/ void LayoutEditorTools::setLogicDiverging()
{
 TrackSegment* track = (TrackSegment*)layoutTurnout->getConnectA();
 if (track==nullptr)
 {
//    JOptionPane.showMessageDialog(setSignalsFrame,
//            tr("InfoMessage7"),"",JOptionPane.INFORMATION_MESSAGE);
  QMessageBox::information(setSignalsFrame, tr("Information"), tr("Cannot set up logic because all connections\nhave not been defined around this item."));
  return;
 }
 LayoutBlock* block = track->getLayoutBlock();
 if (block==nullptr)
 {
//    JOptionPane.showMessageDialog(setSignalsFrame,
//            tr("InfoMessage6"),"",JOptionPane.INFORMATION_MESSAGE);
  QMessageBox::information(setSignalsFrame, tr("Information"), tr("Cannot set up logic because blocks have\nnot been defined around this item."));
  return;
 }
 Sensor* occupancy = block->getOccupancySensor();
 if (occupancy==nullptr)
 {
//    JOptionPane.showMessageDialog(setSignalsFrame,
//        java.text.MessageFormat.format(tr("InfoMessage4"),
//            new Object[]{block->getUserName()}),
//                NULL,JOptionPane.INFORMATION_MESSAGE);
  QMessageBox::information(setSignalsFrame, tr("Information"), tr("Cannot set up logic because block \"{0}\"\ndoesn''t have an occupancy sensor.").arg(block->getUserName()));
  return;
 }
 SignalHead* nextHead = getNextSignalFromObject(track,
        layoutTurnout, divergingField->text().trimmed(), setSignalsFrame);
 if ( (nextHead==nullptr) && (!reachedEndBumper()) )
 {
//    JOptionPane.showMessageDialog(setSignalsFrame,
//        java.text.MessageFormat.format(tr("InfoMessage5"),
//            new Object[]{block->getUserName()}),
//                NULL,JOptionPane.INFORMATION_MESSAGE);
  QMessageBox::information(setSignalsFrame, tr("Information"), tr("Cannot set up logic because the next signal (in or\nat the end of block \"%1\") apparently is not yet defined.").arg(block->getUserName()));
  return;
 }
 if (!initializeBlockBossLogic(divergingField->text().trimmed())) return;
 logic->setMode(BlockBossLogic::TRAILINGDIVERGING);
 logic->setTurnout(turnout->getSystemName());
 logic->setSensor1(occupancy->getSystemName());
 if (nextHead!=nullptr)
 {
    logic->setWatchedSignal1(nextHead->getSystemName(),false);
 }
 if (auxSignal!=nullptr)
 {
    logic->setWatchedSignal1Alt(auxSignal->getSystemName());
 }
 if (!layoutTurnout->isMainlineC())
    logic->setLimitSpeed2(true);
 finalizeBlockBossLogic();
}

/**
 * Utility routines used by multiple tools
 */

/**
 * Returns the layout turnout corresponding to a given turnout.
 * If require double crossover is requested, and error message is sent to the user if the
 *		layout turnout is not a double crossover, and NULL is returned.
 * If a layout turnout corresponding to the turnout is not found, an error message
 *		is sent to the user and NULL is returned.
 */
/*public*/ LayoutTurnout* LayoutEditorTools::getLayoutTurnoutFromTurnout(Turnout* turnout, bool requireDoubleXover, QString str, JFrame* theFrame)
{
 LayoutTurnout* t = NULL;
 for (LayoutTrack* lt : layoutEditor->getLayoutTurnouts()) {
  t = (LayoutTurnout*)lt;
  if (t->getTurnout() == turnout)
  {
   // have the layout turnout corresponding to the turnout
   if ( (t->getTurnoutType()==LayoutTurnout::DOUBLE_XOVER) && (!requireDoubleXover) )
   {
//                javax->swing.JOptionPane.showMessageDialog(theFrame,
//                            tr("InfoMessage1"),"",
//                            javax->swing.JOptionPane.INFORMATION_MESSAGE);
    QMessageBox::information(theFrame, tr("information"), tr("Sorry, this tool does\nnot work with crossover turnouts."));

    return NULL;
   }
   if (requireDoubleXover && (t->getTurnoutType()!=LayoutTurnout::DOUBLE_XOVER) ) {
//                javax->swing.JOptionPane.showMessageDialog(theFrame,
//                            tr("InfoMessage8"),"",
//                            javax->swing.JOptionPane.INFORMATION_MESSAGE);
    QMessageBox::information(theFrame, tr("Information"), tr("Sorry, Set Signals at Crossover does\nnot work with other types of turnouts."));
    return NULL;
   }
   return t;
  }
 }
    // layout turnout not found
//    JOptionPane.showMessageDialog(theFrame,
//            java.text.MessageFormat.format(tr("SignalsError3"),
//                    new Object[]{str}), tr("Error"),
//                        JOptionPane.ERROR_MESSAGE);
 QMessageBox::critical(theFrame, tr("Error"), tr("Error - Turnout \"%1\" is not drawn on the panel.\nPlease enter the name of a drawn turnout.").arg(str));
 return NULL;
}

/**
 * Returns the SignalHead* corresponding to an entry field in the specified dialog->
 *		This also takes care of UpperCase and trimming of leading and trailing blanks.
 *		If entry is required, and no entry is present, and error message is sent.
 *      An error message also results if a signal head with the entered name is not
 *      found in the SignalTable.
 */
/*public*/ SignalHead* LayoutEditorTools::getSignalHeadFromEntry(JTextField* signalName, bool requireEntry, JFrame* frame)
{
 QString str = signalName->text().trimmed();
 if ( (str==nullptr) || (str==("")) )
 {
  if (requireEntry)
  {
//            JOptionPane.showMessageDialog(frame,tr("SignalsError5"),
//                                tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(frame, tr("Error"), tr("Error - Signal head name was not entered. Please enter\na signal head name for required positions or cancel."));
  }
  return NULL;
 }
 SignalHead* head = ((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->getSignalHead(str);
 if (head==nullptr)
 {
//        JOptionPane.showMessageDialog(frame,
//                java.text.MessageFormat.format(tr("SignalsError4"),
//                    new Object[]{str}), tr("Error"),
//                        JOptionPane.ERROR_MESSAGE);
  QMessageBox::critical(0, tr("Error"), tr("Error - Signal head \"%1\" does not exist. Please correct name\nor enter this Signal Head in the Signal Table, and try again.").arg(str));
  return NULL ;
 }
 else if ( (head->getUserName()==nullptr) || (head->getUserName()==("")) ||
                                head->getUserName()!=(str) )
 {
//  str = str.toUpper();
  signalName->setText(str);
 }
 return (head);
}
/**
 * Returns a SignalHead* given a name
 */
/*public*/ SignalHead* LayoutEditorTools::getHeadFromName(QString str) {
    if ( (str==nullptr) || (str==("")) ) {
        return NULL;
    }
    return  (((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->getSignalHead(str));
}

/**
 * Places a signal head icon on the panel after rotation at the designated place, with all
 *		with all icons taken care of.
 */
/*public*/ void LayoutEditorTools::setSignalHeadOnPanel(int rotation, QString headName,int xLoc, int yLoc)
{
 SignalHeadIcon* l = new SignalHeadIcon(layoutEditor);
 l->setSignalHead(headName);
 l->setIcon(tr("Red"), signalIconEditor->getIcon(0));
 l->setIcon(tr("Flashing Red"), signalIconEditor->getIcon(1));
 l->setIcon(tr("Yellow"), signalIconEditor->getIcon(2));
 l->setIcon(tr("Flashing Yellow"), signalIconEditor->getIcon(3));
 l->setIcon(tr("Green"), signalIconEditor->getIcon(4));
 l->setIcon(tr("Flashing Green"), signalIconEditor->getIcon(5));
 l->setIcon(tr("Dark"), signalIconEditor->getIcon(6));
 l->setIcon(tr("Held"), signalIconEditor->getIcon(7));
 l->setIcon(tr("Lunar"), signalIconEditor->getIcon(8));
 l->setIcon(tr("FlashingLunar"), signalIconEditor->getIcon(9));
 ((Positionable*)l)->setLocation(xLoc,yLoc);
 if (rotation>0)
 {
  QStringListIterator e( l->getIconStateNames());
  while (e.hasNext())
  {
   l->getIcon(e.next())->setRotation(rotation,l);
  }
 }
 layoutEditor->putSignal(l);
}

/**
 * Returns an index if the specified signal head is assigned to the
 *		LayoutTurnout initialized. Otherwise returns the NONE index->
 *		The index specifies the turnout position of the signal head
 *		according to the code listed at the beginning of this module.
 */
/*private*/ int LayoutEditorTools::isHeadAssignedHere(SignalHead* head, LayoutTurnout* lTurnout) {
    QString sysName = head->getSystemName();
    QString uName = head->getUserName();
    QString name = lTurnout->getSignalA1Name();
    if ( (name!=nullptr) && (name.length()>0) && ((name==(uName)) ||
                    (name==(sysName))) ) return A1;
    name = lTurnout->getSignalA2Name();
    if ( (name!=nullptr) && (name.length()>0) && ((name==(uName)) ||
                    (name==(sysName))) ) return A2;
    name = lTurnout->getSignalA3Name();
    if ( (name!=nullptr) && (name.length()>0) && ((name==(uName)) ||
                    (name==(sysName))) ) return A3;
    name = lTurnout->getSignalB1Name();
    if ( (name!=nullptr) && (name.length()>0) && ((name==(uName)) ||
                    (name==(sysName))) ) return B1;
    name = lTurnout->getSignalB2Name();
    if ( (name!=nullptr) && (name.length()>0) && ((name==(uName)) ||
                    (name==(sysName))) ) return B2;
    name = lTurnout->getSignalC1Name();
    if ( (name!=nullptr) && (name.length()>0) && ((name==(uName)) ||
                    (name==(sysName))) ) return C1;
    name = lTurnout->getSignalC2Name();
    if ( (name!=nullptr) && (name.length()>0) && ((name==(uName)) ||
                    (name==(sysName))) ) return C2;
    name = lTurnout->getSignalD1Name();
    if ( (name!=nullptr) && (name.length()>0) && ((name==(uName)) ||
                    (name==(sysName))) ) return D1;
    name = lTurnout->getSignalD2Name();
    if ( (name!=nullptr) && (name.length()>0) && ((name==(uName)) ||
                    (name==(sysName))) ) return D2;
    return NONE;
}

/**
 * Returns true if an icon for the specified SignalHead* is on the panel
 */
/*public*/ bool LayoutEditorTools::isHeadOnPanel(SignalHead* head)
{
    SignalHeadIcon* h = NULL;
    for (int i=0;i<layoutEditor->signalList->size();i++) {
        h = layoutEditor->signalList->at(i);
        if (h->getSignalHead() == head) {
            return true;
        }
    }
    return false;
}
/**
 * Returns true if the specified Signal Head is assigned to an object
 *		on the panel, regardless of whether an icon is displayed or not
 */
/*public*/ bool LayoutEditorTools::isHeadAssignedAnywhere(SignalHead* head)
{
    QString sName = head->getSystemName();
    QString uName = head->getUserName();
    for (LayoutTrack* lt : layoutEditor->getLayoutTurnouts()) {
        LayoutTurnout* to = (LayoutTurnout*)lt;
        if ((to->getSignalA1Name()!=nullptr) &&
                (to->getSignalA1Name()==(sName) || ((uName!=nullptr) &&
                (to->getSignalA1Name()==(uName))))) return true;
        if ((to->getSignalA2Name()!=nullptr) &&
                (to->getSignalA2Name()==(sName) || ((uName!=nullptr) &&
                (to->getSignalA2Name()==(uName))))) return true;
        if ((to->getSignalA3Name()!=nullptr) &&
                (to->getSignalA3Name()==(sName) || ((uName!=nullptr) &&
                (to->getSignalA3Name()==(uName))))) return true;
        if ((to->getSignalB1Name()!=nullptr) &&
                (to->getSignalB1Name()==(sName) || ((uName!=nullptr) &&
                (to->getSignalB1Name()==(uName))))) return true;
        if ((to->getSignalB2Name()!=nullptr) &&
                (to->getSignalB2Name()==(sName) || ((uName!=nullptr) &&
                (to->getSignalB2Name()==(uName))))) return true;
        if ((to->getSignalC1Name()!=nullptr) &&
                (to->getSignalC1Name()==(sName) || ((uName!=nullptr) &&
                (to->getSignalC1Name()==(uName))))) return true;
        if ((to->getSignalC2Name()!=nullptr) &&
                (to->getSignalC2Name()==(sName) || ((uName!=nullptr) &&
                (to->getSignalC2Name()==(uName))))) return true;
        if ((to->getSignalD1Name()!=nullptr) &&
                (to->getSignalD1Name()==(sName) || ((uName!=nullptr) &&
                (to->getSignalD1Name()==(uName))))) return true;
        if ((to->getSignalD2Name()!=nullptr) &&
                (to->getSignalD2Name()==(sName) || ((uName!=nullptr) &&
                (to->getSignalD2Name()==(uName))))) return true;
    }
    for (LayoutTrack* lt : layoutEditor->getLayoutTurnouts()) {
        PositionablePoint* po = (PositionablePoint*)lt;
        if ((po->getEastBoundSignal()!=nullptr) &&
                (po->getEastBoundSignal()==(sName) || ((uName!=nullptr) &&
                (po->getEastBoundSignal()==(uName))))) return true;
        if ((po->getWestBoundSignal()!=nullptr) &&
                (po->getWestBoundSignal()==(sName) || ((uName!=nullptr) &&
                (po->getWestBoundSignal()==(uName))))) return true;
    }
    for (LayoutTrack* lt : layoutEditor->getLayoutTurnouts()) {
        LevelXing* x = (LevelXing*)lt;
        if ((x->getSignalAName()!=nullptr) &&
                (x->getSignalAName()==(sName) || ((uName!=nullptr) &&
                (x->getSignalAName()==(uName))))) return true;
        if ((x->getSignalBName()!=nullptr) &&
                (x->getSignalBName()==(sName) || ((uName!=nullptr) &&
                (x->getSignalBName()==(uName))))) return true;
        if ((x->getSignalCName()!=nullptr) &&
                (x->getSignalCName()==(sName) || ((uName!=nullptr) &&
                (x->getSignalCName()==(uName))))) return true;
        if ((x->getSignalDName()!=nullptr) &&
                (x->getSignalDName()==(sName) || ((uName!=nullptr) &&
                (x->getSignalDName()==(uName))))) return true;
    }
    return false;
}
/**
 * Removes the assignment of the specified SignalHead* to either a turnout,
 *		a positionable point, or a level crossing wherever it is assigned
 */
/*public*/ void LayoutEditorTools::removeAssignment(SignalHead* head)
{
    QString sName = head->getSystemName();
    QString uName = head->getUserName();
    for (LayoutTrack* lt : layoutEditor->getLayoutTurnouts()) {
        LayoutTurnout* to = (LayoutTurnout*)lt;
        if ((to->getSignalA1Name()!=nullptr) &&
                (to->getSignalA1Name()==(sName) || ((uName!=nullptr) &&
                (to->getSignalA1Name()==(uName))))) to->setSignalA1Name("");
        if ((to->getSignalA2Name()!=nullptr) &&
                (to->getSignalA2Name()==(sName) || ((uName!=nullptr) &&
                (to->getSignalA2Name()==(uName))))) to->setSignalA2Name("");
        if ((to->getSignalA3Name()!=nullptr) &&
                (to->getSignalA3Name()==(sName) || ((uName!=nullptr) &&
                (to->getSignalA3Name()==(uName))))) to->setSignalA3Name("");
        if ((to->getSignalB1Name()!=nullptr) &&
                (to->getSignalB1Name()==(sName) || ((uName!=nullptr) &&
                (to->getSignalB1Name()==(uName))))) to->setSignalB1Name("");
        if ((to->getSignalB2Name()!=nullptr) &&
                (to->getSignalB2Name()==(sName) || ((uName!=nullptr) &&
                (to->getSignalB2Name()==(uName))))) to->setSignalB2Name("");
        if ((to->getSignalC1Name()!=nullptr) &&
                (to->getSignalC1Name()==(sName) || ((uName!=nullptr) &&
                (to->getSignalC1Name()==(uName))))) to->setSignalC1Name("");
        if ((to->getSignalC2Name()!=nullptr) &&
                (to->getSignalC2Name()==(sName) || ((uName!=nullptr) &&
                (to->getSignalC2Name()==(uName))))) to->setSignalC2Name("");
        if ((to->getSignalD1Name()!=nullptr) &&
                (to->getSignalD1Name()==(sName) || ((uName!=nullptr) &&
                (to->getSignalD1Name()==(uName))))) to->setSignalD1Name("");
        if ((to->getSignalD2Name()!=nullptr) &&
                (to->getSignalD2Name()==(sName) || ((uName!=nullptr) &&
                (to->getSignalD2Name()==(uName))))) to->setSignalD2Name("");
    }
    for (LayoutTrack* lt : layoutEditor->getLayoutTurnouts()) {
        PositionablePoint* po = (PositionablePoint*)lt;
        if ((po->getEastBoundSignal()!=nullptr) &&
                (po->getEastBoundSignal()==(sName) || ((uName!=nullptr) &&
                (po->getEastBoundSignal()==(uName)))))
            po->setEastBoundSignal("");
        if ((po->getWestBoundSignal()!=nullptr) &&
                (po->getWestBoundSignal()==(sName) || ((uName!=nullptr) &&
                (po->getWestBoundSignal()==(uName)))))
            po->setWestBoundSignal("");
    }
    for (LayoutTrack* lt : layoutEditor->getLayoutTurnouts()) {
        LevelXing* x = (LevelXing*)lt;
        if ((x->getSignalAName()!=nullptr) &&
                (x->getSignalAName()==(sName) || ((uName!=nullptr) &&
                (x->getSignalAName()==(uName))))) x->setSignalAName("");
        if ((x->getSignalBName()!=nullptr) &&
                (x->getSignalBName()==(sName) || ((uName!=nullptr) &&
                (x->getSignalBName()==(uName))))) x->setSignalBName("");
        if ((x->getSignalCName()!=nullptr) &&
                (x->getSignalCName()==(sName) || ((uName!=nullptr) &&
                (x->getSignalCName()==(uName))))) x->setSignalCName("");
        if ((x->getSignalDName()!=nullptr) &&
                (x->getSignalDName()==(sName) || ((uName!=nullptr) &&
                (x->getSignalDName()==(uName))))) x->setSignalDName("");
    }
}

/**
 * Removes the SignalHead* with the specified name from the panel and from
 *		assignment to any turnout, positionable point, or level crossing
 */
//@SuppressWarnings("NULL")
/*public*/ void LayoutEditorTools::removeSignalHeadFromPanel(QString signalName) {
    if ( (signalName==nullptr) || (signalName.length()<1) ) return;
    SignalHead* head = ((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->
                                                    getSignalHead(signalName);
    removeAssignment(head);
    SignalHeadIcon* h = NULL;
    int index = -1;
    for (int i=0;(i<layoutEditor->signalList->size())&&(index==-1);i++) {
        h = layoutEditor->signalList->at(i);
        if (h->getSignalHead() == head) {
            index = i;
        }
    }
    if (index!=(-1)) {
        layoutEditor->signalList->remove(index);
        h->remove();
        h->dispose();
        needRedraw = true;
    }
}

/*
 * Initializes a BlockBossLogic for creation of a signal logic for the signal
 *		head named in "signalHeadName".
 * Should not be called until enough informmation has been gathered to allow
 *		configuration of the Simple Signal Logic.
 */
/*public*/ bool LayoutEditorTools::initializeBlockBossLogic(QString signalHeadName) {
    logic = BlockBossLogic::getStoppedObject(signalHeadName);
    if (logic==nullptr) {
        log->error("Trouble creating BlockBossLogic for '"+signalHeadName+"'.");
        return false;
    }
    return true;
}
/*
 * Finalizes a successfully created signal logic
 */
/*public*/ void LayoutEditorTools::finalizeBlockBossLogic() {
    if (logic==nullptr) return;
    logic->retain();
    logic->start();
    logic = NULL;
}

/*
 * Returns the signal head at the end of the block "track" is assigned to->
 *		"track" is the Track Segment leaving "object".
 *		"object" must be either an anchor point or one of the connecting
 *			points of a turnout or level crossing.
 * Note: returns 'NULL' is signal is not present where it is expected, or
 *		if an End Bumper is reached. To test for end bumper, use the
 *      associated routine "reachedEndBumper()". Reaching a turntable ray
 *		track connection is considered reaching an end bumper.
 * Note: Normally this routine requires a signal at any turnout it finds.
 *		However, if 'skipIncludedTurnout' is true, this routine will skip
 *		over an absent signal at an included turnout, that is a turnout
 *		with its throat track segment and its continuing track segment in
 *		the same block-> When this happens, the user is warned.
 */
/*public*/ SignalHead* LayoutEditorTools::getNextSignalFromObject(TrackSegment* track, QObject* object,
                            QString headName, JmriJFrame* frame) {
    hitEndBumper = false;
    auxSignal = NULL;
    TrackSegment* t = track;
    QObject* obj = object;
    bool inBlock = true;
    int type = 0;
    QObject* connect = NULL;
    while (inBlock) {
        if (t->getConnect1()==obj) {
            type = t->getType2();
            connect = t->getConnect2();
        }
        else {
            type = t->getType1();
            connect = t->getConnect1();
        }
        if (type==LayoutEditor::POS_POINT) {
            PositionablePoint* p = (PositionablePoint*)connect;
            if (p->getType()==PositionablePoint::END_BUMPER) {
                hitEndBumper = true;
                return NULL;
            }
            if (p->getConnect1()==t)
                t=p->getConnect2();
            else
                t=p->getConnect1();
            if (t==nullptr) return NULL;
            if (track->getLayoutBlock()!=t->getLayoutBlock()) {
                // p is a block boundary - should be signalled
                QString signalName;
                if (isAtWestEndOfAnchor(t,p))
                    signalName = p->getWestBoundSignal();
                else signalName = p->getEastBoundSignal();
                if ((signalName==nullptr)||(signalName==(""))) return NULL;
                return ((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->getSignalHead(signalName);
            }
            obj = p;
        }
        else if (type==LayoutEditor::TURNOUT_A) {
            // Reached turnout throat, should be signalled
            LayoutTurnout* to = (LayoutTurnout*)connect;
            QString signalName = to->getSignalA2Name();
            if ((!(signalName==nullptr))&&(!(signalName==(""))))
                auxSignal = ((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->
                                getSignalHead(signalName);
            signalName = to->getSignalA1Name();
            if ((signalName==nullptr)||(signalName==(""))) {
                if (!layoutEditor->skipIncludedTurnout)
                    return NULL;
                t = getContinuingTrack(to,type);
                if ( (t==nullptr) || (track->getLayoutBlock()!=t->getLayoutBlock()) )
                    return NULL;
                warnOfSkippedTurnout(frame, to->getTurnoutName(), headName);
                obj = to;
            }
            else {
                return ((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->
                                getSignalHead(signalName);
            }
        }
        else if (type==LayoutEditor::TURNOUT_B) {
            // Reached turnout continuing, should be signalled
            LayoutTurnout* to = (LayoutTurnout*)connect;
            QString signalName = to->getSignalB2Name();
            if (to->getContinuingSense()==Turnout::THROWN)
                signalName = to->getSignalC2Name();
            if ((!(signalName==nullptr))&&(!(signalName==(""))))
                auxSignal = ((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->
                                getSignalHead(signalName);
            if (to->getContinuingSense()==Turnout::CLOSED)
                signalName = to->getSignalB1Name();
            else
                signalName = to->getSignalC1Name();
            if ((signalName==nullptr)||(signalName==("")))  {
                if (!layoutEditor->skipIncludedTurnout)
                    return NULL;
                t = getContinuingTrack(to,type);
                if ( (t==nullptr) || (track->getLayoutBlock()!=t->getLayoutBlock()) )
                    return NULL;
                warnOfSkippedTurnout(frame, to->getTurnoutName(), headName);
                obj = to;
            }
            else {
                return ((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->
                                getSignalHead(signalName);
            }
        }
        else if (type==LayoutEditor::TURNOUT_C) {
            // Reached turnout diverging, should be signalled
            LayoutTurnout* to = (LayoutTurnout*)connect;
            QString signalName = to->getSignalC2Name();
            if (to->getContinuingSense()==Turnout::THROWN)
                signalName = to->getSignalB2Name();
            if ((!(signalName==nullptr))&&(!(signalName==(""))))
                auxSignal = ((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->
                                getSignalHead(signalName);
            if (to->getContinuingSense()==Turnout::CLOSED)
                signalName = to->getSignalC1Name();
            else
                signalName = to->getSignalB1Name();
            if ((signalName==nullptr)||(signalName==("")))  {
                if (!layoutEditor->skipIncludedTurnout)
                    return NULL;
                t = getContinuingTrack(to,type);
                if ( (t==nullptr) || (track->getLayoutBlock()!=t->getLayoutBlock()) )
                    return NULL;
                warnOfSkippedTurnout(frame, to->getTurnoutName(), headName);
                obj = to;
            }
            else {
                return ((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->
                                getSignalHead(signalName);
            }
        }
        else if (type==LayoutEditor::TURNOUT_D) {
            // Reached turnout xover 4, should be signalled
            LayoutTurnout* to = (LayoutTurnout*)connect;
            QString signalName = to->getSignalD2Name();
            if ((!(signalName==nullptr))&&(!(signalName==(""))))
                auxSignal = ((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->
                                getSignalHead(signalName);
            signalName = to->getSignalD1Name();
            if ((signalName==nullptr)||(signalName==("")))   {
                if (!layoutEditor->skipIncludedTurnout)
                    return NULL;
                t = getContinuingTrack(to,type);
                if ( (t==nullptr) || (track->getLayoutBlock()!=t->getLayoutBlock()) )
                    return NULL;
                warnOfSkippedTurnout(frame, to->getTurnoutName(), headName);
                obj = to;
            }
            else {
                return ((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->
                                getSignalHead(signalName);
            }
        }
        else if (type==LayoutEditor::LEVEL_XING_A) {
            // Reached level crossing that may or may not be a block boundary
            LevelXing* x = (LevelXing*)connect;
            QString signalName = x->getSignalAName();
            if ((signalName!=nullptr)&&(signalName!=("")))
                return ((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->
                                getSignalHead(signalName);
            t=(TrackSegment*)x->getConnectC();
            if (t==nullptr) return NULL;
            if (track->getLayoutBlock()!=t->getLayoutBlock()) return NULL;
            obj = x;
        }
        else if (type==LayoutEditor::LEVEL_XING_B) {
            // Reached level crossing that may or may not be a block boundary
            LevelXing* x = (LevelXing*)connect;
            QString signalName = x->getSignalBName();
            if ((signalName!=nullptr)&&(signalName!=("")))
                return ((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->                                getSignalHead(signalName);
            t=(TrackSegment*)x->getConnectD();
            if (t==nullptr) return NULL;
            if (track->getLayoutBlock()!=t->getLayoutBlock()) return NULL;
            obj = x;
        }
        else if (type==LayoutEditor::LEVEL_XING_C) {
            // Reached level crossing that may or may not be a block boundary
            LevelXing* x = (LevelXing*)connect;
            QString signalName = x->getSignalCName();
            if ((signalName!=nullptr)&&(signalName!=("")))
                return ((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->
                                getSignalHead(signalName);
            t=(TrackSegment*)x->getConnectA();
            if (t==nullptr) return NULL;
            if (track->getLayoutBlock()!=t->getLayoutBlock()) return NULL;
            obj = x;
        }
        else if (type==LayoutEditor::LEVEL_XING_D) {
            // Reached level crossing that may or may not be a block boundary
            LevelXing* x = (LevelXing*)connect;
            QString signalName = x->getSignalDName();
            if ((signalName!=nullptr)&&(signalName!=("")))
                return ((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->
                                getSignalHead(signalName);
            t=(TrackSegment*)x->getConnectB();
            if (t==nullptr) return NULL;
            if (track->getLayoutBlock()!=t->getLayoutBlock()) return NULL;
            obj = x;
        }
        else if (type==LayoutEditor::SLIP_A) {
            LayoutSlip* sl = (LayoutSlip*)connect;
            QString signalName = sl->getSignalA2Name();
            if ((!(signalName==nullptr))&&(!(signalName==(""))))
                auxSignal = ((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->
                                getSignalHead(signalName);
            signalName = sl->getSignalA1Name();
            if ((signalName==nullptr)||(signalName==(""))) {
                if (!layoutEditor->skipIncludedTurnout)
                    return NULL;
                t = getContinuingTrack(sl,type);
                if ( (t==nullptr) || (track->getLayoutBlock()!=t->getLayoutBlock()) )
                    return NULL;
                warnOfSkippedTurnout(frame, sl->getTurnoutName(), headName);
                obj = sl;
            }
            else {
                return ((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->
                                getSignalHead(signalName);
            }
        }
        else if (type==LayoutEditor::SLIP_B) {
            LayoutSlip* sl = (LayoutSlip*)connect;
            QString signalName;
            if(sl->getTurnoutType()==LayoutSlip::DOUBLE_SLIP){
                signalName = sl->getSignalB2Name();
                if ((!(signalName==nullptr))&&(!(signalName==(""))))
                    auxSignal = ((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->
                                    getSignalHead(signalName);
            }
            signalName = sl->getSignalB1Name();
            if ((signalName==nullptr)||(signalName==(""))) {
                if (!layoutEditor->skipIncludedTurnout)
                    return NULL;
                t = getContinuingTrack(sl,type);
                if ( (t==nullptr) || (track->getLayoutBlock()!=t->getLayoutBlock()) )
                    return NULL;
                warnOfSkippedTurnout(frame, sl->getTurnoutName(), headName);
                obj = sl;
            }
            else {
                return ((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->
                                getSignalHead(signalName);
            }
        }
        else if (type==LayoutEditor::SLIP_C) {
            LayoutSlip* sl = (LayoutSlip*)connect;
            QString signalName;
            if(sl->getTurnoutType()==LayoutSlip::DOUBLE_SLIP){
                signalName = sl->getSignalC2Name();
                if ((!(signalName==nullptr))&&(!(signalName==(""))))
                    auxSignal = ((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->
                                    getSignalHead(signalName);
            }
            signalName = sl->getSignalC1Name();
            if ((signalName==nullptr)||(signalName==(""))) {
                if (!layoutEditor->skipIncludedTurnout)
                    return NULL;
                t = getContinuingTrack(sl,type);
                if ( (t==nullptr) || (track->getLayoutBlock()!=t->getLayoutBlock()) )
                    return NULL;
                warnOfSkippedTurnout(frame, sl->getTurnoutName(), headName);
                obj = sl;
            }
            else {
                return ((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->
                                getSignalHead(signalName);
            }
        }
        else if (type==LayoutEditor::SLIP_D) {
            LayoutSlip* sl = (LayoutSlip*)connect;
            QString signalName = sl->getSignalD2Name();
            if ((!(signalName==nullptr))&&(!(signalName==(""))))
                auxSignal = ((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->
                                getSignalHead(signalName);
            signalName = sl->getSignalD1Name();
            if ((signalName==nullptr)||(signalName==(""))) {
                if (!layoutEditor->skipIncludedTurnout)
                    return NULL;
                t = getContinuingTrack(sl,type);
                if ( (t==nullptr) || (track->getLayoutBlock()!=t->getLayoutBlock()) )
                    return NULL;
                warnOfSkippedTurnout(frame, sl->getTurnoutName(), headName);
                obj = sl;
            }
            else {
                return ((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->
                                getSignalHead(signalName);
            }
        }

        else if (type>=LayoutEditor::TURNTABLE_RAY_OFFSET) {
            hitEndBumper = true;
            return NULL;
        }
    }
    return NULL;
}

/*private*/ void LayoutEditorTools::warnOfSkippedTurnout(JmriJFrame* frame, QString turnoutName, QString headName)
{
//    JOptionPane.showMessageDialog(frame,
//            java.text.MessageFormat.format(tr("SignalsWarn2"),
//                new Object[]{turnoutName, headName}),
//                    NULL,JOptionPane.WARNING_MESSAGE);
    QMessageBox::warning(frame, tr("Warning"), tr("Warning - Per your instructions, the absense of signals at turnout \"%1\"\nwas ignored when setting up logic for signal head \"%2\".").arg(turnoutName).arg(headName));
}
/*private*/ TrackSegment* LayoutEditorTools::getContinuingTrack(LayoutTurnout* to, int type)	{
    int ty = to->getTurnoutType();
    if ( (ty==LayoutTurnout::RH_TURNOUT) || (ty==LayoutTurnout::LH_TURNOUT) ||
                                                (ty==LayoutTurnout::RH_TURNOUT) ) {
        if (type == LayoutEditor::TURNOUT_A) {
            if (to->getContinuingSense()==Turnout::CLOSED) return (TrackSegment*)to->getConnectB();
            else return (TrackSegment*)to->getConnectC();
        }
        else return (TrackSegment*)to->getConnectA();
    }
    else if ( (ty==LayoutTurnout::DOUBLE_XOVER) || (ty==LayoutTurnout::RH_XOVER) ||
                (ty==LayoutTurnout::LH_XOVER) ) {
        if (type==LayoutEditor::TURNOUT_A) return (TrackSegment*)to->getConnectB();
        else if (type==LayoutEditor::TURNOUT_B) return (TrackSegment*)to->getConnectA();
        else if (type==LayoutEditor::TURNOUT_C) return (TrackSegment*)to->getConnectD();
        else if (type==LayoutEditor::TURNOUT_D) return (TrackSegment*)to->getConnectC();
    }
    log->error ("Bad connection type around turnout "+to->getTurnoutName());
    return NULL;
}

/*
 * Returns 'true' if an end bumper was reached during the last call to
 *		GetNextSignalFromObject. Also used in the odd case of reaching a
 *		turntable ray track connection, which is treated as an end
 *		bumper here.
 */
/*public*/ bool LayoutEditorTools::reachedEndBumper() {return hitEndBumper;}
/*
 * Returns 'true' if "track" enters a block boundary at the west(north) end of
 *		"point". Returns "false" otherwise. If track is neither horizontal or
 *      vertical, assumes horizontal, as done when setting signals at block boundary->
 *	"track" is a TrackSegment* connected to "point".
 *  "point" is an anchor point serving as a block boundary->
 */
/*static*/ /*public*/ bool LayoutEditorTools::isAtWestEndOfAnchor(TrackSegment* t, PositionablePoint* p)
{
 if (p->getType() == PositionablePoint::EDGE_CONNECTOR)
 {
  if (p->getConnect1() == t) {
      if (p->getConnect1Dir() == Path::NORTH || p->getConnect1Dir() == Path::WEST) {
          return false;
      }
      return true;
  } else {
      if (p->getConnect1Dir() == Path::NORTH || p->getConnect1Dir() == Path::WEST) {
          return true;
      }
      return false;
  }
 }

    TrackSegment* tx = NULL;
    if (p->getConnect1()==t)
        tx = p->getConnect2();
    else if (p->getConnect2() == t)
        tx = p->getConnect1();
    else {
        log->error("track not connected to anchor point");
        return false;
    }
    QPointF coords1;
    if (t->getConnect1()==p)
        coords1 = LayoutEditor::getCoords(t->getConnect2(),t->getType2());
    else
        coords1 = LayoutEditor::getCoords(t->getConnect1(),t->getType1());
    QPointF coords2;
    if(tx!=nullptr) {
        if (tx->getConnect1()==p)
            coords2 = LayoutEditor::getCoords(tx->getConnect2(),tx->getType2());
        else
            coords2 = LayoutEditor::getCoords(tx->getConnect1(),tx->getType1());
    } else {
        if (t->getConnect1()==p)
            coords2 = LayoutEditor::getCoords(t->getConnect1(),t->getType1());
        else
            coords2 = LayoutEditor::getCoords(t->getConnect2(),t->getType2());
    }
    double delX = coords1.x() - coords2.x();
    double delY = coords1.y() - coords2.y();
    if (qAbs(delX) > 2.0*qAbs(delY)) {
        // track is Horizontal
        if (delX>0.0) return false;
        else return true;
    }
    else if(qAbs(delY) > 2.0*qAbs(delX)) {
        // track is Vertical
        if (delY>0.0) return false;
        else return true;
    }
    // track is not vertical or horizontal, assume horizontal
//		log->error ("Track is not vertical or horizontal at anchor");
    if (delX>0.0) return false;
    return true;
}

/**
 * Tool to set signals at a block boundary, including placing the signal icons and
 *		setup of Simple Signal Logic for each signal head
 * <P>
 * Block boundary must be at an Anchor Point on the LayoutEditor panel.
 */


// display dialog for Set Signals at Block Boundary tool
/*public*/ void LayoutEditorTools::setSignalsAtBlockBoundaryFromMenu(PositionablePoint* p, MultiIconEditor* theEditor, JFrame *theFrame )
{

    boundaryFromMenu = true;
    boundary = p;
    block1NameField->setText(boundary->getConnect1()->getLayoutBlock()->getId());
    block2NameField->setText(boundary->getConnect2()->getLayoutBlock()->getId());
    setSignalsAtBlockBoundary(theEditor,theFrame);
    return;
}

/*public*/ void LayoutEditorTools::setSignalsAtBlockBoundary( MultiIconEditor* theEditor, JFrame* theFrame )
{
 signalIconEditor = theEditor;
 signalFrame = theFrame;
 if (setSignalsAtBoundaryOpen)
 {
  setSignalsAtBoundaryFrame->setVisible(true);
  return;
 }
 // Initialize if needed
 if (setSignalsAtBoundaryFrame == nullptr)
 {
  setSignalsAtBoundaryFrame = new JmriJFrameX(tr("Set Signals at Block Boundary"), false, true );
  //setSignalsAtBoundaryFrame->layout()->addWidgetHelpMenu("package.jmri.jmrit.display.SetSignalsAtBoundary", true);
  //setSignalsAtBoundaryFrame->setLocation(70,30);
  QWidget* theContentPane = new QWidget(setSignalsAtBoundaryFrame);
  setSignalsAtBoundaryFrame->setCentralWidget(theContentPane);
  QVBoxLayout* centralWidgetLayout;
  theContentPane->setLayout(centralWidgetLayout=new QVBoxLayout(theContentPane/*, BoxLayout.Y_AXIS)*/));
  QWidget* panel11 = new QWidget();
  panel11->setLayout(new QHBoxLayout());
  if (boundaryFromMenu)
  {
   QLabel* block1NameLabel = new QLabel( tr("Block")+" 1 "+
                tr("Name")+" : "+boundary->getConnect1()->getLayoutBlock()->getId());
   panel11->layout()->addWidget(block1NameLabel);
  }
  else
  {
   QLabel* block1NameLabel = new QLabel( tr("Block")+" 1 "+ tr("Name")+" : ");
   panel11->layout()->addWidget(block1NameLabel);
   panel11->layout()->addWidget(block1NameField);
   block1NameField->setToolTip(tr("Enter name of one block where signals are needed at block boundary."));
  }
  centralWidgetLayout->addWidget(panel11);
  QWidget* panel12 = new QWidget();
  panel12->setLayout(new QHBoxLayout());
  if (boundaryFromMenu)
  {
   QLabel* block2NameLabel = new QLabel( tr("Block")+" 2 "+
                tr("Name")+" : "+boundary->getConnect2()->getLayoutBlock()->getId());
   panel12->layout()->addWidget(block2NameLabel);
  }
  else
  {
   QLabel* block2NameLabel = new QLabel( tr("Block")+" 2 "+tr("Name")+" : ");
   panel12->layout()->addWidget(block2NameLabel);
   panel12->layout()->addWidget(block2NameField);
   block2NameField->setToolTip(tr("Enter name of one block where signals are needed at block boundary."));
  }
  centralWidgetLayout->addWidget(panel12);
  QFrame* line = new QFrame();
  line->setObjectName(QString::fromUtf8("line"));
  line->setGeometry(QRect(10, 30, 571, 16));
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  centralWidgetLayout->addWidget(/*new JSeparator(JSeparator::HORIZONTAL)*/line);
  QWidget* panel2 = new QWidget();
  panel2->setLayout(new QHBoxLayout());
  QLabel* shTitle = new QLabel(tr("Signal Heads"));
  panel2->layout()->addWidget(shTitle);
  panel2->layout()->addWidget(new QLabel("   "));
  panel2->layout()->addWidget(getAnchorSavedSignalHeads = new QPushButton(tr("Get Saved")));
//        getAnchorSavedSignalHeads->layout()->addWidgetActionListener(new ActionListener() {
//                /*public*/ void actionPerformed(ActionEvent e) {
//                    getSavedAnchorSignals(e);
//                }
//            });
  connect(getAnchorSavedSignalHeads, SIGNAL(clicked()), this, SLOT(getSavedAnchorSignals(ActionEvent* /*e*/)));
  getAnchorSavedSignalHeads->setToolTip( tr("Click to retrieve signal heads previously stored.") );
  centralWidgetLayout->addWidget(panel2);
  QWidget* panel21 = new QWidget();
  panel21->setLayout(new QHBoxLayout());
  QLabel* eastBoundLabel = new QLabel(tr("East (or South) Bound")+" : ");
  panel21->layout()->addWidget(eastBoundLabel);
  panel21->layout()->addWidget(eastBoundField);
  centralWidgetLayout->addWidget(panel21);
  eastBoundField->setToolTip(tr("Enter name (system or user) of signal head for left-bound (east) or down-bound (south) trains."));
  QWidget* panel22 = new QWidget();
  panel22->setLayout(new QHBoxLayout());
  panel22->layout()->addWidget(new QLabel("   "));
  panel22->layout()->addWidget(setEastBound);
  setEastBound->setToolTip(tr("Check to place icon for above signal head on panel near anchor point."));
  panel22->layout()->addWidget(new QLabel("  "));
  panel22->layout()->addWidget(setupLogicEastBound);
  setupLogicEastBound->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
  centralWidgetLayout->addWidget(panel22);
  QWidget* panel31 = new QWidget();
  panel31->setLayout(new QHBoxLayout());
  QLabel* westBoundLabel = new QLabel(tr("West (or North) Bound")+" : ");
  panel31->layout()->addWidget(westBoundLabel);
  panel31->layout()->addWidget(westBoundField);
  centralWidgetLayout->addWidget(panel31);
  westBoundField->setToolTip(tr("Enter name (system or user) of signal head for right-bound (west) or up-bound (north) trains."));
  QWidget* panel32 = new QWidget();
  panel32->setLayout(new QHBoxLayout());
  panel32->layout()->addWidget(new QLabel("   "));
  panel32->layout()->addWidget(setWestBound);
  setWestBound->setToolTip(tr("AnchorCheck to place icon for above signal head on panel near turnout."));
  panel32->layout()->addWidget(new QLabel("  "));
  panel32->layout()->addWidget(setupLogicWestBound);
  setupLogicWestBound->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
  centralWidgetLayout->addWidget(panel32);
  QFrame* line2 = new QFrame();
  line2->setObjectName(QString::fromUtf8("line"));
  line2->setGeometry(QRect(10, 30, 571, 16));
  line2->setFrameShape(QFrame::HLine);
  centralWidgetLayout->addWidget(/*new JSeparator(JSeparator::HORIZONTAL)*/line2);
  QWidget* panel6 = new QWidget();
  panel6->setLayout(new QHBoxLayout());
  panel6->layout()->addWidget(changeSignalAtBoundaryIcon = new QPushButton(tr("Change Signal Head Icon")));
//    changeSignalAtBoundaryIcon->layout()->addWidgetActionListener(new ActionListener() {
//            /*public*/ void actionPerformed(ActionEvent e) {
//                signalFrame->setVisible(true);
//            }
//        });
  connect(changeSignalAtBoundaryIcon, SIGNAL(clicked()), this, SLOT(changeSignalAtBoundaryIcon_clicked()));
  changeSignalAtBoundaryIcon->setToolTip( tr("Click to change icons used to represent signal heads. (Use left-facing icons.)") );
  panel6->layout()->addWidget(new QLabel("  "));
  panel6->layout()->addWidget(setSignalsAtBoundaryDone = new QPushButton(tr("Done")));
//    setSignalsAtBoundaryDone->layout()->addWidgetActionListener(new ActionListener() {
//        /*public*/ void actionPerformed(ActionEvent e) {
//            setSignalsAtBoundaryDonePressed(e);
//        }
//    });
  connect(setSignalsAtBoundaryDone, SIGNAL(clicked()), this, SLOT(setSignalsAtBoundaryDonePressed()));
  setSignalsAtBoundaryDone->setToolTip( tr("Click Done to perform tasks requested above and dismiss this dialog.") );
  panel6->layout()->addWidget(setSignalsAtBoundaryCancel = new QPushButton(tr("Cancel")));
//    setSignalsAtBoundaryCancel->layout()->addWidgetActionListener(new ActionListener() {
//        /*public*/ void actionPerformed(ActionEvent e) {
//            setSignalsAtBoundaryCancelPressed(e);
//        }
//    });
  connect(setSignalsAtBoundaryCancel, SIGNAL(clicked()), SLOT(setSignalsAtBoundaryCancelPressed()));
  setSignalsAtBoundaryCancel->setToolTip( tr("Click Cancel to dismiss this dialog without making changes.") );
  centralWidgetLayout->addWidget(panel6);
// TODO: figure out how to process the close event.
//    setSignalsAtBoundaryFrame->layout()->addWidgetWindowListener(new java.awt.event.WindowAdapter() {
//        /*public*/ void windowClosing(java.awt.event.WindowEvent e) {
//            setSignalsAtBoundaryCancelPressed(nullptr);
//        }
//    });

  if (boundaryFromMenu) getSavedAnchorSignals(nullptr);
 }
 //setSignalsAtBoundaryFrame.pack();
 setSignalsAtBoundaryFrame->setVisible(true);
 setSignalsAtBoundaryOpen = true;
}
void LayoutEditorTools::changeSignalAtBoundaryIcon_clicked() // SLOT[]
{
  signalFrame->setVisible(true);
}

/*private*/ void LayoutEditorTools::getSavedAnchorSignals (ActionEvent* /*a*/) // SLOT[]
{
    if ( !getBlockInformation() ) return;
    eastBoundField->setText(boundary->getEastBoundSignal());
    westBoundField->setText(boundary->getWestBoundSignal());
}
/*private*/ void LayoutEditorTools::setSignalsAtBoundaryCancelPressed (ActionEvent* /*a*/) // SLOT() []
{
    setSignalsAtBoundaryOpen = false;
    boundaryFromMenu = false;
    setSignalsAtBoundaryFrame->setVisible(false);
}
/*private*/ void LayoutEditorTools::setSignalsAtBoundaryDonePressed (ActionEvent* /*a*/) // SLOT[]
{
 if ( !getBlockInformation() ) return;
 eastBoundHead = getSignalHeadFromEntry(eastBoundField,false,setSignalsAtBoundaryFrame);
 westBoundHead = getSignalHeadFromEntry(westBoundField,false,setSignalsAtBoundaryFrame);
 if ( (eastBoundHead==nullptr) && (westBoundHead==nullptr) )
 {
//        JOptionPane.showMessageDialog(setSignalsAtBoundaryFrame,
//                        tr("SignalsError12"),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
  QMessageBox::critical(0, tr("Error"), tr("Error - No signal heads entered.\nPlease enter signal heads or cancel."));
        return;
 }
 // place or update signals as requested
 if ( (eastBoundHead!=nullptr) && setEastBound->isChecked() )
 {
  if (isHeadOnPanel(eastBoundHead) &&
                (eastBoundHead!=getHeadFromName(boundary->getEastBoundSignal())))
  {
//            JOptionPane.showMessageDialog(setSignalsAtBoundaryFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{eastBoundField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(0, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel because it is already on the panel.").arg(eastBoundField->text()));
   return;
  }
  else if ( (!trackHorizontal) && (!trackVertical) )
  {
//            JOptionPane.showMessageDialog(setSignalsAtBoundaryFrame,
//                tr("InfoMessage3"),"",JOptionPane.INFORMATION_MESSAGE);
   QMessageBox::information(0, tr("Information"), tr("Sorry, cannot place signal heads at anchor points                                                              where track is not oriented vertical or horizontal."));

   if (eastBoundHead!=getHeadFromName(boundary->getEastBoundSignal()))
   {
    removeSignalHeadFromPanel(boundary->getEastBoundSignal());
    removeAssignment(eastBoundHead);
    boundary->setEastBoundSignal(eastBoundField->text().trimmed());
   }
  }
  else
  {
   removeSignalHeadFromPanel(boundary->getEastBoundSignal());
   placeEastBound();
   removeAssignment(eastBoundHead);
   boundary->setEastBoundSignal(eastBoundField->text().trimmed());
   needRedraw = true;
  }
 }
 else if ( (eastBoundHead!=nullptr) &&
            (eastBoundHead!=getHeadFromName(boundary->getEastBoundSignal())) &&
            (eastBoundHead!=getHeadFromName(boundary->getWestBoundSignal())) )
 {
  if (isHeadOnPanel(eastBoundHead))
  {
//            JOptionPane.showMessageDialog(setSignalsAtBoundaryFrame,
//                java.text.MessageFormat.format(tr("SignalsError13"),
//                    new Object[]{eastBoundField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(0, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this block boundary                                               because it is already on the panel at a different place.").arg(eastBoundField->text()));

   return;
  }
  else
  {
   removeSignalHeadFromPanel(boundary->getEastBoundSignal());
   removeAssignment(eastBoundHead);
   boundary->setEastBoundSignal(eastBoundField->text().trimmed());
  }
 }
 else if ( (eastBoundHead!=nullptr) &&
            (eastBoundHead==getHeadFromName(boundary->getWestBoundSignal())) )
 {
// need to figure out what to do in this case.
 }
 if ( (westBoundHead!=nullptr) && setWestBound->isChecked() )
 {
  if (isHeadOnPanel(westBoundHead) &&
                (westBoundHead!=getHeadFromName(boundary->getWestBoundSignal())))
  {
//            JOptionPane.showMessageDialog(setSignalsAtBoundaryFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{westBoundField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(0, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel                                                      because it is already on the panel.").arg(westBoundField->text()));

   return;
  }
  else if ( (!trackHorizontal) && (!trackVertical) )
  {
//            JOptionPane.showMessageDialog(setSignalsAtBoundaryFrame,
//                tr("InfoMessage3"),"",JOptionPane.INFORMATION_MESSAGE);
   QMessageBox::information(0, tr("Information"), tr("Sorry, cannot place signal heads at anchor points\n                                                              where track is not oriented vertical or horizontal."));

   if (westBoundHead!=getHeadFromName(boundary->getWestBoundSignal()))
   {
    removeSignalHeadFromPanel(boundary->getWestBoundSignal());
    removeAssignment(westBoundHead);
    boundary->setWestBoundSignal(westBoundField->text().trimmed());
   }
  }
  else
  {
    removeSignalHeadFromPanel(boundary->getWestBoundSignal());
    placeWestBound();
    removeAssignment(westBoundHead);
    boundary->setWestBoundSignal(westBoundField->text().trimmed());
    needRedraw = true;
  }
 }
 else if ( (westBoundHead!=nullptr) &&
            (westBoundHead!=getHeadFromName(boundary->getEastBoundSignal())) &&
            (westBoundHead!=getHeadFromName(boundary->getWestBoundSignal())) )
 {
  if (isHeadOnPanel(westBoundHead))
  {
//            JOptionPane.showMessageDialog(setSignalsAtBoundaryFrame,
//                java.text.MessageFormat.format(tr("SignalsError13"),
//                    new Object[]{westBoundField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(0, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this block boundary\n                                            because it is already on the panel at a different place.").arg(westBoundField->text()));

   return;
  }
  else
  {
   removeSignalHeadFromPanel(boundary->getWestBoundSignal());
   removeAssignment(westBoundHead);
   boundary->setWestBoundSignal(westBoundField->text().trimmed());
  }
 }
 else if ( (westBoundHead!=nullptr) &&
            (westBoundHead==getHeadFromName(boundary->getEastBoundSignal())) )
 {
// need to figure out what to do in this case.
 }
 if ( (eastBoundHead!=nullptr) && setupLogicEastBound->isChecked() )
 {
        setLogicEastBound();
 }
 if ( (westBoundHead!=nullptr) && setupLogicWestBound->isChecked() )
 {
   setLogicWestBound();
 }
 setSignalsAtBoundaryOpen = false;
 boundaryFromMenu = false;
 setSignalsAtBoundaryFrame->setVisible(false);
 if (needRedraw)
 {
  layoutEditor->redrawPanel();
  needRedraw = false;
  layoutEditor->setDirty();
 }
}

/*
* Do some thing here for end bumpers.
*
*/
/*private*/ bool LayoutEditorTools::getBlockInformation()
{
    //might have to do something to trick it with an end bumper
    if (!boundaryFromMenu) {
        block1 = getBlockFromEntry(block1NameField);
        if (block1==nullptr) return false;
        block2 = getBlockFromEntry(block2NameField);
        if (block2==nullptr) return false;
        PositionablePoint* p = NULL;
        boundary = NULL;
        for (PositionablePoint* p : layoutEditor->getPositionablePoints()) {
            if (p->getType() == PositionablePoint::ANCHOR) {
                LayoutBlock* bA = NULL;
                LayoutBlock* bB = NULL;
                if (p->getConnect1()!=nullptr) bA = p->getConnect1()->getLayoutBlock();
                if (p->getConnect2()!=nullptr) bB = p->getConnect2()->getLayoutBlock();
                if ( (bA!=nullptr) && (bB!=nullptr) && (bA!=bB) ) {
                    if ( ( (bA==block1) && (bB==block2) ) ||
                                ( (bA==block2) && (bB==block1) ) ) {
                        boundary = p;
                    }
                }
            }
        }
        if (boundary==nullptr) {
//            JOptionPane.showMessageDialog(setSignalsAtBoundaryFrame,
//                        tr("SignalsError7"),
//                                tr("Error"),JOptionPane.ERROR_MESSAGE);
            QMessageBox::critical(0, tr("Error"), tr("Error - The two blocks entered do not join at an anchor point.\nPlease enter the correct blocks and try again."));
            return false;
        }
    }
    // set track orientation at boundary
    eastTrack = NULL;
    westTrack = NULL;
    TrackSegment* track1 = boundary->getConnect1();
    QPointF point1;
    if (track1->getConnect1()==boundary)
        point1 = layoutEditor->getCoords(track1->getConnect2(),track1->getType2());
    else
        point1 = layoutEditor->getCoords(track1->getConnect1(),track1->getType1());
    TrackSegment* track2 = boundary->getConnect2();
    QPointF point2;
    if(boundary->getType()==PositionablePoint::END_BUMPER)
        return true;
    if (track2->getConnect1()==boundary)
        point2 = layoutEditor->getCoords(track2->getConnect2(),track2->getType2());
    else
        point2 = layoutEditor->getCoords(track2->getConnect1(),track2->getType1());
    double delX = point1.x() - point2.x();
    double delY = point1.y() - point2.y();
    trackVertical = false;
    trackHorizontal = false;
    if (qAbs(delX) > 2.0*qAbs(delY)) {
        trackHorizontal = true;
        if (delX>0.0) {
            eastTrack = track1;
            westTrack = track2;
        }
        else {
            eastTrack = track2;
            westTrack = track1;
        }
    }
    if (qAbs(delY) > 2.0*qAbs(delX)) {
        trackVertical = true;
        if (delY>0.0) {
            eastTrack = track1;		// south
            westTrack = track2;		// north
        }
        else {
            eastTrack = track2;		// south
            westTrack = track1;		// north
        }
    }
    if (eastTrack==nullptr) {
        // did not meet the horizontal or vertical test, assume horizontal
        if (delX>0.0) {
            eastTrack = track1;
            westTrack = track2;
        }
        else {
            eastTrack = track2;
            westTrack = track1;
        }
    }
    return true;
}

/*private*/ LayoutBlock* LayoutEditorTools::getBlockFromEntry(JTextField* blockNameField) {
    QString str = blockNameField->text().trimmed();
    if ( (str==nullptr) || (str==("")) ) {
//        JOptionPane.showMessageDialog(setSignalsAtBoundaryFrame,tr("SignalsError9"),
//                                tr("Error"),JOptionPane.ERROR_MESSAGE);
        QMessageBox::critical(0, tr("Error"), tr("Error - Block name was not entered. Please\nenter the name of a block on the panel."));
        return NULL;
    }
    LayoutBlock* block = (LayoutBlock*)qobject_cast<LayoutBlockManager*>(InstanceManager::getDefault("LayoutBlockManager"))->
                                                    getByUserName(str);
    if (block==nullptr)
    {
//        JOptionPane.showMessageDialog(setSignalsAtBoundaryFrame,
//                java.text.MessageFormat.format(tr("SignalsError10"),
//                    new Object[]{str}), tr("Error"),
//                        JOptionPane.ERROR_MESSAGE);
     QMessageBox::critical(0, tr("Error"), tr("Error - Block \"%1\" does not exist. Please correct name\nor assign this block to track on the panel.").arg(str));
        return NULL ;
    }
    if ( !block->isOnPanel(layoutEditor) ) {
//        JOptionPane.showMessageDialog(setSignalsAtBoundaryFrame,
//                java.text.MessageFormat.format(tr("SignalsError11"),
//                    new Object[]{str}), tr("Error"),
//                        JOptionPane.ERROR_MESSAGE);
        QMessageBox::critical(0, tr("Error"), tr("Error - Block \"%1\" exists, but is not\nassigned to track on this panel.").arg(str));
        return NULL ;
    }
    return (block);
}

/*private*/ void LayoutEditorTools::placeEastBound() {
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    QPointF p = boundary->getCoordsCenter();
    if (trackHorizontal) {
        setSignalHeadOnPanel(2,eastBoundField->text().trimmed(),
            (int)(p.x()-testIcon->getIconHeight()-8),
            (int)(p.y()-testIcon->getIconWidth()) );
    }
    else if (trackVertical) {
        setSignalHeadOnPanel(1,eastBoundField->text().trimmed(),
            (int)(p.x()-4-testIcon->getIconHeight()),
            (int)(p.y()-4-testIcon->getIconWidth()) );
    }
}
/*private*/ void LayoutEditorTools::placeWestBound() {
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    QPointF p = boundary->getCoordsCenter();
    if (trackHorizontal) {
        setSignalHeadOnPanel(0,westBoundField->text().trimmed(),
            (int)(p.x()+4),
            (int)(p.y()+5) );
    }
    else if (trackVertical) {
        setSignalHeadOnPanel(3,westBoundField->text().trimmed(),
            (int)(p.x()+5),
            (int)(p.y())+4 );
    }
}
/*private*/ void LayoutEditorTools::setLogicEastBound() {
    LayoutBlock* eastBlock = eastTrack->getLayoutBlock();
    Sensor* eastBlockOccupancy = eastBlock->getOccupancySensor();
    if (eastBlockOccupancy==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAtBoundaryFrame,
//            java.text.MessageFormat.format(tr("InfoMessage4"),
//                new Object[]{eastBlock.getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
        QMessageBox::information(0, tr("Information"), tr("Cannot set up logic because block \"%1\"\n                                                          doesn''t have an occupancy sensor.").arg(eastBlock->getUserName()));
        return;
    }
    SignalHead* nextHead = getNextSignalFromObject(eastTrack,
            boundary, eastBoundField->text().trimmed(), setSignalsAtBoundaryFrame);
    if ( (nextHead==nullptr) && (!reachedEndBumper()) ) {
//        JOptionPane.showMessageDialog(setSignalsAtBoundaryFrame,
//            java.text.MessageFormat.format(tr("InfoMessage5"),
//                new Object[]{eastBlock.getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
        QMessageBox::information(0, tr("Information"), tr("Cannot set up logic because the next signal (in or\n                                                          at the end of block \"%1\") apparently is not yet defined.").arg(eastBlock->getUserName()));
        return;
    }
    if (!initializeBlockBossLogic(eastBoundField->text().trimmed())) return;
    logic->setMode(BlockBossLogic::SINGLEBLOCK);
    logic->setSensor1(eastBlockOccupancy->getSystemName());
    if (nextHead!=nullptr) {
        logic->setWatchedSignal1(nextHead->getSystemName(),false);
    }
    if (auxSignal!=nullptr) {
        logic->setWatchedSignal1Alt(auxSignal->getSystemName());
    }
    finalizeBlockBossLogic();
}
/*private*/ void LayoutEditorTools::setLogicWestBound() {
    LayoutBlock* westBlock = westTrack->getLayoutBlock();
    Sensor* westBlockOccupancy = westBlock->getOccupancySensor();
    if (westBlockOccupancy==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAtBoundaryFrame,
//            java.text.MessageFormat.format(tr("InfoMessage4"),
//                new Object[]{westBlock.getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
        QMessageBox::information(0, tr("Information"), tr("Cannot set up logic because block \"%1\"\n                                                          doesn''t have an occupancy sensor.").arg(westBlock->getUserName()));
        return;
    }
    SignalHead* nextHead = getNextSignalFromObject(westTrack,
            boundary, westBoundField->text().trimmed(), setSignalsAtBoundaryFrame);
    if ( (nextHead==nullptr) && (!reachedEndBumper()) ) {
//        JOptionPane.showMessageDialog(setSignalsAtBoundaryFrame,
//            java.text.MessageFormat.format(tr("InfoMessage5"),
//                new Object[]{westBlock.getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
        QMessageBox::information(0, tr("Information"), tr("Cannot set up logic because the next signal (in or\n                                                          at the end of block \"%1\") apparently is not yet defined.").arg(westBlock->getUserName()));

        return;
    }
    if (!initializeBlockBossLogic(westBoundField->text().trimmed())) return;
    logic->setMode(BlockBossLogic::SINGLEBLOCK);
    logic->setSensor1(westBlockOccupancy->getSystemName());
    if (nextHead!=nullptr) {
        logic->setWatchedSignal1(nextHead->getSystemName(),false);
    }
    if (auxSignal!=nullptr) {
        logic->setWatchedSignal1Alt(auxSignal->getSystemName());
    }
    finalizeBlockBossLogic();
}

/**
 * Tool to set signals at a double crossover turnout, including placing the
 *		signal icons and setup of Simple Signal Logic for each signal head
 * <P>
 * This tool assumes left facing signal head icons have been selected, and
 *		will rotate the signal head icons accordingly.
 * <P>
 * This tool will place icons on the outside edge of the turnout.
 * <P>
 * At least one signal at each of the four connection points is
 *		required. A second signal at each is optional.
 * <P>
 * This tool only places signal icons if the turnout is either mostly vertical
 *		or mostly horizontal. Some user adjustment may be needed.
 */


// display dialog for Set Signals at Crossover Turnout tool
/*public*/ void LayoutEditorTools::setSignalsAtXoverTurnoutFromMenu( LayoutTurnout* to,
                MultiIconEditor* theEditor, JFrame* theFrame )
{
 xoverFromMenu = true;
 layoutTurnout = to;
 turnout = to->getTurnout();
 xoverType = layoutTurnout->getTurnoutType();
 if ( (xoverType!=LayoutTurnout::DOUBLE_XOVER) && (xoverType!=LayoutTurnout::RH_XOVER) &&
                                     (xoverType!=LayoutTurnout::LH_XOVER) )
 {
  log->error ("entered Set Signals at XOver, with a non-crossover turnout");
  return;
 }
 xoverTurnoutName = layoutTurnout->getTurnoutName();
 setSignalsAtXoverTurnoutFromMenuFlag = true;
 setSignalsAtXoverTurnout(theEditor, theFrame);
 setSignalsAtXoverTurnoutFromMenuFlag = false;
}

/*public*/ void LayoutEditorTools::setSignalsAtXoverTurnout( MultiIconEditor* theEditor, JFrame* theFrame )
{
 signalIconEditor = theEditor;
 signalFrame = theFrame;
 if (!setSignalsAtXoverTurnoutFromMenuFlag)
 {
  QList<LayoutTurnout*> xovers = QList<LayoutTurnout*>();
  for (LayoutTrack* lt : layoutEditor->getLayoutTurnouts())
  {
   if(qobject_cast<LayoutTurnout*>(lt))
   {
    LayoutTurnout* layoutTurnout = qobject_cast<LayoutTurnout*>(lt);
   if ((layoutTurnout->getTurnoutType() == LayoutTurnout::RH_XOVER)
           || (layoutTurnout->getTurnoutType() == LayoutTurnout::LH_XOVER)
           || (layoutTurnout->getTurnoutType() == LayoutTurnout::DOUBLE_XOVER)) {
       xovers.append(layoutTurnout);
   }
   }
  }
  QComboBox* jcb = new QComboBox();
  //xovers.toArray(new LayoutTurnout[xovers.size()]));
  //jcb->addItems(xovers);
  foreach (LayoutTurnout* lt, xovers) {
   jcb->addItem(lt->getName(), VPtr<LayoutTurnout>::asQVariant(lt));
  }
  jcb->setEditable(true);
  JOptionPane::showMessageDialog(layoutEditor, VPtr<QComboBox>::asQVariant(jcb),
          tr("Enter name of crossover turnout"),
          JOptionPane::QUESTION_MESSAGE);
  LayoutTurnout* layoutTurnout = VPtr<LayoutTurnout>::asPtr(jcb->currentData());
  xoverTurnoutName = layoutTurnout->getTurnoutName();

  if (xoverTurnoutName.length()<3) return;  // cancelled
 }
 if (!getTurnoutInformation(true))
  return;
 xoverTurnoutNameLabel->setText(tr("Turnout")+" "+
                        tr("Name")+" : "+xoverTurnoutName);
 xoverType = layoutTurnout->getTurnoutType();
 if (setSignalsAtXoverOpen)
 {
  setSignalsAtXoverFrame->setVisible(true);
  return;
 }
 // Initialize if needed
 if (setSignalsAtXoverFrame == nullptr)
 {
  setSignalsAtXoverFrame = new JmriJFrameX(tr("Set Signals at Crossover"), false, true);
//        setSignalsAtXoverFrame->layout()->addWidgetHelpMenu("package.jmri.jmrit.display.SetSignalsAtXoverTurnout", true);
//        setSignalsAtXoverFrame->setLocation(70,30);
  QFont font;
  font.setPointSize(8);
  setSignalsAtXoverFrame->setFont(font);
  QScrollArea* scrollArea = new QScrollArea();
  scrollArea->setWidgetResizable(true);
  setSignalsAtXoverFrame->setCentralWidget(scrollArea);
  QWidget* scrollWidget = new QWidget;
  scrollArea->setWidget(scrollWidget);
  QVBoxLayout* centralWidgetLayout;
  scrollWidget->setLayout(centralWidgetLayout = new QVBoxLayout(scrollWidget/*, BoxLayout.Y_AXIS*/));
  ((QVBoxLayout*)centralWidgetLayout)->setSpacing(1);
  ((QVBoxLayout*)centralWidgetLayout)->setMargin(0);
  QWidget* panel1 = new QWidget();
  panel1->setLayout(new QHBoxLayout());
  panel1->layout()->addWidget(xoverTurnoutNameLabel);
  centralWidgetLayout->addWidget(panel1);
  QFrame* line = new QFrame();
  line->setObjectName(QString::fromUtf8("line"));
  line->setGeometry(QRect(10, 30, 571, 16));
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  centralWidgetLayout->addWidget(line);
  QWidget* panel2 = new QWidget();
  panel2->setLayout(new QHBoxLayout());
  QLabel* shTitle = new QLabel(tr("Signal Heads"));
  panel2->layout()->addWidget(shTitle);
  panel2->layout()->addWidget(new QLabel("   "));
  panel2->layout()->addWidget(getSavedXoverSignalHeads = new QPushButton(tr("Get Saved")));
//  getSavedXoverSignalHeads->layout()->addWidgetActionListener(new ActionListener() {
//            /*public*/ void actionPerformed(ActionEvent e) {
//                xoverTurnoutSignalsGetSaved(e);
//            }
//        });
  connect(getSavedXoverSignalHeads, SIGNAL(clicked()), this, SLOT(xoverTurnoutSignalsGetSaved()));
  getSavedXoverSignalHeads->setToolTip( "Click to retrieve signal heads previously stored.");
  centralWidgetLayout->addWidget(panel2);
    QWidget* panel21 = new QWidget();
    panel21->setLayout(new QHBoxLayout());
    QLabel* a1Label = new QLabel(tr("A Corner - Continuing")+" : ");
    panel21->layout()->addWidget(a1Label);
    panel21->layout()->addWidget(a1Field);
    centralWidgetLayout->addWidget(panel21);
    a1Field->setToolTip(tr("Enter name (system or user) of signal head located here."));
    QWidget* panel22 = new QWidget();
    panel22->setLayout(new QHBoxLayout());
    panel22->layout()->addWidget(new QLabel("   "));
    panel22->layout()->addWidget(setA1Head);
    setA1Head->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
    panel22->layout()->addWidget(new QLabel("  "));
    panel22->layout()->addWidget(setupA1Logic);
    setupA1Logic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
    centralWidgetLayout->addWidget(panel22);
    if (!(xoverType == LayoutTurnout::LH_XOVER))
    {
        QWidget* panel23 = new QWidget();
        panel23->setLayout(new QHBoxLayout());
        QLabel* a2Label = new QLabel(tr("A Corner - Diverging")+" : ");
        panel23->layout()->addWidget(a2Label);
        panel23->layout()->addWidget(a2Field);
        centralWidgetLayout->addWidget(panel23);
        a2Field->setToolTip(tr("Enter name (system or user) of signal head located here."));
        QWidget* panel24 = new QWidget();
        panel24->setLayout(new QHBoxLayout());
        panel24->layout()->addWidget(new QLabel("   "));
        panel24->layout()->addWidget(setA2Head);
        setA2Head->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel24->layout()->addWidget(new QLabel("  "));
        panel24->layout()->addWidget(setupA2Logic);
        setupA2Logic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
        centralWidgetLayout->addWidget(panel24);
    }
    QWidget* panel31 = new QWidget();
    panel31->setLayout(new QHBoxLayout());
    QLabel* b1Label = new QLabel(tr("B Corner - Continuing")+" : ");
    panel31->layout()->addWidget(b1Label);
    panel31->layout()->addWidget(b1Field);
    centralWidgetLayout->addWidget(panel31);
    b1Field->setToolTip(tr("Enter name (system or user) of signal head located here."));
    QWidget* panel32 = new QWidget();
    panel32->setLayout(new QHBoxLayout());
    panel32->layout()->addWidget(new QLabel("   "));
    panel32->layout()->addWidget(setB1Head);
    setB1Head->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
    panel32->layout()->addWidget(new QLabel("  "));
    panel32->layout()->addWidget(setupB1Logic);
    setupB1Logic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
    centralWidgetLayout->addWidget(panel32);
    if (!(xoverType == LayoutTurnout::RH_XOVER)) {
        QWidget* panel33 = new QWidget();
        panel33->setLayout(new QHBoxLayout());
        QLabel* b2Label = new QLabel(tr("B Corner - Diverging")+" : ");
        panel33->layout()->addWidget(b2Label);
        panel33->layout()->addWidget(b2Field);
        centralWidgetLayout->addWidget(panel33);
        b2Field->setToolTip(tr("Enter name (system or user) of signal head located here."));
        QWidget* panel34 = new QWidget();
        panel34->setLayout(new QHBoxLayout());
        panel34->layout()->addWidget(new QLabel("   "));
        panel34->layout()->addWidget(setB2Head);
        setB2Head->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel34->layout()->addWidget(new QLabel("  "));
        panel34->layout()->addWidget(setupB2Logic);
        setupB2Logic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
        centralWidgetLayout->addWidget(panel34);
    }
    QWidget* panel41 = new QWidget();
    panel41->setLayout(new QHBoxLayout());
    QLabel* c1Label = new QLabel(tr("C Corner - Continuing")+" : ");
    panel41->layout()->addWidget(c1Label);
    panel41->layout()->addWidget(c1Field);
    centralWidgetLayout->addWidget(panel41);
    c1Field->setToolTip(tr("Enter name (system or user) of signal head located here."));
    QWidget* panel42 = new QWidget();
    panel42->setLayout(new QHBoxLayout());
    panel42->layout()->addWidget(new QLabel("   "));
    panel42->layout()->addWidget(setC1Head);
    setC1Head->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
    panel42->layout()->addWidget(new QLabel("  "));
    panel42->layout()->addWidget(setupC1Logic);
    setupC1Logic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
    centralWidgetLayout->addWidget(panel42);
    if (!(xoverType == LayoutTurnout::LH_XOVER)) {
        QWidget* panel43 = new QWidget();
        panel43->setLayout(new QHBoxLayout());
        QLabel* c2Label = new QLabel(tr("C Corner - Diverging")+" : ");
        panel43->layout()->addWidget(c2Label);
        panel43->layout()->addWidget(c2Field);
        centralWidgetLayout->addWidget(panel43);
        c2Field->setToolTip(tr("Enter name (system or user) of signal head located here."));
        QWidget* panel44 = new QWidget();
        panel44->setLayout(new QHBoxLayout());
        panel44->layout()->addWidget(new QLabel("   "));
        panel44->layout()->addWidget(setC2Head);
        setC2Head->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel44->layout()->addWidget(new QLabel("  "));
        panel44->layout()->addWidget(setupC2Logic);
        setupC2Logic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
        centralWidgetLayout->addWidget(panel44);
    }
    QWidget* panel51 = new QWidget();
    panel51->setLayout(new QHBoxLayout());
    QLabel* d1Label = new QLabel(tr("D Corner - Continuing")+" : ");
    panel51->layout()->addWidget(d1Label);
    panel51->layout()->addWidget(d1Field);
    centralWidgetLayout->addWidget(panel51);
    d1Field->setToolTip(tr("Enter name (system or user) of signal head located here."));
    QWidget* panel52 = new QWidget();
    panel52->setLayout(new QHBoxLayout());
    panel52->layout()->addWidget(new QLabel("   "));
    panel52->layout()->addWidget(setD1Head);
    setD1Head->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
    panel52->layout()->addWidget(new QLabel("  "));
    panel52->layout()->addWidget(setupD1Logic);
    setupD1Logic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
    centralWidgetLayout->addWidget(panel52);
    if (!(xoverType == LayoutTurnout::RH_XOVER))
    {
        QWidget* panel53 = new QWidget();
        panel53->setLayout(new QHBoxLayout());
        QLabel* d2Label = new QLabel(tr("D Corner - Diverging")+" : ");
        panel53->layout()->addWidget(d2Label);
        panel53->layout()->addWidget(d2Field);
        centralWidgetLayout->addWidget(panel53);
        d2Field->setToolTip(tr("Enter name (system or user) of signal head located here."));
        QWidget* panel54 = new QWidget();
        panel54->setLayout(new QHBoxLayout());
        panel54->layout()->addWidget(new QLabel("   "));
        panel54->layout()->addWidget(setD2Head);
        setD2Head->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel54->layout()->addWidget(new QLabel("  "));
        panel54->layout()->addWidget(setupD2Logic);
        setupD2Logic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
        centralWidgetLayout->addWidget(panel54);
    }
    QFrame* line2 = new QFrame();
    line2->setObjectName(QString::fromUtf8("line"));
    line2->setGeometry(QRect(10, 30, 571, 16));
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);
    centralWidgetLayout->addWidget(line);
    QWidget* panel6 = new QWidget();
    panel6->setLayout(new QHBoxLayout());
    panel6->layout()->addWidget(changeXoverSignalIcon = new QPushButton(tr("Change Signal Head Icon")));
//    changeXoverSignalIcon->layout()->addWidgetActionListener(new ActionListener() {
//            /*public*/ void actionPerformed(ActionEvent e) {
//                signalFrame->setVisible(true);
//            }
//        });
    changeXoverSignalIcon->setToolTip( tr("Click to change icons used to represent signal heads. (Use left-facing icons.)") );
    connect(changeXoverSignalIcon, SIGNAL(clicked()), this, SLOT(on_changeXoverSignalIcon_clicked()));
    panel6->layout()->addWidget(new QLabel("  "));
    panel6->layout()->addWidget(setXoverSignalsDone = new QPushButton(tr("Done")));
//    setXoverSignalsDone->layout()->addWidgetActionListener(new ActionListener() {
//        /*public*/ void actionPerformed(ActionEvent e) {
//            setXoverSignalsDonePressed(e);
//        }
//    });
    connect(setXoverSignalsDone, SIGNAL(clicked()), this, SLOT(setXoverSignalsDonePressed()));
    setXoverSignalsDone->setToolTip( tr("Click Done to perform tasks requested above and dismiss this dialog.") );
    panel6->layout()->addWidget(setXoverSignalsCancel = new QPushButton(tr("Cancel")));
//    setXoverSignalsCancel->layout()->addWidgetActionListener(new ActionListener() {
//        /*public*/ void actionPerformed(ActionEvent e) {
//            setXoverSignalsCancelPressed(e);
//        }
//    });
    connect(setXoverSignalsCancel, SIGNAL(clicked()), this, SLOT(setXoverSignalsCancelPressed()));
    setXoverSignalsCancel->setToolTip( tr("Click Cancel to dismiss this dialog without making changes.") );
    centralWidgetLayout->addWidget(panel6);
//    setSignalsAtXoverFrame->layout()->addWidgetWindowListener(new java.awt.event.WindowAdapter() {
//        /*public*/ void windowClosing(java.awt.event.WindowEvent e) {
//            setXoverSignalsCancelPressed(nullptr);
//        }
//    });
    if (xoverFromMenu) xoverTurnoutSignalsGetSaved(nullptr);
}
    //setSignalsAtXoverFrame.pack();
    setSignalsAtXoverFrame->setVisible(true);
    setSignalsAtXoverOpen = true;
}
void LayoutEditorTools::on_changeXoverSignalIcon_clicked() // SLOT
{
  signalFrame->setVisible(true);
}

/*private*/ void LayoutEditorTools::xoverTurnoutSignalsGetSaved (ActionEvent* /*a*/)  // SLOT[]
{
 a1Field->setText(layoutTurnout->getSignalA1Name());
 a2Field->setText(layoutTurnout->getSignalA2Name());
 b1Field->setText(layoutTurnout->getSignalB1Name());
 b2Field->setText(layoutTurnout->getSignalB2Name());
 c1Field->setText(layoutTurnout->getSignalC1Name());
 c2Field->setText(layoutTurnout->getSignalC2Name());
 d1Field->setText(layoutTurnout->getSignalD1Name());
 d2Field->setText(layoutTurnout->getSignalD2Name());
}

/*private*/ void LayoutEditorTools::setXoverSignalsCancelPressed (ActionEvent* /*a*/) // SLOT[]
{
    setSignalsAtXoverOpen = false;
    xoverFromMenu = false;
    setSignalsAtXoverFrame->setVisible(false);
}
/*private*/ void LayoutEditorTools::setXoverSignalsDonePressed (ActionEvent* /*a*/)
{
 if ( !getXoverSignalHeadInformation() ) return;
 // place signal icons if requested, and assign signal heads to this turnout
 if (setA1Head->isChecked())
 {
  if (isHeadOnPanel(a1Head) &&
            (a1Head!=getHeadFromName(layoutTurnout->getSignalA1Name())))
  {
//            JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{a1Field->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(setSignalsAtXoverFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\nbecause it is already on the panel.").arg(a1Field->text().trimmed()));
   return;
  }
   else if ( (!layoutTurnoutHorizontal) && (!layoutTurnoutVertical) )
  {
//            JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                tr("InfoMessage2"),"",JOptionPane.INFORMATION_MESSAGE);
   QMessageBox::information(0, tr("Information"), tr("Sorry, cannot place signal heads at turnouts\n                                                   that are not oriented vertical or horizontal."));

   if (a1Head!=getHeadFromName(layoutTurnout->getSignalA1Name()))
   {
    removeSignalHeadFromPanel(layoutTurnout->getSignalA1Name());
    removeAssignment(a1Head);
    layoutTurnout->setSignalA1Name(a1Field->text().trimmed());
   }
  }
  else
  {
   removeSignalHeadFromPanel(layoutTurnout->getSignalA1Name());
   placeA1();
   removeAssignment(a1Head);
   layoutTurnout->setSignalA1Name(a1Field->text().trimmed());
   needRedraw = true;
  }
 }
 else
 {
  int assigned = isHeadAssignedHere(a1Head,layoutTurnout);
  if (assigned == NONE)
  {
   if ( isHeadOnPanel(a1Head) &&
                                isHeadAssignedAnywhere(a1Head) )
   {
//                JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{a1Field->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
    QMessageBox::critical(setSignalsAtXoverFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\n                                                                              because it is already on the panel at a different place.").arg(a1Field->text().trimmed()));
    return;
   }
   else
   {
    removeSignalHeadFromPanel(layoutTurnout->getSignalA1Name());
    removeAssignment(a1Head);
    layoutTurnout->setSignalA1Name(a1Field->text().trimmed());
   }
  }
  else if (assigned!=A1)
  {
// need to figure out what to do in this case.
  }
 }
 if ( (a2Head!=nullptr) && setA2Head->isChecked() )
 {
        if (isHeadOnPanel(a2Head) &&
            (a2Head!=getHeadFromName(layoutTurnout->getSignalA2Name()))) {
//            JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{a2Field->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
            QMessageBox::critical(setSignalsAtXoverFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\nbecause it is already on the panel.").arg(a2Field->text().trimmed()));
            return;
        }
        else if ( (!layoutTurnoutHorizontal) && (!layoutTurnoutVertical) ) {
//            JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                tr("InfoMessage2"),"",JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(0, tr("Information"), tr("Sorry, cannot place signal heads at turnouts\n                                                   that are not oriented vertical or horizontal."));

            if (a2Head!=getHeadFromName(layoutTurnout->getSignalA2Name())) {
                removeSignalHeadFromPanel(layoutTurnout->getSignalA2Name());
                removeAssignment(a2Head);
                layoutTurnout->setSignalA2Name(a2Field->text().trimmed());
            }
        }
        else {
            removeSignalHeadFromPanel(layoutTurnout->getSignalA2Name());
            placeA2();
            removeAssignment(a2Head);
            layoutTurnout->setSignalA2Name(a2Field->text().trimmed());
            needRedraw = true;
        }
 }
 else if (a2Head!=nullptr)
 {
        int assigned = isHeadAssignedHere(a2Head,layoutTurnout);
        if (assigned == NONE) {
            if ( isHeadOnPanel(a2Head) &&
                                isHeadAssignedAnywhere(a2Head) ) {
//                JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{a2Field->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
                QMessageBox::critical(setSignalsAtXoverFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\n                                                                              because it is already on the panel at a different place.").arg(a2Field->text().trimmed()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutTurnout->getSignalA2Name());
                removeAssignment(a2Head);
                layoutTurnout->setSignalA2Name(a2Field->text().trimmed());
            }
        }
        else if (assigned!=A2) {
// need to figure out what to do in this case.
        }
 }
 else if (a2Head==nullptr)
 {
  removeSignalHeadFromPanel(layoutTurnout->getSignalA2Name());
        layoutTurnout->setSignalA2Name("");
 }
 if (setB1Head->isChecked())
 {
  if (isHeadOnPanel(b1Head) &&
            (b1Head!=getHeadFromName(layoutTurnout->getSignalB1Name()))) {
//            JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{b1Field->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
            QMessageBox::critical(setSignalsAtXoverFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\nbecause it is already on the panel.").arg(b1Field->text().trimmed()));
            return;
        }
        else if ( (!layoutTurnoutHorizontal) && (!layoutTurnoutVertical) ) {
//            JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                tr("InfoMessage2"),"",JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(0, tr("Information"), tr("Sorry, cannot place signal heads at turnouts\nthat are not oriented vertical or horizontal."));

            if (b1Head!=getHeadFromName(layoutTurnout->getSignalB1Name())) {
                removeSignalHeadFromPanel(layoutTurnout->getSignalB1Name());
                removeAssignment(b1Head);
                layoutTurnout->setSignalB1Name(b1Field->text().trimmed());
            }
        }
        else {
            removeSignalHeadFromPanel(layoutTurnout->getSignalB1Name());
            placeB1();
            removeAssignment(b1Head);
            layoutTurnout->setSignalB1Name(b1Field->text().trimmed());
            needRedraw = true;
        }
    }
    else {
        int assigned = isHeadAssignedHere(b1Head,layoutTurnout);
        if (assigned == NONE) {
            if ( isHeadOnPanel(b1Head) &&
                                isHeadAssignedAnywhere(b1Head) ) {
//                JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{b1Field->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
                QMessageBox::critical(setSignalsAtXoverFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\n                                                                              because it is already on the panel at a different place.").arg(b1Field->text()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutTurnout->getSignalB1Name());
                removeAssignment(b1Head);
                layoutTurnout->setSignalB1Name(b1Field->text().trimmed());
            }
        }
        else if (assigned!=B1) {
// need to figure out what to do in this case.
        }
    }
    if ( (b2Head!=nullptr) && setB2Head->isChecked() ) {
        if (isHeadOnPanel(b2Head) &&
            (b2Head!=getHeadFromName(layoutTurnout->getSignalB2Name()))) {
//            JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{b2Field->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
            QMessageBox::critical(setSignalsAtXoverFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\nbecause it is already on the panel.").arg(b2Field->text().trimmed()));
            return;
        }
        else if ( (!layoutTurnoutHorizontal) && (!layoutTurnoutVertical) ) {
//            JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                tr("InfoMessage2"),"",JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(0, tr("Information"), tr("Sorry, cannot place signal heads at turnouts\n                                                   that are not oriented vertical or horizontal."));

            if (b2Head!=getHeadFromName(layoutTurnout->getSignalB2Name())) {
                removeSignalHeadFromPanel(layoutTurnout->getSignalB2Name());
                removeAssignment(b2Head);
                layoutTurnout->setSignalB2Name(b2Field->text().trimmed());
            }
        }
        else {
            removeSignalHeadFromPanel(layoutTurnout->getSignalB2Name());
            placeB2();
            removeAssignment(b2Head);
            layoutTurnout->setSignalB2Name(b2Field->text().trimmed());
            needRedraw = true;
        }
    }
    else if (b2Head!=nullptr) {
        int assigned = isHeadAssignedHere(b2Head,layoutTurnout);
        if (assigned == NONE) {
            if ( isHeadOnPanel(b2Head) &&
                                isHeadAssignedAnywhere(b2Head) ) {
//                JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{b2Field->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
                QMessageBox::critical(setSignalsAtXoverFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\n                                                                              because it is already on the panel at a different place.").arg(b2Field->text()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutTurnout->getSignalB2Name());
                removeAssignment(b2Head);
                layoutTurnout->setSignalB2Name(b2Field->text().trimmed());
            }
        }
        else if (assigned!=B2) {
// need to figure out what to do in this case.
        }
    }
    else if (b2Head==nullptr) {
        removeSignalHeadFromPanel(layoutTurnout->getSignalB2Name());
        layoutTurnout->setSignalB2Name("");
    }
    if (setC1Head->isChecked()) {
        if (isHeadOnPanel(c1Head) &&
            (c1Head!=getHeadFromName(layoutTurnout->getSignalC1Name()))) {
//            JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{c1Field->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
            QMessageBox::critical(setSignalsAtXoverFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\nbecause it is already on the panel.").arg(c1Field->text().trimmed()));
            return;
        }
        else if ( (!layoutTurnoutHorizontal) && (!layoutTurnoutVertical) ) {
//            JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                tr("InfoMessage2"),"",JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(0, tr("Information"), tr("Sorry, cannot place signal heads at turnouts\n                                                   that are not oriented vertical or horizontal."));

            if (c1Head!=getHeadFromName(layoutTurnout->getSignalC1Name())) {
                removeSignalHeadFromPanel(layoutTurnout->getSignalC1Name());
                removeAssignment(c1Head);
                layoutTurnout->setSignalC1Name(c1Field->text().trimmed());
            }
        }
        else {
            removeSignalHeadFromPanel(layoutTurnout->getSignalC1Name());
            placeC1();
            removeAssignment(c1Head);
            layoutTurnout->setSignalC1Name(c1Field->text().trimmed());
            needRedraw = true;
        }
    }
    else {
        int assigned = isHeadAssignedHere(c1Head,layoutTurnout);
        if (assigned == NONE) {
            if ( isHeadOnPanel(c1Head) &&
                                isHeadAssignedAnywhere(c1Head) ) {
//                JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{c1Field->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
                QMessageBox::critical(setSignalsAtXoverFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\n                                                                              because it is already on the panel at a different place.").arg(c1Field->text()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutTurnout->getSignalC1Name());
                removeAssignment(c1Head);
                layoutTurnout->setSignalC1Name(c1Field->text().trimmed());
            }
        }
        else if (assigned!=C1) {
// need to figure out what to do in this case.
        }
    }
    if ( (c2Head!=nullptr) && setC2Head->isChecked() ) {
        if (isHeadOnPanel(c2Head) &&
            (c2Head!=getHeadFromName(layoutTurnout->getSignalC2Name()))) {
//            JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{c2Field->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
            QMessageBox::critical(setSignalsAtXoverFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\nbecause it is already on the panel.").arg(c2Field->text().trimmed()));
            return;
        }
        else if ( (!layoutTurnoutHorizontal) && (!layoutTurnoutVertical) ) {
//            JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                tr("InfoMessage2"),"",JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(0, tr("Information"), tr("Sorry, cannot place signal heads at turnouts\n                                                   that are not oriented vertical or horizontal."));

            if (c2Head!=getHeadFromName(layoutTurnout->getSignalC2Name())) {
                removeSignalHeadFromPanel(layoutTurnout->getSignalC2Name());
                removeAssignment(c2Head);
                layoutTurnout->setSignalC2Name(c2Field->text().trimmed());
            }
        }
        else {
            removeSignalHeadFromPanel(layoutTurnout->getSignalC2Name());
            placeC2();
            removeAssignment(c2Head);
            layoutTurnout->setSignalC2Name(c2Field->text().trimmed());
            needRedraw = true;
        }
    }
    else if (c2Head!=nullptr) {
        int assigned = isHeadAssignedHere(c2Head,layoutTurnout);
        if (assigned == NONE) {
            if ( isHeadOnPanel(c2Head) &&
                                isHeadAssignedAnywhere(c2Head) ) {
//                JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{c2Field->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
                QMessageBox::critical(setSignalsAtXoverFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\n                                                                              because it is already on the panel at a different place.").arg(c2Field->text()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutTurnout->getSignalC2Name());
                removeAssignment(c2Head);
                layoutTurnout->setSignalC2Name(c2Field->text().trimmed());
            }
        }
        else if (assigned!=C2) {
// need to figure out what to do in this case.
        }
    }
    else if (c2Head==nullptr) {
        removeSignalHeadFromPanel(layoutTurnout->getSignalC2Name());
        layoutTurnout->setSignalC2Name("");
    }
    if (setD1Head->isChecked()) {
        if (isHeadOnPanel(d1Head) &&
            (d1Head!=getHeadFromName(layoutTurnout->getSignalD1Name()))) {
//            JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{d1Field->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
            QMessageBox::critical(setSignalsAtXoverFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\nbecause it is already on the panel.").arg(d1Field->text().trimmed()));
            return;
        }
        else if ( (!layoutTurnoutHorizontal) && (!layoutTurnoutVertical) ) {
//            JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                tr("InfoMessage2"),"",JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(0, tr("Information"), tr("Sorry, cannot place signal heads at turnouts\n                                                   that are not oriented vertical or horizontal."));

            if (d1Head!=getHeadFromName(layoutTurnout->getSignalD1Name())) {
                removeSignalHeadFromPanel(layoutTurnout->getSignalD1Name());
                removeAssignment(d1Head);
                layoutTurnout->setSignalD1Name(d1Field->text().trimmed());
            }
        }
        else {
            removeSignalHeadFromPanel(layoutTurnout->getSignalD1Name());
            placeD1();
            removeAssignment(d1Head);
            layoutTurnout->setSignalD1Name(d1Field->text().trimmed());
            needRedraw = true;
        }
    }
    else {
        int assigned = isHeadAssignedHere(d1Head,layoutTurnout);
        if (assigned == NONE) {
            if ( isHeadOnPanel(d1Head) &&
                                isHeadAssignedAnywhere(d1Head) ) {
//                JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{d1Field->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
                QMessageBox::critical(setSignalsAtXoverFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\n                                                                              because it is already on the panel at a different place.").arg(d1Field->text()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutTurnout->getSignalD1Name());
                removeAssignment(d1Head);
                layoutTurnout->setSignalD1Name(d1Field->text().trimmed());
            }
        }
        else if (assigned!=D1) {
// need to figure out what to do in this case.
        }
    }
    if ( (d2Head!=nullptr) && setD2Head->isChecked() ) {
        if (isHeadOnPanel(d2Head) &&
            (d2Head!=getHeadFromName(layoutTurnout->getSignalD2Name()))) {
//            JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{d2Field->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
            QMessageBox::critical(setSignalsAtXoverFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\nbecause it is already on the panel.").arg(d2Field->text().trimmed()));
            return;
        }
        else if ( (!layoutTurnoutHorizontal) && (!layoutTurnoutVertical) ) {
//            JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                tr("InfoMessage2"),"",JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(0, tr("Information"), tr("Sorry, cannot place signal heads at turnouts\n                                                   that are not oriented vertical or horizontal."));

            if (d2Head!=getHeadFromName(layoutTurnout->getSignalD2Name())) {
                removeSignalHeadFromPanel(layoutTurnout->getSignalD2Name());
                removeAssignment(d2Head);
                layoutTurnout->setSignalD2Name(d2Field->text().trimmed());
            }
        }
        else {
            removeSignalHeadFromPanel(layoutTurnout->getSignalD2Name());
            placeD2();
            removeAssignment(d2Head);
            layoutTurnout->setSignalD2Name(d2Field->text().trimmed());
            needRedraw = true;
        }
    }
    else if (d2Head!=nullptr) {
        int assigned = isHeadAssignedHere(d2Head,layoutTurnout);
        if (assigned == NONE) {
            if ( isHeadOnPanel(d2Head) &&
                                isHeadAssignedAnywhere(d2Head) ) {
//                JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{d2Field->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
                QMessageBox::critical(setSignalsAtXoverFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\n                                                                              because it is already on the panel at a different place.").arg(d2Field->text()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutTurnout->getSignalD2Name());
                removeAssignment(d2Head);
                layoutTurnout->setSignalD2Name(d2Field->text().trimmed());
            }
        }
        else if (assigned!=D2) {
// need to figure out what to do in this case.
        }
    }
    else if (d2Head==nullptr) {
        removeSignalHeadFromPanel(layoutTurnout->getSignalD2Name());
        layoutTurnout->setSignalD2Name("");
    }
    // setup logic if requested
    if (setupA1Logic->isChecked() || setupA2Logic->isChecked()) {
        if (xoverType == LayoutTurnout::LH_XOVER) {
            setLogicXoverContinuing(a1Head,(TrackSegment*)layoutTurnout->getConnectB());
        }
        else {
            setLogicXover(a1Head,(TrackSegment*)layoutTurnout->getConnectB(),a2Head,
                    (TrackSegment*)layoutTurnout->getConnectC(),setupA1Logic->isChecked(),
                            setupA2Logic->isChecked());
        }
    }
    if (setupB1Logic->isChecked() || setupB2Logic->isChecked()) {
        if (xoverType == LayoutTurnout::RH_XOVER) {
            setLogicXoverContinuing(b1Head,(TrackSegment*)layoutTurnout->getConnectA());
        }
        else {
            setLogicXover(b1Head,(TrackSegment*)layoutTurnout->getConnectA(),b2Head,
                    (TrackSegment*)layoutTurnout->getConnectD(),setupB1Logic->isChecked(),
                            setupB2Logic->isChecked());
        }
    }
    if (setupC1Logic->isChecked() || setupC2Logic->isChecked()) {
        if (xoverType == LayoutTurnout::LH_XOVER) {
            setLogicXoverContinuing(c1Head,(TrackSegment*)layoutTurnout->getConnectD());
        }
        else {
            setLogicXover(c1Head,(TrackSegment*)layoutTurnout->getConnectD(),c2Head,
                    (TrackSegment*)layoutTurnout->getConnectA(),setupC1Logic->isChecked(),
                            setupC2Logic->isChecked());
        }
    }
    if (setupD1Logic->isChecked() || setupD2Logic->isChecked()) {
        if (xoverType == LayoutTurnout::RH_XOVER) {
            setLogicXoverContinuing(d1Head,(TrackSegment*)layoutTurnout->getConnectC());
        }
        else {
            setLogicXover(d1Head,(TrackSegment*)layoutTurnout->getConnectC(),d2Head,
                    (TrackSegment*)layoutTurnout->getConnectB(),setupD1Logic->isChecked(),
                            setupD2Logic->isChecked());
        }
    }
    // make sure this layout turnout is not linked to another
    layoutTurnout->setLinkType(LayoutTurnout::NO_LINK);
    layoutTurnout->setLinkedTurnoutName("");
    // finish up
    setSignalsAtXoverOpen = false;
    xoverFromMenu = false;
    setSignalsAtXoverFrame->setVisible(false);
    if (needRedraw) {
        layoutEditor->redrawPanel();
        needRedraw = false;
        layoutEditor->setDirty();
    }
}

/*private*/ bool LayoutEditorTools::getXoverSignalHeadInformation() {
    a1Head = getSignalHeadFromEntry(a1Field,true,setSignalsAtXoverFrame);
    if (a1Head==nullptr) return false;
    if (!(xoverType == LayoutTurnout::LH_XOVER ))
        a2Head = getSignalHeadFromEntry(a2Field,false,setSignalsAtXoverFrame);
    else
        a2Head = NULL;
    b1Head = getSignalHeadFromEntry(b1Field,true,setSignalsAtXoverFrame);
    if (b1Head==nullptr) return false;
    if (!(xoverType == LayoutTurnout::RH_XOVER ))
        b2Head = getSignalHeadFromEntry(b2Field,false,setSignalsAtXoverFrame);
    else
        b2Head = NULL;
    c1Head = getSignalHeadFromEntry(c1Field,true,setSignalsAtXoverFrame);
    if (c1Head==nullptr) return false;
    if (!(xoverType == LayoutTurnout::LH_XOVER ))
        c2Head = getSignalHeadFromEntry(c2Field,false,setSignalsAtXoverFrame);
    else
        c2Head = NULL;
    d1Head = getSignalHeadFromEntry(d1Field,true,setSignalsAtXoverFrame);
    if (d1Head==nullptr) return false;
    if (!(xoverType == LayoutTurnout::RH_XOVER ))
        d2Head = getSignalHeadFromEntry(d2Field,false,setSignalsAtXoverFrame);
    else
        d2Head = NULL;
    return true;
}
/*private*/ void LayoutEditorTools::placeA1() {
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    if( layoutTurnoutHorizontal && layoutTurnoutThroatLeft ) {
        setSignalHeadOnPanel(2,a1Field->text().trimmed(),
            (int)(layoutTurnout->getCoordsA().x()-testIcon->getIconWidth()),
            (int)(layoutTurnout->getCoordsA().y()+4) );
    }
    else if( layoutTurnoutHorizontal && (!layoutTurnoutThroatLeft) ) {
        setSignalHeadOnPanel(0,a1Field->text().trimmed(),
            (int)(layoutTurnout->getCoordsA().x()),
            (int)(layoutTurnout->getCoordsA().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnoutVertical && layoutTurnoutThroatUp ) {
        setSignalHeadOnPanel(1,a1Field->text().trimmed(),
            (int)(layoutTurnout->getCoordsA().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout->getCoordsA().y()-testIcon->getIconHeight()) );
    }
    else if( layoutTurnoutVertical && (!layoutTurnoutThroatUp) ) {
        setSignalHeadOnPanel(3,a1Field->text().trimmed(),
            (int)(layoutTurnout->getCoordsA().x()+4),
            (int)(layoutTurnout->getCoordsA().y()) );
    }
}
/*private*/ void LayoutEditorTools::placeA2() {
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    if( layoutTurnoutHorizontal && layoutTurnoutThroatLeft ) {
        setSignalHeadOnPanel(2,a2Field->text().trimmed(),
            (int)(layoutTurnout->getCoordsA().x()-4-(2*testIcon->getIconWidth())),
            (int)(layoutTurnout->getCoordsA().y()+4) );
    }
    else if( layoutTurnoutHorizontal && (!layoutTurnoutThroatLeft) ) {
        setSignalHeadOnPanel(0,a2Field->text().trimmed(),
            (int)(layoutTurnout->getCoordsA().x()+4+testIcon->getIconWidth()),
            (int)(layoutTurnout->getCoordsA().y()-4-testIcon->getIconHeight()));
    }
    else if( layoutTurnoutVertical && layoutTurnoutThroatUp ) {
        setSignalHeadOnPanel(1,a2Field->text().trimmed(),
            (int)(layoutTurnout->getCoordsA().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout->getCoordsA().y()-4-(2*testIcon->getIconHeight())));
    }
    else if( layoutTurnoutVertical && (!layoutTurnoutThroatUp) ) {
        setSignalHeadOnPanel(3,a2Field->text().trimmed(),
            (int)(layoutTurnout->getCoordsA().x()+4),
            (int)(layoutTurnout->getCoordsA().y()+4+testIcon->getIconHeight()));
    }
}
/*private*/ void LayoutEditorTools::placeB1() {
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    if( layoutTurnoutHorizontal && layoutTurnoutThroatLeft ) {
        setSignalHeadOnPanel(0,b1Field->text().trimmed(),
            (int)(layoutTurnout->getCoordsB().x()),
            (int)(layoutTurnout->getCoordsB().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnoutHorizontal && (!layoutTurnoutThroatLeft) ) {
        setSignalHeadOnPanel(2,b1Field->text().trimmed(),
            (int)(layoutTurnout->getCoordsB().x()-testIcon->getIconWidth()),
            (int)(layoutTurnout->getCoordsB().y()+4) );
    }
    else if( layoutTurnoutVertical && layoutTurnoutThroatUp ) {
        setSignalHeadOnPanel(3,b1Field->text().trimmed(),
            (int)(layoutTurnout->getCoordsB().x()+4),
            (int)(layoutTurnout->getCoordsB().y()) );
    }
    else if( layoutTurnoutVertical && (!layoutTurnoutThroatUp) ) {
        setSignalHeadOnPanel(1,b1Field->text().trimmed(),
            (int)(layoutTurnout->getCoordsB().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout->getCoordsB().y()-testIcon->getIconHeight()) );
    }
}
/*private*/ void LayoutEditorTools::placeB2() {
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    if( layoutTurnoutHorizontal && layoutTurnoutThroatLeft ) {
        setSignalHeadOnPanel(0,b2Field->text().trimmed(),
            (int)(layoutTurnout->getCoordsB().x()+4+testIcon->getIconWidth()),
            (int)(layoutTurnout->getCoordsB().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnoutHorizontal && (!layoutTurnoutThroatLeft) ) {
        setSignalHeadOnPanel(2,b2Field->text().trimmed(),
            (int)(layoutTurnout->getCoordsB().x()-4-(2*testIcon->getIconWidth())),
            (int)(layoutTurnout->getCoordsB().y()+4) );
    }
    else if( layoutTurnoutVertical && layoutTurnoutThroatUp ) {
        setSignalHeadOnPanel(3,b2Field->text().trimmed(),
            (int)(layoutTurnout->getCoordsB().x()+4),
            (int)(layoutTurnout->getCoordsB().y()+4+testIcon->getIconHeight()) );
    }
    else if( layoutTurnoutVertical && (!layoutTurnoutThroatUp) ) {
        setSignalHeadOnPanel(1,b2Field->text().trimmed(),
            (int)(layoutTurnout->getCoordsB().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout->getCoordsB().y()-4-(2*testIcon->getIconHeight())) );
    }
}
/*private*/ void LayoutEditorTools::placeC1() {
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    if( layoutTurnoutHorizontal && layoutTurnoutThroatLeft ) {
        setSignalHeadOnPanel(0,c1Field->text().trimmed(),
            (int)(layoutTurnout->getCoordsC().x()),
            (int)(layoutTurnout->getCoordsC().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnoutHorizontal && (!layoutTurnoutThroatLeft) ) {
        setSignalHeadOnPanel(2,c1Field->text().trimmed(),
            (int)(layoutTurnout->getCoordsC().x()-testIcon->getIconWidth()),
            (int)(layoutTurnout->getCoordsC().y()+4) );
    }
    else if( layoutTurnoutVertical && layoutTurnoutThroatUp ) {
        setSignalHeadOnPanel(3,c1Field->text().trimmed(),
            (int)(layoutTurnout->getCoordsC().x()+4),
            (int)(layoutTurnout->getCoordsC().y()) );
    }
    else if( layoutTurnoutVertical && (!layoutTurnoutThroatUp) ) {
        setSignalHeadOnPanel(1,c1Field->text().trimmed(),
            (int)(layoutTurnout->getCoordsC().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout->getCoordsC().y()-testIcon->getIconHeight()) );
    }
}
/*private*/ void LayoutEditorTools::placeC2()
{
 if (testIcon == nullptr)
     testIcon = signalIconEditor->getIcon(0);
 if( layoutTurnoutHorizontal && layoutTurnoutThroatLeft ) {
     setSignalHeadOnPanel(0,c2Field->text().trimmed(),
         (int)(layoutTurnout->getCoordsC().x()+4+testIcon->getIconWidth()),
         (int)(layoutTurnout->getCoordsC().y()-4-testIcon->getIconHeight()) );
 }
 else if( layoutTurnoutHorizontal && (!layoutTurnoutThroatLeft) ) {
     setSignalHeadOnPanel(2,c2Field->text().trimmed(),
         (int)(layoutTurnout->getCoordsC().x()-4-(2*testIcon->getIconWidth())),
         (int)(layoutTurnout->getCoordsC().y()+4) );
 }
 else if( layoutTurnoutVertical && layoutTurnoutThroatUp ) {
     setSignalHeadOnPanel(3,c2Field->text().trimmed(),
         (int)(layoutTurnout->getCoordsC().x()+4),
         (int)(layoutTurnout->getCoordsC().y()+4+testIcon->getIconHeight()) );
 }
 else if( layoutTurnoutVertical && (!layoutTurnoutThroatUp) ) {
     setSignalHeadOnPanel(1,c2Field->text().trimmed(),
         (int)(layoutTurnout->getCoordsC().x()-4-testIcon->getIconWidth()),
         (int)(layoutTurnout->getCoordsC().y()-4-(2*testIcon->getIconHeight())) );
 }
}
/*private*/ void LayoutEditorTools::placeD1()
{
 if (testIcon == nullptr)
     testIcon = signalIconEditor->getIcon(0);
 if( layoutTurnoutHorizontal && layoutTurnoutThroatLeft ) {
     setSignalHeadOnPanel(2,d1Field->text().trimmed(),
         (int)(layoutTurnout->getCoordsD().x()-testIcon->getIconWidth()),
         (int)(layoutTurnout->getCoordsD().y()+4) );
 }
 else if( layoutTurnoutHorizontal && (!layoutTurnoutThroatLeft) ) {
     setSignalHeadOnPanel(0,d1Field->text().trimmed(),
         (int)(layoutTurnout->getCoordsD().x()),
         (int)(layoutTurnout->getCoordsD().y()-4-testIcon->getIconHeight()) );
 }
 else if( layoutTurnoutVertical && layoutTurnoutThroatUp ) {
     setSignalHeadOnPanel(1,d1Field->text().trimmed(),
         (int)(layoutTurnout->getCoordsD().x()-4-testIcon->getIconWidth()),
         (int)(layoutTurnout->getCoordsD().y()-testIcon->getIconHeight()) );
 }
 else if( layoutTurnoutVertical && (!layoutTurnoutThroatUp) ) {
     setSignalHeadOnPanel(3,d1Field->text().trimmed(),
         (int)(layoutTurnout->getCoordsD().x()+4),
         (int)(layoutTurnout->getCoordsD().y()) );
 }
}
/*private*/ void LayoutEditorTools::placeD2() {
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    if( layoutTurnoutHorizontal && layoutTurnoutThroatLeft ) {
        setSignalHeadOnPanel(2,d2Field->text().trimmed(),
            (int)(layoutTurnout->getCoordsD().x()-4-(2*testIcon->getIconWidth())),
            (int)(layoutTurnout->getCoordsD().y()+4) );
    }
    else if( layoutTurnoutHorizontal && (!layoutTurnoutThroatLeft) ) {
        setSignalHeadOnPanel(0,d2Field->text().trimmed(),
            (int)(layoutTurnout->getCoordsD().x()+4+testIcon->getIconWidth()),
            (int)(layoutTurnout->getCoordsD().y()-4-testIcon->getIconHeight()));
    }
    else if( layoutTurnoutVertical && layoutTurnoutThroatUp ) {
        setSignalHeadOnPanel(1,d2Field->text().trimmed(),
            (int)(layoutTurnout->getCoordsD().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout->getCoordsD().y()-4-(2*testIcon->getIconHeight())));
    }
    else if( layoutTurnoutVertical && (!layoutTurnoutThroatUp) ) {
        setSignalHeadOnPanel(3,d2Field->text().trimmed(),
            (int)(layoutTurnout->getCoordsD().x()+4),
            (int)(layoutTurnout->getCoordsD().y()+4+testIcon->getIconHeight()));
    }
}
//@SuppressWarnings("NULL")
/*private*/ void LayoutEditorTools::setLogicXover(SignalHead* head,TrackSegment* track,SignalHead* secondHead,TrackSegment* track2,
            bool setup1, bool setup2)
{
 if ( (track==nullptr) && setup1 )
 {
//        JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                tr("InfoMessage7"),"",JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsAtXoverFrame, tr("Information"), tr("Cannot set up logic because all connections\nhave not been defined around this item."));
     return;
 }
 Sensor* occupancy = NULL;
 SignalHead* nextHead = NULL;
 if ( (track!=nullptr) && setup1) {
     LayoutBlock* block = track->getLayoutBlock();
     if (block==nullptr) {
//            JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                tr("InfoMessage6"),"",JOptionPane.INFORMATION_MESSAGE);
         QMessageBox::information(setSignalsAtXoverFrame, tr("Information"), tr("Cannot set up logic because blocks have\nnot been defined around this item."));
         return;
     }
     occupancy = block->getOccupancySensor();
     if (occupancy==nullptr) {
//            JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                java.text.MessageFormat.format(tr("InfoMessage4"),
//                    new Object[]{block->getUserName()}),
//                        NULL,JOptionPane.INFORMATION_MESSAGE);
         QMessageBox::information(setSignalsAtXoverFrame, tr("Information"), tr("Cannot set up logic because block \"%1\"\ndoesn''t have an occupancy sensor.").arg(block->getUserName()));
         return;
     }
     nextHead = getNextSignalFromObject(track,
             layoutTurnout, head->getSystemName(), setSignalsAtXoverFrame);
     if ( (nextHead==nullptr) && (!reachedEndBumper()) ) {
//            JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                java.text.MessageFormat.format(tr("InfoMessage5"),
//                    new Object[]{block->getUserName()}),
//                        NULL,JOptionPane.INFORMATION_MESSAGE);
         QMessageBox::information(setSignalsAtXoverFrame, tr("Information"), tr("Cannot set up logic because the next signal (in or\nat the end of block \"%1\") apparently is not yet defined.").arg(block->getUserName()));
         return;
     }
     if (secondHead!=nullptr) {
         if (!initializeBlockBossLogic(head->getSystemName())) return;
         logic->setMode(BlockBossLogic::TRAILINGMAIN);
         logic->setTurnout(turnout->getSystemName());
         logic->setSensor1(occupancy->getSystemName());
         if (nextHead!=nullptr) {
             logic->setWatchedSignal1(nextHead->getSystemName(),false);
         }
         if (auxSignal!=nullptr) {
             logic->setWatchedSignal1Alt(auxSignal->getSystemName());
         }
         finalizeBlockBossLogic();
     }
 }
 if ( (secondHead!=nullptr) && !setup2 ) return;
 SignalHead* savedAuxSignal = auxSignal;
 if (track2==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                tr("InfoMessage7"),"",JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsAtXoverFrame, tr("Information"), tr("Cannot set up logic because all connections\nhave not been defined around this item."));
     return;
 }
 LayoutBlock* block2 = track2->getLayoutBlock();
 if (block2==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                tr("InfoMessage6"),"",JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsAtXoverFrame, tr("Information"), tr("Cannot set up logic because blocks have\nnot been defined around this item."));
     return;
 }
 Sensor* occupancy2 = block2->getOccupancySensor();
 if (occupancy2==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//            java.text.MessageFormat.format(tr("InfoMessage4"),
//                new Object[]{block2.getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsAtXoverFrame, tr("Information"), tr("Cannot set up logic because block \"%1\"\n doesn''t have an occupancy sensor.").arg(block2->getUserName()));
     return;
 }
 QString headName = head->getSystemName();
 if (secondHead!=nullptr) headName = secondHead->getSystemName();
 SignalHead* nextHead2 = getNextSignalFromObject(track2,
         layoutTurnout, headName, setSignalsAtXoverFrame);
 if ( (nextHead2==nullptr) && (!reachedEndBumper()) ) {
//        JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//            java.text.MessageFormat.format(tr("InfoMessage5"),
//                new Object[]{block2.getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsAtXoverFrame, tr("Information"), tr("Cannot set up logic because the next signal (in or\nat the end of block \"%1\") apparently is not yet defined.").arg(block2->getUserName()));
     return;
 }
 if ( (secondHead==nullptr) && (track!=nullptr) && setup1 )
 {
  if (!initializeBlockBossLogic(head->getSystemName())) return;
  logic->setMode(BlockBossLogic::FACING);
  logic->setTurnout(turnout->getSystemName());
  logic->setWatchedSensor1(occupancy->getSystemName());
  logic->setWatchedSensor2(occupancy2->getSystemName());
  if (nextHead!=nullptr) {
      logic->setWatchedSignal1(nextHead->getSystemName(),false);
  }
  if (savedAuxSignal!=nullptr) {
      logic->setWatchedSignal1Alt(savedAuxSignal->getSystemName());
  }
  if (nextHead2!=nullptr) {
      logic->setWatchedSignal2(nextHead2->getSystemName());
  }
  if (auxSignal!=nullptr) {
      logic->setWatchedSignal2Alt(auxSignal->getSystemName());
  }
  logic->setLimitSpeed2(true);
  finalizeBlockBossLogic();
 }
 else if ( (secondHead!=nullptr) && setup2)
 {
  if (!initializeBlockBossLogic(secondHead->getSystemName())) return;
  logic->setMode(BlockBossLogic::TRAILINGDIVERGING);
  logic->setTurnout(turnout->getSystemName());
  logic->setSensor1(occupancy2->getSystemName());
  if (nextHead2!=nullptr)
  {
   logic->setWatchedSignal1(nextHead2->getSystemName(),false);
  }
  if (auxSignal!=nullptr)
  {
   logic->setWatchedSignal1Alt(auxSignal->getSystemName());
  }
  logic->setLimitSpeed2(true);
  finalizeBlockBossLogic();
 }
}
/*private*/ void LayoutEditorTools::setLogicXoverContinuing(SignalHead* head,TrackSegment* track) {
    if (track==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                tr("InfoMessage7"),"",JOptionPane.INFORMATION_MESSAGE);
        QMessageBox::information(setSignalsAtXoverFrame, tr("Information"), tr("Cannot set up logic because all connections\nhave not been defined around this item."));
        return;
    }
    LayoutBlock* block = track->getLayoutBlock();
    if (block==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//                tr("InfoMessage6"),"",JOptionPane.INFORMATION_MESSAGE);
        QMessageBox::information(setSignalsAtXoverFrame, tr("Information"), tr("Cannot set up logic because blocks have\nnot been defined around this item."));
        return;
    }
    Sensor* occupancy = block->getOccupancySensor();
    if (occupancy==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//            java.text.MessageFormat.format(tr("InfoMessage4"),
//                new Object[]{block->getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
        QMessageBox::information(setSignalsAtXoverFrame, tr("Information"), tr("Cannot set up logic because block \"%1\"\ndoesn''t have an occupancy sensor.").arg(block->getUserName()));
        return;
    }
    SignalHead* nextHead = getNextSignalFromObject(track,
            layoutTurnout, head->getSystemName(), setSignalsAtXoverFrame);
    if ( (nextHead==nullptr) && (!reachedEndBumper()) ) {
//        JOptionPane.showMessageDialog(setSignalsAtXoverFrame,
//            java.text.MessageFormat.format(tr("InfoMessage5"),
//                new Object[]{block->getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
        QMessageBox::information(setSignalsAtXoverFrame, tr("Information"), tr("Cannot set up logic because the next signal (in or\nat the end of block \"%1\") apparently is not yet defined.").arg(block->getUserName()));
        return;
    }
    if (!initializeBlockBossLogic(head->getSystemName())) return;
    logic->setMode(BlockBossLogic::TRAILINGMAIN);
    logic->setTurnout(turnout->getSystemName());
    logic->setSensor1(occupancy->getSystemName());
    if (nextHead!=nullptr) {
        logic->setWatchedSignal1(nextHead->getSystemName(),false);
    }
    if (auxSignal!=nullptr) {
        logic->setWatchedSignal1Alt(auxSignal->getSystemName());
    }
    finalizeBlockBossLogic();
}

/**
 * Tool to set signals at a level crossing, including placing the
 *		signal icons and setup of Simple Signal Logic for each signal head
 * <P>
 * This tool assumes left facing signal head icons have been selected, and
 *		will rotate the signal head icons accordingly.
 * <P>
 * This tool will place icons on the right side of each track->
 * <P>
 * Both tracks do not need to be signalled. If one signal for a track,
 *		A-C or B-D, the other must also be present.
 * <P>
 * Some user adjustment of turnout positions may be needed.
 */


// display dialog for Set Signals at Level Crossing tool
/*public*/ void LayoutEditorTools::setSignalsAtLevelXingFromMenu (LevelXing* xing, MultiIconEditor* theEditor,
        JFrame* theFrame )
{
    xingFromMenu = true;
    levelXing = xing;
    blockANameField->setText(levelXing->getBlockNameAC());
    blockCNameField->setText(levelXing->getBlockNameBD());
    setSignalsAtLevelXing(theEditor,theFrame);
    return;
}

/*public*/ void LayoutEditorTools::setSignalsAtLevelXing( MultiIconEditor* theEditor, JFrame* theFrame )
{
    signalIconEditor = theEditor;
    signalFrame = theFrame;
    if (setSignalsAtXingOpen) {
        setSignalsAtXingFrame->setVisible(true);
        return;
    }
    aField->setText("");
    bField->setText("");
    cField->setText("");
    dField->setText("");
    // Initialize if needed
    if (setSignalsAtXingFrame == nullptr)
    {
        setSignalsAtXingFrame = new JmriJFrameX( tr("SignalsAtLevelXing"), false, true );
        setSignalsAtXingFrame->addHelpMenu("package.jmri.jmrit.display.SetSignalsAtLevelXing", true);
        setSignalsAtXingFrame->setLocation(70,30);
//        Container theContentPane = setSignalsAtXingFrame.getContentPane();
//        theContentPane->setLayout(new BoxLayout(theContentPane, BoxLayout.Y_AXIS));
        QWidget* centralWidget = new QWidget;
        QVBoxLayout* centralWidgetLayout = new QVBoxLayout(centralWidget);
        setSignalsAtXingFrame->setCentralWidget(centralWidget);
        QWidget* panel11 = new QWidget();
        panel11->setLayout(new QHBoxLayout());
        if (xingFromMenu) {
            QLabel* blockANameLabel = new QLabel( tr("BlockAtA")+" "+
                        tr("Name")+" : "+levelXing->getBlockNameAC());

            panel11->layout()->addWidget(blockANameLabel);
        }
        else {
            QLabel* blockANameLabel = new QLabel( tr("BlockAtA")+" "+
                                                            tr("Name")+" : ");
            panel11->layout()->addWidget(blockANameLabel);
            panel11->layout()->addWidget(blockANameField);
            blockANameField->setToolTip(tr("Enter name of one block where signals are needed at block boundary."));
        }
        centralWidgetLayout->addWidget(panel11);
        QWidget* panel12 = new QWidget();
        panel12->setLayout(new QHBoxLayout());
        if (xingFromMenu) {
            QLabel* blockCNameLabel = new QLabel( tr("BlockAtC")+" "+
                        tr("Name")+" : "+levelXing->getBlockNameBD());

            panel12->layout()->addWidget(blockCNameLabel);
        }
        else {
            QLabel* blockCNameLabel = new QLabel( tr("BlockAtC")+" "+
                                                            tr("Name")+" : ");
            panel12->layout()->addWidget(blockCNameLabel);
            panel12->layout()->addWidget(blockCNameField);
            blockCNameField->setToolTip(tr("SignalsBlockNameHint"));
        }
        centralWidgetLayout->addWidget(panel12);
        centralWidgetLayout->addWidget(new JSeparator(JSeparator::HORIZONTAL));
        QWidget* panel2 = new QWidget();
        panel2->setLayout(new QHBoxLayout());
        QLabel* shTitle = new QLabel(tr("SignalHeads"));
        panel2->layout()->addWidget(shTitle);
        panel2->layout()->addWidget(new QLabel("   "));
        panel2->layout()->addWidget(getSavedXingSignalHeads = new QPushButton(tr("GetSaved")));
//        getSavedXingSignalHeads->layout()->addWidgetActionListener(new ActionListener() {
//                /*public*/ void actionPerformed(ActionEvent e) {
//                    xingSignalsGetSaved(e);
//                }
//            });
        connect(getSavedXoverSignalHeads, SIGNAL(clicked()), this, SLOT(xingSignalsGetSaved()));
        getSavedXingSignalHeads->setToolTip("Click to retrieve signal heads previously stored.");
        centralWidgetLayout->addWidget(panel2);
        QWidget* panel21 = new QWidget();
        panel21->setLayout(new QHBoxLayout());
        QLabel* aLabel = new QLabel(tr("ATrack")+" : ");
        panel21->layout()->addWidget(aLabel);
        panel21->layout()->addWidget(aField);
        centralWidgetLayout->addWidget(panel21);
        aField->setToolTip(tr("Enter name (system or user) of signal head located here."));
        QWidget* panel22 = new QWidget();
        panel22->setLayout(new QHBoxLayout());
        panel22->layout()->addWidget(new QLabel("   "));
        panel22->layout()->addWidget(setAHead);
        setAHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel22->layout()->addWidget(new QLabel("  "));
        panel22->layout()->addWidget(setupALogic);
        setupALogic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
        centralWidgetLayout->addWidget(panel22);
        QWidget* panel31 = new QWidget();
        panel31->setLayout(new QHBoxLayout());
        QLabel* bLabel = new QLabel(tr("BTrack")+" : ");
        panel31->layout()->addWidget(bLabel);
        panel31->layout()->addWidget(bField);
        centralWidgetLayout->addWidget(panel31);
        bField->setToolTip(tr("Enter name (system or user) of signal head located here."));
        QWidget* panel32 = new QWidget();
        panel32->setLayout(new QHBoxLayout());
        panel32->layout()->addWidget(new QLabel("   "));
        panel32->layout()->addWidget(setBHead);
        setBHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel32->layout()->addWidget(new QLabel("  "));
        panel32->layout()->addWidget(setupBLogic);
        setupBLogic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
        centralWidgetLayout->addWidget(panel32);
        QWidget* panel41 = new QWidget();
        panel41->setLayout(new QHBoxLayout());
        QLabel* cLabel = new QLabel(tr("CTrack")+" : ");
        panel41->layout()->addWidget(cLabel);
        panel41->layout()->addWidget(cField);
        centralWidgetLayout->addWidget(panel41);
        cField->setToolTip(tr("Enter name (system or user) of signal head located here."));
        QWidget* panel42 = new QWidget();
        panel42->setLayout(new QHBoxLayout());
        panel42->layout()->addWidget(new QLabel("   "));
        panel42->layout()->addWidget(setCHead);
        setCHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel42->layout()->addWidget(new QLabel("  "));
        panel42->layout()->addWidget(setupCLogic);
        setupCLogic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
        centralWidgetLayout->addWidget(panel42);
        QWidget* panel51 = new QWidget();
        panel51->setLayout(new QHBoxLayout());
        QLabel* dLabel = new QLabel(tr("DTrack")+" : ");
        panel51->layout()->addWidget(dLabel);
        panel51->layout()->addWidget(dField);
        centralWidgetLayout->addWidget(panel51);
        dField->setToolTip(tr("Enter name (system or user) of signal head located here."));
        QWidget* panel52 = new QWidget();
        panel52->setLayout(new QHBoxLayout());
        panel52->layout()->addWidget(new QLabel("   "));
        panel52->layout()->addWidget(setDHead);
        setDHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel52->layout()->addWidget(new QLabel("  "));
        panel52->layout()->addWidget(setupDLogic);
        setupDLogic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
        centralWidgetLayout->addWidget(panel52);
        centralWidgetLayout->addWidget(new JSeparator(JSeparator::HORIZONTAL));
        QWidget* panel6 = new QWidget();
        panel6->setLayout(new QHBoxLayout());
        panel6->layout()->addWidget(changeXingSignalIcon = new QPushButton(tr("Change Signal Head Icon")));
//        changeXingSignalIcon->layout()->addWidgetActionListener(new ActionListener() {
//                /*public*/ void actionPerformed(ActionEvent e) {
//                    signalFrame->setVisible(true);
//                }
//            });
        connect(changeXingSignalIcon, SIGNAL(clicked()), this, SLOT(on_changeSignalIcon_clicked()));
        changeXingSignalIcon->setToolTip( tr("Click to change icons used to represent signal heads. (Use left-facing icons.)") );
        panel6->layout()->addWidget(new QLabel("  "));
        panel6->layout()->addWidget(setXingSignalsDone = new QPushButton(tr("Done")));
//        setXingSignalsDone->layout()->addWidgetActionListener(new ActionListener() {
//            /*public*/ void actionPerformed(ActionEvent e) {
//                setXingSignalsDonePressed(e);
//            }
//        });
        connect(setXingSignalsDone, SIGNAL(clicked()), this, SLOT(setXingSignalsDonePressed()));
        setXingSignalsDone->setToolTip( tr("Click Done to perform tasks requested above and dismiss this dialog.") );
        panel6->layout()->addWidget(setXingSignalsCancel = new QPushButton(tr("Cancel")));
//        setXingSignalsCancel->layout()->addWidgetActionListener(new ActionListener() {
//            /*public*/ void actionPerformed(ActionEvent e) {
//                setXingSignalsCancelPressed(e);
//            }
//        });
        connect(setXingSignalsCancel, SIGNAL(clicked()), this, SLOT(setXingSignalsCancelPressed()));
        setXingSignalsCancel->setToolTip( tr("Click Cancel to dismiss this dialog without making changes.") );
        centralWidgetLayout->addWidget(panel6);
//        setSignalsAtXingFrame->layout()->addWidgetWindowListener(new java.awt.event.WindowAdapter() {
//            /*public*/ void windowClosing(java.awt.event.WindowEvent e) {
//                setXingSignalsCancelPressed(nullptr);
//            }
//        });
        setSignalsAtXingFrame->addWindowListener(new SXWindowListener(this));
        if (xingFromMenu) xingSignalsGetSaved(nullptr);
    }
    setSignalsAtXingFrame->adjustSize();
    setSignalsAtXingFrame->setVisible(true);
    setSignalsAtXingOpen = true;
}
/*private*/ void LayoutEditorTools::xingSignalsGetSaved (ActionEvent* /*a*/)
{
 if ( !getLevelCrossingInformation() ) return;
 aField->setText(levelXing->getSignalAName());
 bField->setText(levelXing->getSignalBName());
 cField->setText(levelXing->getSignalCName());
 dField->setText(levelXing->getSignalDName());
}

/*private*/ void LayoutEditorTools::setXingSignalsCancelPressed (ActionEvent* /*a*/) {

    setSignalsAtXingOpen = false;
    setSignalsAtXingFrame->setVisible(false);
    xingFromMenu = false;

}
/*private*/ void LayoutEditorTools::setXingSignalsDonePressed (ActionEvent* /*a*/)
{
#if 1
    if ( !getLevelCrossingInformation() ) return;
    if ( !getXingSignalHeadInformation() ) return;
    // place or update signals as requested
    if ( (aHead!=nullptr) && setAHead->isChecked() ) {
        if (isHeadOnPanel(aHead) &&
                (aHead!=getHeadFromName(levelXing->getSignalAName())) ) {
//            JOptionPane.showMessageDialog(setSignalsAtXingFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{aField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAtXingFrame, tr("Error"),  tr("Error - Cannot place signal head \"%1\" on the panel\nbecause it is already on the panel.").arg(aField->text()));
            return;
        }
        else if ( (!levelXingACHorizontal) && (!levelXingACVertical) ) {
//            JOptionPane.showMessageDialog(setSignalsAtXingFrame,
//                tr("InfoMessage3"),"",JOptionPane.INFORMATION_MESSAGE);
         QMessageBox::information(setSignalsAtXingFrame, tr("Information"), tr("Sorry, cannot place signal heads at anchor points\nwhere track is not oriented vertical or horizontal."));
            if ( aHead!=getHeadFromName(levelXing->getSignalAName()) ) {
                removeSignalHeadFromPanel(levelXing->getSignalAName());
                removeAssignment(aHead);
                levelXing->setSignalAName(aField->text().trimmed());
            }
        }
        else {
            removeSignalHeadFromPanel(levelXing->getSignalAName());
            placeXingA();
            removeAssignment(aHead);
            levelXing->setSignalAName(aField->text().trimmed());
            needRedraw = true;
        }
    }
    else if ( (aHead!=nullptr) &&
            (aHead!=getHeadFromName(levelXing->getSignalAName())) &&
            (aHead!=getHeadFromName(levelXing->getSignalBName())) &&
            (aHead!=getHeadFromName(levelXing->getSignalCName())) &&
            (aHead!=getHeadFromName(levelXing->getSignalDName())) )
    {
        if (isHeadOnPanel(aHead))
        {
//            JOptionPane.showMessageDialog(setSignalsAtXingFrame,
//                java.text.MessageFormat.format(tr("SignalsError13"),
//                    new Object[]{aField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAtXingFrame, tr("Error"),  tr("Error - Cannot assign signal head \"%1\" to this block boundary\nbecause it is already on the panel at a different place.").arg(aField->text()));
         return;
        }
        else {
            removeSignalHeadFromPanel(levelXing->getSignalAName());
            removeAssignment(aHead);
            levelXing->setSignalAName(aField->text().trimmed());
        }
    }
    else if ( (aHead!=nullptr) &&
            ( (aHead==getHeadFromName(levelXing->getSignalBName())) ||
                (aHead==getHeadFromName(levelXing->getSignalCName())) ||
                (aHead==getHeadFromName(levelXing->getSignalDName())) ) ) {
// need to figure out what to do in this case.
    }
    else if (aHead==nullptr) {
        removeSignalHeadFromPanel(levelXing->getSignalAName());
        levelXing->setSignalAName("");
    }
    if ( (bHead!=nullptr) && setBHead->isChecked() ) {
        if (isHeadOnPanel(bHead) &&
                (bHead!=getHeadFromName(levelXing->getSignalBName()))) {
//            JOptionPane.showMessageDialog(setSignalsAtXingFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{bField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAtXingFrame, tr("Error"),  tr("Error - Cannot place signal head \"%1\" on the panel\nbecause it is already on the panel.").arg(bField->text()));
            return;
        }
        else if ( (!levelXingACHorizontal) && (!levelXingACVertical) ) {
//            JOptionPane.showMessageDialog(setSignalsAtXingFrame,
//                tr("InfoMessage3"),"",JOptionPane.INFORMATION_MESSAGE);
         QMessageBox::information(setSignalsAtXingFrame, tr("Information"), tr("Sorry, cannot place signal heads at anchor points\nwhere track is not oriented vertical or horizontal."));
            if (bHead!=getHeadFromName(levelXing->getSignalBName())) {
                removeSignalHeadFromPanel(levelXing->getSignalBName());
                removeAssignment(bHead);
                levelXing->setSignalBName(bField->text().trimmed());
            }
        }
        else {
            removeSignalHeadFromPanel(levelXing->getSignalBName());
            placeXingB();
            removeAssignment(bHead);
            levelXing->setSignalBName(bField->text().trimmed());
            needRedraw = true;
        }
    }
    else if ( (bHead!=nullptr) &&
            (bHead!=getHeadFromName(levelXing->getSignalAName())) &&
            (bHead!=getHeadFromName(levelXing->getSignalBName())) &&
            (bHead!=getHeadFromName(levelXing->getSignalCName())) &&
            (bHead!=getHeadFromName(levelXing->getSignalDName())) ) {
        if (isHeadOnPanel(bHead)) {
//            JOptionPane.showMessageDialog(setSignalsAtXingFrame,
//                java.text.MessageFormat.format(tr("SignalsError13"),
//                    new Object[]{bField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAtXingFrame, tr("Error"),  tr("Error - Cannot assign signal head \"%1\" to this block boundary\nbecause it is already on the panel at a different place.").arg(bField->text()));
            return;
        }
        else {
            removeSignalHeadFromPanel(levelXing->getSignalBName());
            removeAssignment(bHead);
            levelXing->setSignalBName(bField->text().trimmed());
        }
    }
    else if ( (bHead!=nullptr) &&
            ( (bHead==getHeadFromName(levelXing->getSignalAName())) ||
                (bHead==getHeadFromName(levelXing->getSignalCName())) ||
                (bHead==getHeadFromName(levelXing->getSignalDName())) ) ) {
// need to figure out what to do in this case.
    }
    else if (bHead==nullptr) {
        removeSignalHeadFromPanel(levelXing->getSignalBName());
        levelXing->setSignalBName("");
    }
    if ( (cHead!=nullptr) && setCHead->isChecked() ) {
        if (isHeadOnPanel(cHead) &&
                (cHead!=getHeadFromName(levelXing->getSignalCName())) ) {
//            JOptionPane.showMessageDialog(setSignalsAtXingFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{cField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAtXingFrame, tr("Error"),  tr("Error - Cannot place signal head \"%1\" on the panel\nbecause it is already on the panel.").arg(cField->text()));
            return;
        }
        else if ( (!levelXingACHorizontal) && (!levelXingACVertical) ) {
//            JOptionPane.showMessageDialog(setSignalsAtXingFrame,
//                tr("InfoMessage3"),"",JOptionPane.INFORMATION_MESSAGE);
         QMessageBox::information(setSignalsAtXingFrame, tr("Information"), tr("Sorry, cannot place signal heads at anchor points\nwhere track is not oriented vertical or horizontal."));
            if ( cHead!=getHeadFromName(levelXing->getSignalCName()) ) {
                removeSignalHeadFromPanel(levelXing->getSignalCName());
                removeAssignment(cHead);
                levelXing->setSignalCName(cField->text().trimmed());
            }
        }
        else {
            removeSignalHeadFromPanel(levelXing->getSignalCName());
            placeXingC();
            removeAssignment(cHead);
            levelXing->setSignalCName(cField->text().trimmed());
            needRedraw = true;
        }
    }
    else if ( (cHead!=nullptr) &&
            (cHead!=getHeadFromName(levelXing->getSignalAName())) &&
            (cHead!=getHeadFromName(levelXing->getSignalBName())) &&
            (cHead!=getHeadFromName(levelXing->getSignalCName())) &&
            (cHead!=getHeadFromName(levelXing->getSignalDName())) ) {
        if (isHeadOnPanel(cHead)) {
//            JOptionPane.showMessageDialog(setSignalsAtXingFrame,
//                java.text.MessageFormat.format(tr("SignalsError13"),
//                    new Object[]{cField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAtXingFrame, tr("Error"),  tr("Error - Cannot assign signal head \"%1\" to this block boundary\nbecause it is already on the panel at a different place.").arg(cField->text()));
            return;
        }
        else {
            removeSignalHeadFromPanel(levelXing->getSignalCName());
            removeAssignment(cHead);
            levelXing->setSignalCName(cField->text().trimmed());
        }
    }
    else if ( (cHead!=nullptr) &&
            ( (cHead==getHeadFromName(levelXing->getSignalBName())) ||
                (cHead==getHeadFromName(levelXing->getSignalAName())) ||
                (cHead==getHeadFromName(levelXing->getSignalDName())) ) ) {
// need to figure out what to do in this case.
    }
    else if (cHead==nullptr) {
        removeSignalHeadFromPanel(levelXing->getSignalCName());
        levelXing->setSignalCName("");
    }
    if ( (dHead!=nullptr) && setDHead->isChecked() ) {
        if (isHeadOnPanel(dHead) &&
                (dHead!=getHeadFromName(levelXing->getSignalDName())) ) {
//            JOptionPane.showMessageDialog(setSignalsAtXingFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{dField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAtXingFrame, tr("Error"),  tr("Error - Cannot place signal head \"%1\" on the panel\nbecause it is already on the panel.").arg(dField->text()));
            return;
        }
        else if ( (!levelXingACHorizontal) && (!levelXingACVertical) ) {
//            JOptionPane.showMessageDialog(setSignalsAtXingFrame,
//                tr("InfoMessage3"),"",JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(setSignalsAtXingFrame, tr("Information"), tr("Sorry, cannot place signal heads at anchor points\nwhere track is not oriented vertical or horizontal."));
            if ( dHead!=getHeadFromName(levelXing->getSignalDName()) ) {
                removeSignalHeadFromPanel(levelXing->getSignalDName());
                removeAssignment(dHead);
                levelXing->setSignalDName(dField->text().trimmed());
            }
        }
        else {
            removeSignalHeadFromPanel(levelXing->getSignalDName());
            placeXingD();
            removeAssignment(dHead);
            levelXing->setSignalDName(dField->text().trimmed());
            needRedraw = true;
        }
    }
    else if ( (dHead!=nullptr) &&
            (dHead!=getHeadFromName(levelXing->getSignalAName())) &&
            (dHead!=getHeadFromName(levelXing->getSignalBName())) &&
            (dHead!=getHeadFromName(levelXing->getSignalCName())) &&
            (dHead!=getHeadFromName(levelXing->getSignalDName())) ) {
        if (isHeadOnPanel(dHead)) {
//            JOptionPane.showMessageDialog(setSignalsAtXingFrame,
//                java.text.MessageFormat.format(tr("SignalsError13"),
//                    new Object[]{dField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAtXingFrame, tr("Error"),  tr("Error - Cannot assign signal head \"%1\" to this block boundary\nbecause it is already on the panel at a different place.").arg(dField->text()));
            return;
        }
        else {
            removeSignalHeadFromPanel(levelXing->getSignalDName());
            removeAssignment(dHead);
            levelXing->setSignalDName(dField->text().trimmed());
        }
    }
    else if ( (dHead!=nullptr) &&
            ( (dHead==getHeadFromName(levelXing->getSignalBName())) ||
                (dHead==getHeadFromName(levelXing->getSignalCName())) ||
                (dHead==getHeadFromName(levelXing->getSignalAName())) ) ) {
// need to figure out what to do in this case.
    }
    else if (dHead==nullptr) {
        removeSignalHeadFromPanel(levelXing->getSignalDName());
        levelXing->setSignalDName("");
    }
    // setup logic if requested
    if ( setupALogic->isChecked() && (aHead!=nullptr) ) {
        setLogicXing( aHead, (TrackSegment*)levelXing->getConnectC(),
            levelXing->getLayoutBlockBD(), (TrackSegment*)levelXing->getConnectB(),
                (TrackSegment*)levelXing->getConnectD(), aField->text());
    }
    if ( setupBLogic->isChecked() && (bHead!=nullptr) ) {
        setLogicXing( bHead, (TrackSegment*)levelXing->getConnectD(),
            levelXing->getLayoutBlockAC(), (TrackSegment*)levelXing->getConnectA(),
                (TrackSegment*)levelXing->getConnectC(), bField->text());
    }
    if ( setupCLogic->isChecked() && (cHead!=nullptr) ) {
        setLogicXing( cHead, (TrackSegment*)levelXing->getConnectA(),
            levelXing->getLayoutBlockBD(), (TrackSegment*)levelXing->getConnectB(),
                (TrackSegment*)levelXing->getConnectD(), cField->text());
    }
    if ( setupDLogic->isChecked() && (dHead!=nullptr) ) {
        setLogicXing( dHead, (TrackSegment*)levelXing->getConnectB(),
            levelXing->getLayoutBlockAC(), (TrackSegment*)levelXing->getConnectA(),
                (TrackSegment*)levelXing->getConnectC(), dField->text());
    }
    // finish up
    setSignalsAtXingOpen = false;
    setSignalsAtXingFrame->setVisible(false);
    xingFromMenu = false;
    if (needRedraw) {
        layoutEditor->redrawPanel();
        needRedraw = false;
        layoutEditor->setDirty();
    }
#endif
}
#if 1
/*private*/ bool LayoutEditorTools::getLevelCrossingInformation() {
    if (!xingFromMenu) {
        levelXing = NULL;
         QList<LevelXing*> levelXings = layoutEditor->getLevelXings();
        if (levelXings.size()<=0) {
//            JOptionPane.showMessageDialog(setSignalsAtXingFrame,
//                tr("SignalsError15"),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAtXingFrame, tr("Error"), tr("Error - There are no level crossings on your panel.\nPlease add a level crossing or cancel."));
            return false;
        }
        else if (levelXings.size()==1) {
            levelXing = (LevelXing*)levelXings.at(0);
        }
        else {
            LayoutBlock* xingBlockA = NULL;
            LayoutBlock* xingBlockC = NULL;
            xingBlockA = getBlockFromEntry(blockANameField);
            if (xingBlockA==nullptr) return false;
            if (blockCNameField->text().trimmed().length()>0) {
                xingBlockC = getBlockFromEntry(blockCNameField);
                if (xingBlockC==nullptr) return false;
            }
            LevelXing* x = NULL;
            int foundCount = 0;
            // make two block tests first
            if (xingBlockC!=nullptr) {
                for (int i = 0;(i<levelXings.size());i++) {
                    x = (LevelXing*)levelXings.at(i);
                    LayoutBlock* xA = NULL;
                    LayoutBlock* xB = NULL;
                    LayoutBlock* xC = NULL;
                    LayoutBlock* xD = NULL;
                    LayoutBlock* xAC = x->getLayoutBlockAC();
                    LayoutBlock* xBD = x->getLayoutBlockBD();
                    if (x->getConnectA()!=nullptr) xA = ((TrackSegment*)x->getConnectA())->getLayoutBlock();
                    if (x->getConnectB()!=nullptr) xB = ((TrackSegment*)x->getConnectB())->getLayoutBlock();
                    if (x->getConnectC()!=nullptr) xC = ((TrackSegment*)x->getConnectC())->getLayoutBlock();
                    if (x->getConnectD()!=nullptr) xD = ((TrackSegment*)x->getConnectD())->getLayoutBlock();
                    if ( ( (xA!=nullptr) && (xC!=nullptr) && ( ((xA==xingBlockA)&&(xC==xingBlockC)) ||
                            ((xA==xingBlockC)&&(xC==xingBlockA)) ) ) ||
                            ( (xB!=nullptr) && (xD!=nullptr) && ( ((xB==xingBlockA)&&(xD==xingBlockC)) ||
                            ((xB==xingBlockC)&&(xD==xingBlockA)) ) ) ) {
                        levelXing = x;
                        foundCount ++;
                    }
                    else if ( (xAC!=nullptr) && (xBD!=nullptr) && ( ((xAC==xingBlockA) && (xBD==xingBlockC)) ||
                                ((xAC==xingBlockC) && (xBD==xingBlockA)) ) ) {
                        levelXing = x;
                        foundCount ++;
                    }
                }
            }
            if (foundCount==0) {
                // try one block test
                for (int i = 0;(i<layoutEditor->getLevelXings().size());i++) {
                    x = layoutEditor->getLevelXings().at(i);
                    if ((xingBlockA == x->getLayoutBlockAC()) || (xingBlockA == x->getLayoutBlockBD())) {
                        levelXing = x;
                        foundCount ++;
                    }
                }
            }
            if (foundCount>1) {
//                JOptionPane.showMessageDialog(setSignalsAtXingFrame,
//                        java.text.MessageFormat.format(tr("SignalsError16"),
//                            new Object[]{" "+foundCount+" "}),
//                                tr("Error"),JOptionPane.ERROR_MESSAGE);
             QMessageBox::critical(setSignalsAtXingFrame, tr("Error"), tr("or - The entered block names match \"%1\" level crossings.\nPlease enter block names that match only one level crossing.").arg(foundCount));
                return false;
            }
            if (levelXing==nullptr) {
//                JOptionPane.showMessageDialog(setSignalsAtXingFrame,
//                            tr("SignalsError17"),
//                                    tr("Error"),JOptionPane.ERROR_MESSAGE);
             QMessageBox::critical(setSignalsAtXingFrame, tr("Error"), tr("Error - No level crossing matches the block(s)\nentered. Please try again or cancel."));
                return false;
            }
        }
    }
    //if (levelXing->getConnectA()!=nullptr) xingTrackA = ((TrackSegment*)levelXing->getConnectA());
    //if (levelXing->getConnectB()!=nullptr) xingTrackB = ((TrackSegment*)levelXing->getConnectB());
    //if (levelXing->getConnectC()!=nullptr) xingTrackC = ((TrackSegment*)levelXing->getConnectC());
    //if (levelXing->getConnectD()!=nullptr) xingTrackD = ((TrackSegment*)levelXing->getConnectD());
    double delX = levelXing->getCoordsA().x() - levelXing->getCoordsC().x();
    double delY = levelXing->getCoordsA().y() - levelXing->getCoordsC().y();
    levelXingACHorizontal = false;
    levelXingACVertical = false;
    levelXingALeft = false;
    levelXingAUp = false;
    levelXingBUp = false;
    levelXingBLeft = false;
    if (qAbs(delX) > 2.0*qAbs(delY)) {
        levelXingACHorizontal = true;
        if (delX < 0.0) levelXingALeft = true;
        if (levelXing->getCoordsB().y() < levelXing->getCoordsD().y())
            levelXingBUp = true;
    }
    else if (qAbs(delY) > 2.0*qAbs(delX)) {
        levelXingACVertical = true;
        if (delY <0.0) levelXingAUp = true;
        if (levelXing->getCoordsB().x() < levelXing->getCoordsD().x())
            levelXingBLeft = true;
    }
    return true;
}

/*private*/ bool LayoutEditorTools::getXingSignalHeadInformation() {
    // note that all heads are optional, but pairs must be present
    aHead = getSignalHeadFromEntry(aField,false,setSignalsAtXingFrame);
    bHead = getSignalHeadFromEntry(bField,false,setSignalsAtXingFrame);
    cHead = getSignalHeadFromEntry(cField,false,setSignalsAtXingFrame);
    dHead = getSignalHeadFromEntry(dField,false,setSignalsAtXingFrame);
    if ( ((aHead!=nullptr) && (cHead==nullptr)) || ((aHead==nullptr) && (cHead!=nullptr)) ||
            ((bHead!=nullptr) && (dHead==nullptr)) || ((bHead==nullptr) && (dHead!=nullptr)) ) {
//        JOptionPane.showMessageDialog(setSignalsAtXingFrame,
//                tr("SignalsError14"),
//                    tr("Error"),JOptionPane.ERROR_MESSAGE);
     QMessageBox::critical(setSignalsAtXingFrame, tr("Error"), tr("Error - This tool requires signal heads be entered as pairs, A and C,\nand/or B and D. You entered only one head of a pair. "));
        return false;
    }
    if ( (aHead==nullptr) && (bHead==nullptr) && (cHead==nullptr) && (dHead==nullptr) ) {
//        JOptionPane.showMessageDialog(setSignalsAtXingFrame,
//                tr("SignalsError12"),
//                    tr("Error"),JOptionPane.ERROR_MESSAGE);
     QMessageBox::critical(setSignalsAtXingFrame, tr("Error"), tr("Error - No signal heads entered.\nPlease enter signal heads or cancel."));
        return false;
    }
    return true;
}
/*private*/ void LayoutEditorTools::placeXingA()
{
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    QPointF p = levelXing->getCoordsA();
    if (levelXingACHorizontal && levelXingALeft)
    {
        setSignalHeadOnPanel(2,aField->text().trimmed(),
            (int)(p.x()-testIcon->getIconWidth()),
            (int)(p.y()+4) );
    }
    else if (levelXingACHorizontal && (!levelXingALeft) )
    {
        setSignalHeadOnPanel(0,aField->text().trimmed(),
            (int)(p.x()),
            (int)(p.y()-4-testIcon->getIconHeight()) );
    }
    else if (levelXingACVertical && levelXingAUp)
    {
        setSignalHeadOnPanel(1,aField->text().trimmed(),
            (int)(p.x()-2-testIcon->getIconWidth()),
            (int)(p.y()-testIcon->getIconHeight()) );
    }
    else if (levelXingACVertical && (!levelXingAUp) ) {
        setSignalHeadOnPanel(3,aField->text().trimmed(),
            (int)(p.x()+4),
            (int)(p.y()+2) );
    }
}
/*private*/ void LayoutEditorTools::placeXingB() {
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    QPointF p = levelXing->getCoordsB();
    if (levelXingACVertical && levelXingBLeft) {
        setSignalHeadOnPanel(2,bField->text().trimmed(),
            (int)(p.x()-testIcon->getIconWidth()),
            (int)(p.y()+4) );
    }
    else if (levelXingACVertical && (!levelXingBLeft) ) {
        setSignalHeadOnPanel(0,bField->text().trimmed(),
            (int)(p.x()),
            (int)(p.y()-4-testIcon->getIconHeight()) );
    }
    else if (levelXingACHorizontal && levelXingBUp) {
        setSignalHeadOnPanel(1,bField->text().trimmed(),
            (int)(p.x()-2-testIcon->getIconWidth()),
            (int)(p.y()-testIcon->getIconHeight()) );
    }
    else if (levelXingACHorizontal && (!levelXingBUp) ) {
        setSignalHeadOnPanel(3,bField->text().trimmed(),
            (int)(p.x()+4),
            (int)(p.y()+2) );
    }
}
/*private*/ void LayoutEditorTools::placeXingC() {
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    QPointF p = levelXing->getCoordsC();
    if (levelXingACHorizontal && (!levelXingALeft) ) {
        setSignalHeadOnPanel(2,cField->text().trimmed(),
            (int)(p.x()-testIcon->getIconWidth()),
            (int)(p.y()+4) );
    }
    else if (levelXingACHorizontal && levelXingALeft) {
        setSignalHeadOnPanel(0,cField->text().trimmed(),
            (int)(p.x()),
            (int)(p.y()-4-testIcon->getIconHeight()) );
    }
    else if (levelXingACVertical && (!levelXingAUp) ) {
        setSignalHeadOnPanel(1,cField->text().trimmed(),
            (int)(p.x()-2-testIcon->getIconWidth()),
            (int)(p.y()-testIcon->getIconHeight()) );
    }
    else if (levelXingACVertical && levelXingAUp) {
        setSignalHeadOnPanel(3,cField->text().trimmed(),
            (int)(p.x()+4),
            (int)(p.y()+2) );
    }
}
/*private*/ void LayoutEditorTools::placeXingD() {
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    QPointF p = levelXing->getCoordsD();
    if (levelXingACVertical && (!levelXingBLeft) ) {
        setSignalHeadOnPanel(2,dField->text().trimmed(),
            (int)(p.x()-testIcon->getIconWidth()),
            (int)(p.y()+4) );
    }
    else if (levelXingACVertical && levelXingBLeft) {
        setSignalHeadOnPanel(0,dField->text().trimmed(),
            (int)(p.x()),
            (int)(p.y()-4-testIcon->getIconHeight()) );
    }
    else if (levelXingACHorizontal && (!levelXingBUp) ) {
        setSignalHeadOnPanel(1,dField->text().trimmed(),
            (int)(p.x()-2-testIcon->getIconWidth()),
            (int)(p.y()-testIcon->getIconHeight()) );
    }
    else if (levelXingACHorizontal && levelXingBUp) {
        setSignalHeadOnPanel(3,dField->text().trimmed(),
            (int)(p.x()+4),
            (int)(p.y()+2) );
    }
}
//@SuppressWarnings("NULL")
/*private*/ void LayoutEditorTools::setLogicXing(SignalHead* head, TrackSegment* track, LayoutBlock*crossBlock,
            TrackSegment* crossTrack1, TrackSegment* crossTrack2, QString headName) {
    if (track==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAtXingFrame,
//                tr("InfoMessage7"),"",JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::critical(setSignalsAtXingFrame, tr("Error"), tr("Error - The two blocks entered do not join at an anchor point.\nPlease enter the correct blocks and try again."));
        return;
    }
    Sensor* occupancy = NULL;
    Sensor* crossOccupancy = NULL;
    Sensor* track1Occupancy = NULL;
    Sensor* track2Occupancy = NULL;
    SignalHead* nextHead = NULL;
    LayoutBlock* block = track->getLayoutBlock();
    if (block==nullptr) {
     QMessageBox::information(setSignalsAtXingFrame, tr("Information"), tr("Cannot set up logic because blocks have\nnot been defined around this item."));
        return;
    }
    occupancy = block->getOccupancySensor();
    if (occupancy==nullptr)
    {
//        JOptionPane.showMessageDialog(setSignalsAtXingFrame,
//                java.text.MessageFormat.format(tr("InfoMessage4"),
//                    new Object[]{block->getUserName()}),
//                        NULL,JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::critical(setSignalsAtXingFrame, tr("Information"), tr("Cannot set up logic because block \"%1\"\ndoesn''t have an occupancy sensor.").arg(block->getUserName()));
        return;
    }
    if (crossBlock!=nullptr) {
        crossOccupancy = crossBlock->getOccupancySensor();
    }
    if (crossTrack1!=nullptr) {
        block = crossTrack1->getLayoutBlock();
        if (block!=nullptr) {
            track1Occupancy = block->getOccupancySensor();
            if (track1Occupancy==crossOccupancy) track1Occupancy = NULL;
        }
    }
    if (crossTrack2!=nullptr) {
        block = crossTrack2->getLayoutBlock();
        if (block!=nullptr) {
            track2Occupancy = block->getOccupancySensor();
            if ( (track2Occupancy==crossOccupancy) ||
                 (track2Occupancy==track1Occupancy) ) track2Occupancy = NULL;
        }
    }
    nextHead = getNextSignalFromObject(track,levelXing,
                            head->getSystemName(), setSignalsAtXoverFrame);
    if ( (nextHead==nullptr) && (!reachedEndBumper()) ) {
//        JOptionPane.showMessageDialog(setSignalsAtXingFrame,
//                java.text.MessageFormat.format(tr("InfoMessage5"),
//                    new Object[]{block->getUserName()}),
//                        NULL,JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::critical(setSignalsAtXingFrame, tr("Information"), tr("Cannot set up logic because the next signal (in or\nat the end of block \"%1\") apparently is not yet defined.").arg(block->getUserName()));
        return;
    }
    if ( (crossOccupancy==nullptr) && (track1Occupancy==nullptr) && (track2Occupancy==nullptr) ) {
//        JOptionPane.showMessageDialog(setSignalsAtXingFrame,
//                java.text.MessageFormat.format(tr("SignalsWarn1"),
//                    new Object[]{headName}),
//                        NULL,JOptionPane.WARNING_MESSAGE);
     QMessageBox::warning(setSignalsAtXingFrame, tr("Warning"), tr("Warning - Logic was set up at signal head \"%1\", but no sensors\nto protect for cross traffic at the level crossing were found.").arg(headName));
    }
    if (!initializeBlockBossLogic(head->getSystemName())) return;
    logic->setMode(BlockBossLogic::SINGLEBLOCK);
    logic->setSensor1(occupancy->getSystemName());
    if (nextHead!=nullptr) {
        logic->setWatchedSignal1(nextHead->getSystemName(),false);
    }
    if (auxSignal!=nullptr) {
        logic->setWatchedSignal1Alt(auxSignal->getSystemName());
    }
    if (crossOccupancy!=nullptr) {
        logic->setSensor2(crossOccupancy->getSystemName());
        if (track1Occupancy!=nullptr) {
            logic->setSensor3(track1Occupancy->getSystemName());
            if (track2Occupancy!=nullptr)
                logic->setSensor4(track2Occupancy->getSystemName());
        }
        else if (track2Occupancy!=nullptr)
            logic->setSensor3(track2Occupancy->getSystemName());
    }
    else if (track1Occupancy!=nullptr) {
        logic->setSensor2(track1Occupancy->getSystemName());
        if (track2Occupancy!=nullptr)
            logic->setSensor3(track2Occupancy->getSystemName());
    }
    else if (track2Occupancy!=nullptr)
        logic->setSensor2(track2Occupancy->getSystemName());
    finalizeBlockBossLogic();
}
#endif
/**
 * Tool to set signals at throat-to-throat turnouts, including placing the
 *		signal icons and setup of signal logic for each signal head
 * <P>
 * This tool can only be accessed from the Tools menu. There is no access
 *		from a turnout pop-up menu.
 * <P>
 * This tool requires a situation where two turnouts are connected throat-
 *		to-throat by a single "short" track segment.  The actual length of
 *		the track segment is not tested. If this situation is not found,
 *		and error message is sent to the user.  To get started with this
 *      the user needs to enter at least one of the two connected turnouts.
 * <P>
 * This tool assumes two turnouts connected throat-to-throat, as would be
 *		used to represent a double slip turnout. The turnouts may be either
 *		left-handed, right-handed, wye, or any pair of these. This tool also
 *		assumes that there are no signals at the throat junction. The signal
 *		heads will be rotated to face outward--away from the throats. Four
 *		sets of one or two signal heads will be placed, one at each of the
 *		converging and diverging for each turnout.
 * <P>
 * This tool assumes that each of the four tracks is contained in a
 *		different block-> Things work best if the two throat-to-throat
 *		turnouts are in their own separate block, but this is not necessary.
 * <P>
 * This tool will place icons on the outside edges of each turnout.
 * <P>
 * At least one signal at each of the four connection points is
 *		required. A second signal at each is optional.
 * <P>
 * This tool only places signal icons if the turnout is either mostly vertical
 *		or mostly horizontal. Some user adjustment may be needed.
 */

/*public*/ void LayoutEditorTools::setThroatToThroatFromMenu( LayoutTurnout* to, QString linkedTurnoutName,  MultiIconEditor* theEditor, JFrame* theFrame )
{
 turnout1NameField->setText(to->getTurnoutName());
 turnout2NameField->setText(linkedTurnoutName);
 a1TToTField->setText("");
 a2TToTField->setText("");
 b1TToTField->setText("");
 b2TToTField->setText("");
 c1TToTField->setText("");
 c2TToTField->setText("");
 d1TToTField->setText("");
 d2TToTField->setText("");
 setSignalsAtTToTTurnouts(theEditor,theFrame);
}
/*public*/ void LayoutEditorTools::setSignalsAtTToTTurnouts( MultiIconEditor* theEditor, JFrame* theFrame )
{
 signalIconEditor = theEditor;
 signalFrame = (JmriJFrame*)theFrame;
 if (setSignalsAtTToTOpen) {
     setSignalsAtTToTFrame->setVisible(true);
     return;
 }
 // Initialize if needed
 if (setSignalsAtTToTFrame == nullptr)
 {
     setSignalsAtTToTFrame = new JmriJFrameX( tr("SignalsAtTToTTurnout"), false, true );
     setSignalsAtTToTFrame->addHelpMenu("package.jmri.jmrit.display.SetSignalsAtTToTTurnout", true);
     setSignalsAtTToTFrame->setLocation(70,30);
     QWidget* theContentPane = setSignalsAtTToTFrame->getContentPane();
     QVBoxLayout* centralWidgetLayout;
     theContentPane->setLayout(centralWidgetLayout = new QVBoxLayout(theContentPane/*, BoxLayout.Y_AXIS*/));
     QWidget* panel1 = new QWidget();
     panel1->setLayout(new QHBoxLayout());
     QLabel* turnout1NameLabel = new QLabel( tr("Turnout")+" 1 "+
                                                         tr("Name") );
     panel1->layout()->addWidget(turnout1NameLabel);
     panel1->layout()->addWidget(turnout1NameField);
     turnout1NameField->setToolTip(tr("Enter name (system or user) of turnout where signals are needed."));
     centralWidgetLayout->addWidget(panel1);
     QWidget* panel11 = new QWidget();
     panel11->setLayout(new QHBoxLayout());
     QLabel* turnout2NameLabel = new QLabel( tr("Turnout")+" 2 "+
                                                         tr("Name") );
     panel11->layout()->addWidget(turnout2NameLabel);
     panel11->layout()->addWidget(turnout2NameField);
     turnout2NameField->setToolTip(tr("Enter name (system or user) of turnout where signals are needed."));
     centralWidgetLayout->addWidget(panel11);
     centralWidgetLayout->addWidget(new JSeparator(JSeparator::HORIZONTAL));
     // Provide for retrieval of names of previously saved signal heads
     QWidget* panel2 = new QWidget();
     QLabel* shTitle = new QLabel(tr("SignalHeads"));
     panel2->layout()->addWidget(shTitle);
     panel2->layout()->addWidget(new QLabel("     "));
     panel2->layout()->addWidget(getSavedTToTSignalHeads = new QPushButton(tr("Get Saved")));
//        getSavedTToTSignalHeads->addActionListener(new ActionListener() {
//                /*public*/ void actionPerformed(ActionEvent e) {
//                    tToTTurnoutSignalsGetSaved(e);
//                }
//            });
     connect(getSavedTToTSignalHeads, SIGNAL(clicked()), this, SLOT(tToTTurnoutSignalsGetSaved()));
     getSavedTToTSignalHeads->setToolTip( "Click to retrieve signal heads previously stored.");
     centralWidgetLayout->addWidget(panel2);
     centralWidgetLayout->addWidget(new JSeparator(JSeparator::HORIZONTAL));
     // Signal heads located at turnout 1
     QWidget* panel21x = new QWidget();
     panel21x->setLayout(new QHBoxLayout());
     panel21x->layout()->addWidget(new QLabel(tr("Signal located at ")+" "+tr("Turnout")+" 1 - "+ tr("continuing track")));
     centralWidgetLayout->addWidget(panel21x);
     QWidget* panel21 = new QWidget();
     panel21->setLayout(new QHBoxLayout());
     panel21->layout()->addWidget(new QLabel(tr("Protects Turnout")+" 2 - "+tr("continuing track")+" : "));
     panel21->layout()->addWidget(a1TToTField);
     centralWidgetLayout->addWidget(panel21);
     a1TToTField->setToolTip(tr("Enter name (system or user) of signal head located here."));
     QWidget* panel22 = new QWidget();
     panel22->setLayout(new QHBoxLayout());
     panel22->layout()->addWidget(new QLabel(tr("Or Both")+" 2 "+tr("Tracks)")+"   "));
     panel22->layout()->addWidget(setA1TToTHead);
     setA1TToTHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
     panel22->layout()->addWidget(new QLabel("  "));
     panel22->layout()->addWidget(setupA1TToTLogic);
     setupA1TToTLogic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
     centralWidgetLayout->addWidget(panel22);
     QWidget* panel23 = new QWidget();
     panel23->setLayout(new QHBoxLayout());
     panel23->layout()->addWidget(new QLabel(tr("Protects Turnout")+" 2 - "+tr("diverging track")+" : "));
     panel23->layout()->addWidget(a2TToTField);
     centralWidgetLayout->addWidget(panel23);
     a2TToTField->setToolTip(tr("Enter name (system or user) of signal head located here."));
     QWidget* panel24 = new QWidget();
     panel24->setLayout(new QHBoxLayout());
     panel24->layout()->addWidget(new QLabel("                "));
     panel24->layout()->addWidget(setA2TToTHead);
     setA2TToTHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
     panel24->layout()->addWidget(new QLabel("  "));
     panel24->layout()->addWidget(setupA2TToTLogic);
     setupA2TToTLogic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
     centralWidgetLayout->addWidget(panel24);
     QWidget* panel31x = new QWidget();
     panel31x->setLayout(new QHBoxLayout());
     panel31x->layout()->addWidget(new QLabel(tr("Signal located at ")+" "+tr("Turnout")+" 1 - "+
                                     tr("diverging track")));
     centralWidgetLayout->addWidget(panel31x);
     QWidget* panel31 = new QWidget();
     panel31->setLayout(new QHBoxLayout());
     panel31->layout()->addWidget(new QLabel(tr("Protects Turnout")+" 2 - "+tr("continuing track")+" : "));
     panel31->layout()->addWidget(b1TToTField);
     centralWidgetLayout->addWidget(panel31);
     b1TToTField->setToolTip(tr("Enter name (system or user) of signal head located here."));
     QWidget* panel32 = new QWidget();
     panel32->setLayout(new QHBoxLayout());
     panel32->layout()->addWidget(new QLabel(tr("(or both Turnout")+" 2 "+tr("Tracks)")+"   "));
     panel32->layout()->addWidget(setB1TToTHead);
     setB1TToTHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
     panel32->layout()->addWidget(new QLabel("  "));
     panel32->layout()->addWidget(setupB1TToTLogic);
     setupB1TToTLogic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
     centralWidgetLayout->addWidget(panel32);
     QWidget* panel33 = new QWidget();
     panel33->setLayout(new QHBoxLayout());
     panel33->layout()->addWidget(new QLabel(tr("Protects Turnout")+" 2 - "+tr("diverging track")+" : "));
     panel33->layout()->addWidget(b2TToTField);
     centralWidgetLayout->addWidget(panel33);
     b2TToTField->setToolTip(tr("Enter name (system or user) of signal head located here."));
     QWidget* panel34 = new QWidget();
     panel34->setLayout(new QHBoxLayout());
     panel34->layout()->addWidget(new QLabel("                "));
     panel34->layout()->addWidget(setB2TToTHead);
     setB2TToTHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
     panel34->layout()->addWidget(new QLabel("  "));
     panel34->layout()->addWidget(setupB2TToTLogic);
     setupB2TToTLogic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
     centralWidgetLayout->addWidget(panel34);
     centralWidgetLayout->addWidget(new JSeparator(JSeparator::HORIZONTAL));
     // Signal heads located at turnout 2
     QWidget* panel41x = new QWidget();
     panel41x->setLayout(new QHBoxLayout());
     panel41x->layout()->addWidget(new QLabel(tr("Signal located at ")+" "+tr("Turnout")+" 2 - "+
                                     tr("continuing track")));
     centralWidgetLayout->addWidget(panel41x);
     QWidget* panel41 = new QWidget();
     panel41->setLayout(new QHBoxLayout());
     panel41->layout()->addWidget(new QLabel(tr("Protects Turnout")+" 1 - "+tr("continuing track")+" : "));
     panel41->layout()->addWidget(c1TToTField);
     centralWidgetLayout->addWidget(panel41);
     c1TToTField->setToolTip(tr("Enter name (system or user) of signal head located here."));
     QWidget* panel42 = new QWidget();
     panel42->setLayout(new QHBoxLayout());
     panel42->layout()->addWidget(new QLabel(tr("OrBoth")+" 1 "+tr("Tracks)")+"   "));
     panel42->layout()->addWidget(setC1TToTHead);
     setC1TToTHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
     panel42->layout()->addWidget(new QLabel("  "));
     panel42->layout()->addWidget(setupC1TToTLogic);
     setupC1TToTLogic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
     centralWidgetLayout->addWidget(panel42);
     QWidget* panel43 = new QWidget();
     panel43->setLayout(new QHBoxLayout());
     panel43->layout()->addWidget(new QLabel(tr("Protects Turnout")+" 1 - "+tr("diverging track")+" : "));
     panel43->layout()->addWidget(c2TToTField);
     centralWidgetLayout->addWidget(panel43);
     c2TToTField->setToolTip(tr("Enter name (system or user) of signal head located here."));
     QWidget* panel44 = new QWidget();
     panel44->setLayout(new QHBoxLayout());
     panel44->layout()->addWidget(new QLabel("                "));
     panel44->layout()->addWidget(setC2TToTHead);
     setC2TToTHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
     panel44->layout()->addWidget(new QLabel("  "));
     panel44->layout()->addWidget(setupC2TToTLogic);
     setupC2TToTLogic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
     centralWidgetLayout->addWidget(panel44);
     QWidget* panel51x = new QWidget();
     panel51x->setLayout(new QHBoxLayout());
     panel51x->layout()->addWidget(new QLabel(tr("Signal located at ")+" "+tr("Turnout")+" 2 - "+
                                     tr("diverging track")));
     centralWidgetLayout->addWidget(panel51x);
     QWidget* panel51 = new QWidget();
     panel51->setLayout(new QHBoxLayout());
     panel51->layout()->addWidget(new QLabel(tr("Protects Turnout")+" 1 - "+tr("continuing track")+" : "));
     panel51->layout()->addWidget(d1TToTField);
     centralWidgetLayout->addWidget(panel51);
     d1TToTField->setToolTip(tr("Enter name (system or user) of signal head located here."));
     QWidget* panel52 = new QWidget();
     panel52->setLayout(new QHBoxLayout());
     panel52->layout()->addWidget(new QLabel(tr("OrBoth")+" 1 "+tr("Tracks)")+"   "));
     panel52->layout()->addWidget(setD1TToTHead);
     setD1TToTHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
     panel52->layout()->addWidget(new QLabel("  "));
     panel52->layout()->addWidget(setupD1TToTLogic);
     setupD1TToTLogic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
     centralWidgetLayout->addWidget(panel52);
     QWidget* panel53 = new QWidget();
     panel53->setLayout(new QHBoxLayout());
     panel53->layout()->addWidget(new QLabel(tr("Protects Turnout")+" 1 - "+tr("diverging track")+" : "));
     panel53->layout()->addWidget(d2TToTField);
     centralWidgetLayout->addWidget(panel53);
     d2TToTField->setToolTip(tr("Enter name (system or user) of signal head located here."));
     QWidget* panel54 = new QWidget();
     panel54->setLayout(new QHBoxLayout());
     panel54->layout()->addWidget(new QLabel("                "));
     panel54->layout()->addWidget(setD2TToTHead);
     setD2TToTHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
     panel54->layout()->addWidget(new QLabel("  "));
     panel54->layout()->addWidget(setupD2TToTLogic);
     setupD2TToTLogic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
     centralWidgetLayout->addWidget(panel54);
     centralWidgetLayout->addWidget(new JSeparator(JSeparator::HORIZONTAL));
     QWidget* panel6 = new QWidget();
     panel6->setLayout(new QHBoxLayout());
     panel6->layout()->addWidget(changeTToTSignalIcon = new QPushButton(tr("Change Signal Head Icon")));
//        changeTToTSignalIcon->layout()->addWidgetActionListener(new ActionListener() {
//                /*public*/ void actionPerformed(ActionEvent e) {
//                    signalFrame->setVisible(true);
//                }
//            });
     connect(changeTToTSignalIcon, SIGNAL(clicked()), this, SLOT(on_changeTToTSignalIcon_triggered()));
     changeTToTSignalIcon->setToolTip( tr("Change Signal Head IconHint") );
     panel6->layout()->addWidget(new QLabel("  "));
     panel6->layout()->addWidget(setTToTSignalsDone = new QPushButton(tr("Done")));
//        setTToTSignalsDone->addActionListener(new ActionListener() {
//            /*public*/ void actionPerformed(ActionEvent e) {
//                setTToTSignalsDonePressed(e);
//            }
//        });
     connect(setTToTSignalsDone, SIGNAL(clicked()), this, SLOT(setTToTSignalsDonePressed()));
     setTToTSignalsDone->setToolTip( tr("Click Done to perform tasks requested above and dismiss this dialog.") );
     panel6->layout()->addWidget(setTToTSignalsCancel = new QPushButton(tr("Cancel")));
//        setTToTSignalsCancel->addActionListener(new ActionListener() {
//            /*public*/ void actionPerformed(ActionEvent e) {
//                setTToTSignalsCancelPressed(e);
//            }
//        });
     connect(setTToTSignalsCancel, SIGNAL(clicked()), this, SLOT(setTToTSignalsCancelPressed()));
     setTToTSignalsCancel->setToolTip( tr("Click Cancel to dismiss this dialog without making changes.") );
     centralWidgetLayout->addWidget(panel6);
//        setSignalsAtTToTFrame->layout()->addWidgetWindowListener(new java.awt.event.WindowAdapter() {
//            /*public*/ void windowClosing(java.awt.event.WindowEvent e) {
//                setTToTSignalsCancelPressed(nullptr);
//            }
//        });
}
setSignalsAtTToTFrame->pack();
setSignalsAtTToTFrame->setVisible(true);
setSignalsAtTToTOpen = true;
}
void LayoutEditorTools::on_changeTToTSignalIcon_triggered()
{
signalFrame->setVisible(true);
}

/*private*/ void LayoutEditorTools::tToTTurnoutSignalsGetSaved (ActionEvent* /*a*/) {
 if ( !getTToTTurnoutInformation() ) return;
 a1TToTField->setText(layoutTurnout1->getSignalB1Name());
 a2TToTField->setText(layoutTurnout1->getSignalB2Name());
 b1TToTField->setText(layoutTurnout1->getSignalC1Name());
 b2TToTField->setText(layoutTurnout1->getSignalC2Name());
 c1TToTField->setText(layoutTurnout2->getSignalB1Name());
 c2TToTField->setText(layoutTurnout2->getSignalB2Name());
 d1TToTField->setText(layoutTurnout2->getSignalC1Name());
 d2TToTField->setText(layoutTurnout2->getSignalC2Name());
}

/*private*/ bool LayoutEditorTools::getTToTTurnoutInformation() {
 int type = 0;
 QObject* connect = NULL;
 QString str = "";
 turnout1 = NULL;
 turnout2 = NULL;
 layoutTurnout1 = NULL;
 layoutTurnout2 = NULL;
 str = turnout1NameField->text().trimmed();
 if ( (str==nullptr) || (str==("")) )
 {
  // turnout 1 not entered, test turnout 2
  str = turnout2NameField->text().trimmed();
  if ( (str==nullptr) || (str==("")) )
  {
         // no entries in turnout fields
//            JOptionPane.showMessageDialog(setSignalsAtTToTFrame,tr("SignalsError1"),
//                                tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - No turnout name was entered. Please enter a turnout name or cancel."));
         return false;
  }
  turnout2 = (Turnout*)((ProxyTurnoutManager*)InstanceManager::turnoutManagerInstance())->getTurnout(str);
  if (turnout2==nullptr)
  {
//            JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                java.text.MessageFormat.format(tr("SignalsError2"),
//                    new Object[]{str}), tr("Error"),
//                        JOptionPane.ERROR_MESSAGE);
     QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - No turnout is defined for \"%1\". Please enter\n                                                                    a turnout name in the Turnout Table and on the panel."));
     return false ;
     }
     if ( (turnout2->getUserName()==nullptr) || (turnout2->getUserName()==("")) ||
                             turnout2->getUserName()!=(str) ) {
//         str = str.toUpper();
         turnout2NameField->setText(str);
     }
     layoutTurnout2 = getLayoutTurnoutFromTurnout(turnout2,false,str,setSignalsAtTToTFrame);
     if (layoutTurnout2==nullptr)
         return false;
     // have turnout 2 and layout turnout 2 - look for turnout 1
     connectorTrack = (TrackSegment*)layoutTurnout2->getConnectA();
     if (connectorTrack == nullptr)
     {
         // Inform user of error, and terminate
//            JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                    tr("SignalsError18"),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - This tool requires two turnouts (RH, LH, or WYE)\nconnected throat-to-throat by a single track segment."));
         return false;
     }
     type = connectorTrack->getType1();
     connect = connectorTrack->getConnect1();
     if (connect == layoutTurnout2) {
         type = connectorTrack->getType2();
         connect = connectorTrack->getConnect2();
     }
     if ( (type != LayoutEditor::TURNOUT_A) || (connect==nullptr) ) {
         // Not two turnouts connected throat-to-throat by a single Track Segment
         // Inform user of error and terminate
//            JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                    tr("SignalsError18"),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - This tool requires two turnouts (RH, LH, or WYE)\nconnected throat-to-throat by a single track segment."));
         return false;
     }
     layoutTurnout1 = (LayoutTurnout*)connect;
     turnout1 = layoutTurnout1->getTurnout();
     if (turnout1==nullptr) {
//            JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                    tr("SignalsError18"),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - This tool requires two turnouts (RH, LH, or WYE)\nconnected throat-to-throat by a single track segment."));
         return false;
     }
     turnout1NameField->setText(layoutTurnout1->getTurnoutName());
 }
 else {
     // something was entered in the turnout 1 field
     turnout1 = ((ProxyTurnoutManager*)InstanceManager::turnoutManagerInstance())->getTurnout(str);
     if (turnout1==nullptr) {
//            JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                java.text.MessageFormat.format(tr("SignalsError2"),
//                    new Object[]{str}), tr("Error"),
//                        JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - No turnout is defined for \"%1\". Please enter\na turnout name in the Turnout Table and on the panel.").arg(str));
         return false ;
     }
     if ( (turnout1->getUserName()==nullptr) || (turnout1->getUserName()==("")) ||
                             turnout1->getUserName()!=(str) ) {
//         str = str.toUpper();
         turnout1NameField->setText(str);
     }
     // have turnout 1 - get corresponding layoutTurnout
     layoutTurnout1 = getLayoutTurnoutFromTurnout(turnout1,false,str,setSignalsAtTToTFrame);
     if (layoutTurnout1 == nullptr)
         return false;
     turnout1NameField->setText(str);
     // have turnout 1 and layout turnout 1 - was something entered for turnout 2
     str = turnout2NameField->text().trimmed();
     if ( (str==nullptr) || (str==("")) ) {
         // no entry for turnout 2
         connectorTrack = (TrackSegment*)layoutTurnout1->getConnectA();
         if (connectorTrack == nullptr) {
             // Inform user of error, and terminate
//                JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                        tr("SignalsError18"),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
             QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - This tool requires two turnouts (RH, LH, or WYE)\nconnected throat-to-throat by a single track segment."));
             return false;
         }
         type = connectorTrack->getType1();
         connect = connectorTrack->getConnect1();
         if (connect == layoutTurnout1) {
             type = connectorTrack->getType2();
             connect = connectorTrack->getConnect2();
         }
         if ( (type != LayoutEditor::TURNOUT_A) || (connect==nullptr) ) {
             // Not two turnouts connected throat-to-throat by a single Track Segment
             // Inform user of error and terminate
//                JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                        tr("SignalsError18"),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
             QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - This tool requires two turnouts (RH, LH, or WYE)\nconnected throat-to-throat by a single track segment."));
             return false;
         }
         layoutTurnout2 = (LayoutTurnout*)connect;
         turnout2 = layoutTurnout2->getTurnout();
         if (turnout2==nullptr) {
//                JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                        tr("SignalsError18"),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
             QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - This tool requires two turnouts (RH, LH, or WYE)\nconnected throat-to-throat by a single track segment."));
             return false;
         }
         turnout2NameField->setText(layoutTurnout2->getTurnoutName());
     }
     else {
         // turnout 2 entered also
         turnout2 = ((ProxyTurnoutManager*)InstanceManager::turnoutManagerInstance())->getTurnout(str);
         if (turnout2==nullptr) {
//                JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                    java.text.MessageFormat.format(tr("SignalsError2"),
//                            new Object[]{str}), tr("Error"),
//                                JOptionPane.ERROR_MESSAGE);
             QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - No turnout is defined for \"%1\". Please enter\na turnout name in the Turnout Table and on the panel.").arg(str));
             return false ;
         }
         if ( (turnout2->getUserName()==nullptr) || (turnout2->getUserName()==("")) ||
                             turnout2->getUserName()!=(str) ) {
//             str = str.toUpper();
             turnout2NameField->setText(str);
         }
         layoutTurnout2 = getLayoutTurnoutFromTurnout(turnout2,false,str,setSignalsAtTToTFrame);
         if (layoutTurnout2==nullptr)
             return false;
         turnout2NameField->setText(str);
         // check that layout turnout 1 and layout turnout 2 are connected throat-to-throat
         if (layoutTurnout1->getConnectA()!=layoutTurnout2->getConnectA()) {
             // Not two turnouts connected throat-to-throat by a single Track Segment
             // Inform user of error and terminate
//                JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                        tr("SignalsError18"),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
             QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - This tool requires two turnouts (RH, LH, or WYE)\nconnected throat-to-throat by a single track segment."));
             return false;
         }
         connectorTrack = (TrackSegment*)layoutTurnout1->getConnectA();
     }
 }
 // have both turnouts, correctly connected - complete initialization
 layoutTurnout1Horizontal = false;
 layoutTurnout1Vertical = false;
 layoutTurnout2ThroatLeft = false;
 layoutTurnout2Vertical = false;
 layoutTurnout1ThroatLeft = false;
 layoutTurnout1ThroatUp = false;
 layoutTurnout2ThroatLeft = false;
 layoutTurnout2ThroatUp = false;
 layoutTurnout1BUp = false;
 layoutTurnout1BLeft = false;
 layoutTurnout2BUp = false;
 layoutTurnout2BLeft = false;
 double delX = layoutTurnout1->getCoordsA().x() - layoutTurnout1->getCoordsB().x();
 double delY = layoutTurnout1->getCoordsA().y() - layoutTurnout1->getCoordsB().y();
 if (qAbs(delX) > 2.0*qAbs(delY)) {
     layoutTurnout1Horizontal = true;
     if (delX < 0.0) layoutTurnout1ThroatLeft = true;
     if (layoutTurnout1->getCoordsB().y() < layoutTurnout1->getCoordsC().y())
         layoutTurnout1BUp = true;
 }
    else if (qAbs(delY) > 2.0*qAbs(delX)) {
        layoutTurnout1Vertical = true;
        if (delY <0.0) layoutTurnout1ThroatUp = true;
        if (layoutTurnout1->getCoordsB().x() < layoutTurnout1->getCoordsC().x())
            layoutTurnout1BLeft = true;
    }
    delX = layoutTurnout2->getCoordsA().x() - layoutTurnout2->getCoordsB().x();
    delY = layoutTurnout2->getCoordsA().y() - layoutTurnout2->getCoordsB().y();
    if (qAbs(delX) > 2.0*qAbs(delY)) {
        layoutTurnout2Horizontal = true;
        if (delX < 0.0) layoutTurnout2ThroatLeft = true;
        if (layoutTurnout2->getCoordsB().y() < layoutTurnout2->getCoordsC().y())
            layoutTurnout2BUp = true;
    }
    else if (qAbs(delY) > 2.0*qAbs(delX)) {
        layoutTurnout2Vertical = true;
        if (delY <0.0) layoutTurnout2ThroatUp = true;
        if (layoutTurnout2->getCoordsB().x() < layoutTurnout2->getCoordsC().x())
            layoutTurnout2BLeft = true;
    }
    return true;
}

/*private*/ void LayoutEditorTools::setTToTSignalsDonePressed (ActionEvent* /*a*/) {
    if ( !getTToTTurnoutInformation() ) return;
    if ( !getTToTSignalHeadInformation() ) return;
    // place signal icons if requested, and assign signal heads to this turnout
    if (setA1TToTHead->isChecked()) {
        if (isHeadOnPanel(a1TToTHead) &&
            (a1TToTHead!=getHeadFromName(layoutTurnout1->getSignalB1Name()))) {
//            JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{a1Field->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
            QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\nbecause it is already on the panel.").arg(a1Field->text().trimmed()));
            return;
        }
        else if ( (!layoutTurnout1Horizontal) && (!layoutTurnout1Vertical) ) {
//            JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                tr("InfoMessage2"),"",JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(0, tr("Information"), tr("Sorry, cannot place signal heads at turnouts\n                                                   that are not oriented vertical or horizontal."));

            if (a1TToTHead!=getHeadFromName(layoutTurnout1->getSignalB1Name())) {
                removeSignalHeadFromPanel(layoutTurnout1->getSignalB1Name());
                removeAssignment(a1TToTHead);
                layoutTurnout1->setSignalB1Name(a1TToTField->text().trimmed());
            }
        }
        else {
            removeSignalHeadFromPanel(layoutTurnout1->getSignalB1Name());
            if (layoutTurnout1->getContinuingSense()==Turnout::CLOSED)
                placeA1TToT(a1TToTField->text().trimmed());
            else
                placeB1TToT(a1TToTField->text().trimmed());
            removeAssignment(a1TToTHead);
            layoutTurnout1->setSignalB1Name(a1TToTField->text().trimmed());
            needRedraw = true;
        }
    }
    else {
        int assigned = isHeadAssignedHere(a1TToTHead,layoutTurnout1);
        if (assigned == NONE) {
            if ( isHeadOnPanel(a1TToTHead) &&
                                isHeadAssignedAnywhere(a1TToTHead) ) {
//                JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{a1TToTField->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
                QMessageBox::information(0, tr("Information"), tr("Error - Cannot assign signal head \"%1\" to this turnout\n because it is already on the panel at a different place.").arg(a1TToTField->text().trimmed()));
                                             return;
            }
            else {
                removeSignalHeadFromPanel(layoutTurnout1->getSignalB1Name());
                removeAssignment(a1TToTHead);
                layoutTurnout1->setSignalB1Name(a1TToTField->text().trimmed());
            }
        }
        else if (assigned!=B1) {
// need to figure out what to do in this case - assigned to a different position on the same turnout.
        }
    }
    if ( (a2TToTHead!=nullptr) && setA2TToTHead->isChecked() ) {
        if (isHeadOnPanel(a2TToTHead) &&
            (a2TToTHead!=getHeadFromName(layoutTurnout1->getSignalB2Name()))) {
//            JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{a2Field->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
            QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\n because it is already on the panel.").arg(a2Field->text().trimmed()));
            return;
        }
        else if ( (!layoutTurnout1Horizontal) && (!layoutTurnout1Vertical) ) {
//            JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                tr("InfoMessage2"),"",JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(0, tr("Information"), tr("Sorry, cannot place signal heads at turnouts\n                                                   that are not oriented vertical or horizontal."));

            if (a2TToTHead!=getHeadFromName(layoutTurnout1->getSignalB2Name())) {
                removeSignalHeadFromPanel(layoutTurnout1->getSignalB2Name());
                removeAssignment(a2TToTHead);
                layoutTurnout1->setSignalB2Name(a2TToTField->text().trimmed());
            }
        }
        else {
            removeSignalHeadFromPanel(layoutTurnout1->getSignalB2Name());
            if (layoutTurnout1->getContinuingSense()==Turnout::CLOSED)
                placeA2TToT(a2TToTField->text().trimmed());
            else
                placeB2TToT(a2TToTField->text().trimmed());
            removeAssignment(a2TToTHead);
            layoutTurnout1->setSignalB2Name(a2TToTField->text().trimmed());
            needRedraw = true;
        }
    }
    else if (a2TToTHead!=nullptr) {
        int assigned = isHeadAssignedHere(a2TToTHead,layoutTurnout1);
        if (assigned == NONE) {
            if ( isHeadOnPanel(a2TToTHead) &&
                                isHeadAssignedAnywhere(a2TToTHead) ) {
//                JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{a2TToTField->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
                QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\n because it is already on the panel at a different place.").arg(a2TToTField->text().trimmed()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutTurnout1->getSignalB2Name());
                removeAssignment(a2TToTHead);
                layoutTurnout1->setSignalB2Name(a2TToTField->text().trimmed());
            }
        }
        else if (assigned!=B2) {
// need to figure out what to do in this case.
        }
    }
    else if (a2TToTHead==nullptr) {
        removeSignalHeadFromPanel(layoutTurnout1->getSignalB2Name());
        layoutTurnout1->setSignalB2Name("");
    }
    if (setB1TToTHead->isChecked()) {
        if (isHeadOnPanel(b1TToTHead) &&
            (b1TToTHead!=getHeadFromName(layoutTurnout1->getSignalC1Name()))) {
//            JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{b1TToTField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
            QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\n because it is already on the panel.").arg(b1TToTField->text().trimmed()));
            return;
        }
        else if ( (!layoutTurnout1Horizontal) && (!layoutTurnout1Vertical) ) {
//            JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                tr("InfoMessage2"),"",JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(0, tr("Information"), tr("Sorry, cannot place signal heads at turnouts\n                                                   that are not oriented vertical or horizontal."));

            if (b1TToTHead!=getHeadFromName(layoutTurnout1->getSignalC1Name())) {
                removeSignalHeadFromPanel(layoutTurnout->getSignalC1Name());
                removeAssignment(b1TToTHead);
                layoutTurnout1->setSignalC1Name(b1TToTField->text().trimmed());
            }
        }
        else {
            removeSignalHeadFromPanel(layoutTurnout1->getSignalC1Name());
            if (layoutTurnout1->getContinuingSense()==Turnout::CLOSED)
                placeB1TToT(b1TToTField->text().trimmed());
            else
                placeA1TToT(b1TToTField->text().trimmed());
            removeAssignment(b1TToTHead);
            layoutTurnout1->setSignalC1Name(b1TToTField->text().trimmed());
            needRedraw = true;
        }
    }
    else {
        int assigned = isHeadAssignedHere(b1TToTHead,layoutTurnout1);
        if (assigned == NONE) {
            if ( isHeadOnPanel(b1TToTHead) &&
                                isHeadAssignedAnywhere(b1TToTHead) ) {
//                JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{b1TToTField->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
                QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\n because it is already on the panel at a different place.").arg(b1TToTField->text().trimmed()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutTurnout1->getSignalC1Name());
                removeAssignment(b1TToTHead);
                layoutTurnout1->setSignalC1Name(b1TToTField->text().trimmed());
            }
        }
        else if (assigned!=C1) {
// need to figure out what to do in this case.
        }
    }
    if ( (b2TToTHead!=nullptr) && setB2TToTHead->isChecked() ) {
        if (isHeadOnPanel(b2TToTHead) &&
            (b2TToTHead!=getHeadFromName(layoutTurnout1->getSignalC2Name()))) {
//            JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{b2TToTField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
            QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\n because it is already on the panel.").arg(b2Field->text().trimmed()));
            return;
        }
        else if ( (!layoutTurnout1Horizontal) && (!layoutTurnout1Vertical) ) {
//            JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                tr("InfoMessage2"),"",JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(0, tr("Information"), tr("Sorry, cannot place signal heads at turnouts\n                                                   that are not oriented vertical or horizontal."));

            if (b2TToTHead!=getHeadFromName(layoutTurnout1->getSignalC2Name())) {
                removeSignalHeadFromPanel(layoutTurnout1->getSignalC2Name());
                removeAssignment(b2TToTHead);
                layoutTurnout1->setSignalC2Name(b2TToTField->text().trimmed());
            }
        }
        else {
            removeSignalHeadFromPanel(layoutTurnout1->getSignalC2Name());
            if (layoutTurnout1->getContinuingSense()==Turnout::CLOSED)
                placeB2TToT(b2TToTField->text().trimmed());
            else
                placeA2TToT(b2TToTField->text().trimmed());
            removeAssignment(b2TToTHead);
            layoutTurnout1->setSignalC2Name(b2TToTField->text().trimmed());
            needRedraw = true;
        }
    }
    else if (b2TToTHead!=nullptr) {
        int assigned = isHeadAssignedHere(b2TToTHead,layoutTurnout1);
        if (assigned == NONE) {
            if ( isHeadOnPanel(b2TToTHead) &&
                                isHeadAssignedAnywhere(b2TToTHead) ) {
//                JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{b2TToTField->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
                QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\n because it is already on the panel at a different place.").arg(b2TToTField->text().trimmed()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutTurnout1->getSignalC2Name());
                removeAssignment(b2TToTHead);
                layoutTurnout1->setSignalC2Name(b2TToTField->text().trimmed());
            }
        }
        else if (assigned!=C2) {
// need to figure out what to do in this case.
        }
    }
    else if (b2TToTHead==nullptr) {
        removeSignalHeadFromPanel(layoutTurnout1->getSignalC2Name());
        layoutTurnout1->setSignalC2Name("");
    }
    // signal heads on turnout 2
    if (setC1TToTHead->isChecked()) {
        if (isHeadOnPanel(c1TToTHead) &&
            (c1TToTHead!=getHeadFromName(layoutTurnout2->getSignalB1Name()))) {
//            JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{c1TToTField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
            QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\n because it is already on the panel.").arg(c1TToTField->text().trimmed()));
            return;
        }
        else if ( (!layoutTurnout2Horizontal) && (!layoutTurnout2Vertical) ) {
//            JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                tr("InfoMessage2"),"",JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(0, tr("Information"), tr("Sorry, cannot place signal heads at turnouts\n                                                   that are not oriented vertical or horizontal."));

            if (c1TToTHead!=getHeadFromName(layoutTurnout2->getSignalB1Name())) {
                removeSignalHeadFromPanel(layoutTurnout2->getSignalB1Name());
                removeAssignment(c1TToTHead);
                layoutTurnout2->setSignalB1Name(c1TToTField->text().trimmed());
            }
        }
        else {
            removeSignalHeadFromPanel(layoutTurnout2->getSignalB1Name());
            if (layoutTurnout2->getContinuingSense()==Turnout::CLOSED)
                placeC1TToT(c1TToTField->text().trimmed());
            else
                placeD1TToT(c1TToTField->text().trimmed());
            removeAssignment(c1TToTHead);
            layoutTurnout2->setSignalB1Name(c1TToTField->text().trimmed());
            needRedraw = true;
        }
    }
    else {
        int assigned = isHeadAssignedHere(c1TToTHead,layoutTurnout2);
        if (assigned == NONE) {
            if ( isHeadOnPanel(c1TToTHead) &&
                                isHeadAssignedAnywhere(c1TToTHead) ) {
//                JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{c1TToTField->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
                QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\n because it is already on the panel at a different place.").arg(c1TToTField->text().trimmed()));

                return;
            }
            else {
                removeSignalHeadFromPanel(layoutTurnout2->getSignalB1Name());
                removeAssignment(c1TToTHead);
                layoutTurnout2->setSignalB1Name(c1TToTField->text().trimmed());
            }
        }
        else if (assigned!=B1) {
// need to figure out what to do in this case.
        }
    }
    if ( (c2TToTHead!=nullptr) && setC2TToTHead->isChecked() ) {
        if (isHeadOnPanel(c2TToTHead) &&
            (c2TToTHead!=getHeadFromName(layoutTurnout2->getSignalB2Name()))) {
//            JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{c2TToTField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
            QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\n because it is already on the panel.").arg(c2TToTField->text().trimmed()));
            return;
        }
        else if ( (!layoutTurnout2Horizontal) && (!layoutTurnout2Vertical) ) {
//            JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                tr("InfoMessage2"),"",JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(0, tr("Information"), tr("Sorry, cannot place signal heads at turnouts\n                                                   that are not oriented vertical or horizontal."));

            if (c2TToTHead!=getHeadFromName(layoutTurnout2->getSignalB2Name())) {
                removeSignalHeadFromPanel(layoutTurnout2->getSignalB2Name());
                removeAssignment(c2TToTHead);
                layoutTurnout2->setSignalC2Name(c2TToTField->text().trimmed());
            }
        }
        else {
            removeSignalHeadFromPanel(layoutTurnout2->getSignalB2Name());
            if (layoutTurnout2->getContinuingSense()==Turnout::CLOSED)
                placeC2TToT(c2TToTField->text().trimmed());
            else
                placeD2TToT(c2TToTField->text().trimmed());
            removeAssignment(c2TToTHead);
            layoutTurnout2->setSignalB2Name(c2TToTField->text().trimmed());
            needRedraw = true;
        }
    }
    else if (c2TToTHead!=nullptr) {
        int assigned = isHeadAssignedHere(c2TToTHead,layoutTurnout2);
        if (assigned == NONE) {
            if ( isHeadOnPanel(c2TToTHead) &&
                                isHeadAssignedAnywhere(c2TToTHead) ) {
//                JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{c2TToTField->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
                QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\nbecause it is already on the panel at a different place.").arg(c2TToTField->text().trimmed()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutTurnout2->getSignalB2Name());
                removeAssignment(c2TToTHead);
                layoutTurnout2->setSignalB2Name(c2TToTField->text().trimmed());
            }
        }
        else if (assigned!=B2) {
// need to figure out what to do in this case.
        }
    }
    else if (c2TToTHead==nullptr) {
        removeSignalHeadFromPanel(layoutTurnout2->getSignalB2Name());
        layoutTurnout2->setSignalB2Name("");
    }
    if (setD1TToTHead->isChecked()) {
        if (isHeadOnPanel(d1TToTHead) &&
            (d1TToTHead!=getHeadFromName(layoutTurnout2->getSignalC1Name()))) {
//                JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{d1TToTField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
            QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\n because it is already on the panel.").arg(d1TToTField->text().trimmed()));
            return;
        }
        else if ( (!layoutTurnout2Horizontal) && (!layoutTurnout2Vertical) ) {
//            JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                tr("InfoMessage2"),"",JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(0, tr("Information"), tr("Sorry, cannot place signal heads at turnouts\n                                                   that are not oriented vertical or horizontal."));

            if (d1TToTHead!=getHeadFromName(layoutTurnout2->getSignalC1Name())) {
                removeSignalHeadFromPanel(layoutTurnout2->getSignalC1Name());
                removeAssignment(d1TToTHead);
                layoutTurnout2->setSignalC1Name(d1TToTField->text().trimmed());
            }
        }
        else {
            removeSignalHeadFromPanel(layoutTurnout2->getSignalC1Name());
            if (layoutTurnout2->getContinuingSense()==Turnout::CLOSED)
                placeD1TToT(d1TToTField->text().trimmed());
            else
                placeC1TToT(d1TToTField->text().trimmed());
            removeAssignment(d1TToTHead);
            layoutTurnout2->setSignalC1Name(d1TToTField->text().trimmed());
            needRedraw = true;
        }
    }
    else {
        int assigned = isHeadAssignedHere(d1TToTHead,layoutTurnout2);
        if (assigned == NONE) {
            if ( isHeadOnPanel(d1TToTHead) &&
                                isHeadAssignedAnywhere(d1TToTHead) ) {
//                JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{d1TToTField->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
                QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\nbecause it is already on the panel at a different place.").arg(d1TToTField->text().trimmed()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutTurnout2->getSignalC1Name());
                removeAssignment(d1TToTHead);
                layoutTurnout2->setSignalC1Name(d1TToTField->text().trimmed());
            }
        }
        else if (assigned!=C1) {
// need to figure out what to do in this case.
        }
    }
    if ( (d2TToTHead!=nullptr) && setD2TToTHead->isChecked() ) {
        if (isHeadOnPanel(d2TToTHead) &&
            (d2TToTHead!=getHeadFromName(layoutTurnout2->getSignalC2Name()))) {
//            JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{d2TToTField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
            QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\n because it is already on the panel.").arg(d2TToTField->text().trimmed()));
            return;
        }
        else if ( (!layoutTurnout2Horizontal) && (!layoutTurnout2Vertical) ) {
//            JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                tr("InfoMessage2"),"",JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(0, tr("Information"), tr("Sorry, cannot place signal heads at turnouts\n                                                   that are not oriented vertical or horizontal."));

            if (d2TToTHead!=getHeadFromName(layoutTurnout2->getSignalC2Name())) {
                removeSignalHeadFromPanel(layoutTurnout2->getSignalC2Name());
                removeAssignment(d2TToTHead);
                layoutTurnout2->setSignalC2Name(d2TToTField->text().trimmed());
            }
        }
        else {
            removeSignalHeadFromPanel(layoutTurnout2->getSignalC2Name());
            if (layoutTurnout2->getContinuingSense()==Turnout::CLOSED)
                placeD2TToT(d2TToTField->text().trimmed());
            else
                placeC2TToT(d2TToTField->text().trimmed());
            removeAssignment(d2TToTHead);
            layoutTurnout2->setSignalC2Name(d2TToTField->text().trimmed());
            needRedraw = true;
        }
    }
    else if (d2TToTHead!=nullptr) {
        int assigned = isHeadAssignedHere(d2TToTHead,layoutTurnout2);
        if (assigned == NONE) {
            if ( isHeadOnPanel(d2TToTHead) &&
                                isHeadAssignedAnywhere(d2TToTHead) ) {
//                JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{d2TToTField->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
                QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\nbecause it is already on the panel at a different place.").arg(d2TToTField->text().trimmed()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutTurnout2->getSignalC2Name());
                removeAssignment(d2TToTHead);
                layoutTurnout2->setSignalC2Name(d2TToTField->text().trimmed());
            }
        }
        else if (assigned!=C2) {
// need to figure out what to do in this case.
        }
    }
    else if (d2TToTHead==nullptr) {
        removeSignalHeadFromPanel(layoutTurnout2->getSignalC2Name());
        layoutTurnout2->setSignalC2Name("");
    }

    // setup logic if requested
    if (setupA1TToTLogic->isChecked() || setupA2TToTLogic->isChecked()) {
        setLogicTToT(a1TToTHead,(TrackSegment*)layoutTurnout2->getConnectB(),a2TToTHead,
                (TrackSegment*)layoutTurnout2->getConnectC(),setupA1TToTLogic->isChecked(),
                setupA2TToTLogic->isChecked(),true,layoutTurnout2,layoutTurnout1);
    }
    if (setupB1TToTLogic->isChecked() || setupB2TToTLogic->isChecked()) {
        setLogicTToT(b1TToTHead,(TrackSegment*)layoutTurnout2->getConnectB(),b2TToTHead,
                (TrackSegment*)layoutTurnout2->getConnectC(),setupB1TToTLogic->isChecked(),
                setupB2TToTLogic->isChecked(),false,layoutTurnout2,layoutTurnout1);
    }
    if (setupC1TToTLogic->isChecked() || setupC2TToTLogic->isChecked()) {
        setLogicTToT(c1TToTHead,(TrackSegment*)layoutTurnout1->getConnectB(),c2TToTHead,
                (TrackSegment*)layoutTurnout1->getConnectC(),setupC1TToTLogic->isChecked(),
                setupC2TToTLogic->isChecked(),true,layoutTurnout1,layoutTurnout2);
    }
    if (setupD1TToTLogic->isChecked() || setupD2TToTLogic->isChecked()) {
        setLogicTToT(d1TToTHead,(TrackSegment*)layoutTurnout1->getConnectB(),d2TToTHead,
                (TrackSegment*)layoutTurnout1->getConnectC(),setupD1TToTLogic->isChecked(),
                setupD2TToTLogic->isChecked(),false,layoutTurnout1,layoutTurnout2);
    }
    // link the two turnouts
    layoutTurnout1->setLinkedTurnoutName(turnout2NameField->text().trimmed());
    layoutTurnout1->setLinkType(LayoutTurnout::THROAT_TO_THROAT);
    layoutTurnout2->setLinkedTurnoutName(turnout1NameField->text().trimmed());
    layoutTurnout2->setLinkType(LayoutTurnout::THROAT_TO_THROAT);
    // finish up
    setSignalsAtTToTOpen = false;
    setSignalsAtTToTFrame->setVisible(false);
    if (needRedraw) {
        layoutEditor->redrawPanel();
        needRedraw = false;
        layoutEditor->setDirty();
    }
}
/*private*/ bool LayoutEditorTools::getTToTSignalHeadInformation() {
    a1TToTHead = getSignalHeadFromEntry(a1TToTField,true,setSignalsAtTToTFrame);
    if (a1TToTHead==nullptr) return false;
    a2TToTHead = getSignalHeadFromEntry(a2TToTField,false,setSignalsAtTToTFrame);
    b1TToTHead = getSignalHeadFromEntry(b1TToTField,true,setSignalsAtTToTFrame);
    if (b1TToTHead==nullptr) return false;
    b2TToTHead = getSignalHeadFromEntry(b2TToTField,false,setSignalsAtTToTFrame);
    c1TToTHead = getSignalHeadFromEntry(c1TToTField,true,setSignalsAtTToTFrame);
    if (c1TToTHead==nullptr) return false;
    c2TToTHead = getSignalHeadFromEntry(c2TToTField,false,setSignalsAtTToTFrame);
    d1TToTHead = getSignalHeadFromEntry(d1TToTField,true,setSignalsAtTToTFrame);
    if (d1TToTHead==nullptr) return false;
    d2TToTHead = getSignalHeadFromEntry(d2TToTField,false,setSignalsAtTToTFrame);
    return true;
}
/*private*/ void LayoutEditorTools::placeA1TToT(QString headName) {
    // place head near the continuing track of turnout 1
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    if( layoutTurnout1Horizontal && layoutTurnout1ThroatLeft && layoutTurnout1BUp ) {
        setSignalHeadOnPanel(0,headName,
            (int)(layoutTurnout1->getCoordsB().x()),
            (int)(layoutTurnout1->getCoordsB().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnout1Horizontal && layoutTurnout1ThroatLeft && (!layoutTurnout1BUp) ) {
        setSignalHeadOnPanel(0,headName,
            (int)(layoutTurnout1->getCoordsB().x()),
            (int)(layoutTurnout1->getCoordsB().y()+4) );
    }
    else if( layoutTurnout1Horizontal && (!layoutTurnout1ThroatLeft) && layoutTurnout1BUp ) {
        setSignalHeadOnPanel(2,headName,
            (int)(layoutTurnout1->getCoordsB().x()-testIcon->getIconWidth()),
            (int)(layoutTurnout1->getCoordsB().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnout1Horizontal && (!layoutTurnout1ThroatLeft) && (!layoutTurnout1BUp) ) {
        setSignalHeadOnPanel(2,headName,
            (int)(layoutTurnout1->getCoordsB().x()-testIcon->getIconWidth()),
            (int)(layoutTurnout1->getCoordsB().y()+4) );
    }
    else if( layoutTurnout1Vertical && layoutTurnout1ThroatUp && layoutTurnout1BLeft ) {
        setSignalHeadOnPanel(3,headName,
            (int)(layoutTurnout1->getCoordsB().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout1->getCoordsB().y()) );
    }
    else if( layoutTurnout1Vertical && layoutTurnout1ThroatUp && (!layoutTurnout1BLeft) ) {
        setSignalHeadOnPanel(3,headName,
            (int)(layoutTurnout1->getCoordsB().x()+4),
            (int)(layoutTurnout1->getCoordsB().y()) );
    }
    else if( layoutTurnout1Vertical && (!layoutTurnout1ThroatUp) && layoutTurnout1BLeft ) {
        setSignalHeadOnPanel(1,headName,
            (int)(layoutTurnout1->getCoordsB().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout1->getCoordsB().y()-testIcon->getIconHeight()) );
    }
    else if( layoutTurnout1Vertical && (!layoutTurnout1ThroatUp) && (!layoutTurnout1BLeft) ) {
        setSignalHeadOnPanel(1,headName,
            (int)(layoutTurnout1->getCoordsB().x()+4),
            (int)(layoutTurnout1->getCoordsB().y()-testIcon->getIconHeight()) );
    }
}
/*private*/ void LayoutEditorTools::placeA2TToT(QString headName) {
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    if( layoutTurnout1Horizontal && layoutTurnout1ThroatLeft && layoutTurnout1BUp ) {
        setSignalHeadOnPanel(0,headName,
            (int)(layoutTurnout1->getCoordsB().x()+4+testIcon->getIconWidth()),
            (int)(layoutTurnout1->getCoordsB().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnout1Horizontal && layoutTurnout1ThroatLeft && (!layoutTurnout1BUp) ) {
        setSignalHeadOnPanel(0,headName,
            (int)(layoutTurnout1->getCoordsB().x()+4+testIcon->getIconWidth()),
            (int)(layoutTurnout1->getCoordsB().y()+4) );
    }
    else if( layoutTurnout1Horizontal && (!layoutTurnout1ThroatLeft) && layoutTurnout1BUp ) {
        setSignalHeadOnPanel(2,headName,
            (int)(layoutTurnout1->getCoordsB().x()-4-(2*testIcon->getIconWidth())),
            (int)(layoutTurnout1->getCoordsB().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnout1Horizontal && (!layoutTurnout1ThroatLeft) && (!layoutTurnout1BUp) ) {
        setSignalHeadOnPanel(2,headName,
            (int)(layoutTurnout1->getCoordsB().x()-4-(2*testIcon->getIconWidth())),
            (int)(layoutTurnout1->getCoordsB().y()+4) );
    }
    else if( layoutTurnout1Vertical && layoutTurnout1ThroatUp && layoutTurnout1BLeft ) {
        setSignalHeadOnPanel(3,headName,
            (int)(layoutTurnout1->getCoordsB().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout1->getCoordsB().y()+4+testIcon->getIconHeight()) );
    }
    else if( layoutTurnout1Vertical && layoutTurnout1ThroatUp && (!layoutTurnout1BLeft) ) {
        setSignalHeadOnPanel(3,headName,
            (int)(layoutTurnout1->getCoordsB().x()+4),
            (int)(layoutTurnout1->getCoordsB().y()+4+testIcon->getIconHeight()) );
    }
    else if( layoutTurnout1Vertical && (!layoutTurnout1ThroatUp) && layoutTurnout1BLeft ) {
        setSignalHeadOnPanel(1,headName,
            (int)(layoutTurnout1->getCoordsB().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout1->getCoordsB().y()-4-(2*testIcon->getIconHeight())) );
    }
    else if( layoutTurnout1Vertical && (!layoutTurnout1ThroatUp) && (!layoutTurnout1BLeft) ) {
        setSignalHeadOnPanel(1,headName,
            (int)(layoutTurnout1->getCoordsB().x()+4),
            (int)(layoutTurnout1->getCoordsB().y()-4-(2*testIcon->getIconHeight())) );
    }
}
/*private*/ void LayoutEditorTools::placeB1TToT(QString headName) {
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    if( layoutTurnout1Horizontal && layoutTurnout1ThroatLeft && layoutTurnout1BUp) {
        setSignalHeadOnPanel(0,headName,
            (int)(layoutTurnout1->getCoordsC().x()),
            (int)(layoutTurnout1->getCoordsC().y()+4) );
    }
    else if( layoutTurnout1Horizontal && layoutTurnout1ThroatLeft && (!layoutTurnout1BUp) ) {
        setSignalHeadOnPanel(0,headName,
            (int)(layoutTurnout1->getCoordsC().x()),
            (int)(layoutTurnout1->getCoordsC().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnout1Horizontal && (!layoutTurnout1ThroatLeft) && layoutTurnout1BUp ) {
        setSignalHeadOnPanel(2,headName,
            (int)(layoutTurnout1->getCoordsC().x()-testIcon->getIconWidth()),
            (int)(layoutTurnout1->getCoordsC().y()+4) );
    }
    else if( layoutTurnout1Horizontal && (!layoutTurnout1ThroatLeft) && (!layoutTurnout1BUp) ) {
        setSignalHeadOnPanel(2,headName,
            (int)(layoutTurnout1->getCoordsC().x()-testIcon->getIconWidth()),
            (int)(layoutTurnout1->getCoordsC().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnout1Vertical && layoutTurnout1ThroatUp && layoutTurnout1BLeft ) {
        setSignalHeadOnPanel(3,headName,
            (int)(layoutTurnout1->getCoordsC().x()+4),
            (int)(layoutTurnout1->getCoordsC().y()) );
    }
    else if( layoutTurnout1Vertical && layoutTurnout1ThroatUp && (!layoutTurnout1BLeft) ) {
        setSignalHeadOnPanel(3,headName,
            (int)(layoutTurnout1->getCoordsC().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout1->getCoordsC().y()) );
    }
    else if( layoutTurnout1Vertical && (!layoutTurnout1ThroatUp) && layoutTurnout1BLeft ) {
        setSignalHeadOnPanel(1,headName,
            (int)(layoutTurnout1->getCoordsC().x()+4),
            (int)(layoutTurnout1->getCoordsC().y()-testIcon->getIconHeight()) );
    }
    else if( layoutTurnout1Vertical && (!layoutTurnout1ThroatUp) && (!layoutTurnout1BLeft) ) {
        setSignalHeadOnPanel(1,headName,
            (int)(layoutTurnout1->getCoordsC().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout1->getCoordsC().y()-testIcon->getIconHeight()) );
    }
}
/*private*/ void LayoutEditorTools::placeB2TToT(QString headName) {
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    if( layoutTurnout1Horizontal && layoutTurnout1ThroatLeft && layoutTurnout1BUp) {
        setSignalHeadOnPanel(0,headName,
            (int)(layoutTurnout1->getCoordsC().x()+4+testIcon->getIconWidth()),
            (int)(layoutTurnout1->getCoordsC().y()+4) );
    }
    else if( layoutTurnout1Horizontal && layoutTurnout1ThroatLeft && (!layoutTurnout1BUp) ) {
        setSignalHeadOnPanel(0,headName,
            (int)(layoutTurnout1->getCoordsC().x()+4+testIcon->getIconWidth()),
            (int)(layoutTurnout1->getCoordsC().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnout1Horizontal && (!layoutTurnout1ThroatLeft) && layoutTurnout1BUp ) {
        setSignalHeadOnPanel(2,headName,
            (int)(layoutTurnout1->getCoordsC().x()-4-(2*testIcon->getIconWidth())),
            (int)(layoutTurnout1->getCoordsC().y()+4) );
    }
    else if( layoutTurnout1Horizontal && (!layoutTurnout1ThroatLeft) && (!layoutTurnout1BUp) ) {
        setSignalHeadOnPanel(2,headName,
            (int)(layoutTurnout1->getCoordsC().x()-4-(2*testIcon->getIconWidth())),
            (int)(layoutTurnout1->getCoordsC().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnout1Vertical && layoutTurnout1ThroatUp && layoutTurnout1BLeft ) {
        setSignalHeadOnPanel(3,headName,
            (int)(layoutTurnout1->getCoordsC().x()+4),
            (int)(layoutTurnout1->getCoordsC().y()+4+testIcon->getIconHeight()) );
    }
    else if( layoutTurnout1Vertical && layoutTurnout1ThroatUp && (!layoutTurnout1BLeft) ) {
        setSignalHeadOnPanel(3,headName,
            (int)(layoutTurnout1->getCoordsC().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout1->getCoordsC().y()+4+testIcon->getIconHeight()) );
    }
    else if( layoutTurnout1Vertical && (!layoutTurnout1ThroatUp) && layoutTurnout1BLeft ) {
        setSignalHeadOnPanel(1,headName,
            (int)(layoutTurnout1->getCoordsC().x()+4),
            (int)(layoutTurnout1->getCoordsC().y()-4-(2*testIcon->getIconHeight())) );
    }
    else if( layoutTurnout1Vertical && (!layoutTurnout1ThroatUp) && (!layoutTurnout1BLeft) ) {
        setSignalHeadOnPanel(1,headName,
            (int)(layoutTurnout1->getCoordsC().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout1->getCoordsC().y()-4-(2*testIcon->getIconHeight())) );
    }
}
/*private*/ void LayoutEditorTools::placeC1TToT(QString headName) {
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    if( layoutTurnout2Horizontal && layoutTurnout2ThroatLeft && layoutTurnout2BUp ) {
        setSignalHeadOnPanel(0,headName,
            (int)(layoutTurnout2->getCoordsB().x()),
            (int)(layoutTurnout2->getCoordsB().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnout2Horizontal && layoutTurnout2ThroatLeft && (!layoutTurnout2BUp) ) {
        setSignalHeadOnPanel(0,headName,
            (int)(layoutTurnout2->getCoordsB().x()),
            (int)(layoutTurnout2->getCoordsB().y()+4) );
    }
    else if( layoutTurnout2Horizontal && (!layoutTurnout2ThroatLeft) && layoutTurnout2BUp ) {
        setSignalHeadOnPanel(2,headName,
            (int)(layoutTurnout2->getCoordsB().x()-testIcon->getIconWidth()),
            (int)(layoutTurnout2->getCoordsB().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnout2Horizontal && (!layoutTurnout2ThroatLeft) && (!layoutTurnout2BUp) ) {
        setSignalHeadOnPanel(2,headName,
            (int)(layoutTurnout2->getCoordsB().x()-testIcon->getIconWidth()),
            (int)(layoutTurnout2->getCoordsB().y()+4) );
    }
    else if( layoutTurnout2Vertical && layoutTurnout2ThroatUp && layoutTurnout2BLeft ) {
        setSignalHeadOnPanel(3,headName,
            (int)(layoutTurnout2->getCoordsB().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout2->getCoordsB().y()) );
    }
    else if( layoutTurnout2Vertical && layoutTurnout2ThroatUp && (!layoutTurnout2BLeft) ) {
        setSignalHeadOnPanel(3,headName,
            (int)(layoutTurnout2->getCoordsB().x()+4),
            (int)(layoutTurnout2->getCoordsB().y()) );
    }
    else if( layoutTurnout2Vertical && (!layoutTurnout2ThroatUp) && layoutTurnout2BLeft ) {
        setSignalHeadOnPanel(1,headName,
            (int)(layoutTurnout2->getCoordsB().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout2->getCoordsB().y()-testIcon->getIconHeight()) );
    }
    else if( layoutTurnout2Vertical && (!layoutTurnout2ThroatUp) && (!layoutTurnout2BLeft) ) {
        setSignalHeadOnPanel(1,headName,
            (int)(layoutTurnout2->getCoordsB().x()+4),
            (int)(layoutTurnout2->getCoordsB().y()-testIcon->getIconHeight()) );
    }
}
/*private*/ void LayoutEditorTools::placeC2TToT(QString headName) {
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    if( layoutTurnout2Horizontal && layoutTurnout2ThroatLeft && layoutTurnout2BUp ) {
        setSignalHeadOnPanel(0,headName,
            (int)(layoutTurnout2->getCoordsB().x()+4+testIcon->getIconWidth()),
            (int)(layoutTurnout2->getCoordsB().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnout2Horizontal && layoutTurnout2ThroatLeft && (!layoutTurnout2BUp) ) {
        setSignalHeadOnPanel(0,headName,
            (int)(layoutTurnout2->getCoordsB().x()+4+testIcon->getIconWidth()),
            (int)(layoutTurnout2->getCoordsB().y()+4) );
    }
    else if( layoutTurnout2Horizontal && (!layoutTurnout2ThroatLeft) && layoutTurnout2BUp ) {
        setSignalHeadOnPanel(2,headName,
            (int)(layoutTurnout2->getCoordsB().x()-4-(2*testIcon->getIconWidth())),
            (int)(layoutTurnout2->getCoordsB().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnout2Horizontal && (!layoutTurnout2ThroatLeft) && (!layoutTurnout2BUp) ) {
        setSignalHeadOnPanel(2,headName,
            (int)(layoutTurnout2->getCoordsB().x()-4-(2*testIcon->getIconWidth())),
            (int)(layoutTurnout2->getCoordsB().y()+4) );
    }
    else if( layoutTurnout2Vertical && layoutTurnout2ThroatUp && layoutTurnout2BLeft ) {
        setSignalHeadOnPanel(3,headName,
            (int)(layoutTurnout2->getCoordsB().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout2->getCoordsB().y()+4+testIcon->getIconHeight()) );
    }
    else if( layoutTurnout2Vertical && layoutTurnout2ThroatUp && (!layoutTurnout2BLeft) ) {
        setSignalHeadOnPanel(3,headName,
            (int)(layoutTurnout2->getCoordsB().x()+4),
            (int)(layoutTurnout2->getCoordsB().y()+4+testIcon->getIconHeight()) );
    }
    else if( layoutTurnout2Vertical && (!layoutTurnout2ThroatUp) && layoutTurnout2BLeft ) {
        setSignalHeadOnPanel(1,headName,
            (int)(layoutTurnout2->getCoordsB().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout2->getCoordsB().y()-4-(2*testIcon->getIconHeight())) );
    }
    else if( layoutTurnout2Vertical && (!layoutTurnout2ThroatUp) && (!layoutTurnout2BLeft) ) {
        setSignalHeadOnPanel(1,headName,
            (int)(layoutTurnout2->getCoordsB().x()+4),
            (int)(layoutTurnout2->getCoordsB().y()-4-(2*testIcon->getIconHeight())) );
    }
}
/*private*/ void LayoutEditorTools::placeD1TToT(QString headName) {
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    if( layoutTurnout2Horizontal && layoutTurnout2ThroatLeft && layoutTurnout2BUp) {
        setSignalHeadOnPanel(0,headName,
            (int)(layoutTurnout2->getCoordsC().x()),
            (int)(layoutTurnout2->getCoordsC().y()+4) );
    }
    else if( layoutTurnout2Horizontal && layoutTurnout2ThroatLeft && (!layoutTurnout2BUp) ) {
        setSignalHeadOnPanel(0,headName,
            (int)(layoutTurnout2->getCoordsC().x()),
            (int)(layoutTurnout2->getCoordsC().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnout2Horizontal && (!layoutTurnout2ThroatLeft) && layoutTurnout2BUp ) {
        setSignalHeadOnPanel(2,headName,
            (int)(layoutTurnout2->getCoordsC().x()-testIcon->getIconWidth()),
            (int)(layoutTurnout2->getCoordsC().y()+4) );
    }
    else if( layoutTurnout2Horizontal && (!layoutTurnout2ThroatLeft) && (!layoutTurnout2BUp) ) {
        setSignalHeadOnPanel(2,headName,
            (int)(layoutTurnout2->getCoordsC().x()-testIcon->getIconWidth()),
            (int)(layoutTurnout2->getCoordsC().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnout2Vertical && layoutTurnout2ThroatUp && layoutTurnout2BLeft ) {
        setSignalHeadOnPanel(3,headName,
            (int)(layoutTurnout2->getCoordsC().x()+4),
            (int)(layoutTurnout2->getCoordsC().y()) );
    }
    else if( layoutTurnout2Vertical && layoutTurnout2ThroatUp && (!layoutTurnout2BLeft) ) {
        setSignalHeadOnPanel(3,headName,
            (int)(layoutTurnout2->getCoordsC().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout2->getCoordsC().y()) );
    }
    else if( layoutTurnout2Vertical && (!layoutTurnout2ThroatUp) && layoutTurnout2BLeft ) {
        setSignalHeadOnPanel(1,headName,
            (int)(layoutTurnout2->getCoordsC().x()+4),
            (int)(layoutTurnout2->getCoordsC().y()-testIcon->getIconHeight()) );
    }
    else if( layoutTurnout2Vertical && (!layoutTurnout2ThroatUp) && (!layoutTurnout2BLeft) ) {
        setSignalHeadOnPanel(1,headName,
            (int)(layoutTurnout2->getCoordsC().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout2->getCoordsC().y()-testIcon->getIconHeight()) );
    }
}
/*private*/ void LayoutEditorTools::placeD2TToT(QString headName) {
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    if( layoutTurnout2Horizontal && layoutTurnout2ThroatLeft && layoutTurnout2BUp) {
        setSignalHeadOnPanel(0,headName,
            (int)(layoutTurnout2->getCoordsC().x()+4+testIcon->getIconWidth()),
            (int)(layoutTurnout2->getCoordsC().y()+4) );
    }
    else if( layoutTurnout2Horizontal && layoutTurnout2ThroatLeft && (!layoutTurnout2BUp) ) {
        setSignalHeadOnPanel(0,headName,
            (int)(layoutTurnout2->getCoordsC().x()+4+testIcon->getIconWidth()),
            (int)(layoutTurnout2->getCoordsC().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnout2Horizontal && (!layoutTurnout2ThroatLeft) && layoutTurnout2BUp ) {
        setSignalHeadOnPanel(2,headName,
            (int)(layoutTurnout2->getCoordsC().x()-4-(2*testIcon->getIconWidth())),
            (int)(layoutTurnout2->getCoordsC().y()+4) );
    }
    else if( layoutTurnout2Horizontal && (!layoutTurnout2ThroatLeft) && (!layoutTurnout2BUp) ) {
        setSignalHeadOnPanel(2,headName,
            (int)(layoutTurnout2->getCoordsC().x()-4-(2*testIcon->getIconWidth())),
            (int)(layoutTurnout2->getCoordsC().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnout2Vertical && layoutTurnout2ThroatUp && layoutTurnout2BLeft ) {
        setSignalHeadOnPanel(3,headName,
            (int)(layoutTurnout2->getCoordsC().x()+4),
            (int)(layoutTurnout2->getCoordsC().y()+4+testIcon->getIconHeight()) );
    }
    else if( layoutTurnout2Vertical && layoutTurnout2ThroatUp && (!layoutTurnout2BLeft) ) {
        setSignalHeadOnPanel(3,headName,
            (int)(layoutTurnout2->getCoordsC().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout2->getCoordsC().y()+4+testIcon->getIconHeight()) );
    }
    else if( layoutTurnout2Vertical && (!layoutTurnout2ThroatUp) && layoutTurnout2BLeft ) {
        setSignalHeadOnPanel(1,headName,
            (int)(layoutTurnout2->getCoordsC().x()+4),
            (int)(layoutTurnout2->getCoordsC().y()-4-(2*testIcon->getIconHeight())) );
    }
    else if( layoutTurnout2Vertical && (!layoutTurnout2ThroatUp) && (!layoutTurnout2BLeft) ) {
        setSignalHeadOnPanel(1,headName,
            (int)(layoutTurnout2->getCoordsC().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnout2->getCoordsC().y()-4-(2*testIcon->getIconHeight())) );
    }
}
//@SuppressWarnings("NULL")
/*private*/ void LayoutEditorTools::setLogicTToT(SignalHead* head,TrackSegment* track1,SignalHead* secondHead,TrackSegment* track2,
                bool setup1, bool setup2, bool continuing,
                    LayoutTurnout* farTurnout, LayoutTurnout* nearTurnout) {
    // initialize common components and ensure all is defined
    LayoutBlock* connectorBlock = connectorTrack->getLayoutBlock();
    LayoutBlock* nearTurnoutBlock = nearTurnout->getLayoutBlock();
    LayoutBlock* farTurnoutBlock = farTurnout->getLayoutBlock();
    Sensor* connectorOccupancy = NULL;
    if ( (connectorBlock==nullptr) || (nearTurnoutBlock==nullptr) || (farTurnoutBlock==nullptr) ) {
//        JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                tr("InfoMessage6"),"",JOptionPane.INFORMATION_MESSAGE);
        QMessageBox::information(setSignalsAtTToTFrame, tr("Information"), tr("Cannot set up logic because blocks have\nnot been defined around this item."));
        return;
    }
    connectorOccupancy = connectorBlock->getOccupancySensor();
    if (connectorOccupancy==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                java.text.MessageFormat.format(tr("InfoMessage4"),
//                    new Object[]{connectorBlock.getUserName()}),
//                        NULL,JOptionPane.INFORMATION_MESSAGE);
        QMessageBox::information(setSignalsAtTToTFrame, tr("Information"), tr("Cannot set up logic because block \"%1\"\ndoesn''t have an occupancy sensor.").arg(connectorBlock->getUserName()));
        return;
    }
    // setup signal head for continuing track of far turnout (or both tracks of far turnout)
    if ( (track1==nullptr) && setup1 ) {
//        JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                tr("InfoMessage7"),"",JOptionPane.INFORMATION_MESSAGE);
        QMessageBox::information(setSignalsAtTToTFrame, tr("Information"), tr("Cannot set up logic because all connections\nhave not been defined around this item."));
        return;
    }
    Sensor* occupancy = NULL;
    SignalHead* nextHead = NULL;
    if ( (track1!=nullptr) && setup1) {
        LayoutBlock* block = track1->getLayoutBlock();
        if (block==nullptr) {
//            JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                tr("InfoMessage6"),"",JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(setSignalsAtTToTFrame, tr("Information"), tr("Cannot set up logic because blocks have\nnot been defined around this item."));
            return;
        }
        occupancy = block->getOccupancySensor();
        if (occupancy==nullptr) {
//            JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                java.text.MessageFormat.format(tr("InfoMessage4"),
//                    new Object[]{block->getUserName()}),
//                        NULL,JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(setSignalsAtTToTFrame, tr("Information"), tr("Cannot set up logic because block \"%1\"\ndoesn''t have an occupancy sensor.").arg(block->getUserName()));
            return;
        }
        nextHead = getNextSignalFromObject(track1,farTurnout,
                                    head->getSystemName(), setSignalsAtTToTFrame);
        if ( (nextHead==nullptr) && (!reachedEndBumper()) ) {
//            JOptionPane.showMessageDialog(setSignalsFrame,
//                java.text.MessageFormat.format(tr("InfoMessage5"),
//                    new Object[]{block->getUserName()}),
//                        NULL,JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(setSignalsFrame, tr("Information"), tr("Cannot set up logic because the next signal (in or\nat the end of block \"%1\") apparently is not yet defined.").arg(block->getUserName()));
            return;
        }
        if (secondHead!=nullptr) {
            // this head signals only the continuing track of the far turnout
            if (!initializeBlockBossLogic(head->getSystemName())) return;
            logic->setMode(BlockBossLogic::TRAILINGMAIN);
            logic->setTurnout(farTurnout->getTurnout()->getSystemName());
            logic->setSensor1(occupancy->getSystemName());
            if (occupancy!=connectorOccupancy)
                logic->setSensor2(connectorOccupancy->getSystemName());
            if (nextHead!=nullptr) {
                logic->setWatchedSignal1(nextHead->getSystemName(),false);
            }
            if (auxSignal!=nullptr) {
                logic->setWatchedSignal1Alt(auxSignal->getSystemName());
            }
            QString nearSensorName = setupNearLogix(nearTurnout,continuing,head);
            addNearSensorToLogic(nearSensorName);
            finalizeBlockBossLogic();
        }
    }
    if ( (secondHead!=nullptr) && !setup2 ) return;
    SignalHead* savedAuxSignal = auxSignal;
    if (track2==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                tr("InfoMessage7"),"",JOptionPane.INFORMATION_MESSAGE);
        QMessageBox::information(setSignalsAtTToTFrame, tr("Information"), tr("Cannot set up logic because all connections\nhave not been defined around this item."));
        return;
    }
    LayoutBlock* block2 = track2->getLayoutBlock();
    if (block2==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                tr("InfoMessage6"),"",JOptionPane.INFORMATION_MESSAGE);
        QMessageBox::information(setSignalsAtTToTFrame, tr("Information"), tr("Cannot set up logic because blocks have\nnot been defined around this item."));
        return;
    }
    Sensor* occupancy2 = block2->getOccupancySensor();
    if (occupancy2==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//            java.text.MessageFormat.format(tr("InfoMessage4"),
//                new Object[]{block2.getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
        QMessageBox::information(setSignalsAtTToTFrame, tr("Information"), tr("Cannot set up logic because block \"%1\"\ndoesn''t have an occupancy sensor.").arg(block2->getUserName()));
        return;
    }
    SignalHead* nextHead2 = NULL;
    if (secondHead!=nullptr) {
        nextHead2 = getNextSignalFromObject(track2,
                farTurnout, secondHead->getSystemName(), setSignalsAtTToTFrame);
        if ( (nextHead2==nullptr) && (!reachedEndBumper()) ) {
//            JOptionPane.showMessageDialog(setSignalsAtTToTFrame,
//                java.text.MessageFormat.format(tr("InfoMessage5"),
//                    new Object[]{block2.getUserName()}),
//                        NULL,JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(setSignalsAtTToTFrame, tr("Information"), tr("Cannot set up logic because the next signal (in or\nat the end of block \"%1\") apparently is not yet defined.").arg(block2->getUserName()));
            return;
        }
    }
    if ( (secondHead==nullptr) && (track1!=nullptr) && setup1 ) {
        if (!initializeBlockBossLogic(head->getSystemName())) return;
        logic->setMode(BlockBossLogic::FACING);
        logic->setTurnout(farTurnout->getTurnout()->getSystemName());
        logic->setWatchedSensor1(occupancy->getSystemName());
        logic->setWatchedSensor2(occupancy2->getSystemName());
        logic->setSensor2(connectorOccupancy->getSystemName());
        if (nextHead!=nullptr) {
            logic->setWatchedSignal1(nextHead->getSystemName(),false);
        }
        if (savedAuxSignal!=nullptr) {
            logic->setWatchedSignal1Alt(savedAuxSignal->getSystemName());
        }
        if (nextHead2!=nullptr) {
            logic->setWatchedSignal2(nextHead2->getSystemName());
        }
        if (auxSignal!=nullptr) {
            logic->setWatchedSignal2Alt(auxSignal->getSystemName());
        }
        QString nearSensorName = setupNearLogix(nearTurnout,continuing,head);
        addNearSensorToLogic(nearSensorName);
        logic->setLimitSpeed2(true);
        finalizeBlockBossLogic();
    }
    else if ( (secondHead!=nullptr) && setup2) {
        if (!initializeBlockBossLogic(secondHead->getSystemName())) return;
        logic->setMode(BlockBossLogic::TRAILINGDIVERGING);
        logic->setTurnout(farTurnout->getTurnout()->getSystemName());
        logic->setSensor1(occupancy2->getSystemName());
        if (occupancy2!=connectorOccupancy)
            logic->setSensor2(connectorOccupancy->getSystemName());
        if (nextHead2!=nullptr) {
            logic->setWatchedSignal1(nextHead2->getSystemName(),false);
        }
        if (auxSignal!=nullptr) {
            logic->setWatchedSignal1Alt(auxSignal->getSystemName());
        }
        QString nearSensorName = setupNearLogix(nearTurnout,continuing,head);
        addNearSensorToLogic(nearSensorName);
        logic->setLimitSpeed2(true);
        finalizeBlockBossLogic();
    }
}

/*
 * Sets up a Logix to set a sensor active if a turnout is set against
 *      a track->  This routine creates an internal sensor for the purpose.
 * Note: The sensor and logix are named IS or IX followed by TTT_X_HHH where
 *		TTT is the system name of the turnout, X is either C or T depending
 *      on "continuing", and HHH is the system name of the signal head.
 * Note: If there is any problem, a string of "" is returned, and a warning
 *		message is issued.
 */
/*private*/ QString LayoutEditorTools::setupNearLogix(LayoutTurnout* nearTurnout, bool continuing, SignalHead* head) {
    QString turnoutName = nearTurnout->getTurnout()->getSystemName();
    QString namer = turnoutName+"_T_"+head->getSystemName();
    if (!continuing) namer = turnoutName+"_C_"+head->getSystemName();
    QString sensorName = "IS"+namer;
    QString logixName = "IX"+namer;
    Sensor* sensor = ((ProxySensorManager*) InstanceManager::sensorManagerInstance())->provideSensor(sensorName);
    if (sensor==nullptr) {
        log->error("Trouble creating sensor "+sensorName+" while setting up Logix->");
        return "";
    }
    if (((LogixManager*)InstanceManager::getDefault("LogixManager"))->getBySystemName(logixName)==nullptr)
    {
        // Logix does not exist, create it
        Logix* x = ((LogixManager*)InstanceManager::getDefault("LogixManager"))->createNewLogix(logixName,"");
        if (x==nullptr) {
            log->error("Trouble creating logix "+logixName+" while setting up signal logic->");
            return "";
        }
        QString cName = x->getSystemName() + "C1";
        Conditional* c = ((ConditionalManager*)InstanceManager::getDefault("ConditionalManager"))->
                                            createNewConditional(cName,"");
        if (c==nullptr) {
            log->error("Trouble creating conditional "+cName+" while setting up Logix->");
            return "";
        }
        int type = Conditional::TYPE_TURNOUT_THROWN;
        if (!continuing) type = Conditional::TYPE_TURNOUT_CLOSED;
        QList <ConditionalVariable*>* variableList = c->getCopyOfStateVariables();
        variableList->append(new ConditionalVariable(false, Conditional::OPERATOR_AND, type, turnoutName, true));
        c->setStateVariables(variableList);
       QList <ConditionalAction*>* actionList = c->getCopyOfActions();
        actionList->append(new DefaultConditionalAction(Conditional::ACTION_OPTION_ON_CHANGE_TO_TRUE,
                                             Conditional::ACTION_SET_SENSOR, sensorName,
                                             Sensor::ACTIVE, ""));
        actionList->append(new DefaultConditionalAction(Conditional::ACTION_OPTION_ON_CHANGE_TO_FALSE,
                                             Conditional::ACTION_SET_SENSOR, sensorName,
                                             Sensor::INACTIVE, ""));
        c->setAction(actionList);										// string data
        x->addConditional(cName,-1);
        x->activateLogix();
    }
    return sensorName;
}
/*
 * Adds the sensor specified to the open BlockBossLogic, provided it is not already there and
 *		provided there is an open slot. If 'name' is NULL or empty, returns without doing anything.
 */
/*private*/ void LayoutEditorTools::addNearSensorToLogic(QString name) {
    if ( (name==nullptr) || name==("") ) return;
    // return if a sensor by this name is already present
    if ( (logic->getSensor1()!=nullptr) && (logic->getSensor1())==(name) ) return;
    if ( (logic->getSensor2()!=nullptr) && (logic->getSensor2())==(name) ) return;
    if ( (logic->getSensor3()!=nullptr) && (logic->getSensor3())==(name) ) return;
    if ( (logic->getSensor4()!=nullptr) && (logic->getSensor4())==(name) ) return;
    if ( (logic->getSensor5()!=nullptr) && (logic->getSensor5())==(name) ) return;
    // add in the first available slot
    if (logic->getSensor1()==nullptr) logic->setSensor1(name);
    else if (logic->getSensor2()==nullptr) logic->setSensor2(name);
    else if (logic->getSensor3()==nullptr) logic->setSensor3(name);
    else if (logic->getSensor4()==nullptr) logic->setSensor4(name);
    else if (logic->getSensor5()==nullptr) logic->setSensor5(name);
    else log->error("Error - could not add sensor to SSL for signal head "+logic->getDrivenSignal());
}

/**
 * Tool to set signals at a three-way turnout, including placing the
 *		signal icons and setup of signal logic for each signal head
 * <P>
 * This tool can only be accessed from the Tools menu. There is no access
 *		from a turnout pop-up menu.
 * <P>
 * This tool requires a situation where two turnouts are connected to model a 3-way
 *		turnout, with the throat of the second turnout connected to the continuing
 *		leg of the first turnout by a very short track segment. The actual length of
 *		the track segment is not tested. If this situation is not found,
 *		and error message is sent to the user.
 * <P>
 * This tool assumes two turnouts connected with the throat of the second turnout
 *		connected to the continuing leg of the first turnout, as used to
 *		represent a 3-way turnout. The turnouts may be either
 *		left-handed, or right-handed, or any pair of these. This tool also
 *		assumes that there are no signals between the two turnouts. Signal
 *		heads are allowed/required at the continuing leg of the second turnout, at
 *		each of the diverging legs, and at the throat. At the throat, either one or
 *		three heads are provided for. So four or six heads will be placed.
 * <P>
 * This tool assumes that each of the four tracks, the continuing, the two diverging,
 *		and the throat is contained in a different block-> The two turnouts used to
 *		model the 3-way turnout must be in the same block-> Things work best if the
 *		two turnouts are in the same block as the track connecting at the throat, or
 *		if the two turnouts are in their own separate block, either works fine.
 * <P>
 * This tool only places signal icons if the turnout is either mostly vertical
 *		or mostly horizontal. Some user adjustment may be needed.
 */


#if 0
/*public*/ void LayoutEditorTools::set3WayFromMenu(QString aName, QString bName,
                MultiIconEditor* theEditor, JFrame* theFrame ) {
    turnoutANameField->setText(aName);
    turnoutBNameField->setText(bName);
    a13WayField->setText("");
    a23WayField->setText("");
    a33WayField->setText("");
    b3WayField->setText("");
    c3WayField->setText("");
    d3WayField->setText("");
    setSignalsAt3WayTurnout(theEditor,theFrame);
}
/*public*/ void LayoutEditorTools::setSignalsAt3WayTurnout( MultiIconEditor* theEditor, JFrame* theFrame ) {
    signalIconEditor = theEditor;
    signalFrame = theFrame;
    if (setSignalsAt3WayOpen) {
        setSignalsAt3WayTurnoutFrame->setVisible(true);
        return;
    }
    // Initialize if needed
    if (setSignalsAt3WayTurnoutFrame == nullptr) {
        setSignalsAt3WayTurnoutFrame = new JmriJFrameX( tr("SignalsAt3WayTurnout"), false, true );
        setSignalsAt3WayTurnoutFrame->addHelpMenu("package.jmri.jmrit.display.SetSignalsAt3WayTurnout", true);
        setSignalsAt3WayTurnoutFrame->setLocation(70,30);
        QWidget* theContentPane = setSignalsAt3WayTurnoutFrame->getContentPane();
        theContentPane->setLayout(new QVBoxLayout);//(theContentPane, BoxLayout.Y_AXIS));
        QWidget* panel1 = new QWidget();
        panel1->setLayout(new QHBoxLayout());
        QLabel* turnoutANameLabel = new QLabel( tr("Turnout A Name (closest to throat)") );
        panel1->layout()->addWidget(turnoutANameLabel);
        panel1->layout()->addWidget(turnoutANameField);
        turnoutANameField->setToolTip(tr("Enter name (system or user) of turnout where signals are needed."));
        theContentPane->layout()->addWidget(panel1);
        QWidget* panel11 = new QWidget();
        panel11->setLayout(new QHBoxLayout());
        QLabel* turnoutBNameLabel = new QLabel( tr("Turnout B Name (farthest from throat)") );
        panel11->layout()->addWidget(turnoutBNameLabel);
        panel11->layout()->addWidget(turnoutBNameField);
        turnoutBNameField->setToolTip(tr("Enter name (system or user) of turnout where signals are needed."));
        theContentPane->layout()->addWidget(panel11);
        theContentPane->layout()->addWidget(new JSeparator(JSeparator::HORIZONTAL));
        // Provide for retrieval of names of previously saved signal heads
        QWidget* panel2 = new QWidget();
        QLabel* shTitle = new QLabel(tr("SignalHeads"));
        panel2->layout()->addWidget(shTitle);
        panel2->layout()->addWidget(new QLabel("     "));
        panel2->layout()->addWidget(getSaved3WaySignalHeads = new QPushButton(tr("Get Saved")));
//        getSaved3WaySignalHeads->addWidgetActionListener(new ActionListener() {
//                /*public*/ void actionPerformed(ActionEvent e) {
//                        getSaved3WaySignals(e);
//                }
//            });
        connect(getSaved3WaySignalHeads, SIGNAL(clicked(bool)), this, SLOT(getSaved3WaySignals()));
        getSaved3WaySignalHeads->setToolTip( "Click to retrieve signal heads previously stored." );
        theContentPane->layout()->addWidget(panel2);
        theContentPane->layout()->addWidget(new JSeparator(JSeparator::HORIZONTAL));
        // Signal heads located at turnout A
        QWidget* panel20	= new QWidget();
        panel20->setLayout(new QHBoxLayout());
        panel20->layout()->addWidget(new QLabel(tr("Signal located at ")+" "+tr("Turnout")+" A "));
        theContentPane->layout()->addWidget(panel20);
        QWidget* panel21 = new QWidget();
        panel21->setLayout(new QHBoxLayout());
        panel21->layout()->addWidget(new QLabel("    "+tr("Throat")+" - "+
                                        tr("Continuing")+" : "));
        panel21->layout()->addWidget(a13WayField);
        a13WayField->setToolTip(tr("Enter name (system or user) of signal head located here."));
        theContentPane->layout()->addWidget(panel21);
        QWidget* panel22 = new QWidget();
        panel22->setLayout(new QHBoxLayout());
        panel22->layout()->addWidget(new QLabel("   "));
        panel22->layout()->addWidget(setA13WayHead);
        setA13WayHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel22->layout()->addWidget(new QLabel("  "));
        panel22->layout()->addWidget(setupA13WayLogic);
        setupA13WayLogic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
        theContentPane->layout()->addWidget(panel22);
        QWidget* panel23 = new QWidget();
        panel23->setLayout(new QHBoxLayout());
        panel23->layout()->addWidget(new QLabel("    "+tr("Throat")+" - "+
                                        tr("DivergingA")+" : "));
        panel23->layout()->addWidget(a23WayField);
        a23WayField->setToolTip(tr("Enter name (system or user) of signal head located here."));
        theContentPane->layout()->addWidget(panel23);
        QWidget* panel24 = new QWidget();
        panel24->setLayout(new QHBoxLayout());
        panel24->layout()->addWidget(new QLabel("   "));
        panel24->layout()->addWidget(setA23WayHead);
        setA23WayHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel24->layout()->addWidget(new QLabel("  "));
        panel24->layout()->addWidget(setupA23WayLogic);
        setupA23WayLogic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
        theContentPane->layout()->addWidget(panel24);
        QWidget* panel25 = new QWidget();
        panel25->setLayout(new QHBoxLayout());
        panel25->layout()->addWidget(new QLabel("    "+tr("Throat")+" - "+
                                        tr("DivergingB")+" : "));
        panel25->layout()->addWidget(a33WayField);
        a33WayField->setToolTip(tr("Enter name (system or user) of signal head located here."));
        theContentPane->layout()->addWidget(panel25);
        QWidget* panel26 = new QWidget();
        panel26->setLayout(new QHBoxLayout());
        panel26->layout()->addWidget(new QLabel("   "));
        panel26->layout()->addWidget(setA33WayHead);
        setA33WayHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel26->layout()->addWidget(new QLabel("  "));
        panel26->layout()->addWidget(setupA33WayLogic);
        setupA33WayLogic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
        theContentPane->layout()->addWidget(panel26);
        QWidget* panel31 = new QWidget();
        panel31->setLayout(new QHBoxLayout());
        panel31->layout()->addWidget(new QLabel("        "+tr("DivergingA")+" : "));
        panel31->layout()->addWidget(b3WayField);
        b3WayField->setToolTip(tr("Enter name (system or user) of signal head located here."));
        theContentPane->layout()->addWidget(panel31);
        QWidget* panel32 = new QWidget();
        panel32->setLayout(new QHBoxLayout());
        panel32->layout()->addWidget(new QLabel("   "));
        panel32->layout()->addWidget(setB3WayHead);
        setB3WayHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel32->layout()->addWidget(new QLabel("  "));
        panel32->layout()->addWidget(setupB3WayLogic);
        setupB3WayLogic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
        theContentPane->layout()->addWidget(panel32);
        theContentPane->layout()->addWidget(new JSeparator(JSeparator::HORIZONTAL));
        // Signal heads located at turnout B
        QWidget* panel40 = new QWidget();
        panel40->setLayout(new QHBoxLayout());
        panel40->layout()->addWidget(new QLabel(tr("Signal located at ")+" "+tr("Turnout")+" B "));
        theContentPane->layout()->addWidget(panel40);
        QWidget* panel41 = new QWidget();
        panel41->setLayout(new QHBoxLayout());
        panel41->layout()->addWidget(new QLabel("        "+tr("Continuing")+" : "));
        panel41->layout()->addWidget(c3WayField);
        c3WayField->setToolTip(tr("Enter name (system or user) of signal head located here."));
        theContentPane->layout()->addWidget(panel41);
        QWidget* panel42 = new QWidget();
        panel42->setLayout(new QHBoxLayout());
        panel42->layout()->addWidget(new QLabel("   "));
        panel42->layout()->addWidget(setC3WayHead);
        setC3WayHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel42->layout()->addWidget(new QLabel("  "));
        panel42->layout()->addWidget(setupC3WayLogic);
        setupC3WayLogic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
        theContentPane->layout()->addWidget(panel42);
        QWidget* panel43 = new QWidget();
        panel43->setLayout(new QHBoxLayout());
        panel43->layout()->addWidget(new QLabel("        "+tr("DivergingB")+" : "));
        panel43->layout()->addWidget(d3WayField);
        d3WayField->setToolTip(tr("Enter name (system or user) of signal head located here."));
        theContentPane->layout()->addWidget(panel43);
        QWidget* panel44 = new QWidget();
        panel44->setLayout(new QHBoxLayout());
        panel44->layout()->addWidget(new QLabel("   "));
        panel44->layout()->addWidget(setD3WayHead);
        setD3WayHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel44->layout()->addWidget(new QLabel("  "));
        panel44->layout()->addWidget(setupD3WayLogic);
        setupD3WayLogic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
        theContentPane->layout()->addWidget(panel44);
        theContentPane->layout()->addWidget(new JSeparator(JSeparator::HORIZONTAL));
        // buttons
        QWidget* panel6 = new QWidget();
        panel6->setLayout(new QHBoxLayout());
        panel6->layout()->addWidget(change3WaySignalIcon = new QPushButton(tr("Change Signal Head Icon")));
//        change3WaySignalIcon->addActionListener(new ActionListener() {
//                /*public*/ void actionPerformed(ActionEvent e) {
//                    signalFrame->setVisible(true);
//                }
//            });
        connect(change3WaySignalIcon, SIGNAL(clicked(bool)),this, SLOT(On_change3WaySignalIconClicked()));
        change3WaySignalIcon->setToolTip( tr("Change Signal Head IconHint") );
        panel6->layout()->addWidget(new QLabel("  "));
        panel6->layout()->addWidget(set3WaySignalsDone = new QPushButton(tr("Done")));
//        set3WaySignalsDone->addActionListener(new ActionListener() {
//            /*public*/ void actionPerformed(ActionEvent e) {
//                set3WaySignalsDonePressed(e);
//            }
//        });
        connect(set3WaySignalsDone, SIGNAL(clicked(bool)), this, SLOT(set3WaySignalsDonePressed()));
        set3WaySignalsDone->setToolTip( tr("Click Done to perform tasks requested above and dismiss this dialog.") );
        panel6->layout()->addWidget(set3WaySignalsCancel = new QPushButton(tr("Cancel")));
//        set3WaySignalsCancel.addActionListener(new ActionListener() {
//            /*public*/ void actionPerformed(ActionEvent e) {
//                set3WaySignalsCancelPressed(e);
//            }
//        });
        connect(set3WaySignalsCancel, SIGNAL(clicked(bool)), this, SLOT(set3WaySignalsCancelPressed()));
        set3WaySignalsCancel->setToolTip( tr("Click Cancel to dismiss this dialog without making changes.") );
        theContentPane->layout()->addWidget(panel6);
//        setSignalsAt3WayTurnoutFrame->addWindowListener(new java.awt.event.WindowAdapter() {
//            /*public*/ void windowClosing(java.awt.event.WindowEvent e) {
//                set3WaySignalsCancelPressed(nullptr);
//            }
//        });
        setSignalsAt3WayTurnoutFrame->addWindowListener(new S3WWindowListener(this));
    }

    setSignalsAt3WayTurnoutFrame->adjustSize();
    setSignalsAt3WayTurnoutFrame->setVisible(true);
    setSignalsAt3WayOpen = true;
}

void LayoutEditorTools::On_change3WaySignalIconClicked()
{
 signalFrame->setVisible(true);
}
#endif
/*public*/ void LayoutEditorTools::setSignalsAt3WayTurnoutFromMenu(
        /*@Nonnull*/ QString aName, /*@Nonnull*/ QString bName,
        /*@Nonnull*/ MultiIconEditor* theEditor, /*@Nonnull*/ JFrame* theFrame) {
    Turnout* ta = ((ProxyTurnoutManager*)InstanceManager::getDefault("TurnoutManager"))->getTurnout(aName);
    Turnout* tb = ((ProxyTurnoutManager*)InstanceManager::getDefault("TurnoutManager"))->getTurnout(bName);
    turnoutAComboBox->setSelectedItem(ta);
    turnoutBComboBox->setSelectedItem(tb);
    a1_3WaySignalHeadComboBox->setSelectedItem(nullptr);
    a2_3WaySignalHeadComboBox->setSelectedItem(nullptr);
    a3_3WaySignalHeadComboBox->setSelectedItem(nullptr);
    b_3WaySignalHeadComboBox->setSelectedItem(nullptr);
    c_3WaySignalHeadComboBox->setSelectedItem(nullptr);
    d_3WaySignalHeadComboBox->setSelectedItem(nullptr);
    setSignalsAt3WayTurnoutFromMenuFlag = true;
    setSignalsAt3WayTurnout(theEditor, theFrame);
    setSignalsAt3WayTurnoutFromMenuFlag = false;
}

/*public*/ void LayoutEditorTools::setSignalsAt3WayTurnout(/*@Nonnull */MultiIconEditor* theEditor,
        /*@Nonnull*/ JFrame* theFrame) {
    signalIconEditor = theEditor;
    signalFrame = theFrame;

    //Initialize if needed
    if (setSignalsAt3WayTurnoutFrame == nullptr) {
        setSignalsAt3WayTurnoutOpenFlag = false;
        setSignalsAt3WayTurnoutFrame = new JmriJFrameX(tr("Set Signal Heads at 3-Way Turnout"), false, true);
        oneFrameToRuleThemAll(setSignalsAt3WayTurnoutFrame);
        setSignalsAt3WayTurnoutFrame->setDefaultCloseOperation(JFrame::DISPOSE_ON_CLOSE);
        setSignalsAt3WayTurnoutFrame->addHelpMenu("package.jmri.jmrit.display.SetSignalsAt3WayTurnout", true);
        setSignalsAt3WayTurnoutFrame->setLocation(70, 30);
        QWidget* theContentPane = setSignalsAt3WayTurnoutFrame->getContentPane();
        theContentPane->setLayout(new QVBoxLayout());//theContentPane, BoxLayout.Y_AXIS));

        JPanel* panel1A = new JPanel(new FlowLayout());
        turnoutANameLabel = new JLabel(tr("Turnout A Name (closest to throat)"));
        panel1A->layout()->addWidget(turnoutANameLabel);
        panel1A->layout()->addWidget(turnoutAComboBox);
        turnoutAComboBox->setToolTip(tr("SignalsTurnoutNameHint"));
        theContentPane->layout()->addWidget(panel1A);

        JPanel* panel1B = new JPanel(new FlowLayout());
        turnoutBNameLabel = new JLabel(tr("Turnout B Name (farthest from throat)"));
        panel1B->layout()->addWidget(turnoutBNameLabel);
        panel1B->layout()->addWidget(turnoutBComboBox);
        turnoutBComboBox->setToolTip(tr("Enter name (system or user) of turnout where signals are needed."));
        theContentPane->layout()->addWidget(panel1B);
        theContentPane->layout()->addWidget(new JSeparator(JSeparator::HORIZONTAL));
        //Provide for retrieval of names of previously saved signal heads

        JPanel* panel2 = new JPanel(new FlowLayout());
        JLabel* shTitle = new JLabel(tr("SignalHeads"));
        panel2->layout()->addWidget(shTitle);
        panel2->layout()->addWidget(new JLabel("		"));
        panel2->layout()->addWidget(getSaved3WaySignalHeads = new QPushButton(tr("Get Saved")));
        //getSaved3WaySignalHeads.addActionListener((ActionEvent e) -> {
        connect(getSaved3WaySignalHeads, &QPushButton::clicked, [=]{
            getSaved3WaySignals(/*e*/);
        });
        getSaved3WaySignalHeads->setToolTip(tr("GetSavedHint"));
        theContentPane->layout()->addWidget(panel2);

        theContentPane->layout()->addWidget(new JSeparator(JSeparator::HORIZONTAL));
        JPanel* panel2a = new JPanel(new FlowLayout());
        panel2a->layout()->addWidget(new JLabel("   "));
        panel2a->layout()->addWidget(setPlaceAllHeads);
        setPlaceAllHeads->setToolTip(tr("Check to place all signal head icons"));
        //setPlaceAllHeads.addActionListener((ActionEvent e) -> {
        connect(setPlaceAllHeads, &QCheckBox::clicked, [=]{
            bool isSelected = setPlaceAllHeads->isChecked();
            //(de)select all checkboxes
            setA13WayHead->setChecked(isSelected);
            setA23WayHead->setChecked(isSelected);
            setA33WayHead->setChecked(isSelected);
            setB3WayHead->setChecked(isSelected);
            setC3WayHead->setChecked(isSelected);
            setD3WayHead->setChecked(isSelected);
        });
        panel2a->layout()->addWidget(new JLabel("  "));
        panel2a->layout()->addWidget(setupAllLogic);
        setupAllLogic->setToolTip(tr("Check to set up Simple Signal Logic for all Signal Heads"));
        //setupAllLogic.addActionListener((ActionEvent e) -> {
        connect(setupAllLogic, &QCheckBox::clicked, [=]{
            bool isSelected = setupAllLogic->isChecked();
            //(de)select all checkboxes
            setupA13WayLogic->setChecked(isSelected);
            setupA23WayLogic->setChecked(isSelected);
            setupA33WayLogic->setChecked(isSelected);
            setupB3WayLogic->setChecked(isSelected);
            setupC3WayLogic->setChecked(isSelected);
            setupD3WayLogic->setChecked(isSelected);
        });
        theContentPane->layout()->addWidget(panel2a);
        theContentPane->layout()->addWidget(new JSeparator(JSeparator::HORIZONTAL));

        //Signal heads located at turnout A
        JPanel* panel20 = new JPanel(new FlowLayout());
        panel20->layout()->addWidget(new JLabel(tr("Signal located at")
                + " " + tr("Turnout") + " A "));
        theContentPane->layout()->addWidget(panel20);

        JPanel* panel21 = new JPanel(new FlowLayout());
        panel21->layout()->addWidget(new JLabel(
                throatString + " - "
                + continuingString));
        panel21->layout()->addWidget(a1_3WaySignalHeadComboBox);
        a1_3WaySignalHeadComboBox->setToolTip(tr("Enter name (system or user) of signal head located here."));
        theContentPane->layout()->addWidget(panel21);

        JPanel* panel22 = new JPanel(new FlowLayout());
        panel22->layout()->addWidget(new JLabel("   "));
        panel22->layout()->addWidget(setA13WayHead);
        setA13WayHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel22->layout()->addWidget(new JLabel("  "));
        panel22->layout()->addWidget(setupA13WayLogic);
        setupA13WayLogic->setToolTip(tr("Check to set up Simple Signal Logic for the above signal head."));
        theContentPane->layout()->addWidget(panel22);

        JPanel* panel23 = new JPanel(new FlowLayout());
        panel23->layout()->addWidget(new JLabel(
                throatString + " - "
                + divergingAString));
        panel23->layout()->addWidget(a2_3WaySignalHeadComboBox);
        a2_3WaySignalHeadComboBox->setToolTip(tr("Enter name (system or user) of signal head located here."));
        theContentPane->layout()->addWidget(panel23);

        JPanel* panel24 = new JPanel(new FlowLayout());
        panel24->layout()->addWidget(new JLabel("   "));
        panel24->layout()->addWidget(setA23WayHead);
        setA23WayHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel24->layout()->addWidget(new JLabel("  "));
        panel24->layout()->addWidget(setupA23WayLogic);
        setupA23WayLogic->setToolTip(tr("Check to set up Simple Signal Logic for the above signal head."));
        theContentPane->layout()->addWidget(panel24);

        JPanel* panel25 = new JPanel(new FlowLayout());
        panel25->layout()->addWidget(new JLabel(
                throatString + " - "
                + divergingBString));
        panel25->layout()->addWidget(a3_3WaySignalHeadComboBox);
        a3_3WaySignalHeadComboBox->setToolTip(tr("Enter name (system or user) of signal head located here."));
        theContentPane->layout()->addWidget(panel25);

        JPanel* panel26 = new JPanel(new FlowLayout());
        panel26->layout()->addWidget(new JLabel("   "));
        panel26->layout()->addWidget(setA33WayHead);
        setA33WayHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel26->layout()->addWidget(new JLabel("  "));
        panel26->layout()->addWidget(setupA33WayLogic);
        setupA33WayLogic->setToolTip(tr("Check to set up Simple Signal Logic for the above signal head."));
        theContentPane->layout()->addWidget(panel26);

        JPanel* panel31 = new JPanel(new FlowLayout());
        panel31->layout()->addWidget(new JLabel(
                divergingBString));
        panel31->layout()->addWidget(b_3WaySignalHeadComboBox);
        b_3WaySignalHeadComboBox->setToolTip(tr("Enter name (system or user) of signal head located here."));
        theContentPane->layout()->addWidget(panel31);

        JPanel* panel32 = new JPanel(new FlowLayout());
        panel32->layout()->addWidget(new JLabel("   "));
        panel32->layout()->addWidget(setB3WayHead);
        setB3WayHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel32->layout()->addWidget(new JLabel("  "));
        panel32->layout()->addWidget(setupB3WayLogic);
        setupB3WayLogic->setToolTip(tr("Check to set up Simple Signal Logic for the above signal head."));
        theContentPane->layout()->addWidget(panel32);
        theContentPane->layout()->addWidget(new JSeparator(JSeparator::HORIZONTAL));
        //Signal heads located at turnout B

        JPanel* panel40 = new JPanel(new FlowLayout());
        panel40->layout()->addWidget(new JLabel(tr("Signal located at")
                + " " + tr("BeanNameTurnout") + " B "));
        theContentPane->layout()->addWidget(panel40);

        JPanel* panel41 = new JPanel(new FlowLayout());
        panel41->layout()->addWidget(new JLabel(
                continuingString));
        panel41->layout()->addWidget(c_3WaySignalHeadComboBox);
        c_3WaySignalHeadComboBox->setToolTip(tr("Enter name (system or user) of signal head located here."));
        theContentPane->layout()->addWidget(panel41);

        JPanel* panel42 = new JPanel(new FlowLayout());
        panel42->layout()->addWidget(new JLabel("   "));
        panel42->layout()->addWidget(setC3WayHead);
        setC3WayHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel42->layout()->addWidget(new JLabel("  "));
        panel42->layout()->addWidget(setupC3WayLogic);
        setupC3WayLogic->setToolTip(tr("Check to set up Simple Signal Logic for the above signal head."));
        theContentPane->layout()->addWidget(panel42);

        JPanel* panel43 = new JPanel(new FlowLayout());
        panel43->layout()->addWidget(new JLabel(
                divergingBString));
        panel43->layout()->addWidget(d_3WaySignalHeadComboBox);
        d_3WaySignalHeadComboBox->setToolTip(tr("Enter name (system or user) of signal head located here."));
        theContentPane->layout()->addWidget(panel43);

        JPanel* panel44 = new JPanel(new FlowLayout());
        panel44->layout()->addWidget(new JLabel("   "));
        panel44->layout()->addWidget(setD3WayHead);
        setD3WayHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
        panel44->layout()->addWidget(new JLabel("  "));
        panel44->layout()->addWidget(setupD3WayLogic);
        setupD3WayLogic->setToolTip(tr("Check to set up Simple Signal Logic for the above signal head."));
        theContentPane->layout()->addWidget(panel44);
        theContentPane->layout()->addWidget(new JSeparator(JSeparator::HORIZONTAL));
        //buttons

        JPanel* panel6 = new JPanel(new FlowLayout());
        panel6->layout()->addWidget(change3WaySignalIcon = new QPushButton(tr("Change Signal Head Icon")));
        //change3WaySignalIcon.addActionListener((ActionEvent e) -> {
        connect(change3WaySignalIcon, &QPushButton::clicked, [=]{
            signalFrame->setVisible(true);
        });
        change3WaySignalIcon->setToolTip(tr("ChangeSignalIconHint"));
        panel6->layout()->addWidget(new JLabel("	 "));
        panel6->layout()->addWidget(set3WaySignalsDone = new QPushButton(tr("Done")));
        //set3WaySignalsDone.addActionListener((ActionEvent e) -> {
        connect(set3WaySignalsDone, &QPushButton::clicked, [=]{
            set3WaySignalsDonePressed(/*e*/);
        });
        set3WaySignalsDone->setToolTip(tr("Click [%1] to accept any changes made above and close this dialog.").arg(tr("Done")));

        //make this button the default button (return or enter activates)
        //Note: We have to invoke this later because we don't currently have a root pane
//        SwingUtilities.invokeLater(() -> {
//            JRootPane rootPane = SwingUtilities.getRootPane(set3WaySignalsDone);
//            rootPane.setDefaultButton(set3WaySignalsDone);
        set3WaySignalsDone->setDefault(true);
//        });

        panel6->layout()->addWidget(set3WaySignalsCancel = new QPushButton(tr("Cancel")));
        //set3WaySignalsCancel.addActionListener((ActionEvent e) -> {
        connect(set3WaySignalsCancel, &QPushButton::clicked, [=]{
            set3WaySignalsCancelPressed(/*e*/);
        });
        set3WaySignalsCancel->setToolTip(tr("Click [%1] to dismiss this dialog without making changes.").arg(tr("Cancel")));
        theContentPane->layout()->addWidget(panel6);
#if 0
        setSignalsAt3WayTurnoutFrame.addWindowListener(new WindowAdapter() {
            @Override
            public void windowClosing(WindowEvent e) {
                set3WaySignalsCancelPressed(null);
            }
        });
#endif
    }
    setPlaceAllHeads->setChecked(false);
    setupAllLogic->setChecked(false);

    turnoutAComboBox->setVisible(!setSignalsAt3WayTurnoutFromMenuFlag);
    turnoutBComboBox->setVisible(!setSignalsAt3WayTurnoutFromMenuFlag);
    if (setSignalsAt3WayTurnoutFromMenuFlag) {
        turnoutANameLabel->setText(tr("Turnout") + " A"
                + turnoutAComboBox->getSelectedItemDisplayName());
        turnoutBNameLabel->setText(
                tr("Turnout") + " B"
                + turnoutBComboBox->getSelectedItemDisplayName());
        getSaved3WaySignals(/*null*/);
    } else {
        turnoutANameLabel->setText(
                tr("Turnout A Name (closest to throat)"));
        turnoutBNameLabel->setText(
                tr("Turnout B Name (farthest from throat)"));
    }

    if (!setSignalsAt3WayTurnoutOpenFlag) {
        setSignalsAt3WayTurnoutFrame->resize(QSize());
        setSignalsAt3WayTurnoutFrame->pack();
        setSignalsAt3WayTurnoutOpenFlag = true;
    }
    setSignalsAt3WayTurnoutFrame->setVisible(true);
}   //setSignalsAt3WayTurnout

/*private*/ void LayoutEditorTools::getSaved3WaySignals (ActionEvent* /*a*/) {
    if ( !get3WayTurnoutInformation() ) return;
    a13WayField->setText(layoutTurnoutA->getSignalA1Name());
    a23WayField->setText(layoutTurnoutA->getSignalA2Name());
    a33WayField->setText(layoutTurnoutA->getSignalA3Name());
    b3WayField->setText(layoutTurnoutA->getSignalC1Name());
    c3WayField->setText(layoutTurnoutB->getSignalB1Name());
    d3WayField->setText(layoutTurnoutB->getSignalC1Name());
}
/*private*/ void LayoutEditorTools::set3WaySignalsCancelPressed (ActionEvent* /*a*/) {
    setSignalsAt3WayOpen = false;
    setSignalsAt3WayTurnoutFrame->setVisible(false);
}

/*private*/ bool LayoutEditorTools::get3WayTurnoutInformation() {
    int type = 0;
    QObject* connect = NULL;
    QString str = "";
    turnoutA = NULL;
    turnoutB = NULL;
    layoutTurnoutA = NULL;
    layoutTurnoutB = NULL;
    str = turnoutANameField->text().trimmed();
    if ( (str==nullptr) || (str==("")) ) {
        // turnout A not entered, test turnout B
        str = turnoutBNameField->text().trimmed();
        if ( (str==nullptr) || (str==("")) ) {
            // no entries in turnout fields
//            JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,tr("SignalsError1"),
//                                tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAt3WayTurnoutFrame,tr("Error"), tr("Error - No turnout name was entered. Please enter a turnout name or cancel.") );
            return false;
        }
        turnoutB = ((ProxyTurnoutManager*)InstanceManager::turnoutManagerInstance())->getTurnout(str);
        if (turnoutB==nullptr) {
//            JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                java.text.MessageFormat.format(tr("SignalsError2"),
//                    new Object[]{str}), tr("Error"),
//                        JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAt3WayTurnoutFrame,tr("Error"), tr("Error - No turnout is defined for \"%1\". Please enter\na turnout name in the Turnout Table and on the panel.").arg(str) );
            return false ;
        }
        if ( (turnoutB->getUserName()==nullptr) || (turnoutB->getUserName()==("")) ||
                                turnoutB->getUserName()!=(str) ) {
//            str = str.toUpper();
            turnoutBNameField->setText(str);
        }
        layoutTurnoutB = getLayoutTurnoutFromTurnout(turnoutB,false,str,setSignalsAt3WayTurnoutFrame);
        if (layoutTurnoutB==nullptr)
            return false;
        // have turnout B and layout turnout B - look for turnout A
        connectorTrack = (TrackSegment*)layoutTurnoutB->getConnectA();
        if (connectorTrack == nullptr) {
            // Inform user of error, and terminate
//            JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                    tr("SignalsError19"),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAt3WayTurnoutFrame, tr("Error"), tr("Error - This tool requires two turnouts (RH or LH) connected,\nas specified, by a short track segment. Cannot find these."));
            return false;
        }
        type = connectorTrack->getType1();
        connect = connectorTrack->getConnect1();
        if (connect == layoutTurnoutB) {
            type = connectorTrack->getType2();
            connect = connectorTrack->getConnect2();
        }
        if ( (type != LayoutEditor::TURNOUT_B) || (connect==nullptr) ) {
            // Not two turnouts connected as required by a single Track Segment
            // Inform user of error and terminate
//            JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                    tr("SignalsError19"),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAt3WayTurnoutFrame, tr("Error"), tr("Error - This tool requires two turnouts (RH or LH) connected,\nas specified, by a short track segment. Cannot find these."));
            return false;
        }
        layoutTurnoutA = (LayoutTurnout*)connect;
        turnoutA = layoutTurnoutA->getTurnout();
        if (turnoutA==nullptr) {
//            JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                    tr("SignalsError19"),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAt3WayTurnoutFrame, tr("Error"), tr("Error - This tool requires two turnouts (RH or LH) connected,\nas specified, by a short track segment. Cannot find these."));
            return false;
        }
        turnoutANameField->setText(layoutTurnoutA->getTurnoutName());
    }
    else {
        // something was entered in the turnout A field
        turnoutA = ((ProxyTurnoutManager*)InstanceManager::turnoutManagerInstance())->getTurnout(str);
        if (turnoutA==nullptr) {
//            JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                java.text.MessageFormat.format(tr("SignalsError2"),
//                    new Object[]{str}), tr("Error"),
//                        JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAt3WayTurnoutFrame,tr("Error"), tr("Error - No turnout is defined for \"%1\". Please enter\na turnout name in the Turnout Table and on the panel.").arg(str) );
            return false ;
        }
        if ( (turnoutA->getUserName()==nullptr) || (turnoutA->getUserName()==("")) ||
                                turnoutA->getUserName()!=(str) ) {
//            str = str.toUpper();
            turnoutANameField->setText(str);
        }
        // have turnout A - get corresponding layoutTurnout
        layoutTurnoutA = getLayoutTurnoutFromTurnout(turnoutA,false,str,setSignalsAt3WayTurnoutFrame);
        if (layoutTurnoutA == nullptr)
            return false;
        turnoutANameField->setText(str);
        // have turnout A and layout turnout A - was something entered for turnout B
        str = turnoutBNameField->text().trimmed();
        if ( (str==nullptr) || (str==("")) ) {
            // no entry for turnout B
            connectorTrack = (TrackSegment*)layoutTurnoutA->getConnectB();
            if (connectorTrack == nullptr) {
                // Inform user of error, and terminate
//                JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                        tr("SignalsError19"),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
                QMessageBox::critical(setSignalsAt3WayTurnoutFrame, tr("Error"), tr("Error - This tool requires two turnouts (RH or LH) connected,\nas specified, by a short track segment. Cannot find these."));
                return false;
            }
            type = connectorTrack->getType1();
            connect = connectorTrack->getConnect1();
            if (connect == layoutTurnoutA) {
                type = connectorTrack->getType2();
                connect = connectorTrack->getConnect2();
            }
            if ( (type != LayoutEditor::TURNOUT_A) || (connect==nullptr) ) {
                // Not two turnouts connected with the throat of B connected to the continuing of A
                //    by a single Track Segment.  Inform user of error and terminat.e
//                JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                        tr("SignalsError19"),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
             QMessageBox::critical(setSignalsAt3WayTurnoutFrame, tr("Error"), tr("Error - This tool requires two turnouts (RH or LH) connected,\nas specified, by a short track segment. Cannot find these."));
                return false;
            }
            layoutTurnoutB = (LayoutTurnout*)connect;
            turnoutB = layoutTurnoutB->getTurnout();
            if (turnoutB==nullptr) {
//                JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                        tr("SignalsError19"),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
             QMessageBox::critical(setSignalsAt3WayTurnoutFrame, tr("Error"), tr("Error - This tool requires two turnouts (RH or LH) connected,\nas specified, by a short track segment. Cannot find these."));
                return false;
            }
            turnoutBNameField->setText(layoutTurnoutB->getTurnoutName());
        }
        else {
            // turnout B entered also
            turnoutB = ((ProxyTurnoutManager*)InstanceManager::turnoutManagerInstance())->getTurnout(str);
            if (turnoutB==nullptr) {
//                JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                    java.text.MessageFormat.format(tr("SignalsError2"),
//                            new Object[]{str}), tr("Error"),
//                                JOptionPane.ERROR_MESSAGE);
             QMessageBox::critical(setSignalsAt3WayTurnoutFrame, tr("Error"), tr("Error - No turnout is defined for \"%1\". Please enter\na turnout name in the Turnout Table and on the panel.").arg(str));
                return false ;
            }
            if ( (turnoutB->getUserName()==nullptr) || (turnoutB->getUserName()==("")) ||
                                turnoutB->getUserName()!=(str) ) {
//                str = str.toUpper();
                turnoutBNameField->setText(str);
            }
            layoutTurnoutB = getLayoutTurnoutFromTurnout(turnoutB,false,str,setSignalsAt3WayTurnoutFrame);
            if (layoutTurnoutB==nullptr)
                return false;
            turnoutBNameField->setText(str);
            // check that layout turnout A and layout turnout B are connected as required
            if (layoutTurnoutA->getConnectB()!=layoutTurnoutB->getConnectA()) {
                // Not two turnouts connected as required by a single Track Segment
                // Inform user of error and terminate
             QMessageBox::critical(setSignalsAt3WayTurnoutFrame, tr("Error"), tr("Error - This tool requires two turnouts (RH or LH) connected,\nas specified, by a short track segment. Cannot find these."));
                return false;
            }
            connectorTrack = (TrackSegment*)layoutTurnoutA->getConnectB();
        }
    }
    // have both turnouts, correctly connected - complete initialization
    layoutTurnoutAHorizontal = false;
    layoutTurnoutAVertical = false;
    layoutTurnoutBThroatLeft = false;
    layoutTurnoutBVertical = false;
    layoutTurnoutAThroatLeft = false;
    layoutTurnoutAThroatUp = false;
    layoutTurnoutBThroatLeft = false;
    layoutTurnoutBThroatUp = false;
    layoutTurnoutABUp = false;
    layoutTurnoutABLeft = false;
    layoutTurnoutBBUp = false;
    layoutTurnoutBBLeft = false;
    double delX = layoutTurnoutA->getCoordsA().x() - layoutTurnoutA->getCoordsB().x();
    double delY = layoutTurnoutA->getCoordsA().y() - layoutTurnoutA->getCoordsB().y();
    if (qAbs(delX) > 2.0*qAbs(delY)) {
        layoutTurnoutAHorizontal = true;
        if (delX < 0.0) layoutTurnoutAThroatLeft = true;
        if (layoutTurnoutA->getCoordsB().y() < layoutTurnoutA->getCoordsC().y())
            layoutTurnoutABUp = true;
    }
    else if (qAbs(delY) > 2.0*qAbs(delX)) {
        layoutTurnoutAVertical = true;
        if (delY <0.0) layoutTurnoutAThroatUp = true;
        if (layoutTurnoutA->getCoordsB().x() < layoutTurnoutA->getCoordsC().x())
            layoutTurnoutABLeft = true;
    }
    delX = layoutTurnoutB->getCoordsA().x() - layoutTurnoutB->getCoordsB().x();
    delY = layoutTurnoutB->getCoordsA().y() - layoutTurnoutB->getCoordsB().y();
    if (qAbs(delX) > 2.0*qAbs(delY)) {
        layoutTurnoutBHorizontal = true;
        if (delX < 0.0) layoutTurnoutBThroatLeft = true;
        if (layoutTurnoutB->getCoordsB().y() < layoutTurnoutB->getCoordsC().y())
            layoutTurnoutBBUp = true;
    }
    else if (qAbs(delY) > 2.0*qAbs(delX)) {
        layoutTurnoutBVertical = true;
        if (delY <0.0) layoutTurnoutBThroatUp = true;
        if (layoutTurnoutB->getCoordsB().x() < layoutTurnoutB->getCoordsC().x())
            layoutTurnoutBBLeft = true;
    }
    return true;
}

/*private*/ void LayoutEditorTools::set3WaySignalsDonePressed (ActionEvent* /*a*/) {
    // process turnout names
    if ( !get3WayTurnoutInformation() ) return;
    // process signal head names
    if ( !get3WaySignalHeadInformation() ) return;
    // place signals as requested at turnout A
    if (setA13WayHead->isChecked()) {
        if (isHeadOnPanel(a13WayHead) &&
            (a13WayHead!=getHeadFromName(layoutTurnoutA->getSignalA1Name()))) {
//            JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{a13WayField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\n because it is already on the panel.").arg(a13WayField->text().trimmed()));
            return;
        }
        else if ( (!layoutTurnoutAHorizontal) && (!layoutTurnoutAVertical) ) {
//            JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                tr("InfoMessage2"),"",JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(0, tr("Information"), tr("Sorry, cannot place signal heads at turnouts\n                                                   that are not oriented vertical or horizontal."));

            if (a13WayHead!=getHeadFromName(layoutTurnoutA->getSignalA1Name())) {
                removeSignalHeadFromPanel(layoutTurnoutA->getSignalA1Name());
                removeAssignment(a13WayHead);
                layoutTurnoutA->setSignalA1Name(a13WayField->text().trimmed());
            }
        }
        else {
            removeSignalHeadFromPanel(layoutTurnoutA->getSignalA1Name());
            place3WayThroatContinuing();
            removeAssignment(a13WayHead);
            layoutTurnoutA->setSignalA1Name(a13WayField->text().trimmed());
            needRedraw = true;
        }
    }
    else {
        int assigned = isHeadAssignedHere(a13WayHead,layoutTurnoutA);
        if (assigned == NONE) {
            if ( isHeadOnPanel(a13WayHead) &&
                                isHeadAssignedAnywhere(a13WayHead) ) {
//                JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{a13WayField->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
             QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\nbecause it is already on the panel at a different place.").arg(a13WayField->text().trimmed()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutTurnoutA->getSignalA1Name());
                removeAssignment(a13WayHead);
                layoutTurnoutA->setSignalA1Name(a13WayField->text().trimmed());
            }
        }
        else if (assigned!=A1) {
// need to figure out what to do in this case.
        }
    }
    if ( (setA23WayHead->isChecked()) && (a23WayHead!=nullptr) ) {
        if (isHeadOnPanel(a23WayHead) &&
            (a23WayHead!=getHeadFromName(layoutTurnoutA->getSignalA2Name()))) {
//            JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{a23WayField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\n because it is already on the panel.").arg(a23WayField->text().trimmed()));
            return;
        }
        else if ( (!layoutTurnoutAHorizontal) && (!layoutTurnoutAVertical) ) {
//            JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                tr("InfoMessage2"),"",JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(0, tr("Information"), tr("Sorry, cannot place signal heads at turnouts\n                                                   that are not oriented vertical or horizontal."));

            if (a23WayHead!=getHeadFromName(layoutTurnoutA->getSignalA2Name())) {
                removeSignalHeadFromPanel(layoutTurnoutA->getSignalA2Name());
                removeAssignment(a23WayHead);
                layoutTurnoutA->setSignalA2Name(a23WayField->text().trimmed());
            }
        }
        else {
            removeSignalHeadFromPanel(layoutTurnoutA->getSignalA2Name());
            place3WayThroatDivergingA();
            removeAssignment(a23WayHead);
            layoutTurnoutA->setSignalA2Name(a23WayField->text().trimmed());
            needRedraw = true;
        }
    }
    else if (a23WayHead!=nullptr) {
        int assigned = isHeadAssignedHere(a23WayHead,layoutTurnoutA);
        if (assigned == NONE) {
            if ( isHeadOnPanel(a23WayHead) &&
                                isHeadAssignedAnywhere(a23WayHead) ) {
//                JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{a23WayField->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
             QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\nbecause it is already on the panel at a different place.").arg(a23WayField->text().trimmed()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutTurnoutA->getSignalA2Name());
                removeAssignment(a23WayHead);
                layoutTurnoutA->setSignalA2Name(a23WayField->text().trimmed());
            }
        }
        else if (assigned!=A2) {
// need to figure out what to do in this case.
        }
    }
    else if (a23WayHead==nullptr) {
        removeSignalHeadFromPanel(layoutTurnoutA->getSignalA2Name());
        layoutTurnoutA->setSignalA2Name("");
    }
    if ( (setA33WayHead->isChecked()) && (a33WayHead!=nullptr) ) {
        if (isHeadOnPanel(a33WayHead) &&
            (a33WayHead!=getHeadFromName(layoutTurnoutA->getSignalA3Name()))) {
//            JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{a33WayField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\n because it is already on the panel.").arg(a33WayField->text().trimmed()));
            return;
        }
        else if ( (!layoutTurnoutAHorizontal) && (!layoutTurnoutAVertical) ) {
//            JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                tr("InfoMessage2"),"",JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(0, tr("Information"), tr("Sorry, cannot place signal heads at turnouts\n                                                   that are not oriented vertical or horizontal."));

            if (a33WayHead!=getHeadFromName(layoutTurnoutA->getSignalA3Name())) {
                removeSignalHeadFromPanel(layoutTurnoutA->getSignalA3Name());
                removeAssignment(a33WayHead);
                layoutTurnoutA->setSignalA3Name(a33WayField->text().trimmed());
            }
        }
        else {
            removeSignalHeadFromPanel(layoutTurnoutA->getSignalA3Name());
            place3WayThroatDivergingB();
            removeAssignment(a33WayHead);
            layoutTurnoutA->setSignalA3Name(a33WayField->text().trimmed());
            needRedraw = true;
        }
    }
    else if (a33WayHead!=nullptr) {
        int assigned = isHeadAssignedHere(a33WayHead,layoutTurnoutA);
        if (assigned == NONE) {
            if ( isHeadOnPanel(a33WayHead) &&
                                isHeadAssignedAnywhere(a33WayHead) ) {
//                JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{a33WayField->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
             QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\nbecause it is already on the panel at a different place.").arg(a33WayField->text().trimmed()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutTurnoutA->getSignalA3Name());
                removeAssignment(a33WayHead);
                layoutTurnoutA->setSignalA3Name(a33WayField->text().trimmed());
            }
        }
        else if (assigned!=A3) {
// need to figure out what to do in this case.
        }
    }
    else if (a33WayHead==nullptr) {
        removeSignalHeadFromPanel(layoutTurnoutA->getSignalA3Name());
        layoutTurnoutA->setSignalA3Name("");
    }
    if (setB3WayHead->isChecked()) {
        if (isHeadOnPanel(b3WayHead) &&
            (b3WayHead!=getHeadFromName(layoutTurnoutA->getSignalC1Name()))) {
//            JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{b3WayField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\n because it is already on the panel.").arg(b3WayField->text().trimmed()));
            return;
        }
        else if ( (!layoutTurnoutAHorizontal) && (!layoutTurnoutAVertical) ) {
//            JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                tr("InfoMessage2"),"",JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(0, tr("Information"), tr("Sorry, cannot place signal heads at turnouts\n                                                   that are not oriented vertical or horizontal."));

            if (b3WayHead!=getHeadFromName(layoutTurnoutA->getSignalB1Name())) {
                removeSignalHeadFromPanel(layoutTurnoutA->getSignalC1Name());
                removeAssignment(b3WayHead);
                layoutTurnoutA->setSignalC1Name(b3WayField->text().trimmed());
            }
        }
        else {
            removeSignalHeadFromPanel(layoutTurnoutA->getSignalC1Name());
            place3WayDivergingA();
            removeAssignment(b3WayHead);
            layoutTurnoutA->setSignalC1Name(b3WayField->text().trimmed());
            needRedraw = true;
        }
    }
    else {
        int assigned = isHeadAssignedHere(b3WayHead,layoutTurnoutA);
        if (assigned == NONE) {
            if ( isHeadOnPanel(b3WayHead) &&
                                isHeadAssignedAnywhere(b3WayHead) ) {
//                JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{b3WayField->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
             QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\nbecause it is already on the panel at a different place.").arg(b3WayField->text().trimmed()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutTurnoutA->getSignalC1Name());
                removeAssignment(b3WayHead);
                layoutTurnoutA->setSignalC1Name(b3WayField->text().trimmed());
            }
        }
        else if (assigned!=A1) {
// need to figure out what to do in this case.
        }
    }
    // place signals as requested at Turnout B
    if (setC3WayHead->isChecked()) {
        if (isHeadOnPanel(c3WayHead) &&
            (c3WayHead!=getHeadFromName(layoutTurnoutB->getSignalB1Name()))) {
//            JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{c3WayField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\n because it is already on the panel.").arg(c3WayField->text().trimmed()));
            return;
        }
        else if ( (!layoutTurnoutBHorizontal) && (!layoutTurnoutBVertical) ) {
//            JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                tr("InfoMessage2"),"",JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(0, tr("Information"), tr("Sorry, cannot place signal heads at turnouts\n                                                   that are not oriented vertical or horizontal."));

            if (c3WayHead!=getHeadFromName(layoutTurnoutB->getSignalB1Name())) {
                removeSignalHeadFromPanel(layoutTurnoutB->getSignalB1Name());
                removeAssignment(c3WayHead);
                layoutTurnoutB->setSignalB1Name(c3WayField->text().trimmed());
            }
        }
        else {
            removeSignalHeadFromPanel(layoutTurnoutB->getSignalB1Name());
            place3WayContinuing();
            removeAssignment(c3WayHead);
            layoutTurnoutB->setSignalB1Name(c3WayField->text().trimmed());
            needRedraw = true;
        }
    }
    else {
        int assigned = isHeadAssignedHere(c3WayHead,layoutTurnoutB);
        if (assigned == NONE) {
            if (isHeadOnPanel(c3WayHead)  &&
                                isHeadAssignedAnywhere(c3WayHead) ) {
//                JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{c3WayField->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
             QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\nbecause it is already on the panel at a different place.").arg(c3WayField->text().trimmed()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutTurnoutB->getSignalB1Name());
                removeAssignment(c3WayHead);
                layoutTurnoutB->setSignalB1Name(c3WayField->text().trimmed());
            }
        }
        else if (assigned!=B1) {
// need to figure out what to do in this case.
        }
    }
    if (setD3WayHead->isChecked()) {
        if (isHeadOnPanel(d3WayHead) &&
            (d3WayHead!=getHeadFromName(layoutTurnoutB->getSignalC1Name()))) {
//            JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{d3WayField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\n because it is already on the panel.").arg(d3WayField->text().trimmed()));
            return;
        }
        else if ( (!layoutTurnoutBHorizontal) && (!layoutTurnoutBVertical) ) {
//            JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                tr("InfoMessage2"),"",JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(0, tr("Information"), tr("Sorry, cannot place signal heads at turnouts\n                                                   that are not oriented vertical or horizontal."));

            if (d3WayHead!=getHeadFromName(layoutTurnoutB->getSignalC1Name())) {
                removeSignalHeadFromPanel(layoutTurnoutB->getSignalC1Name());
                removeAssignment(d3WayHead);
                layoutTurnoutB->setSignalC1Name(d3WayField->text().trimmed());
            }
        }
        else {
            removeSignalHeadFromPanel(layoutTurnoutB->getSignalC1Name());
            place3WayDivergingB();
            removeAssignment(d3WayHead);
            layoutTurnoutB->setSignalC1Name(d3WayField->text().trimmed());
            needRedraw = true;
        }
    }
    else {
        int assigned = isHeadAssignedHere(d3WayHead,layoutTurnoutB);
        if (assigned == NONE) {
            if (isHeadOnPanel(d3WayHead) &&
                                isHeadAssignedAnywhere(d3WayHead) ) {
//                JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{d3WayField->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
             QMessageBox::critical(setSignalsAtTToTFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\nbecause it is already on the panel at a different place.").arg(d3WayField->text().trimmed()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutTurnoutB->getSignalC1Name());
                removeAssignment(d3WayHead);
                layoutTurnoutB->setSignalC1Name(d3WayField->text().trimmed());
            }
        }
        else if (assigned!=C1) {
// need to figure out what to do in this case.
        }
    }
    // setup Logic if requested and enough information is available
    if (setupA13WayLogic->isChecked()) {
        set3WayLogicThroatContinuing();
    }
    if ( (a23WayHead!=nullptr) && setupA23WayLogic->isChecked() ) {
        set3WayLogicThroatDivergingA();
    }
    if ( (a33WayHead!=nullptr) && setupA33WayLogic->isChecked() ) {
        set3WayLogicThroatDivergingB();
    }
    if (setupB3WayLogic->isChecked()) {
        set3WayLogicDivergingA();
    }
    if (setupC3WayLogic->isChecked()) {
        set3WayLogicContinuing();
    }
    if (setupD3WayLogic->isChecked()) {
        set3WayLogicDivergingB();
    }
    // link the two turnouts
    layoutTurnoutA->setLinkedTurnoutName(turnoutBNameField->text().trimmed());
    layoutTurnoutA->setLinkType(LayoutTurnout::FIRST_3_WAY);
    layoutTurnoutB->setLinkedTurnoutName(turnoutANameField->text().trimmed());
    layoutTurnoutB->setLinkType(LayoutTurnout::SECOND_3_WAY);
    // finish up
    setSignalsAt3WayOpen = false;
    setSignalsAt3WayTurnoutFrame->setVisible(false);
    if (needRedraw) {
        layoutEditor->redrawPanel();
        needRedraw = false;
        layoutEditor->setDirty();
    }
}

/*private*/ bool LayoutEditorTools::get3WaySignalHeadInformation() {
    a13WayHead = getSignalHeadFromEntry(a13WayField,true,setSignalsAt3WayTurnoutFrame);
    if (a13WayHead==nullptr) return false;
    a23WayHead = getSignalHeadFromEntry(a23WayField,false,setSignalsAt3WayTurnoutFrame);
    a33WayHead = getSignalHeadFromEntry(a33WayField,false,setSignalsAt3WayTurnoutFrame);
    if ( ((a23WayHead==nullptr) && (a33WayHead!=nullptr)) || ((a33WayHead==nullptr) &&
                    (a23WayHead!=nullptr)) ) return false;
    b3WayHead = getSignalHeadFromEntry(b3WayField,true,setSignalsAt3WayTurnoutFrame);
    if (b3WayHead==nullptr) return false;
    c3WayHead = getSignalHeadFromEntry(c3WayField,true,setSignalsAt3WayTurnoutFrame);
    if (c3WayHead==nullptr) return false;
    d3WayHead = getSignalHeadFromEntry(d3WayField,true,setSignalsAt3WayTurnoutFrame);
    if (d3WayHead==nullptr) return false;
    return true;
}
/*private*/ void LayoutEditorTools::place3WayThroatContinuing() {
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    if( layoutTurnoutAHorizontal && layoutTurnoutAThroatLeft ) {
        setSignalHeadOnPanel(2,a13WayField->text().trimmed(),
            (int)(layoutTurnoutA->getCoordsA().x()-testIcon->getIconWidth()),
            (int)(layoutTurnoutA->getCoordsA().y()+4) );
    }
    else if( layoutTurnoutAHorizontal && (!layoutTurnoutAThroatLeft) ) {
        setSignalHeadOnPanel(0,a13WayField->text().trimmed(),
            (int)(layoutTurnoutA->getCoordsA().x()),
            (int)(layoutTurnoutA->getCoordsA().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnoutAVertical && layoutTurnoutAThroatUp ) {
        setSignalHeadOnPanel(1,a13WayField->text().trimmed(),
            (int)(layoutTurnoutA->getCoordsA().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnoutA->getCoordsA().y()-testIcon->getIconHeight()) );
    }
    else if( layoutTurnoutAVertical && (!layoutTurnoutAThroatUp) ) {
        setSignalHeadOnPanel(3,a13WayField->text().trimmed(),
            (int)(layoutTurnoutA->getCoordsA().x()+4),
            (int)(layoutTurnoutA->getCoordsA().y()) );
    }
}

/*private*/ void LayoutEditorTools::place3WayThroatDivergingA() {
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    if( layoutTurnoutAHorizontal && layoutTurnoutAThroatLeft ) {
        setSignalHeadOnPanel(2,a23WayField->text().trimmed(),
            (int)(layoutTurnoutA->getCoordsA().x()-(2*testIcon->getIconWidth())),
            (int)(layoutTurnoutA->getCoordsA().y()+4) );
    }
    else if( layoutTurnoutAHorizontal && (!layoutTurnoutAThroatLeft) ) {
        setSignalHeadOnPanel(0,a23WayField->text().trimmed(),
            (int)(layoutTurnoutA->getCoordsA().x()+4+testIcon->getIconWidth()),
            (int)(layoutTurnoutA->getCoordsA().y()-4-testIcon->getIconHeight()));
    }
    else if( layoutTurnoutAVertical && layoutTurnoutAThroatUp ) {
        setSignalHeadOnPanel(1,a23WayField->text().trimmed(),
            (int)(layoutTurnoutA->getCoordsA().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnoutA->getCoordsA().y()-4-(2*testIcon->getIconHeight())));
    }
    else if( layoutTurnoutAVertical && (!layoutTurnoutAThroatUp) ) {
        setSignalHeadOnPanel(3,a23WayField->text().trimmed(),
            (int)(layoutTurnoutA->getCoordsA().x()+4),
            (int)(layoutTurnoutA->getCoordsA().y()+4+testIcon->getIconHeight()));
    }
}
/*private*/ void LayoutEditorTools::place3WayThroatDivergingB() {
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    if( layoutTurnoutAHorizontal && layoutTurnoutAThroatLeft ) {
        setSignalHeadOnPanel(2,a33WayField->text().trimmed(),
            (int)(layoutTurnoutA->getCoordsA().x()-(3*testIcon->getIconWidth())),
            (int)(layoutTurnoutA->getCoordsA().y()+4) );
    }
    else if( layoutTurnoutAHorizontal && (!layoutTurnoutAThroatLeft) ) {
        setSignalHeadOnPanel(0,a33WayField->text().trimmed(),
            (int)(layoutTurnoutA->getCoordsA().x()+8+(2*testIcon->getIconWidth())),
            (int)(layoutTurnoutA->getCoordsA().y()-4-testIcon->getIconHeight()));
    }
    else if( layoutTurnoutAVertical && layoutTurnoutAThroatUp ) {
        setSignalHeadOnPanel(1,a33WayField->text().trimmed(),
            (int)(layoutTurnoutA->getCoordsA().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnoutA->getCoordsA().y()-4-(3*testIcon->getIconHeight())));
    }
    else if( layoutTurnoutAVertical && (!layoutTurnoutAThroatUp) ) {
        setSignalHeadOnPanel(3,a33WayField->text().trimmed(),
            (int)(layoutTurnoutA->getCoordsA().x()+4),
            (int)(layoutTurnoutA->getCoordsA().y()+8+(2*testIcon->getIconHeight())));
    }
}

/*private*/ void LayoutEditorTools::place3WayDivergingA() {
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    if( layoutTurnoutAHorizontal && layoutTurnoutAThroatLeft && layoutTurnoutABUp) {
        setSignalHeadOnPanel(0,b3WayField->text().trimmed(),
            (int)(layoutTurnoutA->getCoordsC().x()),
            (int)(layoutTurnoutA->getCoordsC().y()+4) );
    }
    else if( layoutTurnoutAHorizontal && layoutTurnoutAThroatLeft && (!layoutTurnoutABUp) ) {
        setSignalHeadOnPanel(0,b3WayField->text().trimmed(),
            (int)(layoutTurnoutA->getCoordsC().x()),
            (int)(layoutTurnoutA->getCoordsC().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnoutAHorizontal && (!layoutTurnoutAThroatLeft) && layoutTurnoutABUp ) {
        setSignalHeadOnPanel(2,b3WayField->text().trimmed(),
            (int)(layoutTurnoutA->getCoordsC().x()-testIcon->getIconWidth()),
            (int)(layoutTurnoutA->getCoordsC().y()+4) );
    }
    else if( layoutTurnoutAHorizontal && (!layoutTurnoutAThroatLeft) && (!layoutTurnoutABUp) ) {
        setSignalHeadOnPanel(2,b3WayField->text().trimmed(),
            (int)(layoutTurnoutA->getCoordsC().x()-testIcon->getIconWidth()),
            (int)(layoutTurnoutA->getCoordsC().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnoutAVertical && layoutTurnoutAThroatUp && layoutTurnoutABLeft ) {
        setSignalHeadOnPanel(3,b3WayField->text().trimmed(),
            (int)(layoutTurnoutA->getCoordsC().x()+4),
            (int)(layoutTurnoutA->getCoordsC().y()) );
    }
    else if( layoutTurnoutAVertical && layoutTurnoutAThroatUp && (!layoutTurnoutABLeft) ) {
        setSignalHeadOnPanel(3,b3WayField->text().trimmed(),
            (int)(layoutTurnoutA->getCoordsC().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnoutA->getCoordsC().y()) );
    }
    else if( layoutTurnoutAVertical && (!layoutTurnoutAThroatUp) && layoutTurnoutABLeft ) {
        setSignalHeadOnPanel(1,b3WayField->text().trimmed(),
            (int)(layoutTurnoutA->getCoordsC().x()+4),
            (int)(layoutTurnoutA->getCoordsC().y()-testIcon->getIconHeight()) );
    }
    else if( layoutTurnoutAVertical && (!layoutTurnoutAThroatUp) && (!layoutTurnoutABLeft) ) {
        setSignalHeadOnPanel(1,b3WayField->text().trimmed(),
            (int)(layoutTurnoutA->getCoordsC().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnoutA->getCoordsC().y()-testIcon->getIconHeight()) );
    }
}
/*private*/ void LayoutEditorTools::place3WayContinuing() {
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    if( layoutTurnoutBHorizontal && layoutTurnoutBThroatLeft && layoutTurnoutBBUp) {
        setSignalHeadOnPanel(0,c3WayField->text().trimmed(),
            (int)(layoutTurnoutB->getCoordsB().x()),
            (int)(layoutTurnoutB->getCoordsB().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnoutBHorizontal && layoutTurnoutBThroatLeft && (!layoutTurnoutBBUp) ) {
        setSignalHeadOnPanel(0,c3WayField->text().trimmed(),
            (int)(layoutTurnoutB->getCoordsB().x()),
            (int)(layoutTurnoutB->getCoordsB().y()+4) );
    }
    else if( layoutTurnoutBHorizontal && (!layoutTurnoutBThroatLeft) && layoutTurnoutBBUp ) {
        setSignalHeadOnPanel(2,c3WayField->text().trimmed(),
            (int)(layoutTurnoutB->getCoordsB().x()-testIcon->getIconWidth()),
            (int)(layoutTurnoutB->getCoordsB().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnoutBHorizontal && (!layoutTurnoutBThroatLeft) && (!layoutTurnoutBBUp) ) {
        setSignalHeadOnPanel(2,c3WayField->text().trimmed(),
            (int)(layoutTurnoutB->getCoordsB().x()-testIcon->getIconWidth()),
            (int)(layoutTurnoutB->getCoordsB().y()+4) );
    }
    else if( layoutTurnoutBVertical && layoutTurnoutBThroatUp && layoutTurnoutBBLeft ) {
        setSignalHeadOnPanel(3,c3WayField->text().trimmed(),
            (int)(layoutTurnoutB->getCoordsB().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnoutB->getCoordsB().y()) );
    }
    else if( layoutTurnoutBVertical && layoutTurnoutBThroatUp && (!layoutTurnoutBBLeft) ) {
        setSignalHeadOnPanel(3,c3WayField->text().trimmed(),
            (int)(layoutTurnoutB->getCoordsB().x()+4),
            (int)(layoutTurnoutB->getCoordsB().y()) );
    }
    else if( layoutTurnoutBVertical && (!layoutTurnoutBThroatUp) && layoutTurnoutBBLeft ) {
        setSignalHeadOnPanel(1,c3WayField->text().trimmed(),
            (int)(layoutTurnoutB->getCoordsB().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnoutB->getCoordsB().y()-testIcon->getIconHeight()) );
    }
    else if( layoutTurnoutBVertical && (!layoutTurnoutBThroatUp) && (!layoutTurnoutBBLeft) ) {
        setSignalHeadOnPanel(1,c3WayField->text().trimmed(),
            (int)(layoutTurnoutB->getCoordsB().x()+4),
            (int)(layoutTurnoutB->getCoordsB().y()-testIcon->getIconHeight()) );
    }
}
/*private*/ void LayoutEditorTools::place3WayDivergingB() {
    if (testIcon == nullptr)
        testIcon = signalIconEditor->getIcon(0);
    if( layoutTurnoutBHorizontal && layoutTurnoutBThroatLeft && layoutTurnoutBBUp) {
        setSignalHeadOnPanel(0,d3WayField->text().trimmed(),
            (int)(layoutTurnoutB->getCoordsC().x()),
            (int)(layoutTurnoutB->getCoordsC().y()+4) );
    }
    else if( layoutTurnoutBHorizontal && layoutTurnoutBThroatLeft && (!layoutTurnoutBBUp) ) {
        setSignalHeadOnPanel(0,d3WayField->text().trimmed(),
            (int)(layoutTurnoutB->getCoordsC().x()),
            (int)(layoutTurnoutB->getCoordsC().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnoutBHorizontal && (!layoutTurnoutBThroatLeft) && layoutTurnoutBBUp ) {
        setSignalHeadOnPanel(2,d3WayField->text().trimmed(),
            (int)(layoutTurnoutB->getCoordsC().x()-testIcon->getIconWidth()),
            (int)(layoutTurnoutB->getCoordsC().y()+4) );
    }
    else if( layoutTurnoutBHorizontal && (!layoutTurnoutBThroatLeft) && (!layoutTurnoutBBUp) ) {
        setSignalHeadOnPanel(2,d3WayField->text().trimmed(),
            (int)(layoutTurnoutB->getCoordsC().x()-testIcon->getIconWidth()),
            (int)(layoutTurnoutB->getCoordsC().y()-4-testIcon->getIconHeight()) );
    }
    else if( layoutTurnoutBVertical && layoutTurnoutBThroatUp && layoutTurnoutBBLeft ) {
        setSignalHeadOnPanel(3,d3WayField->text().trimmed(),
            (int)(layoutTurnoutB->getCoordsC().x()+4),
            (int)(layoutTurnoutB->getCoordsC().y()) );
    }
    else if( layoutTurnoutBVertical && layoutTurnoutBThroatUp && (!layoutTurnoutBBLeft) ) {
        setSignalHeadOnPanel(3,d3WayField->text().trimmed(),
            (int)(layoutTurnoutB->getCoordsC().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnoutB->getCoordsC().y()) );
    }
    else if( layoutTurnoutBVertical && (!layoutTurnoutBThroatUp) && layoutTurnoutBBLeft ) {
        setSignalHeadOnPanel(1,d3WayField->text().trimmed(),
            (int)(layoutTurnoutB->getCoordsC().x()+4),
            (int)(layoutTurnoutB->getCoordsC().y()-testIcon->getIconHeight()) );
    }
    else if( layoutTurnoutBVertical && (!layoutTurnoutBThroatUp) && (!layoutTurnoutBBLeft) ) {
        setSignalHeadOnPanel(1,d3WayField->text().trimmed(),
            (int)(layoutTurnoutB->getCoordsC().x()-4-testIcon->getIconWidth()),
            (int)(layoutTurnoutB->getCoordsC().y()-testIcon->getIconHeight()) );
    }
}
/*private*/ void LayoutEditorTools::set3WayLogicThroatContinuing() {
    TrackSegment* track = (TrackSegment*)layoutTurnoutB->getConnectB();
    if (track==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                tr("InfoMessage7"),"",JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsAtTToTFrame, tr("Information"), tr("Cannot set up logic because all connections\nhave not been defined around this item."));
        return;
    }
    LayoutBlock* block = track->getLayoutBlock();
    if (block==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                tr("InfoMessage6"),"",JOptionPane.INFORMATION_MESSAGE);
        QMessageBox::information(setSignalsAt3WayTurnoutFrame, tr("Information"), tr("Cannot set up logic because blocks have\nnot been defined around this item."));
        return;
    }
    Sensor* occupancy = block->getOccupancySensor();
    if (occupancy==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//            java.text.MessageFormat.format(tr("InfoMessage4"),
//                new Object[]{block->getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
        QMessageBox::information(setSignalsAt3WayTurnoutFrame, tr("Information"), tr("Cannot set up logic because block \"%1\"\ndoesn''t have an occupancy sensor.").arg(block->getUserName()));
        return;
    }
    SignalHead* nextHead = getNextSignalFromObject(track, layoutTurnoutB,
            a13WayField->text().trimmed(), setSignalsAt3WayTurnoutFrame);
    if ( (nextHead==nullptr) && (!reachedEndBumper()) ) {
//        JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//            java.text.MessageFormat.format(tr("InfoMessage5"),
//                new Object[]{block->getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Cannot set up logic because the next signal (in or \nat the end of block \"%1\") apparently is not yet defined.").arg(block->getUserName()));
        return;
    }
    if (a23WayHead!=nullptr) {
        // set up logic for continuing head with 3 heads at throat
        if (!initializeBlockBossLogic(a13WayField->text().trimmed())) return;
        logic->setMode(BlockBossLogic::TRAILINGMAIN);
        logic->setTurnout(turnoutB->getSystemName());
        logic->setSensor1(occupancy->getSystemName());
        if (nextHead!=nullptr) {
            logic->setWatchedSignal1(nextHead->getSystemName(),false);
        }
        if (auxSignal!=nullptr) {
            logic->setWatchedSignal1Alt(auxSignal->getSystemName());
        }
        QString nearSensorName = setupNearLogix(layoutTurnoutA,true,a13WayHead);
        addNearSensorToLogic(nearSensorName);
        finalizeBlockBossLogic();
        return;
    }
    // only one head at the throat
//    JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                tr("InfoMessage9"),"",JOptionPane.INFORMATION_MESSAGE);
    QMessageBox::information(setSignalsAtTToTFrame, tr("Information"), tr("Sorry, Layout Editor does not support setting up\nlogic for a single head at the throat of a 3-way turnout."));
    return;
}

/*private*/ void LayoutEditorTools::set3WayLogicThroatDivergingA() {
    TrackSegment* track = (TrackSegment*)layoutTurnoutA->getConnectC();
    if (track==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                tr("InfoMessage7"),"",JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsAtTToTFrame, tr("Information"), tr("Cannot set up logic because all connections\nhave not been defined around this item."));
        return;
    }
    LayoutBlock* block = track->getLayoutBlock();
    if (block==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                tr("InfoMessage6"),"",JOptionPane.INFORMATION_MESSAGE);
        QMessageBox::information(setSignalsAt3WayTurnoutFrame, tr("Information"), tr("Cannot set up logic because blocks have\nnot been defined around this item."));
        return;
    }
    Sensor* occupancy = block->getOccupancySensor();
    if (occupancy==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//            java.text.MessageFormat.format(tr("InfoMessage4"),
//                new Object[]{block->getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsAt3WayTurnoutFrame, tr("Information"), tr("Cannot set up logic because block \"%1\"\ndoesn''t have an occupancy sensor.").arg(block->getUserName()));
        return;
    }
    SignalHead* nextHead = getNextSignalFromObject(track, layoutTurnoutA,
            a23WayField->text().trimmed(), setSignalsAt3WayTurnoutFrame);
    if ( (nextHead==nullptr) && (!reachedEndBumper()) ) {
//        JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//            java.text.MessageFormat.format(tr("InfoMessage5"),
//                new Object[]{block->getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Cannot set up logic because the next signal (in or \nat the end of block \"%1\") apparently is not yet defined.").arg(block->getUserName()));
        return;
    }
    if (!initializeBlockBossLogic(a23WayField->text().trimmed())) return;
    logic->setMode(BlockBossLogic::TRAILINGDIVERGING);
    logic->setTurnout(turnoutA->getSystemName());
    logic->setSensor1(occupancy->getSystemName());
    if (nextHead!=nullptr)
        logic->setWatchedSignal1(nextHead->getSystemName(),false);
    if (auxSignal!=nullptr) {
        logic->setWatchedSignal1Alt(auxSignal->getSystemName());
    }
    if (!layoutTurnoutA->isMainlineC())
        logic->setLimitSpeed2(true);
    finalizeBlockBossLogic();
}
/*private*/ void LayoutEditorTools::set3WayLogicThroatDivergingB() {
    TrackSegment* track = (TrackSegment*)layoutTurnoutB->getConnectC();
    if (track==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                tr("InfoMessage7"),"",JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsAtTToTFrame, tr("Information"), tr("Cannot set up logic because all connections\nhave not been defined around this item."));
        return;
    }
    LayoutBlock* block = track->getLayoutBlock();
    if (block==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                tr("InfoMessage6"),"",JOptionPane.INFORMATION_MESSAGE);
        QMessageBox::information(setSignalsAt3WayTurnoutFrame, tr("Information"), tr("Cannot set up logic because blocks have\nnot been defined around this item."));
        return;
    }
    Sensor* occupancy = block->getOccupancySensor();
    if (occupancy==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//            java.text.MessageFormat.format(tr("InfoMessage4"),
//                new Object[]{block->getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsAt3WayTurnoutFrame, tr("Information"), tr("Cannot set up logic because block \"%1\"\ndoesn''t have an occupancy sensor.").arg(block->getUserName()));
        return;
    }
    SignalHead* nextHead = getNextSignalFromObject(track, layoutTurnoutB,
            a33WayField->text().trimmed(), setSignalsAt3WayTurnoutFrame);
    if ( (nextHead==nullptr) && (!reachedEndBumper()) ) {
//        JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//            java.text.MessageFormat.format(tr("InfoMessage5"),
//                new Object[]{block->getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Cannot set up logic because the next signal (in or \nat the end of block \"%1\") apparently is not yet defined.").arg(block->getUserName()));
        return;
    }
    if (!initializeBlockBossLogic(a33WayField->text().trimmed())) return;
    logic->setMode(BlockBossLogic::TRAILINGDIVERGING);
    logic->setTurnout(turnoutB->getSystemName());
    logic->setSensor1(occupancy->getSystemName());
    if (nextHead!=nullptr)
        logic->setWatchedSignal1(nextHead->getSystemName(),false);
    if (auxSignal!=nullptr) {
        logic->setWatchedSignal1Alt(auxSignal->getSystemName());
    }
    QString nearSensorName = setupNearLogix(layoutTurnoutA,true,a33WayHead);
    addNearSensorToLogic(nearSensorName);
    if (!layoutTurnoutB->isMainlineC())
        logic->setLimitSpeed2(true);
    finalizeBlockBossLogic();
}
/*private*/ void LayoutEditorTools::set3WayLogicDivergingA() {
    TrackSegment* track = (TrackSegment*)layoutTurnoutA->getConnectA();
    if (track==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                tr("InfoMessage7"),"",JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsAtTToTFrame, tr("Information"), tr("Cannot set up logic because all connections\nhave not been defined around this item."));
        return;
    }
    LayoutBlock* block = track->getLayoutBlock();
    if (block==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                tr("InfoMessage6"),"",JOptionPane.INFORMATION_MESSAGE);
        QMessageBox::information(setSignalsAt3WayTurnoutFrame, tr("Information"), tr("Cannot set up logic because blocks have\nnot been defined around this item."));
        return;
    }
    Sensor* occupancy = block->getOccupancySensor();
    if (occupancy==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//            java.text.MessageFormat.format(tr("InfoMessage4"),
//                new Object[]{block->getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsAt3WayTurnoutFrame, tr("Information"), tr("Cannot set up logic because block \"%1\"\ndoesn''t have an occupancy sensor.").arg(block->getUserName()));
        return;
    }
    SignalHead* nextHead = getNextSignalFromObject(track, layoutTurnoutA,
            b3WayField->text().trimmed(), setSignalsAt3WayTurnoutFrame);
    if ( (nextHead==nullptr) && (!reachedEndBumper()) ) {
//        JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//            java.text.MessageFormat.format(tr("InfoMessage5"),
//                new Object[]{block->getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Cannot set up logic because the next signal (in or \nat the end of block \"%1\") apparently is not yet defined.").arg(block->getUserName()));
        return;
    }
    if (!initializeBlockBossLogic(b3WayField->text().trimmed())) return;
    logic->setMode(BlockBossLogic::TRAILINGDIVERGING);
    logic->setTurnout(turnoutA->getSystemName());
    logic->setSensor1(occupancy->getSystemName());
    if (nextHead!=nullptr) {
        logic->setWatchedSignal1(nextHead->getSystemName(),false);
    }
    if (auxSignal!=nullptr) {
        logic->setWatchedSignal1Alt(auxSignal->getSystemName());
    }
    if (!layoutTurnoutA->isMainlineC())
        logic->setLimitSpeed2(true);
    finalizeBlockBossLogic();
}
/*private*/ void LayoutEditorTools::set3WayLogicContinuing() {
    TrackSegment* track = (TrackSegment*)layoutTurnoutA->getConnectA();
    if (track==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                tr("InfoMessage7"),"",JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsAtTToTFrame, tr("Information"), tr("Cannot set up logic because all connections\nhave not been defined around this item."));
        return;
    }
    LayoutBlock* block = track->getLayoutBlock();
    if (block==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                tr("InfoMessage6"),"",JOptionPane.INFORMATION_MESSAGE);
        QMessageBox::information(setSignalsAt3WayTurnoutFrame, tr("Information"), tr("Cannot set up logic because blocks have\nnot been defined around this item."));
        return;
    }
    Sensor* occupancy = block->getOccupancySensor();
    if (occupancy==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//            java.text.MessageFormat.format(tr("InfoMessage4"),
//                new Object[]{block->getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsAt3WayTurnoutFrame, tr("Information"), tr("Cannot set up logic because block \"%1\"\ndoesn''t have an occupancy sensor.").arg(block->getUserName()));
        return;
    }
    SignalHead* nextHead = getNextSignalFromObject(track, layoutTurnoutA,
            c3WayField->text().trimmed(), setSignalsAt3WayTurnoutFrame);
    if ( (nextHead==nullptr) && (!reachedEndBumper()) ) {
//        JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//            java.text.MessageFormat.format(tr("InfoMessage5"),
//                new Object[]{block->getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Cannot set up logic because the next signal (in or \nat the end of block \"%1\") apparently is not yet defined.").arg(block->getUserName()));
        return;
    }
    if (!initializeBlockBossLogic(c3WayField->text().trimmed())) return;
    logic->setMode(BlockBossLogic::TRAILINGMAIN);
    logic->setTurnout(turnoutB->getSystemName());
    logic->setSensor1(occupancy->getSystemName());
    if (nextHead!=nullptr) {
        logic->setWatchedSignal1(nextHead->getSystemName(),false);
    }
    if (auxSignal!=nullptr) {
        logic->setWatchedSignal1Alt(auxSignal->getSystemName());
    }
    QString nearSensorName = setupNearLogix(layoutTurnoutA,true,c3WayHead);
    addNearSensorToLogic(nearSensorName);
    if (!layoutTurnoutB->isMainlineB())
        logic->setLimitSpeed2(true);
    finalizeBlockBossLogic();
}
/*private*/ void LayoutEditorTools::set3WayLogicDivergingB() {
    TrackSegment* track = (TrackSegment*)layoutTurnoutA->getConnectA();
    if (track==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                tr("InfoMessage7"),"",JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsAtTToTFrame, tr("Information"), tr("Cannot set up logic because all connections\nhave not been defined around this item."));
        return;
    }
    LayoutBlock* block = track->getLayoutBlock();
    if (block==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//                tr("InfoMessage6"),"",JOptionPane.INFORMATION_MESSAGE);
        QMessageBox::information(setSignalsAt3WayTurnoutFrame, tr("Information"), tr("Cannot set up logic because blocks have\nnot been defined around this item."));
        return;
    }
    Sensor* occupancy = block->getOccupancySensor();
    if (occupancy==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//            java.text.MessageFormat.format(tr("InfoMessage4"),
//                new Object[]{block->getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::information(setSignalsAt3WayTurnoutFrame, tr("Information"), tr("Cannot set up logic because block \"%1\"\ndoesn''t have an occupancy sensor.").arg(block->getUserName()));
        return;
    }
    SignalHead* nextHead = getNextSignalFromObject(track, layoutTurnoutA,
            d3WayField->text().trimmed(), setSignalsAt3WayTurnoutFrame);
    if ( (nextHead==nullptr) && (!reachedEndBumper()) ) {
//        JOptionPane.showMessageDialog(setSignalsAt3WayTurnoutFrame,
//            java.text.MessageFormat.format(tr("InfoMessage5"),
//                new Object[]{block->getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Cannot set up logic because the next signal (in or \nat the end of block \"%1\") apparently is not yet defined.").arg(block->getUserName()));
        return;
    }
    if (!initializeBlockBossLogic(d3WayField->text().trimmed())) return;
    logic->setMode(BlockBossLogic::TRAILINGDIVERGING);
    logic->setTurnout(turnoutB->getSystemName());
    logic->setSensor1(occupancy->getSystemName());
    if (nextHead!=nullptr) {
        logic->setWatchedSignal1(nextHead->getSystemName(),false);
    }
    if (auxSignal!=nullptr) {
        logic->setWatchedSignal1Alt(auxSignal->getSystemName());
    }
    QString nearSensorName = setupNearLogix(layoutTurnoutA,true,d3WayHead);
    addNearSensorToLogic(nearSensorName);
    if (!layoutTurnoutB->isMainlineC())
        logic->setLimitSpeed2(true);
    finalizeBlockBossLogic();
}

//
//The following is for placement of sensors and signal masts at points around the layout
//

/*public*/ void LayoutEditorTools::setSensorsAtBlockBoundaryFromMenu( PositionablePoint* p, MultiIconEditor* theEditor, JFrame* theFrame) {
    boundaryFromMenu = true;
    boundary = p;
    block1NameField->setText(boundary->getConnect1()->getLayoutBlock()->getId());
    if (boundary->getType()!=PositionablePoint::ANCHOR)
        block2NameField->setText(boundary->getConnect1()->getLayoutBlock()->getId());
    else
        block2NameField->setText(boundary->getConnect2()->getLayoutBlock()->getId());
    setSensorsAtBlockBoundary(theEditor,theFrame);
    return;
}

/*public*/ void LayoutEditorTools::setSensorsAtBlockBoundary( MultiIconEditor* theEditor, JFrame* theFrame)
{
 sensorIconEditor = theEditor;
 sensorFrame = theFrame;
 if (setSensorsAtBoundaryOpen)
 {
  setSensorsAtBoundaryFrame->setVisible(true);
  return;
 }
 // Initialize if needed
 if (setSensorsAtBoundaryFrame == nullptr)
 {
  westBoundSensor = new BeanDetails("Sensor", InstanceManager::sensorManagerInstance());
  eastBoundSensor = new BeanDetails("Sensor", InstanceManager::sensorManagerInstance());

  setSensorsAtBoundaryFrame = new JmriJFrameX( tr("Set Sensors at Block Boundary"), false, true );
  setSensorsAtBoundaryFrame->addHelpMenu("package.jmri.jmrit.display.SetSensorsAtBoundary", true);
  setSensorsAtBoundaryFrame->setLocation(70,30);
//  if(setSensorsAtBoundaryFrame->centralWidget() == nullptr)
//  {
//   QWidget* centralWidget = new QWidget();
//   QVBoxLayout* centralWidgetLayout = new QVBoxLayout;
//   centralWidget->setLayout(centralWidgetLayout);
//   setSensorsAtBoundaryFrame->setCentralWidget(centralWidget);
//  }
  QWidget* theContentPane = setSensorsAtBoundaryFrame->getContentPane();
  QVBoxLayout* centralWidgetLayout;
  theContentPane->setLayout(centralWidgetLayout = new QVBoxLayout());
  QWidget* header = new QWidget();
  header->setLayout(new QVBoxLayout); //(header, BoxLayout.Y_AXIS));
  QWidget* panel11 = new QWidget();
  panel11->setLayout(new QHBoxLayout());
  if (boundaryFromMenu)
  {
   QLabel* block1NameLabel = new QLabel( tr("Block")+" 1 "+
       tr("Name")+" : "+boundary->getConnect1()->getLayoutBlock()->getId());
   panel11->layout()->addWidget(block1NameLabel);
  }
  else
  {
   QLabel* block1NameLabel = new QLabel( tr("Block")+" 1 "+
                                                   tr("Name")+" : ");
   panel11->layout()->addWidget(block1NameLabel);
   panel11->layout()->addWidget(block1NameField);
   block1NameField->setToolTip(tr("Enter name of one block where sensors are needed at block boundary."));
  }
  header->layout()->addWidget(panel11);
  QWidget* panel12 = new QWidget();
  panel12->setLayout(new QHBoxLayout());
  if ((boundaryFromMenu) && (boundary->getType()==PositionablePoint::ANCHOR))
  {
   QLabel* block2NameLabel = new QLabel( tr("Block")+" 2 "+
       tr("Name")+" : "+boundary->getConnect2()->getLayoutBlock()->getId());
   panel12->layout()->addWidget(block2NameLabel);
  }
  else if (boundary->getType()==PositionablePoint::ANCHOR)
  {
   QLabel* block2NameLabel = new QLabel( tr("Block")+" 2 "+
                                                   tr("Name")+" : ");
   panel12->layout()->addWidget(block2NameLabel);
   panel12->layout()->addWidget(block2NameField);
   block2NameField->setToolTip(tr("Enter name of one block where sensors are needed at block boundary."));
  }
  header->layout()->addWidget(panel12);
  header->layout()->addWidget(new JSeparator(JSeparator::HORIZONTAL));
  centralWidgetLayout->addWidget(header,0, Qt::AlignTop);// BorderLayout::North);
  QWidget* panel2 = new QWidget();

  QWidget*main = new QWidget();
  main->setLayout(new QVBoxLayout); //(main, BoxLayout.Y_AXIS));

  panel2->setLayout(new QHBoxLayout());
  QLabel* shTitle = new QLabel(tr("Sensors"));
  panel2->layout()->addWidget(shTitle);
  panel2->layout()->addWidget(new QLabel("   "));
  panel2->layout()->addWidget(getAnchorSavedSensors = new QPushButton(tr("Get Saved")));
//        getAnchorSavedSensors->layout()->addWidgetActionListener(new ActionListener() {
//                /*public*/ void actionPerformed(ActionEvent e) {
//                    getSavedAnchorSensors(e);
//                }
//            });
  connect(getAnchorSavedSensors, SIGNAL(clicked()), this, SLOT(getSavedAnchorSensors()));
  getAnchorSavedSensors->setToolTip( "Click to retrieve signal heads previously stored.");
  if(boundary->getType()!=PositionablePoint::END_BUMPER)
      main->layout()->addWidget(panel2);

  if(boundary->getType()!=PositionablePoint::END_BUMPER)
  {
   eastBoundSensor->setBoundaryTitle(tr("East/SouthBound"));
   if ((boundaryFromMenu)  && (boundary->getType()==PositionablePoint::ANCHOR))
   {
    if(isAtWestEndOfAnchor(boundary->getConnect1(), boundary)){
        eastBoundSensor->setBoundaryLabelText("Protecting Block : " + boundary->getConnect2()->getLayoutBlock()->getDisplayName());
    }
    else
    {
     eastBoundSensor->setBoundaryLabelText("Protecting Block : " + boundary->getConnect1()->getLayoutBlock()->getDisplayName());
    }
   }
   //eastBoundSensor->getDetailsPanel()->setBackground( QColor(255,255,200));
   eastBoundSensor->getDetailsPanel()->setStyleSheet("background: {color rgb(255,255,200)}");
//            main->layout()->addWidget(eastBoundSensor->getDetailsPanel());

   westBoundSensor->setBoundaryTitle(tr("West/NorthBound"));
   if (boundaryFromMenu)
   {
    if(isAtWestEndOfAnchor(boundary->getConnect1(), boundary)){
        westBoundSensor->setBoundaryLabelText("Protecting Block : " + boundary->getConnect1()->getLayoutBlock()->getDisplayName());
    } else {
        westBoundSensor->setBoundaryLabelText("Protecting Block : " + boundary->getConnect2()->getLayoutBlock()->getDisplayName());
    }
   }
   //westBoundSensor->getDetailsPanel()->setBackground( QColor(200,255,255));
   westBoundSensor->getDetailsPanel()->setStyleSheet("background: {color rgb(255,255,200)}");

   main->layout()->addWidget(westBoundSensor->getDetailsPanel());
  }
  else
  {
   if(isAtWestEndOfAnchor(boundary->getConnect1(), boundary) && (boundaryFromMenu)){
       eastBoundSensor->setBoundaryLabelText("Protecting Block : " + boundary->getConnect1()->getLayoutBlock()->getDisplayName());
       //eastBoundSensor->getDetailsPanel()->setBackground(QColor(200,255,255));
       eastBoundSensor->getDetailsPanel()->setStyleSheet("background: {color rgb(200,255,255)}");

       main->layout()->addWidget(eastBoundSensor->getDetailsPanel());
   } else if (boundaryFromMenu) {
       westBoundSensor->setBoundaryLabelText("Protecting Block : " + boundary->getConnect1()->getLayoutBlock()->getDisplayName());
       //westBoundSensor->getDetailsPanel()->setBackground(new Color(255,255,200));
       westBoundSensor->getDetailsPanel()->setStyleSheet("background: {color rgb(200,255,255)}");

       main->layout()->addWidget(westBoundSensor->getDetailsPanel());
   }
  }
  main->layout()->addWidget(new JSeparator(JSeparator::HORIZONTAL));
  centralWidgetLayout->addWidget(main, 0, Qt::AlignCenter); //BorderLayout::Center);

  QWidget* panel6 = new QWidget();
  panel6->setLayout(new QHBoxLayout());
  panel6->layout()->addWidget(changeSensorAtBoundaryIcon = new QPushButton(tr("Change Sensor Icon")));
//        changeSensorAtBoundaryIcon->layout()->addWidgetActionListener(new ActionListener() {
//                /*public*/ void actionPerformed(ActionEvent e) {
//                    sensorFrame->setVisible(true);
//                }
//            });
  connect(changeSensorAtBoundaryIcon, SIGNAL(clicked()), this, SLOT(On_changeSensorAtBoundaryIcon_clicked()));
  changeSensorAtBoundaryIcon->setToolTip( tr("Click to change icons used to represent sensors.") );
  panel6->layout()->addWidget(new QLabel("  "));
  panel6->layout()->addWidget(setSensorsAtBoundaryDone = new QPushButton(tr("Done")));
//        setSensorsAtBoundaryDone->layout()->addWidgetActionListener(new ActionListener() {
//            /*public*/ void actionPerformed(ActionEvent e) {
//                setSensorsAtBoundaryDonePressed(e);
//            }
//        });
  connect(setSensorsAtBoundaryDone, SIGNAL(clicked()), this, SLOT(setSensorsAtBoundaryDonePressed()));
  setSensorsAtBoundaryDone->setToolTip( tr("Click Done to perform tasks requested above and dismiss this dialog.") );
  panel6->layout()->addWidget(setSensorsAtBoundaryCancel = new QPushButton(tr("Cancel")));
//        setSensorsAtBoundaryCancel->layout()->addWidgetActionListener(new ActionListener() {
//            /*public*/ void actionPerformed(ActionEvent e) {
//                setSensorsAtBoundaryCancelPressed(e);
//            }
//        });
  connect(setSensorsAtBoundaryCancel, SIGNAL(clicked()), this, SLOT(setSensorsAtBoundaryCancelPressed()));
  setSensorsAtBoundaryCancel->setToolTip( tr("Click Cancel to dismiss this dialog without making changes.") );
  centralWidgetLayout->addWidget(panel6, 0, Qt::AlignBottom); //BorderLayout::South);
//        setSensorsAtBoundaryFrame->layout()->addWidgetWindowListener(new java.awt.event.WindowAdapter() {
//            /*public*/ void windowClosing(java.awt.event.WindowEvent e) {
//                setSensorsAtBoundaryCancelPressed(nullptr);
//            }
//        });

  if (boundaryFromMenu) getSavedAnchorSensors(nullptr);
 }
 else
 {
  if (boundaryFromMenu) getSavedAnchorSensors(nullptr);
 }
 //setSensorsAtBoundaryFrame->setPreferredSize(nullptr);
 setSensorsAtBoundaryFrame->adjustSize();
 setSensorsAtBoundaryFrame->setVisible(true);
 setSensorsAtBoundaryOpen = true;
}

void LayoutEditorTools::On_changeSensorAtBoundaryIcon_clicked()
{
 sensorFrame->setVisible(true);
}

/**
 * Returns the Sensor* corresponding to an entry field in the specified dialog->
 *		This also takes care of UpperCase and trimming of leading and trailing blanks.
 *		If entry is required, and no entry is present, and error message is sent.
 *      An error message also results if a sensor head with the entered name is not
 *      found in the SensorTable.
 */
/*public*/ Sensor* LayoutEditorTools::getSensorFromEntry(QString sensorName, bool requireEntry,
                JmriJFrame* frame) {
    QString str = sensorName;
    if ( (str==nullptr) || (str==("")) ) {
        if (requireEntry) {
//            JOptionPane.showMessageDialog(frame,tr("SensorsError5"),
//                                tr("Error"),JOptionPane.ERROR_MESSAGE);
            QMessageBox::critical(frame, tr("Error"), tr("Error - Sensor name was not entered. Please enter\na sensor name for required positions or cancel."));
        }
        return NULL;
    }
    Sensor* head = ((ProxySensorManager*)InstanceManager::sensorManagerInstance())->getSensor(str);
    if (head==nullptr) {
//        JOptionPane.showMessageDialog(frame,
//                java.text.MessageFormat.format(tr("SensorsError4"),
//                    new Object[]{str}), tr("Error"),
//                        JOptionPane.ERROR_MESSAGE);
        QMessageBox::critical(frame, tr("Error"), tr("Error - Sensor \"%1\" does not exist. Please correct name\nor enter this Sensor in the Sensor Table, and try again.").arg(str));
        return NULL ;
    }
    return (head);
}
/**
 * Returns a Sensor* given a name
 */
/*public*/ Sensor* LayoutEditorTools::getSensorFromName(QString str) {
    if ( (str==nullptr) || (str==("")) ) {
        return NULL;
    }
    return  (((ProxySensorManager*)InstanceManager::sensorManagerInstance())->getSensor(str));
}

/*public*/ SensorIcon* LayoutEditorTools::getSensorIcon(QString sensorName) {
    SensorIcon* l = new SensorIcon(new NamedIcon("program:/resources/icons/smallschematics/tracksegments/circuit-error.gif",
                                                "program:/resources/icons/smallschematics/tracksegments/circuit-error.gif"),layoutEditor);
    l->setIcon("SensorStateActive", sensorIconEditor->getIcon(0));
    l->setIcon("SensorStateInactive", sensorIconEditor->getIcon(1));
    l->setIcon("BeanStateInconsistent", sensorIconEditor->getIcon(2));
    l->setIcon("BeanStateUnknown", sensorIconEditor->getIcon(3));
    l->setSensor(sensorName);
    return l;
}

/**
 * Returns true if the specified Sensor*  is assigned to an object
 *		on the panel, regardless of whether an icon is displayed or not
 * With sensors we do allow the same sensor to be allocated in both directions.
 */
/*public*/ bool LayoutEditorTools::isSensorAssignedAnywhere(Sensor* sensor)
{
 bool result = false;
 for (PositionablePoint* po : layoutEditor->getPositionablePoints())
 {
     if (po->getEastBoundSensor() == sensor) {
      result = true;
      break;
     }
     if (po->getWestBoundSensor() == sensor) {
      result = true;
      break;
     }
 }
 if (!result)
 {
  //check turnouts and slips
  for (LayoutTurnout* to : *layoutEditor->getLayoutTurnoutsAndSlips()) {
      if (whereIsSensorAssigned(sensor, to) != LayoutTurnout::NONE) {
          result = true;
          break;
      }
  }
 }
 if (!result)
 {
  //check level crossings
  for (LevelXing* x : layoutEditor->getLevelXings())
  {
   if ((x->getSensorA() != nullptr) && x->getSensorA() == sensor) {
       result = true;
       break;
   }
   if ((x->getSensorB() != nullptr) && x->getSensorB() == sensor) {
       result = true;
       break;
   }
   if ((x->getSensorC() != nullptr) && x->getSensorC() == sensor) {
       result = true;
       break;
   }
   if ((x->getSensorD() != nullptr) && x->getSensorD() == sensor) {
       result = true;
       break;
   }
  }
 }

 return result;
}   //isSensorAssignedAnywhere

/*private*/ int LayoutEditorTools::whereIsSensorAssigned(Sensor* sensor, LayoutTurnout* lTurnout) {
    int result = LayoutTurnout::NONE;

    if (sensor != nullptr && lTurnout != nullptr) {
        QString sName = sensor->getSystemName();
        QString uName = sensor->getUserName();

        QString name = lTurnout->getSensorAName();
        if (!name.isEmpty() && name == (uName) || name == (sName)) {
            return LayoutTurnout::POINTA1;
        }
        name = lTurnout->getSensorBName();
        if (!name.isEmpty() && name == (uName) || name == (sName)) {
            return LayoutTurnout::POINTA2;
        }
        name = lTurnout->getSensorCName();
        if (!name.isEmpty() && name == (uName) || name == (sName)) {
            return LayoutTurnout::POINTA3;
        }
        name = lTurnout->getSensorDName();
        if (!name.isEmpty() && name == (uName) || name == (sName)) {
            return LayoutTurnout::POINTB1;
        }
    }
    return result;
}   //whereIsSensorAssigned

bool LayoutEditorTools::sensorAssignedElseWhere(Sensor* sensor){
//        int i = JOptionPane.showConfirmDialog(NULL, java.text.MessageFormat.format(tr("DuplicateSensorAssign"),
//                new Object[]{ sensor }),
//        tr("DuplicateSensorAssignTitle"),
//        JOptionPane.YES_NO_OPTION);
//    if(i ==0) {
//        return true;
    switch(QMessageBox::question(0, tr("Sensor Already Assigned"), tr("Sensor \"%1\" has already been assigned on the panel\nAre you sure you want to assign it again?").arg(sensor->getDisplayName()), QMessageBox::Yes | QMessageBox::No))
    {
    case QMessageBox::Yes:
     return true;
    default:
     break;
    }
    return false;
}
/**
 * Removes the assignment of the specified Sensor* to either a turnout,
 *		a positionable point, or a level crossing wherever it is assigned
 */
/*public*/ bool LayoutEditorTools::removeSensorAssignment(Sensor* sensor)
{
 log->trace(tr("Remove sensor assignment at block boundary for '%1'").arg(sensor->getDisplayName()));  // NOI18N
         if (!static_cast<EntryExitPairs*>(InstanceManager::getDefault("EntryExitPairs"))->deleteNxPair(sensor)) {
             log->trace(tr("Removal of NX pairs for sensor '%1' failed").arg(sensor->getDisplayName()));  // NOI18N
             return false;
         }
         for (PositionablePoint* po : layoutEditor->getPositionablePoints()) {
             if (po->getEastBoundSensor() == sensor) {
                 po->setEastBoundSensor(nullptr);
             }
             if (po->getWestBoundSensor() == sensor) {
                 po->setWestBoundSensor(nullptr);
             }
         }

         for (LayoutTrack* lt : layoutEditor->getLayoutTurnouts()) {
          LayoutTurnout* to = (LayoutTurnout*)lt;
             if (to->getSensorA() == sensor) {
                 to->setSensorA(nullptr);
             }
             if (to->getSensorB() == sensor) {
                 to->setSensorB(nullptr);
             }
             if (to->getSensorC() == sensor) {
                 to->setSensorC(nullptr);
             }
             if (to->getSensorD() == sensor) {
                 to->setSensorD(nullptr);
             }
         }

         for (LayoutTrack* lt : layoutEditor->getLayoutSlips())
         {
          LayoutSlip* to = (LayoutSlip*)lt;
             if (to->getSensorA() == sensor) {
                 to->setSensorA(nullptr);
             }
             if (to->getSensorB() == sensor) {
                 to->setSensorB(nullptr);
             }
             if (to->getSensorC() == sensor) {
                 to->setSensorC(nullptr);
             }
             if (to->getSensorD() == sensor) {
                 to->setSensorD(nullptr);
             }
         }

         for (LayoutTrack* lt : layoutEditor->getLevelXings()) {
          LevelXing* x = (LevelXing*)lt;
             if (x->getSensorA() == sensor) {
                 x->setSensorAName(nullptr);
             }
             if (x->getSensorB() == sensor) {
                 x->setSensorBName(nullptr);
             }
             if (x->getSensorC() == sensor) {
                 x->setSensorCName(nullptr);
             }
             if (x->getSensorD() == sensor) {
                 x->setSensorDName(nullptr);
             }
         }

         return true;
}

/**
 * Removes the Sensor* with the specified name from the panel and from
 *		assignment to any turnout, positionable point, or level crossing
 */
//@SuppressWarnings("NULL")
/*public*/ bool LayoutEditorTools::removeSensorFromPanel(Sensor* sensor)
{
 log->trace(tr("Remove sensor icon and assignment for '%1'").arg(sensor->getDisplayName()));  // NOI18N
 if (!removeSensorAssignment(sensor)) {
     return false;
 }

 SensorIcon* h = nullptr;
 int index = -1;
 for (int i = 0; (i < layoutEditor->sensorList->size()) && (index == -1); i++) {
     h = layoutEditor->sensorList->at(i);
     if (h->getSensor() == sensor) {
         index = i;
     }
 }
 if ((h != nullptr) && (index != -1)) {
     layoutEditor->sensorList->remove(index);
     h->remove();
     h->dispose();
     needRedraw = true;
 }
 return true;
}

/*private*/ void LayoutEditorTools::getSavedAnchorSensors (ActionEvent* /*a*/) // SLOT
{
    if ( !getSimpleBlockInformation() ) return;
    eastBoundSensor->setTextField(boundary->getEastBoundSensorName());
    westBoundSensor->setTextField(boundary->getWestBoundSensorName());

    if(boundary->getType()!=PositionablePoint::END_BUMPER)
    {
        if(isAtWestEndOfAnchor(boundary->getConnect1(), boundary))
        {
            eastBoundSensor->setBoundaryLabelText("Protecting Block : " + boundary->getConnect2()->getLayoutBlock()->getDisplayName());
        }
        else {
            eastBoundSensor->setBoundaryLabelText("Protecting Block : " + boundary->getConnect1()->getLayoutBlock()->getDisplayName());
        }
        if(isAtWestEndOfAnchor(boundary->getConnect1(), boundary)){
            westBoundSensor->setBoundaryLabelText("Protecting Block : " + boundary->getConnect1()->getLayoutBlock()->getDisplayName());
        } else {
            westBoundSensor->setBoundaryLabelText("Protecting Block : " + boundary->getConnect2()->getLayoutBlock()->getDisplayName());
        }
    } else {
        if(isAtWestEndOfAnchor(boundary->getConnect1(), boundary)){
            westBoundSensor->setBoundaryLabelText("End of Block " + boundary->getConnect1()->getLayoutBlock()->getDisplayName());
        } else{
            eastBoundSensor->setBoundaryLabelText("End of Block " + boundary->getConnect1()->getLayoutBlock()->getDisplayName());
        }
    }

//    setSensorsAtBoundaryFrame->setPreferredSize(nullptr);
//    setSensorsAtBoundaryFrame.pack();
}

/*private*/ void LayoutEditorTools::setSensorsAtBoundaryCancelPressed (ActionEvent* /*a*/) {
    setSensorsAtBoundaryOpen = false;
    boundaryFromMenu = false;
    setSensorsAtBoundaryFrame->setVisible(false);
}

/*private*/ void LayoutEditorTools::setSensorsAtBoundaryDonePressed (ActionEvent* /*a*/) // SLOT
{
 if ( !getSimpleBlockInformation() ) return;
 Sensor* eastSensor = getSensorFromEntry(eastBoundSensor->getText(),false,setSensorsAtBoundaryFrame);
 Sensor* westSensor = getSensorFromEntry(westBoundSensor->getText(),false,setSensorsAtBoundaryFrame);
 Sensor* currEastSensor = InstanceManager::sensorManagerInstance()->getSensor(boundary->getEastBoundSensorName());
 Sensor* currWestSensor = InstanceManager::sensorManagerInstance()->getSensor(boundary->getWestBoundSensorName());

 if (log->isTraceEnabled()) {
     log->trace(tr("current sensors: east = %1, west = %2").arg(  // NOI18N
             (currEastSensor == nullptr) ? "- none- " : currEastSensor->getDisplayName()).arg(  // NOI18N
             (currWestSensor == nullptr) ? "- none- " : currWestSensor->getDisplayName()));  // NOI18N
     log->trace(tr("new sensors: east = %1, west = %2").arg(  // NOI18N
             (eastSensor == nullptr) ? "- none- " : eastSensor->getDisplayName()).arg(  // NOI18N
             (westSensor == nullptr) ? "- none- " : westSensor->getDisplayName()));  // NOI18N
 }

 if (eastSensor == nullptr) {
     if (currEastSensor != nullptr && removeSensorFromPanel(currEastSensor)) {
         boundary->setEastBoundSensor(nullptr);
     }
 } else if (eastBoundSensor != nullptr) {
     setBoundarySensor(eastSensor, currEastSensor, eastBoundSensor, "East");  // NOI18N
 }

 if (westSensor == nullptr) {
     if (currWestSensor != nullptr && removeSensorFromPanel(currWestSensor)) {
         boundary->setWestBoundSensor(nullptr);
     }
 } else if (westBoundSensor != nullptr) {
     setBoundarySensor(westSensor, currWestSensor, westBoundSensor, "West");  // NOI18N
 }

 setSensorsAtBlockBoundaryOpenFlag = false;
 setSensorsAtBlockBoundaryFrame->setVisible(false);
 if (needRedraw) {
     layoutEditor->redrawPanel();
     needRedraw = false;
     layoutEditor->setDirty();
 }
}

/**
 * Attached a sensor to the block boundary positional point.
 * @since 4.11.2
 * @param newSensor The sensor that is being added.
 * @param currSensor The sensor that might already be there, otherwise null.
 * @param beanDetail The BeanDetails object that contains the supporting data.
 * @param direction The direction, East or West.
 */
void LayoutEditorTools::setBoundarySensor(Sensor* newSensor, Sensor* currSensor,
        BeanDetails* beanDetail, QString direction) {
    if (currSensor == nullptr) {
        if (!isSensorAssignedAnywhere(newSensor)) {
            log->trace(tr("Add sensor '%1'").arg(newSensor->getDisplayName()));  // NOI18N
            if (direction == ("West")) {  // NOI18N
                boundary->setWestBoundSensor(beanDetail->getText());
            } else {
                boundary->setEastBoundSensor(beanDetail->getText());
            }
            if (beanDetail->addToPanel()) {
                log->trace(tr("Add icon for sensor '%1'").arg(newSensor->getDisplayName()));  // NOI18N
                if (direction == ("West")) {  // NOI18N
                    placeWestBoundIcon(getSensorIcon(beanDetail->getText()),
                            beanDetail->isRightSelected(), 0.0);
                } else {
                    placeEastBoundIcon(getSensorIcon(beanDetail->getText()),
                            beanDetail->isRightSelected(), 0.0);
                }
                needRedraw = true;
            }
        } else {
            sensorAssignedElseWhere(newSensor);
        }
    } else if (currSensor == newSensor) {
         if (beanDetail->addToPanel()) {
            if (!isSensorOnPanel(newSensor)) {
                log->trace(tr("Add icon for existing sensor '%1'").arg(newSensor->getDisplayName()));  // NOI18N
                if (direction == ("West")) {  // NOI18N
                    placeWestBoundIcon(getSensorIcon(beanDetail->getText()),
                            beanDetail->isRightSelected(), 0.0);
                } else {
                    placeEastBoundIcon(getSensorIcon(beanDetail->getText()),
                            beanDetail->isRightSelected(), 0.0);
                }
                needRedraw = true;
            }
        }
    } else {
        if (!isSensorAssignedAnywhere(newSensor)) {
            if (removeSensorFromPanel(currSensor)) {
                log->trace(tr("Replace sensor '%1' with sensor '%2'").arg(  // NOI18N
                        currSensor->getDisplayName()).arg(newSensor->getDisplayName()));
                if (direction == ("West")) {  // NOI18N
                    boundary->setWestBoundSensor(beanDetail->getText());
                } else {
                    boundary->setEastBoundSensor(beanDetail->getText());
                }
                if (beanDetail->addToPanel()) {
                    log->trace(tr("Add icon for replacement sensor '%1'").arg(  // NOI18N
                            newSensor->getDisplayName()));
                    if (direction == ("West")) {  // NOI18N
                        placeWestBoundIcon(getSensorIcon(beanDetail->getText()),
                                beanDetail->isRightSelected(), 0.0);
                    } else {
                        placeEastBoundIcon(getSensorIcon(beanDetail->getText()),
                                beanDetail->isRightSelected(), 0.0);
                    }
                    needRedraw = true;
                }
            }
        } else {
            sensorAssignedElseWhere(newSensor);
        }
    }
}

/*public*/ bool LayoutEditorTools::isSensorOnPanel(Sensor* sensor)
{
    SensorIcon* s = NULL;
    for (int i=0;i<layoutEditor->sensorList->size();i++) {
        s = layoutEditor->sensorList->at(i);
        if (s->getSensor() == sensor) {
            return true;
        }
    }
    return false;
}



/*public*/ void LayoutEditorTools::setSignalMastsAtBlockBoundaryFromMenu(PositionablePoint* p)
{
 boundaryFromMenu = true;
 boundary = p;
 block1NameField->setText(boundary->getConnect1()->getLayoutBlock()->getId());
 if(boundary->getType()==PositionablePoint::ANCHOR)
     block2NameField->setText(boundary->getConnect2()->getLayoutBlock()->getId());
 setSignalMastsAtBlockBoundary();
 return;
}

/*public*/ void LayoutEditorTools::setSignalMastsAtBlockBoundary()
{
 if (setSignalMastsAtBoundaryOpen) {
     setSignalMastsAtBoundaryFrame->setVisible(true);
     return;
 }
 // Initialize if needed
 if (setSignalMastsAtBoundaryFrame == nullptr)
 {
  eastSignalMast = new BeanDetails("Signal Mast", (Manager*)InstanceManager::getDefault("SignalMastManager"));
  westSignalMast = new BeanDetails("Signal Mast", (Manager*)InstanceManager::getDefault("SignalMastManager"));
  setSignalMastsAtBoundaryFrame = new JmriJFrameX( tr("Set Signal Masts at Block Boundary"), false, true );
  setSignalMastsAtBoundaryFrame->addHelpMenu("package.jmri.jmrit.display.SetSignalMastsAtBoundary", true);
  setSignalMastsAtBoundaryFrame->setLocation(70,30);
  //QWidget* theContentPane = setSignalMastsAtBoundaryFrame->getContentPane();
  QScrollArea* centralWidget = new QScrollArea;
  centralWidget->setWidgetResizable(true);
  QWidget* scrollWidget = new QWidget;
  centralWidget->setWidget(scrollWidget);
  BorderLayout* centralWidgetLayout;
  scrollWidget->setLayout(centralWidgetLayout = new BorderLayout());
  QWidget*header = new QWidget();
  header->setLayout(new QVBoxLayout); //(header, BoxLayout.Y_AXIS));
  QWidget* panel11 = new QWidget();
  panel11->setLayout(new QHBoxLayout());
  if (boundaryFromMenu)
  {
   QLabel* block1NameLabel = new QLabel( tr("Block")+" 1 "+
       tr("Name")+" : "+boundary->getConnect1()->getLayoutBlock()->getId());
   panel11->layout()->addWidget(block1NameLabel);
  }
  else
  {
   QLabel* block1NameLabel = new QLabel( tr("Block")+" 1 "+
                                                   tr("Name")+" : ");
   panel11->layout()->addWidget(block1NameLabel);
   panel11->layout()->addWidget(block1NameField);
   block1NameField->setToolTip(tr("Enter name of one block where signal masts are needed at block boundary."));
  }
  header->layout()->addWidget(panel11);
  QWidget* panel12 = new QWidget();
  panel12->setLayout(new QHBoxLayout());
  if ((boundaryFromMenu) && (boundary->getType()==PositionablePoint::ANCHOR))
  {
   QLabel* block2NameLabel = new QLabel( tr("Block")+" 2 "+
       tr("Name")+" : "+boundary->getConnect2()->getLayoutBlock()->getId());
   panel12->layout()->addWidget(block2NameLabel);
  }
  else if (boundary->getType()==PositionablePoint::ANCHOR)
  {
   QLabel* block2NameLabel = new QLabel( tr("Block")+" 2 "+
                                                   tr("Name")+" : ");
   panel12->layout()->addWidget(block2NameLabel);
   panel12->layout()->addWidget(block2NameField);
   block2NameField->setToolTip(tr("Enter name of one block where signal masts are needed at block boundary."));
  }

  header->layout()->addWidget(panel12);
  header->layout()->addWidget(new JSeparator(JSeparator::HORIZONTAL));
  ((BorderLayout*)centralWidgetLayout)->addWidget(header, BorderLayout::North);

  QWidget*main = new QWidget();
  main->setLayout(new QVBoxLayout);//(main, BoxLayout.Y_AXIS));

  QWidget* panel2 = new QWidget();
  panel2->setLayout(new QHBoxLayout());
  QLabel* shTitle = new QLabel(tr("Signal Masts"));
  panel2->layout()->addWidget(shTitle);
  panel2->layout()->addWidget(new QLabel("   "));
  panel2->layout()->addWidget(getAnchorSavedSignalMasts = new QPushButton(tr("Get Saved")));
//        getAnchorSavedSignalMasts->layout()->addWidgetActionListener(new ActionListener();
//        {
//                /*public*/ void actionPerformed(ActionEvent e) {
//                    getSavedAnchorSignalMasts(e);
//                }
//            });
  connect(getAnchorSavedSignalMasts, SIGNAL(clicked()), this, SLOT(getSavedAnchorSignalMasts()));
  getAnchorSavedSignalMasts->setToolTip( "Click to retrieve signal heads previously stored." );
  if(boundary->getType()!=PositionablePoint::END_BUMPER)
      main->layout()->addWidget(panel2);

  if(boundary->getType()!=PositionablePoint::END_BUMPER)
  {
   eastSignalMast->setBoundaryTitle(tr("East/SouthBound"));
   if (boundaryFromMenu)
   {
       if(isAtWestEndOfAnchor(boundary->getConnect1(), boundary)){
           eastSignalMast->setBoundaryLabelText("Protecting Block : " + boundary->getConnect2()->getLayoutBlock()->getDisplayName());
       }
       else {
           eastSignalMast->setBoundaryLabelText("Protecting Block : " + boundary->getConnect1()->getLayoutBlock()->getDisplayName());
       }
   }
   //eastSignalMast->getDetailsPanel()->setBackground(new Color(255,255,200));
   eastSignalMast->getDetailsPanel()->setStyleSheet("background: {color rgb(255,255,200)}");

   main->layout()->addWidget(eastSignalMast->getDetailsPanel());

   westSignalMast->setBoundaryTitle(tr("West/NorthBound"));
   if (boundaryFromMenu){
       if(isAtWestEndOfAnchor(boundary->getConnect1(), boundary)){
           westSignalMast->setBoundaryLabelText("Protecting Block : " + boundary->getConnect1()->getLayoutBlock()->getDisplayName());
       } else {
           westSignalMast->setBoundaryLabelText("Protecting Block : " + boundary->getConnect2()->getLayoutBlock()->getDisplayName());
       }
   }
   //westSignalMast->getDetailsPanel()->setBackground(new Color(200,255,255));
   westSignalMast->getDetailsPanel()->setStyleSheet("background: {color rgb(200,255,255)}");
   main->layout()->addWidget(westSignalMast->getDetailsPanel());
  }
  else
  {
   if(isAtWestEndOfAnchor(boundary->getConnect1(), boundary) && (boundaryFromMenu))
   {
    eastSignalMast->setBoundaryLabelText("End of Block " + boundary->getConnect1()->getLayoutBlock()->getDisplayName());
    //eastSignalMast->getDetailsPanel()->setBackground(new Color(200,255,255));
    eastSignalMast->getDetailsPanel()->setStyleSheet("background: {color rgb(200,255,255)}");
    main->layout()->addWidget(eastSignalMast->getDetailsPanel());
   }
   else if (boundaryFromMenu)
   {
    westSignalMast->setBoundaryLabelText("End of Block " + boundary->getConnect1()->getLayoutBlock()->getDisplayName());
    //westSignalMast->getDetailsPanel()->setBackground(new Color(255,255,200));
    westSignalMast->getDetailsPanel()->setStyleSheet("background: {color rgb(255,255,200)}");
    main->layout()->addWidget(westSignalMast->getDetailsPanel());
   }
  }
  main->layout()->addWidget(new JSeparator(JSeparator::HORIZONTAL));
  ((BorderLayout*)centralWidgetLayout)->addWidget(main, BorderLayout::Center);
  QWidget* panel6 = new QWidget();
  panel6->setLayout(new QHBoxLayout());
  panel6->layout()->addWidget(setSignalMastsAtBoundaryDone = new QPushButton(tr("Done")));
//        setSignalMastsAtBoundaryDone->layout()->addWidgetActionListener(new ActionListener() {
//            /*public*/ void actionPerformed(ActionEvent e) {
//                setSignalMastsAtBoundaryDonePressed(e);
//            }
//        });
  connect(setSignalMastsAtBoundaryDone, SIGNAL(clicked()), this, SLOT(setSignalMastsAtBoundaryDonePressed()));
  setSignalMastsAtBoundaryDone->setToolTip( tr("Click Done to perform tasks requested above and dismiss this dialog.") );
  panel6->layout()->addWidget(setSignalMastsAtBoundaryCancel = new QPushButton(tr("Cancel")));
//        setSignalMastsAtBoundaryCancel->layout()->addWidgetActionListener(new ActionListener() {
//            /*public*/ void actionPerformed(ActionEvent e) {
//                setSignalMastsAtBoundaryCancelPressed(e);
//            }
//        });
  connect(setSignalMastsAtBoundaryCancel, SIGNAL(clicked()), this, SLOT(setSignalMastsAtBoundaryCancelPressed()));
  setSignalMastsAtBoundaryCancel->setToolTip( tr("Click Cancel to dismiss this dialog without making changes.") );
  ((BorderLayout*)centralWidgetLayout)->addWidget(panel6, BorderLayout::South);
//        setSignalMastsAtBoundaryFrame->layout()->addWidgetWindowListener(new java.awt.event.WindowAdapter() {
//            /*public*/ void windowClosing(java.awt.event.WindowEvent e) {
//                setSignalMastsAtBoundaryCancelPressed(nullptr);
//            }
//        });
  if (boundaryFromMenu) getSavedAnchorSignalMasts(nullptr);
 } else if (boundaryFromMenu) {
     getSavedAnchorSignalMasts(nullptr);
 }
 refreshSignalMastAtBoundaryComboBox();
 //setSignalMastsAtBoundaryFrame->setPreferredSize(nullptr);
 setSignalMastsAtBoundaryFrame->pack();
 setSignalMastsAtBoundaryFrame->setVisible(true);
 setSignalMastsAtBoundaryOpen = true;
}

/**
 * Returns the SignalMast corresponding to an entry field in the specified dialog->
 *		This also takes care of UpperCase and trimming of leading and trailing blanks.
 *		If entry is required, and no entry is present, and error message is sent.
 *      An error message also results if a signalMast head with the entered name is not
 *      found in the SignalMastTable.
 */
/*public*/ SignalMast* LayoutEditorTools::getSignalMastFromEntry(QString signalMastName,bool requireEntry,
                JmriJFrame* /*frame*/) {
    QString str = signalMastName;
    if ( (str==nullptr) || (str==("")) ) {
        if (requireEntry) {
//            JOptionPane.showMessageDialog(frame,tr("SignalMastsError5"),
//                                tr("Error"),JOptionPane.ERROR_MESSAGE);
            QMessageBox::critical(setSignalMastsAtBoundaryFrame, tr("Error"), tr("Error - SignalMast name was not entered. Please enter\n                                                                                 a signal mast name for required positions or cancel."));
        }
        return NULL;
    }
    SignalMast* head = ((DefaultSignalMastManager*)InstanceManager::getDefault("SignalMastManager"))->getSignalMast(str);
    if (head==nullptr) {
//        JOptionPane.showMessageDialog(frame,
//                java.text.MessageFormat.format(tr("SignalMastsError4"),
//                    new Object[]{str}), tr("Error"),
//                        JOptionPane.ERROR_MESSAGE);
        QMessageBox::critical(setSignalMastsAtBoundaryFrame, tr("Error"), tr("Error - SignalMast \"%1\" does not exist. Please correct name\nor enter this SignalMast in the SignalMast Table, and try again.").arg(str));
        return NULL ;
    }
    return (head);
}
/**
 * Returns a SignalMast given a name
 */
/*public*/ SignalMast* LayoutEditorTools::getSignalMastFromName(QString str) {
    if ( (str==nullptr) || (str==("")) ) {
        return NULL;
    }
    return  (((SignalMastManager*)InstanceManager::getDefault("SignalMastManager"))->getSignalMast(str));
}

/**
 * Returns true if the specified SignalMast  is assigned to an object
 *		on the panel, regardless of whether an icon is displayed or not
 */
/*public*/ bool LayoutEditorTools::isSignalMastAssignedAnywhere(SignalMast* signalMast)
{
 for (PositionablePoint* po : layoutEditor->getPositionablePoints()) {
     if ((po->getEastBoundSignalMast() != nullptr) && po->getEastBoundSignalMast() == signalMast) {
         return true;
     }
     if ((po->getWestBoundSignalMast() != nullptr) && po->getWestBoundSignalMast() == signalMast) {
         return true;
     }
 }

 for (LayoutTrack* lt : layoutEditor->getLayoutTurnouts()) {
  LayoutTurnout* to = (LayoutTurnout*)lt;
     if ((to->getSignalAMast() != nullptr) && to->getSignalDMast() == signalMast) {
         return true;
     }
     if ((to->getSignalBMast() != nullptr) && to->getSignalDMast() == signalMast) {
         return true;
     }
     if ((to->getSignalCMast() != nullptr) && to->getSignalDMast() == signalMast) {
         return true;
     }
     if ((to->getSignalDMast() != nullptr) && to->getSignalDMast() == signalMast) {
         return true;
     }
 }

 for (LayoutTrack* lt : layoutEditor->getLayoutSlips()) {
  LayoutSlip* to = (LayoutSlip*)lt;
     if ((to->getSignalAMast() != nullptr) && to->getSignalDMast() == signalMast) {
         return true;
     }
     if ((to->getSignalBMast() != nullptr) && to->getSignalDMast() == signalMast) {
         return true;
     }
     if ((to->getSignalCMast() != nullptr) && to->getSignalDMast() == signalMast) {
         return true;
     }
     if ((to->getSignalDMast() != nullptr) && to->getSignalDMast() == signalMast) {
         return true;
     }
 }

 for (LayoutTrack* lt : layoutEditor->getLevelXings()) {
  LevelXing* x = (LevelXing*)lt;
     if ((x->getSignalAMast() != nullptr) && x->getSignalAMast() == signalMast) {
         return true;
     }
     if ((x->getSignalBMast() != nullptr) && x->getSignalAMast() == signalMast) {
         return true;
     }
     if ((x->getSignalCMast() != nullptr) && x->getSignalAMast() == signalMast) {
         return true;
     }
     if ((x->getSignalDMast() != nullptr) && x->getSignalAMast() == signalMast) {
         return true;
     }
 }

 return false;
}
/**
 * Removes the assignment of the specified SignalMast to either a turnout,
 *		a positionable point, or a level crossing wherever it is assigned
 */
/*public*/ void LayoutEditorTools::removeSignalMastAssignment(SignalMast* signalMast)
{
    if (signalMast==nullptr)
        return;
    for (PositionablePoint* po : layoutEditor->getPositionablePoints()) {
        if ((po->getEastBoundSignalMast() != nullptr) && po->getEastBoundSignalMast() == signalMast) {
            po->setEastBoundSignalMast(nullptr);
        }
        if ((po->getWestBoundSignalMast() != nullptr) && po->getWestBoundSignalMast() == signalMast) {
            po->setWestBoundSignalMast(nullptr);
        }
    }

    for (LayoutTrack* lt : layoutEditor->getLayoutTurnouts())
    {
     LayoutTurnout* to = (LayoutTurnout*)lt;
     if ((to->getSignalAMast() != nullptr) && to->getSignalAMast() == signalMast) {
         to->setSignalAMast(nullptr);
     }
     if ((to->getSignalBMast() != nullptr) && to->getSignalBMast() == signalMast) {
         to->setSignalBMast(nullptr);
     }
     if ((to->getSignalCMast() != nullptr) && to->getSignalCMast() == signalMast) {
         to->setSignalCMast(nullptr);
     }
     if ((to->getSignalDMast() != nullptr) && to->getSignalDMast() == signalMast) {
         to->setSignalDMast(nullptr);
     }
 }

 for (LayoutTrack* lt : layoutEditor->getLayoutSlips()) {
  LayoutSlip* to = (LayoutSlip*)lt;
     if ((to->getSignalAMast() != nullptr) && to->getSignalAMast() == signalMast) {
         to->setSignalAMast(nullptr);
     }

     if ((to->getSignalBMast() != nullptr) && to->getSignalBMast() == signalMast) {
         to->setSignalBMast(nullptr);
     }

     if ((to->getSignalCMast() != nullptr) && to->getSignalCMast() == signalMast) {
         to->setSignalCMast(nullptr);
     }

     if ((to->getSignalDMast() != nullptr) && to->getSignalDMast() == signalMast) {
         to->setSignalDMast(nullptr);
     }
 }

 for (LayoutTrack* lt : layoutEditor->getLevelXings()) {
  LevelXing* x = (LevelXing*)lt;
     if ((x->getSignalAMast() != nullptr) && x->getSignalAMast() == signalMast) {
         x->setSignalAMast(nullptr);
     }

     if ((x->getSignalBMast() != nullptr) && x->getSignalBMast() == signalMast) {
         x->setSignalBMast(nullptr);
     }

     if ((x->getSignalCMast() != nullptr) && x->getSignalCMast() == signalMast) {
         x->setSignalCMast(nullptr);
     }

     if ((x->getSignalDMast() != nullptr) && x->getSignalDMast() == signalMast) {
         x->setSignalDMast(nullptr);
     }
 }
}
/**
 * Removes the SignalMast with the specified name from the panel and from
 *		assignment to any turnout, positionable point, or level crossing
 */
//@SuppressWarnings("NULL")
/*public*/ void LayoutEditorTools::removeSignalMastFromPanel(QString signalMastName) {
    if ( (signalMastName==nullptr) || (signalMastName.length()<1) ) return;
    SignalMast* signalMast = ((SignalMastManager*)InstanceManager::getDefault("SignalMastManager"))->
                                                    getSignalMast(signalMastName);
    removeSignalMastAssignment(signalMast);
    SignalMastIcon* h = NULL;
    int index = -1;
    for (int i=0;(i<layoutEditor->signalMastList->size())&&(index==-1);i++) {
        h = layoutEditor->signalMastList->at(i);
        if (h->getSignalMast() == signalMast) {
            index = i;
        }
    }
    if (index!=(-1)) {
        layoutEditor->signalMastList->remove(index);
        h->remove();
        h->dispose();
        needRedraw = true;
    }
}

/*private*/ void LayoutEditorTools::getSavedAnchorSignalMasts (ActionEvent* /*a*/) {
    if ( !getSimpleBlockInformation() ) return;
    eastSignalMast->setTextField(boundary->getEastBoundSignalMastName());
    westSignalMast->setTextField(boundary->getWestBoundSignalMastName());
    if(boundary->getType()!=PositionablePoint::END_BUMPER){
        if(isAtWestEndOfAnchor(boundary->getConnect1(), boundary)){
            eastSignalMast->setBoundaryLabelText("Protecting Block : " + boundary->getConnect2()->getLayoutBlock()->getDisplayName());
        }
        else {
            eastSignalMast->setBoundaryLabelText("Protecting Block : " + boundary->getConnect1()->getLayoutBlock()->getDisplayName());
        }
        if(isAtWestEndOfAnchor(boundary->getConnect1(), boundary)){
            westSignalMast->setBoundaryLabelText("Protecting Block : " + boundary->getConnect1()->getLayoutBlock()->getDisplayName());
        } else {
            westSignalMast->setBoundaryLabelText("Protecting Block : " + boundary->getConnect2()->getLayoutBlock()->getDisplayName());
        }
    } else {
        if(isAtWestEndOfAnchor(boundary->getConnect1(), boundary)){
            westSignalMast->setBoundaryLabelText("End of Block " + boundary->getConnect1()->getLayoutBlock()->getDisplayName());
        } else{
            eastSignalMast->setBoundaryLabelText("End of Block " + boundary->getConnect1()->getLayoutBlock()->getDisplayName());
        }
    }
//    setSignalMastsAtBoundaryFrame->setPreferredSize(nullptr);
//    setSignalMastsAtBoundaryFrame.pack();
}

/*private*/ void LayoutEditorTools::setSignalMastsAtBoundaryCancelPressed (ActionEvent* /*a*/) {
    setSignalMastsAtBoundaryOpen = false;
    boundaryFromMenu = false;
    setSignalMastsAtBoundaryFrame->setVisible(false);
}

void LayoutEditorTools::refreshSignalMastAtBoundaryComboBox(){
    createListUsedSignalMasts();
    usedMasts.removeAt(usedMasts.indexOf(eastSignalMast->getBean()));
    usedMasts.removeAt(usedMasts.indexOf(westSignalMast->getBean()));
    eastSignalMast->getCombo()->excludeItems(usedMasts);
    westSignalMast->getCombo()->excludeItems(usedMasts);
}

/*private*/ void LayoutEditorTools::setSignalMastsAtBoundaryDonePressed (ActionEvent* /*a*/) // SLOT[]
{
 if ( !getSimpleBlockInformation() ) return;
 SignalMast* oldBlock1SignalMast = getSignalMastFromName(boundary->getEastBoundSignalMastName());
 SignalMast* oldBlock2SignalMast = getSignalMastFromName(boundary->getWestBoundSignalMastName());
 SignalMast* block1BoundSignalMast = getSignalMastFromEntry(eastSignalMast->getText(),false,setSignalMastsAtBoundaryFrame);
 SignalMast* block2BoundSignalMast = getSignalMastFromEntry(westSignalMast->getText(),false,setSignalMastsAtBoundaryFrame);

 if(block1BoundSignalMast==nullptr)
 {
  if(static_cast<LayoutBlockManager*>(InstanceManager::getDefault("LayutBlockManager"))->isAdvancedRoutingEnabled() && ((DefaultSignalMastLogicManager*)InstanceManager::signalMastLogicManagerInstance())->isSignalMastUsed(oldBlock1SignalMast))
  {
   SignallingGuiTools::removeSignalMastLogic(setSignalMastsAtBoundaryFrame, oldBlock1SignalMast);
  }

  removeSignalMastFromPanel(boundary->getEastBoundSignalMastName());
  removeSignalMastAssignment(((DefaultSignalMastManager*)InstanceManager::getDefault("SignalMastManager"))->getSignalMast(boundary->getEastBoundSignalMastName()));
  boundary->setEastBoundSignalMast("");
 }
 if(block2BoundSignalMast==nullptr)
 {
  if(static_cast<LayoutBlockManager*>(InstanceManager::getDefault("LayutBlockManager"))->isAdvancedRoutingEnabled() && ((DefaultSignalMastLogicManager*)InstanceManager::signalMastLogicManagerInstance())->isSignalMastUsed(oldBlock2SignalMast))
  {
   SignallingGuiTools::removeSignalMastLogic(setSignalMastsAtBoundaryFrame, oldBlock2SignalMast);
  }

  removeSignalMastFromPanel(boundary->getWestBoundSignalMastName());
  removeSignalMastAssignment(((DefaultSignalMastManager*)InstanceManager::getDefault("SignalMastManager"))->getSignalMast(boundary->getWestBoundSignalMastName()));
  boundary->setWestBoundSignalMast("");
 }
 if(block2BoundSignalMast!=NULL && block1BoundSignalMast!=nullptr)
 {
  if(block1BoundSignalMast == block2BoundSignalMast)
  {
//            JOptionPane.showMessageDialog(setSignalMastsAtBoundaryFrame,
//                tr("SignalMastsError14"),
//                tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(setSignalMastsAtBoundaryFrame, tr("Error"), tr("Error - Can not assign the same signal mast in both directions"));
   return;
  }
  if(oldBlock1SignalMast==block2BoundSignalMast && oldBlock2SignalMast==block1BoundSignalMast)
  {
   //We are going for a swap!
   //Need to remove old items first
   removeSignalMastFromPanel(boundary->getWestBoundSignalMastName());
   removeSignalMastFromPanel(boundary->getEastBoundSignalMastName());
   removeSignalMastAssignment(block1BoundSignalMast);
   removeSignalMastAssignment(block2BoundSignalMast);
   //Then place new ones
   SignalMastIcon* l;
   if (eastSignalMast->addToPanel())
   {
    l = new SignalMastIcon(layoutEditor);
    l->setSignalMast(eastSignalMast->getText());
    placeEastBoundIcon(l, eastSignalMast->isRightSelected(), 0);
   }
   if (westSignalMast->addToPanel())
   {
    l = new SignalMastIcon(layoutEditor);
    l->setSignalMast(westSignalMast->getText());
    placeWestBoundIcon(l, westSignalMast->isRightSelected(), 0);
   }
   boundary->setEastBoundSignalMast(eastSignalMast->getText());
   boundary->setWestBoundSignalMast(westSignalMast->getText());
   //Then sort out the logic
   if(static_cast<LayoutBlockManager*>(InstanceManager::getDefault("LayutBlockManager"))->isAdvancedRoutingEnabled())
   {
    SignallingGuiTools::swapSignalMastLogic(setSignalMastsAtBoundaryFrame, block1BoundSignalMast, block2BoundSignalMast);
   }
   needRedraw = true;
  }
 }
 if(!needRedraw)
 {
  if(block1BoundSignalMast!=nullptr)
  {
   if (eastSignalMast->addToPanel())
   {
    if (isSignalMastAssignedAnywhere(block1BoundSignalMast) &&
                        (block1BoundSignalMast!=oldBlock1SignalMast))
    {
//                    JOptionPane.showMessageDialog(setSignalMastsAtBoundaryFrame,
//                        java.text.MessageFormat.format(tr("SignalMastsError6"),
//                            new Object[]{eastSignalMast->getText()}),
//                                tr("Error"),JOptionPane.ERROR_MESSAGE);
     QMessageBox::critical(setSignalMastsAtBoundaryFrame, tr("Error"), tr("Error - Cannot place signal mast \"%1}\" on the panel\n                                                                                         because it is already on the panel.").arg(eastSignalMast->getText()));
     return;
    }
    else
    {
     removeSignalMastFromPanel(boundary->getEastBoundSignalMastName());
     SignalMastIcon* l = new SignalMastIcon(layoutEditor);
     l->setSignalMast(eastSignalMast->getText());
     placeEastBoundIcon(l, eastSignalMast->isRightSelected(), 0);
     removeSignalMastAssignment(block1BoundSignalMast);
     boundary->setEastBoundSignalMast(eastSignalMast->getText());
     needRedraw = true;
    }
   }
   else if ((block1BoundSignalMast!=getSignalMastFromName(boundary->getEastBoundSignalMastName())) &&
    (block1BoundSignalMast!=getSignalMastFromName(boundary->getWestBoundSignalMastName())) )
   {
    if (isSignalMastOnPanel(block1BoundSignalMast))
    {
    //                    JOptionPane.showMessageDialog(setSignalMastsAtBoundaryFrame,
    //                        java.text.MessageFormat.format(tr("SignalMastsError13"),
    //                            new Object[]{eastSignalMast->getText()}),
    //                                tr("Error"),JOptionPane.ERROR_MESSAGE);
     QMessageBox::critical(setSignalMastsAtBoundaryFrame, tr("Error"), tr("Error - Cannot assign signal mast \"%1\" to this block boundary\nbecause it is already on the panel at a different place.").arg(eastSignalMast->getText()));
     return;
    }
    else
    {
     removeSignalMastFromPanel(boundary->getEastBoundSignalMastName());
     removeSignalMastAssignment(block1BoundSignalMast);
     boundary->setEastBoundSignalMast(eastSignalMast->getText());
    }
   }
  }
  if(block2BoundSignalMast!=nullptr)
  {
   if (westSignalMast->addToPanel() )
   {
    if (isSignalMastAssignedAnywhere(block2BoundSignalMast) &&
                        (block2BoundSignalMast!=oldBlock2SignalMast))
    {
//                    JOptionPane.showMessageDialog(setSignalMastsAtBoundaryFrame,
//                        java.text.MessageFormat.format(tr("SignalMastsError6"),
//                            new Object[]{westSignalMast->getText()}),
//                                tr("Error"),JOptionPane.ERROR_MESSAGE);
     QMessageBox::critical(setSignalMastsAtBoundaryFrame, tr("Error"), tr("Error - Cannot place signal mast \"%1}\" on the panel\n                                                                                         because it is already on the panel.").arg(westSignalMast->getText()));
     return;
    }
    else /*(oldBlock2SignalMast!=block2BoundSignalMast)*/
    {
     removeSignalMastFromPanel(boundary->getWestBoundSignalMastName());
     SignalMastIcon* l = new SignalMastIcon(layoutEditor);
     l->setSignalMast(westSignalMast->getText());
     placeWestBoundIcon(l, westSignalMast->isRightSelected(), 0);
     removeSignalMastAssignment(block2BoundSignalMast);
     boundary->setWestBoundSignalMast(westSignalMast->getText());
     needRedraw = true;
    }
   }
   else if ( (block2BoundSignalMast!=getSignalMastFromName(boundary->getEastBoundSignalMastName())) &&
    (block2BoundSignalMast!=oldBlock2SignalMast) )
   {
    if (isSignalMastAssignedAnywhere(block2BoundSignalMast))
    {
                    //Need to do this better, so that the signalMast can be on panel multiple times but only alocated to one anchor at a time
//                    JOptionPane.showMessageDialog(setSignalMastsAtBoundaryFrame,
//                        java.text.MessageFormat.format(tr("SignalMastsError13"),
//                            new Object[]{westSignalMast->getText()}),
//                                tr("Error"),JOptionPane.ERROR_MESSAGE);
     QMessageBox::critical(setSignalMastsAtBoundaryFrame, tr("Error"), tr("Error - Cannot assign signal mast \"%1\" to this block boundary\nbecause it is already on the panel at a different place.").arg(westSignalMast->getText()));
     return;
    }
    else
    {
     removeSignalMastFromPanel(boundary->getWestBoundSignalMastName());
     removeSignalMastAssignment(block2BoundSignalMast);
     boundary->setWestBoundSignalMast(westSignalMast->getText());
    }
   }
  }

    //If advanced routing is enabled and then this indicates that we are using this for discovering the signalmast logic paths.
  if(static_cast<LayoutBlockManager*>(InstanceManager::getDefault("LayutBlockManager"))->isAdvancedRoutingEnabled() && (block1BoundSignalMast!=NULL || block2BoundSignalMast!=nullptr))
  {
   updateBoundaryBasedSignalMastLogic(oldBlock1SignalMast, oldBlock2SignalMast,
                                                        block1BoundSignalMast,block2BoundSignalMast);
  }
 }
 setSignalMastsAtBoundaryOpen = false;

 setSignalMastsAtBoundaryFrame->setVisible(false);
 if (needRedraw)
 {
  layoutEditor->redrawPanel();
  needRedraw = false;
  layoutEditor->setDirty();
 }
}

/*public*/ void LayoutEditorTools::updateBoundaryBasedSignalMastLogic(SignalMast* oldBlock1SignalMast, SignalMast* oldBlock2SignalMast,
                                                    SignalMast* block1BoundSignalMast,SignalMast* block2BoundSignalMast){
    SignalMastLogicManager* smlm = InstanceManager::signalMastLogicManagerInstance();
    bool old1Used = ((DefaultSignalMastLogicManager*)smlm)->isSignalMastUsed(oldBlock1SignalMast);
    bool old2Used = ((DefaultSignalMastLogicManager*)smlm)->isSignalMastUsed(oldBlock2SignalMast);
    //Just check that the old ones are used in logics somewhere.
    if(old1Used || old2Used){
        bool new1Used = ((DefaultSignalMastLogicManager*)smlm)->isSignalMastUsed(block1BoundSignalMast);
        bool new2Used = ((DefaultSignalMastLogicManager*)smlm)->isSignalMastUsed(block2BoundSignalMast);
        if(new1Used || new2Used) {
            if((new1Used) && (block1BoundSignalMast!=oldBlock1SignalMast)){
                SignallingGuiTools::removeAlreadyAssignedSignalMastLogic(setSignalMastsAtBoundaryFrame, block1BoundSignalMast);
            }
            if((new2Used) && (block2BoundSignalMast!=oldBlock2SignalMast)){
                SignallingGuiTools::removeAlreadyAssignedSignalMastLogic(setSignalMastsAtBoundaryFrame, block2BoundSignalMast);
            }
        }
        if(block1BoundSignalMast!=nullptr){
            if (oldBlock2SignalMast!=NULL && old2Used &&
                  oldBlock2SignalMast==block1BoundSignalMast){
                SignallingGuiTools::updateSignalMastLogic(setSignalMastsAtBoundaryFrame, oldBlock2SignalMast, block1BoundSignalMast);
            }

            if(oldBlock1SignalMast!=NULL && old1Used &&
                  oldBlock1SignalMast!=block1BoundSignalMast){

                SignallingGuiTools::updateSignalMastLogic(setSignalMastsAtBoundaryFrame, oldBlock1SignalMast, block1BoundSignalMast);
            }
        }
        if(block2BoundSignalMast!=nullptr){
            if(oldBlock1SignalMast!=NULL && old1Used &&
                  oldBlock1SignalMast==block2BoundSignalMast){

                SignallingGuiTools::updateSignalMastLogic(setSignalMastsAtBoundaryFrame, oldBlock1SignalMast, block2BoundSignalMast);
            }
            if (oldBlock2SignalMast!=NULL && old2Used &&
                    oldBlock2SignalMast!=block2BoundSignalMast){
                SignallingGuiTools::updateSignalMastLogic(setSignalMastsAtBoundaryFrame, oldBlock2SignalMast, block2BoundSignalMast);
            }
        }
    }
}

/*public*/ void LayoutEditorTools::setIconOnPanel(PositionableIcon* l, int rotation,
                QPoint p) {
    ((Positionable*)l)->setLocation((int)p.x(), (int)p.y());
    if (rotation>0) {
        l->rotate(rotation);
    }
    if(qobject_cast<SignalMastIcon*>(l)!=nullptr){
        layoutEditor->putSignalMast((SignalMastIcon*) l);
    } else if (qobject_cast<SensorIcon*>(l)!=nullptr){
        layoutEditor->putSensor((SensorIcon*) l);
    } else if (qobject_cast<SignalHeadIcon*>(l)!=nullptr){
        layoutEditor->putSignal((SignalHeadIcon*) l);
    }
}

/*public*/ void LayoutEditorTools::setIconOnPanel(PositionableIcon* l, int rotation,
                int xLoc, int yLoc) {
    ((Positionable*)l)->setLocation(xLoc,yLoc);
    if (rotation>0) {
        l->rotate(rotation);
    }
    if(qobject_cast<SignalMastIcon*>(l)!=nullptr){
        layoutEditor->putSignalMast((SignalMastIcon*) l);
    } else if (qobject_cast<SensorIcon*>(l)!=nullptr){
        layoutEditor->putSensor((SensorIcon*) l);
    } else if (qobject_cast<SignalHeadIcon*>(l)!=nullptr){
        layoutEditor->putSignal((SignalHeadIcon*) l);
    }
}

/*private*/ void LayoutEditorTools::placeEastBoundIcon(PositionableIcon* icon, bool right, double fromPoint) {

    QPointF p = boundary->getCoordsCenter();

    //Track segment is used to determine the alignment, therefore this is opposite to the block that we are protecting
    TrackSegment* t = boundary->getConnect2();
    bool dir = true;
    if(boundary->getType()==PositionablePoint::END_BUMPER){
        t = boundary->getConnect1();
    }
    else {
        if(isAtWestEndOfAnchor(boundary->getConnect1(), boundary)){
            t = boundary->getConnect1();
        }
    }

    QPointF pt2;
    if(t->getConnect1()==boundary){
        pt2 = layoutEditor->getCoords(t->getConnect2(),t->getType2());
    } else {
        pt2 = layoutEditor->getCoords(t->getConnect1(),t->getType1());
    }
    setIconOnPanel(t, icon, dir, QPoint(p.x(), p.y()), QPoint(pt2.x(),pt2.y()), right, fromPoint);

}

/*private*/ void LayoutEditorTools::placeWestBoundIcon(PositionableIcon* icon, bool right, double fromPoint) {

    QPointF p = boundary->getCoordsCenter();

    //Track segment is used to determine the alignment, therefore this is opposite to the block that we are protecting
    TrackSegment* t = boundary->getConnect1();
    bool dir = false;
    if(boundary->getType()!=PositionablePoint::END_BUMPER){
        if(isAtWestEndOfAnchor(boundary->getConnect1(), boundary)){
            t = boundary->getConnect2();
        }
    }

    QPointF pt2;
    if(t->getConnect1()==boundary){
        pt2 = layoutEditor->getCoords(t->getConnect2(),t->getType2());
    } else {
        pt2 = layoutEditor->getCoords(t->getConnect1(),t->getType1());
    }
    setIconOnPanel(t, icon, dir, QPoint(p.x(), p.y()), QPoint(pt2.x(),pt2.y()), right, fromPoint);

}

void LayoutEditorTools::setIconOnPanel(TrackSegment* /*t*/, PositionableIcon* l, bool eastbound, QPoint p, QPoint pt2, bool side, double fromPoint){

    QPoint pt1 = p;

    double pt1x;
    double pt1y;
    pt1x = pt1.x();
    pt1y = pt1.y();

    double pt2x;
    double pt2y;
    pt2x = pt2.x();
    pt2y = pt2.y();


    double triX = pt2x-pt1x;
    double triY = pt2y-pt1y;
    double toDegrees = 57.2957795;

    log->debug(QString("X") + QString::number(triX) + " Y" + QString::number(triY));
    QPoint loc =  QPoint(0,0);
    if(triX==0.0)
    {
        //In a vertical Striaght Line
        if(eastbound){
            loc = northToSouth(p, l, side, fromPoint);
        } else {
            loc = southToNorth(p, l, side, fromPoint);
        }
    }
    else if(triY==0.0)
    {
        //In a Horizontal Straight Line
        if(eastbound){
            loc = westToEast(p, l, side, fromPoint);
        } else {
            loc = eastToWest(p, l, side, fromPoint);
        }
    }
    else
    {
        double a;
        double o;
        // Compute arc's chord
        a = pt2x - pt1x;
        o = pt2y - pt1y;
        double radius=qSqrt(((a*a)+(o*o)));  //chord equates to radius of circle

        double pt1xa;
        double pt1ya;
        pt1xa=pt1x+radius;
        pt1ya=pt1y;
        double a1;
        double o1;
        a1 = pt2x - pt1xa;
        o1 = pt2y - pt1ya;
        double chord=qSqrt(((a1*a1)+(o1*o1)));

        double rsq = qPow(radius,2);

        double anglefromdatum = qAcos((rsq+rsq-qPow(chord, 2))/(2*radius*radius));

        log->debug("radius " + QString::number(radius) + " Chord " + QString::number(chord));
        log->debug("Angle from datum line " + /*Math.toDegrees*/QString::number((anglefromdatum)*toDegrees));
        double tanx = o / a;

        double angletan = qAtan(tanx);

        int oldHeight = l->maxHeight();
        int oldWidth = l->maxWidth();

        int rotate = ((int) toDegrees*(anglefromdatum));
        log->debug(QString::number(toDegrees*(angletan))+ " " + a + " " + o + " " + QString::number(toDegrees*(tanx)));


        //pt1 is always our boundary point
        //East side
        if(pt2x>pt1x){
        //East Sides
            if(pt2y>pt1y){
                //"South East Corner"
                rotate = rotate + 270;  //Correct for SM111, sm101, sm121, SM80
                l->rotate(rotate);
                loc = southEastToNorthWest(p, l, oldWidth, oldHeight, rotate, side, fromPoint);
            } else {
                //"North East corner" //correct for sm110, sm70, sm131
                rotate = 270-rotate;
                l->rotate(rotate);
                loc = northEastToSouthWest(p, l, oldWidth, oldHeight, rotate, side, fromPoint);
            }

        } else {
            //West Side
            if(pt2y>pt1y){
                //South West //WORKING FOR SM141, sm130, SM71
                l->rotate(rotate-90);
                //South West
                loc = southWestToNorthEast(p, l, oldWidth, oldHeight, rotate, side, fromPoint);
            } else {
                //North West //Working FOR SM140, SM81, sm120
                rotate = (180-rotate)+90;
                l->rotate(rotate);
                loc = northWestToSouthEast(p, l, oldWidth, oldHeight, rotate, side, fromPoint);
            }
        }
    }
    setIconOnPanel(l, 0, loc);
}

QPoint LayoutEditorTools::southToNorth(QPoint p, PositionableIcon* l, bool right, double fromPoint){
    int offsetx=0;
    int offsety=(int)(p.y()+offSetFromPoint+fromPoint);
    if (right){
        offsetx = (int)p.x()+offSetFromPoint;
    } else {
        offsetx = (int)p.x()-offSetFromPoint-l->maxWidth();
    }
    return  QPoint(offsetx, offsety);
}

QPoint LayoutEditorTools::northToSouth(QPoint p, PositionableIcon* l, bool right, double fromPoint){
    l->rotate(180);
    int offsetx=0;
    int offsety=(int)(p.y()-(offSetFromPoint+fromPoint)-l->maxHeight());
    if(right){
        offsetx = (int)p.x()-offSetFromPoint-l->maxWidth();
    } else {
        offsetx = (int)p.x()+offSetFromPoint;
    }
    return  QPoint(offsetx, offsety);
}

QPoint LayoutEditorTools::westToEast(QPoint p, PositionableIcon* l, bool right, double fromPoint){
    l->rotate(90);
    int offsetx=(int)(p.x()-(l->maxWidth()+(offSetFromPoint+fromPoint-1)));
    int offsety=0;
    if(right){
        offsety = (int)p.y()+(offSetFromPoint-1);
    } else {
        offsety = (int)p.y() - (offSetFromPoint) - l->maxHeight();
    }
    return  QPoint(offsetx, offsety);
}

QPoint LayoutEditorTools::eastToWest(QPoint p, PositionableIcon* l, bool right, double fromPoint){
    l->rotate(-90);
    int offsetx=(int)(p.x()+offSetFromPoint+fromPoint);
    int offsety=0;
    if(right){
        offsety = (int)p.y() - (offSetFromPoint-1) - l->maxHeight();

    } else {
        offsety = (int)p.y()+(offSetFromPoint);
    }
    return  QPoint(offsetx, offsety);
}

/**
* come back to this as its a bit tight to the rail on SM110 need re checking
*/
QPoint LayoutEditorTools::northEastToSouthWest(QPoint p, PositionableIcon* l, int oldWidth, int oldHeight, double angle, bool right, double fromPoint){
    angle = angle-180;
    if (angle<45){
        //Because of the angle things get shifted about.
        int tmpWidth = oldWidth;
        int tmpHeight=oldHeight;
        oldWidth=tmpHeight;
        oldHeight=tmpWidth;
    }
    double toDegrees = 57.2957795;
    double ang = angle;
    double oppAng = 90-ang;
    angle = /*Math.toRadians*/(angle)/toDegrees;
    double oppAngRad = /*Math.toRadians*/(oppAng)/toDegrees;
    double iconAdj = qSin(angle)*oldHeight;
    double iconAdjOpp = qSin(oppAngRad)*oldHeight;
    double bpa = qSin(angle)*(offSetFromPoint+fromPoint);
    double bpo = qSin(oppAngRad)*(offSetFromPoint+fromPoint);
    double ta = qSin(angle)*offSetFromPoint;
    double to = qSin(oppAngRad)*offSetFromPoint;

    if(log->isDebugEnabled()){
        log->debug("north east to south west " + QString::number(angle));
        log->debug("oldWidth " + QString::number(oldWidth) + " oldHeight " + QString::number(oldHeight));
        log->debug("newWidth " + QString::number(l->maxWidth()) + " newHeight " + QString::number(l->maxHeight()));
        log->debug("Icon adj: " + QString::number(iconAdj) + " opp adj: " + QString::number(iconAdjOpp));
        log->debug("boundary point opp " + QString::number(bpo));
        log->debug("boundary point adj " + QString::number(bpa));
        log->debug("track opp " + QString::number(to));
        log->debug("track adj " + QString::number(ta));
    }
    int xpos =0;
    int ypos = 0;
    if (right){
        //double x_dist_to_Icon = (l.maxWidth()-iconAdj)-(bpa-bpo);
        //double y_dist_to_Icon = bpa+bpo+l.maxHeight();

        double x_dist_to_Icon = (iconAdjOpp)-(bpa-to);
        double y_dist_to_Icon = ta+bpo+l->maxHeight();

        log->debug("x dist " + QString::number(x_dist_to_Icon) + ", y dist " + QString::number(y_dist_to_Icon));

        xpos = (int) (p.x()-x_dist_to_Icon);
        ypos = (int) (p.y()-y_dist_to_Icon);

    } else {
        double y_dist_to_Icon = iconAdjOpp+(bpo-ta);
        double x_dist_to_Icon = to+bpa;
        //double y_dist_to_Icon = (l.maxHeight()-iconAdj)-(ta-bpo);
        //double x_dist_to_Icon = bpa+to;
        log->debug("x dist " + QString::number(x_dist_to_Icon) + ", y dist " + QString::number(y_dist_to_Icon));

        xpos = (int) (p.x()+x_dist_to_Icon);
        ypos = (int) (p.y()-y_dist_to_Icon);

    }
    if(log->isDebugEnabled()){
        log->debug("xpos " + xpos);
        log->debug("yPos " + ypos);
    }
    return  QPoint(xpos, ypos);

}

QPoint LayoutEditorTools::southWestToNorthEast(QPoint p, PositionableIcon* l, int oldWidth, int oldHeight, double angle, bool right, double fromPoint){
    angle = 180-angle;


    double oppAng = angle;
    double ang = 90-oppAng;

    //Because of the angle things get shifted about.
    if (ang<45){ //was angle
        int tmpWidth = oldWidth;
        int tmpHeight=oldHeight;
        oldWidth=tmpHeight;
        oldHeight=tmpWidth;
    }
    double toDegrees = 57.2957795;
    ang = (ang)/toDegrees;
    double oppAngRad = /*Math.toRadians*/(oppAng/toDegrees);
    double iconAdj = qSin(ang)*oldHeight;
    double iconAdjOpp = qSin(oppAngRad)*oldHeight;
    double bpa = qSin(ang)*(offSetFromPoint+fromPoint);  //was angle
    double bpo = qSin(oppAngRad)*(offSetFromPoint+fromPoint);
    double ta = qSin(ang)*offSetFromPoint; //was angle
    double to = qSin(oppAngRad)*offSetFromPoint;

    if(log->isDebugEnabled()){
        log->debug("south west to north east " + QString::number(angle));
        log->debug("oldWidth " + QString::number(oldWidth) + " oldHeight " + QString::number(oldHeight));
        log->debug("newWidth " + QString::number(l->maxWidth()) + " newHeight " + QString::number(l->maxHeight()));
        log->debug("Icon adj: " + QString::number(iconAdj) + " opp adj: " + QString::number(iconAdjOpp));
        log->debug("boundary point opp " +QString::number( bpo));
        log->debug("boundary point adj " + QString::number(bpa));
        log->debug("track opp " + QString::number(to));
        log->debug("track adj " + QString::number(ta));
    }

    int xpos;
    int ypos;

    if(right)
    {
        double x_dist_to_Icon = iconAdj+(bpa-to);
        double y_dist_to_Icon = ta+bpo;
        log->debug("x dist " + QString::number(x_dist_to_Icon) + ", y dist " + QString::number(y_dist_to_Icon));

        xpos = (int) (p.x()-x_dist_to_Icon);
        log->debug("xpos " + xpos);
        ypos = (int) (p.y()+y_dist_to_Icon);
        log->debug("yPos " + ypos);
    }
    else {
        double x_dist_to_Icon = (bpa+to)+l->maxWidth();
        //double y_dist_to_Icon = (iconAdj+(ta-bpo));
        double y_dist_to_Icon = (bpo-ta)-(l->maxHeight()-iconAdjOpp);
        //double y_dist_to_Icon = (iconAdj+(ta-bpo));
        log->debug("x dist " + QString::number(x_dist_to_Icon) + ", y dist " + QString::number(y_dist_to_Icon));
        xpos = (int) (p.x()-x_dist_to_Icon);
        ypos = (int) (p.y()+y_dist_to_Icon);
    }
    if(log->isDebugEnabled()){
        log->debug("xpos " + xpos);
        log->debug("yPos " + ypos);
    }
    return  QPoint(xpos, ypos);

}

//Working FOR SM140, SM81, sm120
QPoint LayoutEditorTools::northWestToSouthEast(QPoint p, PositionableIcon* l, int oldWidth, int oldHeight, double angledeg, bool right, double fromPoint){
    log->debug("angle before " + QString::number(angledeg));
    angledeg=180-angledeg;
    angledeg=90-angledeg;
    log->debug("north west to south east " + QString::number(angledeg));
    if (angledeg<45){
        //Because of the angle things get shifted about.
        int tmpWidth = oldWidth;
        int tmpHeight=oldHeight;
        oldWidth=tmpHeight;
        oldHeight=tmpWidth;
    }
    log->debug("oldWidth " + QString::number(oldWidth) + " oldHeight " + QString::number(oldHeight));
    log->debug("newWidth " + QString::number(l->maxWidth()) + " newHeight " + QString::number(l->maxHeight()));
    //double ang = angle;
    double oppAng = 90-angledeg;
    double angle = /*Math.toRadians*/(angledeg)/57.2957795;
    double oppAngRad = /*Math.toRadians*/(oppAng)/57.2957795;
    double iconAdj = qSin(angle)*oldHeight;
    double iconAdjOpp = qSin(oppAngRad)*oldHeight;

    double bpa = qSin(angle)*(offSetFromPoint+fromPoint);  //distance from point
    double bpo = qSin(oppAngRad)*(offSetFromPoint+fromPoint);
    double ta = qSin(angle)*offSetFromPoint; //distance from track
    double to = qSin(oppAngRad)*offSetFromPoint;

    if(log->isDebugEnabled()){
        log->debug("north west to south east " + QString::number(angledeg));
        log->debug("oldWidth " + QString::number(oldWidth) + " oldHeight " + QString::number(oldHeight));
        log->debug("newWidth " + QString::number(l->maxWidth()) + " newHeight " + QString::number(l->maxHeight()));
        log->debug("Icon adj: " + QString::number(iconAdj) + " opp adj: " + QString::number(iconAdjOpp));
        log->debug("boundary point opp " + QString::number(bpo));
        log->debug("boundary point adj " + QString::number(bpa));
        log->debug("track opp " + QString::number(to));
        log->debug("track adj " + QString::number(ta));
    }
    int xpos = 0;
    int ypos = 0;
    if (right){
        //double x_dist_to_Icon = bpa+bpo+l.maxWidth();
        //double y_dist_to_Icon = bpa-(l.maxHeight()-iconAdj);
        double x_dist_to_Icon = (l->maxWidth()+ta+bpo);
        double y_dist_to_Icon = iconAdj+(bpa-to);


        log->debug("right x dist " + QString::number(x_dist_to_Icon) + ", y dist " + QString::number(y_dist_to_Icon));

        xpos = (int) (p.x()-x_dist_to_Icon);
        ypos = (int) (p.y()-y_dist_to_Icon); //was +
    } else {
        //This still needs to be worked out.
        //double y_dist_to_Icon = bpa+bpo+l.maxHeight();
        //double x_dist_to_Icon = iconAdj+(bpa-bpo);

        double y_dist_to_Icon = l->maxHeight()+bpa+to;//+(l.maxWidth()-iconAdj);
        //double y_dist_to_Icon = bpa-(l.maxHeight()-iconAdj);
        //double y_dist_to_Icon = ta+bpo+l.maxHeight();
        double x_dist_to_Icon = (iconAdjOpp)+(bpo-ta);
        //double x_dist_to_Icon = iconAdj+(bpa-to);
        log->debug("left x dist " + QString::number(x_dist_to_Icon) + ", y dist " + QString::number(y_dist_to_Icon));

        xpos = (int) (p.x()-x_dist_to_Icon);
        ypos = (int) (p.y()-y_dist_to_Icon);
    }
    if(log->isDebugEnabled()){
        log->debug(p.x() + " xpos " + xpos);
        log->debug(p.y() + " yPos " + ypos);
    }
    return  QPoint(xpos, ypos);
}


//Correct for SM111, sm101, sm121, SM80
QPoint LayoutEditorTools::southEastToNorthWest(QPoint p, PositionableIcon* l, int oldWidth, int oldHeight, double angleDeg, bool right, double fromPoint){
    angleDeg = 360-angleDeg;

    if (angleDeg>45){
        //Because of the angle things get shifted about.
        int tmpWidth = oldWidth;
        int tmpHeight=oldHeight;
        oldWidth=tmpWidth;
        oldHeight=tmpHeight;
    }

//        double ang = angle;
    double oppAng = 90-angleDeg;
    double angle = /*Math.toRadians*/(angleDeg)/57.2957795;
    double oppAngRad = /*Math.toRadians*/(oppAng)/57.2957795;
    double iconAdj = qSin(angle)*oldHeight;
    double iconAdjOpp = qSin(oppAngRad)*oldHeight;
    double bpa = qSin(angle)*(offSetFromPoint+fromPoint);
    double bpo = qSin(oppAngRad)*(offSetFromPoint+fromPoint);
    double ta = qSin(angle)*offSetFromPoint; //distance from track
    double to = qSin(oppAngRad)*offSetFromPoint;
    if(log->isDebugEnabled()){
        log->debug("south east to north west " + QString::number(angleDeg));
        log->debug("oldWidth " + QString::number(oldWidth) + " oldHeight " + QString::number(oldHeight));
        log->debug("newWidth " + QString::number(l->maxWidth()) + " newHeight " + QString::number(l->maxHeight()));
        log->debug("Icon adj: " + QString::number(iconAdj) + " opp adj: " + QString::number(iconAdjOpp));
        log->debug("boundary point opp " + QString::number(bpo));
        log->debug("boundary point adj " + QString::number(bpa));
        log->debug("track opp " + QString::number(to));
        log->debug("track adj " + QString::number(ta));
    }
    int xpos = 0;
    int ypos = 0;
    if (right){
        //double x_dist_to_Icon = bpa+bpo;
        //double y_dist_to_Icon = (iconAdj+bpa-bpo);
        double x_dist_to_Icon = bpa+to;
        double y_dist_to_Icon = (bpo-ta)-(l->maxHeight()-iconAdjOpp);

        log->debug(QString::number((bpo-ta)-(l->maxHeight()-iconAdjOpp)));
        log->debug(QString::number(bpo-(iconAdj+ta)));
        /*if(angleDeg<45){
            y_dist_to_Icon = (bpo-ta)-(l.maxHeight()-iconAdjOpp);
        } else {
            y_dist_to_Icon = bpo-(iconAdj+ta);
        }*/
        //double y_dist_to_Icon = (l.maxHeight()-iconAdj)+(bpo-ta);
        xpos = (int) (p.x()+x_dist_to_Icon);
        ypos = (int) (p.y()+y_dist_to_Icon);
        log->debug("right x dist " + QString::number(x_dist_to_Icon) + ", y dist " + QString::number(y_dist_to_Icon));
    }
    else {
        //double x_dist_to_Icon = l.maxWidth()-(iconAdj+(bpa-bpo));
        //double y_dist_to_Icon = bpa+bpo;

        double x_dist_to_Icon = (bpa-to)-(l->maxWidth()-iconAdj);
        double y_dist_to_Icon = bpo+ta;

        xpos = (int) (p.x()+x_dist_to_Icon);
        ypos = (int) (p.y()+y_dist_to_Icon);
        log->debug("left x dist " + QString::number(x_dist_to_Icon) + ", y dist " + QString::number(y_dist_to_Icon));
    }
    if(log->isDebugEnabled()){
        log->debug(QString::number(p.x()) + " xpos " + QString::number(xpos));
        log->debug(QString::number(p.y()) + " yPos " + QString::number(ypos));
    }

    return  QPoint(xpos, ypos);
}


/*public*/ bool LayoutEditorTools::isSignalMastOnPanel(SignalMast* signalMast)
{
    SignalMastIcon* s = NULL;
    for (int i=0;i<layoutEditor->signalMastList->size();i++) {
        s = layoutEditor->signalMastList->at(i);
        if (s->getSignalMast() == signalMast) {
            return true;
        }
    }
    return false;
}


/*public*/ void LayoutEditorTools::setSignalMastsAtTurnoutFromMenu(LayoutTurnout* to, QStringList blocks){
    turnoutMastFromMenu = true;
    layoutTurnout = to;
    turnout = to->getTurnout();
    turnoutMastNameField->setText(to->getTurnoutName());
    turnoutBlocks= QVector<QString>(4);
    for(int i = 0; i<blocks.length();i++){
        turnoutBlocks.replace(i,blocks.at(i));
    }
    setSignalMastsAtTurnouts();
}


void LayoutEditorTools::createListUsedSignalMasts(){
    usedMasts = QList<NamedBean*>();
    for (PositionablePoint* po : layoutEditor->getPositionablePoints()) {
        //We allow the same sensor to be allocated in both directions.
        if (po!=boundary){
            if (po->getEastBoundSignalMast()!=NULL && po->getEastBoundSignalMastName()!=(""))
                usedMasts.append(((DefaultSignalMastManager*)InstanceManager::getDefault("SignalMastManager"))->getSignalMast(po->getEastBoundSignalMastName()));
            if (po->getWestBoundSignalMast()!=NULL && po->getWestBoundSignalMastName()!=(""))
                usedMasts.append(((DefaultSignalMastManager*)InstanceManager::getDefault("SignalMastManager"))->getSignalMast(po->getWestBoundSignalMastName()));
        }
    }

    for (LayoutTurnout* to : layoutEditor->getLayoutTurnouts()) {
        if (to->getSignalAMast()!=NULL && to->getSignalAMastName()!=(""))
            usedMasts.append(((DefaultSignalMastManager*)InstanceManager::getDefault("SignalMastManager"))->getSignalMast(to->getSignalAMastName()));
        if (to->getSignalBMastName()!=NULL && to->getSignalBMastName()!=(""))
            usedMasts.append(((DefaultSignalMastManager*)InstanceManager::getDefault("SignalMastManager"))->getSignalMast(to->getSignalBMastName()));
        if (to->getSignalCMastName()!=NULL && to->getSignalCMastName()!=(""))
            usedMasts.append(((DefaultSignalMastManager*)InstanceManager::getDefault("SignalMastManager"))->getSignalMast(to->getSignalCMastName()));
        if (to->getSignalDMast()!=NULL && to->getSignalDMastName()!=("") )
            usedMasts.append(((DefaultSignalMastManager*)InstanceManager::getDefault("SignalMastManager"))->getSignalMast(to->getSignalDMastName()));
    }
    for (LevelXing* x : layoutEditor->getLevelXings()) {
        if (x->getSignalAMastName()!=NULL && x->getSignalAMastName()!=(""))
            usedMasts.append(((DefaultSignalMastManager*)InstanceManager::getDefault("SignalMastManager"))->getSignalMast(x->getSignalAMastName()));
        if (x->getSignalBMastName()!=NULL && x->getSignalBMastName()!=(""))
            usedMasts.append(((DefaultSignalMastManager*)InstanceManager::getDefault("SignalMastManager"))->getSignalMast(x->getSignalBMastName()));
        if (x->getSignalCMastName()!=NULL && x->getSignalCMastName()!=(""))
            usedMasts.append(((DefaultSignalMastManager*)InstanceManager::getDefault("SignalMastManager"))->getSignalMast(x->getSignalCMastName()));
        if (x->getSignalDMastName()!=NULL && x->getSignalDMastName()!=(""))
            usedMasts.append(((DefaultSignalMastManager*)InstanceManager::getDefault("SignalMastManager"))->getSignalMast(x->getSignalDMastName()));
    }
#if 1
    for (LayoutSlip* sl : layoutEditor->getLayoutSlips()) {
        if (sl->getSignalAMast()!=NULL && sl->getSignalAMastName()!=(""))
            usedMasts.append(((DefaultSignalMastManager*)InstanceManager::getDefault("SignalMastManager"))->getSignalMast(sl->getSignalAMastName()));
        if (sl->getSignalBMast()!=NULL && sl->getSignalBMastName()!=(""))
            usedMasts.append(((DefaultSignalMastManager*)InstanceManager::getDefault("SignalMastManager"))->getSignalMast(sl->getSignalBMastName()));
        if (sl->getSignalCMast()!=NULL && sl->getSignalCMastName()!=(""))
            usedMasts.append(((DefaultSignalMastManager*)InstanceManager::getDefault("SignalMastManager"))->getSignalMast(sl->getSignalCMastName()));
        if (sl->getSignalDMast()!=NULL && sl->getSignalDMastName()!=(""))
            usedMasts.append(((DefaultSignalMastManager*)InstanceManager::getDefault("SignalMastManager"))->getSignalMast(sl->getSignalDMastName()));
    }
#endif
}

void LayoutEditorTools::refreshSignalMastAtTurnoutComboBox(){
    turnoutSignalMastsGetSaved(nullptr);
    createListUsedSignalMasts();
    usedMasts.removeAt(usedMasts.indexOf(turnoutSignalMastA->getBean()));
    usedMasts.removeAt(usedMasts.indexOf(turnoutSignalMastB->getBean()));
    usedMasts.removeAt(usedMasts.indexOf(turnoutSignalMastC->getBean()));
    usedMasts.removeAt(usedMasts.indexOf(turnoutSignalMastD->getBean()));
    turnoutSignalMastA->getCombo()->excludeItems(usedMasts);
    turnoutSignalMastB->getCombo()->excludeItems(usedMasts);
    turnoutSignalMastC->getCombo()->excludeItems(usedMasts);
    turnoutSignalMastD->getCombo()->excludeItems(usedMasts);
}

/*public*/ void LayoutEditorTools::setSignalMastsAtTurnouts(){
    if (setSignalMastsOpen) {
        //We will do a refresh in case the block boundaries have changed.
        turnoutSignalMastsGetSaved(nullptr);
        refreshSignalMastAtTurnoutComboBox();
        signalMastsJmriFrame->setVisible(true);
        return;
    }
    // Initialize if needed
    if (signalMastsJmriFrame == nullptr) {
        signalMastsJmriFrame = new JmriJFrameX(tr("Set Signal Masts at Turnout"), false, true);
        signalMastsJmriFrame->addHelpMenu("package.jmri.jmrit.display.SetSignalMastsAtTurnout", true);
//        signalMastsJmriFrame->setLocation(70,30);
        QWidget* theContentPane = new QWidget();
        signalMastsJmriFrame->setCentralWidget(theContentPane);
        QVBoxLayout* centralWidgetLayout;
        theContentPane->setLayout(centralWidgetLayout = new QVBoxLayout(theContentPane/*, BoxLayout.Y_AXIS*/));
        QWidget* panel1 = new QWidget();
        panel1->setLayout(new QHBoxLayout());
        turnoutSignalMastA = new BeanDetails("Signal Mast", (Manager*)InstanceManager::getDefault("SignalMastManager"));
        turnoutSignalMastB = new BeanDetails("Signal Mast", (Manager*)InstanceManager::getDefault("SignalMastManager"));
        turnoutSignalMastC = new BeanDetails("Signal Mast", (Manager*)InstanceManager::getDefault("SignalMastManager"));
        turnoutSignalMastD= new BeanDetails("Signal Mast", (Manager*)InstanceManager::getDefault("SignalMastManager"));
        if (turnoutMastFromMenu)
        {
            QLabel* turnoutMastNameLabel = new QLabel( tr("Turnout")+" "+
                tr("Name")+" : "+layoutTurnout->getTurnoutName());
            panel1->layout()->addWidget(turnoutMastNameLabel);
            turnoutSignalMastA->setTextField(layoutTurnout->getSignalAMastName());
            turnoutSignalMastB->setTextField(layoutTurnout->getSignalBMastName());
            turnoutSignalMastC->setTextField(layoutTurnout->getSignalCMastName());
            turnoutSignalMastD->setTextField(layoutTurnout->getSignalDMastName());
        }
        else {
            QLabel* turnoutMastNameLabel = new QLabel( tr("Turnout")+" "+
                                                            tr("Name") );
            panel1->layout()->addWidget(turnoutMastNameLabel);
            panel1->layout()->addWidget(turnoutMastNameField);
            turnoutMastNameField->setToolTip(tr("Enter name (system or user) of signal mast"));
        }
        centralWidgetLayout->addWidget(panel1);
        QFrame* line1 = new QFrame();
        line1->setObjectName(QString::fromUtf8("line"));
        line1->setGeometry(QRect(10, 30, 571, 16));
        line1->setFrameShape(QFrame::HLine);
        line1->setFrameShadow(QFrame::Sunken);
        centralWidgetLayout->addWidget(line1);
        QWidget* panel2 = new QWidget();
        panel2->setLayout(new QHBoxLayout());
        QLabel* shTitle = new QLabel(tr("Signal Masts"));
        panel2->layout()->addWidget(shTitle);
        panel2->layout()->addWidget(new QLabel("   "));
        panel2->layout()->addWidget(getSavedSignalMasts = new QPushButton(tr("Get Saved")));
//        getSavedSignalMasts->layout()->addWidgetActionListener(new ActionListener() {
//                /*public*/ void actionPerformed(ActionEvent e) {
//                    turnoutSignalMastsGetSaved(e);
//                }
//            });
        connect(getSavedSignalMasts, SIGNAL(clicked()), this, SLOT(turnoutSignalMastsGetSaved()));
        getSavedSignalMasts->setToolTip( "Click to retrieve signal heads previously stored.");
        centralWidgetLayout->addWidget(panel2);

//        turnoutSignalMastA.getDetailsPanel()->setBackground(new Color(255,255,200));
        turnoutSignalMastA->getDetailsPanel()->setStyleSheet("background: {color rgb(255,255,200)}");

//        turnoutSignalMastB.getDetailsPanel()->setBackground(new Color(200,255,255));
        turnoutSignalMastB->getDetailsPanel()->setStyleSheet("background: {color rgb(200,255,255)}");

//        turnoutSignalMastC.getDetailsPanel()->setBackground(new Color(200,200,255));
        turnoutSignalMastC->getDetailsPanel()->setStyleSheet("background: {color rgb(200,200,255)}");

//        turnoutSignalMastD.getDetailsPanel()->setBackground(new Color(255,200,200));
        turnoutSignalMastD->getDetailsPanel()->setStyleSheet("background: {color rgb(255,200,200)}");

        signalMastTurnoutPanel->setLayout(signalMastTurnoutPanelLayout = new QGridLayout()); //0,2));

        turnoutSignalMastA->setBoundaryLabel(turnoutBlocks.at(0));
        turnoutSignalMastB->setBoundaryLabel(turnoutBlocks.at(1));
        turnoutSignalMastC->setBoundaryLabel(turnoutBlocks.at(2));
        turnoutSignalMastD->setBoundaryLabel(turnoutBlocks.at(3));

        if(turnoutBlocks.at(0)!=nullptr)
            signalMastTurnoutPanelLayout->addWidget(turnoutSignalMastA->getDetailsPanel(),0,0);
        if(turnoutBlocks.at(1)!=nullptr)
            signalMastTurnoutPanelLayout->addWidget(turnoutSignalMastB->getDetailsPanel(),0,1);
        if(turnoutBlocks.at(2)!=nullptr)
            signalMastTurnoutPanelLayout->addWidget(turnoutSignalMastC->getDetailsPanel(),1,0);
        if(turnoutBlocks.at(3)!=nullptr)
            signalMastTurnoutPanelLayout->addWidget(turnoutSignalMastD->getDetailsPanel(),1,1);
        centralWidgetLayout->addWidget(signalMastTurnoutPanel);

        QFrame* line2 = new QFrame();
        line2->setObjectName(QString::fromUtf8("line"));
        line2->setGeometry(QRect(10, 30, 571, 16));
        line2->setFrameShape(QFrame::HLine);
        line2->setFrameShadow(QFrame::Sunken);
        centralWidgetLayout->addWidget(line2);
        QWidget* panel6 = new QWidget();
        panel6->layout()->addWidget(new QLabel("  "));
        panel6->layout()->addWidget(setSignalMastsDone = new QPushButton(tr("Done")));
//        setSignalMastsDone->layout()->addWidgetActionListener(new ActionListener() {
//            /*public*/ void actionPerformed(ActionEvent e) {
//                setSignalMastsDonePressed(e);
//            }
//        });
        connect(setSignalMastsDone, SIGNAL(clicked()), this, SLOT(setSignalMastsDonePressed()));
        setSignalMastsDone->setToolTip( tr("Click Done to perform tasks requested above and dismiss this dialog.") );
        panel6->layout()->addWidget(setSignalMastsCancel = new QPushButton(tr("Cancel")));
//        setSignalMastsCancel->layout()->addWidgetActionListener(new ActionListener() {
//            /*public*/ void actionPerformed(ActionEvent e) {
//                setSignalMastsCancelPressed(e);
//            }
//        });
        setSignalMastsCancel->setToolTip( tr("Click Cancel to dismiss this dialog without making changes.") );
        centralWidgetLayout->addWidget(panel6);
//        signalMastsJmriFrame->layout()->addWidgetWindowListener(new java.awt.event.WindowAdapter() {
//            /*public*/ void windowClosing(java.awt.event.WindowEvent e) {
////					setSignalMastsCancelPressed(nullptr);
//            }
//        });
        if (turnoutFromMenu) turnoutSignalMastsGetSaved(nullptr);
    }
    refreshSignalMastAtTurnoutComboBox();
//    signalMastsJmriFrame->setPreferredSize(nullptr);
//    signalMastsJmriFrame.pack();
    signalMastsJmriFrame->setVisible(true);
    setSignalMastsOpen = true;
}
/*private*/ void LayoutEditorTools::turnoutSignalMastsGetSaved (ActionEvent* /*a*/) // SLOT
{
 if ( !getTurnoutMastInformation() ) return;
 turnoutBlocks = layoutTurnout->getBlockBoundaries().toVector();
 turnoutSignalMastA->setTextField(layoutTurnout->getSignalAMastName());
 turnoutSignalMastB->setTextField(layoutTurnout->getSignalBMastName());
 turnoutSignalMastC->setTextField(layoutTurnout->getSignalCMastName());
 turnoutSignalMastD->setTextField(layoutTurnout->getSignalDMastName());

 turnoutSignalMastA->setBoundaryLabel(turnoutBlocks.at(0));
 turnoutSignalMastB->setBoundaryLabel(turnoutBlocks.at(1));
 turnoutSignalMastC->setBoundaryLabel(turnoutBlocks.at(2));
 turnoutSignalMastD->setBoundaryLabel(turnoutBlocks.at(3));
 signalMastTurnoutPanel->layout()->removeWidget(turnoutSignalMastA->getDetailsPanel());
 signalMastTurnoutPanel->layout()->removeWidget(turnoutSignalMastB->getDetailsPanel());
 signalMastTurnoutPanel->layout()->removeWidget(turnoutSignalMastC->getDetailsPanel());
 signalMastTurnoutPanel->layout()->removeWidget(turnoutSignalMastD->getDetailsPanel());

 bool blockBoundary = false;
 if(turnoutBlocks.at(0)!=nullptr)
 {
  signalMastTurnoutPanelLayout->addWidget(turnoutSignalMastA->getDetailsPanel(),0,0);
  blockBoundary = true;
 }
 if(turnoutBlocks.at(1)!=nullptr)
 {
  signalMastTurnoutPanelLayout->addWidget(turnoutSignalMastB->getDetailsPanel(),0,1);
  blockBoundary = true;
 }
 if(turnoutBlocks.at(2)!=nullptr)
 {
  signalMastTurnoutPanelLayout->addWidget(turnoutSignalMastC->getDetailsPanel(),1,0);
  blockBoundary = true;
 }
 if(turnoutBlocks.at(3)!=nullptr)
 {
  signalMastTurnoutPanelLayout->addWidget(turnoutSignalMastD->getDetailsPanel(),1,1);
  blockBoundary = true;
 }
 if (!blockBoundary)
  //JOptionPane.showMessageDialog(signalMastsAtXingFrame, "There are no block boundaries on this turnout\nIt is therefore not possible to add Signal Masts to it");
  QMessageBox::critical(0, tr("Error"), tr("There are no block boundaries on this turnout\nIt is therefore not possible to add Signal Masts to it"));
 signalMastsJmriFrame->setMaximumSize(QSize());
 signalMastsJmriFrame->adjustSize();
}

/*private*/ int LayoutEditorTools::isMastAssignedHere(SignalMast* mast, LayoutTurnout* lTurnout)
{
 if ((mast==nullptr) || (lTurnout==nullptr))
     return NONE;
 QString sysName = mast->getSystemName();
 QString uName = mast->getUserName();
 QString name = lTurnout->getSignalAMastName();
 if ( (name!=nullptr) && (name.length()>0) && ((name==(uName)) ||
                 (name==(sysName))) ) return A1;
 name = lTurnout->getSignalBMastName();
 if ( (name!=nullptr) && (name.length()>0) && ((name==(uName)) ||
                 (name==(sysName))) ) return A2;
 name = lTurnout->getSignalCMastName();
 if ( (name!=nullptr) && (name.length()>0) && ((name==(uName)) ||
                 (name==(sysName))) ) return A3;
 name = lTurnout->getSignalDMastName();
 if ( (name!=nullptr) && (name.length()>0) && ((name==(uName)) ||
                 (name==(sysName))) ) return B1;
 return NONE;
}

/*public*/ void LayoutEditorTools::removeAssignment(SignalMast* mast)
{
 QString sName = mast->getSystemName();
 QString uName = mast->getUserName();
 for (LayoutTurnout* to : layoutEditor->getLayoutTurnouts()) {
     if ((to->getSignalAMast()!=nullptr) &&
             (to->getSignalAMastName()==(sName) || ((uName!=nullptr) &&
             (to->getSignalAMastName()==(uName))))) to->setSignalAMast("");
     if ((to->getSignalBMast()!=nullptr) &&
             (to->getSignalBMastName()==(sName) || ((uName!=nullptr) &&
             (to->getSignalBMastName()==(uName))))) to->setSignalBMast("");
     if ((to->getSignalCMast()!=nullptr) &&
             (to->getSignalCMastName()==(sName) || ((uName!=nullptr) &&
             (to->getSignalCMastName()==(uName))))) to->setSignalCMast("");
     if ((to->getSignalDMast()!=nullptr) &&
             (to->getSignalDMastName()==(sName) || ((uName!=nullptr) &&
             (to->getSignalDMastName()==(uName))))) to->setSignalDMast("");
 }
 for (PositionablePoint* po : layoutEditor->getPositionablePoints()) {
     if ((po->getEastBoundSignalMast()!=nullptr) &&
             (po->getEastBoundSignalMastName()==(sName) || ((uName!=nullptr) &&
             (po->getEastBoundSignalMastName()==(uName)))))
         po->setEastBoundSignalMast("");
     if ((po->getWestBoundSignalMast()!=nullptr) &&
             (po->getWestBoundSignalMastName()==(sName) || ((uName!=nullptr) &&
             (po->getWestBoundSignalMastName()==(uName)))))
         po->setWestBoundSignalMast("");
 }
 for (LevelXing* x : layoutEditor->getLevelXings()) {
     if ((x->getSignalAMastName()!=nullptr) &&
             (x->getSignalAMastName()==(sName) || ((uName!=nullptr) &&
             (x->getSignalAMastName()==(uName))))) x->setSignalAMastName("");
     if ((x->getSignalBMastName()!=nullptr) &&
             (x->getSignalBMastName()==(sName) || ((uName!=nullptr) &&
             (x->getSignalBMastName()==(uName))))) x->setSignalBMastName("");
     if ((x->getSignalCMastName()!=nullptr) &&
             (x->getSignalCMastName()==(sName) || ((uName!=nullptr) &&
             (x->getSignalCMastName()==(uName))))) x->setSignalCMastName("");
     if ((x->getSignalDMastName()!=nullptr) &&
             (x->getSignalDMastName()==(sName) || ((uName!=nullptr) &&
             (x->getSignalDMastName()==(uName))))) x->setSignalDMastName("");
 }
}
/*private*/ void LayoutEditorTools::setSignalMastsDonePressed (ActionEvent* /*a*/) {
    // process turnout name
    if ( !getTurnoutMastInformation() ) return;
    // process signal head names
    SignalMast* turnoutMast = getSignalMastFromEntry(turnoutSignalMastA->getText(),false,setSignalsFrame);
    SignalMast* turnoutMastB = getSignalMastFromEntry(turnoutSignalMastB->getText(),false,setSignalsFrame);
    SignalMast* turnoutMastC = getSignalMastFromEntry(turnoutSignalMastC->getText(),false,setSignalsFrame);
    SignalMast* turnoutMastD = getSignalMastFromEntry(turnoutSignalMastD->getText(),false,setSignalsFrame);
    // place signals as requested
    if (turnoutSignalMastA->addToPanel()) {
        if (isSignalMastOnPanel(turnoutMast) &&
            (turnoutMast!=getSignalMastFromName(layoutTurnout->getSignalAMastName())))
        {
//            JOptionPane.showMessageDialog(setSignalsFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{turnoutSignalMastA->text()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
            QMessageBox::critical(setSignalsFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\n                                                                   because it is already on the panel.").arg(turnoutSignalMastA->getText()));
            return;
        }
        else {
            removeSignalMastFromPanel(layoutTurnout->getSignalAMastName());
            SignalMastIcon* l = new SignalMastIcon(layoutEditor);
            l->setSignalMast(turnoutSignalMastA->getText());
            placingBlock(l, turnoutSignalMastA->isRightSelected(), 0.0);
            removeAssignment(turnoutMast);
            layoutTurnout->setSignalAMast(turnoutSignalMastA->getText());
            needRedraw = true;
        }
    }
    else if (turnoutMast!=nullptr){
        int assigned = isMastAssignedHere(turnoutMast,layoutTurnout);
        if (assigned == NONE) {
            if ( isSignalMastOnPanel(turnoutMast) &&
                                isSignalMastAssignedAnywhere(turnoutMast) ) {
//                JOptionPane.showMessageDialog(setSignalsFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{turnoutSignalMastA->text()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
                QMessageBox::critical(setSignalsFrame, tr("Error"), tr("Error - Cannot assign signal head \"{0}\" to this turnout\n                                                                       because it is already on the panel at a different place.").arg(turnoutSignalMastA->getText()));
                return;
            }
            else {
                removeSignalMastFromPanel(layoutTurnout->getSignalAMastName());
                removeAssignment(turnoutMast);
                layoutTurnout->setSignalAMast(turnoutSignalMastA->getText());
            }
        }
        else if (assigned!=A1) {
// need to figure out what to do in this case.
        }
    }
    else {
        removeSignalMastFromPanel(layoutTurnout->getSignalAMastName());
        layoutTurnout->setSignalAMast("");
    }
    if ( (turnoutSignalMastB->addToPanel()) && (turnoutMastB!=nullptr) ) {
        if (isSignalMastOnPanel(turnoutMastB) &&
            (turnoutMastB!=getSignalMastFromName(layoutTurnout->getSignalBMastName()))) {
//            JOptionPane.showMessageDialog(setSignalsFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{turnoutSignalMastB->text()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
            QMessageBox::critical(setSignalsFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\n                                                                   because it is already on the panel.").arg(turnoutSignalMastB->getText()));
            return;
        }
        else {
            removeSignalMastFromPanel(layoutTurnout->getSignalBMastName());
            SignalMastIcon* l = new SignalMastIcon(layoutEditor);
            l->setSignalMast(turnoutSignalMastB->getText());
            placingBlockB(l, turnoutSignalMastB->isRightSelected(), 0.0);
            removeAssignment(turnoutMastB);
            layoutTurnout->setSignalBMast(turnoutSignalMastB->getText());
            needRedraw = true;
        }
    }
    else if (turnoutMastB!=nullptr) {
        int assigned = isMastAssignedHere(turnoutMastB,layoutTurnout);
        if (assigned == NONE) {
            if (isSignalMastOnPanel(turnoutMastB) &&
                                isSignalMastAssignedAnywhere(turnoutMastB) ) {
//                JOptionPane.showMessageDialog(setSignalsFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{turnoutSignalMastB->text()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
                QMessageBox::critical(setSignalsFrame, tr("Error"), tr("Error - Cannot assign signal head \"{0}\" to this turnout\n                                                                       because it is already on the panel at a different place.").arg(turnoutSignalMastB->getText()));
                return;
            }
            else {
                removeSignalMastFromPanel(layoutTurnout->getSignalBMastName());
                removeAssignment(turnoutMastB);
                layoutTurnout->setSignalBMast(turnoutSignalMastB->getText());
            }
        }
        else if (assigned!=A2) {
// need to figure out what to do in this case.
        }
    }
    else {
        removeSignalMastFromPanel(layoutTurnout->getSignalBMastName());
        layoutTurnout->setSignalBMast("");
    }
    if(turnoutMastC!=nullptr){
        if (turnoutSignalMastC->addToPanel()) {
            if (isSignalMastOnPanel(turnoutMastC) &&
                (turnoutMastC!=getSignalMastFromName(layoutTurnout->getSignalCMastName()))) {
//                JOptionPane.showMessageDialog(setSignalsFrame,
//                    java.text.MessageFormat.format(tr("SignalsError6"),
//                        new Object[]{turnoutSignalMastC->text()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
                QMessageBox::critical(setSignalsFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\n                                                                   because it is already on the panel.").arg(turnoutSignalMastC->getText()));
                return;
            }
            else {
                removeSignalMastFromPanel(layoutTurnout->getSignalCMastName());
                SignalMastIcon* l = new SignalMastIcon(layoutEditor);
                l->setSignalMast(turnoutSignalMastC->getText());
                placingBlockC(l, turnoutSignalMastC->isRightSelected(), 0.0);
                removeAssignment(turnoutMastC);
                layoutTurnout->setSignalCMast(turnoutSignalMastC->getText());
                needRedraw = true;
            }
        }
        else {
            int assigned = isMastAssignedHere(turnoutMastC,layoutTurnout);
            if (assigned == NONE) {
                if (isSignalMastOnPanel(turnoutMastC)  &&
                                    isSignalMastAssignedAnywhere(turnoutMastC) ) {
//                    JOptionPane.showMessageDialog(setSignalsFrame,
//                        java.text.MessageFormat.format(tr("SignalsError8"),
//                            new Object[]{turnoutSignalMastC->text()}),
//                                tr("Error"),JOptionPane.ERROR_MESSAGE);
                QMessageBox::critical(setSignalsFrame, tr("Error"), tr("Error - Cannot assign signal head \"{0}\" to this turnout\n                                                                       because it is already on the panel at a different place.").arg(turnoutSignalMastC->getText()));
                    return;
                }
                else {
                    removeSignalMastFromPanel(layoutTurnout->getSignalCMastName());
                    removeAssignment(turnoutMastC);
                    layoutTurnout->setSignalCMast(turnoutSignalMastC->getText());
                }
            }
            else if (assigned!=A3) {
// need to figure out what to do in this case.
            }
        }
    }
    else{
        removeSignalMastFromPanel(layoutTurnout->getSignalCMastName());
        layoutTurnout->setSignalCMast("");
    }
    if(turnoutMastD!=nullptr){
        if (turnoutSignalMastD->addToPanel()) {
            if (isSignalMastOnPanel(turnoutMastD) &&
                (turnoutMastD!=getSignalMastFromName(layoutTurnout->getSignalDMastName()))) {
//                JOptionPane.showMessageDialog(setSignalsFrame,
//                    java.text.MessageFormat.format(tr("SignalsError6"),
//                        new Object[]{divergingField->text()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
                QMessageBox::critical(setSignalsFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\nbecause it is already on the panel.").arg(turnoutSignalMastD->getText()));
                return;
            }
            else {
                removeSignalMastFromPanel(layoutTurnout->getSignalDMastName());
                SignalMastIcon* l = new SignalMastIcon(layoutEditor);
                l->setSignalMast(turnoutSignalMastD->getText());
                placingBlockD(l, turnoutSignalMastD->isRightSelected(), 0.0);
                removeAssignment(turnoutMastD);
                layoutTurnout->setSignalDMast(turnoutSignalMastD->getText());
                needRedraw = true;
            }
        }
        else {
            int assigned = isMastAssignedHere(turnoutMastD,layoutTurnout);
            if (assigned == NONE) {
                if (isSignalMastOnPanel(turnoutMastD) &&
                                    isSignalMastAssignedAnywhere(turnoutMastD) ) {
//                    JOptionPane.showMessageDialog(setSignalsFrame,
//                        java.text.MessageFormat.format(tr("SignalsError8"),
//                            new Object[]{turnoutSignalMastD->text()}),
//                                tr("Error"),JOptionPane.ERROR_MESSAGE);
                                    QMessageBox::critical(setSignalsFrame, tr("Error"), tr("Error - Cannot assign signal head \"{0}\" to this turnout\n                                                                       because it is already on the panel at a different place.").arg(turnoutSignalMastD->getText()));
return;
                }
                else {
                    removeSignalMastFromPanel(layoutTurnout->getSignalDMastName());
                    removeAssignment(turnoutMastD);
                    layoutTurnout->setSignalDMast(turnoutSignalMastD->getText());
                }
            }
            else if (assigned!=B1) {
// need to figure out what to do in this case.
            }
        }
    } else {
        removeSignalMastFromPanel(layoutTurnout->getSignalDMastName());
        layoutTurnout->setSignalDMast("");
    }

    // make sure this layout turnout is not linked to another
    layoutTurnout->setLinkType(LayoutTurnout::NO_LINK);
    layoutTurnout->setLinkedTurnoutName("");
    // finish up
    setSignalMastsOpen = false;
    turnoutFromMenu = false;
    signalMastsJmriFrame->setVisible(false);
    if (needRedraw) {
        layoutEditor->redrawPanel();
        needRedraw = false;
        layoutEditor->setDirty();
    }
}

/*private*/ bool LayoutEditorTools::getTurnoutMastInformation(){
 turnout = nullptr;
 layoutTurnout = nullptr;
 QString str = signalMastsTurnoutComboBox->currentText();
 if ((str == "") || str.isEmpty()) {
     JOptionPane::showMessageDialog(layoutEditor, tr("Error - No turnout name was entered. Please enter a turnout name or cancel."),
            tr("ErrorTitle"), JOptionPane::ERROR_MESSAGE);
     return false;
 }
 turnout = InstanceManager::turnoutManagerInstance()->getTurnout(str);
 if (turnout == nullptr) {
     JOptionPane::showMessageDialog(layoutEditor,
             tr("Error - No turnout is defined for \"%1\". Please enter\na turnout name in the Turnout Table and on the panel.").arg(str),
                     tr("Error"),
             JOptionPane::ERROR_MESSAGE);
     return false;
 } else {
     QString uname = turnout->getUserName();
     if ((uname == "") || uname.isEmpty()
             || uname !=(str)) {
         signalMastsTurnoutComboBox->setText(str);
     }
 }
 layoutTurnout = layoutEditor->getFinder()->findLayoutTurnoutByBean(turnout);

 if (layoutTurnout == nullptr) {
     JOptionPane::showMessageDialog(layoutEditor,
             tr("Error - Turnout \"%1\" is not drawn on the panel.\n Please enter the name of a drawn turnout.").arg(str),
                     tr("Error"),
             JOptionPane::ERROR_MESSAGE);
     return false;
 }
 return true;
}

/*private*/ void LayoutEditorTools::placingBlock(PositionableIcon* icon, bool right, double fromPoint) {
    if(qobject_cast<TrackSegment*>(layoutTurnout->getConnectA())!= nullptr)
    {
        TrackSegment* ts = (TrackSegment*) layoutTurnout->getConnectA();
        QPointF p = layoutTurnout->getCoordsA();
        QPointF endPoint;
        if(ts->getConnect1()==layoutTurnout){
            endPoint = layoutEditor->getCoords(ts->getConnect2(), ts->getType2());

        } else {
            endPoint = layoutEditor->getCoords(ts->getConnect1(), ts->getType1());
        }
        bool isEast = false;

        if (MathUtil::equals(endPoint.x(), p.x()))
        {
            log->debug("X in both is the same");
            if (endPoint.y() < p.y()) {
                log->debug("Y end point is less than our point");
                isEast = true;
            }
        } else if (endPoint.x() < p.x()) {
            log->debug("end X point is less than our point");
            isEast = true;
        }

        log->debug(QString("East set is ") + (isEast?"true":"false"));
        setIconOnPanel(ts, icon, isEast, QPoint(p.x(), p.y()), QPoint(endPoint.x(),endPoint.y()), right, fromPoint);
        return;
    }
}
/*private*/ void LayoutEditorTools::placingBlockB(PositionableIcon* icon, bool right, double fromPoint) {

    if(qobject_cast<TrackSegment*>(layoutTurnout->getConnectB())!= nullptr)
    {
        TrackSegment* t = (TrackSegment*) layoutTurnout->getConnectB();
        QPointF p = layoutTurnout->getCoordsB();

        QPointF end;
        if(t->getConnect1()==layoutTurnout){
            end = layoutEditor->getCoords(t->getConnect2(), t->getType2());

        } else {
            end = layoutEditor->getCoords(t->getConnect1(), t->getType1());
        }

        bool east = false;
        if(end.x()<p.x())
            east =true;
        setIconOnPanel(t, icon, east, QPoint(p.x(),p.y()), QPoint(end.x(),end.y()), right, fromPoint);
        return;
    }
}
/*private*/ void LayoutEditorTools::placingBlockC(PositionableIcon* icon, bool right, double fromPoint) {

    if(qobject_cast<TrackSegment*>(layoutTurnout->getConnectC())!= nullptr)
    {
        TrackSegment* t = (TrackSegment*) layoutTurnout->getConnectC();
        QPointF p = layoutTurnout->getCoordsC();

        QPointF end;
        if(t->getConnect1()==layoutTurnout){
            end = layoutEditor->getCoords(t->getConnect2(), t->getType2());

        } else {
            end = layoutEditor->getCoords(t->getConnect1(), t->getType1());
        }
        bool east = false;

        if(end.x()<p.x())
            east = true;
        setIconOnPanel(t, icon, east, QPoint(p.x(),p.y()), QPoint(end.x(),end.y()), right, fromPoint);
        return;
    }
}
/*private*/ void LayoutEditorTools::placingBlockD(PositionableIcon* icon, bool right, double fromPoint)
{
    //if(layoutTurnout->getConnectD() instanceof TrackSegment){
    if(qobject_cast<TrackSegment*>(layoutTurnout->getConnectD())!= nullptr)
    {
        TrackSegment* t = (TrackSegment*) layoutTurnout->getConnectD();
        QPointF p = layoutTurnout->getCoordsD();

        QPointF end;
        if(t->getConnect1()==layoutTurnout){
            end = layoutEditor->getCoords(t->getConnect2(), t->getType2());

        } else {
            end = layoutEditor->getCoords(t->getConnect1(), t->getType1());
        }

        //TrackSegment* t = boundary->getConnect2();
        bool east = false;
        if(end.x()<p.x())
            east = true;
        setIconOnPanel(t, icon, east, QPoint(p.x(),p.y()), QPoint(end.x(),end.y()), right, fromPoint);
        return;
    }
}

/*private*/ void LayoutEditorTools::setSignalMastsCancelPressed (ActionEvent* /*a*/) {
    setSignalMastsOpen = false;
    turnoutFromMenu = false;
    signalMastsJmriFrame->setVisible(false);
}

/*public*/ void LayoutEditorTools::setSignalMastsAtSlipFromMenu (LayoutSlip* slip, QVector<QString> blocks,
        JFrame* theFrame )
{
 slipMastFromMenu = true;
 layoutSlip = slip;
 layoutTurnout = slip;
 blockANameMastField->setText(layoutSlip->getBlockName());
 slipBlocks = new QVector<QString>(4);
 for(int i = 0; i<blocks.size(); i++)
 {
  slipBlocks->replace(i, blocks[i]);
 }
 setSignalMastsAtLayoutSlip(theFrame);
 return;
}
/*public*/ void LayoutEditorTools::setSignalMastsAtLayoutSlip(JFrame* theFrame)
{
 signalFrame = theFrame;
#if 1
 if (setSignalMastsAtSlipOpen)
 {
  slipSignalMastsGetSaved(nullptr);
  signalMastsAtSlipFrame->setVisible(true);
  return;
 }
 // Initialize if needed
 if (signalMastsAtSlipFrame == nullptr)
 {
  slipSignalMastA = new BeanDetails("Signal Mast", (Manager*) InstanceManager::getDefault("SignalMastManager"));
  slipSignalMastB = new BeanDetails("Signal Mast", (Manager*) InstanceManager::getDefault("SignalMastManager"));
  slipSignalMastC = new BeanDetails("Signal Mast",(Manager*) InstanceManager::getDefault("SignalMastManager"));
  slipSignalMastD = new BeanDetails("Signal Mast", (Manager*) InstanceManager::getDefault("SignalMastManager"));

  signalMastsAtSlipFrame = new JmriJFrameX( tr("Set Signal Masts at a Slip"), false, true );
  signalMastsAtSlipFrame->addHelpMenu("package.jmri.jmrit.display.SetSignalsAtLayoutSlip", true);
  signalMastsAtSlipFrame->setLocation(70,30);
  //QWidget* theContentPane = signalMastsAtSlipFrame->getContentPane();
  QScrollArea* centralWidget = new QScrollArea();
  centralWidget->setWidgetResizable(true);
  QWidget* scrollWidget = new QWidget;
  centralWidget->setWidget(scrollWidget);
  QVBoxLayout* centralWidgetLayout;
  scrollWidget->setLayout(centralWidgetLayout = new QVBoxLayout(scrollWidget)); //(theContentPane, BoxLayout.Y_AXIS));
  signalMastsAtSlipFrame->setCentralWidget(centralWidget);
  QWidget* panel11 = new QWidget();
  panel11->setLayout(new QHBoxLayout());
  if (slipMastFromMenu)
  {
   QLabel* blockANameLabel = new QLabel( tr("BlockAtA")+" "+
               tr("Name")+" : "+layoutSlip->getBlockName());

   panel11->layout()->addWidget(blockANameLabel);
   slipSignalMastA->setTextField(layoutSlip->getSignalAMastName());
   slipSignalMastB->setTextField(layoutSlip->getSignalBMastName());
   slipSignalMastC->setTextField(layoutSlip->getSignalCMastName());
   slipSignalMastD->setTextField(layoutSlip->getSignalDMastName());
  }
  else
  {
   QLabel* blockANameLabel = new QLabel( tr("BlockAtA")+" "+
                                                      tr("Name")+" : ");
   panel11->layout()->addWidget(blockANameLabel);
   panel11->layout()->addWidget(blockANameMastField);
   blockANameMastField->setToolTip(tr("SignalsBlockNameHint"));
  }
  centralWidgetLayout->addWidget(panel11);
  QWidget* panel12 = new QWidget();
  panel12->setLayout(new QHBoxLayout());
  if (slipMastFromMenu)
  {
   QLabel* blockCNameLabel = new QLabel( tr("BlockAtC")+" "+
               tr("Name")+" : "+layoutSlip->getBlockName());

   panel12->layout()->addWidget(blockCNameLabel);
  }

  centralWidgetLayout->addWidget(panel12);
  centralWidgetLayout->addWidget(new JSeparator(JSeparator::HORIZONTAL));
  QWidget* panel2 = new QWidget();
  panel2->setLayout(new QHBoxLayout());
  QLabel* shTitle = new QLabel(tr("Signal Mast"));
  panel2->layout()->addWidget(shTitle);
  panel2->layout()->addWidget(new QLabel("   "));
  panel2->layout()->addWidget(getSavedSlipSignalMasts = new QPushButton(tr("GetSaved")));
//        getSavedSlipSignalMasts->layout()->addWidgetActionListener(new ActionListener() {
//                /*public*/ void actionPerformed(ActionEvent e) {
//                    slipSignalMastsGetSaved(e);
//                }
//            });
  connect(getSavedSlipSignalMasts, SIGNAL(clicked()), this, SLOT(slipSignalMastsGetSaved()));
  getSavedSlipSignalMasts->setToolTip("Click to retrieve signal heads previously stored.");
  centralWidgetLayout->addWidget(panel2);

  //slipSignalMastA->getDetailsPanel()->setBackground(new Color(255,255,200));
  slipSignalMastA->getDetailsPanel()->setStyleSheet("QWidget {background: rgb(255,255,200)");

  //slipSignalMastB->getDetailsPanel()->setBackground(new Color(200,255,255));
  slipSignalMastB->getDetailsPanel()->setStyleSheet("QWidget {background: rgb(200,255,255)");

  //slipSignalMastC.getDetailsPanel()->setBackground(new Color(200,200,255));
  slipSignalMastC->getDetailsPanel()->setStyleSheet("QWidget {background: rgb(200,200,255)");

  //slipSignalMastD.getDetailsPanel()->setBackground(new Color(255,200,200));
  slipSignalMastD->getDetailsPanel()->setStyleSheet("QWidget {background: rgb(255,200,200)");

  signalMastLayoutSlipPanel->setLayout(signalMastLayoutSlipPanelLayout =new QGridLayout(/*0,2*/));

  slipSignalMastA->setBoundaryLabel(slipBlocks->at(0));
  slipSignalMastB->setBoundaryLabel(slipBlocks->at(1));
  slipSignalMastC->setBoundaryLabel(slipBlocks->at(2));
  slipSignalMastD->setBoundaryLabel(slipBlocks->at(3));

  if(slipBlocks->at(0)!=nullptr)
      signalMastLayoutSlipPanelLayout->addWidget(slipSignalMastA->getDetailsPanel(),0,0);
  if(slipBlocks->at(1)!=nullptr)
      signalMastLayoutSlipPanelLayout->addWidget(slipSignalMastB->getDetailsPanel(),0,1);
  if(slipBlocks->at(2)!=nullptr)
      signalMastLayoutSlipPanelLayout->addWidget(slipSignalMastC->getDetailsPanel(),1,0);
  if(slipBlocks->at(3)!=nullptr)
      signalMastLayoutSlipPanelLayout->addWidget(slipSignalMastD->getDetailsPanel(),1,1);

  centralWidgetLayout->addWidget(signalMastLayoutSlipPanel);
  centralWidgetLayout->addWidget(new JSeparator(JSeparator::HORIZONTAL));
  QWidget* panel6 = new QWidget();
  panel6->setLayout(new QHBoxLayout());

  panel6->layout()->addWidget(new QLabel("  "));
  panel6->layout()->addWidget(setSlipSignalMastsDone = new QPushButton(tr("Done")));
//        setSlipSignalMastsDone->layout()->addWidgetActionListener(new ActionListener() {
//            /*public*/ void actionPerformed(ActionEvent e) {
//                setSlipSignalMastsDonePressed(e);
//            }
//        });
  connect(setSlipSignalMastsDone, SIGNAL(clicked()), this, SLOT(setSlipSignalMastsDonePressed()));
  setSlipSignalMastsDone->setToolTip( tr("Click Done to perform tasks requested above and dismiss this dialog.") );
  panel6->layout()->addWidget(setSlipSignalMastsCancel = new QPushButton(tr("Cancel")));
//        setSlipSignalMastsCancel->layout()->addWidgetActionListener(new ActionListener() {
//            /*public*/ void actionPerformed(ActionEvent e) {
//                setSlipSignalMastsCancelPressed(e);
//            }
//        });
  connect(setSlipSignalMastsCancel, SIGNAL(clicked()), this, SLOT(setSlipSignalMastsCancelPressed()));
  setSlipSignalMastsCancel->setToolTip( tr("Click Cancel to dismiss this dialog without making changes.") );
  centralWidgetLayout->addWidget(panel6);
//        signalMastsAtSlipFrame->layout()->addWidgetWindowListener(new java.awt.event.WindowAdapter() {
//            /*public*/ void windowClosing(java.awt.event.WindowEvent e) {
//                setSlipSignalMastsCancelPressed(nullptr);
//            }
//        });
  signalMastsAtSlipFrame->addWindowListener(new SMSWindowListener(this));
  if (slipMastFromMenu) slipSignalMastsGetSaved(nullptr);
 }
 refreshSignalMastAtSlipComboBox();
 //signalMastsAtSlipFrame->setMinimumSize(QSize());
 signalMastsAtSlipFrame->adjustSize();
 signalMastsAtSlipFrame->setVisible(true);
 setSignalMastsAtSlipOpen = true;
#endif
}


void LayoutEditorTools::refreshSignalMastAtSlipComboBox(){
 slipSignalMastsGetSaved(nullptr);
 createListUsedSignalMasts();
 usedMasts.removeOne(slipSignalMastA->getBean());
 usedMasts.removeOne(slipSignalMastB->getBean());
 usedMasts.removeOne(slipSignalMastC->getBean());
 usedMasts.removeOne(slipSignalMastD->getBean());
 slipSignalMastA->getCombo()->excludeItems(usedMasts);
 slipSignalMastB->getCombo()->excludeItems(usedMasts);
 slipSignalMastC->getCombo()->excludeItems(usedMasts);
 slipSignalMastD->getCombo()->excludeItems(usedMasts);
}

/*private*/ void LayoutEditorTools::slipSignalMastsGetSaved (ActionEvent* /*a*/) {
 if ( !getSlipMastInformation() ) return;
 slipBlocks = layoutSlip->getBlockBoundaries();
 slipSignalMastA->setTextField(layoutSlip->getSignalAMastName());
 slipSignalMastB->setTextField(layoutSlip->getSignalBMastName());
 slipSignalMastC->setTextField(layoutSlip->getSignalCMastName());
 slipSignalMastD->setTextField(layoutSlip->getSignalDMastName());

 slipSignalMastA->setBoundaryLabel(slipBlocks->at(0));
 slipSignalMastB->setBoundaryLabel(slipBlocks->at(1));
 slipSignalMastC->setBoundaryLabel(slipBlocks->at(2));
 slipSignalMastD->setBoundaryLabel(slipBlocks->at(3));

 bool boundary = false;
 signalMastLayoutSlipPanel->layout()->removeWidget(slipSignalMastA->getDetailsPanel());
 signalMastLayoutSlipPanel->layout()->removeWidget(slipSignalMastB->getDetailsPanel());
 signalMastLayoutSlipPanel->layout()->removeWidget(slipSignalMastC->getDetailsPanel());
 signalMastLayoutSlipPanel->layout()->removeWidget(slipSignalMastD->getDetailsPanel());
 if(slipBlocks->at(0)!=nullptr){
     signalMastLayoutSlipPanelLayout->addWidget(slipSignalMastA->getDetailsPanel());
     boundary = true;
 }
 if(slipBlocks->at(1)!=nullptr){
     signalMastLayoutSlipPanelLayout->addWidget(slipSignalMastB->getDetailsPanel());
     boundary = true;
 }
 if(slipBlocks->at(2)!=nullptr){
     signalMastLayoutSlipPanelLayout->addWidget(slipSignalMastC->getDetailsPanel());
     boundary = true;
 }
 if(slipBlocks->at(3)!=nullptr){
     signalMastLayoutSlipPanelLayout->addWidget(slipSignalMastD->getDetailsPanel());
     boundary = true;
 }
 if (!boundary){
//        JOptionPane.showMessageDialog(signalMastsAtSlipFrame, "There are no block boundaries on this level crossing\nIt is therefore not possible to add Signal Masts to it");
  QMessageBox::information(signalMastsAtSlipFrame, tr("information"), tr("There are no block boundaries on this level crossing\nIt is therefore not possible to add Signal Masts to it"));
 }
 signalMastsAtSlipFrame->setMinimumSize(QSize());
 signalMastsAtSlipFrame->adjustSize();
}

/*private*/ bool LayoutEditorTools::getSlipMastInformation() {
    if (!slipMastFromMenu) {
        layoutSlip = NULL;
        QList<LayoutSlip*> layoutSlips = layoutEditor->getLayoutSlips();
        if (layoutSlips.size()<=0) {
//            JOptionPane.showMessageDialog(signalMastsAtSlipFrame,
//                tr("SignalsError15"),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(signalMastsAtSlipFrame, tr("Error"), tr("Error - There are no level crossings on your panel.\nPlease add a level crossing or cancel."));
            return false;
        }
        else if (layoutSlips.size()==1) {
            layoutSlip = layoutSlips.at(0);
        }
        else {
            LayoutBlock*slipBlockA = NULL;
            //LayoutBlock*slipBlockC = NULL;
            slipBlockA = getBlockFromEntry(blockANameMastField);
            if (slipBlockA==nullptr) return false;

            LayoutSlip* x = NULL;
            int foundCount = 0;
            // make two block tests first
            for (LayoutSlip* x : layoutEditor->getLayoutSlips()) {
                LayoutBlock* xA = NULL;
                LayoutBlock* xB = NULL;
                LayoutBlock* xC = NULL;
                LayoutBlock* xD = NULL;
                LayoutBlock* xAC = x->getLayoutBlock();
                if (x->getConnectA()!=nullptr) xA = ((TrackSegment*)x->getConnectA())->getLayoutBlock();
                if (x->getConnectB()!=nullptr) xB = ((TrackSegment*)x->getConnectB())->getLayoutBlock();
                if (x->getConnectC()!=nullptr) xC = ((TrackSegment*)x->getConnectC())->getLayoutBlock();
                if (x->getConnectD()!=nullptr) xD = ((TrackSegment*)x->getConnectD())->getLayoutBlock();
                if ( ( (xA!=nullptr) && (xC!=nullptr) && ( (xA==slipBlockA) ||
                        (xC==slipBlockA) ) ) ||
                        ( (xB!=nullptr) && (xD!=nullptr) && ( (xB==slipBlockA) ||
                        (xD==slipBlockA) ) ) ) {
                    layoutSlip = x;
                    foundCount ++;
                }
                else if ( (xAC!=nullptr) && ( xAC==slipBlockA
                             )) {
                    layoutSlip = x;
                    foundCount ++;
                }
            }
            if (foundCount==0) {
                // try one block test
                for (LayoutSlip* x : layoutEditor->getLayoutSlips()) {
                    if (slipBlockA == x->getLayoutBlock())
                    {
                        layoutSlip = x;
                        foundCount ++;
                    }
                }
            }
            if (foundCount>1) {
//                JOptionPane.showMessageDialog(signalMastsAtSlipFrame,
//                        java.text.MessageFormat.format(tr("SignalsError16"),
//                            new Object[]{" "+foundCount+" "}),
//                                tr("Error"),JOptionPane.ERROR_MESSAGE);
             QMessageBox::critical(signalMastsAtSlipFrame, tr("Error"), tr("Error - The entered block names match \"%1\" level crossings.\n           Please enter block names that match only one level crossing.").arg(foundCount)); return false;
            }
            if (layoutSlip==nullptr) {
//                JOptionPane.showMessageDialog(signalMastsAtSlipFrame,
//                            tr("SignalsError17"),
//                                    tr("Error"),JOptionPane.ERROR_MESSAGE);
                QMessageBox::critical(signalMastsAtSlipFrame, tr("Error"), tr("Error - No level crossing matches the block(s)\n                            entered. Please try again or cancel."));return false;
            }
        }
    }
    return true;
}

/*private*/ void LayoutEditorTools::setSlipSignalMastsCancelPressed (ActionEvent* /*a*/) {
    setSignalMastsAtSlipOpen = false;
    signalMastsAtSlipFrame->setVisible(false);
    slipMastFromMenu = false;
}

/*private*/ void LayoutEditorTools::setSlipSignalMastsDonePressed (ActionEvent* /*a*/)
{
 if ( !getSlipMastInformation() ) return;
 SignalMast* aMast = getSignalMastFromEntry(slipSignalMastA->getText(),false,signalMastsAtSlipFrame);
 SignalMast* bMast = getSignalMastFromEntry(slipSignalMastB->getText(),false,signalMastsAtSlipFrame);
 SignalMast* cMast = getSignalMastFromEntry(slipSignalMastC->getText(),false,signalMastsAtSlipFrame);
 SignalMast* dMast = getSignalMastFromEntry(slipSignalMastD->getText(),false,signalMastsAtSlipFrame);
 // place or update signals as requested
 if ( (aMast!=nullptr) && slipSignalMastA->addToPanel() )
 {
     if (isSignalMastOnPanel(aMast) &&
             (aMast!=getSignalMastFromName(layoutSlip->getSignalAMastName())) )
     {
//            JOptionPane.showMessageDialog(signalMastsAtSlipFrame,
//                java.text.MessageFormat.format(tr("SignalMastsError6"),
//                    new Object[]{slipSignalMastA->text()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
      QMessageBox::critical(signalMastsAtSlipFrame, tr("Error"), tr("Error - Cannot place signal mast \"%1\" on the panel\n                                                                because it is already on the panel.").arg(slipSignalMastA->getText()));
         return;
     }
     else {
         removeSignalMastFromPanel(layoutSlip->getSignalAMastName());
         SignalMastIcon* l = new SignalMastIcon(layoutEditor);
         l->setSignalMast(slipSignalMastA->getText());
         placingBlock(l, slipSignalMastA->isRightSelected(), 0.0);
         removeAssignment(aMast);
         layoutSlip->setSignalAMast(slipSignalMastA->getText());
         needRedraw = true;
     }
 }
 else if ( (aMast!=nullptr) &&
         (aMast!=getSignalMastFromName(layoutSlip->getSignalAMastName())) &&
         (aMast!=getSignalMastFromName(layoutSlip->getSignalBMastName())) &&
         (aMast!=getSignalMastFromName(layoutSlip->getSignalCMastName())) &&
         (aMast!=getSignalMastFromName(layoutSlip->getSignalDMastName())) ) {
     if (isSignalMastOnPanel(aMast)) {
//            JOptionPane.showMessageDialog(signalMastsAtSlipFrame,
//                java.text.MessageFormat.format(tr("SignalMastsError13"),
//                    new Object[]{slipSignalMastA->text()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
      QMessageBox::critical(signalMastsAtSlipFrame, tr("Error"), tr("Error - Cannot assign signal mast \"%1\" to this block boundary\n                                                        because it is already on the panel at a different place.").arg(slipSignalMastA->getText()));
         return;
     }
     else {
         removeSignalMastFromPanel(layoutSlip->getSignalAMastName());
         removeAssignment(aMast);
         layoutSlip->setSignalAMast(slipSignalMastA->getText());
     }
 }
 else if ( (aMast!=nullptr) &&
         ( (aMast==getSignalMastFromName(layoutSlip->getSignalBMastName())) ||
             (aMast==getSignalMastFromName(layoutSlip->getSignalCMastName())) ||
             (aMast==getSignalMastFromName(layoutSlip->getSignalDMastName())) ) ) {
// need to figure out what to do in this case.
 }
 else if (aMast==nullptr) {
     removeSignalMastFromPanel(layoutSlip->getSignalAMastName());
     layoutSlip->setSignalAMast("");
 }
 if ( (bMast!=nullptr) && slipSignalMastB->addToPanel()) {
     if (isSignalMastOnPanel(bMast) &&
             (bMast!=getSignalMastFromName(layoutSlip->getSignalBMastName()))) {
//            JOptionPane.showMessageDialog(signalMastsAtSlipFrame,
//                java.text.MessageFormat.format(tr("SignalMastsError6"),
//                    new Object[]{slipSignalMastB->text()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
                                       QMessageBox::critical(signalMastsAtSlipFrame, tr("Error"), tr("Error - Cannot place signal mast \"%1\" on the panel\n                                                                because it is already on the panel.").arg(slipSignalMastB->getText()));
         return;
     }
     else {
         removeSignalMastFromPanel(layoutSlip->getSignalBMastName());
         SignalMastIcon* l = new SignalMastIcon(layoutEditor);
         l->setSignalMast(slipSignalMastB->getText());
         placingBlockB(l, slipSignalMastB->isRightSelected(), 0.0);
         removeAssignment(bMast);
         layoutSlip->setSignalBMast(slipSignalMastB->getText());
         needRedraw = true;
     }
 }
 else if ( (bMast!=nullptr) &&
         (bMast!=getSignalMastFromName(layoutSlip->getSignalAMastName())) &&
         (bMast!=getSignalMastFromName(layoutSlip->getSignalBMastName())) &&
         (bMast!=getSignalMastFromName(layoutSlip->getSignalCMastName())) &&
         (bMast!=getSignalMastFromName(layoutSlip->getSignalDMastName())) ) {
     if (isSignalMastOnPanel(bMast)) {
//            JOptionPane.showMessageDialog(signalMastsAtSlipFrame,
//                java.text.MessageFormat.format(tr("SignalMastsError13"),
//                    new Object[]{slipSignalMastB->text()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
      QMessageBox::critical(signalMastsAtSlipFrame, tr("Error"), tr("Error - Cannot assign signal mast \"%1\" to this block boundary\n                                                        because it is already on the panel at a different place.").arg(slipSignalMastB->getText()));
         return;
     }
     else {
         removeSignalMastFromPanel(layoutSlip->getSignalBMastName());
         removeAssignment(bMast);
         layoutSlip->setSignalBMast(slipSignalMastB->getText());
     }
 }
 else if ( (bMast!=nullptr) &&
         ( (bMast==getSignalMastFromName(layoutSlip->getSignalAMastName())) ||
             (bMast==getSignalMastFromName(layoutSlip->getSignalCMastName())) ||
             (bMast==getSignalMastFromName(layoutSlip->getSignalDMastName())) ) ) {
// need to figure out what to do in this case.
 }
 else if (bMast==nullptr) {
     removeSignalMastFromPanel(layoutSlip->getSignalBMastName());
     layoutSlip->setSignalBMast("");
 }
 if ( (cMast!=nullptr) && slipSignalMastC->addToPanel() ) {
     if (isSignalMastOnPanel(cMast) &&
             (cMast!=getSignalMastFromName(layoutSlip->getSignalCMastName())) ) {
//            JOptionPane.showMessageDialog(signalMastsAtSlipFrame,
//                java.text.MessageFormat.format(tr("SignalMastsError6"),
//                    new Object[]{slipSignalMastC->text()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
      QMessageBox::critical(signalMastsAtSlipFrame, tr("Error"), tr("Error - Cannot place signal mast \"%1\" on the panel\n                                                                because it is already on the panel.").arg(slipSignalMastC->getText()));
         return;
     }
     else {
         removeSignalMastFromPanel(layoutSlip->getSignalCMastName());
         SignalMastIcon* l = new SignalMastIcon(layoutEditor);
         l->setSignalMast(slipSignalMastC->getText());
         placingBlockC(l, slipSignalMastA->isRightSelected(), 0.0);
         removeAssignment(cMast);
         layoutSlip->setSignalCMast(slipSignalMastC->getText());
         needRedraw = true;
     }
 }
 else if ( (cMast!=nullptr) &&
         (cMast!=getSignalMastFromName(layoutSlip->getSignalAMastName())) &&
         (cMast!=getSignalMastFromName(layoutSlip->getSignalBMastName())) &&
         (cMast!=getSignalMastFromName(layoutSlip->getSignalCMastName())) &&
         (cMast!=getSignalMastFromName(layoutSlip->getSignalDMastName())) ) {
     if (isSignalMastOnPanel(cMast)) {
//            JOptionPane.showMessageDialog(signalMastsAtSlipFrame,
//                java.text.MessageFormat.format(tr("SignalMastsError13"),
//                    new Object[]{slipSignalMastC->text()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
      QMessageBox::critical(signalMastsAtSlipFrame, tr("Error"), tr("Error - Cannot assign signal mast \"%1\" to this block boundary\n                                                        because it is already on the panel at a different place.").arg(slipSignalMastC->getText()));
         return;
     }
     else {
         removeSignalMastFromPanel(layoutSlip->getSignalCMastName());
         removeAssignment(cMast);
         layoutSlip->setSignalCMast(slipSignalMastC->getText());
     }
 }
 else if ( (cMast!=nullptr) &&
         ( (cMast==getSignalMastFromName(layoutSlip->getSignalBMastName())) ||
             (cMast==getSignalMastFromName(layoutSlip->getSignalAMastName())) ||
             (cMast==getSignalMastFromName(layoutSlip->getSignalDMastName())) ) ) {
// need to figure out what to do in this case.
 }
 else if (cMast==nullptr) {
     removeSignalMastFromPanel(layoutSlip->getSignalCMastName());
     layoutSlip->setSignalCMast("");
 }
 if ( (dMast!=nullptr) && slipSignalMastD->addToPanel() ) {
     if (isSignalMastOnPanel(dMast) &&
             (dMast!=getSignalMastFromName(layoutSlip->getSignalDMastName())) ) {
      //            JOptionPane.showMessageDialog(signalMastsAtSlipFrame,
      //                java.text.MessageFormat.format(tr("SignalMastsError6"),
      //                    new Object[]{slipSignalMastD->text()}),
      //                        tr("Error"),JOptionPane.ERROR_MESSAGE);
      QMessageBox::critical(signalMastsAtSlipFrame, tr("Error"), tr("Error - Cannot place signal mast \"%1\" on the panel\n                                                                because it is already on the panel.").arg(slipSignalMastD->getText()));
         return;
     }
     else {
         removeSignalMastFromPanel(layoutSlip->getSignalDMastName());
         SignalMastIcon* l = new SignalMastIcon(layoutEditor);
         l->setSignalMast(slipSignalMastD->getText());
         placingBlockD(l, slipSignalMastD->isRightSelected(), 0.0);
         removeAssignment(dMast);
         layoutSlip->setSignalDMast(slipSignalMastD->getText());
         needRedraw = true;
     }
 }
 else if ( (dMast!=nullptr) &&
         (dMast!=getSignalMastFromName(layoutSlip->getSignalAMastName())) &&
         (dMast!=getSignalMastFromName(layoutSlip->getSignalBMastName())) &&
         (dMast!=getSignalMastFromName(layoutSlip->getSignalCMastName())) &&
         (dMast!=getSignalMastFromName(layoutSlip->getSignalDMastName())) ) {
     if (isSignalMastOnPanel(dMast)) {
//            JOptionPane.showMessageDialog(signalMastsAtSlipFrame,
//                java.text.MessageFormat.format(tr("SignalMastsError13"),
//                    new Object[]{slipSignalMastD->text()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
      QMessageBox::critical(signalMastsAtSlipFrame, tr("Error"), tr("Error - Cannot assign signal mast \"%1\" to this block boundary\n                                                        because it is already on the panel at a different place.").arg(slipSignalMastD->getText()));
         return;
     }
     else {
         removeSignalMastFromPanel(layoutSlip->getSignalDMastName());
         removeAssignment(dMast);
         layoutSlip->setSignalDMast(slipSignalMastD->getText());
     }
 }
 else if ( (dMast!=nullptr) &&
         ( (dMast==getSignalMastFromName(layoutSlip->getSignalBMastName())) ||
             (dMast==getSignalMastFromName(layoutSlip->getSignalCMastName())) ||
             (dMast==getSignalMastFromName(layoutSlip->getSignalAMastName())) ) ) {
// need to figure out what to do in this case.
 }
 else if (dMast==nullptr) {
     removeSignalMastFromPanel(layoutSlip->getSignalDMastName());
     layoutSlip->setSignalDMast("");
 }
 // setup logic if requested
 // finish up
 setSignalMastsAtSlipOpen = false;
 signalMastsAtSlipFrame->setVisible(false);
 slipMastFromMenu = false;
 if (needRedraw) {
     layoutEditor->redrawPanel();
     needRedraw = false;
     layoutEditor->setDirty();
 }
}


// display dialog for Set Signals at Level Crossing tool
/*public*/ void LayoutEditorTools::setSignalMastsAtLevelXingFromMenu (LevelXing* xing, QVector<QString>* blocks, JFrame* theFrame )
{
  xingMastFromMenu = true;
  levelXing = xing;
  blockANameMastField->setText(levelXing->getBlockNameAC());
  blockCNameMastField->setText(levelXing->getBlockNameBD());
  xingBlocks= new QVector<QString>(4);
  for(int i = 0; i<blocks->size(); i++)
  {
   xingBlocks->replace(i, blocks->at(i));
  }
  setSignalMastsAtLevelXing(theFrame);
  return;
}
/*public*/ void LayoutEditorTools::setSignalMastsAtLevelXing(JFrame* theFrame)
{
 signalFrame = theFrame;
 if (setSignalMastsAtXingOpen)
 {
  xingSignalMastsGetSaved(nullptr);
  signalMastsAtXingFrame->setVisible(true);
  return;
 }
 // Initialize if needed
 if (signalMastsAtXingFrame == nullptr)
 {
  xingSignalMastA = new BeanDetails("Signal Mast", (Manager*) InstanceManager::getDefault("SignalMastManager"));
  xingSignalMastB = new BeanDetails("Signal Mast",(Manager*) InstanceManager::getDefault("SignalMastManager"));
  xingSignalMastC = new BeanDetails("Signal Mast",(Manager*) InstanceManager::getDefault("SignalMastManager"));
  xingSignalMastD = new BeanDetails("Signal Mast",(Manager*) InstanceManager::getDefault("SignalMastManager"));

  signalMastsAtXingFrame = new JmriJFrameX( tr("SignalMastsAtLevelXing"), false, true );
  signalMastsAtXingFrame->addHelpMenu("package.jmri.jmrit.display.SetSignalsAtLevelXing", true);
  signalMastsAtXingFrame->setLocation(70,30);
  QWidget* theContentPane = new QWidget;
  /*signalMastsAtXingFrame.getContentPane();*/
  signalMastsAtXingFrame->setCentralWidget(theContentPane);
  QVBoxLayout* centralWidgetLayout;
  theContentPane->setLayout(centralWidgetLayout = new QVBoxLayout); //(theContentPane, BoxLayout.Y_AXIS));
  QWidget* panel11 = new QWidget();
  panel11->setLayout(new QHBoxLayout());
  if (xingMastFromMenu)
  {
   QLabel* blockANameLabel = new QLabel( tr("BlockAtA")+" "+
                  tr("Name")+" : "+levelXing->getBlockNameAC());
   panel11->layout()->addWidget(blockANameLabel);
   xingSignalMastA->setTextField(levelXing->getSignalAMastName());
   xingSignalMastB->setTextField(levelXing->getSignalBMastName());
   xingSignalMastC->setTextField(levelXing->getSignalCMastName());
   xingSignalMastD->setTextField(levelXing->getSignalDMastName());
  }
  else
  {
   QLabel* blockANameLabel = new QLabel( tr("BlockAtA")+" "+ tr("Name")+" : ");
   panel11->layout()->addWidget(blockANameLabel);
   panel11->layout()->addWidget(blockANameMastField);
   blockANameMastField->setToolTip(tr("SignalsBlockNameHint"));
  }
  centralWidgetLayout->addWidget(panel11);
  QWidget* panel12 = new QWidget();
  panel12->setLayout(new QHBoxLayout());
  if (xingMastFromMenu)
  {
   QLabel* blockCNameLabel = new QLabel( tr("BlockAtC")+" "+ tr("Name")+" : "+levelXing->getBlockNameBD());

   panel12->layout()->addWidget(blockCNameLabel);
  }
  else
  {
   QLabel* blockCNameLabel = new QLabel( tr("BlockAtC")+" "+tr("Name")+" : ");
   panel12->layout()->addWidget(blockCNameLabel);
   panel12->layout()->addWidget(blockCNameMastField);
   blockCNameMastField->setToolTip(tr("SignalsBlockNameHint"));
  }
  centralWidgetLayout->addWidget(panel12);
  centralWidgetLayout->addWidget(new JSeparator(JSeparator::HORIZONTAL));
  QWidget* panel2 = new QWidget();
  panel2->setLayout(new QHBoxLayout());
  QLabel* shTitle = new QLabel(tr("Signal Mast"));
  panel2->layout()->addWidget(shTitle);
  panel2->layout()->addWidget(new QLabel("   "));
  panel2->layout()->addWidget(getSavedXingSignalMasts = new QPushButton(tr("Get Saved")));
//        getSavedXingSignalMasts->layout()->addWidgetActionListener(new ActionListener() {
//                /*public*/ void actionPerformed(ActionEvent e) {
//                    xingSignalMastsGetSaved(e);
//                }
//            });
  connect(getSavedXingSignalMasts, SIGNAL(clicked()), this, SLOT(xingSignalMastsGetSaved()));
  getSavedXingSignalMasts->setToolTip("Click to retrieve signal heads previously stored.");
  centralWidgetLayout->addWidget(panel2);

  //xingSignalMastA.getDetailsPanel()->setBackground(new Color(255,255,200));
  xingSignalMastA->getDetailsPanel()->setStyleSheet("QWidget { background: rgb(255,255,200)}");

  //xingSignalMastB.getDetailsPanel()->setBackground(new Color(200,255,255));
  xingSignalMastB->getDetailsPanel()->setStyleSheet("QWidget { background: rgb(200,255,255)}");

  //xingSignalMastC.getDetailsPanel()->setBackground(new Color(200,200,255));
  xingSignalMastC->getDetailsPanel()->setStyleSheet("QWidget { background: rgb(200,200,255)}");

  //xingSignalMastD.getDetailsPanel()->setBackground(new Color(255,200,200));
  xingSignalMastD->getDetailsPanel()->setStyleSheet("QWidget { background: rgb(255,200,200)}");


  signalMastLevelXingPanel->setLayout(signalMastLevelXingPanelLayout = new QGridLayout(/*0,2*/));

  xingSignalMastA->setBoundaryLabel(xingBlocks->at(0));
  xingSignalMastB->setBoundaryLabel(xingBlocks->at(1));
  xingSignalMastC->setBoundaryLabel(xingBlocks->at(2));
  xingSignalMastD->setBoundaryLabel(xingBlocks->at(3));

  if(xingBlocks->at(0)!=nullptr)
      signalMastLevelXingPanelLayout->addWidget(xingSignalMastA->getDetailsPanel(),0,0);
  if(xingBlocks->at(1)!=nullptr)
      signalMastLevelXingPanelLayout->addWidget(xingSignalMastB->getDetailsPanel(),0,1);
  if(xingBlocks->at(2)!=nullptr)
      signalMastLevelXingPanelLayout->addWidget(xingSignalMastC->getDetailsPanel(),1, 0);
  if(xingBlocks->at(3)!=nullptr)
      signalMastLevelXingPanelLayout->addWidget(xingSignalMastD->getDetailsPanel(),1,1);

  centralWidgetLayout->addWidget(signalMastLevelXingPanel);
  centralWidgetLayout->addWidget(new JSeparator(JSeparator::HORIZONTAL));
  QWidget* panel6 = new QWidget();
  panel6->setLayout(new QHBoxLayout());

  panel6->layout()->addWidget(new QLabel("  "));
  panel6->layout()->addWidget(setXingSignalMastsDone = new QPushButton(tr("Done")));
//  setXingSignalMastsDone->layout()->addWidgetActionListener(new ActionListener() {
//      /*public*/ void actionPerformed(ActionEvent e) {
//          setXingSignalMastsDonePressed(e);
//      }
//  });
  connect(setXingSignalMastsDone, SIGNAL(clicked()), this, SLOT(setXingSignalMastsDonePressed()));
  setXingSignalMastsDone->setToolTip( tr("Click Done to perform tasks requested above and dismiss this dialog.") );
  panel6->layout()->addWidget(setXingSignalMastsCancel = new QPushButton(tr("Cancel")));
//  setXingSignalMastsCancel->layout()->addWidgetActionListener(new ActionListener() {
//      /*public*/ void actionPerformed(ActionEvent e) {
//          setXingSignalMastsCancelPressed(e);
//      }
//  });
  connect(setXingSignalMastsCancel, SIGNAL(clicked()), this, SLOT(setXingSignalMastsCancelPressed()));
  setXingSignalMastsCancel->setToolTip( tr("Click Cancel to dismiss this dialog without making changes.") );
  centralWidgetLayout->addWidget(panel6);
//  signalMastsAtXingFrame->layout()->addWidgetWindowListener(new java.awt.event.WindowAdapter() {
//      /*public*/ void windowClosing(java.awt.event.WindowEvent e) {
//          setXingSignalMastsCancelPressed(nullptr);
//      }
//  });
  signalMastsAtXingFrame->addWindowListener(new XSMSWindowListener(this));
  if (xingMastFromMenu) xingSignalMastsGetSaved(nullptr);
 }
 refreshSignalMastAtXingComboBox();
 signalMastsAtXingFrame->setMinimumSize(QSize());
 signalMastsAtXingFrame->adjustSize();
 signalMastsAtXingFrame->setVisible(true);
 setSignalMastsAtXingOpen = true;
}
void LayoutEditorTools::refreshSignalMastAtXingComboBox()
{
 xingSignalMastsGetSaved(nullptr);
 createListUsedSignalMasts();
 usedMasts.removeOne(xingSignalMastA->getBean());
 usedMasts.removeOne(xingSignalMastB->getBean());
 usedMasts.removeOne(xingSignalMastC->getBean());
 usedMasts.removeOne(xingSignalMastD->getBean());
 xingSignalMastA->getCombo()->excludeItems(usedMasts);
 xingSignalMastB->getCombo()->excludeItems(usedMasts);
 xingSignalMastC->getCombo()->excludeItems(usedMasts);
 xingSignalMastD->getCombo()->excludeItems(usedMasts);
}

/*private*/ void LayoutEditorTools::xingSignalMastsGetSaved (ActionEvent* /*a*/)
{
 if ( !getLevelCrossingMastInformation() ) return;
 xingBlocks = levelXing->getBlockBoundaries();
 xingSignalMastA->setTextField(levelXing->getSignalAMastName());
 xingSignalMastB->setTextField(levelXing->getSignalBMastName());
 xingSignalMastC->setTextField(levelXing->getSignalCMastName());
 xingSignalMastD->setTextField(levelXing->getSignalDMastName());

 xingSignalMastA->setBoundaryLabel(xingBlocks->at(0));
 xingSignalMastB->setBoundaryLabel(xingBlocks->at(1));
 xingSignalMastC->setBoundaryLabel(xingBlocks->at(2));
 xingSignalMastD->setBoundaryLabel(xingBlocks->at(3));

 bool boundary = false;
 signalMastLevelXingPanel->layout()->removeWidget(xingSignalMastA->getDetailsPanel());
 signalMastLevelXingPanel->layout()->removeWidget(xingSignalMastB->getDetailsPanel());
 signalMastLevelXingPanel->layout()->removeWidget(xingSignalMastC->getDetailsPanel());
 signalMastLevelXingPanel->layout()->removeWidget(xingSignalMastD->getDetailsPanel());
 if(xingBlocks->at(0)!=nullptr){
     signalMastLevelXingPanelLayout->addWidget(xingSignalMastA->getDetailsPanel());
     boundary = true;
 }
 if(xingBlocks->at(1)!=nullptr){
     signalMastLevelXingPanelLayout->addWidget(xingSignalMastB->getDetailsPanel());
     boundary = true;
 }
 if(xingBlocks->at(2)!=nullptr){
     signalMastLevelXingPanelLayout->addWidget(xingSignalMastC->getDetailsPanel());
     boundary = true;
 }
 if(xingBlocks->at(3)!=nullptr){
     signalMastLevelXingPanelLayout->addWidget(xingSignalMastD->getDetailsPanel());
     boundary = true;
 }
 if (!boundary){
//        JOptionPane.showMessageDialog(signalMastsAtXingFrame, "There are no block boundaries on this level crossing\nIt is therefore not possible to add Signal Masts to it");
  QMessageBox::information(signalMastsAtXingFrame, tr("Information"), tr("There are no block boundaries on this level crossing\nIt is therefore not possible to add Signal Masts to it"));
 }
 signalMastsAtXingFrame->setMinimumSize(QSize());
 signalMastsAtXingFrame->adjustSize();
}
/*private*/ bool LayoutEditorTools::getLevelCrossingMastInformation()
{
 if (!xingMastFromMenu)
 {
  levelXing = NULL;
  QList<LevelXing*> levelXings = layoutEditor->getLevelXings();
  if (levelXings.size()<=0)
  {
//      JOptionPane.showMessageDialog(signalMastsAtXingFrame,
//          tr("SignalsError15"),
//                      tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(signalMastsAtXingFrame, tr("Error"), tr("Error - There are no level crossings on your panel.\nPlease add a level crossing or cancel."));
      return false;
  }
  else if(levelXings.size()==1)
  {
   levelXing = levelXings.at(0);
  }
  else
  {
   LayoutBlock* xingBlockA = NULL;
   LayoutBlock* xingBlockC = NULL;
   xingBlockA = getBlockFromEntry(blockANameMastField);
   if (xingBlockA==nullptr) return false;
   if (blockCNameMastField->text().length()>0)
   {
    xingBlockC = getBlockFromEntry(blockCNameMastField);
    if (xingBlockC==nullptr) return false;
   }
   LevelXing* x = NULL;
   int foundCount = 0;
   // make two block tests first
   if (xingBlockC!=nullptr)
   {
for (LevelXing* x : layoutEditor->getLevelXings()) {
     LayoutBlock* xA = NULL;
     LayoutBlock* xB = NULL;
     LayoutBlock* xC = NULL;
     LayoutBlock* xD = NULL;
     LayoutBlock* xAC = x->getLayoutBlockAC();
     LayoutBlock* xBD = x->getLayoutBlockBD();
     if (x->getConnectA()!=nullptr) xA = ((TrackSegment*)x->getConnectA())->getLayoutBlock();
     if (x->getConnectB()!=nullptr) xB = ((TrackSegment*)x->getConnectB())->getLayoutBlock();
     if (x->getConnectC()!=nullptr) xC = ((TrackSegment*)x->getConnectC())->getLayoutBlock();
     if (x->getConnectD()!=nullptr) xD = ((TrackSegment*)x->getConnectD())->getLayoutBlock();
     if ( ( (xA!=nullptr) && (xC!=nullptr) && ( ((xA==xingBlockA)&&(xC==xingBlockC)) ||
             ((xA==xingBlockC)&&(xC==xingBlockA)) ) ) ||
             ( (xB!=nullptr) && (xD!=nullptr) && ( ((xB==xingBlockA)&&(xD==xingBlockC)) ||
             ((xB==xingBlockC)&&(xD==xingBlockA)) ) ) )
     {
      levelXing = x;
      foundCount ++;
     }
     else if ( (xAC!=nullptr) && (xBD!=nullptr) && ( ((xAC==xingBlockA) && (xBD==xingBlockC)) ||
                 ((xAC==xingBlockC) && (xBD==xingBlockA)) ) )
     {
      levelXing = x;
      foundCount ++;
     }
    }
   }
   if (foundCount==0)
   {
    // try one block test
    for (LevelXing* x : layoutEditor->getLevelXings()) {
     if ((xingBlockA == x->getLayoutBlockAC()) || (xingBlockA == x->getLayoutBlockBD()))
     {
      levelXing = x;
      foundCount ++;
     }
    }
   }
   if (foundCount>1)
   {
//       JOptionPane.showMessageDialog(signalMastsAtXingFrame,
//               java.text.MessageFormat.format(tr("SignalsError16"),
//                   new Object[]{" "+foundCount+" "}),
//                       tr("Error"),JOptionPane.ERROR_MESSAGE);
    QMessageBox::critical(signalMastsAtXingFrame, tr("Error"), tr("Error - The entered block names match \"%1\" level crossings.\n           Please enter block names that match only one level crossing.").arg(foundCount)); return false;
    return false;
   }
   if (levelXing==nullptr)
   {
//       JOptionPane.showMessageDialog(signalMastsAtXingFrame,
//                   tr("SignalsError17"),
//                           tr("Error"),JOptionPane.ERROR_MESSAGE);
    QMessageBox::critical(signalMastsAtXingFrame, tr("Error"), tr("Error - No level crossing matches the block(s)\n                            entered. Please try again or cancel."));return false;
    return false;
   }
  }
 }

 return true;
}

/*private*/ void LayoutEditorTools::setXingSignalMastsCancelPressed (ActionEvent* /*a*/)
{
 setSignalMastsAtXingOpen = false;
 signalMastsAtXingFrame->setVisible(false);
 xingMastFromMenu = false;
}
#if 1
/*private*/ void LayoutEditorTools::setXingSignalMastsDonePressed (ActionEvent* /*a*/) {
    if ( !getLevelCrossingMastInformation() ) return;
    SignalMast* aMast = getSignalMastFromEntry(xingSignalMastA->getText(),false,signalMastsAtXingFrame);
    SignalMast* bMast = getSignalMastFromEntry(xingSignalMastB->getText(),false,signalMastsAtXingFrame);
    SignalMast* cMast = getSignalMastFromEntry(xingSignalMastC->getText(),false,signalMastsAtXingFrame);
    SignalMast* dMast = getSignalMastFromEntry(xingSignalMastD->getText(),false,signalMastsAtXingFrame);
    //if ( !getXingSignalMastInformation() ) return;
    // place or update signals as requested
    if ( (aMast!=nullptr) && xingSignalMastA->addToPanel() ) {
        if (isSignalMastOnPanel(aMast) &&
                (aMast!=getSignalMastFromName(levelXing->getSignalAMastName())) ) {
//            JOptionPane.showMessageDialog(signalMastsAtXingFrame,
//                java.text.MessageFormat.format(tr("SignalMastsError6"),
//                    new Object[]{xingSignalMastA->text()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(signalMastsAtSlipFrame, tr("Error"), tr("Error - Cannot place signal mast \"%1\" on the panel\n                                                                because it is already on the panel.").arg(xingSignalMastA->getText()));
            return;
        }
        else {
            removeSignalMastFromPanel(levelXing->getSignalAMastName());
            SignalMastIcon* l = new SignalMastIcon(layoutEditor);
            l->setSignalMast(xingSignalMastA->getText());
            placeXingAIcon(l, xingSignalMastA->isRightSelected(), 0.0);
            removeAssignment(aMast);
            levelXing->setSignalAMastName(xingSignalMastA->getText());
            needRedraw = true;
        }
    }
    else if ( (aMast!=nullptr) &&
            (aMast!=getSignalMastFromName(levelXing->getSignalAMastName())) &&
            (aMast!=getSignalMastFromName(levelXing->getSignalBMastName())) &&
            (aMast!=getSignalMastFromName(levelXing->getSignalCMastName())) &&
            (aMast!=getSignalMastFromName(levelXing->getSignalDMastName())) ) {
        if (isSignalMastOnPanel(aMast)) {
//            JOptionPane.showMessageDialog(signalMastsAtXingFrame,
//                java.text.MessageFormat.format(tr("SignalMastsError13"),
//                    new Object[]{xingSignalMastA->text()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(signalMastsAtSlipFrame, tr("Error"), tr("Error - Cannot assign signal mast \"%1\" to this block boundary\n                                                        because it is already on the panel at a different place.").arg(xingSignalMastA->getText()));
            return;
        }
        else {
            removeSignalMastFromPanel(levelXing->getSignalAMastName());
            removeAssignment(aMast);
            levelXing->setSignalAMastName(xingSignalMastA->getText());
        }
    }
    else if ( (aMast!=nullptr) &&
            ( (aMast==getSignalMastFromName(levelXing->getSignalBMastName())) ||
                (aMast==getSignalMastFromName(levelXing->getSignalCMastName())) ||
                (aMast==getSignalMastFromName(levelXing->getSignalDMastName())) ) ) {
// need to figure out what to do in this case.
    }
    else if (aMast==nullptr) {
        removeSignalMastFromPanel(levelXing->getSignalAMastName());
        levelXing->setSignalAMastName("");
    }
    if ( (bMast!=nullptr) && xingSignalMastB->addToPanel()) {
        if (isSignalMastOnPanel(bMast) &&
                (bMast!=getSignalMastFromName(levelXing->getSignalBMastName()))) {
//            JOptionPane.showMessageDialog(signalMastsAtXingFrame,
//                java.text.MessageFormat.format(tr("SignalMastsError6"),
//                    new Object[]{xingSignalMastB->text()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(signalMastsAtSlipFrame, tr("Error"), tr("Error - Cannot place signal mast \"%1\" on the panel\n                                                                because it is already on the panel.").arg(xingSignalMastB->getText()));
         return;
        }
        else {
            removeSignalMastFromPanel(levelXing->getSignalBMastName());
            SignalMastIcon* l = new SignalMastIcon(layoutEditor);
            l->setSignalMast(xingSignalMastB->getText());
            placeXingBIcon(l, xingSignalMastB->isRightSelected(), 0.0);
            removeAssignment(bMast);
            levelXing->setSignalBMastName(xingSignalMastB->getText());
            needRedraw = true;
        }
    }
    else if ( (bMast!=nullptr) &&
            (bMast!=getSignalMastFromName(levelXing->getSignalAMastName())) &&
            (bMast!=getSignalMastFromName(levelXing->getSignalBMastName())) &&
            (bMast!=getSignalMastFromName(levelXing->getSignalCMastName())) &&
            (bMast!=getSignalMastFromName(levelXing->getSignalDMastName())) ) {
        if (isSignalMastOnPanel(bMast)) {
//            JOptionPane.showMessageDialog(signalMastsAtXingFrame,
//                java.text.MessageFormat.format(tr("SignalMastsError13"),
//                    new Object[]{xingSignalMastB->text()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(signalMastsAtSlipFrame, tr("Error"), tr("Error - Cannot assign signal mast \"%1\" to this block boundary\n                                                        because it is already on the panel at a different place.").arg(xingSignalMastB->getText()));
            return;
        }
        else {
            removeSignalMastFromPanel(levelXing->getSignalBMastName());
            removeAssignment(bMast);
            levelXing->setSignalBMastName(xingSignalMastB->getText());
        }
    }
    else if ( (bMast!=nullptr) &&
            ( (bMast==getSignalMastFromName(levelXing->getSignalAMastName())) ||
                (bMast==getSignalMastFromName(levelXing->getSignalCMastName())) ||
                (bMast==getSignalMastFromName(levelXing->getSignalDMastName())) ) ) {
// need to figure out what to do in this case.
    }
    else if (bMast==nullptr) {
        removeSignalMastFromPanel(levelXing->getSignalBMastName());
        levelXing->setSignalBMastName("");
    }
    if ( (cMast!=nullptr) && xingSignalMastC->addToPanel() ) {
        if (isSignalMastOnPanel(cMast) &&
                (cMast!=getSignalMastFromName(levelXing->getSignalCMastName())) ) {
//            JOptionPane.showMessageDialog(signalMastsAtXingFrame,
//                java.text.MessageFormat.format(tr("SignalMastsError6"),
//                    new Object[]{xingSignalMastC->text()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(signalMastsAtSlipFrame, tr("Error"), tr("Error - Cannot place signal mast \"%1\" on the panel\n                                                                because it is already on the panel.").arg(xingSignalMastC->getText()));
            return;
        }
        else {
            removeSignalMastFromPanel(levelXing->getSignalCMastName());
            SignalMastIcon* l = new SignalMastIcon(layoutEditor);
            l->setSignalMast(xingSignalMastC->getText());
            placeXingCIcon(l, xingSignalMastA->isRightSelected(), 0.0);
            removeAssignment(cMast);
            levelXing->setSignalCMastName(xingSignalMastC->getText());
            needRedraw = true;
        }
    }
    else if ( (cMast!=nullptr) &&
            (cMast!=getSignalMastFromName(levelXing->getSignalAMastName())) &&
            (cMast!=getSignalMastFromName(levelXing->getSignalBMastName())) &&
            (cMast!=getSignalMastFromName(levelXing->getSignalCMastName())) &&
            (cMast!=getSignalMastFromName(levelXing->getSignalDMastName())) ) {
        if (isSignalMastOnPanel(cMast)) {
//            JOptionPane.showMessageDialog(signalMastsAtXingFrame,
//                java.text.MessageFormat.format(tr("SignalMastsError13"),
//                    new Object[]{xingSignalMastC->text()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(signalMastsAtSlipFrame, tr("Error"), tr("Error - Cannot assign signal mast \"%1\" to this block boundary\n                                                        because it is already on the panel at a different place.").arg(xingSignalMastC->getText()));
            return;
        }
        else {
            removeSignalMastFromPanel(levelXing->getSignalCMastName());
            removeAssignment(cMast);
            levelXing->setSignalCMastName(xingSignalMastC->getText());
        }
    }
    else if ( (cMast!=nullptr) &&
            ( (cMast==getSignalMastFromName(levelXing->getSignalBMastName())) ||
                (cMast==getSignalMastFromName(levelXing->getSignalAMastName())) ||
                (cMast==getSignalMastFromName(levelXing->getSignalDMastName())) ) )
    {
// need to figure out what to do in this case.
    }
    else if (cMast==nullptr) {
        removeSignalMastFromPanel(levelXing->getSignalCMastName());
        levelXing->setSignalCName("");
    }
    if ( (dMast!=nullptr) && xingSignalMastD->addToPanel() )
    {
        if (isSignalMastOnPanel(dMast) &&
                (dMast!=getSignalMastFromName(levelXing->getSignalDMastName())) ) {
//            JOptionPane.showMessageDialog(signalMastsAtXingFrame,
//                java.text.MessageFormat.format(tr("SignalMastsError6"),
//                    new Object[]{xingSignalMastD->text()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(signalMastsAtSlipFrame, tr("Error"), tr("Error - Cannot place signal mast \"%1\" on the panel\n                                                                because it is already on the panel.").arg(xingSignalMastD->getText()));
         return;
        }
        else {
            removeSignalMastFromPanel(levelXing->getSignalDMastName());
            SignalMastIcon* l = new SignalMastIcon(layoutEditor);
            l->setSignalMast(xingSignalMastD->getText());
            placeXingDIcon(l, xingSignalMastD->isRightSelected(), 0.0);
            removeAssignment(dMast);
            levelXing->setSignalDMastName(xingSignalMastD->getText());
            needRedraw = true;
        }
    }
    else if ( (dMast!=nullptr) &&
            (dMast!=getSignalMastFromName(levelXing->getSignalAMastName())) &&
            (dMast!=getSignalMastFromName(levelXing->getSignalBMastName())) &&
            (dMast!=getSignalMastFromName(levelXing->getSignalCMastName())) &&
            (dMast!=getSignalMastFromName(levelXing->getSignalDMastName())) ) {
        if (isSignalMastOnPanel(dMast)) {
//            JOptionPane.showMessageDialog(signalMastsAtXingFrame,
//                java.text.MessageFormat.format(tr("SignalMastsError13"),
//                    new Object[]{xingSignalMastD->text()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(signalMastsAtSlipFrame, tr("Error"), tr("Error - Cannot assign signal mast \"%1\" to this block boundary\n                                                        because it is already on the panel at a different place.").arg(xingSignalMastD->getText()));
            return;
        }
        else {
            removeSignalMastFromPanel(levelXing->getSignalDMastName());
            removeAssignment(dMast);
            levelXing->setSignalDMastName(xingSignalMastD->getText());
        }
    }
    else if ( (dMast!=nullptr) &&
            ( (dMast==getSignalMastFromName(levelXing->getSignalBMastName())) ||
                (dMast==getSignalMastFromName(levelXing->getSignalCMastName())) ||
                (dMast==getSignalMastFromName(levelXing->getSignalAMastName())) ) ) {
// need to figure out what to do in this case.
    }
    else if (dMast==nullptr) {
        removeSignalMastFromPanel(levelXing->getSignalDMastName());
        levelXing->setSignalDMastName("");
    }
    // setup logic if requested
    // finish up
    setSignalMastsAtXingOpen = false;
    signalMastsAtXingFrame->setVisible(false);
    xingMastFromMenu = false;
    if (needRedraw) {
        layoutEditor->redrawPanel();
        needRedraw = false;
        layoutEditor->setDirty();
    }
}

/*private*/ void LayoutEditorTools::placeXingAIcon(PositionableIcon* icon, bool right, double fromPoint)
{

    //if(levelXing->getConnectA() instanceof TrackSegment)
 if(qobject_cast<TrackSegment*>(levelXing->getConnectA()))
    {
        TrackSegment* t = (TrackSegment*) levelXing->getConnectA();
        QPointF pf = levelXing->getCoordsA();
        QPoint p((int)pf.x(), (int)pf.y());

        QPointF end;
        if(t->getConnect1()==levelXing){
            end = layoutEditor->getCoords(t->getConnect2(), t->getType2());

        } else {
            end = layoutEditor->getCoords(t->getConnect1(), t->getType1());
        }

        bool east = false;

        if(end.x()<p.x())
            east = true;

        setIconOnPanel(t, icon, east, p, QPoint((int)end.x(),(int)end.y()), right, fromPoint);
        return;
    }
}
/*private*/ void LayoutEditorTools::placeXingBIcon(PositionableIcon* icon, bool right, double fromPoint)
{

    //if(levelXing->getConnectB() instanceof TrackSegment){
 if(qobject_cast<TrackSegment*>(levelXing->getConnectB()))
 {
        TrackSegment* t = (TrackSegment*) levelXing->getConnectB();
        QPointF pf = levelXing->getCoordsB();
        QPoint p((int)pf.x(), (int)pf.y());

        QPointF end;
        if(t->getConnect1()==levelXing){
            end = layoutEditor->getCoords(t->getConnect2(), t->getType2());

        } else {
            end = layoutEditor->getCoords(t->getConnect1(), t->getType1());
        }
        bool east = false;

        if(end.x()<p.x())
            east = true;

        setIconOnPanel(t, icon, east, p, QPoint((int)end.x(),(int)end.y()), right, fromPoint);
        return;
    }
}
/*private*/ void LayoutEditorTools::placeXingCIcon(PositionableIcon* icon, bool right, double fromPoint)
{
    //if(levelXing->getConnectC() instanceof TrackSegment) {
 if(qobject_cast<TrackSegment*>(levelXing->getConnectC()))
 {
        TrackSegment* t = (TrackSegment*) levelXing->getConnectC();
        QPointF pf = levelXing->getCoordsC();
        QPoint p((int)pf.x(), (int)pf.y());

        QPointF end;
        if(t->getConnect1()==levelXing){
            end = layoutEditor->getCoords(t->getConnect2(), t->getType2());

        } else {
            end = layoutEditor->getCoords(t->getConnect1(), t->getType1());
        }
        bool east = false;

        if(end.x()<p.x())
            east = true;

        setIconOnPanel(t, icon, east, p, QPoint((int)end.x(),(int)end.y()), right, fromPoint);
        return;
    }
}

/*private*/ void LayoutEditorTools::placeXingDIcon(PositionableIcon* icon, bool right, double fromPoint)
{
    //if(levelXing->getConnectD() instanceof TrackSegment){
 if(qobject_cast<TrackSegment*>(levelXing->getConnectD()))
 {
        TrackSegment* t = (TrackSegment*) levelXing->getConnectD();
        QPointF pf = levelXing->getCoordsD();
        QPoint p((int)pf.x(), (int)pf.y());

        QPointF end;
        if(t->getConnect1()==levelXing){
            end = layoutEditor->getCoords(t->getConnect2(), t->getType2());

        } else {
            end = layoutEditor->getCoords(t->getConnect1(), t->getType1());
        }
        //TrackSegment* t = boundary->getConnect2();
        bool east = false;

        if(end.x()<p.x())
            east = true;

        setIconOnPanel(t, icon, east, p, QPoint((int)end.x(),(int)end.y()), right, fromPoint);
        return;
    }
}	// initialize logging


#endif
/*public*/ void LayoutEditorTools::setSensorsAtTurnoutFromMenu(LayoutTurnout* to, QStringList blocks, MultiIconEditor* theEditor, JFrame* frame)
{
 turnoutSensorFromMenu = true;
 sensorIconEditor = theEditor;
 layoutTurnout = to;
 turnout = to->getTurnout();
 //turnoutMastNameField->setText(to->getTurnoutName());
 sensorsTurnoutComboBox->setText(to->getTurnoutName());
 turnoutSenBlocks =  QVector<QString>(4);
 for(int i = 0; i<blocks.length(); i++)
 {
  turnoutSenBlocks.replace(i, blocks.at(i));
 }
 setSensorsAtTurnouts(frame);
}

/*public*/ void LayoutEditorTools::setSensorsAtTurnouts(JFrame* frame)
{
    turnoutSensorFrame = frame;
    if (setSensorsOpen) {
        turnoutSensorsGetSaved(nullptr);
        setSensorsFrame->setVisible(true);
        return;
    }
    // Initialize if needed
    if (setSensorsFrame == nullptr) {
        setSensorsFrame = new JmriJFrameX(tr("Set Sensors at Turnout") ,false,true);
//        setSensorsFrame->layout()->addWidgetHelpMenu("package.jmri.jmrit.display.SetSensorsAtTurnout", true);
//        setSensorsFrame->setLocation(70,30);
        QWidget* theContentPane = new QWidget();
        setSensorsFrame->setCentralWidget(theContentPane);
        QVBoxLayout* centralWidgetLayout;
        theContentPane->setLayout(centralWidgetLayout=new QVBoxLayout(theContentPane/*, BoxLayout.Y_AXIS*/));
        QWidget* panel1 = new QWidget();
        panel1->setLayout(new QHBoxLayout());
        turnoutSensorA = new BeanDetails("Sensor", (Manager*)InstanceManager::sensorManagerInstance());
        turnoutSensorB = new BeanDetails("Sensor", (Manager*)InstanceManager::sensorManagerInstance());
        turnoutSensorC = new BeanDetails("Sensor", (Manager*)InstanceManager::sensorManagerInstance());
        turnoutSensorD = new BeanDetails("Sensor", (Manager*)InstanceManager::sensorManagerInstance());

        turnoutSensorNameLabel = new QLabel( tr("Turnout")+" "+
                                                            tr("Name") );
        panel1->layout()->addWidget(turnoutSensorNameLabel);
        panel1->layout()->addWidget(sensorsTurnoutComboBox);
        sensorsTurnoutComboBox->setToolTip(tr("Enter name (system or user) of turnout where sensors are needed."));

        centralWidgetLayout->addWidget(panel1);
        QFrame* line1 = new QFrame();
        line1->setObjectName(QString::fromUtf8("line"));
        line1->setGeometry(QRect(10, 30, 571, 16));
        line1->setFrameShape(QFrame::HLine);
        line1->setFrameShadow(QFrame::Sunken);
        centralWidgetLayout->addWidget(line1);
        QWidget* panel2 = new QWidget();
        panel2->setLayout(new QHBoxLayout());
        QLabel* shTitle = new QLabel(tr("Sensors"));
        panel2->layout()->addWidget(shTitle);
        panel2->layout()->addWidget(new QLabel("   "));
        panel2->layout()->addWidget(getSavedSensors = new QPushButton(tr("GetSaved")));
//        getSavedSensors->layout()->addWidgetActionListener(new ActionListener() {
//                /*public*/ void actionPerformed(ActionEvent e) {
//                    turnoutSensorsGetSaved(e);
//                }
//            });
        connect(getSavedSensors, SIGNAL(clicked()), this, SLOT(turnoutSensorsGetSaved()));
        getSavedSensors->setToolTip( "Click to retrieve signal heads previously stored." );
        centralWidgetLayout->addWidget(panel2);

//        turnoutSensorA->getDetailsPanel()->setBackground(new QColor(255,255,200));

//        turnoutSensorB->getDetailsPanel()->setBackground(new QColor(200,255,255));

//        turnoutSensorC->getDetailsPanel()->setBackground(new QColor(200,200,255));

//        turnoutSensorD->getDetailsPanel()->setBackground(new QColor(255,200,200));

        QGridLayout* g = new QGridLayout();
        sensorTurnoutPanel->setLayout(g = new QGridLayout()); //0,2));

        turnoutSensorA->setBoundaryLabel(turnoutSenBlocks.at(0));
        turnoutSensorB->setBoundaryLabel(turnoutSenBlocks.at(1));
        turnoutSensorC->setBoundaryLabel(turnoutSenBlocks.at(2));
        turnoutSensorD->setBoundaryLabel(turnoutSenBlocks.at(3));

        if(turnoutSenBlocks[0]!=nullptr)
            sensorTurnoutPanel->layout()->addWidget(turnoutSensorA->getDetailsPanel());
        if(turnoutSenBlocks[1]!=nullptr)
            sensorTurnoutPanel->layout()->addWidget(turnoutSensorB->getDetailsPanel());
        if(turnoutSenBlocks[2]!=nullptr)
            sensorTurnoutPanel->layout()->addWidget(turnoutSensorC->getDetailsPanel());
        if(turnoutSenBlocks[3]!=nullptr)
            sensorTurnoutPanel->layout()->addWidget(turnoutSensorD->getDetailsPanel());
        centralWidgetLayout->addWidget(sensorTurnoutPanel);

        QFrame* line2 = new QFrame();
        line2->setObjectName(QString::fromUtf8("line"));
        line2->setGeometry(QRect(10, 30, 571, 16));
        line2->setFrameShape(QFrame::HLine);
        line2->setFrameShadow(QFrame::Sunken);
        centralWidgetLayout->addWidget(line2);
        QWidget* panel6 = new QWidget();
        panel6->setLayout(new QHBoxLayout());
        panel6->layout()->addWidget(changeSensorIcon = new QPushButton(tr("Change Sensor Icon")));
//        changeSensorIcon->layout()->addWidgetActionListener(new ActionListener() {
//                /*public*/ void actionPerformed(ActionEvent e) {
//                    turnoutSensorFrame->setVisible(true);
//                }
//            });
        connect(changeSensorIcon, SIGNAL(clicked()), this, SLOT(on_changeSignalIcon_clicked()));
        changeSensorIcon->setToolTip( tr("Click to change icons used to represent sensors.") );
        panel6->layout()->addWidget(new QLabel("  "));
        panel6->layout()->addWidget(setSensorsDone = new QPushButton(tr("Done")));
//        setSensorsDone->layout()->addWidgetActionListener(new ActionListener() {
//            /*public*/ void actionPerformed(ActionEvent e) {
//                setSensorsDonePressed(e);
//            }
//        });
        connect(setSensorsDone, SIGNAL(clicked()), this, SLOT(setSensorsDonePressed()));
        setSensorsDone->setToolTip( tr("SensorDoneHint") );
        panel6->layout()->addWidget(setSensorsCancel = new QPushButton(tr("Cancel")));
//        setSensorsCancel->layout()->addWidgetActionListener(new ActionListener() {
//            /*public*/ void actionPerformed(ActionEvent e) {
//                setSensorsCancelPressed(e);
//            }
//        });
        connect(setSensorsCancel, SIGNAL(clicked()), this, SLOT(setSensorsCancelPressed()));
        setSensorsCancel->setToolTip( tr("Click Cancel to dismiss this dialog without making changes.") );
        centralWidgetLayout->addWidget(panel6);
//        setSensorsFrame->layout()->addWidgetWindowListener(new java.awt.event.WindowAdapter() {
//            /*public*/ void windowClosing(java.awt.event.WindowEvent e) {
//                setSensorsCancelPressed(nullptr);
//            }
//        });
    }
    if (turnoutFromMenu) turnoutSensorsGetSaved(nullptr);
//    setSensorsFrame->setPreferredSize(nullptr);
//    setSensorsFrame.pack();
    setSensorsFrame->setVisible(true);
    setSensorsOpen = true;
}

/*private*/ void LayoutEditorTools::turnoutSensorsGetSaved (ActionEvent* /*a*/) {
    if ( !getTurnoutSensorInformation() ) return;
    turnoutSenBlocks = layoutTurnout->getBlockBoundaries().toVector();
    turnoutSensorA->setTextField(layoutTurnout->getSensorAName());
    turnoutSensorB->setTextField(layoutTurnout->getSensorBName());
    turnoutSensorC->setTextField(layoutTurnout->getSensorCName());
    turnoutSensorD->setTextField(layoutTurnout->getSensorDName());

    turnoutSensorA->setBoundaryLabel(turnoutSenBlocks.at(0));
    turnoutSensorB->setBoundaryLabel(turnoutSenBlocks.at(1));
    turnoutSensorC->setBoundaryLabel(turnoutSenBlocks.at(2));
    turnoutSensorD->setBoundaryLabel(turnoutSenBlocks.at(3));

    sensorTurnoutPanel->layout()->removeWidget(turnoutSensorA->getDetailsPanel());
    sensorTurnoutPanel->layout()->removeWidget(turnoutSensorB->getDetailsPanel());
    sensorTurnoutPanel->layout()->removeWidget(turnoutSensorC->getDetailsPanel());
    sensorTurnoutPanel->layout()->removeWidget(turnoutSensorD->getDetailsPanel());

    bool blockBoundary = false;
    if(turnoutSenBlocks[0]!=nullptr){
        sensorTurnoutPanel->layout()->addWidget(turnoutSensorA->getDetailsPanel());
        blockBoundary = true;
    }
    if(turnoutSenBlocks[1]!=nullptr){
        sensorTurnoutPanel->layout()->addWidget(turnoutSensorB->getDetailsPanel());
        blockBoundary = true;
    }
    if(turnoutSenBlocks[2]!=nullptr){
        sensorTurnoutPanel->layout()->addWidget(turnoutSensorC->getDetailsPanel());
        blockBoundary = true;
    }
    if(turnoutSenBlocks[3]!=nullptr){
        sensorTurnoutPanel->layout()->addWidget(turnoutSensorD->getDetailsPanel());
        blockBoundary = true;
    }
    if (!blockBoundary)
//        JOptionPane.showMessageDialog(setSensorsFrame, "There are no block boundaries on this turnout\nIt is therefore not possible to add Sensors to it");
        QMessageBox::information(setSensorsFrame, tr("Information"), tr("There are no block boundaries on this turnout\nIt is therefore not possible to add Sensors to it"));
//    setSensorsFrame->setPreferredSize(nullptr);
//    setSensorsFrame.pack();
}

/*private*/ int LayoutEditorTools::isSensorAssignedHere(Sensor* sensor, LayoutTurnout* lTurnout)
{
    if ((sensor==nullptr) || (lTurnout==nullptr))
        return NONE;
    QString sysName = sensor->getSystemName();
    QString uName = sensor->getUserName();
    QString name = lTurnout->getSensorAName();
    if ( (name!=nullptr) && (name.length()>0) && ((name==(uName)) ||
                    (name==(sysName))) ) return A1;
    name = lTurnout->getSensorBName();
    if ( (name!=nullptr) && (name.length()>0) && ((name==(uName)) ||
                    (name==(sysName))) ) return A2;
    name = lTurnout->getSensorCName();
    if ( (name!=nullptr) && (name.length()>0) && ((name==(uName)) ||
                    (name==(sysName))) ) return A3;
    name = lTurnout->getSensorDName();
    if ( (name!=nullptr) && (name.length()>0) && ((name==(uName)) ||
                    (name==(sysName))) ) return B1;
    return NONE;
}

///*public*/ bool LayoutEditorTools::removeAssignment(Sensor* sensor)
//{
// QString sysName = sensor->getSystemName();
// QString uName = sensor->getUserName();
// log->trace(tr("Remove sensor assignment at block boundary for '%1'").(arg(sensor->getDisplayName())));  // NOI18N
//         if (!static_cast<EntryExitPairs*>(InstanceManager::getDefault("EntryExitPairs"))->deleteNxPair(sensor)) {
//             log->trace(tr("Removal of NX pairs for sensor '%1' failed").arg(sensor->getDisplayName()));  // NOI18N
//             return false;
//         }
//         for (PositionablePoint* po : layoutEditor->getPositionablePoints()) {
//             if (po->getEastBoundSensor() == sensor) {
//                 po->setEastBoundSensor("");
//             }
//             if (po->getWestBoundSensor() == sensor) {
//                 po->setWestBoundSensor("");
//             }
//         }

//         for (LayoutTurnout* to : layoutEditor->getLayoutTurnouts()) {
//          if ((to->getSensorA()!=nullptr) &&
//                (to->getSensorAName()==(sName) || ((uName!=nullptr) &&
//                (to->getSensorAName()==(uName))))) to->setSensorA("");
//        if ((to->getSensorB()!=nullptr) &&
//                (to->getSensorBName()==(sName) || ((uName!=nullptr) &&
//                (to->getSensorBName()==(uName))))) to->setSensorB("");
//        if ((to->getSensorC()!=nullptr) &&
//                (to->getSensorCName()==(sName) || ((uName!=nullptr) &&
//                (to->getSensorCName()==(uName))))) to->setSensorC("");
//        if ((to->getSensorD()!=nullptr) &&
//                (to->getSensorDName()==(sName) || ((uName!=nullptr) &&
//                (to->getSensorDName()==(uName))))) to->setSensorD("");
//    }

//         for (LayoutSlip* to : layoutEditor->getLayoutSlips()) {
//                    if (to->getSensorA() == sensor) {
//                        to->setSensorA("");
//                    }
//                    if (to->getSensorB() == sensor) {
//                        to->setSensorB("");
//                    }
//                    if (to->getSensorC() == sensor) {
//                        to->setSensorC("");
//                    }
//                    if (to->getSensorD() == sensor) {
//                        to->setSensorD("");
//                    }
//                }
//         for (LevelXing* x : layoutEditor->getLevelXings()) {
//        if ((x->getSensorAName()!=nullptr) &&
//                (x->getSensorAName()==(sName) || ((uName!=nullptr) &&
//                (x->getSensorAName()==(uName))))) x->setSensorAName("");
//        if ((x->getSensorBName()!=nullptr) &&
//                (x->getSensorBName()==(sName) || ((uName!=nullptr) &&
//                (x->getSensorBName()==(uName))))) x->setSensorBName("");
//        if ((x->getSensorCName()!=nullptr) &&
//                (x->getSensorCName()==(sName) || ((uName!=nullptr) &&
//                (x->getSensorCName()==(uName))))) x->setSensorCName("");
//        if ((x->getSensorDName()!=nullptr) &&
//                (x->getSensorDName()==(sName) || ((uName!=nullptr) &&
//                (x->getSensorDName()==(uName))))) x->setSensorDName("");
//    }
//}


/*private*/ void LayoutEditorTools::setSensorsDonePressed (ActionEvent* /*a*/) // SLOT
{
//Placing of turnouts needs to be better handled
    // process turnout name
    if ( !getTurnoutSensorInformation() ) return;
    // process signal head names
    //if ( !getSensorTurnoutInformation() ) return;
    Sensor* sensorA = getSensorFromEntry(turnoutSensorA->getText(),false,setSensorsFrame);
    //if (turnoutSensor==nullptr) return false;
    Sensor* sensorB = getSensorFromEntry(turnoutSensorB->getText(),false,setSensorsFrame);
    //if (turnoutSensorB==nullptr) return false;
    Sensor* sensorC = getSensorFromEntry(turnoutSensorC->getText(),false,setSensorsFrame);
    //if (turnoutSensorC==nullptr) return false;
    Sensor* sensorD = getSensorFromEntry(turnoutSensorD->getText(),false,setSensorsFrame);
    // place signals as requested
    if (turnoutSensorA->addToPanel())
    {
     if (isSensorOnPanel(sensorA) &&
            (sensorA!=getSensorFromName(layoutTurnout->getSensorAName())))
     {
//            JOptionPane.showMessageDialog(setSensorsFrame,
//                java.text.MessageFormat.format(tr("SensorsError6"),
//                    new Object[]{turnoutSensorA->text()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
      QMessageBox::critical(setSensorsFrame, tr("Error"), tr("Error - Cannot place sensor \"%1\" on the panel\nbecause it is already on the panel.").arg(turnoutSensorA->getText())) ;
      return;
     }
     else {
            removeSensorFromPanel(layoutTurnout->getSensorA());
            placingBlock(getSensorIcon(turnoutSensorA->getText()), turnoutSensorA->isRightSelected(), 0.0);
//            removeAssignment(sensorA);
            layoutTurnout->setSensorA(turnoutSensorA->getText());
            needRedraw = true;
        }
    }
    else if (sensorA!=nullptr)
    {
        int assigned = isSensorAssignedHere(sensorA,layoutTurnout);
        if (assigned == NONE) {
            if ( isSensorOnPanel(sensorA) &&
                                isSensorAssignedAnywhere(sensorA) )
            {

//                JOptionPane.showMessageDialog(setSensorsFrame,
//                    java.text.MessageFormat.format(tr("SensorsError8"),
//                        new Object[]{turnoutSensorA->text()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
                QMessageBox::critical(setSensorsFrame, tr("Error"), tr("Error - Cannot assign sensor \"%1\" to this turnout\nbecause it is already on the panel at a different place.").arg(turnoutSensorA->getText()));
                return;
            }
            else {
                removeSensorFromPanel(layoutTurnout->getSensorA());
//                removeAssignment(sensorA);
                layoutTurnout->setSensorA(turnoutSensorA->getText());
            }
        }
        else if (assigned!=A1) {
// need to figure out what to do in this case.
        }
    }
    else {
        removeSensorFromPanel(layoutTurnout->getSensorA());
        layoutTurnout->setSensorA("");
    }
    if ( (turnoutSensorB!=nullptr) && (turnoutSensorB->addToPanel())) {
        if (isSensorOnPanel(sensorB) &&
            (sensorB!=getSensorFromName(layoutTurnout->getSensorBName())))
        {
//            JOptionPane.showMessageDialog(setSensorsFrame,
//                java.text.MessageFormat.format(tr("SensorsError6"),
//                    new Object[]{turnoutSensorB->text()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
            QMessageBox::critical(setSensorsFrame, tr("Error"), tr("Error - Cannot place sensor \"%1\" on the panel\nbecause it is already on the panel.").arg(turnoutSensorB->getText()));
            return;
        }
        else {
            removeSensorFromPanel(layoutTurnout->getSensorB());

            placingBlockB(getSensorIcon(turnoutSensorB->getText()), turnoutSensorB->isRightSelected(), 0.0);
//            removeAssignment(sensorB);
            layoutTurnout->setSensorB(turnoutSensorB->getText());
            needRedraw = true;
        }
    }
    else if (sensorB!=nullptr) {
        int assigned = isSensorAssignedHere(sensorB,layoutTurnout);
        if (assigned == NONE) {
            if (isSensorOnPanel(sensorB) &&
                                isSensorAssignedAnywhere(sensorB) ) {
//                JOptionPane.showMessageDialog(setSensorsFrame,
//                    java.text.MessageFormat.format(tr("SensorsError8"),
//                        new Object[]{turnoutSensorB->text()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
                QMessageBox::critical(setSensorsFrame, tr("Error"), tr("Error - Cannot assign sensor \"%1\" to this turnout\nbecause it is already on the panel at a different place.").arg(turnoutSensorB->getText()));

                return;
            }
            else {
                removeSensorFromPanel(layoutTurnout->getSensorB());
//                removeAssignment(sensorB);
                layoutTurnout->setSensorB(turnoutSensorB->getText());
            }
        }
        else if (assigned!=A2) {
// need to figure out what to do in this case.
        }
    }
    else {
        removeSensorFromPanel(layoutTurnout->getSensorB());
        layoutTurnout->setSensorB("");
    }
    if(sensorC!=nullptr){
        if (turnoutSensorC->addToPanel()) {
            if (isSensorOnPanel(sensorC) &&
                (sensorC!=getSensorFromName(layoutTurnout->getSensorCName()))) {
//                JOptionPane.showMessageDialog(setSensorsFrame,
//                    java.text.MessageFormat.format(tr("SensorsError6"),
//                        new Object[]{turnoutSensorC->text()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
                QMessageBox::critical(setSensorsFrame, tr("Error"), tr("Error - Cannot place sensor \"%1\" on the panel\nbecause it is already on the panel.").arg(turnoutSensorC->getText()));

                return;
            }
            else {
                removeSensorFromPanel(layoutTurnout->getSensorC());

                placingBlockC(getSensorIcon(turnoutSensorC->getText()), turnoutSensorC->isRightSelected(), 0.0);
//                removeAssignment(sensorC);
                layoutTurnout->setSensorC(turnoutSensorC->getText());
                needRedraw = true;
            }
        }
        else {
            int assigned = isSensorAssignedHere(sensorC,layoutTurnout);
            if (assigned == NONE) {
                if (isSensorOnPanel(sensorC)  &&
                                    isSensorAssignedAnywhere(sensorC) ) {
//                    JOptionPane.showMessageDialog(setSensorsFrame,
//                        java.text.MessageFormat.format(tr("SensorsError8"),
//                            new Object[]{turnoutSensorC->text()}),
//                                tr("Error"),JOptionPane.ERROR_MESSAGE);
                    QMessageBox::critical(setSensorsFrame, tr("Error"), tr("Error - Cannot assign sensor \"%1\" to this turnout\nbecause it is already on the panel at a different place.").arg(turnoutSensorC->getText()));

                    return;
                }
                else {
                    removeSensorFromPanel(layoutTurnout->getSensorC());
//                    removeAssignment(sensorC);
                    layoutTurnout->setSensorC(turnoutSensorC->getText());
                }
            }
            else if (assigned!=A3) {
// need to figure out what to do in this case.
            }
        }
    }
    else {
        removeSensorFromPanel(layoutTurnout->getSensorC());
        layoutTurnout->setSensorC("");
    }
    if(sensorD!=nullptr){
        if (turnoutSensorD->addToPanel()) {
            if (isSensorOnPanel(sensorD) &&
                (sensorD!=getSensorFromName(layoutTurnout->getSensorDName()))) {
//                JOptionPane.showMessageDialog(setSensorsFrame,
//                    java.text.MessageFormat.format(tr("SensorsError6"),
//                        new Object[]{divergingField->text()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
                QMessageBox::critical(setSensorsFrame, tr("Error"), tr("Error - Cannot place sensor \"%1\" on the panel\nbecause it is already on the panel.").arg(turnoutSensorD->getText()));

                return;
            }
            else {
                removeSensorFromPanel(layoutTurnout->getSensorD());
                placingBlockD(getSensorIcon(turnoutSensorD->getText()), turnoutSensorD->isRightSelected(), 0.0);
//                removeAssignment(sensorD);
                layoutTurnout->setSensorD(turnoutSensorD->getText());
                needRedraw = true;
            }
        }
        else {
            int assigned = isSensorAssignedHere(sensorD,layoutTurnout);
            if (assigned == NONE) {
                if (isSensorOnPanel(sensorD) &&
                                    isSensorAssignedAnywhere(sensorD) ) {
//                    JOptionPane.showMessageDialog(setSensorsFrame,
//                        java.text.MessageFormat.format(tr("SensorsError8"),
//                            new Object[]{turnoutSensorD->text()}),
//                                tr("Error"),JOptionPane.ERROR_MESSAGE);
                    QMessageBox::critical(setSensorsFrame, tr("Error"), tr("Error - Cannot assign sensor \"%1\" to this turnout\nbecause it is already on the panel at a different place.").arg(turnoutSensorD->getText()));

                    return;
                }
                else {
                    removeSensorFromPanel(layoutTurnout->getSensorD());
//                    removeAssignment(sensorD);
                    layoutTurnout->setSensorD(turnoutSensorD->getText());
                }
            }
            else if (assigned!=B1) {
// need to figure out what to do in this case.
            }
        }
    } else {
        removeSensorFromPanel(layoutTurnout->getSensorD());
        layoutTurnout->setSensorD("");
    }

    // make sure this layout turnout is not linked to another
    layoutTurnout->setLinkType(LayoutTurnout::NO_LINK);
    layoutTurnout->setLinkedTurnoutName("");
    // finish up
    setSensorsOpen = false;
    turnoutFromMenu = false;
    setSensorsFrame->setVisible(false);
    if (needRedraw) {
        layoutEditor->redrawPanel();
        needRedraw = false;
        layoutEditor->setDirty();
    }
}

/*private*/ bool LayoutEditorTools::getTurnoutSensorInformation(){
    LayoutTurnout* t = NULL;
    QString str = "";
    turnout = NULL;
    layoutTurnout = NULL;
    str = sensorsTurnoutComboBox->currentText();
    if ( (str==nullptr) || (str==("")) ) {
//        JOptionPane.showMessageDialog(setSensorsFrame,tr("SensorsError1"),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
        QMessageBox::critical(setSensorsFrame, tr("Error"),tr("Error - No turnout name was entered. Please enter a turnout name or cancel."));
        return false;
    }
    turnout = ((ProxyTurnoutManager*)InstanceManager::turnoutManagerInstance())->getTurnout(str);
    if (turnout==nullptr) {
//        JOptionPane.showMessageDialog(setSensorsFrame,
//            java.text.MessageFormat.format(tr("SensorsError2"),
//                new Object[]{str}), tr("Error"),
//                    JOptionPane.ERROR_MESSAGE);
        QMessageBox::critical(setSensorsFrame, tr("Error"), tr("Error - No turnout is defined for \"%1\". Please enter\n                                                               a turnout name in the Turnout Table and on the panel.").arg(str));
        return false ;
    }
    else if ( (turnout->getUserName()==nullptr) || (turnout->getUserName()==("")) ||
                            turnout->getUserName()!=(str) ) {
        sensorsTurnoutComboBox->setText(str);
    }
    layoutTurnout = layoutEditor->getFinder()->findLayoutTurnoutByBean(turnout);
            if (layoutTurnout == nullptr) {
                JOptionPane::showMessageDialog(layoutEditor,
                        tr("Error - Turnout \"{0}\" is not drawn on the panel.\nPlease enter the name of a drawn turnout.").arg(
                                str), tr("Error"),
                        JOptionPane::ERROR_MESSAGE);
                return false;
            }
            return true;
}

/*private*/ void LayoutEditorTools::setSensorsCancelPressed (ActionEvent* /*a*/) {
    setSensorsOpen =false;
    turnoutSensorFromMenu = false;
    setSensorsFrame->setVisible(false);
}


// display dialog for Set Signals at Level Crossing tool
/*public*/ void LayoutEditorTools::setSensorsAtLevelXingFromMenu (LevelXing* xing, QVector<QString>* blocks, MultiIconEditor* theEditor,
        JFrame* theFrame )
{
 xingSensorFromMenu = true;
 levelXing = xing;
 blockANameSensorField->setText(levelXing->getBlockNameAC());
 blockCNameSensorField->setText(levelXing->getBlockNameBD());
 for(int i = 0; i<blocks->size(); i++)
 {
  xingSensorBlocks->replace(i, blocks->at(i));
 }
 setSensorsAtLevelXing(theEditor, theFrame);
 return;
}

/*public*/ void LayoutEditorTools::setSensorsAtLevelXing(MultiIconEditor* theEditor, JFrame* theFrame)
{
 sensorIconEditor = theEditor;
 sensorXingFrame = theFrame;
 if (setSensorsAtXingOpen)
 {
     xingSensorsGetSaved(nullptr);
     sensorsAtXingFrame->setVisible(true);
     return;
 }
 // Initialize if needed
 if (sensorsAtXingFrame == nullptr)
  sensorsAtXingFrame = new JmriJFrameX( tr("Set Sensors at Level Crossing"), false, true );

  sensorsAtXingFrame->addHelpMenu("package.jmri.jmrit.display.SetSensorsAtLevelXing", true);
  sensorsAtXingFrame->setLocation(70,30);
  QWidget* theContentPane = /*sensorsAtXingFrame.getContentPane();*/ new QWidget;
  sensorsAtXingFrame->setCentralWidget(theContentPane);
  QVBoxLayout* centralWidgetLayout;
  theContentPane->setLayout(centralWidgetLayout = new QVBoxLayout); //, BoxLayout.Y_AXIS));

  QWidget* panel11 = new QWidget();
  panel11->setLayout(new QHBoxLayout());
  xingSensorA = new BeanDetails("Sensor", (Manager*)InstanceManager::sensorManagerInstance());
  xingSensorB = new BeanDetails("Sensor", (Manager*)InstanceManager::sensorManagerInstance());
  xingSensorC = new BeanDetails("Sensor", (Manager*)InstanceManager::sensorManagerInstance());
  xingSensorD = new BeanDetails("Sensor", (Manager*)InstanceManager::sensorManagerInstance());
  if (xingSensorFromMenu)
  {
   QLabel* blockANameLabel = new QLabel( tr("BlockAtA")+" "+
               tr("Name")+" : "+levelXing->getBlockNameAC());

   panel11->layout()->addWidget(blockANameLabel);
   xingSensorA->setTextField(levelXing->getSensorAName());
   xingSensorB->setTextField(levelXing->getSensorBName());
   xingSensorC->setTextField(levelXing->getSensorCName());
   xingSensorD->setTextField(levelXing->getSensorDName());
  }
  else
  {
   QLabel* blockANameLabel = new QLabel( tr("BlockAtA")+" "+
                                                   tr("Name")+" : ");
   panel11->layout()->addWidget(blockANameLabel);
   panel11->layout()->addWidget(blockANameSensorField);
   blockANameSensorField->setToolTip(tr("Enter name of one block where sensors are needed at block boundary."));
  }
  centralWidgetLayout->addWidget(panel11);
  QWidget* panel12 = new QWidget();
  panel12->setLayout(new QHBoxLayout());
  if (xingSensorFromMenu)
  {
   QLabel* blockCNameLabel = new QLabel( tr("BlockAtC")+" "+
               tr("Name")+" : "+levelXing->getBlockNameBD());

   panel12->layout()->addWidget(blockCNameLabel);
  }
  else
  {
   QLabel* blockCNameLabel = new QLabel( tr("BlockAtC")+" "+
                                                   tr("Name")+" : ");
   panel12->layout()->addWidget(blockCNameLabel);
   panel12->layout()->addWidget(blockCNameSensorField);
   blockCNameSensorField->setToolTip(tr("SensorsBlockNameHint"));
  }
  centralWidgetLayout->addWidget(panel12);
  centralWidgetLayout->addWidget(new JSeparator(JSeparator::HORIZONTAL));
  QWidget* panel2 = new QWidget();
  panel2->setLayout(new QHBoxLayout());
  QLabel* shTitle = new QLabel(tr("Sensor"));
  panel2->layout()->addWidget(shTitle);
  panel2->layout()->addWidget(new QLabel("   "));
  panel2->layout()->addWidget(getSavedXingSensors = new QPushButton(tr("GetSaved")));
//  getSavedXingSensors->layout()->addWidgetActionListener(new ActionListener() {
//          /*public*/ void actionPerformed(ActionEvent e) {
//              xingSensorsGetSaved(e);
//          }
//      });
  connect(getSavedXingSensors, SIGNAL(clicked()), this, SLOT(xingSensorsGetSaved()));
  getSavedXingSensors->setToolTip( "Click to retrieve signal heads previously stored.");
  centralWidgetLayout->addWidget(panel2);

  //xingSensorA.getDetailsPanel()->setBackground(new Color(255,255,200));
  xingSensorA->getDetailsPanel()->setStyleSheet("QWidget { background: rgb(255,255,200)}");

  //xingSensorB.getDetailsPanel()->setBackground(new Color(200,255,255));
  xingSensorB->getDetailsPanel()->setStyleSheet("QWidget { background: rgb(200,255,255)}");

  //xingSensorC.getDetailsPanel()->setBackground(new Color(200,200,255));
  xingSensorC->getDetailsPanel()->setStyleSheet("QWidget { background: rgb(200,200,2055)}");

  //xingSensorD.getDetailsPanel()->setBackground(new Color(255,200,200));
  xingSensorD->getDetailsPanel()->setStyleSheet("QWidget { background: rgb(255,200,200)}");


  sensorXingPanel->setLayout(sensorXingPanelLayout = new QGridLayout(/*0,2*/));

  xingSensorA->setBoundaryLabel(xingSensorBlocks->at(0));
  xingSensorB->setBoundaryLabel(xingSensorBlocks->at(1));
  xingSensorC->setBoundaryLabel(xingSensorBlocks->at(2));
  xingSensorD->setBoundaryLabel(xingSensorBlocks->at(3));

  if(xingSensorBlocks->at(0)!=nullptr)
      sensorXingPanelLayout->addWidget(xingSensorA->getDetailsPanel(),0,0);
  if(xingSensorBlocks->at(1)!=nullptr)
      sensorXingPanelLayout->addWidget(xingSensorB->getDetailsPanel(),0,1);
  if(xingSensorBlocks->at(2)!=nullptr)
      sensorXingPanelLayout->addWidget(xingSensorC->getDetailsPanel(),1,0);
  if(xingSensorBlocks->at(3)!=nullptr)
      sensorXingPanelLayout->addWidget(xingSensorD->getDetailsPanel(), 1,1);
  centralWidgetLayout->addWidget(sensorXingPanel);

  centralWidgetLayout->addWidget(new JSeparator(JSeparator::HORIZONTAL));
  QWidget* panel6 = new QWidget();
  panel6->setLayout(new QHBoxLayout());
  panel6->layout()->addWidget(changeSensorXingIcon = new QPushButton(tr("Change Sensor con")));
  //  changeSensorXingIcon->layout()->addWidgetActionListener(new ActionListener() {
  //          /*public*/ void actionPerformed(ActionEvent e) {
  //              sensorXingFrame->setVisible(true);
  //          }
  //      });
  connect(changeSensorXingIcon, SIGNAL(clicked()), this, SLOT(on_changeSensorXingIcon()));
  changeSensorXingIcon->setToolTip( tr("Click to change icons used to represent sensors.") );

  panel6->layout()->addWidget(new QLabel("  "));
  panel6->layout()->addWidget(setXingSensorsDone = new QPushButton(tr("Done")));
//  setXingSensorsDone->layout()->addWidgetActionListener(new ActionListener() {
//      /*public*/ void actionPerformed(ActionEvent e) {
//          setXingSensorsDonePressed(e);
//      }
//  });
  connect(setXingSensorsDone, SIGNAL(clicked()), this, SLOT(setXingSensorsDonePressed()));
  setXingSensorsDone->setToolTip( tr("SensorDoneHint") );
  panel6->layout()->addWidget(setXingSensorsCancel = new QPushButton(tr("Cancel")));
//  setXingSensorsCancel->layout()->addWidgetActionListener(new ActionListener() {
//      /*public*/ void actionPerformed(ActionEvent e) {
//          setXingSensorsCancelPressed(e);
//      }
//  });
  connect(setXingSensorsCancel, SIGNAL(clicked()), this, SLOT(setXingSensorsCancelPressed()));
  setXingSensorsCancel->setToolTip( tr("Click Cancel to dismiss this dialog without making changes.") );
  centralWidgetLayout->addWidget(panel6);
//  sensorsAtXingFrame->layout()->addWidgetWindowListener(new java.awt.event.WindowAdapter() {
//      /*public*/ void windowClosing(java.awt.event.WindowEvent e) {
//          setXingSensorsCancelPressed(nullptr);
//      }
//     });
  sensorsAtXingFrame->addWindowListener(new XSSSWindowListener(this));

 if (xingSensorFromMenu) xingSensorsGetSaved(nullptr);
 sensorsAtXingFrame->setMinimumSize(QSize());
 sensorsAtXingFrame->adjustSize();
 sensorsAtXingFrame->setVisible(true);
 setSensorsAtXingOpen = true;
}

/*private*/ void LayoutEditorTools::xingSensorsGetSaved (ActionEvent* /*a*/)
{
 if ( !getLevelCrossingSensorInformation() ) return;

 xingSensorBlocks = levelXing->getBlockBoundaries();
 xingSensorA->setTextField(levelXing->getSensorAName());
 xingSensorB->setTextField(levelXing->getSensorBName());
 xingSensorC->setTextField(levelXing->getSensorCName());
 xingSensorD->setTextField(levelXing->getSensorDName());

 sensorXingPanel->layout()->removeWidget(xingSensorA->getDetailsPanel());
 sensorXingPanel->layout()->removeWidget(xingSensorB->getDetailsPanel());
 sensorXingPanel->layout()->removeWidget(xingSensorC->getDetailsPanel());
 sensorXingPanel->layout()->removeWidget(xingSensorD->getDetailsPanel());

 xingSensorA->setBoundaryLabel(xingSensorBlocks->at(0));
 xingSensorB->setBoundaryLabel(xingSensorBlocks->at(1));
 xingSensorC->setBoundaryLabel(xingSensorBlocks->at(2));
 xingSensorD->setBoundaryLabel(xingSensorBlocks->at(3));

 bool boundary = false;
 if(xingSensorBlocks->at(0)!=nullptr)
 {
  sensorXingPanelLayout->addWidget(xingSensorA->getDetailsPanel());
  boundary = true;
 }
 if(xingSensorBlocks->at(1)!=nullptr)
 {
  sensorXingPanelLayout->addWidget(xingSensorB->getDetailsPanel());
  boundary = true;
 }
 if(xingSensorBlocks->at(2)!=nullptr){
     sensorXingPanelLayout->addWidget(xingSensorC->getDetailsPanel());
     boundary = true;
 }
 if(xingSensorBlocks->at(3)!=nullptr){
     sensorXingPanelLayout->addWidget(xingSensorD->getDetailsPanel());
     boundary = true;
 }
 if (!boundary)
 {
  //JOptionPane.showMessageDialog(sensorsAtXingFrame, tr("NoBoundaryXingSensor"));
    QMessageBox::information(sensorsAtXingFrame, tr("Information"), tr("There are no block boundaries on this level crossing\nIt is therefore not possible to add Sensors to it"));
 }
 sensorsAtXingFrame->setMinimumSize(QSize());
 sensorsAtXingFrame->adjustSize();
}

void LayoutEditorTools::on_changeSensorXingIcon()
{
 sensorXingFrame->setVisible(true);
}
/*private*/ bool LayoutEditorTools::getLevelCrossingSensorInformation()
{
QList<LevelXing*> levelXings = layoutEditor->getLevelXings();
if (!xingSensorFromMenu)
{
  if (levelXings.size()<=0)
 {
//     JOptionPane.showMessageDialog(sensorsAtXingFrame,
//         tr("SignalsError15"),
//                     tr("Error"),JOptionPane.ERROR_MESSAGE);
  QMessageBox::critical(sensorsAtXingFrame, tr("Error"), tr("Error - There are no level crossings on your panel.\nPlease add a level crossing or cancel."));
  return false;
 }
 else if (levelXings.size()==1)
 {
  levelXing = levelXings.at(0);
 }
 else
 {
  LayoutBlock* xingSensorBlockA = NULL;
  LayoutBlock* xingSensorBlockC = NULL;
  xingSensorBlockA = getBlockFromEntry(blockANameSensorField);
  if (xingSensorBlockA==nullptr) return false;
  if (blockCNameSensorField->text().length()>0)
  {
   xingSensorBlockC = getBlockFromEntry(blockCNameSensorField);
   if (xingSensorBlockC==nullptr) return false;
  }
  LevelXing* x = NULL;
  int foundCount = 0;
  // make two block tests first
  if (xingSensorBlockC!=nullptr)
  {
   for (int i = 0;(i<levelXings.size());i++)
   {
    x = levelXings.at(i);
    LayoutBlock* xA = NULL;
    LayoutBlock* xB = NULL;
    LayoutBlock* xC = NULL;
    LayoutBlock* xD = NULL;
    LayoutBlock* xAC = x->getLayoutBlockAC();
    LayoutBlock* xBD = x->getLayoutBlockBD();
    if (x->getConnectA()!=nullptr) xA = ((TrackSegment*)x->getConnectA())->getLayoutBlock();
    if (x->getConnectB()!=nullptr) xB = ((TrackSegment*)x->getConnectB())->getLayoutBlock();
    if (x->getConnectC()!=nullptr) xC = ((TrackSegment*)x->getConnectC())->getLayoutBlock();
    if (x->getConnectD()!=nullptr) xD = ((TrackSegment*)x->getConnectD())->getLayoutBlock();
    if ( ( (xA!=nullptr) && (xC!=nullptr) && ( ((xA==xingSensorBlockA)&&(xC==xingSensorBlockC)) ||
            ((xA==xingSensorBlockC)&&(xC==xingSensorBlockA)) ) ) ||
            ( (xB!=nullptr) && (xD!=nullptr) && ( ((xB==xingSensorBlockA)&&(xD==xingSensorBlockC)) ||
            ((xB==xingSensorBlockC)&&(xD==xingSensorBlockA)) ) ) )
    {
     levelXing = x;
     foundCount ++;
    }
    else if ( (xAC!=nullptr) && (xBD!=nullptr) && ( ((xAC==xingSensorBlockA) && (xBD==xingSensorBlockC)) ||
                ((xAC==xingSensorBlockC) && (xBD==xingSensorBlockA)) ) )
    {
     levelXing = x;
     foundCount ++;
    }
   }
  }
  if (foundCount==0)
  {
   // try one block test
   for (int i = 0;(i<levelXings.size());i++)
   {
    x = levelXings.at(i);
    if ((xingSensorBlockA == x->getLayoutBlockAC()) || (xingSensorBlockA == x->getLayoutBlockBD())) {
        levelXing = x;
        foundCount ++;
    }
   }
  }
  if (foundCount>1)
  {
//      JOptionPane.showMessageDialog(sensorsAtXingFrame,
//              java.text.MessageFormat.format(tr("SignalsError16"),
//                  new Object[]{" "+foundCount+" "}),
//                      tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(signalMastsAtXingFrame, tr("Error"), tr("Error - The entered block names match \"%1\" level crossings.\n           Please enter block names that match only one level crossing.").arg(foundCount)); return false;
   return false;
  }
  if (levelXing==nullptr)
  {
//      JOptionPane.showMessageDialog(sensorsAtXingFrame,
//                  tr("SignalsError17"),
//                          tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(signalMastsAtXingFrame, tr("Error"), tr("Error - No level crossing matches the block(s)\nentered. Please try again or cancel."));return false;
      return false;
  }
 }
}
return true;
}

/*private*/ void LayoutEditorTools::setXingSensorsCancelPressed (ActionEvent* /*a*/)
{
 setSensorsAtXingOpen = false;
 sensorsAtXingFrame->setVisible(false);
 xingSensorFromMenu = false;
}

/*private*/ void LayoutEditorTools::setXingSensorsDonePressed (ActionEvent* /*a*/)
{
 if ( !getLevelCrossingSensorInformation() ) return;
 Sensor* aSensor = getSensorFromEntry(xingSensorA->getText(),false,sensorsAtXingFrame);
 Sensor* bSensor = getSensorFromEntry(xingSensorB->getText(),false,sensorsAtXingFrame);
 Sensor* cSensor = getSensorFromEntry(xingSensorC->getText(),false,sensorsAtXingFrame);
 Sensor* dSensor = getSensorFromEntry(xingSensorD->getText(),false,sensorsAtXingFrame);
 // place or update signals as requested
 if ( (aSensor!=nullptr) && xingSensorA->addToPanel() )
 {
  if (isSensorOnPanel(aSensor) &&
            (aSensor!=getSensorFromName(levelXing->getSensorAName())) )
  {
//        JOptionPane.showMessageDialog(sensorsAtXingFrame,
//            java.text.MessageFormat.format(tr("SensorsError6"),
//                new Object[]{xingSensorA->text()}),
//                    tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(sensorsAtXingFrame, tr("Error"), tr("Error - Cannot place sensor \"%1\" on the panel\nbecause it is already on the panel.").arg(xingSensorA->getText()));
        return;
  }
  else
  {
   removeSensorFromPanel(levelXing->getSensorA());
   placeXingAIcon(getSensorIcon(xingSensorA->getText()), xingSensorA->isRightSelected(), 0.0);
//   removeAssignment(aSensor);
   levelXing->setSensorAName(xingSensorB->getText());
   needRedraw = true;
  }
 }
 else if ( (aSensor!=nullptr) &&
        (aSensor!=getSensorFromName(levelXing->getSensorAName())) &&
        (aSensor!=getSensorFromName(levelXing->getSensorBName())) &&
        (aSensor!=getSensorFromName(levelXing->getSensorCName())) &&
        (aSensor!=getSensorFromName(levelXing->getSensorDName())) )
 {
  if (isSensorOnPanel(aSensor))
  {
//        JOptionPane.showMessageDialog(sensorsAtXingFrame,
//            java.text.MessageFormat.format(tr("SensorsError13"),
//                new Object[]{xingSensorA->text()}),
//                    tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(sensorsAtXingFrame, tr("Error"), tr("Error - Cannot assign sensor \"%1\" to this block boundary\nbecause it is already on the panel at a different place.").arg(xingSensorA->getText()));
      return;
    }
    else
  {
      removeSensorFromPanel(levelXing->getSensorA());
//      removeAssignment(aSensor);
      levelXing->setSensorAName(xingSensorA->getText());
  }
 }
 else if ( (aSensor!=nullptr) &&
        ( (aSensor==getSensorFromName(levelXing->getSensorBName())) ||
            (aSensor==getSensorFromName(levelXing->getSensorCName())) ||
            (aSensor==getSensorFromName(levelXing->getSensorDName())) ) )
 {
 // need to figure out what to do in this case.
 }
 else if (aSensor==nullptr)
 {
  removeSensorFromPanel(levelXing->getSensorA());
  levelXing->setSensorAName("");
 }

 if ( (bSensor!=nullptr) && xingSensorB->addToPanel() )
 {
  if (isSensorOnPanel(bSensor) &&
          (bSensor!=getSensorFromName(levelXing->getSensorBName()))) {
//        JOptionPane.showMessageDialog(sensorsAtXingFrame,
//            java.text.MessageFormat.format(tr("SensorsError6"),
//                new Object[]{xingSensorB->text()}),
//                    tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(sensorsAtXingFrame, tr("Error"), tr("Error - Cannot place sensor \"%1\" on the panel\nbecause it is already on the panel.").arg(xingSensorB->getText()));
      return;
  }
  else
  {
   removeSensorFromPanel(levelXing->getSensorB());
   placeXingBIcon(getSensorIcon(xingSensorB->getText()), xingSensorB->isRightSelected(), 0.0);
//   removeAssignment(bSensor);
   levelXing->setSensorBName(xingSensorB->getText());
   needRedraw = true;
  }
 }
 else if ( (bSensor!=nullptr) &&
        (bSensor!=getSensorFromName(levelXing->getSensorAName())) &&
        (bSensor!=getSensorFromName(levelXing->getSensorBName())) &&
        (bSensor!=getSensorFromName(levelXing->getSensorCName())) &&
        (bSensor!=getSensorFromName(levelXing->getSensorDName())) )
 {
  if (isSensorOnPanel(bSensor))
  {
//        JOptionPane.showMessageDialog(sensorsAtXingFrame,
//            java.text.MessageFormat.format(tr("SensorsError13"),
//                new Object[]{xingSensorB->text()}),
//                    tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(sensorsAtXingFrame, tr("Error"), tr("Error - Cannot assign sensor \"%1\" to this block boundary\nbecause it is already on the panel at a different place.").arg(xingSensorB->getText()));
      return;
  }
  else
  {
   removeSensorFromPanel(levelXing->getSensorB());
//   removeAssignment(bSensor);
   levelXing->setSensorBName(xingSensorB->getText());
  }
 }
 else if ( (bSensor!=nullptr) &&
        ( (bSensor==getSensorFromName(levelXing->getSensorAName())) ||
            (bSensor==getSensorFromName(levelXing->getSensorCName())) ||
            (bSensor==getSensorFromName(levelXing->getSensorDName())) ) )
 {
  // need to figure out what to do in this case.
 }
 else if (bSensor==nullptr)
 {
  removeSensorFromPanel(levelXing->getSensorB());
  levelXing->setSensorBName("");
 }

 if ( (cSensor!=nullptr) && xingSensorC->addToPanel() )
 {
  if (isSensorOnPanel(cSensor) &&
            (cSensor!=getSensorFromName(levelXing->getSensorCName())) )
  {
//        JOptionPane.showMessageDialog(sensorsAtXingFrame,
//            java.text.MessageFormat.format(tr("SensorsError6"),
//                new Object[]{xingSensorC->text()}),
//                    tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(sensorsAtXingFrame, tr("Error"), tr("Error - Cannot place sensor \"%1\" on the panel\nbecause it is already on the panel.").arg(xingSensorC->getText()));
   return;
  }
  else
  {
    removeSensorFromPanel(levelXing->getSensorC());
    placeXingCIcon(getSensorIcon(xingSensorC->getText()), xingSensorC->isRightSelected(), 0.0);
//    removeAssignment(cSensor);
    levelXing->setSensorCName(xingSensorC->getText());
    needRedraw = true;
  }
 }
 else if ( (cSensor!=nullptr) &&
        (cSensor!=getSensorFromName(levelXing->getSensorAName())) &&
        (cSensor!=getSensorFromName(levelXing->getSensorBName())) &&
        (cSensor!=getSensorFromName(levelXing->getSensorCName())) &&
        (cSensor!=getSensorFromName(levelXing->getSensorDName())) )
 {
  if (isSensorOnPanel(cSensor))
  {
//        JOptionPane.showMessageDialog(sensorsAtXingFrame,
//            java.text.MessageFormat.format(tr("SensorsError13"),
//                new Object[]{xingSensorC->text()}),
//                    tr("Error"),JOptionPane.ERROR_MESSAGE);
    QMessageBox::critical(sensorsAtXingFrame, tr("Error"), tr("Error - Cannot assign sensor \"%1\" to this block boundary\nbecause it is already on the panel at a different place.").arg(xingSensorC->getText()));
    return;
  }
  else
  {
    removeSensorFromPanel(levelXing->getSensorC());
//    removeAssignment(cSensor);
   levelXing->setSensorCName(xingSensorC->getText());
  }
 }
 else if ( (cSensor!=nullptr) &&
        ( (cSensor==getSensorFromName(levelXing->getSensorBName())) ||
            (cSensor==getSensorFromName(levelXing->getSensorAName())) ||
            (cSensor==getSensorFromName(levelXing->getSensorDName())) ) )
 {
   // need to figure out what to do in this case.
 }
 else if (cSensor==nullptr)
 {
   removeSensorFromPanel(levelXing->getSensorC());
   levelXing->setSensorCName("");
 }

 if ( (dSensor!=nullptr) && xingSensorD->addToPanel() )
 {
  if (isSensorOnPanel(dSensor) &&
          (dSensor!=getSensorFromName(levelXing->getSensorDName())) )
  {
//        JOptionPane.showMessageDialog(sensorsAtXingFrame,
//            java.text.MessageFormat.format(tr("SensorsError6"),
//                new Object[]{xingSensorD->text()}),
//                    tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(sensorsAtXingFrame, tr("Error"), tr("Error - Cannot place sensor \"%1\" on the panel\nbecause it is already on the panel.").arg(xingSensorD->getText()));
   return;
  }
  else
  {
    removeSensorFromPanel(levelXing->getSensorD());
    placeXingDIcon(getSensorIcon(xingSensorD->getText()), xingSensorD->isRightSelected(), 0.0);
//    removeAssignment(dSensor);
    levelXing->setSensorDName(xingSensorD->getText());
    needRedraw = true;
  }
 }
 else if ( (dSensor!=nullptr) &&
        (dSensor!=getSensorFromName(levelXing->getSensorAName())) &&
        (dSensor!=getSensorFromName(levelXing->getSensorBName())) &&
        (dSensor!=getSensorFromName(levelXing->getSensorCName())) &&
        (dSensor!=getSensorFromName(levelXing->getSensorDName())) )
 {
  if (isSensorOnPanel(dSensor))
  {
//        JOptionPane.showMessageDialog(sensorsAtXingFrame,
//            java.text.MessageFormat.format(tr("SensorsError13"),
//                new Object[]{xingSensorD->text()}),
//                    tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(sensorsAtXingFrame, tr("Error"), tr("Error - Cannot assign sensor \"%1\" to this block boundary\nbecause it is already on the panel at a different place.").arg(xingSensorD->getText()));
   return;
  }
  else
  {
   removeSensorFromPanel(levelXing->getSensorD());
//   removeAssignment(dSensor);
   levelXing->setSensorDName(xingSensorD->getText());
  }
 }
 else if ( (dSensor!=nullptr) &&
        ( (dSensor==getSensorFromName(levelXing->getSensorBName())) ||
            (dSensor==getSensorFromName(levelXing->getSensorCName())) ||
            (dSensor==getSensorFromName(levelXing->getSensorAName())) ) )
 {
  // need to figure out what to do in this case.
 }
 else if (dSensor==nullptr)
 {
  removeSensorFromPanel(levelXing->getSensorD());
  levelXing->setSensorDName("");
 }
 // setup logic if requested
 // finish up
 setSensorsAtXingOpen = false;
 sensorsAtXingFrame->setVisible(false);
 xingSensorFromMenu = false;
 if (needRedraw)
 {
  layoutEditor->redrawPanel();
  needRedraw = false;
  layoutEditor->setDirty();
 }
}

/*private*/ bool LayoutEditorTools::getSimpleBlockInformation() {
    //might have to do something to trick it with an end bumper
    if (!boundaryFromMenu) {
        block1 = getBlockFromEntry(block1NameField);
        if (block1==nullptr) return false;
        block2 = getBlockFromEntry(block2NameField);
        if (block2==nullptr){
            PositionablePoint* p = NULL;
           for (PositionablePoint* p : layoutEditor->getPositionablePoints()) {
                if (p->getType() == PositionablePoint::END_BUMPER) {
                    boundary = p;
                } else {
                    return false;
                }
            }
        }
        PositionablePoint* p = NULL;
        boundary = NULL;
        for (PositionablePoint* p : layoutEditor->getPositionablePoints()) {
         if (p->getType() == PositionablePoint::ANCHOR) {
                LayoutBlock* bA = NULL;
                LayoutBlock* bB = NULL;
                if (p->getConnect1()!=nullptr) bA = p->getConnect1()->getLayoutBlock();
                if (p->getConnect2()!=nullptr) bB = p->getConnect2()->getLayoutBlock();
                if ( (bA!=nullptr) && (bB!=nullptr) && (bA!=bB) ) {
                    if ( ( (bA==block1) && (bB==block2) ) ||
                                ( (bA==block2) && (bB==block1) ) ) {
                        boundary = p;
                    }
                }
            }
        }
        if (boundary==nullptr) {
//            JOptionPane.showMessageDialog(setSignalsAtBoundaryFrame,
//                        tr("SignalsError7"),
//                                tr("Error"),JOptionPane.ERROR_MESSAGE);
            QMessageBox::critical(setSignalsAtBoundaryFrame,tr("Error"),tr("Error - The two blocks entered do not join at an anchor point.\nPlease enter the correct blocks and try again."));
            return false;
        }
    }
    return true;
}

// display dialog for Set Signals at Level Crossing tool
/*public*/ void LayoutEditorTools::setSensorsAtSlipFromMenu (LayoutSlip* slip, QVector<QString> blocks, MultiIconEditor* theEditor, JFrame* theFrame )
{
 slipSensorFromMenu = true;
 layoutSlip = slip;
 layoutTurnout = slip;
 blockANameSensorField->setText(layoutSlip->getBlockName());
 for(int i = 0; i<blocks.size(); i++)
 {
  slipSensorBlocks->replace(i,blocks[i]);
 }
 setSensorsAtSlip(theEditor, theFrame);
 return;
}

/*public*/ void LayoutEditorTools::setSensorsAtSlip(MultiIconEditor* theEditor, JFrame* theFrame)
{
 sensorIconEditor = theEditor;
 sensorSlipFrame = theFrame;
 if (setSensorsAtSlipOpen)
 {
  slipSensorsGetSaved(nullptr);
  sensorsAtSlipFrame->setVisible(true);
  return;
 }
 // Initialize if needed
 if (sensorsAtSlipFrame == nullptr)
 {
  sensorsAtSlipFrame = new JmriJFrameX( tr("Set Sensors at a Slip"), false, true );
  sensorsAtSlipFrame->addHelpMenu("package.jmri.jmrit.display.SetSensorsAtLevelSlip", true);
  sensorsAtSlipFrame->setLocation(70,30);
//     Container theContentPane = sensorsAtSlipFrame.getContentPane();
//     theContentPane->setLayout(new BoxLayout(theContentPane, BoxLayout.Y_AXIS));
  QScrollArea* centralWidget = new QScrollArea;
  centralWidget->setWidgetResizable(true);
  QWidget* scrollWidget = new QWidget;
  QVBoxLayout* centralWidgetLayout = new QVBoxLayout(scrollWidget);
  centralWidget->setWidget(scrollWidget);
  sensorsAtSlipFrame->setCentralWidget(centralWidget);
  QWidget* panel11 = new QWidget();
  //panel11->setLayout(new QHBoxLayout());
  QHBoxLayout* panel11Layout = new QHBoxLayout(panel11);
  slipSensorA = new BeanDetails("Sensor", InstanceManager::sensorManagerInstance());
  slipSensorB = new BeanDetails("Sensor", InstanceManager::sensorManagerInstance());
  slipSensorC = new BeanDetails("Sensor", InstanceManager::sensorManagerInstance());
  slipSensorD = new BeanDetails("Sensor", InstanceManager::sensorManagerInstance());
  if (slipSensorFromMenu)
  {
   QLabel* blockANameLabel = new QLabel( tr("BlockAtA")+" "+
               tr("Name")+" : "+layoutSlip->getBlockName());

   panel11Layout->addWidget(blockANameLabel);
   slipSensorA->setTextField(layoutSlip->getSensorAName());
   slipSensorB->setTextField(layoutSlip->getSensorBName());
   slipSensorC->setTextField(layoutSlip->getSensorCName());
   slipSensorD->setTextField(layoutSlip->getSensorDName());
  }
  else
  {
   QLabel* blockANameLabel = new QLabel( tr("BlockAtA")+" "+
                                                   tr("Name")+" : ");
   panel11Layout->addWidget(blockANameLabel);
   panel11->layout()->addWidget(blockANameSensorField);
   blockANameSensorField->setToolTip(tr("SensorsBlockNameHint"));
  }
  centralWidgetLayout->addWidget(panel11);
  centralWidgetLayout->addWidget(new JSeparator(JSeparator::HORIZONTAL));
  QWidget* panel2 = new QWidget();
  panel2->setLayout(new QHBoxLayout());
  QLabel* shTitle = new QLabel(tr("Sensor"));
  panel2->layout()->addWidget(shTitle);
  panel2->layout()->addWidget(new QLabel("   "));
  panel2->layout()->addWidget(getSavedSlipSensors = new QPushButton(tr("Get Saved")));
//     getSavedSlipSensors->layout()->addWidgetActionListener(new ActionListener() {
//             /*public*/ void actionPerformed(ActionEvent e) {
//                 slipSensorsGetSaved(e);
//             }
//         });
  connect(getSavedSlipSensors, SIGNAL(clicked()), this, SLOT(slipSensorsGetSaved()));
  getSavedSlipSensors->setToolTip("Click to retrieve signal heads previously stored.");
  centralWidgetLayout->addWidget(panel2);

  //slipSensorA->getDetailsPanel()->setBackground(new Color(255,255,200));
  slipSensorA->getDetailsPanel()->setStyleSheet("QWidget { background: rgb(255,255,200)}");

  //slipSensorB->getDetailsPanel()->setBackground(new Color(200,255,255));
  slipSensorB->getDetailsPanel()->setStyleSheet("QWidget { background: rgb(200,255,255)}");

  //slipSensorC->getDetailsPanel()->setBackground(new Color(200,200,255));
  slipSensorC->getDetailsPanel()->setStyleSheet("QWidget { background: rgb(200,200,255)}");


  //slipSensorD->getDetailsPanel()->setBackground(new Color(255,200,200));
  slipSensorD->getDetailsPanel()->setStyleSheet("QWidget { background: rgb(255,200,200)}");

  sensorSlipPanelLayout = new QGridLayout(sensorSlipPanel);
  //sensorSlipPanel->setLayout(new GridLayout(2,2));

  slipSensorA->setBoundaryLabel(slipSensorBlocks->at(0));
  slipSensorB->setBoundaryLabel(slipSensorBlocks->at(1));
  slipSensorC->setBoundaryLabel(slipSensorBlocks->at(2));
  slipSensorD->setBoundaryLabel(slipSensorBlocks->at(3));

  if(slipSensorBlocks->at(0)!=nullptr)
      sensorSlipPanelLayout->addWidget(slipSensorA->getDetailsPanel(),0,0);
  else
      sensorSlipPanelLayout->addWidget(new QWidget(),0,0);
  if(slipSensorBlocks->at(3)!=nullptr)
      sensorSlipPanelLayout->addWidget(slipSensorD->getDetailsPanel(),0,1);
  else
      sensorSlipPanelLayout->addWidget(new QWidget(),0,1);
  if(slipSensorBlocks->at(1)!=nullptr)
      sensorSlipPanelLayout->addWidget(slipSensorB->getDetailsPanel(),1,0);
  else
      sensorSlipPanelLayout->addWidget(new QWidget(),1,0);
  if(slipSensorBlocks->at(2)!=nullptr)
      sensorSlipPanelLayout->addWidget(slipSensorC->getDetailsPanel(),1,1);
  else
      sensorSlipPanelLayout->addWidget(new QWidget(),1,1);

  centralWidgetLayout->addWidget(sensorSlipPanel);

  centralWidgetLayout->addWidget(new JSeparator(JSeparator::HORIZONTAL));
  QWidget* panel6 = new QWidget();
  panel6->setLayout(new QHBoxLayout());
  panel6->layout()->addWidget(changeSensorSlipIcon = new QPushButton(tr("Change Sensor Icon")));
//     changeSensorSlipIcon->layout()->addWidgetActionListener(new ActionListener() {
//             /*public*/ void actionPerformed(ActionEvent e) {
//                 sensorSlipFrame->setVisible(true);
//             }
//         });
  connect(changeSensorSlipIcon, SIGNAL(clicked()), this, SLOT(on_changeSensorSlipIcon()));
  changeSensorSlipIcon->setToolTip( tr("ChangeSensorIconHint") );

  panel6->layout()->addWidget(new QLabel("  "));
  panel6->layout()->addWidget(setSlipSensorsDone = new QPushButton(tr("Done")));
//     setSlipSensorsDone->layout()->addWidgetActionListener(new ActionListener() {
//         /*public*/ void actionPerformed(ActionEvent e) {
//             setSlipSensorsDonePressed(e);
//         }
//     });
  connect(setSlipSensorsDone, SIGNAL(clicked()),this, SLOT(setSlipSensorsDonePressed()));
  setSlipSensorsDone->setToolTip( tr("Click Done to perform tasks requested above and dismiss this dialog.") );
  panel6->layout()->addWidget(setSlipSensorsCancel = new QPushButton(tr("Cancel")));
//     setSlipSensorsCancel->layout()->addWidgetActionListener(new ActionListener() {
//         /*public*/ void actionPerformed(ActionEvent e) {
//             setSlipSensorsCancelPressed(e);
//         }
//     });
  connect(setSlipSensorsCancel, SIGNAL(clicked()),this, SLOT(setSlipSensorsCancelPressed()));
  setSlipSensorsCancel->setToolTip( tr("Click Cancel to dismiss this dialog without making changes.") );
  centralWidgetLayout->addWidget(panel6);
//     sensorsAtSlipFrame->layout()->addWidgetWindowListener(new java.awt.event.WindowAdapter() {
//         /*public*/ void windowClosing(java.awt.event.WindowEvent e) {
//             setSlipSensorsCancelPressed(nullptr);
//         }
//     });
  sensorsAtSlipFrame->addWindowListener(new SASWindowListener(this));
 }
 if (slipSensorFromMenu) slipSensorsGetSaved(nullptr);
 //sensorsAtSlipFrame->setPreferredSize(nullptr);
 sensorsAtSlipFrame->adjustSize();
 sensorsAtSlipFrame->setVisible(true);
 setSensorsAtSlipOpen = true;
}

void LayoutEditorTools::on_changeSensorSlipIcon()
{
 sensorSlipFrame->setVisible(true);
}

/*private*/ void LayoutEditorTools::slipSensorsGetSaved (ActionEvent* /*a*/)
{
 if ( !getSlipSensorInformation() ) return;

 slipSensorBlocks = layoutSlip->getBlockBoundaries();
 slipSensorA->setTextField(layoutSlip->getSensorAName());
 slipSensorB->setTextField(layoutSlip->getSensorBName());
 slipSensorC->setTextField(layoutSlip->getSensorCName());
 slipSensorD->setTextField(layoutSlip->getSensorDName());

 sensorSlipPanel->layout()->removeWidget(slipSensorA->getDetailsPanel());
 sensorSlipPanel->layout()->removeWidget(slipSensorB->getDetailsPanel());
 sensorSlipPanel->layout()->removeWidget(slipSensorC->getDetailsPanel());
 sensorSlipPanel->layout()->removeWidget(slipSensorD->getDetailsPanel());

 slipSensorA->setBoundaryLabel(slipSensorBlocks->at(0));
 slipSensorB->setBoundaryLabel(slipSensorBlocks->at(1));
 slipSensorC->setBoundaryLabel(slipSensorBlocks->at(2));
 slipSensorD->setBoundaryLabel(slipSensorBlocks->at(3));

 bool boundary = false;
 if(slipSensorBlocks->at(0)!=nullptr){
     sensorSlipPanelLayout->addWidget(slipSensorA->getDetailsPanel());
     boundary = true;
 }
 if(slipSensorBlocks->at(1)!=nullptr){
     sensorSlipPanelLayout->addWidget(slipSensorB->getDetailsPanel());
     boundary = true;
 }
 if(slipSensorBlocks->at(2)!=nullptr){
     sensorSlipPanelLayout->addWidget(slipSensorC->getDetailsPanel());
     boundary = true;
 }
 if(slipSensorBlocks->at(3)!=nullptr){
     sensorSlipPanelLayout->addWidget(slipSensorD->getDetailsPanel());
     boundary = true;
 }
 if (!boundary)
 {
//     JOptionPane.showMessageDialog(sensorsAtSlipFrame, tr("NoBoundarySlipSensor"));
  QMessageBox::warning(sensorsAtSlipFrame, tr("Warning"), tr("There are no block boundaries on this slip turnout\n                                                             It is therefore not possible to add Sensors to it"));
 }
 //sensorsAtSlipFrame->setPreferredSize(nullptr);
 sensorsAtSlipFrame->pack();
}

/*private*/ bool LayoutEditorTools::getSlipSensorInformation()
{
 if (!slipSensorFromMenu)
 {
  layoutSlip = NULL;
  QList<LayoutSlip*> layoutSlips = layoutEditor->getLayoutSlips();
  if (layoutSlips.size()<=0)
  {
//      JOptionPane.showMessageDialog(sensorsAtSlipFrame,
//          tr("SignalsError15"),
//                      tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(sensorsAtSlipFrame, tr("Error"), tr("Error - There are no level crossings on your panel.\n                                                             Please add a level crossing or cancel."));
   return false;
  }
  else if (layoutSlips.size()==1) {
      layoutSlip = layoutSlips.at(0);
  }
  else {
      LayoutBlock* slipSensorBlockA = NULL;
      slipSensorBlockA = getBlockFromEntry(blockANameSensorField);
      if (slipSensorBlockA==nullptr) return false;
      LayoutSlip* x = NULL;
      int foundCount = 0;

      for (int i = 0;(i<layoutSlips.size());i++) {
          x = layoutSlips.at(i);
          LayoutBlock* xA = NULL;
          LayoutBlock* xB = NULL;
          LayoutBlock* xC = NULL;
          LayoutBlock* xD = NULL;
          LayoutBlock* xAC = x->getLayoutBlock();
          if (x->getConnectA()!=nullptr) xA = ((TrackSegment*)x->getConnectA())->getLayoutBlock();
          if (x->getConnectB()!=nullptr) xB = ((TrackSegment*)x->getConnectB())->getLayoutBlock();
          if (x->getConnectC()!=nullptr) xC = ((TrackSegment*)x->getConnectC())->getLayoutBlock();
          if (x->getConnectD()!=nullptr) xD = ((TrackSegment*)x->getConnectD())->getLayoutBlock();
          if ( ( (xA!=nullptr) && (xC!=nullptr) && ( (xA==slipSensorBlockA) ||
                  (xC==slipSensorBlockA) ) ) ||
                  ( (xB!=nullptr) && (xD!=nullptr) && ( ((xB==slipSensorBlockA)) ||
                  ((xD==slipSensorBlockA)) ) ) ) {
              layoutSlip = x;
              foundCount ++;
          }
          else if ( (xAC!=nullptr) && (xAC==slipSensorBlockA) ) {
              layoutSlip = x;
              foundCount ++;
          }
      }
      if (foundCount==0) {
          // try one block test
          for (int i = 0;(i<layoutSlips.size());i++) {
              x = layoutSlips.at(i);
              if (slipSensorBlockA == x->getLayoutBlock()) {
                  layoutSlip = x;
                  foundCount ++;
              }
          }
      }
      if (foundCount>1)
      {
//          JOptionPane.showMessageDialog(sensorsAtSlipFrame,
//                  java.text.MessageFormat.format(tr("SignalsError16"),
//                      new Object[]{" "+foundCount+" "}),
//                          tr("Error"),JOptionPane.ERROR_MESSAGE);
       QMessageBox::critical(sensorsAtSlipFrame, tr("Error"), tr("Error - The entered block names match \"%1\" level crossings.").arg(foundCount));

          return false;
      }
      if (layoutSlip==nullptr) {
//          JOptionPane.showMessageDialog(sensorsAtSlipFrame,
//                      tr("SignalsError17"),
//                              tr("Error"),JOptionPane.ERROR_MESSAGE);
       QMessageBox::critical(sensorsAtSlipFrame, tr("Error"), tr("Error - No level crossing matches the block(s)\n                                                                 entered. Please try again or cancel."));
          return false;
      }
  }
 }
 return true;
}

/*private*/ void LayoutEditorTools::setSlipSensorsCancelPressed (ActionEvent* /*a*/)
{
    setSensorsAtSlipOpen = false;
    sensorsAtSlipFrame->setVisible(false);
    slipSensorFromMenu = false;
}

/*private*/ void LayoutEditorTools::setSlipSensorsDonePressed (ActionEvent* /*a*/)
{
 if ( !getSlipSensorInformation() ) return;
 Sensor* aSensor = getSensorFromEntry(slipSensorA->getText(),false,sensorsAtSlipFrame);
 Sensor* bSensor = getSensorFromEntry(slipSensorB->getText(),false,sensorsAtSlipFrame);
 Sensor* cSensor = getSensorFromEntry(slipSensorC->getText(),false,sensorsAtSlipFrame);
 Sensor* dSensor = getSensorFromEntry(slipSensorD->getText(),false,sensorsAtSlipFrame);
 // place or update signals as requested
 if ( (aSensor!=nullptr) && slipSensorA->addToPanel() )
 {
  if (isSensorOnPanel(aSensor) &&
             (aSensor!=getSensorFromName(layoutSlip->getSensorAName())) )
  {
//         JOptionPane.showMessageDialog(sensorsAtSlipFrame,
//             java.text.MessageFormat.format(tr("SensorsError6"),
//                 new Object[]{slipSensorA->text()}),
//                     tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(sensorsAtSlipFrame, tr("Error"), tr("Error - Cannot place sensor \"%1\" on the panel\n                                                             because it is already on the panel.").arg(slipSensorA->getText()));
   return;
  }
  else
  {
   removeSensorFromPanel(layoutSlip->getSensorA());
   placingBlock(getSensorIcon(slipSensorA->getText()), slipSensorA->isRightSelected(), 0.0);
//   removeAssignment(aSensor);
   layoutSlip->setSensorA(slipSensorA->getText());
   needRedraw = true;
  }
 }
 else if ( (aSensor!=nullptr) &&
         (aSensor!=getSensorFromName(layoutSlip->getSensorAName())) &&
         (aSensor!=getSensorFromName(layoutSlip->getSensorBName())) &&
         (aSensor!=getSensorFromName(layoutSlip->getSensorCName())) &&
         (aSensor!=getSensorFromName(layoutSlip->getSensorDName())) )
 {
  if (isSensorOnPanel(aSensor))
  {
//      JOptionPane.showMessageDialog(sensorsAtSlipFrame,
//          java.text.MessageFormat.format(tr("SensorsError13"),
//              new Object[]{slipSensorA->text()}),
//                  tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(sensorsAtSlipFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this block boundary\n                                                             because it is already on the panel at a different place.").arg(slipSensorA->getText()));
      return;
  }
  else
  {
   removeSensorFromPanel(layoutSlip->getSensorA());
//   removeAssignment(aSensor);
   layoutSlip->setSensorA(slipSensorA->getText());
  }
 }
 else if ( (aSensor!=nullptr) &&
         ( (aSensor==getSensorFromName(layoutSlip->getSensorBName())) ||
             (aSensor==getSensorFromName(layoutSlip->getSensorCName())) ||
             (aSensor==getSensorFromName(layoutSlip->getSensorDName())) ) )
 {
  // need to figure out what to do in this case.
 }
 else if (aSensor==nullptr)
 {
  removeSensorFromPanel(layoutSlip->getSensorA());
  layoutSlip->setSensorA("");
 }
 if ( (bSensor!=nullptr) && slipSensorB->addToPanel() )
 {
  if (isSensorOnPanel(bSensor) &&
             (bSensor!=getSensorFromName(layoutSlip->getSensorBName())))
  {
//         JOptionPane.showMessageDialog(sensorsAtSlipFrame,
//             java.text.MessageFormat.format(tr("SensorsError6"),
//                 new Object[]{slipSensorB->text()}),
//                     tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(sensorsAtSlipFrame, tr("Error"), tr("Error - Cannot place sensor \"%1\" on the panel\n                                                             because it is already on the panel.").arg(slipSensorB->getText()));
   return;
  }
  else
  {
   removeSensorFromPanel(layoutSlip->getSensorB());
   placingBlockB(getSensorIcon(slipSensorB->getText()), slipSensorB->isRightSelected(), 0.0);
//   removeAssignment(bSensor);
   layoutSlip->setSensorB(slipSensorB->getText());
   needRedraw = true;
  }
 }
 else if ( (bSensor!=nullptr) &&
         (bSensor!=getSensorFromName(layoutSlip->getSensorAName())) &&
         (bSensor!=getSensorFromName(layoutSlip->getSensorBName())) &&
         (bSensor!=getSensorFromName(layoutSlip->getSensorCName())) &&
         (bSensor!=getSensorFromName(layoutSlip->getSensorDName())) )
 {
  if (isSensorOnPanel(bSensor))
  {
//         JOptionPane.showMessageDialog(sensorsAtSlipFrame,
//             java.text.MessageFormat.format(tr("SensorsError13"),
//                 new Object[]{slipSensorB->text()}),
//                     tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(sensorsAtSlipFrame, tr("Error"), tr("Error - Cannot assign sensor \"%1\" to this block boundary\n                                                                because it is already on the panel at a different place.").arg(slipSensorB->getText()));
   return;
  }
  else
  {
   removeSensorFromPanel(layoutSlip->getSensorB());
//   removeAssignment(bSensor);
   layoutSlip->setSensorB(slipSensorB->getText());
  }
 }
 else if ( (bSensor!=nullptr) &&
         ( (bSensor==getSensorFromName(layoutSlip->getSensorAName())) ||
             (bSensor==getSensorFromName(layoutSlip->getSensorCName())) ||
             (bSensor==getSensorFromName(layoutSlip->getSensorDName())) ) ) {
// need to figure out what to do in this case.
 }
 else if (bSensor==nullptr)
 {
  removeSensorFromPanel(layoutSlip->getSensorB());
  layoutSlip->setSensorB("");
 }
 if ( (cSensor!=nullptr) && slipSensorC->addToPanel() )
 {
  if (isSensorOnPanel(cSensor) &&
             (cSensor!=getSensorFromName(layoutSlip->getSensorCName())) )
  {
//         JOptionPane.showMessageDialog(sensorsAtSlipFrame,
//             java.text.MessageFormat.format(tr("SensorsError6"),
//                 new Object[]{slipSensorC->text()}),
//                     tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(sensorsAtSlipFrame, tr("Error"), tr("Error - Cannot place sensor \"%1\" on the panel\n                                                             because it is already on the panel.").arg(slipSensorC->getText()));
   return;
  }
  else
  {
   removeSensorFromPanel(layoutSlip->getSensorC());
   placingBlockC(getSensorIcon(slipSensorC->getText()), slipSensorC->isRightSelected(), 0.0);
//   removeAssignment(cSensor);
   layoutSlip->setSensorC(slipSensorC->getText());
   needRedraw = true;
  }
 }
 else if ( (cSensor!=nullptr) &&
         (cSensor!=getSensorFromName(layoutSlip->getSensorAName())) &&
         (cSensor!=getSensorFromName(layoutSlip->getSensorBName())) &&
         (cSensor!=getSensorFromName(layoutSlip->getSensorCName())) &&
         (cSensor!=getSensorFromName(layoutSlip->getSensorDName())) )
 {
     if (isSensorOnPanel(cSensor)) {
//         JOptionPane.showMessageDialog(sensorsAtSlipFrame,
//             java.text.MessageFormat.format(tr("SensorsError13"),
//                 new Object[]{slipSensorC->text()}),
//                     tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(sensorsAtSlipFrame, tr("Error"), tr("Error - Cannot assign sensor \"%1\" to this block boundary\n                                                                because it is already on the panel at a different place.").arg(slipSensorC->getText()));return;
     }
     else {
         removeSensorFromPanel(layoutSlip->getSensorC());
//         removeAssignment(cSensor);
         layoutSlip->setSensorC(slipSensorC->getText());
     }
 }
 else if ( (cSensor!=nullptr) &&
         ( (cSensor==getSensorFromName(layoutSlip->getSensorBName())) ||
             (cSensor==getSensorFromName(layoutSlip->getSensorAName())) ||
             (cSensor==getSensorFromName(layoutSlip->getSensorDName())) ) ) {
// need to figure out what to do in this case.
 }
 else if (cSensor==nullptr) {
     removeSensorFromPanel(layoutSlip->getSensorC());
     layoutSlip->setSensorC("");
 }
 if ( (dSensor!=nullptr) && slipSensorD->addToPanel() )
 {
     if (isSensorOnPanel(dSensor) &&
             (dSensor!=getSensorFromName(layoutSlip->getSensorDName())) ) {
//         JOptionPane.showMessageDialog(sensorsAtSlipFrame,
//             java.text.MessageFormat.format(tr("SensorsError6"),
//                 new Object[]{slipSensorD->text()}),
//                     tr("Error"),JOptionPane.ERROR_MESSAGE);
      QMessageBox::critical(sensorsAtSlipFrame, tr("Error"), tr("Error - Cannot place sensor \"%1\" on the panel\n                                                             because it is already on the panel.").arg(slipSensorD->getText()));
         return;
     }
     else {
         removeSensorFromPanel(layoutSlip->getSensorD());
         placingBlockD(getSensorIcon(slipSensorD->getText()), slipSensorD->isRightSelected(), 0.0);
//         removeAssignment(dSensor);
         layoutSlip->setSensorD(slipSensorD->getText());
         needRedraw = true;
     }
 }
 else if ( (dSensor!=nullptr) &&
         (dSensor!=getSensorFromName(layoutSlip->getSensorAName())) &&
         (dSensor!=getSensorFromName(layoutSlip->getSensorBName())) &&
         (dSensor!=getSensorFromName(layoutSlip->getSensorCName())) &&
         (dSensor!=getSensorFromName(layoutSlip->getSensorDName())) )
 {
  if (isSensorOnPanel(dSensor))
  {
//         JOptionPane.showMessageDialog(sensorsAtSlipFrame,
//             java.text.MessageFormat.format(tr("SensorsError13"),
//                 new Object[]{slipSensorD->text()}),
//                     tr("Error"),JOptionPane.ERROR_MESSAGE);
   QMessageBox::critical(sensorsAtSlipFrame, tr("Error"), tr("Error - Cannot assign sensor \"%1\" to this block boundary\n                                                                because it is already on the panel at a different place.").arg(slipSensorD->getText()));
   return;
  }
  else
  {
   removeSensorFromPanel(layoutSlip->getSensorD());
//   removeAssignment(dSensor);
   layoutSlip->setSensorD(slipSensorD->getText());
  }
 }
 else if ( (dSensor!=nullptr) &&
         ( (dSensor==getSensorFromName(layoutSlip->getSensorBName())) ||
             (dSensor==getSensorFromName(layoutSlip->getSensorCName())) ||
             (dSensor==getSensorFromName(layoutSlip->getSensorAName())) ) )
 {
  // need to figure out what to do in this case.
 }
 else if (dSensor==nullptr)
 {
  removeSensorFromPanel(layoutSlip->getSensorD());
  layoutSlip->setSensorD("");
 }
 // setup logic if requested
 // finish up
 setSensorsAtSlipOpen = false;
 sensorsAtSlipFrame->setVisible(false);
 slipSensorFromMenu = false;
 if (needRedraw)
 {
  layoutEditor->redrawPanel();
  needRedraw = false;
  layoutEditor->setDirty();
 }
}

BeanDetails::BeanDetails(QString beanType, Manager* manager) : QObject()
{
 boundaryLabelText = tr("Boundary Of");
 boundary = new QLabel(boundaryLabelText);

 detailsPanel = new QGroupBox();
 QVBoxLayout* detailsPanelLayout = new QVBoxLayout(detailsPanel);
 addBeanCheck = new QRadioButton(tr("Do Not Place On Panel"));
 left = new QRadioButton(tr("Left Hand Side"));
 right = new QRadioButton(tr("Right Hand Side"));
 buttonGroup = new QButtonGroup();

 boundaryBlocks = new QLabel();
 beanCombo = new JmriBeanComboBox(manager);
 beanCombo->setFirstItemBlank(true);
 beanString = (beanType);
 textLabel = new QLabel(beanString);
 this->manager = manager;
 //this.beanType = beanType;

 buttonGroup->addButton(addBeanCheck);
 buttonGroup->addButton(left);
 buttonGroup->addButton(right);
 addBeanCheck->setChecked(true);

//        boundaryBlocks->setAlignmentX(Component.CENTER_ALIGNMENT);
//        boundaryBlocks->setOpaque(false);
//        detailsPanel->setLayout(new BorderLayout());
//        detailsPanel->setBorder(BorderFactory.createTitledBorder(blackline, tr("BlockBoundary")));
//        boundary->setAlignmentX(Component.CENTER_ALIGNMENT);

 QWidget*boundaryDetails = new QWidget();
 QVBoxLayout* boundaryDetailsLayout = new QVBoxLayout(boundaryDetails);
//        boundaryDetails->setOpaque(false);
//        boundaryDetails->setLayout(new QVBoxLayout(boundaryDetails/*, BoxLayout.Y_AXIS*/));
 boundaryDetailsLayout->addWidget(boundary, 0, Qt::AlignCenter);
 boundaryDetailsLayout->addWidget(boundaryBlocks, 0, Qt::AlignCenter);

 detailsPanelLayout->addWidget(boundaryDetails, /*BorderLayout::North*/0,Qt::AlignTop);
 detailsPanelLayout->addWidget(addIconPanel(), /*BorderLayout::Center*/0, Qt::AlignCenter);
 detailsPanelLayout->addWidget(positionLeftRight(), /*BorderLayout::South*/0, Qt::AlignBottom);
}

void BeanDetails::setTextField(QString value)
{
 beanCombo->setSelectedBean(((AbstractManager*)manager)->getNamedBean(value));
}

QString BeanDetails::getText()
{
 return beanCombo->getSelectedDisplayName();
}

NamedBean* BeanDetails::getBean()
{
 return beanCombo->getSelectedBean();
}

QWidget* BeanDetails::getDetailsPanel()
{
 return detailsPanel;
}

bool BeanDetails::addToPanel()
{
 return !addBeanCheck->isChecked();
}

bool BeanDetails::isRightSelected()
{
 return right->isChecked();
}

void BeanDetails::setBoundaryTitle(QString text)
{
//        detailsPanel->setBorder(BorderFactory.createTitledBorder(blackline, text));
 QString     gbStyleSheet = "QGroupBox { border: 2px solid gray; border-radius: 3px;} QGroupBox::title { /*background-color: transparent;*/  subcontrol-position: top left; /* position at the top left*/  padding:0 0px;} ";
 detailsPanel->setStyleSheet(gbStyleSheet);
 detailsPanel->setTitle(text);
}

void BeanDetails::setBoundaryLabelText(QString text)
{
 boundary->setText(text);
}

void BeanDetails::setBoundaryLabel(QString label)
{
 boundaryBlocks->setText(label);
}

JmriBeanComboBox* BeanDetails::getCombo()
{
 return beanCombo;
}

QWidget* BeanDetails::positionLeftRight()
{
 QWidget* placementPanel = new QWidget();
//        placementPanel->setBorder(BorderFactory.createTitledBorder(blackline, java.text.MessageFormat.format(tr("PlaceItem"),
//                        new Object[]{beanString})));
 placementPanel->setLayout(new QVBoxLayout(placementPanel/*, BoxLayout.Y_AXIS*/));
 //placementPanel->setOpaque(false);
 placementPanel->layout()->addWidget(addBeanCheck);
 placementPanel->layout()->addWidget(left);
 placementPanel->layout()->addWidget(right);
//        addBeanCheck->setOpaque(false);
//        left->setOpaque(false);
//        right->setOpaque(false);

 addBeanCheck->setToolTip(tr("Check to place icon for above %1 on panel near anchor point.").arg(beanString));

 right->setToolTip(tr("Place %1 on the right hand side of the track as if you were facing it").arg(beanString));

 left->setToolTip(tr("Place %1 on the left hand side of the track as if you were facing it").arg(beanString));
 return placementPanel;
}

QWidget* BeanDetails::addIconPanel()
{
 QWidget*addBeanPanel = new QWidget();
//        addBeanPanel->setOpaque(false);
 addBeanPanel->setLayout(new QHBoxLayout());
 addBeanPanel->layout()->addWidget(textLabel);
//        textLabel->setOpaque(false);
 addBeanPanel->layout()->addWidget(beanCombo);
 return addBeanPanel;
}

/*public*/ void LayoutEditorTools::setSlipFromMenu( LayoutSlip* ls,
        MultiIconEditor* theEditor, JFrame* theFrame )
    {
 layoutSlip = ls;
 a1SlipField->setText("");
 a2SlipField->setText("");
 b1SlipField->setText("");
 b2SlipField->setText("");
 c1SlipField->setText("");
 c2SlipField->setText("");
 d1SlipField->setText("");
 d2SlipField->setText("");
 slipSignalFromMenu = true;

 setSignalsAtSlip(theEditor,theFrame);
}
/*public*/ void LayoutEditorTools::setSignalsAtSlip( MultiIconEditor* theEditor, JFrame* theFrame )
{
 signalIconEditor = theEditor;
 signalFrame = theFrame;
 if (setSignalsAtSlipOpen)
 {
  setSignalsAtSlipFrame->setVisible(true);
  return;
 }
 // Initialize if needed
 if (setSignalsAtSlipFrame == nullptr)
 {
  setSignalsAtSlipFrame = new JmriJFrameX( tr("Set Signals at a Slip"), false, true );
  setSignalsAtSlipFrame->addHelpMenu("package.jmri.jmrit.display.SetSignalsAtSlip", true);
  setSignalsAtSlipFrame->setLocation(70,30);
//        Container theContentPane = setSignalsAtSlipFrame.getContentPane();
//        theContentPane->setLayout(new BoxLayout(theContentPane, BoxLayout.Y_AXIS));
  QScrollArea* centralWidget = new QScrollArea;
  centralWidget->setWidgetResizable(true);
  QWidget* scrollWidget = new QWidget;
  QVBoxLayout* centralWidgetLayout = new QVBoxLayout(scrollWidget);
  centralWidget->setWidget(scrollWidget);
  setSignalsAtSlipFrame->setCentralWidget(centralWidget);
  QWidget* panel1 = new QWidget();
  panel1->setLayout(new QHBoxLayout());
  QLabel* turnout1NameLabel = new QLabel( tr("Slip")+" "+
                                                      tr("Name") );
  panel1->layout()->addWidget(turnout1NameLabel);
  panel1->layout()->addWidget(slipNameCombo);
  foreach(LayoutSlip* slip, layoutEditor->getLayoutSlips())
  {
   slipNameCombo->addItem(slip->getDisplayName());
  }

  slipNameCombo->insertItem(0, "");

  if(layoutSlip!=nullptr)
  {
   slipNameCombo->setCurrentIndex(slipNameCombo->findText(layoutSlip->getDisplayName()));
   getSlipTurnoutSignalsGetSaved(nullptr);
  }
  else
  {
   slipNameCombo->setCurrentIndex(0);
  }
//        slipNameCombo->addActionListener(new ActionListener() {
//            /*public*/ void actionPerformed(ActionEvent e) {
//                for(LayoutSlip slip: layoutEditor->slipList){
//                    if(slip->getDisplayName()==(slipNameCombo.getSelectedItem())){
//                        //slip1NameField->setText(slip->getDisplayName());
//                        getSlipTurnoutSignalsGetSaved(e);
//                        dblSlipC2SigPanel->setVisible(false);
//                        dblSlipB2SigPanel->setVisible(false);
//                        if(slip.getSlipType()==LayoutSlip::DOUBLE_SLIP){
//                            dblSlipB2SigPanel->setVisible(true);
//                            dblSlipC2SigPanel->setVisible(true);
//                        }
//                        setSignalsAtSlipFrame.pack();
//                        return;
//                    }
//                }
//            }
//        });
  connect(slipNameCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(on_slipNameComboCurrentIndexChanged(QString)));
  centralWidgetLayout->addWidget(panel1);
  QWidget* panel11 = new QWidget();
  panel11->setLayout(new QHBoxLayout());

  centralWidgetLayout->addWidget(new JSeparator(JSeparator::HORIZONTAL));
  // Signal heads located at turnout 1
  QWidget* panel21x = new QWidget();
  panel21x->setLayout(new QHBoxLayout());
  panel21x->layout()->addWidget(new QLabel(tr("Signal located at ")+" "+tr("Turnout")+" 1 - "+ tr("continuing track")));
  centralWidgetLayout->addWidget(panel21x);
  QWidget* panel21 = new QWidget();
  panel21->setLayout(new QHBoxLayout());
  panel21->layout()->addWidget(new QLabel(tr("Protects Turnout")+" 2 - "+tr("continuing track")+" : "));
  panel21->layout()->addWidget(a1SlipField);
  centralWidgetLayout->addWidget(panel21);
  a1SlipField->setToolTip(tr("Enter name (system or user) of signal head located here."));
  QWidget* panel22 = new QWidget();
  panel22->setLayout(new QHBoxLayout());
  panel22->layout()->addWidget(new QLabel(tr("OrBoth")+" 2 "+tr("Tracks)")+"   "));
  panel22->layout()->addWidget(setA1SlipHead);
  setA1SlipHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
  panel22->layout()->addWidget(new QLabel("  "));
  panel22->layout()->addWidget(setupA1SlipLogic);
  setupA1SlipLogic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
  centralWidgetLayout->addWidget(panel22);
  QWidget* panel23 = new QWidget();
  panel23->setLayout(new QHBoxLayout());
  panel23->layout()->addWidget(new QLabel(tr("Protects Turnout")+" 2 - "+tr("diverging track")+" : "));
  panel23->layout()->addWidget(a2SlipField);
  centralWidgetLayout->addWidget(panel23);
  a2SlipField->setToolTip(tr("Enter name (system or user) of signal head located here."));
  QWidget* panel24 = new QWidget();
  panel24->setLayout(new QHBoxLayout());
  panel24->layout()->addWidget(new QLabel("                "));
  panel24->layout()->addWidget(setA2SlipHead);
  setA2SlipHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
  panel24->layout()->addWidget(new QLabel("  "));
  panel24->layout()->addWidget(setupA2SlipLogic);
  setupA2SlipLogic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
  centralWidgetLayout->addWidget(panel24);
  QWidget* panel31x = new QWidget();
  panel31x->setLayout(new QHBoxLayout());
  panel31x->layout()->addWidget(new QLabel(tr("Signal located at ")+" "+tr("Turnout")+" 1 - "+tr("diverging track")));
  centralWidgetLayout->addWidget(panel31x);
  QWidget* panel31 = new QWidget();
  panel31->setLayout(new QHBoxLayout());
  panel31->layout()->addWidget(new QLabel(tr("Protects Turnout")+" 2 - "+tr("continuing track")+" : "));
  panel31->layout()->addWidget(b1SlipField);
  centralWidgetLayout->addWidget(panel31);
  b1SlipField->setToolTip(tr("Enter name (system or user) of signal head located here."));
  QWidget* panel32 = new QWidget();
  panel32->setLayout(new QHBoxLayout());
  panel32->layout()->addWidget(new QLabel(tr("OrBoth")+" 2 "+tr("Tracks)")+"   "));
  panel32->layout()->addWidget(setB1SlipHead);
  setB1SlipHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
  panel32->layout()->addWidget(new QLabel("  "));
  panel32->layout()->addWidget(setupB1SlipLogic);
  setupB1SlipLogic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
  centralWidgetLayout->addWidget(panel32);

  dblSlipB2SigPanel = new QWidget();
  dblSlipB2SigPanel->setLayout(new QVBoxLayout); //(dblSlipB2SigPanel, BoxLayout.Y_AXIS));
  QWidget* panel33 = new QWidget();
  panel33->setLayout(new QHBoxLayout());
  panel33->layout()->addWidget(new QLabel(tr("Protects Turnout")+" 2 - "+tr("diverging track")+" : "));
  panel33->layout()->addWidget(b2SlipField);
  dblSlipB2SigPanel->layout()->addWidget(panel33);
  b2SlipField->setToolTip(tr("Enter name (system or user) of signal head located here."));
  QWidget* panel34 = new QWidget();
  panel34->setLayout(new QHBoxLayout());
  panel34->layout()->addWidget(new QLabel("                "));
  panel34->layout()->addWidget(setB2SlipHead);
  setB2SlipHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
  panel34->layout()->addWidget(new QLabel("  "));
  panel34->layout()->addWidget(setupB2SlipLogic);
  setupB2SlipLogic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
  dblSlipB2SigPanel->layout()->addWidget(panel34);

  centralWidgetLayout->addWidget(dblSlipB2SigPanel);
  dblSlipB2SigPanel->setVisible(false);
  centralWidgetLayout->addWidget(new JSeparator(JSeparator::HORIZONTAL));
  // Signal heads located at turnout 2
  QWidget* panel41x = new QWidget();
  panel41x->setLayout(new QHBoxLayout());
  panel41x->layout()->addWidget(new QLabel(tr("Signal located at ")+" "+tr("Turnout")+" 2 - "+ tr("continuing track")));
  centralWidgetLayout->addWidget(panel41x);
  QWidget* panel41 = new QWidget();
  panel41->setLayout(new QHBoxLayout());
  panel41->layout()->addWidget(new QLabel(tr("Protects Turnout")+" 1 - "+tr("continuing track")+" : "));
  panel41->layout()->addWidget(c1SlipField);
  centralWidgetLayout->addWidget(panel41);
  c1SlipField->setToolTip(tr("Enter name (system or user) of signal head located here."));
  QWidget* panel42 = new QWidget();
  panel42->setLayout(new QHBoxLayout());
  panel42->layout()->addWidget(new QLabel(tr("OrBoth")+" 1 "+tr("Tracks)")+"   "));
  panel42->layout()->addWidget(setC1SlipHead);
  setC1SlipHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
  panel42->layout()->addWidget(new QLabel("  "));
  panel42->layout()->addWidget(setupC1SlipLogic);
  setupC1SlipLogic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
  centralWidgetLayout->addWidget(panel42);
  dblSlipC2SigPanel = new QWidget();
  dblSlipC2SigPanel->setLayout(new QVBoxLayout); //(dblSlipC2SigPanel, BoxLayout.Y_AXIS));
  QWidget* panel43 = new QWidget();
  panel43->setLayout(new QHBoxLayout());
  panel43->layout()->addWidget(new QLabel(tr("Protects Turnout")+" 1 - "+tr("diverging track")+" : "));
  panel43->layout()->addWidget(c2SlipField);
  dblSlipC2SigPanel->layout()->addWidget(panel43);
  c2SlipField->setToolTip(tr("Enter name (system or user) of signal head located here."));
  QWidget* panel44 = new QWidget();
  panel44->setLayout(new QHBoxLayout());
  panel44->layout()->addWidget(new QLabel("                "));
  panel44->layout()->addWidget(setC2SlipHead);
  setC2SlipHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
  panel44->layout()->addWidget(new QLabel("  "));
  panel44->layout()->addWidget(setupC2SlipLogic);
  setupC2SlipLogic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
  dblSlipC2SigPanel->layout()->addWidget(panel44);
  centralWidgetLayout->addWidget(dblSlipC2SigPanel);
  QWidget* panel51x = new QWidget();
  panel51x->setLayout(new QHBoxLayout());
  panel51x->layout()->addWidget(new QLabel(tr("Signal located at ")+" "+tr("Turnout")+" 2 - "+
                                  tr("diverging track")));
  centralWidgetLayout->addWidget(panel51x);
  QWidget* panel51 = new QWidget();
  panel51->setLayout(new QHBoxLayout());
  panel51->layout()->addWidget(new QLabel(tr("Protects Turnout")+" 1 - "+tr("continuing track")+" : "));
  panel51->layout()->addWidget(d1SlipField);
  centralWidgetLayout->addWidget(panel51);
  d1SlipField->setToolTip(tr("Enter name (system or user) of signal head located here."));
  QWidget* panel52 = new QWidget();
  panel52->setLayout(new QHBoxLayout());
  panel52->layout()->addWidget(new QLabel(tr("OrBoth")+" 1 "+tr("Tracks)")+"   "));
  panel52->layout()->addWidget(setD1SlipHead);
  setD1SlipHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
  panel52->layout()->addWidget(new QLabel("  "));
  panel52->layout()->addWidget(setupD1SlipLogic);
  setupD1SlipLogic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
  centralWidgetLayout->addWidget(panel52);
  QWidget* panel53 = new QWidget();
  panel53->setLayout(new QHBoxLayout());
  panel53->layout()->addWidget(new QLabel(tr("Protects Turnout")+" 1 - "+tr("diverging track")+" : "));
  panel53->layout()->addWidget(d2SlipField);
  centralWidgetLayout->addWidget(panel53);
  d2SlipField->setToolTip(tr("Enter name (system or user) of signal head located here."));
  QWidget* panel54 = new QWidget();
  panel54->setLayout(new QHBoxLayout());
  panel54->layout()->addWidget(new QLabel("                "));
  panel54->layout()->addWidget(setD2SlipHead);
  setD2SlipHead->setToolTip(tr("Check to place icon for above signal head on panel near turnout."));
  panel54->layout()->addWidget(new QLabel("  "));
  panel54->layout()->addWidget(setupD2SlipLogic);
  setupD2SlipLogic->setToolTip(tr("Check to set up Simple Signal Logic for above signal head."));
  centralWidgetLayout->addWidget(panel54);
  centralWidgetLayout->addWidget(new JSeparator(JSeparator::HORIZONTAL));
  QWidget* panel6 = new QWidget();
  panel6->setLayout(new QHBoxLayout());
  panel6->layout()->addWidget(changeTToTSignalIcon = new QPushButton(tr("Change Signal Head Icon")));
//        changeTToTSignalIcon->addActionListener(new ActionListener() {
//                /*public*/ void actionPerformed(ActionEvent e) {
//                    signalFrame->setVisible(true);
//                }
//            });
  connect(changeTToTSignalIcon, SIGNAL(clicked()), this, SLOT(on_changeTToTSignalIcon_triggered()));
  changeTToTSignalIcon->setToolTip( tr("Change Signal Head IconHint") );
  panel6->layout()->addWidget(new QLabel("  "));
  panel6->layout()->addWidget(setSlipSignalsDone = new QPushButton(tr("Done")));
//        setSlipSignalsDone->layout()->addWidgetActionListener(new ActionListener() {
//            /*public*/ void actionPerformed(ActionEvent e) {
//                setSlipSignalsDonePressed(e);
//            }
//        });
  connect(setSlipSignalsDone, SIGNAL(clicked()), this, SLOT(setSlipSignalsDonePressed()));
  setSlipSignalsDone->setToolTip( tr("Click Done to perform tasks requested above and dismiss this dialog.") );
  panel6->layout()->addWidget(setSlipSignalsCancel = new QPushButton(tr("Cancel")));
//        setSlipSignalsCancel->layout()->addWidgetActionListener(new ActionListener() {
//            /*public*/ void actionPerformed(ActionEvent e) {
//                setSlipSignalsCancelPressed(e);
//            }
//        });
  connect(setSlipSignalsCancel, SIGNAL(clicked()), this, SLOT(setSignalsCancelPressed()));
  setSlipSignalsCancel->setToolTip( tr("Click Cancel to dismiss this dialog without making changes.") );
  centralWidgetLayout->addWidget(panel6);
//        setSignalsAtSlipFrame->layout()->addWidgetWindowListener(new java.awt.event.WindowAdapter() {
//            /*public*/ void windowClosing(java.awt.event.WindowEvent e) {
//                setSlipSignalsCancelPressed(nullptr);
//            }
//        });
  setSignalsAtSlipFrame->addWindowListener(new SSWindowListener(this));
 }
 dblSlipC2SigPanel->setVisible(false);
 dblSlipB2SigPanel->setVisible(false);
 if(layoutSlip!=NULL && layoutSlip->getSlipType()==LayoutSlip::DOUBLE_SLIP)
 {
  dblSlipB2SigPanel->setVisible(true);
  dblSlipC2SigPanel->setVisible(true);
 }
 if (slipSignalFromMenu) getSlipTurnoutSignalsGetSaved(nullptr);
 setSignalsAtSlipFrame->adjustSize();
 setSignalsAtSlipFrame->setVisible(true);
 setSignalsAtSlipOpen = true;
}

void LayoutEditorTools::on_slipNameComboCurrentIndexChanged(QString)
{
 foreach(LayoutSlip* slip, layoutEditor->getLayoutSlips())
 {
  if(slip->getDisplayName()==(slipNameCombo->currentText()))
  {
   //slip1NameField->setText(slip->getDisplayName());
   getSlipTurnoutSignalsGetSaved();
   dblSlipC2SigPanel->setVisible(false);
   dblSlipB2SigPanel->setVisible(false);
   if(slip->getSlipType()==LayoutSlip::DOUBLE_SLIP)
   {
    dblSlipB2SigPanel->setVisible(true);
    dblSlipC2SigPanel->setVisible(true);
   }
   setSignalsAtSlipFrame->adjustSize();
   return;
  }
 }
}

/*private*/ void LayoutEditorTools::getSlipTurnoutSignalsGetSaved (ActionEvent* /*a*/)
{
 if ( !getSlipTurnoutInformation() ) return;
 a1SlipField->setText(layoutSlip->getSignalA1Name());
 a2SlipField->setText(layoutSlip->getSignalA2Name());
 b1SlipField->setText(layoutSlip->getSignalB1Name());
 b2SlipField->setText(layoutSlip->getSignalB2Name());
 c1SlipField->setText(layoutSlip->getSignalC1Name());
 c2SlipField->setText(layoutSlip->getSignalC2Name());
 d1SlipField->setText(layoutSlip->getSignalD1Name());
 d2SlipField->setText(layoutSlip->getSignalD2Name());
}
/*private*/ void LayoutEditorTools::setSlipSignalsCancelPressed (ActionEvent* /*a*/)
{
 setSignalsAtSlipOpen = false;
 setSignalsAtSlipFrame->setVisible(false);
}
/*private*/ bool LayoutEditorTools::getSlipTurnoutInformation()
{
 QString str = "";
 turnout1 = NULL;
 turnout2 = NULL;
 layoutSlip = NULL;
 foreach(LayoutSlip* ls, layoutEditor->getLayoutSlips())
 {
  if(ls->getDisplayName()==(slipNameCombo->currentText()))
  {
   turnout1 = ls->getTurnout();
   turnout2 = ls->getTurnoutB();
   layoutSlip=ls;
   layoutTurnout = layoutSlip;
   break;
  }
 }
 if(layoutSlip==nullptr){
     return false;
 }
 if (turnout1==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//            java.text.MessageFormat.format(tr("SignalsError2"),
//                new Object[]{str}), tr("Error"),
//                    JOptionPane.ERROR_MESSAGE);
  QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Error - No turnout is defined for \"%1\". Please enter\na turnout name in the Turnout Table and on the panel.").arg(str));
     return false ;
 }
 if (turnout2==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//            java.text.MessageFormat.format(tr("SignalsError2"),
//                    new Object[]{str}), tr("Error"),
//                        JOptionPane.ERROR_MESSAGE);
  QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Error - No turnout is defined for \"%1\". Please enter\n                                                                  a turnout name in the Turnout Table and on the panel.").arg(str));
     return false ;
 }
 return true;
}
/*private*/ void LayoutEditorTools::setSlipSignalsDonePressed (ActionEvent* /*a*/) {
    if ( !getSlipTurnoutInformation() ) return;
    if ( !getSlipSignalHeadInformation() ) return;
    // place signal icons if requested, and assign signal heads to this turnout
    if (setA1SlipHead->isChecked()) {
        if (isHeadOnPanel(a1SlipHead) &&
            (a1SlipHead!=getHeadFromName(layoutSlip->getSignalB1Name()))) {
//            JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{a1Field->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\n                                                          because it is already on the panel.").arg(a1Field->text()));
            return;
        }
        else {
            removeSignalHeadFromPanel(layoutSlip->getSignalA1Name());
            if (layoutSlip->getContinuingSense()==Turnout::CLOSED)
                placeA1Slip(a1SlipField->text().trimmed());
            else
                placeB1Slip(a1SlipField->text().trimmed());
            removeAssignment(a1SlipHead);
            layoutSlip->setSignalA1Name(a1SlipField->text().trimmed());
            needRedraw = true;
        }
    }
    else {
        int assigned = isHeadAssignedHere(a1SlipHead,layoutSlip);
        if (assigned == NONE) {
            if ( isHeadOnPanel(a1SlipHead) &&
                                isHeadAssignedAnywhere(a1SlipHead) ) {
//                JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{a1SlipField->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
             QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\n                                                                         because it is already on the panel at a different place.").arg(a1SlipField->text()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutSlip->getSignalB1Name());
                removeAssignment(a1SlipHead);
                layoutSlip->setSignalA1Name(a1SlipField->text().trimmed());
            }
        }
        else if (assigned!=B1) {
// need to figure out what to do in this case - assigned to a different position on the same turnout.
        }
    }
    if ( (a2SlipHead!=nullptr) && setA2SlipHead->isChecked() ) {
        if (isHeadOnPanel(a2SlipHead) &&
            (a2SlipHead!=getHeadFromName(layoutSlip->getSignalB2Name()))) {
//            JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{a2Field->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\n                                                          because it is already on the panel.").arg(a2Field->text()));
            return;
        }
        else {
            removeSignalHeadFromPanel(layoutSlip->getSignalB2Name());
            if (layoutSlip->getContinuingSense()==Turnout::CLOSED)
                placeA2Slip(a2SlipField->text().trimmed());
            else
                placeB2Slip(a2SlipField->text().trimmed());
            removeAssignment(a2SlipHead);
            layoutSlip->setSignalA2Name(a2SlipField->text().trimmed());
            needRedraw = true;
        }
    }
    else if (a2SlipHead!=nullptr) {
        int assigned = isHeadAssignedHere(a2SlipHead,layoutSlip);
        if (assigned == NONE) {
            if ( isHeadOnPanel(a2SlipHead) &&
                                isHeadAssignedAnywhere(a2SlipHead) ) {
//                JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{a2SlipField->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
             QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\n                                                                         because it is already on the panel at a different place.").arg(a2SlipField->text()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutSlip->getSignalA2Name());
                removeAssignment(a2SlipHead);
                layoutSlip->setSignalA2Name(a2SlipField->text().trimmed());
            }
        }
        else if (assigned!=B2) {
// need to figure out what to do in this case.
        }
    }
    else if (a2SlipHead==nullptr) {
        removeSignalHeadFromPanel(layoutSlip->getSignalA2Name());
        layoutSlip->setSignalB2Name("");
    }
    if (setB1SlipHead->isChecked())
    {
        log->info(b1SlipField->text().trimmed());
        if (isHeadOnPanel(b1SlipHead) &&
            (b1SlipHead!=getHeadFromName(layoutSlip->getSignalC1Name()))) {
//            JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{b1SlipField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\n                                                          because it is already on the panel.").arg(b1Field->text()));
           return;
        }
        else {
            removeSignalHeadFromPanel(layoutSlip->getSignalB1Name());
            if (layoutSlip->getContinuingSense()==Turnout::CLOSED)
                placeB1Slip(b1SlipField->text().trimmed());
            else
                placeA1Slip(b1SlipField->text().trimmed());
            removeAssignment(b1SlipHead);
            layoutSlip->setSignalB1Name(b1SlipField->text().trimmed());
            needRedraw = true;
        }
    }
    else {
        int assigned = isHeadAssignedHere(b1SlipHead,layoutSlip);
        if (assigned == NONE) {
            if ( isHeadOnPanel(b1SlipHead) &&
                                isHeadAssignedAnywhere(b1SlipHead) ) {
//                JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{b1SlipField->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
             QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\n                                                                         because it is already on the panel at a different place.").arg(b1SlipField->text()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutSlip->getSignalB1Name());
                removeAssignment(b1SlipHead);
                layoutSlip->setSignalB1Name(b1SlipField->text().trimmed());
            }
        }
        else if (assigned!=C1) {
            // need to figure out what to do in this case.
        }
    }
    if(layoutSlip->getTurnoutType()==LayoutSlip::DOUBLE_SLIP){
        if ( (b2SlipHead!=nullptr) && setB2SlipHead->isChecked() ) {
            if (isHeadOnPanel(b2SlipHead) &&
                (b2SlipHead!=getHeadFromName(layoutSlip->getSignalC2Name()))) {
//                JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//                    java.text.MessageFormat.format(tr("SignalsError6"),
//                        new Object[]{b2SlipField->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
             QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\n                                                          because it is already on the panel.").arg(b2Field->text()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutSlip->getSignalB2Name());
                if (layoutSlip->getContinuingSense()==Turnout::CLOSED)
                    placeB2Slip(b2SlipField->text().trimmed());
                else
                    placeA2Slip(b2SlipField->text().trimmed());
                removeAssignment(b2SlipHead);
                layoutSlip->setSignalB2Name(b2SlipField->text().trimmed());
                needRedraw = true;
            }
        }
        else if (b2SlipHead!=nullptr) {
            int assigned = isHeadAssignedHere(b2SlipHead,layoutSlip);
            if (assigned == NONE) {
                if ( isHeadOnPanel(b2SlipHead) &&
                                    isHeadAssignedAnywhere(b2SlipHead) ) {
//                    JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//                        java.text.MessageFormat.format(tr("SignalsError8"),
//                            new Object[]{b2SlipField->text().trimmed()}),
//                                tr("Error"),JOptionPane.ERROR_MESSAGE);
                 QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\n                                                                         because it is already on the panel at a different place.").arg(b2SlipField->text()));
                    return;
                }
                else {
                    removeSignalHeadFromPanel(layoutSlip->getSignalB2Name());
                    removeAssignment(b2SlipHead);
                    layoutSlip->setSignalB2Name(b2SlipField->text().trimmed());
                }
            }
            else if (assigned!=C2) {
// need to figure out what to do in this case.
            }
        }
        else if (b2SlipHead==nullptr) {
            removeSignalHeadFromPanel(layoutSlip->getSignalB2Name());
            layoutSlip->setSignalB2Name("");
        }
    } else {
        if(b2SlipHead!=nullptr){
            BlockBossLogic::getStoppedObject(layoutSlip->getSignalB2Name());
            removeSignalHeadFromPanel(layoutSlip->getSignalB2Name());
            layoutSlip->setSignalB2Name("");
            b2SlipHead=NULL;
        }
    }
    // signal heads on turnout 2
    if (setC1SlipHead->isChecked()) {
        if (isHeadOnPanel(c1SlipHead) &&
            (c1SlipHead!=getHeadFromName(layoutSlip->getSignalB1Name()))) {
//            JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{c1SlipField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\n                                                          because it is already on the panel.").arg(c1Field->text()));
            return;
        }
        else {
            removeSignalHeadFromPanel(layoutSlip->getSignalC1Name());
            if (layoutSlip->getContinuingSense()==Turnout::CLOSED)
                placeC1Slip(c1SlipField->text().trimmed());
            else
                placeD1Slip(c1SlipField->text().trimmed());
            removeAssignment(c1SlipHead);
            layoutSlip->setSignalC1Name(c1SlipField->text().trimmed());
            needRedraw = true;
        }
    }
    else {
        int assigned = isHeadAssignedHere(c1SlipHead,layoutSlip);
        if (assigned == NONE) {
            if ( isHeadOnPanel(c1SlipHead) &&
                                isHeadAssignedAnywhere(c1SlipHead) ) {
//                JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{c1SlipField->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
             QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\n                                                                         because it is already on the panel at a different place.").arg(c1SlipField->text()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutSlip->getSignalC1Name());
                removeAssignment(c1SlipHead);
                layoutSlip->setSignalC1Name(c1SlipField->text().trimmed());
            }
        }
        else if (assigned!=B1) {
// need to figure out what to do in this case.
        }
    }
    if(layoutSlip->getTurnoutType()==LayoutSlip::DOUBLE_SLIP){
        if ( (c2SlipHead!=nullptr) && setC2SlipHead->isChecked() ) {
            if (isHeadOnPanel(c2SlipHead) &&
                (c2SlipHead!=getHeadFromName(layoutSlip->getSignalB2Name()))) {
//                JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//                    java.text.MessageFormat.format(tr("SignalsError6"),
//                        new Object[]{c2SlipField->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
             QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\n                                                          because it is already on the panel.").arg(c2Field->text()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutSlip->getSignalC2Name());
                if (layoutSlip->getContinuingSense()==Turnout::CLOSED)
                    placeC2Slip(c2SlipField->text().trimmed());
                else
                    placeD2Slip(c2SlipField->text().trimmed());
                removeAssignment(c2SlipHead);
                layoutSlip->setSignalC2Name(c2SlipField->text().trimmed());
                needRedraw = true;
            }
        }
        else if (c2SlipHead!=nullptr) {
            int assigned = isHeadAssignedHere(c2SlipHead,layoutSlip);
            if (assigned == NONE) {
                if ( isHeadOnPanel(c2SlipHead) &&
                                    isHeadAssignedAnywhere(c2SlipHead) ) {
//                    JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//                        java.text.MessageFormat.format(tr("SignalsError8"),
//                            new Object[]{c2SlipField->text().trimmed()}),
//                                tr("Error"),JOptionPane.ERROR_MESSAGE);
                 QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\n                                                                         because it is already on the panel at a different place.").arg(c2SlipField->text()));
                    return;
                }
                else {
                    removeSignalHeadFromPanel(layoutSlip->getSignalC2Name());
                    removeAssignment(c2SlipHead);
                    layoutSlip->setSignalC2Name(c2SlipField->text().trimmed());
                }
            }
            else if (assigned!=B2) {
// need to figure out what to do in this case.
            }
        }
        else if (c2SlipHead==nullptr) {
            removeSignalHeadFromPanel(layoutSlip->getSignalC2Name());
            layoutSlip->setSignalC2Name("");
        }
    } else {
        if(c2SlipHead!=nullptr){
            BlockBossLogic::getStoppedObject(layoutSlip->getSignalC2Name());
            removeSignalHeadFromPanel(layoutSlip->getSignalC2Name());
            layoutSlip->setSignalC2Name("");
            c2SlipHead=NULL;
        }
    }
    if (setD1SlipHead->isChecked()) {
        if (isHeadOnPanel(d1SlipHead) &&
            (d1SlipHead!=getHeadFromName(layoutSlip->getSignalC1Name()))) {
//                JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{d1SlipField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\n                                                          because it is already on the panel.").arg(d1Field->text()));
            return;
        }
        else {
            removeSignalHeadFromPanel(layoutSlip->getSignalD1Name());
            if (layoutSlip->getContinuingSense()==Turnout::CLOSED)
                placeD1Slip(d1SlipField->text().trimmed());
            else
                placeC1Slip(d1SlipField->text().trimmed());
            removeAssignment(d1SlipHead);
            layoutSlip->setSignalD1Name(d1SlipField->text().trimmed());
            needRedraw = true;
        }
    }
    else {
        int assigned = isHeadAssignedHere(d1SlipHead,layoutSlip);
        if (assigned == NONE) {
            if ( isHeadOnPanel(d1SlipHead) &&
                                isHeadAssignedAnywhere(d1SlipHead) ) {
//                JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{d1SlipField->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
             QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\n                                                                         because it is already on the panel at a different place.").arg(d1SlipField->text()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutSlip->getSignalD1Name());
                removeAssignment(d1SlipHead);
                layoutSlip->setSignalD1Name(d1SlipField->text().trimmed());
            }
        }
        else if (assigned!=C1) {
// need to figure out what to do in this case.
        }
    }
    if ( (d2SlipHead!=nullptr) && setD2SlipHead->isChecked() ) {
        if (isHeadOnPanel(d2SlipHead) &&
            (d2SlipHead!=getHeadFromName(layoutSlip->getSignalC2Name()))) {
//            JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//                java.text.MessageFormat.format(tr("SignalsError6"),
//                    new Object[]{d2SlipField->text().trimmed()}),
//                        tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Error - Cannot place signal head \"%1\" on the panel\n                                                          because it is already on the panel.").arg(d2Field->text()));
            return;
        }
        else {
            removeSignalHeadFromPanel(layoutSlip->getSignalD2Name());
            if (layoutSlip->getContinuingSense()==Turnout::CLOSED)
                placeD2Slip(d2SlipField->text().trimmed());
            else
                placeC2Slip(d2SlipField->text().trimmed());
            removeAssignment(d2SlipHead);
            layoutSlip->setSignalD2Name(d2SlipField->text().trimmed());
            needRedraw = true;
        }
    }
    else if (d2SlipHead!=nullptr) {
        int assigned = isHeadAssignedHere(d2SlipHead,layoutSlip);
        if (assigned == NONE) {
            if ( isHeadOnPanel(d2SlipHead) &&
                                isHeadAssignedAnywhere(d2SlipHead) ) {
//                JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//                    java.text.MessageFormat.format(tr("SignalsError8"),
//                        new Object[]{d2SlipField->text().trimmed()}),
//                            tr("Error"),JOptionPane.ERROR_MESSAGE);
             QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Error - Cannot assign signal head \"%1\" to this turnout\n                                                                         because it is already on the panel at a different place.").arg(d2SlipField->text()));
                return;
            }
            else {
                removeSignalHeadFromPanel(layoutSlip->getSignalD2Name());
                removeAssignment(d2SlipHead);
                layoutSlip->setSignalD2Name(d2SlipField->text().trimmed());
            }
        }
        else if (assigned!=C2) {
// need to figure out what to do in this case.
        }
    }
    else if (d2SlipHead==nullptr) {
        removeSignalHeadFromPanel(layoutSlip->getSignalD2Name());
        layoutSlip->setSignalD2Name("");
    }
    // setup logic if requested
    if (setupA1SlipLogic->isChecked() || setupA2SlipLogic->isChecked()) {
        setLogicSlip(a1SlipHead,(TrackSegment*)layoutSlip->getConnectC(),a2SlipHead,
                (TrackSegment*)layoutSlip->getConnectD(),setupA1SlipLogic->isChecked(),
                setupA2SlipLogic->isChecked(),layoutSlip, layoutSlip->getTurnout(),
                   layoutSlip->getTurnoutB(), LayoutSlip::STATE_AC, LayoutSlip::STATE_AD, 0);
    }
    if (setupB1SlipLogic->isChecked() || setupB2SlipLogic->isChecked()) {
        setLogicSlip(b1SlipHead,(TrackSegment*)layoutSlip->getConnectD(),b2SlipHead,
                (TrackSegment*)layoutSlip->getConnectC(),setupB1SlipLogic->isChecked(),
                setupB2SlipLogic->isChecked(),layoutSlip, layoutSlip->getTurnout(),
                  layoutSlip->getTurnoutB(), LayoutSlip::STATE_BD, LayoutSlip::STATE_BC, 2);
    }
    if (setupC1SlipLogic->isChecked() || setupC2SlipLogic->isChecked()) {
        setLogicSlip(c1SlipHead,(TrackSegment*)layoutSlip->getConnectA(),c2SlipHead,
                (TrackSegment*)layoutSlip->getConnectB(),setupC1SlipLogic->isChecked(),
                setupC2SlipLogic->isChecked(),layoutSlip, layoutSlip->getTurnoutB(),
                 layoutSlip->getTurnout(),LayoutSlip::STATE_AC, LayoutSlip::STATE_BC, 4);
    }
    if (setupD1SlipLogic->isChecked() || setupD2SlipLogic->isChecked()) {
        setLogicSlip(d1SlipHead,(TrackSegment*)layoutSlip->getConnectB(),d2SlipHead,
                (TrackSegment*)layoutSlip->getConnectA(),setupD1SlipLogic->isChecked(),
                setupD2SlipLogic->isChecked(),layoutSlip, layoutSlip->getTurnoutB(),
                 layoutSlip->getTurnout(),LayoutSlip::STATE_BD, LayoutSlip::STATE_AD, 6);
    }
    // finish up
    setSignalsAtSlipOpen = false;
    setSignalsAtSlipFrame->setVisible(false);
    if (needRedraw) {
        layoutEditor->redrawPanel();
        needRedraw = false;
        layoutEditor->setDirty();
    }
}
/*private*/ bool LayoutEditorTools::getSlipSignalHeadInformation() {
    a1SlipHead = getSignalHeadFromEntry(a1SlipField,true,setSignalsAtSlipFrame);
    if (a1SlipHead==nullptr) return false;
    a2SlipHead = getSignalHeadFromEntry(a2SlipField,false,setSignalsAtSlipFrame);
    b1SlipHead = getSignalHeadFromEntry(b1SlipField,true,setSignalsAtSlipFrame);
    if (b1SlipHead==nullptr) return false;
    b2SlipHead = getSignalHeadFromEntry(b2SlipField,false,setSignalsAtSlipFrame);
    c1SlipHead = getSignalHeadFromEntry(c1SlipField,true,setSignalsAtSlipFrame);
    if (c1SlipHead==nullptr) return false;
    c2SlipHead = getSignalHeadFromEntry(c2SlipField,false,setSignalsAtSlipFrame);
    d1SlipHead = getSignalHeadFromEntry(d1SlipField,true,setSignalsAtSlipFrame);
    if (d1SlipHead==nullptr) return false;
    d2SlipHead = getSignalHeadFromEntry(d2SlipField,false,setSignalsAtSlipFrame);
    return true;
}
/*private*/ void LayoutEditorTools::placeA1Slip(QString headName) {
    // place head near the continuing track of turnout 1
    placingBlock(getSignalHeadIcon(headName), false, 0.0);
}
/*private*/ void LayoutEditorTools::placeA2Slip(QString headName) {
    SignalHeadIcon* l = getSignalHeadIcon(headName);
    placingBlock(l, false, (4+l->getHeight()));
}
/*private*/ void LayoutEditorTools::placeB1Slip(QString headName) {
    placingBlockB(getSignalHeadIcon(headName), true, 0.0);
}
/*private*/ void LayoutEditorTools::placeB2Slip(QString headName) {
    SignalHeadIcon* l = getSignalHeadIcon(headName);
    placingBlockB(l, true, (4+l->getHeight()));
}
/*private*/ void LayoutEditorTools::placeC1Slip(QString headName) {
    placingBlockC(getSignalHeadIcon(headName), false, 0.0);
}
/*private*/ void LayoutEditorTools::placeC2Slip(QString headName) {
    SignalHeadIcon* l = getSignalHeadIcon(headName);
    placingBlockC(l, false, (4+l->getHeight()));
}
/*private*/ void LayoutEditorTools::placeD1Slip(QString headName) {
    placingBlockD(getSignalHeadIcon(headName), true, 0.0);
}
/*private*/ void LayoutEditorTools::placeD2Slip(QString headName) {
 SignalHeadIcon* l = getSignalHeadIcon(headName);
 placingBlockD(l, true, (4+l->getHeight()));
}

/*private*/ void LayoutEditorTools::setLogicSlip(SignalHead* head,TrackSegment* track1,SignalHead* secondHead,TrackSegment* track2, bool setup1, bool setup2,  LayoutSlip* slip, Turnout* nearTurnout, Turnout* farTurnout, int continueState, int divergeState, int number)
{
    // initialize common components and ensure all is defined
    LayoutBlock*connectorBlock = slip->getLayoutBlock();
    Sensor* connectorOccupancy = NULL;
    if (connectorBlock==nullptr){
//        JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//                tr("InfoMessage6"),"",JOptionPane.INFORMATION_MESSAGE);
        QMessageBox::information(setSignalsAtSlipFrame, tr("Information"), tr("Cannot set up logic because blocks have\nnot been defined around this item."));
        return;
    }
    connectorOccupancy = connectorBlock->getOccupancySensor();
    if (connectorOccupancy==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//                java.text.MessageFormat.format(tr("InfoMessage4"),
//                    new Object[]{connectorBlock.getUserName()}),
//                        NULL,JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Cannot set up logic because block \"%1\"\n                                                                doesn''t have an occupancy sensor.").arg(connectorBlock->getUserName()));
        return;
    }

    int nearState = layoutSlip->getTurnoutState(nearTurnout, continueState);
    int farState = layoutSlip->getTurnoutState(farTurnout, continueState);

    // setup signal head for continuing track of far turnout (or both tracks of far turnout)
    if ( (track1==nullptr) && setup1 ) {
//        JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//                tr("InfoMessage7"),"",JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Cannot set up logic because all connections\n                                                                  have not been defined around this item."));
        return;
    }
    Sensor* occupancy = NULL;
    SignalHead* nextHead = NULL;
    if ( (track1!=nullptr) && setup1) {
        LayoutBlock* block = track1->getLayoutBlock();
        if (block==nullptr) {
//            JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//                tr("InfoMessage6"),"",JOptionPane.INFORMATION_MESSAGE);
            QMessageBox::information(setSignalsAtSlipFrame, tr("Information"), tr("Cannot set up logic because blocks have\nnot been defined around this item."));
            return;
        }
        occupancy = block->getOccupancySensor();
        if (occupancy==nullptr) {
//            JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//                java.text.MessageFormat.format(tr("InfoMessage4"),
//                    new Object[]{block->getUserName()}),
//                        NULL,JOptionPane.INFORMATION_MESSAGE);
         QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Cannot set up logic because block \"%1\"\n                                                                doesn''t have an occupancy sensor.").arg(block->getUserName()));
            return;
        }
        //need to sort this out???
        nextHead = getNextSignalFromObject(track1,slip,
                                    head->getSystemName(), setSignalsAtSlipFrame);
        if ( (nextHead==nullptr) && (!reachedEndBumper()) ) {
//            JOptionPane.showMessageDialog(setSignalsFrame,
//                java.text.MessageFormat.format(tr("InfoMessage5"),
//                    new Object[]{block->getUserName()}),
//                        NULL,JOptionPane.INFORMATION_MESSAGE);
         QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Cannot set up logic because the next signal (in or \nat the end of block \"%1\") apparently is not yet defined.").arg(block->getUserName()));
            return;
        }
        if (secondHead!=nullptr)
        {
         // this head signals only the continuing track of the far turnout
         if (!initializeBlockBossLogic(head->getSystemName())) return;
         logic->setMode(BlockBossLogic::TRAILINGMAIN);
         if(farState==Turnout::THROWN)
             logic->setMode(BlockBossLogic::TRAILINGDIVERGING);
         logic->setTurnout(farTurnout->getSystemName());
         logic->setSensor1(occupancy->getSystemName());
         if (occupancy!=connectorOccupancy)
             logic->setSensor2(connectorOccupancy->getSystemName());
         if (nextHead!=nullptr) {
             logic->setWatchedSignal1(nextHead->getSystemName(),false);
         }
         if (auxSignal!=nullptr) {
             logic->setWatchedSignal1Alt(auxSignal->getSystemName());
         }
         QString nearSensorName = setupNearLogixSlip(nearTurnout, nearState, head, farTurnout, farState, slip, number);
         addNearSensorToSlipLogic(nearSensorName);
         finalizeBlockBossLogic();
        }
       }
    if ( (secondHead!=nullptr) && !setup2 ) return;
    SignalHead* savedAuxSignal = auxSignal;
    if (track2==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//                tr("InfoMessage7"),"",JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Cannot set up logic because all connections\n                                                                  have not been defined around this item."));
        return;
    }
    LayoutBlock* block2 = track2->getLayoutBlock();
    if (block2==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//                tr("InfoMessage6"),"",JOptionPane.INFORMATION_MESSAGE);
        QMessageBox::information(setSignalsAtSlipFrame, tr("Information"), tr("Cannot set up logic because blocks have\nnot been defined around this item."));
        return;
    }
    Sensor* occupancy2 = block2->getOccupancySensor();
    if (occupancy2==nullptr) {
//        JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//            java.text.MessageFormat.format(tr("InfoMessage4"),
//                new Object[]{block2.getUserName()}),
//                    NULL,JOptionPane.INFORMATION_MESSAGE);
     QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Cannot set up logic because block \"%1\"\n                                                                doesn''t have an occupancy sensor.").arg(block2->getUserName()));
        return;
    }
    SignalHead* nextHead2 = NULL;
    if (secondHead!=nullptr)
    {
     nextHead2 = getNextSignalFromObject(track2,
             slip, secondHead->getSystemName(), setSignalsAtSlipFrame);
     if ( (nextHead2==nullptr) && (!reachedEndBumper()) ) {
//            JOptionPane.showMessageDialog(setSignalsAtSlipFrame,
//                java.text.MessageFormat.format(tr("InfoMessage5"),
//                    new Object[]{block2.getUserName()}),
//                        NULL,JOptionPane.INFORMATION_MESSAGE);
      QMessageBox::critical(setSignalsAtSlipFrame, tr("Error"), tr("Cannot set up logic because the next signal (in or \n                                                                      at the end of block \"%1\") apparently is not yet defined.").arg(block2->getUserName()));
         return;
     }
    }
    if ( (secondHead==nullptr) && (track1!=nullptr) && setup1 )
    {
     if (!initializeBlockBossLogic(head->getSystemName())) return;
     logic->setMode(BlockBossLogic::FACING);
     logic->setTurnout(farTurnout->getSystemName());
     if(occupancy!=nullptr)
         logic->setWatchedSensor1(occupancy->getSystemName());
     logic->setWatchedSensor2(occupancy2->getSystemName());
     logic->setSensor2(connectorOccupancy->getSystemName());
     if (nextHead!=nullptr) {
         logic->setWatchedSignal1(nextHead->getSystemName(),false);
     }
     if (savedAuxSignal!=nullptr) {
         logic->setWatchedSignal1Alt(savedAuxSignal->getSystemName());
     }
     if (nextHead2!=nullptr) {
         logic->setWatchedSignal2(nextHead2->getSystemName());
     }
     if (auxSignal!=nullptr) {
         logic->setWatchedSignal2Alt(auxSignal->getSystemName());
     }
     QString nearSensorName = setupNearLogixSlip(nearTurnout, nearState,head, farTurnout, farState, slip, number+1);
     addNearSensorToSlipLogic(nearSensorName);
     logic->setLimitSpeed2(true);
     finalizeBlockBossLogic();
    }
    else if ( (secondHead!=nullptr) && setup2)
    {
        if (!initializeBlockBossLogic(secondHead->getSystemName())) return;
        nearState = layoutSlip->getTurnoutState(nearTurnout, divergeState);
        farState = layoutSlip->getTurnoutState(farTurnout, divergeState);

        logic->setMode(BlockBossLogic::TRAILINGDIVERGING);
        if(farState==Turnout::CLOSED){
            logic->setMode(BlockBossLogic::TRAILINGMAIN);
            logic->setLimitSpeed1(true);
        } else {
            logic->setLimitSpeed2(true);
        }
        logic->setTurnout(farTurnout->getSystemName());
        logic->setSensor1(occupancy2->getSystemName());
        if (occupancy2!=connectorOccupancy)
            logic->setSensor2(connectorOccupancy->getSystemName());
        if (nextHead2!=nullptr) {
            logic->setWatchedSignal1(nextHead2->getSystemName(),false);
        }
        if (auxSignal!=nullptr) {
            logic->setWatchedSignal1Alt(auxSignal->getSystemName());
        }
        QString nearSensorName = setupNearLogixSlip(nearTurnout, nearState, secondHead, farTurnout, farState, slip, number+1);
        addNearSensorToSlipLogic(nearSensorName);
        finalizeBlockBossLogic();
    }
}
/*private*/ QString LayoutEditorTools::setupNearLogixSlip(Turnout* turn, int nearState, SignalHead* /*head*/, Turnout* farTurn, int farState, LayoutSlip* slip, int number)
{
 QString turnoutName = turn->getDisplayName();
 QString farTurnoutName = farTurn->getDisplayName();

 QString logixName = "SYS_LAYOUTSLIP:"+slip->getName();
 QString sensorName = "IS:"+logixName+"C"+number;
 Sensor* sensor = ((ProxySensorManager*) InstanceManager::sensorManagerInstance())->provideSensor(sensorName);
 if (sensor==nullptr) {
     log->error("Trouble creating sensor "+sensorName+" while setting up Logix->");
     return "";
 }
 bool newConditional = false;
 Logix* x = (Logix*)((LogixManager*)InstanceManager::getDefault("LogixManager"))->getBySystemName(logixName);
 if(x == nullptr)
 {
  // Logix does not exist, create it
  x = ((LogixManager*)InstanceManager::getDefault("LogixManager"))->createNewLogix(logixName, "");
  if(x==nullptr)
  {
   log->error("Trouble creating logix "+logixName+" while setting up signal logic->");
   return "";
  }
  x->setComment("Layout Slip, Signalhead logic");
 }
 x->deActivateLogix();
 QString cName = logixName+"C" + QString::number(number);

 Conditional* c = ((ConditionalManager*) InstanceManager::getDefault("ConditionalManager"))->getBySystemName(cName);
 if (c == nullptr)
 {
  c = ((ConditionalManager*) InstanceManager::getDefault("ConditionalManager"))->
                     createNewConditional(cName, "");
  newConditional = true;
  if (c == nullptr)
  {
   log->error("Trouble creating conditional " + cName + " while setting up Logix.");
                 return "";
  }
 }
 int type = Conditional::TYPE_TURNOUT_THROWN;
 if (nearState == Turnout::CLOSED) {
     type = Conditional::TYPE_TURNOUT_CLOSED;
 }

 QList<ConditionalVariable*>* variableList = new QList<ConditionalVariable*>();
 variableList->append(new ConditionalVariable(false, Conditional::OPERATOR_AND,
         type, turnoutName, true));

 type = Conditional::TYPE_TURNOUT_THROWN;
 if (farState == Turnout::CLOSED) {
     type = Conditional::TYPE_TURNOUT_CLOSED;
 }
 variableList->append(new ConditionalVariable(false, Conditional::OPERATOR_AND,
         type, farTurnoutName, true));
 c->setStateVariables(variableList);
 QList<ConditionalAction*>* actionList = new QList<ConditionalAction*>();
 actionList->append(new DefaultConditionalAction(Conditional::ACTION_OPTION_ON_CHANGE_TO_TRUE,
         Conditional::ACTION_SET_SENSOR, sensorName,
         Sensor::INACTIVE, ""));
 actionList->append(new DefaultConditionalAction(Conditional::ACTION_OPTION_ON_CHANGE_TO_FALSE,
         Conditional::ACTION_SET_SENSOR, sensorName,
         Sensor::ACTIVE, ""));
 c->setAction(actionList);        // string data
 if (newConditional) {
     x->addConditional(cName, -1);
 }
 x->activateLogix();
 return sensorName;
}
/*
 * Adds the sensor specified to the open BlockBossLogic, provided it is not already there and
 *		provided there is an open slot. If 'name' is NULL or empty, returns without doing anything.
 */
/*private*/ void LayoutEditorTools::addNearSensorToSlipLogic(QString name)
{
 if ( (name==nullptr) || name==("") ) return;
 // return if a sensor by this name is already present
 if ( (logic->getSensor1()!=nullptr) && (logic->getSensor1())==(name) ) return;
 if ( (logic->getSensor2()!=nullptr) && (logic->getSensor2())==(name) ) return;
 if ( (logic->getSensor3()!=nullptr) && (logic->getSensor3())==(name) ) return;
 if ( (logic->getSensor4()!=nullptr) && (logic->getSensor4())==(name) ) return;
 if ( (logic->getSensor5()!=nullptr) && (logic->getSensor5())==(name) ) return;
 // add in the first available slot
 if (logic->getSensor1()==nullptr) logic->setSensor1(name);
 else if (logic->getSensor2()==nullptr) logic->setSensor2(name);
 else if (logic->getSensor3()==nullptr) logic->setSensor3(name);
 else if (logic->getSensor4()==nullptr) logic->setSensor4(name);
 else if (logic->getSensor5()==nullptr) logic->setSensor5(name);
 else log->error("Error - could not add sensor to SSL for signal head "+logic->getDrivenSignal());
}

/*public*/ SignalHeadIcon* LayoutEditorTools::getSignalHeadIcon(QString signalName)
{
 SignalHeadIcon* l = new SignalHeadIcon(layoutEditor);
 l->setSignalHead(signalName);
 l->setIcon(tr("Red"), signalIconEditor->getIcon(0));
 l->setIcon(tr("Flashing Red"), signalIconEditor->getIcon(1));
 l->setIcon(tr("Yellow"), signalIconEditor->getIcon(2));
 l->setIcon(tr("Flashing Yellow"), signalIconEditor->getIcon(3));
 l->setIcon(tr("Green"), signalIconEditor->getIcon(4));
 l->setIcon(tr("Flashing Green"), signalIconEditor->getIcon(5));
 l->setIcon(tr("Dark"), signalIconEditor->getIcon(6));
 l->setIcon(tr("Held"), signalIconEditor->getIcon(7));
 l->setIcon(tr("Lunar"), signalIconEditor->getIcon(8));
 l->setIcon(tr("Flashing Lunar"), signalIconEditor->getIcon(9));
 l->rotate(90);
 return l;
}
/*protected*/ bool LayoutEditorTools::addLayoutTurnoutSignalHeadInfoToMenu(
            /*@Nonnull*/ QString inTurnoutNameA, /*@Nonnull*/ QString inTurnoutNameB,
            /*@Nonnull*/ QMenu* inMenu) {
        bool result = false; //assume failure (pessimist!)

        //lookup turnouts
        turnout = turnout1 = turnoutA = InstanceManager::turnoutManagerInstance()->getTurnout(inTurnoutNameA);
        turnout2 = turnoutB = InstanceManager::turnoutManagerInstance()->getTurnout(inTurnoutNameB);
        //map those to layout turnouts (if possible)
        for (LayoutTurnout* lt : layoutEditor->getLayoutTurnouts()) {
            Turnout* to = lt->getTurnout();
            if (to != nullptr) {
                QString uname = to->getUserName();
                QString sname = to->getSystemName();
                if (!inTurnoutNameA.isEmpty() && (sname == (inTurnoutNameA) || ((uname != "") && uname == (inTurnoutNameA)))) {
                    layoutTurnout = layoutTurnout1 = layoutTurnoutA = lt;
                }
                if (!inTurnoutNameB.isEmpty() && (sname == (inTurnoutNameB) || ((uname != "") && uname == (inTurnoutNameB)))) {
                    layoutTurnout2 = layoutTurnoutB = lt;
                }
            }
        }

        int before_mcc = inMenu->actions().count();
        if (before_mcc != 0) {
            inMenu->addSeparator();// new JSeparator());
        }
        int linkType = layoutTurnout->getLinkType();
        if ((layoutTurnout->getTurnoutType() == LayoutTurnout::DOUBLE_XOVER)
                || (layoutTurnout->getTurnoutType() == LayoutTurnout::RH_XOVER)
                || (layoutTurnout->getTurnoutType() == LayoutTurnout::LH_XOVER)) {
            QAction* jmi = inMenu->addSection(tr("Crossover"));
            jmi->setEnabled(false);
            inMenu->addSeparator();//new JSeparator());
            before_mcc += 2;
            addInfoToMenu("A " + continuingString,  layoutTurnout->getSignalA1Name(), inMenu);
            addInfoToMenu("A " + divergingString,  layoutTurnout->getSignalA2Name(), inMenu);
            addInfoToMenu("B " + continuingString,  layoutTurnout->getSignalB1Name(), inMenu);
            addInfoToMenu("B " + divergingString,  layoutTurnout->getSignalB2Name(), inMenu);
            addInfoToMenu("C " + continuingString,  layoutTurnout->getSignalC1Name(), inMenu);
            addInfoToMenu("C " + divergingString,  layoutTurnout->getSignalC2Name(), inMenu);
            addInfoToMenu("D " + continuingString,  layoutTurnout->getSignalD1Name(), inMenu);
            addInfoToMenu("D " + divergingString,  layoutTurnout->getSignalD2Name(), inMenu);
        } else if (linkType ==  LayoutTurnout::NO_LINK) {
            QAction* jmi = inMenu->addSection(tr("Turnout"));
            jmi->setEnabled(false);
            inMenu->addSeparator();//new JSeparator());
            before_mcc += 2;
            addInfoToMenu(throatContinuingString,  layoutTurnout->getSignalA1Name(), inMenu);
            addInfoToMenu(throatDivergingString,  layoutTurnout->getSignalA2Name(), inMenu);
            addInfoToMenu(continuingString,  layoutTurnout->getSignalB1Name(), inMenu);
            addInfoToMenu(divergingString,  layoutTurnout->getSignalC1Name(), inMenu);
        } else if (linkType ==  LayoutTurnout::THROAT_TO_THROAT) {
            QString menuString = tr("Throat to Throat") + " (";
            menuString += tr("Turnout") + ", " + tr("Route");
            menuString += ", " + tr("SignalHead") + ":)";
            QAction* jmi = inMenu->addSection(menuString);
            jmi->setEnabled(false);
            inMenu->addSeparator();//new JSeparator());
            before_mcc += 2;
            addInfoToMenu(eastString + ", " + continuingString + ", " + continuingString,  layoutTurnout1->getSignalB1Name(), inMenu);
            addInfoToMenu(eastString + ", " + continuingString + ", " + divergingString,  layoutTurnout1->getSignalB2Name(), inMenu);
            addInfoToMenu(eastString + ", " + divergingString + ", " + continuingString,  layoutTurnout1->getSignalC1Name(), inMenu);
            addInfoToMenu(eastString + ", " + divergingString + ", " + divergingString,  layoutTurnout1->getSignalC2Name(), inMenu);
            addInfoToMenu(westString + ", " + continuingString + ", " + continuingString,  layoutTurnout2->getSignalB1Name(), inMenu);
            addInfoToMenu(westString + ", " + continuingString + ", " + divergingString,  layoutTurnout2->getSignalB2Name(), inMenu);
            addInfoToMenu(westString + ", " + divergingString + ", " + continuingString,  layoutTurnout2->getSignalC1Name(), inMenu);
            addInfoToMenu(westString + ", " + divergingString + ", " + divergingString,  layoutTurnout2->getSignalC2Name(), inMenu);
        } else if (linkType ==  LayoutTurnout::FIRST_3_WAY) {
            QAction* jmi = inMenu->addSection(tr("ThreeWay"));
            jmi->setEnabled(false);
            inMenu->addSeparator();//new JSeparator());
            before_mcc += 2;
            addInfoToMenu(throatString + " " + continuingString, layoutTurnoutA->getSignalA1Name(), inMenu);
            addInfoToMenu(throatString + " " + divergingAString, layoutTurnoutA->getSignalA2Name(), inMenu);
            addInfoToMenu(throatString + " " + divergingBString, layoutTurnoutA->getSignalA3Name(), inMenu);
            addInfoToMenu(continuingString, layoutTurnoutA->getSignalC1Name(), inMenu);
            addInfoToMenu(divergingAString, layoutTurnoutB->getSignalB1Name(), inMenu);
            addInfoToMenu(divergingBString, layoutTurnoutB->getSignalC1Name(), inMenu);
        } else if (linkType ==  LayoutTurnout::SECOND_3_WAY) {
            QAction* jmi = inMenu->addSection(tr("ThreeWay"));
            jmi->setEnabled(false);
            inMenu->addSeparator();// new JSeparator());
            before_mcc += 2;
            addInfoToMenu(throatString + " " + continuingString, layoutTurnoutB->getSignalA1Name(), inMenu);
            addInfoToMenu(throatString + " " + divergingAString, layoutTurnoutB->getSignalA2Name(), inMenu);
            addInfoToMenu(throatString + " " + divergingBString, layoutTurnoutB->getSignalA3Name(), inMenu);
            addInfoToMenu(continuingString, layoutTurnoutB->getSignalC1Name(), inMenu);
            addInfoToMenu(divergingAString, layoutTurnoutA->getSignalB1Name(), inMenu);
            addInfoToMenu(divergingBString, layoutTurnoutA->getSignalC1Name(), inMenu);
        }
        int after_mcc = inMenu->actions().count();
        if (before_mcc != after_mcc) {
            inMenu->addSeparator();//new JSeparator());
            result = true;   //it's GOOD!
        }
        return result;
    }   //addLayoutTurnoutSignalHeadInfoToMenu

/*private*/ void LayoutEditorTools::addInfoToMenu(/*@CheckForNull*/ QString title,
        /*@CheckForNull*/ QString info, /*@Nonnull*/ QMenu* menu) {
    if ((title != "") && !title.isEmpty() && (info != "") && !info.isEmpty()) {
        addInfoToMenu(title + ": " + info, menu);
    }
}

/*private*/ void LayoutEditorTools::addInfoToMenu(/*@CheckForNull*/ QString info, /*@Nonnull*/ QMenu* menu) {
    if ((info != "") && !info.isEmpty()) {
        //QAction* jmi = new JMenuItem(info);
     QAction* jmi = menu->addSection(info);
        jmi->setEnabled(false);
        //menu.add(jmi);
    }
}

  /*static*/ Logger* LayoutEditorTools::log = LoggerFactory::getLogger("LayoutEditorTools");
//}

void LayoutEditorTools::block1NameField_textChanged(QString text)
{
 QCompleter* completer = static_cast<LayoutBlockManager*>(InstanceManager::getDefault("LayutBlockManager"))->getCompleter(text, true);
 if(completer!= nullptr)
   block1NameField->setCompleter(completer);
}

void LayoutEditorTools::block2NameField_textChanged(QString text)
{
    QCompleter* completer = static_cast<LayoutBlockManager*>(InstanceManager::getDefault("LayutBlockManager"))->getCompleter(text, true);
    if(completer!= nullptr)
      block2NameField->setCompleter(completer);
}
void LayoutEditorTools::eastBoundField_textChanged(QString text)
{
    QCompleter* completer = static_cast<SignalHeadManager*>(InstanceManager::getDefault("SignalHeadManager"))->getCompleter(text, true);
    if(completer!= nullptr)
      eastBoundField->setCompleter(completer);
}
void LayoutEditorTools::westBoundField_textChanged(QString text)
{
    QCompleter* completer = ((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->getCompleter(text, true);
    if(completer!= nullptr)
      eastBoundField->setCompleter(completer);
}

void LayoutEditorTools::on_a1TToTField_textEdited(QString txt)
{ commonTField(txt, a1TToTField);}

void LayoutEditorTools::on_a2TToTField_textEdited(QString txt)
{ commonTField(txt, a2TToTField);}

void LayoutEditorTools::on_b1TToTField_textEdited(QString txt)
{ commonTField(txt, b1TToTField);}

void LayoutEditorTools::on_b2TToTField_textEdited(QString txt)
{ commonTField(txt, b2TToTField);}

void LayoutEditorTools::on_c1TToTField_textEdited(QString txt)
{ commonTField(txt, c1TToTField);}

void LayoutEditorTools::on_c2TToTField_textEdited(QString txt)
{ commonTField(txt, c2TToTField);}

void LayoutEditorTools::on_d1TToTField_textEdited(QString txt)
{ commonTField(txt, d1TToTField);}

void LayoutEditorTools::on_d2TToTField_textEdited(QString txt)
{ commonTField(txt, d2TToTField);}

void LayoutEditorTools::commonTField(QString txt, JTextField* f)
{
 QCompleter* completer;
 QStringList l = ((SignalHeadManager*)InstanceManager::InstanceManager::getDefault("SignalHeadManager"))->getSystemNameList();
 QStringList c =   QStringList();
 foreach(QString s, l)
 {
  if(s.startsWith(txt))
   c.append(s);
 }
 completer = new QCompleter(c);
 f->setCompleter(completer);
}




/*private*/ void LayoutEditorTools::oneFrameToRuleThemAll(/*@Nonnull*/ JmriJFrame* goodFrame) {
    setSensorsAtBlockBoundaryFrame = closeIfNotFrame(goodFrame, setSensorsAtBlockBoundaryFrame);
//    setSensorsAtLevelXingFrame = closeIfNotFrame(goodFrame, setSensorsAtLevelXingFrame);
//    setSensorsAtSlipFrame = closeIfNotFrame(goodFrame, setSensorsAtSlipFrame);
//    setSensorsAtTurnoutFrame = closeIfNotFrame(goodFrame, setSensorsAtTurnoutFrame);
//    setSignalMastsAtBlockBoundaryFrame = closeIfNotFrame(goodFrame, setSignalMastsAtBlockBoundaryFrame);
//    setSignalMastsAtLayoutSlipFrame = closeIfNotFrame(goodFrame, setSignalMastsAtLayoutSlipFrame);
//    setSignalMastsAtLevelXingFrame = closeIfNotFrame(goodFrame, setSignalMastsAtLevelXingFrame);
//    setSignalMastsAtTurnoutFrame = closeIfNotFrame(goodFrame, setSignalMastsAtTurnoutFrame);
//    setSignalsAt3WayTurnoutFrame = closeIfNotFrame(goodFrame, setSignalsAt3WayTurnoutFrame);
//    setSignalsAtBlockBoundaryFrame = closeIfNotFrame(goodFrame, setSignalsAtBlockBoundaryFrame);
//    setSignalsAtLevelXingFrame = closeIfNotFrame(goodFrame, setSignalsAtLevelXingFrame);
    setSignalsAtSlipFrame = closeIfNotFrame(goodFrame, setSignalsAtSlipFrame);
    setSignalsAtThroatToThroatTurnoutsFrame = closeIfNotFrame(goodFrame, setSignalsAtThroatToThroatTurnoutsFrame);
    setSignalsAtTurnoutFrame = closeIfNotFrame(goodFrame, setSignalsAtTurnoutFrame);
    setSignalsAtXoverTurnoutFrame = closeIfNotFrame(goodFrame, setSignalsAtXoverTurnoutFrame);
}

/*private*/ JmriJFrame* LayoutEditorTools::closeIfNotFrame(/*@Nonnull */JmriJFrame* goodFrame, /*@CheckForNull*/ JmriJFrame* badFrame) {
    JmriJFrame* result = badFrame;
    if ((badFrame != nullptr) && (goodFrame != badFrame)) {
        badFrame->setVisible(false);
        badFrame->dispose();
        result = nullptr;
    }
    return result;
}
