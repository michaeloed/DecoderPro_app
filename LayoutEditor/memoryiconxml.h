#ifndef MEMORYICONXML_H
#define MEMORYICONXML_H
#include "positionablelabelxml.h"

class MemoryIcon;
class MemoryIconXml : public PositionableLabelXml
{
    Q_OBJECT
public:
    explicit MemoryIconXml(QObject *parent = 0);
    /*public*/ QDomElement store(QObject* o);
    /*public*/ bool load(QDomElement element) throw (Exception);
    /*public*/ void load(QDomElement element, QObject* o)throw (Exception);
    MemoryIcon* getIcon();
signals:

public slots:
private:
 Logger* log;
 MemoryIcon* l;
};

#endif // MEMORYICONXML_H
