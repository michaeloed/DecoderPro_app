#include "frmdebugging.h"
#include "commonsubs.h"

/**
 *
 * @author Gregory J. Bedlek Copyright (C) 2018, 2019
 */
///*public*/ class FrmDebugging extends JFrame {

    /**
     * Creates new form dlgProperties
     */
    /*private*/ /*static*/ /*final*/ QString FrmDebugging::FORM_PROPERTIES = "DlgDebugging";   // NOI18N

    /*private*/ void FrmDebugging::initOrig(OtherData* otherData) {
        _mCTCDebugSystemReloadInternalSensorOrig = otherData->_mCTCDebugSystemReloadInternalSensor->getHandleName();
        _mCTCDebug_TrafficLockingRuleTriggeredDisplayInternalSensorOrig = otherData->_mCTCDebug_TrafficLockingRuleTriggeredDisplayInternalSensor->getHandleName();
    }
    /*private*/ bool FrmDebugging::dataChanged() {
        if (_mCTCDebugSystemReloadInternalSensorOrig != (_mCTCSystemReloadInternalSensor->text())) return true;
        if (_mCTCDebug_TrafficLockingRuleTriggeredDisplayInternalSensorOrig != (_mCTCDebug_TrafficLockingRuleTriggeredDisplayInternalSensor->text())) return true;
        return false;
    }

    /*public*/ FrmDebugging::FrmDebugging(AwtWindowProperties* awtWindowProperties, OtherData* otherData, QWidget* parent) : JFrame(parent){
        //super();
        initComponents();
        CommonSubs::addHelpMenu(this, "package.jmri.jmrit.ctc.CTC_menuCfgDeb", true);  // NOI18N
        _mAwtWindowProperties = awtWindowProperties;
        _mOtherData = otherData;
        _mCTCSystemReloadInternalSensor->setText(otherData->_mCTCDebugSystemReloadInternalSensor->getHandleName());
        _mCTCDebug_TrafficLockingRuleTriggeredDisplayInternalSensor->setText(otherData->_mCTCDebug_TrafficLockingRuleTriggeredDisplayInternalSensor->getHandleName());
        initOrig(otherData);
        _mAwtWindowProperties->setWindowState(this, FORM_PROPERTIES);
        //this.getRootPane().setDefaultButton(_mSaveAndClose);
        _mSaveAndClose->setDefault(true);
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    // <editor-fold defaultstate="collapsed" desc="Generated Code">
    /*private*/ void FrmDebugging::initComponents() {

        _mSaveAndClose = new JButton();
        _mCTCSystemReloadInternalSensor = new JTextField();
        jLabel3 = new JLabel();
        jLabel4 = new JLabel();
        _mCTCDebug_TrafficLockingRuleTriggeredDisplayInternalSensor = new JTextField();
        jLabel1 = new JLabel();
        jLabel2 = new JLabel();
        jLabel5 = new JLabel();

        setDefaultCloseOperation(JFrame::DO_NOTHING_ON_CLOSE);
        setTitle(tr("CTC Debugging"));
//        addWindowListener(new java.awt.event.WindowAdapter() {
//            @Override
//            public void windowClosing(java.awt.event.WindowEvent evt) {
//                formWindowClosing(evt);
//            }
//        });
        addWindowListener(new FrmDebugging_windowListener(this));

        _mSaveAndClose->setText(tr("Save andClose"));
//        _mSaveAndClose.addActionListener(new java.awt.event.ActionListener() {
//            @Override
//            public void actionPerformed(java.awt.event.ActionEvent evt) {
        connect(_mSaveAndClose, &JButton::clicked, [=]{
                _mSaveAndCloseActionPerformed(/*evt*/);
//            }
        });

        jLabel3->setText(tr("CTC system reload sensor:"));

        jLabel4->setText(tr("When the above sensor goes active, it triggers the CTC system to reload.  Leave blank for none."));

        jLabel1->setText(tr("CTC debug traffic locking rule triggered display sensor:"));

        jLabel2->setText(tr("When this sensor is active: each time a traffic locking rule is valid, it logs to the JMRI System Console which one was triggered"));

        jLabel5->setText(tr("Leave blank for none."));
#if 0
        GroupLayout layout = new GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addGroup(layout.createParallelGroup(GroupLayout.Alignment.LEADING)
                    .addGroup(layout.createSequentialGroup()
                        .addContainerGap()
                        .addGroup(layout.createParallelGroup(GroupLayout.Alignment.LEADING)
                            .addComponent(jLabel1)
                            .addComponent(jLabel3, GroupLayout.Alignment.TRAILING))
                        .addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
                        .addGroup(layout.createParallelGroup(GroupLayout.Alignment.LEADING)
                            .addComponent(_mCTCDebug_TrafficLockingRuleTriggeredDisplayInternalSensor)
                            .addComponent(_mCTCSystemReloadInternalSensor)))
                    .addGroup(layout.createSequentialGroup()
                        .addGroup(layout.createParallelGroup(GroupLayout.Alignment.LEADING)
                            .addGroup(layout.createSequentialGroup()
                                .addGap(246, 246, 246)
                                .addComponent(_mSaveAndClose))
                            .addGroup(layout.createSequentialGroup()
                                .addContainerGap()
                                .addComponent(jLabel5))
                            .addGroup(layout.createSequentialGroup()
                                .addContainerGap()
                                .addGroup(layout.createParallelGroup(GroupLayout.Alignment.LEADING)
                                    .addComponent(jLabel4)
                                    .addComponent(jLabel2))))
                        .addGap(0, 0, Short.MAX_VALUE)))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(GroupLayout.Alignment.BASELINE)
                    .addComponent(_mCTCSystemReloadInternalSensor, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE)
                    .addComponent(jLabel3))
                .addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jLabel4)
                .addGap(18, 18, 18)
                .addGroup(layout.createParallelGroup(GroupLayout.Alignment.BASELINE)
                    .addComponent(jLabel1)
                    .addComponent(_mCTCDebug_TrafficLockingRuleTriggeredDisplayInternalSensor, GroupLayout.PREFERRED_SIZE, GroupLayout.DEFAULT_SIZE, GroupLayout.PREFERRED_SIZE))
                .addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jLabel2)
                .addPreferredGap(LayoutStyle.ComponentPlacement.RELATED)
                .addComponent(jLabel5)
                .addPreferredGap(LayoutStyle.ComponentPlacement.RELATED, 28, Short.MAX_VALUE)
                .addComponent(_mSaveAndClose)
                .addContainerGap())
        );
