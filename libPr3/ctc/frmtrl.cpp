#include "frmtrl.h"
#include "instancemanager.h"
#include "otherdata.h"
#include "topologyinfo.h"
#include "checkjmriobject.h"
#include "frmtrl_rules.h"

/**
 *
 * @author Gregory J. Bedlek Copyright (C) 2018, 2019
 */
// /*public*/ class FrmTRL extends JFrame {

/**
 * Creates new form DlgTRL
 */
/*private*/ /*static*/ /*final*/ QString FrmTRL::FORM_PROPERTIES = "DlgTRL";     // NOI18N

/*private*/ void FrmTRL::initOrig() {
}
/*private*/ bool FrmTRL::dataChanged() {
    return false;
}

/*public*/ FrmTRL::FrmTRL(  AwtWindowProperties* awtWindowProperties, CodeButtonHandlerData* codeButtonHandlerData,
                CTCSerialData* ctcSerialData, CheckJMRIObject* checkJMRIObject, QWidget* parent) : JFrame(parent){
    //super();
    initComponents();
    CommonSubs::addHelpMenu(this, "package.jmri.jmrit.ctc.CTC_frmTRL", true);  // NOI18N
    _mMainForm = (FrmMainForm*)InstanceManager::getDefault("FrmMainForm");
    _mAwtWindowProperties = awtWindowProperties;
    _mCodeButtonHandlerData = codeButtonHandlerData;
    _mCTCSerialData = ctcSerialData;
    _mCheckJMRIObject = checkJMRIObject;
    initOrig();
    _mAwtWindowProperties->setWindowState(this, FORM_PROPERTIES);
    //this->getRootPane()->setDefaultButton(_mOK);
    _mOK->setDefault(true);
    updateRuleCounts();
    this->setTitle(tr("TitleDlgTRL") + " " + codeButtonHandlerData->myShortStringNoComma());   // NOI18N
    QList<QString> listOfOSSectionOccupiedExternalSensors = getListOfExternalSensorsSlaved(codeButtonHandlerData, _mCTCSerialData->getCodeButtonHandlerDataArrayList());
    _mTopology = new Topology(_mCTCSerialData, listOfOSSectionOccupiedExternalSensors, tr("TLE_Normal"), tr("TLE_Reverse"));  // NOI18N
    bool isMastSignalType = _mCTCSerialData->getOtherData()->_mSignalSystemType == OtherData::SIGNAL_SYSTEM_TYPE::SIGNALMAST;
    bool topologyAvailable = _mTopology->isTopologyAvailable() && isMastSignalType;
    _mAutoGenerate->setVisible(topologyAvailable);
    _mReverseLeftRight->setVisible(topologyAvailable);
    _mAutoGenerateWarning->setVisible(topologyAvailable);
}

/*private*/ QList<QString> FrmTRL::getListOfExternalSensorsSlaved(   CodeButtonHandlerData* currentCodeButtonHandlerData,
                                                            QList<CodeButtonHandlerData*> codeButtonHandlerDataArrayList) {
    QList<QString> returnValue = QList<QString>();
    returnValue.append(currentCodeButtonHandlerData->_mOSSectionOccupiedExternalSensor->getHandleName());    // Put ours in there at least.
    int currentUniqueID = currentCodeButtonHandlerData->_mUniqueID;
    for (CodeButtonHandlerData* codeButtonHandlerData : codeButtonHandlerDataArrayList) {
        if (currentCodeButtonHandlerData != codeButtonHandlerData    // Don't check ourselves, we've already put us in the list.
        && codeButtonHandlerData->_mOSSectionSwitchSlavedToUniqueID != CodeButtonHandlerData::SWITCH_NOT_SLAVED   // It's referencing someone else:
        && currentUniqueID == codeButtonHandlerData->_mOSSectionSwitchSlavedToUniqueID) {  // And it's referening "us"/
            returnValue.append(codeButtonHandlerData->_mOSSectionOccupiedExternalSensor->getHandleName());
        }
    }
    return returnValue;
}

/*public*/ /*static*/ bool FrmTRL::dialogCodeButtonHandlerDataValid(CheckJMRIObject* checkJMRIObject, CodeButtonHandlerData* codeButtonHandlerData) {
    if (!valid(checkJMRIObject, codeButtonHandlerData->_mTRL_LeftTrafficLockingRules)) return false;
    if (!valid(checkJMRIObject, codeButtonHandlerData->_mTRL_RightTrafficLockingRules)) return false;
    return true;
}

