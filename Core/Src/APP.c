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
#include "SENSIRION.h"
#include "RECON.h"

// local routines
static void ProcessMessage(s_CanRxMsg* msg);


sI2cSensor mSS;

TIM_OC_InitTypeDef mPWM;

uint8_t mFanPct = 20;

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
	SENS_Init();
	WDG_Init(3000);
	RECON_Init();


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

	// assign TEMP sensors on OW1 :
	// default sensor assignment:
	TEMP_AssignSensor(T305, VAR_TEMP_RECU_WC, 0);
	//TEMP_AssignSensor(T309, VAR_TEMP_RECU_WH, 0);
	TEMP_AssignSensor(T115, VAR_TEMP_RECU_FC, 0);
	//TEMP_AssignSensor(T116, VAR_TEMP_RECU_FH, 0);


	/* assign SENSIRION sensors  */

  // I2C1
  //DP Fresh
  mSS.BusHandle = &hi2c1;
  mSS.Id = 1;
  mSS.Type = st_SDP810_125;
  mSS.VarId_1 = VAR_DP_RECU_F;
  SENS_AddSensor(mSS);

  // CO2 waste hot
  mSS.BusHandle = &hi2c1;
  mSS.Id = 2;
  mSS.Type = st_SCD4x;
  mSS.VarId_1 = VAR_CO2_RECU;
  SENS_AddSensor(mSS);

  // RH + Temp waste hot
  mSS.BusHandle = &hi2c1;
  mSS.Id = 4;
  mSS.Type = st_SHT4x;
  mSS.VarId_1 = VAR_TEMP_RECU_WH;
  mSS.VarId_2 = VAR_RH_RECU_WH;
  SENS_AddSensor(mSS);


  // I2C2
  //DP Waste
  mSS.BusHandle = &hi2c2;
  mSS.Id = 3;
  mSS.Type = st_SDP810_125;
  mSS.VarId_1 = VAR_DP_RECU_W;
  SENS_AddSensor(mSS);


  // RH + Temp fresh  hot
  mSS.BusHandle = &hi2c2;
  mSS.Id = 0;
  mSS.Type = st_SHT4x;
  mSS.VarId_1 = VAR_TEMP_RECU_FH;
  mSS.VarId_2 = VAR_RH_RECU_FH;
  SENS_AddSensor(mSS);



	/* Configure CAN streamed variables */

	COM_AddStreamedVariable(VAR_TEMP_RECU_WC, 3000);
	COM_AddStreamedVariable(VAR_TEMP_RECU_WH, 3000);
	COM_AddStreamedVariable(VAR_TEMP_RECU_FC, 3000);
	COM_AddStreamedVariable(VAR_TEMP_RECU_FH, 3000);

	COM_AddStreamedVariable(VAR_RH_RECU_FH, 3000);
	COM_AddStreamedVariable(VAR_RH_RECU_WH, 3000);

	COM_AddStreamedVariable(VAR_CO2_RECU, 3000);
  COM_AddStreamedVariable(VAR_DP_RECU_F, 3000);
  COM_AddStreamedVariable(VAR_DP_RECU_W, 3000);


  COM_AddStreamedVariable(VAR_RECU_FAN_F, 3000);
  COM_AddStreamedVariable(VAR_RECU_FAN_W, 3000);
  //COM_AddStreamedVariable(VAR_CURR_RECU_A, 3000);



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
	sDateTime now = RTC_GetTime();
	newDayNumber = now.Day;
	if (dayNumber != newDayNumber)
	{
		dayNumber = newDayNumber;
		// TBD
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
		case CMD_RECU_REMOTE_REQ:
		  RECON_RemoteRequest(par1, par2);
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


