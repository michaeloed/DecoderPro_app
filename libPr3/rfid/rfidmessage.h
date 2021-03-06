#ifndef RFIDMESSAGE_H
#define RFIDMESSAGE_H
#include <QObject>
#include "abstractmrmessage.h"

class RfidMessage : public AbstractMRMessage
{
  //Q_OBJECT
public:
 /*public*/ RfidMessage(int l, QObject *parent = nullptr);
 /*public*/ RfidMessage(QString m,int l, QObject *parent =nullptr);
 /*public*/ void setInterlocked(bool v);
 /*public*/ bool getInterlocked();
 /*public*/ RfidMessage(QByteArray* a, int l, QObject *parent = nullptr);
 /*public*/ /*final*/ void setResponseLength(int l);
 /*public*/ int getResponseLength();
 /*abstract*/ /*public*/ QString toMonitorString();

signals:
    
public slots:
private:
 bool interlocked = false;
 int responseLength = -1;  // -1 is an invalid value, indicating it hasn't been set

protected:
 RfidMessage(QObject *parent = nullptr);

};

#endif // RFIDMESSAGE_H
