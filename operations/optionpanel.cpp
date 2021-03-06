#include "optionpanel.h"
#include "setup.h"
#include <QPushButton>
#include <QCheckBox>
#include <QButtonGroup>
#include <QComboBox>
#include <QBoxLayout>
#include "jtextfield.h"
#include <QRadioButton>
#include <QScrollArea>
#include <QGroupBox>
#include "gridbaglayout.h"
#include "trainmanager.h"
#include "logger.h"
#include <QMessageBox>
#include "operationssetupxml.h"

namespace Operations
{
/**
 * Frame for user edit of setup options
 *
 * @author Dan Boudreau Copyright (C) 2010, 2011, 2012, 2013
 * @version $Revision: 29350 $
 */
///*public*/ class OptionPanel extends OperationsPreferencesPanel {

 /**
  *
  */
 //private static final long serialVersionUID = -5466426954499681092L;


 /*public*/ OptionPanel::OptionPanel(QWidget* parent) : OperationsPreferencesPanel(parent) {
 log = new Logger("OptionPanel");
 // labels
 // major buttons
 saveButton = new QPushButton(tr("Save"));

 // radio buttons
 buildNormal = new QRadioButton(tr("Normal"));
 buildAggressive = new QRadioButton(tr("Aggressive"));

 // check boxes
 routerCheckBox = new QCheckBox(tr("Enable Car Routing"));
 routerYardCheckBox = new QCheckBox(tr("Enable Car Routing via Yards"));
 routerStagingCheckBox = new QCheckBox(tr("Enable Car Routing through Staging"));
 routerAllTrainsBox = new QCheckBox(tr("Use all Trains when Routing"));
 routerRestrictBox = new QCheckBox(tr("Enable Track Destination Restrictions when Routing"));

 valueCheckBox = new QCheckBox(tr("Enable Value Fields"));
 rfidCheckBox = new QCheckBox(tr("Identification Tag Fields"));
 carLoggerCheckBox = new QCheckBox(tr("Enable Car Logging"));
 engineLoggerCheckBox = new QCheckBox(tr("Enable Engine Logging"));
 trainLoggerCheckBox = new QCheckBox(tr("Enable Train Logging"));

 localInterchangeCheckBox = new QCheckBox(tr("Allow Local Classification/Interchange to C/I Moves"));
 localSpurCheckBox = new QCheckBox(tr("Allow Local Spur to Spur Moves"));
 localYardCheckBox = new QCheckBox(tr("Allow Local Yard to Yard Moves"));

 trainIntoStagingCheckBox = new QCheckBox(tr("Enable Type, Road and Load Restrictions into Staging"));
 stagingAvailCheckBox = new QCheckBox(tr("Make departure track available after train is built"));
 stagingTurnCheckBox = new QCheckBox(tr("Allow cars to return to staging"));
 promptFromTrackStagingCheckBox = new QCheckBox(tr("Prompt for Departure Track from Staging"));
 promptToTrackStagingCheckBox = new QCheckBox(tr("Prompt for Arrival Track into Staging"));

 generateCvsManifestCheckBox = new QCheckBox(tr("Generate CSV Manifest"));
 generateCvsSwitchListCheckBox = new QCheckBox(tr("Generate CSV Switch List"));

 enableVsdCheckBox = new QCheckBox(tr("Enable physical locations for Virtual Sound Decoder"));

 // text field
 rfidTextField = new JTextField(10);
 valueTextField = new JTextField(10);

 // combo boxes
 numberPassesComboBox = new QComboBox();

  // load checkboxes
  localInterchangeCheckBox->setChecked(Setup::isLocalInterchangeMovesEnabled());
  localSpurCheckBox->setChecked(Setup::isLocalSpurMovesEnabled());
  localYardCheckBox->setChecked(Setup::isLocalYardMovesEnabled());
  // staging options
  trainIntoStagingCheckBox->setChecked(Setup::isTrainIntoStagingCheckEnabled());
  stagingAvailCheckBox->setChecked(Setup::isStagingTrackImmediatelyAvail());
  stagingTurnCheckBox->setChecked(Setup::isAllowReturnToStagingEnabled());
  promptToTrackStagingCheckBox->setChecked(Setup::isPromptToStagingEnabled());
  promptFromTrackStagingCheckBox->setChecked(Setup::isPromptFromStagingEnabled());
  // router
  routerCheckBox->setChecked(Setup::isCarRoutingEnabled());
  routerYardCheckBox->setChecked(Setup::isCarRoutingViaYardsEnabled());
  routerStagingCheckBox->setChecked(Setup::isCarRoutingViaStagingEnabled());
  routerAllTrainsBox->setChecked(!Setup::isOnlyActiveTrainsEnabled());
  routerRestrictBox->setChecked(Setup::isCheckCarDestinationEnabled());
  // logging options
  carLoggerCheckBox->setChecked(Setup::isCarLoggerEnabled());
  engineLoggerCheckBox->setChecked(Setup::isEngineLoggerEnabled());
  trainLoggerCheckBox->setChecked(Setup::isTrainLoggerEnabled());

  generateCvsManifestCheckBox->setChecked(Setup::isGenerateCsvManifestEnabled());
  generateCvsSwitchListCheckBox->setChecked(Setup::isGenerateCsvSwitchListEnabled());
  valueCheckBox->setChecked(Setup::isValueEnabled());
  rfidCheckBox->setChecked(Setup::isRfidEnabled());
  enableVsdCheckBox->setChecked(Setup::isVsdPhysicalLocationEnabled());

  // load text fields
  rfidTextField->setText(Setup::getRfidLabel());
  valueTextField->setText(Setup::getValueLabel());

  // add tool tips
  saveButton->setToolTip(tr("Writes this window's settings to file"));
  rfidTextField->setToolTip(tr("Enter the name you want for the Id Tag field"));
  valueTextField->setToolTip(tr("Enter the name you want for the value field"));
  stagingTurnCheckBox->setToolTip(tr("This option is also available per train"));

  // load combobox, allow 2 to 4 passes
  for (int x = 2; x < 5; x++) {
      numberPassesComboBox->addItem(QString::number(x), x);
  }

  numberPassesComboBox->setCurrentIndex(numberPassesComboBox->findData(Setup::getNumberPasses()));

  setLayout(new QVBoxLayout); //(this, BoxLayout.Y_AXIS));

  QWidget* panel = new QWidget();
  panel->setLayout(new QVBoxLayout); //(panel, BoxLayout.Y_AXIS));
  QScrollArea* panelPane = new QScrollArea(/*panel*/);
  panelPane->setWidgetResizable(true);

  // Build Options panel
  QGroupBox* pBuild = new QGroupBox();
  pBuild->setLayout(new GridBagLayout); //(pBuild, BoxLayout.Y_AXIS));
  //pBuild.setBorder(BorderFactory.createTitledBorder(tr("BorderLayoutBuildOptions")));
  pBuild->setStyleSheet(gbStyleSheet);
  pBuild->setTitle(tr("Build Options"));
  QWidget* pOpt = new QWidget();
  pOpt->setLayout(new GridBagLayout());

  addItem(pOpt, buildNormal, 1, 0);
  addItem(pOpt, buildAggressive, 2, 0);
  addItem(pBuild, pOpt, 1, 0);

  QGroupBox* pPasses = new QGroupBox();
  pPasses->setLayout(new GridBagLayout());
  //pPasses.setBorder(BorderFactory.createTitledBorder(tr("BorderLayoutNumberPasses")));
  pPasses->setStyleSheet(gbStyleSheet);
  pPasses->setTitle(tr("Number of Passes During Build"));
  addItem(pPasses, numberPassesComboBox, 0, 0);
  addItem(pBuild, pPasses, 1, 1);

  // Switcher Service
  QGroupBox* pSwitcher = new QGroupBox();
  pSwitcher->setLayout(new GridBagLayout());
//     pSwitcher.setBorder(BorderFactory
//             .createTitledBorder(tr("BorderLayoutSwitcherService")));
  pSwitcher->setStyleSheet(gbStyleSheet);
  pSwitcher->setTitle(tr("Switcher Service"));

  addItemLeft(pSwitcher, localInterchangeCheckBox, 1, 1);
  addItemLeft(pSwitcher, localSpurCheckBox, 1, 2);
  addItemLeft(pSwitcher, localYardCheckBox, 1, 3);
  addItemLeft(pBuild, pSwitcher, 1, 2);

  // Staging
  QGroupBox* pStaging = new QGroupBox();
  pStaging->setLayout(new GridBagLayout());
  //pStaging.setBorder(BorderFactory.createTitledBorder(tr("BorderLayoutStaging")));
  pStaging->setStyleSheet(gbStyleSheet);
  pStaging->setTitle(tr("Build Options"));

  addItemLeft(pStaging, trainIntoStagingCheckBox, 1, 4);
  addItemLeft(pStaging, stagingAvailCheckBox, 1, 5);
  addItemLeft(pStaging, stagingTurnCheckBox, 1, 6);
  addItemLeft(pStaging, promptFromTrackStagingCheckBox, 1, 7);
  addItemLeft(pStaging, promptToTrackStagingCheckBox, 1, 8);
  addItemLeft(pBuild, pStaging, 1, 3);

  // Router panel
  QGroupBox* pRouter = new QGroupBox();
  pRouter->setLayout(new GridBagLayout());
  //pRouter.setBorder(BorderFactory.createTitledBorder(tr("BorderLayoutRouterOptions")));
  pRouter->setStyleSheet(gbStyleSheet);
  pRouter->setTitle(tr("Car Routing Options"));
  addItemLeft(pRouter, routerCheckBox, 1, 0);
  addItemLeft(pRouter, routerYardCheckBox, 1, 1);
  addItemLeft(pRouter, routerStagingCheckBox, 1, 2);
  addItemLeft(pRouter, routerAllTrainsBox, 1, 3);
  addItemLeft(pRouter, routerRestrictBox, 1, 4);

  // Logger panel
  QGroupBox* pLogger = new QGroupBox();
  pLogger->setLayout(new GridBagLayout());
  //pLogger.setBorder(BorderFactory.createTitledBorder(tr("BorderLayoutLoggerOptions")));
  pLogger->setStyleSheet(gbStyleSheet);
  pLogger->setTitle(tr("Logging Options"));
  addItemLeft(pLogger, engineLoggerCheckBox, 1, 0);
  addItemLeft(pLogger, carLoggerCheckBox, 1, 1);
  addItemLeft(pLogger, trainLoggerCheckBox, 1, 2);

  // Custom Manifests and Switch Lists
  QGroupBox* pCustom = new QGroupBox();
  pCustom->setLayout(new GridBagLayout());
  //pCustom.setBorder(BorderFactory.createTitledBorder(tr("BorderLayoutCustomManifests")));
  pCustom->setStyleSheet(gbStyleSheet);
  pCustom->setTitle(tr("Custom Manifest and Switch Lists"));
  addItemLeft(pCustom, generateCvsManifestCheckBox, 1, 0);
  addItemLeft(pCustom, generateCvsSwitchListCheckBox, 1, 1);

  // Options
  QGroupBox* pOption = new QGroupBox();
  pOption->setLayout(new GridBagLayout());
  //pOption.setBorder(BorderFactory.createTitledBorder(tr("BorderLayoutOptions")));
  pOption->setStyleSheet(gbStyleSheet);
  pOption->setTitle(tr("Options"));
  addItemLeft(pOption, valueCheckBox, 1, 2);
  addItemLeft(pOption, valueTextField, 2, 2);
  addItemLeft(pOption, rfidCheckBox, 1, 3);
  addItemLeft(pOption, rfidTextField, 2, 3);
  addItemLeft(pOption, enableVsdCheckBox, 1, 4);

  // row 11
  QWidget* pControl = new QWidget();
  pControl->setLayout(new GridBagLayout());
  addItem(pControl, saveButton, 3, 9);

  panel->layout()->addWidget(pBuild);
  panel->layout()->addWidget(pRouter);
  panel->layout()->addWidget(pLogger);
  panel->layout()->addWidget(pCustom);
  panel->layout()->addWidget(pOption);
  panel->layout()->addWidget(pControl);

  panelPane->setWidget(panel);
  layout()->addWidget(panelPane);

  // setup buttons
  addButtonAction(saveButton);

  // radio buttons
  QButtonGroup* buildGroup = new QButtonGroup();
  buildGroup->addButton(buildNormal);
  buildGroup->addButton(buildAggressive);
  addRadioButtonAction(buildNormal);
  addRadioButtonAction(buildAggressive);

  // check boxes
  addCheckBoxAction(routerCheckBox);
  setRouterCheckBoxesEnabled();

  setBuildOption();

  // disable staging option if normal mode
  stagingAvailCheckBox->setEnabled(buildAggressive->isChecked());
  numberPassesComboBox->setEnabled(buildAggressive->isChecked());

  initMinimumSize();
 }

