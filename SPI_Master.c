#include <avr/io.h>
#include "SPI_Master.h"

void spi_slave_port_init(struct slave_device device_name)
{
	SPI_DDR_SS|=(1<<device_name.SPI_SS_PIN);
	SPI_PORT_SS|=(1<<device_name.SPI_SS_PIN);
}

void spi_init(char spi_mode, char prescaler, _Bool LSB_to_MSB)
{
	SPCR|=(1<<SPE)|(1<<SPIE)|(1<<MSTR);
	switch(spi_mode)
	{
		case 0: SPI_MODE_0;
		break;
		case 1: SPI_MODE_1;
		break;
		case 2: SPI_MODE_2;
		break;
		case 3: SPI_MODE_3;
		break;
	}
	switch(prescaler)
	{
		case 2: SPI_PRESCALER_2;
		break;
		case 4: SPI_PRESCALER_4;
		break;
		case 8: SPI_PRESCALER_8;
		break;
		case 16: SPI_PRESCALER_16;
		break;
		case 32: SPI_PRESCALER_32;
		break;
		case 64: SPI_PRESCALER_64;
		break;
		case 128: SPI_PRESCALER_128;
		break;
	}
	if(LSB_to_MSB) SPCR|=(1<<DORD);
	else SPCR&=~(1<<DORD);
	SPI_DDR|=(1<<SPI_MOSI_PIN)|(1<<SPI_SCK_PIN);			
}

void spi_set_SPDR(unsigned char data_transfer)
{	
	SPDR = data_transfer;	
}

void spi_begin_exchange(struct slave_device* device_name, unsigned char data_transfer)
{
	if(!device_name->exchange_process)
	{
		SPI_PORT_SS&=~(1<<device_name->SPI_SS_PIN);
		spi_set_SPDR(data_transfer);
		device_name->byte_counter_exchange = device_name->number_bytes_exchange;					
		device_name->exchange_process = true;
	}	
}

void spi_end_exchange(struct slave_device* device_name)
{
	SPI_PORT_SS|=(1<<device_name->SPI_SS_PIN);
	device_name->exchange_process = false;
}

unsigned char spi_data_receiving()
{
	return SPDR;
}

void spi_exchange_few_bytes(struct slave_device* device_name, unsigned char data_transfer)
{
	device_name->data_receive[device_name->byte_counter_exchange - 1] = SPDR;
	device_name->byte_counter_exchange--;
	if(device_name->byte_counter_exchange == 0) spi_end_exchange(device_name);
	else spi_set_SPDR(data_transfer);
}