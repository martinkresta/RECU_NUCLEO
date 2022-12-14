/*
 * APP.h
 *
 *  Created on: Aug 14, 2021
 *      Author: Martin
*				Brief:  Main application and compile time configuration
 */

#ifndef INC_APP_H_
#define INC_APP_H_

#include "main.h"
#include "MCAN.h"

// CAN node ID
#define THIS_NODE				NODEID_RECU


void APP_Init();
void APP_Start(void);
void APP_ProcessMessages(void);
void APP_Update_1s(void);

#endif /* INC_APP_H_ */
