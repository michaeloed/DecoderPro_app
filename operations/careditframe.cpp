#include "careditframe.h"
#include "carmanager.h"
#include "carmanagerxml.h"
#include "car.h"
#include "carload.h"
#include "carloads.h"
#include "locationmanager.h"
#include "location.h"
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include "jtextfield.h"
#include "xml.h"
#include "control.h"
#include "carroads.h"
#include "cartypes.h"
#include <QBoxLayout>
#include <QGroupBox>
#include "jcombobox.h"
#include <QScrollArea>
#include "logger.h"
#include "rosterentry.h"
#include "track.h"
#include "setup.h"
#include "instancemanager.h"
#include "idtagmanager.h"
#include "carattributeeditframe.h"
#include <QMessageBox>
#include "kernel.h"
#include "gridbaglayout.h"
#include "carlengths.h"
#include "carcolors.h"
#include "carowners.h"
#include "rollingstockattribute.h"
#include "carloadeditframe.h"

//CarEditFrame::CarEditFrame()
//{

//}
namespace Operations
{
/**
 * Frame for user edit of car
 *
 * @author Dan Boudreau Copyright (C) 2008, 2010, 2011, 2014
 * @version $Revision: 29493 $
 */
///*public*/ class CarEditFrame extends OperationsFrame implements java.beans.PropertyChangeListener {



 /*public*/ /*static*/ /*final*/ QString CarEditFrame::ROAD = tr("Road");
 /*public*/ /*static*/ /*final*/ QString CarEditFrame::TYPE = tr("Type");
 /*public*/ /*static*/ /*final*/ QString CarEditFrame::COLOR = tr("Color");
 /*public*/ /*static*/ /*final*/ QString CarEditFrame::LENGTH = tr("Length");
 /*public*/ /*static*/ /*final*/ QString CarEditFrame::OWNER = tr("Owner");
 /*public*/ /*static*/ /*final*/ QString CarEditFrame::KERNEL = tr("Kernel");

 /*public*/ CarEditFrame::CarEditFrame(QWidget* parent) : OperationsFrame(parent)
 {
     //super();
  setObjectName("CarEditFrame");
  carManager = CarManager::instance();
  managerXml = CarManagerXml::instance();
  locationManager = LocationManager::instance();
  log = new Logger("CarEditFrame");

  textWeightOz = new QLabel(tr("Road"));
  textWeightTons = new QLabel(tr("Tons"));

  // major buttons
  editRoadButton = new QPushButton(tr("Edit"));
  clearRoadNumberButton = new QPushButton(tr("Clear"));
  editTypeButton = new QPushButton(tr("Edit"));
  editColorButton = new QPushButton(tr("Edit"));
  editLengthButton = new QPushButton(tr("Edit"));
  fillWeightButton = new QPushButton(tr("Calculate"));
  editLoadButton = new QPushButton(tr("Edit"));
  editKernelButton = new QPushButton(tr("Edit"));
  editOwnerButton = new QPushButton(tr("Edit"));

  saveButton = new QPushButton(tr("Save"));
  deleteButton = new QPushButton(tr("Delete"));
  addButton = new QPushButton(tr("Add"));

  // check boxes
  autoCheckBox = new QCheckBox(tr("Auto"));
  autoTrackCheckBox = new QCheckBox(tr("Auto"));
  passengerCheckBox = new QCheckBox(tr("Passenger"));
  cabooseCheckBox = new QCheckBox(tr("Caboose"));
  fredCheckBox = new QCheckBox(tr("Fred"));
  utilityCheckBox = new QCheckBox(tr("Utility"));
  hazardousCheckBox = new QCheckBox(tr("Hazardous"));

  // text field
  roadNumberTextField = new JTextField(Control::max_len_string_road_number);
  blockingTextField = new JTextField(4);
  builtTextField = new JTextField(Control::max_len_string_built_name + 3);
  weightTextField = new JTextField(Control::max_len_string_weight_name);
  weightTonsTextField = new JTextField(Control::max_len_string_weight_name);
  commentTextField = new JTextField(35);
  valueTextField = new JTextField(8);

  // combo boxes
  roadComboBox = CarRoads::instance()->getComboBox();
  typeComboBox = CarTypes::instance()->getComboBox();
  colorComboBox = CarColors::instance()->getComboBox();
  lengthComboBox = CarLengths::instance()->getComboBox();
  ownerComboBox = new JComboBox(); //.instance().getComboBox();
  locationBox = locationManager->getComboBox();
  trackLocationBox = new JComboBox();
  loadComboBox = CarLoads::instance()->getComboBox();
  kernelComboBox = carManager->getKernelComboBox();
  rfidComboBox = new JComboBox();

  // panels
  pBlocking = new QGroupBox();

  lef = NULL;
  editActive = false;
  f = NULL;
  _car = NULL;

  editButtonMapper = new QSignalMapper();
  connect(editButtonMapper, SIGNAL(mapped(QWidget*)), this, SLOT(buttonEditActionPerformed(QWidget*)));
 }

