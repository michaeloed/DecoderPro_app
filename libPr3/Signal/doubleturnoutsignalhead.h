#ifndef DOUBLETURNOUTSIGNALHEAD_H
#define DOUBLETURNOUTSIGNALHEAD_H
#include "defaultsignalhead.h"
#include "namedbeanhandle.h"

class Turnout;
class LIBPR3SHARED_EXPORT DoubleTurnoutSignalHead : public DefaultSignalHead
{
    Q_OBJECT
public:
    //explicit DoubleTurnoutSignalHead(QObject *parent = 0);
    /*public*/ DoubleTurnoutSignalHead(QString sys, QString user, NamedBeanHandle<Turnout*>* green, NamedBeanHandle<Turnout*>* red, QObject *parent = 0);
    /*public*/ DoubleTurnoutSignalHead(QString sys, NamedBeanHandle<Turnout*>* green, NamedBeanHandle<Turnout*>* red, QObject *parent = 0);
    /*public*/ void dispose() ;
    /*public*/ NamedBeanHandle<Turnout*>* getRed();
    /*public*/ NamedBeanHandle<Turnout*>* getGreen();
    /*public*/ void setRed(NamedBeanHandle<Turnout*>* t);
    /*public*/ void setGreen(NamedBeanHandle<Turnout*>* t);
    /*protected*/ void updateOutput(); // needs to be public for flash to work

signals:

public slots:
 /*public*/ void propertyChange(PropertyChangeEvent* propertyChangeEvent);
 virtual void readOutput();

private:

    NamedBeanHandle<Turnout*>* mRed;
    NamedBeanHandle<Turnout*>* mGreen;
    Logger* log;
    QTimer* readUpdateTimer;
    bool isTurnoutUsed(Turnout* t);
    int mRedCommanded;
    int mGreenCommanded;

protected:
    //friend class DefaultSignalHead;
 friend class TripleTurnoutSignalHead;
 friend class QuadOutputSignalHead;
 friend class TripleOutputSignalHead;
};

#endif // DOUBLETURNOUTSIGNALHEAD_H
