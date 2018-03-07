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
void ILI9341_DrawPicture(uint16_t StartX, uint16_t StartY, uint16_t EndX,
                         uint16_t EndY, uint16_t *pic)
{
        // Maybe will be changed into page/window and DMA mode soon
        uint16_t i, j;
        for (j = StartY; j < EndY; j++)
        {
                for (i = StartX; i < EndX; i++)
                {
                        ILI9341_DrawPixel(i, j, *pic++);
                }
        }
}
// /*
void ILI9341_DrawPictureA(uint16_t StartX, uint16_t StartY, uint16_t EndX, uint16_t EndY, uint16_t *pic)
{
        // Maybe will be changed into page/window and DMA mode soon
        //   uint16_t i,j;
        //   for (j=StartY;j<EndY;j++)
        //   {
        //     for (i=StartX;i<EndX;i++)
        //     {
        //         ILI9341_DrawPixel(i,j,*pic++);
        //     }
        //   }
        // extern int8_t merih_logo[];

        //    LCD_DATA_BUF = (uint16_t) color;

        //         uint32_t pixels_count;

        /* Set cursor position */

        //  uint16_t size = ((EndX-StartX)*(EndY-StartY));
        SPI_LCD_Init(pic, (EndX - StartX) * (EndY - StartY), 0);

        uint16_t tmp_buf[(EndX - StartX) * (EndY - StartY)];

        strncpy(tmp_buf, pic, sizeof(tmp_buf) - 1);

        //  tmp_buf[(EndX-StartX)*(EndY-StartY)-1]='\0';

        ILI9341_SetCursorPosition(StartX, StartY, EndX, EndY);

        /* Set command for GRAM data */
        ILI9341_SendCommand(ILI9341_GRAM);

        /* Send everything */
       ILI9341_CS_RESET;
       ILI9341_WRX_SET;

#warning LCD-SPI Disable/Enable removed
        ////SPI_Cmd(ILI9341_SPI_LCD, DISABLE);
        // SPI_DataSizeConfig(ILI9341_SPI_LCD, SPI_DataSize_16b);
        ILI9341_SPI_LCD->CR2 |= 0x0F00;

        // SPI_Cmd(ILI9341_SPI_LCD, ENABLE);

        /* Calculate pixels count */
        //         pixels_count = (((x1 - x0)+ 1) * ((y1 - y0) + 1));
        DMA_Cmd(ILI9341_DMA_CH, DISABLE);
        //         ILI9341_DMA_CH->CNDTR = pixels_count > 0xFFFF ? 0xFFFF :
        //         pixels_count; //DMA_BufferSize
        ILI9341_DMA_CH->CNDTR = (EndX - StartX) * (EndY - StartY);
        DMA_Cmd(ILI9341_DMA_CH, ENABLE);
        while (!DMA_GetITStatus(DMA1_IT_TC3))
                ;
        while (!SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD, SPI_I2S_FLAG_TXE))
                ;
        while (SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD, SPI_I2S_FLAG_BSY))
                ;

#warning not needed for now. atilla_o
        //         if(pixels_count > 0xFFFF)
        //         {
        //             pixels_count = pixels_count - 0xFFFF;
        //             DMA_Cmd(ILI9341_DMA_CH,DISABLE);
        //             ILI9341_DMA_CH->CNDTR = pixels_count; //DMA_BufferSize
        //             DMA_Cmd(ILI9341_DMA_CH,ENABLE);
        //             while (!DMA_GetITStatus(DMA1_IT_TC3));
        //             while (!SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD
        //             ,SPI_I2S_FLAG_TXE));
        //             while (SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD
        //             ,SPI_I2S_FLAG_BSY));
        //         }

        ILI9341_CS_SET;
        ILI9341_WRX_RESET;

#warning LCD-SPI Disable/Enable removed
        // SPI_Cmd(ILI9341_SPI_LCD, DISABLE);
        // SPI_DataSizeConfig(ILI9341_SPI_LCD, SPI_DataSize_8b);
        ILI9341_SPI_LCD->CR2 &= 0xF0FF;
        ILI9341_SPI_LCD->CR2 |= 0x0700;
        // SPI_Cmd(ILI9341_SPI_LCD, ENABLE);*/
}
void ILI9341_DisplayOn(void)
{
        ILI9341_SendCommand(ILI9341_DISPLAY_ON);
}

void ILI9341_DisplayOff(void)
{
        ILI9341_SendCommand(ILI9341_DISPLAY_OFF);
}

void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint32_t color)
{
        ILI9341_SetCursorPosition(x, y, x, y);
        ILI9341_SendCommand(ILI9341_GRAM);
        ILI9341_SendData(color >> 8);
        ILI9341_SendData(color & 0xFF);
}

void ILI9341_SetCursorPosition(uint16_t x1, uint16_t y1, uint16_t x2,
                               uint16_t y2)
{
        ILI9341_SendCommand(ILI9341_COLUMN_ADDR);
        ILI9341_SendData(x1 >> 8);
        ILI9341_SendData(x1 & 0xFF);
        ILI9341_SendData(x2 >> 8);
        ILI9341_SendData(x2 & 0xFF);

        ILI9341_SendCommand(ILI9341_PAGE_ADDR);
        ILI9341_SendData(y1 >> 8);
        ILI9341_SendData(y1 & 0xFF);
        ILI9341_SendData(y2 >> 8);
        ILI9341_SendData(y2 & 0xFF);
}

