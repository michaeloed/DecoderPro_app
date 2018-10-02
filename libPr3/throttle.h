#ifndef THROTTLE_H
#define THROTTLE_H

#include <QObject>
#include "basicrosterentry.h"
#include "propertychangelistener.h"
#include "Throttle/throttlelistener.h"
#include "libPr3_global.h"

class ThrottleListener;
class LIBPR3SHARED_EXPORT Throttle : public QObject
{
    Q_OBJECT
public:
    explicit Throttle(QObject *parent = 0) : QObject(parent) {}
    /**
     * A Throttle object can be manipulated to change the speed, direction
     * and functions of a single locomotive.
     * <P>
     * A Throttle implementation provides the actual control mechanism.
     * These are obtained via a {@link ThrottleManager}.
     * <P>
     * With some control systems, there are only
     * a limited number of Throttle's available.
     * <p>
     * On DCC systems, Throttles are often actually
     * {@link DccThrottle} objects, which have some additional
     * DCC-specific capabilities.
     * <hr>
     * This file is part of JMRI.
     * <P>
     * JMRI is free software; you can redistribute it and/or modify it under
     * the terms of version 2 of the GNU General Public License as published
     * by the Free Software Foundation. See the "COPYING" file for a copy
     * of this license.
     * <P>
     * JMRI is distributed in the hope that it will be useful, but WITHOUT
     * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
     * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
     * for more details.
     * <P>
     *
     * @author			Bob Jacobsen Copyright (C) 2001, 2008
     * @version			$Revision: 22484 $
     */
//    public interface Throttle {

        /**
         * Constants to represent the functions F0 through F28.
         */
        static QString F0; //F0"; // NOI18N
        static QString F1; //F1"; // NOI18N
        static QString F2; //F2"; // NOI18N
        static QString F3; //F3"; // NOI18N
        static QString F4; //F4"; // NOI18N
        static QString F5; //F5"; // NOI18N
        static QString F6; //F6"; // NOI18N
        static QString F7; //F7"; // NOI18N
        static QString F8; //F8"; // NOI18N
        static QString F9; //F9"; // NOI18N
        static QString F10; //F10"; // NOI18N
        static QString F11; //F11"; // NOI18N
        static QString F12; //F12"; // NOI18N
        static QString F13; //F13"; // NOI18N
        static QString F14; //F14"; // NOI18N
        static QString F15; //F15"; // NOI18N
        static QString F16; //F16"; // NOI18N
        static QString F17; //F17"; // NOI18N
        static QString F18; //F18"; // NOI18N
        static QString F19; //F19"; // NOI18N
        static QString F20; //F20"; // NOI18N
        static QString F21; //F21"; // NOI18N
        static QString F22; //F22"; // NOI18N
        static QString F23; //F23"; // NOI18N
        static QString F24; //F24"; // NOI18N
        static QString F25; //F25"; // NOI18N
        static QString F26; //F26"; // NOI18N
        static QString F27; //F27"; // NOI18N
        static QString F28; //F28"; // NOI18N
        /**
         * Constants to represent the functions F0 through F28.
         */
        static QString F0Momentary; //F0Momentary"; // NOI18N
        static QString F1Momentary; //F1Momentary"; // NOI18N
        static QString F2Momentary; //F2Momentary"; // NOI18N
        static QString F3Momentary; //F3Momentary"; // NOI18N
        static QString F4Momentary; //F4Momentary"; // NOI18N
        static QString F5Momentary; //F5Momentary"; // NOI18N
        static QString F6Momentary; //F6Momentary"; // NOI18N
        static QString F7Momentary; //F7Momentary"; // NOI18N
        static QString F8Momentary; //F8Momentary"; // NOI18N
        static QString F9Momentary; //F9Momentary"; // NOI18N
        static QString F10Momentary; //F10Momentary"; // NOI18N
        static QString F11Momentary; //F11Momentary"; // NOI18N
        static QString F12Momentary; //F12Momentary"; // NOI18N
        static QString F13Momentary; //F13Momentary"; // NOI18N
        static QString F14Momentary; //F14Momentary"; // NOI18N
        static QString F15Momentary; //F15Momentary"; // NOI18N
        static QString F16Momentary; //F16Momentary"; // NOI18N
        static QString F17Momentary; //F17Momentary"; // NOI18N
        static QString F18Momentary; //F18Momentary"; // NOI18N
        static QString F19Momentary; //F19Momentary"; // NOI18N
        static QString F20Momentary; //F20Momentary"; // NOI18N
        static QString F21Momentary; //F21Momentary"; // NOI18N
        static QString F22Momentary; //F22Momentary"; // NOI18N
        static QString F23Momentary; //F23Momentary"; // NOI18N
        static QString F24Momentary; //F24Momentary"; // NOI18N
        static QString F25Momentary; //F25Momentary"; // NOI18N
        static QString F26Momentary; //F26Momentary"; // NOI18N
        static QString F27Momentary; //F27Momentary"; // NOI18N
        static QString F28Momentary; //F28Momentary"; // NOI18N

