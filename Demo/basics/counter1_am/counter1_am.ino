//------------------------
// Impulszähler
// Gibt die Impulse pro Sekunde 
// des Spannungsignals an Pin 7 aus 
//-------------------------
int pin = D2;
unsigned long N;          //Anzahl der Impulse
unsigned long T;          //Zeitintervall in us
unsigned long starttime;  //Startzeit


void setup(){
  Serial.begin(19200);
  pinMode(pin, INPUT);
  T = 1000;
}
 
void loop(){

 
  N = 0; 
  starttime = micros();
  do{  
     if (pulseIn(pin, HIGH)>0) N++;
    }
  while( micros() < (starttime + T) ); 
  
  Serial.print(N);

  Serial.print(" , ");
  Serial.println(digitalRead(pin));
    
}
