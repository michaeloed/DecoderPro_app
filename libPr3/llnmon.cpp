#include "llnmon.h"
#include "proxyturnoutmanager.h"
#include "proxysensormanager.h"
#include "instancemanager.h"
#include "stringutil.h"
#include "lnsv2messagecontents.h"

/**
 * A utility class for formatting LocoNet packets into human-readable text.
 * <p>
 * Rather than using this formatter class, use
 * {@link LocoNetMessage#toMonitorString(String)} (preferred) or
 * {@link LocoNetMessage#toMonitorString()}
 * for each individual LocoNet message instead.
 * <p>
 * Much of this file is a Java-recoding of the display.c file from the llnmon
 * package of John Jabour. Some of the conversions involve explicit decoding of
 * structs defined in loconet.h in that same package. Those parts are (C)
 * Copyright 2001 Ron W. Auld. Use of these parts is by direct permission of the
 * author.
 * <p>
 * Most major comment blocks here are quotes from the Digitrax LocoNet(r) OPCODE
 * SUMMARY: found in the LocoNet(r) Personal Edition 1.
 * <p>
 * Some of the message formats used in this class are Copyright Digitrax, Inc.
 * and used with permission as part of the JMRI project. That permission does
 * not extend to uses in other software products. If you wish to use this code,
 * algorithm or these message formats outside of JMRI, please contact Digitrax
 * Inc for separate permission.
 * <p>
 * Note that the formatted strings end in a \n, and may contain more than one
 * line separated by \n. Someday this should be converted to proper Java line
 * handling, but for now it has to be handled in locomon, the sole user of this.
 * (It could be handled by moving the code from locomon into the display member
 * here)
 * <p>
 * Reverse engineering of OPC_MULTI_SENSE was provided by Al Silverstein, used
 * with permission.
 * <p>
 * Reverse engineering of the Duplex Group/Password/Channel management was
 * provided by Leo Bicknell with help from B. Milhaupt, used with permission.
 * <p>
 * Reverse-engineering of device-specific OpSw messages, throttle text message,
 * and throttle semaphore message was provided by B. Milhaupt, used with
 * permission.
 *
 * @author Bob Jacobsen Copyright 2001, 2002, 2003
 * @author B. Milhaupt Copyright 2015, 2016, 2018
 * @author Randall Wood Copyright 2016
 * <p>
 * @deprecated since 4.13.5; use the
 * {@link LocoNetMessage#toMonitorString(String)} (preferred) or
 * {@link LocoNetMessage#toMonitorString()}
 * for each individual LocoNet message instead of creating a formatter.
 */
LlnMon::LlnMon(QObject *parent) :
    QObject(parent)
{
 common("L");
}

void LlnMon::common(QString prefix)
{
 /*
 * Flag that determines if we print loconet opcodes
 */
showOpCode = false;

/**
 * Flag that determines if we show track status on every slot read
 */
showTrackStatus = true;

/**
 * Most recent track status value
 */
trackStatus = -1;
//protected:
/**
 * Global flag to indicate the message was not fully parsed, so the hex
 * should be included.
 */
 forceHex = false;
 turnoutPrefix = prefix+"T";
 sensorPrefix = "S";
 reporterPrefix = prefix+"R";
 turnoutManager = NULL;
}

/**
 * Create a LocoNet Message Formatter. Use the system connection memo to get
 * the correct managers to allow the user names of managed devices to be
 * included in messages with the system names.
 *
 * @param memo the system connection memo
 * <p>
 * @deprecated since 4.13.5; use the
 * {@link LocoNetMessage#toMonitorString(String)} (preferred) or
 * {@link LocoNetMessage#toMonitorString()}
 * for each individual LocoNet message instead of creating a formatter.
 */
//@Deprecated
/*public*/ LlnMon::LlnMon(LocoNetSystemConnectionMemo* memo, QObject *parent) : QObject(parent)
{
    //this(memo.getSystemPrefix()); // set default managers
 common(memo->getSystemPrefix());
}

/**
 * Create a LocoNet Message Formatter. The managers allow the user names of
 * managed devices to be included in messages with the system names.
 * <p>
 * @param turnoutManager  turnout manager
 * @param sensorManager   sensor manager
 * @param reporterManager reporter manager
 * <p>
 * @deprecated since 4.13.5; use the
 * {@link LocoNetMessage#toMonitorString(String)} (preferred) or
 * {@link LocoNetMessage#toMonitorString()}
 * for each individual LocoNet message instead of creating a formatter.
 */
//@Deprecated
/*public*/ LlnMon::LlnMon(/*@NonNULL*/ TurnoutManager* turnoutManager,
        /*@NonNULL*/ SensorManager* sensorManager,
        /*@NonNULL*/ ReporterManager* reporterManager, QObject *parent) : QObject(parent)
{
    common(turnoutManager->getSystemPrefix());
    this->setLocoNetSensorManager(sensorManager); // a hack to set the sensor prefix
    this->setLocoNetReporterManager(reporterManager);  // a hack to set the reporter prefix
}

/**
 * Create a LocoNet Message Formatter. The managers allow the user names of
 * managed devices to be included in messages with the system names.
 * <p>
 * @param prefix - system connection prefix (i.e. "L")
 * <p>
 * @deprecated since 4.13.5; use the
 * {@link LocoNetMessage#toMonitorString(String)} (preferred) or
 * {@link LocoNetMessage#toMonitorString()}
 * for each individual LocoNet message instead of creating a formatter.
 */
//@Deprecated
/*public*/ LlnMon::LlnMon(/*@NonNULL*/ QString prefix, QObject* parent) : QObject(parent) {
//    turnoutPrefix = prefix+"T";
//    sensorPrefix = prefix+"S";
//    reporterPrefix = prefix+"R";
 common(prefix);
}

/**
 * A utility class for formatting LocoNet packets into human-readable text.
 * <P>
 * Much of this file is a Java-recoding of the display.c file from the llnmon
 * package of John Jabour. Some of the conversions involve explicit decoding of
 * structs defined in loconet.h in that same package. Those parts are (C)
 * Copyright 2001 Ron W. Auld. Use of these parts is by direct permission of the
 * author.
 * <P>
 * Most major comment blocks here are quotes from the Digitrax Loconet(r) OPCODE
 * SUMMARY: found in the Loconet(r) Personal Edition 1.
 * <P>
 * Some of the message formats used in this class are Copyright Digitrax, Inc.
 * and used with permission as part of the JMRI project. That permission does
 * not extend to uses in other software products. If you wish to use this code,
 * algorithm or these message formats outside of JMRI, please contact Digitrax
 * Inc for separate permission.
 * <P>
 * Note that the formatted strings end in a \n, and may contain more than one
 * line separated by \n. Someday this should be converted to proper Java line
 * handling, but for now it has to be handled in locomon, the sole user of this.
 * (It could be handled by moving the code from locomon into the display member
 * here)
 * <P>
 * Reverse engineering of OPC_MULTI_SENSE was provided by Al Silverstein, used
 * with permission.
 * <P>
 * Reverse engineering of the Duplex Group/Password/Channel management was
 * provided by Leo Bicknell with help from B. Milhaupt, used with permission.
 * <P>
 * Reverse-engineering of device-specific OpSw messages, throttle text
 * message, and throttle semaphore message was provided by B. Milhaupt, used
 * with permission.
 * <P>
 * @author Bob Jacobsen Copyright 2001, 2002, 2003
 * @version $Revision: 22622 $
 */
//public class Llnmon {


/**
 * Convert bytes from LocoNet packet into a locomotive address.
 *
 * @param a1
 *            Byte containing the upper bits.
 * @param a2
 *            Byte containting the lower bits.
 * @return 1-4096 address
 */
int LlnMon::LOCO_ADR(int a1, int a2)
{
 return (((a1 & 0x7f) * 128) + (a2 & 0x7f));
} // end of static private int LOCO_ADR(int a1, int a2)

/**
 * Convert bytes from LocoNet packet into a 1-based address for a sensor or
 * turnout.
 *
 * @param a1
 *            Byte containing the upper bits
 * @param a2
 *            Byte containing the lower bits
 * @return 1-4096 address
 */
int LlnMon::SENSOR_ADR(int a1, int a2)
{
 return (((a2 & 0x0f) * 128) + (a1 & 0x7f)) + 1;
} // end of static private int SENSOR_ADR(int a1, int a2)

/*
 * Take an int and convert it to a dotted version number
 * as used by the LocoIO protocol
 * Example:  123 => 1.2.3
 */

/**
 * Take the LocoIO version number and convert to human friendly format.
 *
 * @param val
 *            The LocoIO version.
 * @return String with human readable format.
 */
QString LlnMon::dotme(int val)
{
 if ((val >= 0) && (val < 10)) {
             return tr("%1").arg(val);
         } else if ((val >= 10) && (val < 100)) {
             return tr("%1.%2").arg(val / 10).arg(val % 10);
         } else if ((val >= 100) && (val < 1000)) {
             int hundreds = val / 100;
             int tens = (val - (hundreds * 100)) / 10;
             int ones = val % 10;
             return tr("%1.%2.%3").arg(hundreds).arg(tens).arg(ones);
         }
         return tr("(un-decipherable value %1 is outside of expected range)").arg(val);
     }

/**
 * Convert throttle ID to a human friendly format.
 *
 * @param id1
 *            Byte #1 of the ID.
 * @param id2
 *            Byte #2 of the ID.
 * @return String with human friendly format.
 */
QString LlnMon::idString(int id1, int id2)
{
 /* the decimalIdValueWithoutLocale_SpecificFormatting variable
    is used to generate a string representation of the ID value
    without any local-specific formatting.  In other words, in a
    us_EN locale, we want "14385", not "14,385".
     */
    QString decimalIdValueWithoutLocale_SpecificFormatting
            = QString::number(((id2 & 0x7F) * 128 + (id1 & 0x7F)));

    QString s = tr("%1 %2 (%3)").arg(
            tr("0x%1").arg(
                    StringUtil::twoHexFromInt(id2 & 0x7F)),
            tr("0x%1").arg(
                    StringUtil::twoHexFromInt(id1 & 0x7F)),
            decimalIdValueWithoutLocale_SpecificFormatting);
    return s;
}

/**
 * This function creates a string representation of the loco address in
 * addressLow & addressHigh in a form appropriate for the type of address (2
 * or 4 digit) using the Digitrax 'mixed mode' if necessary.
 *
 * @param addressLow
 * @param addressHigh
 * @return
 */
QString LlnMon::convertToMixed(int addressLow, int addressHigh)
{
 // if we have a 2 digit decoder address and proceed accordingly
 switch (addressHigh)
 {
     case 0x7d:
         log->debug(tr("addressLow / 10 = %1").arg(addressLow / 10));
         switch (addressLow) {
             case 100: case 101: case 102: case 103: case 104: case 105:
             case 106: case 107: case 108: case 109:
                 // N (short, alternately 'An') (or long address NN)
                 return tr("%1 (short, or \"A%2\") (or long address %3)").arg(
                         addressLow).arg(
                         addressLow-100).arg(
                         LOCO_ADR(addressHigh, addressLow));
                         // Note: .toString intentionally used here to remove the "internationalized"
                         // presentation of integers, which, in US English, adds a "," between
                         // the thousands digit and the hundreds digit.  This comma is undesired
                         // in this application.
             case 110: case 111: case 112: case 113: case 114: case 115:
             case 116: case 117: case 118: case 119:
                 // N (short, alternately 'Bn') (or long address NN)
                 return tr("%1 (short, or \"B%2\") (or long address %3)").arg(
                         addressLow).arg(
                         addressLow-110).arg(
                         LOCO_ADR(addressHigh, addressLow));
                         // Note: .toString intentionally used here to remove the "internationalized"
                         // presentation of integers, which, in US English, adds a "," between
                         // the thousands digit and the hundreds digit.  This comma is undesired
                         // in this application.
             case 120: case 121: case 122: case 123: case 124: case 125:
             case 126: case 127:
                 // N (short, alternately 'Cn') (or long address NN)
                 return tr("%1 (short, or \"C%2\") (or long address %3)").arg(
                         addressLow).arg(
                         addressLow-120).arg(
                         LOCO_ADR(addressHigh, addressLow));
                         // Note: .toString intentionally used here to remove the "internationalized"
                         // presentation of integers, which, in US English, adds a "," between
                         // the thousands digit and the hundreds digit.  This comma is undesired
                         // in this application.
             default:
                 // N (short) (or long address NN)
                 return tr("%1 (short) (or long address %2)").arg(
                         addressLow).arg(
                         LOCO_ADR(addressHigh, addressLow));
                         // Note: .toString intentionally used here to remove the "internationalized"
                         // presentation of integers, which, in US English, adds a "," between
                         // the thousands digit and the hundreds digit.  This comma is undesired
                         // in this application.
         }

     case 0x00:
     case 0x7f:
         switch (addressLow) {
             case 100: case 101: case 102: case 103: case 104: case 105:
             case 106: case 107: case 108: case 109:
                 // N (short, alternately 'An')
                 return tr("%1 (short, or \"A{%2\")").arg(
                         addressLow).arg(
                         addressLow-100);
             case 110: case 111: case 112: case 113: case 114: case 115:
             case 116: case 117: case 118: case 119:
                 // N (short, alternately 'Bn')
                 return tr("%1 (short, or \"B%2\")").arg(
                         addressLow).arg(
                         addressLow-110);
             case 120: case 121: case 122: case 123: case 124: case 125:
             case 126: case 127:
                 // N (short, alternately 'Cn')
                 return tr("%1 (short, or \"C%2\")").arg(
                         addressLow).arg(
                         addressLow-120);
             default:
                 // N (short)
                 return tr("%1 (short)").arg(
                         addressLow);
         }
     default:
         // return the full 4 digit address
         return QString::number(LOCO_ADR(addressHigh, addressLow));
         // Note: .toString intentionally used here to remove the "internationalized"
         // presentation of integers, which, in US English, adds a "," between
         // the thousands digit and the hundreds digit.  This comma is undesired
         // in this application.
 }
} // end of private static String convertToMixed(int addressLow, int addressHigh)


/*private*/ QString LlnMon::trackStatusByteToString(int trackStatusByte) {
        return tr("Master supports %1; Track Status: %2/{%3; Programming Track Status: %4").arg(
                (((trackStatusByte & LnConstants::GTRK_MLOK1) != 0)
                        ? tr("LocoNet 1.1")
                        : tr("DT200"))).arg(
                (((trackStatusByte & LnConstants::GTRK_POWER) != 0)
                        ? tr("On")
                        : tr("Off"))).arg(
                (((trackStatusByte & LnConstants::GTRK_IDLE) != 0)
                        ? tr("Running")
                        : tr("Paused"))).arg(
                (((trackStatusByte & LnConstants::GTRK_PROG_BUSY) != 0)
                        ? tr("Busy")
                        : tr("Available"))
        );
    }

    /**
     * Return a string which is formatted by a bundle Resource Name.
     *
     * @param hour   - fast-clock hour
     * @param minute - fast-clock minute
     * @return a formatted string containing the time
     */
    /*private*/ QString LlnMon::fcTimeToString(int hour, int minute) {
//        return tr("%1").arg(
//                LocalTime.of(hour, minute).toString());
           return QString("%1:%2").arg(hour).arg(minute);
    }

    /*protected*/ QVector<QString> LlnMon::interpretF0_F4toStrings(int dirf) {
        QVector<QString> s = QVector<QString>(5);

        s[0] = (((dirf & LnConstants::DIRF_F0) == LnConstants::DIRF_F0)
                ? tr("On")
                : tr("Off"));
        s[1] = (((dirf & LnConstants::DIRF_F1) == LnConstants::DIRF_F1)
                ? tr("On")
                : tr("Off"));
        s[2] = (((dirf & LnConstants::DIRF_F2) == LnConstants::DIRF_F2)
                ? tr("On")
                : tr("Off"));
        s[3] = (((dirf & LnConstants::DIRF_F3) == LnConstants::DIRF_F3)
                ? tr("On")
                : tr("Off"));
        s[4] = (((dirf & LnConstants::DIRF_F4) == LnConstants::DIRF_F4)
                ? tr("On")
                : tr("Off"));
        return s;
    }

    /*protected*/ QString LlnMon::directionOfTravelString(bool isForward) {
        return tr(isForward ? "Forward"
                : "Reverse");
    }

    /*protected*/ QVector<QString> LlnMon::interpretF5_F8toStrings(int snd) {
        QVector<QString> s =  QVector<QString>(4);

        s.replace(0, (((snd & LnConstants::SND_F5) == LnConstants::SND_F5)
                ? tr("On")
                : tr("Off")));

        s.replace(1, (((snd & LnConstants::SND_F6) == LnConstants::SND_F6)
                ? tr("On")
                : tr("Off")));

        s.replace(2, (((snd & LnConstants::SND_F7) == LnConstants::SND_F7)
                ? tr("On")
                : tr("Off")));

        s.replace(3, (((snd & LnConstants::SND_F8) == LnConstants::SND_F8)
                ? tr("On")
                : tr("Off")));

        return s;
    }

    /*private*/ QString LlnMon::figureAddressIncludingAliasing(int adr, int adr2, int ss2, int id1, int id2) {

        /*
         * Build loco address string. String will be a simple
         * number, unless the address is between 100 and 127
         * (inclusive), where a Digitrax "mixed mode" version
         * of the address will be appended.
         */
        QString mixedAdrStr = convertToMixed(adr, adr2);

        /*
         * If the address is a command station "alias" condition,
         * then note it in the string.
         */
        if (adr2 == 0x7f) {
            if ((ss2 & LnConstants::STAT2_ALIAS_MASK) == LnConstants::STAT2_ID_IS_ALIAS) {
                /* this is an aliased address and we have the alias */
                return tr("%1 (Alias for loco %2)").arg(
                        QString::number(LOCO_ADR(id2, id1))).arg( mixedAdrStr);
            } else {
                /* this is an aliased address and we don't have the alias */
                return tr("%1 (via an Alias)").arg(
                        mixedAdrStr);
            }
        } else {
            /* a regular address which is not an alias */
            return mixedAdrStr;
        }
    }

    /*private*/ QString LlnMon::getAlmTaskType(int taskTypeByte) {
        if (taskTypeByte == 2) {
            return tr("(RD)");
        } else if (taskTypeByte == 3) {
            return tr("WR");
        } else if (taskTypeByte == 0) {
            return tr("ID");
        } else {
            return tr("(Unknown task type %1)").arg(
                    taskTypeByte);
        }
    }

    /*public*/ /*static*/ QString LlnMon::getDeviceNameFromIPLInfo(int manuf, int type) {
        if (manuf != LnConstants::RE_IPL_MFR_DIGITRAX) {
            return tr("Unknown device (Manufacturer code %1, product code %2)").arg(
                    manuf).arg( type);
        }
        switch (type) {
            case LnConstants::RE_IPL_DIGITRAX_HOST_ALL:
                return tr("Digitrax (no host device type specified)");
            case LnConstants::RE_IPL_DIGITRAX_HOST_LNRP:
                return tr("Digitrax LNRP host");
            case LnConstants::RE_IPL_DIGITRAX_HOST_UT4:
                return tr("Digitrax UT4(x) host");
            case LnConstants::RE_IPL_DIGITRAX_HOST_WTL12:
                return tr("Walthers (Digitrax) WTL12 host");
            case LnConstants::RE_IPL_DIGITRAX_HOST_DCS210:
                return tr("Digitrax DCS210 host");
            case LnConstants::RE_IPL_DIGITRAX_HOST_DCS240:
                return tr("Digitrax DCS240 host");
            case LnConstants::RE_IPL_DIGITRAX_HOST_PR3:
                return tr("Digitrax PR3 host");
            case LnConstants::RE_IPL_DIGITRAX_HOST_DT402:
                return tr("Digitrax DT402(x) host");
            case LnConstants::RE_IPL_DIGITRAX_HOST_DT500:
                return tr("Digitrax DT500(x) host");
            case LnConstants::RE_IPL_DIGITRAX_HOST_DCS51:
                return tr("Digitrax DCS51 host");
            case LnConstants::RE_IPL_DIGITRAX_HOST_UR92:
                return tr("Digitrax UR92 host");
            case LnConstants::RE_IPL_DIGITRAX_HOST_PR4:
                return tr("Digitrax PR4 host");
            case LnConstants::RE_IPL_DIGITRAX_HOST_LNWI:
                return tr("Digitrax LNWI host");
            case LnConstants::RE_IPL_DIGITRAX_HOST_BXP88:
                return tr("Digitrax BXP88 host");
            case LnConstants::RE_IPL_DIGITRAX_HOST_DB210:
                return tr("Digitrax DB210 host");
            case LnConstants::RE_IPL_DIGITRAX_HOST_DB210OPTO:
                return tr("Digitrax DB210 Opto host");
            case LnConstants::RE_IPL_DIGITRAX_HOST_DB220:
                return tr("Digitrax DB220 host");

            default:
                return tr("Digitrax (unknown host device type %1)").arg( type);
        }
    }

    /*public*/ /*static*/ QString LlnMon::getSlaveNameFromIPLInfo(int manuf, int slaveNum) {
        if (manuf != LnConstants::RE_IPL_MFR_DIGITRAX) {
            return tr("Unknown device (Manufacturer code %1, product code %2)").arg(
                    manuf).arg(slaveNum);
        }
        switch (slaveNum) {
            case LnConstants::RE_IPL_DIGITRAX_SLAVE_ALL:
                return tr("Digitrax (no slave device type specified)");
            case LnConstants::RE_IPL_DIGITRAX_SLAVE_RF24:
                return tr("Digitrax RF24 slave");
            default:
                return tr("Digitrax (unknown slave device type %1)").arg( slaveNum);
        }
    }

/**
 * Format the message into a text string.  If forceHex is set
 * upon return, the message was not fully parsed.
 * @param l Message to parse
 * @return String representation
 */