        /** Speed - expressed as a value 0.0 -> 1.0. Negative means emergency stop.
         * This is an bound property.
         */
        virtual float getSpeedSetting() {return 0.0;}
        virtual void setSpeedSetting(float /*speed*/) {}

        /** direction
         * This is an bound property.
         */
        virtual bool getIsForward() {return false;}
        virtual void setIsForward(bool  /*forward*/) {}

        // functions - note that we use the naming for DCC, though that's not the implication;
        // see also DccThrottle interface
        Q_INVOKABLE virtual bool getF0() {return false;}
        virtual void setF0(bool  /*f0*/) {}

        Q_INVOKABLE virtual bool getF1() {return false;}
        virtual void setF1(bool  /*f1*/) {}

        Q_INVOKABLE virtual bool getF2() {return false;}
        virtual void setF2(bool  /*f2*/) {}

        Q_INVOKABLE virtual bool getF3() {return false;}
        virtual void setF3(bool  /*f3*/) {}

        Q_INVOKABLE virtual bool getF4() {return false;}
        virtual void setF4(bool  /*f4*/) {}

        Q_INVOKABLE virtual bool getF5() {return false;}
        virtual void setF5(bool  /*f5*/) {}

        Q_INVOKABLE virtual bool getF6() {return false;}
        virtual void setF6(bool  /*f6*/) {}

        Q_INVOKABLE virtual bool getF7() {return false;}
        virtual void setF7(bool  /*f7*/) {}

        Q_INVOKABLE virtual bool getF8() {return false;}
        virtual void setF8(bool  /*f8*/) {}

        Q_INVOKABLE virtual bool getF9() {return false;}
        virtual void setF9(bool  /*f9*/) {}

        Q_INVOKABLE virtual bool getF10() {return false;}
        virtual void setF10(bool  /*f10*/) {}

        Q_INVOKABLE virtual bool getF11() {return false;}
        virtual void setF11(bool  /*f11*/) {}

        Q_INVOKABLE virtual bool getF12() {return false;}
        virtual void setF12(bool  /*f12*/) {}

        Q_INVOKABLE virtual bool getF13() {return false;}
        virtual void setF13(bool  /*f13*/) {}

        Q_INVOKABLE virtual bool getF14() {return false;}
        virtual void setF14(bool  /*f14*/) {}

        Q_INVOKABLE virtual bool getF15() {return false;}
        virtual void setF15(bool  /*f15*/) {}

        Q_INVOKABLE virtual bool getF16() {return false;}
        virtual void setF16(bool  /*f16*/) {}

        Q_INVOKABLE virtual bool getF17() {return false;}
        virtual void setF17(bool  /*f17*/) {}

        Q_INVOKABLE virtual bool getF18() {return false;}
        virtual void setF18(bool  /*f18*/) {}

        Q_INVOKABLE virtual bool getF19() {return false;}
        virtual void setF19(bool  /*f19*/) {}

        Q_INVOKABLE virtual bool getF20() {return false;}
        virtual void setF20(bool  /*f20*/) {}

        Q_INVOKABLE virtual bool getF21() {return false;}
        virtual void setF21(bool  /*f21*/) {}

        Q_INVOKABLE virtual bool getF22() {return false;}
        virtual void setF22(bool  /*f22*/) {}

        Q_INVOKABLE virtual bool getF23() {return false;}
        virtual void setF23(bool  /*f23*/) {}

