#ifndef TRIGGERROUTEMODEXML_H
#define TRIGGERROUTEMODEXML_H
#include "abstractxmladapter.h"

class Logger;
class TriggerRouteModelXml : public AbstractXmlAdapter
{
public:
 TriggerRouteModelXml(QObject* parent = 0);
 ~TriggerRouteModelXml() {}
 TriggerRouteModelXml(const TriggerRouteModelXml&) : AbstractXmlAdapter() {}
 /*public*/ QDomElement store(QObject* o);
 /*public*/ bool loadDeferred();
 /*public*/ bool load(QDomElement shared, QDomElement perNode) throw (JmriException);
 /*public*/ void load(QDomElement element, QObject* o)throw (Exception) ;

private:
 /*private*/ /*final*/ static Logger* log;// = LoggerFactory::getLogger("TriggerRouteModelXml");

};
Q_DECLARE_METATYPE(TriggerRouteModelXml)
#endif // TRIGGERROUTEMODEXML_H