QString LlnMon::format(LocoNetMessage l)
{
 QString result;
// bool showStatus = false; /* show track status in this message? */

// int minutes; // temporary time values
// int hours;
// int frac_mins;
 //qDebug()<< QString("opcode 0x%1").arg(l.getOpCode() & 0xff,0,16);
 /*
  * 2 Byte MESSAGE OPCODES
  * ; FORMAT = <OPC>,<CKSUM>
  * ;
  *
  * 4 byte MESSAGE OPCODES
  * ; FORMAT = <OPC>,<ARG1>,<ARG2>,<CKSUM>
  * :
  *  CODES 0xA8 to 0xAF have responses
  *  CODES 0xB8 to 0xBF have responses
  *
  * 6 byte MESSAGE OPCODES
  * ; FORMAT = <OPC>,<ARG1>,<ARG2>,<ARG3>,<ARG4>,<CKSUM>
  * :
  *  CODES 0xC8 to 0xCF have responses
  *  CODES 0xD8 to 0xDF have responses
  */
 switch (l.getOpCode() & 0xff)
 {
 /*
  * 2 Byte MESSAGE OPCODES * ; FORMAT = <OPC>,<CKSUM> * ; *
  */

  /*
   * OPC_IDLE 0x85 ;FORCE IDLE state, Broadcast emergency STOP
   *
   * Page 8 of LocoNet Personal Edition v1.0.
   */
  case LnConstants::OPC_IDLE:
  {
   return tr("Force Idle, Broadcast Emergency STOP.\n");
  } // case LnConstants::OPC_IDLE

  /*
   * OPC_GPON 0x83 ;GLOBAL power ON request
   *
   * Page 8 of LocoNet Personal Edition v1.0.
   */
  case LnConstants::OPC_GPON:
  {
       return tr("Global Power ON.\n");
  } // case LnConstants::OPC_GPON

  /*
   * OPC_GPOFF 0x82 ;GLOBAL power OFF request
   *
   * Page 8 of LocoNet Personal Edition v1.0.
   */
  case LnConstants::OPC_GPOFF:
  {
      return tr("Global Power OFF.\n");
  } // case LnConstants::OPC_GPOFF

  /*
   * OPC_GPBUSY 0x81 ;MASTER busy code, NULL
   *
   * Page 8 of LocoNet Personal Edition v1.0.
   */
  case LnConstants::OPC_GPBUSY:
  {
      return tr("Master is busy.\n");
  } // case LnConstants::OPC_GPBUSY

 /*
  * ; 4 byte MESSAGE OPCODES
  * ; FORMAT = <OPC>,<ARG1>,<ARG2>,<CKSUM>
  * :
  *  CODES 0xA8 to 0xAF have responses
  *  CODES 0xB8 to 0xBF have responses
  */

  /*
   * OPC_LOCO_ADR     0xBF   ; REQ loco ADR
   *                         ; Follow on message: <E7>SLOT READ
   *                         ; <0xBF>,<0>,<ADR>,<CHK> REQ loco ADR
   *                         ; DATA return <E7>, is SLOT#, DATA that ADR was
   *                         : found in.
   *                         ; IF ADR not found, MASTER puts ADR in FREE slot
   *                         ; and sends DATA/STATUS return <E7>......
   *                         ; IF no FREE slot, Fail LACK,0 is returned
   *                         ; [<B4>,<3F>,<0>,<CHK>]
   *
   * Page 8 of LocoNet Personal Edition v1.0.
   */
  case LnConstants::OPC_LOCO_ADR: {
      return tr("Request slot for loco address ")
             + convertToMixed(l.getElement(2), l.getElement(1)) + ".\n";
  } // case LnConstants::OPC_LOCO_ADR

  /*
   * OPC_SW_ACK       0xBD   ; REQ SWITCH WITH acknowledge function (not DT200)
   *                         ; Follow on message: LACK
   *                         ; <0xBD>,<SW1>,<SW2>,<CHK> REQ SWITCH function
   *                         ;       <SW1> =<0,A6,A5,A4- A3,A2,A1,A0>
   *                         ;               7 ls adr bits.
   *                         ;               A1,A0 select 1 of 4 input pairs
   *                         ;               in a DS54
   *                         ;       <SW2> =<0,0,DIR,ON- A10,A9,A8,A7>
   *                         ;               Control bits and 4 MS adr bits.
   *                         ;               DIR=1 for Closed/GREEN
   *                         ;                  =0 for Thrown/RED
   *                         ;               ON=1 for Output ON
   *                         ;                 =0 FOR output OFF
   *                         ; response is:
   *                         ; <0xB4><3D><00> if DCS100 FIFO is full, rejected.
   *                         ; <0xB4><3D><7F> if DCS100 accepted
   *
   * Page 8 of LocoNet Personal Edition v1.0.
   */
  case LnConstants::OPC_SW_ACK:
  {
   result = interpretOpcSwAck(l);
   if (result.length() > 0)
   {
       return result;
   }
   break;
  }


    /*
     * OPC_SW_STATE     0xBC   ; REQ state of SWITCH
     *                         ; Follow on message: LACK
     *                         ; <0xBC>,<SW1>,<SW2>,<CHK> REQ state of SWITCH
     *
     * Page 8 of LocoNet Personal Edition v1.0.
     */
 case LnConstants::OPC_SW_STATE: {
  result = interpretOpcSwState(l);
     if (result.length() > 0) {
         return result;
     }
     break;
 }
     /*
      * OPC_RQ_SL_DATA   0xBB   ; Request SLOT DATA/status block
      *                         ; Follow on message: <E7>SLOT READ
      *                         ; <0xBB>,<SLOT>,<0>,<CHK> Request SLOT DATA/status block.
      *
      * Page 8 of LocoNet Personal Edition v1.0.
      */
 case LnConstants::OPC_RQ_SL_DATA:
 {
  result = interpretOpcRqSlData(l);
     if (result.length() > 0) {
         return result;
     }
     break;
 }

     /*
      * OPC_MOVE_SLOTS   0xBA   ; MOVE slot SRC to DEST
      *                         ; Follow on message: <E7>SLOT READ
      *                         ; <0xBA>,<SRC>,<DEST>,<CHK> Move SRC to DEST if
      *                         ; SRC or LACK etc is NOT IN_USE, clr SRC
      *                         ; SPECIAL CASES:
      *                         ; If SRC=0 ( DISPATCH GET) , DEST=dont care,
      *                         ;    Return SLOT READ DATA of DISPATCH Slot
      *                         ; IF SRC=DEST (NULL move) then SRC=DEST is set to
      *                         ;    IN_USE , if legal move.
      *                         ; If DEST=0, is DISPATCH Put, mark SLOT as DISPATCH
      *                         ;    RETURN slot status <0xE7> of DESTINATION slot
      *                         ;       DEST if move legal
      *                         ;    RETURN Fail LACK code if illegal move
      *                         ;       <B4>,<3A>,<0>,<chk>, illegal to move to/from
      *                         ;       slots 120/127
      *
      * Page 8 of LocoNet Personal Edition v1.0.
      */
 case LnConstants::OPC_MOVE_SLOTS:
 {
  result = interpretOpcMoveSlots(l);
     if (result.length() > 0) {
         return result;
     }
     break;
 }
     /*
      * OPC_LINK_SLOTS   0xB9   ; LINK slot ARG1 to slot ARG2=
      *                         ; Follow on message: <E7>SLOT READ=
      *                         ; <0xB9>,<SL1>,<SL2>,<CHK> SLAVE slot SL1 to slot SL2
      *                         ; Master LINKER sets the SL_CONUP/DN flags
      *                         ; appropriately. Reply is return of SLOT Status
      *                         ; <0xE7>. Inspect to see result of Link, invalid
      *                         ; Link will return Long Ack Fail <B4>,<39>,<0>,<CHK>
      *
      * Page 9 of LocoNet Personal Edition v1.0.
      */
  case LnConstants::OPC_LINK_SLOTS: {
   int src = l.getElement(1);
      int dest = l.getElement(2);
      return tr("Consist loco in slot %1 to loco in slot %2.").arg(src).arg(dest);
  }

     /*
      * OPC_UNLINK_SLOTS 0xB8   ;UNLINK slot ARG1 from slot ARG2
      *                         ; Follow on message: <E7>SLOT READ
      *                         ; <0xB8>,<SL1>,<SL2>,<CHK> UNLINK slot SL1 from SL2
      *                         ; UNLINKER executes unlink STRATEGY and returns new SLOT#
      *                         ; DATA/STATUS of unlinked LOCO . Inspect data to evaluate UNLINK
      *
      * Page 9 of LocoNet Personal Edition v1.0.
      */
 case LnConstants::OPC_UNLINK_SLOTS: {
     int src = l.getElement(1);
     int dest = l.getElement(2);
     return tr("Remove loco in slot ") + QString("%1").arg(src) + tr(" from consist with loco in slot ") + QString("%1").arg(dest) + ".\n";
 } // case LnConstants::OPC_UNLINK_SLOTS

     /*
      * OPC_CONSIST_FUNC 0xB6   ; SET FUNC bits in a CONSIST uplink element
      *                         ; <0xB6>,<SLOT>,<DIRF>,<CHK> UP consist FUNC bits
      *                         ; NOTE this SLOT adr is considered in UPLINKED slot space.
      *
      * Page 9 of LocoNet Personal Edition v1.0.
      */
  case LnConstants::OPC_CONSIST_FUNC:
  {
      result = interpretOpcConsistFunc(l);
      if (result.length() > 0) {
          return result;
      }
      break;
  }

     /*
      * OPC_SLOT_STAT1   0xB5   ; WRITE slot stat1
      *                         ; <0xB5>,<SLOT>,<STAT1>,<CHK> WRITE stat1
      *
      * Page 9 of LocoNet Personal Edition v1.0.
      */
 case LnConstants::OPC_SLOT_STAT1: {
     int slot = l.getElement(1);
     int stat = l.getElement(2);
     return tr("Write slot ") + QString("%1").arg(slot) + tr(" with status value ") + QString("%1").arg(stat)
            + " (0x"+ QString("%1").arg(stat,0,16) + ") - Loco is "
            + LnConstants::CONSIST_STAT(stat) + ", " + LnConstants::LOCO_STAT(stat)
            + tr("\n\tand operating in ") + LnConstants::DEC_MODE(stat) + tr(" speed step mode.\n");
 } // case LnConstants::OPC_SLOT_STAT1

     /*
      * OPC_LONG_ACK     0xB4   ; Long acknowledge
      *                         ; <0xB4>,<LOPC>,<ACK1>,<CHK> Long acknowledge
      *                         ; <LOPC> is COPY of OPCODE responding to (msb=0).
      *                         ; LOPC=0 (unused OPC) is also VALID fail code
      *                         ; <ACK1> is appropriate response code for the OPCode
      *
      * Page 9 of LocoNet Personal Edition v1.0.
      */
 case LnConstants::OPC_LONG_ACK:
 {
  result = interpretLongAck(l);
      if (result.length() > 0) {
          return result;
      }
      break;
 }
    /*
     * OPC_INPUT_REP    0xB2   ; General SENSOR Input codes
     *                         ; <0xB2>, <IN1>, <IN2>, <CHK>
     *                         ;   <IN1> =<0,A6,A5,A4- A3,A2,A1,A0>,
     *                         ;           7 ls adr bits.
     *                         ;           A1,A0 select 1 of 4 inputs pairs in a DS54.
     *                         ;   <IN2> =<0,X,I,L- A10,A9,A8,A7>,
     *                         ;           Report/status bits and 4 MS adr bits.
     *                         ;           "I"=0 for DS54 "aux" inputs
     *                         ;              =1 for "switch" inputs mapped to 4K SENSOR space.
     *                         ;
     *                         ;           (This is effectively a least significant adr bit when
     *                         ;            using DS54 input configuration)
     *                         ;
     *                         ;           "L"=0 for input SENSOR now 0V (LO),
     *                         ;              =1 for Input sensor >=+6V (HI)
     *                         ;           "X"=1, control bit,
     *                         ;              =0 is RESERVED for future!
     *
     * Page 9 of LocoNet Personal Edition v1.0.
     */
 case LnConstants::OPC_INPUT_REP:
 {
  result = interpretOpcInputRep(l);
     if (result.length() > 0) {
         return result;
     }
     break;
 } // case LnConstants::OPC_INPUT_REP

     /*
      * OPC_SW_REP       0xB1   ; Turnout SENSOR state REPORT
      *                         ; <0xB1>,<SN1>,<SN2>,<CHK> SENSOR state REPORT
      *                         ;   <SN1> =<0,A6,A5,A4- A3,A2,A1,A0>,
      *                         ;           7 ls adr bits.
      *                         ;           A1,A0 select 1 of 4 input pairs in a DS54
      *                         ;   <SN2> =<0,1,I,L- A10,A9,A8,A7>
      *                         ;           Report/status bits and 4 MS adr bits.
      *                         ;           this <B1> opcode encodes input levels
      *                         ;           for turnout feedback
      *                         ;           "I" =0 for "aux" inputs (normally not feedback),
      *                         ;               =1 for "switch" input used for
      *                         ;                  turnout feedback for DS54
      *                         ;                  ouput/turnout # encoded by A0-A10
      *                         ;           "L" =0 for this input 0V (LO),
      *                         ;               =1 this input > +6V (HI)
      *                         ;
      *                         ;   alternately;
      *                         ;
      *                         ;   <SN2> =<0,0,C,T- A10,A9,A8,A7>
      *                         ;           Report/status bits and 4 MS adr bits.
      *                         ;           this <B1> opcode encodes current OUTPUT levels
      *                         ;           "C" =0 if "Closed" ouput line is OFF,
      *                         ;               =1 "closed" output line is ON
      *                         ;                  (sink current)
      *                         ;           "T" =0 if "Thrown" output line is OFF,
      *                         ;               =1 "thrown" output line is ON
      *                         ;                  (sink I)
      *
      * Page 9 of LocoNet Personal Edition v1.0.
      */
 case LnConstants::OPC_SW_REP:
 {
  result = interpretOpcSwRep(l);
     if (result.length() > 0) {
         return result;
     }
     break;
 }

      /*
       * OPC_SW_REQ       0xB0   ; REQ SWITCH function
       *                         ; <0xB0>,<SW1>,<SW2>,<CHK> REQ SWITCH function
       *                         ;   <SW1> =<0,A6,A5,A4- A3,A2,A1,A0>,
       *                         ;           7 ls adr bits.
       *                         ;           A1,A0 select 1 of 4 input pairs in a DS54
       *                         ;   <SW2> =<0,0,DIR,ON- A10,A9,A8,A7>
       *                         ;           Control bits and 4 MS adr bits.
       *                         ;   DIR  =1 for Closed,/GREEN,
       *                         ;        =0 for Thrown/RED
       *                         ;   ON   =1 for Output ON,
       *                         ;        =0 FOR output OFF
       *                         ;
       *                         ;   Note-Immediate response of <0xB4><30><00> if command failed,
       *                         ;        otherwise no response "A" CLASS codes
       *
       * Page 9 of LocoNet Personal Edition v1.0.
       * Page 12 special form Broadcast.
       * Page 13 special form LocoNet interrogate.
       */
 case LnConstants::OPC_SW_REQ: {
  result = interpretOpcSwReq(l);
  if (result.length() > 0) {
      return result;
  }
  break;
 }
     /*
      * OPC_LOCO_SND     0xA2   ;SET SLOT sound functions
      *
      * Page 10 of LocoNet Personal Edition v1.0.
      */
 case LnConstants::OPC_LOCO_SND: {
  result = interpretOpcLocoSnd(l);
     if (result.length() > 0) {
         return result;
     }
     break;
 } // case LnConstants::OPC_LOCO_SND

     /*
      * OPC_LOCO_DIRF 0xA1 ;SET SLOT dir, F0-4 state
      *
      * Page 10 of LocoNet Personal Edition v1.0.
      */
 case LnConstants::OPC_LOCO_DIRF: {
  result = interpretOpcLocoDirf(l);
     if (result.length() > 0) {
         return result;
     }
     break;
 }

     /*
      * OPC_LOCO_SPD 0xA0 ;SET SLOT speed e.g. <0xA0><SLOT#><SPD><CHK>
      *
      * Page 10 of LocoNet Personal Edition v1.0.
      */
 case LnConstants::OPC_LOCO_SPD: {
  result = interpretOpcLocoSpd(l);
     if (result.length() > 0) {
         return result;
     }
     break;
 }

 /*
  * ; 6 byte MESSAGE OPCODES
  * ; FORMAT = <OPC>,<ARG1>,<ARG2>,<ARG3>,<ARG4>,<CKSUM>
  * :
  *  CODES 0xC8 to 0xCF have responses
  *  CODES 0xD8 to 0xDF have responses
  */

     /*
      * OPC_PANEL_QUERY 0xDF messages used by throttles to discover
      * panels
      *
      * This op code is not documented by Digitrax. Reverse engineering
      * performed by Leo Bicknell.  The opcode "name" OPC_PANEL_QUERY
      * is not necessarily the name used by Digitrax.
       */
 case LnConstants::OPC_PANEL_QUERY: {
  result = interpretOpcPanelQuery(l);
     if (result.length() > 0) {
         return result;
     }
     break;

 }

     /*
      * OPC_PANEL_RESPONSE 0xD7 messages used by throttles to discover
      * panels
      *
      * This op code is not documented by Digitrax. Reverse engineering
      * performed by Leo Bicknell.  The opcode "name" OPC_PANEL_RESPONSE
      * is not necessarily the name used by Digitrax.

      */
 case LnConstants::OPC_PANEL_RESPONSE: {
  result = interpretOpcPanelResponse(l);
     if (result.length() > 0) {
         return result;
     }
     break;

 }

     /*
      * OPC_MULTI_SENSE 0xD0 messages about power management and
      * transponding
      *
      * If byte 1 high nibble is 0x20 or 0x00 this is a transponding
      * message
      *
      * This op code is not documented by Digitrax. Reverse engineering
      * performed by Al Silverstein, and corrections added by B. Milhaupt.
      */
 case LnConstants::OPC_MULTI_SENSE: {
  result = interpretOpcMultiSense(l);
     if (result.length() > 0) {
         return result;
     }
     break;
 }

 /*
  * ; VARIABLE byte MESSAGE OPCODES
  * ; FORMAT = <OPC>,<COUNT>,<ARG2>,<ARG3>,...,<ARG(COUNT-3)>,<CKSUM>
  * :
  */

     /**********************************************************************************************
      * OPC_WR_SL_DATA   0xEF   ; WRITE SLOT DATA, 10 bytes                                         *
      *                         ; Follow on message: LACK                                           *
      *                         ; <0xEF>,<0E>,<SLOT#>,<STAT>,<ADR>,<SPD>,<DIRF>,                    *
      *                         ;        <TRK>,<SS2>,<ADR2>,<SND>,<ID1>,<ID2>,<CHK>                 *
      *                         ; SLOT DATA WRITE, 10 bytes data /14 byte MSG                       *
      ***********************************************************************************************
      * OPC_SL_RD_DATA   0xE7   ; SLOT DATA return, 10 bytes                                        *
      *                         ; <0xE7>,<0E>,<SLOT#>,<STAT>,<ADR>,<SPD>,<DIRF>,                    *
      *                         ;        <TRK>,<SS2>,<ADR2>,<SND>,<ID1>,<ID2>,<CHK>                 *
      *                         ; SLOT DATA READ, 10 bytes data /14 byte MSG                        *
      *                         ;                                                                   *
      *                         ; NOTE; If STAT2.2=0 EX1/EX2 encodes an ID#,                        *
      *                         ;       [if STAT2.2=1 the STAT.3=0 means EX1/EX2                    *
      *                         ;        are ALIAS]                                                 *
      *                         ;                                                                   *
      *                         ; ID1/ID2 are two 7 bit values encoding a 14 bit                    *
      *                         ;         unique DEVICE usage ID.                                   *
      *                         ;                                                                   *
      *                         ;   00/00 - means NO ID being used                                  *
      *                         ;                                                                   *
      *                         ;   01/00 - ID shows PC usage.                                      *
      *                         ;    to         Lo nibble is TYP PC#                                *
      *                         ;   7F/01       (PC can use hi values)                              *
      *                         ;                                                                   *
      *                         ;   00/02 -SYSTEM reserved                                          *
      *                         ;    to                                                             *
      *                         ;   7F/03                                                           *
      *                         ;                                                                   *
      *                         ;   00/04 -NORMAL throttle RANGE                                    *
      *                         ;    to                                                             *
      *                         ;   7F/7E                                                           *
      ***********************************************************************************************
      * Notes:                                                                                      *
      * The SLOT DATA bytes are, in order of TRANSMISSION for <E7> READ or <EF> WRITE.              *
      * NOTE SLOT 0 <E7> read will return MASTER config information bytes.                          *
      *                                                                                             *
      * 0) SLOT NUMBER:                                                                             *
      *                                                                                             *
      * ; 0-7FH, 0 is special SLOT,                                                                 *
      *                     ; 070H-07FH DIGITRAX reserved:                                          *
      *                                                                                             *
      * 1) SLOT STATUS1:                                                                            *
      *                                                                                             *
      *     D7-SL_SPURGE    ; 1=SLOT purge en,                                                      *
      *                     ; ALSO adrSEL (INTERNAL use only) (not seen on NET!)                    *
      *                                                                                             *
      *     D6-SL_CONUP     ; CONDN/CONUP: bit encoding-Control double linked Consist List          *
      *                     ;    11=LOGICAL MID CONSIST , Linked up AND down                        *
      *                     ;    10=LOGICAL CONSIST TOP, Only linked downwards                      *
      *                     ;    01=LOGICAL CONSIST SUB-MEMBER, Only linked upwards                 *
      *                     ;    00=FREE locomotive, no CONSIST indirection/linking                 *
      *                     ; ALLOWS "CONSISTS of CONSISTS". Uplinked means that                    *
      *                     ; Slot SPD number is now SLOT adr of SPD/DIR and STATUS                 *
      *                     ; of consist. i.e. is ;an Indirect pointer. This Slot                   *
      *                     ; has same BUSY/ACTIVE bits as TOP of Consist. TOP is                   *
      *                     ; loco with SPD/DIR for whole consist. (top of list).                   *
      *                     ; BUSY/ACTIVE: bit encoding for SLOT activity                           *
      *                                                                                             *
      *     D5-SL_BUSY      ; 11=IN_USE loco adr in SLOT -REFRESHED                                 *
      *                                                                                             *
      *     D4-SL_ACTIVE    ; 10=IDLE loco adr in SLOT -NOT refreshed                               *
      *                     ; 01=COMMON loco adr IN SLOT -refreshed                                 *
      *                     ; 00=FREE SLOT, no valid DATA -not refreshed                            *
      *                                                                                             *
      *     D3-SL_CONDN     ; shows other SLOT Consist linked INTO this slot, see SL_CONUP          *
      *                                                                                             *
      *     D2-SL_SPDEX     ; 3 BITS for Decoder TYPE encoding for this SLOT                        *
      *                                                                                             *
      *     D1-SL_SPD14     ; 011=send 128 speed mode packets                                       *
      *                                                                                             *
      *     D0-SL_SPD28     ; 010=14 step MODE                                                      *
      *                     ; 001=28 step. Generate Trinary packets for this                        *
      *                     ;              Mobile ADR                                               *
      *                     ; 000=28 step. 3 BYTE PKT regular mode                                  *
      *                     ; 111=128 Step decoder, Allow Advanced DCC consisting                   *
      *                     ; 100=28 Step decoder ,Allow Advanced DCC consisting                    *
      *                                                                                             *
      * 2) SLOT LOCO ADR:                                                                           *
      *                                                                                             *
      *     LOCO adr Low 7 bits (byte sent as ARG2 in ADR req opcode <0xBF>)                        *
      *                                                                                             *
      * 3) SLOT SPEED:                                                                              *
      *     0x00=SPEED 0 ,STOP inertially                                                           *
      *     0x01=SPEED 0 EMERGENCY stop                                                             *
      *     0x02->0x7F increasing SPEED,0x7F=MAX speed                                              *
      *     (byte also sent as ARG2 in SPD opcode <0xA0> )                                          *
      *                                                                                             *
      * 4) SLOT DIRF byte: (byte also sent as ARG2 in DIRF opcode <0xA1>)                           *
      *                                                                                             *
      *     D7-0        ; always 0                                                                  *
      *     D6-SL_XCNT  ; reserved , set 0                                                          *
      *     D5-SL_DIR   ; 1=loco direction FORWARD                                                  *
      *     D4-SL_F0    ; 1=Directional lighting ON                                                 *
      *     D3-SL_F4    ; 1=F4 ON                                                                   *
      *     D2-SL_F3    ; 1=F3 ON                                                                   *
      *     D1-SL_F2    ; 1=F2 ON                                                                   *
      *     D0-SL_F1    ; 1=F1 ON                                                                   *
      *                                                                                             *
      *                                                                                             *
      *                                                                                             *
      *                                                                                             *
      * 5) TRK byte: (GLOBAL system /track status)                                                  *
      *                                                                                             *
      *     D7-D4       Reserved                                                                    *
      *     D3          GTRK_PROG_BUSY 1=Programming TRACK in this Master is BUSY.                  *
      *     D2          GTRK_MLOK1     1=This Master IMPLEMENTS LocoNet 1.1 capability,             *
      *                                0=Master is DT200                                            *
      *     D1          GTRK_IDLE      0=TRACK is PAUSED, B'cast EMERG STOP.                        *
      *     D0          GTRK_POWER     1=DCC packets are ON in MASTER, Global POWER up              *
      *                                                                                             *
      * 6) SLOT STATUS:                                                                             *
      *                                                                                             *
      *     D3          1=expansion IN ID1/2, 0=ENCODED alias                                       *
      *     D2          1=Expansion ID1/2 is NOT ID usage                                           *
      *     D0          1=this slot has SUPPRESSED ADV consist-7)                                   *
      *                                                                                             *
      * 7) SLOT LOCO ADR HIGH:                                                                      *
      *                                                                                             *
      * Locomotive address high 7 bits. If this is 0 then Low address is normal 7 bit NMRA SHORT    *
      * address. If this is not zero then the most significant 6 bits of this address are used in   *
      * the first LONG address byte ( matching CV17). The second DCC LONG address byte matches CV18 *
      * and includes the Adr Low 7 bit value with the LS bit of ADR high in the MS postion of this  *
      * track adr byte.                                                                             *
      *                                                                                             *
      * Note a DT200 MASTER will always interpret this as 0.                                        *
      *                                                                                             *
      * 8) SLOT SOUND:                                                                              *
      *                                                                                             *
      *     Slot sound/ Accesory Function mode II packets. F5-F8                                    *
      *     (byte also sent as ARG2 in SND opcode)                                                  *
      *                                                                                             *
      *     D7-D4           reserved                                                                *
      *     D3-SL_SND4/F8                                                                           *
      *     D2-SL_SND3/F7                                                                           *
      *     D1-SL_SND2/F6                                                                           *
      *     D0-SL_SND1/F5   1= SLOT Sound 1 function 1active (accessory 2)                          *
      *                                                                                             *
      * 9) EXPANSION RESERVED ID1:                                                                  *
      *                                                                                             *
      *     7 bit ls ID code written by THROTTLE/PC when STAT2.4=1                                  *
      *                                                                                             *
      * 10) EXPANSION RESERVED ID2:                                                                 *
      *                                                                                             *
      *     7 bit ms ID code written by THROTTLE/PC when STAT2.4=1                                  *
      **********************************************************************************************/
 case LnConstants::OPC_WR_SL_DATA: /* page 10 of Loconet PE */
 case LnConstants::OPC_SL_RD_DATA: { // Page 10 of LocoNet PE
  result = interpretOpcWrSlDataOpcSlRdData(l);
     if (result.length() > 0) {
         return result;
     }
     break;

 }

 case LnConstants::OPC_ALM_WRITE:
 case LnConstants::OPC_ALM_READ: {
  if (l.getElement(1) == 0x10) {

                      if ((l.getElement(2) == 0)
                              && (l.getElement(3) == 0)
                              && (l.getElement(6) == 0)) {
                          return tr("Get Aliasing Information.");
                      } else if ((l.getElement(2) == 0)
                              && (l.getElement(3) == 0)
                              && (l.getElement(6) == 0x0b)) {
                          return tr("Aliasing Report: %1 aliases supported.").arg( l.getElement(4) * 2);
                      } else if ((l.getElement(2) == 0)
                              && (l.getElement(6) == 0xf)
                              && (l.getElement(14) == 0)) {
                          // Alias read and write messages
                          QString message;
                          if (l.getElement(3) == 0x2) {
                              if (l.getOpCode() == LnConstants::OPC_ALM_WRITE) {
                                  return tr("Get Alias pair %1.").arg( l.getElement(4));
                              } else {
                                  message = "Report Alias pair %1: %2 is an alias for %3; %4 is an alias for %5.";
                              }
                          } else {
                              break;
                          }
                          QString longAddr = convertToMixed(l.getElement(7), l.getElement(8));
                          int shortAddr = l.getElement(9);
                          QString longAddr2 = convertToMixed(l.getElement(11), l.getElement(12));
                          int shortAddr2 = l.getElement(13);
                          int pair = l.getElement(4);

                          return QString(message).arg( pair).arg(
                                  longAddr).arg( shortAddr).arg( longAddr2).arg( shortAddr2);
                      } else if ((l.getElement(2) == 0)
                              && (l.getElement(3) == 0x43)) {
                          QString longAddr = convertToMixed(l.getElement(7), l.getElement(8));
                          int shortAddr = l.getElement(9);
                          QString longAddr2 = convertToMixed(l.getElement(11), l.getElement(12));
                          int shortAddr2 = l.getElement(13);
                          int pair = l.getElement(4);
                          return tr("Set Alias pair %1: %2 is an alias for %3; %4 is an alias for %5.").arg(
                                  pair).arg( longAddr).arg( shortAddr).arg( longAddr2).arg( shortAddr2);
                      } else if ((l.getElement(2) == 0)
                              && (l.getElement(6) == 0)
                              && (l.getElement(14) == 0)) {
                          return tr("Get Alias pair %1.").arg( l.getElement(4));
                      }

                  }
                  return tr(
                          ((l.getElement(0) == LnConstants::OPC_ALM_WRITE)
                          ? "Write ALM msg %1 ATASK=%2 %3 BLKL=%4 BLKH=%5 LOGIC=%6\nARG1L=%7 ARG1H=%8 ARG2L=%9 ARG2H=%10\nARG3L=%11 ARG3H=%12 ARG4L=%13 ARG4H=%14."
                          : "Read ALM msg (Write reply) %1 ATASK=%2 %3 BLKL=%4 BLKH=%5 LOGIC=%6\nARG1L=%7 ARG1H=%8 ARG2L=%9 ARG2H=%10\nARG3L=%11 ARG3H=%12 ARG4L=%13 ARG4H=%14.")).arg(
                          l.getElement(2)).arg(
                          l.getElement(3)).arg(
                          getAlmTaskType(l.getElement(3))).arg(
                          l.getElement(4)).arg(
                          l.getElement(5)).arg(
                          l.getElement(6)).arg(
                          tr("0x%1").arg(
                                  StringUtil::twoHexFromInt(l.getElement(7)))).arg(
                          tr("0x%1").arg(
                                  StringUtil::twoHexFromInt(l.getElement(8)))).arg(
                          tr("0x%1").arg(
                                  StringUtil::twoHexFromInt(l.getElement(9)))).arg(
                          tr("0x%1").arg(
                                  StringUtil::twoHexFromInt(l.getElement(10)))).arg(
                          tr("0x%1").arg(
                                  StringUtil::twoHexFromInt(l.getElement(11)))).arg(
                          tr("0x%1").arg(
                                  StringUtil::twoHexFromInt(l.getElement(12)))).arg(
                          tr("0x%1").arg(
                                  StringUtil::twoHexFromInt(l.getElement(13)))).arg(
                          tr("0x%1").arg(
                                  StringUtil::twoHexFromInt(l.getElement(14))));

 } // case LnConstants::OPC_ALM_READ

     /*
      * OPC_PEER_XFER   0xE5    ; move 8 bytes PEER to PEER, SRC->DST   NO resp
      *                         ; <0xE5>,<10>,<SRC>,<DSTL><DSTH>,<PXCT1>,<D1>,<D2>,<D3>,<D4>,
      *                         ; <PXCT2>,<D5>,<D6>,<D7>,<D8>,<CHK>
      *                         ; SRC/DST are 7 bit args. DSTL/H=0 is BROADCAST msg
      *                         ;     SRC=0 is MASTER
      *                         ;     SRC=0x70-0x7E are reserved
      *
      * Page 10 of LocoNet Personal Edition v1.0.
      *
      * Duplex group management reverse engineered by Leo Bicknell, with input from
      * B. Milhaupt.
      */
 case LnConstants::OPC_PEER_XFER:
 {
  result = interpretOpcPeerXfer(l);
  if (result.length() > 0) {
      return result;
  }
  break;
 }

 case 0xe4://LnConstants::OPC_LISSY_UPDATE:
 {
     /*
      * OPC_LISSY_UPDATE   0xE5
      *
      * LISSY is an automatic train detection system made by Uhlenbrock.
      * All documentation appears to be in German.
      *
      */
     switch (l.getElement(1))
     {
         case 0x08: // Format LISSY message
         {
             int unit = (l.getElement(4) & 0x7F);
             int address = (l.getElement(6) & 0x7F) + 128 * (l.getElement(5) & 0x7F);
             if (l.getElement(2) == 0x00) {
                 // Reverse-engineering note: interpretation of element 2 per wiki.rocrail.net
                 // OPC_LISSY_REP
                 return "Lissy " + QString("%1").arg(unit) +
                         " IR Report: Loco " + QString("%1").arg(address) +
                         " moving " +
                         ((l.getElement(3)&0x20)==0 ? "north\n":"south\n");
             }
             else if (l.getElement(2) == 0x01) {
                 // Reverse-engineering note: interpretation of element 2 per wiki.rocrail.net
                 // OPC_WHEELCNT_REP
                 int wheelCount = (l.getElement(6)&0x7F)+128*(l.getElement(5)&0x7F);
                 return "Lissy " + QString("%1").arg(unit) +
                         " Wheel Report: " + QString("%1").arg(wheelCount) +
                         " wheels moving " +
                         ((l.getElement(3)&0x20)==0 ? "north\n":"south\n");
             }
             else {
                 forceHex = true;
                 return "Unrecognized Lissy message varient.\n";
              }
         }
         case 0x0A: // Format special message
         {
             int element = l.getElement(2) * 128 + l.getElement(3);
             int stat1 = l.getElement(5);
             int stat2 = l.getElement(6);
             QString status;
             if ((stat1 & 0x10) != 0)
                 if ((stat1 & 0x20) != 0)
                     status = " AX, XA reserved; ";
                 else
                     status = " AX reserved; ";
             else if ((stat1 & 0x20) != 0)
                 status = " XA reserved; ";
             else
                 status = " no reservation; ";
             if ((stat2 & 0x01) != 0)
                 status += "Turnout thrown; ";
             else
                 status += "Turnout closed; ";
             if ((stat1 & 0x01) != 0)
                 status += "Occupied";
             else
                 status += "Not occupied";
             return "SE" + QString("%1").arg(element + 1) + " (" + QString("%1").arg(element) + ") reports AX:" + QString("%1").arg(l.getElement(7))
                    + " XA:" + QString("%1").arg(l.getElement(8)) + status + "\n";
         }
         default:
             forceHex = true;
             return "Unrecognized OPC_LISSY_UPDATE command varient.\n";
         }
     } // case LnConstants::OPC_LISSY_UPDATE

     /*
      * OPC_IMM_PACKET   0xED   ;SEND n-byte packet immediate LACK
      *                         ; Follow on message: LACK
      *                         ; <0xED>,<0B>,<7F>,<REPS>,<DHI>,<IM1>,<IM2>,
      *                         ;        <IM3>,<IM4>,<IM5>,<CHK>
      *                         ;   <DHI>=<0,0,1,IM5.7-IM4.7,IM3.7,IM2.7,IM1.7>
      *                         ;   <REPS>  D4,5,6=#IM bytes,
      *                         ;           D3=0(reserved);
      *                         ;           D2,1,0=repeat CNT
      *                         ; IF Not limited MASTER then
      *                         ;   LACK=<B4>,<7D>,<7F>,<chk> if CMD ok
      *                         ; IF limited MASTER then Lim Masters respond
      *                         ;   with <B4>,<7E>,<lim adr>,<chk>
      *                         ; IF internal buffer BUSY/full respond
      *                         ;   with <B4>,<7D>,<0>,<chk>
      *                         ;   (NOT IMPLEMENTED IN DT200)
      *
      * This sends a raw NMRA packet across the LocoNet.
      *
      * Page 11 of LocoNet Personal Edition v1.0.
      *
      * Decodes for the F9-F28 functions taken from the NMRA standards and
      * coded by Leo Bicknell.
      */
     case 0xed://LnConstants::OPC_IMM_PACKET:
     {
         // sendPkt = (sendPktMsg *) msgBuf;
         int val7f = l.getElement(2); /* fixed value of 0x7f */
         int reps = l.getElement(3); /* repeat count */
         int dhi = l.getElement(4); /* high bits of data bytes */
         int im1 = l.getElement(5);
         int im2 = l.getElement(6);
         int im3 = l.getElement(7);
         int im4 = l.getElement(8);
         int im5 = l.getElement(9);
         int mobileDecoderAddress =-999;
         int nmraInstructionType = -999;
         int nmraSubInstructionType = -999;
         int playableWhistleLevel = -999;

         // see if it really is a 'Send Packet' as defined in Loconet PE
         if (val7f == 0x7f) {
             int len = ((reps & 0x70) >> 4);
             // duplication of packet data as packetInt was deemed necessary
             // due to issues with msBit loss when converting from "byte" to
             // integral forms
             //byte[] packet = new byte[len];
             QList<char> packet;
             for(int i =0; i < len; i++)
              packet.append(0);
             //packet.reserve(len);
             //int[] packetInt = new int[len];
             QList<int> packetInt;
             packetInt.reserve(len);
             for(int i =0; i < len; i++)
              packetInt.append(0);
             packet[0] = (char) (im1 + ((dhi & 0x01) != 0 ? 0x80 : 0));
             packetInt[0] = (im1 + ((dhi & 0x01) != 0 ? 0x80 : 0));
             if (len >= 2) {
                 packet[1] = (char) (im2 + ((dhi & 0x02) != 0 ? 0x80 : 0));
                 packetInt[1] = (im2 + ((dhi & 0x02) != 0 ? 0x80 : 0));
             }
             if (len >= 3) {
                 packet[2] = (char) (im3 + ((dhi & 0x04) != 0 ? 0x80 : 0));
                 packetInt[2] = (im3 + ((dhi & 0x04) != 0 ? 0x80 : 0));
             }
             if (len >= 4) {
                 packet[3] = (char) (im4 + ((dhi & 0x08) != 0 ? 0x80 : 0));
                 packetInt[3] = (im4 + ((dhi & 0x08) != 0 ? 0x80 : 0));
             }
             if (len >= 5) {
                 packet[4] = (char) (im5 + ((dhi & 0x10) != 0 ? 0x80 : 0));
                 packetInt[4] = (im5 + ((dhi & 0x10) != 0 ? 0x80 : 0));
             }

             int address;
             // compute some information which is useful for decoding
             // the "Playable" whistle message
             // Information reverse-engineered by B. Milhaupt and used with permission
             if ((packetInt[0] & 0x80)  == 0x0) {
                 // immediate packet addresses a 7-bit multi-function (mobile) decoder
                 mobileDecoderAddress = packetInt[0];
                 nmraInstructionType = (packetInt[1] & 0xE) >> 5;
                 nmraSubInstructionType = (packetInt[1] & 0x1f);
                 if ((nmraSubInstructionType == 0x1d) && (packetInt[2] == 0x7f)) {
                     playableWhistleLevel = packetInt[3];
                 }
             }
             else if ((packetInt[0] & 0xC0) == 0xC0) {
                 // immediate packet addresses a 14-bit multi-function (mobile) decoder
                 mobileDecoderAddress =  ( (packetInt[0] & 0x3F) << 8) + packetInt[1];
                 nmraInstructionType = (packetInt[2] & 0xE0) >> 5;
                 nmraSubInstructionType = (packetInt[2] & 0x1f);
                 if ((nmraSubInstructionType == 0x1d) && (packetInt[3] == 0x7f)) {
                     playableWhistleLevel = packetInt[4];
                 }
             }
             else {
                 // immediate packet not addressed to a multi-function (mobile) decoder
             }
             QString generic = "";
             if ((mobileDecoderAddress >= 0) &&
                     (nmraInstructionType == 1) &&
                     (nmraSubInstructionType == 0x1D)) {
                 // the "Playable" whistle message
                 // Information reverse-engineered by B. Milhaupt and used with permission
                  generic = "Playable Whistle control - Loco " +
                         QString("%1").arg(mobileDecoderAddress) +
                         " whistle to " +
                         QString("%1").arg(playableWhistleLevel) +
                         " (repeat "+ (reps & 0x7) +" times).\n";
                  return generic;
             }
             /*
              * We use this two places below, so we generate it once here.
              * That seems wrong, but what we really need is to be able to
              * decode any NMRA packet here, which is a lot more work!
              */
             generic = "Send packet immediate: "
                              + QString("%1").arg((reps & 0x70) >> 4)
                              + " bytes, repeat count " + QString("%1").arg(reps & 0x07)
                              + "(" + QString("%1").arg(reps) + ")" + "\n\tDHI=0x"
                              + QString("%1").arg(dhi,0,16) + ", IM1=0x"
                              + QString("%1").arg(im1,0,16) + ", IM2=0x"
                              + QString("%1").arg(im2,0,16) + ", IM3=0x"
                              + QString("%1").arg(im3,0,16) + ", IM4=0x"
                              + QString("%1").arg(im4,0,16) + ", IM5=0x"
                              + QString("%1").arg(im5,0,16) + "\n\tpacket: ";

             // F9-F28 w/a long address.
             if ((packetInt[0] & 0xC0) == 0xC0) {
                 address = ((packetInt[0] & 0x3F) << 8) + packetInt[1];

                 if ((packetInt[2] & 0xFF) == 0xDF) {
                     // Functions 21-28
                     return "Send packet immediate: Locomotive " + QString("%1").arg(address)
                            + " set" + " F21="
                            + ((packetInt[3] & 0x01) > 0 ? tr("On") : tr("Off"))
                            + ", F22="
                            + ((packetInt[3] & 0x02) > 0 ? tr("On") : tr("Off"))
                            + ", F23="
                            + ((packetInt[3] & 0x04) > 0 ? tr("On") : tr("Off"))
                            + ", F24="
                            + ((packetInt[3] & 0x08) > 0 ? tr("On") : tr("Off"))
                            + ", F25="
                            + ((packetInt[3] & 0x10) > 0 ? tr("On") : tr("Off"))
                            + ", F26="
                            + ((packetInt[3] & 0x20) > 0 ? tr("On") : tr("Off"))
                            + ", F27="
                            + ((packetInt[3] & 0x40) > 0 ? tr("On") : tr("Off"))
                            + ", F28="
                            + ((packetInt[3] & 0x80) > 0 ? tr("On") : tr("Off")) + "\n";
                 } else if ((packetInt[2] & 0xFF) == 0xDE) {
                     // Functions 13-20
                     return "Send packet immediate: Locomotive " + QString("%1").arg(address)
                            + " set" + " F13="
                            + ((packetInt[3] & 0x01) > 0 ? tr("On") : tr("Off"))
                            + ", F14="
                            + ((packetInt[3] & 0x02) > 0 ? tr("On") : tr("Off"))
                            + ", F15="
                            + ((packetInt[3] & 0x04) > 0 ? tr("On") : tr("Off"))
                            + ", F16="
                            + ((packetInt[3] & 0x08) > 0 ? tr("On") : tr("Off"))
                            + ", F17="
                            + ((packetInt[3] & 0x10) > 0 ? tr("On") : tr("Off"))
                            + ", F18="
                            + ((packetInt[3] & 0x20) > 0 ? tr("On") : tr("Off"))
                            + ", F19="
                            + ((packetInt[3] & 0x40) > 0 ? tr("On") : tr("Off"))
                            + ", F20="
                            + ((packetInt[3] & 0x80) > 0 ? tr("On") : tr("Off")) + "\n";
                 } else if ((packetInt[2] & 0xF0) == 0xA0) {
                     // Functions 8-12
                     return "Send packet immediate: Locomotive " + QString("%1").arg(address)
                            + " set" + ", F09="
                            + ((packetInt[2] & 0x01) > 0 ? tr("On") : tr("Off"))
                            + ", F10="
                            + ((packetInt[2] & 0x02) > 0 ? tr("On") : tr("Off"))
                            + ", F11="
                            + ((packetInt[2] & 0x04) > 0 ? tr("On") : tr("Off"))
                            + ", F12="
                            + ((packetInt[2] & 0x08) > 0 ? tr("On") : tr("Off")) + "\n";
                 } else {
                     // Unknown
                     return generic /*+ jmri.NmraPacket.format(packet) */+ "\n";
                 }
             } else { // F9-F28 w/a short address.
                 address = packetInt[0];
                 if ((packetInt[1] & 0xFF) == 0xDF) {
                     // Functions 21-28
                     return "Send packet immediate: Locomotive " + QString("%1").arg(address)
                            + " set" + " F21="
                            + ((packetInt[2] & 0x01) > 0 ? tr("On") : tr("Off"))
                            + ", F22="
                            + ((packetInt[2] & 0x02) > 0 ? tr("On") : tr("Off"))
                            + ", F23="
                            + ((packetInt[2] & 0x04) > 0 ? tr("On") : tr("Off"))
                            + ", F24="
                            + ((packetInt[2] & 0x08) > 0 ? tr("On") : tr("Off"))
                            + ", F25="
                            + ((packetInt[2] & 0x10) > 0 ? tr("On") : tr("Off"))
                            + ", F26="
                            + ((packetInt[2] & 0x20) > 0 ? tr("On") : tr("Off"))
                            + ", F27="
                            + ((packetInt[2] & 0x40) > 0 ? tr("On") : tr("Off"))
                            + ", F28="
                            + ((packetInt[2] & 0x80) > 0 ? tr("On") : tr("Off")) + "\n";
                 } else if ((packetInt[1] & 0xFF) == 0xDE) {
                     // Functions 13-20
                     return "Send packet immediate: Locomotive " + QString("%1").arg(address)
                            + " set" + " F13="
                            + ((packetInt[2] & 0x01) > 0 ? tr("On") : tr("Off"))
                            + ", F14="
                            + ((packetInt[2] & 0x02) > 0 ? tr("On") : tr("Off"))
                            + ", F15="
                            + ((packetInt[2] & 0x04) > 0 ? tr("On") : tr("Off"))
                            + ", F16="
                            + ((packetInt[2] & 0x08) > 0 ? tr("On") : tr("Off"))
                            + ", F17="
                            + ((packetInt[2] & 0x10) > 0 ? tr("On") : tr("Off"))
                            + ", F18="
                            + ((packetInt[2] & 0x20) > 0 ? tr("On") : tr("Off"))
                            + ", F19="
                            + ((packetInt[2] & 0x40) > 0 ? tr("On") : tr("Off"))
                            + ", F20="
                            + ((packetInt[2] & 0x80) > 0 ? tr("On") : tr("Off")) + "\n";
                 } else if ((packetInt[1] & 0xF0) == 0xA0) {
                     // Functions 8-12
                     return "Send packet immediate: Locomotive " + QString("%1").arg(address)
                            + " set" + " F09="
                            + ((packetInt[1] & 0x01) > 0 ? tr("On") : tr("Off"))
                            + ", F10="
                            + ((packetInt[1] & 0x02) > 0 ? tr("On") : tr("Off"))
                            + ", F11="
                            + ((packetInt[1] & 0x04) > 0 ? tr("On") : tr("Off"))
                            + ", F12="
                            + ((packetInt[1] & 0x08) > 0 ? tr("On") : tr("Off")) + "\n";
                 } else {
                     // Unknown
                     return generic /*+ jmri.NmraPacket.format(packet)*/ + "\n";
                 }
             } // else { // F9-F28 w/a short address.
         } else {
             /* Hmmmm... */
             forceHex = true;
             return "Undefined Send Packet Immediate, 3rd byte id 0x"
                    + QString("%1").arg(val7f,0,16) + " not 0x7f.\n";
         }
     } // case LnConstants::OPC_IMM_PACKET

     case 0xd3://LnConstants::RE_OPC_PR3_MODE:
 {
  result = interpretOpcPr3Mode(l);
  if (result.length() > 0) {
      return result;
  }
  break;
 }
#if 0
 case LnConstants::RE_OPC_IB2_F9_F12: {
  result = interpretIb2F9_to_F12(l);
  if (result.length() > 0) {
      return result;
  }
  break;
}

case LnConstants::RE_OPC_IB2_SPECIAL: { // 0xD4
  result = interpretIb2Special(l);
  if (result.length() > 0) {
      return result;
  }
  break;

}//  case LnConstants.RE_OPC_IB2_SPECIAL: { //0xD4
#endif
     default:
     forceHex = true;
     return "Unable to parse command.\n";

 } // end switch over opcode type - default handles unrecognized cases,
 // so can't reach here
} // end of protected String format(LocoNetMessage l)

