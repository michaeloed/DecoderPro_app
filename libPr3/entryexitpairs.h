#ifndef ENTRYEXITPAIRS_H
#define ENTRYEXITPAIRS_H
#include "manager.h"
#include "logger.h"
#include <QMutex>
#include "exceptions.h"
#include "sensor.h"
#include <QColor>
#include "libPr3_global.h"
#include <QTimer>
#include "source.h"

class DeletePair;
class JDialog;
class StackNXPanel;
class DestinationPoints;
class SignalMastLogicManager;
class LayoutBlock;
class LayoutEditor;
class Source;
class PointDetails;
/*static*/ class StackDetails {
public:
        DestinationPoints* dp;
        bool reverse;

        StackDetails(DestinationPoints* dp, bool reverse) {
            this->dp = dp;
            this->reverse = reverse;
        }

        bool getReverse() {
            return reverse;
        }

        DestinationPoints* getDestinationPoint() {
            return dp;
        }
    };
/**
 * Class to store NX sets consisting of a source point, a destination point,
 * a direction and a reference.
 */
/*static*/ class SourceToDest {
public:
    Source* s = nullptr;
    DestinationPoints* dp = nullptr;
    bool direction = false;
    int ref = -1;

    /**
     * Constructor for a SourceToDest element.
     * @param s a source point
     * @param dp a destination point
     * @param dir a direction
     * @param ref Integer used as reference
     */
    SourceToDest(Source* s, DestinationPoints* dp, bool dir, int ref) {
        this->s = s;
        this->dp = dp;
        this->direction = dir;
        this->ref = ref;
    }
};

class LIBPR3SHARED_EXPORT EntryExitPairs : public Manager
{
    Q_OBJECT
public:
    explicit EntryExitPairs(QObject *parent = 0);
 ~EntryExitPairs() {}
 EntryExitPairs(const EntryExitPairs&) : Manager() {}
    /*public*/ int routingMethod;// = LayoutBlockConnectivityTools.METRIC;
    //Method to get delay between issuing Turnout commands
    /*public*/ int turnoutSetDelay;// = 0;

    /*final*/ static int HOPCOUNT;// = LayoutBlockConnectivityTools.HOPCOUNT;
    /*final*/ static int METRIC;// = LayoutBlockConnectivityTools.METRIC;

//    /*public*/ /*final*/ static int NXBUTTONSELECTED;// = 0x08;
//    /*public*/ /*final*/ static int NXBUTTONACTIVE;// = Sensor.ACTIVE;
//    /*public*/ /*final*/ static int NXBUTTONINACTIVE;// = Sensor.INACTIVE;
    enum NXSTATE
    {
     NXBUTTONSELECTED = 0x08,
     NXBUTTONACTIVE = Sensor::ACTIVE,
     NXBUTTONINACTIVE = Sensor::INACTIVE
    };

    /**
    * Constant value to represent that the entryExit will only set up the
    * turnouts between two different points
    */
    /*public*/ /*final*/ static int SETUPTURNOUTSONLY;// = 0x00;

    /**
    * Constant value to represent that the entryExit will set up the
    * turnouts between two different points and configure the signalmast logic
    * to use the correct blocks.
    */
    /*public*/ /*final*/ static int SETUPSIGNALMASTLOGIC;// = 0x01;

   /**
    * Constant value to represent that the entryExit will do full interlocking
    * it will set the turnouts and "reserve" the blocks.
    */
    /*public*/ /*final*/ static int FULLINTERLOCK;// = 0x02;
    enum ACTIONS
    {
     PROMPTUSER = 0x00,
     AUTOCLEAR = 0x01,
     AUTOCANCEL = 0x02,
     AUTOSTACK = 0x03
    };

