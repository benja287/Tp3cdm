/*
#ifndef DS3231_H_
#define DS3231_H_

#include <avr/io.h>

void DS3231_GetTime(uint8_t *second, uint8_t *minute, uint8_t *hour, uint8_t *day, uint8_t *date, uint8_t *month, uint8_t *year);
void DS3231_SetTime(uint8_t second, uint8_t minute, uint8_t hour, uint8_t day, uint8_t date, uint8_t month, uint8_t year);
uint8_t get_time(char* buffer, int tamanio);


#endif // DS3231_H_ */

#ifndef DS3231_H_
#define DS3231_H_

#include <stdint.h>

// Inicializa el módulo TWI para comunicarse con el DS3231
void DS3231_Init(void);

// Establece la hora y fecha en el DS3231
void DS3231_SetTime(uint8_t segundos, uint8_t minutos, uint8_t horas, uint8_t dia, uint8_t fecha, uint8_t mes, uint8_t ano);

// Lee la hora y fecha actual del DS3231
void DS3231_GetTime(uint8_t *segundos, uint8_t *minutos, uint8_t *horas, uint8_t *dia, uint8_t *fecha, uint8_t *mes, uint8_t *ano);

#endif /* DS3231_H_ */