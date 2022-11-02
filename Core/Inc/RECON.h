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

#define  CO2_MAX_TARGET     1000  // 1000ppm
#define  CO2_BEST_TARGET    800
#define  CO2_HYSTERESIS     100

#define  FAN_MAX_NIGHT      25
#define  FAN_MAX_DAY        50
#define  FAN_MIN            10

#define  ANTIFREEZE_TEMP    0
#define  LOW_SOC_THRESHOLD  40
#define  MIN_ACTION_TIME_S  1800


#define FAN_IN            0
#define FAN_OUT           1

void RECON_Init(void);

void RECON_Update_1s(void);


#endif /* INC_RECON_H_ */
