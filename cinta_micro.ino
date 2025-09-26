#define sensor 13

#define cintaIn1 12
#define cintaIn2 14

#define bombaIn1 25
#define bombaIn2 26

typedef enum { Stop = 0,
               Run = 1,
               Fill = 2
} Estados;

Estados cinta = Run;
hw_timer_t *timer = NULL;


int Started = 0;


void ARDUINO_ISR_ATTR timerIsr() {
  Serial.println("Interrupt");

  digitalWrite(bombaIn1, LOW);

  cinta = Stop;
}


void MoveCinta(float vel){

  if(vel < 0 || vel > 2.2) return;

  vel = map(vel,0,2.2,0,255);

  analogWrite(cintaIn1, vel);
  analogWrite(cintaIn2, 0);
}

void LlenaBote(int ml){
  long int t;
  t = map(ml,0,32,0,30000000);

  if(!Started){
  digitalWrite(bombaIn1,HIGH);
  digitalWrite(bombaIn2,LOW);

  timerAlarm(timer, t, true, 0);
  timerStart(timer);
  Started = 1;
  }
}


void setup() {
  pinMode(sensor, INPUT);     //
  pinMode(cintaIn1, OUTPUT);  //
  pinMode(cintaIn2, OUTPUT);  //

  pinMode(bombaIn1, OUTPUT);  //
  pinMode(bombaIn2, OUTPUT);  //

  Serial.begin(115200);

  timer = timerBegin(1000000);
  if (timer == NULL) Serial.println("Timer no iniciado");

  timerAttachInterrupt(timer, &timerIsr);


  //digitalWrite(bombaIn1, HIGH);*/
}

void loop() {
    float vel = 2;
    float ml = 10;

    switch(cinta){
      case Run:
        if(digitalRead(sensor)) cinta = Fill;
        MoveCinta(vel);    
        break;

      case Fill:
        MoveCinta(0);
        LlenaBote(ml);
        break;
      
      case Stop: break;
    }
  
   /*Serial.print("t: ");

   Serial.println(timerRead(timer));*/

  int lectura = digitalRead(sensor);
  


  /*switch(lectura){
    
  }

  Serial.println(lectura);
*/
 // delay(100);
}
