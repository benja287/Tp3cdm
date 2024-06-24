/*
#ifndef TWI_H_
#define TWI_H_

#include <avr/io.h>
#define F_CPU 16000000UL // Definición de la frecuencia del CPU (16 MHz en la mayoría de las placas Arduino)
#define F_SCL 100000UL // Velocidad del reloj I2C
#define Prescaler 1
#define TWBR_val ((((F_CPU / F_SCL) / Prescaler) - 16) / 2)

void TWI_Init(void);
void TWI_Start(void);
void TWI_Stop(void);
void TWI_Write(uint8_t data);
uint8_t TWI_Read_ACK(void);
uint8_t TWI_Read_NACK(void);

#endif // TWI_H_ */
#ifndef TWI_H_
#define TWI_H_

#include <avr/io.h>
#define XTAL 16000000UL //  frecuencia del cristal o oscilador del microcontrolador
#define SCL 100000UL // frecuencia de la línea SCL (Serial Clock Line) del bus I2C
#define TWBR_val (((XTAL / SCL)  - 16) / 2)// es el valor del registro TWI Bit Rate
void TWI_Init(void);
void TWI_Start(void);
void TWI_Stop(void);
void TWI_Write(uint8_t data);
uint8_t TWI_Read_ACK(void);
uint8_t TWI_Read_NACK(void);




#endif /* TWI_H_ */