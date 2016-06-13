#ifndef LEVELXING_H
#define LEVELXING_H

#include <QObject>
#include "layoutblock.h"
#include "layouteditor.h"
#include "QPointF"
#include "signalmast.h"
#include <QMenu>
#include <QTextEdit>
#include <QPushButton>
#include <QMenuItem>
#include <QMouseEvent>
#include "layouteditortools.h"
#include <QWidget>
class QMenuItem;
class LayoutBlock;
class LevelXing : public QObject
{
    Q_OBJECT
public:
    explicit LevelXing(QObject *parent = 0);
    /**
     * constructor method
     */
    /*public*/ LevelXing(QString id, QPointF* c, LayoutEditor* myPanel);
    /**
     * Accessor methods
    */
    /*public*/ QString getID() {return ident;}
    /*public*/ QString getBlockNameAC() {return blockNameAC;}
    /*public*/ QString getBlockNameBD() {return blockNameBD;}
    /*public*/ QString getSignalAName() {return signalAName;}
    /*public*/ void setSignalAName(QString signalName) {signalAName = signalName;}
    /*public*/ QString getSignalBName() {return signalBName;}
    /*public*/ void setSignalBName(QString signalName) {signalBName = signalName;}
    /*public*/ QString getSignalCName() {return signalCName;}
    /*public*/ void setSignalCName(QString signalName) {signalCName = signalName;}
    /*public*/ QString getSignalDName() {return signalDName;}
    /*public*/ void setSignalDName(QString signalName) {signalDName = signalName;}

    /*public*/ QString getSignalAMastName() {return signalAMastName;}
    /*public*/ void setSignalAMastName(QString signalName) {signalAMastName = signalName;}
    /*public*/ QString getSignalBMastName() {return signalBMastName;}
    /*public*/ void setSignalBMastName(QString signalName) {signalBMastName = signalName;}
    /*public*/ QString getSignalCMastName() {return signalCMastName;}
    /*public*/ void setSignalCMastName(QString signalName) {signalCMastName = signalName;}
    /*public*/ QString getSignalDMastName() {return signalDMastName;}
    /*public*/ void setSignalDMastName(QString signalName) {signalDMastName = signalName;}
    /*public*/ QString getSensorAName();
    /*public*/ void setSensorAName(QString sensorName);
    /*public*/ QString getSensorBName();
    /*public*/ void setSensorBName(QString sensorName);
    /*public*/ QString getSensorCName();
    /*public*/ void setSensorCName(QString sensorName);
    /*public*/ QString getSensorDName();
    /*public*/ void setSensorDName(QString sensorName);
    /*public*/ QObject* getConnectA() {return connectA;}
    /*public*/ QObject* getConnectB() {return connectB;}
    /*public*/ QObject* getConnectC() {return connectC;}
    /*public*/ QObject* getConnectD() {return connectD;}
    /*public*/ void setConnectA(QObject* o,int type);
    /*public*/ void setConnectB(QObject* o,int type);
    /*public*/ void setConnectC(QObject* o,int type);
    /*public*/ void setConnectD(QObject* o,int type);
    /*public*/ LayoutBlock* getLayoutBlockAC();
    /*public*/ LayoutBlock* getLayoutBlockBD();
    /*public*/ QPointF* getCoordsCenter();
    /*public*/ QPointF* getCoordsA();
    /*public*/ QPointF* getCoordsB();
    /*public*/ QPointF* getCoordsC();
    /*public*/ QPointF* getCoordsD();
    /**
     * Add Layout Blocks
     */
    /*public*/ void setLayoutBlockAC (LayoutBlock* b) ;
    /*public*/ void setLayoutBlockBD (LayoutBlock* b);
    /*public*/ void reCheckBlockBoundary();
    void removeSML(QString signalMast);
    /**
     * Methods to test if mainline track or not
     *  Returns true if either connecting track segment is mainline
     *  Defaults to not mainline if connecting track segments are missing
     */
    /*public*/ bool isMainlineAC();
    /*public*/ bool isMainlineBD();
    /**
     * Modify coordinates methods
     */
    /*public*/ void setCoordsCenter(QPointF* p);
    /*public*/ void setCoordsA(QPointF* p);
    /*public*/ void setCoordsB(QPointF* p);
    /*public*/ void setCoordsC(QPointF* p);
    /*public*/ void setCoordsD(QPointF* p);
    /*public*/ void scaleCoords(float xFactor, float yFactor);
    /**
     * Initialization method
     *   The above variables are initialized by PositionablePointXml, then the following
     *        method is called after the entire LayoutEditor is loaded to set the specific
     *        TrackSegment objects.
     */
    /*public*/ void setObjects(LayoutEditor* p);
    // initialization instance variables (used when loading a LayoutEditor)
    /*public*/ QString connectAName;// = "";
    /*public*/ QString connectBName;// = "";
    /*public*/ QString connectCName;// = "";
    /*public*/ QString connectDName;// = "";
    /*public*/ QString tBlockNameAC;// = "";
    /*public*/ QString tBlockNameBD;// = "";
    QMenu* popup;// = NULL;
    LayoutEditorTools* tools;// = NULL;
    /*public*/ QStringList getBlockBoundaries();
    /*public*/ void removeSignalMastLogic(SignalMast* sm);


signals:
    
public slots:
private:
    // defined constants

