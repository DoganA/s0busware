/*
 *    Dateiname: Value.h
 *
 *       Author: Dogan Alkan
 * Matrikel-Nr.: s772991
 * Beschreibung: Es wurde im Rahmen der Bachelorarbeit erstellt.
 *               Diese Headerdatei dient als Klassendefinition der "Datenklasse".
 */
#include <map>
#include <iterator>

#ifndef VALUES_H_
#define VALUES_H_

class Value {
	private:
		double valuesAsSumm;	//Speichert die Summe aller Werte
		int numberOfValues;		//Anzahl der bisherigen empfangenen Werte
		int impulsConstant;		//Impulsconstante,....
		long lastTimestamp;
		std::map<long, double> consumption;


	public:
		Value();
		virtual ~Value();

		void setValuesAsSum(double values);	//Speichert die Summe aller Werte
		double getValuesAsSum();
		int setNumberOfValues(int number);
		int getNumberOfValues();		//Anzahl der bisherigen empfangenen Werte
		void setImpulsConstant(int impulsConst);
		int getImpulsConstant();
		void setLastTimestamp(long lastTs);
		long getLastTimestamp();
};

#endif /* VALUES_H_ */
