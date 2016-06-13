#ifndef CONTROLPANELEDITORXML_H
#define CONTROLPANELEDITORXML_H
#include "abstractxmladapter.h"
#include "liblayouteditor_global.h"

class Editor;
class NamedIcon;
class LIBLAYOUTEDITORSHARED_EXPORT ControlPanelEditorXml : public AbstractXmlAdapter
{
    Q_OBJECT
public:
    explicit ControlPanelEditorXml(QObject *parent = 0);
    ~ControlPanelEditorXml();
 ControlPanelEditorXml(const ControlPanelEditorXml&) :AbstractXmlAdapter() {}
    /*public*/ QDomElement store(QObject* o) ;
    /*public*/ void load(QDomElement element, QObject* o)throw (Exception) ;
    /*public*/ bool load(QDomElement element)throw (Exception) ;
    /*public*/ int loadOrder();
 /*public*/ QDomElement storeIcon(QString elemName, NamedIcon* icon);
 /*public*/ NamedIcon* loadIcon(QString key, QDomElement element, Editor* ed);

signals:

public slots:
private:
    Logger* log;
};
Q_DECLARE_METATYPE(ControlPanelEditorXml)
#endif // CONTROLPANELEDITORXML_H
