/*
 * dht11.c
 *
 * Created: 10/6/2024 18:25:11
 *  Author: Alexis
 */ 
#include "dht11.h"
#define F_CPU 16000000UL
#include <util/delay.h>
#include "serialPort.h"
#define TIEMPO_ESPERA_SOLICITUD_DHT11 1000  // Tiempo máximo de espera para la solicitud de datos (en microsegundos)
// Iniciar la comunicación con el sensor DHT11
void iniciar_comunicacion_dht11(void) {
	//SerialPort_Send_String("Iniciando comunicacion con el sensor DHT11\r\n");
	DDIR_DHT11 |= (1 << PIN_DHT11);  // Configurar el pin como salida
	//   _delay_ms(20);
	PORT_DHT11 &= ~(1 << PIN_DHT11);  // Poner el pin en estado bajo
	_delay_ms(20);  // Esperar al menos 18 ms
	PORT_DHT11 |= (1 << PIN_DHT11);  // Poner el pin en estado alto
	_delay_us(4);  // Esperar 4 microsegundos
	DDIR_DHT11 &= ~(1 << PIN_DHT11);  // Configurar el pin como entrada
	_delay_us(160);
}

uint8_t leer_byte_dht11(void) {
	uint8_t datos = 0;
	//_delay_us(160);

	for (uint8_t i = 0; i < 8; i++) {
		

		// Esperar a que el pin se ponga en alto indicando el inicio de la transmisión de un bit
		while (!(PINC & (1 << PIN_DHT11)) );

		// Esperar 40 microsegundos para posicionarse en la mitad del pulso
		_delay_us(40);

		if (PINC & (1 << PIN_DHT11)) {
			// Si el pin sigue en alto después de 40us, es un "1"
			datos |= (1 << (7 - i));

			// Esperar a que el pin se ponga en bajo indicando el final del bit
			while (PINC & (1 << PIN_DHT11) );
			} else {
			// Si el pin está en bajo después de 40us, es un "0"
			// Nada que hacer ya que el bit ya es "0" por defecto
		}
	}
	return datos;
}

// Leer los datos del sensor DHT11
uint8_t leer_datos_dht11(uint8_t *temperatura, uint8_t *humedad) {
	//SerialPort_Send_String("Leyendo datos del sensor DHT11\r\n");
	iniciar_comunicacion_dht11();  // Iniciar la comunicación

	uint16_t tiempo_espera_solicitud = TIEMPO_ESPERA_SOLICITUD_DHT11;
	while (tiempo_espera_solicitud--) {  // Esperar la respuesta del sensor
		if (!(PINC & (1 << PIN_DHT11))) {
			break;
		}
	}

	if (tiempo_espera_solicitud == 0) {  // Tiempo de espera agotado
		SerialPort_Send_String("Error: Tiempo de espera agotado\r\n");
		return ERROR_TIEMPO_ESPERA_DHT11;
	}

	uint8_t datos[5] = {0};
	for (uint8_t i = 0; i < 5; i++) {
		datos[i] = leer_byte_dht11();  // Leer cada byte de datos
	}

	uint8_t suma = datos[0] + datos[1] + datos[2] + datos[3];
	if (datos[4] != suma) {  // Verificar el checksum
		SerialPort_Send_String("Error: Checksum incorrecto\r\n");
		return ERROR_CHECKSUM_DHT11;
	}
	

	*humedad = datos[0];
	*temperatura = datos[2];

	return OK_DHT11;
}