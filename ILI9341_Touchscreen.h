/*
 * ILI9341_Touchscreen.h
 *
 *  Created on: Feb 9, 2018
 *      Author: CarlosSalazar
 */

#ifndef ILI9341_TOUCHSCREEN_H_
#define ILI9341_TOUCHSCREEN_H_

#include "stm32f30x.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_conf.h"
#include "ILI9341.h"
#include "stm32f30x_gpio.h"

// If the touch pad has been pressed
#define TOUCHPAD_PRESSED  		1
#define TOUCHPAD_NOT_PRESSED 	0

//Data noisy
#define TOUCHPAD_DATA_OK		1
#define TOUCHPAD_DATA_NOISY		0

//Calibration
#define X_OFFSET				48.96
#define Y_OFFSET				37.72
#define X_MAGNITUDE				1.3
#define Y_MAGNITUDE				1.3

//Converting 16-bit value into screen coordinates
#define X_TRANSLATION			204
#define	Y_TRANSLATION			273

#define CMD_RDY					0X90
#define CMD_RDX					0XD0

// Increase accuracy, read samples
#define NO_OF_POSITION_SAMPLES	1000

//Internal Touchpad command, do not call directly
uint16_t TP_Read(void);

//Internal Touchpad command, do not call directly
void TP_Write(uint8_t value);

//Read coordinates of touchscreen press. Position[0] = X, Position[1] = Y
uint8_t TP_Read_Coordinates(uint16_t Coordinates[2]);

//Check if Touchpad was pressed. Returns TOUCHPAD_PRESSED (1) or TOUCHPAD_NOT_PRESSED (0)
uint8_t TP_Touchpad_Pressed(void);

#endif /* ILI9341_TOUCHSCREEN_H_ */
