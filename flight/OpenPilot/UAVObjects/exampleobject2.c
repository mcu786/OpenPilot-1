/**
 ******************************************************************************
 *
 * @file       exampleobject2.c
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 * @brief      Implementation of the ExampleObject2 object. This file has been 
 *             automatically generated by the UAVObjectGenerator.
 * 
 * @note       Object definition file: exampleobject2.xml. 
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
#include "exampleobject2.h"

// Private variables
static UAVObjHandle handle;

// Private functions
static void setDefaultFieldValues();

/**
 * Initialize object.
 * \return 0 Success
 * \return -1 Failure
 */
int32_t ExampleObject2Initialize()
{
	UAVObjMetadata metadata;

	// Register object with the object manager
	handle = UAVObjRegister(EXAMPLEOBJECT2_OBJID, EXAMPLEOBJECT2_NAME, 0, EXAMPLEOBJECT2_ISSINGLEINST, EXAMPLEOBJECT2_ISSETTINGS, EXAMPLEOBJECT2_NUMBYTES);
	if (handle == 0) return -1;

	// Initialize metadata
	metadata.telemetryAcked = 1;
	metadata.telemetryUpdateMode = UPDATEMODE_PERIODIC;
	metadata.telemetryUpdatePeriod = 100;
	metadata.gcsTelemetryAcked = 1;
	metadata.gcsTelemetryUpdateMode = UPDATEMODE_ONCHANGE;
	metadata.gcsTelemetryUpdatePeriod = 0;
	metadata.loggingUpdateMode = UPDATEMODE_NEVER;
	metadata.loggingUpdatePeriod = 0;
	UAVObjSetMetadata(handle, &metadata);

    // Initialize field values
    setDefaultFieldValues();

	// Done
	return 0;
}

/**
 * Initialize object fields with the default values.
 * If a default value is not specified the object fields
 * will be initialized to zero.
 */
static void setDefaultFieldValues()
{
	ExampleObject2Data data;
	ExampleObject2Get(&data);
	memset(&data, 0, sizeof(ExampleObject2Data));

	ExampleObject2Set(&data);
}

/**
 * Get object handle
 */
UAVObjHandle ExampleObject2Handle()
{
	return handle;
}



