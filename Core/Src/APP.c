/*
 * APP.c
 *
 *  Created on: Aug 14, 2021
 *      Author: Martin
 *      Brief:  Main application and compile time configuration
 *
 */


#include "APP.h"
#include "scheduler.h"
#include "UI.h"
#include "OW.h"
#include "TEMP.h"
#include "VARS.h"
#include "MCAN.h"
#include "COM.h"
#include "RTC.h"
#include "watchdog.h"

// local routines
static void ProcessMessage(s_CanRxMsg* msg);

// public methods
void APP_Init(void)
{
	sUIHwInit uihw;
	Scheduler_Init();
	OW_Init();
	TEMP_Init();
	VAR_Init();
	MCAN_Init(&hcan1, THIS_NODE);
	COM_Init(THIS_NODE);
	WDG_Init(3000);


  /*Assign pins for onboard UI  */
	uihw.Led_Life.Pin = LD2_Pin;
	uihw.Led_Life.Port = LD2_GPIO_Port;
	uihw.Led_Life.Logic = eUIL_POSITIVE;

	uihw.Led_R.Pin = LED_R_Pin;
	uihw.Led_R.Port = LED_R_GPIO_Port;
	uihw.Led_R.Logic = eUIL_NEGATIVE;

	uihw.Led_G.Pin = LED_G_Pin;
	uihw.Led_G.Port = LED_G_GPIO_Port;
	uihw.Led_G.Logic = eUIL_NEGATIVE;

	uihw.Led_B.Pin = LED_B_Pin;
	uihw.Led_B.Port = LED_B_GPIO_Port;
	uihw.Led_B.Logic = eUIL_NEGATIVE;

	uihw.Buzzer.Pin = BUZZ_Pin;
	uihw.Buzzer.Port = BUZZ_GPIO_Port;
	uihw.Buzzer.Logic = eUIL_POSITIVE;

	UI_Init(&uihw);
	UI_LED_Life_SetMode(eUI_BLINKING_SLOW);



	/* Lights and buttons assignment */


	/*Gestures definition*/


	/* Temperature sensors configuration */
	// define hardware OW busses
	TEMP_AddHwBus(0,OW1_GPIO_Port, OW1_Pin);

	// assign sensors on OW1 :
	// default sensor assignment:
	TEMP_AssignSensor(T305, VAR_TEMP_RECU_WC, 0);
	TEMP_AssignSensor(T309, VAR_TEMP_RECU_WH, 0);

	/* Configure CAN streamed variables */

	COM_AddStreamedVariable(VAR_TEMP_RECU_WC, 3000);
	COM_AddStreamedVariable(VAR_TEMP_RECU_WH, 3000);



	/*configure elmeters*/

//	ELM_AddMeter(ELM_OTHER, EL1_Pin, VAR_CONS_OTHER_WH);

}

void APP_Start(void)
{

	MCAN_Start();
	while (1)   // endless loop
	{
		Scheduler_Check_Flag();

		s_CanRxMsg rmsg;
		while(1 == COM_GetRxMessage(&rmsg))  // process all messages in buffer
		{
				ProcessMessage(&rmsg);
		}
	}
}

void APP_Update_1s(void)
{

	static uint8_t dayNumber = 0;
	uint8_t newDayNumber = 0;
	int16_t consKitchen;
	int16_t powKitchen;
	uint16_t invalid;
	sDateTime now = RTC_GetTime();
	uint16_t pumpPeriod;
	newDayNumber = now.Day;
	if (dayNumber != newDayNumber)
	{
		dayNumber = newDayNumber;
		// TBD
	}


	  if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin))
	  {
	     UI_LED_Life_SetMode(eUI_BLINKING_SLOW);
	  }

}

static void ProcessMessage(s_CanRxMsg* msg)
{
	uint16_t cmd = msg->header.StdId & 0xFF0;  // maskout nodeid
	//uint8_t producer = msg->header.StdId & 0x00F;  // maskout cmd
	int16_t par1,par2,par3; //,par4;
	uint32_t unixtime = 0;
	par1 = (msg->data[0] << 8) | msg->data[1];
	par2 = (msg->data[2] << 8) | msg->data[3];
	par3 = (msg->data[4] << 8) | msg->data[5];
	// par4 = (msg->data[6] << 8) | msg->data[7];

	switch (cmd)
	{
		case CMD_BUTTON_STATE:
			break;
		case CMD_VAR_VALUE:
			VAR_SetVariable(par1, par2, par3);
			break;
		case CMD_GESTURE:
			// TBD
			break;
		case CMD_RTC_SYNC: // set RTC time
			unixtime |= msg->data[0] << 24;
			unixtime |= msg->data[1] << 16;
			unixtime |= msg->data[2] << 8;
			unixtime |= msg->data[3];
			RTC_SetUnixTime(unixtime);
			break;
	}
	return;
}