/**
 * This function creates a string representation of a LocoNet buffer. The
 * string may be more than one line, and is terminated with a newline.
 *
 * @return The created string representation.
 */
QString LlnMon::displayMessage(LocoNetMessage l)
{

 forceHex = false;
 QString s = format(l);
 if (forceHex)
  s += "contents: " + l.toString() + "\n";
 if (showOpCode)
  s = LnConstants::OPC_NAME(l.getOpCode()) + ": " + s;
 return s;
} // end of public String displayMessage(LocoNetMessage l)



/**
* sets the loconet turnout manager which is used to find turnout "user names" from
* turnout "system names"
* @param loconetTurnoutManager
*/
/*public*/ void LlnMon::setLocoNetTurnoutManager (TurnoutManager* loconetTurnoutManager)
{
 turnoutManager = loconetTurnoutManager;
 qDebug() << QString("systemPrefix = %1").arg(turnoutManager->getSystemPrefix());
 turnoutPrefix = turnoutManager->getSystemPrefix().mid(0,1).append( "T");
}

/**
* sets the loconet sensor manager which is used to find sensor "user names" from
* sensor "system names"
* @param loconetSensorManager
*/
/*public*/ void LlnMon::setLocoNetSensorManager (SensorManager* loconetSensorManager)
{
 sensorManager = loconetSensorManager;
 sensorPrefix = sensorManager->getSystemPrefix().mid(0,1) +"S";
}

/*public*/ void LlnMon::setLocoNetReporterManager (ReporterManager* loconetReporterManager)
{
 reporterManager =  loconetReporterManager;
 reporterPrefix = reporterManager->getSystemPrefix().mid(0,1) + "R";
}

