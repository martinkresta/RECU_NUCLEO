/*
 * RECON.h - Automatic controll of HVAC unit FANs speed control, based on CO2, temperatures and battery SOC
 *
 *  Created on: 2. 11. 2022
 *      Author: marti
 *
 */

#ifndef INC_RECON_H_
#define INC_RECON_H_

#include "main.h"
#include "com.h"
#include "uha_vars_gen.h"   /* Vars2 group types */

#define  CO2_MAX_TARGET     1000  // 1000ppm
#define  CO2_BEST_TARGET    800
#define  CO2_HYSTERESIS     100

#define  FAN_MAX_NIGHT      25
#define  FAN_MAX_DAY_FULL   55
#define  FAN_MAX_DAY_ECO    35
#define  FAN_MIN            20 // 10
#define  FAN_ANTI_DRY       20 // 15
#define  RH_ANTI_DRY        50   // default of the anti dry RH limit (settable, RECON_SetAntiDryRh)
#define  RECON_DEF_CO2_SETPOINT_PPM  600   // default CO2 where the fans start to rise (settable, RECON_SetCo2Setpoint)
#define  RH_ANTI_DRY_HIST   2


#define  INTERIEROR_COOLING_ENABLE_TEMP_C  21.0f  // degC
#define  COOLING_OUTSIDE_MARGIN_C  1.0f  // degC: summer cooling while outside < indoor + margin

#define  ANTIFREEZE_TEMP_OUT_C  4.0f  // degC


#define  ANTIFREEZE_HYST_C    4.0f  // degC

#define  LOW_SOC_OFF_THRESHOLD  30
#define  LOW_SOC_ECO_THRESHOLD  85

#define  MIN_ACTION_TIME_S  900
#define  TIME_HYST_DELAY_S  120

#define MAX_REMOTE_REQUEST_DURATION 1000


#define FAN_IN            0
#define FAN_OUT           1

/*
typedef enum
{
  errm_AutoControl = 0,   // cancelling remote request
  errm_SligtOvepressure,
  errm_MaxOverpressure,
  errm_SlightUnderpressure,
  errm_MaxUnderpressure
}eRemoteReqMode;*/

/* pFans, pStates: produced groups (Vars2.Recu.Fans, Vars2.States.Recu); read: own sensors (Vars2.Recu.Air), room and
 * outside temperatures (Vars2.Temperatures), battery SOC (Vars2.GlobalEnergy.Battery) */
void RECON_Init(sVarsRecuFans* pFans, sVarsStatesRecu* pStates, const sVarsRecuAir* pAir,
                const sVarsTemperatures* pTemps, const sVarsGlobalEnergyBattery* pBattery);

void RECON_Update_1s(void);

void RECON_RemoteRequest(eRecuRemoteReqMode mode, uint16_t duration_s);
/* command channel setpoints (clamped to CMD_RECU_*_MIN/MAX), used from the next RECON_Update_1s() */
void RECON_SetCo2Setpoint(float co2_ppm);
void RECON_SetAntiDryRh(float rh_pct);


#endif /* INC_RECON_H_ */
