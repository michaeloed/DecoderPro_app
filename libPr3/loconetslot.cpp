#include "loconetslot.h"
#include "lnconstants.h"
#include "Throttle/throttlewindow.h"
#include "loggerfactory.h"

LocoNetSlot::LocoNetSlot(int slotNum, QObject *parent) :
    QObject(parent)
{
 common(slotNum);
}

/**
  * Creates a slot object based on the contents of a LocoNet message.
  * The slot number is assumed to be found in byte 2 of the message
  *
  * @param l  a LocoNet message
  * @throws LocoNetException if the slot does not have an easily-found
  * slot number
  */
 /*public*/ LocoNetSlot::LocoNetSlot(LocoNetMessage* l, QObject *parent) throw (LocoNetException) : QObject(parent){
 common(0);
     // TODO: Consider limiting the types of LocoNet message which can be
     // used to construct the object to only LocoNet slot write or slot
     // report messages, since a LocoNetSlot object constructed from a LocoNet
     // "speed" message or "dir/func" message does not give any other useful
     // information for object initialization.
     slot = l->getElement(2);
     setSlot(l);
}

void LocoNetSlot::common(int slotNum)
{
  slot = slotNum;
  localF9 = false;
  localF10 = false;
  localF11 = false;
  localF12 = false;
  localF13 = false;
  localF14 = false;
  localF15 = false;
  localF16 = false;
  localF17 = false;
  localF18 = false;
  localF19 = false;
  localF20 = false;
  localF21 = false;
  localF22 = false;
  localF23 = false;
  localF24 = false;
  localF25 = false;
  localF26 = false;
  localF27 = false;
  localF28 = false;
  stat = 0;	// <STAT> is the status of the slot
  addr = 0;	// full address of the loco, made from
  //    <ADDR> is the low 7 (0-6) bits of the Loco address
  //    <ADD2> is the high 7 bits (7-13) of the 14-bit loco address
  spd = 0;	// <SPD> is the current speed (0-127)
  _dirf = 0;	// <DIRF> is the current Direction and the setting for functions F0-F4
  trk = 7;	// <TRK> is the global track status
  _ss2 = 0;	// <SS2> is the an additional slot status
  _snd = 0; 	// <SND> is the settings for functions F5-F8
  _id = 0;  // throttle id, made from
  //     <ID1> and <ID2> normally identify the throttle controlling the loco

}

/**
 * Get decoder mode.
 *
 * The decoder (operating) mode is taken from those bits in the slot's STAT
 * byte which reflect the "speed steps" and "consisting" mode.  Note that
 * the other bits from the STAT byte are not visible via this method.
 * this
 * <p>
 * For slot numbers not normally associated with mobile decoders, these bits
 * may have other meanings.
 * <p>
 * Possible values are
 * {@link LnConstants#DEC_MODE_128A},
 * {@link LnConstants#DEC_MODE_28A},
 * {@link LnConstants#DEC_MODE_128},
 * {@link LnConstants#DEC_MODE_14},
 * {@link LnConstants#DEC_MODE_28TRI},
 * {@link LnConstants#DEC_MODE_28}
 *
 * @return the encoded decoder operating mode.
 */
/*public*/ int LocoNetSlot::decoderType() {
    return stat & LnConstants::DEC_MODE_MASK;
}

/**
 * Get slot status.
 * <p>
 * These bits are set based on the STAT byte as seen in LocoNet slot write and
 * slot read messages.  These bits determine whether the command station is
 * actively "refreshing" the loco's speed and direction information on the
 * DCC track signal, and whether the slot is able to be re-assigned for use
 * by another locomotive.
 * <p>
 * For slot numbers not normally associated with mobile decoders, these bits
 * may have other meanings.
 * <p>
 * This returns only those bits of the slot's STAT byte which are related to
 * the slot's "status".
 * <p>
 * Possible values are
 * {@link LnConstants#LOCO_IN_USE},
 * {@link LnConstants#LOCO_IDLE},
 * {@link LnConstants#LOCO_COMMON},
 * {@link LnConstants#LOCO_FREE}
 * @return the slot status bits associated with the slot
 */
