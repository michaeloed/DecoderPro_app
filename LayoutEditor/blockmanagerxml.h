#ifndef BLOCKMANAGERXML_H
#define BLOCKMANAGERXML_H
#include "abstractmemorymanagerconfigxml.h"
#include "liblayouteditor_global.h"

class Block;
class BeanSetting;
class Path;
class LIBLAYOUTEDITORSHARED_EXPORT BlockManagerXml : public AbstractMemoryManagerConfigXML
{
    Q_OBJECT
public:
    explicit BlockManagerXml(QObject *parent = 0);
    ~BlockManagerXml();
 BlockManagerXml(const BlockManagerXml&) : AbstractMemoryManagerConfigXML() {}
    /*public*/ void setStoreElementClass(QDomElement memories);
    /*public*/ QDomElement store(QObject* o);
    void addPath(QDomElement e, Path* p);
    void addBeanSetting(QDomElement e, BeanSetting* bs);
    /*public*/ bool load(QDomElement blocks) throw (JmriConfigureXmlException);
    /*public*/ void loadBlock(QDomElement element) throw (JmriConfigureXmlException);
    /*public*/ void loadPath(Block* block, QDomElement element) throw (JmriConfigureXmlException);
    /*public*/ void loadBeanSetting(Path* path, QDomElement element);
    /*public*/ int loadOrder();

signals:

public slots:
private:
 Logger* log;
};
Q_DECLARE_METATYPE(BlockManagerXml)
#endif // BLOCKMANAGERXML_H