 /*public*/ void CarEditFrame::initComponents()
 {
  // the following code sets the frame's initial state

  // load tool tips
  weightTextField->setToolTip(tr("Enter car's weight in oz"));
  weightTextField->setValidator(new QDoubleValidator(0,99,1));
  weightTonsTextField->setToolTip(tr("Enter car's weight in tons"));
  autoCheckBox->setToolTip(tr("Automatically calculate car's weight based on length"));
  autoTrackCheckBox->setToolTip(tr("When checked, show only available tracks for this car"));
  passengerCheckBox->setToolTip(tr("Place car towards the rear of the train"));
  cabooseCheckBox->setToolTip(tr("Place car at the end of the train"));
  fredCheckBox->setToolTip(tr("Car has Flashing Rear End Device (FRED)"));
  utilityCheckBox->setToolTip(tr("Utility car, print quantity in train and not car's road and number"));
  hazardousCheckBox->setToolTip(tr("Add hazardous warning to manifest"));
  blockingTextField->setToolTip(tr("Enter a number between 0 and 100. Low numbers place car towards the front of the train, high numbers towards the rear."));
  fillWeightButton->setToolTip(tr("Calculate car weight based on NMRA recommendations for scale and car length"));
  builtTextField->setToolTip(tr("Enter when the car was built, use 4 digits yyyy or mm-yy"));
  valueTextField->setToolTip(tr("Enter car's value (cost)"));

  editRoadButton->setToolTip(tr("Add, delete or replace %1").arg(tr("Road").toLower()));
  editTypeButton->setToolTip(tr("Add, delete or replace %1").arg(tr("Type").toLower()));
  editColorButton->setToolTip(tr("Add, delete or replace %1").arg(tr("Color").toLower()));
  editLengthButton->setToolTip(tr("Add, delete or replace %1").arg(tr("Length").toLower()));
  editLoadButton->setToolTip(tr("Add, delete or replace %1").arg(tr("Load").toLower()));
  editOwnerButton->setToolTip(tr("Add, delete or replace %1").arg(tr("Owner").toLower()));
  editKernelButton->setToolTip(tr("Add, delete or replace %1").arg(tr("Kernel").toLower()));

  // default check box selections
  autoCheckBox->setChecked(true);
  passengerCheckBox->setChecked(false);
  cabooseCheckBox->setChecked(false);
  fredCheckBox->setChecked(false);
  hazardousCheckBox->setChecked(false);

  // create panel
  QWidget* pPanel = new QWidget();
  //pPanel.setLayout(new BoxLayout(pPanel, BoxLayout.Y_AXIS));
  QVBoxLayout* pPanelLayout = new QVBoxLayout(pPanel);

  // Layout the panel by rows
  // row 1
  QGroupBox* pRoad = new QGroupBox();
  //pRoad.setLayout(new GridBagLayout());
  //pRoad.setBorder(BorderFactory.createTitledBorder(tr("Road")));
  pRoad->setStyleSheet(gbStyleSheet);
  pRoad->setTitle(tr("Road"));
  addItem(pRoad, roadComboBox, 1, 0);
  addItem(pRoad, editRoadButton, 2, 0);
  pPanelLayout->addWidget(pRoad);

  // row 2
  QGroupBox* pRoadNumber = new QGroupBox();
  //pRoadNumber.setLayout(new GridBagLayout());
  //pRoadNumber.setBorder(BorderFactory.createTitledBorder(tr("RoadNumber")));
  pRoadNumber->setStyleSheet(gbStyleSheet);
  pRoadNumber->setTitle(tr("Road Number"));
  addItem(pRoadNumber, roadNumberTextField, 1, 0);
  addItem(pRoadNumber, clearRoadNumberButton, 2, 0);
  pPanelLayout->addWidget(pRoadNumber);

  // row 3
  QGroupBox* pType = new QGroupBox();
  //pType.setLayout(new GridBagLayout());
  //pType.setBorder(BorderFactory.createTitledBorder(tr("Type")));
  pType->setStyleSheet(gbStyleSheet);
  pType->setTitle(tr("Type"));
  addItem(pType, typeComboBox, 0, 0);
  addItem(pType, editTypeButton, 2, 0);
  addItem(pType, hazardousCheckBox, 3, 0);
  addItem(pType, passengerCheckBox, 0, 1);
  addItem(pType, cabooseCheckBox, 1, 1);
  addItem(pType, fredCheckBox, 2, 1);
  addItem(pType, utilityCheckBox, 3, 1);
  pPanelLayout->addWidget(pType);

  // row 3a
  //pBlocking.setLayout(new GridBagLayout());
  //pBlocking.setBorder(BorderFactory.createTitledBorder(tr("BorderLayoutPassengerBlocking")));
  pBlocking->setStyleSheet(gbStyleSheet);
  pBlocking->setTitle(tr("Passenger Car Blocking Order"));
  addItem(pBlocking, blockingTextField, 0, 0);
  blockingTextField->setText("0");
  pPanelLayout->addWidget(pBlocking);
  pBlocking->setVisible(false);

  // row 4
  QGroupBox* pLength = new QGroupBox();
  //pLength.setLayout(new GridBagLayout());
  //pLength.setBorder(BorderFactory.createTitledBorder(tr("Length")));
  pLength->setStyleSheet(gbStyleSheet);
  pLength->setTitle("Length");
  addItem(pLength, lengthComboBox, 1, 0);
  addItem(pLength, editLengthButton, 2, 0);
  pPanelLayout->addWidget(pLength);

  // row 5
  // row 7
  QGroupBox* pWeight = new QGroupBox();
  //pWeight.setLayout(new GridBagLayout());
  //pWeight.setBorder(BorderFactory.createTitledBorder(tr("Weight")));
  pWeight->setStyleSheet(gbStyleSheet);
  pWeight->setTitle("Weight");
  addItem(pWeight, textWeightOz, 0, 0);
  addItem(pWeight, weightTextField, 1, 0);
  addItem(pWeight, fillWeightButton, 2, 0);
  addItem(pWeight, autoCheckBox, 3, 0);
  addItem(pWeight, textWeightTons, 0, 1);
  addItem(pWeight, weightTonsTextField, 1, 1);
  pPanelLayout->addWidget(pWeight);

  // row 11
  QGroupBox* pLocation = new QGroupBox();
//     pLocation.setLayout(new GridBagLayout());
//     pLocation.setBorder(BorderFactory.createTitledBorder(tr("LocationAndTrack")));
  pLocation->setStyleSheet(gbStyleSheet);
  pLocation->setTitle(tr("Location and Track"));
  addItem(pLocation, locationBox, 1, 0);
  addItem(pLocation, trackLocationBox, 2, 0);
  addItem(pLocation, autoTrackCheckBox, 3, 0);
  pPanelLayout->addWidget(pLocation);

  // optional panel
  QGroupBox* pOptional = new QGroupBox();
  pOptional->setStyleSheet(gbStyleSheet);
  pOptional->setTitle("Optional");
  QVBoxLayout* pOptionalLayout1 = new QVBoxLayout(pOptional);
  //pOptional.setLayout(new BoxLayout(pOptional, BoxLayout.Y_AXIS));
  QScrollArea* optionPane = new QScrollArea(/*pOptional*/);
  pOptionalLayout1->addWidget(optionPane);
  QWidget* scrollWidget = new QWidget;
  QVBoxLayout* pOptionalLayout = new QVBoxLayout(scrollWidget);
  optionPane->setWidget(scrollWidget);
  optionPane->setWidgetResizable(true);

//   optionPane.setBorder(BorderFactory.createTitledBorder(tr("BorderLayoutOptional")));

  // row 12
  QGroupBox* pColor = new QGroupBox();
  pColor->setStyleSheet(gbStyleSheet);
  //pColor.setLayout(new GridBagLayout());
  //pColor.setBorder(BorderFactory.createTitledBorder(tr("Color")));
  pColor->setStyleSheet(gbStyleSheet);
  pColor->setTitle(tr("Color"));
  addItem(pColor, colorComboBox, 1, 0);
  addItem(pColor, editColorButton, 2, 0);
  pOptionalLayout->addWidget(pColor);

  // row 13
  QGroupBox* pLoad = new QGroupBox();
//     pLoad.setLayout(new GridBagLayout());
//     pLoad.setBorder(BorderFactory.createTitledBorder(tr("Load")));
  pLoad->setStyleSheet(gbStyleSheet);
  pLoad->setTitle(tr("Load"));
  pLoad->setLayout(new GridBagLayout);
  addItem(pLoad, loadComboBox, 1, 0);
  addItem(pLoad, editLoadButton, 2, 0);
  pOptionalLayout->addWidget(pLoad);

  // row 15
  QGroupBox* pKernel = new QGroupBox();
//     pKernel.setLayout(new GridBagLayout());
//     pKernel.setBorder(BorderFactory.createTitledBorder(tr("Kernel")));
  pKernel->setStyleSheet(gbStyleSheet);
  pKernel->setTitle(tr("Kernel"));
  addItem(pKernel, kernelComboBox, 1, 0);
  addItem(pKernel, editKernelButton, 2, 0);
  pOptionalLayout->addWidget(pKernel);

  // row 17
  QGroupBox* pBuilt = new QGroupBox();
//     pBuilt.setLayout(new GridBagLayout());
//     pBuilt.setBorder(BorderFactory.createTitledBorder(tr("Built")));
  pBuilt->setStyleSheet(gbStyleSheet);
  pBuilt->setTitle(tr("Built"));
  addItem(pBuilt, builtTextField, 1, 0);
  pOptionalLayout->addWidget(pBuilt);

  // row 19
  QGroupBox* pOwner = new QGroupBox();
//     pOwner.setLayout(new GridBagLayout());
//     pOwner.setBorder(BorderFactory.createTitledBorder(tr("Owner")));
  pOwner->setStyleSheet(gbStyleSheet);
  pOwner->setTitle(tr("Owner"));
  addItem(pOwner, ownerComboBox, 1, 0);
  addItem(pOwner, editOwnerButton, 2, 0);
  pOptionalLayout->addWidget(pOwner);

  // row 20
  if (Setup::isValueEnabled())
  {
   QGroupBox* pValue = new QGroupBox();
//         pValue.setLayout(new GridBagLayout());
//         pValue.setBorder(BorderFactory.createTitledBorder(Setup::getValueLabel()));
   pValue->setStyleSheet(gbStyleSheet);
   pValue->setTitle(Setup::getValueLabel());
   addItem(pValue, valueTextField, 1, 0);
   pOptionalLayout->addWidget(pValue);
  }

  // row 22
  if (Setup::isRfidEnabled())
  {
   QGroupBox* pRfid = new QGroupBox();
//         pRfid.setLayout(new GridBagLayout());
//         pRfid.setBorder(BorderFactory.createTitledBorder(Setup::getRfidLabel()));
   pRfid->setStyleSheet(gbStyleSheet);
   pRfid->setTitle(Setup::getRfidLabel());
   addItem(pRfid, rfidComboBox, 1, 0);
   rfidComboBox->addItem("", VPtr<IdTag>::asQVariant(NULL));
   //InstanceManager::getDefault("IdTagManager").getNamedBeanList().forEach((tag) -> rfidComboBox.addItem((IdTag) tag));
   QList<NamedBean*>* list = ((IdTagManager*)InstanceManager::getDefault("IdTagManager"))->getNamedBeanList();
   foreach (NamedBean* tag, *list)
   {
    rfidComboBox->addItem(tag->getDisplayName(), VPtr<IdTag>::asQVariant((IdTag*)tag));
   }
   pOptionalLayout->addWidget(pRfid);
  }

  // row 24
  QGroupBox* pComment = new QGroupBox();
//     pComment.setLayout(new GridBagLayout());
//     pComment.setBorder(BorderFactory.createTitledBorder(tr("Comment")));
  pComment->setStyleSheet(gbStyleSheet);
  pComment->setTitle(tr("Comment"));
  addItem(pComment, commentTextField, 1, 0);
  pOptionalLayout->addWidget(pComment);

  // button panel
  QWidget* pButtons = new QWidget();
  //pButtons.setLayout(new GridBagLayout());
  addItem(pButtons, deleteButton, 0, 25);
  addItem(pButtons, addButton, 1, 25);
  addItem(pButtons, saveButton, 3, 25);

  // add panels
  //getContentPane().setLayout(new BoxLayout(getContentPane(), BoxLayout.Y_AXIS));
  QVBoxLayout* thisLayout = new QVBoxLayout(getContentPane());
  thisLayout->addWidget(pPanel);
  thisLayout->addWidget(pOptional);
  thisLayout->addWidget(pButtons);

  // setup buttons
  addEditButtonAction(editRoadButton);
  addButtonAction(clearRoadNumberButton);
  addEditButtonAction(editTypeButton);
  addEditButtonAction(editLengthButton);
  addEditButtonAction(editColorButton);
  addEditButtonAction(editKernelButton);
  addEditButtonAction(editOwnerButton);

  addButtonAction(deleteButton);
  addButtonAction(addButton);
  addButtonAction(saveButton);
  addButtonAction(fillWeightButton);
  addButtonAction(editLoadButton);

  // setup combobox
  addComboBoxAction(typeComboBox);
  addComboBoxAction(lengthComboBox);
  addComboBoxAction(locationBox);

  // setup checkbox
  addCheckBoxAction(cabooseCheckBox);
  addCheckBoxAction(fredCheckBox);
  addCheckBoxAction(passengerCheckBox);
  addCheckBoxAction(autoTrackCheckBox);
  autoTrackCheckBox->setEnabled(false);

  // build menu
  // JMenuBar menuBar = new JMenuBar();
  // JMenu toolMenu = new JMenu("Tools");
  // menuBar.add(toolMenu);
  // setJMenuBar(menuBar);
  addHelpMenu("package.jmri.jmrit.operations.Operations_CarsEdit", true); // NOI18N

  // get notified if combo box gets modified
  //CarRoads::instance().addPropertyChangeListener(this);
  connect(CarRoads::instance()->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));
  //CarLoads::instance().addPropertyChangeListener(this);
  connect(CarLoads::instance()->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));
  //CarTypes.instance().addPropertyChangeListener(this);
  connect(CarTypes::instance()->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));
