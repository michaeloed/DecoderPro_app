#include "simpleclockframe.h"
#include "decimalformat.h"
#include <QVBoxLayout>
#include "instancemanager.h"
#include "simpletimebase.h"
#include <QComboBox>
#include <QCheckBox>
#include "jlabel.h"

#include "defaultclockcontrol.h"
#include <QGroupBox>
#include <QDateTime>
#include <QMessageBox>
#include <QSizePolicy>
#include "clockcontrol.h"
#include <QBoxLayout>
#include <QPushButton>
#include "calendar.h"
#include "borderfactory.h"

SimpleClockFrame::SimpleClockFrame(QWidget *parent) :
    JmriJFrame(parent)
{
 changed = false;
 showTime = false;
 threeDigits = new DecimalFormat("0.000");	// 3 digit precision for speedup factor
 timeSourceBox = nullptr;
 clockStartBox = nullptr;

 synchronizeCheckBox = nullptr;
 correctCheckBox = nullptr;
 displayCheckBox = nullptr;
 showStartupCheckBox = nullptr;
 startStoppedCheckBox = nullptr;
 startSetTimeCheckBox = nullptr;

 factorField = new JTextField(5);
 hoursField = new JTextField(2);
 minutesField = new JTextField(2);
 startHoursField = new JTextField(2);
 startMinutesField = new JTextField(2);

 setRateButton = new QPushButton(tr("Set"));
 setTimeButton = new QPushButton(tr("Set"));
 startButton = new QPushButton(tr("Start"));
 stopButton = new QPushButton(tr("Stop"));
 setStartTimeButton = new QPushButton(tr("Set"));

 clockStatus = new JLabel();
 timeLabel = new JLabel();
 internalSourceIndex = 0;
 hardwareSourceIndex = 1;

 startNone = 0;
 startNixieClock = 1;
 startAnalogClock = 2;
 startLcdClock = 3;
 log = new Logger("SimpleClockFrame");
}
SimpleClockFrame::~SimpleClockFrame()
{
 delete log;
}

/**
 * Frame for user configuration of Simple Timebase
 *
 * The current implementation (2007) handles the internal clock and one hardware clock
 *
 * @author	Dave Duchamp   Copyright (C) 2004, 2007
 * @version	$Revision: 22216 $
 */
///*public*/ class SimpleClockFrame extends JmriJFrame
//	implements java.beans.PropertyChangeListener {



/**
 * Constructor method
 */
//    /*public*/ SimpleClockFrame() {
//    	super();

//    }

/**
 *  Initialize the config window
 */