void ILI9341_Delay(volatile unsigned int delay)
{
        for (; delay != 0; delay--)
                ;
}

void ILI9341_Rotate(ILI9341_Orientation_t orientation)
{
        ILI9341_SendCommand(ILI9341_MAC);
        if (orientation == ILI9341_Orientation_Portrait_1)
        {
                ILI9341_SendData(0x58);
        }
        else if (orientation == ILI9341_Orientation_Portrait_2)
        {
                ILI9341_SendData(0x88);
        }
        else if (orientation == ILI9341_Orientation_Landscape_1)
        {
                ILI9341_SendData(0x28);
        }
        else if (orientation == ILI9341_Orientation_Landscape_2)
        {
                ILI9341_SendData(0xE8);
        }

        if (orientation == ILI9341_Orientation_Portrait_1 ||
            orientation == ILI9341_Orientation_Portrait_2)
        {
                ILI9341_Opts.width = ILI9341_WIDTH;
                ILI9341_Opts.height = ILI9341_HEIGHT;
                ILI9341_Opts.orientation = ILI9341_Portrait;
        }
        else
        {
                ILI9341_Opts.width = ILI9341_HEIGHT;
                ILI9341_Opts.height = ILI9341_WIDTH;
                ILI9341_Opts.orientation = ILI9341_Landscape;
        }
}

void ILI9341_Puts(uint16_t x, uint16_t y, char *str, FontDef_t *font,
                  uint32_t foreground, uint32_t background)
{
        uint16_t startX = x;

        /* Set X and Y coordinates */
        ILI9341_x = x;
        ILI9341_y = y;

        while (*str)
        {
                /* New line */
                if (*str == '\n')
                {
                        ILI9341_y += font->FontHeight + 1;
                        /* if after \n is also \r, than go to the left of the screen */
                        if (*(str + 1) == '\r')
                        {
                                ILI9341_x = 0;
                                str++;
                        }
                        else
                        {
                                ILI9341_x = startX;
                        }
                        str++;
                        continue;
                }
                else if (*str == '\r')
                {
                        str++;
                        continue;
                }

                /* Put character to LCD */
                ILI9341_Putc(ILI9341_x, ILI9341_y, *str++, font, foreground, background);
        }
}

void ILI9341_GetStringSize(char *str, FontDef_t *font, uint16_t *width,
                           uint16_t *height)
{
        uint16_t w = 0;
        *height = font->FontHeight;
        while (*str++)
        {
                w += font->FontWidth;
        }
        *width = w;
}

