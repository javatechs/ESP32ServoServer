#include "Arduino.h"
#include <ESP32Servo.h>
#include <SimplePacketComs.h>
#include <WiFi.h>
#include <PacketEvent.h>
#include <Esp32SimplePacketComs.h>
#include <Preferences.h>
#include <wifi/WifiManager.h>
#include <server/NameCheckerServer.h>
#include "src/ServoServer.h"
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include "src/GetIMU.h"
// SImple packet coms implementation using WiFi
UDPSimplePacket coms;
// WIfi stack management state machine
WifiManager manager;
//The setup function is called once at startup of the sketch
String * name = new String("hidDevice");
Adafruit_BNO055 bno;
GetIMU * sensor;
bool imuWorking = false;
void setup()
{
	manager.setupScan();
	sensor = new GetIMU();
	Serial.println("Loading with name: "+name[0]);

	//Initialize the sensor
	if (bno.begin()) {
		delay(1000);
		bno.setExtCrystalUse(true);
		sensor->startSensor(&bno);
		imuWorking = true;
	}else{
		Serial.print(
				"Oops, no BNO055 detected ... Check your wiring or I2C ADDR!");
	}

	coms.attach(new NameCheckerServer(name));
	if (imuWorking) {
    coms.attach(sensor);
	}
	coms.attach(new ServoServer());
}

// The loop function is called in an endless loop
void loop()
{
	manager.loop();
	coms.server();
}
