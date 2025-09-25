//i2c.h

#include "types.h"

void init_i2c(void);

void i2c_start(void);

void i2c_restart(void);

void i2c_stop(void);

void i2c_write(u8);

u8 i2c_nack(void);

u8 i2c_mack(void);

//i2c_defines.h

//define pin functions   

#define FUNC2 1 

#define SCL_PIN_0_2   FUNC2 

#define SDA_PIN_0_3   FUNC2 


//defines for I2C_SPEED Configuration  

#define FOSCC 12000000 

#define CCLKK (5*FOSCC) 

#define PCLKK (CCLKK/4)    //Hz 

#define I2C_SPEED 100000    //Hz 

#define BITRATE  ((PCLKK/I2C_SPEED)/2) 


//bit defines for I2CONSET sfr 

#define   AA_BIT 2 

#define   SI_BIT 3 

#define  STO_BIT 4 

#define  STA_BIT 5 

#define I2EN_BIT 6 


//bit defines for I2CONCLR sfr 

#define   AAC_BIT 2 

#define   SIC_BIT 3 

#define  STAC_BIT 5 

#define I2ENC_BIT 6 


#define I2C_EEPROM_SA 0x50 //7Bit Slave Addr 


//i2c_eeprom.h

#include "types.h"

void i2c_eeprom_bytewrite(u8,u8,u8);

u8   i2c_eeprom_randomread(u8,u8);

void i2c_eeprom_pagewrite(u8,u8,u8 *,u8);

void i2c_eeprom_sequentialread(u8,u8,u8 *,u8);
