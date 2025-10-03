#define sensor 13

#define cintaIn1 12
#define cintaIn2 14

#define bombaIn1 25
#define bombaIn2 26

typedef enum { MoverCinta = 0,
               LlenaBote = 1,
               BoteLleno = 2,
               Fin = 3,
               Espera = 4
} Estados;

Estados Sistema = MoverCinta;

hw_timer_t *timer1 = NULL;
hw_timer_t *timer2 = NULL;


int Started = 0;



typedef struct{
  int vel;
  int d;
} CINTA;

CINTA Cinta;

void ARDUINO_ISR_ATTR bombaIsr() {
  Serial.println("Interrupcion de bomba");
  digitalWrite(bombaIn1, LOW);
  timerStop(timer1);
  Sistema = BoteLleno;
}

void ARDUINO_ISR_ATTR cintaIsr() {
  Serial.println("Interrupcion de cinta");
  analogWrite(cintaIn1, 0);
  timerStop(timer2);
  Sistema = Fin;
}

void MoveCinta(void){

  if(Cinta.vel < 0 || Cinta.vel > 2.2) return;

  int vel = map(Cinta.vel,0,2.2,0,255);

  analogWrite(cintaIn1, vel);
  analogWrite(cintaIn2, 0);
}

void ActivaBomba(int ml){
  digitalWrite(bombaIn1,HIGH);
  digitalWrite(bombaIn2,LOW);
}


void MoveCintaPos(void){
  long int t;
  //t = map(cm,0,10,0,)
}



void setup() {
  pinMode(sensor, INPUT);     //
  pinMode(cintaIn1, OUTPUT);  //
  pinMode(cintaIn2, OUTPUT);  //

  pinMode(bombaIn1, OUTPUT);  //
  pinMode(bombaIn2, OUTPUT);  //

  Serial.begin(115200);

  // Timer Divider
  timer1 = timerBegin(1000000);
  timer2 = timerBegin(1000000);

  if (timer1 == NULL) Serial.println("Timer no iniciado");
  if (timer2 == NULL) Serial.println("Timer2 no iniciado");
  timerAttachInterrupt(timer1, &bombaIsr);
  
  timerAttachInterrupt(timer2, &cintaIsr);

  int ml = 20;
  Cinta.vel = 2;
  Cinta.d = 10;

  int t1 = map(ml,0,32,0,30000000);
  int t2 = map(Cinta.d,0,10,0,(Cinta.d/Cinta.vel)*1000000);
  
  timerAlarm(timer1, t1, true, 0);
  timerAlarm(timer2, t2, true, 0);


  delay(5000);
}

void loop() {
    float vel = 2;
    float ml = 10;
    int lectura;

     lectura = analogRead(sensor);


    Serial.print("Estado: ");
    Serial.println(Sistema);
    

    switch(Sistema){
      case MoverCinta:
        MoveCinta();
        
        if (!lectura)
          Sistema = LlenaBote;
        break;

      case LlenaBote:
        // Parar la cinta
        digitalWrite(cintaIn1, LOW);
        digitalWrite(cintaIn2, LOW);
        
        // Encender la bomba
        digitalWrite(bombaIn1,HIGH);
        digitalWrite(bombaIn2,LOW);
        
        // Poner el timer en funcion de los ml
        timerStart(timer1);
        Sistema = Espera;
      break;

      case(BoteLleno):
        MoveCinta();
        timerStart(timer2);
        Sistema = Espera;
      break;

      case(Fin):
      break;

      case Espera: break;

    }
  
   /*Serial.print("t: ");

   Serial.println(timerRead(timer));*/

  


  /*switch(lectura){
    
  }

  Serial.println(lectura);
*/
  delay(10);
}
