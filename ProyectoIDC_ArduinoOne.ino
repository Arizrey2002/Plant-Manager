#include <dht.h>
#include <LedControl.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(4,5);


LedControl lc = LedControl(10,8,9,1);
dht DHT;
LiquidCrystal_I2C  lcd(0x27,2,1,0,4,5,6,7);

#define DHT11_PIN 13
#define YL69_PIN A0
#define SEN2609_PIN A1
#define KY018_PIN A2
#define SCL_PIN A4
#define HCSR04_Tri_PIN 12
#define HCSR04_Echo_PIN 11
#define onButton_PIN 2
#define plantButton_PIN 3

int chk = 0;
int waterLvl = 0;
int valorLuz = 0;
int duracion = 0;
int distancia = 0;
int plantSelec = 1;
int buttonPlantState = 1;
int buttonPlantStateNew;
int buttonState = 1;
int buttonStateNew;
bool pulsado = false;
String aguaStatus;
String nomPlant;

int lecturaSensorYL69 = 0;

int valorLuzMapeado = 0;

//ESTADO AGUA
int rellenarAgua = 0;

//VALORES REALES DE TEMPERATURA
int tempPlant = 0;
//VALORES UMBRAL DE TEMPERATURA
int tempMAX = 0;
int tempMIN = 0;

//VALORES REALES DE HUMEDAD AIRE
int humPlant = 0;
//VALORES UMBRAL DE HUMEDAD AIRE
int humMAX = 0;
int humMIN = 0;

//VALORES REALES DE HUMEDAD TIERRA
int humTPlant = 0;
//VALORES UMBRAL DE HUMEDAD TIERRA
int humTMAX = 0;
int humTMIN = 0;

//SALUD PLANTA
int tempSalud = 0;
int humSalud = 0;
int humTSalud = 0;

//NUMERO DE PLANTAS REGISTRADAS
int numPlantas = 4;

char tempSend[4];


void setup() {
  Serial.begin(9600);
  pinMode(onButton_PIN, INPUT);
  pinMode(HCSR04_Tri_PIN, OUTPUT);
  pinMode(HCSR04_Echo_PIN, INPUT);
  pinMode(onButton_PIN, INPUT);
  pinMode(plantButton_PIN, INPUT_PULLUP);

  lc.shutdown(0,false);
  lc.setIntensity(0,4);
  lc.clearDisplay(0);

  lcd.begin (16,2); 
 
  // Switch on the backlight
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home (); 

  mySerial.begin(115200);
}

