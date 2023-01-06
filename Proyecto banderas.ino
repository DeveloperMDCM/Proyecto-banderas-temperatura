//Variables globales y librerias
#define led1_temperatura 12 //pin 12
#define led2_temperatura  11 //pin 11
#define led3_temperatura  10 //pin 10
// Definimos el pin  donde se activa la alarma el
#define alarma_insendio 9 // pin 9
#define retardo 3000 //ms
double resultado_luz = 0;
//Librerias para display I2C y Sensor DHT22 
#include <Wire.h> //permite comunicarse con dispositivos por bus I2C (Inter-Integrated Circuit o 2 alambres). Usa dos lineas: SDA (datos) y SCL (reloj)
#include <Adafruit_GFX.h> //complemento para hacer funcionar el sensor DHT
#include <Adafruit_SSD1306.h> //complemento para hacer funcionar el sensor DHT
#include <DHT.h> //Libreria para el sensor digital DHT22
#include <DHT_U.h> 
#define SCREEN_WIDTH 128 // Ancho del display I2C OLED
#define SCREEN_HEIGHT 64 // Alto del display I2C OLED
// Definimos el pin digital donde se conecta el sensor
#define DHTPIN 32 //pin 2
// Dependiendo del tipo de sensor
#define DHTTYPE DHT22
// Inicializamos el sensor DHT12
DHT dht(DHTPIN, DHTTYPE); // declaramos el pin de conexion y el tipo de sensor a utilizar
// Los pines para el I2C son:. 
// En arduino UNO:       A4(SDA), A5(SCL)
// En arduino MEGA 2560: 20(SDA), 21(SCL)
// En  LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //Funcion para definir el alto y acho del display para poder inprimir en el

