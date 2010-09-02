/**
 ******************************************************************************
 * @addtogroup UAVObjects OpenPilot UAVObjects
 * @{ 
 * @addtogroup AHRSSettings AHRSSettings 
 * @brief Settings for the @ref AHRSCommsModule to control the algorithm and what is updated
 *
 * Autogenerated files and functions for AHRSSettings Object
 
 * @{ 
 *
 * @file       ahrssettings.h
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 * @brief      Implementation of the AHRSSettings object. This file has been 
 *             automatically generated by the UAVObjectGenerator.
 * 
 * @note       Object definition file: ahrssettings.xml. 
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

#ifndef AHRSSETTINGS_H
#define AHRSSETTINGS_H

// Object constants
#define AHRSSETTINGS_OBJID 1565605328U
#define AHRSSETTINGS_NAME "AHRSSettings"
#define AHRSSETTINGS_METANAME "AHRSSettingsMeta"
#define AHRSSETTINGS_ISSINGLEINST 1
#define AHRSSETTINGS_ISSETTINGS 1
#define AHRSSETTINGS_NUMBYTES sizeof(AHRSSettingsData)

// Object access macros
/**
 * @function AHRSSettingsGet(dataOut)
 * @brief Populate a AHRSSettingsData object
 * @param[out] dataOut 
 */
#define AHRSSettingsGet(dataOut) UAVObjGetData(AHRSSettingsHandle(), dataOut)
#define AHRSSettingsSet(dataIn) UAVObjSetData(AHRSSettingsHandle(), dataIn)
#define AHRSSettingsInstGet(instId, dataOut) UAVObjGetInstanceData(AHRSSettingsHandle(), instId, dataOut)
#define AHRSSettingsInstSet(instId, dataIn) UAVObjSetInstanceData(AHRSSettingsHandle(), instId, dataIn)
#define AHRSSettingsConnectQueue(queue) UAVObjConnectQueue(AHRSSettingsHandle(), queue, EV_MASK_ALL_UPDATES)
#define AHRSSettingsConnectCallback(cb) UAVObjConnectCallback(AHRSSettingsHandle(), cb, EV_MASK_ALL_UPDATES)
#define AHRSSettingsCreateInstance() UAVObjCreateInstance(AHRSSettingsHandle())
#define AHRSSettingsRequestUpdate() UAVObjRequestUpdate(AHRSSettingsHandle())
#define AHRSSettingsRequestInstUpdate(instId) UAVObjRequestInstanceUpdate(AHRSSettingsHandle(), instId)
#define AHRSSettingsUpdated() UAVObjUpdated(AHRSSettingsHandle())
#define AHRSSettingsInstUpdated(instId) UAVObjUpdated(AHRSSettingsHandle(), instId)
#define AHRSSettingsGetMetadata(dataOut) UAVObjGetMetadata(AHRSSettingsHandle(), dataOut)
#define AHRSSettingsSetMetadata(dataIn) UAVObjSetMetadata(AHRSSettingsHandle(), dataIn)
#define AHRSSettingsReadOnly(dataIn) UAVObjReadOnly(AHRSSettingsHandle())

// Object data
typedef struct {
    uint8_t Algorithm;
    uint8_t UpdateRaw;
    uint8_t UpdateFiltered;
    int32_t UpdatePeriod;

} __attribute__((packed)) AHRSSettingsData;

// Field information
// Field Algorithm information
/* Enumeration options for field Algorithm */
typedef enum { AHRSSETTINGS_ALGORITHM_SIMPLE=0, AHRSSETTINGS_ALGORITHM_INSGPS=1 } AHRSSettingsAlgorithmOptions;
// Field UpdateRaw information
/* Enumeration options for field UpdateRaw */
typedef enum { AHRSSETTINGS_UPDATERAW_FALSE=0, AHRSSETTINGS_UPDATERAW_TRUE=1 } AHRSSettingsUpdateRawOptions;
// Field UpdateFiltered information
/* Enumeration options for field UpdateFiltered */
typedef enum { AHRSSETTINGS_UPDATEFILTERED_FALSE=0, AHRSSETTINGS_UPDATEFILTERED_TRUE=1 } AHRSSettingsUpdateFilteredOptions;
// Field UpdatePeriod information


// Generic interface functions
int32_t AHRSSettingsInitialize();
UAVObjHandle AHRSSettingsHandle();

#endif // AHRSSETTINGS_H

/**
 * @}
 * @}
 */
