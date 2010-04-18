/**
 ******************************************************************************
 *
 * @file       systemstats.h
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 * @brief      Implementation of the SystemStats object. This file has been 
 *             automatically generated by the UAVObjectGenerator.
 * 
 * @note       Object definition file: systemstats.xml. 
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

#ifndef SYSTEMSTATS_H
#define SYSTEMSTATS_H

#include "openpilot.h"

// Object constants
#define SYSTEMSTATS_OBJID 680908530U
#define SYSTEMSTATS_NAME "SystemStats"
#define SYSTEMSTATS_ISSINGLEINST 1
#define SYSTEMSTATS_ISSETTINGS 0
#define SYSTEMSTATS_NUMBYTES sizeof(SystemStatsData)

// Object access macros
#define SystemStatsGet(dataOut) UAVObjGetData(SystemStatsHandle(), dataOut)
#define SystemStatsSet(dataIn) UAVObjSetData(SystemStatsHandle(), dataIn)
#define SystemStatsInstGet(instId, dataOut) UAVObjGetInstanceData(SystemStatsHandle(), instId, dataOut)
#define SystemStatsInstSet(instId, dataIn) UAVObjSetInstanceData(SystemStatsHandle(), instId, dataIn)
#define SystemStatsConnectQueue(queue) UAVObjConnectQueue(SystemStatsHandle(), queue, EV_MASK_ALL_UPDATES)
#define SystemStatsConnectCallback(cb) UAVObjConnectCallback(SystemStatsHandle(), cb, EV_MASK_ALL_UPDATES)
#define SystemStatsCreateInstance() UAVObjCreateInstance(SystemStatsHandle())
#define SystemStatsRequestUpdate() UAVObjRequestUpdate(SystemStatsHandle())
#define SystemStatsRequestInstUpdate(instId) UAVObjRequestInstanceUpdate(SystemStatsHandle(), instId)
#define SystemStatsUpdated() UAVObjUpdated(SystemStatsHandle())
#define SystemStatsInstUpdated(instId) UAVObjUpdated(SystemStatsHandle(), instId)
#define SystemStatsGetMetadata(dataOut) UAVObjGetMetadata(SystemStatsHandle(), dataOut)
#define SystemStatsSetMetadata(dataIn) UAVObjSetMetadata(SystemStatsHandle(), dataIn)

// Object data
typedef struct {
    uint32_t FlightTime;
    uint16_t HeapRemaining;
    uint8_t CPULoad;

} __attribute__((packed)) SystemStatsData;

// Enumeration types


// Generic interface functions
int32_t SystemStatsInitialize();
UAVObjHandle SystemStatsHandle();

#endif // SYSTEMSTATS_H
