#include "layoutslip.h"
#include "layoutturnout.h"
#include "layoutblock.h"
#include "inputdialog.h"
#include <QMessageBox>
#include "editturnout.h"
#include "jmrijframe.h"
#include "jmribeancombobox.h"
#include "jtextfield.h"
#include "signalmastmanager.h"
#include "signalmastlogicmanager.h"
#include "createeditblock.h"
#include "mathutil.h"
#include "signallingguitools.h"
#include "layouttrackeditors.h"

//LayoutSlip::LayoutSlip(QObject *parent) :
//    LayoutTurnout(parent)
//{
//}
///*final*/ /*public*/ const /*static*/ int LayoutSlip::UNKNOWN = 0x01;
///*final*/ /*public*/ const /*static*/ int LayoutSlip::STATE_AC = 0x02;
///*final*/ /*public*/ const /*static*/ int LayoutSlip::STATE_BD = 0x04;
///*final*/ /*public*/ const /*static*/ int LayoutSlip::STATE_AD = 0x06;
///*final*/ /*public*/ const /*static*/ int LayoutSlip::STATE_BC = 0x08;


QComboBox* TurnoutState::getComboA()
{
 if(turnoutABox==nullptr)
 {
  QStringList state;
  state << ((ProxyTurnoutManager*)InstanceManager::turnoutManagerInstance())->getClosedText() << ((ProxyTurnoutManager*)InstanceManager::turnoutManagerInstance())->getThrownText();
  turnoutABox = new QComboBox();
  turnoutABox->addItems(state);
  if(turnoutA == Turnout::THROWN)
   turnoutABox->setCurrentIndex(1);
 }
 return turnoutABox;
}

QComboBox* TurnoutState::getComboB()
{
 if(turnoutBBox==nullptr)
 {
  QStringList state;
  state << ((ProxyTurnoutManager*)InstanceManager::turnoutManagerInstance())->getClosedText() << ((ProxyTurnoutManager*)InstanceManager::turnoutManagerInstance())->getThrownText();
  turnoutBBox = new QComboBox();
  turnoutBBox->addItems(state);
  if(turnoutB == Turnout::THROWN)
   turnoutBBox->setCurrentIndex(1);
 }
 return turnoutBBox;
}

/**
 * A LayoutSlip is two track segment on a layout that cross at an angle.
 * <P>
 * A LayoutSlip has four connection points, designated A, B, C, and D.
 *		At the crossing, A-C and B-D are straight segments.  A train proceeds
 *		through the crossing on either of these segments.
 * <P>
 * For drawing purposes, each LayoutSlip carries a center point and displacements
 *		for A and B.  The displacements for C = - the displacement for A, and the
 *		displacement for D = - the displacement for B.  The center point and these
 *      displacements may be adjusted by the user when in edit mode.
 * <P>
 * When LayoutSlips are first created, there are no connections.  Block information
 *		and connections are added when available.
 * <P>
 * Signal Head names are saved here to keep track of where signals are. LayoutSlip
 *		only serves as a storage place for signal head names. The names are placed here
 *		by Set Signals at Level Crossing in Tools menu.
 *
 * @author Dave Duchamp Copyright (c) 2004-2007
 * @version $Revision: 19729 $
 */

///*public*/ class LayoutSlip extends LayoutTurnout
//{

//	// Defined text resource
//	ResourceBundle rb = ResourceBundle.getBundle("jmri.jmrit.display.layoutEditor.LayoutEditorBundle");

/**
 * constructor method
 */
/*public*/ LayoutSlip::LayoutSlip(QString id, QPointF c, double rot, LayoutEditor* myPanel, int type) : LayoutTurnout(id, -1, c,rot,0,0,myPanel)
{
 init();
 //instance = this;
 layoutEditor = myPanel;
 //ident = id;
 //center = c;
 dispC =  QPointF(-20.0,0.0);
 dispB =  QPointF(-14.0,14.0);
 setTurnoutType(type);
 rotateCoords(rot);
}
void LayoutSlip::init()
{
 turnoutBName="";
 namedTurnoutB = nullptr;
 mTurnoutListener = nullptr;
 popup = nullptr;
 tools = nullptr;
 testState = UNKNOWN;
 active = true;
 turnoutStates =  QHash<int, TurnoutState*>();
 currentState = UNKNOWN;
 connectA = nullptr;
 connectB = nullptr;
 connectC = nullptr;
 connectD = nullptr;
 needRedraw = false;
 needsBlockUpdate = false;
 namedTurnout = nullptr;
 active = false;
 sensorANamed =nullptr;
 sensorBNamed = nullptr;
 sensorCNamed = nullptr;
 sensorDNamed = nullptr;
 rects = nullptr;
 editAdditionalMenu = new QVector<QObject*>();
 viewAdditionalMenu = new QVector<QObject*>();
}

/*public*/ void LayoutSlip::setTurnoutType(int slipType)
{
 setSlipType(slipType);
 type = slipType;
}

/*public*/ void LayoutSlip::setSlipType(int slipType)
{
 if(type==slipType)
  return;
 type=slipType;
 if(type==DOUBLE_SLIP)
 {
  turnoutStates.insert(STATE_AC, new TurnoutState(Turnout::CLOSED, Turnout::CLOSED));
  turnoutStates.insert(STATE_BD, new TurnoutState(Turnout::THROWN, Turnout::THROWN));
  turnoutStates.insert(STATE_AD, new TurnoutState(Turnout::CLOSED, Turnout::THROWN));
  turnoutStates.insert(STATE_BC, new TurnoutState(Turnout::THROWN, Turnout::CLOSED));
 }
 else
 {
  turnoutStates.insert(STATE_AC, new TurnoutState(Turnout::CLOSED, Turnout::THROWN));
  turnoutStates.insert(STATE_BD, new TurnoutState(Turnout::THROWN, Turnout::CLOSED));
  turnoutStates.insert(STATE_AD, new TurnoutState(Turnout::THROWN, Turnout::THROWN));
  turnoutStates.remove(STATE_BC);
 }
}

/*public*/ int LayoutSlip::getSlipType()
{
 return type;
}

/*public*/ int LayoutSlip::getSlipState()
{
 return currentState;
}

/*public*/ QString LayoutSlip::getTurnoutBName()
{
 if (namedTurnoutB!=nullptr)
     return namedTurnoutB->getName();
 return turnoutBName;
}

/*public*/ Turnout* LayoutSlip::getTurnoutB()
{
 if (namedTurnoutB==nullptr)
 {
  // set physical turnout if possible and needed
  setTurnoutB(turnoutBName);
  if (namedTurnoutB==nullptr)
   return nullptr;
 }
 return namedTurnoutB->getBean();
}

/*public*/ void LayoutSlip::setTurnoutB(QString tName)
{
 if (namedTurnoutB!=nullptr)
  deactivateTurnout();
 turnoutBName = tName;
 Turnout* turnout = ((ProxyTurnoutManager*)InstanceManager::turnoutManagerInstance())->
                     getTurnout(turnoutBName);
 if (turnout!=nullptr)
 {
  namedTurnoutB =((NamedBeanHandleManager*) InstanceManager::getDefault("NamedBeanHandleManager"))->getNamedBeanHandle(turnoutBName, turnout);
  activateTurnout();
 }
 else
 {
  turnoutBName = "";
  namedTurnoutB = nullptr;
 }
}

//@Override
/*public*/ LayoutTrack *LayoutSlip::getConnection(int location) throw (JmriException) {
    switch (location) {
        case SLIP_A:
            return connectA;
        case SLIP_B:
            return connectB;
        case SLIP_C:
            return connectC;
        case SLIP_D:
            return connectD;
    }
    log.error("Invalid Point Type " + QString::number(location)); //I18IN
    throw new JmriException("Invalid Point");
}

//@Override
/*public*/ void LayoutSlip::setConnection(int location, LayoutTrack* o, int type) throw (JmriException) {
    if ((type != TRACK) && (type != NONE)) {
        log.error("unexpected type of connection to layoutturnout - " + QString::number(type));
        throw JmriException("unexpected type of connection to layoutturnout - " + QString::number(type));
    }
    switch (location) {
        case SLIP_A:
            connectA = o;
            break;
        case SLIP_B:
            connectB = o;
            break;
        case SLIP_C:
            connectC = o;
            break;
        case SLIP_D:
            connectD = o;
            break;
        default:
            log.error("Invalid Point Type " + location); //I18IN
            throw JmriException("Invalid Point");
    }
}

/*public*/ QString LayoutSlip::getDisplayName()
{
 QString name = "Slip :";
 if(getTurnoutName()!=nullptr)
 {
  name += "("+getTurnoutName();
  if(getTurnoutBName()!=nullptr)
   name+=":"+getTurnoutBName()+")";
 }
 else if(getTurnoutBName()!=nullptr)
 {
  name += "("+getTurnoutBName()+")";
 }
 return name;
}

/**
 * Toggle slip states if clicked on, physical turnout exists, and not
 * disabled
 */
/*public*/ void LayoutSlip::toggleState() {
    switch (currentState) {
        case STATE_AC:
            if (singleSlipStraightEqual()) {
                setTurnoutState(turnoutStates.value(STATE_AD));
                currentState = STATE_AD;
            } else {
                setTurnoutState(turnoutStates.value(STATE_BD));
                currentState = STATE_BD;
            }
            break;
        case STATE_BD:
            setTurnoutState(turnoutStates.value(STATE_AD));
            currentState = STATE_AD;
            break;
        case STATE_AD:
            if (type == SINGLE_SLIP) {
                setTurnoutState(turnoutStates.value(STATE_AC));
                currentState = STATE_AC;
            } else {
                setTurnoutState(turnoutStates.value(STATE_BC));
                currentState = STATE_BC;
            }
            break;
        case STATE_BC:
            setTurnoutState(turnoutStates.value(STATE_AC));
            currentState = STATE_AC;
            break;
        default:
            setTurnoutState(turnoutStates.value(STATE_BD));
            currentState = STATE_BD;
            break;
    }

}

