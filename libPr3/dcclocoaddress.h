#ifndef DCCLOCOADDRESS_H
#define DCCLOCOADDRESS_H
#include "locoaddress.h"
#include "libPr3_global.h"
#include <QHash>

class LIBPR3SHARED_EXPORT DccLocoAddress :  public locoAddress
{
    Q_OBJECT
public:
 DccLocoAddress(QObject *parent = 0) {}
    explicit DccLocoAddress(int number, bool isLong, QObject *parent = 0);
    DccLocoAddress(int number, Protocol protocol, QObject *parent = 0);
    DccLocoAddress(const DccLocoAddress& l , QObject* parent = 0);
    //bool operator ==(const LocoAddress &a) override;
    Q_INVOKABLE /*public*/ bool equals(QObject* a);
    int hashCode();
    QString toString();
    bool isLongAddress();
    virtual Protocol getProtocol() const;
    virtual int getNumber() const;
signals:
    
public slots:
protected:
 long number;
 Protocol protocol;
friend class NmraConsist;
    
};

template <class T>
class DccLocoHash : public QObject
{
 //Q_OBJECT
public:
 DccLocoHash<T>()
 {
  hash = QHash<DccLocoAddress*, T>();
 }
 void insert(DccLocoAddress* a, T v)
 {
  hash.insert(a,v);
 }
 bool contains(DccLocoAddress* aIn)
 {
  foreach(DccLocoAddress* a, hash.keys())
  {
   if(a->equals(aIn))
    return true;
  }
  return false;
 }
 void remove(DccLocoAddress* aIn)
 {
  foreach(DccLocoAddress* a, hash.keys())
  {
   if(a->equals(aIn))
   {
    hash.remove(a);
    return;
   }
  }
 }
 T value(DccLocoAddress* aIn)
 {
  foreach(DccLocoAddress* a, hash.keys())
  {
   if(a->equals(aIn))
   {
    return hash.value(a);
   }
  }
  return 0;
 }
 int size()
 {
  return hash.keys().size();
 }

private:
 QHash<DccLocoAddress*, T> hash;
};

class DccLocoAddressList : public QObject
{
 //Q_OBJECT
public:
 DccLocoAddressList()
 {
  list = QList<DccLocoAddress*>();
 }
 DccLocoAddressList(QList<DccLocoAddress*> listIn)
 {
  list = QList<DccLocoAddress*>(listIn);
 }
 void append(DccLocoAddress* a)
 {
  list.append(a);
 }
 DccLocoAddress* at(int i)
 {
  return list.at(i);
 }
 bool contains(DccLocoAddress* aIn)
 {
  foreach (DccLocoAddress*a, list) {
   if(a->equals(aIn))
    return true;
  }
  return false;
 }
 int indexOf(DccLocoAddress* aIn)
 {
  for(int i=0; i < list.count(); i++)
  {
   if(list.at(i)->equals(aIn))
    return i;
  }
  return -1;
 }
 void removeOne(DccLocoAddress* aIn)
 {
  foreach (DccLocoAddress* a, list) {
   if(a->equals(aIn))
   {
    list.removeOne(a);
    return;
   }
  }
 }
 void removeAt(int i)
 {
  list.removeAt(i);
  return;
 }

 int size()
 {
  return list.size();
 }
 QList<DccLocoAddress*>* toList()
 {
  return &list;
 }
private:
 QList<DccLocoAddress*> list;
};
#endif // DCCLOCOADDRESS_H
