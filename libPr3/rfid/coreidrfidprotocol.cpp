#include "coreidrfidprotocol.h"
#include "abstractmrreply.h"
#include "loggerfactory.h"

/**
 * Common routines to extract the Tag information and validate checksum for
 * implementations that use the CORE-ID / ID-Innovations protocol.
 * <hr>
 * This file is part of JMRI.
 * <P>
 * JMRI is free software; you can redistribute it and/or modify it under the
 * terms of version 2 of the GNU General  public License as published by the Free
 * Software Foundation. See the "COPYING" file for a copy of this license.
 * <P>
 * JMRI is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General  public License for more details.
 * <P>
 *
 * @author Matthew Harris Copyright (C) 2014
 */
 // /*public*/ class CoreIdRfidProtocol extends RfidProtocol {

/*private*/ /*static*/ /*final*/ int CoreIdRfidProtocol::SPECIFICMAXSIZE = 16;

/**
 * Constructor for CORE-ID RFID Protocol. Used when a single reader is
 * connected directly to a port, not via a concentrator.
 */
 /*public*/ CoreIdRfidProtocol::CoreIdRfidProtocol(QObject* parent) :RfidProtocol(parent){
    //super();
}

/**
 * Constructor for CORE-ID RFID Protocol. Supports the use of concentrators
 * where a character range is used to determine the specific reader port.
 *
 * @param concentratorFirst - character representing first concentrator port
 * @param concentratorLast  - character representing last concentrator port
 * @param portPosition      - position of port character in reply string
 */
 /*public*/ CoreIdRfidProtocol::CoreIdRfidProtocol(char concentratorFirst, char concentratorLast, int portPosition, QObject *parent) : RfidProtocol(concentratorFirst, concentratorLast, portPosition, parent){
    //super(concentratorFirst, concentratorLast, portPosition);
}

 /*public*/ /*static*/ /*final*/ int CoreIdRfidProtocol::getMaxSize() {
    return SPECIFICMAXSIZE;
}

//@Override
 /*public*/ QString CoreIdRfidProtocol::initString() {
    // None required for CORE-ID
    return "";
}

//@Override
 /*public*/ QString CoreIdRfidProtocol::getTag(AbstractMRReply*msg) {
    QString sb;// = new StringBuilder(10);

    for (int i = 1; i < 11; i++) {
        sb.append((char) msg->getElement(i));
    }

    return sb;
}

//@Override
 /*public*/ bool CoreIdRfidProtocol::providesChecksum() {
    return true;
}

//@Override
 /*public*/ QString CoreIdRfidProtocol::getCheckSum(AbstractMRReply*msg) {
    QString sb;//= new StringBuilder(2);

    for (int i = 11; i < 13; i++) {
        sb.append((char) msg->getElement(i));
    }

    return sb/*.toString()*/;
}

//@Override
 /*public*/ bool CoreIdRfidProtocol::isValid(AbstractMRReply*msg) {
    return (((!isConcentrator && msg->getElement(0) == 0x02
            && (msg->getElement(SPECIFICMAXSIZE - 1) & 0xFF) == 0x03)
            || (isConcentrator
            && msg->getElement(portPosition) >= concentratorFirst
            && msg->getElement(portPosition) <= concentratorLast
            && (msg->getElement(SPECIFICMAXSIZE - 1) & 0xFF) == 0x3E))
            && (msg->getElement(SPECIFICMAXSIZE - 2) & 0xFF) == 0x0A
            && (msg->getElement(SPECIFICMAXSIZE - 3) & 0xFF) == 0x0D
            && isCheckSumValid(msg));
}

 /*public*/ bool CoreIdRfidProtocol::isCheckSumValid(AbstractMRReply*msg) {
    QByteArray tag = convertHexString(getTag(msg));
    int checksum = 0;
    for (int i = 0; i < 5; i++) {
        checksum = checksum ^ tag[i];
        log->debug("read " + QString::number(tag[i]));
    }
    log->debug("Checksum: " + getCheckSum(msg) + " converted: " + convertHexString(getCheckSum(msg))[0]);
    return checksum == convertHexString(getCheckSum(msg))[0];
}

//@Override
 /*public*/ bool CoreIdRfidProtocol::endOfMessage(AbstractMRReply* msg) {
    if (msg->getNumDataElements() == SPECIFICMAXSIZE) {
        if (((msg->getElement(SPECIFICMAXSIZE - 1) & 0xFF) == 0x03
                || (msg->getElement(SPECIFICMAXSIZE - 1) & 0xFF) == 0x3E)
                && (msg->getElement(SPECIFICMAXSIZE - 2) & 0xFF) == 0x0A
                && (msg->getElement(SPECIFICMAXSIZE - 3) & 0xFF) == 0x0D) {
            return true;
        }
        if (log->isDebugEnabled()) {
            log->debug("Not a correctly formed message");
        }
        return true;
    } else if (isConcentrator && (msg->getNumDataElements() == 1) && (msg->getElement(0) & 0xFF) == 0x3E) {
        log->debug(tr("Init message from Concentrator: %1").arg(msg->toString()));
        return true;
    }
    return false;
}

//@Override
 /*public*/ QString CoreIdRfidProtocol::toMonitorString(AbstractMRReply*msg) {
    // check for valid message
    if (isValid(msg)) {
        QString sb;// = new StringBuilder();
        sb.append("Reply from CORE-ID reader.");
        if (isConcentrator) {
            sb.append(" Reply from port ");
            sb.append(getReaderPort(msg));
        }
        sb.append(" Tag read ");
        sb.append(getTag(msg));
        sb.append(" checksum ");
        sb.append(getCheckSum(msg));
        sb.append(" valid? ");
        sb.append(isCheckSumValid(msg) ? "yes" : "no");
        return sb/*.toString()*/;
    } else {
        return RfidProtocol::toMonitorString(msg);
    }
}

/*private*/ /*static*/ /*final*/ Logger* CoreIdRfidProtocol::log = LoggerFactory::getLogger("CoreIdRfidProtocol");