void LayoutSlip::setTurnoutState(TurnoutState* ts)
{
 if(getTurnout()!=nullptr)
     getTurnout()->setCommandedState(ts->getTurnoutAState());
 if(getTurnoutB()!=nullptr)
     getTurnoutB()->setCommandedState(ts->getTurnoutBState());
}

/**
 * Activate/Deactivate turnout to redraw when turnout state changes
 */
/*private*/ void LayoutSlip::activateTurnout()
{
 if (namedTurnout!=nullptr)
 {
//        namedTurnout->getBean()->addPropertyChangeListener(mTurnoutListener =
//                            new PropertyChangeListener());
//        {
//            /*public*/ void propertyChange(PropertyChangeEvent* e) {
//                updateState();
//            }
//        }, namedTurnoutgetName(), "Layout Editor Slip");
     AbstractTurnout* t = (AbstractTurnout* )namedTurnout->getBean();
     connect(t->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(updateState()));
    }
    if (namedTurnoutB!=nullptr)
    {
//        namedTurnoutB->getBean()->addPropertyChangeListener(mTurnoutListener =
//                            new PropertyChangeListener());
//        {
//            /*public*/ void propertyChange(java.beans.PropertyChangeEvent e) {
//                updateState();
//            }
//        }, namedTurnoutB.getName(), "Layout Editor Slip");
        AbstractTurnout* t = (AbstractTurnout*)namedTurnoutB->getBean();
        connect(t->pcs, SIGNAL(propertyChange(PropertyChangeEvent*)), this, SLOT(updateState()));

    }
}
/*private*/ void LayoutSlip::deactivateTurnout()
{
    if (mTurnoutListener!=nullptr) {
        namedTurnout->getBean()->removePropertyChangeListener(mTurnoutListener);
        if(namedTurnoutB!=nullptr){
            namedTurnoutB->getBean()->removePropertyChangeListener(mTurnoutListener);
        }
        mTurnoutListener = nullptr;
    }
}

/*public*/ QPointF LayoutSlip::getCoordsCenter() {return center;}
/*public*/ QPointF LayoutSlip::getCoordsA() {
    double x = center.x() + dispC.x();
    double y = center.y() + dispC.y();
    return  QPointF(x,y);
}
/*public*/ QPointF LayoutSlip::getCoordsB() {
    double x = center.x() + dispB.x();
    double y = center.y() + dispB.y();
    return  QPointF(x,y);
}
/*public*/ QPointF LayoutSlip::getCoordsC() {
    double x = center.x() - dispC.x();
    double y = center.y() - dispC.y();
    return  QPointF(x,y);
}
/*public*/ QPointF LayoutSlip::getCoordsD() {
    double x = center.x() - dispB.x();
    double y = center.y() - dispB.y();
    return  QPointF(x,y);
}
/*protected*/ QPointF LayoutSlip::getCoordsLeft() {
    QPointF leftCenter = MathUtil::midPoint(getCoordsA(), getCoordsB());
    double circleRadius = LayoutEditor::SIZE * layoutEditor->getTurnoutCircleSize();
    double leftFract = circleRadius / MathUtil::distance(center, leftCenter);//center.distance(leftCenter);
    return MathUtil::lerp(center, leftCenter, leftFract);
}

/*protected*/ QPointF LayoutSlip::getCoordsRight() {
    QPointF rightCenter = MathUtil::midPoint(getCoordsC(), getCoordsD());
    double circleRadius = LayoutEditor::SIZE * layoutEditor->getTurnoutCircleSize();
    double rightFract = circleRadius / MathUtil::distance(center, rightCenter);//center.distance(rightCenter);
    return MathUtil::lerp(center, rightCenter, rightFract);
}
/**
 * return the coordinates for the specified connection type
 *
 * @param connectionType the connection type
 * @return the QPointF coordinates
 */
//@Override
/*public*/ QPointF LayoutSlip::getCoordsForConnectionType(int connectionType) {
    QPointF result = center;
    switch (connectionType) {
        case SLIP_A:
            result = getCoordsA();
            break;
        case SLIP_B:
            result = getCoordsB();
            break;
        case SLIP_C:
            result = getCoordsC();
            break;
        case SLIP_D:
            result = getCoordsD();
            break;
        case SLIP_CENTER:
            break;
        case SLIP_LEFT:
            result = getCoordsLeft();
            break;
        case SLIP_RIGHT:
            result = getCoordsRight();
            break;
        default:
            log.error("Invalid connection type " + QString(connectionType)); //I18IN
    }
    return result;
}
/*private*/ void LayoutSlip::updateBlockInfo()
{
 LayoutBlock* b1 = nullptr;
 LayoutBlock* b2 = nullptr;
 if (block!=nullptr) block->updatePaths();
 if (connectA!=nullptr) {
     b1 = ((TrackSegment*)connectA)->getLayoutBlock();
     if ((b1!=nullptr)&&(b1!=block)) b1->updatePaths();
 }
 if (connectC!=nullptr)
 {
     b2 = ((TrackSegment*)connectC)->getLayoutBlock();
     if ((b2!=nullptr)&&(b2!=block)&&(b2!=b1)) b2->updatePaths();
 }

 if (connectB!=nullptr)
 {
     b1 = ((TrackSegment*)connectB)->getLayoutBlock();
     if ((b1!=nullptr)&&(b1!=block)) b1->updatePaths();
 }
 if (connectD!=nullptr)
 {
     b2 = ((TrackSegment*)connectD)->getLayoutBlock();
     if ((b2!=nullptr)&&(b2!=block)&&(b2!=b1)) b2->updatePaths();
 }
 reCheckBlockBoundary();
}
/*
    this is used by ConnectivityUtil to determine the turnout state necessary to get from prevLayoutBlock ==> currLayoutBlock ==> nextLayoutBlock
 */
//@Override
/*protected*/ int LayoutSlip::getConnectivityStateForLayoutBlocks(
        /*@Nullable*/ LayoutBlock* thisLayoutBlock,
        /*@Nullable*/ LayoutBlock* prevLayoutBlock,
        /*@Nullable*/ LayoutBlock* nextLayoutBlock,
        bool suppress) {
    int result = Turnout::UNKNOWN;
    LayoutBlock* layoutBlockA = ((TrackSegment*) getConnectA())->getLayoutBlock();
    LayoutBlock* layoutBlockB = ((TrackSegment*) getConnectB())->getLayoutBlock();
    LayoutBlock* layoutBlockC = ((TrackSegment*) getConnectC())->getLayoutBlock();
    LayoutBlock* layoutBlockD = ((TrackSegment*) getConnectD())->getLayoutBlock();

    if (layoutBlockA == thisLayoutBlock) {
        if (layoutBlockC == nextLayoutBlock || layoutBlockC == prevLayoutBlock) {
            result = LayoutSlip::STATE_AC;
        } else if (layoutBlockD == nextLayoutBlock || layoutBlockD == prevLayoutBlock) {
            result = LayoutSlip::STATE_AD;
        } else if (layoutBlockC == thisLayoutBlock) {
            result = LayoutSlip::STATE_AC;
        } else if (layoutBlockD == thisLayoutBlock) {
            result = LayoutSlip::STATE_AD;
        }
    } else if (layoutBlockB == thisLayoutBlock) {
        if (getTurnoutType() == LayoutSlip::DOUBLE_SLIP) {
            if (layoutBlockD == nextLayoutBlock || layoutBlockD == prevLayoutBlock) {
                result = LayoutSlip::STATE_BD;
            } else if (layoutBlockC == nextLayoutBlock || layoutBlockC == prevLayoutBlock) {
                result = LayoutSlip::STATE_BC;
            } else if (layoutBlockD == thisLayoutBlock) {
                result = LayoutSlip::STATE_BD;
            } else if (layoutBlockC == thisLayoutBlock) {
                result = LayoutSlip::STATE_BC;
            }
        } else {
            if (layoutBlockD == nextLayoutBlock || layoutBlockD == prevLayoutBlock) {
                result = LayoutSlip::STATE_BD;
            } else if (layoutBlockD == thisLayoutBlock) {
                result = LayoutSlip::STATE_BD;
            }
        }
    } else if (layoutBlockC == thisLayoutBlock) {
        if (getTurnoutType() == LayoutSlip::DOUBLE_SLIP) {
            if (layoutBlockA == nextLayoutBlock || layoutBlockA == prevLayoutBlock) {
                result = LayoutSlip::STATE_AC;
            } else if (layoutBlockB == nextLayoutBlock || layoutBlockB == prevLayoutBlock) {
                result = LayoutSlip::STATE_BC;
            } else if (layoutBlockA == thisLayoutBlock) {
                result = LayoutSlip::STATE_AC;
            } else if (layoutBlockB == thisLayoutBlock) {
                result = LayoutSlip::STATE_BC;
            }
        } else {
            if (layoutBlockA == nextLayoutBlock || layoutBlockA == prevLayoutBlock) {
                result = LayoutSlip::STATE_AC;
            } else if (layoutBlockA == thisLayoutBlock) {
                result = LayoutSlip::STATE_AC;
            }
        }
    } else if (layoutBlockD == thisLayoutBlock) {
        if (layoutBlockA == nextLayoutBlock || layoutBlockA == prevLayoutBlock) {
            result = LayoutSlip::STATE_AD;
        } else if (layoutBlockB == nextLayoutBlock || layoutBlockB == prevLayoutBlock) {
            result = LayoutSlip::STATE_BD;
        } else if (layoutBlockA == thisLayoutBlock) {
            result = LayoutSlip::STATE_AD;
        } else if (layoutBlockB == thisLayoutBlock) {
            result = LayoutSlip::STATE_AD;
        }
    } else {
        result = LayoutSlip::UNKNOWN;
    }
    if (!suppress && (result == LayoutSlip::UNKNOWN)) {
        log.error("Cannot determine slip setting for " + getName());
    }
    return result;
}   // getConnectivityStateForLayoutBlocks

