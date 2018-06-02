/*
 *    Dateiname: WriteCSV.cpp
 *
 *       Author: Dogan Alkan
 * Matrikel-Nr.: s772991
 * Beschreibung: In dieser Datei werden die Funktionen der Klasse header definiert.
 */

#include "WriteCSV.h"

using namespace std;

WriteCSV::WriteCSV() {
	// TODO Auto-generated constructor stub
}
//-----------------------------------------------------------------------------
WriteCSV::WriteCSV(string path)
//-----------------------------------------------------------------------------
{
	// TODO Auto-generated constructor stub
	this.mPath = path;
}
//-----------------------------------------------------------------------------
WriteCSV::~WriteCSV()
//-----------------------------------------------------------------------------
{
	// TODO Auto-generated destructor stub
}
/**
 * Setzt den path zu einer CSV Datei und schreibt die Daten, die gesetzt werden sollen.
 * @param path -> Pfad zum speicherort.
 * 	    value -> daten die abgespeichert werden sollen.
 * @return 0 -> Falls die Datei abgespeichert werden konnte
 * 		 -1 -> Falls die Daten nicht abgespeichert werden konnten.
 */
//-----------------------------------------------------------------------------
int WriteCSV::setCSV(string path, string values)
//-----------------------------------------------------------------------------
{
	return WriteCSV::appendToFile(path.c_str(), values);
}
/**
 * Schreibt den angegebenen string (str) einer Datei (filename) an, der mit dem
 * Pfadangabe angegebenen wurde.
 *
 * (Anmerkung: entnommen aus dem Projekt s0enow, aus dem Quellcodedatei s0enow.cpp)
 *
 * @param path -> char Zeiger, der die Pfadangabe zu einer Datei beinhaltet.
 * 	    value -> daten die abgespeichert werden sollen.
 * @return 0 -> Falls die Datei abgespeichert werden konnte
 * 		 -1 -> Falls die Daten nicht abgespeichert werden konnten.
 *
 * Quelle: [vgl. s0ec]
 */
//-----------------------------------------------------------------------------
int WriteCSV::appendToFile(const char *filename, string values)
//-----------------------------------------------------------------------------
{
	//Variablen
	FILE *fd;				//Filedescriptor zur Datei
	struct stat st = { 0 };	//zum ��berpr��fen, ob die angegebene Datei existiert
	struct tm* ptm;
	struct timeval tv;
	char date_time_string[20];
	char date_string[11];
	char filepath[200];
	char str2[200];
    umask(0022);

	/* Create directory if not exist*/
	if(stat(filename, &st) == -1) {
		mkdir(filename, 0755);
	}

	/* File-/ Dateiname ermitteln anhand des Datums und Uhrzeit */
	gettimeofday(&tv, NULL);
	ptm = localtime(&tv.tv_sec);
	gettimeofday(&tv, NULL);

	strftime(date_string, sizeof(date_string), "%Y-%m-%d", ptm);
	strftime(date_time_string, sizeof(date_time_string), "%Y-%m-%d %H:%M:%S", ptm);
	sprintf(filepath, "%s/%s.csv", filename, date_string);
	sprintf(str2, "%s;%s\n", date_time_string, values.c_str());

	printf("Now will be add to file: %s this string: %s", filepath, str2);

	//��ffnen der Datei
	fd = fopen(filepath, "a");
	//Schreiben in die Datei
	if (fd != NULL) {
	   	fputs(str2, fd);
	    fclose(fd);
	    return 0;EXI
	} else {
//		syslog( LOG_INFO, "Can not open File  %s. %s.%s",filepath, DAEMON_VERSION, DAEMON_BUILD);
	}
	return -1;
}
