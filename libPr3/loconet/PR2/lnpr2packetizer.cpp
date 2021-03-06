#include "lnpr2packtizer.h"
#include "loconetsystemconnectionmemo.h".

/**
 * Special LnPr2Packetizer implementation for PR2.
 * Differs only in handling PR2's non-echo.
 *
 * @author Bob Jacobsen Copyright (C) 2006
 */
// /*public*/ class LnPr2Packetizer extends LnPacketizer {

//@SuppressFBWarnings(value = "ST_WRITE_TO_STATIC_FROM_INSTANCE_METHOD", // NOI18N
//        justification = "Only used during system initialization") // NOI18N
/*public*/ LnPr2Packetizer::LnPr2Packetizer(QObject* parent)
 : LnPacketizer(new LocoNetSystemConnectionMemo(), parent) {
    //super();
    //self = this;
    echo = true;
}

/*public*/ LnPr2Packetizer::LnPr2Packetizer(LocoNetSystemConnectionMemo* memo, QObject* parent)
 : LnPacketizer(memo, parent)
{
 echo = true;
}


//    private final static Logger log = LoggerFactory.getLogger(LnPr2Packetizer.class);