void ILI9341_Putc(uint16_t x, uint16_t y, char c, FontDef_t *font,
                  uint32_t foreground, uint32_t background)
{
        uint32_t i, b, j;
        /* Set coordinates */
        ILI9341_x = x;
        ILI9341_y = y;

        if ((ILI9341_x + font->FontWidth) > ILI9341_Opts.width)
        {
                /* If at the end of a line of display, go to new line and set x to 0
                 * position */
                ILI9341_y += font->FontHeight;
                ILI9341_x = 0;
        }

/* Draw rectangle for background */
// Background colour is disabled by mehmetb (seems not good to me, may be
// temporary)
#warning yazi yazdirmada background icin kullaniliyor. Kapatilmali
        //         ILI9341_DrawFilledRectangle(ILI9341_x, ILI9341_y, ILI9341_x +
        //         font->FontWidth, ILI9341_y + font->FontHeight, background);

        /* Draw font data */
        for (i = 0; i < font->FontHeight; i++)
        {
                b = font->data[(c - 32) * font->FontHeight + i];
                for (j = 0; j < font->FontWidth; j++)
                {
                        if ((b << j) & 0x8000)
                        {
                                ILI9341_DrawPixel(ILI9341_x + j, (ILI9341_y + i), foreground);
                        }
                }
        }

        /* Set new pointer */
        ILI9341_x += font->FontWidth;
}
void ILI9341_Printf_DMA(uint16_t x_origin, uint16_t y_origin, uint16_t fgcolour,
                        uint16_t bgcolour, uint16_t *font, char *textptr)
{

        /*
         * Known bugs before:
         * if calculations of the new font doesn't right, screen will go full white
         *
         */

        //  serialprint("Printf DMA\r\n");

        uint8_t text_length = strlen(textptr);

        uint16_t info_offset;
        uint16_t chr_width, chr_width2;
        uint16_t chr_bytes;
        uint16_t chr_hight;
        uint16_t chr_offset;

        uint16_t cur_char = 0, xwidth = 0;

        uint16_t x, y, cntr = 0;

        chr_hight = font[6]; // the hight is always stored in the 6th bit

        uint16_t bitmapchar[450];

        SPI_LCD_Init(bitmapchar, 450, 1);

        for (uint8_t q = 0; q < chr_hight; q++)
        {

                //    GPIOC->BSRR=GPIO_Pin_11;
                //    SPI_LCD_Init(bitmapchar,300,1);
                for (cur_char = 0; cur_char < text_length; cur_char++)
                {

                        info_offset = (((uint16_t)textptr[cur_char] - 32) * 4) +
                                      8; // work out how far into the font array is the info about
                                         // the current chr skipping the fist 8 byts in the table
                        // asciideki ilk 31 karakter önemsenmiyor. Aldığımız karakter mapi 32-127
                        // arasında olduğu için.

                        chr_width = font[info_offset];                                             // the first byte contanes the width
                                                                                                   // information, this is only how wide the
                                                                                                   // chr is without padding
                        chr_offset = ((font[info_offset + 2]) << 8 | (font[info_offset + 1])) + q; // The next two bytes contane the offset in the table to
                        // the bitmap of the chr

                        uint8_t chr_offset_tmp = chr_offset;

                        chr_bytes = chr_width / 8; // work our how many bytes wide the char is

                        if (chr_width % 8)
                        {
                                chr_bytes++;
                        }
                        uint8_t chr_bytes_tmp = chr_bytes;
                        //        if(chr_bytes>1)
                        //          chr_offset=chr_offset+(q/2);
                        //        else
                        //          chr_offset=chr_offset+q;

                        //        serialprint("2 ok\r\n");

                        for (uint8_t j = 0; j < chr_bytes; j++) // loop through horizontal bytes
                        {
                                for (uint8_t k = 0; k < 8; k++) // loop through each bit
                                {

                                        if (q == 0)
                                                xwidth++;

                                        if (BitTest(font[chr_offset], k))
                                        {

                                                bitmapchar[cntr] = fgcolour;
                                        }
                                        else
                                        {

                                                bitmapchar[cntr] = bgcolour;
                                        }

                                        cntr++;
                                }

                                //          chr_offset++;
                        }
                }

                cntr = 0;

                //  Delay(1);

                ILI9341_SetCursorPosition(x_origin, (y_origin + q), x_origin + xwidth - 1,
                                          (y_origin + q));

                /* Set command for GRAM data */
                ILI9341_SendCommand(ILI9341_GRAM);

                Delay(1);
                /* Send everything */
                ILI9341_CS_RESET;
                ILI9341_WRX_SET;

                ILI9341_SPI_LCD->CR2 |= 0x0F00;

                DMA_Cmd(ILI9341_DMA_CH, DISABLE);
                //         ILI9341_DMA_CH->CNDTR = cntr > 0xFFFF ? 0xFFFF : cntr;
                //         //DMA_BufferSize
                ILI9341_DMA_CH->CNDTR = (xwidth - 1);

                DMA_Cmd(ILI9341_DMA_CH, ENABLE);
                while (!DMA_GetITStatus(DMA1_IT_TC3))
                        ;
                //         while (!SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD
                //         ,SPI_I2S_FLAG_TXE));
                while (SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD, SPI_I2S_FLAG_BSY))
                        ;

                ILI9341_CS_SET;
                ILI9341_WRX_RESET;

                ILI9341_SPI_LCD->CR2 &= 0xF0FF;
                ILI9341_SPI_LCD->CR2 |= 0x0700;

                //  GPIOC->BRR=GPIO_Pin_11;

                //  Delay(10);
        }
        //  serialprint(" size %d\r\n",(xwidth-1)*chr_hight);
        //  SPI_LCD_Init(bitmapchar,cntr-chr_hight);
        //
        //  Delay(10);
        //
        //         ILI9341_SetCursorPosition(x_origin, y_origin,
        //         x_origin+xwidth-1,y_origin+chr_hight);
        //
        //
        //         /* Set command for GRAM data */
        //         ILI9341_SendCommand(ILI9341_GRAM);
        //
        //
        //         /* Send everything */
        			ILI9341_CS_RESET;
        			ILI9341_WRX_SET;
        //
        //         ILI9341_SPI_LCD->CR2 |=  0x0F00;
        //
        //         DMA_Cmd(ILI9341_DMA_CH,DISABLE);
        // //         ILI9341_DMA_CH->CNDTR = cntr > 0xFFFF ? 0xFFFF : cntr;
        // //DMA_BufferSize
        //  ILI9341_DMA_CH->CNDTR = (xwidth-1)*chr_hight;
        //
        //         DMA_Cmd(ILI9341_DMA_CH,ENABLE);
        //         while (!DMA_GetITStatus(DMA1_IT_TC3));
        //         while (!SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD ,SPI_I2S_FLAG_TXE));
        //         while (SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD ,SPI_I2S_FLAG_BSY));
        //
        //
        // // #warning not needed for now. atilla_o
        // //         if(cntr > 0xFFFF)
        // //         {
        // //             cntr = cntr - 0xFFFF;
        // //             DMA_Cmd(ILI9341_DMA_CH,DISABLE);
        // //             ILI9341_DMA_CH->CNDTR = cntr; //DMA_BufferSize
        // //             DMA_Cmd(ILI9341_DMA_CH,ENABLE);
        // //             while (!DMA_GetITStatus(DMA1_IT_TC3));
        // //             while (!SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD
        // ,SPI_I2S_FLAG_TXE));
        // //             while (SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD
        // ,SPI_I2S_FLAG_BSY));
        // //         }
        //
        			ILI9341_CS_SET;
          ILI9341_WRX_RESET;
        //
        //
        //         ILI9341_SPI_LCD->CR2 &=  0xF0FF;
        //         ILI9341_SPI_LCD->CR2 |=  0x0700;
}