/*public*/ int LocoNetSlot::slotStatus() {
    return stat & LnConstants::LOCOSTAT_MASK;
}

/**
 * Get secondary slot status.
 * <p>
 * These bits are set based on the STAT2 byte as seen in LocoNet slot write and
 * slot read messages.  These bits determine how the command station interprets
 * the "address" field of the slot.
 * <p>
 * For slot numbers not normally associated with mobile decoders, these bits
 * may have other meanings.
 * <p>
 * This returns only those bits of the slot's STAT2 byte which are related to
 * the slot's "secondary status".
 *
 * @return the slot secondary status bits associated with the slot
 */

/*public*/ int LocoNetSlot::ss2() {
    return _ss2;
}

/**
 * Get consist status.
 * <p>
 * This returns only those bits of the slot's STAT byte which are related to
 * the slot's "consisting status".
 * <p>
 * For slot numbers not normally associated with mobile decoders, these bits
 * may have other meanings.
 * <p>
 * Possible values are
 * {@link LnConstants#CONSIST_NO},
 * {@link LnConstants#CONSIST_TOP},
 * {@link LnConstants#CONSIST_MID},
 * {@link LnConstants#CONSIST_SUB}
 * @return the slot "consist status", with unrelated bits zeroed
 */
/*public*/ int LocoNetSlot::consistStatus() {
    return stat & LnConstants::CONSIST_MASK;
}

// direction and functions
/**
 * Returns the direction of loco movement which applies when the slot's speed
 * is set for movement.
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if slot is set for forward movement, else false
 */
/*public*/ bool LocoNetSlot::isForward() {
    return 0 == (_dirf & LnConstants::DIRF_DIR);
}

