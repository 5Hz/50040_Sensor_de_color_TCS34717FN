#include <Wire.h>


//definicion de registros del sensor *******************************************************
#define colorsin           0x29   //direccion del sensor de color
//      -Nombre-        -direcciÃ³n-
#define ENABLE             0x80   // R/W
#define ATIME              0x81   // R/W
#define WTIME              0x83   // R/W
#define AILTL              0x84   // R/W
#define AILTH              0x85   // R/W
#define AIHTL              0x86   // R/W
#define AIHTH              0x87   // R/W
#define PERS               0x8C   // R/W
#define CONFIG             0x8D   // R/W
#define CONTROL            0x8F   // R/W
#define ID                 0x92   // R/W
#define STATUS             0x93   // R
#define CDATA              0x94   // R
#define CDATAH             0x95   // R
#define RDATA              0x96   // R
#define RDATAH             0x97   // R
#define GDATA              0x98   // R
#define GDATAH             0x99   // R
#define BDATA              0x9A   // R
#define BDATAH             0x9B   // R


byte receivedVal = 0x00;

unsigned int RedLow = 0;
unsigned int RedHigh = 0;
unsigned int GreenLow = 0;
unsigned int GreenHigh = 0;
unsigned int BLow = 0;
unsigned int BHigh = 0;
unsigned int CLow = 0;
unsigned int CHigh = 0;

  byte leeRegistro(int _address_, int _register_);
  int concatenate(byte _highByte_, byte _lowByte_);
  void escribeRegistro(int _address_, int _register_, byte _value_);
  
void setup()
{

Wire.begin();

Serial.begin(9600);
Serial.write("Convertidor de luz a digital" "\n");

escribeRegistro(colorsin, ENABLE, 0x03);
Wire.beginTransmission(colorsin);
Wire.write(0x80);
Wire.write(0x03); 
Wire.endTransmission();

Wire.beginTransmission(colorsin); 
Wire.requestFrom(0x29,1);
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

void loop() 
{
Wire.beginTransmission(colorsin);
Wire.write(0x96);
Wire.endTransmission();

Wire.beginTransmission(colorsin); 
Wire.requestFrom(colorsin,1);
RedLow = Wire.read();
Wire.endTransmission();

Wire.beginTransmission(colorsin);
Wire.write(0x97);
Wire.endTransmission();

Wire.beginTransmission(colorsin); 
Wire.requestFrom(colorsin,1);
RedHigh = Wire.read();
Wire.endTransmission();


RedHigh = (RedHigh * 256) + RedLow;

Serial.print(RedHigh, DEC);


Wire.beginTransmission(0x29);
Wire.write(0x9A);
Wire.endTransmission();

Wire.beginTransmission(0x29); 
Wire.requestFrom(0x29,1);
GreenLow = Wire.read();
Wire.endTransmission();

Wire.beginTransmission(0x29);
Wire.write(0x9B);
Wire.endTransmission();

Wire.beginTransmission(0x29); 
Wire.requestFrom(0x29,1);
GreenHigh = Wire.read();
Wire.endTransmission();


GreenHigh = (GreenHigh * 256) + GreenLow;
Serial.print("            ");
Serial.println(GreenHigh, DEC);
delay(500);
}

//funciones comunicacion I2C *************************************************************

//lee registro de sensor por protocolo I2C ***********************************************
//requiere direccion del sensor y registro a leer ****************************************
byte leeRegistro(int _address_, int _register_)
{
  Wire.beginTransmission(_address_); //inicia comunicacion con sensor elegido
  Wire.write(_register_);            //escribe el registro del cual solicitas la informacion
  Wire.endTransmission();            //termina la transmision
  Wire.requestFrom(_address_, 1);    //solicita 1 byte del sensor elegido (previamente elegido)
  do{}while(Wire.available() < 1);   //espera a recibir la informacion del registro
  byte valorRegistro = Wire.read();  //guarda valor recibido
  return valorRegistro;              //regresa el valor al programa principal
}

//escribe sobre un registro del sensor por protocolo I2C **********************************
//requiere direccion del sensor, registro a escribir, y valor a escribir en el ************
void escribeRegistro(int _address_, int _register_, byte _value_)
{
  Wire.beginTransmission(_address_); //inicia comunicacion con el sensor seleccionado
  Wire.write(_register_); //selecciona el registro al cual leer/escribir
  Wire.write(_value_); //modo de medicion continua
  Serial.print("estado de transmision: "); Serial.println(Wire.endTransmission()); //termina la transmision
  //e imprime en el serial su estado (exitosa o no)
}

//concatena 2 bytes para convertirlos en una palabra (tipo de dato integer) ***************
int concatenate(byte _highByte_, byte _lowByte_)
{
  int _integer_ = (_highByte_ <<8) | _lowByte_; //crea un entero a partir de sus 2 bytes (MSB y LSB)
  return _integer_;
}

