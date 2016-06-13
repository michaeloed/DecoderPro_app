#ifndef GUILAFCONFIGPANELXML_H
#define GUILAFCONFIGPANELXML_H
#include "abstractxmladapter.h"
#include "libpref_global.h"

class LIBPREFSHARED_EXPORT GuiLafConfigPaneXml : public AbstractXmlAdapter
{
    Q_OBJECT
public:
    explicit GuiLafConfigPaneXml(QObject *parent = 0);
    ~GuiLafConfigPaneXml() {}
    GuiLafConfigPaneXml(const GuiLafConfigPaneXml&) : AbstractXmlAdapter() {}
    /*public*/ QDomElement store(QObject* o);
    /*public*/ bool load(QDomElement e) throw (Exception);
    /*public*/ void updateLookAndFeel(QString name, QString className);
    /*public*/ void load(QDomElement element, QObject* o) throw (Exception);

signals:

public slots:
private:
    Logger* log;
};
Q_DECLARE_METATYPE(GuiLafConfigPaneXml)
#endif // GUILAFCONFIGPANELXML_H
