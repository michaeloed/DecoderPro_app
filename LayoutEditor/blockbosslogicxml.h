#ifndef BLOCKBOSSLOGICXML_H
#define BLOCKBOSSLOGICXML_H
#include "abstractxmladapter.h"
#include "liblayouteditor_global.h"

class LIBLAYOUTEDITORSHARED_EXPORT BlockBossLogicXml : public AbstractXmlAdapter
{
    Q_OBJECT
public:
    explicit BlockBossLogicXml(QObject *parent = 0);
    ~BlockBossLogicXml();
 BlockBossLogicXml(const BlockBossLogicXml&) : AbstractXmlAdapter() {}
    /*public*/ QDomElement store(QObject* o);
    /*public*/ bool load(QDomElement element) throw (Exception);
    /*public*/ void load(QDomElement element, QObject* o) throw (Exception);
    /*public*/ int loadOrder();

signals:

public slots:
private:
    Logger * log;
    QDomElement storeSensor(QString name);

};

#endif // BLOCKBOSSLOGICXML_H