//Logo y/o imagen temperatura
static const unsigned char PROGMEM temperatura[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x18, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 
  0x00, 0x24, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x24, 0x04, 0x00, 
  0x00, 0x24, 0x0e, 0x00, 0x00, 0x24, 0x09, 0x00, 0x00, 0x24, 0x09, 0x00, 0x00, 0x24, 0x06, 0x00, 
  0x00, 0x24, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 
  0x00, 0x24, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0x99, 0x00, 0x00, 
  0x00, 0x99, 0x00, 0x00, 0x00, 0x81, 0x00, 0x00, 0x00, 0x42, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//Logo y/o imagen Humedad
static const unsigned char PROGMEM Humedad[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x18, 0x00, 0x00, 
  0x3c, 0x00, 0x00, 0x7e, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0x00, 0x01, 0x1f, 0x80, 0x02, 0x0e, 
  0xc0, 0x02, 0x4c, 0x40, 0x07, 0x08, 0xe0, 0x07, 0x91, 0xe0, 0x07, 0xe3, 0xe0, 0x07, 0xc7, 0xe0, 
  0x07, 0x89, 0xe0, 0x07, 0x10, 0xe0, 0x02, 0x32, 0x40, 0x03, 0x70, 0x40, 0x01, 0xf8, 0x80, 0x00, 
  0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//Logo y/o imagen Luz
static const unsigned char PROGMEM Luz[] = {
0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 
  0x00, 0x01, 0x80, 0x00, 0x06, 0x01, 0x80, 0x60, 0x07, 0x00, 0x00, 0xe0, 0x03, 0x80, 0x01, 0xc0, 
  0x01, 0x8f, 0xf1, 0x80, 0x00, 0x1f, 0xf8, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x7f, 0xfe, 0x00, 
  0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0xfc, 0xff, 0xff, 0x3f, 
  0xfc, 0xff, 0xff, 0x3f, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 
  0x00, 0x7f, 0xfe, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x01, 0x8f, 0xf1, 0x80, 
  0x03, 0x80, 0x01, 0xc0, 0x07, 0x00, 0x00, 0xe0, 0x06, 0x01, 0x80, 0x60, 0x00, 0x01, 0x80, 0x00, 
  0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x01, 0x80, 0x00
};

void setup() {
  Serial.begin(9600); //Inicialisamos el monitor serial
  //Declaramos los leds y/o banderas y la alarma como salida
  pinMode(led1_temperatura, OUTPUT); //led verde (Bandera Verde)
  pinMode(led2_temperatura, OUTPUT); //led Amarillo (Bandera Amarilloja)
  pinMode(led3_temperatura, OUTPUT); //led rojo (Bandera roja
  pinMode(alarma_insendio, OUTPUT);  // Pin para alarma cuando la temperatura es muy alta

  dht.begin(); // Inicimos el sensor DHT
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //Compatibilidad Para el I2C
  display.clearDisplay(); //Limpia el display I2C
}
void mostrarDatos() {  //Se declara la función mostrarDatos(), esta no devuelve ningún valor ni recibe parámetros
  // Leemos la humedad relativa
  float h = dht.readHumidity();
  // Leemos la temperatura en grados centígrados (por defecto)
  float t = dht.readTemperature();
  // Comprobamos si esta conectado el sensor o si hay un error en la lectura
  if (isnan(h) || isnan(t)) {
    Serial.println("Error al obtener los datos del sensor DHT12");
    return;
  }
   double  LDR = analogRead(A0); // Guardamos el valor del LDR que entra por la entrada anologa A0
   resultado_luz = (LDR*100/1023.0); //Convertimos el valor de LDR al porcentaje correspondiente en luminocidad
   //los Serial.print imprimen mensajes en el monitor serial y los Display.print inprimen en el display IC2 oled
  Serial.print("Humedad: "); //Imprime la palabra humedad
  Serial.print(h); //Imprime el valor de humedad
  Serial.print(" %"); //Imprime %
  Serial.println(""); //Salto de linea
  Serial.print("Temperatura: "); //Imprime la palabra Temperatura
  Serial.print(t); //Imprime el valor de la temperatura en en grados centígrados
  Serial.println(" °C "); //Imprime la palabra humedad
  Serial.print("Luminocidad: ");//Imprime la palabra humedad
  Serial.print(resultado_luz,2); //Imprime el porcentaje de luminocidad con 2 cifras
  Serial.print(" %");
  display.setTextColor(WHITE); //Color por defecto para imprimir en el display
  display.setTextSize(1);  //Tamaño del texto a imprimir en el display
  display.setTextColor(WHITE);
  display.drawBitmap(0, 0, temperatura, 32,32, 1); //Imprime imagen temperatura
  //display.drawBitmap(0, 30, Humedad, 24,24, 1); //Imprime imagen Humedad
  display.drawBitmap(90, 0, Luz, 32,32, 1);    //Imprime imagen Luz
  display.setTextSize(1);
  //Movemos de posicion la temperatura en el display bicolor cuando la temperatura es muy alta
   if(t>32){
      display.setCursor(28,9); // posicionamiento de la temperatura cuando sube a más de 32 grados
     }
     else{
      display.setCursor(28,20); // posicionamiento de la temperatura cuando es menor a  32 grados
     }
     //imprime en el monitor serial los datos obtenidos
  display.print(t);
  display.setTextSize(1); //Tamaño del texto
  display.cp437(true); 
  display.write(248); //Simbolo grados
  display.setTextSize(1);
  display.print("C");
  display.setTextSize(1);
  display.setCursor(90,45);
  display.print(resultado_luz); //Imprime el porcentaje de Luminocidad
  display.write(37); //Simbolo porcentaje
  display.display(); 
   display.clearDisplay(); //Limpia el display I2C oled para poder borrar y mostrar los datos consecutivos
//Comprobacion de temperatura y tiempo nublado
  if(t<=26 && resultado_luz<=15.0){
   digitalWrite(led1_temperatura, 1); //enciende led1 verde pin 12
   Serial.println("Nublado");
   display.setTextSize(1); 
   display.setCursor(25,40); //Posicionamiento del texto
   display.print("Nublado");
   display.setCursor(25,53); //Posicionamiento del texto
   display.print("H:");
   display.print(h);
   display.print("%");
      //Comprobacion de temperatura y tiempo Parcialmente nublado
  }else if(t<=26 && resultado_luz>=16){
    digitalWrite(led1_temperatura, 1); //enciende led2 verde pin 12
     Serial.println("Parcialmente nublado");
      display.setTextSize(1); 
     display.setCursor(10,31); //Posicionamiento del texto
     display.print("Parcialmente");
     display.setCursor(25,40); //Posicionamiento del texto
     display.print("Nublado");
     display.setTextSize(1); 
    display.setCursor(25,53); //Posicionamiento del texto
   display.print("H:");
   display.print(h);
   display.print("%");
  }else{
     digitalWrite(led1_temperatura, 0); // apaga el led verde
  }
  //Comprobacion de temperatura y tiempo soleado
    if(t>26 && t<=32 && resultado_luz>0 && resultado_luz<=70){
     digitalWrite(led2_temperatura, 1); //enciende led2 amarillo pin 11 
     Serial.println("Soleado");
     display.setTextSize(1); 
     display.setCursor(25,35); //Posicionamiento del texto
     display.print("Soleado");
     display.setTextSize(1); 
     display.setCursor(25,50); //Posicionamiento del texto
     display.print("H:");
      display.print(h);
     display.print("%");
  }
   else {
     digitalWrite(led2_temperatura, 0);
  }
  //Comprobacion de temperatura y tiempo con posobles lesiones de piel 
   if(t>32 && resultado_luz>0){
     digitalWrite(led3_temperatura, 1); //enciende led3 rojo pin 10
     digitalWrite(alarma_insendio, 1);  //enciende alarma 
     display.setTextSize(1); 
     display.setCursor(25,33); //Posicionamiento del texto
     display.print("Posibles");
     display.setCursor(25,43); //Posicionamiento del texto
     display.print("lesiones");
      display.setCursor(30,53); //Posicionamiento del texto
     display.print("de piel");
  }
  else{
     digitalWrite(led3_temperatura, 0); // Apaga led3 rojo pin 10
     digitalWrite(alarma_insendio, 0); // Apaga alarma
  }
  return;
}
 //Ciclo de repeticion infinita
void loop() 
{
   delay(1000);
  // Esperamos 1 segundos entre medidas y/o lecturas
   mostrarDatos(); // Funcion que muestra todos los datos

}