//@SuppressWarnings("deprecation")
/*public*/ void SimpleClockFrame::initComponents() throw (Exception)
{
 setTitle(tr("Fast Clock Setup"));
 resize(400,500);

 QWidget* contentPane = getContentPane(true);
 QVBoxLayout* l = (QVBoxLayout*)contentPane->layout();
 l->setMargin(2);
 l->setContentsMargins(0,0,0,0);
 l->setSpacing(2);

 // Determine current state of the clock
 clock = static_cast<Timebase*>(InstanceManager::getDefault("Timebase"));
 if (clock==nullptr)
 {
  // could not initialize clock
  log->error("Could not obtain a timebase instance.");
  setVisible(false);
  dispose();
  throw new JmriException("Could not obtain a timebase instance");
 }
 if (!clock->getIsInitialized())
 {
  // if clocks have not been initialized at start up, do so now
  clock->initializeHardwareClock();
 }

 // Set up time source choice
 QWidget* panel11 = new QWidget();
 QHBoxLayout* lh1;
 panel11->setLayout(lh1 = new QHBoxLayout);
 lh1->setMargin(0);
 lh1->setContentsMargins(0,0,0,0);
 lh1->setSpacing(0);

 panel11->layout()->addWidget(new JLabel(tr("Time Source:")+" "));
 timeSourceBox = new QComboBox();
 panel11->layout()->addWidget(timeSourceBox);
 timeSourceBox->addItem(tr("Internal Computer Clock"));

 hardwareName = ((ClockControl*)InstanceManager::getDefault("ClockControl"))->getHardwareClockName();
 if (hardwareName!=nullptr) timeSourceBox->addItem(hardwareName);
 timeSourceBox->setToolTip(tr("Select time source for the fast clock."));
//    timeSourceBox.addActionListener(new java.awt.event.ActionListener() {
//            /*public*/ void actionPerformed(java.awt.event.ActionEvent e) {
//                setTimeSourceChanged();
//            }
//        });
 connect(timeSourceBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setTimeSourceChanged()));
 l->addWidget(panel11);
 if (hardwareName!="")
 {
  if (clock->getInternalMaster()) timeSourceBox->setCurrentIndex(internalSourceIndex);
  else timeSourceBox->setCurrentIndex(hardwareSourceIndex);
  QWidget* panel11x = new QWidget();
  QHBoxLayout* lh2;
  panel11x->setLayout(lh2 = new QHBoxLayout);
  lh2->setMargin(0);
  lh2->setContentsMargins(0,0,0,0);
  lh2->setSpacing(0);
  synchronizeCheckBox = new QCheckBox(tr("Synchronize Internal Fast Clock and")+" " + hardwareName);
  synchronizeCheckBox->setToolTip(tr("Check to synchronize the internal and hardware clocks."));
  synchronizeCheckBox->setChecked(clock->getSynchronize());
//        synchronizeCheckBox.addActionListener(new java.awt.event.ActionListener() {
//            /*public*/ void actionPerformed(java.awt.event.ActionEvent e) {
//                synchronizeChanged();
//            }
//        });
  connect(synchronizeCheckBox, SIGNAL(toggled(bool)), this, SLOT(synchronizeChanged()));
  panel11x->layout()->addWidget(synchronizeCheckBox);
  l->addWidget(panel11x);
  if (((ClockControl*)InstanceManager::getDefault("ClockControl"))->canCorrectHardwareClock())
  {
   QWidget* panel11y = new QWidget();
   QHBoxLayout* lh3;
   panel11y->setLayout(lh3 = new QHBoxLayout);
   lh3->setMargin(0);
   lh3->setContentsMargins(0,0,0,0);
   lh3->setSpacing(0);
   correctCheckBox = new QCheckBox(tr("Correct hardware clock"));
   correctCheckBox->setToolTip(tr("Check to correct hardware clock accuracy using the computer clock."));
   correctCheckBox->setChecked(clock->getCorrectHardware());
//            correctCheckBox.addActionListener(new java.awt.event.ActionListener() {
//                /*public*/ void actionPerformed(java.awt.event.ActionEvent e) {
//                    correctChanged();
//                }
//            });
   connect(correctCheckBox, SIGNAL(toggled(bool)), this, SLOT( correctChanged()));
   panel11y->layout()->addWidget(correctCheckBox);
   l->addWidget(panel11y);
  }
  if (((ClockControl*)InstanceManager::getDefault("ClockControl"))->canSet12Or24HourClock())
  {
   QWidget* panel11z = new QWidget();
   QHBoxLayout* lh4;
   panel11z->setLayout(lh4 = new QHBoxLayout);
   lh4->setMargin(0);
   lh4->setContentsMargins(0,0,0,0);
   lh4->setSpacing(0);
   displayCheckBox = new QCheckBox(tr("Use 12-hour clock display"));
   displayCheckBox->setToolTip(tr("Check to request a 12-hour fast clock display on your hardware clock."));
   displayCheckBox->setChecked(clock->use12HourDisplay());
//   displayCheckBox.addActionListener(new java.awt.event.ActionListener() {
//        /*public*/ void actionPerformed(java.awt.event.ActionEvent e) {
//            displayChanged();
//        }
//    });
   connect(displayCheckBox, SIGNAL(clicked()), this, SLOT(displayChanged()));
   panel11z->layout()->addWidget(displayCheckBox);
   l->addWidget(panel11z);
  }
 }

 // Set up speed up factor
 QWidget* panel12 = new QWidget();
 QHBoxLayout* lh5;
 panel12->setLayout(lh5 = new QHBoxLayout);
 lh5->setMargin(0);
 lh5->setContentsMargins(0,0,0,0);
 lh5->setSpacing(0);
 panel12->layout()->addWidget(new JLabel(tr("Fast Clock Rate:")+" "));
 panel12->layout()->addWidget(factorField);
 factorField->setText(threeDigits->format(clock->userGetRate()));
 factorField->setToolTip(tr("Enter multiplication factor for fast time."));
 panel12->layout()->addWidget(new JLabel(":1 "));
 setRateButton->setToolTip(tr("Click to set Fast Clock Rate to entered value."));
