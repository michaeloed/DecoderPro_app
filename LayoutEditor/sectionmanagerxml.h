#ifndef SECTIONMANAGERXML_H
#define SECTIONMANAGERXML_H
#include "abstractnamedbeanmanagerconfigxml.h"
#include "liblayouteditor_global.h"

class LIBLAYOUTEDITORSHARED_EXPORT SectionManagerXml : public AbstractNamedBeanManagerConfigXML
{
    Q_OBJECT
public:
    explicit SectionManagerXml(QObject *parent = 0);
 ~SectionManagerXml() {}
 SectionManagerXml(const SectionManagerXml&) : AbstractNamedBeanManagerConfigXML() {}
    /*public*/ QDomElement store(QObject* o);
    /*public*/ void setStoreElementClass(QDomElement sections);
    /*public*/ void load(QDomElement element, QObject* o)  throw (Exception);
    /*public*/ bool load(QDomElement sections) throw (Exception);
    /*public*/ void loadSections(QDomElement sections);
    /*public*/ int loadOrder();

signals:

public slots:
private:
    Logger* log;

};
Q_DECLARE_METATYPE(SectionManagerXml)
#endif // SECTIONMANAGERXML_H
