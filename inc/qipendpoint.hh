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

#ifndef QIPENDPOINT_HH
#define QIPENDPOINT_HH

#include <QScopedPointer>
#include <QHostAddress>

class QIPEndpoint : public QObject
{
public:
    enum class Protocol
    {
        Udp,
        Tcp
    };

    enum class Type
    {
        Simple,
        IgdForwarding
    };

    QIPEndpoint();
    ~QIPEndpoint();

    virtual short internalPort() const = 0;
    virtual short externalPort() const = 0;
    virtual QHostAddress internalIp() const = 0;
    virtual QHostAddress externalIp() const = 0;

    virtual Type type() const = 0;
};

#endif // QIPENDPOINT_HH
