/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtNfc module of the Qt Toolkit.
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

#ifndef QNEARFIELDTAGTYPE4_H
#define QNEARFIELDTAGTYPE4_H

#include <QtNfc/QNearFieldTarget>

QT_BEGIN_NAMESPACE_NFC

class Q_NFC_EXPORT QNearFieldTagType4 : public QNearFieldTarget
{
    Q_OBJECT

public:
    explicit QNearFieldTagType4(QObject *parent = 0);
    ~QNearFieldTagType4();

    Type type() const { return NfcTagType4; }

    quint8 version();

    virtual RequestId select(const QByteArray &name);
    virtual RequestId select(quint16 fileIdentifier);

    virtual RequestId read(quint16 length = 0, quint16 startOffset = 0);
    virtual RequestId write(const QByteArray &data, quint16 startOffset = 0);

protected:
    bool handleResponse(const QNearFieldTarget::RequestId &id, const QByteArray &response);
};

QT_END_NAMESPACE_NFC

#endif // QNEARFIELDTAGTYPE4_H
