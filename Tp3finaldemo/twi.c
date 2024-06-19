/*
 * twi.c
 *
 * Created: 10/6/2024 18:29:52
 *  Author: Alexis
 */ 
#include "twi.h"

void TWI_Init(void) {
	TWSR = 0x00; // establece  los bits del prescaler del registro TWSR a cero 
	TWBR = (uint8_t)TWBR_val; // corresponde a una frecuencia  de 100 khz para el reloj  SCL, considerando  que el cristal(XTAL)  del microcontrolador funciona  a 16 Mhz
	TWCR = (1<<TWEN); //Habilita el módulo TWI
}

void TWI_Start(void) {
	TWCR = (1<<TWSTA) | (1<<TWEN) | (1<<TWINT);//Configurar  el registro del control TWI(TWCR)  para enviar una condición de inicio (START)  en el bus  I2C y habilita el módulo  TWI
	while (!(TWCR & (1<<TWINT))); // Espera a que se complete la condición de start
}

void TWI_Stop(void) {
	TWCR = (1<<TWSTO) | (1<<TWEN) | (1<<TWINT);// Configurar el registro de control TWI (TWCR) para enviar una condición de parada (STOP) en el bus I2C
}

void TWI_Write(uint8_t data) {
	TWDR = data;//Cargar el dato que se desea enviar en el  registro de datos TWI(TWDR)
	TWCR = (1<<TWEN) | (1<<TWINT);//Configurar el registro control  TWI(TWCR) para iniciar la transmisión del dato 
	while (!(TWCR & (1<<TWINT))); // Espera a que la escritura se complete es decir esperar a que se complete la transmisión del dato

}
uint8_t TWI_Read_ACK(void) {
	TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWEA);//Configurar el registro de control TWI (TWCR) para recibir el próximo byte y enviar una señal de reconocimiento (ACK) al esclavo.
	while (!(TWCR & (1<<TWINT))); // Espera a que la lectura se complete
	return TWDR;
}

uint8_t TWI_Read_NACK(void) {
	TWCR = (1<<TWEN) | (1<<TWINT);//Configurar el registro de control TWI (TWCR) para recibir el próximo byte y no enviar una señal de reconocimiento (NACK) al esclavo
	while (!(TWCR & (1<<TWINT))); // Espera a que la lectura se complete
	return TWDR;
}