//    setRateButton.addActionListener(new java.awt.event.ActionListener() {
//            /*public*/ void actionPerformed(java.awt.event.ActionEvent e) {
//                setRateButtonActionPerformed();
//            }
//        });
 connect(setRateButton, SIGNAL(clicked()), this, SLOT(setRateButtonActionPerformed()));
 panel12->layout()->addWidget(setRateButton);
 l->addWidget(panel12);

 // Set up time setup information
 QWidget* panel2 = new QWidget();
 QHBoxLayout* lh6;
 panel2->setLayout(lh6 = new QHBoxLayout);
 lh6->setMargin(0);
 lh6->setContentsMargins(0,0,0,0);
 lh6->setSpacing(0);
 panel2->layout()->addWidget(new JLabel(tr("NewTime")+" "));
 panel2->layout()->addWidget(hoursField);
 hoursField->setText("00");
 hoursField->setToolTip(tr("Enter hours (0-23) for a 24-hour clock time."));
 panel2->layout()->addWidget(new JLabel(":"));
 panel2->layout()->addWidget(minutesField);
 minutesField->setText("00");
 minutesField->setToolTip(tr("Enter minutes (0-59) for a 24-hour clock time."));
 setTimeButton->setToolTip(tr("Click to set Fast Clock Time to entered value."));
//    setTimeButton.addActionListener(new java.awt.event.ActionListener() {
//            /*public*/ void actionPerformed(java.awt.event.ActionEvent e) {
//                setTimeButtonActionPerformed();
//            }
//        });
 connect(setTimeButton, SIGNAL(clicked()), this, SLOT(setTimeButtonActionPerformed()));
 panel2->layout()->addWidget(setTimeButton);
 l->addWidget(panel2);

 QString gbStyleSheet = "QGroupBox { border: 2px solid gray; border-radius: 3px;} QGroupBox::title { /*background-color: transparent;*/  subcontrol-position: top left; /* position at the top left*/  padding:0 0px;} ";

 // Set up startup options panel
 JPanel* startupOptionsPane = new JPanel();
 startupOptionsPane->setLayout(new QVBoxLayout());//startupOptionsPane, BoxLayout.Y_AXIS));
 JPanel* panel61 = new JPanel(new FlowLayout());
 panel61->layout()->addWidget(new JLabel(tr("Start with Fast Clock") + " "));
 startRunBox = new QComboBox();
 startRunBox->addItem(tr("Running"));
 startRunBox->addItem(tr("Stopped"));
 startRunBox->addItem(tr("NoChange"));
 startRunBox->setToolTip(tr("Select whether you want to start or stop the clock when loading the configuration. You can also choose to not change the running of the clock, which is useful when JMRI is not the clock master."));
 switch (clock->getClockInitialRunState()) {
     case DO_STOP:
         startRunBox->setCurrentIndex(START_STOPPED);
         break;
     case DO_START:
         startRunBox->setCurrentIndex(START_RUNNING);
         break;
     case DO_NOTHING:
         startRunBox->setCurrentIndex(START_NORUNCHANGE);
         break;
     default:
//         jmri.util.Log4JUtil.warnOnce(log, "Unexpected initial run state = {}", clock.getClockInitialRunState());
         break;
 }
// startRunBox.addActionListener(new ActionListener() {
//     @Override
//     public void actionPerformed(ActionEvent actionEvent) {
 connect(startRunBox, &QComboBox::currentTextChanged, [=]{
         startRunBoxChanged();
//     }
 });
 panel61->layout()->addWidget(startRunBox);
 startupOptionsPane->layout()->addWidget(panel61);

 JPanel* panel62 = new JPanel(new FlowLayout);
 startSetTimeCheckBox = new QCheckBox(tr("Set Fast Clock Time to"));
 startSetTimeCheckBox->setToolTip(tr("Check after entering start time at right if time should be set at start up."));
 startSetTimeCheckBox->setChecked(clock->getStartSetTime());
// startSetTimeCheckBox.addActionListener(new ActionListener() {
//     @Override
//     public void actionPerformed(ActionEvent e) {
 connect(startSetTimeCheckBox, &QCheckBox::toggled, [=]{
         startSetTimeChanged();
//     }
 });
 panel62->layout()->addWidget(startSetTimeCheckBox);
 Calendar* cal = Calendar::getInstance();
 cal->setTime(clock->getStartTime());
 startHoursField->setText(QString::number(cal->get(Calendar::HOUR_OF_DAY)));
 startHoursField->setToolTip(tr("Enter start time hours (0-23) for a 24-hour clock."));
 panel62->layout()->addWidget(startHoursField);
 panel62->layout()->addWidget(new JLabel(":"));
 startMinutesField->setText(QString::number(cal->get(Calendar::MINUTE)));
 startMinutesField->setToolTip(tr("Enter start time minutes (0-59) for a 24-hour clock."));
 panel62->layout()->addWidget(startMinutesField);
 setStartTimeButton->setToolTip(tr("Click to set start up Fast Clock Time to entered value."));
