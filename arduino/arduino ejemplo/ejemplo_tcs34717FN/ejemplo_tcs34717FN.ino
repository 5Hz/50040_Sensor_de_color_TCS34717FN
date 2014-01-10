/******************************************************************
** Código escrito en Electrónica 5Hz                             **
** www.5hz-electronica.com                                       **
**                                                               **
**                                                               **
** Descripción del código:                                       **
**                                                               **
** Ejemplo para el sensor de color TCS34717FN                    **
** Puede ser necesario calibrar el sensor según las condiciones  **
** de luminosidad multiplicando cada valor leido por una const.  **
*******************************************************************
Conexiones:
UNO   MMA7361

3.3V  VCC
GND   GND
10    ST
12    GSEL
13    SLP
*/

#include <Wire.h>

//direccion I2C del sensor
int Direccion =0x29;
//LED conectado al pin 10
int led = 10;

byte receivedVal = 0x00;

//variables donde se almacenan los valores de los registros de cada color
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
  //inicia comunicacion I2C y serial
  Wire.begin();
  Serial.begin(9600);
  Serial.write("Convertidor de luz a digital" "\n");
  //enciende LED para que ilumine los objetos a sensar
  pinMode(led, OUTPUT);
  analogWrite(led, 50);
  
  //Despierta el sensor
  Wire.beginTransmission(Direccion);
  Wire.write(0x80);
  Wire.write(0x03); 
  Wire.endTransmission();
  
  Wire.beginTransmission(Direccion); 
  Wire.requestFrom(Direccion,1);
  receivedVal = Wire.read();
  Wire.endTransmission();
  
  //Comprueba que se haya escrito correctamente el registro, si es asi, la conexion se realizó correctamente.
  if (I2CreadByte(Direccion) == 0x03) 
  {
    Serial.write("Conexion correcta" "\n");
    Serial.write("ADC (R, G, B):" "\n");
  }
  else 
  {
    Serial.write("la conexion al sensor fallo: ");
    Serial.println(receivedVal, DEC);
  }
  delay(50); 

}

void loop() {
  //Lee el valor bajo del Rojo.
  I2Csend(Direccion, 0x96);
  RedLow = I2CreadByte(Direccion);
  //Lee el valor alto del Rojo
  I2Csend(Direccion, 0x97);
  RedHigh = I2CreadByte(Direccion);
  //Concatena los 2 valores para formar un solo numero
  RedHigh = (RedHigh * 256) + RedLow;
  //Imprime el valor leído en el puerto serial
  Serial.print(RedHigh, DEC);
  
  //Se repite para el color verde
  
  I2Csend(Direccion, 0x98);
  GreenLow = I2CreadByte(Direccion);
  I2Csend(Direccion, 0x99);
  GreenHigh = I2CreadByte(Direccion);
  GreenHigh = (GreenHigh * 256) + GreenLow;
  Serial.print("            ");
  Serial.print    (GreenHigh, DEC);
  
  //Se repite para el color Azul
  
  I2Csend(Direccion, 0x9A);
  BlueLow = I2CreadByte(Direccion);
  I2Csend(Direccion, 0x9B);
  BlueHigh = I2CreadByte(Direccion); 
  BlueHigh = (BlueHigh * 256) + BlueLow;
  Serial.print("            ");
  Serial.println(BlueHigh, DEC);
  delay(500);
}

void I2Csend(byte address, byte value)
{
  Wire.beginTransmission(address);
  Wire.write(value);
  Wire.endTransmission();
}

byte I2CreadByte(int address)
{
  Wire.beginTransmission(address); 
  Wire.requestFrom(address,1);
  byte value = Wire.read();
  Wire.endTransmission();
  return value;
}
