/*
 * IL9341_MENU.c
 *
 *  Created on: Feb 22, 2018
 *      Author: CarlosSalazar
 */
#include "ILI9341.h"
#include "ILI9341_MENU.h"
#include "ILI9341_GUI_Conf_desird.h"
#include "ILI9341_GUI.h"
#include "common.h"
#include "math.h"
#include "ILI9341_Touchscreen.h"
#include "ILI9341_fonts.h"
//int xpos;
//int ypos;

//uint16_t x0;
//uint8_t y0;
//uint16_t xWidth;
//uint8_t y1;
//FontDef_t *format;
//uint8_t status;
//uint8_t radius;
//uint16_t foreground_color;
//uint16_t background_color_1;
//uint16_t background_color_2;
//alignment align;
//char *text;
void createButton(buttonStruct button){
	GradientBox gradibox;
	gradibox.x0 = button.buttonX0;
	gradibox.y0 = button.buttonY0;
	gradibox.xWidth = button.buttonX1;
	gradibox.y1 = button.buttonY1;
	gradibox.radius = button.buttonRad;
	gradibox.status = button.buttonStatus;
	gradibox.background_color_1 = button.background_color_1;
	gradibox.background_color_2 = button.background_color_2;
	gradibox.foreground_color = button.foreground_color;
	Window_Draw_GradientBox(gradibox);
	lcdprint(175,135,&Font_11x18,button.foreground_color,button.background_color_2,"     MENU");
}

void create_menu(menuStruct menu)
{

//	if (menu.menuid != 11)
//		{
//			ILI9341_DrawFilledRectangle(0,0,320,240,ILI9341_COLOR_BLACK);
//			ILI9341_DrawLine(MENU_X0, MENU_Y0, MENU_X0 + MENU_XWIDTH, MENU_Y0, GUI_BOX_COLOR12);
//			ILI9341_DrawLine(MENU_X0 + MENU_XWIDTH, MENU_Y0, MENU_X0 + MENU_XWIDTH, MENU_Y0 + MENU_YWIDTH, GUI_BOX_COLOR12);
//			ILI9341_DrawLine(MENU_X0, MENU_Y0, MENU_X0, MENU_Y0 + MENU_YWIDTH, GUI_BOX_COLOR12);
//			ILI9341_DrawLine(MENU_X0, MENU_Y0 + MENU_YWIDTH, MENU_X0 + MENU_XWIDTH, MENU_Y0 + MENU_YWIDTH, GUI_BOX_COLOR12);
//		}
	switch (menu.menuType)
	{
	case TYPE_SETTING_MENU:
	{
		ILI9341_DrawFilledRectangle(0,0,320,240,ILI9341_COLOR_BLACK);
		lcdprint(130,10, &Font_16x26,ILI9341_COLOR_WHITE,ILI9341_COLOR_BLACK, "MAIN  MENU");
		lcdprint(120,80, &Font_16x26,ILI9341_COLOR_WHITE,ILI9341_COLOR_BLACK, "SET TEMPERATURE");
		lcdprint(120,140,&Font_16x26,ILI9341_COLOR_WHITE,ILI9341_COLOR_BLACK, "EDIT PROFILE");
	}
	break;
	case TYPE_TEMPERATURE_MENU:
		{
			ILI9341_DrawFilledRectangle(0,0,320,240,ILI9341_COLOR_BLACK);
			ILI9341_DrawFilledCircle(58,55, 45, ILI9341_COLOR_GREEN);
			lcdprint(58,55, &Font_7x10,ILI9341_COLOR_BLACK,ILI9341_COLOR_GREEN, "+");
			ILI9341_DrawFilledCircle(60, 180, 45, ILI9341_COLOR_RED);
			lcdprint(58,175, &Font_7x10,ILI9341_COLOR_BLACK,ILI9341_COLOR_RED,  "-");
			break;
		}
	case TYPE_PROFILE_MENU:
	{
		ILI9341_DrawFilledRectangle(0,0,320,240,ILI9341_COLOR_BLACK);
		lcdprint(110,40, &Font_16x26,ILI9341_COLOR_WHITE,ILI9341_COLOR_BLACK, "RAMP UP [C/s]");// Ideally 2.5 [Cº/s]
		lcdprint(110,80, &Font_16x26,ILI9341_COLOR_WHITE,ILI9341_COLOR_BLACK, "SOAK TEMPERATURE [C]");
		lcdprint(110,120, &Font_16x26,ILI9341_COLOR_WHITE,ILI9341_COLOR_BLACK,"SOAK TIME [s]");
		lcdprint(110,160, &Font_16x26,ILI9341_COLOR_WHITE,ILI9341_COLOR_BLACK,"PEAK TEMPERATURE [C]"); // Peak Temperature should be whiting this range (210-225)Cº
		lcdprint(110,200, &Font_16x26,ILI9341_COLOR_WHITE,ILI9341_COLOR_BLACK,"PEAK TIME [s]");//should not exceed more than 30sec.
		break;
	}
}
}

