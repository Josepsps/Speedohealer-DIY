//1986 Porsche 944 Turbo Speedometer Calibrator
//Public Domain
//raw 944 transmission sensor signal must be processed by external schmitt trigger
//Schmitt trigger should bypass filer and pull-up resistor
//Use LM2940-10 with decoupling caps per datasheet to power Arduino in automotive setting
//Arduino Pin 7 supplies modified signal for speedometer
 
const int debounce = 2500;
const int speedometerPin = 7;                    
const int sensorPin = 3;                                     
int pulseState = LOW;                                        
volatile unsigned long currentMicros = 0; 
volatile unsigned long previousMicros= 0;  
volatile unsigned long currentSpeed = 0;  
volatile unsigned long previousSpeed = 0;
volatile unsigned long interval = 0;                 
unsigned long modInterval = 0;                
float calFactor = .91;   // decrease to slow down speedometer                           
                                     // calFactor of 1 makes no change to speedometer
 
void setup()
{
pinMode (13, OUTPUT);                        
pinMode(speedometerPin, OUTPUT);    
pinMode(sensorPin, INPUT);                 
digitalWrite (sensorPin, HIGH);             
attachInterrupt (1, iSr, FALLING);        
}
 
void loop()
{
  noInterrupts();                                                 
  modInterval=interval;                  //                  
  interrupts();                                                      
  currentMicros = micros();                   //get current time             
  if (currentMicros-previousSpeed<1000000)    //si tiempo actual-tiempoInterrupcion < 1 segundo   (¿siempre?)
  {
   if (currentMicros - previousMicros>((modInterval/2)/calFactor))          // si tiempo actual-tiempo previo >  (tiempo entre interrupciones/2  / correccion)
       { previousMicros = currentMicros;                                                    
        if (pulseState == LOW) pulseState = HIGH; else pulseState = LOW;
        digitalWrite(13, pulseState);         //to blink onboard LED                                                         
        digitalWrite(speedometerPin, pulseState);
        }                                  
   }
}
 
void iSr()
{
  currentSpeed=micros();                   
  if (digitalRead(sensorPin)==LOW) 
     {
        if ((currentSpeed - previousSpeed) > debounce)  
           {
            interval = currentSpeed - previousSpeed;        // interval tiempo entre interrupciones
            previousSpeed=currentSpeed;                     //    
           }
      }
}