#include <LiquidCrystal_I2C.h> // Libreria que controla el I2C
#include<Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

int trigAlcohol = 2;
int echoAlcohol = 3;
int buzzer = 5;
int bomba = 12;
int Vled = 11;                          //Definimos las variables que utilizaremos durante el programa
int Rled = 10;
int tiempo;
int cont;
int temperatura;
float distancia;
int sensor;    // variable almacena valor leido de entrada analogica A0
float total;   // valor de la suma de las 5 lecturas de temperatura
float totaltemp;

void setup() {
  Serial.begin(9600);
  pinMode(trigAlcohol, OUTPUT);
  pinMode(echoAlcohol, INPUT);
  pinMode(bomba, OUTPUT);                    //Definimos como salido o entrada los pines que usamos del Arduino
  pinMode(Vled, OUTPUT);
  pinMode(Rled, OUTPUT);
  pinMode(buzzer, OUTPUT);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("BIENVENIDO");            //Pantalla de Bienvenida que se mostrara por 5 segundos
  lcd.setCursor(2, 1);
  lcd.print("SOY COVIDUINO");
  delay(5000);
  lcd.clear();
  mostrarLcd();




}

void loop() {

  activarAlcohol();
  calcularTemp();
  ActivarAlarma(); 
  mostrarLcd();
                    //Llamado de funciones principales
  



}
void activarAlcohol() {                           //Funcion que se encarga de activar la bomba de agua y tirar el alcohol
  digitalWrite(trigAlcohol, HIGH);
  delay(10);
  digitalWrite(trigAlcohol, LOW);                 //Activar sensor de ultrasonido
  tiempo = pulseIn(echoAlcohol, HIGH);
  distancia = tiempo / 58.2;
  Serial.print("distancia");
  Serial.println(distancia);

  if (distancia <= 5) {
    cont++;
    digitalWrite(bomba, HIGH);
    digitalWrite(Vled, HIGH);
    tone(buzzer, 800);
    delay(100);
    mostrarLcd();                               //Si la distancia de la mano es menor a 5cm, activa la bomba, el led verde y el Buzzer por 100 milisegundos
    noTone(buzzer);
    digitalWrite(Vled, LOW);
    digitalWrite(bomba, LOW);
    delay(5000);

  }

  else {
    digitalWrite(bomba, LOW);
    digitalWrite(Vled, LOW);
  }
}

void calcularTemp() {                                                               //Funcion que se encarga de calcular la temperatura
  total = 0;          // valor inicial de SUMA en cero
  for (int i = 0; i < 5; i++) {   // bucle que repite 5 veces
    sensor = analogRead(A0);     // lectura de entrada analogica A0
    
    temperatura = ((sensor * 5000.0) / 1023) / 10;// lectura de entrada analogica A0
    // de entrada A0 en grados centigrados
    total =  temperatura + total;      // suma de cada lectura de temperatura
    delay(500);         // demora de medio seg. entre lecturas
    
  }
  totaltemp=total/5;
  
}

void ActivarAlarma()                          //Funcion que activa una alarma en caso de que supere los 37.5 grados de temperatura corporal
{
  if (totaltemp >= 37.5)
  {
    lcd.clear();

    for (int i = 0; i < 5; i++)
    {
      digitalWrite(Rled, HIGH);
      tone(buzzer, 800);
      lcd.setCursor(0, 0);
      lcd.print("TIENE FIEBRE");
      delay(800);
      lcd.clear();
      digitalWrite(Rled, LOW);
      noTone(buzzer);
      lcd.setCursor(0, 1);
      lcd.print("Temp: ");
      lcd.setCursor(6, 1);
      lcd.print(totaltemp);
      lcd.setCursor(8, 1);
      lcd.print((char)223);
      lcd.setCursor(9, 1);
      lcd.print("C");

      delay(800);
      lcd.clear();
    }
    lcd.clear();
    mostrarLcd();
  }
}
void mostrarLcd()                             //Funcion que se encarga de mostrar informacion por pantalla
{
  lcd.setCursor(0, 0);
  lcd.print("PERSONAS:");
  lcd.setCursor(10, 0);
  lcd.print(cont);
  lcd.setCursor(0, 1);
  lcd.print("TEMP: ");
  lcd.setCursor(6, 1);
  lcd.print(totaltemp);
  lcd.setCursor(8, 1);
  lcd.print((char)223);
  lcd.setCursor(9, 1);
  lcd.print(" C");
}