/**
 * Returns the slot's F0 state
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F0 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF0() {
    // TODO: Consider throwing an exception (here and in similar methods)
    // if the slot is one of the "special" slots where the slot is not
    // storing mobile decoder funciton state in the associated bit.
    return 0 != (_dirf & LnConstants::DIRF_F0);
}

/**
 * Returns the slot's F1 state
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F1 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF1() {
    return 0 != (_dirf & LnConstants::DIRF_F1);
}

/**
 * Returns the slot's F2 state
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F2 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF2() {
    return 0 != (_dirf & LnConstants::DIRF_F2);
}

/**
 * Returns the slot's F3 state
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F3 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF3() {
    return 0 != (_dirf & LnConstants::DIRF_F3);
}

/**
 * Returns the slot's F4 state
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F4 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF4() {
    return 0 != (_dirf & LnConstants::DIRF_F4);
}

/**
 * Returns the slot's F5 state
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F5 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF5() {
    return 0 != (_snd & LnConstants::SND_F5);
}

/**
 * Returns the slot's F6 state
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F6 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF6() {
    return 0 != (_snd & LnConstants::SND_F6);
}

/**
 * Returns the slot's F7 state
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F7 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF7() {
    return 0 != (_snd & LnConstants::SND_F7);
}

/**
 * Returns the slot's F8 state
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F8 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF8() {
    return 0 != (_snd & LnConstants::SND_F8);
}

/**
 * Returns the slot's F9 state
 * <p>
 * Some command stations do not actively remember the state of this function.
 * JMRI attempts to track the messages which control this function, but may not
 * reliably do so in some cases.
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F9 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF9() {
    return localF9;
}

/**
 * Returns the slot's F10 state
 * <p>
 * Some command stations do not actively remember the state of this function.
 * JMRI attempts to track the messages which control this function, but may not
 * reliably do so in some cases.
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F10 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF10() {
    return localF10;
}

/**
 * Returns the slot's F11 state
 * <p>
 * Some command stations do not actively remember the state of this function.
 * JMRI attempts to track the messages which control this function, but may not
 * reliably do so in some cases.
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F11 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF11() {
    return localF11;
}

/**
 * Returns the slot's F12 state
 * <p>
 * Some command stations do not actively remember the state of this function.
 * JMRI attempts to track the messages which control this function, but may not
 * reliably do so in some cases.
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F12 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF12() {
    return localF12;
}

/**
 * Returns the slot's F13 state
 * <p>
 * Some command stations do not actively remember the state of this function.
 * JMRI attempts to track the messages which control this function, but may not
 * reliably do so in some cases.
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F13 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF13() {
    return localF13;
}

/**
 * Returns the slot's F14 state
 * <p>
 * Some command stations do not actively remember the state of this function.
 * JMRI attempts to track the messages which control this function, but may not
 * reliably do so in some cases.
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F14 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF14() {
    return localF14;
}

/**
 * Returns the slot's F15 state
 * <p>
 * Some command stations do not actively remember the state of this function.
 * JMRI attempts to track the messages which control this function, but may not
 * reliably do so in some cases.
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F15 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF15() {
    return localF15;
}

/**
 * Returns the slot's F16 state
 * <p>
 * Some command stations do not actively remember the state of this function.
 * JMRI attempts to track the messages which control this function, but may not
 * reliably do so in some cases.
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F16 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF16() {
    return localF16;
}

/**
 * Returns the slot's F17 state
 * <p>
 * Some command stations do not actively remember the state of this function.
 * JMRI attempts to track the messages which control this function, but may not
 * reliably do so in some cases.
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F17 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF17() {
    return localF17;
}

/**
 * Returns the slot's F1 state
 * <p>
 * Some command stations do not actively remember the state of this function.
 * JMRI attempts to track the messages which control this function, but may not
 * reliably do so in some cases.
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F1 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF18() {
    return localF18;
}

/**
 * Returns the slot's F19 state
 * <p>
 * Some command stations do not actively remember the state of this function.
 * JMRI attempts to track the messages which control this function, but may not
 * reliably do so in some cases.
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F19 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF19() {
    return localF19;
}

/**
 * Returns the slot's F20 state
 * <p>
 * Some command stations do not actively remember the state of this function.
 * JMRI attempts to track the messages which control this function, but may not
 * reliably do so in some cases.
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F20 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF20() {
    return localF20;
}

/**
 * Returns the slot's F21 state
 * <p>
 * Some command stations do not actively remember the state of this function.
 * JMRI attempts to track the messages which control this function, but may not
 * reliably do so in some cases.
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F21 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF21() {
    return localF21;
}

/**
 * Returns the slot's F22 state
 * <p>
 * Some command stations do not actively remember the state of this function.
 * JMRI attempts to track the messages which control this function, but may not
 * reliably do so in some cases.
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F22 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF22() {
    return localF22;
}

/**
 * Returns the slot's F23 state
 * <p>
 * Some command stations do not actively remember the state of this function.
 * JMRI attempts to track the messages which control this function, but may not
 * reliably do so in some cases.
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F23 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF23() {
    return localF23;
}

/**
 * Returns the slot's F24 state
 * <p>
 * Some command stations do not actively remember the state of this function.
 * JMRI attempts to track the messages which control this function, but may not
 * reliably do so in some cases.
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F24 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF24() {
    return localF24;
}

/**
 * Returns the slot's F25 state
 * <p>
 * Some command stations do not actively remember the state of this function.
 * JMRI attempts to track the messages which control this function, but may not
 * reliably do so in some cases.
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F25 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF25() {
    return localF25;
}

/**
 * Returns the slot's F26 state
 * <p>
 * Some command stations do not actively remember the state of this function.
 * JMRI attempts to track the messages which control this function, but may not
 * reliably do so in some cases.
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F26 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF26() {
    return localF26;
}

/**
 * Returns the slot's F27 state
 * <p>
 * Some command stations do not actively remember the state of this function.
 * JMRI attempts to track the messages which control this function, but may not
 * reliably do so in some cases.
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F27 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF27() {
    return localF27;
}

/**
 * Returns the slot's F28 state
 * <p>
 * Some command stations do not actively remember the state of this function.
 * JMRI attempts to track the messages which control this function, but may not
 * reliably do so in some cases.
 * <p>
 * For slot numbers not normally associated with mobile decoders, this bit
 * may have other meanings.
 *
 * @return true if F28 is "on", else false
 */
