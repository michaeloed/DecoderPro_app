#include "model.h"

/**
 * Model holds RPS representation of layout geometry and logical structure.
 * <p>
 * The layout geometry is expressed as a set of (perhaps overlapping) Regions.
 *
 * @since 2.1.7
 * @author	Bob Jacobsen Copyright (C) 2008
 */
///*public*/ class Model {

Model::Model(QObject *parent) : QObject(parent)
{
 regions = new QList<Region*>();
}

void Model::loadValues() {
    // load dummy contents
    setInitialModel();
}

/*public*/ void Model::dispose() {
}


/**
 * Include a region in the model
 */
/*public*/ void Model::addRegion(Region* r) {
    regions->append(r);
}

/**
 * Include a region in the model
 */
/*public*/ void Model::removeRegion(Region* r) {
    regions->removeOne(r);
}
#if 0
/**
 * Get the list of active regions.
 * <p>
 * This list should be immutable, hence copied, but for now it's not.
 */
/*public*/ List<Region> getRegions() {
    return regions;
}

// Store model info
/*public*/ void storeModel(File file) { //throws org.jdom2.JDOMException, IOException {
/*         PositionFile pf = new PositionFile(); */
    /*         pf.prepare(); */
    /*         pf.setConstants(getVSound(), getOffset()); */
    /*          */
    /*         for (int i = 1; i<=getReceiverCount(); i++) { */
    /*             if (getReceiver(i) == null) continue; */
    /*             pf.setReceiver(i, getReceiver(i)); */
    /*         } */
    /*         pf.store(file); */
}

/*public*/ void loadModel(File file) { // throws org.jdom2.JDOMException, IOException {
/*         // start by getting the file */
    /*         PositionFile pf = new PositionFile(); */
    /*         pf.loadFile(file); */
    /*          */
    /*         // get VSound */
    /*         setVSound(pf.getVSound()); */
    /*          */
    /*         // get offset */
    /*         setOffset(pf.getOffset()); */
    /*          */
    /*         // get receivers */
    /*         setReceiverCount(pf.maxReceiver());  // count from 1 */
    /*         Point3d p; */
    /*         for (int i = 1; i<=getReceiverCount(); i++) {     */
    /*             p = pf.getReceiverPosition(i); */
    /*             if (p == null) continue; */
    /*             log.debug("load "+i+" with "+p); */
    /*             setReceiver(i, new Receiver(p)); */
    /*         } */
}
#endif
void Model::setInitialModel() {
    /*         File defaultFile = new File(PositionFile.defaultFilename()); */
    /*         try { */
    /*             loadAlignment(defaultFile); */
    /*         } catch (Exception e) { */
    /*             log.debug("load exception"+e); */
    /*             // load dummy values */
    /*             setReceiverCount(2); */
    /*             setReceiver(0, new Receiver(new Point3d(0.0,0.0,72.0))); */
    /*             setReceiver(1, new Receiver(new Point3d(72.0,0.0,72.0))); */
    /*         }                 */
}

// for now, we only allow one Model
/*static*/ Model* Model::_instance = NULL;

/*static*/ /*public*/ Model* Model::instance() {
    if (_instance == NULL) {
        Model* i = new Model();
        i->loadValues();
        // don't expose until initialized
        _instance = i;
    }
    return _instance;
}