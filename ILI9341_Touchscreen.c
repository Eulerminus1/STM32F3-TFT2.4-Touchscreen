/*
 * ILI9341_Touchscreen.c
 *
 *  Created on: Feb 9, 2018
 *      Author: CarlosSalazar
 */
#include "ILI9341_Touchscreen.h"
#include "stdio.h"
#include "stm32f30x_conf.h"
uint16_t xpos;
uint16_t ypos;

//Internal Touchpad command, do not call directly
uint16_t TP_Read(void)
{
    uint8_t i = 16;
    uint16_t value = 0;

    while(i > 0x00)
    {
        value <<= 1;
        GPIO_WriteBit(GPIOB,GPIO_Pin_9, SET);
        GPIO_WriteBit(GPIOB,GPIO_Pin_9, RESET);

        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)!= 0)
        {
            value++;
        }

        i--;
    };

    return value;
}

//Internal Touchpad command, do not call directly
void TP_Write(uint8_t value)
{
    uint8_t i = 0x08;

    	GPIO_WriteBit(GPIOB,GPIO_Pin_9, RESET);

    while(i > 0)
    {
        if((value & 0x80) != 0x00)
        {
        	GPIO_WriteBit(GPIOB,GPIO_Pin_8, SET);
        }
        else
        {
        	GPIO_WriteBit(GPIOB,GPIO_Pin_8, RESET);
        }

        value <<= 1;
    		GPIO_WriteBit(GPIOB,GPIO_Pin_9, SET);
        	GPIO_WriteBit(GPIOB,GPIO_Pin_9, RESET);
        i--;
    };
}



//Read coordinates of touchscreen press. Position[0] = X, Position[1] = Y
uint8_t TP_Read_Coordinates(uint16_t Coordinates[2])
{
		GPIO_WriteBit(GPIOB,GPIO_Pin_9, SET);
		GPIO_WriteBit(GPIOB,GPIO_Pin_8, SET);
		GPIO_WriteBit(ILI9341_TP_CS_PORT,ILI9341_TP_CS_PIN, SET);



		uint32_t avg_x, avg_y = 0;
		uint16_t rawx, rawy = 0;
		uint32_t calculating_x, calculating_y = 0;

    uint32_t samples = NO_OF_POSITION_SAMPLES;
    uint32_t counted_samples = 0;

    	GPIO_WriteBit(ILI9341_TP_CS_PORT,ILI9341_TP_CS_PIN , RESET);
    while((samples > 0) && GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == 0)
    {
        TP_Write(CMD_RDY);

				rawy = TP_Read();
				avg_y += rawy;
				calculating_y += rawy;


        TP_Write(CMD_RDX);
        rawx = TP_Read();
				avg_x += rawx;
				calculating_x += rawx;
        samples--;
				counted_samples++;
    };

    		GPIO_WriteBit(ILI9341_TP_CS_PORT,ILI9341_TP_CS_PIN, SET);

		if((counted_samples == NO_OF_POSITION_SAMPLES)&&GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8) == 0)
		{

		calculating_x /= counted_samples;
		calculating_y /= counted_samples;

		rawx = calculating_x;
		rawy = calculating_y;

		rawx *= -1;
		rawy *= -1;

		//CONVERTING 16bit Value to Screen coordinates
    // 65535/273 = 240!
		// 65535/204 = 320!
    Coordinates[0] =((320- (rawx/X_TRANSLATION)) - X_OFFSET)*X_MAGNITUDE;
		Coordinates[1] =((240-(rawy/Y_TRANSLATION)) - Y_OFFSET)*Y_MAGNITUDE;
		return TOUCHPAD_DATA_OK;
				}
				else
				{
					Coordinates[0] = 0;
					Coordinates[1] = 0;

					return TOUCHPAD_DATA_NOISY;
				}
		}


//Check if Touchpad was pressed. Returns TOUCHPAD_PRESSED (1) or TOUCHPAD_NOT_PRESSED (0)
uint8_t TP_Touchpad_Pressed(void)
{
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)==0)
	{
		return TOUCHPAD_PRESSED;
	}
	else
	{
		return TOUCHPAD_NOT_PRESSED;
	}
}