/*public*/ bool LocoNetSlot::isF28() {
    return localF28;
}

// loco address, speed
/**
 * Returns the mobile decoder address associated with the slot.
 * <p>
 * Note that the returned address can encode a "short" address, a "long"
 * address or an "alias".
 * <p>
 * For slot numbers not normally associated with mobile decoders, these bits
 * may have other meanings.
 *
 * @return the mobile decoder address
 */
/*public*/ int LocoNetSlot::locoAddr() {
    return addr;
}

/**
 * Returns the mobile decoder speed associated with the slot
 * <p>
 * If this slot object is consisted to another slot and is not the "top" of
 * the consist, then the return value is the slot number to which this slot
 * is consisted.
 * <p>
 * For slot numbers not normally associated with mobile decoders, these bits
 * may have other meanings.
 *
 * @return the current speed step associated with the slot.
 */
/*public*/ int LocoNetSlot::speed() {
    return spd;
}

/**
 * Returns the mobile decoder direction and F0-F4 bits, as used in the DIRF bits
 * of various LocoNet messages.
 * <p>
 * If this slot object is consisted to another slot and is not the "top" of
 * the consist, then the "direction" bit reflects the relative direction of this
 * loco with respect to the loco it is consisted to, where "Reverse" means it
 * travels in the "reverse" direction with respect to the loco to which it is
 * consisted.
 * <p>
 * For slot numbers not normally associated with mobile decoders, these bits
 * may have other meanings.
 *
 * @return the &lt;DIRF&gt; byte value
 */
/*public*/ int LocoNetSlot::dirf() {
    return _dirf;
}

/**
 * Returns the mobile decoder F5-F8 bits, as used in the SND bits
 * of various LocoNet messages.
 * <p>
 * For slot numbers not normally associated with mobile decoders, these bits
 * may have other meanings.
 *
 * @return the &lt;SND&gt; byte value
 */
/*public*/ int LocoNetSlot::snd() {
    return _snd;
}

/**
 * Returns the "Throttle ID" associated with the slot.
 * <p>
 * The returned value is a 14-bit integer comprised of ID1 as the least-significant
 * bits and ID2 as the most-significant bits.
 * <p>
 * For slot numbers not normally associated with mobile decoders, these bits
 * may have other meanings.
 *
 * @return an integer representing the throttle ID number
 */
/*public*/ int LocoNetSlot::id() {
    return _id;
}

// programmer track special case accessors
/**
 * Returns the programmer command associated with the slot.
 * <p>
 * The returned value is taken from the &lt;PCMD&gt; byte of programmer slot read
 * and write LocoNet messages.
 * <p>
 * For slot numbers other than the programmer slot, these bits
 * may have other meanings.
 *
 * @return the &lt;PCMD&gt; byte
 */
/*public*/ int LocoNetSlot::pcmd() {
    return _pcmd;
}

/*public*/ int LocoNetSlot::cvval() {
    return _snd + (_ss2 & 2) * 64;
}