    static QWidget* glassPane;// = new QWidget();
    /*public*/ void setDispatcherIntegration(bool boo);
    /*public*/ bool getDispatcherIntegration();
    /*public*/ QWidget* getGlassPane();
    /*public*/ void addNXSourcePoint(LayoutBlock* facing, LayoutBlock* protecting, NamedBean* loc, LayoutEditor* panel);
    /*public*/ void addNXSourcePoint(NamedBean* source);
    /*public*/ void addNXSourcePoint(NamedBean* source, LayoutEditor* panel);
    /*public*/ QObject* getEndPointLocation(NamedBean* source, LayoutEditor* panel);
    /*public*/ int getXMLOrder();
    /*public*/ NamedBean* getBySystemName(QString systemName);
    /*public*/ NamedBean* getBeanBySystemName(QString systemName);
    /*public*/ NamedBean* getBeanByUserName(QString userName);
    /*public*/ NamedBean* getNamedBean(QString name);
    /*public*/ char systemLetter();
    /*public*/ QString getSystemPrefix();
    /*public*/ char typeLetter() ;
    /*public*/ QString makeSystemName(QString s);
    /*public*/ QStringList getSystemNameArray();
    /*public*/ QStringList getSystemNameList();
    /*public*/ void Register(NamedBean* n);
    /*public*/ void deregister(NamedBean* n);
    /*public*/ void setClearDownOption(int i);
    /*public*/ int getClearDownOption();
    /*public*/ void dispose();
    /*public*/ QObjectList* getSourceList(LayoutEditor* panel);
    /*public*/ Source* getSourceForPoint(PointDetails* pd);
    /*public*/ void setSingleSegmentRoute(QString nxPair);
    /*public*/ void setMultiPointRoute(PointDetails* requestpd, LayoutEditor* panel);
    /*public*/ int getNxPairNumbers(LayoutEditor* panel);
    /*public*/ QObjectList getNxSource(LayoutEditor* panel);
    /*public*/ QObjectList getNxDestination();
    /*public*/ QList<LayoutEditor*> getSourcePanelList();
    /*public*/ void addNXDestination(NamedBean* source, NamedBean* destination, LayoutEditor* panel);
    /*public*/ void addNXDestination(NamedBean* source, NamedBean* destination, LayoutEditor* panel, QString id);
    /*public*/ QObjectList* getDestinationList(QObject* obj, LayoutEditor* panel);
    /*public*/ bool deleteNxPair(NamedBean* sensor);
    /*public*/ void deleteNxPair(NamedBean* source, NamedBean* destination, LayoutEditor* panel);
    /*public*/ bool isDestinationValid(QObject* source, QObject* dest, LayoutEditor* panel);
    /*public*/ bool isUniDirection(QObject* source, LayoutEditor* panel, QObject* dest);
    /*public*/ void setUniDirection(QObject* source, LayoutEditor* panel, QObject* dest, bool set);
    /*public*/ bool canBeBiDirectional(QObject* source, LayoutEditor* panel, QObject* dest);
    /*public*/ bool isEnabled(QObject* source, LayoutEditor* panel, QObject* dest);
    /*public*/ void setEnabled(QObject* source, LayoutEditor* panel, QObject* dest, bool set);
    /*public*/ void setEntryExitType(QObject* source, LayoutEditor* panel, QObject* dest, int set);
    /*public*/ int getEntryExitType(QObject* source, LayoutEditor* panel, QObject* dest);
    /*public*/ QString getUniqueId(QObject* source, LayoutEditor* panel, QObject* dest);
    /*public*/ QStringList getEntryExitList();
    /*public*/ bool isPathActive(QObject* sourceObj, QObject* destObj, LayoutEditor* panel);
    /*public*/ void cancelInterlock(QObject* source, LayoutEditor* panel, QObject* dest);

