/*
 * dataStruct.h
 *
 *  Created on: 04.08.2014
 *      Author: rensky
 */

#ifndef DATASTRUCT_H_
   #define DATASTRUCT_H_


struct valuePack {
	char chanel;
	double valuesAsSumm;	//Speichert die Summe aller Werte
	int numberOfValues;		//Anzahl der bisherigen empfangenen Werte
	int impulsConst;		//Impulsconstante,....
	long lastTs;			//Der Timestamp des letzten Empfangenen wertes.

};


#endif /* DATASTRUCT_H_ */