void im_resize_print(int8_t *pic, uint16_t pic_x_size, uint8_t pic_y_size,
                     uint8_t scale, uint8_t x_origin, uint8_t y_origin)
{
        uint8_t tmp_buf[pic_x_size * scale];
        uint16_t cntr = 0;

        for (uint16_t i = 0; i < pic_y_size; i++)
        {
                cntr = 0;
                for (uint8_t j = 0; j < pic_x_size; j++)
                {

                        for (uint8_t k = 0; k < scale; k++)
                        {
                                tmp_buf[cntr] = pic[j];

                                cntr++;
                        }
                }

                ILI9341_DrawPictureA(x_origin, y_origin + i,
                                     x_origin + (pic_x_size * scale), y_origin + i,
                                     tmp_buf);

                //  SPI_LCD_Init(tmp_buf,(pic_x_size*scale));
                //
                //         ILI9341_SetCursorPosition(x_origin, y_origin+i,
                //         x_origin+(pic_x_size*scale), y_origin+i);
                //
                //         /* Set command for GRAM data */
                //         ILI9341_SendCommand(ILI9341_GRAM);
                //
                //         /* Send everything */
                			ILI9341_CS_RESET;
                			ILI9341_WRX_SET;
                //
                //         ILI9341_SPI_LCD->CR2 |=  0x0F00;
                //
                //         DMA_Cmd(ILI9341_DMA_CH,DISABLE);
                // //         ILI9341_DMA_CH->CNDTR = pixels_count > 0xFFFF ? 0xFFFF :
                // pixels_count; //DMA_BufferSize
                //  ILI9341_DMA_CH->CNDTR = (pic_x_size*scale);
                //         DMA_Cmd(ILI9341_DMA_CH,ENABLE);
                //         while (!DMA_GetITStatus(DMA1_IT_TC3));
                //         while (!SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD
                //         ,SPI_I2S_FLAG_TXE));
                //         while (SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD ,SPI_I2S_FLAG_BSY));
        }
}

