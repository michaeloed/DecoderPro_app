#ifndef LOCONETLISTENER_H
#define LOCONETLISTENER_H

#include <QObject>
#include "loconetmessage.h"
/**
 * LocoNetListener provides the call-back interface for notification when a
 * new LocoNet message arrives from the layout.
 *<P>
 * Note that the LocoNetListener implementation cannot assume that messages will
 * be returned in any particular thread. We may eventually revisit this, as returning
 * messages in the Swing GUI thread would result in some simplification of client code.
 * We've not done that yet because we're not sure that deadlocks can be avoided in that
 * case.
 *
 * @author			Bob Jacobsen  Copyright (C) 2001
 * @version			$Revision: 17977 $
 */
class LIBPR3SHARED_EXPORT LocoNetListener
{
public:
 explicit LocoNetListener(QObject *parent = 0);
/**
 * Member function that will be invoked by a LocoNetInterface implementation
 * to forward a LocoNet message from the layout.
 *
 * @param msg  The received LocoNet message.  Note that this same object
 *             may be presented to multiple users. It should not be
 *             modified here.
 */

 virtual void message(LocoNetMessage* msg) =0;
signals:
    
public slots:
    
};

#endif // LOCONETLISTENER_H
