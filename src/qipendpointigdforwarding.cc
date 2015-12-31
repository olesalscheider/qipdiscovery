
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

#include "qipendpointigdforwarding.hh"
#include "qipdiscoverydebug.hh"
#include <miniupnpc.h>
#include <upnpcommands.h>
#include <QTimer>

class QIPEndpointIgdForwardingPrivate
{
public:
    QString controlUrl;
    QString description;
    short internalPort;
    short externalPort;
    QIPEndpoint::Protocol protocol;
    QHostAddress internalIp;
    QTimer refreshTimer;
};

QIPEndpointIgdForwarding::QIPEndpointIgdForwarding(QString controlUrl, QString description, QIPEndpoint::Protocol protocol,
                                                   QHostAddress internalIp, short internalPort) :
    d(new QIPEndpointIgdForwardingPrivate)
{
    d->controlUrl = controlUrl;
    d->description = description;
    d->protocol = protocol;
    d->internalIp = internalIp;
    d->internalPort = internalPort;

    updatePortmapping();
    QObject::connect(&d->refreshTimer, &QTimer::timeout, this, &QIPEndpointIgdForwarding::updatePortmapping);
    d->refreshTimer.start(300);
}

QIPEndpointIgdForwarding::~QIPEndpointIgdForwarding()
{
    d->refreshTimer.stop();

    int retval = UPNP_DeletePortMapping(d->controlUrl.toLatin1(),
                                        "urn:schemas-upnp-org:service:WANIPConnection:1",
                                        QString::number(d->externalPort).toLatin1(),
                                        protocolString(d->protocol).toLatin1(),
                                        nullptr /* remote host */);
    if (retval) {
        qCWarning(QIPDISCOVERY) << "DeletePortMapping failed with error code" << retval;
    }
}

void QIPEndpointIgdForwarding::updatePortmapping()
{
    char reservedPort[6];
    int retval = UPNP_AddAnyPortMapping(d->controlUrl.toLatin1(),
                                        "urn:schemas-upnp-org:service:WANIPConnection:1",
                                        QString::number(d->internalPort).toLatin1(),
                                        QString::number(d->internalPort).toLatin1(),
                                        d->internalIp.toString().toLatin1(),
                                        d->description.toLatin1(),
                                        protocolString(d->protocol).toLatin1(),
                                        nullptr /* remote host */,
                                        "600" /* lease duration */,
                                        reservedPort);
    if (retval) {
        qCWarning(QIPDISCOVERY) << "AddAnyPortMapping failed with error code" << retval;
    } else {
        d->externalPort = QString(reservedPort).toShort();
    }
}

QString QIPEndpointIgdForwarding::protocolString(QIPEndpoint::Protocol protocol)
{
    switch (protocol) {
    case QIPEndpoint::Protocol::Udp:
        return QLatin1String("UDP");
    case QIPEndpoint::Protocol::Tcp:
        return QLatin1String("TCP");
    default:
        return QLatin1String("");
    }
}

short QIPEndpointIgdForwarding::internalPort() const
{
    return d->internalPort;
}

short QIPEndpointIgdForwarding::externalPort() const
{
    return d->externalPort;
}

QHostAddress QIPEndpointIgdForwarding::internalIp() const
{
    return d->internalIp;
}

QHostAddress QIPEndpointIgdForwarding::externalIp() const
{
    char externalIpStr[16];
    int retval = UPNP_GetExternalIPAddress(d->controlUrl.toLatin1(),
                                           "urn:schemas-upnp-org:service:WANIPConnection:1",
                                           externalIpStr);
    if (retval) {
        qCWarning(QIPDISCOVERY) << "GetExternalIPAddress failed with error code" << retval;
        return QHostAddress();
    }

    QHostAddress externalIp = QHostAddress(QString(externalIpStr));
    return externalIp;
}

QIPEndpoint::Type QIPEndpointIgdForwarding::type() const
{
    return QIPEndpoint::Type::IgdForwarding;
}
