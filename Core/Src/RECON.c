/*
 * RECON.h - Automatic controll of HVAC unit FANs speed control, based on CO2, temperatures and battery SOC
 *
 *  Created on: 2. 11. 2022
 *      Author: marti
 *
 */


#include "RECON.h"
#include "RTC.h"
#include "VARS.h"


static TIM_OC_InitTypeDef mPWM;

static uint8_t  mFanInPct;
static uint8_t mFanOutPct;

static uint8_t mFansPct;

static uint32_t mActionTimer;



void SetFanPct(uint8_t Fan, uint8_t pct);


void RECON_Init(void)
{
  mPWM.OCMode = TIM_OCMODE_PWM1;
  mPWM.Pulse = 200;
  mPWM.OCPolarity = TIM_OCPOLARITY_HIGH;
  mPWM.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  mPWM.OCFastMode = TIM_OCFAST_DISABLE;
  mPWM.OCIdleState = TIM_OCIDLESTATE_RESET;
  mPWM.OCNIdleState = TIM_OCNIDLESTATE_RESET;

  mFanInPct = 30;
  mFanOutPct = 30;
  SetFanPct(FAN_IN,mFanInPct);
  SetFanPct(FAN_OUT,mFanOutPct);
}

void RECON_Update_1s(void)
{
  int16_t input_temp, co2 ,soc;
  uint8_t fan_limit, invalid ;

  mActionTimer++;

  // Chech day/night time
  sDateTime now = RTC_GetTime();
  if(now.Hour > 6 && now.Hour < 21) fan_limit = FAN_MAX_DAY;
  else fan_limit = FAN_MAX_NIGHT;

  // collect the variables
  invalid = 0;
  input_temp = VAR_GetVariable(VAR_TEMP_RECU_FC,&invalid);  // input fresh air
  co2 = VAR_GetVariable(VAR_CO2_RECU, &invalid);
  soc  = VAR_GetVariable(VAR_BAT_SOC,&invalid);     // battery soc


  if(invalid)
  {
    // TBD
    return;
  }

  // check SOC
  if(soc < LOW_SOC_THRESHOLD)
  {
    SetFanPct(FAN_IN,0);
    SetFanPct(FAN_OUT,10);
    return ;
  }

  // check freezing condition
  if(soc < LOW_SOC_THRESHOLD)
  {
    SetFanPct(FAN_IN,0);
    SetFanPct(FAN_OUT,15);
    return;
  }


  // check CO2
  if(mActionTimer > MIN_ACTION_TIME_S)
  {
    if(co2 > CO2_MAX_TARGET)
    {
      mFanInPct += 5;
      mFanOutPct += 5;
      mActionTimer = 0;
    }
    else if(co2 < CO2_MAX_TARGET - CO2_HYSTERESIS)
    {
      mFanInPct -= 5;
      mFanOutPct -= 5;
      mActionTimer = 0;
    }
  }

  // range check

  if(mFanInPct < FAN_MIN) mFanInPct = FAN_MIN;
  if(mFanOutPct < FAN_MIN) mFanOutPct = FAN_MIN;


  if(mFanInPct > fan_limit) mFanInPct = fan_limit;
  if(mFanOutPct > fan_limit) mFanOutPct = fan_limit;


  SetFanPct(FAN_IN,mFanInPct);
  SetFanPct(FAN_OUT,mFanOutPct);










}


void SetFanPct(uint8_t Fan, uint8_t pct)
{
  mPWM.Pulse = pct * 10;
  if(Fan == FAN_IN)
  {
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
    if (HAL_TIM_PWM_ConfigChannel(&htim1, &mPWM, TIM_CHANNEL_1) != HAL_OK)
    {
      Error_Handler();
    }
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    VAR_SetVariable(VAR_RECU_FAN_F, pct,1);
  }
  else if(Fan == FAN_OUT)
  {
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
    if (HAL_TIM_PWM_ConfigChannel(&htim1, &mPWM, TIM_CHANNEL_2) != HAL_OK)
    {
      Error_Handler();
    }
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    VAR_SetVariable(VAR_RECU_FAN_W, pct,1);
  }


}
