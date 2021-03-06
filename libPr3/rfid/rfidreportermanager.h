#ifndef RFIDREPORTERMANAGER_H
#define RFIDREPORTERMANAGER_H
#include "abstractreportermanager.h"
#include "rfidreporter.h"

class RfidSystemConnectionMemo;
class RfidMessage;
class LIBPR3SHARED_EXPORT RfidReporterManager : public AbstractReporterManager
{
    Q_OBJECT
public:
    //explicit RfidReporterManager(QObject *parent = 0);
    /*public*/ RfidReporterManager(RfidSystemConnectionMemo* memo, QObject *parent = 0);
    /*public*/ QString getSystemPrefix();
    Reporter* createNewReporter(QString systemName, QString userName);
    /*public*/ void message(RfidMessage* m);

signals:
    
public slots:
private:
    /*private*/ QString prefix;
 Logger* log;
};

#endif // RFIDREPORTERMANAGER_H
