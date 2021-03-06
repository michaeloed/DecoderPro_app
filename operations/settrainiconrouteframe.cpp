#include "settrainiconrouteframe.h"
#include "routemanager.h"
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include "trainicon.h"
#include "route.h"
#include "logger.h"
#include "control.h"
#include <QSpinBox>
#include <QBoxLayout>
#include "setup.h"
#include <QGroupBox>
#include "gridbaglayout.h"
#include "propertychangeevent.h"
#include "routelocation.h"
#include "routemanagerxml.h"
#include <QMessageBox>
#include "panelmenu.h"
#include "editor.h"
#include "instancemanager.h"

namespace Operations
{
 /**
  *
  */
 //private static final long serialVersionUID = 3933825267912834479L;


 /*public*/ SetTrainIconRouteFrame::SetTrainIconRouteFrame(QString routeName, QWidget* parent)
 : OperationsFrame(tr("Train Icon"),parent)
 {
  //super(tr("MenuSetTrainIcon"));
  log = new Logger("SetTrainIconRouteFrame");
  value = QMessageBox::No; //JOptionPane.NO_OPTION;
  FORWARD = 1;
  BACK = -1;
  NONE = 0;

  routeManager = RouteManager::instance();

  // labels
  textX = new JLabel("   X  ");
  textY = new JLabel("   Y  ");

  routeLocationName = new JLabel();

  // text field
  // check boxes
  // major buttons
  previousButton = new QPushButton(tr("Previous"));
  nextButton = new QPushButton(tr("Next"));
  placeButton = new QPushButton(tr("Place Test Icon"));
  applyButton = new QPushButton(tr("Apply"));
  saveButton = new QPushButton(tr("Save"));

  // combo boxes
  // Spinners
  spinTrainIconX = new QSpinBox(); //(new SpinnerNumberModel(0, 0, 10000, 1));
  spinTrainIconX->setMinimum(0);
  spinTrainIconX->setMaximum(10000);
  spinTrainIconX->setValue(0);
  spinTrainIconX->setSingleStep(1);
  spinTrainIconY = new QSpinBox(); //(new SpinnerNumberModel(0, 0, 10000, 1));
  spinTrainIconY->setMinimum(0);
  spinTrainIconY->setMaximum(10000);
  spinTrainIconY->setValue(0);
  spinTrainIconY->setSingleStep(1);

  _routeIndex = 0;
  _tIon = NULL;

  // create route
  if (routeName == NULL) {
      return;
  }
  _route = RouteManager::instance()->getRouteByName(routeName);
  //_route.addPropertyChangeListener(this);
  connect(_route->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));

  // general GUI config
  //getContentPane().setLayout(new BoxLayout(getContentPane(), BoxLayout.Y_AXIS));
  getContentPane()->setLayout(new QVBoxLayout);

  // set tool tips
  placeButton->setToolTip(tr("Places a set of train icons on panel") + " " + Setup::getPanelName());
  applyButton->setToolTip(tr("Update route with the new coordinates"));
  saveButton->setToolTip(tr("Saves the train icon coordinates for the selected location"));

  // Set up the panels
  QGroupBox* pRoute = new QGroupBox();
//  pRoute.setBorder(BorderFactory
//          .createTitledBorder(tr("Route") + " " + _route.getName()));
  pRoute->setStyleSheet(gbStyleSheet);
  pRoute->setTitle(tr("Route"));
  pRoute->setLayout(new GridBagLayout());
  addItem(pRoute, previousButton, 0, 0);
  addItem(pRoute, routeLocationName, 1, 0);
  addItem(pRoute, nextButton, 2, 0);

  QGroupBox* pSpin = new QGroupBox();
  pSpin->setLayout(new GridBagLayout());
  //pSpin.setBorder(BorderFactory.createTitledBorder(tr("TrainIcon")));
  pSpin->setStyleSheet(gbStyleSheet);
  pSpin->setTitle(tr("Train Icon"));
  addItem(pSpin, textX, 0, 0);
  addItem(pSpin, spinTrainIconX, 1, 0);
  addItem(pSpin, textY, 2, 0);
  addItem(pSpin, spinTrainIconY, 3, 0);

  QGroupBox* pControl = new QGroupBox();
  pControl->setLayout(new GridBagLayout());
  //pControl.setBorder(BorderFactory.createTitledBorder(""));
  pControl->setStyleSheet(gbStyleSheet);
  pControl->setTitle(tr("Route"));
  addItem(pControl, placeButton, 0, 0);
  addItem(pControl, applyButton, 1, 0);
  addItem(pControl, saveButton, 2, 0);

