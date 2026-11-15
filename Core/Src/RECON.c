/*
 * RECON.h - Automatic controll of HVAC unit FANs speed control, based on CO2, temperatures and battery SOC
 *
 *  Created on: 2. 11. 2022
 *      Author: marti
 *
 */


#include "RECON.h"
#include "RTC.h"
#include "VARS2.h"


static TIM_OC_InitTypeDef mPWM;

static uint16_t  mFanInPct;
static uint16_t mFanOutPct;
static uint16_t mFansPct;

static uint32_t mActionTimer;
static uint32_t mHystTimer;
static uint32_t mRemoteRequestTimer;
static eRecuRemoteReqMode mRemoteMode;

static uint8_t mManualControl;
static uint8_t mAntiDryOn;
static uint8_t mSummerCoolingMode;

static sVarsRecuFans* mpFans;                     /* produced */
static sVarsStatesRecu* mpStates;                 /* produced: remote mode + active limitation (VARS2 send: on_change) */
static eRecuLimit mLimit;                         /* highest-priority limitation taken in this RECON_Update_1s() */
static const sVarsRecuAir* mpAir;                 /* read: own sensors */
static const sVarsTemperatures* mpTemps;          /* read: room and outside temperatures */
static const sVarsGlobalEnergyBattery* mpBattery; /* read: SOC */




void SetFanPct(uint8_t Fan, uint8_t pct);


/* limitation branch taken: keep the highest priority of this cycle (eRecuLimit: higher number = higher priority) */
static void Limit(eRecuLimit limit)
{
  if (limit > mLimit)
  {
    mLimit = limit;
  }
}

/* states for VARS2, written at every exit of RECON_Update_1s() */
static void PublishStates(void)
{
  mpStates->RemoteMode.value = (int32_t)mRemoteMode;
  mpStates->Limit.value = (int32_t)mLimit;
}


void RECON_Init(sVarsRecuFans* pFans, sVarsStatesRecu* pStates, const sVarsRecuAir* pAir,
                const sVarsTemperatures* pTemps, const sVarsGlobalEnergyBattery* pBattery)
{
  mpFans = pFans;
  mpStates = pStates;
  mLimit = RecuLimit_None;
  VARS2_SetGroupValid(mpStates, 1);   // states valid from now on (values: PublishStates() every second)
  mpAir = pAir;
  mpTemps = pTemps;
  mpBattery = pBattery;

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
  mAntiDryOn = 0;
  mRemoteMode = errm_AutoControl;
  mSummerCoolingMode = 0;
}

