#include <SD.h>
#include <RTClib.h>
#include <Wire.h>
#include <SPI.h>
#include <Ethernet.h>
#include <ThingSpeak.h>

RTC_DS1307 RTC;
DateTime horaactual;
int m,dbarray[100];

File myFile;
String archivo;

byte mac[]={0xDE,0xAD,0xBE,0xEF,0xFE,0xED};
IPAddress ip(192,168,100,120);
EthernetClient client;
unsigned long myChannelNumber = 2238454;        // número de canal. 
const char * myWriteAPIKey = "OB4LNFLF9HHHMUYJ"; // write API Key. 

#define SoundSensorPin A1  

void setup(){
Ethernet.begin(mac, ip);
Ethernet.init(9);  // Most Arduino Ethernet hardware
Serial.begin(115200); //115200 son los baudios de comunicacion
if(!SD.begin(4)){
Serial.println("No se pudo inicializar");
return;
}
Serial.println("ok");
Wire.begin();
RTC.begin();
ThingSpeak.begin(client);  // Initialize ThingSpeak
m=0;
archivo="arch" + String(m) + ".txt";
while(SD.exists(archivo)){
Serial.print("el archivo existe: ");
m=m+1;
archivo="arch" + String(m) + ".txt";
}
}


void loop() {
for(int j=1;j<4320;j++){
for(int k=1;k<101;k++){
float dbValue,suma=0;
for(int i=1;i<101;i++){
dbValue = analogRead(SoundSensorPin)*50.0 / 1024.0 * 5;
suma=suma+dbValue;
}
dbarray[j-1]=suma/100;;
}

//calculo media de db
int dbmedia=0;
for(int a=0;a<100;a++){
dbmedia=dbmedia+dbarray[a];
}
dbmedia=dbmedia/100;

//calculo maximo de db
int dbmaximo=0;
for(int a=0;a<100;a++){
  if(dbarray[a]>dbmaximo){
    dbmaximo=dbarray[a];
  }
}

//calculo minimo de db
int dbminimo=200;
for(int a=0;a<100;a++){
  if(dbarray[a]<dbminimo){
    dbminimo=dbarray[a];
  }
}

horaactual = RTC.now();

ThingSpeak.setField(3,dbminimo);
ThingSpeak.setField(2,dbmaximo);
ThingSpeak.setField(1,dbmedia);
ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);     // write to the ThingSpeak channel 
myFile = SD.open(archivo,FILE_WRITE);    //abrimos  el archivo
if(myFile){ 
myFile.print(horaactual.day());
myFile.print('/');
myFile.print(horaactual.month());
myFile.print('/');
myFile.print(horaactual.year());
myFile.print(' ');
myFile.print(horaactual.hour());
myFile.print(':');
myFile.print(horaactual.minute());
myFile.print(' '); 
myFile.print(dbminimo);
myFile.print(' '); 
myFile.print(dbmedia);
myFile.print(' '); 
myFile.println(dbmaximo);
myFile.close(); //cerramos el archivo
}else{
Serial.println("Error al abrir el archivo");
}
delay(120000);  // el numero es la cantidad de mseg entre cada medicion
}
m++;
Serial.println(m);
archivo="arch" + String(m) + ".txt";
}
