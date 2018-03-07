/*
 * ILI9341_MENU_CONTROLLER.c
 *
 *  Created on: Feb 23, 2018
 *      Author: CarlosSalazar
 */
#include "ILI9341.h"
#include "ILI9341_MENU.h"
#include "ILI9341_GUI_Conf_desird.h"
#include "common.h"
#include "math.h"
#include "ILI9341_Touchscreen.h"
#include "ILI9341_fonts.h"
#include "ILI9341_MENU_CONTROLLER.h"
volatile  menuStruct men[MENU_COUNT];
volatile uint8_t active_menu;
uint16_t position_array[2];
uint16_t xpos;
uint16_t ypos;
float desired1;
void menu_init(void)
{
	buttonStruct button1;
	button1.buttonX0  = 140;
	button1.buttonY0  = 100;
	button1.buttonX1  = 120;
	button1.buttonY1  = 180;
	button1.buttonRad = 30;
	button1.buttonStatus = 0;
	button1.background_color_1 = ILI9341_COLOR_GREEN;
	button1.background_color_2 = ILI9341_COLOR_BLACK;
	button1.foreground_color   = ILI9341_COLOR_WHITE;
	button1.buttonArea.buttonThresX0 = 0;
	button1.buttonArea.buttonThresX1 = 0;
	button1.buttonArea.buttonThresY0 = 0;
	button1.buttonArea.buttonThresY1 = 0;
	createButton(button1);

	men[0].item_count = 1;
	men[0].position =0;
	men[0].menuType = TYPE_SETTING_MENU;
	men[0].itemType = CREATE_OPTIONBOX;
	men[0].refresh=0;
	men[0].func = TYPE_CHANGE_MENU;
	men[0].menuid = 1; // menuid can't be '0'!

	men[2].item_count = 2;
	men[2].position = 0;
	men[2].menuType = TYPE_TEMPERATURE_MENU;
	men[2].itemType = CREATE_OPTIONBOX;
	men[2].refresh = 0;
	men[2].func = TYPE_CHANGE_MENU;
	men[2].menuid = 12; // menuid can't be '0'!

	men[3].item_count =5;
	men[3].position = 0;
	men[3].menuType = TYPE_PROFILE_MENU;
	men[3].itemType = CREATE_OPTIONBOX;
	men[3].refresh = 0;
	men[3].func = TYPE_CHANGE_MENU;
	men[3].menuid = 13; // menuid can't be '0'!
//
//
//	ILI9341_Scr_Fill(ILI9341_COLOR_BLACK);
//	create_menu(men[0]);//Main Menu
}

void menu_controller(void)
{
	while (1)
	{
		if (TP_Touchpad_Pressed())
		{
		if(TP_Read_Coordinates(position_array) == TOUCHPAD_DATA_OK)
		{
			xpos = position_array[0];
			ypos = position_array[1];
			lcdprint(10,10, &Font_7x10,ILI9341_COLOR_BLACK,ILI9341_COLOR_WHITE,"xpos = %d ypos = %d",xpos,ypos);
			if (xpos==0 && ypos>60 && ypos<80)
			{
				create_menu(men[3]);
			}
		}
		}
	}
}