/*private*/ void FrmTRL::updateRuleCounts() {
    _mLeftNumberOfRules->setText(tr("InfoDlgTRLRules") + " " + QString::number(_mCodeButtonHandlerData->_mTRL_LeftTrafficLockingRules.size()));   // NOI18N
    _mRightNumberOfRules->setText(tr("InfoDlgTRLRules") + " " + QString::number(_mCodeButtonHandlerData->_mTRL_RightTrafficLockingRules.size()));      // NOI18N
    _mLeftNumberOfRulesPrompt->setForeground(valid(_mCheckJMRIObject, _mCodeButtonHandlerData->_mTRL_LeftTrafficLockingRules) ? Qt::black : Qt::red);
    _mRightNumberOfRulesPrompt->setForeground(valid(_mCheckJMRIObject, _mCodeButtonHandlerData->_mTRL_RightTrafficLockingRules) ? Qt::black : Qt::red);
}

/*private*/ /*static*/ bool FrmTRL::valid(CheckJMRIObject* checkJMRIObject, QList<TrafficLockingData*> trafficLockingRules) {
    for (TrafficLockingData* trafficLockingRule : trafficLockingRules) {
        if (!checkJMRIObject->validClass(VPtr<TrafficLockingData>::asQVariant( trafficLockingRule))) return false; // Error
    }
    return true;    // All valid
}

/**
 * This method is called from within the constructor to initialize the form.
 * WARNING: Do NOT modify this code. The content of this method is always
 * regenerated by the Form Editor.
 */
