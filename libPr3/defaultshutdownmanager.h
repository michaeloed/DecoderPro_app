#ifndef DEFAULTSHUTDOWNMANAGER_H
#define DEFAULTSHUTDOWNMANAGER_H
#include "logger.h"
#include "exceptions.h"
#include "shutdownmanager.h"
#include <QVector>
#include "libPr3_global.h"

class ShutDownTask;
class LIBPR3SHARED_EXPORT DefaultShutDownManager : public ShutDownManager
{
    Q_OBJECT
public:
    explicit DefaultShutDownManager(QObject *parent = 0);
    ~DefaultShutDownManager() {}
    DefaultShutDownManager(const DefaultShutDownManager&) : ShutDownManager() {}
    /*public*/ void _register(ShutDownTask* s);
    /*public*/ void deregister(ShutDownTask* s);
    /*public*/ bool shutdown();
    /*public*/ bool restart();

signals:
    
public slots:
private:
    Logger log;
    QVector<ShutDownTask*>* tasks;// = new QVector<ShutDownTask*>();
    static bool shuttingDown;// = false;
protected:
    /*protected*/ bool shutdown(int status, bool exit);

};
Q_DECLARE_METATYPE(DefaultShutDownManager)
#endif // DEFAULTSHUTDOWNMANAGER_H
