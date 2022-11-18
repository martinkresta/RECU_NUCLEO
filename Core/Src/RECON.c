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

static uint16_t  mFanInPct;
static uint16_t mFanOutPct;
static uint16_t mFansPct;

static uint32_t mActionTimer;

static uint8_t mManualControl;



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
  mManualControl = 0;
}

void RECON_Update_1s(void)
{
  int16_t fc_temp, wc_temp, co2 ,soc, dumping_factor;
  uint16_t fan_limit, invalid ;

  mActionTimer++;

  // Chech day/night time
  sDateTime now = RTC_GetTime();
  if(now.Hour > 6 && now.Hour < 21) fan_limit = FAN_MAX_DAY;
  else fan_limit = FAN_MAX_NIGHT;

  // collect the variables
  invalid = 0;
  fc_temp = VAR_GetVariable(VAR_TEMP_RECU_FC,&invalid);  // input fresh air
  wc_temp = VAR_GetVariable(VAR_TEMP_RECU_WC,&invalid);  // input fresh air
  soc  = VAR_GetVariable(VAR_BAT_SOC,&invalid);     // battery soc

  if(invalid)  // safety configuration in case some critical inputs are not valid
  {
    SetFanPct(FAN_IN,0);
    SetFanPct(FAN_OUT,10);
    return ;
  }

  co2 = VAR_GetVariable(VAR_CO2_RECU, &invalid);

  // check SOC - low power configuration.
  if(soc < LOW_SOC_THRESHOLD)
  {
    SetFanPct(FAN_IN,0);
    SetFanPct(FAN_OUT,10);
    return ;
  }


  // check CO2

  if(mManualControl == 0)  // control by co2
  {
    if(co2 > 600)
    {
      mFansPct = (co2 - 600) / 10;
    }
    else
    {
      mFansPct = 10;
    }
  }
  else  // control by the button
  {
    fan_limit = 100;
    if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin))
   {
      mFansPct += 5;
      if(mFansPct > 100)
      {
        mFansPct = FAN_MIN;
      }
   }
  }


  // optimal Fan PWM is calculated with respect to CO2 concentration.

  // now we can adjust ratio of the two fans and also apply the antifreeze feature

  if(wc_temp > (ANTIFREEZE_TEMP_OUT_C10 + ANTIFREEZE_HYST_C10))  // no risk of freezink -> full ventilataion
  {
    mFanOutPct = mFansPct;
    mFanInPct = (mFansPct * 8) / 10;
    //mFanInPct = mFansPct;

    // range check

    if(mFanInPct < FAN_MIN) mFanInPct = FAN_MIN;
    if(mFanOutPct < FAN_MIN) mFanOutPct = FAN_MIN;
    if(mFanInPct > fan_limit) mFanInPct = fan_limit;
    if(mFanOutPct > fan_limit) mFanOutPct = fan_limit;
  }
  else if(wc_temp > ANTIFREEZE_TEMP_OUT_C10) // mitigating risk of freezing - reduced fresh(cold) air fan
  {
    dumping_factor = ((wc_temp - ANTIFREEZE_TEMP_OUT_C10) / 10);
    mFanOutPct = mFansPct;
    mFanInPct = (mFansPct * (5 + dumping_factor)) / 10;  // input fan limted to 50% of the output fan

    // range check
    if(mFanOutPct < FAN_MIN) mFanOutPct = FAN_MIN;
    if(mFanOutPct > fan_limit)
    {
      mFanOutPct = fan_limit;
      mFanInPct = (fan_limit * 5) / 10;
    }

  }
  else // high risk of freezing.  Turn off the input fan completely
  {
    mFanOutPct = 10;  // minimal output fan
    mFanInPct = 0;  // stopped input fan
  }




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