void RECON_Update_1s(void)
{
  float fh_temp, upstairs_temp, downstairs_temp, outside_temp, wc_temp, soc, wh_humidity;
  int16_t co2, dumping_factor;
  uint16_t fan_limit;

  mActionTimer++;
  mHystTimer++;
  mLimit = RecuLimit_None;


  sDateTime now = RTC_GetTime();
  if(now.Month > 4 && now.Month < 9)  // summer cooling period from may to august
  {
    mSummerCoolingMode = 1;
    fan_limit = FAN_MAX_DAY_FULL; // fan power is not limited during night time, because we need high intensity ventilation for cooling effect
  }
  else
  {
    mSummerCoolingMode = 0;
    // Chech day/night time
    if(now.Hour > 6 && now.Hour < 21)
    {
      fan_limit = FAN_MAX_DAY_FULL;
    }
    else
    {
      fan_limit = FAN_MAX_NIGHT;
    }
  }



  // collect the variables
  fh_temp = mpAir->FreshHot_C.value;
  wc_temp = mpAir->WasteCold_C.value;
  upstairs_temp = mpTemps->Elecon.Kidroom_C.value;        // indoor temperature
  downstairs_temp = mpTemps->IoboardD.Downstairs_C.value;  // indoor temperature
  outside_temp = mpTemps->Elecon.Outside_C.value;
  soc = mpBattery->Soc_pct.value;                         // battery soc

  // safety configuration in case some critical inputs are not valid (FreshCold_C is checked but not used)
  if(!(mpAir->FreshCold_C.valid && mpAir->FreshHot_C.valid && mpAir->WasteCold_C.valid &&
       mpTemps->Elecon.Kidroom_C.valid && mpTemps->IoboardD.Downstairs_C.valid &&
       mpTemps->Elecon.Outside_C.valid && mpBattery->Soc_pct.valid))
  {
    SetFanPct(FAN_IN,0);
    SetFanPct(FAN_OUT,10);
    Limit(RecuLimit_InvalidInputs);
    PublishStates();
    return ;
  }

  // validity of CO2 and humidity is not checked (as before): the last values are used
  co2 = (int16_t)mpAir->Co2_ppm.value;
  wh_humidity = mpAir->WasteHotRh_pct.value;


  // check SOC - low power configuration.
  if(soc < LOW_SOC_OFF_THRESHOLD)
  {
    SetFanPct(FAN_IN,0);
    SetFanPct(FAN_OUT,10);
    Limit(RecuLimit_BatteryOff);
    PublishStates();
    return ;
  }

  if(soc < LOW_SOC_ECO_THRESHOLD && fan_limit > FAN_MAX_DAY_ECO)
  {
    fan_limit = FAN_MAX_DAY_ECO;
    Limit(RecuLimit_Eco);
  }


  if(mManualControl == 0)  // automatic control
  {
    // select control mode (co2 based or anti-dry  limitation)
    if((mAntiDryOn == 0) && wh_humidity <= RH_ANTI_DRY) // activate anti dry
    {
      mAntiDryOn = 1;

    }
    else if((mAntiDryOn == 1) && wh_humidity > RH_ANTI_DRY + RH_ANTI_DRY_HIST) // deactivate anti dry feature and check CO2
    {
      mAntiDryOn = 0;
    }

    // set fan limit  if anti dry mode is active
    if (mAntiDryOn == 1)
    {
      fan_limit = FAN_ANTI_DRY;  // limit the fans to FAN_ANTI_DRY.  (The air humidity has higher prio than CO2 concentration)
      Limit(RecuLimit_AntiDry);
    }
    else
    {
      // do not overwrite the fan limit
    }

    // optimal Fan PWM is calculated with respect to CO2 concentration.
    if(co2 > 610)
    {
      mFansPct = (co2 - 600) / 10;
    }
    else
    {
      mFansPct = 10;
    }

  }

  else  // manual control by the button
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

  if(mSummerCoolingMode == 1)  // summer mode: we don't care about co2 and humidity, we just want to cool the interior NOTE: SUMMER BYPASS FLAP HAS TO BE OPEN!
  {
    float indoor_temp = upstairs_temp;
    if(downstairs_temp > indoor_temp)
    {
      indoor_temp = downstairs_temp;
    }
    if(now.Hour > 9 && now.Hour < 23)  // day time - cooling disabled (not effective)
    {
      mFanOutPct = FAN_MIN;
      mFanInPct = 0;
    }
    else
    {
      if(indoor_temp >= INTERIEROR_COOLING_ENABLE_TEMP_C)  // limit cooling only to when indoor is not too cold
      {
        if (outside_temp < (indoor_temp + COOLING_OUTSIDE_MARGIN_C) &&  mHystTimer > TIME_HYST_DELAY_S)
        {
          mFanOutPct = 35;
          mFanInPct = 35;
          mHystTimer = 0;
        }
        if(fh_temp > indoor_temp &&  mHystTimer > TIME_HYST_DELAY_S)
        {
          mFanOutPct = FAN_MIN;
          mFanInPct = 0;
          mHystTimer = 0;
        }
      }
    }
  }

  if(mSummerCoolingMode == 0)
  {
    // now we can adjust ratio of the two fans and also apply the antifreeze feature if needed
    if(wc_temp > (ANTIFREEZE_TEMP_OUT_C + ANTIFREEZE_HYST_C))  // no risk of freezink -> full ventilataion
    {
      mFanOutPct = mFansPct;

      // apply limitations
      if(mFanOutPct > fan_limit) mFanOutPct = fan_limit;

      // calculate input fan relatively to output fan
      mFanInPct = (mFanOutPct * 9) / 10;   // optimal fan ratio to avoid overpressure/underpressure
      //mFanInPct = mFansPct;

      // range check (this can corrupt optimal fan ratio, but whatever)
      if(mFanInPct != 0 && mFanInPct < FAN_MIN)  mFanInPct = FAN_MIN;
      if(mFanOutPct != 0 && mFanOutPct < FAN_MIN) mFanOutPct = FAN_MIN;

    }
    else if(wc_temp > ANTIFREEZE_TEMP_OUT_C) // mitigating risk of freezing - reduced fresh(cold) air fan
    {
      dumping_factor = (int16_t)(wc_temp - ANTIFREEZE_TEMP_OUT_C);   // whole degC above the limit (0..3)
      Limit(RecuLimit_AntifreezeReduced);
      mFanOutPct = mFansPct;
      mFanInPct = (mFansPct * (5 + dumping_factor)) / 10;  // input fan limted down to 50% of the output fan

      // range check
      if(mFanOutPct != 0 && mFanOutPct < FAN_MIN) mFanOutPct = FAN_MIN;
      if(mFanOutPct > fan_limit)
      {
        mFanOutPct = fan_limit;
        mFanInPct = (fan_limit * (5 + dumping_factor)) / 10;
      }
    }
  }

  // Timing of remote requests
  if(mRemoteRequestTimer > 0)
  {
    mRemoteRequestTimer --;
  }
  else  // cancel the remote request mode, if time has elapsed
  {
    mRemoteMode = errm_AutoControl;
  }


  // adjust the fan_x values according to remote request
  switch (mRemoteMode)
  {
    case errm_AutoControl:
        // do nothing, remote request off
      break;
    case errm_SligtOvepressure:
      mFanInPct = mFanOutPct + 25;
      break;
    case errm_MaxOverpressure:
      mFanInPct = 95;   // full fan power
      break;
    case errm_SlightUnderpressure:
      mFanOutPct = mFanInPct + 25;
      break;
    case errm_MaxUnderpressure:
      mFanOutPct = 95;
      mFanInPct = 0;
      break;
  }





  // safety Anti freeze feature  which cannot be overwritten by any control mode

  if(wc_temp < ANTIFREEZE_TEMP_OUT_C)  // less then 4.0 C  // high risk of freezing.  Turn off the input fan completely
  {
    mFanOutPct = 10;  // minimal output fan
    mFanInPct = 0;  // stopped input fan
    Limit(RecuLimit_AntifreezeStop);
  }


  SetFanPct(FAN_IN,mFanInPct);
  SetFanPct(FAN_OUT,mFanOutPct);
  PublishStates();

}


void RECON_RemoteRequest(eRecuRemoteReqMode mode, uint16_t duration_s)
{
  if(duration_s > MAX_REMOTE_REQUEST_DURATION)
  {
    duration_s = MAX_REMOTE_REQUEST_DURATION;
  }
  mRemoteRequestTimer = duration_s;
  mRemoteMode = mode;
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
    VarSet(&mpFans->Fresh_pct, (float)pct, 1);
  }
  else if(Fan == FAN_OUT)
  {
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
    if (HAL_TIM_PWM_ConfigChannel(&htim1, &mPWM, TIM_CHANNEL_2) != HAL_OK)
    {
      Error_Handler();
    }
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
    VarSet(&mpFans->Waste_pct, (float)pct, 1);
  }


}