// setStartTimeButton.addActionListener(new ActionListener() {
//     @Override
//     public void actionPerformed(ActionEvent e) {
 connect(setStartTimeButton, &QPushButton::clicked, [=]{
         startSetTimeChanged();
//     }
 });
 panel62->layout()->addWidget(setStartTimeButton);
 startupOptionsPane->layout()->addWidget(panel62);

 JPanel* panelStartSetRate = new JPanel(new FlowLayout());
 startSetRateCheckBox = new QCheckBox(tr("Set Fast Clock Rate to") + " ");
 startSetRateCheckBox->setToolTip(tr("Click to set the rate of clock at startup to the entered value."));
 startSetRateCheckBox->setChecked(clock->getSetRateAtStart());
// startSetRateCheckBox.addActionListener(new ActionListener() {
//     @Override
//     public void actionPerformed(ActionEvent e) {
 connect(startSetRateCheckBox, &QCheckBox::toggled, [=]{
         startSetRateChanged();
//     }
 });
 panelStartSetRate->layout()->addWidget(startSetRateCheckBox);
 panelStartSetRate->layout()->addWidget(startFactorField);
 startFactorField->setText(threeDigits->format(clock->getStartRate()));
 startFactorField->setToolTip(tr("TipFactorField"));
// startFactorField.addActionListener(new ActionListener() {
//     @Override
//     public void actionPerformed(ActionEvefocusnt actionEvent) {
 connect(startFactorField, &JTextField::editingFinished, [=]{
         startFactorFieldChanged();
//     }
 });
 startFactorField->addFocusListener(/*new FocusAdapter()*/new SCFFocusListener(this) );
// {
//     @Override
//     public void focusLost(FocusEvent focusEvent) {
//         if (!focusEvent.isTemporary()) {
//             startFactorFieldChanged();
//         }
//         super.focusLost(focusEvent);
//     }
// });
 panelStartSetRate->layout()->addWidget(new JLabel(":1 "));
 startupOptionsPane->layout()->addWidget(panelStartSetRate);


 JPanel* panel63 = new JPanel(new FlowLayout());
 panel63->layout()->addWidget(new JLabel(tr("Display Selected Clock") + " "));
 clockStartBox = new QComboBox();
 panel63->layout()->addWidget(clockStartBox);
 clockStartBox->addItem(tr("None"));
 clockStartBox->addItem(tr("Nixie Clock"));
 clockStartBox->addItem(tr("Analog Clock"));
 clockStartBox->addItem(tr("Lcd Clock"));
 clockStartBox->addItem(tr("Pragotron Clock"));
 clockStartBox->setCurrentIndex(startNone);
 if (clock->getStartClockOption() == Timebase::NIXIE_CLOCK) {
     clockStartBox->setCurrentIndex(startNixieClock);
 } else {
     if (clock->getStartClockOption() == Timebase::ANALOG_CLOCK) {
         clockStartBox->setCurrentIndex(startAnalogClock);
     } else {
         if (clock->getStartClockOption() == Timebase::LCD_CLOCK) {
             clockStartBox->setCurrentIndex(startLcdClock);
         } else {
             if (clock->getStartClockOption() == Timebase::PRAGOTRON_CLOCK) {
                 clockStartBox->setCurrentIndex(startPragotronClock);
             }
         }
     }
 }
 clockStartBox->setToolTip(tr("TipClockStartOption"));
// clockStartBox.addActionListener(new ActionListener() {
//     @Override
//     public void actionPerformed(ActionEvent e) {
 connect(clockStartBox, &QComboBox::currentTextChanged, [=]{
         setClockStartChanged();
//     }
 });
 startupOptionsPane->layout()->addWidget(panel63);
 JPanel* panel64 = new JPanel(new FlowLayout());
 displayStartStopButton= new QCheckBox(tr("Display Start/Stop button on clock"));
 displayStartStopButton->setChecked(clock->getShowStopButton());
