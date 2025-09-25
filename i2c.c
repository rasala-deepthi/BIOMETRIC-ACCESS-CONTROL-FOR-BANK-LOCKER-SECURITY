//i2c.c

#include "types.h"

#include <LPC21xx.h>
#include "delay.h"

#include "i2c.h"


void init_i2c(void)

{

	//Cfg p0.2 for SCL functions 
	PINSEL0|=0x00000140;


  	I2SCLL=BITRATE; 

  	I2SCLH=BITRATE; 

  //I2C Peripheral Enable for Communication 

  	I2CONSET=1<<I2EN_BIT;  

}


void i2c_start(void)

{

  //initiate start event

  I2CONSET=1<<STA_BIT;

  //wait for start event completion/status	

	while(((I2CONSET>>SI_BIT)&1)==0);

	//clear start event bit

	I2CONCLR=1<<STAC_BIT;

}


void i2c_restart(void)

{

	//initiate restart event

  I2CONSET=1<<STA_BIT;

	//clr prev SIC_BIT to trigger restart 

  I2CONCLR=1<<SIC_BIT; 

  //wait for start event completion/status	

	while(((I2CONSET>>SI_BIT)&1)==0);

	//clear restart event bit

	I2CONCLR=1<<STAC_BIT;

}


void i2c_stop(void)

{

  //initiate stop event

  I2CONSET=1<<STO_BIT;

  //clear prev SIC_BIT to trigger stop

  I2CONCLR=1<<SIC_BIT;	

}


void i2c_write(u8 sDat)

{

	//write to tx-buffer

	I2DAT=sDat;

	//clear prev SIC_BIT to trigger

	//serialization out msb to lsb order

	I2CONCLR=1<<SIC_BIT;

	//wait until serialization completion

	while(((I2CONSET>>SI_BIT)&1)==0);

	//& assuming positive ack from slave

}


u8 i2c_nack(void)

{

	//clear sic_bit to trigger

	//serialization in msb to lsb order

	I2CONCLR=1<<SIC_BIT; 

  //wait until serialization completion

  while(((I2CONSET>>SI_BIT)&1)==0); 

	//& issue nack to slave

	

	//read recvd byte

  return I2DAT; 

}


u8 i2c_mack(void)

{

	//set to issue mack to slave

	I2CONSET=1<<AA_BIT;

	//clear sic_bit to trigger

	//serialization in msb to lsb order

	I2CONCLR=1<<SIC_BIT; 

  //wait until serialization completion

  while(((I2CONSET>>SI_BIT)&1)==0); 

	//& issue mack to slave

	//& clear AA_BIT

	I2CONCLR=1<<AAC_BIT;

	

	//read recvd byte

  return I2DAT; 


}
void i2c_eeprom_bytewrite(u8 slaveAddr,

	                        u8 wBuffAddr,

                          u8 wByte)

{

	i2c_start();

	i2c_write(slaveAddr<<1);//SA+W

	i2c_write(wBuffAddr);

	i2c_write(wByte);

	i2c_stop();

	delay_ms(10);

}


u8   i2c_eeprom_randomread(u8 slaveAddr,

                           u8 rBuffAddr)

{

	u8 rByte;

	i2c_start();

	i2c_write(slaveAddr<<1);//SA+W

	i2c_write(rBuffAddr);

	i2c_restart();

	i2c_write((slaveAddr<<1)|1);//SA+R

	rByte=i2c_nack();

	i2c_stop();

	return rByte;

}




void i2c_eeprom_pagewrite(u8 slaveAddr,

	                        u8 wBuffStartAddr,

                          u8 *pBytes,

                          u8 nBytes)

{

	u32 i;

	i2c_start();

	i2c_write(slaveAddr<<1);//SA+W

	i2c_write(wBuffStartAddr);

	for(i=0;i<nBytes;i++)

	{

	  i2c_write(pBytes[i]);

	}

  i2c_stop();

  delay_ms(10);	

}

void i2c_eeprom_sequentialread(u8 slaveAddr,

	                             u8 rBuffStartAddr,

                               u8 *pBytes,

                               u8 nBytes)

{

	u32 i;

	i2c_start();

	i2c_write(slaveAddr<<1);//SA+W

	i2c_write(rBuffStartAddr);

	i2c_restart();

	i2c_write((slaveAddr<<1)|1);//SA+R

	for(i=0;i<(nBytes-1);i++)

	{

	 pBytes[i]=i2c_mack();

	}

	pBytes[i]=i2c_nack();

	i2c_stop();

}