/*private*/ QString LlnMon::interpretOpcPeerXfer20_1(LocoNetMessage l) {
        switch (l.getElement(3)) {
            case 0x08: {
                return tr("Query Duplex Receivers.");
            }
            case 0x10: {
                return tr("Duplex Receiver Response.");
            }
            default: {
                break;
            }
        }
        return "";
    }

    /*private*/ QString LlnMon::interpretOpcPeerXfer20_2(LocoNetMessage l) {
        switch (l.getElement(3)) {
            case 0x00: {
                int channel = l.getElement(5) | ((l.getElement(4) & 0x01) << 7);

                return tr("Set Duplex Channel to %1.").arg(channel);
            }
            case 0x08: {
                return tr("Query Duplex Channel.");
            }
            case 0x10: {
                int channel = l.getElement(5) | ((l.getElement(4) & 0x01) << 7);

                return tr("Reported Duplex Channel is %1.").arg(channel);
            }
            default: {
                break;
            }
        }
        return "";
    }

    /*private*/ QString LlnMon::interpretOpcPeerXfer20_3(LocoNetMessage l) {
        // Characters appear to be 8 bit values, but transmitted over a 7 bit
        // encoding, so high order bits are stashed in element 4 and 9.
        char groupNameArray[] = {(char) (l.getElement(5) | ((l.getElement(4) & 0x01) << 7)),
            (char) (l.getElement(6) | ((l.getElement(4) & 0x02) << 6)),
            (char) (l.getElement(7) | ((l.getElement(4) & 0x04) << 5)),
            (char) (l.getElement(8) | ((l.getElement(4) & 0x08) << 4)),
            (char) (l.getElement(10) | ((l.getElement(9) & 0x01) << 7)),
            (char) (l.getElement(11) | ((l.getElement(9) & 0x02) << 6)),
            (char) (l.getElement(12) | ((l.getElement(9) & 0x04) << 5)),
            (char) (l.getElement(13) | ((l.getElement(9) & 0x08) << 4))};
        QString groupName =  QString(groupNameArray);

        // The pass code is stuffed in here, each digit in 4 bits.  But again, it's a
        // 7 bit encoding, so the MSB of the "upper" half is stuffed into byte 14.
        int p1 = ((l.getElement(14) & 0x01) << 3) | ((l.getElement(15) & 0x70) >> 4);
        int p2 = l.getElement(15) & 0x0F;
        int p3 = ((l.getElement(14) & 0x02) << 2) | ((l.getElement(16) & 0x70) >> 4);
        int p4 = l.getElement(16) & 0x0F;

        // It's not clear you can set A-F from throttles or Digitrax's tools, but
        // they do take and get returned if you send them on the wire...
        QString passcode = StringUtil::twoHexFromInt(p1) + StringUtil::twoHexFromInt(p2)
                + StringUtil::twoHexFromInt(p3) + StringUtil::twoHexFromInt(p4);

        // The MSB is stuffed elsewhere again...
        int channel = l.getElement(17) | ((l.getElement(14) & 0x04) << 5);

        // The MSB is stuffed elsewhere one last time.
        int id = l.getElement(18) | ((l.getElement(14) & 0x08) << 4);

        switch (l.getElement(3)) {
            case 0x00: {
                return tr("Set Duplex Group Name to %1.").arg(
                        groupName);
            }
            case 0x08: {
                return tr("LN_MSG_DUPLEX_NAME_QUERY");
            }
            case 0x10: {
                return tr("Reported Duplex Group Name=\"%1\", Password=%2, Channel=%3, ID=%4.").arg(
                        groupName).arg(passcode).arg(channel).arg(id);
            }
            default: {
                break;
            }
        }
        return "";
    }

    /*private*/ QString LlnMon::interpretOpcPeerXfer20_4(LocoNetMessage l) {
        // The MSB is stuffed elsewhere again...
        int id = l.getElement(5) | ((l.getElement(4) & 0x01) << 7);

        switch (l.getElement(3)) {
            case 0x00: {
                return tr("Set Duplex ID to %1.").arg(id);
            }
            case 0x08: {
                return tr("Query Duplex ID.");
            }
            case 0x10: {
                return tr("Reported Duplex ID is %1.").arg(id);
            }
            default: {
                break;
            }
        }
        return "";
    }

    /*private*/ QString LlnMon::interpretOpcPeerXfer20_7(LocoNetMessage l) {
        if (l.getElement(3) == 0x08) {
            return tr("Query Duplex Password.");
        }

        if ((l.getElement(5) < 0x30) || (l.getElement(5) > 0x3c)
                || (l.getElement(6) < 0x30) || (l.getElement(6) > 0x3c)
                || (l.getElement(7) < 0x30) || (l.getElement(7) > 0x3c)
                || (l.getElement(8) < 0x30) || (l.getElement(8) > 0x3c)) {
            return "";
        }
        char groupPasswordArray[] = {(char) l.getElement(5),
            (char) l.getElement(6),
            (char) l.getElement(7),
            (char) l.getElement(8)};
        if ((groupPasswordArray[0] > 0x39) && (groupPasswordArray[0] < 0x3d)) {
            groupPasswordArray[0] += ('A' - '9' - 1);
        }
        if ((groupPasswordArray[1] > 0x39) && (groupPasswordArray[1] < 0x3d)) {
            groupPasswordArray[1] += ('A' - '9' - 1);
        }
        if ((groupPasswordArray[2] > 0x39) && (groupPasswordArray[2] < 0x3d)) {
            groupPasswordArray[2] += ('A' - '9' - 1);
        }
        if ((groupPasswordArray[3] > 0x39) && (groupPasswordArray[3] < 0x3d)) {
            groupPasswordArray[3] += ('A' - '9' - 1);
        }
        QString groupPassword =  QString(groupPasswordArray);

        switch (l.getElement(3)) {
            case 0x00: {
                return tr("Set Duplex Password to %1.").arg(groupPassword);
            }
            case 0x10: {
                return tr("Reported Duplex Password is %1.").arg(groupPassword);
            }
            default: {
                break;
            }
        }
        return "";
    }

    /*private*/ QString LlnMon::interpretOpcPeerXfer20_10(LocoNetMessage l) {
        switch (l.getElement(3)) {
            case 0x08: {
                return tr("Query Duplex Channel %1 noise/activity.").arg( l.getElement(5));
            }
            case 0x10: {
                // High order bit stashed in another element again.
                int level = (l.getElement(6) & 0x7F) + ((l.getElement(4) & 0x02) << 6);

                return tr("Reported Duplex Channel %1 noise/activity level is %2/255.").arg( l.getElement(5)).arg(level);
            }
            default: {
                break;
            }
        }
        return "";
    }

    /*private*/ QString LlnMon::interpretOpcPeerXfer20_8(LocoNetMessage l) {
        /**
         * **********************************************************************************
         * IPL-capable device ping - OPC_RE_IPL (Device Ping Operations) * The
         * message bytes as assigned as follows:
         * <p>
         * <E5> <14> <08> <GR_OP_T> <DI_F2> <DI_Ss0>
         * <DI_Ss1> ...
         * <p>
         * <DI_Ss2> <DI_Ss3> <DI_U1> <00> <00> <DI_U2>
         * <DI_U3> ...
         * <p>
         * <00> <00><00> <00><00> <CHK> * where:
         * <p>
         * <DI_F2> encodes additional bits for the Slave device serial number. *
         * bits 7-4 always 0000b * bit 3 Bit 31 of Slave Device Serial Number *
         * bit 2 Bit 23 of Slave Device Serial Number * bit 1 Bit 15 of Slave
         * device Serial Number * bit 0 Bit 7 of Slave device Serial Number
         * <p>
         * <DI_Ss0> encodes 7 bits of the 32 bit Host device serial number: *
         * bit 7 always 0 * bits 6-0 Bits 6:0 of Slave device serial number
         * <p>
         * <DI_Ss1> encodes 7 bits of the 32 bit Host device serial number: *
         * bit 7 always 0 * bits 6-0 Bits 14:8 of Slave device serial number
         * <p>
         * <DI_Ss2> encodes 7 bits of the 32 bit Host device serial number: *
         * bit 7 always 0 * bits 6-0 Bits 22:16 of Slave device serial number
         * <p>
         * <DI_Ss3> encodes 7 bits of the 32 bit Host device serial number: *
         * bit 7 always 0 * bits 6-0 Bits 30:24 of Slave device serial number
         * <p>
         * <DI_U1> unknown data * when <GR_OP_T> = 0x08 * is always 0 * when
         * <GR_OP_T> = 0x10 * is not reverse-engineered and may be non-zero.
         * <p>
         * <DI_U2> unknown data * when <GR_OP_T> = 0x08 * is always 0 * when
         * <GR_OP_T> = 0x10 * is not reverse-engineered and may be non-zero.
         * <p>
         * <DI_U3> unknown data * when <GR_OP_T> = 0x08 * is always 0 * when
         * <GR_OP_T> = 0x10 * is not reverse-engineered and may be non-zero. * *
         * Information reverse-engineered by B. Milhaupt and used with
         * permission *
         * **********************************************************************************
         */
        /* OPC_RE_IPL (IPL Ping Operation) */
        // Operations related to DigiIPL Device "Ping" operations
        //
        // "Ping" request issued from DigiIPL ver 1.09 issues this message on LocoNet.
        // The LocoNet request message encodes a serial number but NOT a device type.
        //
        // Depending on which devices are selected in DigiIPL when the "ping"
        // is selected, (and probably the S/Ns of the devices attached to the LocoNet,
        // the response is as follows:
        //     DT402D  LocoNet message includes the serial number from the DT402D's
        //             Slave (RF24) serial number.  If a UR92 is attached to LocoNet,
        //             it will send the message via its RF link to the addressed
        //             DT402D.  (UR92 apparantly assumes that the long 802.15.4
        //             address of the DT402D is based on the serial number embedded
        //             in the LocoNet message, with the MS 32 bits based on the UR92
        //             long address MS 32 bits).  If more than one UR92 is attached
        //             to LocoNet, all will pass the message to the RF interface.
        //     UR92    LocoNet message includes the Slave serial number from the UR92.
        //             These messages are not passed to the RF link by the addressed
        //             UR92.  If more than one UR92 is attached to LocoNet, and the
        //             addressed UR92 hears the RF version of the LocoNet message, it
        //             will respond via the RF interface with an acknowledge packet,
        //             and a UR92 (not sure which one) responds on LocoNet with a
        //             Ping report <e5><14><08><10>.
        //     PR3     LocoNet message includes an effective serial number of all
        //             zeros.  There is no LocoNet message reply generated to a
        //             request to a PR3 S/N, but there will be a reply on the PR3's
        //             computer interface if the ping request was sent via the PR3's
        //             computer interface (i.e. not from some other LocoNet agent).
        //     UT4D    While it has been suggested that the UT4D supports firmware
        //             updates, the UT4D does not respond to the Ping message.
        //     LNRP    While it has been suggested that the LNRP supports firmware
        //             updates, the LNRP does not respond to the Ping message.
        //
        // Ping Report values:
        //     <unkn1> Seems always to be <0C>.  None of the bytes relate to
        //             Duplex Channel Number.
        //     <unkn2> Matches byte 15 of the MAC payload of the reply sent by the
        //             targeted UR92.
        //     <unkn3> Unclear what this byte means.
        //
        // Information reverse-engineered by B. Milhaupt and used with permission
        switch (l.getElement(3)) {
            case 0x08:
                /* OPC_RE_IPL (IPL Ping Query) */
                // Ping Request: <e5><14><08><08><msBits><Sn0><Sn1><Sn2><Sn3><0><0><0><0><0><0><0><0><0><0><0><Chk>

                if ((((l.getElement(4) & 0xF) != 0) || (l.getElement(5) != 0)
                        || (l.getElement(6) != 0) || (l.getElement(7) != 0) || (l.getElement(8) != 0))
                        && (l.getElement(9) == 0) && (l.getElement(10) == 0)
                        && (l.getElement(11) == 0) && (l.getElement(12) == 0)
                        && (l.getElement(13) == 0) && (l.getElement(14) == 0)
                        && (l.getElement(15) == 0) && (l.getElement(16) == 0)
                        && (l.getElement(17) == 0) && (l.getElement(18) == 0)) {

                    int hostSnInt;
                    hostSnInt = (l.getElement(5) + (((l.getElement(4) & 0x1) == 1) ? 128 : 0))
                            + ((l.getElement(6) + (((l.getElement(4) & 0x2) == 2) ? 128 : 0)) * 256)
                            + ((l.getElement(7) + (((l.getElement(4) & 0x4) == 4) ? 128 : 0)) * 256 * 256)
                            + ((l.getElement(8) + (((l.getElement(4) & 0x8) == 8) ? 128 : 0)) * 256 * 256 * 256);
                    return tr("Pinging device with serial number 0x%1.").arg(hostSnInt,16);
                }
                break;
            case 0x10:
                /* OPC_RE_IPL (IPL Ping Report) */

                // Ping Report:  <e5><14><08><10><msbits><Sn0><Sn1><Sn2><Sn3><unkn1><0><0><Unkn2><Unkn3><0><0><0><0><0><Chk>
                if (((l.getElement(4) & 0xF) != 0) || (l.getElement(5) != 0) || (l.getElement(6) != 0)
                        || (l.getElement(7) != 0) || (l.getElement(8) != 0)) {   // if any serial number bit is non-zero //
                    int hostSnInt = (l.getElement(5) + (((l.getElement(4) & 0x1) == 1) ? 128 : 0))
                            + ((l.getElement(6) + (((l.getElement(4) & 0x2) == 2) ? 128 : 0)) * 256)
                            + ((l.getElement(7) + (((l.getElement(4) & 0x4) == 4) ? 128 : 0)) * 256 * 256)
                            + ((l.getElement(8) + (((l.getElement(4) & 0x8) == 8) ? 128 : 0)) * 256 * 256 * 256);
                    return tr("Ping Report response from device with serial number %1: Local RSSI=%2, Remote RSSI=%3.").arg(hostSnInt).arg(
                            StringUtil::twoHexFromInt(l.getElement(12) + (((l.getElement(9)) & 0x4) == 0x4 ? 128 : 0))).arg(
                            StringUtil::twoHexFromInt(l.getElement(13) + (((l.getElement(9)) & 0x8) == 0x8 ? 128 : 0))
                    );
                }
                break;
            default:
                break;
        }
        return "";
    }

    /*private*/ QString LlnMon::interpretOpcPeerXfer20_0f(LocoNetMessage l) {
        QString device;

        switch (l.getElement(3)) {
            case 0x08: {
                if ((l.getElement(4) == 0)
                        && (l.getElement(5) == 0) && (l.getElement(6) == 0)
                        && (l.getElement(7) == 0) && (l.getElement(8) == 0)
                        && (l.getElement(9) == 0) && (l.getElement(10) == 0)
                        && (l.getElement(11) == 1) && (l.getElement(12) == 0)
                        && (l.getElement(13) == 0) && (l.getElement(14) == 0)
                        && (l.getElement(15) == 0) && (l.getElement(16) == 0)
                        && (l.getElement(17) == 0) && (l.getElement(18) == 0)) {
                    /**
                     * **********************************************************************************
                     * IPL capable device query - RE_IPL_IDENTITY_OPERATION
                     * (Device Query) * The message bytes are assigned as
                     * follows:
                     * <p>
                     * <E5> <14> <0F> <08> <00> <00>
                     * <00> <00> <00> <00> <00> <01>
                     * <00> <00> ...
                     * <p>
                     * <00> <00> <00> <00> <00> <CHK> * * Information
                     * reverse-engineered by B. Milhaupt and used with
                     * permission *
                     * **********************************************************************************
                     */
                    // Request for all IPL-queryable devices to report their presence
                    //
                    // Information reverse-engineered by B. Milhaupt and used with permission

                    return tr("Discover all IPL-capable devices request.");
                } else if (((l.getElement(5) != 0) || (l.getElement(6) != 0))) {
                    /**
                     * **********************************************************************************
                     * IPL device query by type - RE_IPL_IDENTITY_OPERATION
                     * (Device Query) * The message bytes are assigned as
                     * follows:
                     * <p>
                     * <E5> <14> <0F> <08> <DI_Hmf>
                     * <DI_Hst> <DI_Slv> <00> <00> <00>
                     * <00> <01> ...
                     * <p>
                     * <00> <00> <00> <00> <00> <00>
                     * <00> <CHK> * where:
                     * <p>
                     * <DI_Hmf> DigiIPL-capable Host device manufacturer number.
                     * This is not * the same as an NMRA Manufacturer ID. * 0x00
                     * Digitrax * Others No other Host device manufacturer *
                     * numbers have been reverse- * engineered
                     * <p>
                     * <DI_Hst> encodes the DigiIPL-capable Host device type as
                     * follows: * When <DI_Hmf> = 0x00 * 0x00 (0 decimal) No
                     * Host device type reported * 0x04 (4 decimal) UT4D (Note
                     * that UT4D, UT4 and UT4R do * not respond to this DigiIPL
                     * * request) * 0x18 (24 decimal) RF24 - not typically a
                     * Host device * 0x23 (35 decimal) PR3 * 0x2A (42 decimal)
                     * DT402 (or DT402R or DT402D) * 0x33 (51 decimal) DCS51 *
                     * 0x5C (92 decimal) UR92 * Others No other Host device
                     * types have been * reverse-engineered * When
                     * <DI_Hmf> is not 0x00 * All values Not reverse-engineered
                     * <p>
                     * <DI_Slv> encodes the DigiIPL-capable Slave device type as
                     * follows: * When <DI_Smf> = 0x00 * 0x00 (0 decimal) Report
                     * for all Slave device types * 0x18 (24 decimal) RF24 *
                     * Others No other Slave device types have been *
                     * reverse-engineered * * Information reverse-engineered by
                     * B. Milhaupt and used with permission *
                     * **********************************************************************************
                     */
                    // Request for IPL-queryable devices of given manufacturer and type to report
                    // their presence
                    //
                    // Note that standard definitions are provided for UT4D and RF24, even though these
                    // devices do not respond to this query.  Note that UT4D will respond to IPL capable
                    // device query with DI_Hmf = 0, DI_Hst = 0, DI_Slv = 0, and DI_Smf = 0.
                    //
                    // Information reverse-engineered by B. Milhaupt and used with permission

                    device = getDeviceNameFromIPLInfo(l.getElement(4), l.getElement(5));
                    QString slave = getSlaveNameFromIPLInfo(l.getElement(4), l.getElement(6));
                    return tr("Discover %1 devices and/or {%2 devices.").arg(
                            device).arg(slave);
                }
                break;
            } // end case 0x08, which decodes 0xe5 0x14 0x0f 0x08
            case 0x10: {
                return interpretOpcPeerXfer20Sub10(l);
            } // end case 0x10, which decodes 0xe5 0x14 0x0f 0x10
            default: {
                break;
            }

        } // end of switch (l.getElement(3)), which decodes 0xe5 0x14 0x0f 0x??

        return "";
    }

    /*private*/ QString LlnMon::interpretOpcPeerXfer20(LocoNetMessage l) {
        // Duplex Radio Management
        // DigiIPL messages
        // LocoIO, LocoServo, LocoBuffer, LocoBooster configuration messages

        switch (l.getElement(2)) {
            case 0x01: {
                // Seems to be a query for just duplex devices.
                QString result = interpretOpcPeerXfer20_1(l);
                if (result.length() > 0) {
                    return result;
                }
                break;
            }
            case 0x02: {
                // Request Duplex Radio Channel
                QString result = interpretOpcPeerXfer20_2(l);
                if (result.length() > 0) {
                    return result;
                }
                break;
            }

            case 0x03: {
                // Duplex Group Name
                QString result = interpretOpcPeerXfer20_3(l);
                if (result.length() > 0) {
                    return result;
                }
                break;
            }
            case 0x04: {
                // Duplex Group ID
                QString result = interpretOpcPeerXfer20_4(l);
                if (result.length() > 0) {
                    return result;
                }
                break;
            }
            case 0x07: {
                // Duplex Group Password
                QString result = interpretOpcPeerXfer20_7(l);
                if (result.length() > 0) {
                    return result;
                }
                break;
            }
            case 0x10: {
                // Radio Channel Noise/Activity
                QString result = interpretOpcPeerXfer20_10(l);
                if (result.length() > 0) {
                    return result;
                }
                break;
            }

            case 0x08: //LnConstants::RE_IPL_PING_OPERATION:
        { // case 0x08, which decodes 0xe5 0x14 0x08
                QString result = interpretOpcPeerXfer20_8(l);
                if (result.length() > 0) {
                    return result;
                }
                break;
            }

            case 0x0f: //LnConstants::RE_IPL_IDENTITY_OPERATION:
        { // case 0x0f, which decodes 0xe5 0x14 0x0f
                // Operations related to DigiIPL "Ping", "Identify" and "Discover"
                QString result = interpretOpcPeerXfer20_0f(l);
                if (result.length() > 0) {
                    return result;
                }
                break;

            }

            default: {
                break;
            }
        }
        return "";
    }

    /*private*/ QString LlnMon::interpretOpcPeerXfer20Sub10(LocoNetMessage l) {
        /**
         * **********************************************************************************
         * IPL device identity report - RE_IPL_IDENTITY_OPERATION (Device
         * Report) * The message bytes are assigned as follows:
         * <p>
         * <E5> <14> <0F> <08> <DI_Hmf> <DI_Hst>
         * <DI_Slv> <DI_Smf> <DI_Hsw> ...
         * <p>
         * <DI_F1> <DI_Ssw> <DI_Hs0> <DI_Hs1>
         * <DI_Hs2> <DI_F2> <DI_Ss0> ...
         * <p>
         * <DI_Ss1> <DI_Ss2> <DI_Ss3> <CHK> * where:
         * <p>
         * <DI_Hmf> DigiIPL-capable Host device manufacturer number. This is not
         * * the same as an NMRA Manufacturer ID. * 0x00 Digitrax * Others No
         * other Host device manufacturer * numbers have been reverse- *
         * engineered
         * <p>
         * <DI_Hst> encodes the DigiIPL-capable Host device type as follows: *
         * When
         * <DI_Hmf> = 0x00 * 0x00 (0 decimal) No Host device type reported *
         * 0x04 (4 decimal) UT4D * 0x23 (35 decimal) PR3 * 0x2A (42 decimal)
         * DT402 (or DT402R or DT402D) * 0x33 (51 decimal) DCS51 * 0x5C (92
         * decimal) UR92 * Others No other Host device types have been *
         * reverse-engineered * When <DI_Hmf> is not 0x00 * All values Not
         * reverse-engineered
         * <p>
         * <DI_Slv> encodes the DigiIPL-capable Slave device type as follows: *
         * When
         * <DI_Smf> = 0x00 * 0x00 (0 decimal) Report for all Slave device types
         * * 0x18 (24 decimal) RF24 * Others No other Slave device types have
         * been * reverse-engineered
         * <p>
         * <DI_Smf> DigiIPL-capable Slave device manufacturer number. This is
         * not * the same as an NMRA Manufacturer ID. * 0x00 Digitrax * Others
         * No other Slave device manufacturer * numbers have been reverse- *
         * engineered
         * <p>
         * <DI_Hsw> encodes the DigiIPL-capable Host device firmware revision *
         * number as follows: * bit 7 always 0 * bits 6-3 Host device firmware
         * major revision number * bits 2-0 Host device firmware minor revision
         * number
         * <p>
         * <DI_F1> encodes additional bits for the Slave device firmware major *
         * revision number and for the Host device serial number. * bits 7-4
         * always 0000b * bit 3 Bit 23 of Host Device Serial Number * bit 2 Bit
         * 15 of Host Device Serial Number * bit 1 Bit 7 of Host Device Serial
         * Number * bit 0 bit 4 of Slave device firmware Major number
         * <p>
         * <DI_Ssw> encodes the DigiIPL-capable Slave device firmware revision *
         * number as follows: * bit 7 always 0 * bits 6-3 Host device firmware
         * major revision number * bits 6-3 4 least-significant bits of Slave
         * device firmware major * revision number (see also <DI_F1>[0]) * bits
         * 2-0 Slave device firmware minor revision number
         * <p>
         * <DI_Hs0> encodes 7 bits of the 24 bit Host device serial number: *
         * bit 7 always 0 * bits 6-3 Bits 6-0 of Host device serial number
         * <p>
         * <DI_Hs1> encodes 7 bits of the 24 bit Host device serial number: *
         * bit 7 always 0 * bits 6-3 Bits 14-9 of Host device serial number
         * <p>
         * <DI_Hs2> encodes 7 bits of the 24 bit Host device serial number: *
         * bit 7 always 0 * bits 6-3 Bits 22-16 of Host device serial number
         * <p>
         * <DI_F2> encodes additional bits for the Slave device serial number. *
         * bits 7-4 always 0000b * bit 3 Bit 31 of Slave Device Serial Number *
         * bit 2 Bit 23 of Slave Device Serial Number * bit 1 Bit 15 of Slave
         * Device Serial Number * bit 0 Bit 7 of Slave Device Serial Number
         * <p>
         * <DI_Ss0> encodes 7 bits of the 32 bit Slave device serial number: *
         * bit 7 always 0 * bits 6-3 Bits 6-0 of Slave device serial number
         * <p>
         * <DI_Ss1> encodes 7 bits of the 32 bit Slave device serial number: *
         * bit 7 always 0 * bits 6-3 Bits 14-9 of Slave device serial number
         * <p>
         * <DI_Ss2> encodes 7 bits of the 32 bit Slave device serial number: *
         * bit 7 always 0 * bits 6-3 Bits 22-16 of Slave device serial number
         * <p>
         * <DI_Ss3> encodes 7 bits of the 32 bit Slave device serial number: *
         * bit 7 always 0 * bits 6-3 Bits 30-24 of Slave device serial number *
         * * Information reverse-engineered by B. Milhaupt and used with
         * permission *
         * **********************************************************************************
         */
        // Request for one specific IPL-queryable device to return its identity information.
        // Expected response is of type <E5><14><10>...
        //
        // Note that standard definitions are provided for RF24, even though these
        // devices do not generate this report.
        //
        // Information reverse-engineered by B. Milhaupt and used with permission
        QString hostType = getDeviceNameFromIPLInfo(l.getElement(4), l.getElement(5));

        QString hostVer = ((l.getElement(8) & 0x78) >> 3) + "." + ((l.getElement(8) & 0x7));

        int hostSnInt = ((l.getElement(13) + (((l.getElement(9) & 0x8) == 8) ? 128 : 0)) * 256 * 256)
                + ((l.getElement(12) + (((l.getElement(9) & 0x4) == 4) ? 128 : 0)) * 256)
                + (l.getElement(11) + (((l.getElement(9) & 0x2) == 2) ? 128 : 0));
        QString hostSN = QString(hostSnInt,16);
        QString hostInfo = tr("Host: %1, S/N=%2, S/W Version=%3").arg(
                hostType).arg(hostSN).arg(hostVer);

        QString slaveType = getSlaveNameFromIPLInfo(l.getElement(4), l.getElement(6));
        QString slaveInfo;
        if (l.getElement(6) != 0) {
            QString slaveVer = (((l.getElement(10) & 0x78) >> 3) + ((l.getElement(9) & 1) << 4)) + "." + ((l.getElement(10) & 0x7));
            int slaveSnInt
                    = ((l.getElement(15) + (((l.getElement(14) & 0x1) == 1) ? 128 : 0)))
                    + ((l.getElement(16) + (((l.getElement(14) & 0x2) == 2) ? 128 : 0)) * 256)
                    + ((l.getElement(17) + (((l.getElement(14) & 0x4) == 4) ? 128 : 0)) * 256 * 256)
                    + ((l.getElement(18) + (((l.getElement(14) & 0x8) == 8) ? 128 : 0)) * 256 * 256 * 256);
            slaveInfo = tr("Slave: %1, S/N=%2, S/W Version=%3").arg(slaveType).arg(
                    slaveSnInt,16).arg(
                    slaveVer);
        } else {
            slaveInfo = tr("Slave: None");
        }
        return tr("IPL Identity report.\n%1\n%2.\n").arg(
                hostInfo).arg(
                slaveInfo);
    }

    /*private*/ QString LlnMon::interpretOpcPeerXfer16(LocoNetMessage l) {
        /*
         * SRC=7F is THROTTLE msg xfer
         *  ; <DSTL><DSTH> encode ID#,
         *  ; <0><0> is THROT B'CAST
         *  ; <PXCT1>=<0,XC2,XC1,XC0 - D4.7,D3.7,D2.7,D1.7>
         *  ; XC0-XC2=ADR type CODE-0=7 bit Peer
         * TO Peer adrs *
         *  ; 1=<D1>is SRC HI,<D2>is DST HI
         *  ; <PXCT2>=<0,XC5,XC4,XC3 - D8.7,D7.7,D6.7,D5.7>
         *  ; XC3-XC5=data type CODE- 0=ANSI TEXT string,
         *  ; balance RESERVED *
         * ****************************************************
         * SV programming format 1
         *
         * This is the message format as implemented by the certain
         * existing devices. New designs should not use this format. The
         * message bytes are assigned as follows:
         *   ; <0xE5> <0x10> <SRC> <DST> <0x01> <PXCT1>
         *   ; <D1> <D2> <D3> <D4> <PXCT2>
         *   ; <D5> <D6> <D7> <D8> <CHK>
         *
         * The upper nibble of PXCT1 must be 0,
         * and the upper nibble of PXCT2 must be 1. The meanings of the
         * remaining bytes are as defined in the LocoNet Personal
         * Edition specification.
         * *********************************************
         * SV programming format 2
         *
         * This is the recommended format for new designs.
         * The message bytes as assigned as follows: *
         *  ; <0xE5> <0x10> <SRC> <SV_CMD> <SV_TYPE> <SVX1>
         *  ; <DST_L> <DST_H> <SV_ADRL> <SV_ADRH> <SVX2>
         *  ; <D1> <D2> <D3> <D4> <CHK>
         *
         * The upper nibble of both SVX1 (PXCT1) and SVX2 (PXCT2) must be 1.
         */

        int src = l.getElement(2); // source of transfer
        int dst_l = l.getElement(3); // ls 7 bits of destination
        int dst_h = l.getElement(4); // ms 7 bits of destination
        int pxct1 = l.getElement(5);
        int pxct2 = l.getElement(10);

        QVector<int> d = l.getPeerXfrData();

        if ((src == 0x7F) && (dst_l == 0x7F) && (dst_h == 0x7F)
                && ((pxct1 & 0x70) == 0x40)) {
            // Download (firmware?) messages.
            int sub = pxct2 & 0x70;
            switch (sub) {
                case 0x00: // setup
                    return tr("Download setup message: manufacturer=%1, H/W version={%2, S/W version=%3, device=%4, options={%5.").arg(
                            l.getElement(6)).arg(
                            l.getElement(8)).arg(
                            l.getElement(9)).arg(
                            tr("0x%1").arg(
                                    StringUtil::twoHexFromInt(l.getElement(7)))).arg(
                            l.getElement(11));
                case 0x10: // set address
                    return tr("Download set address %1.").arg(
                            tr("0x%1").arg(
                                    StringUtil::twoHexFromInt(d[0])
                                    + StringUtil::twoHexFromInt(d[1])
                                    + StringUtil::twoHexFromInt(d[2])));
                case 0x20: // send data
                case 0x30: // verify
                    return tr((sub == 0x20) ? "Download send data %1 %2 %3 %4 %5 %6 %7 %8." : "Download verify request %1 %2 %3 %4 %5 %6 %7 %8.").arg(
                            tr("0x%1").arg(StringUtil::twoHexFromInt(d[0])),
                            tr("0x%1").arg(StringUtil::twoHexFromInt(d[1])),
                            tr("0x%1").arg(StringUtil::twoHexFromInt(d[2])),
                            tr("0x%1").arg(StringUtil::twoHexFromInt(d[3])),
                            tr("0x%1").arg(StringUtil::twoHexFromInt(d[4])),
                            tr("0x%1").arg(StringUtil::twoHexFromInt(d[5])),
                            tr("0x%1").arg(StringUtil::twoHexFromInt(d[6])),
                            tr("0x%1").arg(StringUtil::twoHexFromInt(d[7])));
                case 0x40: // end op
                    return tr("Download end operation.");
                default: // everything else isn't understood, go to default
                    break;
            }
        }

        if ((src == 0x7F) && (dst_l == 0x0) && (dst_h == 0x0)
                && ((pxct1 & 0x3) == 0x00) && ((pxct2 & 0x70) == 0x70)) {
            // throttle semaphore symbol message
            return tr("Throttle Semaphore Symbol Control: Loco %1, Semaphore body %2, Vertical arm %3, Diagonal arm %4, Horizontal arm %5; Any lit arms are %6.").arg(
                    ((d[0] * 128) + d[1])).arg(
                    tr(((d[2] & 0x10) == 0x10)
                            ? "LN_MSG_THROTTLE_SEMAPHORE_HELPER_LIT"
                            : "LN_MSG_THROTTLE_SEMAPHORE_HELPER_UNLIT")).arg(
                    tr(((d[2] & 0x08) == 0x08)
                            ? "LN_MSG_THROTTLE_SEMAPHORE_HELPER_LIT"
                            : "LN_MSG_THROTTLE_SEMAPHORE_HELPER_UNLIT")).arg(
                    tr(((d[2] & 0x04) == 0x04)
                            ? "LN_MSG_THROTTLE_SEMAPHORE_HELPER_LIT"
                            : "LN_MSG_THROTTLE_SEMAPHORE_HELPER_UNLIT")).arg(
                    tr(((d[2] & 0x02) == 0x02)
                            ? "LN_MSG_THROTTLE_SEMAPHORE_HELPER_LIT"
                            : "LN_MSG_THROTTLE_SEMAPHORE_HELPER_UNLIT")).arg(
                    tr(((d[2] & 0x01) == 0x01)
                            ? "LN_MSG_THROTTLE_SEMAPHORE_HELPER_BLINKING"
                            : "LN_MSG_THROTTLE_SEMAPHORE_HELPER_UNBLINKING")
            );
        }

        if ((src == 0x7F) && ((pxct1 & 0x70) == 0x00)) {

            if ((dst_l == 0x00) && (dst_h == 0x00)) {
                //char c[] = new char[]{0, 0, 0, 0, 0, 0, 0, 0};
             QVector<char> c = QVector<char>(8,0);
                c.replace(0, (char) d[0]);
                c.replace(1, (char) d[1]);
                c.replace(2, (char) d[2]);
                c.replace(3, (char) d[3]);
                c.replace(4, (char) d[4]);
                c.replace(5, (char) d[5]);
                c.replace(6, (char) d[6]);
                c.replace(7, (char) d[7]);
                return tr("Send Throttle Text Message to all throttles with message %1%2%3%4%5%6%7.").arg(                        c[0]).arg(c[1]).arg(c[2]).arg(c[3]).arg(c[4]).arg(c[5]).arg(c[6]).arg(c[7]);
            } else {
                return tr("Send Throttle Text Message to Throttle %9 with message %1%2%3%4%5%6%7.").arg(
                        (char) d[0]).arg((char) d[1]).arg(
                        (char) d[2]).arg((char) d[3]).arg(
                        (char) d[4]).arg((char) d[5]).arg(
                        (char) d[6]).arg((char) d[7]).arg(
                        convertToMixed(dst_l,dst_h));
            }
        }

        QString result = interpretSV1Message(l);
        if (result.length() > 0) {
            return result;
        }
        result = interpretSV0Message(l);
        if (result.length() > 0) {
            return result;
        }

        // check for a specific type - SV Programming messages format 2
        result = interpretSV2Message(l);
        if (result.length() > 0) {
            return result;
        }

        return "";
    }

    /*private*/ QString LlnMon::interpretSV1Message(LocoNetMessage l) {
        QVector<int> d = l.getPeerXfrData();
        if ((l.getElement(4) != 1)
                || ((l.getElement(5) & 0x70) != 0)
                || ((l.getElement(10) & 0x70) != 0x10)) {
            // is not an SV1 message
            return "";
        }
        if (l.getElement(2) == 0x50) {
            // Packets from the LocoBuffer
            QString dst_subaddrx = (l.getElement(4) != 0x01 ? "" : ((d[4] != 0) ? "/" + QString::number(d[4], 16) : ""));
            // LocoBuffer to LocoIO
            return QString("LocoBuffer => LocoIO@")
                    + ((l.getElement(3) == 0) ? "broadcast" : QString::number(l.getElement(3)) + dst_subaddrx)
                    + " "
                    + (d[0] == 2 ? "Query SV" + QString::number(d[1]) : QString("Write SV") + QString::number(d[1]) + "=0x" + QString::number(d[3], 16))
                    + ((d[2] != 0) ? QString(" Firmware rev ") + dotme(d[2]) : "") + ".\n";
        }
        return "";
    }

    /*private*/ QString LlnMon::interpretSV0Message(LocoNetMessage l) {
        int dst_h = l.getElement(4);
        int pxct1 = l.getElement(5);
        int pxct2 = l.getElement(10);
        if ((dst_h != 0x01) || ((pxct1 & 0xF0) != 0x00)
                || ((pxct2 & 0xF0) != 0x00)) {
            return "";
        }

        // (Jabour/Deloof LocoIO), SV Programming messages format 1
        int dst_l = l.getElement(3);
        QVector<int> d = l.getPeerXfrData();
        int src = l.getElement(2);

        QString src_subaddrx = ((d[4] != 0) ? "/" + QString::number(d[4], 16) : "");
        QString dst_subaddrx = ((d[4] != 0) ? "/" + QString::number(d[4], 16) : "");

        QString src_dev = ((src == 0x50) ? "Locobuffer" : QString("LocoIO@") + "0x" + QString::number(src,16) + src_subaddrx);
        QString dst_dev = ((dst_l == 0x50) ? "LocoBuffer "
                : ((dst_l == 0x0) ? "broadcast"
                        : QString("LocoIO@0x") + QString::number(dst_l,16) + dst_subaddrx));
        QString operation = (src == 0x50)
                ? ((d[0] == 2) ? "Query" : "Write")
                : ((d[0] == 2) ? "Report" : "Write");

        return src_dev + "=> " + dst_dev + " "
                + operation + " SV" + QString::number(d[1])
                + ((src == 0x50) ? (d[0] != 2 ? ("=0x" + QString::number(d[3], 16)) : "")
                        : " = " + ((d[0] == 2) ? ((d[2] != 0) ? (d[5] < 10) ? "" + d[5]
                                                : d[5] + " (0x" + QString::number(d[5], 16) + ")"
                                        : (d[7] < 10) ? "" + d[7]
                                                : d[7] + " (0x" + QString::number(d[7], 16) + ")")
                                : (d[7] < 10) ? "" + d[7]
                                        : d[7] + " (0x" + QString::number(d[7], 16) + ")"))
                + ((d[2] != 0) ? " Firmware rev " + dotme(d[2]) : "") + ".\n";
    }

    /*private*/ QString LlnMon::interpretSV2Message(LocoNetMessage l) {
        // (New Designs)
        QString svReply = "";
        LnSv2MessageContents* svmc = NULL;
        try {
            // assume the message is an SV2 message
            svmc = new LnSv2MessageContents(&l);
        } catch (IllegalArgumentException e) {
            // message is not an SV2 message.  Ignore the exception.
        }
        if (svmc != NULL) {
            // the message was indeed an SV2 message
            try {
                // get string representation of the message from an
                // available translation which is best suited to
                // the currently-active "locale"
                svReply = svmc->toString();
            } catch (IllegalArgumentException e) {
                // message is not a properly-formatted SV2 message.  Ignore the exception.
            }
        }
        return svReply;
    }

    /*private*/ QString LlnMon::interpretOpcPeerXfer10(LocoNetMessage l) {
        // throttle status
        int tcntrl = l.getElement(2);
        QString stat;
        switch (tcntrl) {
            case 0x40:
                stat = tr("(OK)");
                break;
            case 0x7F:
                stat = tr("(no key, immed, ignored)");
                break;
            case 0x43:
                stat = tr("(+ key during msg)");
                break;
            case 0x42:
                stat = tr("(- key during msg)");
                break;
            case 0x41:
                stat = tr("(R/S key during msg, aborts)");
                break;
            case 0x4e:
                stat = tr("(Throttle response to Semaphore Display Command)");
                break;
            default:
                stat = tr("(unknown)");
                break;
        }

        return tr("Throttle status TCNTRL=%1 %2, Throttle ID=%3, SLA=%4, SLB=%5.").arg(
                StringUtil::twoHexFromInt(tcntrl)).arg(
                stat).arg(
                idString(l.getElement(3), l.getElement(4))).arg(
                tr("0x%1").arg(
                        StringUtil::twoHexFromInt(l.getElement(7)))).arg(
                tr("0x%1").arg(
                        StringUtil::twoHexFromInt(l.getElement(8))));
    }

    /*private*/ QString LlnMon::interpretOpcPeerXfer9(LocoNetMessage l) {
        /*
         * Transponding "find" query and report messages.
         * Information reverse-engineered by B. Milhaupt and used with permission */
        switch (l.getElement(2)) {
            case 0x40: {
                /**
                 ).arg( **********************************************************************************
                 * Transponding "find" query message * The message bytes are
                 * assigned as follows:
                 * <p>
                 * <0xE5> <0x09> <0x40> <AD_H> <AD_L> <0x00>
                 * <0x00> <0x00> <CHK> * where:
                 * <p>
                 * <AD_H> is encoded as shown below: * When
                 * <AD_H> = 0x7D, * Address is a 7 bit value defined solely by
                 * <AD_L>. * When <AD_H> is not 0x7D, * Address is a 14 bit
                 * value; AD_H{6:0} represent the upper 7 bits * of the 14 bit
                 * address.
                 * <p>
                 * <AD_L> contains the least significant 7 bits of the 14 or 7
                 * bit address. * * Information reverse-engineered by B.
                 * Milhaupt and used with permission *
                 * **********************************************************************************
                 */
                QString locoAddr = convertToMixed(l.getElement(4), l.getElement(3));
                return tr("Transponding Find query for loco address %1.").arg(
                        locoAddr);
            }
            case 0x00: {
                /**
                 * **********************************************************************************
                 * Transponding "find" report message * The message bytes are
                 * assigned as follows:
                 * <p>
                 * <0xE5> <0x09> <0x00> <AD_H> <AD_L> <TR_ST>
                 * <TR_ZS> <0x00> <CHK> * where:
                 * <p>
                 * <AD_H> is encoded as shown below: * When
                 * <AD_H> = 0x7D, * Address is a 7 bit value defined solely by
                 * <AD_L>. * When <AD_H> is not 0x7D, * Address is a 14 bit
                 * value; AD_H{6:0} represent the upper 7 bits * of the 14 bit
                 * address.
                 * <p>
                 * <AD_L> contains the least significant 7 bits of the 14 or 7
                 * bit address.
                 * <p>
                 * <TR_ST> contains the transponding status for the addressed
                 * equipment, * encoded as: * bits 7-6 always 00b * bit 5
                 * encodes transponding presence * 0 = Addressed equipment is
                 * absent * 1 = Addressed equipment is present * bits 4-0 encode
                 * bits 7-3 of the Detection Section
                 * <p>
                 * <TR_ZS> contains the zone number and detection section,
                 * encoded as: * bit 7 always 0 * bits 6-4 encode bits 2-0 of
                 * the Detection Section * bits 3-1 encode the Transponding Zone
                 * as shown below * 000b Zone A * 001b Zone B * 010b Zone C *
                 * 011b Zone D * 100b Zone E * 101b Zone F * 110b Zone G * 111b
                 * Zone H * bit 0 always 0 * * Information reverse-engineered by
                 * B. Milhaupt and used with permission *
                 * **********************************************************************************
                 */

                int section = ((l.getElement(5) & 0x1F) << 3) + ((l.getElement(6) & 0x70) >> 4) + 1;
                QString zone;
                QString locoAddr = convertToMixed(l.getElement(4), l.getElement(3));

                switch (l.getElement(6) & 0x0F) {
                    case 0x00:
                        zone = "A";
                        break;
                    case 0x02:
                        zone = "B";
                        break;
                    case 0x04:
                        zone = "C";
                        break;
                    case 0x06:
                        zone = "D";
                        break;
                    case 0x08:
                        zone = "E";
                        break;
                    case 0x0A:
                        zone = "F";
                        break;
                    case 0x0C:
                        zone = "G";
                        break;
                    case 0x0E:
                        zone = "H";
                        break;
                    default:
                        zone = tr("unknown (%1)").arg(
                                l.getElement(6) & 0x0F);
                        break;
                }

                // get system and user names
                QString reporterSystemName = reporterPrefix
                        + ((l.getElement(5) & 0x1F) * 128 + l.getElement(6) + 1);

                Reporter* reporter = ((ProxyReporterManager*) InstanceManager::getDefault("ReporterManager"))->provideReporter(reporterSystemName);

                QString uname = reporter->getUserName();
                if ((uname != NULL) && (!uname.isEmpty())) {
                    return tr("Transponder Find report: address %1 present at %2 (%3) (BDL16x Board %4 RX4 zone %5).").arg(
                            locoAddr).arg(
                            reporterSystemName).arg(
                            uname).arg(
                            section).arg(
                            zone);
                }
                return tr("Transponder Find report: address %1 present at %2 (BDL16x Board %3 RX4 zone %4).").arg(
                        locoAddr).arg(
                        reporterSystemName).arg(
                        section).arg(
                        zone);
            }
            default: {
                break;
            }
        }
        return "";
    }

    /*private*/ QString LlnMon::interpretOpcPeerXfer7(LocoNetMessage l) {
        // This might be Uhlenbrock IB-COM start/stop programming track
        if (l.getElement(2) == 0x01 && l.getElement(3) == 0x49 && l.getElement(4) == 0x42) {
            switch (l.getElement(5)) {
                case 0x40: {
                    return tr("Uhlenbrock IB-COM / Intellibox II Stop Programming Track.");
                }
                case 0x41: {
                    return tr("Uhlenbrock IB-COM / Intellibox II Start Programming Track.");
                }
                default:
                    break;
            }
        }
        return "";
    }

    /*private*/ QString LlnMon::interpretOpcPeerXfer(LocoNetMessage l) {
        QString result = "";
        // The first byte seems to determine the type of message.
        switch (l.getElement(1)) {
            case 0x10: { //l.getZElement(1)
                result = interpretOpcPeerXfer16(l);
                if (result.length() > 0) {
                    return result;
                }
                break;
            }
            case 0x0A: {
                result = interpretOpcPeerXfer10(l);
                if (result.length() > 0) {
                    return result;
                }
                break;

            }
            case 0x14: {
                result = interpretOpcPeerXfer20(l);
                if (result.length() > 0) {
                    return result;
                }
                break;
            }
            case 0x09: { // l.getZElement(1)
                result = interpretOpcPeerXfer9(l);
                if (result.length() > 0) {
                    return result;
                }
                break;
            }
            case 0x07: {
                result = interpretOpcPeerXfer7(l);
                if (result.length() > 0) {
                    return result;
                }
                break;
            }
            default: {
                break;
            }
        }
        return "";

    }

    /*private*/ QString LlnMon::interpretLongAck(LocoNetMessage l) {
        int opcode = l.getElement(1);
        int ack1 = l.getElement(2);

        switch (opcode | 0x80) {
            case (LnConstants::OPC_LOCO_ADR):
                // response for OPC_LOCO_ADR
                return tr("LONG_ACK: NO FREE SLOTS!");

            case (LnConstants::OPC_LINK_SLOTS):
                // response for OPC_LINK_SLOTS
                return tr("LONG_ACK: Invalid consist, unable to link.");

            case (LnConstants::OPC_SW_ACK):
                // response for OPC_SW_ACK
                switch (ack1) {
                    case 0:
                        return tr("LONG_ACK: The Command Station FIFO is full, the switch command was rejected.");
                    case 0x7f:
                        return tr("LONG_ACK: The Command Station accepted the switch command.");
                    default:
                        forceHex = true;
                        return tr("LONG_ACK: Unknown response to 'Request Switch with ACK' command, value %1.").arg(
                                tr("0x%1").arg(
                                        StringUtil::twoHexFromInt(ack1)));
                }
            case (LnConstants::OPC_SW_REQ):
                // response for OPC_SW_REQ
                return tr("LONG_ACK: Switch request Failed!");

            case (LnConstants::OPC_WR_SL_DATA):
                // response for OPC_WR_SL_DATA
                switch (ack1) {
                    case 0:
                        return tr("LONG_ACK: The Slot Write command was rejected.");
                    case 0x01:
                        return tr("LONG_ACK: The Slot Write command was accepted.");
                    case 0x23:
                    case 0x2b:
                    case 0x6B:
                        return tr("LONG_ACK: DCS51 programming reply, thought to mean OK.");
                    case 0x40:
                        return tr("LONG_ACK: The Slot Write command was accepted blind (no response will be sent).");
                    case 0x7f:
                        return tr("LONG_ACK: Function not implemented, no reply will follow.");
                    default:
                        forceHex = true;
                        return tr("LONG_ACK: Unknown response to Write Slot Data message value %1.").arg(
                                tr("0x%1").arg(
                                        StringUtil::twoHexFromInt(ack1)));
                }

            case (LnConstants::OPC_SW_STATE):
                // response for OPC_SW_STATE
                return tr("LONG_ACK: Command station response to switch state request %1 (%2).").arg(
                        tr("0x%1").arg(
                                StringUtil::twoHexFromInt(ack1)),
                        tr((((ack1 & 0x20) != 0)
                                ? "Closed"
                                : "Thrown")));

            case (LnConstants::OPC_MOVE_SLOTS):
                // response for OPC_MOVE_SLOTS
                switch (ack1) {
                    case 0:
                        return tr("LONG_ACK: The Move Slots command was rejected.");
                    case 0x7f:
                        return tr("LONG_ACK: The Move Slots command was accepted.");
                    default:
                        forceHex = true;
                        return tr("LONG_ACK: Unknown response to Move Slots message %1.").arg(
                                tr("0x%1").arg(
                                        StringUtil::twoHexFromInt(ack1)));
                }

            case LnConstants::OPC_IMM_PACKET:
                // response for OPC_IMM_PACKET
                if (ack1 == 0) {
                    return tr("LONG_ACK: the Send IMM Packet command was rejected, the buffer is full/busy.");
                } else if (ack1 == 0x7f) {
                    return tr("LONG_ACK: the Send IMM Packet command was accepted.");
                } else if (l.getElement(1) == 0x6D && l.getElement(2) == 0x01) {
                    return tr("Long_ACK: Uhlenbrock IB-COM / Intellibox II CV programming request was accepted.");

                } else {
                    forceHex = true;
                    return tr("LONG_ACK: Unknown response to Send IMM Packet value %1.").arg(
                            tr("0x%1").arg(
                                    StringUtil::twoHexFromInt(ack1)));
                }

            case LnConstants::OPC_IMM_PACKET_2:
                // response for OPC_IMM_PACKET
                return tr("LONG_ACK: the Lim Master responded to the Send IMM Packet command with %1 (%2).").arg(
                        ack1).arg(tr("0x%1").arg(
                                StringUtil::twoHexFromInt(ack1)));

            case (/*LnConstants::RE_LACK_SPEC_CASE1*/0x50 | 0x80): // 0x50 plus opcode bit so can match the switch'd value:
            case (/*LnConstants::RE_LACK_SPEC_CASE2*/0x00 | 0x80): //0x00 plus opcode bit so can match the switch'd value:
        {
                // OpSwitch read response reverse-engineered by B. Milhaupt and
                // used with permission
                int responseValue = l.getElement(2);
                if (responseValue == 0x7f) {
                    return tr("LONG_ACK: OpSwitch operation accepted.");
                } else {
                    return tr("LONG_ACK: OpSwitch report - opSwitch is %1 (%2).").arg(
                            (((responseValue & 0x20) == 0x20) ? 1 : 0)).arg(
                            (((responseValue & 0x20) == 0x20)
                                    ? tr("Closed")
                                    : tr("Thrown")));
                }
        }
            case LnConstants::OPC_ALM_READ:
                if (l.getElement(2) == 0) {
                    return tr("LONG_ACK: opcode %1 does not support requested slot.").arg(
                            tr("0x%1").arg(
                                    StringUtil::twoHexFromInt(opcode)));
                }
                break;
            default:
                break;
        }
        return "";
    }

    /*private*/ QString LlnMon::interpretPm4xPowerEvent(LocoNetMessage l) {
        int pCMD = (l.getElement(3) & 0xF0);

        if ((pCMD == 0x30) || (pCMD == 0x10)) {
            // autoreverse
            int cm1 = l.getElement(3);
            int cm2 = l.getElement(4);
            QString sect1Mode, sect1State;
            QString sect2Mode, sect2State;
            QString sect3Mode, sect3State;
            QString sect4Mode, sect4State;

            if ((cm1 & 1) != 0) {
                sect1Mode = tr("Auto-Reversing mode -");
                sect1State = ((cm2 & 1) != 0)
                        ? tr("Reversed")
                        : tr("Normal");
            } else {
                sect1Mode = tr("Circuit-Breaker mode -");
                sect1State = ((cm2 & 1) != 0)
                        ? tr("Shorted")
                        : tr("Unshorted");
            }

            if ((cm1 & 2) != 0) {
                sect2Mode = tr("Auto-Reversing mode -");
                sect2State = ((cm2 & 2) != 0)
                        ? tr("Reversed")
                        : tr("Normal");
            } else {
                sect2Mode = tr("Circuit-Breaker mode -");
                sect2State = ((cm2 & 2) != 0)
                        ? tr("Shorted")
                        : tr("Unshorted");
            }

            if ((cm1 & 4) != 0) {
                sect3Mode = tr("Auto-Reversing mode -");
                sect3State = ((cm2 & 4) != 0)
                        ? tr("Reversed")
                        : tr("Normal");
            } else {
                sect3Mode = tr("Circuit-Breaker mode -");
                sect3State = ((cm2 & 4) != 0)
                        ? tr("Shorted")
                        : tr("Unshorted");
            }

            if ((cm1 & 8) != 0) {
                sect4Mode = tr("Auto-Reversing mode -");
                sect4State = ((cm2 & 8) != 0)
                        ? tr("Reversed")
                        : tr("Normal");
            } else {
                sect4Mode = tr("Circuit-Breaker mode -");
                sect4State = ((cm2 & 8) != 0)
                        ? tr("Shorted")
                        : tr("Unshorted");
            }
            return tr("PM4x (Board ID %1) Power Status Report\nSub-District 1 - %2 %3.\nSub-District 2 - %4 %5.\nSub-District 3 - %6 %7.\nSub-District 4 - %8 %9.").arg(
                    (l.getElement(2) + 1) + ((l.getElement(1) & 0x1) << 7)).arg(
                    sect1Mode).arg( sect1State).arg( sect2Mode).arg( sect2State).arg(
                    sect3Mode).arg( sect3State).arg( sect4Mode).arg( sect4State);
        }
        return "";
    }

    /*private*/ QString LlnMon::interpretOpSws(LocoNetMessage l) {
        int pCMD = (l.getElement(3) & 0xF0);
        if (pCMD == 0x70) {
            // programming
            int deviceType = l.getElement(3) & 0x7;
            QString device;
            switch (deviceType) {
                case LnConstants::RE_MULTI_SENSE_DEV_TYPE_PM4X:
                    device = tr("PM4x");
                    break;
                case LnConstants::RE_MULTI_SENSE_DEV_TYPE_BDL16X:
                    device = tr("BDL16x");
                    break;
                case LnConstants::RE_MULTI_SENSE_DEV_TYPE_SE8:
                    device = tr("SE8C");
                    break;
                case LnConstants::RE_MULTI_SENSE_DEV_TYPE_DS64:
                    device = tr("DS64");
                    break;
                default:
                    return "";
            }

            int val = (l.getElement(4) & 0x01);
            int opsw = (l.getElement(4) & 0x7E) / 2 + 1;
            int bdaddr = l.getElement(2) + 1;
            if ((l.getElement(1) & 0x1) != 0) {
                bdaddr += 128;
            }

            if ((deviceType == 0) && (bdaddr == 1) && (l.getElement(4) == 0)) {
                return tr("PM4x 1 Query OpSw1 - Also acts as device query for some device types.");
            }

            if ((l.getElement(1) & 0x10) != 0) {
                // write
                QString valType = (val == 1)
                        ? tr("Closed")
                        : tr("Thrown");
                return tr("%1 %2 Write OpSw%3 value=%4 (%5).").arg(
                        device).arg( bdaddr).arg( opsw).arg( val).arg( valType);
            } else {
                // query
                return tr("%1 %2 Query OpSw%3.").arg(
                        device).arg( bdaddr).arg( opsw);
            }
        }
        return "";
    }

    /*private*/ QString LlnMon::interpretDeviceType(LocoNetMessage l) {
        int pCMD = (l.getElement(3) & 0xF0);
        if (pCMD == 0x00) {
            /**
             * **************************************************
             * Device type report * The message bytes as assigned as follows:
             * <p>
             * <0xD0> <DQT_REQ> <DQT_BRD> <DQT_B3> <DQT_B4>
             * <CHK> * * where:
             * <p>
             * <DQT_REQ> contains the device query request, * encoded as: * bits
             * 7-4 always 0110b * bits 3-1 always 001b * bit 0 (BoardID-1)<7>
             * <p>
             * <DQT_BRD> contains most the device board ID number, * encoded as:
             * * bit 7 always 0b * bits 6-0 (BoardID-1)<6:0>
             * <p>
             * <DQT_B3> contains the board type identification, * encoded as: *
             * bits 7-4 always 0000b * bits 3-0 contain the encoded device type,
             * * encoded as: * 0000b PM4x device * 0001b BDL16x device * 0010b
             * SE8C device * 0011b DS64 device * others Unknown device type
             * <p>
             * <DQT_B4> contains device version number: * bit 7 always 0b * bits
             * 6-0 VersionNumber(6:0) * * Information reverse-engineered by B.
             * Milhaupt and used with permission *
             * **************************************************
             */
            // This message is a report which is sent by a LocoNet device
            // in response to a query of attached devices
            // Note - this scheme is supported by only some Digitrax devices.
            //
            // A VersionNumber of 0 implies the hardware does not report
            // a valid version number.
            //
            // Device type report reverse-engineered by B. Milhaupt and
            // used with permission
            int deviceType = l.getElement(3) & 0x7;
            QString device = tr("Unknown device type");
            switch (deviceType) {
                case LnConstants::RE_MULTI_SENSE_DEV_TYPE_PM4X:
                    device = tr("PM4x");
                    break;
                case LnConstants::RE_MULTI_SENSE_DEV_TYPE_BDL16X:
                    device = tr("BDL16x");
                    break;
                case LnConstants::RE_MULTI_SENSE_DEV_TYPE_SE8:
                    device = tr("SE8C");
                    break;
                case LnConstants::RE_MULTI_SENSE_DEV_TYPE_DS64:
                    device = tr("DS64");
                    break;
                default:
                    log->warn(tr("Unhandled device type: %1").arg(deviceType));
                    break;
            }

            int bdaddr = l.getElement(2) + 1;
            if ((l.getElement(1) & 0x1) != 0) {
                bdaddr += 128;
            }
            QString versionNumber = QString::number(l.getElement(4));
            if (l.getElement(4) == 0) {
                versionNumber = tr("(unknown)");
            }
            return tr("Device type report - %1 Board ID %2 Version %3 is present.").arg(
                    device).arg( bdaddr).arg( versionNumber);
        }
        return "";
    }

    /*private*/ QString LlnMon::interpretOpcMultiSense(LocoNetMessage l) {
        int type = l.getElement(1) & LnConstants::OPC_MULTI_SENSE_MSG;
        QString result;
        switch (type) {
            case LnConstants::OPC_MULTI_SENSE_POWER:
                // This is a PM42 power event.
                result = interpretPm4xPowerEvent(l);
                if (result.length() > 0) {
                    return result;
                }
                result = interpretOpSws(l);
                if (result.length() > 0) {
                    return result;
                }
                result = interpretDeviceType(l);
                if (result.length() > 0) {
                    return result;
                } else {
                    break;
                }

            case LnConstants::OPC_MULTI_SENSE_PRESENT:
            case LnConstants::OPC_MULTI_SENSE_ABSENT:
                result = interpretOpcMultiSenseTranspPresence(l);
                if (result.length() > 0) {
                    return result;
                }
                break;
            default:
                break;
        }
        return "";
    }

    /*private*/ QString LlnMon::interpretOpcMultiSenseTranspPresence(LocoNetMessage l) {
        // Transponding Event
        // get system and user names
        QString reporterSystemName;
        QString reporterUserName;
        QString zone;
        int bxp88Zone = 1 + (l.getElement(2) & 0x07);
        switch (l.getElement(2) & 0x0f) { // ignore bit 0 which seems to provide some unknown info from the BXP88
            case 0x00:
                zone = tr("A");
                break;
            case 0x02:
                zone = tr("B");
                break;
            case 0x04:
                zone = tr("C");
                break;
            case 0x06:
                zone = tr("D");
                break;
            case 0x08:
                zone = tr("E");
                break;
            case 0x0A:
                zone = tr("F");
                break;
            case 0x0C:
                zone = tr("G");
                break;
            case 0x0E:
                zone = tr("H");
                break;
            default:
                zone = tr("(Unknown %1)").arg(
                        (l.getElement(2) & 0x0F));
                break;
        }
        int type = l.getElement(1) & LnConstants::OPC_MULTI_SENSE_MSG;

        reporterSystemName = reporterPrefix
                + ((l.getElement(1) & 0x1F) * 128 + l.getElement(2) + 1);

                Reporter* reporter = ((ProxyReporterManager*) InstanceManager::getDefault("ReporterManager"))->provideReporter(reporterSystemName);
        reporterUserName = "";
        QString uname = reporter->getUserName();
        if ((uname != NULL) && (!uname.isEmpty())) {
            reporterUserName = uname;
        }
        int bxpa1Number = 1 + l.getElement(2) + (l.getElement(1) & 0x1F) * 128;
        int bxp88Number = 1 + (l.getElement(2)/8) + (l.getElement(1) & 0x1F) * 16;
        int section = 1 + (l.getElement(2) / 16) + (l.getElement(1) & 0x1F) * 8;

        QString locoAddr = convertToMixed(l.getElement(4), l.getElement(3));
        QString transpActivity = (type == LnConstants::OPC_MULTI_SENSE_PRESENT)
                ? tr("present at")
                : tr("absent at");

        if ((l.getElement(2) & 0x1) == 0) {
            return tr("Transponder address %1 %2 %3 (%4) (BDL16x Board ID %5 RX4 zone %6 or BXP88 Board ID %7 section %8 or the BXPA1 Board ID %9 section).").arg(
                    locoAddr).arg(transpActivity).arg(reporterSystemName).arg(
                    reporterUserName).arg(section).arg(zone).arg(bxp88Number).arg(bxp88Zone).arg(bxpa1Number);
        } else {
            return tr("Transponder address %1 %2 %3 (%4) (BXP88 Board ID %5 section %6 or the BXPA1 Board ID %7 section).").arg(
                    locoAddr).arg(transpActivity).arg(reporterSystemName).arg(
                    reporterUserName).arg(bxp88Number).arg(bxp88Zone).arg(bxpa1Number);
        }
    }

    /*private*/ QString LlnMon::interpretOpcWrSlDataOpcSlRdData(LocoNetMessage l) {
        int slot = l.getElement(2); // slot number for this request
        QString mode;
        int command = l.getElement(0);
        int id1 = l.getElement(11); // ls 7 bits of ID code
        int id2 = l.getElement(12); // ms 7 bits of ID code
        /*
         * These messages share a common data format with the only difference being
         * whether we are reading or writing the slot data.
         */
        if (command == LnConstants::OPC_WR_SL_DATA) {
            mode = tr("Request");
        } else {
            mode = tr("Response");
        }

        QString result;

        switch (slot) {
            case LnConstants::FC_SLOT:
                result = interpretFastClockSlot(l, mode, id1, id2);
                if (result.length() > 0) {
                    return result;
                }
                break;
            case LnConstants::PRG_SLOT:
                result = interpretProgSlot(l, mode, id1, id2, command);
                if (result.length() > 0) {
                    return result;
                }
                break;

            case 0x79:
            case 0x7a:
            case 0x7D:
                return "";
            case LnConstants::CFG_EXT_SLOT:
                result = interpretCmdStnExtCfgSlotRdWr(l, command);
                if (result.length() > 0) {
                    return result;
                }
                break;

            // end programming track block
            case LnConstants::CFG_SLOT:
                result = interpretCmdStnCfgSlotRdWr(l, command);
                if (result.length() > 0) {
                    return result;
                }
                break;

            default:
                result = interpretStandardSlotRdWr(l, id1, id2, command, slot);
                if (result.length() > 0) {
                    return result;
                }
                break;
        }

        return "";
    }

    /*private*/ QString LlnMon::interpretOpcInputRep(LocoNetMessage l) {
        int in1 = l.getElement(1);
        int in2 = l.getElement(2);
        int contactNum = ((SENSOR_ADR(in1, in2) - 1) * 2 + ((in2 & LnConstants::OPC_INPUT_REP_SW) != 0 ? 2 : 1));
        // get system and user names
        QString sensorSystemName = sensorPrefix + QString::number(contactNum);
        QString sensorUserName = "";
        Sensor* sensor = ((ProxySensorManager*) InstanceManager::getDefault("SensorManager"))->provideSensor(sensorSystemName);
        sensorUserName = "";
        QString uname = sensor->getUserName();
        if ((uname != NULL) && (!uname.isEmpty())) {
            sensorUserName = uname;
        }
        int sensorid = (SENSOR_ADR(in1, in2) - 1) * 2
                + ((in2 & LnConstants::OPC_INPUT_REP_SW) != 0 ? 2 : 1);

        int bdlid = ((sensorid - 1) / 16) + 1;
        int bdlin = ((sensorid - 1) % 16) + 1;
        QString bdl = tr("BDL16 # %1, DS%2").arg(
                bdlid).arg(bdlin);

        int boardid = ((sensorid - 1) / 8) + 1;
        int boardindex = ((sensorid - 1) % 8);
        QString otherBoardsNames;
        QString otherBoardsInputs;
        if (sensorid < 289) {
            otherBoardsNames = tr("DS54/DS64/SE8c # %1").arg( boardid);
            otherBoardsInputs = tr("%1/%2/%3").arg(
                    ds54sensors[boardindex]).arg( ds64sensors[boardindex]).arg(
                    se8csensors[boardindex]);
        } else {
            otherBoardsNames = tr("DS54/DS64 # %1").arg( boardid);
            otherBoardsInputs = tr("%1/%2").arg(
                    ds54sensors[boardindex]).arg( ds64sensors[boardindex]);
        }

        // There is no way to tell what kind of a board sent the message.
        // To be user friendly, we just print all the known combos.
        return tr("Sensor %1 (%2) is %3.  (%4; %5, %6).").arg(
                sensorSystemName, sensorUserName).arg(
                tr((in2 & LnConstants::OPC_INPUT_REP_HI) != 0
                        ? "LN_MSG_SENSOR_STATE_HIGH" : "LN_MSG_SENSOR_STATE_LOW")).arg(
                bdl).arg(
                otherBoardsNames, otherBoardsInputs);
    }

    /*private*/ QString LlnMon::interpretOpcSwRep(LocoNetMessage l) {
        int sn1 = l.getElement(1);
        int sn2 = l.getElement(2);
        // get system and user names
        QString turnoutUserName = "";

        QString turnoutSystemName = turnoutPrefix
                + QString::number(SENSOR_ADR(sn1, sn2));
        Turnout* turnout = (Turnout*)((ProxyTurnoutManager*) InstanceManager::getDefault("TurnoutManager"))->provideTurnout(turnoutSystemName);

        QString uname = turnout->getUserName();
        if ((uname != NULL) && (!uname.isEmpty())) {
            turnoutUserName = uname;
        } else {
            turnoutUserName = "";
        }

        if ((sn2 & LnConstants::OPC_SW_REP_INPUTS) != 0) {
            return tr("Turnout %1 (%2) %3 is %4").arg(
                    turnoutSystemName).arg(turnoutUserName).arg(
                    tr(((sn2 & LnConstants::OPC_SW_REP_SW) != 0
                            ? "Switch input"
                            : "Aux input"))).arg(
                    tr((((sn2 & LnConstants::OPC_SW_REP_HI) != 0)
                            ? "Closed (input off)"
                            : "Thrown (input on)")));
        }
        return tr("Turnout %1 (%2) output state: Closed output is %3, Thrown output is %4.").arg(
                turnoutSystemName).arg(turnoutUserName).arg(
                ((((sn2 & LnConstants::OPC_SW_REP_CLOSED) != 0)
                        ? "On (sink)"
                        : "Off (open)"))).arg(
                ((((sn2 & LnConstants::OPC_SW_REP_THROWN) != 0)
                        ? "On (sink)"
                        : "Off (open)")));
    }

    /*private*/ QString LlnMon::interpretOpcSwAck(LocoNetMessage l) {
        int sw2 = l.getElement(2);
        if ((sw2 & 0x40) == 0x40) {
            return "";
        }
        // get system and user names
        QString turnoutUserName = "";

        QString turnoutSystemName = turnoutPrefix
                + QString::number(SENSOR_ADR(l.getElement(1), l.getElement(2)));
        Turnout* turnout = (Turnout*)((ProxyTurnoutManager*) InstanceManager::getDefault("TurnoutManager"))->provideTurnout(turnoutSystemName);

        QString uname = turnout->getUserName();
        if ((uname != NULL) && (!uname.isEmpty())) {
            turnoutUserName = uname;
        } else {
            turnoutUserName = "";
        }

        QString pointsDirection = ((sw2 & LnConstants::OPC_SW_ACK_CLOSED) != 0)
                ? QString("Closed")
                : QString("Thrown");
        QString outputState = (((sw2 & LnConstants::OPC_SW_ACK_OUTPUT) != 0)
                ? tr("On (sink)")
                : tr("Off (open)"));
        return tr("Requesting Switch at %1 (%2) to %3 (%4), with acknowledgment.").arg(turnoutSystemName).arg(
                turnoutUserName).arg(pointsDirection).arg(outputState);
    }

    /*private*/ QString LlnMon::interpretOpcSwState(LocoNetMessage l) {
        // get system and user names
        if ((l.getElement(2) & 0x40) != 0x00) {
            return "";
        }
        QString turnoutUserName = "";
        QString turnoutSystemName = turnoutPrefix
                + QString::number(SENSOR_ADR(l.getElement(1), l.getElement(2)));
        Turnout* turnout = (Turnout*)((ProxyTurnoutManager*) InstanceManager::getDefault("TurnoutManager"))->provideTurnout(turnoutSystemName);

        QString uname = turnout->getUserName();
        if ((uname != NULL) && (!uname.isEmpty())) {
            turnoutUserName = uname;
        } else {
            turnoutUserName = "";
        }

        return tr("Request status of switch %1 (%2)").arg(turnoutSystemName).arg(
                turnoutUserName);
    }

    /*private*/ QString LlnMon::interpretOpcRqSlData(LocoNetMessage l) {
        int slot = l.getElement(1) + 128 * l.getElement(2);

        switch (slot) {
            // Slots > 120 are all special, but these are the only ones we know to decode.
            case LnConstants::FC_SLOT:
                return tr("Request Fast Clock information.");
            case LnConstants::CFG_SLOT:
                return tr("Request Command Station OpSwitches (or DCS210/DCS240 check for multiple command stations on LocoNet).");
            case LnConstants::CFG_EXT_SLOT:
                return tr("Request Extended Command Station OpSwitches (DCS210/DCS240 only)");
            case LnConstants::PRG_SLOT:
                return tr("Request Programming Track information.");
            case 0x79:
            case 0x7a:
            case 0x7d:
                break;
            default:
                return tr("Request data/status for slot %1.").arg(slot);
        }
        return "";
    }

    /*private*/ QString LlnMon::interpretOpcMoveSlots(LocoNetMessage l) {
        int src = l.getElement(1);
        int dest = l.getElement(2);
        if ((src >= 0x79) && (src <= 0x7f)) {
            return "";
        }
        if ((dest >= 0x79) && (dest <= 0x7f)) {
            return "";
        }

        /* check special cases */
        if (src == 0) {
            /* DISPATCH GET */

            return tr("Get most recently dispatched slot.");
        } else if (src == dest) {
            /* IN USE */

            return tr("Set status of slot %1 to IN_USE.").arg(src);
        } else if (dest == 0) {
            /* DISPATCH PUT */

            return tr("Mark slot %1 as DISPATCHED.").arg(src);
        } else {
            /* general move */

            return tr("Move data in slot %1 to slot %2.").arg(src, dest);
        }
    }

    /*private*/ QString LlnMon::interpretOpcConsistFunc(LocoNetMessage l) {
        int slot = l.getElement(1);
        int dirf = l.getElement(2);
        if ((dirf & 0x40) == 0x40) {
            return "";
        }
        return tr("Set consist in slot %1 %2.").arg(
                slot).arg(
                interpretDIRF(dirf));
    }

    /*private*/ QString LlnMon::interpretOpcLocoSnd(LocoNetMessage l) {
        int slot = l.getElement(1);
        int snd = l.getElement(2);
        return tr("Set loco in slot %1 Sound1/F5=%2 Sound2/F6=%3 Sound3/F7=%4 Sound4/F8=%5.").arg(
                slot).arg(
                tr((snd & LnConstants::SND_F5) != 0
                        ? "On" : "Off")).arg(
                tr((snd & LnConstants::SND_F6) != 0
                        ? "On" : "Off")).arg(
                tr((snd & LnConstants::SND_F7) != 0
                        ? "On" : "Off")).arg(
                tr((snd & LnConstants::SND_F8) != 0
                        ? "On" : "Off"));

    }

    /*protected*/ QString LlnMon::interpretDIRF(int dirf) {
        if ((dirf & 0x40) == 0x40) {
            return "";
        }
        QVector<QString> dirf0_4 = interpretF0_F4toStrings(dirf);
        return tr("direction %1 F0=%2 F1=%3 F2=%4 F3=%5 F4=%6").arg(
                tr((dirf & LnConstants::DIRF_DIR) != 0
                        ? "Reverse" : "Forward")).arg(
                dirf0_4[0]).arg( dirf0_4[1]).arg( dirf0_4[2]).arg( dirf0_4[3]).arg( dirf0_4[4]);

    }

    /*private*/ QString LlnMon::interpretOpcLocoDirf(LocoNetMessage l) {
        int slot = l.getElement(1);
        int dirf = l.getElement(2);

        QString dirFinfo = interpretDIRF(dirf);
        if (dirFinfo.length() == 0) {
            return "";
        }

        return tr("Set loco in slot %1 %2.").arg(
                slot).arg(dirFinfo);
    }

    /*private*/ QString LlnMon::interpretOpcLocoSpd(LocoNetMessage l) {
        int slot = l.getElement(1);
        int spd = l.getElement(2);

        if (spd == LnConstants::OPC_LOCO_SPD_ESTOP) {
            return tr("Set speed of loco in slot %1 to EMERGENCY STOP!").arg(slot);
        } else {
            return tr("Set speed of loco in slot %1 to %2.").arg(slot).arg(spd);
        }

    }

    /*private*/ QString LlnMon::interpretOpcPanelQuery(LocoNetMessage l) {
        switch (l.getElement(1)) {
            case 0x00: {
                return tr("LN_MSG_OPC_DF_TETHERLESS_QUERY");
            }
            case 0x40: {
                if (l.getElement(2) == 0x1F) {
                    // Some UR devices treat this operation as a set plus query, others
                    // treat this only as a set.
                    return tr("Set LocoNet ID to %1.").arg(l.getElement(3));
                }
                break;
            }
            default: {
                break;
            }
        }
        return "";
    }

