/**
 ******************************************************************************
 * @addtogroup OpenPilotModules OpenPilot Modules
 * @{
 * @addtogroup BatteryModule Battery Module
 * @brief Measures battery voltage and current
 * Updates the FlightBatteryState object
 * @{
 *
 * @file       battery.c
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 * @brief      Module to read the battery sensor periodically
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

/**
 * Output object: FlightBatteryState
 *
 * This module will periodically generate information on the battery state.
 *
 * UAVObjects are automatically generated by the UAVObjectGenerator from
 * the object definition XML file.
 *
 * Modules have no API, all communication to other modules is done through UAVObjects.
 * However modules may use the API exposed by shared libraries.
 * See the OpenPilot wiki for more details.
 * http://www.openpilot.org/OpenPilot_Application_Architecture
 *
 */

#include "openpilot.h"

#include "flightbatterystate.h"
#include "batterysettings.h"

//
// Configuration
//
#define DEBUG_PORT			PIOS_COM_TELEM_RF
#define STACK_SIZE			1024
#define TASK_PRIORITY		(tskIDLE_PRIORITY + 1)
#define SAMPLE_PERIOD_MS	500
#define POWER_SENSOR_VERSION		1


//#define ENABLE_DEBUG_MSG

#ifdef ENABLE_DEBUG_MSG
#define DEBUG_MSG(format, ...) PIOS_COM_SendFormattedString(DEBUG_PORT, format, ## __VA_ARGS__)
#else
#define DEBUG_MSG(format, ...)
#endif

// Private types

// Private variables
static xTaskHandle taskHandle;

// Private functions
static void task(void *parameters);

/**
 * Initialise the module, called on startup
 * \returns 0 on success or -1 if initialisation failed
 */
int32_t BatteryInitialize(void)
{
	// Start main task
	xTaskCreate(task, (signed char *)"Battery", STACK_SIZE, NULL, TASK_PRIORITY, &taskHandle);

	return 0;
}

/**
 * Module thread, should not return.
 */
static void task(void *parameters)
{
	portTickType lastSysTime;
	FlightBatteryStateData flightBatteryData;

	PIOS_COM_ChangeBaud(DEBUG_PORT, 57600);

	DEBUG_MSG("Battery Started\n");

	AlarmsSet(SYSTEMALARMS_ALARM_BATTERY, SYSTEMALARMS_ALARM_ERROR);

	lastSysTime = xTaskGetTickCount();

	while (1) {
#if (POWER_SENSOR_VERSION == 1)
		// TODO: Compare with floating point calculations
		uint cnt = 0;
		uint32_t mAs = 0;
		uint32_t mV, mA;

		mV = PIOS_ADC_PinGet(2) * 1257 / 100;
		mA = (PIOS_ADC_PinGet(1) - 28) * 4871 / 100;
		mAs += mA * SAMPLE_PERIOD_MS / 1000;	// FIXME: Use real time between samples

		DEBUG_MSG("%03d %06d => %06dmV   %06d => %06dmA  %06dmAh\n", cnt, PIOS_ADC_PinGet(2), mV, PIOS_ADC_PinGet(1), mA, mAs / 3600);
		cnt++;

		flightBatteryData.Voltage = (float)mV / 1000;
		flightBatteryData.Current = (float)mA / 1000;
		flightBatteryData.ConsumedEnergy = mAs / 3600;
		FlightBatteryStateSet(&flightBatteryData);

#endif
#if (POWER_SENSOR_VERSION == 2)
		portTickType thisSysTime;
		BatterySettingsData batterySettings;
		float dT = SAMPLE_PERIOD_MS / 1000;
		float Bob;
		float energyRemaining;

		// Check how long since last update
		thisSysTime = xTaskGetTickCount();
		if(thisSysTime > lastSysTime) // reuse dt in case of wraparound
			dT = (thisSysTime - lastSysTime) / (portTICK_RATE_MS / 1000.0f);
		lastSysTime = thisSysTime;

		BatterySettingsGet(&batterySettings);

		//calculate the battery parameters
		flightBatteryData.Voltage = ((float)PIOS_ADC_PinGet(2)) * 0.008065 * batterySettings.Calibrations[BATTERYSETTINGS_CALIBRATIONS_VOLTAGE]; //in Volts
		flightBatteryData.Current = ((float)PIOS_ADC_PinGet(1)) * 0.016113 * batterySettings.Calibrations[BATTERYSETTINGS_CALIBRATIONS_CURRENT]; //in Amps
	Bob =dT; // FIXME: something funky happens if I don't do this... Andrew
		flightBatteryData.ConsumedEnergy += (flightBatteryData.Current * 1000.0 * dT / 3600.0) ;//in mAh

		if (flightBatteryData.Current > flightBatteryData.PeakCurrent)flightBatteryData.PeakCurrent = flightBatteryData.Current; //in Amps
		flightBatteryData.AvgCurrent=(flightBatteryData.AvgCurrent*0.8)+(flightBatteryData.Current*0.2); //in Amps

		//sanity checks
		if (flightBatteryData.AvgCurrent<0)flightBatteryData.AvgCurrent=0.0;
		if (flightBatteryData.PeakCurrent<0)flightBatteryData.PeakCurrent=0.0;
		if (flightBatteryData.ConsumedEnergy<0)flightBatteryData.ConsumedEnergy=0.0;

		energyRemaining = batterySettings.BatteryCapacity - flightBatteryData.ConsumedEnergy; // in mAh
		flightBatteryData.EstimatedFlightTime = ((energyRemaining / (flightBatteryData.AvgCurrent*1000.0))*3600.0);//in Sec

		//generate alarms where needed...
		if ((flightBatteryData.Voltage<=0)&&(flightBatteryData.Current<=0))
		{
			AlarmsSet(SYSTEMALARMS_ALARM_BATTERY, SYSTEMALARMS_ALARM_ERROR);
			AlarmsSet(SYSTEMALARMS_ALARM_FLIGHTTIME, SYSTEMALARMS_ALARM_ERROR);
		}
		else
		{
			if (flightBatteryData.EstimatedFlightTime < 30) AlarmsSet(SYSTEMALARMS_ALARM_FLIGHTTIME, SYSTEMALARMS_ALARM_CRITICAL);
			else if (flightBatteryData.EstimatedFlightTime < 60) AlarmsSet(SYSTEMALARMS_ALARM_FLIGHTTIME, SYSTEMALARMS_ALARM_WARNING);
			else AlarmsClear(SYSTEMALARMS_ALARM_FLIGHTTIME);

			// FIXME: should make the battery voltage detection dependent on battery type.
			if ((flightBatteryData.Voltage < batterySettings.BatteryVoltage * 0.75)||(flightBatteryData.Current > batterySettings.BatteryCapacity * 0.85))AlarmsSet(SYSTEMALARMS_ALARM_BATTERY, SYSTEMALARMS_ALARM_CRITICAL);
			else if ((flightBatteryData.Voltage < batterySettings.BatteryVoltage * 0.85)||(flightBatteryData.Current > batterySettings.BatteryCapacity * 0.95))AlarmsSet(SYSTEMALARMS_ALARM_BATTERY, SYSTEMALARMS_ALARM_WARNING);
			else AlarmsClear(SYSTEMALARMS_ALARM_BATTERY);
		}

		FlightBatteryStateSet(&flightBatteryData);
#endif
		vTaskDelayUntil(&lastSysTime, SAMPLE_PERIOD_MS / portTICK_RATE_MS);
	}
}

/**
  * @}
  */

/**
 * @}
 */
