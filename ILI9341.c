#include "ILI9341.h"
#include "common.h"
#include "stdarg.h"
#include "stdio.h"
#include "stm32f30x_conf.h"
#include "ILI9341_GUI_Conf_desird.h"

DMA_InitTypeDef DMA_InitStructure;
typedef enum { ILI9341_Landscape,
               ILI9341_Portrait } ILI9341_Orientation;

typedef struct
{
        uint16_t width;
        uint16_t height;
        ILI9341_Orientation orientation;
} ILI931_Options_t;

//#define ILI9341_SD_SET GPIO_SetBits(ILI9341_SD_CS_PORT, ILI9341_SD_CS_PIN)
//#define ILI9341_SD_RESET                GPIO_ResetBits((ILI9341_SD_CS_PORT, ILI9341_SD_CS_PIN)

//#define ILI9341_FONT_SET GPIO_SetBits(ILI9341_FONT_CS_PORT, ILI9341_FONT_CS_PIN)
//#define ILI9341_FONT_RESET \
        GPIO_ResetBits(ILI9341_FONT_CS_PORT, ILI9341_FONT_CS_PIN)

//#define ILI9341_FLASH_SET \
        GPIO_SetBits(ILI9341_FLASH_CS_PORT, ILI9341_FLASH_CS_PIN)
//#define ILI9341_FLASH_RESET \
        GPIO_ResetBits(ILI9341_FLASH_CS_PORT, ILI9341_FLASH_CS_PIN)

#define ILI9341_BLIGHT_SET GPIO_SetBits(ILI9341_BLIGHT_PORT, ILI9341_BLIGHT_PIN)
#define ILI9341_BLIGHT_RESET \
	GPIO_ResetBits(ILI9341_BLIGHT_PORT, ILI9341_BLIGHT_PIN)

#define ILI9341_RESET 0x01
#define ILI9341_SLEEP_OUT 0x11
#define ILI9341_GAMMA 0x26
#define ILI9341_DISPLAY_OFF 0x28
#define ILI9341_DISPLAY_ON 0x29
#define ILI9341_COLUMN_ADDR 0x2A
#define ILI9341_PAGE_ADDR 0x2B
#define ILI9341_GRAM 0x2C
#define ILI9341_MAC 0x36
#define ILI9341_PIXEL_FORMAT 0x3A
#define ILI9341_WDB 0x51
#define ILI9341_WCD 0x53
#define ILI9341_RGB_INTERFACE 0xB0
#define ILI9341_FRC 0xB1
#define ILI9341_BPC 0xB5
#define ILI9341_DFC 0xB6
#define ILI9341_POWER1 0xC0
#define ILI9341_POWER2 0xC1
#define ILI9341_VCOM1 0xC5
#define ILI9341_VCOM2 0xC7
#define ILI9341_POWERA 0xCB
#define ILI9341_POWERB 0xCF
#define ILI9341_PGAMMA 0xE0
#define ILI9341_NGAMMA 0xE1
#define ILI9341_DTCA 0xE8
#define ILI9341_DTCB 0xEA
#define ILI9341_POWER_SEQ 0xED
#define ILI9341_3GAMMA_EN 0xF2
#define ILI9341_INTERFACE 0xF6
#define ILI9341_PRC 0xF7

uint16_t ILI9341_x;
uint16_t ILI9341_y;
ILI931_Options_t ILI9341_Opts;
uint8_t ILI9341_INT_CalledFromPuts = 0;
SPI_InitTypeDef SPI_InitStruct;

// uint16_t LCD_DATA_BUF[900];

void ILI9341_InitLCD(void);

void ILI9341_Delay(volatile unsigned int delay);
void ILI9341_SetCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2,
                               uint16_t y2);
