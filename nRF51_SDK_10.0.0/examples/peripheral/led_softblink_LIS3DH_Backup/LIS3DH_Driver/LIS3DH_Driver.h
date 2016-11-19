

#include "pca10028.h"
#include "stdint.h"

typedef  unsigned char BYTE; 
typedef  unsigned short WORD;
typedef  unsigned int DWORD;


/*  SPI1 GPIO Config*/

#define SPI_PSELSCK1             9 //13// 30//05//29   /*!< GPIO pin number for SPI clock              */
#define SPI_PSELMOSI1            8 //12// 02// 21   /*!< GPIO pin number for Master Out Slave In    */
#define SPI_PSELMISO1            3 //11//01// 23   /*!< GPIO pin number for Master In Slave Out    */
#define SPI_PSELSS1              1 //10// 00//28   /*!< GPIO pin number for Slave Select           */


/*Timer initalization parameters*/   
#define OP_QUEUES_SIZE          3
#define APP_TIMER_PRESCALER     0 
#define SPI_OPERATING_FREQUENCY (0x02000000uL << (uint32_t)Freq_8Mbps/*Freq_1Mbps*/)  /**< Slave clock frequency. */
#define TIMEOUT_COUNTER         0x3000uL                                /**< Timeout for SPI transaction in units of loop iterations. */

typedef enum
{
    Freq_125Kbps = 0,        /*!< drive SClk with frequency 125Kbps */
    Freq_250Kbps,            /*!< drive SClk with frequency 250Kbps */
    Freq_500Kbps,            /*!< drive SClk with frequency 500Kbps */
    Freq_1Mbps,              /*!< drive SClk with frequency 1Mbps */
    Freq_2Mbps,              /*!< drive SClk with frequency 2Mbps */
    Freq_4Mbps,              /*!< drive SClk with frequency 4Mbps */
    Freq_8Mbps               /*!< drive SClk with frequency 8Mbps */
} SPIFrequency_t;

/**
 *  SPI master module number
 */
typedef enum
{
    SPI0 = 0,               /*!< SPI module 0 */
    SPI1                    /*!< SPI module 1 */
} SPIModuleNumber;

/**
 *  SPI mode
 */
typedef enum
{
    //------------------------Clock polarity 0, Clock starts with level 0-------------------------------------------
    SPI_MODE0 = 0,          /*!< Sample data at rising edge of clock and shift serial data at falling edge */
    SPI_MODE1,              /*!< sample data at falling edge of clock and shift serial data at rising edge */
    //------------------------Clock polarity 1, Clock starts with level 1-------------------------------------------
    SPI_MODE2,              /*!< sample data at falling edge of clock and shift serial data at rising edge */
    SPI_MODE3               /*!< Sample data at rising edge of clock and shift serial data at falling edge */
} SPIMode;



void Enable_GSensor(void);
void Disable_GSensor(void);
void Enable_GSensor2(void);
void write_Sen_datas(uint8_t Addr,uint8_t data); 
void Read_Sen_datas(uint8_t Addr,uint8_t *buff,uint8_t Len);


