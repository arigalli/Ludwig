/*este sketch permite medir un sonido. Para que lo haga por el monitor serial introducimos
 * el n{umero 1 y oprimimos Send. Ludwig1Medicion toma 20 mediciones, las promedia y da 
 * el resultado"
 */


#define SoundSensorPin A1  //this pin read the analog voltage from the sound level meter
#define VREF  5.0  //voltage on AREF pin,default:operating voltage
char mesage;

void setup(){
Serial.begin(115200); //115200 son los baudios de comunicacion

}

  


void loop() {

while(Serial.available()>0){
mesage=Serial.read();  //queda esperando el numero 1 para medir
Serial.println(mesage);
if(mesage=='1'){
float voltageValue,dbValue,suma=0;
for(int i=1;i<21;i++){
voltageValue = analogRead(SoundSensorPin) / 1024.0 * VREF;
dbValue = voltageValue * 50.0;  //convert voltage to decibel value
suma=suma+dbValue;
delay(10);
}
dbValue=suma/20;
Serial.print(dbValue,1); //el 1 indica la cantidad de decimales
Serial.println(" dB");   //println, introduce retorno de carro 
}
Serial.println("para medir oprima 1");
}

}
    
