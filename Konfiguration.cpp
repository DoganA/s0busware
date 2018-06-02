/*
 *    Dateiname: Konfiguration.cpp
 *
 *       Author: Dogan Alkan
 * Matrikel-Nr.: s772991
 * Beschreibung: Diese Klasse Dient als Datenmodell.
 *               Die algorithmen bzw. Bezeichnungen haben ähnlichkeiten zu der cfile Funktion in [s0ec].
 */

#include "Konfiguration.h"

Konfiguration::Konfiguration() {
	// TODO Auto-generated constructor stub
	std::string path = "";
	instance = NULL;
}

Konfiguration::~Konfiguration() {
	// TODO Auto-generated destructor stub
}
/**
 * Setzt den Pfad zur Konfigurationsdatei, aus dem gelesen werden soll.
 *
 * @return Datennordner -> Pfad zur Datenordner
 */
int Konfiguration::setPath(string path){
		try
		{
			cfg.readFile(path.c_str());
		}
		catch(const FileIOException &fioex)
		{
			std::cerr << "I/O error while reading file." << std::endl;

			return -1;
		}
		catch(const ParseException &pex)
		{
		    std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine() << " - " << pex.getError() << std::endl;
			return -1;
		}
		return 0;
}
/**
 * liest den Datenordner aus, in die verbrauchswert abgespeichert werden sollen.
 *
 * @return Datennordner -> Pfad zur Datenordner
 */
string Konfiguration::getDatafolder(){
	string Datafolder = "";
	try
	{
		if (cfg.lookupValue("Datafolder", Datafolder))
		{
			char *tmp = (char*) malloc(sizeof(char) * Datafolder.length()+1);
			memcpy(tmp,Datafolder.c_str(),sizeof(char) * Datafolder.length()+1);
			Datafolder = tmp;
			syslog(LOG_INFO, "Datafolder:%s", Datafolder.c_str());

			return Datafolder;
		}
	}
	catch(const SettingNotFoundException &nfex)
	{
		cerr << "No Datafolder setting in configuration file." << endl;
	}
	return Datafolder;
}
/**
 * Liefert den Messstellennamen.
 *
 * @return Messstellenname -> string
 */
string Konfiguration::getMessstelle(){
	string Messstellenname = "";
	try
		{
			if (cfg.lookupValue("Messstelle",Messstellenname))
			{
				char *tmp = (char*) malloc(sizeof(char) * Messstellenname.length()+1);
				memcpy(tmp,Messstellenname.c_str(),sizeof(char) * Messstellenname.length()+1);

				Messstellenname = tmp;
				syslog(LOG_INFO, "Messstelle:%s", Messstellenname.c_str());

				return Messstellenname;
			}
		}
		catch(const SettingNotFoundException &nfex)
		{
			cerr << "No Messstelle setting in configuration file." << endl;
		}
		return NULL;
}
/*
 * Liefert die Zeitraum, in dem der Mittelwert gebildet werden soll.
 *
 * @return Mittelwertzeit -> int bzw. Ganze zahl
 */
int Konfiguration::getMittelwertzeit(){
	int Mittelwertzeit = 0;
	try
	{
		cfg.lookupValue("Mittelwertzeit", Mittelwertzeit);
		syslog(LOG_INFO, "Mittelwertzeit:%d", Mittelwertzeit);

		return Mittelwertzeit;
	}
	catch(const SettingNotFoundException &nfex)
	{
		cerr << "No Mittelwertzeit setting in configuration file." << endl;
	}
	return -1;
}
/**
 * Liest die Impulskonstanten f��r die einzelnen Kanaelen bzw. Zaehler ein.
 *
 * @return Impulskonstanten -> Array von ganzzhalen
 */
int * Konfiguration::getImpulswerte(){
	//Variablen

	stringstream name;
	int impulswerte[4];

	for (int i = 0; i < 4; i++) {


		name.str("");
		name << "Kanal" << i;
		try
		{
			cfg.lookupValue(name.str(), impulswerte[i]);
			syslog( LOG_INFO, "%s = %d", name.str().c_str(), impulswerte[i]);
		}
		catch(const SettingNotFoundException &nfex)
		{
			cerr << "No " << name.str() << " setting in configuration file." << endl << std::flush;
		}
}
}
/** EOF */
