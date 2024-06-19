/*
 * dht11.h
 *
 * Created: 10/6/2024 18:24:02
 *  Author: Alexis
 */ 
#ifndef DHT11_H
#define DHT11_H

#include <avr/io.h>

// Definiciones para el pin del DHT11 (PC0 en el ATmega328P)
#define PIN_DHT11 0                 // Número de pin del DHT11
#define PORT_DHT11 PORTC            // Puerto del DHT11
#define DDIR_DHT11 DDRC             // Dirección de datos del puerto del DHT11

// Códigos de retorno para la función leer_datos_dht11
#define OK_DHT11 0
#define ERROR_TIEMPO_ESPERA_DHT11 1
#define ERROR_CHECKSUM_DHT11 2

// Prototipos de funciones
void iniciar_comunicacion_dht11(void);
uint8_t leer_byte_dht11(void);
uint8_t leer_datos_dht11(uint8_t *temperatura, uint8_t *humedad);

#endif // DHT11_H