//     CarLengths.instance().addPropertyChangeListener(this);
  connect(CarLengths::instance()->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));
//     CarColors.instance().addPropertyChangeListener(this);
  connect(CarColors::instance()->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));
//     CarOwners.instance().addPropertyChangeListener(this);
  connect(CarOwners::instance()->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));
  //locationManager.addPropertyChangeListener(this);
  connect(locationManager->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));
  //carManager.addPropertyChangeListener(this);
  connect(carManager->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));

  adjustSize();
  setMinimumSize(QSize(Control::panelWidth500, Control::panelHeight500));
  setVisible(true);
 }

 /*public*/ void CarEditFrame::loadCar(Car* car) {
     _car = car;

     if (!CarRoads::instance()->containsName(car->getRoadName())) {
//         if (JOptionPane.showConfirmDialog(this, tr("roadNameNotExist"),
//                 new Object[]{car.getRoadName()}), tr("carAddRoad"), JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) {
      if(QMessageBox::question(this, tr("Add road name?"), tr("Road name \"%1\" does not exist in your roster, add?").arg(car->getRoadName()), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
      {
             CarRoads::instance()->addName(car->getRoadName());
         }
     }
     roadComboBox->setCurrentIndex(roadComboBox->findText(car->getRoadName()));

     roadNumberTextField->setText(car->getNumber());

     if (!CarTypes::instance()->containsName(car->getTypeName())) {
//         if (JOptionPane.showConfirmDialog(this, tr("typeNameNotExist"),
//                 new Object[]{car.getTypeName()}), tr("carAddType"), JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) {
      if(QMessageBox::question(this, tr("Add car type?"), tr("Type \"%1\" does not exist in your roster, add?").arg(car->getTypeName()), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
      {
             CarTypes::instance()->addName(car->getTypeName());
         }
     }
     typeComboBox->setCurrentIndex(typeComboBox->findText(car->getTypeName()));

     if (!CarLengths::instance()->containsName(car->getLength())) {
//         if (JOptionPane.showConfirmDialog(this, tr("lengthNameNotExist"),
//                 new Object[]{car.getLength()}), tr("carAddLength"), JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) {
      if(QMessageBox::question(this, tr("Add car length?"), tr("Length \"%1\" does not exist in your roster, add?").arg(car->getLength()), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
      {
             CarLengths::instance()->addName(car->getLength());
         }
     }
     lengthComboBox->setCurrentIndex(lengthComboBox->findText(car->getLength()));

     if (!CarColors::instance()->containsName(car->getColor())) {
//         if (JOptionPane.showConfirmDialog(this, tr("colorNameNotExist"),
//                 new Object[]{car.getColor()}), tr("carAddColor"), JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) {
      if(QMessageBox::question(this, tr("Add car color?"), tr("Color \"%1\" does not exist in your roster, add?").arg(car->getColor()), QMessageBox::Yes | QMessageBox::No)== QMessageBox::Yes)
      {
             CarColors::instance()->addName(car->getColor());
         }
     }

     colorComboBox->setCurrentIndex(colorComboBox->findText(car->getColor()));
     weightTextField->setText(car->getWeight());
     weightTonsTextField->setText(car->getWeightTons());
     passengerCheckBox->setChecked(car->isPassenger());
     cabooseCheckBox->setChecked(car->isCaboose());
     utilityCheckBox->setChecked(car->isUtility());
     utilityCheckBox->setChecked(car->isUtility());
     fredCheckBox->setChecked(car->hasFred());
     hazardousCheckBox->setChecked(car->isHazardous());

     pBlocking->setVisible(passengerCheckBox->isChecked());

     locationBox->setCurrentIndex(locationBox->findData(VPtr<Location>::asQVariant(car->getLocation())));
     updateTrackLocationBox();

     builtTextField->setText(car->getBuilt());

     if (!CarOwners::instance()->containsName(car->getOwner())) {
//         if (JOptionPane.showConfirmDialog(this, tr("ownerNameNotExist"),
//                 new Object[]{car.getOwner()}), tr("addOwner"), JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) {
      if(QMessageBox::question(this, tr("Add owner?"), tr("Owner \"%1\" does not exist in your roster, add?").arg(car->getOwner()), QMessageBox::Yes | QMessageBox::No)== QMessageBox::Yes)
      {
       CarOwners::instance()->addName(car->getOwner());
      }
     }
     ownerComboBox->setCurrentIndex(ownerComboBox->findText(car->getOwner()));

     if (!CarLoads::instance()->containsName(car->getTypeName(), car->getLoadName())) {
//         if (JOptionPane.showConfirmDialog(this, tr("loadNameNotExist"),
//                 new Object[]{car.getLoadName()}), tr("addLoad"), JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) {
      if(QMessageBox::question(this, tr("Add load?"), tr("Load \"%1\" does not exist in your roster, add?").arg(car->getLoadName()), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
      {
             CarLoads::instance()->addName(car->getTypeName(), car->getLoadName());
         }
     }
     // listen for changes in car load
     //car.addPropertyChangeListener(this);
     connect(car->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));
     CarLoads::instance()->updateComboBox(car->getTypeName(), loadComboBox);
     loadComboBox->setCurrentIndex(loadComboBox->findText(car->getLoadName()));

     kernelComboBox->setCurrentIndex(kernelComboBox->findText(car->getKernelName()));

     commentTextField->setText(car->getComment());
     valueTextField->setText(car->getValue());
     rfidComboBox->setCurrentIndex(rfidComboBox->findData(VPtr<DefaultIdTag>::asQVariant(car->getIdTag())));
     autoTrackCheckBox->setEnabled(true);
     blockingTextField->setText(QString::number(car->getBlocking()));

     setTitle(tr("Edit Car"));
 }

 // combo boxes
 /*public*/ void CarEditFrame::comboBoxActionPerformed(QWidget* ae)
{
 JComboBox* source = (JComboBox*)ae;
     if (source == typeComboBox && typeComboBox->currentText() != NULL) {
         log->debug("Type comboBox sees change, update car loads");
         CarLoads::instance()->updateComboBox( typeComboBox->currentText(), loadComboBox);
         // turnout off auto for location tracks
         autoTrackCheckBox->setChecked(false);
         autoTrackCheckBox->setEnabled(false);
         updateTrackLocationBox();
     }
     if (source == locationBox) {
         updateTrackLocationBox();
     }
     if (source == lengthComboBox && autoCheckBox->isChecked()) {
         calculateWeight();
     }
 }

 /*private*/ void CarEditFrame::updateTrackLocationBox() {
     if (locationBox->currentText() == "") {
         trackLocationBox->clear();
     } else {
         log->debug("Update tracks for location: " + locationBox->currentText());
         Location* loc = VPtr<Location>::asPtr( locationBox->itemData(locationBox->currentIndex()));
         loc->updateComboBox(trackLocationBox, _car, autoTrackCheckBox->isChecked(), false);
         if (_car != NULL && _car->getLocation() == loc  )
         {
          if(_car->getTrack() != NULL)
             trackLocationBox->setCurrentIndex(trackLocationBox->findText(_car->getTrack()->getName()));
          else
           trackLocationBox->setCurrentIndex(-1);
         }
     }
 }

 /*public*/ void CarEditFrame::checkBoxActionPerformed(QWidget* ae) {
     QCheckBox* source = (QCheckBox*) ae;
     log->debug(tr("checkbox change %1").arg(source->text()));
     if (source == cabooseCheckBox && cabooseCheckBox->isChecked()) {
         fredCheckBox->setChecked(false);
     }
     if (source == fredCheckBox && fredCheckBox->isChecked()) {
         cabooseCheckBox->setChecked(false);
     }
     if (source == autoTrackCheckBox) {
         updateTrackLocationBox();
     }
     if (source == passengerCheckBox) {
         pBlocking->setVisible(passengerCheckBox->isChecked());
     }
 }

 // Save, Delete, Add, Clear, Calculate, Edit Load buttons
 /*public*/ void CarEditFrame::buttonActionPerformed(QWidget* ae)
{
 QPushButton* source = (QPushButton*)ae;

     if (source == saveButton)
     {
         // log->debug("car save button pressed");
         if (!checkCar(_car)) {
             return;
         }
         // if the car's road or number changes, the car needs a new id
         if (_car != NULL
                 && _car->getRoadName()!=(Car::NONE)
                 && (_car->getRoadName()!=(roadComboBox->currentText()) || _car->getNumber()
                         !=(roadNumberTextField->text()))) {
             QString road =  roadComboBox->currentText();
             QString number = roadNumberTextField->text();
             carManager->changeId(_car, road, number);
             _car->setRoadName(road);
             _car->setNumber(number);
         }
         saveCar(true);
         // save car file
         writeFiles();
         if (Setup::isCloseWindowOnSaveEnabled()) {
             dispose();
         }
     }
     if (source == deleteButton) {
         log->debug("car delete button activated");
         if (_car != NULL && _car->getRoadName()==(roadComboBox->currentText())
                 && _car->getNumber()==(roadNumberTextField->text())) {
             carManager->deregister(_car);
             _car = NULL;
             // save car file
             writeFiles();
         } else {
             Car* car = carManager->getByRoadAndNumber( roadComboBox->currentText(), roadNumberTextField
                     ->text());
             if (car != NULL) {
                 carManager->deregister(car);
                 // save car file
                 writeFiles();
             }
         }
     }

     if (source == addButton) {
         if (!checkCar(NULL)) {
             return;
         }
         saveCar(false);
         // save car file
         writeFiles();
     }

     if (source == clearRoadNumberButton) {
         roadNumberTextField->setText("");
         roadNumberTextField->setFocus();
     }

     if (source == fillWeightButton) {
         calculateWeight();
     }

     if (source == editLoadButton) {
         if (lef != NULL) {
             lef->dispose();
         }
         lef = new CarLoadEditFrame();
         //lef.setLocationRelativeTo(this);
         lef->initComponents( typeComboBox->currentText(),  loadComboBox->currentText());
     }

 }

 /**
  * Need to also write the location and train files if a road name was
  * deleted. Need to also write files if car type was changed.
  */
 /*private*/ void CarEditFrame::writeFiles() {
     OperationsXml::save();
 }

 /*private*/ bool CarEditFrame::checkCar(Car* c) {
     QString roadNum = roadNumberTextField->text();
     if (roadNum.length() > Control::max_len_string_road_number) {
//         JOptionPane.showMessageDialog(this, tr("carRoadNum"),
//                 new Object[]{Control::max_len_string_road_number + 1}), tr("carRoadLong"),
//                 JOptionPane.ERROR_MESSAGE);
      QMessageBox::critical(this, tr("Car road number too long!"), tr("Car road number must be less than %1 characters").arg(Control::max_len_string_road_number + 1) );
         return false;
     }
     // check to see if car with road and number already exists
     Car* car = carManager->getByRoadAndNumber( roadComboBox->currentText(), roadNumberTextField
             ->text());
     if (car != NULL) {
         // new car?
         if (c == NULL) {
//             JOptionPane.showMessageDialog(this, tr("carRoadExists"), Bundle
//                     .getMessage("carCanNotAdd"), JOptionPane.ERROR_MESSAGE);
          QMessageBox::critical(this, tr("Can not add car!"), tr("Car with this road name and number already exists!"));
             return false;
         }
         // old car with new road or number?
         if (car->getId()!=(c->getId())) {
//             JOptionPane.showMessageDialog(this, tr("carRoadExists"), Bundle
//                     .getMessage("carCanNotUpdate"), JOptionPane.ERROR_MESSAGE);
             QMessageBox::critical(this, tr("Can not update car!"), tr("Car with this road name and number already exists!"));
             return false;
         }
     }
     // check car's weight has proper format
     bool ok;
     //try {
         //Number number = NumberFormat.getNumberInstance().parse(weightTextField->text());
         double number = weightTextField->text().toDouble(&ok);
         log->debug(tr("Car weight in oz: %1").arg(number));
     if(!ok) {
//         JOptionPane.showMessageDialog(this, tr("carWeightFormat"), Bundle
//                 .getMessage("carActualWeight"), JOptionPane.ERROR_MESSAGE);
      QMessageBox::critical(this, tr("Car's actual weight incorrect"), tr("Car's weight must be in the format of xx.x oz"));
         return false;
     }
     // check car's weight in tons has proper format
     //try {
         weightTonsTextField->text().toInt(&ok);
     if(!ok) {
//         JOptionPane.showMessageDialog(this, tr("carWeightFormatTon"), Bundle
//                 .getMessage("carWeightTon"), JOptionPane.ERROR_MESSAGE);
      QMessageBox::critical(this, tr("Car weight in tons incorrect"), tr("Car's weight must be in the format of xx tons"));
         return false;
     }
     return true;
 }

 /*private*/ void CarEditFrame::calculateWeight()
 {
  if (lengthComboBox->currentText() != NULL)
  {
   QString length =  lengthComboBox->currentText();
   //try {
   bool ok;
   double carLength = length.toDouble(&ok) * 12 / Setup::getScaleRatio();
   double carWeight = (Setup::getInitalWeight() + carLength * Setup::getAddWeight()) / 1000;
//             NumberFormat nf = NumberFormat.getNumberInstance();
//             nf.setMaximumFractionDigits(1);
   weightTextField->setText(QString::number(carWeight,'g',2)); // car weight in ounces.
   int tons = (int) (carWeight * Setup::getScaleTonRatio());
   // adjust weight for caboose
   if (cabooseCheckBox->isChecked() || passengerCheckBox->isChecked()) {
       tons = (int) length.toDouble(&ok) * .9; // .9 tons/foot
   }
   weightTonsTextField->setText(QString::number(tons));
   if(!ok)
   {
//             JOptionPane.showMessageDialog(this, tr("carLengthMustBe"), Bundle
//                     .getMessage("carWeigthCanNot"), JOptionPane.ERROR_MESSAGE);
    QMessageBox::critical(this, tr("Can not calculate car weight!"), tr("Car length must be a number in feet"));
   }
  }
 }

 /*private*/ void CarEditFrame::saveCar(bool isSave) {
     if (roadComboBox->currentText() == NULL) {
         return;
     }
     if (_car == NULL || _car->getRoadName()!=(roadComboBox->currentText())
             || _car->getNumber()!=(roadNumberTextField->text())) {
         _car = carManager->newCar( roadComboBox->currentText(), roadNumberTextField->text());
         //_car->addPropertyChangeListener(this);
         connect(_car->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));
     }
     if (typeComboBox->currentText() != NULL) {
         _car->setTypeName( typeComboBox->currentText());
     }
     if (lengthComboBox->currentText() != NULL) {
         _car->setLength( lengthComboBox->currentText());
     }
     if (colorComboBox->currentText() != NULL) {
         _car->setColor( colorComboBox->currentText());
     }
     //try {
         //_car->setWeight(NumberFormat.getNumberInstance().parse(weightTextField->text()).toString());
     _car->setWeight(weightTextField->text());
     //} catch (ParseException e1) {

     //}
     _car->setWeightTons(weightTonsTextField->text());

     // ask if all cars of this type should be passenger
     if (isSave && _car->isPassenger() ^ passengerCheckBox->isChecked()) {
//         if (JOptionPane.showConfirmDialog(this, passengerCheckBox->isChecked() ? Bundle
//                 .getMessage("carModifyTypePassenger") : tr("carRemoveTypePassenger"),
//                 new Object[]{_car->getTypeName()}), tr("carModifyAllType"),
//                 new Object[]{_car->getTypeName()}), JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) {
      QString msg = passengerCheckBox->isChecked() ? tr("Change all of your cars of type %1 to passenger?").arg(_car->getTypeName()) : tr("Remove passenger from all of your car of type %1?").arg(_car->getTypeName());
                                                     //                 new Object[]{_car->getTypeName()}), tr("carModifyAllType"
      if(QMessageBox::question(this, tr("Modify all cars of type %1?").arg(_car->getTypeName()), msg, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
      {
             // go through the entire list and change the passenger setting
             // for all cars of this type
             foreach (RollingStock* rs, *carManager->getList()) {
                 Car* c = (Car*) rs;
                 if (c->getTypeName()==(_car->getTypeName())) {
                     c->setPassenger(passengerCheckBox->isChecked());
                 }
             }
         }
     }
     _car->setPassenger(passengerCheckBox->isChecked());
     int blocking = 0;
     //try {
     bool ok;
         blocking = blockingTextField->text().toInt(&ok);
         // only allow numbers between 0 and 100
         if (blocking < 0 || blocking > 100) {
             blocking = 0;
         }
     if(!ok) {
         log->warn("Blocking must be a number between 0 and 100");
     }
     // ask if blocking order should be the same
     if (isSave && _car->getKernel() == NULL && passengerCheckBox->isChecked() && _car->getBlocking() != blocking) {
//         if (JOptionPane.showConfirmDialog(this, tr("carChangeBlocking"),
//                 new Object[]{blocking, _car->getTypeName()}), Bundle
//                 .getMessage("carModifyAllType"), new Object[]{_car->getTypeName()}), JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) {
      if(QMessageBox::question(this, tr("Modify all cars of type %1?").arg(_car->getTypeName()), tr("Change passenger car order blocking to %1 for all of your %2 cars?").arg(blocking).arg(_car->getTypeName()), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
      {
             // go through the entire list and change the passenger setting
             // for all cars of this type
             foreach (RollingStock* rs, *carManager->getList()) {
                 Car* c = (Car*) rs;
                 if (c->isPassenger() && c->getTypeName()==(_car->getTypeName())) {
                     c->setBlocking(blocking);
                 }
             }
         }
     }
     _car->setBlocking(blocking);
     // ask if all cars of this type should be caboose
     if (isSave && _car->isCaboose() ^ cabooseCheckBox->isChecked()) {
//         if (JOptionPane.showConfirmDialog(this, cabooseCheckBox->isChecked() ? Bundle
//                 .getMessage("carModifyTypeCaboose") : tr("carRemoveTypeCaboose"),
//                 new Object[]{_car->getTypeName()}), tr("carModifyAllType"),
//                 new Object[]{_car->getTypeName()}), JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) {
      QString msg = cabooseCheckBox->isChecked() ? tr("Change all of your cars of type %1 to caboose?").arg(_car->getTypeName()) : tr("Remove caboose from all of your car of type %1?").arg(_car->getTypeName());
      if(QMessageBox::question(this, tr("Modify all cars of type %1?").arg(_car->getTypeName()), msg, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
      {
             // go through the entire list and change the caboose setting for all cars of this type
             foreach (RollingStock* rs, *carManager->getList()) {
                 Car* c = (Car*) rs;
                 if (c->getTypeName()==(_car->getTypeName())) {
                     c->setCaboose(cabooseCheckBox->isChecked());
                 }
             }
         }
     }
     _car->setCaboose(cabooseCheckBox->isChecked());
     // ask if all cars of this type should be utility
     if (isSave && _car->isUtility() ^ utilityCheckBox->isChecked()) {
//         if (JOptionPane.showConfirmDialog(this, utilityCheckBox->isChecked() ? Bundle
//                 .getMessage("carModifyTypeUtility") : tr("carRemoveTypeUtility"),
//                 new Object[]{_car->getTypeName()}), tr("carModifyAllType"),
//                 new Object[]{_car->getTypeName()}), JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) {
      QString msg = utilityCheckBox->isChecked() ? tr("Change all of your cars of type %1 to utility?").arg(_car->getTypeName()) : tr("Remove utility from all of your car of type %1?").arg(_car->getTypeName());
      if(QMessageBox::question(this, tr("Modify all cars of type %1?").arg(_car->getTypeName()), msg, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
      {
             // go through the entire list and change the utility for all cars of this type
             foreach (RollingStock* rs, *carManager->getList()) {
                 Car* c = (Car*) rs;
                 if (c->getTypeName()==(_car->getTypeName())) {
                     c->setUtility(utilityCheckBox->isChecked());
                 }
             }
         }
     }
     _car->setUtility(utilityCheckBox->isChecked());
     // ask if all cars of this type should be hazardous
     if (isSave && _car->isHazardous() ^ hazardousCheckBox->isChecked()) {
//         if (JOptionPane.showConfirmDialog(this, hazardousCheckBox->isChecked() ? Bundle
//                 .getMessage("carModifyTypeHazardous") : tr("carRemoveTypeHazardous"),
//                 new Object[]{_car->getTypeName()}), tr("carModifyAllType"),
//                 new Object[]{_car->getTypeName()}), JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) {
      QString msg = hazardousCheckBox->isChecked() ? tr("carModifyTypeHazardous").arg(_car->getTypeName()) : tr("carRemoveTypeHazardous").arg(_car->getTypeName());
      if(QMessageBox::question(this, tr("Modify all cars of type %1?").arg(_car->getTypeName()), msg, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
      {
             // go through the entire list and change the hazardous setting for all cars of this type
             foreach (RollingStock* rs, *carManager->getList()) {
                 Car* c = (Car*) rs;
                 if (c->getTypeName()==(_car->getTypeName())) {
                     c->setHazardous(hazardousCheckBox->isChecked());
                 }
             }
         }
     }
     _car->setHazardous(hazardousCheckBox->isChecked());
     _car->setFred(fredCheckBox->isChecked());
     _car->setBuilt(builtTextField->text());
     if (ownerComboBox->currentText() != NULL) {
         _car->setOwner( ownerComboBox->currentText());
     }
     if (kernelComboBox->currentText() != NULL) {
         if (kernelComboBox->currentText()==(CarManager::NONE)) {
             _car->setKernel(NULL);
         } else if (_car->getKernelName()!=(kernelComboBox->currentText())) {
             _car->setKernel(carManager->getKernelByName( kernelComboBox->currentText()));
             // if car has FRED or caboose make lead
             if (_car->hasFred() || _car->isCaboose()) {
                 _car->getKernel()->setLead(_car);
             }
             _car->setBlocking(_car->getKernel()->getSize());
         }
     }
     if (loadComboBox->currentText() != NULL && _car->getLoadName()!=(loadComboBox->currentText())) {
         _car->setLoadName( loadComboBox->currentText());
         // check to see if car is part of kernel, and ask if all the other cars in the kernel should be changed
         if (_car->getKernel() != NULL) {
             QList<Car*> cars = _car->getKernel()->getCars();
             if (cars.size() > 1) {
//                 if (JOptionPane.showConfirmDialog(this, tr("carInKernel"),
//                         new Object[]{_car->toString()}), tr("carPartKernel"),
//                         new Object[]{_car->getKernelName()}), JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) {
              if(QMessageBox::question(this, tr("Car is part of kernel (%1)").arg(_car->getKernelName()), tr("Car (%1) is part of a kernel, do you want the other cars to also have the same settings?").arg(_car->toString()), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
              {
                     // go through the entire list and change the loads for all cars
                     foreach (Car* car, cars) {
                         if (CarLoads::instance()->containsName(car->getTypeName(), _car->getLoadName())) {
                             car->setLoadName(_car->getLoadName());
                         }
                     }
                 }
             }
         }
     }
     _car->setComment(commentTextField->text());
     _car->setValue(valueTextField->text());
     int ix = rfidComboBox->currentIndex();
     DefaultIdTag* t =  VPtr<DefaultIdTag>::asPtr(rfidComboBox->itemData(rfidComboBox->currentIndex()));
     if(rfidComboBox->currentIndex() >= 0)
      _car->setIdTag( VPtr<DefaultIdTag>::asPtr(rfidComboBox->itemData(rfidComboBox->currentIndex())));
     else
      _car->setIdTag(NULL);
     autoTrackCheckBox->setEnabled(true);

     // update blocking
     blockingTextField->setText(QString::number(_car->getBlocking()));

     if (locationBox->currentText() != NULL && trackLocationBox->currentText() == NULL) {
//         JOptionPane.showMessageDialog(this, tr("rsFullySelect"), Bundle
//                 .getMessage("rsCanNotLoc"), JOptionPane.ERROR_MESSAGE);
      QMessageBox::information(this, tr("Can not update car location"), tr("To place a car, you must select the car's location and track"));
     } else {
         // update location only if it has changed
         if (_car->getLocation() == NULL || _car->getLocation()!=(VPtr<Location>::asPtr(locationBox->itemData(locationBox->currentIndex())))
                 || _car->getTrack() == NULL || _car->getTrack()!=(VPtr<Track>::asPtr(trackLocationBox->itemData(trackLocationBox->currentIndex())))) {
             setLocation(_car);
             // is this car part of a kernel?
             if (_car->getKernel() != NULL) {
                 QList<Car*> cars = _car->getKernel()->getCars();
                 if (cars.size() > 1) {
//                     if (JOptionPane.showConfirmDialog(this, Bundle
//                             .getMessage("carInKernel"), new Object[]{_car->toString()}), MessageFormat
//                             .format(tr("carPartKernel"), new Object[]{_car->getKernelName()}),
//                             JOptionPane.YES_NO_OPTION) == JOptionPane.YES_OPTION) {
                  if(QMessageBox::question(this, tr("Car is part of kernel (%1)").arg(_car->getKernelName()), tr("Car (%1) is part of a kernel, do you want the other cars to also have the same settings?").arg(_car->toString()), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
                  {
                         // go through the entire list and change the location for all cars
                         foreach (Car* car, cars) {
                             if (car != _car) {
                                 setLocation(car);
                             }
                         }
                     }
                 }
             }
         }
     }
 }

 /*private*/ void CarEditFrame::setLocation(Car* car) {
     if (locationBox->currentText() == NULL) {
         car->setLocation(NULL, NULL);
     } else {
         car->setSavedRouteId(RollingStock::NONE); // clear last route id
         QString status = car->setLocation( VPtr<Location>::asPtr(locationBox->itemData(locationBox->currentIndex())), VPtr<Track>::asPtr(trackLocationBox
                 ->itemData(trackLocationBox->currentIndex())));
         if (status!=(Track::OKAY)) {
             log->debug(tr("Can't set car's location because of %1").arg(status));
//             JOptionPane.showMessageDialog(this, tr("rsCanNotLocMsg"),
//                     new Object[]{car.toString(), status}), tr("rsCanNotLoc"),
//                     JOptionPane.ERROR_MESSAGE);
           QMessageBox::critical(this, tr("Can not update car location"), tr("Can't set car (%1) location and track due to {%2").arg(_car->toString()).arg(status));
             // does the user want to force the rolling stock to this track?
//             int results = JOptionPane.showOptionDialog(this, tr("rsForce"),
//                     new Object[]{car.toString(), (Track) trackLocationBox->currentText()}), MessageFormat
//                     .format(tr("rsOverride"), new Object[]{status}), JOptionPane.YES_NO_OPTION,
//                     JOptionPane.QUESTION_MESSAGE, NULL, NULL, NULL);
           int results =QMessageBox::question(this, tr("Do you want to override track''s %1?").arg(status), tr("Do you want to force car (%1) to track (%2)?").arg(car->toString()).arg(trackLocationBox->currentText()), QMessageBox::Yes | QMessageBox::No);
             if (results == QMessageBox::Yes)
             {
                 log->debug("Force rolling stock to track");
                 car->setLocation(VPtr<Location>::asPtr(locationBox->itemData(locationBox->currentIndex())), VPtr<Track>::asPtr(trackLocationBox
                         ->itemData(trackLocationBox->currentIndex())), true);
             }
         }
     }
 }

 /*private*/ void CarEditFrame::addEditButtonAction(QPushButton* b)
{
//     b.addActionListener(new java.awt.event.ActionListener() {
//         /*public*/ void actionPerformed(java.awt.event.ActionEvent e) {
//             buttonEditActionPerformed(e);
//         }
//     });
 editButtonMapper->setMapping(b,b);
 connect(b, SIGNAL(clicked()), editButtonMapper, SLOT(map()));
 }


 // edit buttons only one frame active at a time
 /*public*/ void CarEditFrame::buttonEditActionPerformed(QWidget* ae) {

     if (editActive) {
         f->dispose();
     }
     f = new CarAttributeEditFrame();
//     f->setLocationRelativeTo(this);
     //f.addPropertyChangeListener(this);
     connect(f->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));
     editActive = true;
 QPushButton* source = (QPushButton*)ae;
     if (source == editRoadButton) {
         f->initComponents(ROAD,  roadComboBox->currentText());
     }
     if (source == editTypeButton) {
         f->initComponents(TYPE,  typeComboBox->currentText());
     }
     if (source == editColorButton) {
         f->initComponents(COLOR,  colorComboBox->currentText());
     }
     if (source == editLengthButton) {
         f->initComponents(LENGTH,  lengthComboBox->currentText());
     }
     if (source == editOwnerButton) {
         f->initComponents(OWNER,  ownerComboBox->currentText());
     }
     if (source == editKernelButton) {
         f->initComponents(KERNEL,  kernelComboBox->currentText());
     }
 }

 /*public*/ void CarEditFrame::dispose() {
     removePropertyChangeListeners();
     OperationsFrame::dispose();
 }

 /*private*/ void CarEditFrame::removePropertyChangeListeners()
 {
  //CarRoads::instance().removePropertyChangeListener(this);
  disconnect(CarRoads::instance()->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));
  //CarLoads::instance().removePropertyChangeListener(this);
  disconnect(CarLoads::instance()->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));
  //CarTypes.instance().removePropertyChangeListener(this);
  disconnect(CarTypes::instance()->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));
//  CarLengths.instance().removePropertyChangeListener(this);
//  CarColors.instance().removePropertyChangeListener(this);
  disconnect(CarColors::instance()->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));
//  CarOwners.instance().removePropertyChangeListener(this);
  //locationManager.removePropertyChangeListener(this);
  disconnect(locationManager->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));
  //carManager.removePropertyChangeListener(this);
  disconnect(carManager->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));
  if (_car != NULL) {
      //_car->removePropertyChangeListener(this);
   disconnect(_car->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));
  }
 }

 /*public*/ void CarEditFrame::propertyChange(PropertyChangeEvent* e) {
     if (Control::SHOW_PROPERTY) {
         log->debug(tr("Property change: (%1) old: (%2) new: (%3)").arg(e->getPropertyName()).arg(e->getOldValue().toString()).arg(e
                 ->getNewValue().toString()));
     }
     if (e->getPropertyName()==(CarRoads::CARROADS_CHANGED_PROPERTY)) {
         CarRoads::instance()->updateComboBox(roadComboBox);
         if (_car != NULL) {
             roadComboBox->setCurrentIndex(roadComboBox->findText(_car->getRoadName()));
         }
     }
     if (e->getPropertyName()==(CarTypes::CARTYPES_CHANGED_PROPERTY)) {
         CarTypes::instance()->updateComboBox(typeComboBox);
         if (_car != NULL) {
             typeComboBox->setCurrentIndex(typeComboBox->findText(_car->getTypeName()));
         }
     }

     if (e->getPropertyName()==(CarColors::CARCOLORS_CHANGED_PROPERTY)) {
         CarColors::instance()->updateComboBox(colorComboBox);
         if (_car != NULL) {
             colorComboBox->setCurrentIndex(colorComboBox->findText(_car->getColor()));
         }
     }
     if (e->getPropertyName()==(CarLengths::CARLENGTHS_CHANGED_PROPERTY)) {
         CarLengths::instance()->updateComboBox(lengthComboBox);
         if (_car != NULL) {
             lengthComboBox->setCurrentIndex(lengthComboBox->findText(_car->getLength()));
         }
     }

     if (e->getPropertyName()==(CarManager::KERNEL_LISTLENGTH_CHANGED_PROPERTY)
             || e->getPropertyName()==(Car::KERNEL_NAME_CHANGED_PROPERTY)) {
         carManager->updateKernelComboBox(kernelComboBox);
         if (_car != NULL) {
             kernelComboBox->setCurrentIndex(kernelComboBox->findText(_car->getKernelName()));
         }
     }

     if (e->getPropertyName()==(CarOwners::CAROWNERS_CHANGED_PROPERTY)) {
         CarOwners::instance()->updateComboBox(ownerComboBox);
         if (_car != NULL) {
             ownerComboBox->setCurrentIndex(ownerComboBox->findText(_car->getOwner()));
         }
     }

     if (e->getPropertyName()==(LocationManager::LISTLENGTH_CHANGED_PROPERTY)
             || e->getPropertyName()==(RollingStock::TRACK_CHANGED_PROPERTY)) {
         LocationManager::instance()->updateComboBox(locationBox);
         updateTrackLocationBox();
         if (_car != NULL && _car->getLocation() != NULL) {
             locationBox->setCurrentIndex(locationBox->findText(_car->getLocation()->getName()));
         }
     }
     if (e->getPropertyName()==(Car::LOAD_CHANGED_PROPERTY)) {
         if (_car != NULL) {
             loadComboBox->setCurrentIndex(loadComboBox->findText(_car->getLoadName()));
         }
     }
     if (e->getPropertyName()==(CarLoads::LOAD_CHANGED_PROPERTY)) {
         if (_car != NULL) {
             CarLoads::instance()->updateComboBox(typeComboBox->currentText(), loadComboBox);
             loadComboBox->setCurrentIndex(loadComboBox->findText(_car->getLoadName()));
         }
     }

     if (e->getPropertyName()==(CarAttributeEditFrame::DISPOSE)) {
         editActive = false;
     }

 }

 /*public*/ QString CarEditFrame::getClassName()
 {
  return "jmri.jmrit.operations.rollingstock.cars.CarEditFrame";
 }

}
