
/*
 * ds3231.c
 *
 * Created: 14/6/2024 16:00:00
 *  Author: alexis
 */ 

#include "twi.h"
#include "ds3231.h"

#define DS3231_ADDRESS 0x68
#define REG_SECONDS 0x00//dirección del registro que almacena los segundos 
#define REG_MINUTES 0x01//dirección del registro que almacena los minutos
#define REG_HOURS 0x02//dirección del registro que almacena las horas 
#define REG_DAY 0x03//direccion que almacena el dia de la semana
#define REG_DATE 0x04//dirección del registro que almacena el dia del mes 
#define REG_MONTH 0x05//dirección del registro que almacena el mes
#define REG_YEAR 0x06//dirección del registro que almacena el año 

// Convierte un número BCD a decimal
static uint8_t bcd2dec(uint8_t bcd) {
    return (bcd >> 4) * 10 + (bcd & 0x0F);
}

// Convierte un número decimal a BCD
static uint8_t dec2bcd(uint8_t dec) {
    return ((dec / 10) << 4) | (dec % 10);
}

// Escribe un valor en un registro específico del DS3231
static void DS3231_WriteRegister(uint8_t reg, uint8_t value) {
	TWI_Start();  // Inicia la comunicación I2C
	TWI_Write((DS3231_ADDRESS << 1) | 0x00);  // Envía la dirección del DS3231 con el bit de escritura (0)
	TWI_Write(reg);  // Envía la dirección del registro al que queremos escribir
	TWI_Write(value);  // Envía el valor que queremos escribir en ese registro
	TWI_Stop();  // Termina la comunicación I2C
}

// Lee un valor de un registro específico del DS3231
static uint8_t DS3231_ReadRegister(uint8_t reg) {
	uint8_t value;  // Variable para almacenar el valor leído
	TWI_Start();  // Inicia la comunicación I2C
	TWI_Write((DS3231_ADDRESS << 1) | 0x00);  // Envía la dirección del DS3231 con el bit de escritura (0)
	TWI_Write(reg);  // Envía la dirección del registro que queremos leer
	TWI_Start();  // Reinicia la comunicación I2C (repeated start)
	TWI_Write((DS3231_ADDRESS << 1) | 0x01);  // Envía la dirección del DS3231 con el bit de lectura (1)
	value = TWI_Read_NACK();  // Lee el valor del registro y envía NACK (indicando que no queremos leer más)
	TWI_Stop();  // Termina la comunicación I2C
	return value;  // Devuelve el valor leído
}

// Inicializa el módulo TWI para comunicarse con el DS3231
void DS3231_Init(void) {
	TWI_Init();  // Inicializa el hardware TWI (I2C) del microcontrolador
}

// Establece la hora y fecha en el DS3231
void DS3231_SetTime(uint8_t segundos, uint8_t minutos, uint8_t horas, uint8_t dia, uint8_t fecha, uint8_t mes, uint8_t ano) {
	DS3231_WriteRegister(REG_SECONDS, dec2bcd(segundos));  // Escribe los segundos (convertidos a BCD)
	DS3231_WriteRegister(REG_MINUTES, dec2bcd(minutos));   // Escribe los minutos (convertidos a BCD)
	DS3231_WriteRegister(REG_HOURS, dec2bcd(horas));       // Escribe las horas (convertidas a BCD)
	DS3231_WriteRegister(REG_DAY, dec2bcd(dia));           // Escribe el día de la semana (convertido a BCD)
	DS3231_WriteRegister(REG_DATE, dec2bcd(fecha));        // Escribe el día del mes (convertido a BCD)
	DS3231_WriteRegister(REG_MONTH, dec2bcd(mes));         // Escribe el mes (convertido a BCD)
	DS3231_WriteRegister(REG_YEAR, dec2bcd(ano));          // Escribe el año (convertido a BCD)
}

// Lee la hora y fecha actual del DS3231
void DS3231_GetTime(uint8_t *segundos, uint8_t *minutos, uint8_t *horas, uint8_t *dia, uint8_t *fecha, uint8_t *mes, uint8_t *ano) {
	*segundos = bcd2dec(DS3231_ReadRegister(REG_SECONDS));  // Lee y convierte los segundos de BCD a decimal
	*minutos = bcd2dec(DS3231_ReadRegister(REG_MINUTES));   // Lee y convierte los minutos de BCD a decimal
	*horas = bcd2dec(DS3231_ReadRegister(REG_HOURS));       // Lee y convierte las horas de BCD a decimal
	*dia = bcd2dec(DS3231_ReadRegister(REG_DAY));           // Lee y convierte el día de la semana de BCD a decimal
	*fecha = bcd2dec(DS3231_ReadRegister(REG_DATE));        // Lee y convierte el día del mes de BCD a decimal
	*mes = bcd2dec(DS3231_ReadRegister(REG_MONTH));         // Lee y convierte el mes de BCD a decimal
	*ano = bcd2dec(DS3231_ReadRegister(REG_YEAR));          // Lee y convierte el año de BCD a decimal
}
