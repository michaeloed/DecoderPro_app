#ifndef XMLINCLUDE_H
#define XMLINCLUDE_H

#include <QObject>
#include <QtXml>
#include "xmlfile.h"

class XmlInclude : public XmlFile
{
public:
 XmlInclude();
 static QDomDocumentFragment processInclude(QDomElement e, int level = 0);

public slots:
 void loadXml();

private:

};

#endif // XMLINCLUDE_H
