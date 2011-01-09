/**
 ******************************************************************************
 * @addtogroup OpenPilotModules OpenPilot Modules
 * @{ 
 * @addtogroup AltitudeModule Altitude Module
 * @brief Communicate with BMP085 and update @ref BaroAltitude "BaroAltitude UAV Object"
 * @{ 
 *
 * @file       altitude.c
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 * @brief      Altitude module, handles temperature and pressure readings from BMP085
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
 * Output object: BaroAltitude
 *
 * This module will periodically update the value of the BaroAltitude object.
 *
 */

#include "openpilot.h"
#include "baroaltitude.h"	// object that will be updated by the module

// Private constants
#define STACK_SIZE_BYTES 500
#define TASK_PRIORITY (tskIDLE_PRIORITY+1)
//#define UPDATE_PERIOD 100
#define UPDATE_PERIOD 25

// Private types

// Private variables
static xTaskHandle taskHandle;

// down sampling variables
#define alt_ds_size    4
static int32_t alt_ds_temp = 0;
static int32_t alt_ds_pres = 0;
static int alt_ds_count = 0;

        // Private functions
static void altitudeTask(void *parameters);

/**
 * Initialise the module, called on startup
 * \returns 0 on success or -1 if initialisation failed
 */
int32_t AltitudeInitialize()
{
	// Start main task
	xTaskCreate(altitudeTask, (signed char *)"Altitude", STACK_SIZE_BYTES/4, NULL, TASK_PRIORITY, &taskHandle);
	TaskMonitorAdd(TASKINFO_RUNNING_ALTITUDE, taskHandle);

	// init down-sampling data
    alt_ds_temp = 0;
    alt_ds_pres = 0;
    alt_ds_count = 0;

	return 0;
}

/**
 * Module thread, should not return.
 */
static void altitudeTask(void *parameters)
{
	BaroAltitudeData data;
	portTickType lastSysTime;

	PIOS_BMP085_Init();

	// Main task loop
	lastSysTime = xTaskGetTickCount();
	while (1)
	{
		// Update the temperature data
		PIOS_BMP085_StartADC(TemperatureConv);
		xSemaphoreTake(PIOS_BMP085_EOC, portMAX_DELAY);
		PIOS_BMP085_ReadADC();
		alt_ds_temp += PIOS_BMP085_GetTemperature();

		// Update the pressure data
		PIOS_BMP085_StartADC(PressureConv);
		xSemaphoreTake(PIOS_BMP085_EOC, portMAX_DELAY);
		PIOS_BMP085_ReadADC();
		alt_ds_pres += PIOS_BMP085_GetPressure();

		if (++alt_ds_count >= alt_ds_size)
        {
		    alt_ds_count = 0;

		    // Convert from 1/10ths of degC to degC
            data.Temperature = alt_ds_temp / (10.0 * alt_ds_size);
            alt_ds_temp = 0;

            // Convert from Pa to kPa
            data.Pressure = alt_ds_pres / (1000.0f * alt_ds_size);
            alt_ds_pres = 0;

            // Compute the current altitude (all pressures in kPa)
            data.Altitude = 44330.0 * (1.0 - powf((data.Pressure / (BMP085_P0 / 1000.0)), (1.0 / 5.255)));

            // Update the AltitudeActual UAVObject
            BaroAltitudeSet(&data);
        }

		// Delay until it is time to read the next sample
		vTaskDelayUntil(&lastSysTime, UPDATE_PERIOD / portTICK_RATE_MS);
	}
}

/**
  * @}
 * @}
 */