// methods to interact with LocoNet
//@SuppressWarnings("fallthrough")
//@edu.umd.cs.findbugs.annotations.SuppressWarnings(value="SF_SWITCH_FALLTHROUGH")
void LocoNetSlot::setSlot(LocoNetMessage* l) throw (LocoNetException)
{ // exception if message can't be parsed
    // sort out valid messages, handle
    switch (l->getOpCode()) {
    case LnConstants::OPC_SL_RD_DATA:
      lastUpdateTime = QDateTime::currentDateTime().currentMSecsSinceEpoch();
      //fall through
    case LnConstants::OPC_WR_SL_DATA: {
        if ( l->getElement(1) != 0x0E ) return;  // not an appropriate reply
        // valid, so fill contents
        if (slot != l->getElement(2))
            log->error(QString("Asked to handle message not for this slot (%1) ").arg(slot) + l->toString());
        stat = l->getElement(3);
        _pcmd = l->getElement(4);
        addr = l->getElement(4)+128*l->getElement(9);
        spd =  l->getElement(5);
        _dirf = l->getElement(6);
        trk =  l->getElement(7);
        _ss2 =  l->getElement(8);
        // item 9 is in add2
        _snd =  l->getElement(10);
        _id =   l->getElement(11)+128*l->getElement(12);

        notifySlotListeners();
        return;
    }
    case LnConstants::OPC_SLOT_STAT1:
        if (slot != l->getElement(1))
            log->error(QString("Asked to handle message not for this slot %1 ").arg(l->getElement(1)) +l->toString());
        stat = l->getElement(2);
        notifySlotListeners();
        lastUpdateTime = QDateTime::currentDateTime().currentMSecsSinceEpoch() ;
        return;
    case LnConstants::OPC_LOCO_SND: {
        // set sound functions in slot - first, clear bits
        _snd &= ~(LnConstants::SND_F5 | LnConstants::SND_F6
                 | LnConstants::SND_F7 | LnConstants::SND_F8);
        // and set them as masked
        _snd |= ((LnConstants::SND_F5 | LnConstants::SND_F6
                 | LnConstants::SND_F7 | LnConstants::SND_F8) & l->getElement(2));
        notifySlotListeners();
        lastUpdateTime = QDateTime::currentDateTime().currentMSecsSinceEpoch() ;
        return;
    }
    case  LnConstants::OPC_LOCO_DIRF: {
     // When slot is consist-mid or consist-sub, this LocoNet Opcode
        // can only change the functions; direction cannot be changed.
        if (((stat & LnConstants::CONSIST_MASK) == LnConstants::CONSIST_MID) ||
                ((stat & LnConstants::CONSIST_MASK) == LnConstants::CONSIST_SUB)) {
            // set functions in slot - first, clear bits, preserving DIRF_DIR bit
            _dirf &= LnConstants::DIRF_DIR | (~(LnConstants::DIRF_F0
                    | LnConstants::DIRF_F1 | LnConstants::DIRF_F2
                    | LnConstants::DIRF_F3 | LnConstants::DIRF_F4));
            // and set the function bits from the LocoNet message
            _dirf += ((LnConstants::DIRF_F0
                    | LnConstants::DIRF_F1 | LnConstants::DIRF_F2
                    | LnConstants::DIRF_F3 | LnConstants::DIRF_F4) & l->getElement(2));
        } else {
            // set direction, functions in slot - first, clear bits
            _dirf &= ~(LnConstants::DIRF_DIR | LnConstants::DIRF_F0
                    | LnConstants::DIRF_F1 | LnConstants::DIRF_F2
                    | LnConstants::DIRF_F3 | LnConstants::DIRF_F4);
            // and set them as masked
            _dirf += ((LnConstants::DIRF_DIR | LnConstants::DIRF_F0
                    | LnConstants::DIRF_F1 | LnConstants::DIRF_F2
                    | LnConstants::DIRF_F3 | LnConstants::DIRF_F4) & l->getElement(2));

        }
        notifySlotListeners();
        lastUpdateTime = QDateTime::currentMSecsSinceEpoch();//System.currentTimeMillis();
        return;
    }
    case LnConstants::OPC_MOVE_SLOTS: {
     int toSlot = l->getElement(2);
     if ( toSlot == 0 ) {
         //dispatched implies common
         stat = (stat & ~LnConstants::LOCOSTAT_MASK) | LnConstants::LOCO_COMMON;
     }
     // change in slot status will be reported by the reply,
     // so don't need to do anything here (but could)
     lastUpdateTime = QDateTime::currentMSecsSinceEpoch();//System.currentTimeMillis();
     notifySlotListeners();
     return;
    }
    case LnConstants::OPC_LOCO_SPD: {
     // This opcode has no effect on the slot's speed setting if the
     // slot is mid-consist or sub-consist.
     if (((stat & LnConstants::CONSIST_MASK) != LnConstants::CONSIST_MID) &&
             ((stat & LnConstants::CONSIST_MASK) != LnConstants::CONSIST_SUB)) {

         spd = l->getElement(2);
         notifySlotListeners();
         lastUpdateTime = QDateTime::currentMSecsSinceEpoch();//System.currentTimeMillis();
     } else {
         log->info(tr("Ignoring speed change for slot %1 marked as consist-mid or consist-sub.").arg(slot));
     }
     return;
    }
    default:
    {
     log->debug( tr("message can't be parsed op code 0x%1").arg(l->getOpCode(),0,16 ));
     throw LocoNetException("message can't be parsed");
    }
 }
}