void loop() {
  //CONSEGUIR ESTADO BOTONES
  buttonPlantStateNew = digitalRead(plantButton_PIN);
  buttonStateNew = digitalRead(onButton_PIN);

  if(buttonStateNew == 1 && buttonState == 1){
    if(pulsado){
      pulsado = false;
    } else {
      pulsado = true;
    }
  }
  
  
    if(pulsado){
      //VALOR LUZ
      valorLuz = analogRead(A1);

      //SENSOR DE HUMEDAD Y TEMPERATURA SENCILLO: DHT11
      chk = DHT.read11(DHT11_PIN);

      //EXTRACCION DE DATOS TEMPERATURA Y HUMEDAD
      tempPlant = DHT.temperature;
      humPlant = DHT.humidity;

      //EXTRACCION DE DATOS HUMEDAD TIERRA
      lecturaSensorYL69 = analogRead(YL69_PIN);
      humTPlant = map (lecturaSensorYL69, 1023, 0, 0, 100);
      
      //SENSOR DE NIVEL DEL AGUA: SEN-2609
      waterLvl = analogRead(SEN2609_PIN);
      
      //COMPROBACION NIVEL AGUA
      if(waterLvl <= 100){
        aguaStatus= "Vacio!";
        rellenarAgua = 1;
      } else if(waterLvl > 100 && waterLvl <= 230){
        aguaStatus= "Bajo!";
        rellenarAgua = 0;
      } else if(waterLvl > 230 && waterLvl <= 270){
        aguaStatus = "Medio";
        rellenarAgua = 0;
      } else{
        aguaStatus = "Alto";
        rellenarAgua = 0;
      }

      //EXTRACCION DE DATOS SENSOR DE LUZ: PON EL NOMBRE DEL SENSOR
      valorLuz = analogRead(KY018_PIN);
      valorLuzMapeado = map(valorLuz,0,1023,0,100);

      //SENSOR DE PROXIMIDAD
      digitalWrite(HCSR04_Tri_PIN, HIGH);
      delay(1);
      digitalWrite(HCSR04_Tri_PIN, LOW);
      
      duracion = pulseIn(HCSR04_Echo_PIN, HIGH);
      distancia = duracion/58.2;

      if(distancia <= 15){
        if(humTSalud == 0 && humSalud == 0 && tempSalud == 0){
          lc.clearDisplay(0);

          lcd.setBacklight(HIGH);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Cuidando:" + nomPlant);
          lcd.setCursor(0, 1);
          lcd.print("T:" + String(tempPlant));
          lcd.setCursor(5, 1);
          lcd.print("H:" + String(humPlant));
          lcd.setCursor(10, 1);
          lcd.print("HT:" + String(humTPlant));
          //CARITA FELIZ  
          //FILA 0
          lc.setLed(0,0,1,true);
          lc.setLed(0,0,2,true);
          lc.setLed(0,0,3,true);
          lc.setLed(0,0,4,true);
          lc.setLed(0,0,5,true);
          lc.setLed(0,0,6,true);  

          //FILA 1
          lc.setLed(0,1,0,true);
          lc.setLed(0,1,7,true);

          //FILA 2
          lc.setLed(0,2,0,true);
          lc.setLed(0,2,2,true);
          lc.setLed(0,2,5,true);
          lc.setLed(0,2,7,true);

          //FILA 3
          lc.setLed(0,3,0,true);
          lc.setLed(0,3,1,true);
          lc.setLed(0,3,7,true);

          //FILA 4
          lc.setLed(0,4,0,true);
          lc.setLed(0,4,1,true);
          lc.setLed(0,4,7,true);

          //FILA 5
          lc.setLed(0,5,0,true);
          lc.setLed(0,5,2,true);
          lc.setLed(0,5,5,true);
          lc.setLed(0,5,7,true);

          //FILA 6
          lc.setLed(0,6,0,true);
          lc.setLed(0,6,7,true);

          //FILA 7
          lc.setLed(0,7,1,true);
          lc.setLed(0,7,2,true);
          lc.setLed(0,7,3,true);
          lc.setLed(0,7,4,true);
          lc.setLed(0,7,5,true);
          lc.setLed(0,7,6,true); 


          delay(5000);

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Estado Agua:");
          lcd.setCursor(0, 1);
          lcd.print(aguaStatus);

          delay(500);
      } else { //HAY PROBLEMA
          //PANTALLA LCD
          lcd.setBacklight(HIGH);
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Cuidando:" + nomPlant);
          lcd.setCursor(0, 1);
          lcd.print("T:" + String(tempPlant));

          if(tempSalud == 1){
            lcd.print("!");
          }

          lcd.setCursor(5, 1);
          lcd.print("H:" + String(humPlant));

          if(humSalud == 1){
            lcd.print("!");
          }

          lcd.setCursor(10, 1);
          lcd.print("HT:" + String(humTPlant));

          if(humTSalud == 1){
            lcd.print("!");
          }
          
          //CARITA CARITA TRISTE
          //FILA 0
          lc.setLed(0,0,1,true);
          lc.setLed(0,0,2,true);
          lc.setLed(0,0,3,true);
          lc.setLed(0,0,4,true);
          lc.setLed(0,0,5,true);
          lc.setLed(0,0,6,true);  

          //FILA 1
          lc.setLed(0,1,0,true);
          lc.setLed(0,1,7,true);

          //FILA 2
          lc.setLed(0,2,0,true);
          lc.setLed(0,2,1,true);
          lc.setLed(0,2,5,true);
          lc.setLed(0,2,7,true);

          //FILA 3
          lc.setLed(0,3,0,true);
          lc.setLed(0,3,2,true);
          lc.setLed(0,3,7,true);

          //FILA 4
          lc.setLed(0,4,0,true);
          lc.setLed(0,4,2,true);
          lc.setLed(0,4,7,true);

          //FILA 5
          lc.setLed(0,5,0,true);
          lc.setLed(0,5,1,true);
          lc.setLed(0,5,5,true);
          lc.setLed(0,5,7,true);

          //FILA 6
          lc.setLed(0,6,0,true);
          lc.setLed(0,6,7,true);

          //FILA 7
          lc.setLed(0,7,1,true);
          lc.setLed(0,7,2,true);
          lc.setLed(0,7,3,true);
          lc.setLed(0,7,4,true);
          lc.setLed(0,7,5,true);
          lc.setLed(0,7,6,true);

          delay(5000);

          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Estado Agua:");
          lcd.setCursor(0, 1);
          lcd.print(aguaStatus);

          delay(500);   
      }
    } else {
      lc.clearDisplay(0);
      lcd.clear();
      lcd.setBacklight(LOW);
    }

      //ESTADO PLANTA
      if(tempMIN < DHT.temperature && DHT.temperature < tempMAX){
        tempSalud = 0;
      } else {
        tempSalud= 1;
      }

      if(humMIN < DHT.temperature && DHT.temperature < humMAX){
        humSalud = 0;
      } else {
        humSalud = 1;
      }

      if(humTMIN < DHT.temperature && DHT.temperature < humTMAX){
        humTSalud = 0;
     } else {
        humTSalud = 1;
     }

     delay(1000);

     //SEND DATA TO WI-FI MODULE
     mySerial.println(tempPlant);

    } else{
     lcd.setBacklight(HIGH);
      if(buttonPlantStateNew == 0 && buttonPlantState == 1){
        if(plantSelec < numPlantas){
          plantSelec++;
        } else {
          plantSelec = 1;
        }
        delay(500);
      }
      
      lc.clearDisplay(0);


      //CREAR RUEDA OPCIONES
      //CARITA CARITA TRISTE
      //FILA 0
      lc.setLed(0,0,2,true);
      lc.setLed(0,0,3,true);
      lc.setLed(0,0,4,true);
      lc.setLed(0,0,5,true);

      //FILA 1
      lc.setLed(0,1,1,true);
      lc.setLed(0,1,6,true);

      //FILA 2
      lc.setLed(0,2,0,true);
      lc.setLed(0,2,2,true);
      lc.setLed(0,2,5,true);
      lc.setLed(0,2,7,true);

      //FILA 3
      lc.setLed(0,3,0,true);
      lc.setLed(0,3,3,true);
      lc.setLed(0,3,4,true);
      lc.setLed(0,3,7,true);

      //FILA 4
      lc.setLed(0,4,0,true);
      lc.setLed(0,4,3,true);
      lc.setLed(0,4,4,true);
      lc.setLed(0,4,7,true);

      //FILA 5
      lc.setLed(0,5,0,true);
      lc.setLed(0,5,2,true);
      lc.setLed(0,5,5,true);
      lc.setLed(0,5,7,true);

      //FILA 6
      lc.setLed(0,6,1,true);
      lc.setLed(0,6,6,true);

      //FILA 7
      lc.setLed(0,7,2,true);
      lc.setLed(0,7,3,true);
      lc.setLed(0,7,4,true);
      lc.setLed(0,7,5,true);

      delay(1500);

      switch(plantSelec){
        case 1:
        //VALORES DE TEMPERATURA ROSA
        tempMAX = 30;
        tempMIN = 15;
        //VALORES DE HUMEDAD AIRE ROSA
        humMAX = 80;
        humMIN = 50;
        //VALORES DE HUMEDAD TIERRA ROSA
        humTMAX = 90;
        humTMIN = 40;
        //NOMBRE PLANTA = ROSA
        nomPlant = "ROSA";
        break;

        case 2:
        //VALORES DE TEMPERATURA TULIPAN
        tempMAX = 28;
        tempMIN = 15;
        //VALORES DE HUMEDAD AIRE TULIPAN
        humMAX = 95;
        humMIN = 75;
        //VALORES DE HUMEDAD TIERRA TULIPAN
        humTMAX = 90;
        humTMIN = 50;
        //NOMBRE PLANTA = ROSA
        nomPlant = "TULIPAN";
        break;

        case 3:
        //VALORES DE TEMPERATURA LIRIO
        tempMAX = 25;
        tempMIN = 15;
        //VALORES DE HUMEDAD AIRE LIRIO
        humMAX = 90;
        humMIN = 40;
        //VALORES DE HUMEDAD TIERRA LIRIO
        humTMAX = 90;
        humTMIN = 40;
        //NOMBRE PLANTA = ROSA
        nomPlant = "LIRIO";
        break;

        case 4:
        //VALORES DE TEMPERATURA CACTUS
        tempMAX = 40;
        tempMIN = 7;
        //VALORES DE HUMEDAD AIRE CACTUS
        humMAX = 99;
        humMIN = 20;
        //VALORES DE HUMEDAD TIERRA CACTUS
        humTMAX = 99;
        humTMIN = 10;
        //NOMBRE PLANTA = CACTUS
        nomPlant = "CACTUS";
        break;
      }

     lcd.clear();
     lcd.setCursor(0, 0);
     lcd.print("Cuidando:" + nomPlant);
     lcd.setCursor(0,1);
     lcd.print("T:" + String(tempMIN));
     lcd.setCursor(4,1);
     lcd.print("|" + String(tempMAX));
     lcd.setCursor(8,1);
     lcd.print("HT:" + String(humTMIN));
     lcd.setCursor(13,1);
     lcd.print("|" + String(humTMAX));
   }
}



