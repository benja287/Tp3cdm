/*
#include "twi.h"
#include "ds3231.h"
#include "stdio.h"
#define DS3231_ADDRESS 0x68

uint8_t bcd_to_decimal(uint8_t val) {
	return ((val / 16 * 10) + (val % 16));
}
uint8_t decimal_to_bcd(uint8_t val) {
	return ((val / 10 * 16) + (val % 10));
}
void DS3231_SetTime(uint8_t second, uint8_t minute, uint8_t hour, uint8_t day, uint8_t date, uint8_t month, uint8_t year) {
	TWI_Start();
	TWI_Write((DS3231_ADDRESS << 1) | 0x00); // Direcci�n de escritura
	TWI_Write(0x00); // Direcci�n de registro inicial para escritura
	
	TWI_Write(decimal_to_bcd(second)); // Escribir segundos
	TWI_Write(decimal_to_bcd(minute)); // Escribir minutos
	TWI_Write(decimal_to_bcd(hour));   // Escribir horas
	TWI_Write(decimal_to_bcd(day));    // Escribir d�a de la semana
	TWI_Write(decimal_to_bcd(date));   // Escribir d�a del mes
	TWI_Write(decimal_to_bcd(month));  // Escribir mes
	TWI_Write(decimal_to_bcd(year));   // Escribir a�o
	
	TWI_Stop();
}
void DS3231_GetTime(uint8_t *second, uint8_t *minute, uint8_t *hour, uint8_t *day, uint8_t *date, uint8_t *month, uint8_t *year) {
	TWI_Start();
	TWI_Write((DS3231_ADDRESS << 1) | 0x00); // Direcci�n de escritura
	TWI_Write(0x00); // Direcci�n de registro inicial para lectura
	
	TWI_Start();
	TWI_Write((DS3231_ADDRESS << 1) | 0x01); // Direcci�n de lectura
	
	*second = bcd_to_decimal(TWI_Read_ACK());
	*minute = bcd_to_decimal(TWI_Read_ACK());
	*hour = bcd_to_decimal(TWI_Read_ACK());
	*day = bcd_to_decimal(TWI_Read_ACK());
	*date = bcd_to_decimal(TWI_Read_ACK());
	*month = bcd_to_decimal(TWI_Read_ACK());
	*year = bcd_to_decimal(TWI_Read_NACK()); // El �ltimo debe ser le�do con NACK
	
	TWI_Stop();
}

uint8_t get_time(char* buffer, int tamanio){
	uint8_t second, minute, hour, day, date, month, year;
	DS3231_GetTime(&second, &minute, &hour, &day, &date, &month, &year);

	// Formatear la fecha y la hora
	
	snprintf(buffer, tamanio, "FECHA: %02u/%02u/%02u HORA: %02u:%02u:%02u \r\n", date, month, year, hour, minute, second);

	// Enviar la fecha y la hora al terminal virtual
	//SerialPort_Send_String(buffer);
	return 1;
}*/#include "twi.h"
#include "ds3231.h"

#define DS3231_ADDRESS 0x68
#define REG_SECONDS 0x00//direcci�n del registro que almacena los segundos
#define REG_MINUTES 0x01//direcci�n del registro que almacena los minutos
#define REG_HOURS 0x02//direcci�n del registro que almacena las horas
#define REG_DAY 0x03//direccion que almacena el dia de la semana
#define REG_DATE 0x04//direcci�n del registro que almacena el dia del mes
#define REG_MONTH 0x05//direcci�n del registro que almacena el mes
#define REG_YEAR 0x06//direcci�n del registro que almacena el a�o

// Convierte un n�mero BCD a decimal
static uint8_t bcd2dec(uint8_t bcd) {
return (bcd >> 4) * 10 + (bcd & 0x0F);
}

// Convierte un n�mero decimal a BCD
static uint8_t dec2bcd(uint8_t dec) {
return ((dec / 10) << 4) | (dec % 10);
}