/**
 * Load functions 9 through 28 from loconet "Set Direct"
 * message.
 */
void LocoNetSlot::functionMessage(long pkt)
{
    // parse for which set of functions
    if ( (pkt&0xFFFFFF0) == 0xA0) {
        // F9-12
        localF9  = ((pkt&0x01) != 0);
        localF10 = ((pkt&0x02) != 0);
        localF11 = ((pkt&0x04) != 0);
        localF12 = ((pkt&0x08) != 0);
        notifySlotListeners();
    } else if ( (pkt&0xFFFFFF00) == 0xDE00) {
        // check F13-20
        localF13 = ((pkt&0x01) != 0);
        localF14 = ((pkt&0x02) != 0);
        localF15 = ((pkt&0x04) != 0);
        localF16 = ((pkt&0x08) != 0);
        localF17 = ((pkt&0x10) != 0);
        localF18 = ((pkt&0x20) != 0);
        localF19 = ((pkt&0x40) != 0);
        localF20 = ((pkt&0x80) != 0);
        notifySlotListeners();
    } else if ( (pkt&0xFFFFFF00) == 0xDF00) {
        // check F21-28
        localF21 = ((pkt&0x01) != 0);
        localF22 = ((pkt&0x02) != 0);
        localF23 = ((pkt&0x04) != 0);
        localF24 = ((pkt&0x08) != 0);
        localF25 = ((pkt&0x10) != 0);
        localF26 = ((pkt&0x20) != 0);
        localF27 = ((pkt&0x40) != 0);
        localF28 = ((pkt&0x80) != 0);
        notifySlotListeners();
    }
}

/**
 * Update the decoder type bits in STAT1 (D2, D1, D0)
 * @param status New values for STAT1 (D2, D1, D0)
 * @return Formatted LocoNet message to change value.
 */
LocoNetMessage* LocoNetSlot::writeMode(int status)
{
    LocoNetMessage* l = new LocoNetMessage(4);
    l->setOpCode( LnConstants::OPC_SLOT_STAT1 );
    l->setElement(1, slot);
    l->setElement(2, (stat&~LnConstants::DEC_MODE_MASK)|status);
    return l;
}

/**
 * Sets the object's ID value and returns a LocoNet message to inform the
 * command station that the throttle ID has been changed.
 * @param newID  the new ID number to set into the slot object
 * @return a LocoNet message containing a "Slot Write" message to inform the
 * command station that a specific throttle is controlling the slot.
 */
/*public*/ LocoNetMessage* LocoNetSlot::writeThrottleID(int newID) {
    _id = (newID & 0x17F);
    return writeSlot();
}

/**
 * Update the status mode bits in STAT1 (D5, D4)
 * @param status New values for STAT1 (D5, D4)
 * @return Formatted LocoNet message to change value.
 */
