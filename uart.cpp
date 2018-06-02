/*
 * Dateiname:    uart.cpp
 *
 * Author:       Dogan Alkan
 *
 * Copyrighth:   (C) 2015 All Rights Reserved By Dogan Alkan.
 * Beschreibung: Diese Datei bzw. die Klasse BuswareS0 dient dazu die Daten, die vom
 * 				 Aufsatz (Cape) Busware SD0 über den UART kommen, zu Empfangen. Die Empfangenen
 * 				 Daten werden dann den jeweiligen Chanels, also den jewiligen S0-Anschlüssen des
 * 				 Aufsatzes aufgeteilt.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h>
#include <sys/time.h>
#include "uart.h"

#define VALUES_ANZAHL 4

uart::uart() {
	// TODO Auto-generated constructor stub
	running = false;
	uart0_filestream = -1;
	sem_init(&sem_data, 0, 1);

}

uart::~uart() {
	// TODO Auto-generated destructor stub
}

int uart::start(const char *device, Value *value){
	// OPEN THE UART
	// The flags (defined in fcntl.h):
	// Access modes (use 1 of these):
	// O_RDONLY - Open for reading only.
	// O_RDWR - Open for reading and writing.
	// O_WRONLY - Open for writing only.
	//
	// O_NDELAY / O_NONBLOCK (same function) - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
	// if there is no input immediately available (instead of blocking). Likewise, write requests can also return
	// immediately with a failure status if the output can't be written immediately.
	//
	// O_NOCTTY - When set and path identifies a terminal device,
	// open() shall not cause the terminal device to become the controlling terminal for the process.
	uart0_filestream = open(device, O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	if (uart0_filestream == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
	}

	// CONFIGURE THE UART
	// The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
	// Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400,
        //		B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
	// CSIZE:- CS5, CS6, CS7, CS8
	// CLOCAL - Ignore modem status lines
	// CREAD - Enable receiver
	// IGNPAR = Ignore characters with parity errors
	// ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
	// PARENB - Parity enable
	// PARODD - Odd parity (else even)
	struct termios options;
	tcgetattr(uart0_filestream, &options);
	options.c_cflag = B38400 | CS8 | CLOCAL | CREAD; //<Set baud rate
	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart0_filestream, TCIFLUSH);
	tcsetattr(uart0_filestream, TCSANOW, &options);

	this->device = device;
	running = true;

	if (pthread_create(&runningThread, NULL, uart::callRunFunction, this) != 0) {
		printf("Running Thread can not be create.");
	} else {
		printf("Busware Thread gestartet.");
	}
	this->value = value;

	return true;
}
/**
 * Stopt die verbindung über die vrbindung.
 */
void uart::stop(void) {
	running = false;
}
/**
 *
 * Konvertiert S0-Impulse in Watt pro stunde
 * Converts the S0-Impuls to Watt per hour and saves the valueBach of the electricity.
 *
 * Quelle: [s0ec]
 */
void uart::update_average_values(Value *value) {
	unsigned long ts = unixtime();
	int timestamp = 0;
	double wattProImpuls = 0;
	double tmp_value = 0;

	if (value->getLastTimestamp() != 0) {
		sem_wait(&sem_data);

		timestamp = (int) (ts - value->getLastTimestamp());   //Berechnen der Zeitdifferenz zum letzten timestamp
		wattProImpuls = 1000.0 / (double) value->getImpulsConstant();   //Errechnet den Verbrauch anhand der Impulskonstante, die in der Regel auf dem Stromz�hler steht

		tmp_value = wattProImpuls * (3.6 / (double) timestamp) * 1000000.0; // Zeit in MS
		value->getValuesAsSum() += tmp_value / 1000.0;
		value->setNumberOfValues(value->getNumberOfValues()+1);

		sem_post(&sem_data);
		printf("Summe: %.3f Anzahl %d TMPValue: %.3f Zeit: %d ms \n",
				value->getValuesAsSum(), value->getNumberOfValues(), tmp_value, timestamp);
	}
	value->setLastTimestamp(ts);
}
/**
 * This function receive data over the UART-Interface.
 * (Diese Funktion ist dazu Konzipiert, in einem Thread zu laufen)
 *
 * @param Ein zeiger auf die "Umschlie�ende" Klasse.
 */
