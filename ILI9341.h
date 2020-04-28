#ifndef ILI9341_H
#define ILI9341_H

#include "stm32f30x.h"
#include "stm32f30x_rcc.h"
#include "ILI9341_fonts.h"
#include "stm32f30x_conf.h"
#include "stm32f30x_gpio.h"

#define BitSet(A, B) (A |= (1 << (B)))
#define BitReset(A, B) (A &= ~(1 << (B)))
#define BitTest(A, B) ((A) & (1 << (B)))

//CLOCK & PINS
#define ILI9341_SPI_LCD                         SPI1
#define ILI9341_SPI_CLK                         RCC_APB2Periph_SPI1
#define ILI9341_SPI_AF                          GPIO_AF_5

#define ILI9341_SDI_PORT                        GPIOA
#define ILI9341_SDI_PIN                         GPIO_Pin_7
#define ILI9341_SDO_PORT                        GPIOA
#define ILI9341_SDO_PIN                         GPIO_Pin_6
#define ILI9341_TP_SDO_PORT						GPIOB
#define ILI9341_TP_SDO_PIN						GPIO_Pin_1
#define ILI9341_TP_SDI_PORT						GPIOB
#define ILI9341_TP_SDI_PIN						GPIO_Pin_8
#define ILI9341_SCL_PORT                        GPIOA
#define ILI9341_SCL_PIN                         GPIO_Pin_5
#define ILI9341_TP_SCL_PORT                     GPIOB
#define ILI9341_TP_SCL_PIN 						GPIO_Pin_9
#define ILI9341_WRX_PORT                        GPIOC
#define ILI9341_WRX_PIN                         GPIO_Pin_0
#define ILI9341_BLIGHT_PORT                     GPIOA
#define ILI9341_BLIGHT_PIN                      GPIO_Pin_15
#define ILI9341_CS_PORT							GPIOB
#define ILI9341_CS_PIN							GPIO_Pin_4
#define ILI9341_SPI_LCD_AF                      GPIO_AF_5
#define ILI9341_RST_PORT						GPIOB
#define ILI9341_RST_PIN							GPIO_Pin_3
#define ILI9341_TP_PORT							GPIOA
#define ILI9341_TP_PIN							GPIO_Pin_8
#define ILI9341_TP_CS_PORT						GPIOB
#define ILI9341_TP_CS_PIN						GPIO_Pin_2


#define ILI9341_SPI_LCD_PRESCALER               SPI_BaudRatePrescaler_2
#define ILI9341_SPI_LCD_DATASIZE                SPI_DataSize_8b
#define ILI9341_SPI_LCD_FIRSTBIT                SPI_FirstBit_MSB
#define ILI9341_SPI_LCD_SPI_Mode                SPI_Mode_Master
#define ILI9341_SPI_LCD_SPI_Direction           SPI_Direction_1Line_Tx
#define ILI9341_SPI_LCD_SPI_CPOL                SPI_CPOL_Low
#define ILI9341_SPI_LCD_SPI_CPHA                SPI_CPHA_1Edge
#define ILI9341_SPI_LCD_SPI_SPI_NSS             SPI_NSS_Soft

#define ILI9341_DMA_CLK                         RCC_AHBPeriph_DMA1
#define ILI9341_DMA_CH                          DMA1_Channel3



#define ILI9341_WIDTH                           240
#define ILI9341_HEIGHT                          320
#define ILI9341_PIXEL                           76800

//COLORS
#define ILI9341_COLOR_WHITE                     0xFFFF
#define ILI9341_COLOR_BLACK                     0x0000
#define ILI9341_COLOR_RED                       0xF800
#define ILI9341_COLOR_GREEN                     0x07E0
#define ILI9341_COLOR_GREEN2                    0xB723
#define ILI9341_COLOR_BLUE                      0x001F
#define ILI9341_COLOR_BLUE2                     0x051D
#define ILI9341_COLOR_YELLOW                    0xFFE0
#define ILI9341_COLOR_ORANGE                    0xFBE4
#define ILI9341_COLOR_CYAN                      0x07FF
#define ILI9341_COLOR_MAGENTA                   0xA254
#define ILI9341_COLOR_GRAY                      0x7BEF
#define ILI9341_COLOR_BROWN                     0xBBCA
#define ILI9341_COLOR_NAVY                      0x000F   
#define ILI9341_COLOR_DARKGREEN                 0x03E0   
#define ILI9341_COLOR_DARKCYAN                  0x03EF   
#define ILI9341_COLOR_MAROON                    0x7800
#define ILI9341_COLOR_PURPLE                    0x780F   
#define ILI9341_COLOR_OLIVE                     0x7BE0   
#define ILI9341_COLOR_LIGHTGREY                 0xC618   
#define ILI9341_COLOR_DARKGREY                  0x7BEF    
#define ILI9341_COLOR_GREENYELLOW               0xAFE5   
#define ILI9341_COLOR_PINK                      0xF81F
#define ILI9341_COLOR_TRANSPARENT               0x80000000