#else
        JPanel* contentPane = new JPanel();
        QVBoxLayout* contentPaneLayout;
        contentPane->setLayout(contentPaneLayout = new QVBoxLayout());
        setCentralWidget(contentPane);

        JPanel* panel1 = new JPanel(new QHBoxLayout());
        ((QHBoxLayout*)panel1->layout())->addWidget(jLabel3,0, Qt::AlignRight);
        panel1->layout()->addWidget(_mCTCSystemReloadInternalSensor);
        contentPaneLayout->addWidget(panel1, 0, Qt::AlignHCenter);
        contentPaneLayout->addWidget(jLabel4);

        JPanel* panel2 = new JPanel(new QHBoxLayout());
        ((QHBoxLayout*)panel2->layout())->addWidget(jLabel1,0, Qt::AlignRight);
        panel1->layout()->addWidget(_mCTCDebug_TrafficLockingRuleTriggeredDisplayInternalSensor);
        contentPaneLayout->addWidget(panel2, 0, Qt::AlignHCenter);
        contentPaneLayout->addWidget(jLabel2, 0, Qt::AlignHCenter);
        contentPaneLayout->addWidget(jLabel5, 0, Qt::AlignHCenter);

        JPanel* panel3 = new JPanel(new FlowLayout());
        panel3->layout()->addWidget(_mSaveAndClose);
        contentPaneLayout->addWidget(panel3, 0, Qt::AlignHCenter);

#endif
        pack();
    }// </editor-fold>

    /*private*/ void FrmDebugging::formWindowClosing(QCloseEvent* evt) {
        _mAwtWindowProperties->saveWindowState(this, FORM_PROPERTIES);
        if (CommonSubs::allowClose(this, dataChanged())) dispose();
    }

    /*private*/ void FrmDebugging::_mSaveAndCloseActionPerformed(/*java.awt.event.ActionEvent evt*/) {
        if (dataChanged()) {

            NBHSensor* newSensor = CommonSubs::getNBHSensor(_mCTCSystemReloadInternalSensor->text(), true);
            if (newSensor != nullptr) _mOtherData->_mCTCDebugSystemReloadInternalSensor = newSensor;

            newSensor = CommonSubs::getNBHSensor(_mCTCDebug_TrafficLockingRuleTriggeredDisplayInternalSensor->text(), true);
            if (newSensor != nullptr) _mOtherData->_mCTCDebug_TrafficLockingRuleTriggeredDisplayInternalSensor = newSensor;
        }

        _mClosedNormally = true;
        _mAwtWindowProperties->saveWindowState(this, FORM_PROPERTIES);
        dispose();
    }

