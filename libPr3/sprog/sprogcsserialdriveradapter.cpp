#include "sprogcsserialdriveradapter.h"
#include "sprogconstants.h"

using namespace Sprog;
/**
 * Implements SerialPortAdapter for the Sprog system.
 * <P>
 * This connects an Sprog command station via a serial com port. Also used for
 * the USB SPROG, which appears to the computer as a serial port.
 * <P>
 * The current implementation only handles the 9,600 baud rate, and does not use
 * any other options at configuration time.
 *
 * @author	Andrew Crosland Copyright (C) 2006
 */
// /*public*/ class SprogCSSerialDriverAdapter
//        extends jmri.jmrix.sprog.serialdriver.SerialDriverAdapter {

/*public*/ SprogCSSerialDriverAdapter::SprogCSSerialDriverAdapter(QObject* parent)
 : SerialDriverAdapter(SprogConstants::OPS, parent)
{
    //super(SprogMode.OPS);
    options.insert("TrackPowerState", new Option(tr("Track Power At StartUp:"),
            QStringList()<<tr("Off") << tr("On"),
            true)); // first element (TrackPowerState) NOI18N
    //Set the username to match name, once refactored to handle multiple connections or user setable names/prefixes then this can be removed
    this->getSystemConnectionMemo()->setUserName(tr("SPROG Command Station"));
}

/**
 * @deprecated JMRI Since 4.4 instance() shouldn't be used, convert to JMRI multi-system support structure
 */
//@Deprecated
/*static*/ /*public*/ SprogCSSerialDriverAdapter* SprogCSSerialDriverAdapter::instance() {
    return NULL;
}