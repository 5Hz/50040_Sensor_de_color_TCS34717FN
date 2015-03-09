#include <Wire.h>

//definicion de registros del sensor *******************************************************
#define colorsin           0x29   //direccion del sensor de color
//      -Nombre-        -direccion-
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
//----------------------------------------------------------------------------------------------------------
//Prototipos de funciones

byte leeRegistro(int _address_, int _register_);
unsigned int concatenar(byte _highByte_, byte _lowByte_);
void escribeRegistro(int _address_, int _register_, byte _value_);
//----------------------------------------------------------------------------------------------------------

//funciones comunicacion I2C *******************************************************************************

//lee registro de sensor por protocolo I2C
//requiere direccion del sensor y registro a leer
byte leeRegistro(int _address_, int _register_)
{
  Wire.beginTransmission(_address_); //inicia comunicacion con sensor elegido
  Wire.write(_register_);            //escribe el registro del cual solicitas la informacion
  Wire.endTransmission();            //termina la transmision
  
  Wire.beginTransmission(_address_);
  Wire.requestFrom(_address_, 1);    //solicita 1 byte del sensor elegido (previamente elegido)
  byte valorRegistro = Wire.read();  //guarda valor recibido
  Wire.endTransmission();
  return valorRegistro;              //regresa el valor al programa principal
}
//----------------------------------------------------------------------------------------------------------

//escribe sobre un registro del sensor por protocolo I2C
//requiere direccion del sensor, registro a escribir, y valor a escribir en el
void escribeRegistro(int _address_, int _register_, byte _value_)
{
  Wire.beginTransmission(_address_); //inicia comunicacion con el sensor seleccionado
  Wire.write(_register_); //selecciona el registro al cual leer/escribir
  Wire.write(_value_); //modo de medicion continua
  Wire.endTransmission();
  //Serial.print("estado de transmision: "); Serial.println(Wire.endTransmission()); //termina la transmision
  //e imprime en el serial su estado (exitosa o no)
}
//----------------------------------------------------------------------------------------------------------

//concatena 2 bytes para convertirlos en una palabra (tipo de dato integer)
unsigned int concatenar(byte _highByte_, byte _lowByte_)
{
  int _integer_ = (_highByte_ <<8) | _lowByte_; //crea un entero a partir de sus 2 bytes (MSB y LSB)
  return _integer_;
}
//----------------------------------------------------------------------------------------------------------

//***********************************************************************************************************
//Programa principal:
void setup()
{
  Wire.begin();
  Serial.begin(9600);
  
  pinMode(12, OUTPUT);
  digitalWrite(12, HIGH);
  delay(100);
  digitalWrite(12, LOW);
  delay(100);
  digitalWrite(12, HIGH);
  
  Serial.write("Convertidor de color a digital" "\n");
  
  //Habilita el sensor
  escribeRegistro(colorsin, ENABLE, 0x03);
  byte valorRegistroEnable = leeRegistro(colorsin, ENABLE);
  //verifica que se haya habilitado correctamente
  if (valorRegistroEnable == 0x03) {
    Serial.write("Inicializacion exitosa :)" "\n");
  }
  else {
    Serial.write("la conexion al sensor fallo :(" "\n" "valor recibido: ");
    Serial.println(valorRegistroEnable, DEC);
    while(true) ; //el programa no hace nada si falla la comunicacion
  }
  delay(50); 
}
//----------------------------------------------------------------------------------------------------------

void loop() 
{
  byte RedLow = leeRegistro(colorsin, RDATA);
  byte RedHigh = leeRegistro(colorsin, RDATAH);
  unsigned int Red = concatenar(RedHigh, RedLow);
  Serial.print(Red, DEC);
  
  byte GreenLow = leeRegistro(colorsin, GDATA);
  byte GreenHigh = leeRegistro(colorsin, GDATAH);
  unsigned int Green = concatenar(GreenHigh, GreenLow);
  Serial.print("            ");
  Serial.print(Green, DEC);
  
  byte BlueLow = leeRegistro(colorsin, BDATA);
  byte BlueHigh = leeRegistro(colorsin, BDATAH);
  unsigned int Blue = concatenar(BlueHigh, BlueLow);
  Serial.print("            ");
  Serial.println(Blue, DEC);
  delay(500);
}
//----------------------------------------------------------------------------------------------------------
