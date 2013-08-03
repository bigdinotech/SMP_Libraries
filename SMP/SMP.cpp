/**
SMP Arduino Library
Dino Tinitigan
**/

#include "SMP.h"

SMP::SMP(byte modID, Stream& port, int busyPin, int sendingPin)
  : ID(modID), _port(port)
{
	header = 170;
	priority = 200;
	bPin = busyPin;
	sPin = sendingPin;
	pinMode(bPin, INPUT);
	pinMode(sPin, OUTPUT);
	digitalWrite(sPin, HIGH);
}

SMP::SMP(byte modID, Stream& port, int busyPin, int sendingPin, int modPriority)
  : ID(modID), _port(port)
{
	header = 170;
	bPin = busyPin;
	sPin = sendingPin;
	priority = modPriority;
	pinMode(bPin, INPUT);
	pinMode(sPin, OUTPUT);
	digitalWrite(sPin, HIGH);
}

void SMP::sendData(byte packet[])
{
	int test = digitalRead(bPin);
	int timeout = 0;
	bool timedOut = false;
	
	while(test && timeout<100)//busy waiting
	{
		int x = random(priority);
		delay(x);
		test = digitalRead(bPin);
		timeout++;
		timedOut = true;
	}
	
	if(!timedOut)
	{
		digitalWrite(sPin, LOW);			//lock the BUS
		int dataLength = packet[0];
		byte dataChecksum = ID;
		_port.write(header);				//Header Byte
		_port.write(ID);					//ID Byte
		for(int i = 0; i < dataLength +1; i++)
		{
			dataChecksum += packet[i];
			_port.write(packet[i]);			//Length & Data Bytes
		}
		_port.write(dataChecksum);			//Checksum Byte
		_port.flush();
		digitalWrite(sPin, HIGH);			//free the BUS
	}
}

int SMP::readData(byte packet[])
{
	byte modID;
	byte buff;
	byte dataChecksum = 0;
	byte dLength;
	unsigned int tPre;
	unsigned int tWatch;
	if(_port.available() >= 5)
	{
		buff = _port.read();
		if(buff == header)
		{
			modID = _port.read();		//ID Byte
			dataChecksum += modID;
			dLength = _port.read();		//Length Byte
			dataChecksum += dLength;
			packet[0] = dLength;
			tPre = millis();
			for(int i = 0; i < dLength; i++)
			{
				tWatch = millis();
				buff = _port.read();		//Data Bytes
				dataChecksum += buff;
				packet[i +1] = buff;
				if((tWatch - tPre)>100)
				{
					return 0;	//timeout
				}
			}
			if(dataChecksum == _port.read())
			{
				//valid SMP Packet
				return modID;
			}
			else
			{
				//invalid SMP Packet
				return 0;
			}
		}
		else
		{
			//header error, attempt to continue reading more data
			readData(packet);
		}
	}
	else
	{
		return 0;
	}
}

void SMP::setID(byte modID)
{
	ID = modID;
}

void SMP::setPriority(int modPriority)
{
	priority = modPriority;
}
  