        Q_INVOKABLE virtual bool getF24() {return false;}
        virtual void setF24(bool  /*f24*/) {}

        Q_INVOKABLE virtual bool getF25() {return false;}
        virtual void setF25(bool  /*f25*/) {}

        Q_INVOKABLE virtual bool getF26() {return false;}
        virtual void setF26(bool  /*f26*/) {}

        Q_INVOKABLE virtual bool getF27() {return false;}
        virtual void setF27(bool  /*f27*/) {}

        Q_INVOKABLE virtual bool getF28() {return false;}
        virtual void setF28(bool  /*f28*/) {}

        // functions momentary status - note that we use the naming for DCC,
        // though that's not the implication;
        // see also DccThrottle interface
        virtual bool getF0Momentary() {return false;}
        virtual void setF0Momentary(bool  /*f0Momentary*/) {}

        virtual bool getF1Momentary()  {return false;}
        virtual void setF1Momentary(bool  /*f1Momentary*/)  {}

        virtual bool getF2Momentary()  {return false;}
        virtual void setF2Momentary(bool  /*f2Momentary*/)  {}

        virtual bool getF3Momentary()  {return false;}
        virtual void setF3Momentary(bool  /*f3Momentary*/)  {}

        virtual bool getF4Momentary()  {return false;}
        virtual void setF4Momentary(bool  /*f4Momentary*/)  {}

        virtual bool getF5Momentary()  {return false;}
        virtual void setF5Momentary(bool  /*f5Momentary*/)  {}

        virtual bool getF6Momentary()  {return false;}
        virtual void setF6Momentary(bool  /*f6Momentary*/)  {}

        virtual bool getF7Momentary()  {return false;}
        virtual void setF7Momentary(bool  /*f7Momentary*/)  {}

        virtual bool getF8Momentary()  {return false;}
        virtual void setF8Momentary(bool  /*f8Momentary*/)  {}

        virtual bool getF9Momentary()  {return false;}
        virtual void setF9Momentary(bool  /*f9Momentary*/)  {}

        virtual bool getF10Momentary()  {return false;}
        virtual void setF10Momentary(bool  /*f10Momentary*/)  {}

        virtual bool getF11Momentary()  {return false;}
        virtual void setF11Momentary(bool  /*f11Momentary*/)  {}

        virtual bool getF12Momentary()  {return false;}
        virtual void setF12Momentary(bool  /*f12Momentary*/)  {}

        virtual bool getF13Momentary()  {return false;}
        virtual void setF13Momentary(bool  /*f13Momentary*/)  {}

        virtual bool getF14Momentary()  {return false;}
        virtual void setF14Momentary(bool  /*f14Momentary*/)  {}

        virtual bool getF15Momentary()  {return false;}
        virtual void setF15Momentary(bool  /*f15Momentary*/)  {}

        virtual bool getF16Momentary()  {return false;}
        virtual void setF16Momentary(bool  /*f16Momentary*/)  {}

        virtual bool getF17Momentary()  {return false;}
        virtual void setF17Momentary(bool  /*f17Momentary*/)  {}

        virtual bool getF18Momentary()  {return false;}
        virtual void setF18Momentary(bool  /*f18Momentary*/)  {}

        virtual bool getF19Momentary()  {return false;}
        virtual void setF19Momentary(bool  /*f19Momentary*/)  {}

        virtual bool getF20Momentary()  {return false;}
        virtual void setF20Momentary(bool  /*f20Momentary*/)  {}

        virtual bool getF21Momentary()  {return false;}
        virtual void setF21Momentary(bool  /*f21Momentary*/)  {}

        virtual bool getF22Momentary()  {return false;}
        virtual void setF22Momentary(bool  /*f22Momentary*/)  {}

        virtual bool getF23Momentary()  {return false;}
        virtual void setF23Momentary(bool  /*f23Momentary*/)  {}

        virtual bool getF24Momentary()  {return false;}
        virtual void setF24Momentary(bool  /*f24Momentary*/)  {}

        virtual bool getF25Momentary()  {return false;}
        virtual void setF25Momentary(bool  /*f25Momentary*/)  {}

        virtual bool getF26Momentary()  {return false;}
        virtual void setF26Momentary(bool  /*f26Momentary*/)  {}