#define ILI9341_RST_SET 	 GPIO_SetBits(ILI9341_RST_PORT, ILI9341_RST_PIN)
#define ILI9341_RST_RESET  	 GPIO_ResetBits(ILI9341_RST_PORT, ILI9341_RST_PIN)
#define ILI9341_CS_SET 		 GPIO_SetBits(ILI9341_CS_PORT, ILI9341_CS_PIN)
#define ILI9341_CS_RESET 	 GPIO_ResetBits(ILI9341_CS_PORT, ILI9341_CS_PIN)
#define ILI9341_WRX_SET 	 GPIO_SetBits(ILI9341_WRX_PORT, ILI9341_WRX_PIN)
#define ILI9341_WRX_RESET 	 GPIO_ResetBits(ILI9341_WRX_PORT, ILI9341_WRX_PIN)
#define ILI9341_PEN_SET 	 GPIO_SetBits(ILI9341_PEN_PORT, ILI9341_PEN_PIN)
#define ILI9341_PEN_RESET 	 GPIO_ResetBits(ILI9341_PEN_PORT, ILI9341_PEN_PIN)
#define ILI9341_SDO_SET 	 GPIO_SetBits(ILI9341_SDO_PORT, ILI9341_SDO_PIN)
#define ILI9341_SDO_RESET 	 GPIO_ResetBits(ILI9341_SDO_PORT, ILI9341_SDO_PIN)
#define ILI9341_SDI_SET 	 GPIO_SetBits(ILI9341_SDI_PORT, ILI9341_SDI_PIN)
#define ILI9341_SDI_RESET 	 GPIO_ResetBits(ILI9341_SDI_PORT, ILI9341_SDI_PIN)
#define ILI9341_SCL_SET 	 GPIO_SetBits(ILI9341_SCL_PORT, ILI9341_SCL_PIN)
#define ILI9341_SCL_RESET 	 GPIO_ResetBits(ILI9341_SCL_PORT, ILI9341_SCL_PIN)


typedef enum 
{
        ILI9341_Orientation_Portrait_1, 
        ILI9341_Orientation_Portrait_2, 
        ILI9341_Orientation_Landscape_1,
        ILI9341_Orientation_Landscape_2 
} 
ILI9341_Orientation_t;

void ILI9341_SendData(uint8_t data);
void ILI9341_SendCommand(uint8_t data);


uint16_t LCD_DATA_BUF[900];
// uint16_t LCD_DATA_BUF;
void ILI9341_Init(void);
void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint32_t color);
void ILI9341_Fill(uint32_t color);
void ILI9341_Scr_Fill(uint16_t color);
void ILI9341_Rotate(ILI9341_Orientation_t orientation);
void ILI9341_Putc(uint16_t x, uint16_t y, char c, FontDef_t* font, uint32_t foreground, uint32_t background);
void ILI9341_Puts(uint16_t x, uint16_t y, char* str, FontDef_t *font, uint32_t foreground, uint32_t background);
void ILI9341_GetStringSize(char* str, FontDef_t* font, uint16_t* width, uint16_t* height);
void ILI9341_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color);
void ILI9341_DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t color);
void ILI9341_DrawFilledRectangle(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1, uint16_t color) ;
void ILI9341_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color);
void ILI9341_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color);
void ILI9341_DisplayOn(void);
void ILI9341_DisplayOff(void);
void ILI9341_DrawPicture(uint16_t StartX,uint16_t StartY,uint16_t EndX,uint16_t EndY,uint16_t *pic);
void ILI9341_DrawPictureA(uint16_t StartX,uint16_t StartY,uint16_t EndX,uint16_t EndY,uint16_t *pic);
void ILI9341_Printf_DMA(uint16_t x_origin, uint16_t y_origin, uint16_t fgcolour, uint16_t bgcolour, uint16_t* font, char* textptr);
void im_resize_print(int8_t* pic,uint16_t pic_x_size,uint8_t pic_y_size, uint8_t scale, uint8_t x_origin, uint8_t y_origin);
void glcd_text(uint16_t x_origin, uint16_t y_origin, char* textptr, uint16_t fgcolour, uint16_t bgcolour, uint16_t* font);
void glcd_text_int_char( uint16_t x_origin, uint16_t y_origin, uint16_t textptr, uint16_t fgcolour, uint16_t bgcolour, uint16_t* font, uint8_t DMA ) ;
void lcdprint(uint16_t x, uint16_t y,FontDef_t *font, uint32_t foreground, uint32_t background , const char * format, ...);
//void ILI9341_INT_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void SPI_SendData8(SPI_TypeDef* SPIx, uint8_t Data);
void SPI_LCD_Init(uint16_t *ptr, uint16_t ptr_size, uint8_t a);
void ILI9341_DrawGraph(double x0,double y0,double gx,double gy,double x1,double y1,double xlo,double xhi,double xinc,double ylo,double yhi,double yinc, uint16_t color);
void UG_DrawLine( int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t c );

uint16_t RGB_to_Color(uint8_t Red, uint8_t Green, uint8_t Blue);

 typedef struct 
  {
    uint8_t Red;
    uint8_t Green;
    uint8_t Blue;
  } C_to_RGB;
C_to_RGB Color_to_RGB(uint16_t Color);
 

#endif
