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
public:
	SMP(byte modID, Stream& port, int bPin, int sPin);

public:
	inline Stream& port	() const {return _port;}
	void setID(byte modID);
	void sendData(byte packet[]);
	int getData(byte packet[]);
	
};

#endif