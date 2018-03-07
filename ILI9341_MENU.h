/*
 * ILI9341_MENU.h
 *
 *  Created on: Feb 21, 2018
 *      Author: CarlosSalazar
 */

#ifndef ILI9341_MENU_H_
#define ILI9341_MENU_H_

#include "stm32f30x.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_conf.h"
#include "ILI9341.h"
#include "stm32f30x_gpio.h"

typedef enum{
	CREATE_OPTIONBOX = 0x01,
	CREATE_TEMPERATUREBOX = 0x02,
	CREATE_TEMPERATUREBOX2 = 0x03,
	CREATE_SAVEBOX = 0x04,
	CREATE_LINE=0x05,

} itemTypeStruct;

typedef enum {

	TYPE_SETTING_MENU = 0x01,
	TYPE_TEMPERATURE_MENU = 0x02,
	TYPE_PROFILE_MENU=0x03,

} menuTypeStruct;

typedef struct{
	uint16_t buttonThresX0;
	uint16_t buttonThresX1;
	uint16_t buttonThresY0;
	uint16_t buttonThresY1;

} buttonClickStruct;

typedef struct{
	uint16_t buttonX0;
	uint16_t buttonX1;
	uint16_t buttonY0;
	uint16_t buttonY1;
	uint16_t buttonRad;
	char *text;
	FontDef_t *format;
	uint16_t buttonStatus;
	uint16_t foreground_color;
	uint16_t background_color_1;
	uint16_t background_color_2;
	buttonClickStruct buttonArea;
} buttonStruct;
void createButton(buttonStruct button);
typedef enum {
	TYPE_FUNC = 0x01,
	TYPE_CHANGE_MENU = 0x02,
} FuncType;

typedef struct{
	uint16_t position;
	FuncType func;
	uint16_t refresh;
	uint16_t previous;
	uint16_t menuid;
	uint16_t item_count;
	itemTypeStruct itemType;
	menuTypeStruct menuType;
} menuStruct;

void create_menu(menuStruct menu);

#endif /* ILI9341_MENU_H_ */