void glcd_text(uint16_t x_origin, uint16_t y_origin, char *textptr, uint16_t fgcolour, uint16_t bgcolour, uint16_t *font)
{
        /*
         * original code can be found at this URL:
         * https://www.ccsinfo.com/forum/viewtopic.php?p=174988
         *
         * CCS code altered for C
         */

        uint8_t text_length = strlen(textptr);

        uint16_t info_offset;
        uint16_t chr_width, total_width = 0;
        uint16_t chr_bytes;
        uint16_t chr_hight;
        uint16_t chr_offset;

        uint16_t i, j, k;
        uint16_t x, y, cntr = 0;

        uint16_t tmp_var = 0;

        chr_hight = font[6]; // the hight is always stored in the 6th bit
        //     uint16_t bitmapchar[30][30];
        // GPIOC->BSRR=GPIO_Pin_11;
        while (*textptr != '\0')
        {
                /*              if (*textptr == 'Ş' | *textptr == 'Ü' | *textptr == 'İ' | *textptr == 'Ç' | *textptr == 'Ö' | *textptr == 'Ğ')
                {
                        switch(*textptr)
                        {
                        case 'Ş':
                                tmp_var=350;
                                break;
                        case 'Ü':
                                tmp_var=220;
                                break;
                        case 'İ':
                                tmp_var=304;
                                break;
                        case 'Ç':
                                tmp_var=199;
                                break;
                        default:
                                break;

                        }
                        glcd_text_int_char(x_origin,y_origin,tmp_var,fgcolour,bgcolour,font,0);


                }
                else*/ if (textptr == ' ')
                {
                        x_origin = x_origin + 50;
                }
                else
                {
                        info_offset = (((uint16_t)*textptr - 32) * 4) +
                                      8; // work out how far into the font array is the info about
                                         // the current chr skipping the fist 8 byts in the table
                        // asciideki ilk 31 karakter önemsenmiyor. Aldığımız karakter mapi 32-127
                        // arasında olduğu için.

                        chr_width = font[info_offset]; // the first byte contanes the width
                                                       // information, this is only how wide the
                                                       // chr is without padding
                        chr_offset = (font[info_offset + 2]) << 8 |
                                     (font[info_offset + 1]); // The next two bytes contane the
                                                              // offset in the table to the bitmap
                                                              // of the chr

                        chr_bytes = chr_width / 8; // work our how many bytes wide the char is
                        if (chr_width % 8)
                        {
                                chr_bytes++;
                        }

                        x = x_origin; // save the start x position
                        y = y_origin; // save the start y position

                        for (i = 0; i < chr_hight; i++) // loop thought vertical bytes
                        {
                                for (j = 0; j < chr_bytes; j++) // loop thought horizontal bytes
                                {
                                        for (k = 0; k < 8; k++) // loop though each bit
                                        {

                                                if (BitTest(font[chr_offset], k))
                                                {

                                                        ILI9341_DrawPixel(x, y, fgcolour); // bit is set, make the pixel the text colour
                                                        //                 bitmapchar[x][y]=0x1433;
                                                }
                                                else
                                                {
#warning yazı yazdırmada background kapalı

                                                 ILI9341_DrawPixel(x, y, bgcolour);
                                                        //                 bitmapchar[x][y]=0x1433;
                                                        //                                           ILI9341_DrawPixel(x,y,bgcolour);
                                                        //                                           //bit is not set,
                                                        //                                           make it the
                                                        //                                           background colour
                                                }
                                                cntr++;
                                                x++; // move to the next vertical line
                                        }
                                        chr_offset++; // move to the next byte
                                }
                                x = x_origin; // move the x origin to start drawing the next horizontal
                                              // bytes
                                y++;          // move down to the next row
                        }

                        x_origin = x_origin + chr_width + 1; // move to the next chr position
                        total_width = total_width + x_origin;
                }

                //             uint8_t cntr2=0;
                //
                //             for(uint8_t i=0;i<x;i++){
                //        for(uint8_t j=0; j<chr_width;j++){
                //      LCD_DATA_BUF[cntr2]=bitmapchar[j][i];
                //      cntr2++;
                //        }}
                //              for(uint8_t i=0;i<150;i++){
                //
                //         serialprint("%d\r\n",LCD_DATA_BUF[i]);
                //      }

                //  SPI_LCD_Init(LCD_DATA_BUF,cntr2,1);
                //  Delay(10);
                //         ILI9341_SetCursorPosition(x_origin, y_origin, x_origin+chr_width
                //         , y_origin+chr_hight);
                //
                //
                //         /* Set command for GRAM data */
                //         ILI9341_SendCommand(ILI9341_GRAM);
                //
                //
                //
                //         /* Send everything */
                			ILI9341_CS_RESET;
                			ILI9341_WRX_SET;
                //
                //         ILI9341_SPI_LCD->CR2 |=  0x0F00;
                //
                //         DMA_Cmd(ILI9341_DMA_CH,DISABLE);
                // //         ILI9341_DMA_CH->CNDTR = cntr > 0xFFFF ? 0xFFFF : cntr;
                // //DMA_BufferSize
                //  ILI9341_DMA_CH->CNDTR = cntr2;
                //         DMA_Cmd(ILI9341_DMA_CH,ENABLE);
                //         while (!DMA_GetITStatus(DMA1_IT_TC3));
                //         while (!SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD
                //         ,SPI_I2S_FLAG_TXE));
                //         while (SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD ,SPI_I2S_FLAG_BSY));
                //
                // /*
                 #warning not needed for now. atilla_o
                //         if(cntr > 0xFFFF)
                //         {
                //             cntr = cntr - 0xFFFF;
                //             DMA_Cmd(ILI9341_DMA_CH,DISABLE);
                //             ILI9341_DMA_CH->CNDTR = cntr; //DMA_BufferSize
                //             DMA_Cmd(ILI9341_DMA_CH,ENABLE);
                //             while (!DMA_GetITStatus(DMA1_IT_TC3));
                //             while (!SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD ,SPI_
                //        I2S_FLAG_TXE));
                //             while (SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD
                //             ,SPI_I2S_FLAG_BSY));
                //         }*/
                //
                			ILI9341_CS_SET;
                        // ILI9341_WRX_RESET;
                //
                //
                //         ILI9341_SPI_LCD->CR2 &=  0xF0FF;
                //         ILI9341_SPI_LCD->CR2 |=  0x0700;
                textptr++; // move to next char in string
        }
        //       GPIOC->BRR=GPIO_Pin_11;
        //       SPI_LCD_Init(color1,1);
        //       uint8_t tmp_buf[300];

        //       ILI9341_DrawPictureA(0,0,300,chr_width,bitmapchar);

        //       for(uint8_t i=0;i<chr_hight;i++){
        //
        //        for(uint8_t j=0; bitmapchar[i][j]!='\0';j++)
        //          tmp_buf[j]=bitmapchar[j];

        //  SPI_LCD_Init(bitmapchar,x_origin*chr_hight,0);
        //
        //         ILI9341_SetCursorPosition(0, 0, x_origin , chr_hight-1);
        //
        //
        //         /* Set command for GRAM data */
        //         ILI9341_SendCommand(ILI9341_GRAM);

        /* Send everything */
        		ILI9341_CS_RESET;
        		ILI9341_WRX_SET;
        //
        //         ILI9341_SPI_LCD->CR2 |=  0x0F00;
        //
        //         DMA_Cmd(ILI9341_DMA_CH,DISABLE);
        // //         ILI9341_DMA_CH->CNDTR = cntr > 0xFFFF ? 0xFFFF : cntr;
        // //DMA_BufferSize
        //  ILI9341_DMA_CH->CNDTR = x_origin*chr_hight;
        //         DMA_Cmd(ILI9341_DMA_CH,ENABLE);
        //         while (!DMA_GetITStatus(DMA1_IT_TC3));
        //         while (!SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD ,SPI_I2S_FLAG_TXE));
        //         while (SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD ,SPI_I2S_FLAG_BSY));
        //

        // #warning not needed for now. atilla_o
        //         if(cntr > 0xFFFF)
        //         {
        //             cntr = cntr - 0xFFFF;
        //             DMA_Cmd(ILI9341_DMA_CH,DISABLE);
        //             ILI9341_DMA_CH->CNDTR = cntr; //DMA_BufferSize
        //             DMA_Cmd(ILI9341_DMA_CH,ENABLE);
        //             while (!DMA_GetITStatus(DMA1_IT_TC3));
        //             while (!SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD
        //             ,SPI_I2S_FLAG_TXE));
        //             while (SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD
        //             ,SPI_I2S_FLAG_BSY));
        //         }

        		ILI9341_CS_SET;
               ILI9341_WRX_RESET;
        //
        //
        //         ILI9341_SPI_LCD->CR2 &=  0xF0FF;
        //         ILI9341_SPI_LCD->CR2 |=  0x0700;

        //  }
}

