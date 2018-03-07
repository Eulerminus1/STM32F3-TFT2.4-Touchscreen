/*
 * ILI9341_MENU_CONTROLLER.h
 *
 *  Created on: Feb 23, 2018
 *      Author: CarlosSalazar
 */

#ifndef ILI9341_MENU_CONTROLLER_H_
#define ILI9341_MENU_CONTROLLER_H_
#include "stm32f30x.h"
#include "stm32f30x_rcc.h"
#include "stm32f30x_conf.h"
#include "ILI9341.h"
#include "stm32f30x_gpio.h"
#include "ILI9341_GUI_Conf_desird.h"

void menu_init(void);
void menu_controller(void);
void backToPreviousMenu(void);
void enterTheNextMenu(void);



#endif /* ILI9341_MENU_CONTROLLER_H_ */
