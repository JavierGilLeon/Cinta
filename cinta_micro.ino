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


int newData = 0;
char receivedChar = 0;

typedef struct{
  int vel;
  int d;
} CINTA;

CINTA Cinta;

void ARDUINO_ISR_ATTR bombaIsr() {
  digitalWrite(bombaIn1, LOW);
  timerStop(timer1);
  Sistema = BoteLleno;
}

void ARDUINO_ISR_ATTR cintaIsr() {
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


void recvOneChar() {
    if (Serial.available() > 0) {
        receivedChar = Serial.read();
        newData = true;
    }
}

void setup() {
  pinMode(sensor, INPUT);     //
  pinMode(cintaIn1, OUTPUT);  //
  pinMode(cintaIn2, OUTPUT);  //

  pinMode(bombaIn1, OUTPUT);  //
  pinMode(bombaIn2, OUTPUT);  //

  int ml;


  Serial.begin(115200);

  delay(1000);
  
  // --- Solicitar datos al usuario ---
  Serial.println("Introduce la velocidad (0 - 2.2): ");
  while (Serial.available() == 0) {}
  Cinta.vel = Serial.parseFloat();

  Serial.print("Velocidad recibida: ");
  Serial.println(Cinta.vel);

  Serial.println("Introduce los ml a llenar: ");
  while (Serial.available() == 0) {}
  ml = Serial.parseInt();

  Serial.print("Mililitros recibidos: ");
  Serial.println(ml);

  Serial.println("Introduce la distancia: ");
  while (Serial.available() == 0) {}
  Cinta.d = Serial.parseInt();

  Serial.print("Distancia recibida: ");
  Serial.println(Cinta.d);

  // Timer Divider

  timer1 = timerBegin(1000000);
  timer2 = timerBegin(1000000);

  if (timer1 == NULL) Serial.println("Timer1 no iniciado");
  if (timer2 == NULL) Serial.println("Timer2 no iniciado");

  timerAttachInterrupt(timer1, &bombaIsr);
  timerAttachInterrupt(timer2, &cintaIsr);


  

  int t1 = map(ml,0,32,0,30000000);
  int t2 = map(Cinta.d,0,10,0,(Cinta.d/Cinta.vel)*1000000);
  
  timerAlarm(timer1, t1, true, 0);
  timerStop(timer1);

  timerAlarm(timer2, t2, true, 0);
  timerStop(timer2);

  delay(5000);

  /*int n = 3;
  while(n){
    if(!newData)
      recvOneChar();

    else
      {
        switch(n){
          case 3: Cinta.vel = receivedChar; break;
          case 2: ml        = receivedChar; break;
          case 1: Cinta.d   = receivedChar; break;
        }
        newData = 0;
        n -= 1;
      }
  }
 */ 
}

void loop() {
    float vel = 2;
    float ml = 10;
    int lectura;

     lectura = analogRead(sensor);
    

    switch(Sistema){
      case MoverCinta:
        MoveCinta();

        
        if (!lectura)
          Sistema = LlenaBote;
        break;

      case LlenaBote:
        Serial.println("Llenando el bote");

        // Parar la cinta
        analogWrite(cintaIn1, 0);
        analogWrite(cintaIn2, 0);
        
        // Encender la bomba
        digitalWrite(bombaIn1,HIGH);
        digitalWrite(bombaIn2,LOW);
        
        // Poner el timer en funcion de los ml
        timerStart(timer1);
        Sistema = Espera;
      break;

      case(BoteLleno):
        MoveCinta();
        Serial.println("Cinta activada");
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