/*private*/ QString LlnMon::interpretOpcSwReq(LocoNetMessage l)
{
     int sw1 = l.getElement(1);
     int sw2 = l.getElement(2);
     if ((sw2 & 0x40) == 0x40) {
         return "";
     }

     if ((!(((sw2 & 0xCF) == 0x0F) && ((sw1 & 0xFC) == 0x78)))
             && (!(((sw2 & 0xCF) == 0x07) && ((sw1 & 0xFC) == 0x78))))
     {
      // ordinary form, LPU V1.0 page 9
      // handle cases which are not "stationary decoder interrogate" messages
      // get system and user names
      QString turnoutUserName = "";

      QString turnoutSystemName = turnoutPrefix
              + QString::number(SENSOR_ADR(l.getElement(1), l.getElement(2)));
      Turnout* turnout = (Turnout*)((ProxyTurnoutManager*) InstanceManager::getDefault("TurnoutManager"))->provideTurnout(turnoutSystemName);

      QString uname = turnout->getUserName();
      if ((uname != NULL) && (!uname.isEmpty())) {
          turnoutUserName = uname;
      } else {
          turnoutUserName = "";
      }

      QString pointsDirection = ((sw2 & LnConstants::OPC_SW_ACK_CLOSED) != 0
              ? tr("Closed")
              : tr("Thrown"));
      QString outputState = ((sw2 & LnConstants::OPC_SW_ACK_OUTPUT) != 0
              ? tr("Output On")
              : tr("Output Off"));
      if (turnoutUserName.length() == 0) {
          return tr("Requesting Switch at %1 to %2 (%3).").arg(
                  turnoutSystemName).arg(
                  pointsDirection, outputState);
      } else {
          return tr("Requesting Switch at %1 (%2) to %3 (%4).").arg(
                  turnoutSystemName).arg(turnoutUserName).arg(
                  pointsDirection).arg( outputState);
      }
     }

     /*
     Handle cases which are "stationary decoder interrogate" messages.
      */

     /*
      * Decodes a/c/b bits to allow proper creation of a list of addresses
      * which ought to reply to the "stationary decoder interrogate" message.
      */
     int a = (sw2 & 0x20) >> 5;
     int c = (sw1 & 0x02) >> 1;
     int b = (sw1 & 0x01);

     /*
      * All this blob does is loop through the ranges indicated by the
      * a/c/b bits, they are mask bits in the midde of the range. The
      * idea is to get 8 sensors at a time, since that is generally what
      * units have, and to query units 1, 9, 17... then 2, 10, 18... and
      * so on such that if they are all in a row they don't get hit at
      * the same time.
      */
     int topbits = 0;
     int midbits = (a << 2) + (c << 1) + b;
     int count = 0;
     QString addrListB; // = new StringBuilder();
     for (topbits = 0; topbits < 32; topbits++)
     {
      // The extra "+1" adjusts for the fact that we show 1-2048,
      // rather than 0-2047 on the wire.
      int lval = (topbits << 6) + (midbits << 3) + 1;
      int hval = lval + 7;

      if ((count % 8) != 0)
      {
          addrListB.append(", "); // NOI18N
      }
      else
      {
       if (count == 0) {
           addrListB.append("\t"); // NOI18N
       } else {
           addrListB.append(",\n\t");  // NOI18N
       }
      }
      addrListB.append("").append(QString::number(lval));  // NOI18N
      addrListB.append("-").append(QString::number(hval)); // NOI18N
      count++;
     }

     QString addrList = addrListB;

     if (((sw2 & 0xCF) == 0x0F) && ((sw1 & 0xFC) == 0x78)) {
         // broadcast address LPU V1.0 page 12
         return tr("Interrogate Stationary Decoders with bits a/c/b of %1/%2/%3; turnouts...\n%4.").arg(a).arg(c).arg(b).arg(addrList);
     } else {
         // broadcast address LPU V1.0 page 13
         return tr("Interrogate LocoNet Turnouts/Sensors with bits a/c/b of %1/%2/%3; addresses...\n%4.").arg(
                 a).arg(c).arg(b).arg(addrList);
     }
    }

    /*private*/ QString LlnMon::interpretFastClockSlot(LocoNetMessage l, QString mode, int id1, int id2) {
        /**
         * FAST Clock: The system FAST clock and parameters are implemented in
         * Slot#123 <7B>. Use <EF> to write new clock information, Slot read of
         * 0x7B,<BB><7B>.., will return current System clock information, and
         * other throttles will update to this SYNC. Note that all attached
         * display devices keep a current clock calculation based on this SYNC
         * read value, i.e. devices MUST not continuously poll the clock SLOT to
         * generate time, but use this merely to restore SYNC and follow current
         * RATE etc. This clock slot is typically "pinged" * or read SYNC'd
         * every 70 to 100 seconds, by a single user, so all attached devices
         * can synchronise any phase drifts. Upon seeing a SYNC read, all
         * devices should reset their local sub-minute phase counter and
         * invalidate the SYNC update ping generator.
         * <p>
         * Clock Slot Format:
         * <p>
         * <0xEF>,<0E>,<7B>,<CLK_RATE>,<FRAC_MINSL>,<FRAC_MINSH>,<256-MINS_60>,
         * <TRK><256-HRS_24>,<DAYS>,<CLK_CNTRL>,<ID1>,<1D2>,<CHK>
         * <p>
         * where:
         * <p>
         * <CLK_RATE> 0=Freeze clock, * 1=normal 1:1 rate, 10=10:1 etc, max
         * VALUE is 7F/128 to 1
         * <p>
         * <FRAC_MINSL> FRAC mins hi/lo are a sub-minute counter, depending on
         * the CLOCK generator
         * <p>
         * <FRAC_MINSH> Not for ext. usage. This counter is reset when valid
         * <E6><7B>
         * SYNC message is seen
         * <p>
         * <256-MINS_60> This is FAST clock MINUTES subtracted from 256. Modulo
         * 0-59
         * <p>
         * <256-HRS_24> This is FAST clock HOURS subtracted from 256. Modulo
         * 0-23
         * <p>
         * <DAYS> number of 24 Hr clock rolls, positive count
         * <p>
         * <CLK_CNTRL> Clock Control Byte D6- 1=This is valid Clock information,
         * 0=ignore this <E6><7B>, SYNC reply
         * <p>
         * <ID1>,<1D2> This is device ID last setting the clock.
         * <p>
         * <00><00> shows no set has happened
         * <p>
         * <7F><7x> are reserved for PC access *
         */

        int minutes; // temporary time values
        int hours;
        int clk_rate = l.getElement(3); // 0 = Freeze clock, 1 = normal,
        // 10 = 10:1 etc. Max is 0x7f
        int mins_60 = l.getElement(6); // 256 - minutes
        int track_stat = l.getElement(7); // track status
        int hours_24 = l.getElement(8); // 256 - hours
        int days = l.getElement(9); // clock rollovers
        int clk_cntrl = l.getElement(10); // bit 6 = 1; data is valid
        // clock info
        // "  " 0; ignore this reply
        // id1/id2 is device id of last device to set the clock
        // "   " = zero shows not set has happened

        /* recover hours and minutes values */
        minutes = ((255 - mins_60) & 0x7f) % 60;
        hours = ((256 - hours_24) & 0x7f) % 24;
        hours = (24 - hours) % 24;
        minutes = (60 - minutes) % 60;

        return tr("%1 Fast Clock is %2 %3, rate is %4:1. Day %5, %6. Last set by ID %7.\n%8.").arg(
                mode).arg(
                ((clk_cntrl & 0x20) != 0 ? "" : tr("Synchronized,"))).arg(
                (clk_rate != 0 ? tr("Running")
                        : tr("Frozen"))).arg(
                clk_rate).arg(
                days).arg(
                fcTimeToString(hours, minutes)).arg(
                idString(id1, id2)).arg(
                trackStatusByteToString(track_stat));
    }

    /*private*/ QString LlnMon::interpretProgSlot(LocoNetMessage l, QString /*mode*/, int /*id1*/, int /*id2*/, int command) {
        /**
         * ********************************************************************************************
         * Programmer track: * ================= * The programmer track is
         * accessed as Special slot #124 ( $7C, 0x7C). It is a full *
         * asynchronous shared system resource. * * To start Programmer task,
         * write to slot 124. There will be an immediate LACK acknowledge * that
         * indicates what programming will be allowed. If a valid programming
         * task is started, * then at the final (asynchronous) programming
         * completion, a Slot read <E7> from slot 124 * will be sent. This is
         * the final task status reply. * * Programmer Task Start: *
         * ----------------------
         * <p>
         * <0xEF>,<0E>,<7C>,<PCMD>,<0>,<HOPSA>,<LOPSA>,<TRK>;<CVH>,<CVL>,
         * <p>
         * <DATA7>,<0>,<0>,<CHK> * * This OPC leads to immediate LACK codes:
         * <p>
         * <B4>,<7F>,<7F>,<chk> Function NOT implemented, no reply.
         * <p>
         * <B4>,<7F>,<0>,<chk> Programmer BUSY , task aborted, no reply.
         * <p>
         * <B4>,<7F>,<1>,<chk> Task accepted , <E7> reply at completion.
         * <p>
         * <B4>,<7F>,<0x40>,<chk> Task accepted blind NO <E7>
         * reply at completion. * * Note that the <7F> code will occur in
         * Operations Mode Read requests if the System is not * configured for
         * and has no Advanced Acknowlegement detection installed.. Operations
         * Mode * requests can be made and executed whilst a current Service
         * Mode programming task is keeping * the Programming track BUSY. If a
         * Programming request is rejected, delay and resend the * complete
         * request later. Some readback operations can keep the Programming
         * track busy for up * to a minute. Multiple devices, throttles/PC's
         * etc, can share and sequentially use the * Programming track as long
         * as they correctly interpret the response messages. Any Slot RD * from
         * the master will also contain the Programmer Busy status in bit 3 of
         * the <TRK> byte. * * A <PCMD> value of
         * <00> will abort current SERVICE mode programming task and will echo
         * with * an <E6> RD the command string that was aborted. * * <PCMD>
         * Programmer Command: * -------------------------- * Defined as * D7 -0
         * * D6 -Write/Read 1= Write, * 0=Read * D5 -Byte Mode 1= Byte
         * operation, * 0=Bit operation (if possible) * D4 -TY1 Programming Type
         * select bit * D3 -TY0 Prog type select bit * D2 -Ops Mode 1=Ops Mode
         * on Mainlines, * 0=Service Mode on Programming Track * D1 -0 reserved
         * * D0 -0-reserved * * Type codes: * ----------- * Byte Mode Ops Mode
         * TY1 TY0 Meaning * 1 0 0 0 Paged mode byte Read/Write on Service Track
         * * 1 0 0 0 Paged mode byte Read/Write on Service Track * 1 0 0 1
         * Direct mode byteRead/Write on Service Track * 0 0 0 1 Direct mode bit
         * Read/Write on Service Track * x 0 1 0 Physical Register byte
         * Read/Write on Service Track * x 0 1 1 Service Track- reserved
         * function * 1 1 0 0 Ops mode Byte program, no feedback * 1 1 0 1 Ops
         * mode Byte program, feedback * 0 1 0 0 Ops mode Bit program, no
         * feedback * 0 1 0 1 Ops mode Bit program, feedback * *
         * <HOPSA>Operations Mode Programming * 7 High address bits of Loco to
         * program, 0 if Service Mode
         * <p>
         * <LOPSA>Operations Mode Programming * 7 Low address bits of Loco to
         * program, 0 if Service Mode
         * <p>
         * <TRK> Normal Global Track status for this Master, * Bit 3 also is 1
         * WHEN Service Mode track is BUSY
         * <p>
         * <CVH> High 3 BITS of CV#, and ms bit of DATA.7
         * <p>
         * <0,0,CV9,CV8 - 0,0, D7,CV7>
         * <p>
         * <CVL> Low 7 bits of 10 bit CV address.
         * <p>
         * <0,CV6,CV5,CV4-CV3,CV2,CV1,CV0>
         * <p>
         * <DATA7>Low 7 BITS OF data to WR or RD COMPARE
         * <p>
         * <0,D6,D5,D4 - D3,D2,D1,D0> * ms bit is at CVH bit 1 position. * *
         * Programmer Task Final Reply: * ---------------------------- * (if saw
         * LACK
         * <B4>,<7F>,<1>,<chk> code reply at task start)
         * <p>
         * <0xE7>,<0E>,<7C>,<PCMD>,<PSTAT>,<HOPSA>,<LOPSA>,<TRK>;<CVH>,<CVL>,
         * <p>
         * <DATA7>,<0>,<0>,<CHK> * * <PSTAT> Programmer Status error flags.
         * Reply codes resulting from * completed task in PCMD * D7-D4 -reserved
         * * D3 -1= User Aborted this command * D2 -1= Failed to detect READ
         * Compare acknowledge response * from decoder * D1 -1= No Write
         * acknowledge response from decoder * D0 -1= Service Mode programming
         * track empty- No decoder detected * * This <E7> response is issued
         * whenever a Programming task is completed. It echos most of the *
         * request information and returns the PSTAT status code to indicate how
         * the task completed. * If a READ was requested <DATA7> and <CVH>
         * contain the returned data, if the PSTAT indicates * a successful
         * readback (typically =0). Note that if a Paged Read fails to detect a
         * * successful Page write acknowledge when first setting the Page
         * register, the read will be * aborted, showing no Write acknowledge
         * flag D1=1. *
         * ********************************************************************************************
         */
        int cvData;
        int cvNumber;

        // progTask = (progTaskMsg *) msgBuf;
        // slot - slot number for this request - slot 124 is programmer
        int pcmd = l.getElement(3); // programmer command
        int pstat = l.getElement(4); // programmer status error flags in
        // reply message
        int hopsa = l.getElement(5); // Ops mode - 7 high address bits
        // of loco to program
        int lopsa = l.getElement(6); // Ops mode - 7 low address bits of
        // loco to program
        /* trk - track status. Note: bit 3 shows if prog track is busy */
        int cvh = l.getElement(8); // hi 3 bits of CV# and msb of data7
        int cvl = l.getElement(9); // lo 7 bits of CV#
        int data7 = l.getElement(10); // 7 bits of data to program, msb
        // is in cvh above

        cvData = (((cvh & LnConstants::CVH_D7) << 6) | (data7 & 0x7f)); // was
        // PROG_DATA
        cvNumber = (((((cvh & LnConstants::CVH_CV8_CV9) >> 3) | (cvh & LnConstants::CVH_CV7)) * 128) + (cvl & 0x7f)) + 1; // was
        // PROG_CV_NUM(progTask)

        if (command == LnConstants::OPC_WR_SL_DATA) {
            /* interpret the programming mode request (to programmer) */
            switch ((pcmd & (LnConstants::PCMD_MODE_MASK | LnConstants::PCMD_RW))) {
                case LnConstants::PAGED_ON_SRVC_TRK:
                    return tr("Byte Read in Paged Mode on Service Track: %1.").arg(
                            tr("CV%1").arg(
                                    cvNumber));
                case LnConstants::PAGED_ON_SRVC_TRK | LnConstants::PCMD_RW:
                    return tr("Byte Write in Paged Mode on Service Track: %1.").arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg(cvData).arg(tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData))).arg( StringUtil::to8Bits(cvData, true)));
                case LnConstants::DIR_BYTE_ON_SRVC_TRK:
                    return tr("Byte Read in Direct Mode on Service Track: %1.").arg(
                            tr("CV%1").arg(
                                    cvNumber));
                case LnConstants::DIR_BYTE_ON_SRVC_TRK | LnConstants::PCMD_RW:
                    return tr("Byte Write in Direct Mode on Service Track: %1.").arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg(cvData).arg(tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData))).arg( StringUtil::to8Bits(cvData, true)));
                case LnConstants::DIR_BIT_ON_SRVC_TRK:
                    return tr("Bit Read in Direct Mode on Service Track: %1.").arg(
                            tr("CV%1").arg(
                                    cvNumber)).arg(cvData).arg(tr("0x%1").arg(
                                    StringUtil::twoHexFromInt(cvData))).arg( StringUtil::to8Bits(cvData, true));
                case LnConstants::DIR_BIT_ON_SRVC_TRK | LnConstants::PCMD_RW:
                    return tr("Bit Write in Direct Mode on Service Track: %1.").arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg(cvData).arg(tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData))).arg( StringUtil::to8Bits(cvData, true)));
                case LnConstants::REG_BYTE_RW_ON_SRVC_TRK:
                case LnConstants::REG_BYTE_RW_ON_SRVC_TRK | LnConstants::PCMD_BYTE_MODE:
                    return tr("Byte Read in Physical Register Mode on Service Track: %1.").arg(
                            tr("CV%1").arg(
                                    cvNumber));
                case LnConstants::REG_BYTE_RW_ON_SRVC_TRK | LnConstants::PCMD_RW:
                case LnConstants::REG_BYTE_RW_ON_SRVC_TRK | LnConstants::PCMD_BYTE_MODE | LnConstants::PCMD_RW:
                    return tr("Byte Write in Physical Register Mode on Service Track:%1.").arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg(cvData).arg(tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData))).arg(StringUtil::to8Bits(cvData, true)));
                case LnConstants::SRVC_TRK_RESERVED:
                case LnConstants::SRVC_TRK_RESERVED | LnConstants::PCMD_BYTE_MODE:
                    return tr("Service Track RESERVED MODE Read Detected!: %1.").arg(
                            tr("CV%1").arg(
                                    cvNumber));
                case LnConstants::SRVC_TRK_RESERVED | LnConstants::PCMD_RW:
                case LnConstants::SRVC_TRK_RESERVED | LnConstants::PCMD_BYTE_MODE | LnConstants::PCMD_RW:
                    return tr("LN_MSG_SLOT_PROG_MODE_REQUEST_SRVC_TRK_WR_RESERVED").arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg( cvData).arg( tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData)), StringUtil::to8Bits(cvData, true)));
                case LnConstants::OPS_BYTE_NO_FEEDBACK:
                    return tr("Byte Read (No feedback) on Main Track (Ops Mode): Decoder address %1: %2.").arg(
                            convertToMixed(lopsa, hopsa)).arg(
                            tr("CV%1").arg(
                                    cvNumber));
                case LnConstants::OPS_BYTE_NO_FEEDBACK | LnConstants::PCMD_RW:
                    return tr("Byte Write (No feedback) on Main Track: Decoder address %1: %2.").arg(
                            convertToMixed(lopsa, hopsa)).arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg(cvData).arg(
                                    tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData))).arg(StringUtil::to8Bits(cvData, true)));
                case LnConstants::OPS_BYTE_FEEDBACK:
                    return tr("Byte Read on Main Track (Ops Mode): Decoder address %1: %2.").arg(
                            convertToMixed(lopsa, hopsa)).arg(
                            tr("CV%1").arg(
                                    cvNumber));
                case LnConstants::OPS_BYTE_FEEDBACK | LnConstants::PCMD_RW:
                    return tr("Byte Write on Main Track (Ops Mode): Decoder address %1: %2.").arg(
                            convertToMixed(lopsa, hopsa)).arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg(cvData).arg(tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData)), StringUtil::to8Bits(cvData, true)));
                case LnConstants::OPS_BIT_NO_FEEDBACK:
                    return tr("Bit Read (No feedback) on Main Track (Ops Mode): Decoder address %1: %2.").arg(
                            convertToMixed(lopsa, hopsa)).arg(
                            tr("CV%1").arg(
                                    cvNumber));
                case LnConstants::OPS_BIT_NO_FEEDBACK | LnConstants::PCMD_RW:
                    return tr("Bit Write (No feedback) on Main Track (Ops Mode): Decoder address %1: %2.").arg(
                            convertToMixed(lopsa, hopsa)).arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg(cvData).arg(tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData)), StringUtil::to8Bits(cvData, true)));
                case LnConstants::OPS_BIT_FEEDBACK:
                    return tr("Bit Read on Main Track (Ops Mode): Decoder address %1: %2.").arg(
                            convertToMixed(lopsa, hopsa)).arg(
                            tr("CV%1").arg(
                                    cvNumber));
                case LnConstants::OPS_BIT_FEEDBACK | LnConstants::PCMD_RW:
                    return tr("Bit Write on Main Track (Ops Mode): Decoder address %1: %2.").arg(
                            convertToMixed(lopsa, hopsa)).arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg(cvData).arg(tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData)), StringUtil::to8Bits(cvData, true)));
                case 0:
                    return tr("Uhlenbrock IB-COM / Intellibox II Programming Read: %1.").arg(
                            tr("CV%1").arg(
                                    cvNumber));
                case LnConstants::PCMD_RW:
                    return tr("Uhlenbrock IB-COM / Intellibox II Programming Write: %1.").arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg(cvData).arg(tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData))).arg(StringUtil::to8Bits(cvData, true)));
                default:
                    return tr("Unknown Programming slot access with programming mode %1 (%2).").arg(
                            pcmd).arg(
                            tr("0x%1").arg(
                                    StringUtil::twoHexFromInt(pcmd)),
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg(cvData).arg(tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData)), StringUtil::to8Bits(cvData, true)));
            }
        } else {
            /* interpret the  programming mode response (from programmer) */
            /* if we're reading the slot back, check the status
             * this is supposed to be the Programming task final reply
             * and will have the resulting status byte.
             */
            QString responseMessage = "(ODD BEHAVIOR - Default value not overwritten - report to developers!"; // NOI18N
            forceHex = true;
            if (pstat != 0) {
                if ((pstat & LnConstants::PSTAT_USER_ABORTED) != 0) {
                    responseMessage = tr("Failed, User Aborted");
                    forceHex = false;
                } else if ((pstat & LnConstants::PSTAT_READ_FAIL) != 0) {
                    responseMessage = tr("Failed, Read Compare Acknowledge not detected");
                    forceHex = false;
                } else if ((pstat & LnConstants::PSTAT_WRITE_FAIL) != 0) {
                    responseMessage = tr("Failed, No Write Acknowledge from decoder");
                    forceHex = false;
                } else if ((pstat & LnConstants::PSTAT_NO_DECODER) != 0) {
                    responseMessage = tr("Failed, Service Mode programming track empty");
                    forceHex = false;
                } else if ((pstat & 0xF0) != 0) {
                    if ((pstat & 0xF0) == 0x10) {
                        // response from transponding decoder
                        responseMessage = tr("Was successful via RX4/BDL16x");
                        forceHex = false;

                    } else {
                        responseMessage = tr("Unable to decode response = %1").arg(
                                tr("0x%1").arg(
                                        StringUtil::twoHexFromInt(pstat)));
                    }
                }
            } else {
                responseMessage = tr("Was Successful");
                forceHex = false;
            }

            switch ((pcmd & (LnConstants::PCMD_MODE_MASK | LnConstants::PCMD_RW))) {
                case LnConstants::PAGED_ON_SRVC_TRK:
                    return tr("Programming Response: Read Byte in Paged Mode on Service Track %1: %2.").arg(
                            responseMessage).arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg(cvData).arg(
                                    tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData)),
                                    StringUtil::to8Bits(cvData, true)));
                case LnConstants::PAGED_ON_SRVC_TRK | LnConstants::PCMD_RW:
                    return tr("Programming Response: Write Byte in Paged Mode on Service Track %1: %2.").arg(
                            responseMessage).arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg(cvData).arg(
                                    tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData)),
                                    StringUtil::to8Bits(cvData, true)));
                case LnConstants::DIR_BYTE_ON_SRVC_TRK:
                    return tr("Programming Response: Read Byte in Direct Mode on Service Track %1: %2.").arg(
                            responseMessage).arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg(cvData).arg(
                                    tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData))).arg(
                                    StringUtil::to8Bits(cvData, true)));
                case LnConstants::DIR_BYTE_ON_SRVC_TRK | LnConstants::PCMD_RW:
                    return tr("Programming Response: Write Byte in Direct Mode on Service Track %1: %2.").arg(
                            responseMessage).arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg(
                                    cvData).arg(
                                    tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData))).arg(
                                    StringUtil::to8Bits(cvData, true)));
                case LnConstants::DIR_BIT_ON_SRVC_TRK:
                    return tr("Programming Response: Read Bit in Direct Mode on Service Track %1: %2.").arg(
                            responseMessage).arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg(cvData).arg(
                                    tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData))).arg(
                                    StringUtil::to8Bits(cvData, true)));
                case LnConstants::DIR_BIT_ON_SRVC_TRK | LnConstants::PCMD_RW:
                    return tr("Programming Response: Write Bit in Direct Mode on Service Track %1: %2.").arg(
                            responseMessage).arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg(cvData).arg(
                                    tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData))).arg(
                                    StringUtil::to8Bits(cvData, true)));
                case LnConstants::REG_BYTE_RW_ON_SRVC_TRK:
                case LnConstants::REG_BYTE_RW_ON_SRVC_TRK | LnConstants::PCMD_BYTE_MODE:
                    return tr("Programming Response: Read Byte in Physical Register Mode on Service Track %1: %2.").arg(
                            responseMessage).arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg(cvData).arg(
                                    tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData))).arg(
                                    StringUtil::to8Bits(cvData, true)));
                case LnConstants::REG_BYTE_RW_ON_SRVC_TRK | LnConstants::PCMD_RW:
                case LnConstants::REG_BYTE_RW_ON_SRVC_TRK | LnConstants::PCMD_BYTE_MODE | LnConstants::PCMD_RW:
                    return tr("Programming Response: Write Byte in Physical Register Mode on Service Track %1: %2.").arg(
                            responseMessage).arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg(cvData).arg(
                                    tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData))).arg(
                                    StringUtil::to8Bits(cvData, true)));
                case LnConstants::SRVC_TRK_RESERVED:
                case LnConstants::SRVC_TRK_RESERVED | LnConstants::PCMD_BYTE_MODE:
                    return tr("Programming Response: Service Track RESERVED MODE Read Detected! %1: %2.").arg(
                            responseMessage).arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg( cvData).arg(
                                    tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData))).arg(
                                    StringUtil::to8Bits(cvData, true)));
                case LnConstants::SRVC_TRK_RESERVED | LnConstants::PCMD_RW:
                case LnConstants::SRVC_TRK_RESERVED | LnConstants::PCMD_BYTE_MODE | LnConstants::PCMD_RW:
                    return tr("Programming Response: Service Track RESERVED MODE Write Detected! %1: %2.").arg(
                            responseMessage).arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg(cvData).arg(
                                    tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData))).arg(
                                    StringUtil::to8Bits(cvData, true)));
                case LnConstants::OPS_BYTE_NO_FEEDBACK:
                    return tr("Programming Response: Byte Read (No feedback) on Main Track (Ops Mode) %1: Decoder address %2: %3.").arg(
                            responseMessage).arg(
                            convertToMixed(lopsa, hopsa)).arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg( cvData).arg(
                                    tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData))).arg(
                                    StringUtil::to8Bits(cvData, true)));
                case LnConstants::OPS_BYTE_NO_FEEDBACK | LnConstants::PCMD_RW:
                    return tr("Programming Response: Byte Write (No feedback) on Main Track (Ops Mode) %1: Decoder address %2: %3.").arg(
                            responseMessage).arg(
                            convertToMixed(lopsa, hopsa),
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg( cvData).arg(
                                    tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData))).arg(
                                    StringUtil::to8Bits(cvData, true)));
                case LnConstants::OPS_BYTE_FEEDBACK:
                    return tr("Programming Response: Byte Read on Main Track (Ops Mode) %1: Decoder address %2: %3.").arg(
                            responseMessage).arg(
                            convertToMixed(lopsa, hopsa)).arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg(cvData).arg(
                                    tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData))).arg(
                                    StringUtil::to8Bits(cvData, true)));
                case LnConstants::OPS_BYTE_FEEDBACK | LnConstants::PCMD_RW:
                    return tr("Programming Response: Byte Write on Main Track (Ops Mode) %1: Decoder address %2: %3.").arg(
                            responseMessage).arg(
                            convertToMixed(lopsa, hopsa)).arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg(cvData).arg(
                                    tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData))).arg(
                                    StringUtil::to8Bits(cvData, true)));
                case LnConstants::OPS_BIT_NO_FEEDBACK:
                    return tr("Programming Response: Bit Read (No feedback) on Main Track (Ops Mode) %1: Decoder address %2: %3.").arg(
                            responseMessage).arg(
                            convertToMixed(lopsa, hopsa)).arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg( cvData).arg(
                                    tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData))).arg(
                                    StringUtil::to8Bits(cvData, true)));
                case LnConstants::OPS_BIT_NO_FEEDBACK | LnConstants::PCMD_RW:
                    return tr("Programming Response: Bit Write (No feedback) on Main Track (Ops Mode) %1: Decoder address %2: %3.").arg(
                            responseMessage).arg(
                            convertToMixed(lopsa, hopsa)).arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg( cvData).arg(
                                    tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData))).arg(
                                    StringUtil::to8Bits(cvData, true)));
                case LnConstants::OPS_BIT_FEEDBACK:
                    return tr("Programming Response: Bit Read on Main Track (Ops Mode) %1: Decoder address %2: %3.").arg(
                            responseMessage).arg(
                            convertToMixed(lopsa, hopsa)).arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg( cvData).arg(
                                    tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData))).arg(
                                    StringUtil::to8Bits(cvData, true)));
                case LnConstants::OPS_BIT_FEEDBACK | LnConstants::PCMD_RW:
                    return tr("Programming Response: Bit Write on Main Track (Ops Mode) %1: Decoder address %2: %3.").arg(
                            responseMessage).arg(
                            convertToMixed(lopsa, hopsa)).arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg( cvData).arg(
                                    tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData))).arg(
                                    StringUtil::to8Bits(cvData, true)));
                case 0:
                    return tr("Programming Response: Uhlenbrock IB-COM / Intellibox II Programming Read %1: %2.").arg(
                            responseMessage).arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg( cvData).arg(
                                    tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData))).arg(
                                    StringUtil::to8Bits(cvData, true)));
                case LnConstants::PCMD_RW:
                    return tr("Programming Response: Uhlenbrock IB-COM / Intellibox II Programming Write %1: %2.").arg(
                            responseMessage).arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg( cvData).arg(
                                    tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData))).arg(
                                    StringUtil::to8Bits(cvData, true)));
                default:
                    return tr("Programming Response: Unknown Programming slot access with programming mode %1 (%2) %3: %4.").arg(
                            pcmd).arg(
                            tr("0x%1").arg(
                                    StringUtil::twoHexFromInt(pcmd))).arg(
                            responseMessage).arg(
                            tr("CV%1 value %2 (%3, %4b)").arg(
                                    cvNumber).arg( cvData).arg(
                                    tr("0x%1").arg(
                                            StringUtil::twoHexFromInt(cvData))).arg(
                                    StringUtil::to8Bits(cvData, true)));
            }
        }
    }

    /*private*/ QString LlnMon::interpretCmdStnCfgSlotRdWr(LocoNetMessage l, int command) {

        /**
         * ************************************************
         * Configuration slot, holding op switches
         * ************************************************
         * <p>
         * NOTE: previously, this message provided specific text about the
         * meaning of each OpSw when it was closed. With the advent of newer
         * Digitrax command stations, the specific information was no longer
         * completely accurate. As such, this information now only shows bits as
         * "closed" or "thrown".
         */
        QString thrown = tr("Thrown");
        QString closed = tr("Closed");

        QString opswGroup1, opswGroup2, opswGroup3, opswGroup4,
                opswGroup5, opswGroup6, opswGroup7, opswGroup8;
        opswGroup1 = tr("OpSw%1=%2, OpSw%3=%4, OpSw%5=%6, OpSw%7=%8, OpSw%9=%10, OpSw%11=%12, OpSw%13=%14, OpSw%15=%16").arg(
                1).arg( ((l.getElement(3) & 0x01) != 0 ? closed : thrown)).arg(
                2).arg( ((l.getElement(3) & 0x02) != 0 ? closed : thrown)).arg(
                3).arg( ((l.getElement(3) & 0x04) != 0 ? closed : thrown)).arg(
                4).arg( ((l.getElement(3) & 0x08) != 0 ? closed : thrown)).arg(
                5).arg( ((l.getElement(3) & 0x10) != 0 ? closed : thrown)).arg(
                6).arg( ((l.getElement(3) & 0x20) != 0 ? closed : thrown)).arg(
                7).arg( ((l.getElement(3) & 0x40) != 0 ? closed : thrown)).arg(
                8).arg( thrown);
        opswGroup2 = tr("OpSw%1=%2, OpSw%3=%4, OpSw%5=%6, OpSw%7=%8, OpSw%9=%10, OpSw%11=%12, OpSw%13=%14, OpSw%15=%16").arg(
                9).arg( ((l.getElement(4) & 0x01) != 0 ? closed : thrown)).arg(
                10).arg( ((l.getElement(4) & 0x02) != 0 ? closed : thrown)).arg(
                11).arg( ((l.getElement(4) & 0x04) != 0 ? closed : thrown)).arg(
                12).arg( ((l.getElement(4) & 0x08) != 0 ? closed : thrown)).arg(
                13).arg( ((l.getElement(4) & 0x10) != 0 ? closed : thrown)).arg(
                14).arg( ((l.getElement(4) & 0x20) != 0 ? closed : thrown)).arg(
                15).arg( ((l.getElement(4) & 0x40) != 0 ? closed : thrown)).arg(
                16).arg( thrown);
        opswGroup3 = tr("OpSw%1=%2, OpSw%3=%4, OpSw%5=%6, OpSw%7=%8, OpSw%9=%10, OpSw%11=%12, OpSw%13=%14, OpSw%15=%16").arg(
                17).arg( ((l.getElement(5) & 0x01) != 0 ? closed : thrown)).arg(
                18).arg( ((l.getElement(5) & 0x02) != 0 ? closed : thrown)).arg(
                19).arg( ((l.getElement(5) & 0x04) != 0 ? closed : thrown)).arg(
                20).arg( ((l.getElement(5) & 0x08) != 0 ? closed : thrown)).arg(
                21).arg( ((l.getElement(5) & 0x10) != 0 ? closed : thrown)).arg(
                22).arg( ((l.getElement(5) & 0x20) != 0 ? closed : thrown)).arg(
                23).arg( ((l.getElement(5) & 0x40) != 0 ? closed : thrown)).arg(
                24).arg( thrown);
        opswGroup4 = tr("OpSw%1=%2, OpSw%3=%4, OpSw%5=%6, OpSw%7=%8, OpSw%9=%10, OpSw%11=%12, OpSw%13=%14, OpSw%15=%16").arg(
                25).arg( ((l.getElement(6) & 0x01) != 0 ? closed : thrown)).arg(
                26).arg( ((l.getElement(6) & 0x02) != 0 ? closed : thrown)).arg(
                27).arg( ((l.getElement(6) & 0x04) != 0 ? closed : thrown)).arg(
                28).arg( ((l.getElement(6) & 0x08) != 0 ? closed : thrown)).arg(
                29).arg( ((l.getElement(6) & 0x10) != 0 ? closed : thrown)).arg(
                30).arg( ((l.getElement(6) & 0x20) != 0 ? closed : thrown)).arg(
                31).arg( ((l.getElement(6) & 0x40) != 0 ? closed : thrown)).arg(
                32).arg( thrown);
        opswGroup5 = tr("OpSw%1=%2, OpSw%3=%4, OpSw%5=%6, OpSw%7=%8, OpSw%9=%10, OpSw%11=%12, OpSw%13=%14, OpSw%15=%16").arg(
                33).arg( ((l.getElement(8) & 0x01) != 0 ? closed : thrown)).arg(
                34).arg( ((l.getElement(8) & 0x02) != 0 ? closed : thrown)).arg(
                35).arg( ((l.getElement(8) & 0x04) != 0 ? closed : thrown)).arg(
                36).arg( ((l.getElement(8) & 0x08) != 0 ? closed : thrown)).arg(
                37).arg( ((l.getElement(8) & 0x10) != 0 ? closed : thrown)).arg(
                38).arg( ((l.getElement(8) & 0x20) != 0 ? closed : thrown)).arg(
                39).arg( ((l.getElement(8) & 0x40) != 0 ? closed : thrown)).arg(
                40).arg( thrown);
        opswGroup6 = tr("OpSw%1=%2, OpSw%3=%4, OpSw%5=%6, OpSw%7=%8, OpSw%9=%10, OpSw%11=%12, OpSw%13=%14, OpSw%15=%16").arg(
                41).arg( ((l.getElement(9) & 0x01) != 0 ? closed : thrown)).arg(
                42).arg( ((l.getElement(9) & 0x02) != 0 ? closed : thrown)).arg(
                43).arg( ((l.getElement(9) & 0x04) != 0 ? closed : thrown)).arg(
                44).arg( ((l.getElement(9) & 0x08) != 0 ? closed : thrown)).arg(
                45).arg( ((l.getElement(9) & 0x10) != 0 ? closed : thrown)).arg(
                46).arg( ((l.getElement(9) & 0x20) != 0 ? closed : thrown)).arg(
                47).arg( ((l.getElement(9) & 0x40) != 0 ? closed : thrown)).arg(
                48).arg( thrown);
        opswGroup7 = tr("OpSw%1=%2, OpSw%3=%4, OpSw%5=%6, OpSw%7=%8, OpSw%9=%10, OpSw%11=%12, OpSw%13=%14, OpSw%15=%16").arg(
                49).arg( ((l.getElement(10) & 0x01) != 0 ? closed : thrown)).arg(
                50).arg( ((l.getElement(10) & 0x02) != 0 ? closed : thrown)).arg(
                51).arg( ((l.getElement(10) & 0x04) != 0 ? closed : thrown)).arg(
                52).arg( ((l.getElement(10) & 0x08) != 0 ? closed : thrown)).arg(
                53).arg( ((l.getElement(10) & 0x10) != 0 ? closed : thrown)).arg(
                54).arg( ((l.getElement(10) & 0x20) != 0 ? closed : thrown)).arg(
                55).arg( ((l.getElement(10) & 0x40) != 0 ? closed : thrown)).arg(
                56).arg( thrown);
        opswGroup8 = tr("OpSw%1=%2, OpSw%3=%4, OpSw%5=%6, OpSw%7=%8, OpSw%9=%10, OpSw%11=%12, OpSw%13=%14, OpSw%15=%16").arg(
                57).arg( ((l.getElement(11) & 0x01) != 0 ? closed : thrown)).arg(
                58).arg( ((l.getElement(11) & 0x02) != 0 ? closed : thrown)).arg(
                59).arg( ((l.getElement(11) & 0x04) != 0 ? closed : thrown)).arg(
                60).arg( ((l.getElement(11) & 0x08) != 0 ? closed : thrown)).arg(
                61).arg( ((l.getElement(11) & 0x10) != 0 ? closed : thrown)).arg(
                62).arg( ((l.getElement(11) & 0x20) != 0 ? closed : thrown)).arg(
                63).arg( ((l.getElement(11) & 0x40) != 0 ? closed : thrown)).arg(
                64).arg( thrown);
        return tr(((command == LnConstants::OPC_WR_SL_DATA)
                ? "Write Command Station OpSw values:\n%1,/n%2,/n%3,\n%4,\n%5,\n%6,\n%7,\n%8."
                : "Report of current Command Station OpSw values:\n%1,/n%2,/n%3,\n%4,\n%5,\n%6,\n%7,\n%8.")).arg(
                opswGroup1).arg( opswGroup2).arg( opswGroup3).arg( opswGroup4).arg(
                opswGroup5).arg( opswGroup6).arg( opswGroup7).arg( opswGroup8);

    }

    /*private*/ QString LlnMon::interpretCmdStnExtCfgSlotRdWr(LocoNetMessage l, int command) {
    /*
     * ************************************************
     * Extended Configuration slot, holding op switches
     * ************************************************
     */
        QString thrown = tr("Thrown");
        QString closed = tr("losed");

        QString opswGroup1, opswGroup2, opswGroup3, opswGroup4,
                opswGroup5, opswGroup6, opswGroup7, opswGroup8;
        opswGroup1 = tr("OpSw%1=%2, OpSw%3=%4, OpSw%5=%6, OpSw%7=%8, OpSw%9=%10, OpSw%11=%12, OpSw%13=%14, OpSw%15=%16").arg(
                65).arg( ((l.getElement(3) & 0x01) != 0 ? closed : thrown)).arg(
                66).arg( ((l.getElement(3) & 0x02) != 0 ? closed : thrown)).arg(
                67).arg( ((l.getElement(3) & 0x04) != 0 ? closed : thrown)).arg(
                68).arg( ((l.getElement(3) & 0x08) != 0 ? closed : thrown)).arg(
                69).arg( ((l.getElement(3) & 0x10) != 0 ? closed : thrown)).arg(
                70).arg( ((l.getElement(3) & 0x20) != 0 ? closed : thrown)).arg(
                71).arg( ((l.getElement(3) & 0x40) != 0 ? closed : thrown)).arg(
                72).arg( thrown);
        opswGroup2 = tr("OpSw%1=%2, OpSw%3=%4, OpSw%5=%6, OpSw%7=%8, OpSw%9=%10, OpSw%11=%12, OpSw%13=%14, OpSw%15=%16").arg(
                73).arg( ((l.getElement(4) & 0x01) != 0 ? closed : thrown)).arg(
                74).arg( ((l.getElement(4) & 0x02) != 0 ? closed : thrown)).arg(
                75).arg( ((l.getElement(4) & 0x04) != 0 ? closed : thrown)).arg(
                76).arg( ((l.getElement(4) & 0x08) != 0 ? closed : thrown)).arg(
                77).arg( ((l.getElement(4) & 0x10) != 0 ? closed : thrown)).arg(
                78).arg( ((l.getElement(4) & 0x20) != 0 ? closed : thrown)).arg(
                79).arg( ((l.getElement(4) & 0x40) != 0 ? closed : thrown)).arg(
                80).arg( thrown);
        opswGroup3 = tr("OpSw%1=%2, OpSw%3=%4, OpSw%5=%6, OpSw%7=%8, OpSw%9=%10, OpSw%11=%12, OpSw%13=%14, OpSw%15=%16").arg(
                81).arg( ((l.getElement(5) & 0x01) != 0 ? closed : thrown)).arg(
                82).arg( ((l.getElement(5) & 0x02) != 0 ? closed : thrown)).arg(
                83).arg( ((l.getElement(5) & 0x04) != 0 ? closed : thrown)).arg(
                84).arg( ((l.getElement(5) & 0x08) != 0 ? closed : thrown)).arg(
                85).arg( ((l.getElement(5) & 0x10) != 0 ? closed : thrown)).arg(
                86).arg( ((l.getElement(5) & 0x20) != 0 ? closed : thrown)).arg(
                87).arg( ((l.getElement(5) & 0x40) != 0 ? closed : thrown)).arg(
                88).arg( thrown);
        opswGroup4 = tr("OpSw%1=%2, OpSw%3=%4, OpSw%5=%6, OpSw%7=%8, OpSw%9=%10, OpSw%11=%12, OpSw%13=%14, OpSw%15=%16").arg(
                89).arg( ((l.getElement(6) & 0x01) != 0 ? closed : thrown)).arg(
                90).arg( ((l.getElement(6) & 0x02) != 0 ? closed : thrown)).arg(
                91).arg( ((l.getElement(6) & 0x04) != 0 ? closed : thrown)).arg(
                92).arg( ((l.getElement(6) & 0x08) != 0 ? closed : thrown)).arg(
                93).arg( ((l.getElement(6) & 0x10) != 0 ? closed : thrown)).arg(
                94).arg( ((l.getElement(6) & 0x20) != 0 ? closed : thrown)).arg(
                95).arg( ((l.getElement(6) & 0x40) != 0 ? closed : thrown)).arg(
                96).arg( thrown);
        opswGroup5 = tr("OpSw%1=%2, OpSw%3=%4, OpSw%5=%6, OpSw%7=%8, OpSw%9=%10, OpSw%11=%12, OpSw%13=%14, OpSw%15=%16").arg(
                97).arg( ((l.getElement(8) & 0x01) != 0 ? closed : thrown)).arg(
                98).arg( ((l.getElement(8) & 0x02) != 0 ? closed : thrown)).arg(
                99).arg( ((l.getElement(8) & 0x04) != 0 ? closed : thrown)).arg(
                100).arg( ((l.getElement(8) & 0x08) != 0 ? closed : thrown)).arg(
                101).arg( ((l.getElement(8) & 0x10) != 0 ? closed : thrown)).arg(
                102).arg( ((l.getElement(8) & 0x20) != 0 ? closed : thrown)).arg(
                103).arg( ((l.getElement(8) & 0x40) != 0 ? closed : thrown)).arg(
                104).arg( thrown);
        opswGroup6 = tr("OpSw%1=%2, OpSw%3=%4, OpSw%5=%6, OpSw%7=%8, OpSw%9=%10, OpSw%11=%12, OpSw%13=%14, OpSw%15=%16").arg(
                105).arg( ((l.getElement(9) & 0x01) != 0 ? closed : thrown)).arg(
                106).arg( ((l.getElement(9) & 0x02) != 0 ? closed : thrown)).arg(
                107).arg( ((l.getElement(9) & 0x04) != 0 ? closed : thrown)).arg(
                108).arg( ((l.getElement(9) & 0x08) != 0 ? closed : thrown)).arg(
                109).arg( ((l.getElement(9) & 0x10) != 0 ? closed : thrown)).arg(
                110).arg( ((l.getElement(9) & 0x20) != 0 ? closed : thrown)).arg(
                111).arg( ((l.getElement(9) & 0x40) != 0 ? closed : thrown)).arg(
                112).arg( thrown);
        opswGroup7 = tr("OpSw%1=%2, OpSw%3=%4, OpSw%5=%6, OpSw%7=%8, OpSw%9=%10, OpSw%11=%12, OpSw%13=%14, OpSw%15=%16").arg(
                113).arg( ((l.getElement(10) & 0x01) != 0 ? closed : thrown)).arg(
                114).arg( ((l.getElement(10) & 0x02) != 0 ? closed : thrown)).arg(
                115).arg( ((l.getElement(10) & 0x04) != 0 ? closed : thrown)).arg(
                116).arg( ((l.getElement(10) & 0x08) != 0 ? closed : thrown)).arg(
                117).arg( ((l.getElement(10) & 0x10) != 0 ? closed : thrown)).arg(
                118).arg( ((l.getElement(10) & 0x20) != 0 ? closed : thrown)).arg(
                119).arg( ((l.getElement(10) & 0x40) != 0 ? closed : thrown)).arg(
                120).arg( thrown);
        opswGroup8 = tr("OpSw%1=%2, OpSw%3=%4, OpSw%5=%6, OpSw%7=%8, OpSw%9=%10, OpSw%11=%12, OpSw%13=%14, OpSw%15=%16").arg(
                121).arg( ((l.getElement(11) & 0x01) != 0 ? closed : thrown)).arg(
                122).arg( ((l.getElement(11) & 0x02) != 0 ? closed : thrown)).arg(
                123).arg( ((l.getElement(11) & 0x04) != 0 ? closed : thrown)).arg(
                124).arg( ((l.getElement(11) & 0x08) != 0 ? closed : thrown)).arg(
                125).arg( ((l.getElement(11) & 0x10) != 0 ? closed : thrown)).arg(
                126).arg( ((l.getElement(11) & 0x20) != 0 ? closed : thrown)).arg(
                127).arg( ((l.getElement(11) & 0x40) != 0 ? closed : thrown)).arg(
                128).arg( thrown);
        return tr(((command == LnConstants::OPC_WR_SL_DATA)
                ? "Write Extended Command Station OpSw values:\n%1,/n%2,/n%3,\n%4,\n%5,\n%6,\n%7,\n%8."
                : "Report of current Extended Command Station OpSw values:\n%1,/n%2,/n%3,\n%4,\n%5,\n%6,\n%7,\n%8.")).arg(
                opswGroup1).arg( opswGroup2).arg( opswGroup3).arg( opswGroup4).arg(
                opswGroup5).arg( opswGroup6).arg( opswGroup7).arg( opswGroup8);
    }

    /*private*/ QString LlnMon::interpretStandardSlotRdWr(LocoNetMessage l, int id1, int id2, int command, int slot) {

        /**
         * ************************************************
         * normal slot read/write message - see info above *
         * ************************************************
         */
        int trackStatus = l.getElement(7); // track status
        int stat = l.getElement(3); // slot status
        int adr = l.getElement(4); // loco address
        int spd = l.getElement(5); // command speed
        int dirf = l.getElement(6); // direction and F0-F4 bits
        QVector<QString> dirf0_4 = interpretF0_F4toStrings(dirf);
        int ss2 = l.getElement(8); // slot status 2 (tells how to use
        // ID1/ID2 & ADV Consist)
        int adr2 = l.getElement(9); // loco address high
        int snd = l.getElement(10); // Sound 1-4 / F5-F8
        QVector<QString> sndf5_8 = interpretF5_F8toStrings(snd);

        QString locoAdrStr = figureAddressIncludingAliasing(adr, adr2, ss2, id1, id2);
        return tr(((command == LnConstants::OPC_WR_SL_DATA)
                ? "Write slot %1 information:\nLoco %2 is %3, %4, operating in %5 SS mode, and is moving %6 at speed %7,\nF0=%8, F1=%9, F2=%10, F3=%11, F4=%12, F5=%13, F6=%14, F7=%15, F8=%16\n%17; %18, %19."
                : "Report of slot %1 information:\nLoco %2 is %3, %4, operating in %5 SS mode, and is moving %6 at speed %7,\nF0=%8, F1=%9, F2=%10, F3=%11, F4=%12, F5=%13, F6=%14, F7=%15, F8=%16\n%17; %18, %19.")).arg(
                slot ).arg(
                locoAdrStr ).arg(
                LnConstants::CONSIST_STAT(stat) ).arg(
                LnConstants::LOCO_STAT(stat) ).arg(
                LnConstants::DEC_MODE(stat) ).arg(
                directionOfTravelString((dirf & LnConstants::DIRF_DIR) == 0) ).arg(
                spd ).arg( // needs re-interpretation for some cases of slot consisting state
                dirf0_4[0] ).arg(
                dirf0_4[1] ).arg(
                dirf0_4[2] ).arg(
                dirf0_4[3] ).arg(
                dirf0_4[4] ).arg(
                sndf5_8[0] ).arg(
                sndf5_8[1] ).arg(
                sndf5_8[2] ).arg(
                sndf5_8[3] ).arg(
                trackStatusByteToString(trackStatus) ).arg(
                tr("STAT2=%1").arg(
                        tr("0x%1").arg(
                                StringUtil::twoHexFromInt(ss2)))).arg(
                tr("ThrottleID=%1").arg(
                        idString(id1, id2)));
    }

    /*private*/ QString LlnMon::interpretOpcPanelResponse(LocoNetMessage l) {
        switch (l.getElement(1)) {
            case 0x12: {
                // Bit 3 (0x08 in hex) is set by every UR-92 we've ever captured.
                // The hypothesis is this indicates duplex enabled, but this has
                // not been confirmed with Digitrax.
                return tr("UR92 Responding with LocoNet ID %1%2.").arg(
                        l.getElement(3) & 0x07).arg(
                        ((l.getElement(3) & 0x08) == 0x08
                        ? tr(", duplex enabled")
                        : ""));
            }
            case 0x17: {
                return tr("UR90 Responding with LocoNet ID %1.").arg(
                        l.getElement(3) & 0x07);
            }
            case 0x1F: {
                return tr("UR91 Responding with LocoNet ID %1.").arg(
                        l.getElement(3) & 0x07);
            }
            default: {
                return "";
            }
        }
    }
