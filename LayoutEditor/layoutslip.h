#ifndef LAYOUTSLIP_H
#define LAYOUTSLIP_H
#include "layoutturnout.h"
#include "instancemanager.h"
#include "liblayouteditor_global.h"
#include <QComboBox>

class EditScene;
class JmriBeanComboBox;
class InstanceManager;
/*static*/ class LIBLAYOUTEDITORSHARED_EXPORT TurnoutState
{
 public:
 int turnoutA;// = Turnout::CLOSED;
 int turnoutB;// = Turnout::CLOSED;
 QComboBox* turnoutABox;
 QComboBox* turnoutBBox;

 TurnoutState(int turnoutA, int turnoutB)
 {
  turnoutA = Turnout::CLOSED;
  turnoutB = Turnout::CLOSED;
  turnoutABox = NULL;
  turnoutBBox = NULL;

  this->turnoutA = turnoutA;
  this->turnoutB = turnoutB;
 }

 int getTurnoutAState()
 {
  return turnoutA;
 }

 int getTurnoutBState()
 {
  return turnoutB;
 }

 void setTurnoutAState(int state)
 {
  turnoutA = state;
 }

 void setTurnoutBState(int state){
    turnoutB = state;
 }

 QComboBox* getComboA();

 QComboBox* getComboB();

 int getTestTurnoutAState()
 {
  if(turnoutABox->currentIndex()==0)
      return Turnout::CLOSED;
  return Turnout::THROWN;
 }

 int getTestTurnoutBState()
 {
  if(turnoutBBox->currentIndex()==0)
   return Turnout::CLOSED;
  return Turnout::THROWN;
 }

 void updateStatesFromCombo()
 {
  if(turnoutABox==NULL || turnoutBBox==NULL)
  {
   return;
  }
  if(turnoutABox->currentIndex()==0)
  {
   turnoutA= Turnout::CLOSED;
  }
  else
  {
   turnoutA=Turnout::THROWN;
  }
  if(turnoutBBox->currentIndex()==0)
  {
   turnoutB= Turnout::CLOSED;
  }
  else
  {
   turnoutB=Turnout::THROWN;
  }
 }

 bool equals(TurnoutState* ts)
 {
    if(ts->getTurnoutAState()!=this->getTurnoutAState())
        return false;
    if(ts->getTurnoutBState()!=this->getTurnoutBState())
        return false;
    return true;
 }

};

class LIBLAYOUTEDITORSHARED_EXPORT LayoutSlip : public LayoutTurnout
{
    Q_OBJECT
public:
    //explicit LayoutSlip(QObject *parent = 0);
    /**
     * constructor method
     */
    /*public*/ LayoutSlip(QString id, QPointF c, double rot, LayoutEditor* myPanel, int type);
    /*public*/ void setTurnoutType(int slipType);
    /*public*/ void setSlipType(int slipType);
    /*public*/ int getSlipType();
    /*public*/ int getSlipState();
    /*public*/ QString getTurnoutBName() ;
    /*public*/ Turnout* getTurnoutB();
    /*public*/ void setTurnoutB(QString tName);
    /*public*/ QString getDisplayName();

    // operational instance variables (not saved between sessions)

//    /*final*/ /*public*/ const static int UNKNOWN = /*Turnout::UNKNOWN*/0x01;
//    /*final*/ /*public*/ const static int STATE_AC;// = 0x02;
//    /*final*/ /*public*/ const static int STATE_BD;// = 0x04;
//    /*final*/ /*public*/ const static int STATE_AD;// = 0x06;
//    /*final*/ /*public*/ const static int STATE_BC;// = 0x08;
    enum STATES
    {
     UNKNOWN = 0x01,
     STATE_AC = 0x02,
     STATE_BD = 0x04,
     STATE_AD = 0x06,
     STATE_BC = 0x08
    };
    /*public*/ int currentState;// = UNKNOWN;
    /*public*/ QPointF getCoordsCenter();
    /*public*/ QPointF getCoordsA() ;
    /*public*/ QPointF getCoordsB() ;
    /*public*/ QPointF getCoordsC();
    /*public*/ QPointF getCoordsD();
    /*public*/ void reCheckBlockBoundary();
    void removeSML(QString signalMast);
    /**
     * Methods to test if mainline track or not
     *  Returns true if either connecting track segment is mainline
     *  Defaults to not mainline if connecting track segments are missing
     */
    /*public*/ bool isMainline();
    /**
     * Modify coordinates methods
     */;
    /*public*/ void setCoordsCenter(QPointF p);
    /*public*/ void setCoordsA(QPointF p);
    /*public*/ void setCoordsB(QPointF p);
    /*public*/ void setCoordsC(QPointF p);
    /*public*/ void setCoordsD(QPointF p);
    /*public*/ void scaleCoords(float xFactor, float yFactor);
    double round (double x);
    /*public*/ void setObjects(LayoutEditor* p);
    /**
     * Clean up when this object is no longer needed.  Should not
     * be called while the object is still displayed; see remove()
     */
    void dispose();
    /**
     * Removes this object from display and persistance
     */
    void remove();
    void disableSML(QString signalMast);
    /**
     * "active" means that the object is still displayed, and should be stored.
     */

