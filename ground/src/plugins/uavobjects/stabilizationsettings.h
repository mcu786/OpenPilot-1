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
 * @note       Object definition file: stabilizationsettings.xml. 
 *             This is an automatically generated file.
 *             DO NOT modify manually.
 *
 * @file       stabilizationsettings.h
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
#ifndef STABILIZATIONSETTINGS_H
#define STABILIZATIONSETTINGS_H

#include "uavdataobject.h"
#include "uavobjectmanager.h"

class UAVOBJECTS_EXPORT StabilizationSettings: public UAVDataObject
{
    Q_OBJECT

public:
    // Field structure
    typedef struct {
        quint16 UpdatePeriod;
        float RollMax;
        float PitchMax;
        float ThrottleMax;
        float PitchKp;
        float PitchKi;
        float PitchKd;
        float RollKp;
        float RollKi;
        float RollKd;
        float YawKp;
        float YawKi;
        float YawKd;

    } __attribute__((packed)) DataFields;

    // Field information
    // Field UpdatePeriod information
    // Field RollMax information
    // Field PitchMax information
    // Field ThrottleMax information
    // Field PitchKp information
    // Field PitchKi information
    // Field PitchKd information
    // Field RollKp information
    // Field RollKi information
    // Field RollKd information
    // Field YawKp information
    // Field YawKi information
    // Field YawKd information

  
    // Constants
    static const quint32 OBJID = 2185681924U;
    static const QString NAME;
    static const bool ISSINGLEINST = 1;
    static const bool ISSETTINGS = 1;
    static const quint32 NUMBYTES = sizeof(DataFields);

    // Functions
    StabilizationSettings();

    DataFields getData();
    void setData(const DataFields& data);
    Metadata getDefaultMetadata();
    UAVDataObject* clone(quint32 instID);

    static StabilizationSettings* GetInstance(UAVObjectManager* objMngr, quint32 instID = 0);
	
private:
    DataFields data;

    void setDefaultFieldValues();

};

#endif // STABILIZATIONSETTINGS_H
