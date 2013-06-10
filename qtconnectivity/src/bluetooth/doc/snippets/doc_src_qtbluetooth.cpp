/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the documentation of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

//! [include]
#include <qbluetoothlocaldevice.h>
//! [include]

//! [namespace]
QT_USE_NAMESPACE_BLUETOOTH
//! [namespace]

//! [turningon]
QBluetoothLocalDevice localDevice;
QString localDeviceName;

// Check if Bluetooth is available on this device
if (localDevice.isValid()) {

    // Turn Bluetooth on
    localDevice.powerOn();

    // Read local device name
    localDeviceName = localDevice.name();

    // Make it visible to others
    localDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable)
}
//! [turningon]

//! [discovery]
// Create a discovery agent and connect to its signals
QBluetoothDiscoveryAgent *discoveryAgent = new QBluetoothDiscoveryAgent(this);
connect(discoveryAgent, SIGNAL(deviceDiscovered(const QBluetoothDeviceInfo&)),
        this, SLOT(deviceDiscovered(const QBluetoothDeviceInfo&)));

// Start a discovery
discoveryAgent->start();

...

// In your local slot, read information about the found devices
void MyClass::deviceDiscovered(const QBluetoothDeviceInfo &device)
{
    qDebug() << "Found new device:" << device.name() << '(' << device.address().toString() << ')';
}
//! [discovery]

//! [sendfile]
// Create a transfer manager
QBluetoothTransferManager *transferManager = new QBluetoothTransferManager(this);

// Create the transfer request and file to be sent
QBluetoothTransferRequest request(device.address());
QFile *file = new QFile("testfile.txt");

// Ask the transfer manager to send it
QBluetoothTransferReply *reply = transferManager->put(request, file);

// Connect to the reply's signals to be informed about the status and do cleanups when done
connect(reply, SIGNAL(finished(QBluetoothTransferReply*)),
        this, SLOT(transferFinished(QBluetoothTransferReply*)));
//! [sendfile]