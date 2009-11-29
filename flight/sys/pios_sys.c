/**
 ******************************************************************************
 *
 * @file       pios_sys.c  
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2009.   
 * @brief      Sets up basic system hardware, functions are called from Main.
 * @see        The GNU Public License (GPL) Version 3
 * @defgroup   PIOS_SYS
 * @{
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


/* Project Includes */
#include "pios.h"


/* Private Function Prototypes */
void NVIC_Configuration(void);


/* Local Variables */
FATFS Fatfs[_DRIVES];	// File system object for each logical drive */


/**
* Function Name  : SysInit
* Description    : Brings up the System and Initializes peripherals 
* Input          : None
* Output         : None
* Return         : None
*/
//TODO: Get these in the right order, settings need to be loaded ASAP
void SysInit(void)
{
	/* Setup STM32 system (RCC, clock, PLL and Flash configuration) - CMSIS Function */
	SystemInit();
	
	/* Initialize NVIC */
	NVIC_Configuration();
	
	/* Initialize LEDs */
	LED_INIT();
	
	/* Initialize FatFS disk */
	if(f_mount(0, &Fatfs[0]) != FR_OK) {
		/* Failed to mount MicroSD filesystem, flash LED1 forever */
		while(1) {
			for(int i = 0; i < 1000; i++);
			LED_TOGGLE(LED1);
		}
	}
	
	/* Call LoadSettings which populates System Vars */
	LoadSettings();
	
}


/**
* Function Name  : GPIO_Configuration
* Description    : Configures base level GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*/
void GPIO_Configuration(void)
{

}


/**
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location and SysTick
* Input          : None
* Output         : None
* Return         : None
*/
void NVIC_Configuration(void)
{
	/* Set the Vector Table base address as specified in .ld file */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);

	/* 4 bits for Interupt priorities so no sub priorities */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	/* Configure HCLK clock as SysTick clock source. */
	SysTick_CLKSourceConfig( SysTick_CLKSource_HCLK );
}
