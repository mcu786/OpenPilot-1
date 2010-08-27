/**
 ******************************************************************************
 * @addtogroup UAVObjects OpenPilot UAVObjects
 * @{ 
 * @addtogroup AHRSCalibration AHRSCalibration
 * @brief Contains the calibration settings for the @ref AHRSCommsModule
 *
 * Autogenerated files and functions for AHRSCalibration Object
 * @{ 
 *
 * @file       ahrscalibration.c
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 * @brief      Implementation of the AHRSCalibration object. This file has been 
 *             automatically generated by the UAVObjectGenerator.
 * 
 * @note       Object definition file: ahrscalibration.xml. 
 *             This is an automatically generated file.
 *             DO NOT modify manually.
 *
 * @see        The GNU Public License (GPL) Version 3
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

#include "openpilot.h"
#include "ahrscalibration.h"

// Private variables
static UAVObjHandle handle;

// Private functions
static void setDefaults(UAVObjHandle obj, uint16_t instId);

/**
 * Initialize object.
 * \return 0 Success
 * \return -1 Failure
 */
int32_t AHRSCalibrationInitialize()
{
	// Register object with the object manager
	handle = UAVObjRegister(AHRSCALIBRATION_OBJID, AHRSCALIBRATION_NAME, AHRSCALIBRATION_METANAME, 0,
			AHRSCALIBRATION_ISSINGLEINST, AHRSCALIBRATION_ISSETTINGS, AHRSCALIBRATION_NUMBYTES, &setDefaults);

	// Done
	if (handle != 0)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

/**
 * Initialize object fields and metadata with the default values.
 * If a default value is not specified the object fields
 * will be initialized to zero.
 */
static void setDefaults(UAVObjHandle obj, uint16_t instId)
{
	AHRSCalibrationData data;
	UAVObjMetadata metadata;

	// Initialize object fields to their default values
	UAVObjGetInstanceData(obj, instId, &data);
	memset(&data, 0, sizeof(AHRSCalibrationData));
    data.measure_var = 0;
    data.accel_bias[0] = -2048;
    data.accel_bias[1] = -2048;
    data.accel_bias[2] = -2048;
    data.accel_scale[0] = 0.012;
    data.accel_scale[1] = 0.012;
    data.accel_scale[2] = 0.012;
    data.accel_var[0] = 5e-05;
    data.accel_var[1] = 5e-05;
    data.accel_var[2] = 5e-05;
    data.gyro_bias[0] = -1675;
    data.gyro_bias[1] = -1675;
    data.gyro_bias[2] = -1675;
    data.gyro_scale[0] = 0.007;
    data.gyro_scale[1] = 0.007;
    data.gyro_scale[2] = 0.007;
    data.gyro_var[0] = 0.0001;
    data.gyro_var[1] = 0.0001;
    data.gyro_var[2] = 0.0001;
    data.mag_bias[0] = 0;
    data.mag_bias[1] = 0;
    data.mag_bias[2] = 0;
    data.mag_var[0] = 5e-05;
    data.mag_var[1] = 5e-05;
    data.mag_var[2] = 5e-05;

	UAVObjSetInstanceData(obj, instId, &data);

	// Initialize object metadata to their default values
	metadata.access = ACCESS_READWRITE;
	metadata.gcsAccess = ACCESS_READWRITE;
	metadata.telemetryAcked = 1;
	metadata.telemetryUpdateMode = UPDATEMODE_ONCHANGE;
	metadata.telemetryUpdatePeriod = 0;
	metadata.gcsTelemetryAcked = 1;
	metadata.gcsTelemetryUpdateMode = UPDATEMODE_ONCHANGE;
	metadata.gcsTelemetryUpdatePeriod = 0;
	metadata.loggingUpdateMode = UPDATEMODE_NEVER;
	metadata.loggingUpdatePeriod = 0;
	UAVObjSetMetadata(obj, &metadata);
}

/**
 * Get object handle
 */
UAVObjHandle AHRSCalibrationHandle()
{
	return handle;
}

/**
 * @}
 */

