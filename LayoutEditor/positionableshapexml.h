#ifndef POSITIONABLESHAPEXML_H
#define POSITIONABLESHAPEXML_H
#include "abstractxmladapter.h"
#include "liblayouteditor_global.h"

class PositionableShape;
class LIBLAYOUTEDITORSHARED_EXPORT PositionableShapeXml : public AbstractXmlAdapter
{
    Q_OBJECT
public:
    explicit PositionableShapeXml(QObject *parent = 0);
    /*public*/ QDomElement store(QObject* o);
    /*public*/ void storeCommonAttributes(PositionableShape* p, QDomElement element);
    /*public*/ QDomElement storeColor(QString name, QColor c);
    /*public*/ bool load(QDomElement element) throw (Exception);
    /*public*/ void load(QDomElement element, QObject* o) throw (Exception);
    /*public*/ void loadCommonAttributes(PositionableShape* ps, int defaultLevel, QDomElement element);
    /*public*/ QColor getColor(QDomElement element, QString name);
    /*public*/ int getInt(QDomElement element, QString name);

signals:

public slots:
private:
    Logger* log;
};

#endif // POSITIONABLESHAPEXML_H
