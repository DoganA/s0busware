/*
 *   File name:   Konfiguration.h

 *       Author:  Dogan Alkan
 * Matrikel-Nr.: s772991
 *  Description: Liest Konfigurationsdateien ein und gibt die für die Verarbeitung
 * 				 und Speicherung der Daten notwendigen Konfigurationseinstellungen
 * 				 zururück.
 */
#include <libconfig.h++>
#include <iostream>
#include <syslog.h>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <sstream>

using namespace std;
using namespace libconfig;

#ifndef KONFIGURATION_H_
   #define KONFIGURATION_H_

class Konfiguration {
	private:
		Config cfg;

	public:
		Konfiguration();
		virtual ~Konfiguration();
		int setPath(std::string path);
		std::string getDatafolder();
		std::string getMessstelle();
		int getMittelwertzeit();
		int * getImpulswerte();
};

#endif /* KONFIGURATION_H_ */
