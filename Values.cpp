/*
 *    Dateiname: Values.cpp
 *
 *       Author: Dogan Alkan
 * Matrikel-Nr.: s772991
 * BeschreibungHier werden die Funktionen der Klasse Values deklariert.
 */

#include "Values.h"

Value::Value() {
	// TODO Auto-generated constructor stub

}

Value::~Value() {
	// TODO Auto-generated destructor stub
}
/**
 * Summiert den Aktuellen Wert mit den bisheigen Werten.
 *
 * @param Wert -> double
 */
void Value::setValue(double values){
	this->valuesAsSumm = values;
}
/**
 * Gibt die bisherige gesammtsumme zurück.
 *
 * @return Gesammtsumme -> double;
 */
double Value::getValuesAsSum(){
	return this->valuesAsSumm;
}
/**
 * Setzt die Anzahl der Werte zurück.
 *
 * @param Nummer -> anzahl der bisherigen Werte;
 */
int Value::setNumberOfValues(int number){
	this->numberOfValues = Value;
}
/**
 * Gibt die Anzahl der bisherigen Werte zurück.
 *
 * @param Nummer -> anzahl der bisherigen Werte;
 */
int Value::getNumberOfValues(){
	return this->numberOfValues;
}
/**
 * Setzt die Impulskonstante, für einen Zähler.
 *
 * @param impulsConst -> Impulskonstante für einen bestimmten Zähler.
 */
void Value::setImpulsConstant(int impulsConst){
	this->impulsConstant = impulsConst;
}
/**
 * Gibt die Impulskonstante, für einen Zähler zurück.
 *
 * @return Impulskonstante eines Zählers.
 */
int Value::getImpulsConstant(){
	return this->impulsConstant;
}
/**
 * Setzt den Timestamp, also Zeitstempel (aktuelle Zeit).
 *
 * @param lastTs -> Zeitstempel
 */
void Value::setLastTimestamp(long lastTs){
	this->lastTimestamp = lastTs;
}
/**
 * Gibt den letzten gesetzten Zeitstempel zurück.
 *
 * @return Impulskonstante eines Zählers.
 */
long Value::getLastTimestamp(){
	return this->lastTimestamp;
}