//void scr_Fill(uint16_t color);
uint32_t DMA_Inc_Opt;
void SPI_LCD_Init(uint16_t *ptr, uint16_t ptr_size, uint8_t a)
{

        if (a == 0)
        {
                RCC_APB2PeriphClockCmd(ILI9341_SPI_CLK, ENABLE);

                init_pin(ILI9341_SDI_PORT, ILI9341_SDI_PIN, GPIO_Mode_AF, GPIO_Speed_50MHz,
                         GPIO_OType_PP, GPIO_PuPd_NOPULL);
                init_pin(ILI9341_SDO_PORT, ILI9341_SDO_PIN, GPIO_Mode_AF, GPIO_Speed_50MHz,
                         GPIO_OType_PP, GPIO_PuPd_NOPULL);
                init_pin(ILI9341_SCL_PORT, ILI9341_SCL_PIN, GPIO_Mode_AF, GPIO_Speed_50MHz,
                         GPIO_OType_PP, GPIO_PuPd_NOPULL);
                init_pin(ILI9341_CS_PORT, ILI9341_CS_PIN, GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_UP);
                GPIO_PinAFConfig(ILI9341_SDI_PORT, get_pin_src(ILI9341_SDI_PIN),
                                 ILI9341_SPI_AF);
                GPIO_PinAFConfig(ILI9341_SDO_PORT, get_pin_src(ILI9341_SDO_PIN),
                                 ILI9341_SPI_AF);
                GPIO_PinAFConfig(ILI9341_SCL_PORT, get_pin_src(ILI9341_SCL_PIN),
                                 ILI9341_SPI_AF);

                SPI_I2S_DeInit(ILI9341_SPI_LCD);
                SPI_StructInit(&SPI_InitStruct);
                SPI_InitStruct.SPI_BaudRatePrescaler = ILI9341_SPI_LCD_PRESCALER;
                SPI_InitStruct.SPI_DataSize = ILI9341_SPI_LCD_DATASIZE;
                SPI_InitStruct.SPI_Direction = SPI_Direction_1Line_Tx;
                SPI_InitStruct.SPI_FirstBit = ILI9341_SPI_LCD_FIRSTBIT;
                SPI_InitStruct.SPI_Mode = ILI9341_SPI_LCD_SPI_Mode;
                SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
                SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
                SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
                SPI_Init(ILI9341_SPI_LCD, &SPI_InitStruct);
        }

        /*--Enable DMA1 clock--*/
        RCC_AHBPeriphClockCmd(ILI9341_DMA_CLK, ENABLE);

        DMA_DeInit(ILI9341_DMA_CH); // Set DMA registers to default values
        DMA_StructInit(&DMA_InitStructure);
        DMA_InitStructure.DMA_PeripheralBaseAddr =
            (uint32_t)&ILI9341_SPI_LCD
                ->DR; // Address of peripheral the DMA must map to
        DMA_InitStructure.DMA_MemoryBaseAddr =
            (uint32_t)&ptr[0]; // Variable from which data will be transmitted
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
        DMA_InitStructure.DMA_BufferSize = ptr_size; // Buffer size
        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

        if (ptr_size <= 1)
                DMA_Inc_Opt = DMA_MemoryInc_Disable;
        else
                DMA_Inc_Opt = DMA_MemoryInc_Enable;

        DMA_InitStructure.DMA_MemoryInc = DMA_Inc_Opt;

        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
        DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
        DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
        DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
        DMA_Init(ILI9341_DMA_CH, &DMA_InitStructure); // Initialise the DMA
        DMA_Cmd(ILI9341_DMA_CH, ENABLE);              // Enable the DMA1 - Channel3

        /*Enable SPI1*/
        SPI_Cmd(ILI9341_SPI_LCD, ENABLE);

        /*Enable the SPI2 RX & TX DMA requests*/
        SPI_I2S_DMACmd(ILI9341_SPI_LCD, SPI_I2S_DMAReq_Tx, ENABLE);
}

uint16_t RGB_to_Color(uint8_t Red, uint8_t Green,
                      uint8_t Blue) // 24bit RGB888 into RGB565
{
        uint16_t B = (Blue >> 3) & 0x001F;
        uint16_t G = ((Green >> 2) << 5) & 0x07E0;
        uint16_t R = ((Red >> 3) << 11) & 0xF800;

        return (uint16_t)(R | G | B);
}

C_to_RGB Color_to_RGB(uint16_t Color) // 16bit RGB565 into RGB888
{
        C_to_RGB retval;
        retval.Blue = (Color & 0x001F) << 3;
        retval.Green = ((Color >> 5) & 0x003F) << 2;
        retval.Red = ((Color >> 11) & 0x001F) << 3;

        return (retval);
}

