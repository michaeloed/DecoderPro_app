#ifndef DEFAULTCONDITIONALMANAGERXML_H
#define DEFAULTCONDITIONALMANAGERXML_H
#include "abstractnamedbeanmanagerconfigxml.h"
#include "liblayouteditor_global.h"

class LIBLAYOUTEDITORSHARED_EXPORT DefaultConditionalManagerXml : public AbstractNamedBeanManagerConfigXML
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit DefaultConditionalManagerXml(QObject *parent = 0);
 ~DefaultConditionalManagerXml() {}
 DefaultConditionalManagerXml(const DefaultConditionalManagerXml&) :  AbstractNamedBeanManagerConfigXML() {}
    /*public*/ QDomElement store(QObject* o);
    /*public*/ void setStoreElementClass(QDomElement conditionals);
    /*public*/ void load(QDomElement element, QObject* o) throw (Exception) override;
    /*public*/ bool load(QDomElement conditionals)  throw (Exception) override;
    /*public*/ void loadConditionals(QDomElement conditionals);
    /*public*/ int loadOrder()const override;

signals:

public slots:
private:
 Logger* log;
 /*private*/ QString attributes(QDomElement e);

protected:
 /*protected*/ void replaceConditionalManager();

};
Q_DECLARE_METATYPE(DefaultConditionalManagerXml)
#endif // DEFAULTCONDITIONALMANAGERXML_H
