/**
 ******************************************************************************
 *
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 * @see        The GNU Public License (GPL) Version 3
 * @addtogroup GCSPlugins GCS Plugins
 * @{
 * @addtogroup UAVObjectsPlugin UAVObjects Plugin
 * @{
 * @brief      The UAVUObjects GCS plugin 
 *   
 * @note       Object definition file: exampleobject2.xml. 
 *             This is an automatically generated file.
 *             DO NOT modify manually.
 *
 * @file       exampleobject2.cpp
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
#include "exampleobject2.h"
#include "uavobjectfield.h"

const QString ExampleObject2::NAME = QString("ExampleObject2");

/**
 * Constructor
 */
ExampleObject2::ExampleObject2(): UAVDataObject(OBJID, ISSINGLEINST, ISSETTINGS, NAME)
{
    // Create fields
    QList<UAVObjectField*> fields;
    QStringList Field1ElemNames;
    Field1ElemNames.append("0");
    fields.append( new UAVObjectField(QString("Field1"), QString("unit1"), UAVObjectField::INT8, Field1ElemNames, QStringList()) );
    QStringList Field2ElemNames;
    Field2ElemNames.append("0");
    fields.append( new UAVObjectField(QString("Field2"), QString("unit2"), UAVObjectField::INT16, Field2ElemNames, QStringList()) );
    QStringList Field3ElemNames;
    Field3ElemNames.append("0");
    fields.append( new UAVObjectField(QString("Field3"), QString("unit3"), UAVObjectField::INT32, Field3ElemNames, QStringList()) );
    QStringList Field4ElemNames;
    Field4ElemNames.append("0");
    Field4ElemNames.append("1");
    Field4ElemNames.append("2");
    Field4ElemNames.append("3");
    fields.append( new UAVObjectField(QString("Field4"), QString("unit4"), UAVObjectField::FLOAT32, Field4ElemNames, QStringList()) );

    // Initialize object
    initializeFields(fields, (quint8*)&data, NUMBYTES);
    // Set the default field values
    setDefaultFieldValues();
}

/**
 * Get the default metadata for this object
 */
UAVObject::Metadata ExampleObject2::getDefaultMetadata()
{
    UAVObject::Metadata metadata;
    metadata.flightAccess = ACCESS_READWRITE;
    metadata.gcsAccess = ACCESS_READWRITE;
    metadata.gcsTelemetryAcked = 1;
    metadata.gcsTelemetryUpdateMode = UAVObject::UPDATEMODE_ONCHANGE;
    metadata.gcsTelemetryUpdatePeriod = 0;
    metadata.flightTelemetryAcked = 1;
    metadata.flightTelemetryUpdateMode = UAVObject::UPDATEMODE_PERIODIC;
    metadata.flightTelemetryUpdatePeriod = 500;
    metadata.loggingUpdateMode = UAVObject::UPDATEMODE_NEVER;
    metadata.loggingUpdatePeriod = 0;
    return metadata;
}

/**
 * Initialize object fields with the default values.
 * If a default value is not specified the object fields
 * will be initialized to zero.
 */
void ExampleObject2::setDefaultFieldValues()
{

}

/**
 * Get the object data fields
 */
ExampleObject2::DataFields ExampleObject2::getData()
{
    QMutexLocker locker(mutex);
    return data;
}

/**
 * Set the object data fields
 */
void ExampleObject2::setData(const DataFields& data)
{
    QMutexLocker locker(mutex);
    // Get metadata
    Metadata mdata = getMetadata();
    // Update object if the access mode permits
    if ( mdata.gcsAccess == ACCESS_READWRITE )
    {
        this->data = data;
        emit objectUpdatedAuto(this); // trigger object updated event
        emit objectUpdated(this);
    }
}

/**
 * Create a clone of this object, a new instance ID must be specified.
 * Do not use this function directly to create new instances, the
 * UAVObjectManager should be used instead.
 */
UAVDataObject* ExampleObject2::clone(quint32 instID)
{
    ExampleObject2* obj = new ExampleObject2();
    obj->initialize(instID, this->getMetaObject());
    return obj;
}

/**
 * Static function to retrieve an instance of the object.
 */
ExampleObject2* ExampleObject2::GetInstance(UAVObjectManager* objMngr, quint32 instID)
{
    return dynamic_cast<ExampleObject2*>(objMngr->getObject(ExampleObject2::OBJID, instID));
}
