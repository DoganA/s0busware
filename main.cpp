/******************************************************************************
 * Dateiname: 	 main.cpp
 * Erstellt am:  09.03.2015
 *       Author: 		 Dogan Alkan
 * Beschreibung: Diese Fatei dient als Hauptdatei, für den C++ Code
 *               Referenzen der, von anderen Projekten Übernommenen funktionen
 *               stehen im Kommentarblock, der jeweiligen Funktionen.
 ******************************************************************************
 * Copyright (C) 2015  Dogan Alkan (d.alk@hotmail.de)
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <sys/stat.h>           /* declares the stat() functions; umask */
#include <sys/time.h>
#include <syslog.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <jsoncpp/json/json.h>

#include "DataStruct.h"
#include "values.h"
#include "Konfiguration.h"
#include "WriteCSV.h"
#include "Deamon.h"
#include "uart.h"

#define ENTWICKLUNG


//#define BUSWARE_DEVICE "/dev/ttyS1"
#define UART_DEVICE "/dev/ttyAMA0"   //Schnitttelle, von der eingelesen werden soll (UART)
#define NUMBER_INPUTS 4				 //Anzahl der Zähler/Chanels
#define DAEMON_NAME "bachelorarbeit"
#define BUF_LEN 64


using namespace std;


Konfiguration config;
Value *value;
uart *uartS0;
char str[120];

sem_t sem_averrage;


//Funktionsprototypen
//int appendToFile(const char *filename, char *str);
void logPrint(char *msg, unsigned int level);

/**
 * Funktion berechnet den Mittelwert und speichert die Mittelwerte ab.
 * Quelle [vgl. s0ec]
 */
void *intervallFunction(void *time) { // Der Type ist wichtig: void* als Parameter und R��ckgabe
	int ttime = *((int*) time);
	double averrage[NUMBER_INPUTS];   //durchschnitswert für unterschiedliche Temperaturen
	char str[200];
	str[0] = '\0';

	printf("Thread wird gestartet\n");

	while(true)
	{
		string saveFolder = "";
		string Datafolder = config.getDatafolder();
		string Messstellenname = config.getMessstelle();

		sleep(ttime);

		//Mittelwert der Daten berechnen
        sem_wait(&(uartS0->sem_data));
		for (int i = 0; i < NUMBER_INPUTS; i++) {
            if (value[i].getNumberOfValues() > 0) {
                averrage[i] = value[i].getValuesAsSum() / value[i].getNumberOfValues(); //Mittelwert bilden
			} else {
				averrage[i] = 0;
			}
            value[i].setNumberOfValues(0);
            value[i].setNumberOfValues(0);

			//String bilden, um einer CSV Datei einzufügen.
			sprintf(str, "%s%.3f;", str, averrage[i]);
		}
        sem_post(&(uartS0->sem_data));

		//Daten in eine .csv Datei schreiben
		sprintf(str, "%s", str);

		size_t len = strlen(str);

		if(len>0){
		  str[len-1] = '\0';
		}

		saveFolder.append(Datafolder);
		saveFolder.append("/");
		saveFolder.append(Messstellenname);
		if (WriteCSV::setCSV(saveFolder.c_str(), str) != 0) {
			printf("Can not append to File %s.",
					"filename_noch_nicht_vergeben");
		}
		str[0] = '\0';
	}
	printf("Thread wird beendet\n");

	return NULL;  // oder in C++: return 0;// Damit kann man Werte zur��ckgeben
}

