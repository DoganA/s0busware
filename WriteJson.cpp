/*
 *    Dateiname: WriteJson.h
 *
 *       Author: Dogan Alkan
 * Matrikel-Nr.: s772991
 * Beschreibung: Diese Klasse beschreibt JSON-Dateien.
 */
#include <sys/time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include "WriteJson.h"
#include "Values.h"

using namespace std;
using namespace Json;

WriteJson::WriteJson() {
	// TODO Auto-generated constructor stub

}

WriteJson::~WriteJson() {
	// TODO Auto-generated destructor stub
}
/******************************************************************************
 * Schreibt eine Json struktur.
 *
 *  @param path -> Pfad zum Speicherort.
 * 	       timestamp -> Zeitstempel für die Daten.
 * 	       value[] -> array mit daten, die in der Json Struktur abgelegt werden sollen.
 *****************************************************************************/
Json::Value WriteJson::setJson(string path, Value *value){
	Json::Value root, val;
	val["value"]= getValue(value);
	root["values"] = val;


	this->writeFile(path, root);

	return root;
}
/**
 * Gibt den einen einzelnen Json Objekt für einen timestamp.
 *
 * return values-> json Objekt.
 */
Json::Value WriteJson::getValue(valuePack *value){
	Json::Value values;
	char channel = 'A';
	value["timestamp"] = value->lastTs;//vp.lastTS


	for(int x = 0; x <=4; x++){
		value["channel"][channel] = "";//vp.valuesAsSumm
		value[x].impulsConst;

		channel++;
	}

	return values;
}
/**
 * Schreibt die ihr übergebene Json-Struktur in eine Datei.
 * @return 0 -> Wenn Datei beschrieben wurde.
 */
int WriteJson::writeFile(string filename, Json::Value root){
	//Variablen
	std::ofstream fileWriter;   //zum öffnen einer Datei.
	Json::FastWriter writer;	//zum erstellen von Json Documenten

	//Variable
	FILE *fd;	//zum öffnen der Datei
	struct stat st = { 0 };
	struct tm* ptm;
	char date_time_string[20];
	char date_string[11];
	char filepath[200];
	char str2[200];
	umask(0022);

	/* Create directory if not exist*/
	if (stat(filename.c_str(), &st) == -1) {
		mkdir(filename.c_str(), 0755);
	}
	/* Filename ermitteln anhand des Datums */
	gettimeofday(&tv, NULL);
	ptm = localtime(&tv.tv_sec);
	strftime(date_string, sizeof(date_string), "%Y-%m-%d", ptm);
	strftime(date_time_string, sizeof(date_time_string), "%Y-%m-%d %H:%M:%S", ptm);
	sprintf(filepath, "%s/%s.json", filename, date_string);
	sprintf(str2, "%s;%s\n", date_time_string, values.c_str());

	//Öffnen der Datei, in die geschrieben werden soll.
	fileWriter.open(filename.c_str(), std::ofstream::in | std::ofstream::app);

	//Schreiben der Json Datei.
	fileWriter << filename.write(root);

	//Schliesen des Streams,zum Datei beschreiben
	fileWriter.close();

	return 0;
}