/*public*/ void LayoutSlip::reCheckBlockBoundary(){
    if(connectA==nullptr && connectB==nullptr && connectC==nullptr && connectD==nullptr){
        //This is no longer a block boundary, therefore will remove signal masts and sensors if present
        if(getSignalAMastName()!=(""))
            removeSML(getSignalAMastName());
        if(getSignalBMastName()!=(""))
            removeSML(getSignalBMastName());
        if(getSignalCMastName()!=(""))
            removeSML(getSignalCMastName());
        if(getSignalDMastName()!=(""))
            removeSML(getSignalDMastName());
        setSignalAMast("");
        setSignalBMast("");
        setSignalCMast("");
        setSignalDMast("");
        setSensorA("");
        setSensorB("");
        setSensorC("");
        setSensorD("");
        //May want to look at a method to remove the assigned mast from the panel and potentially any logics generated
    }  else if(connectA==nullptr || connectB==nullptr || connectC==nullptr || connectD==nullptr){
        //could still be in the process of rebuilding the point details
        return;
    }

    TrackSegment* trkA;
    TrackSegment* trkB;
    TrackSegment* trkC;
    TrackSegment* trkD;

    //if(connectA instanceof TrackSegment){
    if(qobject_cast<TrackSegment*>(connectA)!= nullptr)
    {
        trkA = (TrackSegment*)connectA;
        if(trkA->getLayoutBlock()==block)
        {
            if(getSignalAMastName()!=(""))
                removeSML(getSignalAMastName());
            setSignalAMast("");
            setSensorA("");
        }
    }
    //if(connectC instanceof TrackSegment) {
    if(qobject_cast<TrackSegment*>(connectC)!= nullptr)
    {
        trkC = (TrackSegment*)connectC;
        if(trkC->getLayoutBlock()==block){
            if(getSignalCMastName()!=(""))
                removeSML(getSignalCMastName());
            setSignalCMast("");
            setSensorC("");
        }
    }
    //if(connectB instanceof TrackSegment){
    if(qobject_cast<TrackSegment*>(connectB)!= nullptr)
    {
        trkB = (TrackSegment*)connectB;
        if(trkB->getLayoutBlock()==block){
            if(getSignalBMastName()!=(""))
                removeSML(getSignalBMastName());
            setSignalBMast("");
            setSensorB("");
        }
    }

    //if(connectD instanceof TrackSegment) {
    if(qobject_cast<TrackSegment*>(connectD)!= nullptr)
    {
        trkD = (TrackSegment*)connectC;
        if(trkD->getLayoutBlock()==block){
            if(getSignalDMastName()!=(""))
                removeSML(getSignalDMastName());
            setSignalDMast("");
            setSensorD("");
        }
    }
}

void LayoutSlip::removeSML(QString signalMast){
#if 1 // TODO:
    if(signalMast==nullptr || signalMast == (""))
        return;
    SignalMast* mast = ((SignalMastManager*)InstanceManager::getDefault("SignalMastManager"))->getSignalMast(signalMast);
    if(static_cast<LayoutBlockManager*>(InstanceManager::getDefault("LayutBlockManager"))->isAdvancedRoutingEnabled() && InstanceManager::signalMastLogicManagerInstance()->isSignalMastUsed(mast)){
        InstanceManager::signalMastLogicManagerInstance()->disableLayoutEditorUse(mast);
        SignallingGuiTools::removeSignalMastLogic(nullptr, mast);
    }
#endif
}

/**
 * Methods to test if mainline track or not
 *  Returns true if either connecting track segment is mainline
 *  Defaults to not mainline if connecting track segments are missing
 */
/*public*/ bool LayoutSlip::isMainline() {
    if ( ((connectA != nullptr) && (((TrackSegment*)connectA)->getMainline())) ||
        ((connectB != nullptr) && (((TrackSegment*)connectB)->getMainline())) ) {
        return true;
    }
    else {
        return false;
    }
}

/**
 * {@inheritDoc}
 */
//@Override
/*protected*/ int LayoutSlip::findHitPointType(/*@Nonnull*/ QPointF hitPoint, bool useRectangles, bool requireUnconnected) {
    int result = NONE;  // assume point not on connection

    if (!requireUnconnected) {
        // calculate radius of turnout control circle
        double circleRadius = LayoutEditor::SIZE * layoutEditor->getTurnoutCircleSize();

        // get left and right centers
        QPointF leftCenter = getCoordsLeft();
        QPointF rightCenter = getCoordsRight();

        if (useRectangles) {
            // calculate turnout's left control rectangle
            QRectF leftRectangle = layoutEditor->trackControlCircleRectAt(leftCenter);
            if (leftRectangle.contains(hitPoint)) {
                //point is in this turnout's left control rectangle
                result = SLIP_LEFT;
            }
            QRectF rightRectangle = layoutEditor->trackControlCircleRectAt(rightCenter);
            if (rightRectangle.contains(hitPoint)) {
                //point is in this turnout's right control rectangle
                result = SLIP_RIGHT;
            }
        } else {
            //check east/west turnout control circles
            double leftDistance = MathUtil::distance(hitPoint,leftCenter);
            double rightDistance = MathUtil::distance(hitPoint, rightCenter);

            if ((leftDistance <= circleRadius) || (rightDistance <= circleRadius)) {
                //mouse was pressed on this slip
                result = (leftDistance < rightDistance) ? SLIP_LEFT : SLIP_RIGHT;
            }
        }
    }

    // have we found anything yet?
    if (result == NONE) {
        // rather than create rectangles for all the points below and
        // see if the passed in point is in one of those rectangles
        // we can create a rectangle for the passed in point and then
        // test if any of the points below are in that rectangle instead.
        QRectF r = layoutEditor->trackEditControlRectAt(hitPoint);

        if (!requireUnconnected || (getConnectA() == nullptr)) {
            //check the A connection point
            if (r.contains(getCoordsA())) {
                result = SLIP_A;
            }
        }

        if (!requireUnconnected || (getConnectB() == nullptr)) {
            //check the B connection point
            if (r.contains(getCoordsB())) {
                result = SLIP_B;
            }
        }

        if (!requireUnconnected || (getConnectC() == nullptr)) {
            //check the C connection point
            if (r.contains(getCoordsC())) {
                result = SLIP_C;
            }
        }

        if (!requireUnconnected || (getConnectD() == nullptr)) {
            //check the D connection point
            if (r.contains(getCoordsD())) {
                result = SLIP_D;
            }
        }
    }
    return result;
}   // findHitPointType
/**
 * Modify coordinates methods
 */
/*public*/ void LayoutSlip::setCoordsCenter(QPointF p) {
    center = p;
}
/*public*/ void LayoutSlip::setCoordsA(QPointF p) {
    double x = center.x() - p.x();
    double y = center.y() - p.y();
    dispC =  QPoint(-x,-y);
}
/*public*/ void LayoutSlip::setCoordsB(QPointF p) {
    double x = center.x() - p.x();
    double y = center.y() - p.y();
    dispB =  QPointF(-x,-y);
}
/*public*/ void LayoutSlip::setCoordsC(QPointF p) {
    double x = center.x() - p.x();
    double y = center.y() - p.y();
    dispC =  QPointF(x,y);
}
/*public*/ void LayoutSlip::setCoordsD(QPointF p) {
    double x = center.x() - p.x();
    double y = center.y() - p.y();
    dispB =  QPointF(x,y);
}
/*public*/ void LayoutSlip::scaleCoords(float xFactor, float yFactor) {
    QPointF pt =  QPointF(round(center.x()*xFactor),
                                    round(center.y()*yFactor));
    center = pt;
    pt =  QPointF(round(dispC.x()*xFactor),
                                    round(dispC.y()*yFactor));
    dispC = pt;
    pt =  QPointF(round(dispB.x()*xFactor),
                                    round(dispB.y()*yFactor));
    dispB = pt;
}
double LayoutSlip::round (double x) {
    int i = (int)(x+0.5);
    return i;
}

/**
 * Initialization method
 *   The above variables are initialized by PositionablePointXml, then the following
 *        method is called after the entire LayoutEditor is loaded to set the specific
 *        TrackSegment objects.
 */
/*public*/ void LayoutSlip::setObjects(LayoutEditor* p) {
    connectA = p->findTrackSegmentByName(connectAName);
    connectB = p->findTrackSegmentByName(connectBName);
    connectC = p->findTrackSegmentByName(connectCName);
    connectD = p->findTrackSegmentByName(connectDName);
    if (tBlockName.length()>0) {
        block = p->getLayoutBlock(tBlockName);
        if (block!=nullptr) {
            blockName = tBlockName;
            block->incrementUse();
        }
        else {
            log.error("bad blocknameac '"+tBlockName+"' in slip "+ident);
        }
    }
}

/**
 * Display popup menu for information and editing
 */
