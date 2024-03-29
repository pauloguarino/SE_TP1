/* Copyright 2016, Eric Pernia.
 * All rights reserved.
 *
 * This file is part sAPI library for microcontrollers.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 * Date: 2016-04-26
 */

/*==================[conditions]=============================================*/



/*==================[inclusions]=============================================*/

//#include "blinky.h"   // <= own header (optional)
#include "sapi.h"       // <= sAPI header

/*==================[macros and definitions]=================================*/

#define TP1_1 (1)
#define TP1_2 (2)
#define TP1_3 (3)
#define TP1_4 (4)
#define TP1_5 (5)
#define TP1_6 (6)
#define TEST (TP1_6)

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

void myTickHook( void *);

/*==================[internal data definition]===============================*/

volatile bool LED_Time_Flag = false;
volatile bool BUTTON_Status_Flag = false;
volatile bool BUTTON_Time_Flag = false;
volatile bool toggled = false;
uint32_t BUTTON_Status_Counter = 0;
/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

#if (TEST == TP1_3)
/* FUNCION que se ejecuta cada vez que ocurre un Tick. */
void myTickHook( void *ptr ){

   static bool_t ledState = OFF;

   gpioMap_t led = (gpioMap_t)ptr;

   if( ledState ){
      ledState = OFF;
   }
   else{
      ledState = ON;
   }
   gpioWrite( led, ledState );
}

#elif (TEST == TP1_4 || TEST == TP1_5)
/* FUNCION que se ejecuta cada vez que ocurre un Tick. */
void myTickHook( void *ptr ) {
	LED_Time_Flag = true;
}
#elif (TEST == TP1_6)

#define TICKRATE_1MS	(1)				/* 1000 ticks por segundo */
#define TICKRATE_10MS	(10)			/* 100 ticks por segundo */
#define TICKRATE_50MS	(50)			/* 10 ticks por segundo */
#define TICKRATE_MS		(TICKRATE_50MS)	/* defino el TICKRATE a 1MS */

#define BUTTON_STATUS_10MS	(10)
#define BUTTON_STATUS_100MS	(100)
#define BUTTON_STATUS_500MS	(500)
#define BUTTON_STATUS_MS	(BUTTON_STATUS_10MS / TICKRATE_MS)

/* FUNCION que se ejecuta cada vez que ocurre un Tick. */
void myTickHook( void *ptr ) {
	BUTTON_Time_Flag = true;

    if (!gpioRead( TEC1 ) || !gpioRead( TEC2 ) || !gpioRead( TEC3 ) || !gpioRead( TEC4 ))
		BUTTON_Status_Flag = true;
	else {
		BUTTON_Status_Flag = false;
		BUTTON_Status_Counter = BUTTON_STATUS_MS;
		toggled = false;
	}
}
#endif
/*==================[external functions definition]==========================*/