// Escribe un valor en un registro espec�fico del DS3231
static void DS3231_WriteRegister(uint8_t reg, uint8_t value) {
TWI_Start();  // Inicia la comunicaci�n I2C
TWI_Write((DS3231_ADDRESS << 1) | 0x00);  // Env�a la direcci�n del DS3231 con el bit de escritura (0)
TWI_Write(reg);  // Env�a la direcci�n del registro al que queremos escribir
TWI_Write(value);  // Env�a el valor que queremos escribir en ese registro
TWI_Stop();  // Termina la comunicaci�n I2C
}

// Lee un valor de un registro espec�fico del DS3231
static uint8_t DS3231_ReadRegister(uint8_t reg) {
uint8_t value;  // Variable para almacenar el valor le�do
TWI_Start();  // Inicia la comunicaci�n I2C
TWI_Write((DS3231_ADDRESS << 1) | 0x00);  // Env�a la direcci�n del DS3231 con el bit de escritura (0)
TWI_Write(reg);  // Env�a la direcci�n del registro que queremos leer
TWI_Start();  // Reinicia la comunicaci�n I2C (repeated start)
TWI_Write((DS3231_ADDRESS << 1) | 0x01);  // Env�a la direcci�n del DS3231 con el bit de lectura (1)
value = TWI_Read_NACK();  // Lee el valor del registro y env�a NACK (indicando que no queremos leer m�s)
TWI_Stop();  // Termina la comunicaci�n I2C
return value;  // Devuelve el valor le�do
}

// Inicializa el m�dulo TWI para comunicarse con el DS3231
void DS3231_Init(void) {
TWI_Init();  // Inicializa el hardware TWI (I2C) del microcontrolador
}

// Establece la hora y fecha en el DS3231
void DS3231_SetTime(uint8_t segundos, uint8_t minutos, uint8_t horas, uint8_t dia, uint8_t fecha, uint8_t mes, uint8_t ano) {
DS3231_WriteRegister(REG_SECONDS, dec2bcd(segundos));  // Escribe los segundos (convertidos a BCD)
DS3231_WriteRegister(REG_MINUTES, dec2bcd(minutos));   // Escribe los minutos (convertidos a BCD)
DS3231_WriteRegister(REG_HOURS, dec2bcd(horas));       // Escribe las horas (convertidas a BCD)
DS3231_WriteRegister(REG_DAY, dec2bcd(dia));           // Escribe el d�a de la semana (convertido a BCD)
DS3231_WriteRegister(REG_DATE, dec2bcd(fecha));        // Escribe el d�a del mes (convertido a BCD)
DS3231_WriteRegister(REG_MONTH, dec2bcd(mes));         // Escribe el mes (convertido a BCD)
DS3231_WriteRegister(REG_YEAR, dec2bcd(ano));          // Escribe el a�o (convertido a BCD)
}

// Lee la hora y fecha actual del DS3231
void DS3231_GetTime(uint8_t *segundos, uint8_t *minutos, uint8_t *horas, uint8_t *dia, uint8_t *fecha, uint8_t *mes, uint8_t *ano) {
*segundos = bcd2dec(DS3231_ReadRegister(REG_SECONDS));  // Lee y convierte los segundos de BCD a decimal
*minutos = bcd2dec(DS3231_ReadRegister(REG_MINUTES));   // Lee y convierte los minutos de BCD a decimal
*horas = bcd2dec(DS3231_ReadRegister(REG_HOURS));       // Lee y convierte las horas de BCD a decimal
*dia = bcd2dec(DS3231_ReadRegister(REG_DAY));           // Lee y convierte el d�a de la semana de BCD a decimal
*fecha = bcd2dec(DS3231_ReadRegister(REG_DATE));        // Lee y convierte el d�a del mes de BCD a decimal
*mes = bcd2dec(DS3231_ReadRegister(REG_MONTH));         // Lee y convierte el mes de BCD a decimal
*ano = bcd2dec(DS3231_ReadRegister(REG_YEAR));          // Lee y convierte el a�o de BCD a decimal
}