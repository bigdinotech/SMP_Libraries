/**
SMP Arduino Library
Dino Tinitigan
**/

#ifndef SMP_h
#define SMP_h

#include <Arduino.h>

class SMP
{
private:
	byte ID;
	byte header;
	Stream&    _port;
	int bPin;
	int sPin;
	int priority;
	int waitDelay;
public:
	SMP(byte modID, Stream& port, int bPin, int sPin);
	SMP(byte modID, Stream& port, int bPin, int sPin, int modPriority);
public:
	inline Stream& port	() const {return _port;}
	void sendData(byte packet[]);	
	int readData(byte packet[]);
	void setID(byte modID);
	void setPriority(int priority);
	
};

#endif