/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int main(void){

/* ----------------- ****** ----------------- */
/* ----------------- BLINKY ----------------- */
/* ----------------- ****** ----------------- */

#if (TEST == TP1_1)

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {

      /* Prendo el led azul */
      gpioToggle( LEDG);

      delay(1000);

      /* Apago el led azul */
      gpioToggle( LEDG);

      delay(1000);

   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */

   return 0;

#elif (TEST == TP1_2)

/* ----------------- **** ----------------- */
/* ----------------- LEDS ----------------- */
/* ----------------- **** ----------------- */

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   gpioConfig( GPIO0, GPIO_INPUT );

   gpioConfig( GPIO1, GPIO_OUTPUT );

   /* Variable para almacenar el valor de tecla leido */
   bool_t valor;

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {

      valor = !gpioRead( TEC1 );
      gpioWrite( LEDB, valor );

      valor = !gpioRead( TEC2 );
      gpioWrite( LED1, valor );

      valor = !gpioRead( TEC3 );
      gpioWrite( LED2, valor );

      valor = !gpioRead( TEC4 );
      gpioWrite( LED3, valor );

      valor = !gpioRead( GPIO0 );
      gpioWrite( GPIO1, valor );

   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0;

#elif TEST == TP1_3

/* ----------------- ******** ----------------- */
/* ----------------- TICKHOOK ----------------- */
/* ----------------- ******** ----------------- */

   /* ------------- INICIALIZACIONES ------------- */

   /* Inicializar la placa */
   boardConfig();

   /* Inicializar el conteo de Ticks con resolucion de 50ms (se ejecuta
      periodicamente una interrupcion cada 50ms que incrementa un contador de
      Ticks obteniendose una base de tiempos). */
   tickConfig( 50 );

   /* Se agrega ademas un "tick hook" nombrado myTickHook. El tick hook es
      simplemente una funcion que se ejecutara peri�odicamente con cada
      interrupcion de Tick, este nombre se refiere a una funcion "enganchada"
      a una interrupcion.
      El segundo parametro es el parametro que recibe la funcion myTickHook
      al ejecutarse. En este ejemplo se utiliza para pasarle el led a titilar.
   */
   tickCallbackSet( myTickHook, (void*)LEDR );
   delay(1000);

   /* ------------- REPETIR POR SIEMPRE ------------- */
   while(1) {
      tickCallbackSet( myTickHook, (void*)LEDG );
      delay(1000);
      tickCallbackSet( myTickHook, (void*)LEDB );
      delay(1000);
      tickCallbackSet( myTickHook, (void*)LED1 );
      delay(1000);
      tickCallbackSet( myTickHook, (void*)LED2 );
      delay(1000);
      tickCallbackSet( myTickHook, (void*)LED3 );
      delay(1000);
      tickCallbackSet( myTickHook, (void*)LEDR );
      delay(1000);
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0;

#elif TEST == TP1_4

#define TICKRATE_1MS	(1)					/* 1000 ticks por segundo */
#define TICKRATE_10MS	(10)				/* 100 ticks por segundo */
#define TICKRATE_50MS	(50)				/* 20 ticks por segundo */
#define TICKRATE_MS		(TICKRATE_1MS)		/* defino el TICKRATE a 1MS */

#define LED_TOGGLE_100MS	(100)		/**/
#define LED_TOGGLE_500MS	(500)		/**/
#define LED_TOGGLE_1000MS	(1000)		/**/
#define LED_TOGGLE_MS		(LED_TOGGLE_500MS / TICKRATE_MS)
   
   /* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */

   	/* ------------- INICIALIZACIONES ------------- */
   	uint32_t LED_Toggle_Counter = 0;

   	/* Inicializar la placa */
   	boardConfig();

   	/* Inicializar el conteo de Ticks con resolucion de 1ms (se ejecuta
          periodicamente una interrupcion cada TICKRATE_MS que incrementa un contador de
          Ticks obteniendose una base de tiempos). */
   	tickConfig( TICKRATE_MS );

   	/* Se agrega ademas un "tick hook" nombrado myTickHook. El tick hook es
          simplemente una funcion que se ejecutara peri�odicamente con cada
          interrupcion de Tick, este nombre se refiere a una funcion "enganchada"
          a una interrupcion.
          El segundo parametro es el parametro que recibe la funcion myTickHook
          al ejecutarse. En este ejemplo se utiliza para pasarle el led a titilar.
   	*/
   	tickCallbackSet( myTickHook, (void*)NULL );

   	gpioMap_t current_LED = LEDB;
   	bool LED_ON = false;
   	/* ------------- REPETIR POR SIEMPRE ------------- */
   	while(1) {
   		__WFI();

   		if (LED_Time_Flag == true) {
   			LED_Time_Flag = false;

   			if (LED_Toggle_Counter == 0) {
   				LED_Toggle_Counter = LED_TOGGLE_MS;
   				gpioToggle(current_LED);
   				if(LED_ON) {
					switch(current_LED) {
					case LEDB:
						current_LED = LEDG;
						break;
					case LEDG:
						current_LED = LEDR;
						break;
					case LEDR:
						current_LED = LED1;
						break;
					case LED1:
						current_LED = LED2;
						break;
					case LED2:
						current_LED = LED3;
						break;
					case LED3:
						current_LED = LEDB;
						break;
					}
					LED_ON = false;
   				}
   				else
   					LED_ON = true;
   			}
   			else
   				LED_Toggle_Counter--;
   		}
   	}

   	/* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
          por ningun S.O. */
   	return 0 ;

#elif TEST == TP1_5

#define TICKRATE_1MS	(1)				/* 1000 ticks por segundo */
#define TICKRATE_10MS	(10)			/* 100 ticks por segundo */
#define TICKRATE_50MS	(50)			/* 10 ticks por segundo */
#define TICKRATE_MS		(TICKRATE_50MS)	/* defino el TICKRATE a 1MS */

#define LED_TOGGLE_100MS	(100)		/* */
#define LED_TOGGLE_500MS	(500)		/**/
#define LED_TOGGLE_1000MS	(1000)		/**/
#define LED_TOGGLE_MS		(LED_TOGGLE_500MS / TICKRATE_MS)   
   
/* The DEBUG* functions are sAPI debug print functions.
Code that uses the DEBUG* functions will have their I/O routed to
the sAPI DEBUG UART. */  
DEBUG_PRINT_ENABLE;
  uint32_t LED_Toggle_Counter = 0;

   	   /* ------------- INICIALIZACIONES ------------- */

   	   /* Inicializar la placa */
   	   boardConfig();

   	   /* Inicializar el conteo de Ticks con resolucion de 1ms (se ejecuta
   	      periodicamente una interrupcion cada 1ms que incrementa un contador de
   	      Ticks obteniendose una base de tiempos). */
   	   tickConfig(  TICKRATE_MS	 );

   	   /* Se agrega ademas un "tick hook" nombrado myTickHook. El tick hook es
   	      simplemente una funcion que se ejecutara peri�odicamente con cada
   	      interrupcion de Tick, este nombre se refiere a una funcion "enganchada"
   	      a una interrupcion.
   	      El segundo parametro es el parametro que recibe la funcion myTickHook
   	      al ejecutarse. En este ejemplo se utiliza para pasarle el led a titilar.
   	   */
   	   debugPrintConfigUart( UART_USB, 115200);
   	   debugPrintString( "DEBUG c/sAPI\r\n" );

      	tickCallbackSet( myTickHook, (void*)NULL );
#define LED_NUMBER (6)
      	char LED_STRINGS[LED_NUMBER][20] = {
      			"LED_B Toggle \n",
				"LED_G Toggle \n",
				"LED_R Toggle \n",
				"LED_1 Toggle \n",
				"LED_2 Toggle \n",
				"LED_3 Toggle \n"
      	};
      	uint32_t i = 0; //no estoy seguro si conviene usar otro tipo de dato para esto
      	gpioMap_t current_LED = LEDB;
      	bool LED_ON = false;
      	/* ------------- REPETIR POR SIEMPRE ------------- */
      	while(1) {
      		__WFI();

      		if (LED_Time_Flag == true) {
      			LED_Time_Flag = false;

      			if (LED_Toggle_Counter == 0) {
      				LED_Toggle_Counter = LED_TOGGLE_MS;
      				gpioToggle(current_LED);
      				debugPrintString(LED_STRINGS[i]);
      				if(LED_ON) {
      					i++;
						switch(current_LED) {
							case LEDB:
								current_LED = LEDG;
								break;
							case LEDG:
								current_LED = LEDR;
								break;
							case LEDR:
								current_LED = LED1;
								break;
							case LED1:
								current_LED = LED2;
								break;
							case LED2:
								current_LED = LED3;
								break;
							case LED3:
								current_LED = LEDB;
								i = 0;
								break;
						}
						LED_ON = false;
      				}
      				else
      					LED_ON = true;
      			}
      			else
      				LED_Toggle_Counter--;
      		}
      	}

      	/* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
             por ningun S.O. */
      	return 0 ;

#elif TEST == TP1_6

/* The DEBUG* functions are sAPI debug print functions.
Code that uses the DEBUG* functions will have their I/O routed to
the sAPI DEBUG UART. */
DEBUG_PRINT_ENABLE;

   	   /* ------------- INICIALIZACIONES ------------- */

   	   /* Inicializar la placa */
   	   boardConfig();

   	   /* Inicializar el conteo de Ticks con resolucion de 1ms (se ejecuta
   	      periodicamente una interrupcion cada 1ms que incrementa un contador de
   	      Ticks obteniendose una base de tiempos). */
   	   tickConfig(  TICKRATE_MS	 );

   	   /* Se agrega ademas un "tick hook" nombrado myTickHook. El tick hook es
   	      simplemente una funcion que se ejecutara peri�odicamente con cada
   	      interrupcion de Tick, este nombre se refiere a una funcion "enganchada"
   	      a una interrupcion.
   	      El segundo parametro es el parametro que recibe la funcion myTickHook
   	      al ejecutarse. En este ejemplo se utiliza para pasarle el led a titilar.
   	   */

   	   debugPrintConfigUart( UART_USB, 115200);
   	   debugPrintString( "DEBUG c/sAPI\r\n" );

      	tickCallbackSet( myTickHook, (void*)NULL );
#define LED_NUMBER (6)
      	char LED_STRINGS[LED_NUMBER][20] = {
      			"LED_B Toggle \n",
				"LED_G Toggle \n",
				"LED_R Toggle \n",
				"LED_1 Toggle \n",
				"LED_2 Toggle \n",
				"LED_3 Toggle \n"
      	};
      	uint32_t i = 0; //no estoy seguro si conviene usar otro tipo de dato para esto
      	gpioMap_t current_LED = LEDB;
      	bool LED_ON = false;
      	/* ------------- REPETIR POR SIEMPRE ------------- */
      	while(1) {
      		__WFI();

      		if (BUTTON_Time_Flag == true) {
				BUTTON_Time_Flag = false;
				if(BUTTON_Status_Flag == true && toggled == false) {
					BUTTON_Status_Flag = false;

					if (BUTTON_Status_Counter == 0) {
						BUTTON_Status_Counter = BUTTON_STATUS_MS;
						gpioToggle(current_LED);
						debugPrintString(LED_STRINGS[i]);
						toggled = true;
						if(LED_ON) {
							i++;
							switch(current_LED) {
								case LEDB:
									current_LED = LEDG;
									break;
								case LEDG:
									current_LED = LEDR;
									break;
								case LEDR:
									current_LED = LED1;
									break;
								case LED1:
									current_LED = LED2;
									break;
								case LED2:
									current_LED = LED3;
									break;
								case LED3:
									current_LED = LEDB;
									i = 0;
									break;
							}
							LED_ON = false;
						}
						else
							LED_ON = true;
					}
					else
						BUTTON_Status_Counter--;
				}
      		}
      	}

      	/* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
             por ningun S.O. */
      	return 0 ;

#else
#error "NO TEST SET"
#endif

}

/*==================[end of file]============================================*/