    /*public*/ /*final*/ static int CANCELROUTE;// = 0;
    /*public*/ /*final*/ static int CLEARROUTE;// = 1;
    /*public*/ /*final*/ static int EXITROUTE;// = 2;
    /*public*/ /*final*/ static int STACKROUTE;// = 4;
    /*/*public*/ static void flashSensor(PointDetails* pd);
    /*public*/ static void stopFlashSensor(PointDetails* pd);
    /*synchronized*/ /*public*/ void setNXButtonState(PointDetails* nxPoint, int state);
    /*public*/ PointDetails* getPointDetails(QObject* obj, LayoutEditor* panel);
    PointDetails* getPointDetails(LayoutBlock* source, LayoutBlock* destination, LayoutEditor* panel);
    //No point in have multiple copies of what is the same thing.
    static QList<PointDetails*>* pointDetails;// = new ArrayList<PointDetails>();
    /*public*/ QString getPointAsString(NamedBean* obj, LayoutEditor* panel);
    /*public*/ void removePropertyChangeListener(PropertyChangeListener* list, NamedBean* obj, LayoutEditor* panel);
//    java.beans.PropertyChangeSupport pcs = new java.beans.PropertyChangeSupport(this);
    /*public*/ /*synchronized*/ void addPropertyChangeListener(PropertyChangeListener* l);
    /*public*/ /*synchronized*/ void removePropertyChangeListener(PropertyChangeListener* l);
    /*protected*/ void firePropertyChange(QString p, QVariant old, QVariant n) ;
    /*public*/ void automaticallyDiscoverEntryExitPairs(LayoutEditor* editor, int interlockType) throw (JmriException);
    /*public*/ int getSettingTimer();
    /*public*/ void setSettingTimer(int i);
    /*public*/ bool useDifferentColorWhenSetting();
    /*public*/ QColor getSettingRouteColor();
    /*public*/ void setSettingRouteColor(QColor col);
    /*public*/ bool isAssignableFromType() {return true;}
    /*public*/ QSet<NamedBean*> getNamedBeanSet();
    /*synchronized*/ /*public*/ void stackNXRoute(DestinationPoints* dp, bool reverse);
    /*public*/ QList<DestinationPoints*> getStackedInterlocks();
    /*public*/ bool isRouteStacked(DestinationPoints* dp, bool reverse);
    /*synchronized*/ /*public*/ void cancelStackedRoute(DestinationPoints* dp, bool reverse);
    /*public*/ QList<QString> layoutBlockSensors(/*@Nonnull*/ LayoutBlock* layoutBlock);

signals:
    void propertyChange(PropertyChangeEvent*);
public slots:
    void on_propertyChange(PropertyChangeEvent*);
    /*public*/ void propertyDestinationPropertyChange(PropertyChangeEvent* e);

private:
    int routeClearOption;// = PROMPTUSER;
    /*private*/ PointDetails* providePoint(NamedBean* source, LayoutEditor* panel);
    Logger* log;
    QObjectList destinationList;// = new ArrayList<Object>();
    /*private*/ PointDetails* providePoint(LayoutBlock* source, LayoutBlock* protecting, LayoutEditor* panel);
    bool runWhenStablised;// = false;
    LayoutEditor* toUseWhenStable;
    int interlockTypeToUseWhenStable;
    SignalMastLogicManager* smlm;
    QMutex mutex;
    /*private*/ int settingTimer;// = 2000;
    /*private*/ QColor settingRouteColor;// = null;
    bool allocateToDispatcher;// = false;
    /*private*/ void setMultiPointRoute(PointDetails* fromPd, PointDetails* toPd);
    int refCounter = 0;
    /**
     * List holding SourceToDest sets of routes between two points.
     */
    QList<SourceToDest*> routesToSet;// = new ArrayList<>();
    int currentDealing = 0;
    /*synchronized*/ void processRoutesToSet();
    /*synchronized*/ void removeRemainingRoute();
    QList<StackDetails*> stackList;// = new ArrayList<>();
    QTimer* checkTimer;
    StackNXPanel* stackPanel = nullptr;
    JDialog* stackDialog = nullptr;
    /**
     * List of NX pairs that are scheduled for deletion.
     * @since 4.11.2
     */
    QList<DeletePair*> deletePairList;// = new ArrayList<>();
    /*private*/ bool checkNxPairs();
    /*private*/ bool confirmDeletePairs();
    /*private*/ void deleteNxPairs();
    QHash<PointDetails*, Source*> nxpair;// = new HashMap<>();
    void createDeletePairList(NamedBean* sensor);

private slots:
    /*synchronized*/ void checkRoute();
 friend class DeletePair;
};
Q_DECLARE_METATYPE(EntryExitPairs)

/**
 * Class to store NX pair components.
 * @since 4.11.2
 */
class DeletePair {
public:
    NamedBean* src = nullptr;
    NamedBean* dest = nullptr;
    LayoutEditor* pnl = nullptr;
    DestinationPoints* dp = nullptr;
    EntryExitPairs* pairs;

    /**
     * Constructor for a DeletePair row.
     * @param src Source sensor bean
     * @param dest Ddestination sensor bean
     * @param pnl The LayoutEditor panel for the source bean
     */
    DeletePair(NamedBean* src, NamedBean* dest, LayoutEditor* pnl, EntryExitPairs* pairs) {
        this->src = src;
        this->dest = dest;
        this->pnl = pnl;
     this->pairs = pairs;

        // Get the actual destination point, if any.
        PointDetails* sourcePoint = pairs->getPointDetails(src, pnl);
        PointDetails* destPoint = pairs->getPointDetails(dest, pnl);
        if (sourcePoint != nullptr && destPoint != nullptr) {
            if (pairs->nxpair.contains(sourcePoint)) {
                this->dp = pairs->nxpair.value(sourcePoint)->getDestForPoint(destPoint);
            }
        }
    }
};

#endif // ENTRYEXITPAIRS_H