        virtual bool getF27Momentary()  {return false;}
        virtual void setF27Momentary(bool  /*f27Momentary*/)  {}

        virtual bool getF28Momentary()  {return false;}
        virtual void setF28Momentary(bool  /*f28Momentary*/)  {}

        /**
         * Locomotive address.  The exact format is defined by the
         * specific implementation, as subclasses of LocoAddress will contain
         * different information.
         *
         * This is an unbound property.
         */
        virtual LocoAddress* getLocoAddress()  {return NULL;}

        // register for notification if any of the properties change
        virtual void removePropertyChangeListener(PropertyChangeListener* /*p*/)  {}
        virtual void addPropertyChangeListener(PropertyChangeListener* /*p*/)  {}
        virtual QVector<PropertyChangeListener*>* getListeners()  {return NULL;}


        /**
         * Not for general use, see {@link #release()} and {@link #dispatch()}.
         * <p>
         * Dispose of object when finished it.  This does not
         * free any hardware resources used; rather, it just cleans up the
         * software implementation.
         * <P>
         * Used for handling certain internal error conditions, where
         * the object still exists but hardware is not associated with it.
         * <P>
         * After this, further usage of
         * this Throttle object will result in a JmriException.
         * @deprecated Calls to dispose of a throttle should now be made via the
         * throttle manager or by using {@link #dispose(ThrottleListener l)}.
         */
//        @Deprecated
//        virtual void dispose();

        /**
         * Finished with this Throttle, tell the layout that the locomotive
         * is available for reuse/reallocation by somebody else.
         * <P>
         * After this, further usage of
         * this Throttle object will result in a JmriException.
         * Do not call dispose after release.
         * <P>
         * Normally, release ends with a call to dispose.
         * @deprecated Calls to dispose of a throttle should now be made via the
         * throttle manager or by using {@link #release(ThrottleListener l)}
         */
//        @Deprecated
//        virtual void release();

        /**
         * Finished with this Throttle, tell the layout that the locomotive
         * is available for reuse/reallocation by somebody else. If possible,
         * tell the layout that this locomotive has been dispatched to another user.
         * Not all layouts will implement this, in which case it is synomous with
         * release();
         * <P>
         * After this, further usage of
         * this Throttle object will result in a JmriException.
         * <P>
         * Normally, dispatch ends with a call to dispose.
         * @deprecated Calls to dispose of a throttle should now be made via the
         * throttle manager, or by using {@link #dispatch(ThrottleListener l)}
         */
//        @Deprecated
//        virtual void dispatch();


            /**
         * Not for general use, see {@link #release(ThrottleListener l)} and {@link #dispatch(ThrottleListener l)}.
         * <p>
         * Dispose of object when finished it.  This does not
         * free any hardware resources used; rather, it just cleans up the
         * software implementation.
         * <P>
         * Used for handling certain internal error conditions, where
         * the object still exists but hardware is not associated with it.
         * <P>
         * After this, further usage of
         * this Throttle object will result in a JmriException.
         */
        virtual void dispose(ThrottleListener* /*l*/)  {}

        /**
         * Finished with this Throttle, tell the layout that the locomotive
         * is available for reuse/reallocation by somebody else.
         * <P>
         * After this, further usage of
         * this Throttle object will result in a JmriException.
         * Do not call dispose after release.
         * <P>
         * Normally, release ends with a call to dispose.
         */
        virtual void release(ThrottleListener* /*l*/)  {}

        /**
         * Finished with this Throttle, tell the layout that the locomotive
         * is available for reuse/reallocation by somebody else. If possible,
         * tell the layout that this locomotive has been dispatched to another user.
         * Not all layouts will implement this, in which case it is synomous with
         * release();
         * <P>
         * After this, further usage of
         * this Throttle object will result in a JmriException.
         * <P>
         * Normally, dispatch ends with a call to dispose.
         */
        virtual void dispatch(ThrottleListener* /*l*/)  {}

        virtual void setRosterEntry(BasicRosterEntry* /*re*/)  {}
        virtual BasicRosterEntry* getRosterEntry()  {return NULL;}

signals:
    void propertyChange(PropertyChangeEvent*);
public slots:
    
};

#endif // THROTTLE_H