// <editor-fold defaultstate="collapsed" desc="Generated Code">
/*private*/ void FrmTRL::initComponents() {

    _mEditLeftTrafficLockingRules = new JButton();
    _mEditRightTrafficLockingRules = new JButton();
    _mLeftNumberOfRulesPrompt = new JLabel();
    _mRightNumberOfRulesPrompt = new JLabel();
    _mOK = new JButton();
    _mLeftNumberOfRules = new JLabel();
    _mRightNumberOfRules = new JLabel();
    jLabel10 = new JLabel();
    jLabel4 = new JLabel();
    jLabel11 = new JLabel();
    _mAutoGenerate = new JButton();
    _mReverseLeftRight = new JButton();
    _mAutoGenerateWarning = new JLabel();

    setDefaultCloseOperation(JFrame::DISPOSE_ON_CLOSE);
    setTitle(tr("Edit traffic locking"));
//    addWindowListener(new java.awt.event.WindowAdapter() {
//        @Override
//        /*public*/ void windowClosing(java.awt.event.WindowEvent evt) {
//            formWindowClosing(evt);
//        }
//    });
    addWindowListener(new FrmTRL_windowListener(this));

    _mEditLeftTrafficLockingRules->setText(tr("Edit"));
//    _mEditLeftTrafficLockingRules.addActionListener(new java.awt.event.ActionListener() {
//        /*public*/ void actionPerformed(/*java.awt.event.ActionEvent evt*/) {
    connect(_mEditLeftTrafficLockingRules, &JButton::clicked, [=]{
            _mEditLeftTrafficLockingRulesActionPerformed(/*evt*/);
            //        }
    });

    _mEditRightTrafficLockingRules->setText(tr("Edit"));
//    _mEditRightTrafficLockingRules.addActionListener(new java.awt.event.ActionListener() {
//        /*public*/ void actionPerformed(/*java.awt.event.ActionEvent evt*/) {
    connect(_mEditRightTrafficLockingRules, &JButton::clicked, [=]{
            _mEditRightTrafficLockingRulesActionPerformed(/*evt*/);
//        }
    });

    _mLeftNumberOfRulesPrompt->setText(tr("Left"));

    _mRightNumberOfRulesPrompt->setText(tr("Right"));

    _mOK->setText(tr("OK"));
//    _mOK.addActionListener(new java.awt.event.ActionListener() {
//        /*public*/ void actionPerformed(/*java.awt.event.ActionEvent evt*/) {
    connect(_mOK, &JButton::clicked, [=]{
            _mOKActionPerformed(/*evt*/);
//        }
    });

    _mLeftNumberOfRules->setText(tr("Rules: ?"));

    _mRightNumberOfRules->setText(tr("Rules: ?"));

    jLabel10->setText(tr("For example: Where you go into staging and there is"));

    jLabel4->setText(tr("NOTE: Having NO rules ALWAYS allows that direction."));

    jLabel11->setText(tr("no O.S. section set controlling entry/exit from the staging area."));

    _mAutoGenerate->setText(tr("LabelDlgTRLAutoGenerate"));
//    _mAutoGenerate.addActionListener(new java.awt.event.ActionListener() {
//        /*public*/ void actionPerformed(/*java.awt.event.ActionEvent evt*/) {
    connect(_mAutoGenerate, &JButton::clicked, [=]{
            _mAutoGenerateActionPerformed(/*evt*/);
//        }
    });

    _mReverseLeftRight->setText(tr("LabelDlgTRLReverseLeftRight"));
//    _mReverseLeftRight.addActionListener(new java.awt.event.ActionListener() {
//        /*public*/ void actionPerformed(/*java.awt.event.ActionEvent evt*/) {
    connect(_mReverseLeftRight, &JButton::clicked, [=]{
            _mReverseLeftRightActionPerformed(/*evt*/);
//        }
    });

    _mAutoGenerateWarning->setText(tr("ALL are deleted first:"));
#if 0
    GroupLayout layout = new GroupLayout(getContentPane());
    getContentPane()->setLayout(layout);
    layout->setHorizontalGroup(
        layout.createParallelGroup(GroupLayout.Alignment.LEADING)
        .addGroup(layout.createSequentialGroup()
            .addGroup(layout.createParallelGroup(GroupLayout.Alignment.LEADING)
                .addGroup(layout.createSequentialGroup()
                    .addGap(49, 49, 49)
                    .addGroup(layout.createParallelGroup(GroupLayout.Alignment.TRAILING)
                        .addComponent(_mLeftNumberOfRulesPrompt)
                        .addComponent(_mRightNumberOfRulesPrompt))
                    .addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
                    .addGroup(layout.createParallelGroup(GroupLayout.Alignment.LEADING)
                        .addGroup(layout.createSequentialGroup()
                            .addComponent(_mEditRightTrafficLockingRules)
                            .addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
                            .addComponent(_mRightNumberOfRules, GroupLayout.PREFERRED_SIZE, 62, GroupLayout.PREFERRED_SIZE)
                            .addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
                            .addComponent(_mReverseLeftRight))
                        .addGroup(layout.createSequentialGroup()
                            .addComponent(_mEditLeftTrafficLockingRules)
                            .addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
                            .addComponent(_mLeftNumberOfRules, GroupLayout.PREFERRED_SIZE, 62, GroupLayout.PREFERRED_SIZE)
                            .addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
                            .addGroup(layout.createParallelGroup(GroupLayout.Alignment.LEADING)
                                .addComponent(_mAutoGenerateWarning)
                                .addComponent(_mAutoGenerate, GroupLayout.PREFERRED_SIZE, 117, GroupLayout.PREFERRED_SIZE)))))
                .addGroup(layout.createSequentialGroup()
                    .addContainerGap()
                    .addComponent(jLabel4))
                .addGroup(layout.createSequentialGroup()
                    .addContainerGap()
                    .addComponent(jLabel10))
                .addGroup(layout.createSequentialGroup()
                    .addContainerGap()
                    .addComponent(jLabel11))
                .addGroup(layout.createSequentialGroup()
                    .addGap(164, 164, 164)
                    .addComponent(_mOK)))
            .addContainerGap(GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE))
    );
    layout->setVerticalGroup(
        layout.createParallelGroup(GroupLayout.Alignment.LEADING)
        .addGroup(layout.createSequentialGroup()
            .addContainerGap()
            .addComponent(_mAutoGenerateWarning)
            .addPreferredGap(LayoutStyle.ComponentPlacement.RELATED, GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
            .addGroup(layout.createParallelGroup(GroupLayout.Alignment.BASELINE)
                .addComponent(_mLeftNumberOfRulesPrompt)
                .addComponent(_mEditLeftTrafficLockingRules)
                .addComponent(_mLeftNumberOfRules)
                .addComponent(_mAutoGenerate))
            .addGap(18, 18, 18)
            .addGroup(layout.createParallelGroup(GroupLayout.Alignment.BASELINE)
                .addComponent(_mEditRightTrafficLockingRules)
                .addComponent(_mRightNumberOfRulesPrompt)
                .addComponent(_mRightNumberOfRules)
                .addComponent(_mReverseLeftRight))
            .addGap(13, 13, 13)
            .addComponent(jLabel4)
            .addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
            .addComponent(jLabel10)
            .addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
            .addComponent(jLabel11)
            .addGap(18, 18, 18)
            .addComponent(_mOK)
            .addContainerGap(31, Short.MAX_VALUE))
    );
#else
    JPanel* contentPane = new JPanel();
    QVBoxLayout* contentPaneLayout;
    contentPane->setLayout(contentPaneLayout = new QVBoxLayout());
    setCentralWidget(contentPane);


#endif
    pack();
}// </editor-fold>

/*private*/ void FrmTRL::formWindowClosing(QCloseEvent* evt) {
    _mAwtWindowProperties->saveWindowState(this, FORM_PROPERTIES);
    if (CommonSubs::allowClose(this, dataChanged())) dispose();
}

/*private*/ void FrmTRL::_mEditLeftTrafficLockingRulesActionPerformed(/*java.awt.event.ActionEvent evt*/) {
    if (_mMainForm->_mTRL_RulesFormOpen) return;
    _mMainForm->_mTRL_RulesFormOpen = true;
#if 1
    FrmTRL_Rules* dialog = new FrmTRL_Rules( _mAwtWindowProperties, _mCodeButtonHandlerData,
                                            true, _mCTCSerialData, _mCheckJMRIObject);
//    dialog.addWindowListener(new WindowAdapter() {
//        @Override
//        /*public*/ void windowClosed(WindowEvent e) {
//            if (dialog.closedNormally()) {
//                _mClosedNormally = true;
//                updateRuleCounts();
//            }
//            _mMainForm->_mTRL_RulesFormOpen = false;
//        }
//    });
    dialog->addWindowListener(new FrmTRL_Rules_windowListener(dialog, this));
    dialog->setVisible(true);  // MUST BE AFTER "addWindowListener"!  BUG IN AWT/SWING!
#endif
}

/*private*/ void FrmTRL::_mEditRightTrafficLockingRulesActionPerformed(/*java.awt.event.ActionEvent evt*/) {
    if (_mMainForm->_mTRL_RulesFormOpen) return;
    _mMainForm->_mTRL_RulesFormOpen = true;
#if 1
    FrmTRL_Rules* dialog = new FrmTRL_Rules( _mAwtWindowProperties, _mCodeButtonHandlerData,
                                            false, _mCTCSerialData, _mCheckJMRIObject);
//    dialog.addWindowListener(new WindowAdapter() {
//        @Override
//        /*public*/ void windowClosed(WindowEvent e) {
//            if (dialog.closedNormally()) {
//                _mClosedNormally = true;
//                updateRuleCounts();
//            }
//            _mMainForm._mTRL_RulesFormOpen = false;
//        }
//    });
    dialog->addWindowListener(new FrmTRL_Rules_windowListener(dialog, this));
    dialog->setVisible(true);  // MUST BE AFTER "addWindowListener"!  BUG IN AWT/SWING!
#endif
}

/*private*/ void FrmTRL::_mOKActionPerformed(/*java.awt.event.ActionEvent evt*/) {
    dispose();
}

/*private*/ void FrmTRL::_mReverseLeftRightActionPerformed(/*java.awt.event.ActionEvent evt*/) {
    QList<TrafficLockingData*> blah = _mCodeButtonHandlerData->_mTRL_LeftTrafficLockingRules;
    _mCodeButtonHandlerData->_mTRL_LeftTrafficLockingRules = _mCodeButtonHandlerData->_mTRL_RightTrafficLockingRules;
    _mCodeButtonHandlerData->_mTRL_RightTrafficLockingRules = blah;
    updateRuleCounts();
}

/*private*/ void FrmTRL::_mAutoGenerateActionPerformed(/*java.awt.event.ActionEvent evt*/) {

    QList<TopologyInfo*> topologyInfosArrayList = _mTopology->getTrafficLockingRules(true);        // Left traffic.
    _mCodeButtonHandlerData->_mTRL_LeftTrafficLockingRules.clear();
    for (int index = 0; index < topologyInfosArrayList.size(); index++) {
        TopologyInfo* topologyInfo = topologyInfosArrayList.at(index);
        TrafficLockingData* trafficLockingData = new TrafficLockingData(index + 1, topologyInfo->getDestinationSignalMast(), topologyInfo);
        _mCodeButtonHandlerData->_mTRL_LeftTrafficLockingRules.append(trafficLockingData);
    }

    topologyInfosArrayList = _mTopology->getTrafficLockingRules(false);        // Right traffic.
    _mCodeButtonHandlerData->_mTRL_RightTrafficLockingRules.clear();
    for (int index = 0; index < topologyInfosArrayList.size(); index++) {
        TopologyInfo* topologyInfo = topologyInfosArrayList.at(index);
        TrafficLockingData* trafficLockingData = new TrafficLockingData(index + 1, topologyInfo->getDestinationSignalMast(), topologyInfo);
        _mCodeButtonHandlerData->_mTRL_RightTrafficLockingRules.append(trafficLockingData);
    }

    updateRuleCounts();
}

