/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -v -c ClientIf -p clientif.h:clientif.cpp biz.acksoft.USBInterface.xml
 *
 * qdbusxml2cpp is Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef CLIENTIF_H_1511036530
#define CLIENTIF_H_1511036530

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface biz.acksoft.USBInterface
 */
class ClientIf: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "biz.acksoft.USBInterface"; }

public:
    ClientIf(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~ClientIf();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<int> DeviceCount()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("DeviceCount"), argumentList);
    }

    inline QDBusPendingReply<QVariantList> GetDevice(int index)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(index);
        return asyncCallWithArgumentList(QStringLiteral("GetDevice"), argumentList);
    }

    inline QDBusPendingReply<QVariantList> GetDeviceByKey(uchar bus, uchar address)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(bus) << QVariant::fromValue(address);
        return asyncCallWithArgumentList(QStringLiteral("GetDeviceByKey"), argumentList);
    }

    inline QDBusPendingReply<bool> HasParent(uchar bus, uchar address)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(bus) << QVariant::fromValue(address);
        return asyncCallWithArgumentList(QStringLiteral("HasParent"), argumentList);
    }

    inline QDBusPendingReply<QString> SendMessage(const QString &cmd)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(cmd);
        return asyncCallWithArgumentList(QStringLiteral("SendMessage"), argumentList);
    }

    inline QDBusPendingReply<QString> getDeviceClass(uchar bus, uchar address)
    {
        QList<QVariant> argumentList;
        argumentList << QVariant::fromValue(bus) << QVariant::fromValue(address);
        return asyncCallWithArgumentList(QStringLiteral("getDeviceClass"), argumentList);
    }

    inline QDBusPendingReply<QString> read()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("read"), argumentList);
    }

    inline QDBusPendingReply<QString> write()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QStringLiteral("write"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void deviceChange(uchar bus, uchar address, bool bAdded);
    void somethingHappened(const QString &signalMessage);
};

namespace biz {
  namespace acksoft {
    typedef ::ClientIf USBInterface;
  }
}
#endif