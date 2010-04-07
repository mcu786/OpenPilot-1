/**
 ******************************************************************************
 *
 * @file       uavobjectfieldenum.cpp
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

#include "uavobjectfieldstring.h"
#include <QtEndian>

UAVObjectFieldString::UAVObjectFieldString(const QString& name, quint32 maxSize):
        UAVObjectField(name, QString(""), maxSize)
{
    numBytesPerElement = sizeof(quint8);
}

QString UAVObjectFieldString::getString()
{
    QMutexLocker locker(obj->getMutex());
    // Null terminate last element
    setValue('\0', numElements - 1);
    // Read characters into string until a null is received
    QString str;
    quint8 ch;
    for (quint32 index = 0; index < numElements; ++index)
    {
        // Get character and check if end of string is received
        ch = getValue(index);
        if ( ch != '\0' )
        {
            str.append(ch);
        }
        else
        {
            break;
        }
    }
    // Done
    return str;
}

void UAVObjectFieldString::setString(QString& str)
{
    QMutexLocker locker(obj->getMutex());
    // Copy string to data
    QByteArray barray = str.toAscii();
    quint32 index;
    for (index = 0; index < (quint32)barray.length() && index < (numElements-1); ++index)
    {
        setValue(barray[index], index);
    }
    // Null terminate
    setValue('\0', index);
}


/**
 * Initialize all values
 */
void UAVObjectFieldString::initializeValues()
{
    for (quint32 n = 0; n < numElements; ++n)
    {
        setValue(0, n);
    }
}

/**
 * Pack the field in to an array of bytes.
 */
qint32 UAVObjectFieldString::pack(quint8* dataOut)
{
    QMutexLocker locker(obj->getMutex());
    // Pack each element in output buffer
    for (quint32 index = 0; index < numElements; ++index)
    {
        dataOut[numBytesPerElement*index] = data[offset + numBytesPerElement*index];
    }
    // Done
    return getNumBytes();
}

/**
 * Unpack the field from an array of bytes.
 */
qint32 UAVObjectFieldString::unpack(const quint8* dataIn)
{
    QMutexLocker locker(obj->getMutex());
    // Pack each element in output buffer
    for (quint32 index = 0; index < numElements; ++index)
    {
        data[offset + numBytesPerElement*index] = dataIn[numBytesPerElement*index];
    }
    // Done
    return getNumBytes();
}

/**
 * Get the field value as a double.
 */
double UAVObjectFieldString::getDouble(quint32 index)
{
    QMutexLocker locker(obj->getMutex());
    double ret = 0.0;
    // Check if index is out of bounds or no data available
    if (index < numElements && data != NULL)
    {
        quint8 value;
        memcpy(&value, &data[offset + numBytesPerElement*index], numBytesPerElement);
        ret = (double)value;
    }
    // Done
    return ret;
}

/**
 * Set the field value from a double.
 */
void UAVObjectFieldString::setDouble(double value, quint32 index)
{
    QMutexLocker locker(obj->getMutex());
    // Check if index is out of bounds or no data available
    if (index < numElements && data != NULL)
    {
        quint8 tmpValue;
        tmpValue = (quint8)value;
        memcpy(&data[offset + numBytesPerElement*index], &tmpValue, numBytesPerElement);
    }

    // Emit updated event
    emit fieldUpdated(this);
}

/**
 * Get the number of bytes per field element.
 */
quint32 UAVObjectFieldString::getNumBytesElement()
{
    return numBytesPerElement;
}

/**
 * Get the field value.
 */
quint8 UAVObjectFieldString::getValue(quint32 index)
{
    QMutexLocker locker(obj->getMutex());
    // Check if index is out of bounds or no data available
    if (index < numElements && data != NULL)
    {
        quint8 value;
        memcpy(&value, &data[offset + numBytesPerElement*index], numBytesPerElement);
        return value;
    }
    else
    {
        return 0;
    }
}

/**
 * Set the field value.
 */
void UAVObjectFieldString::setValue(quint8 value, quint32 index)
{
    QMutexLocker locker(obj->getMutex());
    // Check if index is out of bounds or no data available
    if (index < numElements && data != NULL)
    {
        memcpy(&data[offset + numBytesPerElement*index], &value, numBytesPerElement);
    }

    // Emit updated event
    emit fieldUpdated(this);
}