#if 0
    /*private*/ QString interpretOpcLissyUpdate(LocoNetMessage l) {
        /*
         * OPC_LISSY_UPDATE   0xE4
         *
         * LISSY is an automatic train detection system made by Uhlenbrock.
         * All documentation appears to be in German.
         */
        switch (l.getElement(1)) {
            case 0x08: // Format LISSY message
                int unit = (l.getElement(4) & 0x7F);
                int address = (l.getElement(6) & 0x7F) + 128 * (l.getElement(5) & 0x7F);
                switch (l.getElement(2)) {
                    case 0x00:
                        // Reverse-engineering note: interpretation of element 2 per wiki.rocrail.net
                        // OPC_LISSY_REP
                        return tr("LN_MSG_LISSY_IR_REPORT_LOCO_MOVEMENT",
                                unit,
                                QString::number(address),
                                ((l.getElement(3) & 0x20) == 0
                                ? tr("LN_MSG_LISSY_IR_REPORT_HELPER_DIRECTION_NORTH")
                                : tr("LN_MSG_LISSY_IR_REPORT_HELPER_DIRECTION_SOUTH")));
                    case 0x01:
                        // Reverse-engineering note: interpretation of element 2 per wiki.rocrail.net
                        // OPC_WHEELCNT_REP
                        int wheelCount = (l.getElement(6) & 0x7F) + 128 * (l.getElement(5) & 0x7F);
                        return tr("LN_MSG_LISSY_WHEEL_REPORT_LOCO_MOVEMENT",
                                unit, QString::number(wheelCount),
                                ((l.getElement(3) & 0x20) == 0
                                ? tr("LN_MSG_LISSY_IR_REPORT_HELPER_DIRECTION_NORTH")
                                : tr("LN_MSG_LISSY_IR_REPORT_HELPER_DIRECTION_SOUTH")));
                    default:
                        break;
                }
                break;

            case 0x0A: // Format special message
                int element = l.getElement(2) * 128 + l.getElement(3);
                int stat1 = l.getElement(5);
                int stat2 = l.getElement(6);
                String status;
                switch (stat1 & 0x30) {
                    case 0x30:
                        status = tr("LN_MSG_SE_REPORT_HELPER_BOTH_RES");
                        break;
                    case 0x10:
                        status = tr("LN_MSG_SE_REPORT_HELPER_AX_RES");
                        break;
                    case 0x20:
                        status = tr("LN_MSG_SE_REPORT_HELPER_XA_RES");
                        break;
                    default:
                        status = tr("LN_MSG_SE_REPORT_HELPER_NO_RES");
                        break;
                }

                return tr("LN_MSG_SE_REPORT",
                        (element + 1), element,
                        l.getElement(7), l.getElement(8),
                        status,
                        tr(((stat2 & 0x01) != 0)
                                ? "LN_MSG_SWITCH_STATE_THROWN"
                                : "LN_MSG_SWITCH_STATE_CLOSED"),
                        tr(((stat1 & 0x01) != 0)
                                ? "LN_MSG_SE_REPORT_HELPER_OCCUPIED"
                                : "LN_MSG_SE_REPORT_HELPER_UNOCCUPIED"));
            case 0x09:
                if (l.getElement(4) == 0x00) {
                    forceHex = true;
                    return tr("LN_MSG_UNRECOGNIZED_SIG_STATE_REPORT_MAY_BE_FROM_CML_HW");
                }
                break;
            default:
                break;
        }
        return "";
    }

    /*private*/ QString interpretOpcImmPacket(LocoNetMessage l) {
        /*
         * OPC_IMM_PACKET   0xED   ;SEND n-byte packet immediate LACK
         *                         ; Follow on message: LACK
         *                         ; <0xED>,<0B>,<7F>,<REPS>,<DHI>,<IM1>,<IM2>,
         *                         ;        <IM3>,<IM4>,<IM5>,<CHK>
         *                         ;   <DHI>=<0,0,1,IM5.7-IM4.7,IM3.7,IM2.7,IM1.7>
         *                         ;   <REPS>  D4,5,6=#IM bytes,
         *                         ;           D3=0(reserved);
         *                         ;           D2,1,0=repeat CNT
         *                         ; IF Not limited MASTER then
         *                         ;   LACK=<B4>,<7D>,<7F>,<chk> if CMD ok
         *                         ; IF limited MASTER then Lim Masters respond
         *                         ;   with <B4>,<7E>,<lim adr>,<chk>
         *                         ; IF internal buffer BUSY/full respond
         *                         ;   with <B4>,<7D>,<0>,<chk>
         *                         ;   (NOT IMPLEMENTED IN DT200)
         *
         * This sends a raw NMRA packet across the LocoNet.
         *
         * Page 11 of LocoNet Personal Edition v1.0.
         *
         * Decodes for the F9-F28 functions taken from the NMRA standards and
         * coded by Leo Bicknell.
         */
        // sendPkt = (sendPktMsg *) msgBuf;
        int val7f = l.getElement(2);
        /* fixed value of 0x7f */

        int reps = l.getElement(3);
        /* repeat count */

        int dhi = l.getElement(4);
        /* high bits of data bytes */

        int im1 = l.getElement(5);
        int im2 = l.getElement(6);
        int im3 = l.getElement(7);
        int im4 = l.getElement(8);
        int im5 = l.getElement(9);
        int mobileDecoderAddress = -999;
        int nmraInstructionType = -999;
        int nmraSubInstructionType = -999;
        int playableWhistleLevel = -999;

        // see if it really is a 'Send Packet' as defined in LocoNet PE
        if ((val7f == 0x7f) && (l.getElement(1) == 0x0B)) {
            int len = ((reps & 0x70) >> 4);
            if (len < 2) {
                return ""; // no valid NMRA packets of less than 2 bytes.
            }
            // duplication of packet data as packetInt was deemed necessary
            // due to issues with msBit loss when converting from "byte" to
            // integral forms
            byte[] packet = new byte[len];
            int[] packetInt = new int[len];
            packet[0] = (byte) (im1 + ((dhi & 0x01) != 0 ? 0x80 : 0));
            packetInt[0] = (im1 + ((dhi & 0x01) != 0 ? 0x80 : 0));
            if (len >= 2) {
                packet[1] = (byte) (im2 + ((dhi & 0x02) != 0 ? 0x80 : 0));
                packetInt[1] = (im2 + ((dhi & 0x02) != 0 ? 0x80 : 0));
            }
            if (len >= 3) {
                packet[2] = (byte) (im3 + ((dhi & 0x04) != 0 ? 0x80 : 0));
                packetInt[2] = (im3 + ((dhi & 0x04) != 0 ? 0x80 : 0));
            }
            if (len >= 4) {
                packet[3] = (byte) (im4 + ((dhi & 0x08) != 0 ? 0x80 : 0));
                packetInt[3] = (im4 + ((dhi & 0x08) != 0 ? 0x80 : 0));
            }
            if (len >= 5) {
                packet[4] = (byte) (im5 + ((dhi & 0x10) != 0 ? 0x80 : 0));
                packetInt[4] = (im5 + ((dhi & 0x10) != 0 ? 0x80 : 0));
            }

            int address;
            // compute some information which is useful for decoding
            // the "Playable" whistle message
            // Information reverse-engineered by B. Milhaupt and used with permission
            if ((packetInt[0] & 0x80) == 0x0) {
                // immediate packet addresses a 7-bit multi-function (mobile) decoder
                mobileDecoderAddress = packetInt[0];
                nmraInstructionType = (packetInt[1] & 0xE) >> 5;
                nmraSubInstructionType = (packetInt[1] & 0x1f);
                if ((nmraSubInstructionType == 0x1d) && (packetInt[2] == 0x7f)) {
                    playableWhistleLevel = packetInt[3];
                }
            } else if ((packetInt[0] & 0xC0) == 0xC0) {
                // immediate packet addresses a 14-bit multi-function (mobile) decoder
                mobileDecoderAddress = ((packetInt[0] & 0x3F) << 8) + packetInt[1];
                nmraInstructionType = (packetInt[2] & 0xE0) >> 5;
                nmraSubInstructionType = (packetInt[2] & 0x1f);
                if ((nmraSubInstructionType == 0x1d) && (packetInt[3] == 0x7f)) {
                    playableWhistleLevel = packetInt[4];
                }
            } else {
                // immediate packet not addressed to a multi-function (mobile) decoder
                log.debug("got Here 1.");
            }
            if ((mobileDecoderAddress >= 0)
                    && (nmraInstructionType == 1)
                    && (nmraSubInstructionType == 0x1D)) {
                // the "Playable" whistle message
                // Information reverse-engineered by B. Milhaupt and used with permission
                return tr("LN_MSG_PLAYABLE_WHISTLE_CONTROL",
                        QString::number(mobileDecoderAddress),
                        playableWhistleLevel,
                        (reps & 0x7));
            }

            // F9-F28 w/a long address.
            if ((packetInt[0] & 0xC0) == 0xC0) {
                address = ((packetInt[0] & 0x3F) << 8) + packetInt[1];

                if ((packetInt[2] & 0xFF) == 0xDF) {
                    // Functions 21-28
                    return tr("LN_MSG_SEND_PACKET_IMM_SET_F21_TO_F28",
                            QString::number(address),
                            tr(((packetInt[3] & 0x01) != 0 ? "On" : "Off")),
                            tr(((packetInt[3] & 0x02) != 0 ? "On" : "Off")),
                            tr(((packetInt[3] & 0x04) != 0 ? "On" : "Off")),
                            tr(((packetInt[3] & 0x08) != 0 ? "On" : "Off")),
                            tr(((packetInt[3] & 0x10) != 0 ? "On" : "Off")),
                            tr(((packetInt[3] & 0x20) != 0 ? "On" : "Off")),
                            tr(((packetInt[3] & 0x40) != 0 ? "On" : "Off")),
                            tr(((packetInt[3] & 0x80) != 0 ? "On" : "Off")));
                } else if ((packetInt[2] & 0xFF) == 0xDE) {
                    // Functions 13-20
                    return tr("LN_MSG_SEND_PACKET_IMM_SET_F13_TO_F20",
                            QString::number(address),
                            tr((((packetInt[3] & 0x01) != 0) ? "On" : "Off")),
                            tr((((packetInt[3] & 0x02) != 0) ? "On" : "Off")),
                            tr((((packetInt[3] & 0x04) != 0) ? "On" : "Off")),
                            tr((((packetInt[3] & 0x08) != 0) ? "On" : "Off")),
                            tr((((packetInt[3] & 0x10) != 0) ? "On" : "Off")),
                            tr((((packetInt[3] & 0x20) != 0) ? "On" : "Off")),
                            tr((((packetInt[3] & 0x40) != 0) ? "On" : "Off")),
                            tr((((packetInt[3] & 0x80) != 0) ? "On" : "Off")));
                } else if ((packetInt[2] & 0xF0) == 0xA0) {
                    // Functions 9-12
                    return tr("LN_MSG_SEND_PACKET_IMM_SET_F9_TO_F12",
                            QString::number(address),
                            tr((((packetInt[2] & 0x01) != 0) ? "On" : "Off")),
                            tr((((packetInt[2] & 0x02) != 0) ? "On" : "Off")),
                            tr((((packetInt[2] & 0x04) != 0) ? "On" : "Off")),
                            tr((((packetInt[2] & 0x08) != 0) ? "On" : "Off")));
                } else {
                    return tr("LN_MSG_OPC_IMM_PKT_GENERIC",
                            ((reps & 0x70) >> 4),
                            (reps & 0x07),
                            reps,
                            tr("0x%1",
                                    StringUtil::twoHexFromInt(dhi)),
                            tr("0x%1",
                                    StringUtil::twoHexFromInt(im1)),
                            tr("0x%1",
                                    StringUtil::twoHexFromInt(im2)),
                            tr("0x%1",
                                    StringUtil::twoHexFromInt(im3)),
                            tr("0x%1",
                                    StringUtil::twoHexFromInt(im4)),
                            tr("0x%1",
                                    StringUtil::twoHexFromInt(im5)),
                            NmraPacket.format(packet));
                }
            } else { // F9-F28 w/a short address.
                address = packetInt[0];
                if ((packetInt[1] & 0xFF) == 0xDF) {
                    // Functions 21-28
                    return tr("LN_MSG_SEND_PACKET_IMM_SET_F21_TO_F28",
                            address,
                            tr(((packetInt[2] & 0x01) != 0 ? "On" : "Off")),
                            tr(((packetInt[2] & 0x02) != 0 ? "On" : "Off")),
                            tr(((packetInt[2] & 0x04) != 0 ? "On" : "Off")),
                            tr(((packetInt[2] & 0x08) != 0 ? "On" : "Off")),
                            tr(((packetInt[2] & 0x10) != 0 ? "On" : "Off")),
                            tr(((packetInt[2] & 0x20) != 0 ? "On" : "Off")),
                            tr(((packetInt[2] & 0x40) != 0 ? "On" : "Off")),
                            tr(((packetInt[2] & 0x80) != 0 ? "On" : "Off")));

                } else if ((packetInt[1] & 0xFF) == 0xDE) {
                    // Functions 13-20
                    return tr("LN_MSG_SEND_PACKET_IMM_SET_F13_TO_F20",
                            address,
                            tr(((packetInt[2] & 0x01) != 0 ? "On" : "Off")),
                            tr(((packetInt[2] & 0x02) != 0 ? "On" : "Off")),
                            tr(((packetInt[2] & 0x04) != 0 ? "On" : "Off")),
                            tr(((packetInt[2] & 0x08) != 0 ? "On" : "Off")),
                            tr(((packetInt[2] & 0x10) != 0 ? "On" : "Off")),
                            tr(((packetInt[2] & 0x20) != 0 ? "On" : "Off")),
                            tr(((packetInt[2] & 0x40) != 0 ? "On" : "Off")),
                            tr(((packetInt[2] & 0x80) != 0 ? "On" : "Off")));
                } else if ((packetInt[1] & 0xF0) == 0xA0) {
                    // Functions 9-12
                    return tr("LN_MSG_SEND_PACKET_IMM_SET_F9_TO_F12",
                            address,
                            tr(((packetInt[1] & 0x01) != 0 ? "On" : "Off")),
                            tr(((packetInt[1] & 0x02) != 0 ? "On" : "Off")),
                            tr(((packetInt[1] & 0x04) != 0 ? "On" : "Off")),
                            tr(((packetInt[1] & 0x08) != 0 ? "On" : "Off")));
                } else {
                    // Unknown
                    return tr("LN_MSG_OPC_IMM_PKT_GENERIC",
                            ((reps & 0x70) >> 4),
                            (reps & 0x07),
                            reps,
                            tr("0x%1",
                                    StringUtil::twoHexFromInt(dhi)),
                            tr("0x%1",
                                    StringUtil::twoHexFromInt(im1)),
                            tr("0x%1",
                                    StringUtil::twoHexFromInt(im2)),
                            tr("0x%1",
                                    StringUtil::twoHexFromInt(im3)),
                            tr("0x%1",
                                    StringUtil::twoHexFromInt(im4)),
                            tr("0x%1",
                                    StringUtil::twoHexFromInt(im5)),
                            NmraPacket.format(packet));
                }
            } // else { // F9-F28 w/a short address.
        } else if (l.getElement(1) == 0x1F) {
            if (l.getElement(2) == 0x01 && l.getElement(3) == 0x49 && l.getElement(4) == 0x42
                    && l.getElement(6) != 0x5E && l.getElement(10) == 0x70 && l.getElement(11) == 0x00 && l.getElement(15) == 0x10) {
                // Uhlenbrock IB-COM / Intellibox I and II read or write CV value on programming track
                String cv = QString::number(l.getElement(8) * 256 + ((l.getElement(5) & 0x02) * 64) + l.getElement(7));
                int val = l.getElement(9) + 16 * (l.getElement(5) & 0x08);
                switch (l.getElement(6)) {
                    case 0x6C:
                        return tr("LN_MSG_UHLEN_READ_CV_REG_MODE_FROM_PT", cv);
                    case 0x6D:
                        return tr("LN_MSG_UHLEN_WRITE_CV_REG_MODE_FROM_PT", cv);
                    case 0x6E:
                        return tr("LN_MSG_UHLEN_READ_CV_PAGED_MODE_FROM_PT", cv);
                    case 0x6F:
                        return tr("LN_MSG_UHLEN_WRITE_CV_PAGED_MODE_FROM_PT", cv);
                    case 0x71:
                        return tr("LN_MSG_UHLEN_WRITE_CV_DIRECT_BYTE_MODE_FROM_PT",
                                cv, val);
                    case 0x70: // observed on Intellibox II, even though it does not work on IB-COM
                    case 0x72:
                        return tr("LN_MSG_UHLEN_READ_CV_DIRECT_BYTE_MODE_FROM_PT", cv);
                    default:
                        break;
                }
                return "";
            } else if (l.getElement(2) == 0x01 && l.getElement(3) == 0x49 && l.getElement(4) == 0x42
                    && l.getElement(6) == 0x5E) {
                // Uhlenbrock IB-COM / Intellibox I and II write CV value on main track
                int addr = l.getElement(8) * 256 + ((l.getElement(5) & 0x02) * 64) + l.getElement(7);
                String cv = QString::number(l.getElement(11) * 256 + ((l.getElement(5) & 0x08) << 4) + l.getElement(9));
                int val = ((l.getElement(10) & 0x02) << 6) + l.getElement(12);
                return tr("LN_MSG_UHLEN_CV_OPS_MODE_WRITE",
                        addr, cv, val);
            }
        }
        return ""; // not an understood message.
    }