void glcd_text_int_char(uint16_t x_origin, uint16_t y_origin, uint16_t textptr, uint16_t fgcolour, uint16_t bgcolour, uint16_t *font, uint8_t DMA)
{
        /*
         * original code can be found at this URL:
         * https://www.ccsinfo.com/forum/viewtopic.php?p=174988
         *
         * CCS code altered for C
         */
        uint16_t info_offset;
        uint16_t chr_width;
        uint16_t chr_bytes;
        uint16_t chr_hight;
        uint16_t chr_offset;

        uint16_t i, j, k;
        uint16_t x, y, a = 1;

        uint16_t dma_buffer[1600];
        uint16_t cntr = 0;
        chr_hight = font[6]; // the hight is always stored in the 6th bit

        //    while(textptr != '\0')
        //       {
        //          if(textptr == ' ')
        //          //found a space, we will just insert a few lines
        //             {
        //                x_origin = x_origin + 5;                              //move
        //                to the next chr position
        //             }
        //          else
        //             {
        info_offset = (((uint16_t)textptr - font[2]) * 4) +
                      8;               // work out how far into the font array is the info about the
                                       // current chr skipping the fist 8 byts in the table
        chr_width = font[info_offset]; // the first byte contanes the width
                                       // information, this is only how wide the chr
                                       // is without padding
        chr_offset = (font[info_offset + 2]) << 8 |
                     (font[info_offset + 1]); // The next two bytes contane the offset
                                              // in the table to the bitmap of the chr

        chr_bytes = chr_width / 8; // work our how many bytes wide the char is
        if (chr_width % 8)
        {
                chr_bytes++;
        }

        x = x_origin; // save the start x position
        y = y_origin; // save the start y position

        for (i = 0; i < chr_hight; i++) // loop thought vertical bytes
        {
                for (j = 0; j < chr_bytes; j++) // loop thought horizontal bytes
                {
                        for (k = 0; k < 8; k++) // loop though each bit
                        {
                                if (BitTest(font[chr_offset], k))
                                {

                                        if (DMA)
                                        {

                                                dma_buffer[cntr] = fgcolour;
                                        }
                                        else
                                                ILI9341_DrawPixel(x, y, fgcolour); // bit is set, make the pixel the text colour
                                }
                                else
                                {
#warning yazı yazdırmada background kapalı
                                        //                                           ILI9341_DrawPixel(x,y,bgcolour);
                                        //                                           //bit is not set, make it
                                        //                                           the background colour
                                        if (DMA)
                                                dma_buffer[cntr] = bgcolour;
                                }
                                x++; // move to the next vertical line
                                cntr++;
                        }
                        chr_offset++; // move to the next byte
                }
                x = x_origin; // move the x origin to start drawing the next horizontal
                              // bytes
                y++;          // move down to the next row
        }

        x_origin = x_origin + chr_width + 1; // move to the next chr position
                                             // 	if(DMA)
                                             // 	ILI9341_DrawPictureA(x_origin,y_origin,x_origin+39,y_origin+39,dma_buffer);

        if (DMA)
        {

                if (a)
                {
                        SPI_LCD_Init(dma_buffer, cntr, 1);
                        a = 0;
                        Delay(10);
                }
                //                  Delay(10);
                ILI9341_SetCursorPosition(x_origin, y_origin, x_origin + chr_width - 1, y_origin + chr_hight);

                /* Set command for GRAM data */
                ILI9341_SendCommand(ILI9341_GRAM);

                /* Send everything */
                ILI9341_CS_RESET;
                ILI9341_WRX_SET;

                ILI9341_SPI_LCD->CR2 |= 0x0F00;

                DMA_Cmd(ILI9341_DMA_CH, DISABLE);
                //         ILI9341_DMA_CH->CNDTR = cntr > 0xFFFF ? 0xFFFF : cntr;
                //DMA_BufferSize
                ILI9341_DMA_CH->CNDTR = cntr;
                DMA_Cmd(ILI9341_DMA_CH, ENABLE);
                while (!DMA_GetITStatus(DMA1_IT_TC3))
                        ;
                while (!SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD, SPI_I2S_FLAG_TXE));
                while (SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD, SPI_I2S_FLAG_BSY));

                /*
                #warning not needed for now. atilla_o
                        if(cntr > 0xFFFF)
                        {
                            cntr = cntr - 0xFFFF;
                            DMA_Cmd(ILI9341_DMA_CH,DISABLE);
                            ILI9341_DMA_CH->CNDTR = cntr; //DMA_BufferSize
                            DMA_Cmd(ILI9341_DMA_CH,ENABLE);
                            while (!DMA_GetITStatus(DMA1_IT_TC3));
                            while (!SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD ,SPI_
                       I2S_FLAG_TXE));
                            while (SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD
                            ,SPI_I2S_FLAG_BSY));
                        }*/

                ILI9341_CS_SET;
               ILI9341_WRX_RESET;

                ILI9341_SPI_LCD->CR2 &= 0xF0FF;
                ILI9341_SPI_LCD->CR2 |= 0x0700;
        }
}

//          textptr++; //move to next char in string
//       }
// }

