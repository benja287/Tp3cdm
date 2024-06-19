/*
 * serialPort.c
 *
 * Created: 10/6/2024 18:31:31
 *  Author: Alexis
 */ 
#include "SerialPort.h"

// ------ Definiciones de Funciones Públicas -------------------

// Inicialización de Puerto Serie

void SerialPort_Init(uint8_t config){
	// config = 0x33 ==> Configuro UART 9600bps, 8 bit data, 1 stop @ F_CPU = 8MHz.
	// config = 0x25 ==> Configuro UART 9600bps, 8 bit data, 1 stop @ F_CPU = 4Hz.
	UCSR0B = 0;//Deshabilita  todas las funciones de transmisión  y recepción inicialmente
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);//configura la longitud de los datos a 8 bits 
	//UBRR0H y UBRR0L son registros que establecen la tasa de baudios 
	//config se desplaza y se divide en partes altas y bajas para establecer la velocidad de comunicación
	UBRR0H = (unsigned char)(config>>8);
	UBRR0L = (unsigned char)config;
}


// Inicialización de Transmisor

void SerialPort_TX_Enable(void){
	UCSR0B |= (1<<TXEN0);//habilita el transmisor serie(Tx)
}
//habilita las interrupciones del transmisor 
void SerialPort_TX_Interrupt_Enable(void){
	UCSR0B |= (1<<UDRIE0);//habilita la interrupción cuando  el registro de datos UDR0 esta vacio y listo para recibir nuevos datos 
	//UCSR0B |=(1<<TXCIE0); //habilita la interrupción  al finalizar la transmisión si se necesitara

}
//deshabilita las interrupciones del transmisor 
void SerialPort_TX_Interrupt_Disable(void)
{
	UCSR0B &=~(1<<UDRIE0);//deshabilita la interrupción del registro de datos UDR0 vacío
	//UCSR0B &=~(1<<TXCIE0); //deshabilita la interrupción al finalizar la transmisión

}


// Inicialización de Receptor

void SerialPort_RX_Enable(void){
	UCSR0B |= (1<<RXEN0);//habilita el receptor serie (RX)
}
//habilita las interrupciones del receptor
void SerialPort_RX_Interrupt_Enable(void){
	UCSR0B |= (1<<RXCIE0);//habilita la interrupción cuando  hay datos nuevos en el registro de datos UDR0
}


// Transmisión

// Espera hasta que el buffer de TX este libre.
void SerialPort_Wait_For_TX_Buffer_Free(void){
	// Pooling - Bloqueante hasta que termine de transmitir.
	while(!(UCSR0A & (1<<UDRE0)));
}
//envia un dato  a través del puerto  serie 
void SerialPort_Send_Data(char data){
	UDR0 = data;//escribe el dato en el registro de datos UDR0 para su transmisión 
}
//envia una cadena de caracteres   a través del puerto  serie 
void SerialPort_Send_String(char * msg){ //
	uint8_t i = 0;
	//'\0' = 0x00
	while(msg[i]){ //recorre la cadena de caracteres  hasta encontrar el caracter nulo 
		SerialPort_Wait_For_TX_Buffer_Free(); //9600bps formato 8N1, 10bits, 10.Tbit=10/9600=1ms ademas espera hasta que el buffer de transmisión  este libre
		SerialPort_Send_Data(msg[i]);//envia cada caracter  de la cadena
		i++;
	}
}


// Recepción

// Espera hasta que el buffer de RX este completo.
void SerialPort_Wait_Until_New_Data(void){
	// Pooling - Bloqueante, puede durar indefinidamente!
	while(!(UCSR0A & (1<<RXC0)));// espera hasta que el registro UDR0 tenga nuevos datos
}

//recibe un dato  del puerto  serie 
char SerialPort_Recive_Data(void){
	return UDR0; //devuelve el dato recibido  desde el registro de datos  UDR0
}

//envia un numero de 8 bits como una secuencia de caracteres 
void SerialPort_Send_uint8_t(uint8_t num){
	//Convierte el número en tres dígitos y los envía uno a uno
	SerialPort_Wait_For_TX_Buffer_Free();
	SerialPort_Send_Data('0'+num/100);
	
	num-=100;
	
	SerialPort_Wait_For_TX_Buffer_Free();
	SerialPort_Send_Data('0'+num/10);
	
	SerialPort_Wait_For_TX_Buffer_Free();
	SerialPort_Send_Data('0'+ num%10);
}

/***************************************************************
	This function writes a integer type value to UART
	Arguments:
	1)int val	: Value to print
	2)unsigned int field_length :total length of field in which the value is printed
	must be between 1-5 if it is -1 the field length is no of digits in the val
****************************************************************/
//envia un numero entero  como una cadena de caracteres 
void SerialPort_send_int16_t(int val,unsigned int field_length)
{
	char str[5]={0,0,0,0,0};
	int i=4,j=0;
	while(val)
	{
	str[i]=val%10;
	val=val/10;
	i--;
	}
	if(field_length==-1)
		while(str[j]==0) j++;
	else
		j=5-field_length;

	if(val<0) {
		SerialPort_Wait_For_TX_Buffer_Free();
		SerialPort_Send_Data('-');
		}
	for(i=j;i<5;i++)
	{
	SerialPort_Wait_For_TX_Buffer_Free();
	SerialPort_Send_Data('0'+str[i]);
	}
}

