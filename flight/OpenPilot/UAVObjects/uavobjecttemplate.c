/**
 ******************************************************************************
 *
 * @file       $(NAMELC).c
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 * @brief      Implementation of the $(NAME) object. This file has been 
 *             automatically generated by the UAVObjectGenerator.
 * 
 * @note       Object definition file: $(XMLFILE). 
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

#include "$(NAMELC).h"

// Private variables
UAVObjHandle handle;

/**
 * Initialize object.
 * \return 0 Success
 * \return -1 Failure
 */
int32_t $(NAME)Initialize()
{
	UAVObjMetadata metadata;

	// Register object with the object manager
	handle = UAVObjRegister($(NAMEUC)_OBJID, $(NAMEUC)_NAME, 0, $(NAMEUC)_SINGLEINST, $(NAMEUC)_NUMBYTES);
	if (handle == 0) return -1;

	// Initialize metadata
	metadata.telemetryAcked = $(FLIGHTTELEM_ACKED);
	metadata.telemetryUpdateMode = $(FLIGHTTELEM_UPDATEMODE);
	metadata.telemetryUpdatePeriod = $(FLIGHTTELEM_UPDATEPERIOD);
	metadata.gcsTelemetryAcked = $(GCSTELEM_ACKED);
	metadata.gcsTelemetryUpdateMode = $(GCSTELEM_UPDATEMODE);
	metadata.gcsTelemetryUpdatePeriod = $(GCSTELEM_UPDATEPERIOD);
	metadata.loggingUpdateMode = $(LOGGING_UPDATEMODE);
	metadata.loggingUpdatePeriod = $(LOGGING_UPDATEPERIOD);
	UAVObjSetMetadata(handle, &metadata);

	// Done
	return 0;
}

/**
 * Get object handle
 */
UAVObjHandle $(NAME)GetHandle()
{
	return handle;
}