#endif
    /*private*/ QString LlnMon::interpretOpcPr3Mode(LocoNetMessage l) {
        /*
         * Sets the operating mode of the PR3 device, if present.
         *
         * Information reverse-engineered by B. Milhaupt and used with permission
         */

        if ((l.getElement(1) == 0x10) && ((l.getElement(2) & 0x7c) == 0)
                && (l.getElement(3) == 0) && (l.getElement(4) == 0)) {
            // set PR3 mode of operation, where LS 2 bits of byte 2 are encoded as:
            // 0x00 Set the PR3 mode to MS100 interface mode with PR3 LocoNet termination disabled
            // 0x01 Set the PR3 to decoder programming track mode
            // 0x03 Set the PR3 to MS100 interface mode with PR3 LocoNet termination enabled

            switch (l.getElement(2) & 0x3) {
                case 0x00: {
                    return tr("Set PR3 to MS100 mode without PR3 termination of LocoNet (i.e. use PR3 with command station present).");
                }
                case 0x02: {
                    return tr("Set PR3 to MS100 mode without PR3 termination of LocoNet (i.e. use PR3 with command station present).");
                }
                case 0x03: {
                    return tr("Set PR3 to MS100 mode with PR3 termination of LocoNet (i.e. use PR3 without command station present).");
                }
                default: {
                    break;
                }
            }
        }
        return "";
    }