void ILI9341_Init()
{
        init_pin(ILI9341_WRX_PORT, ILI9341_WRX_PIN, GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_UP);

        init_pin(ILI9341_RST_PORT, ILI9341_RST_PIN, GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_UP);

        //init_pin(ILI9341_SD_CS_PORT, ILI9341_SD_CS_PIN, GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_UP);
        //init_pin(ILI9341_FONT_CS_PORT, ILI9341_FONT_CS_PIN, GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_UP);
        //init_pin(ILI9341_FLASH_CS_PORT, ILI9341_FLASH_CS_PIN, GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_UP);
         init_pin(ILI9341_BLIGHT_PORT, ILI9341_BLIGHT_PIN, GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_UP);
         init_pin(ILI9341_TP_PORT, ILI9341_TP_PIN, GPIO_Mode_IN, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL);
         init_pin(ILI9341_TP_CS_PORT, ILI9341_TP_CS_PIN, GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL);
         init_pin(ILI9341_TP_SDO_PORT, ILI9341_TP_SDO_PIN, GPIO_Mode_IN, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_UP);
         init_pin(ILI9341_TP_SDI_PORT, ILI9341_TP_SDI_PIN, GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL);
         init_pin( ILI9341_TP_SCL_PORT,  ILI9341_TP_SCL_PIN, GPIO_Mode_OUT, GPIO_Speed_50MHz, GPIO_OType_PP, GPIO_PuPd_NOPULL);


        ILI9341_CS_SET;
        //ILI9341_SD_SET;
        //ILI9341_FONT_SET;
        //ILI9341_FLASH_SET;
        ILI9341_BLIGHT_SET;
        uint8_t a;

        SPI_LCD_Init(a, 0, 0);
        ILI9341_InitLCD();
        //ILI9341_Scr_Fill(0x00);
        ILI9341_Rotate(ILI9341_Orientation_Landscape_2);
        ILI9341_DrawFilledRectangle(0, 0, 320, 240, GUI_BACKGROUNDCOLOR);
}

void ILI9341_InitLCD(void)
{
		ILI9341_RST_RESET;
        ILI9341_Delay(20000);
        ILI9341_RST_SET;
        ILI9341_Delay(20000);
        ILI9341_SendCommand(ILI9341_RESET);
        ILI9341_Delay(50000);
        ILI9341_SendCommand(ILI9341_POWERA);
        ILI9341_SendData(0x39);
        ILI9341_SendData(0x2C);
        ILI9341_SendData(0x00);
        ILI9341_SendData(0x34);
        ILI9341_SendData(0x02);
        ILI9341_SendCommand(ILI9341_POWERB);
        ILI9341_SendData(0x00);
        ILI9341_SendData(0xC1);
        ILI9341_SendData(0x30);
        ILI9341_SendCommand(ILI9341_DTCA);
        ILI9341_SendData(0x85);
        ILI9341_SendData(0x00);
        ILI9341_SendData(0x78);
        ILI9341_SendCommand(ILI9341_DTCB);
        ILI9341_SendData(0x00);
        ILI9341_SendData(0x00);
        ILI9341_SendCommand(ILI9341_POWER_SEQ);
        ILI9341_SendData(0x64);
        ILI9341_SendData(0x03);
        ILI9341_SendData(0x12);
        ILI9341_SendData(0x81);
        ILI9341_SendCommand(ILI9341_PRC);
        ILI9341_SendData(0x20);
        ILI9341_SendCommand(ILI9341_POWER1);
        ILI9341_SendData(0x23);
        ILI9341_SendCommand(ILI9341_POWER2);
        ILI9341_SendData(0x10);
        ILI9341_SendCommand(ILI9341_VCOM1);
        ILI9341_SendData(0x3E);
        ILI9341_SendData(0x28);
        ILI9341_SendCommand(ILI9341_VCOM2);
        ILI9341_SendData(0x86);
        ILI9341_SendCommand(ILI9341_MAC);
        ILI9341_SendData(0x48);
        ILI9341_SendCommand(ILI9341_PIXEL_FORMAT);
        ILI9341_SendData(0x55);
        ILI9341_SendCommand(ILI9341_FRC);
        ILI9341_SendData(0x00);
        ILI9341_SendData(0x18);
        ILI9341_SendCommand(ILI9341_DFC);
        ILI9341_SendData(0x08);
        ILI9341_SendData(0x82);
        ILI9341_SendData(0x27);
        ILI9341_SendCommand(ILI9341_3GAMMA_EN);
        ILI9341_SendData(0x00);
        ILI9341_SendCommand(ILI9341_COLUMN_ADDR);
        ILI9341_SendData(0x00);
        ILI9341_SendData(0x00);
        ILI9341_SendData(0x00);
        ILI9341_SendData(0xEF);
        ILI9341_SendCommand(ILI9341_PAGE_ADDR);
        ILI9341_SendData(0x00);
        ILI9341_SendData(0x00);
        ILI9341_SendData(0x01);
        ILI9341_SendData(0x3F);
        ILI9341_SendCommand(ILI9341_GAMMA);
        ILI9341_SendData(0x01);
        ILI9341_SendCommand(ILI9341_PGAMMA);
        ILI9341_SendData(0x0F);
        ILI9341_SendData(0x31);
        ILI9341_SendData(0x2B);
        ILI9341_SendData(0x0C);
        ILI9341_SendData(0x0E);
        ILI9341_SendData(0x08);
        ILI9341_SendData(0x4E);
        ILI9341_SendData(0xF1);
        ILI9341_SendData(0x37);
        ILI9341_SendData(0x07);
        ILI9341_SendData(0x10);
        ILI9341_SendData(0x03);
        ILI9341_SendData(0x0E);
        ILI9341_SendData(0x09);
        ILI9341_SendData(0x00);
        ILI9341_SendCommand(ILI9341_NGAMMA);
        ILI9341_SendData(0x00);
        ILI9341_SendData(0x0E);
        ILI9341_SendData(0x14);
        ILI9341_SendData(0x03);
        ILI9341_SendData(0x11);
        ILI9341_SendData(0x07);
        ILI9341_SendData(0x31);
        ILI9341_SendData(0xC1);
        ILI9341_SendData(0x48);
        ILI9341_SendData(0x08);
        ILI9341_SendData(0x0F);
        ILI9341_SendData(0x0C);
        ILI9341_SendData(0x31);
        ILI9341_SendData(0x36);
        ILI9341_SendData(0x0F);
        ILI9341_SendCommand(ILI9341_SLEEP_OUT);
        ILI9341_Delay(1000000);
        ILI9341_SendCommand(ILI9341_DISPLAY_ON);
        ILI9341_SendCommand(ILI9341_GRAM);
}

