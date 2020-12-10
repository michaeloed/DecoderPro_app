#ifndef CODEBUTTONHANDLERDATA_H
#define CODEBUTTONHANDLERDATA_H

#include <QObject>
#include "nbhsignal.h"
#include "nbhsensor.h"
#include <qlist.h>
#include "nbhturnout.h"
#include <QButtonGroup>
#include "projectscommonsubs.h"
#include <QMap>
#include "callondata.h"
#include "trafficlockingdata.h"

class CodeButtonHandlerData : public QObject
{
  Q_OBJECT
 public:
  explicit CodeButtonHandlerData(int uniqueID, int switchNumber, int signalEtcNumber, int guiColumnNumber, QObject *parent = nullptr);
  /*public*/ static /*final*/ int SWITCH_NOT_SLAVED;// = -1;
  /*public*/ /*enum*/ class LOCK_IMPLEMENTATION
  {
   public:
// The values in paren's are the RadioGroup values set by "CommonSubs.numberButtonGroup",
// gotten by calling "CommonSubs.getButtonSelectedInt".
    enum LOCK_IMPLEMENTATIONS
    {
      GREGS = 0,
      OTHER = 1
    };
   public:
      /*private*/ /*final*/ int _mRadioGroupValue;
      /*private*/ /*final*/ static QMap<int, LOCK_IMPLEMENTATION> map;// = new HashMap<>();
      /*private*/ LOCK_IMPLEMENTATION (int radioGroupValue=0) { _mRadioGroupValue = radioGroupValue; }
//      static { for (LOCK_IMPLEMENTATION value : LOCK_IMPLEMENTATION.values()) { map.put(value._mRadioGroupValue, value); }}
      /*public*/ int getInt() { return _mRadioGroupValue; }
      /*public*/ static LOCK_IMPLEMENTATION getLockImplementation(int radioGroupValue);// { return map.value(radioGroupValue); }
      /*public*/ static LOCK_IMPLEMENTATION getLockImplementation(QButtonGroup* buttonGroup);// { return map.value(ProjectsCommonSubs::getButtonSelectedInt(buttonGroup)); }
   friend class CodeButtonHandlerData;
  };

  /*public*/ /*enum*/ class TURNOUT_TYPE {
   public:
    enum TURNOUT_TYPES
    {
// The values in paren's are the RadioGroup values set by "CommonSubs.numberButtonGroup",
// gotten by calling "CommonSubs.getButtonSelectedInt".
      TURNOUT = 0,
   CROSSOVER=1,
   DOUBLE_CROSSOVER = 2
  };
      /*private*/ /*final*/ int _mRadioGroupValue;
      /*private*/ /*final*/ static QMap<int, TURNOUT_TYPE> map;// = new HashMap<>();
      /*private*/ TURNOUT_TYPE (int radioGroupValue=0) { _mRadioGroupValue = radioGroupValue; }
//      static { for (TURNOUT_TYPE value : TURNOUT_TYPE.values()) { map.put(value._mRadioGroupValue, value); }}
      /*public*/ int getInt() { return _mRadioGroupValue; }
      /*public*/ static TURNOUT_TYPE getTurnoutType(int radioGroupValue);// { return map.value(radioGroupValue); }
      /*public*/ static TURNOUT_TYPE getTurnoutType(QButtonGroup* buttonGroup);// { return map.value(ProjectsCommonSubs::getButtonSelectedInt(buttonGroup)); }
    friend class CodeButtonHandlerData;
  };

  /*public*/ enum TRAFFIC_DIRECTION {
      LEFT,
      BOTH,
      RIGHT
  };

  /*
  Because of "getAllInternalSensorStringFields", ANY JMRI sensor object that we
  create should have "InternalSensor" (case sensitive,
  example: _mCodeButtonInternalSensor) as ANY PART of their variable name and
  declared as type String.  This will insure that the GUI program will write these
  sensors out to a separate file for JMRI to load to automatically create
  these senosrs.  Other sensors that pre-exist within JMRI should NOT have
  that as part of their variable name (ex: _mOSSectionOccupiedExternalSensor).

  Also, see CheckJMRIObject's "public static final String EXTERNAL_xxx" definitions
  at the top for "automatic" JMRI object verification.
  */