LocoNetMessage*  LocoNetSlot::writeStatus(int status)
{
    LocoNetMessage* l = new LocoNetMessage(4);
    l->setOpCode( LnConstants::OPC_SLOT_STAT1 );
    l->setElement(1, slot);
    l->setElement(2, (stat&~LnConstants::LOCOSTAT_MASK)|status);
    return l;
}

LocoNetMessage* LocoNetSlot::dispatchSlot()
{
    LocoNetMessage* l = new LocoNetMessage(4);
    l->setOpCode( LnConstants::OPC_MOVE_SLOTS );
    l->setElement(1, slot);
    l->setElement(2, 0);
    return l;
}

/**
 * Create a LocoNet OPC_SLOT_STAT1 message which releases this slot to the
 * "Common" state
 *
 * The invoking method must send the returned LocoNet message to LocoNet in
 * order to have a useful effect.
 *
 * Upon receipt of the echo of the transmitted OPC_SLOT_STAT1 message, the
 * LocoNetSlot object will notify its listeners.
 *
 * @return LocoNet message which "releases" the slot to the "Common" state
*/
/*public*/ LocoNetMessage* LocoNetSlot::releaseSlot() {
    return writeStatus(LnConstants::LOCO_COMMON);
}

LocoNetMessage* LocoNetSlot::writeSlot() {
    LocoNetMessage* l = new LocoNetMessage(14);
    l->setOpCode( LnConstants::OPC_WR_SL_DATA );
    l->setElement(1, 0x0E);
    l->setElement(2, slot & 0x7F);
    l->setElement(3, stat & 0x7F);
    l->setElement(4, addr & 0x7F); l->setElement(9, (addr/128)&0x7F);
    l->setElement(5, spd & 0x7F);
    l->setElement(6, _dirf & 0x7F);
    l->setElement(7, trk & 0x7F);
    l->setElement(8, _ss2 & 0x7F);
    // item 9 is add2
    l->setElement(10, _snd & 0x7F);
    l->setElement(11, _id&0x7F); l->setElement(12, (_id/128)&0x7F );
    return l;
}


/*public*/ /*synchronized*/ void LocoNetSlot::addSlotListener(SlotListener* l)
{
    QMutexLocker locker(&mutex);
    // add only if not already registered
    if (!slotListeners.contains(l))
    {
        slotListeners.append(l);
    }
}

/*public*/ /*synchronized*/ void LocoNetSlot::removeSlotListener(SlotListener* l)
{
    QMutexLocker locker(&mutex);
    if (slotListeners.contains(l)) {
        slotListeners.remove(slotListeners.indexOf(l));
    }
}


void LocoNetSlot::notifySlotListeners()
{
 // make a copy of the listener list to synchronized not needed for transmit
 QVector<SlotListener*>* v  = new QVector<SlotListener*>(slotListeners);
//    synchronized(this)
//        {
//            v = new ArrayList<SlotListener>(slotListeners);
//        }
 if (log->isDebugEnabled()) log->debug(QString("notify %1 SlotListeners").arg(v->size()));
 // forward to all listeners
 int cnt = v->size();
 for (int i=0; i < cnt; i++)
 {
  SlotListener* client = v->at(i);
  if(qobject_cast<ThrottleWindow*>(client->self())!= NULL)
   ((ThrottleWindow*)client)->notifyChangedSlot(this);
  else
  if(qobject_cast<LocoNetThrottle*>(client->self())!= NULL)
   ((LocoNetThrottle*)client)->notifyChangedSlot(this);
  else
  {
   client->notifyChangedSlot(this);
   log->debug(tr("Unhandled SlotListener '%1'").arg(client->self()->metaObject()->className()));
  }
 }
}
/**
 * Get the track status byte (location 7)
 * <p>
 * Note that the &lt;TRK&gt; byte is not accurate on some command stations.
 *
 * @return the effective &lt;TRK&gt; byte
 */
/*public*/ int LocoNetSlot::getTrackStatus() { return trk; }

