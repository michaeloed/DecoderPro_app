#include "enginemanagerxml.h"
#include "engine.h"
#include "enginemanager.h"
#include "control.h"
#include "logger.h"
#include "file.h"
#include "enginemodels.h"
#include "locationmanagerxml.h"
#include "enginelengths.h"
#include "enginetypes.h"
#include "instancemanager.h"
//#include "locationmanager.h"
#include "operationssetupxml.h"

//EngineManagerXml::EngineManagerXml(QObject *parent) :
//  OperationsXml(parent)
//{
//}
namespace Operations
{
/**
 * Loads and stores engines using xml files. Also loads and stores engine
 * models, engine types, engine lengths, and engine consist names.
 *
 * @author Daniel Boudreau Copyright (C) 2008
 * @version $Revision: 29493 $
 */
///*public*/ class EngineManagerXml extends OperationsXml {

 /*public*/ EngineManagerXml::EngineManagerXml(QObject *parent) :
  OperationsXml(parent)
 {
  log = new Logger("EngineManagerXml");
  operationsFileName = "OperationsEngineRoster.xml"; // NOI18N
  setProperty("InstanceManagerAutoDefault", "true");
  setProperty("InstanceManagerAutoInitialize", "true");

 }

 /**
  * record the single instance *
  */
// /*private*/ /*static*/ EngineManagerXml*  EngineManagerXml::_instance = NULL;

 /*public*/ /*static*/ /*synchronized*/ EngineManagerXml* EngineManagerXml::instance()
 {
  return static_cast<EngineManagerXml*>(InstanceManager::getDefault("EngineManagerXml"));
 }

 /*public*/ void EngineManagerXml::writeFile(QString name) //throws java.io.FileNotFoundException, java.io.IOException
 {
     if (log->isDebugEnabled()) {
         log->debug(tr("writeFile %1").arg(name));
     }
     // This is taken in large part from "Java and XML" page 368
     File* file = new File(findFile(name)->fileName());
     if (file == NULL) {
         file = new File(name);
     }
     // create root element
     QDomDocument doc = QDomDocument();
     //QDomElement root = new Element("operations-config"); // NOI18N
//     QDomElement root = QDomElement();
//     root.setTagName("operations-config");
//     QDomDocument doc = newDocument(root, dtdLocation + "operations-engines.dtd"); // NOI18N
     QDomProcessingInstruction xmlProcessingInstruction = doc.createProcessingInstruction("xml", "version=\"1.0\"  encoding=\"UTF-8\"");
     doc.appendChild(xmlProcessingInstruction);
     QDomElement root = doc.createElement("operations-config");
     // add XSLT processing instruction
//     QMap<QString, QString> m = new QMap<QString, QString>();
//     m.put("type", "text/xsl"); // NOI18N
//     m.put("href", xsltLocation + "operations-engines.xsl"); // NOI18N
//     ProcessingInstruction p = new ProcessingInstruction("xml-stylesheet", m); // NOI18N
//     doc.addContent(0, p);
       QDomProcessingInstruction p = doc.createProcessingInstruction("type", "text/xsl");
       root.appendChild(p);
       p = doc.createProcessingInstruction("href", tr("%1operations-operations-engines.xsl").arg(xsltLocation));
      root.appendChild(p);
      doc.appendChild(root);
     EngineModels::instance()->store(root, doc);
     EngineTypes::instance()->store(root, doc);
     EngineLengths::instance()->store(root, doc);
     EngineManager::instance()->store(root,doc);

     writeXML(file, doc);

     // done - engine file now stored, so can't be dirty
     setDirty(false);
 }

 /**
  * Read the contents of a roster XML file into this object. Note that this
  * does not clear any existing entries.
  */
 //@Override
 /*public*/ void EngineManagerXml::readFile(QString name) //throws org.jdom2.JDOMException, java.io.IOException
 {
     // suppress rootFromName(name) warning message by checking to see if file exists
     if (findFile(name) == NULL) {
         log->debug(tr("%1 file could not be found").arg(name));
         return;
     }
     // find root
     QDomElement root = rootFromName(name);
     if (root == QDomElement()) {
         log->debug(tr("%1 file could not be read").arg(name));
         return;
     }

     EngineModels::instance()->load(root);
     EngineTypes::instance()->load(root);
     EngineLengths::instance()->load(root);
     EngineManager::instance()->load(root);

     log->debug("Engines have been loaded!");
//     RollingStockLogger.instance().enableEngineLogging(Setup.isEngineLoggerEnabled());
     // clear dirty bit
     setDirty(false);
     // clear location dirty flag, locations get modified during the loading of cars and locos
     //LocationManagerXml::instance()->setDirty(false);
     static_cast<LocationManagerXml*>(InstanceManager::getDefault("LocationManagerXml"))->setDirty(false);
 }

 /*public*/ void EngineManagerXml::setOperationsFileName(QString name) {
     operationsFileName = name;
 }

 /*public*/ QString EngineManagerXml::getOperationsFileName() {
     return operationsFileName;
 }

 //@Override
 /*public*/ void EngineManagerXml::initialize() {
     load();
 }

 /*public*/ void EngineManagerXml::dispose(){
//     _instance = NULL;
 }


} // end namespace