/*protected*/ QMenu *LayoutSlip::showPopup(QGraphicsSceneMouseEvent* /*e*/)
{
 if (popup != nullptr )
 {
  popup->clear();
 }
 else
 {
  popup = new QMenu();
 }
 if(layoutEditor->isEditable())
 {
  popup->addAction(new QAction(getName(),this));
  bool blockAssigned = false;
  if ( (blockName==nullptr) || (blockName==("")) )
   popup->addAction (new QAction(tr("NoBlock"),this));
  else
  {
   popup->addAction(new QAction(tr("Block ID")+": "+getLayoutBlock()->getID(), this));
   blockAssigned = true;
  }

  popup->addSeparator();
  QAction* editAction;
  popup->addAction(editAction =new QAction(tr("Edit"),this));
//        {
//                /*public*/ void actionPerformed(ActionEvent e) {
//                    editLayoutSlip(instance);
//                }
//            });
  connect(editAction, SIGNAL(triggered()), this, SLOT(OnEditAction()));
  QAction* removeAction;
  popup->addAction(removeAction = new QAction(tr("Remove"),this));
//        {
//                /*public*/ void actionPerformed(ActionEvent e) {
//                    if (layoutEditor.removeLayoutSlip(instance)) {
//                        // Returned true if user did not cancel
//                        remove();
//                        dispose();
//                    }
//                }
//            });
  connect(removeAction, SIGNAL(triggered()), this, SLOT(on_removeAction_triggered()));
  if ( (connectA==nullptr) && (connectB==nullptr) &&
                    (connectC==nullptr) && (connectD==nullptr) )
  {
   QAction* rotateItem = new QAction(tr("Rotate")+"...", this);
   popup->addAction(rotateItem);
   connect(rotateItem, SIGNAL(triggered()), this, SLOT(on_rotate_triggered()));
//            rotateItem.addActionListener(new ActionListener() {
//                /*public*/ void actionPerformed(ActionEvent event) {
//                    bool entering = true;
//                    bool error = false;
//                    QString newAngle = "";
//                    while (entering) {
//                        // prompt for rotation angle
//                        error = false;
//                        newAngle = JOptionPane.showInputDialog(layoutEditor,
//                                            tr("EnterRotation")+" :");
//                        if (newAngle.length()<1) return;  // cancelled
//                        double rot = 0.0;
//                        try {
//                            rot = Double.parseDouble(newAngle);
//                        }
//                        catch (Exception e) {
//                            JOptionPane.showMessageDialog(layoutEditor,tr("Error3")+
//                                " "+e,tr("Error"),JOptionPane.ERROR_MESSAGE);
//                            error = true;
//                            newAngle = "";
//                        }
//                        if (!error) {
//                            entering = false;
//                            if (rot!=0.0) {
//                               rotateCoords(rot);
//                               layoutEditor.redrawPanel();
//                            }
//                        }
//                    }
//                }
//            });
   }
#if 1
   if (blockAssigned)
   {
         //            popup.add(new AbstractAction(tr("SetSignals")) {
         //                /*public*/ void actionPerformed(ActionEvent e) {
         //                        if (tools == NULL) {
         //                            tools = new LayoutEditorTools(layoutEditor);
         //                        }
         //                    tools.setSlipFromMenu((LayoutSlip)instance,
         //                            layoutEditor.signalIconEditor,layoutEditor.signalFrame);
         //                }
         //            });
     AbstractAction* setSignals = new AbstractAction(tr("Set Signals"),this);
     popup->addAction(setSignals);
     connect(setSignals, SIGNAL(triggered()), this, SLOT(on_setSignalsAct_triggered()));
    }

    /*final*/ QVector<QString>* boundaryBetween = getBlockBoundaries();
    bool blockBoundaries = false;

    for (int i = 0; i<4; i++)
    {
     if(boundaryBetween->at(i) !=nullptr)
      blockBoundaries=true;
    }
    if (blockBoundaries)
    {
//             popup.add(new AbstractAction(tr("SetSignalMasts")) {
//                /*public*/ void actionPerformed(ActionEvent e) {
//                    if (tools == NULL) {
//                        tools = new LayoutEditorTools(layoutEditor);
//                    }
//                    tools.setSignalMastsAtSlipFromMenu((LayoutSlip)instance, boundaryBetween, layoutEditor.signalFrame);
//                }
//            });
     AbstractAction* setSignalMasts = new AbstractAction(tr("Set SignalMasts"), this);
     popup->addAction(setSignalMasts);
     connect(setSignalMasts, SIGNAL(triggered()), this, SLOT(on_setSignalMastsAct_triggered()));
//             popup.add(new AbstractAction(tr("SetSensors")) {
//                /*public*/ void actionPerformed(ActionEvent e) {
//                    if (tools == NULL) {
//                        tools = new LayoutEditorTools(layoutEditor);
//                    }
//                    tools.setSensorsAtSlipFromMenu((LayoutSlip)instance, boundaryBetween, layoutEditor.sensorIconEditor, layoutEditor.sensorFrame);
//                }
//            });
  AbstractAction* setSensors = new AbstractAction(tr("Set Sensors"),this);
  popup->addAction(setSensors);
  connect(setSensors, SIGNAL(triggered()), this, SLOT(on_setSensorsAct_triggered()));
 }
#endif
     //if (InstanceManager::layoutBlockManagerInstance().isAdvancedRoutingEnabled())
  if(layoutEditor->layoutBlockManager->isAdvancedRoutingEnabled())
  {
   if(blockAssigned)
   {
    QAction* routingAction;
    popup->addAction(routingAction = new QAction(tr("ViewBlockRouting"),this));
//             {
//                    /*public*/ void actionPerformed(ActionEvent e)
//                 {
//                        AbstractAction  routeTableAction = new  LayoutBlockRouteTableAction("ViewRouting", getLayoutBlock());
//                        routeTableAction.actionPerformed(e);
//                    }
//                });
    connect(routingAction, SIGNAL(triggered()), this, SLOT(OnRoutingAction()));
   }
  }
  setAdditionalEditPopUpMenu(popup);
  layoutEditor->setShowAlignmentMenu(popup);
  //popup->show(e.getComponent(), e.x(), e.y());
  popup->exec(QCursor::pos());
 }
 else if(!viewAdditionalMenu->isEmpty())
 {
  setAdditionalViewPopUpMenu(popup);
  //popup.show(e.getComponent(), e.x(), e.y());
  popup->exec(QCursor::pos());
 }
 return popup;
}
void LayoutSlip::OnEditAction()
{
 layoutEditor->getLayoutTrackEditors()->editLayoutSlip(this);
}

void LayoutSlip::on_setSignalsAct_triggered()
{
 if (tools == nullptr)
 {
  tools = new LayoutEditorTools(layoutEditor);
 }
 tools->setSlipFromMenu(this,
 layoutEditor->signalIconEditor,layoutEditor->signalFrame);
}

void LayoutSlip::on_setSignalMastsAct_triggered()
{
 if (tools == nullptr)
 {
  tools = new LayoutEditorTools(layoutEditor);
 }
 tools->setSignalMastsAtSlipFromMenu(this, boundaryBetween, layoutEditor->signalFrame);
}

void LayoutSlip::on_setSensorsAct_triggered()
{
 if (tools == nullptr)
 {
  tools = new LayoutEditorTools(layoutEditor);
 }
 tools->setSensorsAtSlipFromMenu(this, boundaryBetween, layoutEditor->sensorIconEditor, layoutEditor->sensorFrame);
}

void LayoutSlip::on_rotate_triggered()
{
    bool entering = true;
    bool error = false;
    QString newAngle = "";
    while (entering)
    {
     // prompt for rotation angle
     error = false;
     newAngle = InputDialog::showInputDialog(tr("EnterRotation Angle (degrees CW)")+" :", layoutEditor);
     if (newAngle.length()<1) return;  // cancelled
     double rot = 0.0;
     try
     {
      bool bOk=false;
      rot = newAngle.toDouble(&bOk);
      if(!bOk) throw new Exception;
     }
     catch (Exception e)
     {
   //   JOptionPane.showMessageDialog(layoutEditor,tr("Error3")+
   //            " "+e,tr("Error"),JOptionPane.ERROR_MESSAGE);
         QMessageBox::critical(layoutEditor, "Error", tr("Invalid value"));
      error = true;
      newAngle = "";
     }
     if (!error)
     {
      entering = false;
      if (rot!=0.0)
      {
       rotateCoords(rot);
       layoutEditor->redrawPanel();
      }
     }
    }

}

void LayoutSlip::on_removeAction_triggered()
{
 if (layoutEditor->removeLayoutSlip(this))
 {
    // Returned true if user did not cancel
    remove();
    dispose();
 }
}

