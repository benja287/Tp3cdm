/*
 * ISR.c
 *
 * Created: 10/6/2024 18:40:03
 *  Author: Alexis
 */ 
#include <avr/interrupt.h>
#include "ISR.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include "serialPort.h"
#include "mef.h"

extern volatile uint8_t enviar_datos; // Bandera para controlar el envío de datos

ISR(USART_RX_vect) {
	//SerialPort_Send_String("se recibio algun caracter\r\n");

	char recibir_caracter = UDR0; // Leer el carácter recibido

	if (recibir_caracter == 's' || recibir_caracter == 'S') {
		enviar_datos = !enviar_datos; // Alternar la bandera de enviar_datos
	}
}

