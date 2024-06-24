
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "mef.h"
#include "stdio.h"
#include "serialPort.h"
#include "dht11.h"
#include "ds3231.h"
#include "twi.h"

extern volatile uint8_t flag_Paso2S;
extern volatile uint8_t enviar_datos;

// Definir los estados de la máquina de estados finitos
typedef enum {
	ESTADO_Esperando,
	ESTADO_Enviar_Datos
} estado_t;

estado_t estado;
void convertir_a_cadena(uint8_t num, char* str, int width);
void formatear_concatenar(char* dest, const char* src);

void formatearMensaje(char* buffer, size_t buffer_size, uint8_t temperatura, uint8_t humedad,
uint8_t fecha, uint8_t mes, uint8_t ano, uint8_t horas, uint8_t minutos, uint8_t segundos) {
	// Variables para cadenas temporales
	char temp_str[4];
	char hum_str[4];
	char fecha_str[3], mes_str[3], ano_str[3];
	char horas_str[3], minutos_str[3], segundos_str[3];

	// Convertir los números a cadenas
	convertir_a_cadena(temperatura, temp_str, 2);
	convertir_a_cadena(humedad, hum_str, 2);
	convertir_a_cadena(fecha, fecha_str, 2);
	convertir_a_cadena(mes, mes_str, 2);
	convertir_a_cadena(ano, ano_str, 2);
	convertir_a_cadena(horas, horas_str, 2);
	convertir_a_cadena(minutos, minutos_str, 2);
	convertir_a_cadena(segundos, segundos_str, 2);

	// Formatear y concatenar las cadenas
	buffer[0] = '\0'; // Inicializar el buffer con una cadena vacía
	formatear_concatenar(buffer, "TEMP: ");
	formatear_concatenar(buffer, temp_str);
	formatear_concatenar(buffer, "°C HUM: ");
	formatear_concatenar(buffer, hum_str);
	formatear_concatenar(buffer, "% FECHA: ");
	formatear_concatenar(buffer, fecha_str);
	formatear_concatenar(buffer, "/");
	formatear_concatenar(buffer, mes_str);
	formatear_concatenar(buffer, "/");
	formatear_concatenar(buffer, ano_str);
	formatear_concatenar(buffer, " HORA: ");
	formatear_concatenar(buffer, horas_str);
	formatear_concatenar(buffer, ":");
	formatear_concatenar(buffer, minutos_str);
	formatear_concatenar(buffer, ":");
	formatear_concatenar(buffer, segundos_str);
	formatear_concatenar(buffer, "\r\n");
}

// Función para convertir un entero a una cadena con ancho fijo
void convertir_a_cadena(uint8_t num, char* str, int width) {
	int i;
	for (i = width - 1; i >= 0; i--) {
		str[i] = (num % 10) + '0';
		num /= 10;
	}
	str[width] = '\0';
}

// Función para concatenar cadenas
void formatear_concatenar(char* dest, const char* src) {
	while (*dest) {
		dest++;
	}
	while (*src) {
		*dest++ = *src++;
	}
	*dest = '\0';
}


// Función para el estado esperando
void estado_esperando(void) {
	if (enviar_datos) {//Verifica si la variable global enviar_datos es verdadera
		estado = ESTADO_Enviar_Datos;//cambia el estado actual (estado) a ESTADO_Enviar_Datos
		flag_Paso2S = 0;
	}
}

// Función para el estado de enviar datos
void estado_enviar_datos(void) {
	uint8_t temperatura, humedad;
	uint8_t segundos, minutos, horas, dia, fecha, mes, ano;
	char buffer_datos[60];

     
	// Leer datos del DHT11
	if (leer_datos_dht11(&temperatura, &humedad) == OK_DHT11) {
		// Leer la fecha y la hora del DS3231
		DS3231_GetTime(&segundos, &minutos, &horas, &dia, &fecha, &mes, &ano);

		// Formatear el mensaje con los datos obtenidos
		formatearMensaje(buffer_datos, sizeof(buffer_datos), temperatura, humedad, fecha, mes, ano, horas, minutos, segundos);

		// Enviar el mensaje a la terminal serie
		SerialPort_Send_String(buffer_datos);
		} else {
		SerialPort_Send_String("Error al leer el DHT11\r\n");
	}

	estado = ESTADO_Esperando; // Volver al estado esperando
}

// Inicializa la MEF colocando el estado inicial como ESTADO_Esperando.
void MEF_Init(void) {
	estado = ESTADO_Esperando;
}

// Inicialización de datos y configuración UART
void Datos_Init() {
	SerialPort_Init(103);//inicializa el UART con una configuración específica, donde el valor 103 se usa para configurar la velocidad de transmisión y otros parámetros del UART, como la cantidad de bits de datos y la paridad
	SerialPort_TX_Enable();//habilita la transmisión (TX) del UART, permitiendo que el microcontrolador pueda enviar datos a través del puerto serie.
	SerialPort_RX_Enable();//habilita la recepción (RX) del UART, permitiendo que el microcontrolador pueda recibir datos a través del puerto serie
	SerialPort_RX_Interrupt_Enable(); // Habilitar interrupción RX completa
	sei(); // Habilitar interrupciones globales
	MEF_Init();
	// Enviar mensaje inicial a la terminal serie
	SerialPort_Send_String("Presione la tecla 's' o 'S' para iniciar/detener el envío de datos.\r\n");
	TWI_Init(); // Inicializa el I2C
}
void (*MEF[])(void) = {estado_esperando, estado_enviar_datos};
// Función para actualizar el estado actual
void Actualizar_Datos(void) {
	(*MEF[estado])(); // Ejecuta la función correspondiente al estado actual
}