// displayStartStopButton.addActionListener(new ActionListener() {
//     @Override
//     public void actionPerformed(ActionEvent e) {
 connect(displayStartStopButton, &QCheckBox::toggled, [=]{
         showStopButtonChanged();
//     }
 });
 panel64->layout()->addWidget(displayStartStopButton);
 startupOptionsPane->layout()->addWidget(panel64);

 Border* panel6Border = BorderFactory::/*createEtchedBorder*/createLineBorder(Qt::red);
 Border* panel6Titled = BorderFactory::createTitledBorder(panel6Border,
         tr("BoxLabelStartUp"));
 startupOptionsPane->setBorder(panel6Titled);
 contentPane->layout()->addWidget(startupOptionsPane);


 // Set up clock information panel
 QGroupBox* panel3 = new QGroupBox();
 QVBoxLayout* l3;
 panel3->setLayout(l3 = new QVBoxLayout(panel3/*, BoxLayout.Y_AXIS*/));
 l3->setMargin(0);
 l3->setContentsMargins(0,1,0,1);
 l3->setSpacing(0);
 panel3->setStyleSheet(gbStyleSheet);

 QWidget* panel31 = new QWidget();
 panel31->setLayout(new QHBoxLayout);
 panel31->layout()->addWidget(clockStatus);
 panel3->layout()->addWidget(panel31);
 QWidget* panel32 = new QWidget();
 panel32->setLayout(new QHBoxLayout);
 panel32->layout()->addWidget(new JLabel(tr("Current Fast Time:")+" "));
 setTimeLabel();
 panel32->layout()->addWidget(timeLabel);
 panel3->layout()->addWidget(panel32);
//    Border panel3Border = BorderFactory.createEtchedBorder();
//    Border panel3Titled = BorderFactory.createTitledBorder(panel3Border,
//                                            tr("BoxLabelClockState"));
//    panel3.setBorder(panel3Titled);
 panel3->setTitle( tr("Clock State"));
 l->addWidget(panel3);

 // Set up Start and Stop buttons
 QWidget* panel4 = new QWidget();
 panel4->setLayout(new QHBoxLayout);
 QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
 sizePolicy.setHorizontalStretch(0);
 sizePolicy.setVerticalStretch(0);
 sizePolicy.setHeightForWidth(startButton->sizePolicy().hasWidthForHeight());
 startButton->setSizePolicy(sizePolicy);
 startButton->setToolTip(tr("Click to start Fast Clock."));
 stopButton->setSizePolicy(sizePolicy);
//    startButton.addActionListener(new java.awt.event.ActionListener() {
//            /*public*/ void actionPerformed(java.awt.event.ActionEvent e) {
//                startButtonActionPerformed();
//            }
//        });
 connect(startButton, SIGNAL(clicked()), this, SLOT(startButtonActionPerformed()));
 panel4->layout()->addWidget(startButton);
 stopButton->setToolTip(tr("Click to stop Fast Clock."));
//    stopButton.addActionListener(new java.awt.event.ActionListener() {
//            /*public*/ void actionPerformed(java.awt.event.ActionEvent e) {
//                stopButtonActionPerformed();
//            }
//        });
 connect(stopButton, SIGNAL(clicked()), this, SLOT(stopButtonActionPerformed()));
 panel4->layout()->addWidget(stopButton);
 l->addWidget(panel4);

 // update contents for current status
 updateRunningButton();

 // add help menu to window
 addHelpMenu("package.jmri.jmrit.simpleclock.SimpleClockFrame", true);

 // pack for display
 pack();

 // listen for changes to the timebase parameters
 clock->addPropertyChangeListener((PropertyChangeListener*)this);
// if(!connect(clock, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*))))
//     qDebug() << "connect failed";

 // request callback to update time
//    clock.addMinuteChangeListener( new java.beans.PropertyChangeListener() {
//                /*public*/ void propertyChange(java.beans.PropertyChangeEvent e) {
//                    updateTime();
//                }
//            });
 if(!connect(clock, SIGNAL(minuteTick()), this, SLOT(updateTime())))
  qDebug() << "connect failed";
 return;
}

/*private*/ void SimpleClockFrame::startFactorFieldChanged() {
    double v = startFactorField->text().toDouble();
    if (v != 0 && !v == (clock->getStartRate())) {
        clock->setStartRate(v);
        changed = true;
    }
    startFactorField->setText(threeDigits->format(clock->getStartRate()));
}

