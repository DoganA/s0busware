/*
 *    Dateiname: WriteCSV.h
 *
 *       Author: Dogan Alkan
 * Matrikel-Nr.: s772991
 * Beschreibung: Es wurde im Rahmen der Bachelorarbeit erstellt.
 *               Diese Headerdatei dient als Klassendefinition um csv Dateien zu beschreiben.
 */
#include <string>
#include <syslog.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>

#ifndef SRC_WRITECSV_H_
#define SRC_WRITECSV_H_

class WriteCSV {
	private:
        std::string mPath;
        static int appendToFile(const char *filename, std::string values);

	public:
		WriteCSV();
		WriteCSV(std::string path);

		virtual ~WriteCSV();


		static int setCSV(std::string path, std::string values);
};

#endif /* SRC_WRITECSV_H_ */
