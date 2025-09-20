#define sensor 13

#define in1 12
#define in2 14

typedef enum{Stop = 0,
             Run  = 1
}Estados;

Estados cinta = Run;
hw_timer_t *timer = NULL;



void IRAM_ATTR timerInterrupt() {

}


void setup()
{
  pinMode(sensor, INPUT); //
  pinMode(in1, OUTPUT); //
  pinMode(in2, OUTPUT); //
  Serial.begin(115200);

  timer = timerBegin(timer,)
}
void loop()
{

 int lectura = digitalRead(sensor);

  switch(lectura){
    case Stop:
      digitalWrite(in1,HIGH);
      digitalWrite(in2,LOW);
    break;
    
    case Run:
      digitalWrite(in2,HIGH);
      digitalWrite(in1,LOW);
    break;

    default: 
      lectura = Run; 
  }

  Serial.println(lectura);

  delay(100);
}



