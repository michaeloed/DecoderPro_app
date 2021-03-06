#ifndef WARRANTMANAGERXML_H
#define WARRANTMANAGERXML_H
#include "abstractxmladapter.h"
#include "liblayouteditor_global.h"

class ThrottleSetting;
class BlockOrder;
class Warrant;
class LIBLAYOUTEDITORSHARED_EXPORT WarrantManagerXml : public AbstractXmlAdapter
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit WarrantManagerXml(QObject *parent = 0);
 ~WarrantManagerXml()  override{}
 WarrantManagerXml(const WarrantManagerXml&) : AbstractXmlAdapter() {}
    /*public*/ QDomElement store(QObject* o);
    QDomElement storeTrain(Warrant* warrant, QString type);
    QDomElement storeOrder(BlockOrder* order, QString type);
    QDomElement storeCommand(ThrottleSetting* command, QString type);
    /*public*/ bool load(QDomElement warrants) throw (Exception) override;
    /*public*/ void load(QDomElement element, QObject* o) throw (Exception) override;
    void loadTrain(QDomElement elem, Warrant* warrant);
    BlockOrder* loadBlockOrder(QDomElement elem);
    ThrottleSetting* loadThrottleCommand(QDomElement elem);
    /*public*/ int loadOrder() const override;

signals:

public slots:
private:
    static Logger* log;
};
Q_DECLARE_METATYPE(WarrantManagerXml)
#endif // WARRANTMANAGERXML_H
