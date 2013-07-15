/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtBluetooth module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtTest/QtTest>

#include <QDebug>

#include <qrfcommserver.h>
#include <qbluetoothsocket.h>
#include <qbluetoothlocaldevice.h>

QT_USE_NAMESPACE_BLUETOOTH

Q_DECLARE_METATYPE(QBluetooth::SecurityFlags);

// Max time to wait for connection
static const int MaxConnectTime = 60 * 1000;   // 1 minute in ms

class tst_QRfcommServer : public QObject
{
    Q_OBJECT

public:
    tst_QRfcommServer();
    ~tst_QRfcommServer();

private slots:
    void initTestCase();

    void tst_construction();

    void tst_listen_data();
    void tst_listen();

    void tst_secureFlags();

    void tst_pendingConnections_data();
    void tst_pendingConnections();

    void tst_receive_data();
    void tst_receive();

private:
    QBluetoothLocalDevice localDevice;
};

tst_QRfcommServer::tst_QRfcommServer()
{
}

tst_QRfcommServer::~tst_QRfcommServer()
{
}

void tst_QRfcommServer::initTestCase()
{
    qRegisterMetaType<QBluetooth::SecurityFlags>("QBluetooth::SecurityFlags");

    if (!QBluetoothLocalDevice::allDevices().count())
        QSKIP("Skipping test due to missing Bluetooth device");

    // turn on BT in case it is not on
    if (localDevice.hostMode() == QBluetoothLocalDevice::HostPoweredOff) {
        QSignalSpy hostModeSpy(&localDevice, SIGNAL(hostModeStateChanged(QBluetoothLocalDevice::HostMode)));
        QVERIFY(hostModeSpy.isEmpty());
        localDevice.powerOn();
        int connectTime = 5000;  // ms
        while (hostModeSpy.count() < 1 && connectTime > 0) {
            QTest::qWait(500);
            connectTime -= 500;
        }
        QVERIFY(hostModeSpy.count() > 0);
    }
    QBluetoothLocalDevice::HostMode hostMode= localDevice.hostMode();
    QVERIFY(hostMode == QBluetoothLocalDevice::HostConnectable
         || hostMode == QBluetoothLocalDevice::HostDiscoverable
         || hostMode == QBluetoothLocalDevice::HostDiscoverableLimitedInquiry);
}

void tst_QRfcommServer::tst_construction()
{
    {
        QRfcommServer server;

        QVERIFY(!server.isListening());
        QCOMPARE(server.maxPendingConnections(), 1);
        QVERIFY(!server.hasPendingConnections());
        QVERIFY(server.nextPendingConnection() == 0);
        QVERIFY(server.serverAddress().isNull());
        QCOMPARE(server.serverPort(), quint16(0));
    }
}

void tst_QRfcommServer::tst_listen_data()
{
    QTest::addColumn<QBluetoothAddress>("address");
    QTest::addColumn<quint16>("port");

    QTest::newRow("default") << QBluetoothAddress() << quint16(0);
    QTest::newRow("specified address") << QBluetoothAddress("00:11:B1:08:AD:B8") << quint16(0);
    QTest::newRow("specified port") << QBluetoothAddress() << quint16(10);
    QTest::newRow("specified address/port") << QBluetoothAddress("00:11:B1:08:AD:B8") << quint16(10);
}

void tst_QRfcommServer::tst_listen()
{
    QFETCH(QBluetoothAddress, address);
    QFETCH(quint16, port);

    {
        QRfcommServer server;
        qDebug() << "tst_listen() address=" << address.toString() << "port=" << port;
        bool result = server.listen(address, port);
        QTest::qWait(1000);

        QVERIFY(result);
        QVERIFY(server.isListening());

        if (!address.isNull())
            QCOMPARE(server.serverAddress(), address);

        qDebug()<<"Server Port="<<server.serverPort();
        if (port != 0)
            QCOMPARE(server.serverPort(), port);
        else
            QVERIFY(server.serverPort() != 0);

        QCOMPARE(server.maxPendingConnections(), 1);

        QVERIFY(!server.hasPendingConnections());
        QVERIFY(server.nextPendingConnection() == 0);

        server.close();
        QTest::qWait(2000);

        QVERIFY(!server.isListening());

        QVERIFY(server.serverAddress().isNull());
        QVERIFY(server.serverPort() == 0);

        QVERIFY(server.hasPendingConnections() == false);
        QVERIFY(server.nextPendingConnection() == 0);
    }
}