  getContentPane()->layout()->addWidget(pRoute);
  getContentPane()->layout()->addWidget(pSpin);
  getContentPane()->layout()->addWidget(pControl);

  // add help menu to window
  addHelpMenu("package.jmri.jmrit.operations.Operations_SetTrainIconCoordinates", true); // NOI18N

  // setup buttons
  addButtonAction(previousButton);
  addButtonAction(nextButton);
  addButtonAction(placeButton);
  addButtonAction(applyButton);
  addButtonAction(saveButton);

  // start off with save button disabled
  saveButton->setEnabled(false);

  updateRoute();

  // setup spinners
  addSpinnerChangeListerner(spinTrainIconX);
  addSpinnerChangeListerner(spinTrainIconY);

  initMinimumSize(QSize(Control::panelWidth400, Control::panelHeight400));

 }

 /*public*/ void SetTrainIconRouteFrame::buttonActionPerformed(QWidget* ae) {
 QPushButton* source = (QPushButton*)ae;
     if (source == previousButton) {
         updateRouteLocation(BACK);
     }
     if (source == nextButton) {
         updateRouteLocation(FORWARD);
     }
     if (source == placeButton) {
         placeTestIcons();
     }
     if (source == applyButton) {
         if (value != QMessageBox::Yes) {
//             value = JOptionPane.showConfirmDialog(NULL, MessageFormat.format(Bundle
//                     .getMessage("UpdateTrainIconRoute"), new Object[]{_route.getName()}), Bundle
//                     .getMessage("DoYouWantThisRoute"), JOptionPane.YES_NO_OPTION);
          value = QMessageBox::question(NULL, tr("Do you want to update this route?"), tr("Update train icon coordinates for route \"%1\"?").arg(_route->getName()), QMessageBox::Yes | QMessageBox::No);
         }
         if (value == QMessageBox::Yes) {
             saveButton->setEnabled(true);
         }
         updateTrainIconCoordinates();
     }
     if (source == saveButton) {
         RouteManagerXml::instance()->writeOperationsFile();
         if (Setup::isCloseWindowOnSaveEnabled()) {
             dispose();
         }
     }
 }

 /*public*/ void SetTrainIconRouteFrame::spinnerChangeEvent(QWidget* ae) {
 QSpinBox* source = (QSpinBox*)ae;
     if (source == spinTrainIconX && _tIon != NULL) {
         _tIon->setLocation( (int)spinTrainIconX->value(), (int)((Positionable*)_tIon)->getLocation().y()); // unsure whether this should be Positionable location or widget location?
     }
     if (source == spinTrainIconY && _tIon != NULL) {
         _tIon->setLocation((int)((Positionable*)_tIon)->getLocation().x(),  spinTrainIconY->value());
     }
 }

 /*private*/ void SetTrainIconRouteFrame::loadSpinners(RouteLocation* rl) {
     log->debug(tr("Load spinners route location %1").arg(rl->getName()));
     spinTrainIconX->setValue(rl->getTrainIconX());
     spinTrainIconY->setValue(rl->getTrainIconY());
 }

 // place test markers on panel
 /*private*/ void SetTrainIconRouteFrame::placeTestIcons() {
     Editor* editor = ((PanelMenu*)InstanceManager::getDefault("PanelMenu"))->getEditorByName(Setup::getPanelName());
     if (editor == NULL) {
//         JOptionPane.showMessageDialog(this, MessageFormat.format(tr("LoadPanel"),
//                 new Object[]{Setup.getPanelName()}), tr("PanelNotFound"),
//                 JOptionPane.ERROR_MESSAGE);
      QMessageBox::critical(this, tr("Panel not found!"), tr("You need to load panel \"%1\"").arg(Setup::getPanelName()));
     } else {
         if (_tIon != NULL) {
             _tIon->remove();
         }
         // icon
         _tIon = editor->addTrainIcon(_rl->getName());
// TODO:        _tIon->getTooltip().setText(_route->getName());
//         _tIon->getTooltip().setBackgroundColor(QColor(Qt::white));
         _tIon->setLocation(_rl->getTrainIconX(), _rl->getTrainIconY());
         setTrainIconNameAndColor();
         addIconListener(_tIon);
     }
 }

 /*private*/ void SetTrainIconRouteFrame::setTrainIconNameAndColor() {
     if (_tIon == NULL) {
         return;
     }
     _tIon->setText(_rl->getName());
     // set color based on train direction at current location
     if (_rl->getTrainDirection() == RouteLocation::NORTH) {
         _tIon->setLocoColor(Setup::getTrainIconColorNorth());
     }
     if (_rl->getTrainDirection() == RouteLocation::SOUTH) {
         _tIon->setLocoColor(Setup::getTrainIconColorSouth());
     }
     if (_rl->getTrainDirection() == RouteLocation::EAST) {
         _tIon->setLocoColor(Setup::getTrainIconColorEast());
     }
     if (_rl->getTrainDirection() == RouteLocation::WEST) {
         _tIon->setLocoColor(Setup::getTrainIconColorWest());
     }
 }

 /*private*/ void SetTrainIconRouteFrame::updateRoute() {
     log->debug("Updating route");
     _routeList = _route->getLocationsBySequenceList();
     updateRouteLocation(NONE);
 }

 /*private*/ void SetTrainIconRouteFrame::updateRouteLocation(int direction) {
     if (direction == FORWARD) {
         _routeIndex++;
     }
     if (direction == BACK) {
         _routeIndex--;
     }
     // Confirm that index is in range
     if (_routeIndex > _routeList->size() - 1) {
         _routeIndex = _routeList->size() - 1;
     }
     if (_routeIndex < 0) {
         _routeIndex = 0;
     }

     if (_rl != NULL) {
         //_rl->removePropertyChangeListener(this);
         disconnect(_rl->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));
     }
     if (_routeList->size() > 0) {
         _rl = _routeList->at(_routeIndex);
     }
     if (_rl != NULL) {
         //_rl->addPropertyChangeListener(this);
      connect(_rl->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));
         loadSpinners(_rl);
         routeLocationName->setText(_rl->getName());
     }
     setTrainIconNameAndColor();
 }

 /*private*/ void SetTrainIconRouteFrame::updateTrainIconCoordinates() {
     if (_rl != NULL) {
         //_rl->removePropertyChangeListener(this);
      disconnect(_rl->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));
         _rl->setTrainIconX( spinTrainIconX->value());
         _rl->setTrainIconY( spinTrainIconY->value());
         //_rl->addPropertyChangeListener(this);
         connect(_rl->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(propertyChange(PropertyChangeEvent*)));
     }
 }

 /*private*/ void SetTrainIconRouteFrame::addIconListener(TrainIcon* /*tI*/) {
#if 0 // TODO:
     tI.addComponentListener(new ComponentListener() {
         /*public*/ void componentHidden(java.awt.event.ComponentEvent e) {
         }

         /*public*/ void componentShown(java.awt.event.ComponentEvent e) {
         }

         /*public*/ void componentMoved(java.awt.event.ComponentEvent e) {
             trainIconMoved(e);
         }

         /*public*/ void componentResized(java.awt.event.ComponentEvent e) {
         }
     });
#endif
 }
