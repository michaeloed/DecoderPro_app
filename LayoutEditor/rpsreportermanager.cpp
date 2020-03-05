#include "rpsreportermanager.h"
#include "rpsreporter.h"
#include "distributor.h"
#include "rpssystemconnectionmemo.h"

RpsReporterManager::RpsReporterManager(RpsSystemConnectionMemo* memo, QObject* parent) : AbstractReporterManager(memo, parent)
{

}
/**
 * RPS implementation of a ReporterManager.
 *
 * @author	Bob Jacobsen Copyright (C) 2008
  * @since 2.3.1
 */
// /*public*/ class RpsReporterManager extends AbstractReporterManager {

//@Override
/*public*/ QString RpsReporterManager::getSystemPrefix()const {
    return "R";
}

//@Override
/*protected*/ Reporter* RpsReporterManager::createNewReporter(QString systemName, QString userName) const {
    RpsReporter* r = new RpsReporter(systemName, userName);
    //Distributor::instance()->addMeasurementListener(r);
    connect(Distributor::instance(), SIGNAL(newMeasurement(Measurement*)), r, SLOT(notify(Measurement*)));
    return (Reporter*)r;
}

///*static*/ /*public*/ RpsReporterManager* RpsReporterManager::instance() {
//    if (_instance == NULL) {
//        _instance = new RpsReporterManager(this);
//    }
//    return _instance;
//}

///*static*/ RpsReporterManager* RpsReporterManager::_instance = NULL;

/**
 * {@inheritDoc}
 */
//@Override
/*public*/ SystemConnectionMemo *RpsReporterManager::getMemo() {
    return  memo;
}
#if 0
static { // class initialization
    // now want a ReporterManager always, not just when RPS is created
    if (_instance == null) {
        _instance = new RpsReporterManager();
        jmri.InstanceManager.setReporterManager(jmri.jmrix.rps.RpsReporterManager.instance());
        // log.warn("Setting RpsSensorManager instance at startup time!");
    }
}
#endif
