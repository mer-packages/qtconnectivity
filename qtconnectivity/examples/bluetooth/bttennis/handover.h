/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Toolkit.
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

#ifndef HANDOVER_H
#define HANDOVER_H

#include <QtCore/QObject>

#include <qbluetoothaddress.h>
#include <qbluetoothuuid.h>
#include <qnfcglobal.h>

QT_BEGIN_NAMESPACE_NFC
class QNearFieldManager;
class QNearFieldTarget;
class QLlcpServer;
class QLlcpSocket;
QT_END_NAMESPACE_NFC

QT_USE_NAMESPACE_NFC
QT_USE_NAMESPACE_BLUETOOTH

class Handover : public QObject
{
    Q_OBJECT

public:
    explicit Handover(quint16 serverPort, QObject *parent = 0);
    ~Handover();

    QBluetoothAddress bluetoothAddress() const;
    quint16 serverPort() const;

private slots:
    void handleNewConnection();
    void remoteDisconnected();

    void clientDisconnected();

    void readBluetoothService();
    void sendBluetoothService();

signals:
    void bluetoothServiceChanged();

private:
    QLlcpServer *m_server;
    QLlcpSocket *m_client;
    QLlcpSocket *m_remote;

    QBluetoothAddress m_address;
    quint16 m_serverPort;
    quint16 m_localServerPort;
};

#endif // HANDOVER_H