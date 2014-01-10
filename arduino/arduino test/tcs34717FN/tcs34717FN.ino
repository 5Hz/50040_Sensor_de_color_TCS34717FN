#include <Wire.h>


int Direccion =0x29;


byte receivedVal = 0x00;

unsigned int RedLow = 0;
unsigned int RedHigh = 0;
unsigned int GreenLow = 0;
unsigned int GreenHigh = 0;
unsigned int BlueLow = 0;
unsigned int BlueHigh = 0;
unsigned int CLow = 0;
unsigned int CHigh = 0;
void setup()
{

Wire.begin();

Serial.begin(9600);
Serial.write("Convertidor de luz a digital" "\n");


Wire.beginTransmission(Direccion);
Wire.write(0x80);
Wire.write(0x03); 
Wire.endTransmission();

Wire.beginTransmission(Direccion); 
Wire.requestFrom(Direccion,1);
receivedVal = Wire.read();
Wire.endTransmission();

if (receivedVal == 0x03) {
Serial.write("ADC" "\n");
}
else {
Serial.write("la conexion al sensor fallo: ");
Serial.println(receivedVal, DEC);
}

delay(50); 

}

void loop() {
Wire.beginTransmission(Direccion);
Wire.write(0x96);
Wire.endTransmission();

Wire.beginTransmission(Direccion); 
Wire.requestFrom(Direccion,1);
RedLow = Wire.read();
Wire.endTransmission();

Wire.beginTransmission(Direccion);
Wire.write(0x97);
Wire.endTransmission();

Wire.beginTransmission(Direccion); 
Wire.requestFrom(Direccion,1);
RedHigh = Wire.read();
Wire.endTransmission();


RedHigh = (RedHigh * 256) + RedLow;

Serial.print(RedHigh, DEC);


Wire.beginTransmission(Direccion);
Wire.write(0x98);
Wire.endTransmission();

Wire.beginTransmission(Direccion); 
Wire.requestFrom(Direccion,1);
GreenLow = Wire.read();
Wire.endTransmission();

Wire.beginTransmission(Direccion);
Wire.write(0x99);
Wire.endTransmission();

Wire.beginTransmission(Direccion); 
Wire.requestFrom(Direccion,1);
GreenHigh = Wire.read();
Wire.endTransmission();
GreenHigh = (GreenHigh * 256) + GreenLow;

Serial.print("            ");
Serial.print    (GreenHigh, DEC);

Wire.beginTransmission(Direccion);
Wire.write(0x9A);
Wire.endTransmission();

Wire.beginTransmission(Direccion); 
Wire.requestFrom(Direccion,1);
BlueLow = Wire.read();
Wire.endTransmission();

Wire.beginTransmission(Direccion);
Wire.write(0x9B);
Wire.endTransmission();

Wire.beginTransmission(Direccion); 
Wire.requestFrom(Direccion,1);
BlueHigh = Wire.read();
Wire.endTransmission();


BlueHigh = (BlueHigh * 256) + BlueLow;
Serial.print("            ");
Serial.println(BlueHigh, DEC);
delay(500);
}