void ILI9341_SendCommand(uint8_t data)
{
        while (SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD, SPI_I2S_FLAG_BSY) ==
               SET)
                ;
        ILI9341_WRX_RESET;
        ILI9341_CS_RESET;
        SPI_SendData8(ILI9341_SPI_LCD, data);
        while (SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD, SPI_I2S_FLAG_BSY) ==
               SET)
                ;
        ILI9341_CS_SET;
}

void ILI9341_SendData(uint8_t data)
{
        while (SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD, SPI_I2S_FLAG_BSY) ==
               SET)
                ;
        ILI9341_WRX_SET;
        ILI9341_CS_RESET;
        SPI_SendData8(ILI9341_SPI_LCD, data);
        while (SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD, SPI_I2S_FLAG_BSY) ==
               SET)
                ;
        ILI9341_CS_SET;
}

void ILI9341_DrawGraph(double x0,double y0,double gx,double gy,double x1,double y1,double xlo,double xhi,double xinc,double ylo,double yhi,double yinc,uint16_t color)
{

	float ox=(x0-xlo)*(x1)/(xhi-xlo)+gx;
	float oy=(y0-ylo)*(gy-y1-gy)/(yhi-ylo)+gy;
	double x;
	double y;
	double i;
	double change;
	int start1=0;
	//// Drawing the grid (Cartesian Coordinates)///
	if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) && start1==0)
	{
		ox=(x0-xlo)*(x1)/(xhi-xlo)+gx;
		oy=(y0-ylo)*(gy-y1-gy)/(yhi-ylo)+gy;
		//draw yscale
		for (i=ylo;i<=yhi;i+=yinc){
			change=(i-ylo)*(gy-y1-gy)/(yhi-ylo)+gy;
			if (i==0)
			{
				ILI9341_DrawLine(gx,change, gx+x1, change, ILI9341_COLOR_BLACK);
			}
			else{
				ILI9341_DrawLine(gx,change, gx+x1, change, ILI9341_COLOR_BLACK);
			}
		}
		//draw xscale
		for (i=xlo;i<=xhi;i+=xinc){
			change=(i-xlo)*(x1)/(xhi-xlo)+gx;
			if(i==0){
				ILI9341_DrawLine(change,gy, change, gy-y1, ILI9341_COLOR_BLACK);
			}
			else{
				ILI9341_DrawLine(change,gy, change, gy-y1, ILI9341_COLOR_BLACK);
			}
		}
		/// Draw the labels///
		lcdprint(150,5, &Font_7x10,ILI9341_COLOR_BLACK,ILI9341_COLOR_WHITE, "Reflow");
	}
	//Plotting Data
	x=(x0-xlo)*(x1)/(xhi-xlo)+gx;
	y=(y0-ylo)*(gy-y1-gy)/(yhi-ylo)+gy;
	ILI9341_DrawLine(ox, oy, x, y, color);
	ILI9341_DrawLine(ox, oy+1, x, y+1, color);
	ILI9341_DrawLine(ox, oy-1, x, y-1, color);
	ox=x;
	oy=y;

	}