#if 0
    /*private*/ QString interpretIb2Special(LocoNetMessage l) {
        // Intellibox function control message for mobile decoder F0-F28 (IB-I) and F13-F28 (IB-II)
        if ((l.getElement(1) == LnConstants::RE_IB2_SPECIAL_FUNCS_TOKEN)
                && ((l.getElement(3) == LnConstants::RE_IB1_SPECIAL_F5_F11_TOKEN)
                || (l.getElement(3) == LnConstants::RE_IB2_SPECIAL_F13_F19_TOKEN)
                || (l.getElement(3) == LnConstants::RE_IB2_SPECIAL_F21_F27_TOKEN))) {
            // Intellibox-I function control message for mobile decoder F5 thru F27 except F12 and F20
            // Intellibox-II function control message for mobile decoder F13 thru F27 except F20
            // Note: Intellibox-II documentation implies capability to control
            // MANY more functions.  This capability may be extended by
            // additional tokens in element 3, including the special-case encoding
            // for the "eighth bit" as handled in the following case, below,
            // for F12, F20 & F28
            int funcOffset = 5 + 8 * (l.getElement(3) - LnConstants::RE_IB1_SPECIAL_F5_F11_TOKEN);
            String encodingType;
            if (l.getElement(3) == LnConstants::RE_IB1_SPECIAL_F5_F11_TOKEN) {
                encodingType = tr("LN_MSG_INTELLIBOX_FUNC_CTL_HELPER_IB1");
            } else {
                encodingType = tr("LN_MSG_INTELLIBOX_FUNC_CTL_HELPER_IB2");
            }
            String funcInfo[] = new String[7];
            int mask = 1;
            for (int i = 0; i < 7; i++) {
                // handle 7 bits of data
                funcInfo[i] = tr("LN_MSG_INTELLIBOX_FUNC_CTL_HELPER_INDIV_FUNC",
                        funcOffset + i,
                        tr(((l.getElement(4) & mask) != 0)
                                ? "On"
                                : "Off"));
                mask *= 2;
            }
            return tr("LN_MSG_INTELLIBOX_FUNC_CTL",
                    encodingType, l.getElement(2), funcInfo[0],
                    funcInfo[1], funcInfo[2], funcInfo[3],
                    funcInfo[4], funcInfo[5], funcInfo[6]);
        } else if ((l.getElement(1) == LnConstants::RE_IB2_SPECIAL_FUNCS_TOKEN)
                && (l.getElement(3) == LnConstants::RE_IB2_SPECIAL_F20_F28_TOKEN)) {
            // Special-case for F12, F20 and F28, since the tokens from the previous case
            // can only encode 7 bits of data in element(4).
            return tr("LN_MSG_INTELLIBOX_SPECIAL_FUNC_CTL",
                    l.getElement(2),
                    tr(((l.getElement(4) & LnConstants::RE_IB2_SPECIAL_F12_MASK) != 0)
                            ? "On"
                            : "Off"),
                    tr(((l.getElement(4) & LnConstants::RE_IB2_SPECIAL_F20_MASK) != 0)
                            ? "On"
                            : "Off"),
                    tr(((l.getElement(4) & LnConstants::RE_IB2_SPECIAL_F28_MASK) != 0)
                            ? "On"
                            : "Off"));
        } else if ((l.getElement(1) == LnConstants::RE_IB2_SPECIAL_FUNCS_TOKEN)
                && (l.getElement(3) == LnConstants::RE_IB1_SPECIAL_F0_F4_TOKEN)) {
            // For Intellibox-I "one" with SW version 2.x - Special-case for F0 to F4
            String funcInfo[] = new String[7];
            funcInfo[0] = tr("LN_MSG_INTELLIBOX_FUNC_CTL_HELPER_INDIV_FUNC",
                    0,
                    (l.getElement(4) & LnConstants::RE_IB1_F0_MASK) == 0 ? tr("On")
                    : tr("Off"));
            int mask = 1;
            for (int i = 0; i < 4; i++) {
                // handle 7 bits of data
                funcInfo[i + 1] = tr("LN_MSG_INTELLIBOX_FUNC_CTL_HELPER_INDIV_FUNC",
                        i + 1,
                        tr(((l.getElement(4) & mask) != 0)
                                ? "On"
                                : "Off"));
                mask *= 2;
            }
            return tr("LN_MSG_INTELLIBOX_FUNC_CTL_F0_TO_F4",
                    l.getElement(2),
                    funcInfo[0], funcInfo[1], funcInfo[2], funcInfo[3],
                    funcInfo[4]);
        }
        // Because the usage of other tokens in message element(3) are not yet
        // understood, let execution fall thru to the "default" case
        return "";
    }

    /*private*/ QString interpretIb2F9_to_F12(LocoNetMessage l) {
        // Intellibox-II function control message for mobile decoder F9 thru F12.
        int slot = l.getElement(1);
        int funcs = l.getElement(2);
        return tr("LN_MSG_INTELLIBOX_SLOT_SET_F9_TO_F12",
                slot,
                tr(((funcs & LnConstants::RE_IB2_F9_MASK) != 0 ? "On" : "Off")),
                tr(((funcs & LnConstants::RE_IB2_F10_MASK) != 0 ? "On" : "Off")),
                tr(((funcs & LnConstants::RE_IB2_F11_MASK) != 0 ? "On" : "Off")),
                tr(((funcs & LnConstants::RE_IB2_F12_MASK) != 0 ? "On" : "Off")));
    }
#endif
    /*private*/ /*static*/ /*final*/ QList<QString> LlnMon::ds54sensors = QList<QString>() <<"AuxA"<< "SwiA"<< "AuxB"<< "SwiB"<< "AuxC"<< "SwiC"<< "AuxD"<< "SwiD";    // NOI18N
    /*private*/ /*static*/ /*final*/ QList<QString>  LlnMon::ds64sensors = QList<QString>()<<"A1"<< "S1"<< "A2"<< "S2"<< "A3"<< "S3"<< "A4"<< "S4";                    // NOI18N
    /*private*/ /*static*/ /*final*/ QList<QString>  LlnMon::se8csensors = QList<QString>()<<"DS01"<< "DS02"<< "DS03"<< "DS04"<< "DS05"<< "DS06"<< "DS07"<< "DS08";    // NOI18N

    /*private*/ /*final*/ /*static*/ Logger* LlnMon::log = LoggerFactory::getLogger("Llnmon");