/*private*/ void SimpleClockFrame::startSetRateChanged() {
    clock->setSetRateAtStart(startSetRateCheckBox->isChecked());
    changed = true;
}

/**
 * Adjust to rate changes.
 */
void SimpleClockFrame::updateRate() {
    factorField->setText(threeDigits->format(clock->userGetRate()));
}

/**
 * Method to adjust to running state changes
 */
void SimpleClockFrame::updateRunningButton()
{
 bool running = clock->getRun();
 if (running)
 {
  clockStatus->setText(tr("Fast Clock is running."));
  startButton->setVisible(false);
  stopButton->setVisible(true);
 }
 else
 {
  clockStatus->setText(tr("Fast Clock is stopped."));
  startButton->setVisible(true);
  stopButton->setVisible(false);
 }
 clockStatus->setVisible(true);
}

/**
 * Method to handle Set Rate button
 */
/*public*/ void SimpleClockFrame::setRateButtonActionPerformed() // [slot]
{
    double rate = 1.0;
    bool bOk;
//    try {
    rate = factorField->text().toDouble(&bOk);
//    }
//    catch (Exception e) {
    if(!bOk)
    {
//        JOptionPane.showMessageDialog(this,(tr("ParseRateError")+"\n"+e),
//                tr("ErrorTitle"),JOptionPane.ERROR_MESSAGE);
        QMessageBox::critical(this, tr("Error"), tr("Error in Fast Clock Rate entry:"));
        log->error("Exception when parsing Rate Field: "/*+e*/);
        return;
    }
    if (rate < 0.0) {
//        JOptionPane.showMessageDialog(this,tr("NegativeRateError"),
//                tr("ErrorTitle"),JOptionPane.ERROR_MESSAGE);
        QMessageBox::critical(this, tr("Error"), tr("Error in entry - Rate cannot be negative."));
        factorField->setText(threeDigits->format(clock->userGetRate()));
        return;
    }
    if (((ClockControl*)InstanceManager::getDefault("ClockControl"))->requiresIntegerRate()) {
        double frac = rate-(int)rate;
        if (frac > 0.001) {
//            JOptionPane.showMessageDialog(this,tr("NonIntegerError"),
//                tr("ErrorTitle"),JOptionPane.ERROR_MESSAGE);
            QMessageBox::critical(this, tr("Error"), tr("Error in entry - Your hardware clock requires an integer rate."));
            factorField->setText(threeDigits->format(clock->userGetRate()));
            return;
        }
    }
//    try {
        clock->userSetRate(rate);
//    }
//    catch (Exception e) {
//        JOptionPane.showMessageDialog(this,(tr("SetRateError")+"\n"+e),
//                tr("ErrorTitle"),JOptionPane.ERROR_MESSAGE);
//        log->error("Exception when setting timebase rate: "+e);
//    }
    changed = true;
}
/**
 * Handle start run combo box change
 */
/*private*/ void SimpleClockFrame::startRunBoxChanged() {
    switch (startRunBox->currentIndex()) {
        case START_STOPPED:
            clock->setClockInitialRunState(ClockInitialRunState::DO_STOP);
            break;
        case START_RUNNING:
            clock->setClockInitialRunState(ClockInitialRunState::DO_START);
            break;
        default:
        case START_NORUNCHANGE:
            clock->setClockInitialRunState(ClockInitialRunState::DO_NOTHING);
            break;
    }
    changed = true;
}

/**
 * Handle Show on/off button check box change
 */
/*private*/ void SimpleClockFrame::showStopButtonChanged() {
    clock->setShowStopButton(displayStartStopButton->isChecked());
    changed = true;
}
/**
 * Method to handle time source change
 */
/*private*/ void SimpleClockFrame::setTimeSourceChanged() // SLOT[]
{
    int index = timeSourceBox->currentIndex();
    int oldIndex = internalSourceIndex;
    if (!clock->getInternalMaster()) oldIndex = hardwareSourceIndex;
    // return if nothing changed
    if ( oldIndex == index ) return;
    // change the time source master
    if (index == internalSourceIndex) clock->setInternalMaster(true,true);
    else clock->setInternalMaster(false,true);
    changed = true;
}

/**
 * Method to handle synchronize check box change
 */
/*private*/ void SimpleClockFrame::synchronizeChanged() // SLOT[]
{
    clock->setSynchronize(synchronizeCheckBox->isChecked(),true);
    changed = true;
}

/**
 * Method to handle correct check box change
 */
