
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

#include "qipendpointsimple.hh"
#include <miniupnpc.h>

class QIPEndpointSimplePrivate
{
public:
    QHostAddress internalIp;
    short internalPort;
};

QIPEndpointSimple::QIPEndpointSimple(QHostAddress internalIp, short internalPort) :
    d(new QIPEndpointSimplePrivate)
{
    d->internalIp = internalIp;
    d->internalPort = internalPort;
}

QIPEndpointSimple::~QIPEndpointSimple()
{
}

short QIPEndpointSimple::internalPort() const
{
    return d->internalPort;
}

short QIPEndpointSimple::externalPort() const
{
    return d->internalPort;
}

QHostAddress QIPEndpointSimple::internalIp() const
{
    return d->internalIp;
}

QHostAddress QIPEndpointSimple::externalIp() const
{
    return d->internalIp;
}

QIPEndpoint::Type QIPEndpointSimple::type() const
{
    return QIPEndpoint::Type::Simple;
}
