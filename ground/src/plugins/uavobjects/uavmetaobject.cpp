/**
 ******************************************************************************
 *
 * @file       uavmetaobject.cpp
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 *             Parts by Nokia Corporation (qt-info@nokia.com) Copyright (C) 2009.
 * @brief      
 * @see        The GNU Public License (GPL) Version 3
 * @defgroup   uavobjects_plugin
 * @{
 * 
 *****************************************************************************/
/* 
 * This program is free software; you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation; either version 3 of the License, or 
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
 * for more details.
 * 
 * You should have received a copy of the GNU General Public License along 
 * with this program; if not, write to the Free Software Foundation, Inc., 
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
#include "uavmetaobject.h"
#include "uavobjectfields.h"

/**
 * Constructor
 */
UAVMetaObject::UAVMetaObject(quint32 objID, const QString& name, UAVObject* parent):
        UAVObject(objID, true, name)
{
    this->parent = parent;
    // Setup default metadata of metaobject (can not be changed)
    ownMetadata.flightTelemetryAcked = 1;
    ownMetadata.flightTelemetryUpdateMode = UPDATEMODE_ONCHANGE;
    ownMetadata.flightTelemetryUpdatePeriod = 0;
    ownMetadata.gcsTelemetryAcked = 1;
    ownMetadata.gcsTelemetryUpdateMode = UPDATEMODE_ONCHANGE;
    ownMetadata.gcsTelemetryUpdatePeriod = 0;
    ownMetadata.loggingUpdateMode = UPDATEMODE_ONCHANGE;
    ownMetadata.loggingUpdatePeriod = 0;
    // Setup fields
    QList<UAVObjectField*> fields;
    fields.append(new UAVObjectFieldUInt8(QString("FlightTelemetryAcked"), QString(""), 1));
    fields.append(new UAVObjectFieldUInt8(QString("FlightTelemetryUpdateMode"), QString(""), 1));
    fields.append(new UAVObjectFieldInt32(QString("FlightTelemetryUpdatePeriod"), QString("ms"), 1));
    fields.append(new UAVObjectFieldUInt8(QString("GCSTelemetryAcked"), QString(""), 1));
    fields.append(new UAVObjectFieldUInt8(QString("GCSTelemetryUpdateMode"), QString(""), 1));
    fields.append(new UAVObjectFieldInt32(QString("GCSTelemetryUpdatePeriod"), QString("ms"), 1));
    fields.append(new UAVObjectFieldUInt8(QString("LoggingUpdateMode"), QString(""), 1));
    fields.append(new UAVObjectFieldUInt32(QString("LoggingUpdatePeriod"), QString("ms"), 1));
    // Initialize parent
    UAVObject::initialize(0);
    UAVObject::initializeFields(fields, (quint8*)&parentMetadata, sizeof(Metadata));
    // Setup metadata of parent
    parentMetadata = parent->getDefaultMetadata();
}

/**
 * Get the parent object
 */
UAVObject* UAVMetaObject::getParentObject()
{
    return parent;
}

/**
 * Set the metadata of the metaobject, this function will
 * do nothing since metaobjects have read-only metadata.
 */
void UAVMetaObject::setMetadata(const Metadata& mdata)
{
    return; // can not update metaobject's metadata
}

/**
 * Get the metadata of the metaobject
 */
UAVObject::Metadata UAVMetaObject::getMetadata()
{
    return ownMetadata;
}

/**
 * Get the default metadata
 */
UAVObject::Metadata UAVMetaObject::getDefaultMetadata()
{
    return ownMetadata;
}

/**
 * Set the metadata held by the metaobject
 */
void UAVMetaObject::setData(const Metadata& mdata)
{
    QMutexLocker locker(mutex);
    parentMetadata = mdata;
    emit objectUpdatedAuto(this); // trigger object updated event
    emit objectUpdated(this);
}

/**
 * Get the metadata held by the metaobject
 */
UAVObject::Metadata UAVMetaObject::getData()
{
    QMutexLocker locker(mutex);
    return parentMetadata;
}


