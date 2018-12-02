#ifndef DEFAULTSIGNALMASTANAGERXML_H
#define DEFAULTSIGNALMASTANAGERXML_H
#include "abstractnamedbeanmanagerconfigxml.h"
#include "liblayouteditor_global.h"

class SignalMastRepeater;
class DefaultSignalMastManager;
class LIBLAYOUTEDITORSHARED_EXPORT DefaultSignalMastManagerXml : public AbstractNamedBeanManagerConfigXML
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit DefaultSignalMastManagerXml(QObject *parent = 0);
    ~DefaultSignalMastManagerXml() {}
    DefaultSignalMastManagerXml(const DefaultSignalMastManagerXml&) : AbstractNamedBeanManagerConfigXML() {}
    /*public*/ QDomElement store(QObject* o);
    /*public*/ void setStoreElementClass(QDomElement signalMastLogic);
    /*public*/ bool load(QDomElement shared, QDomElement perNode);
    /*public*/ void load(QDomElement element, QObject* o) throw (Exception);
    /*public*/ int loadOrder();

signals:

public slots:
private:
    Logger* log;
};
Q_DECLARE_METATYPE(DefaultSignalMastManagerXml)
#endif // DEFAULTSIGNALMASTANAGERXML_H