 /*private*/ void OptionPanel::setBuildOption()
 {
  buildNormal->setChecked(!Setup::isBuildAggressive());
  buildAggressive->setChecked(Setup::isBuildAggressive());
 }

 //@Override
 /*public*/ void OptionPanel::radioButtonActionPerformed(QWidget* ae)
 {
  log->debug("radio button selected");
  // can't change the build option if there are trains built
  if (TrainManager::instance()->isAnyTrainBuilt()) {
      setBuildOption(); // restore the correct setting
//         JOptionPane.showMessageDialog(this, tr("CanNotChangeBuild"), Bundle
//                 .getMessage("MustTerminateOrReset"), JOptionPane.ERROR_MESSAGE);
      QMessageBox::critical(this, tr("Must terminate or reset all trains"), tr("Can not change build option when there is a built train!"));
  }
  // disable staging option if normal mode
  stagingAvailCheckBox->setEnabled(buildAggressive->isChecked());
  numberPassesComboBox->setEnabled(buildAggressive->isChecked());
 }

 // Save button
 //@Override
 /*public*/ void OptionPanel::buttonActionPerformed(QWidget* ae)
 {
  QPushButton* source = (QPushButton*)ae;
  if (source == saveButton)
  {
   this->savePreferences();
   if (Setup::isCloseWindowOnSaveEnabled())
   {
    dispose();
   }
  }
 }