/*public*/ QVector<QString>* LayoutSlip::getBlockBoundaries()
{
 /*final*/ QVector<QString>* boundaryBetween = new QVector<QString>(4);

 if ( (blockName!=nullptr) && (blockName!=("")) && (block!=nullptr) )
 {
  //if ((connectA instanceof TrackSegment) &&
  if((qobject_cast<TrackSegment*>(connectA)!=nullptr) &&
  (((TrackSegment*)connectA)->getLayoutBlock()!=block))
  {
   try
   {
    boundaryBetween->replace(0,((TrackSegment*)connectA)->getLayoutBlock()->getDisplayName()+ " - " + block->getDisplayName());
   }
   catch (NullPointerException e)
   {
    //Can be considered normal if tracksegement hasn't yet been allocated a block
    log.debug("TrackSegement at connection A doesn't contain a layout block");
   }
  }
  //if ((connectC instanceof TrackSegment) &&
  if((qobject_cast<TrackSegment*>(connectC)!= nullptr) &&
          (((TrackSegment*)connectC)->getLayoutBlock()!=block))
  {
   try
   {
    boundaryBetween->replace(2,((TrackSegment*)connectC)->getLayoutBlock()->getDisplayName()+ " - " + block->getDisplayName());
   }
   catch (NullPointerException e)
   {
    //Can be considered normal if tracksegement hasn't yet been allocated a block
    log.debug("TrackSegement at connection C doesn't contain a layout block");
   }
  }
  //if ((connectB instanceof TrackSegment) &&
  if((qobject_cast<TrackSegment*>(connectB)!= NULL) &&
          (((TrackSegment*)connectB)->getLayoutBlock()!=block))
  {
   try
   {
     boundaryBetween->replace(1,((TrackSegment*)connectB)->getLayoutBlock()->getDisplayName()+ " - " + block->getDisplayName());
   }
   catch (NullPointerException e)
   {
    //Can be considered normal if tracksegement hasn't yet been allocated a block
    log.debug("TrackSegement at connection B doesn't contain a layout block");
   }
  }
  //if ((connectD instanceof TrackSegment) &&
  if((qobject_cast<TrackSegment*>(connectD)!= NULL) &&

          (((TrackSegment*)connectD)->getLayoutBlock()!=block))
  {
   try
   {
    boundaryBetween->replace(3,((TrackSegment*)connectD)->getLayoutBlock()->getDisplayName()+ " - " + block->getDisplayName());
   }
   catch (NullPointerException e)
   {
    //Can be considered normal if tracksegement hasn't yet been allocated a block
    log.debug("TrackSegement at connection D doesn't contain a layout block");
   }
  }
 }
 return boundaryBetween;
}



/**
 * Clean up when this object is no longer needed.  Should not
 * be called while the object is still displayed; see remove()
 */
void LayoutSlip::dispose() {
    if (popup != NULL) popup->clear();
    popup = NULL;
}

/**
 * Removes this object from display and persistance
 */
void LayoutSlip::remove()
{
 disableSML(getSignalAMastName());
 disableSML(getSignalBMastName());
 disableSML(getSignalCMastName());
 disableSML(getSignalDMastName());
 removeSML(getSignalAMastName());
 removeSML(getSignalBMastName());
 removeSML(getSignalCMastName());
 removeSML(getSignalDMastName());
 // remove from persistance by flagging inactive
 active = false;
}

void LayoutSlip::disableSML(QString signalMast)
{
 if(signalMast==NULL || signalMast==(""))
  return;
 SignalMast* mast = static_cast<SignalMastManager*>(InstanceManager::getDefault("SignalMastManager"))->getSignalMast(signalMast);
 InstanceManager::signalMastLogicManagerInstance()->disableLayoutEditorUse(mast);
}

/**
 * "active" means that the object is still displayed, and should be stored.
 */
/*public*/ bool LayoutSlip::isActive() {
    return active;
}

/*public*/ bool LayoutSlip::singleSlipStraightEqual() {
    if (type != SINGLE_SLIP) {
        return false;
    }
    return turnoutStates.value(STATE_AC) == (turnoutStates.value(STATE_BD));
}

/*protected*/ QHash<int, TurnoutState*> LayoutSlip::getTurnoutStates() {
    return turnoutStates;
}
/*public*/ int LayoutSlip::getTurnoutState(Turnout* turn, int state){
 if(turn==getTurnout())
 {
  return getTurnoutState(state);
 }
 return getTurnoutBState(state);
}

/*public*/ int LayoutSlip::getTurnoutState(int state){
    return turnoutStates.value((state))->getTurnoutAState();
}

/*public*/ int LayoutSlip::getTurnoutBState(int state)
{
 return turnoutStates.value((state))->getTurnoutBState();
}

/*public*/ void LayoutSlip::setTurnoutStates(int state, QString turnStateA, QString turnStateB)
{
 if(!turnoutStates.contains(state))
 {
  log.error("Trying to set invalid state for slip " + getDisplayName());
  return;
 }
 turnoutStates.value(state)->setTurnoutAState((turnStateA).toInt());
 turnoutStates.value(state)->setTurnoutBState((turnStateB).toInt());
}

//Internal call to update the state of the slip depending upon the turnout states.
void LayoutSlip::updateState()
{
 int state_a = getTurnout()->getKnownState();
 int state_b = getTurnoutB()->getKnownState();
 //for(Entry<Integer, TurnoutState*> en: turnoutStates.entrySet()){
 QHashIterator<int, TurnoutState*> en(turnoutStates);
 while(en.hasNext())
 {
  en.next();
  if(en.value()->getTurnoutAState()==state_a)
  {
   if(en.value()->getTurnoutBState()==state_b)
   {
    currentState=en.key();
    layoutEditor->redrawPanel();
    return;
   }
  }
 }
}

/**
 * Check if either turnout is inconsistent.
 * This is used to create an alternate slip image.
 *
 * @return true if either turnout is inconsistent.
 */
/*private*/ bool LayoutSlip::isTurnoutInconsistent() {
    Turnout* tA = getTurnout();
    if (tA != nullptr && tA->getKnownState() == Turnout::INCONSISTENT) {
        return true;
    }
    Turnout* tB = getTurnoutB();
    if (tB != nullptr && tB->getKnownState() == Turnout::INCONSISTENT) {
        return true;
    }
    return false;
}

