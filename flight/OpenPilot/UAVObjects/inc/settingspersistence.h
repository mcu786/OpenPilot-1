/**
 ******************************************************************************
 *
 * @file       settingspersistence.h
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 * @brief      Implementation of the SettingsPersistence object. This file has been 
 *             automatically generated by the UAVObjectGenerator.
 * 
 * @note       Object definition file: settingspersistence.xml. 
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

#ifndef SETTINGSPERSISTENCE_H
#define SETTINGSPERSISTENCE_H

// Object constants
#define SETTINGSPERSISTENCE_OBJID 3652432370U
#define SETTINGSPERSISTENCE_NAME "SettingsPersistence"
#define SETTINGSPERSISTENCE_ISSINGLEINST 1
#define SETTINGSPERSISTENCE_ISSETTINGS 0
#define SETTINGSPERSISTENCE_NUMBYTES sizeof(SettingsPersistenceData)

// Object access macros
#define SettingsPersistenceGet(dataOut) UAVObjGetData(SettingsPersistenceHandle(), dataOut)
#define SettingsPersistenceSet(dataIn) UAVObjSetData(SettingsPersistenceHandle(), dataIn)
#define SettingsPersistenceInstGet(instId, dataOut) UAVObjGetInstanceData(SettingsPersistenceHandle(), instId, dataOut)
#define SettingsPersistenceInstSet(instId, dataIn) UAVObjSetInstanceData(SettingsPersistenceHandle(), instId, dataIn)
#define SettingsPersistenceConnectQueue(queue) UAVObjConnectQueue(SettingsPersistenceHandle(), queue, EV_MASK_ALL_UPDATES)
#define SettingsPersistenceConnectCallback(cb) UAVObjConnectCallback(SettingsPersistenceHandle(), cb, EV_MASK_ALL_UPDATES)
#define SettingsPersistenceCreateInstance() UAVObjCreateInstance(SettingsPersistenceHandle())
#define SettingsPersistenceRequestUpdate() UAVObjRequestUpdate(SettingsPersistenceHandle())
#define SettingsPersistenceRequestInstUpdate(instId) UAVObjRequestInstanceUpdate(SettingsPersistenceHandle(), instId)
#define SettingsPersistenceUpdated() UAVObjUpdated(SettingsPersistenceHandle())
#define SettingsPersistenceInstUpdated(instId) UAVObjUpdated(SettingsPersistenceHandle(), instId)
#define SettingsPersistenceGetMetadata(dataOut) UAVObjGetMetadata(SettingsPersistenceHandle(), dataOut)
#define SettingsPersistenceSetMetadata(dataIn) UAVObjSetMetadata(SettingsPersistenceHandle(), dataIn)

// Object data
typedef struct {
    uint8_t Operation;

} __attribute__((packed)) SettingsPersistenceData;

// Field information
// Field Operation information
/* Enumeration options for field Operation */
typedef enum { SETTINGSPERSISTENCE_OPERATION_LOAD=0, SETTINGSPERSISTENCE_OPERATION_SAVE=1,  } SettingsPersistenceOperationOptions;


// Generic interface functions
int32_t SettingsPersistenceInitialize();
UAVObjHandle SettingsPersistenceHandle();

#endif // SETTINGSPERSISTENCE_H