/*private*/ void SimpleClockFrame::correctChanged()  /// SLOT[]
{
    clock->setCorrectHardware(correctCheckBox->isChecked(),true);
    changed = true;
}

/**
 * Method to handle 12-hour display check box change
 */
/*private*/ void SimpleClockFrame::displayChanged() {
    clock->set12HourDisplay(displayCheckBox->isChecked(),true);
    changed = true;
}

/**
 * Method to handle Set Time button
 */
//@SuppressWarnings("deprecation")
/*public*/ void SimpleClockFrame::setTimeButtonActionPerformed()
{
 int hours = 0;
 int minutes = 0;
 // get hours, reporting errors if any
//    try {
 bool bOk;
 hours = hoursField->text().toInt(&bOk);
//    }
//    catch (Exception e) {
 if(!bOk)
 {
//        JOptionPane.showMessageDialog(this,(tr("HoursError")+"\n"+e),
//                tr("ErrorTitle"),JOptionPane.ERROR_MESSAGE);
  QMessageBox::critical(this, tr("Error"), tr("Error in Fast Time hours entry:"));
  log->error("Exception when parsing hours Field: "/*+e*/);
  return;
 }
 if ( (hours<0) || (hours>23) )
 {
//        JOptionPane.showMessageDialog(this,(tr("HoursRangeError")),
//                tr("ErrorTitle"),JOptionPane.ERROR_MESSAGE);
  QMessageBox::critical(this, tr("Error"), tr("Hours entry not within allowed 0 - 23 range."));
  return;
 }
 // get minutes, reporting errors if any
//    try {
 minutes = minutesField->text().toInt(&bOk);
//    }
//    catch (Exception e) {
 if(!bOk)
 {
//        JOptionPane.showMessageDialog(this,(tr("HoursError")+"\n"+e),
//                tr("ErrorTitle"),JOptionPane.ERROR_MESSAGE);
  QMessageBox::critical(this, tr("Error"), tr("Error in Fast Time hours entry:"));
  log->error("Exception when parsing hours Field: "/*+e*/);
  return;
 }
 if ( (minutes<0) || (minutes>59) )
 {
//        JOptionPane.showMessageDialog(this,(tr("MinutesRangeError")),
//                tr("ErrorTitle"),JOptionPane.ERROR_MESSAGE);
  QMessageBox::critical(this, tr("Error"), tr("Minutes entry not within allowed 0 - 59 range."));
  return;
 }
 // set time of the fast clock
//    long mSecPerHour = 3600000;
//    long mSecPerMinute = 60000;
 QDateTime tem = clock->getTime();
//    int cHours = tem.time().hour();
//    long cNumMSec = tem.time().msec();
//    long nNumMSec = ((cNumMSec/mSecPerHour)*mSecPerHour) - (cHours*mSecPerHour) +
//                (hours*mSecPerHour) + (minutes*mSecPerMinute);
//TODO:    clock->userSetTime(QDateTime(nNumMSec));
 tem.setTime(QTime(hours,minutes,0));
 clock->setTime(tem);
 showTime = true;
 updateTime();
}

/**
 * Method to handle start stopped check box change
 */
/*private*/ void SimpleClockFrame::startStoppedChanged() // SLOT[]
{
 clock->setStartStopped(startStoppedCheckBox->isChecked());
 changed = true;
}

/**
 * Method to handle start set time check box change
 */
