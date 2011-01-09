/**
 ******************************************************************************
 * @addtogroup UAVObjects OpenPilot UAVObjects
 * @{ 
 * @addtogroup FlightPlanStatus FlightPlanStatus 
 * @brief Status of the flight plan script
 *
 * Autogenerated files and functions for FlightPlanStatus Object
 
 * @{ 
 *
 * @file       flightplanstatus.h
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 * @brief      Implementation of the FlightPlanStatus object. This file has been 
 *             automatically generated by the UAVObjectGenerator.
 * 
 * @note       Object definition file: flightplanstatus.xml. 
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

#ifndef FLIGHTPLANSTATUS_H
#define FLIGHTPLANSTATUS_H

// Object constants
#define FLIGHTPLANSTATUS_OBJID 2726772894U
#define FLIGHTPLANSTATUS_NAME "FlightPlanStatus"
#define FLIGHTPLANSTATUS_METANAME "FlightPlanStatusMeta"
#define FLIGHTPLANSTATUS_ISSINGLEINST 1
#define FLIGHTPLANSTATUS_ISSETTINGS 0
#define FLIGHTPLANSTATUS_NUMBYTES sizeof(FlightPlanStatusData)

// Object access macros
/**
 * @function FlightPlanStatusGet(dataOut)
 * @brief Populate a FlightPlanStatusData object
 * @param[out] dataOut 
 */
#define FlightPlanStatusGet(dataOut) UAVObjGetData(FlightPlanStatusHandle(), dataOut)
#define FlightPlanStatusSet(dataIn) UAVObjSetData(FlightPlanStatusHandle(), dataIn)
#define FlightPlanStatusInstGet(instId, dataOut) UAVObjGetInstanceData(FlightPlanStatusHandle(), instId, dataOut)
#define FlightPlanStatusInstSet(instId, dataIn) UAVObjSetInstanceData(FlightPlanStatusHandle(), instId, dataIn)
#define FlightPlanStatusConnectQueue(queue) UAVObjConnectQueue(FlightPlanStatusHandle(), queue, EV_MASK_ALL_UPDATES)
#define FlightPlanStatusConnectCallback(cb) UAVObjConnectCallback(FlightPlanStatusHandle(), cb, EV_MASK_ALL_UPDATES)
#define FlightPlanStatusCreateInstance() UAVObjCreateInstance(FlightPlanStatusHandle())
#define FlightPlanStatusRequestUpdate() UAVObjRequestUpdate(FlightPlanStatusHandle())
#define FlightPlanStatusRequestInstUpdate(instId) UAVObjRequestInstanceUpdate(FlightPlanStatusHandle(), instId)
#define FlightPlanStatusUpdated() UAVObjUpdated(FlightPlanStatusHandle())
#define FlightPlanStatusInstUpdated(instId) UAVObjUpdated(FlightPlanStatusHandle(), instId)
#define FlightPlanStatusGetMetadata(dataOut) UAVObjGetMetadata(FlightPlanStatusHandle(), dataOut)
#define FlightPlanStatusSetMetadata(dataIn) UAVObjSetMetadata(FlightPlanStatusHandle(), dataIn)
#define FlightPlanStatusReadOnly(dataIn) UAVObjReadOnly(FlightPlanStatusHandle())

// Object data
typedef struct {
    uint8_t Status;
    uint8_t ErrorType;
    uint32_t ErrorFileID;
    uint32_t ErrorLineNum;
    float Debug;

} __attribute__((packed)) FlightPlanStatusData;

// Field information
// Field Status information
/* Enumeration options for field Status */
typedef enum { FLIGHTPLANSTATUS_STATUS_NONE=0, FLIGHTPLANSTATUS_STATUS_RUNNING=1, FLIGHTPLANSTATUS_STATUS_IDLE=2, FLIGHTPLANSTATUS_STATUS_VMINITERROR=3, FLIGHTPLANSTATUS_STATUS_SCRIPTSTARTERROR=4, FLIGHTPLANSTATUS_STATUS_RUNTIMEERROR=5 } FlightPlanStatusStatusOptions;
// Field ErrorType information
/* Enumeration options for field ErrorType */
typedef enum { FLIGHTPLANSTATUS_ERRORTYPE_NONE=0 } FlightPlanStatusErrorTypeOptions;
// Field ErrorFileID information
// Field ErrorLineNum information
// Field Debug information


// Generic interface functions
int32_t FlightPlanStatusInitialize();
UAVObjHandle FlightPlanStatusHandle();

#endif // FLIGHTPLANSTATUS_H

/**
 * @}
 * @}
 */