 //@Override
 /*protected*/ void OptionPanel::checkBoxActionPerformed(QWidget* ae)
 {
  if ((QCheckBox*)ae == routerCheckBox)
  {
   setRouterCheckBoxesEnabled();
  }
 }

 /*private*/ void OptionPanel::setRouterCheckBoxesEnabled()
 {
  routerYardCheckBox->setEnabled(routerCheckBox->isChecked());
  routerStagingCheckBox->setEnabled(routerCheckBox->isChecked());
  routerAllTrainsBox->setEnabled(routerCheckBox->isChecked());
  routerRestrictBox->setEnabled(routerCheckBox->isChecked());
 }

 //@Override
 /*public*/ QString OptionPanel::getTabbedPreferencesTitle()
 {
  return tr("Options");
 }

// @Override
 /*public*/ QString OptionPanel::getPreferencesTooltip() {
     return "";
 }

 //@Override
 /*public*/ void OptionPanel::savePreferences()
 {
  // build option
  Setup::setBuildAggressive(buildAggressive->isChecked());
  Setup::setNumberPasses(numberPassesComboBox->currentData().toInt());
  // Local moves?
  Setup::setLocalInterchangeMovesEnabled(localInterchangeCheckBox->isChecked());
  Setup::setLocalSpurMovesEnabled(localSpurCheckBox->isChecked());
  Setup::setLocalYardMovesEnabled(localYardCheckBox->isChecked());
  // Staging options
  Setup::setTrainIntoStagingCheckEnabled(trainIntoStagingCheckBox->isChecked());
  Setup::setStagingTrackImmediatelyAvail(stagingAvailCheckBox->isChecked());
  Setup::setAllowReturnToStagingEnabled(stagingTurnCheckBox->isChecked());
  Setup::setPromptFromStagingEnabled(promptFromTrackStagingCheckBox->isChecked());
  Setup::setPromptToStagingEnabled(promptToTrackStagingCheckBox->isChecked());
  // Car routing enabled?
  Setup::setCarRoutingEnabled(routerCheckBox->isChecked());
  Setup::setCarRoutingViaYardsEnabled(routerYardCheckBox->isChecked());
  Setup::setCarRoutingViaStagingEnabled(routerStagingCheckBox->isChecked());
  Setup::setOnlyActiveTrainsEnabled(!routerAllTrainsBox->isChecked());
  Setup::setCheckCarDestinationEnabled(routerRestrictBox->isChecked());
  // Options
  Setup::setGenerateCsvManifestEnabled(generateCvsManifestCheckBox->isChecked());
  Setup::setGenerateCsvSwitchListEnabled(generateCvsSwitchListCheckBox->isChecked());
  Setup::setValueEnabled(valueCheckBox->isChecked());
  Setup::setValueLabel(valueTextField->text());
  Setup::setRfidEnabled(rfidCheckBox->isChecked());
  Setup::setRfidLabel(rfidTextField->text());
  // Logging enabled?
  Setup::setEngineLoggerEnabled(engineLoggerCheckBox->isChecked());
  Setup::setCarLoggerEnabled(carLoggerCheckBox->isChecked());
  Setup::setTrainLoggerEnabled(trainLoggerCheckBox->isChecked());
  // VSD
  Setup::setVsdPhysicalLocationEnabled(enableVsdCheckBox->isChecked());
  // write the file
  OperationsSetupXml::instance()->writeOperationsFile();
 }