      /*public*/ NBHSensor*            _mCodeButtonInternalSensor;
      /*public*/ NBHSensor*            _mOSSectionOccupiedExternalSensor;              // Required
      /*public*/ NBHSensor*            _mOSSectionOccupiedExternalSensor2;             // Optional
      /*public*/ int                  _mOSSectionSwitchSlavedToUniqueID;
      /*public*/ int                  _mGUIColumnNumber;
      /*public*/ bool              _mGUIGeneratedAtLeastOnceAlready;
      /*public*/ int                  _mCodeButtonDelayTime;
  //  Signal Direction Indicators:
      /*public*/ bool              _mSIDI_Enabled;
      /*public*/ NBHSensor*            _mSIDI_LeftInternalSensor;
      /*public*/ NBHSensor*            _mSIDI_NormalInternalSensor;
      /*public*/ NBHSensor*            _mSIDI_RightInternalSensor;
      /*public*/ int                  _mSIDI_CodingTimeInMilliseconds;
      /*public*/ int                  _mSIDI_TimeLockingTimeInMilliseconds;
      /*public*/ TRAFFIC_DIRECTION    _mSIDI_TrafficDirection;
      /*public*/ QList<NBHSignal*> _mSIDI_LeftRightTrafficSignals =QList<NBHSignal*>();
      /*public*/ QList<NBHSignal*> _mSIDI_RightLeftTrafficSignals = QList<NBHSignal*>();
  //  Signal Direction Lever:
      /*public*/ bool              _mSIDL_Enabled;
      /*public*/ NBHSensor*            _mSIDL_LeftInternalSensor;
      /*public*/ NBHSensor*            _mSIDL_NormalInternalSensor;
      /*public*/ NBHSensor*            _mSIDL_RightInternalSensor;
  //  Switch Direction Indicators:
      /*public*/ bool              _mSWDI_Enabled;
      /*public*/ NBHSensor*            _mSWDI_NormalInternalSensor;
      /*public*/ NBHSensor*            _mSWDI_ReversedInternalSensor;
      /*public*/ NBHTurnout*           _mSWDI_ExternalTurnout;
      /*public*/ int                  _mSWDI_CodingTimeInMilliseconds;
      /*public*/ bool              _mSWDI_FeedbackDifferent;
      /*public*/ TURNOUT_TYPE         _mSWDI_GUITurnoutType;
      /*public*/ bool              _mSWDI_GUITurnoutLeftHand;
      /*public*/ bool              _mSWDI_GUICrossoverLeftHand;
  //  Switch Direction Lever:
      /*public*/ bool              _mSWDL_Enabled;
      /*public*/ NBHSensor            _mSWDL_InternalSensor;
  //  Call On:
      /*public*/ bool              _mCO_Enabled;
      /*public*/ NBHSensor            _mCO_CallOnToggleInternalSensor;
      /*public*/ QList<CallOnData*> _mCO_GroupingsList = QList<CallOnData*>();
  //  Traffic Locking:
      /*public*/ bool              _mTRL_Enabled;
      /*public*/ QList<TrafficLockingData*> _mTRL_LeftTrafficLockingRules = QList<TrafficLockingData*>();
      /*public*/ QList<TrafficLockingData*> _mTRL_RightTrafficLockingRules = QList<TrafficLockingData*>();
  //  Turnout Locking:
      /*public*/ bool              _mTUL_Enabled;
      /*public*/ NBHSensor*            _mTUL_DispatcherInternalSensorLockToggle;
      /*public*/ NBHTurnout*           _mTUL_ExternalTurnout;
      /*public*/ bool              _mTUL_ExternalTurnoutFeedbackDifferent;
      /*public*/ NBHSensor*            _mTUL_DispatcherInternalSensorUnlockedIndicator;
      /*public*/ bool              _mTUL_NoDispatcherControlOfSwitch;
      /*public*/ bool              _mTUL_ndcos_WhenLockedSwitchStateIsClosed;
      /*public*/ bool              _mTUL_GUI_IconsEnabled;
      /*public*/ LOCK_IMPLEMENTATION  _mTUL_LockImplementation;
      /*public*/ NBHTurnout*           _mTUL_AdditionalExternalTurnout1;
      /*public*/ bool              _mTUL_AdditionalExternalTurnout1FeedbackDifferent;
      /*public*/ NBHTurnout*           _mTUL_AdditionalExternalTurnout2;
      /*public*/ bool              _mTUL_AdditionalExternalTurnout2FeedbackDifferent;
      /*public*/ NBHTurnout*           _mTUL_AdditionalExternalTurnout3;
      /*public*/ bool              _mTUL_AdditionalExternalTurnout3FeedbackDifferent;
  //  Indication Locking (Signals):
      /*public*/ bool              _mIL_Enabled;
      /*public*/ QList<NBHSignal*>      _mIL_Signals = QList<NBHSignal*>();

  //  This number NEVER changes, and is how this object is uniquely identified:
      /*public*/ int _mUniqueID = -1;         // FORCE serialization to write out the FIRST unique number 0 into the XML file (to make me happy!)
      /*public*/ int _mSwitchNumber;         // Switch Indicators and lever #
      /*public*/ int _mSignalEtcNumber;      // Signal Indicators, lever, locktoggle, callon and code button number

      /*public*/ QString myString() { return tr("Switch #") + " " + _mSwitchNumber + ", " + tr("Signal Etc. #:") + " " + _mSignalEtcNumber + tr(", Col #:") + " " + _mGUIColumnNumber + (_mGUIGeneratedAtLeastOnceAlready ? "*" : "") + ", [" + _mUniqueID + "]"; }  // NOI18N
      /*public*/ QString myShortStringNoComma() { return _mSwitchNumber + "/" + _mSignalEtcNumber; }

 signals:

 public slots:
};

#endif // CODEBUTTONHANDLERDATA_H
