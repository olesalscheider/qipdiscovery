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

#include "qipdiscoverymanager.hh"
#include "qipdiscoverydebug.hh"
#include "qipendpointigdforwarding.hh"
#include "qipendpointsimple.hh"
#include <miniupnpc.h>
#include <QNetworkInterface>
#include <QUrl>

class QIPDiscoveryManagerPrivate
{
public:
    QStringList controlUrls;
};

QIPDiscoveryManager::QIPDiscoveryManager() : d(new QIPDiscoveryManagerPrivate)
{
    int error;
    auto deviceList = upnpDiscoverDevice("urn:schemas-upnp-org:service:WANIPConnection:1", 2000, "", "", 0, 0, &error);
    if (error) {
        qCCritical(QIPDISCOVERY) << "Device discovery failed with error" << error;
    }

    struct UPNPDev *dev = deviceList;
    while (dev) {
        d->controlUrls << dev->descURL;
        dev = dev->pNext;
    }
    freeUPNPDevlist(deviceList);
}

QIPDiscoveryManager::~QIPDiscoveryManager()
{
}

QList<QSharedPointer<QIPEndpoint>> QIPDiscoveryManager::discoverEndpoints(short localPort,
                                                                            QIPEndpoint::Protocol protocol,
                                                                            QString description)
{
    QList<QSharedPointer<QIPEndpoint>> endpoints;

    for (auto &interface : QNetworkInterface::allInterfaces()) {
        if (interface.flags() & QNetworkInterface::IsLoopBack ||
            !(interface.flags() & QNetworkInterface::IsRunning)) {
            continue;
        }

        for (auto &entry : interface.addressEntries()) {
            QHostAddress ip = entry.ip();
            if (ip.protocol() != QAbstractSocket::IPv4Protocol &&
                ip.protocol() != QAbstractSocket::IPv6Protocol) {
                continue;
            }

            endpoints.push_back(QSharedPointer<QIPEndpoint>(new QIPEndpointSimple(ip, localPort)));
        }
    }

    for (auto &controlUrl : d->controlUrls) {
        QUrl url(controlUrl);
        QHostAddress address(url.host());
        for (auto &interface : QNetworkInterface::allInterfaces()) {
            if (interface.flags() & QNetworkInterface::IsLoopBack ||
                !(interface.flags() & QNetworkInterface::IsRunning)) {
                continue;
            }

            for (auto &entry : interface.addressEntries()) {
                QHostAddress ip = entry.ip();
                if (ip.protocol() != QAbstractSocket::IPv4Protocol) {
                    continue;
                }

                if (address.isInSubnet(ip, entry.prefixLength())) {
                    endpoints.push_back(QSharedPointer<QIPEndpoint>(
                        new QIPEndpointIgdForwarding(controlUrl, description, protocol, ip, localPort)));
                }
            }
        }
    }

    return endpoints;
}
