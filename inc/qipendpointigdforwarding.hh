/*
 * This file is part of qipdiscovery.
 * Copyright (C) 2015 Niels Ole Salscheider <niels_ole@salscheider-online.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>
 */

#ifndef QIPENDPOINTIGDFORWARDING_HH
#define QIPENDPOINTIGDFORWARDING_HH

#include "qipendpoint.hh"
#include <QString>

class QIPEndpointIgdForwardingPrivate;

class QIPEndpointIgdForwarding : public QIPEndpoint
{
public:
    QIPEndpointIgdForwarding(QString controlUrl, QString description, QIPEndpoint::Protocol protocol,
                             QHostAddress internalIp, short internalPort);
    ~QIPEndpointIgdForwarding();

    virtual short internalPort() const override;
    virtual short externalPort() const override;
    virtual QHostAddress internalIp() const override;
    virtual QHostAddress externalIp() const override;

    virtual QIPEndpoint::Type type() const override;

private slots:
    void updatePortmapping();

private:
    static QString protocolString(QIPEndpoint::Protocol protocol);

    const QScopedPointer<QIPEndpointIgdForwardingPrivate> d;
};

#endif // QIPENDPOINTIGDFORWARDING_HH