//@SuppressWarnings("deprecation")
/*private*/ void SimpleClockFrame::startSetTimeChanged()
{
 int hours = 0;
 int minutes = 0;
 // get hours, reporting errors if any
// try {
 bool bOk;
 hours = startHoursField->text().toInt(&bOk);
// }
// catch (Exception e) {
 if(!bOk)
 {
//        JOptionPane.showMessageDialog(this,(tr("HoursError")+"\n"+e),
//                tr("ErrorTitle"),JOptionPane.ERROR_MESSAGE);
  QMessageBox::critical(this, tr("Error"), tr("Error in Fast Time hours entry:"));
  log->error("Exception when parsing hours Field: "/*+e*/);
  return;
 }
 if ( (hours<0) || (hours>23) )
 {
//        JOptionPane.showMessageDialog(this,(tr("HoursRangeError")),
//                tr("ErrorTitle"),JOptionPane.ERROR_MESSAGE);
  QMessageBox::critical(this, tr("Error"), tr("Hours entry not within allowed 0 - 23 range."));
  return;
 }
 // get minutes, reporting errors if any
//    try {
 minutes = startMinutesField->text().toInt(&bOk);
//    }
//    catch (Exception e) {
 if(!bOk)
 {
//        JOptionPane.showMessageDialog(this,(tr("HoursError")+"\n"+e),
//                tr("ErrorTitle"),JOptionPane.ERROR_MESSAGE);
  QMessageBox::critical(this, tr("Error"), tr("Error in Fast Time hours entry:"));
  log->error("Exception when parsing hours Field: "/*+e*/);
  return;
 }
 if ( (minutes<0) || (minutes>59) )
 {
//        JOptionPane.showMessageDialog(this,(tr("MinutesRangeError")),
//                tr("ErrorTitle"),JOptionPane.ERROR_MESSAGE);
  QMessageBox::critical(this, tr("Error"), tr("Minutes entry not within allowed 0 - 59 range."));
  return;
 }
 // set time of the fast clock
//    long mSecPerHour = 3600000;
//    long mSecPerMinute = 60000;
 QDateTime tem = clock->getTime();
//    int cHours = tem.time().hour();
//    long cNumMSec = tem.time().msec();
//    long nNumMSec = ((cNumMSec/mSecPerHour)*mSecPerHour) - (cHours*mSecPerHour) +
//                (hours*mSecPerHour) + (minutes*mSecPerMinute);
//// TODO:    clock->setStartSetTime(startSetTimeCheckBox->isChecked(),new QDateTime(nNumMSec));
 tem.setTime(QTime(hours,minutes,0));
 clock->setTime(tem);
 changed = true;
}

/**
 * Method to handle start clock combo box change
 */
/*private*/ void SimpleClockFrame::setClockStartChanged()
{
 int sel = Timebase::NONE;
 if (clockStartBox->currentIndex()==startNixieClock)
  sel = Timebase::NIXIE_CLOCK;
 else if (clockStartBox->currentIndex()==startAnalogClock)
  sel = Timebase::ANALOG_CLOCK;
 else if (clockStartBox->currentIndex()==startLcdClock)
  sel = Timebase::LCD_CLOCK;
 clock->setStartClockOption(sel);
  changed = true;
}

/**
 * Method to handle Start Clock button
 */
/*public*/ void SimpleClockFrame::startButtonActionPerformed()
{
 clock->setRun(true);
}

/**
 * Method to handle Stop Clock button
 */
/*public*/ void SimpleClockFrame::stopButtonActionPerformed()
{
 clock->setRun(false);
}

/**
 * Method to update clock state information
 */
void SimpleClockFrame::updateTime()  // [slot]
{
 if (clock->getRun() || showTime)
 {
  showTime = false;
  setTimeLabel();
  timeLabel->setVisible(true);
 }
}

/**
 * Method to set the current Timebase time into timeLabel
 */
//@SuppressWarnings("deprecation")
void SimpleClockFrame::setTimeLabel()
{
 // Get time
 QDateTime now = clock->getTime();
 int hours = now.time().hour();
 int minutes = now.time().minute();
 // Format and display the time
 timeLabel->setText(QString::number((hours/10))+QString::number((hours-(hours/10)*10))+":"+QString::number((minutes/10))+QString::number((minutes-(minutes/10)*10)));
}

/**
 * Handle a change to clock properties
 */
/*public*/ void SimpleClockFrame::propertyChange(PropertyChangeEvent* /*e*/)  // [slot]
{
 updateRunningButton();
 updateRate();
}

/**
 * Method to handle window closing event
 */
///*public*/ void windowClosing(java.awt.event.WindowEvent e) {
void SimpleClockFrame::closeEvent(QCloseEvent * e
                                  )
{
 if (changed)
 {
  // remind to save
//  javax.swing.JOptionPane.showMessageDialog(nullptr,
//                tr("Reminder1")+"\n"+tr("Reminder2"),
//                    tr("ReminderTitle"),
//                        javax.swing.JOptionPane.INFORMATION_MESSAGE);
  QMessageBox::information(nullptr, tr("Reminder"),tr("Please remember to save your changes to disk.")+"\n"+tr("( Select 'Store panels...' in the Panels menu. )") );
  changed = false;
 }
 setVisible(false);
 JmriJFrame::windowClosing(e);
}
/*public*/ QString SimpleClockFrame::getClassName()
{
 return "jmri.jmrit.simpleclock.SimpleClockFrame";
}