void ILI9341_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                      uint32_t color)
{
        /* Code by dewoller: https://github.com/dewoller */

        int16_t dx, dy, sx, sy, err, e2;
        uint16_t tmp;

        /* Check for overflow */
        if (x0 >= ILI9341_Opts.width)
        {
                x0 = ILI9341_Opts.width - 1;
        }
        if (x1 >= ILI9341_Opts.width)
        {
                x1 = ILI9341_Opts.width - 1;
        }
        if (y0 >= ILI9341_Opts.height)
        {
                y0 = ILI9341_Opts.height - 1;
        }
        if (y1 >= ILI9341_Opts.height)
        {
                y1 = ILI9341_Opts.height - 1;
        }

        /* Check correction */
        if (x0 > x1)
        {
                tmp = x0;
                x0 = x1;
                x1 = tmp;
        }
        if (y0 > y1)
        {
                tmp = y0;
                y0 = y1;
                y1 = tmp;
        }

        dx = x1 - x0;
        dy = y1 - y0;

        /* Vertical or horizontal line */
        if (dx == 0 || dy == 0)
        {
                ILI9341_DrawFilledRectangle(x0, y0, x1, y1, color);
                return;
        }

        sx = (x0 < x1) ? 1 : -1;
        sy = (y0 < y1) ? 1 : -1;
        err = ((dx > dy) ? dx : -dy) / 2;

        while (1)
        {
                ILI9341_DrawPixel(x0, y0, color);
                if (x0 == x1 && y0 == y1)
                {
                        break;
                }
                e2 = err;
                if (e2 > -dx)
                {
                        err -= dy;
                        x0 += sx;
                }
                if (e2 < dy)
                {
                        err += dx;
                        y0 += sy;
                }
        }
}

void ILI9341_DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
                           uint32_t color)
{
        ILI9341_DrawLine(x0, y0, x1, y0, color);
        ILI9341_DrawLine(x0, y0, x0, y1, color);
        ILI9341_DrawLine(x1, y0, x1, y1, color);
        ILI9341_DrawLine(x0, y1, x1, y1, color);
}

void ILI9341_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint32_t color)
{
        int16_t f = 1 - r;
        int16_t ddF_x = 1;
        int16_t ddF_y = -2 * r;
        int16_t x = 0;
        int16_t y = r;

        ILI9341_DrawPixel(x0, y0 + r, color);
        ILI9341_DrawPixel(x0, y0 - r, color);
        ILI9341_DrawPixel(x0 + r, y0, color);
        ILI9341_DrawPixel(x0 - r, y0, color);

        while (x < y)
        {
                if (f >= 0)
                {
                        y--;
                        ddF_y += 2;
                        f += ddF_y;
                }
                x++;
                ddF_x += 2;
                f += ddF_x;

                ILI9341_DrawPixel(x0 + x, y0 + y, color);
                ILI9341_DrawPixel(x0 - x, y0 + y, color);
                ILI9341_DrawPixel(x0 + x, y0 - y, color);
                ILI9341_DrawPixel(x0 - x, y0 - y, color);

                ILI9341_DrawPixel(x0 + y, y0 + x, color);
                ILI9341_DrawPixel(x0 - y, y0 + x, color);
                ILI9341_DrawPixel(x0 + y, y0 - x, color);
                ILI9341_DrawPixel(x0 - y, y0 - x, color);
        }
}

void ILI9341_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r,
                              uint32_t color)
{
        int16_t f = 1 - r;
        int16_t ddF_x = 1;
        int16_t ddF_y = -2 * r;
        int16_t x = 0;
        int16_t y = r;

        ILI9341_DrawPixel(x0, y0 + r, color);
        ILI9341_DrawPixel(x0, y0 - r, color);
        ILI9341_DrawPixel(x0 + r, y0, color);
        ILI9341_DrawPixel(x0 - r, y0, color);
        ILI9341_DrawLine(x0 - r, y0, x0 + r, y0, color);

        while (x < y)
        {
                if (f >= 0)
                {
                        y--;
                        ddF_y += 2;
                        f += ddF_y;
                }
                x++;
                ddF_x += 2;
                f += ddF_x;

                ILI9341_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, color);
                ILI9341_DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, color);

                ILI9341_DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, color);
                ILI9341_DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, color);
        }
}

void ILI9341_Scr_Fill(uint16_t color)
{
        ILI9341_DrawFilledRectangle(0, 0, ILI9341_Opts.width - 1,
                                    ILI9341_Opts.height - 1, color);
}