 //@Override
 /*public*/ bool OptionPanel::isDirty()
 {
  return ( // build option
          Setup::isBuildAggressive() != buildAggressive->isChecked()
          || Setup::getNumberPasses() != (int) numberPassesComboBox->currentData().toInt()
          // Local moves?
          || Setup::isLocalInterchangeMovesEnabled() != localInterchangeCheckBox->isChecked()
          || Setup::isLocalSpurMovesEnabled() != localSpurCheckBox->isChecked()
          || Setup::isLocalYardMovesEnabled() != localYardCheckBox->isChecked()
          // Staging options
          || Setup::isTrainIntoStagingCheckEnabled() != trainIntoStagingCheckBox->isChecked()
          || Setup::isStagingTrackImmediatelyAvail() != stagingAvailCheckBox->isChecked()
          || Setup::isAllowReturnToStagingEnabled() != stagingTurnCheckBox->isChecked()
          || Setup::isPromptFromStagingEnabled() != promptFromTrackStagingCheckBox->isChecked()
          || Setup::isPromptToStagingEnabled() != promptToTrackStagingCheckBox->isChecked()
          // Car routing enabled?
          || Setup::isCarRoutingEnabled() != routerCheckBox->isChecked()
          || Setup::isCarRoutingViaYardsEnabled() != routerYardCheckBox->isChecked()
          || Setup::isCarRoutingViaStagingEnabled() != routerStagingCheckBox->isChecked()
          || Setup::isOnlyActiveTrainsEnabled() != !routerAllTrainsBox->isChecked()
          || Setup::isCheckCarDestinationEnabled() != routerRestrictBox->isChecked()
          // Options
          || Setup::isGenerateCsvManifestEnabled() != generateCvsManifestCheckBox->isChecked()
          || Setup::isGenerateCsvSwitchListEnabled() != generateCvsSwitchListCheckBox->isChecked()
          || Setup::isValueEnabled() != valueCheckBox->isChecked()
          || Setup::getValueLabel()!=(valueTextField->text())
          || Setup::isRfidEnabled() != rfidCheckBox->isChecked()
          || Setup::getRfidLabel()!=(rfidTextField->text())
          // Logging enabled?
          || Setup::isEngineLoggerEnabled() != engineLoggerCheckBox->isChecked()
          || Setup::isCarLoggerEnabled() != carLoggerCheckBox->isChecked()
          || Setup::isTrainLoggerEnabled() != trainLoggerCheckBox->isChecked()
          // VSD
          || Setup::isVsdPhysicalLocationEnabled() != enableVsdCheckBox->isChecked());
 }
}
