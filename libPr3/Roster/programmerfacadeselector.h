#ifndef PROGRAMMERFACADESELECTOR_H
#define PROGRAMMERFACADESELECTOR_H

#include <QObject>
#include <QtXml>
#include "logger.h"

class Programmer;
class ProgrammerFacadeSelector : public QObject
{
    Q_OBJECT
public:
    explicit ProgrammerFacadeSelector(QObject *parent = 0);
 /*public*/ static Programmer* loadFacadeElements(
         QDomElement element, Programmer* programmer, bool allowCache, Programmer* baseProg);


signals:
private:
 static Logger* log;// = LoggerFactory::getLogger("ProgrammerFacadeSelector");

public slots:

};

#endif // PROGRAMMERFACADESELECTOR_H
