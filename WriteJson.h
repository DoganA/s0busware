/*
 *    Dateiname: WriteJson.h
 *
 *       Author: Dogan Alkan
 * Matrikel-Nr.: s772991
 * Beschreibung: Es wurde im Rahmen der Bachelorarbeit erstellt.
 *               Diese Headerdatei dient als Klassendefinition für den Jsonparser.
 */
#include <string>
#include <jsoncpp/json/json.h>
#include <string>
#include <syslog.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>

#include "Values.h"

#ifndef WRITEJSON_H_
#define WRITEJSON_H_

class WriteJson {
	private:
		Json::Value getValue(Value *value);
		int writeFile(std::string filename, Json::Value root);
	public:
		WriteJson();
		virtual ~WriteJson();

		Json::Value setJson(std::string path, Value *value);
};

#endif /* WRITEJSON_H_ */
