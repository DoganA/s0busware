/*
 *    Dateiname: uart.h
 *
 *       Author: Dogan Alkan
 * Matrikel-Nr.: s772991
 * Beschreibung: Es wurde im Rahmen der Bachelorarbeit erstellt.
 *               Diese Headerdatei dient als Klassendefinition für die Uartverbindung.
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include "Values.h"

#ifndef UART_H
   #define UART_H

   #define TERM_SPEED B38400


class uart {
	public:
			uart();
			virtual ~uart();

			int start(const char *device, Value *value);
			void stop(void);
			sem_t sem_data;

	private:
			static void* callRunFunction(void *arg) { return ((uart*)arg)->receive(arg); } //Funktion übernommen aus [s0ec]
			void* receive(void *This);
			unsigned long long unixtime();
			void update_average_values(Value *value);

			int uart0_filestream;
			bool running;
			pthread_t runningThread;
			const char *device;
			Value *value;
};

#endif /* UART_H_ */
