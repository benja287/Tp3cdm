
#define F_CPU 16000000UL // Definimos la frecuencia del MCU
#include <avr/io.h>
#include <util/delay.h>
#include "mef.h"
#include "twi.h"
#include "stdio.h"
#include "ds3231.h" // Incluir la cabecera del DS3231
#include "dht11.h" // Incluir la cabecera del DHT11
#include "serialPort.h"

volatile uint8_t flag_teclaS = 0;
volatile uint8_t flag_Paso2S = 0;
volatile uint8_t enviar_datos = 0; // Controla si se deben enviar los datos

int main(void) {
   Datos_Init(); // Inicializa la UART y la MEF

   while (1) {
	   // Incrementar el contador cada segundo
	   _delay_ms(1000); // Retraso de 1000ms
	   // Incrementar el contador
	   flag_Paso2S++;

	   // Verificar si se deben enviar datos cada 2 segundos
	   if (flag_Paso2S >= 1) {
		   if (enviar_datos) {
			   Actualizar_Datos();
		   }
		   // Resetear el contador después de encuestar y enviar datos
		   flag_Paso2S = 0;
	   }
   }
   return 0;
}