//@Override
/*protected*/ void LayoutSlip::draw1(EditScene *g2, bool drawMain, bool isBlock) {
    if (isBlock && getLayoutBlock() == nullptr) {
        // Skip the block layer since there is no block assigned.
        return;
    }

    QPointF pA = getCoordsA();
    QPointF pB = getCoordsB();
    QPointF pC = getCoordsC();
    QPointF pD = getCoordsD();

    bool mainlineA = isMainlineA();
    bool mainlineB = isMainlineB();
    bool mainlineC = isMainlineC();
    bool mainlineD = isMainlineD();

    bool drawUnselectedLeg = layoutEditor->isTurnoutDrawUnselectedLeg();

    QColor color = layoutEditor->drawingStroke.color(); //g2.getColor();

    // if this isn't a block line all these will be the same color
    QColor colorA = color;
    QColor colorB = color;
    QColor colorC = color;
    QColor colorD = color;

    if (isBlock) {
        LayoutBlock* lb = getLayoutBlock();
        colorA = (lb == nullptr) ? color : lb->getBlockColor();
        lb = getLayoutBlockB();
        colorB = (lb == nullptr) ? color : lb->getBlockColor();
        lb = getLayoutBlockC();
        colorC = (lb == nullptr) ? color : lb->getBlockColor();
        lb = getLayoutBlockD();
        colorD = (lb == nullptr) ? color : lb->getBlockColor();
    }

    QPointF oneForthPointAC = MathUtil::oneFourthPoint(pA, pC);
    QPointF oneThirdPointAC = MathUtil::oneThirdPoint(pA, pC);
    QPointF midPointAC = MathUtil::midPoint(pA, pC);
    QPointF twoThirdsPointAC = MathUtil::twoThirdsPoint(pA, pC);
    QPointF threeFourthsPointAC = MathUtil::threeFourthsPoint(pA, pC);

    QPointF oneForthPointBD = MathUtil::oneFourthPoint(pB, pD);
    QPointF oneThirdPointBD = MathUtil::oneThirdPoint(pB, pD);
    QPointF midPointBD = MathUtil::midPoint(pB, pD);
    QPointF twoThirdsPointBD = MathUtil::twoThirdsPoint(pB, pD);
    QPointF threeFourthsPointBD = MathUtil::threeFourthsPoint(pB, pD);

    QPointF midPointAD = MathUtil::midPoint(oneThirdPointAC, twoThirdsPointBD);
    QPointF midPointBC = MathUtil::midPoint(oneThirdPointBD, twoThirdsPointAC);

    //QGraphicsItemGroup* itemGroup;//= new QGraphicsItemGroup();

//    invalidateItemType(drawMain);
    if(itemMain)
     itemGroup = itemMain;
    else
    {
     itemGroup = new QGraphicsItemGroup();
     itemMain = itemGroup;
     g2->addItem(itemMain);
    }

    if (isTurnoutInconsistent())
    {
        // If either turnout is inconsistent, draw an alternate slip image
        // draw A<= =>C
        if (drawMain == mainlineA) {
            //g2.setColor(colorA);
         layoutEditor->drawingStroke.setColor(colorA);
            //g2.draw(new Line2D.Double(pA, oneForthPointAC));
         QGraphicsLineItem* lineItem = new QGraphicsLineItem(pA.x(), pA.y(), oneForthPointAC.x(), oneForthPointAC.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
        if (drawMain == mainlineC) {
            //g2.setColor(colorC);
         layoutEditor->drawingStroke.setColor(colorC);
            //g2.draw(new Line2D.Double(threeFourthsPointAC, pC));
         QGraphicsLineItem* lineItem = new QGraphicsLineItem(threeFourthsPointAC.x(), threeFourthsPointAC.y(), pC.x(), pC.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
        // draw B<= =>D
        if (drawMain == mainlineB) {
            //g2.setColor(colorB);
         layoutEditor->drawingStroke.setColor(colorB);
            //g2.draw(new Line2D.Double(pB, oneForthPointBD));
         QGraphicsLineItem* lineItem = new QGraphicsLineItem(pB.x(), pB.y(), oneForthPointBD.x(), oneForthPointBD.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
        if (drawMain == mainlineD) {
            //g2.setColor(colorD);
         layoutEditor->drawingStroke.setColor(colorD);
            //g2.draw(new Line2D.Double(threeFourthsPointBD, pD));
         QGraphicsLineItem* lineItem = new QGraphicsLineItem(threeFourthsPointBD.x(), threeFourthsPointBD.y(), pD.x(), pD.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
        return;
    }

    int slipState = getSlipState();

    if (slipState == STATE_AD)
    {
        // draw A<===>D
        if (drawMain == mainlineA) {
            //g2.setColor(colorA);
         layoutEditor->drawingStroke.setColor(colorA);
            //g2.draw(new Line2D.Double(pA, oneThirdPointAC));
         QGraphicsLineItem* lineItem = new QGraphicsLineItem(pA.x(), pA.y(), oneThirdPointAC.x(), oneThirdPointAC.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
            //g2.draw(new Line2D.Double(oneThirdPointAC, midPointAD));
         lineItem = new QGraphicsLineItem(oneThirdPointAC.x(), oneThirdPointAC.y(), midPointAD.x(), midPointAD.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
        if (drawMain == mainlineD) {
            //g2.setColor(colorD);
         layoutEditor->drawingStroke.setColor(colorD);
            //g2.draw(new Line2D.Double(midPointAD, twoThirdsPointBD));
         QGraphicsLineItem* lineItem = new QGraphicsLineItem(midPointAD.x(), midPointAD.y(), twoThirdsPointBD.x(), twoThirdsPointBD.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
            //g2.draw(new Line2D.Double(twoThirdsPointBD, pD));
         lineItem = new QGraphicsLineItem(twoThirdsPointBD.x(), twoThirdsPointBD.y(), pD.x(), pD.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
    }
    else if (slipState == STATE_AC)
    {
        // draw A<===>C
        if (drawMain == mainlineA) {
            //g2.setColor(colorA);
         layoutEditor->drawingStroke.setColor(colorA);
            //.draw(new Line2D.Double(pA, midPointAC));
         QGraphicsLineItem* lineItem = new QGraphicsLineItem(pA.x(), pA.y(), midPointAC.x(), midPointAC.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
        if (drawMain == mainlineC) {
            //g2.setColor(colorC);
         layoutEditor->drawingStroke.setColor(colorC);
            //g2.draw(new Line2D.Double(midPointAC, pC));
         QGraphicsLineItem* lineItem = new QGraphicsLineItem(midPointAC.x(), midPointAC.y(), pC.x(), pC.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
    }
    else if (!isBlock || drawUnselectedLeg)
    {
        // draw A<= =>C
        if (drawMain == mainlineA) {
            //g2.setColor(colorA);
         layoutEditor->drawingStroke.setColor(colorA);
            //g2.draw(new Line2D.Double(pA, oneForthPointAC));
         QGraphicsLineItem* lineItem = new QGraphicsLineItem(pA.x(), pA.y(), oneForthPointAC.x(), oneForthPointAC.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
        if (drawMain == mainlineC) {
            //g2.setColor(colorC);
         layoutEditor->drawingStroke.setColor(colorC);
            //g2.draw(new Line2D.Double(threeFourthsPointAC, pC));
         QGraphicsLineItem* lineItem = new QGraphicsLineItem(threeFourthsPointAC.x(), threeFourthsPointAC.y(), pC.x(), pC.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
    }

    if (slipState == STATE_BD)
    {
        // draw B<===>D
        if (drawMain == mainlineB) {
            //g2.setColor(colorB);
         layoutEditor->drawingStroke.setColor(colorB);
            //g2.draw(new Line2D.Double(pB, midPointBD));
         QGraphicsLineItem* lineItem = new QGraphicsLineItem(pB.x(), pB.y(), midPointBD.x(), midPointBD.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
        if (drawMain == mainlineD) {
            //g2.setColor(colorD);
         layoutEditor->drawingStroke.setColor(colorD);
            //g2.draw(new Line2D.Double(midPointBD, pD));
         QGraphicsLineItem* lineItem = new QGraphicsLineItem(midPointBD.x(), midPointBD.y(), pD.x(), pD.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
    }
    else if (!isBlock || drawUnselectedLeg)
    {
        // draw B<= =>D
        if (drawMain == mainlineB) {
            //g2.setColor(colorB);
         layoutEditor->drawingStroke.setColor(colorB);
            //g2.draw(new Line2D.Double(pB, oneForthPointBD));
         QGraphicsLineItem* lineItem = new QGraphicsLineItem(pB.x(), pB.y(), oneForthPointBD.x(), oneForthPointBD.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
        if (drawMain == mainlineD) {
            //g2.setColor(colorD);
         layoutEditor->drawingStroke.setColor(colorD);
            //g2.draw(new Line2D.Double(threeFourthsPointBD, pD));
         QGraphicsLineItem* lineItem = new QGraphicsLineItem(threeFourthsPointBD.x(), threeFourthsPointBD.y(), pD.x(), pD.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
    }

    if (slipState == STATE_BC) {
        if (getTurnoutType() == DOUBLE_SLIP) {
            // draw B<===>C
            if (drawMain == mainlineB) {
                //g2.setColor(colorB);
             layoutEditor->drawingStroke.setColor(colorB);
                //g2.draw(new Line2D.Double(pB, oneThirdPointBD));
             QGraphicsLineItem* lineItem = new QGraphicsLineItem(pB.x(), pB.y(), oneThirdPointBD.x(), oneThirdPointBD.y());
             lineItem->setPen(layoutEditor->drawingStroke);
             itemGroup->addToGroup(lineItem);
                //g2.draw(new Line2D.Double(oneThirdPointBD, midPointBC));
             lineItem = new QGraphicsLineItem(oneThirdPointBD.x(), oneThirdPointBD.y(), midPointBC.x(), midPointBC.y());
             lineItem->setPen(layoutEditor->drawingStroke);
             itemGroup->addToGroup(lineItem);
            }
            if (drawMain == mainlineC) {
                //g2.setColor(colorC);
             layoutEditor->drawingStroke.setColor(colorC);
                //g2.draw(new Line2D.Double(midPointBC, twoThirdsPointAC));
             QGraphicsLineItem* lineItem = new QGraphicsLineItem(midPointBC.x(), midPointBC.y(), twoThirdsPointAC.x(), twoThirdsPointAC.y());
             lineItem->setPen(layoutEditor->drawingStroke);
             itemGroup->addToGroup(lineItem);
                //g2.draw(new Line2D.Double(twoThirdsPointAC, pC));
             lineItem = new QGraphicsLineItem(twoThirdsPointAC.x(), twoThirdsPointAC.y(), pC.x(), pC.y());
             lineItem->setPen(layoutEditor->drawingStroke);
             itemGroup->addToGroup(lineItem);
            }
        }   // DOUBLE_SLIP
    }
    else if (!isBlock || drawUnselectedLeg)
    {
        // draw B<= =>C
        if (drawMain == mainlineB) {
            //g2.setColor(colorB);
         layoutEditor->drawingStroke.setColor(colorB);
            //g2.draw(new Line2D.Double(pB, oneForthPointBD));
         QGraphicsLineItem* lineItem = new QGraphicsLineItem(pB.x(), pB.y(), oneForthPointBD.x(), oneForthPointBD.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
        if (drawMain == mainlineC) {
            //g2.setColor(colorC);
         layoutEditor->drawingStroke.setColor(colorC);
            //g2.draw(new Line2D.Double(threeFourthsPointAC, pC));
         QGraphicsLineItem* lineItem = new QGraphicsLineItem(threeFourthsPointAC.x(), threeFourthsPointAC.y(), pC.x(), pC.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
    }
//    if(drawMain)
//    {
//     itemMain = itemGroup;
//     g2->addItem(itemMain);
//    }
//    else
//    {
//     itemSide = itemGroup;
//     g2->addItem(itemSide);
//    }
}   // draw1

/**
 * {@inheritDoc}
 */
//@Override
/*protected*/ void LayoutSlip::draw2(EditScene* g2, bool drawMain, float railDisplacement) {
    QPointF pA = getCoordsA();
    QPointF pB = getCoordsB();
    QPointF pC = getCoordsC();
    QPointF pD = getCoordsD();
    QPointF pM = getCoordsCenter();

    QPointF vAC = MathUtil::normalize(MathUtil::subtract(pC, pA), railDisplacement);
    double dirAC_DEG = MathUtil::computeAngleDEG(pA, pC);
    QPointF vACo = MathUtil::orthogonal(vAC);
    QPointF pAL = MathUtil::subtract(pA, vACo);
    QPointF pAR = MathUtil::add(pA, vACo);
    QPointF pCL = MathUtil::subtract(pC, vACo);
    QPointF pCR = MathUtil::add(pC, vACo);

    QPointF vBD = MathUtil::normalize(MathUtil::subtract(pD, pB), railDisplacement);
    double dirBD_DEG = MathUtil::computeAngleDEG(pB, pD);
    QPointF vBDo = MathUtil::orthogonal(vBD);
    QPointF pBL = MathUtil::subtract(pB, vBDo);
    QPointF pBR = MathUtil::add(pB, vBDo);
    QPointF pDL = MathUtil::subtract(pD, vBDo);
    QPointF pDR = MathUtil::add(pD, vBDo);

    double deltaDEG = MathUtil::absDiffAngleDEG(dirAC_DEG, dirBD_DEG);
    double deltaRAD = qDegreesToRadians(deltaDEG);

    double hypotV = railDisplacement / qCos((M_PI - deltaRAD) / 2.0);
    double hypotK = railDisplacement / qCos(deltaRAD / 2.0);

    log.debug(tr("dir AC: %1, BD: %2, diff: %3").arg(dirAC_DEG).arg(dirBD_DEG).arg(deltaDEG));

    QPointF vDisK = MathUtil::normalize(MathUtil::subtract(vAC, vBD), hypotK);
    QPointF vDisV = MathUtil::normalize(MathUtil::orthogonal(vDisK), hypotV);
    QPointF pKL = MathUtil::subtract(pM, vDisK);
    QPointF pKR = MathUtil::add(pM, vDisK);
    QPointF pVL = MathUtil::add(pM, vDisV);
    QPointF pVR = MathUtil::subtract(pM, vDisV);

    // this is the vector (rail gaps) for the diamond parts
    double railGap = 2.0 / qSin(deltaRAD);
    QPointF vAC2 = MathUtil::normalize(vAC, railGap);
    QPointF vBD2 = MathUtil::normalize(vBD, railGap);
    // KR and VR toward A, KL and VL toward C
    QPointF pKRtA = MathUtil::subtract(pKR, vAC2);
    QPointF pVRtA = MathUtil::subtract(pVR, vAC2);
    QPointF pKLtC = MathUtil::add(pKL, vAC2);
    QPointF pVLtC = MathUtil::add(pVL, vAC2);

    // VR and KL toward B, KR and VL toward D
    QPointF pVRtB = MathUtil::subtract(pVR, vBD2);
    QPointF pKLtB = MathUtil::subtract(pKL, vBD2);
    QPointF pKRtD = MathUtil::add(pKR, vBD2);
    QPointF pVLtD = MathUtil::add(pVL, vBD2);

    // outer (closed) switch points
    QPointF pAPL = MathUtil::add(pAL, MathUtil::subtract(pVL, pAR));
    QPointF pBPR = MathUtil::add(pBR, MathUtil::subtract(pVL, pBL));
    QPointF pCPR = MathUtil::add(pCR, MathUtil::subtract(pVR, pCL));
    QPointF pDPL = MathUtil::add(pDL, MathUtil::subtract(pVR, pDR));

    // this is the vector (rail gaps) for the inner (open) switch points
    QPointF vACo2 = MathUtil::normalize(vACo, 2.0);
    QPointF vBDo2 = MathUtil::normalize(vBDo, 2.0);
    QPointF pASL = MathUtil::add(pAPL, vACo2);
    QPointF pBSR = MathUtil::subtract(pBPR, vBDo2);
    QPointF pCSR = MathUtil::subtract(pCPR, vACo2);
    QPointF pDSL = MathUtil::add(pDPL, vBDo2);

    QPointF pVLP = MathUtil::add(pVLtD, vAC2);
    QPointF pVRP = MathUtil::subtract(pVRtA, vBD2);

    QPointF pKLH = MathUtil::midPoint(pM, pKL);
    QPointF pKRH = MathUtil::midPoint(pM, pKR);

    bool mainlineA = isMainlineA();
    bool mainlineB = isMainlineB();
    bool mainlineC = isMainlineC();
    bool mainlineD = isMainlineD();
    QGraphicsItemGroup* itemGroup = new QGraphicsItemGroup();

//    invalidateItemType(drawMain);
    if(itemMain)
     itemGroup = itemMain;
    else
    {
     itemGroup = new QGraphicsItemGroup();
     itemMain = itemGroup;
     g2->addItem(itemMain);
    }

    if (drawMain == mainlineA) {
        //g2.draw(new Line2D.Double(pAR, pVL));
     QGraphicsLineItem* lineItem = new QGraphicsLineItem(pAR.x(), pAR.y(), pVL.x(), pVL.y());
     lineItem->setPen(layoutEditor->drawingStroke);
     itemGroup->addToGroup(lineItem);
        //g2.draw(new Line2D.Double(pVLtD, pKLtB));
     lineItem = new QGraphicsLineItem(pVLtD.x(), pVLtD.y(), pKLtB.x(), pKLtB.y());
     lineItem->setPen(layoutEditor->drawingStroke);
     itemGroup->addToGroup(lineItem);
        QPainterPath path =  QPainterPath();
        path.moveTo(pAL.x(), pAL.y());
        path.lineTo(pAPL.x(), pAPL.y());
        path.quadTo(pKL.x(), pKL.y(), pDPL.x(), pDPL.y());
        //g2.draw(path);
        QGraphicsPathItem* pathItem = new QGraphicsPathItem(path);
        pathItem->setPen(layoutEditor->drawingStroke);
        itemGroup->addToGroup(pathItem);
    }
#if 1
    if (drawMain == mainlineB) {
        //g2.draw(new Line2D.Double(pBL, pVL));
     QGraphicsLineItem* lineItem = new QGraphicsLineItem(pBL.x(), pBL.y(), pVL.x(), pVL.y());
     lineItem->setPen(layoutEditor->drawingStroke);
     itemGroup->addToGroup(lineItem);
        //g2.draw(new Line2D.Double(pVLtC, pKRtA));
     lineItem = new QGraphicsLineItem(pVLtC.x(), pVLtC.y(), pKRtA.x(), pKRtA.y());
     lineItem->setPen(layoutEditor->drawingStroke);
     itemGroup->addToGroup(lineItem);
        if (getTurnoutType() == DOUBLE_SLIP) {
            QPainterPath path = QPainterPath();
            path.moveTo(pBR.x(), pBR.y());
            path.lineTo(pBPR.x(), pBPR.y());
            path.quadTo(pKR.x(), pKR.y(), pCPR.x(), pCPR.y());
            //g2.draw(path);
            QGraphicsPathItem* pathItem = new QGraphicsPathItem(path);
            pathItem->setPen(layoutEditor->drawingStroke);
            itemGroup->addToGroup(pathItem);
        } else {
            //g2.draw(new Line2D.Double(pBR, pKR));
         QGraphicsLineItem* lineItem = new QGraphicsLineItem(pBR.x(), pBR.y(), pKR.x(), pKR.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
    }
    if (drawMain == mainlineC) {
        //g2.draw(new Line2D.Double(pCL, pVR));
     QGraphicsLineItem* lineItem = new QGraphicsLineItem(pCL.x(), pCL.y(), pVR.x(), pVR.y());
     lineItem->setPen(layoutEditor->drawingStroke);
     itemGroup->addToGroup(lineItem);
     //g2.draw(new Line2D.Double(pVRtB, pKRtD));
     lineItem = new QGraphicsLineItem(pVRtB.x(), pVRtB.y(), pKRtD.x(), pKRtD.y());
     lineItem->setPen(layoutEditor->drawingStroke);
     itemGroup->addToGroup(lineItem);   if (getTurnoutType() == DOUBLE_SLIP) {
            QPainterPath path = QPainterPath();
            path.moveTo(pCR.x(), pCR.y());
            path.lineTo(pCPR.x(), pCPR.y());
            path.quadTo(pKR.x(), pKR.y(), pBPR.x(), pBPR.y());
            //g2.draw(path);
            QGraphicsPathItem* pathItem = new QGraphicsPathItem(path);
            pathItem->setPen(layoutEditor->drawingStroke);
            itemGroup->addToGroup(pathItem);
     } else {
            //g2.draw(new Line2D.Double(pCR, pKR));
      lineItem = new QGraphicsLineItem(pCR.x(), pCR.y(), pKR.x(), pKR.y());
      lineItem->setPen(layoutEditor->drawingStroke);
      itemGroup->addToGroup(lineItem);
     }
    }

    if (drawMain == mainlineD) {
        //g2.draw(new Line2D.Double(pDR, pVR));
        QGraphicsLineItem* lineItem = new QGraphicsLineItem(pDR.x(), pDR.y(), pVR.x(), pVR.y());
        lineItem->setPen(layoutEditor->drawingStroke);
        itemGroup->addToGroup(lineItem);
        //g2.draw(new Line2D.Double(pVRtA, pKLtC));
        lineItem = new QGraphicsLineItem(pVRtA.x(), pVRtA.y(), pKLtC.x(), pKLtC.y());
        lineItem->setPen(layoutEditor->drawingStroke);
        itemGroup->addToGroup(lineItem);
        QPainterPath path = QPainterPath();
        path.moveTo(pDL.x(), pDL.y());
        path.lineTo(pDPL.x(), pDPL.y());
        path.quadTo(pKL.x(), pKL.y(), pAPL.x(), pAPL.y());
        //g2.draw(path);
        QGraphicsPathItem* pathItem = new QGraphicsPathItem(path);
        pathItem->setPen(layoutEditor->drawingStroke);
        itemGroup->addToGroup(pathItem);
    }
#if 1
    int slipState = getSlipState();
    QGraphicsLineItem* lineItem;
    if (slipState == STATE_AD) {
        if (drawMain == mainlineA) {
            //g2.draw(new Line2D.Double(pASL, pKL));
         lineItem = new QGraphicsLineItem(pASL.x(), pASL.y(), pKL.x(), pKL.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
         //g2.draw(new Line2D.Double(pVLP, pKLH));
         lineItem = new QGraphicsLineItem(pVLP.x(), pVLP.y(), pKLH.x(), pKLH.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
        if (drawMain == mainlineB) {
            //g2.draw(new Line2D.Double(pBPR, pKR));
         lineItem = new QGraphicsLineItem(pBPR.x(), pBPR.y(), pKR.x(), pKR.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
         //g2.draw(new Line2D.Double(pVLtC, pKRH));
         lineItem = new QGraphicsLineItem(pVLtC.x(), pVLtC.y(), pKRH.x(), pKRH.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
        if (drawMain == mainlineC) {
            //g2.draw(new Line2D.Double(pCPR, pKR));
         lineItem = new QGraphicsLineItem(pCPR.x(), pCPR.y(), pKR.x(), pKR.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
         //g2.draw(new Line2D.Double(pVRtB, pKRH));
         lineItem = new QGraphicsLineItem(pVRtB.x(), pVRtB.y(), pKRH.x(), pKRH.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
        if (drawMain == mainlineD) {
            //g2.draw(new Line2D.Double(pDSL, pKL));
         lineItem = new QGraphicsLineItem(pDSL.x(), pDSL.y(), pKL.x(), pKL.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
         //g2.draw(new Line2D.Double(pVRP, pKLH));
         lineItem = new QGraphicsLineItem(pVRP.x(), pVRP.y(), pKLH.x(), pKLH.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
    } else if (slipState == STATE_AC) {
        if (drawMain == mainlineA) {
            //g2.draw(new Line2D.Double(pAPL, pKL));
         lineItem = new QGraphicsLineItem(pAPL.x(), pAPL.y(), pKL.x(), pKL.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
         //g2.draw(new Line2D.Double(pVLtD, pKLH));
         lineItem = new QGraphicsLineItem(pVLtD.x(), pVLtD.y(), pKLH.x(), pKLH.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
        if (drawMain == mainlineB) {
            //g2.draw(new Line2D.Double(pBSR, pKR));
         lineItem = new QGraphicsLineItem(pBSR.x(), pBSR.y(), pKR.x(), pKR.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
         //g2.draw(new Line2D.Double(pVLP, pKRH));
         lineItem = new QGraphicsLineItem(pVLP.x(), pVLP.y(), pKRH.x(), pKRH.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
        if (drawMain == mainlineC) {
            //g2.draw(new Line2D.Double(pCPR, pKR));
         lineItem = new QGraphicsLineItem(pCPR.x(), pCPR.y(), pKR.x(), pKR.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
         //g2.draw(new Line2D.Double(pVRtB, pKRH));
         lineItem = new QGraphicsLineItem(pVRtB.x(), pVRtB.y(), pKRH.x(), pKRH.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
        if (drawMain == mainlineD) {
            //g2.draw(new Line2D.Double(pDSL, pKL));
         lineItem = new QGraphicsLineItem(pDSL.x(), pDSL.y(), pKL.x(), pKL.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
         //g2.draw(new Line2D.Double(pVRP, pKLH));
         lineItem = new QGraphicsLineItem(pVRP.x(), pVRP.y(), pKLH.x(), pKLH.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
    } else if (slipState == STATE_BD) {
        if (drawMain == mainlineA) {
            //g2.draw(new Line2D.Double(pASL, pKL));
         lineItem = new QGraphicsLineItem(pASL.x(), pASL.y(), pKL.x(), pKL.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
         //g2.draw(new Line2D.Double(pVLP, pKLH));
         lineItem = new QGraphicsLineItem(pVLP.x(), pVLP.y(), pKLH.x(), pKLH.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
        if (drawMain == mainlineB) {
           // g2.draw(new Line2D.Double(pBPR, pKR));
         lineItem = new QGraphicsLineItem(pBPR.x(), pBPR.y(), pKR.x(), pKR.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
         //g2.draw(new Line2D.Double(pVLtC, pKRH));
         lineItem = new QGraphicsLineItem(pVLtC.x(), pVLtC.y(), pKRH.x(), pKRH.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
        if (drawMain == mainlineC) {
            //g2.draw(new Line2D.Double(pCSR, pKR));
         lineItem = new QGraphicsLineItem(pCSR.x(), pCSR.y(), pKR.x(), pKR.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
         //g2.draw(new Line2D.Double(pVRP, pKRH));
         lineItem = new QGraphicsLineItem(pVRP.x(), pVRP.y(), pKRH.x(), pKRH.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
        if (drawMain == mainlineD) {
            //g2.draw(new Line2D.Double(pDPL, pKL));
         lineItem = new QGraphicsLineItem(pDPL.x(), pDPL.y(), pKL.x(), pKL.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
         //g2.draw(new Line2D.Double(pVRtA, pKLH));
         lineItem = new QGraphicsLineItem(pVRtA.x(), pVRtA.y(), pKLH.x(), pKLH.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
    } else if ((getTurnoutType() == DOUBLE_SLIP)
            && (slipState == STATE_BC)) {
        if (drawMain == mainlineA) {
            //g2.draw(new Line2D.Double(pAPL, pKL));
         lineItem = new QGraphicsLineItem(pAPL.x(), pAPL.y(), pKL.x(), pKL.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
         //g2.draw(new Line2D.Double(pVLtD, pKLH));
         lineItem = new QGraphicsLineItem(pVLtD.x(), pVLtD.y(), pKLH.x(), pKLH.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
        if (drawMain == mainlineB) {
            //g2.draw(new Line2D.Double(pBSR, pKR));
         lineItem = new QGraphicsLineItem(pBSR.x(), pBSR.y(), pKR.x(), pKR.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
         //g2.draw(new Line2D.Double(pVLP, pKRH));
         lineItem = new QGraphicsLineItem(pVLP.x(), pVLP.y(), pKRH.x(), pKRH.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
        if (drawMain == mainlineC) {
            //g2.draw(new Line2D.Double(pCSR, pKR));
         lineItem = new QGraphicsLineItem(pCSR.x(), pCSR.y(), pKR.x(), pKR.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
         //g2.draw(new Line2D.Double(pVRP, pKRH));
         lineItem = new QGraphicsLineItem(pVRP.x(), pVRP.y(), pKRH.x(), pKRH.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
        }
        if (drawMain == mainlineD) {
            //g2.draw(new Line2D.Double(pDPL, pKL));
         lineItem = new QGraphicsLineItem(pDPL.x(), pDPL.y(), pKL.x(), pKL.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);
         //g2.draw(new Line2D.Double(pVRtA, pKLH));
         lineItem = new QGraphicsLineItem(pVRtA.x(), pVRtA.y(), pKLH.x(), pKLH.y());
         lineItem->setPen(layoutEditor->drawingStroke);
         itemGroup->addToGroup(lineItem);}
    }   // DOUBLE_SLIP
#endif
//    if(drawMain)
//    {
//     itemMain = itemGroup;
//     g2->addItem(itemMain);
//    }
//    else
//    {
//     itemSide = itemGroup;
//     g2->addItem(itemSide);
//    }
}   // draw2

/**
 * {@inheritDoc}
 */
//@Override
/*protected*/ void LayoutSlip::highlightUnconnected(EditScene* g2, int specificType)
{
 QGraphicsItemGroup* itemGroup = new QGraphicsItemGroup();

 invalidateItem(g2,rects);

 if (((specificType == NONE) || (specificType == SLIP_A))
            && (getConnectA() == nullptr)) {
        //g2.fill(layoutEditor->trackControlCircleAt(getCoordsA()));
     QGraphicsEllipseItem* ellipseItem = layoutEditor->trackControlCircleAt(getCoordsA());
     ellipseItem->setPen(layoutEditor->drawingStroke);
     itemGroup->addToGroup(ellipseItem);
    }

    if (((specificType == NONE) || (specificType == SLIP_B))
            && (getConnectB() == nullptr)) {
        //g2.fill(layoutEditor->trackControlCircleAt(getCoordsB()));
     QGraphicsEllipseItem* ellipseItem = layoutEditor->trackControlCircleAt(getCoordsB());
     ellipseItem->setPen(layoutEditor->drawingStroke);
     itemGroup->addToGroup(ellipseItem);
    }

    if (((specificType == NONE) || (specificType == SLIP_C))
            && (getConnectC() == nullptr)) {
        //g2.fill(layoutEditor->trackControlCircleAt(getCoordsC()));
    }
    QGraphicsEllipseItem* ellipseItem = layoutEditor->trackControlCircleAt(getCoordsC());
    ellipseItem->setPen(layoutEditor->drawingStroke);
    itemGroup->addToGroup(ellipseItem);

    if (((specificType == NONE) || (specificType == SLIP_D))
            && (getConnectD() == nullptr)) {
        //g2.fill(layoutEditor->trackControlCircleAt(getCoordsD()));
     QGraphicsEllipseItem* ellipseItem = layoutEditor->trackControlCircleAt(getCoordsD());
     ellipseItem->setPen(layoutEditor->drawingStroke);
     itemGroup->addToGroup(ellipseItem);
    }

    rects = itemGroup;
    g2->addItem(rects);
}
#endif
   //@Override
   /*protected*/ void LayoutSlip::drawTurnoutControls(EditScene* g2) {
       // drawHidden left/right turnout control circles
 QGraphicsItemGroup* itemGroup = new QGraphicsItemGroup();
       QPointF leftCircleCenter = getCoordsLeft();

       invalidateItem(g2,circles);

       //g2.draw(layoutEditor->trackControlCircleAt(leftCircleCenter));
       QGraphicsEllipseItem* ellipseItem = layoutEditor->trackControlCircleAt(leftCircleCenter);
       ellipseItem->setPen(layoutEditor->drawingStroke);
       itemGroup->addToGroup(ellipseItem);

       QPointF rightCircleCenter = getCoordsRight();
       //g2.draw(layoutEditor->trackControlCircleAt(rightCircleCenter));
       ellipseItem = layoutEditor->trackControlCircleAt(rightCircleCenter);
       ellipseItem->setPen(layoutEditor->drawingStroke);
       itemGroup->addToGroup(ellipseItem);

       circles = itemGroup;
       g2->addItem(circles);
   }   // drawTurnoutControls


//static org.apache.log4j.Logger log = org.apache.log4j.Logger.getLogger(LayoutSlip::class.getName());

//}