int main(int argc, const char* argv[]) {
	printf("Program starts....");
    uartS0 = new uart();
    std::stringstream configfile;
    value = (Value*) malloc(sizeof(Value) * (NUMBER_INPUTS));
	pthread_t intervalThread;
	char pid_file[16];
	char buffer[BUF_LEN];
	int mittelwertszeit = 0;

    configfile << "/etc/" << DAEMON_NAME << ".cfg";
    config.setPath(configfile.str());


	 int *impulsKonstanten= config.getImpulswerte();

	setlogmask(LOG_UPTO(LOG_INFO));
	openlog(DAEMON_NAME, LOG_CONS | LOG_PERROR, LOG_USER);
	printf("Programm beginnt....");
	syslog(LOG_INFO, "S0/Impulse logger RaspberryPI deamon %s.%s",
				DAEMON_VERSION, DAEMON_BUILD);


	sprintf(pid_file, "/tmp/%s.pid", DAEMON_NAME);

	daemonize( "/tmp/", pid_file );


	//***************************************************************************
	//Initialisieren aller Werte
	for (int i = 0; i < NUMBER_INPUTS; i++) {
        value[i].setNumberOfValues(0);
        value[i].setValuesAsSum(0);
        value[i].setImpulsConstant(impulsKonstanten[i]);
        value[i].setLastTimestamp(0);
	}

	//***************************************************************************/

    sem_init(&sem_averrage, 0, 1);
	/* Thread erstellen für interval Berechnung*/

	uartS0->start(UART_DEVICE, value);   //RPI
	mittelwertszeit = config.getMittelwertzeit();

    if (pthread_create(&intervalThread, NULL, intervallFunction, (void *) &mittelwertszeit) != 0) {
		printf("Interval: Thread can not be create.");
		exit(1);
	}

	// Dient dazu, dass das Programm nicht beendet wird,
	while (true);



	return 0;

}	//end main

/**
 *
 */
//void logPrint(char *msg, unsigned int level) {
//	char date_time_string[20];
//	struct tm* ptm;
//	gettimeofday(&tv, NULL);
//	ge
//	ptm = localtime(&tv.tv_sec);
//	strftime(date_time_string, sizeof(date_time_string), "%Y-%m-%d %H:%M:%S",ptm);
//	if (level >= LogLevel ){
//		printf("[%s] [%d] %s",date_time_string, level, msg);
//	}
//}
/******************************************************************************
 * Fügt den angegebenen string (str) einer Datei (filename) an, der mit dem
 * gesammten Pfadangabe angegebenen wurde.
 * Bei der Datei handelt es sich um eine .csv (also einer comma separated values Datei)
 *
 * @param:	*filename -> char Zeiger, der die Pfadangabe zu einer Datei beinhaltet.
 * 			*str -> char zeiger der anzuf��genden Daten.
 */
//int appendToFile(const char *filename, char *str) {
//	//Variablen
//	FILE *fd;				//Filedescriptor zur Datei
//	struct stat st = { 0 };	//zum ��berpr��fen, ob die angegebene Datei existiert
//	struct tm* ptm;
//	struct timeval tv;
//	char date_time_string[20];
//	char date_string[11];
//	char filepath[200];
//	char str2[200];
//   umask(0022);
//
//	/* Create directory if not exist*/
//	if(stat(filename, &st) == -1) {
//		mkdir(filename, 0755);
//	}
//
//	/* Filename ermitteln anhand des Datums und Uhrzeit */
//	gettimeofday(&tv, NULL);
//	ptm = localtime(&tv.tv_sec);
//
//	strftime(date_string, sizeof(date_string), "%Y-%m-%d", ptm);
//	strftime(date_time_string, sizeof(date_time_string), "%Y-%m-%d %H:%M:%S", ptm);
//	sprintf(filepath, "%s/%s.csv", filename, date_string);
//	sprintf(str2, "%s;%s\n", date_time_string, str);
//
//	printf("Now will be add to file: %s this string: %s", filepath, str2);
//
//	//��ffnen der Datei
//	fd = fopen(filepath, "a");
//	//Schreiben in die Datei
//	if (fd != NULL) {
//	   	fputs(str2, fd);
//	    fclose(fd);
//	    return 0;
//	} else {
//		syslog( LOG_INFO, "Can not open File  %s. %s.%s",filepath, DAEMON_VERSION, DAEMON_BUILD);
//	}
//	return 1;
//}
/* EOF */
