#ifndef JSONBLOCKSOCKETSERVICE_H
#define JSONBLOCKSOCKETSERVICE_H
#include "jsonsocketservice.h"
#include "propertychangelistener.h"
#include <QMap>

class Block;
class BlockListener;
class JsonBlockHttpService;
class JsonBlockSocketService : public JsonSocketService
{
public:
 /*public*/ JsonBlockSocketService(JsonConnection* connection, QObject* parent = 0);
 /*public*/ void onMessage(QString type, QJsonObject data,  QLocale locale) throw (IOException, JmriException, JsonException);
 /*public*/ void onList(QString type, QJsonObject data,  QLocale locale) throw (IOException, JmriException, JsonException);
 /*public*/ void onClose();

private:
 /*private*/ /*final*/ JsonBlockHttpService* service;
 /*private*/ /*final*/ QMap<QString, BlockListener*>* blocks;// = new HashMap<>();
 /*private*/  QLocale locale;
friend class BlockListener;
};
/*private*/ class BlockListener : public PropertyChangeListener {
 Q_OBJECT
 JsonBlockSocketService* jbss;
protected:
   /*protected*/ /*final*/ Block* block;

   /*public*/ BlockListener(Block* block, JsonBlockSocketService* jbss);
public slots:
   /*public*/ void propertyChange(PropertyChangeEvent* e) ;
 friend class JsonBlockSocketService;
};

#endif // JSONBLOCKSOCKETSERVICE_H