#if 0
 protected void trainIconMoved(java.awt.event.ComponentEvent ae) {
     if (source == _tIon) {
         log.debug("train icon X: {} Y: {}", _tIon.getLocation().x, _tIon.getLocation().y);
         spinTrainIconX.setValue(_tIon.getLocation().x);
         spinTrainIconY.setValue(_tIon.getLocation().y);
     }
 }

 private void removeIcons() {
     if (_tIon != NULL) {
         _tIon.remove();
     }
 }

 /*public*/ void dispose() {
     removeIcons();
     _route.removePropertyChangeListener(this);
     if (_rl != NULL) {
         _rl->removePropertyChangeListener(this);
     }
     super.dispose();
 }
#endif
 /*public*/ void SetTrainIconRouteFrame::propertyChange(PropertyChangeEvent* e)
 {
  if(log->isDebugEnabled())
  {
   log->debug(tr("Property change: (%1) old: (%2) new: (%3)").arg(e->getPropertyName()).arg(e->getOldValue().toString()).arg(e
        ->getNewValue().toString()));
     }
     if (e->getSource()==(_route)) {
         updateRoute();
     }
     if (e->getSource()==(_rl)) {
         updateRouteLocation(NONE);
     }
 }

 /*public*/ QString SetTrainIconRouteFrame::getClassName()
 {
  return "jmri.jmrit.operations.routes.tools.SetTrainIconRouteFrame";
 }

}