    /*public*/ int getTurnoutState(Turnout* turn, int state);
    /*public*/ int getTurnoutState(int state);
    /*public*/ int getTurnoutBState(int state);
    /*public*/ void setTurnoutStates(int state, QString turnStateA, QString turnStateB);
    //Internal call to update the state of the slip depending upon the turnout states.
    /*public*/ QVector<QString>* getBlockBoundaries();
    /*public*/ LayoutTrack* getConnection(int location) throw (JmriException);
    /*public*/ void setConnection(int location, LayoutTrack *o, int type) throw (JmriException);
    /*public*/ void drawSlipCircles(EditScene* g2);
    /*public*/ QPointF getCoordsForConnectionType(int connectionType);
    /*public*/ void toggleState();
    /*public*/ bool isActive();
    /*public*/ bool singleSlipStraightEqual();

signals:
    
public slots:
  void on_removeAction_triggered();
  void on_rotate_triggered();
  void updateState();
  void OnEditAction();
  void on_setSignalsAct_triggered();
  void on_setSignalMastsAct_triggered();
  void on_setSensorsAct_triggered();

private:
    /*private*/ QString turnoutBName;//="";
    /*private*/ NamedBeanHandle<Turnout*>* namedTurnoutB;// = NULL;
    /*private*/ PropertyChangeListener* mTurnoutListener;// = NULL;
    void init();
    void setTurnoutState(TurnoutState* ts);
    /**
     * Activate/Deactivate turnout to redraw when turnout state changes
     */
    /*private*/ void activateTurnout();
    /*private*/ void deactivateTurnout();
    /*private*/ void updateBlockInfo();
    QMenu* popup;// = NULL;
    LayoutEditorTools* tools;// = NULL;
 Logger log;
 int testState;// = UNKNOWN;
 bool active;// = true;
 QHash <int, TurnoutState*> turnoutStates;// = new QHash<int, TurnoutState>(4);
 /*private*/ bool isTurnoutInconsistent();

protected:
 /*protected*/ QMenu* showPopup(QGraphicsSceneMouseEvent* e);
// /*protected*/ void editLayoutSlip(LayoutTurnout* o);
 /*protected*/ QPointF getCoordsLeft();
 /*protected*/ QPointF getCoordsRight();
 /*protected*/ int getConnectivityStateForLayoutBlocks(
         /*@Nullable*/ LayoutBlock* thisLayoutBlock,
         /*@Nullable*/ LayoutBlock* prevLayoutBlock,
         /*@Nullable*/ LayoutBlock* nextLayoutBlock,
         bool suppress);
 /*protected*/ void drawTurnoutControls(EditScene* g2);
 /*protected*/ QHash<int, TurnoutState *> getTurnoutStates();
 /*protected*/ int findHitPointType(/*@Nonnull*/ QPointF hitPoint, bool useRectangles, bool requireUnconnected);
 /*protected*/ void highlightUnconnected(EditScene* g2, int specificType);
 /*protected*/ void draw1(EditScene* g2, bool drawMain, bool isBlock);
 /*protected*/ void draw2(EditScene* g2, bool drawMain, float railDisplacement);

friend class LoadXml;
friend class LayoutEditor;
friend class LayoutTrackEditors;
};

#endif // LAYOUTSLIP_H