void ILI9341_DrawFilledRectangle(uint16_t x0, uint16_t y0, uint16_t x1,
                                 uint16_t y1, uint16_t color)
{
        // uint8_t n, i, j;
        // i = color >> 8;
        // j = color & 0xFF;
        uint16_t color1[1];

        color1[0] = color;

        //  if(y1==y0)

        SPI_LCD_Init(color1, 1, 0);

        //  Delay(10);
        //         LCD_DATA_BUF[0]=(uint16_t) color;
        //  LCD_DATA_BUF[1]=ILI9341_COLOR_RED;
        //  LCD_DATA_BUF[2]=ILI9341_COLOR_RED;
        //  LCD_DATA_BUF[3]=ILI9341_COLOR_BLUE;
        //  LCD_DATA_BUF[4]=ILI9341_COLOR_BLUE;
        //  LCD_DATA_BUF[5]=ILI9341_COLOR_BLUE;
        //  LCD_DATA_BUF[6]=ILI9341_COLOR_GREEN;
        //  LCD_DATA_BUF[7]=ILI9341_COLOR_GREEN;
        //  LCD_DATA_BUF[8]=ILI9341_COLOR_GREEN;
        //  LCD_DATA_BUF[9]=ILI9341_COLOR_RED;
        //  LCD_DATA_BUF[10]=ILI9341_COLOR_RED;
        //  LCD_DATA_BUF[11]=ILI9341_COLOR_RED;
        //  LCD_DATA_BUF[12]=ILI9341_COLOR_BLUE;
        //  LCD_DATA_BUF[13]=ILI9341_COLOR_BLUE;
        //  LCD_DATA_BUF[14]=ILI9341_COLOR_BLUE;
        //  LCD_DATA_BUF[15]=ILI9341_COLOR_GREEN;
        //  LCD_DATA_BUF[16]=ILI9341_COLOR_GREEN;
        //  LCD_DATA_BUF[17]=ILI9341_COLOR_GREEN;

        uint32_t pixels_count;

        /* Set cursor position */

        ILI9341_SetCursorPosition(x0, y0, x1, y1);

        /* Set command for GRAM data */
        ILI9341_SendCommand(ILI9341_GRAM);

        /* Send everything */
        ILI9341_CS_RESET;
        ILI9341_WRX_SET;

#warning LCD-SPI Disable/Enable removed
        ////SPI_Cmd(ILI9341_SPI_LCD, DISABLE);
        // SPI_DataSizeConfig(ILI9341_SPI_LCD, SPI_DataSize_16b);
        ILI9341_SPI_LCD->CR2 |= 0x0F00;

        // SPI_Cmd(ILI9341_SPI_LCD, ENABLE);

        /* Calculate pixels count */
        pixels_count = (((x1 - x0) + 1) * ((y1 - y0) + 1));

        DMA_Cmd(ILI9341_DMA_CH, DISABLE);
        ILI9341_DMA_CH->CNDTR =
            pixels_count > 0xFFFF ? 0xFFFF : pixels_count; // DMA_BufferSize
        //         ILI9341_DMA_CH->CNDTR = 18; //DMA_BufferSize
        DMA_Cmd(ILI9341_DMA_CH, ENABLE);
        while (!DMA_GetITStatus(DMA1_IT_TC3))
                ;
        while (!SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD, SPI_I2S_FLAG_TXE))
                ;
        while (SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD, SPI_I2S_FLAG_BSY))
                ;

        if (pixels_count > 0xFFFF)
        {
                pixels_count = pixels_count - 0xFFFF;
                DMA_Cmd(ILI9341_DMA_CH, DISABLE);
                ILI9341_DMA_CH->CNDTR = pixels_count; // DMA_BufferSize
                DMA_Cmd(ILI9341_DMA_CH, ENABLE);
                while (!DMA_GetITStatus(DMA1_IT_TC3))
                        ;
                while (!SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD, SPI_I2S_FLAG_TXE))
                        ;
                while (SPI_I2S_GetFlagStatus(ILI9341_SPI_LCD, SPI_I2S_FLAG_BSY))
                        ;
        }

        	ILI9341_CS_SET;
        //ILI9341_WRX_RESET;

#warning LCD-SPI Disable/Enable removed
        // SPI_Cmd(ILI9341_SPI_LCD, DISABLE);
        // SPI_DataSizeConfig(ILI9341_SPI_LCD, SPI_DataSize_8b);
        ILI9341_SPI_LCD->CR2 &= 0xF0FF;
        ILI9341_SPI_LCD->CR2 |= 0x0700;
        // SPI_Cmd(ILI9341_SPI_LCD, ENABLE);
}

void UG_DrawLine(int16_t x1, int16_t y1, int16_t x2, int16_t y2, int16_t c)
{
        int16_t n, dx, dy, sgndx, sgndy, dxabs, dyabs, x, y, drawx, drawy;

        /* Is hardware acceleration available? */

        dx = x2 - x1;
        dy = y2 - y1;
        dxabs = (dx > 0) ? dx : -dx;
        dyabs = (dy > 0) ? dy : -dy;
        sgndx = (dx > 0) ? 1 : -1;
        sgndy = (dy > 0) ? 1 : -1;
        x = dyabs >> 1;
        y = dxabs >> 1;
        drawx = x1;
        drawy = y1;

        ILI9341_DrawPixel(drawx, drawy, c);

        if (dxabs >= dyabs)
        {
                for (n = 0; n < dxabs; n++)
                {
                        y += dyabs;
                        if (y >= dxabs)
                        {
                                y -= dxabs;
                                drawy += sgndy;
                        }
                        drawx += sgndx;
                        ILI9341_DrawPixel(drawx, drawy, c);
                }
        }
        else
        {
                for (n = 0; n < dyabs; n++)
                {
                        x += dxabs;
                        if (x >= dyabs)
                        {
                                x -= dyabs;
                                drawx += sgndx;
                        }
                        drawy += sgndy;
                        ILI9341_DrawPixel(drawx, drawy, c);
                }
        }
}

void lcdprint(uint16_t x, uint16_t y, FontDef_t *font, uint32_t foreground, uint32_t background, const char *format, ...)
{
        char buf[40];
        va_list vl; // Variable Argument List

        va_start(vl, format);
        vsprintf(buf, format, vl);
        if (font == &Font_7x10)
                glcd_text(x, y, buf, foreground, background, Liberation_Sans19x19);
        else if (font == &Font_11x18)
                glcd_text(x, y, buf, foreground, background, Liberation_Sans19x19);
        else if (font == &Font_16x26)
                glcd_text(x, y, buf, foreground, background, Liberation_Sans19x19);
#warning Fikri: Buyuk yazi fontu flash alanina sigilmasi icin orta yapildi. Buyuk font comment edildi.

        va_end(vl);
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
