#include "nmraconsistmanager.h"
#include "consist.h"
#include "dcclocoaddress.h"
#include "nmraconsist.h"

NmraConsistManager::NmraConsistManager(CommandStation* cs, QObject *parent) :
    AbstractConsistManager()
{
 commandStation = cs;
}
/**
 * NmraConsistManager.java
 *
 * Description: The Default Consist Manager which uses the NmraConsist class for
 * the consists it builds
 *
 * @author Paul Bender Copyright (C) 2003
 * @author Randall Wood Copyright (C) 2013
 * @version $Revision: 23541 $
 */
//package jmri.implementation;

//import jmri.Consist;
//import jmri.ConsistManager;
//import jmri.DccLocoAddress;

// /*public*/ class NmraConsistManager extends DccConsistManager implements ConsistManager {

///*public*/ NmraConsistManager() {
//    super();
//}

//@Override
/*public*/ DccConsist* NmraConsistManager::addConsist(DccLocoAddress* address) {
    if (consistTable->contains(address)) {
        return (consistTable->value(address));
    }
    NmraConsist* consist;
    consist = new NmraConsist(address, commandStation);
    consistTable->insert(address, consist);
    return (consist);
}
