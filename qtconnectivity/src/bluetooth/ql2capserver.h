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

#ifndef QL2CAPSERVER_H
#define QL2CAPSERVER_H

#include <QtBluetooth/qbluetoothglobal.h>

#include <QObject>

#include <QtBluetooth/QBluetoothAddress>
#include <QtBluetooth/qbluetooth.h>
#include <QtBluetooth/QBluetoothSocket>

QT_BEGIN_NAMESPACE_BLUETOOTH

class QL2capServerPrivate;
class QBluetoothSocket;

class Q_BLUETOOTH_EXPORT QL2capServer : public QObject
{
    Q_OBJECT

public:
    QL2capServer(QObject *parent = 0);
    ~QL2capServer();

    void close();

    bool listen(const QBluetoothAddress &address = QBluetoothAddress(), quint16 port = 0);
    bool isListening() const;

    void setMaxPendingConnections(int numConnections);
    int maxPendingConnections() const;

    bool hasPendingConnections() const;
    QBluetoothSocket *nextPendingConnection();

    QBluetoothAddress serverAddress() const;
    quint16 serverPort() const;

    void setSecurityFlags(QBluetooth::SecurityFlags security);
    QBluetooth::SecurityFlags securityFlags() const;

Q_SIGNALS:
    void newConnection();

protected:
    QL2capServerPrivate *d_ptr;

private:
    Q_DECLARE_PRIVATE(QL2capServer)

#ifdef QT_BLUEZ_BLUETOOTH
    Q_PRIVATE_SLOT(d_func(), void _q_newConnection())
#endif

};

QT_END_NAMESPACE_BLUETOOTH

#endif
