#include <Stepper.h>                                                           //Biblioteka za stepper motor
const int stepsPerRevolution = 2048;                                           //Ukupan broj koraka stepper motora
Stepper myStepper = Stepper(stepsPerRevolution, 12, 10, 11, 9);                //Povezivanje stepper motora sa ulazima
#include <Servo.h>                                                             //Biblioteka za servo motor
#define EchoPin 7
#define TrigPin 8
#define ENABLE 5
#define DIRA 3
#define DIRB 4
#define LightPin 13
#define BuzzerPin 6
#define servoPin 46
int skrenuoLevo;
int skrenuoDesno;
int duration;
int distance;
int PhotoPin = 0;
int LaznaUzbuna;
int poslednjiUgao=90;
int sledeciUgao;
int ugao_okretanja;
int ukljucen;
int pozicija_kormila=0;
int  ukljuciti=0; 
int rucno_ukljucivanje_svetla=0;
void NapraviPuls(){                             // Fja za pravljenje impulsa za ultrasonic senzor
digitalWrite(TrigPin, LOW);
delayMicroseconds(2);
digitalWrite(TrigPin, HIGH);
delayMicroseconds(10);
digitalWrite(TrigPin, LOW);

  }
void setup() {
    pinMode(EchoPin, INPUT);
    pinMode(TrigPin, OUTPUT);
    pinMode(ENABLE,OUTPUT);
    pinMode(DIRA,OUTPUT);
    pinMode(DIRB,OUTPUT);   
    Serial.begin(9600);
    Serial1.begin(9600);
    myStepper.setSpeed(10); 
    
}

void loop() {
  
if(ukljuciti==0){                                  //Ako jos uvek nije ukljucen krecemo se samo kroz ovaj blok
  if (Serial1.available()>0){
  ukljucen=Serial1.read();
  Serial.println("Ukljucen je");
  Serial.println(ukljucen);}
   if(ukljucen==1){                              //Ako se pritisne dugme ON onda se salje 1
    ukljuciti=1;
    digitalWrite(ENABLE,HIGH); // enable on
    digitalWrite(DIRA,LOW);                      //Odredjujemo u kom se smeru okrece
    digitalWrite(DIRB,HIGH);
    pozicija_kormila=0;
    }}
else{                                              // Ovaj ostatak koda se izvrsava tek nakon sto se brodic upalio
    int reading= analogRead(PhotoPin);             //Citanje vrednosti sa fotootpornika
    if(reading<125 || rucno_ukljucivanje_svetla==1){                                  //Ako je manje od 125(pao mrak) upali svetlo
      digitalWrite(LightPin,HIGH);
       }
    else{digitalWrite(LightPin,LOW);}
    
    NapraviPuls();  
    duration = pulseIn(EchoPin, HIGH);           
    distance= duration*0.034/2;                  //Odredjivanje razdaljine 
    //Serial.println(distance); 
                        
    
    if(distance>0 & distance<15)
  {
     LaznaUzbuna=0;
     Serial.println("STANI");
     digitalWrite(ENABLE,LOW);       // Gasenje motora
     delay(200);
  
   for(int i=0;i< 2; i++){
    NapraviPuls();                              
    duration = pulseIn(EchoPin, HIGH);
    distance= duration*0.034/2;
    Serial.println(distance);
    if(distance>15){
    LaznaUzbuna=1; 
    Serial.println("Lazna uzbuna");
    digitalWrite(ENABLE,HIGH);
    delay(500);}
    }

    if(LaznaUzbuna!=1){
      for(int i=0;i<2;i++){        // ALARM
      tone(BuzzerPin,1000,200);   //prilikom koriscenja ove funkcije neki ulazi ne rade jer i oni koriste fju timer kao i fja tone() 
      delay(400);}
      int iskoci=0;
      
     while(iskoci==0){
        NapraviPuls();
        duration = pulseIn(EchoPin, HIGH);
        distance= duration*0.034/2;
    
        if(distance>25){                                    //Ako je ovaj uslov ispunjen izadji i petlje
        iskoci=1; 
        digitalWrite(ENABLE,HIGH);
        delay(200);}
       }}}
    
 if (Serial1.available()>0){
  int komanda = Serial1.read();
  
  switch(komanda)
  {
   case 0:
      Serial.println("Ugasi motor");
      digitalWrite(ENABLE,LOW);
      digitalWrite(DIRA,LOW);                     //Odredjujemo u kom se smeru okrece
      digitalWrite(DIRB,LOW);
      myStepper.step((-pozicija_kormila)*5*5.68);   //vracanje na pocetnu poziciju (sredina)
      pozicija_kormila=0;
      ukljuciti=0;
      delay(4000);
      break;

    case 1:
      Serial.println("Motor ostaje ukljucen");
      break;        
    case 2:
       Serial.println("Napred");
       digitalWrite(DIRA,LOW);                     //Odredjujemo u kom se smeru okrece
       digitalWrite(DIRB,HIGH);
       break;        
    case 3:
       Serial.println("Nazad");
       digitalWrite(DIRA,HIGH);                     //Odredjujemo u kom se smeru okrece
       digitalWrite(DIRB,LOW);
       break;      
    case 4:
       Serial.println("Desno");
       myStepper.step(5*5.68);
       pozicija_kormila+=1;
       break;        
    case 5:
       Serial.println("Levo");
       myStepper.step(-5*5.68);
       pozicija_kormila-=1;
       break;        
    case 6:
       Serial.println("Ukljuceno/Iskljuceno svetlo");
       if(rucno_ukljucivanje_svetla==0){
       digitalWrite(LightPin,HIGH);
       rucno_ukljucivanje_svetla=1;}
       else {
        digitalWrite(LightPin,LOW);
       rucno_ukljucivanje_svetla=0;}
       break;        
    case 7:
       Serial.println("Sirena");
       for(int i=0;i<2;i++){        // Sirena
       tone(BuzzerPin,4000,200);   //prilikom koriscenja ove funkcije neki ulazi ne rade jer i oni koriste fju timer kao i fja tone() 
       delay(400);}
       break;     
     }   
            
  }}}