    // operational instance variables (not saved between sessions)
    /*private*/ LayoutBlock* blockAC;// = NULL;
    /*private*/ LayoutBlock* blockBD;// = NULL;
    /*private*/ LevelXing* instance;// = NULL;
    /*private*/ LayoutEditor* layoutEditor;// = NULL;

    // persistent instances variables (saved between sessions)
    /*private*/ QString ident;// = "";
    /*private*/ QString blockNameAC;// = "";
    /*private*/ QString blockNameBD; //"";
    /*private*/ QString signalAName; //"";  // signal at A track junction
    /*private*/ QString signalBName; //"";  // signal at B track junction
    /*private*/ QString signalCName; //"";  // signal at C track junction
    /*private*/ QString signalDName; //"";  // signal at D track junction

    /*private*/ QString signalAMastName; //"";  // signal at A track junction
    /*private*/ QString signalBMastName; //"";  // signal at B track junction
    /*private*/ QString signalCMastName; //"";  // signal at C track junction
    /*private*/ QString signalDMastName; //"";  // signal at D track junction

    /*/*private*/ QString sensorAName; //"";  // sensor at A track junction
    /*private*/ QString sensorBName; //"";  // sensor at B track junction
    /*private*/ QString sensorCName; //"";  // sensor at C track junction
    /*private*/ QString sensorDName; //"";  // sensor at D track junction    */

    /*private*/ NamedBeanHandle<Sensor*>* sensorANamed; //NULL; // sensor at A track junction
    /*private*/ NamedBeanHandle<Sensor*>* sensorBNamed; //NULL; // sensor at B track junction
    /*private*/ NamedBeanHandle<Sensor*>* sensorCNamed; //NULL; // sensor at C track junction
    /*private*/ NamedBeanHandle<Sensor*>* sensorDNamed; //NULL; // sensor at D track junction

    /*private*/ QObject* connectA; //NULL;
    /*private*/ QObject* connectB; //NULL;
    /*private*/ QObject* connectC; //NULL;
    /*private*/ QObject* connectD; //NULL;
    /*private*/ QPointF* center; //new QPointF(50.0,50.0);
    /*private*/ QPointF* dispA; //new QPointF(-20.0,0.0);
    /*private*/ QPointF* dispB; //new QPointF(-14.0,14.0);
    // variables for Edit Level Crossing pane
    QWidget* editLevelXingFrame;// = NULL;
    QTextEdit* block1Name;// = new QTextEdit(16);
    QTextEdit* block2Name;// = new QTextEdit(16);
    QPushButton* xingEditDone;
    QPushButton* xingEditCancel;
    QPushButton* xingEdit1Block;
    QPushButton* xingEdit2Block;
    bool editOpen;// = false;
    bool needsRedraw;// = false;
    bool needsBlockUpdate;// = false;
    /**
     * Clean up when this object is no longer needed.  Should not
     * be called while the object is still displayed; see remove()
     */
    void dispose();
    /**
     * Removes this object from display and persistance
     */
    void remove();
    bool active;// = true;
    /**
     * "active" means that the object is still displayed, and should be stored.
     */
    /*public*/ bool isActive();
    QVector<SignalMast*>* sml;// = new QVector<SignalMast*>();
    QVector<QMenuItem*> editAdditionalMenu;// = new QVector<QMenuItem*>(0);
    QVector<QMenuItem*> viewAdditionalMenu;// = new QVector<QMenuItem*>(0);
    /*public*/ void addViewPopUpMenu(QMenuItem menu);
    /*public*/ void setAdditionalEditPopUpMenu(QMenu* popup);
    /*public*/ void setAdditionalViewPopUpMenu(QMenu* popup);

Logger log;
/*private*/ void updateBlockInfo();
private slots:
void on_xingEdit1Block_clicked();
void on_xingEdit2Block_clicked();
void on_xingEditDone_clicked();
void on_xingEditCancel_clicked();

protected:
/**
 * Display popup menu for information and editing
 */
/*protected*/ void showPopUp(QMouseEvent* e, bool isEditable);
/**
 * Edit a Level Crossing
 */
/*protected*/ void editLevelXing(LevelXing* o);

};

#endif // LEVELXING_H
