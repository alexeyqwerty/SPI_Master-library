#include <stdbool.h>

//Define SPI pins for your microcontroller(users settings)
#define SPI_PORT PORTB
#define SPI_DDR DDRB
#define SPI_MISO_PIN 4
#define SPI_MOSI_PIN 3
#define SPI_SCK_PIN 5

//Define slave select port(users settings)
#define SPI_PORT_SS PORTD
#define SPI_DDR_SS DDRD

//Define SPI mode
#define SPI_MODE_0 SPCR&=~(1<<CPHA)&~(1<<CPOL)
#define SPI_MODE_1 SPCR&=~(1<<CPHA), SPCR|=(1<<CPOL)
#define SPI_MODE_2 SPCR|=(1<<CPHA), SPCR&=~(1<<CPOL)
#define SPI_MODE_3 SPCR|=(1<<CPHA)|(1<<CPOL)

//Define SPI prescaler
#define SPI_PRESCALER_2 SPCR&=~(1<<SPR1)&~(1<<SPR0), SPSR|=(1<<SPI2X)
#define SPI_PRESCALER_4 SPCR&=~(1<<SPR1)&~(1<<SPR0), SPSR&=~(1<<SPI2X)
#define SPI_PRESCALER_8 SPCR&=~(1<<SPR1), SPCR|=(1<<SPR0), SPSR|=(1<<SPI2X)
#define SPI_PRESCALER_16 SPCR&=~(1<<SPR1), SPCR|=(1<<SPR0), SPSR&=~(1<<SPI2X)
#define SPI_PRESCALER_32 SPCR|=(1<<SPR1), SPCR&=~(1<<SPR0), SPSR|=(1<<SPI2X)
#define SPI_PRESCALER_64 SPCR|=(1<<SPR1), SPCR&=~(1<<SPR0), SPSR&=~(1<<SPI2X)
#define SPI_PRESCALER_128 SPCR|=(1<<SPR1)|(1<<SPR0), SPSR&=~(1<<SPI2X)

//Create struct object for slave select
struct slave_device
{
	//Output of the micro controller to select the device
	unsigned char SPI_SS_PIN;

	//Needed for count the number of bytes to exchange of data
	unsigned char byte_counter_exchange;

	//Number of bytes for exchange of data
	unsigned char number_bytes_exchange;

	//If true, the data exchange in the process
	_Bool exchange_process;

	//The array for processing the received data 
	unsigned char data_receive[8];
};

//Initialization SPI, spi_mode must be from 0 to 3, prescaler must be 2, 4, 8, 16, 32, 64 or 128
void spi_init(char spi_mode, char prescaler, _Bool LSB_to_MSB);

//Initialization slave port(ports)
void spi_slave_port_init(struct slave_device device_name);

//Begin the exchange SPI
void spi_begin_exchange(struct slave_device* device_name, unsigned char data_transfer);

//End the exchange SPI
void spi_end_exchange(struct slave_device* device_name);

//Return received data from the SPI data register
unsigned char spi_data_receiving();

//For exchanging a few bytes
void spi_exchange_few_bytes(struct slave_device* device_name, unsigned char data_transfer);

//For example, to use these functions
//
// struct slave_device max;
//
// ISR(SPI_STC_vect)
// {
// 		spi_exchange_few_bytes(&max, 23);
// 		if(!max.exchange_process) Blah_blah_blah;
// }
//
// ISR(TIMER1_COMPA_vect)
// {
// 		spi_begin_exchange(&max, 1);
// }
// 
// int main(void)
// {
// 		max.exchange_process = false;
//		max.number_bytes_exchange = 4;
// 		max.SPI_SS_PIN = 0;
// 
// 		spi_slave_port_init(max);
// 		spi_init(0, 16, false);
// 	
//		TCCR1B|=(1<<WGM12)|(1<<CS12)|(1<<CS10);
//		TIMSK|=(1<<OCIE1A);
//		OCR1A = 4000;
// 	
//		sei();
// 	
//		while(1)
// 		{
// 
// 		}
// }

