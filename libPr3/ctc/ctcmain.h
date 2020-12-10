#ifndef CTCMAIN_H
#define CTCMAIN_H

#include <QObject>
#include "codebuttonhandler.h"
#include "propertychangelistener.h"
#include "timer.h"
#include "ctcexceptionbuffer.h"
#include "lockedroutesmanager.h"
#include <QMap>


class CTCSerialData;
class Logger;
class CTCMain : public QObject
{
  Q_OBJECT
 public:
  explicit CTCMain(QObject *parent = nullptr);
  /*public*/ void externalLockTurnout();

 signals:

 public slots:
 private:
  /*private*/ /*final*/ static Logger* log;// = LoggerFactory.getLogger(CTCMain.class);
  /*private*/ /*final*/ CTCSerialData* _mCTCSerialData;
  /*private*/ /*final*/ QList<CodeButtonHandler*> _mCodeButtonHandlersArrayList = QList<CodeButtonHandler*>();       // "Const" after initialization completes.
  /*private*/ NBHSensor* _mCTCDebugSystemReloadInternalSensor = nullptr;
  /*private*/ /*final*/ PropertyChangeListener* _mCTCDebugSystemReloadInternalSensorPropertyChangeListener;// = (PropertyChangeEvent e) -> { handleCTCDebugSystemReload(e); };
  /*private*/ NBHSensor* _mCTCDebug_TrafficLockingRuleTriggeredDisplayInternalSensor = nullptr;
  /*private*/ /*final*/ PropertyChangeListener* _mCTCDebug_TrafficLockingRuleTriggeredDisplayInternalSensorPropertyChangeListener;// = (PropertyChangeEvent e) -> { handleLogging(e); };
//  /*private*/ /*final*/ QMap<int, SignalDirectionIndicatorsInterface*> _mSIDIHashMap = QMap<int, SignalDirectionIndicatorsInterface*>(); // "Const" after initialization completes.
//  /*private*/ /*final*/ QMap<int, SwitchDirectionIndicators> _mSWDIHashMap = QMap<int, SwitchDirectionIndicators>();          // "Const" after initialization completes.
  /*private*/ /*final*/ QMap<int, CodeButtonHandler*> _mCBHashMap = QMap<int, CodeButtonHandler*>();                    // "Const" after initialization completes.
  /*private*/ /*final*/ LockedRoutesManager* _mLockedRoutesManager = new LockedRoutesManager();
  /*private*/ Timer* _mLockTurnoutsTimer = nullptr;
  /*private*/ /*final*/ CTCExceptionBuffer* _mCTCExceptionBuffer = new CTCExceptionBuffer();

 private slots:
  /*private*/ void handleCTCDebugSystemReload(PropertyChangeEvent* e);

};

#endif // CTCMAIN_H