void* uart::receive(void *This){
	//variable
	unsigned char rx_buffer[255];		//buffer for received data - zum buffern der Über UART empfangegen Daten.
	unsigned char proto_buffer[255];	//the content of buffer are transmitt in this variable - hier werden die Daten abgelegt, die über den UART empfangen wurden.
	int position = 0;		//wir als index, zum ladender über UART empfangenen Daten benutzt(für Protobuffer)
	int rx_length = 0;	//für die länge der empfangenen Daten.

	//Fehlerbehandlung, falls über open, keine Verbindung aufgebaut werden konnte
	if (uart0_filestream != -1)
	{
		printf("\nOpen %s. \n", device);
	}

	if (((uart *)This)->uart0_filestream > -1)
	{


		while (((uart *)This)->running)
		{
			//Daten die empfangen werden sollten sind folgende:
			// A 13 10  <-- ASCII Zeichen
			// 0  1  2 	<-- Index/Reihenfolge
			rx_length = read(((uart *)This)->uart0_filestream, (void*)rx_buffer, 255); //Filestream, buffer to store in, number of bytes to read (max)

			if (rx_length < 0)
			{
				//An error occured (will occur if there are no bytes)
				printf("Error! BuswareS0.cpp: Keine Daten über den UART Empfangen");
			} else if (rx_length == 0)
			{
				//No data waiting
			   printf("buswareS0.cpp: Data! No data recived! The Cape are sending no data.");
			} else //Daten was recived (rx_length > 0)
			{
			   position = 0;

				for (int i=0; i < rx_length; i++)
				{
					printf("Buffer mit dem Index %i: %.2x %s", i, rx_buffer[i], rx_buffer);
					printf("Inhalt des Gesammten Buffers %s", rx_buffer);
					proto_buffer[position] = rx_buffer[i];
					position++;
				}
				proto_buffer[4] = 0;
				printf("\n");

				//handle with received Data
				// check mal proto_buffer stelle 1 und 2 ab ist nda 10 und 13

				//Hier muss jetzt irgendwas hin was die chanels A, B, C, D erkennt und umrechnet
				//Es muss noch überprüft werden, ob nur ein einzelnes Buchstabe und "Windows"Zeilenende
				//oder auch andere andere Buchstaben oder Zeichen vorhanden sind.

				if(proto_buffer[0] == 'A' && proto_buffer[0] == '\r' && proto_buffer[0] == '\n'){			//Erster Chanel (A)
					printf("A Chanel else-schleife, %s", proto_buffer);
					update_average_values(&this->value[0]);
				}else if(proto_buffer[0] == 'B' && proto_buffer[0] == '\r' && proto_buffer[0] == '\n'){	//zweiter Chanel (B)
					printf("B Chanel else-schleife, %s", proto_buffer);
					update_average_values(&this->value[1]);
				}else if(proto_buffer[0] == 'C' && proto_buffer[0] == '\r' && proto_buffer[0] == '\n'){	//dritter Chanel (C)
					printf("C Chanel else-schleife, %s", proto_buffer);
					update_average_values(&this->value[2]);
				}else if(proto_buffer[0] == 'D' && proto_buffer[0] == '\r' && proto_buffer[0] == '\n'){	//Vierter Chanel (D)
					printf("D Chanel else-schleife, %s", proto_buffer);
					update_average_values(&this->value[3]);
				}else{
					printf("BuswareS0.cpp: Function: run(void *This): Received Value! The over UART received values are invalid. Received values:\n %s", proto_buffer);
				}
			}
					sleep(1);
		}
	}
	close(((uart *)This)->uart0_filestream);
}
/**
 * over this function received this clas the struct for data
 */
//int BuswareS0::addSensors(valuePack *values) {
//
//	// sensoren Struktur intern abspeichern als zeiger
//	this->values = values;
//
//	return 0;
//}

/**
* Calculates the timestamp after Unixtime.
* Quelle: s0enow.cpp
*/
unsigned long long uart::unixtime() {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	unsigned long long ms_timestamp = (unsigned long long) (tv.tv_sec) * 1000
			+ (unsigned long long) (tv.tv_usec) / 1000;

	return ms_timestamp;
}