void tst_QRfcommServer::tst_pendingConnections_data()
{
    QTest::addColumn<int>("maxConnections");

    QTest::newRow("1 connection") << 1;
    //QTest::newRow("2 connections") << 2;
}

void tst_QRfcommServer::tst_pendingConnections()
{
    QFETCH(int, maxConnections);

    QRfcommServer server;
    QBluetoothLocalDevice localDev;

    QBluetoothAddress address = localDev.address();
    server.setMaxPendingConnections(maxConnections);
    bool result = server.listen(address, 20);  // port == 20
    QTest::qWait(1000);

    QVERIFY(result);
    QVERIFY(server.isListening());

    qDebug() << "tst_pendingConnections() Listening on address " << address.toString() << "RFCOMM channel:" << server.serverPort();

    QCOMPARE(server.maxPendingConnections(), maxConnections);

    QVERIFY(!server.hasPendingConnections());
    QVERIFY(server.nextPendingConnection() == 0);

    /* wait for maxConnections simultaneous connections */
    qDebug() << "Waiting for" << maxConnections << "simultaneous connections.";

    QSignalSpy connectionSpy(&server, SIGNAL(newConnection()));

    int connectTime = MaxConnectTime;
    while (connectionSpy.count() < maxConnections && connectTime > 0) {
        QTest::qWait(1000);
        connectTime -= 1000;
    }

    QList<QBluetoothSocket *> sockets;
    while (server.hasPendingConnections())
        sockets.append(server.nextPendingConnection());

    QCOMPARE(connectionSpy.count(), maxConnections);
    QCOMPARE(sockets.count(), maxConnections);

    foreach (QBluetoothSocket *socket, sockets) {
        qDebug() << socket->state();
        QVERIFY(socket->state() == QBluetoothSocket::ConnectedState);
        QVERIFY(socket->openMode() == QIODevice::ReadWrite);
    }

    QVERIFY(!server.hasPendingConnections());
    QVERIFY(server.nextPendingConnection() == 0);

    while (!sockets.isEmpty()) {
        QBluetoothSocket *socket = sockets.takeFirst();
        socket->close();
        delete socket;
    }

    server.close();
}

void tst_QRfcommServer::tst_receive_data()
{
    QTest::addColumn<QByteArray>("expected");

    QTest::newRow("test") << QByteArray("hello\r\n");
}

void tst_QRfcommServer::tst_receive()
{
    QFETCH(QByteArray, expected);

    QRfcommServer server;
    QBluetoothLocalDevice localDev;

    QBluetoothAddress address = localDev.address();
    bool result = server.listen(address, 20);  // port == 20
    QTest::qWait(1000);

    QVERIFY(result);
    QVERIFY(server.isListening());

    qDebug() << "Listening on address " << address.toString() << "RFCOMM channel:" << server.serverPort();

    int connectTime = MaxConnectTime;
    while (!server.hasPendingConnections() && connectTime > 0) {
        QTest::qWait(1000);
        connectTime -= 1000;
    }

    QVERIFY(server.hasPendingConnections());

    qDebug() << "Got connection";

    QBluetoothSocket *socket = server.nextPendingConnection();

    QVERIFY(socket->state() == QBluetoothSocket::ConnectedState);
    QVERIFY(socket->openMode() == QIODevice::ReadWrite);

    QSignalSpy readyReadSpy(socket, SIGNAL(readyRead()));

    int readyReadTime = 60000;
    while (readyReadSpy.isEmpty() && readyReadTime > 0) {
        QTest::qWait(1000);
        readyReadTime -= 1000;
    }

    QVERIFY(!readyReadSpy.isEmpty());

    const QByteArray data = socket->readAll();

    QCOMPARE(data, expected);
}

void tst_QRfcommServer::tst_secureFlags()
{
    QRfcommServer server;

    server.setSecurityFlags(QBluetooth::NoSecurity);
    QCOMPARE(server.securityFlags(), QBluetooth::NoSecurity);

    server.setSecurityFlags(QBluetooth::Encryption);
    QCOMPARE(server.securityFlags(), QBluetooth::Encryption);
}

QTEST_MAIN(tst_QRfcommServer)

#include "tst_qrfcommserver.moc"
