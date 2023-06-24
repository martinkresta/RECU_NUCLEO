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

#define  CO2_MAX_TARGET     1000  // 1000ppm
#define  CO2_BEST_TARGET    800
#define  CO2_HYSTERESIS     100

#define  FAN_MAX_NIGHT      25
#define  FAN_MAX_DAY_FULL   55
#define  FAN_MAX_DAY_ECO    35
#define  FAN_MIN            10
#define  FAN_ANTI_DRY       15
#define  RH_ANTI_DRY        53
#define  RH_ANTI_DRY_HIST   2


#define  INTERIEROR_COOLING_ENABLE_TEMP  210  // 21C

#define  ANTIFREEZE_TEMP_OUT_C10  40  //4C


#define  ANTIFREEZE_HYST_C10    40  // 4C

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

void RECON_Init(void);

void RECON_Update_1s(void);

void RECON_RemoteRequest(eRecuRemoteReqMode mode, uint16_t duration_s);


#endif /* INC_RECON_H_ */