/**
 * Set the track status byte (location 7)
 * <p>
 * Note that setting the LocoNetSlot object's track status may result in a
 * change to the command station's actual track status if the slot's status
 * is communicated to the command station via an OPC_WR_DL_DATA LocoNet message.
 *
 * @param status is the new track status value.
 */
/*public*/ void LocoNetSlot::setTrackStatus(int status) { trk = status; }

/**
 * Only valid for fast-clock slot.
 * @return "Days" value currently in fast-clock slot.
 */
int LocoNetSlot::getFcDays() {
    if (getSlot()!=LnConstants::FC_SLOT)
     log->error("getFcDays invalid for slot "+QString::number(getSlot()));
    return (addr&0x3f80)/0x80;
}
/**
 * For fast-clock slot, set "days" value.
 */
void LocoNetSlot::setFcDays(int val) {
    if (getSlot()!=LnConstants::FC_SLOT)
     log->error("setFcDays invalid for slot "+QString::number(getSlot()));
    addr = val*128+(addr&0x7f);
}

/**
 * Only valid for fast-clock slot.
 * @return "Hours" value currently stored in fast clock slot.
 */
int LocoNetSlot::getFcHours() {
    if (getSlot()!=LnConstants::FC_SLOT)
     log->error("getFcHours invalid for slot "+ QString::number(getSlot()));
    int temp = ((256-_ss2) &0x7F) % 24;
    return (24 - temp) % 24;
}
/**
 * For fast-clock slot, set "hours" value.
 */
void LocoNetSlot::setFcHours(int val) {
    if (getSlot()!=LnConstants::FC_SLOT) log->error("setFcHours invalid for slot "+QString::number(getSlot()));
    _ss2 = (256-(24-val))&0x7F;
}

/**
 * Only valid for fast-clock slot.
 * @return Return minutes value currently stored in the fast clock slot.
 */
int LocoNetSlot::getFcMinutes()
{
    if (getSlot()!=LnConstants::FC_SLOT) log->error("getFcMinutes invalid for slot "+QString::number(getSlot()));
    int temp = ((255-_dirf) & 0x7F) % 60;
    return (60-temp)% 60;
}
/**
 * For fast-clock slot, set "minutes" value.
 */
void LocoNetSlot::setFcMinutes(int val) {
    if (getSlot()!=LnConstants::FC_SLOT) log->error("setFcMinutes invalid for slot "+QString::number(getSlot()));
    _dirf = (255-(60-val))&0x7F;
}

/**
 * Only valid for fast-clock slot.
 * @return Return frac_mins field which is the number of 65ms ticks until then
 * next minute rollover. These ticks step at the current fast clock rate
 */
int LocoNetSlot::getFcFracMins() {
    if (getSlot()!=LnConstants::FC_SLOT) log->error("getFcMinutes invalid for slot "+QString::number(getSlot()));
    return 0x3FFF - ( (addr & 0x7F) | ((spd & 0x7F ) << 7 ) ) ;
}
/**
 * For fast-clock slot, set "frac_mins" value.
 */
void LocoNetSlot::setFcFracMins(int val) {
    if (getSlot()!=LnConstants::FC_SLOT)
     log->error("setFcMinutes invalid for slot "+QString::number(getSlot()));
    int temp = 0x3FFF - val ;
    addr = addr | (temp & 0x7F);
    spd = ( temp >> 7 ) & 0x7F;
}

/**
 * Only valid for fast-clock slot.
 * @return Rate stored in fast clock slot.
 */
int LocoNetSlot::getFcRate() {
    if (getSlot()!=LnConstants::FC_SLOT) log->error("getFcMinutes invalid for slot "+QString::number(getSlot()));
    return stat;
}
/**
 * For fast-clock slot, set "rate" value.
 */
void LocoNetSlot::setFcRate(int val) {
    if (getSlot()!=LnConstants::FC_SLOT) log->error("setFcMinutes invalid for slot "+QString::number(getSlot()));
    stat = val & 0x7F;
}

/*static*/ Logger* LocoNetSlot::log = LoggerFactory::getLogger("LocoNetSlot");
