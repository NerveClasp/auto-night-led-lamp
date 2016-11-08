/*
 * So we have two sensors. This version of the software for the Arduino works in the following way:
 * Whenever any of the two triggers can see an object closer than 150 cm, it fires up both led lamps and they
 * are lit until there is no interference with the sensor.
 * The sensor is cecking for an object every second. Every time there is no interference the idleCounter increments.
 * If the idleCounter becomes more than wait, the sensor starts checking for the interference less frequetly to save up some energy.
 */

int inFirst = 8;//first sensor's echo pin
int pingFirst = 9;//first sensor's trig pin

int inSecond = 10;//second sensor's echo pin
int pingSecond = 11;//second sensor's trig pin

int ledFirst = 12;//first led's power pin
int ledSecond = 13;//second led's power pin
String inFirst, inSecond = "out"; 

int idleCounter, afterLife = 0;
int maxLife = 20;
double wait = 120000;

void setup() {
  Serial.begin(9600);
  pinMode(inFirst, INPUT);
  pinMode(inSecond, INPUT);
  pinMode(pingFirst, OUTPUT);
  pinMode(pingSecond, OUTPUT);
  pinMode(ledFirst, OUTPUT);
  pinMode(ledSecond, OUTPUT);
  digitalWrite(pingFirst, LOW);
  digitalWrite(pingFirst, LOW);
}
 
void loop() {
  long duration, cmFirst, cmSecond,tmp1;
  //sending the first sensor's signal (high level for 10 mcs)
  digitalWrite(pingFirst, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingFirst, LOW);
  //making a pause so that the sensor can react
  delayMicroseconds(500);
  //detecting the time in miliseconds
  tmp1=micros();
  //waiting until the echo in signal becomes low
  while(digitalRead(inFirst)==HIGH){
    //if there is no respond from the sensor, that means there are no objects closer than max distance. Exiting
    if(micros()-tmp1 > 15000)
      break;
  }
  //counting the time it took for the signal to come back
  duration=micros()-tmp1;
  cmFirst = microsecondsToCentimeters(duration);//converting the time into santimeters
  
  Serial.print("Distance: ");
  //do the same for the second sensor
  digitalWrite(pingSecond, HIGH);
  delayMicroseconds(10);
  digitalWrite(pingSecond, LOW);
  delayMicroseconds(500);
  tmp1=micros();
  while(digitalRead(inSecond)==HIGH){
    if(micros()-tmp1 > 15000)
      break;
  }
  duration=micros()-tmp1;
  cmSecond = microsecondsToCentimeters(duration);
  /*
   * this is for the main branch, will detele later (still new to all this git stuff, sorry))
  if(cmFirst < 150 && inSecond != "in"){
    inFirst = "in";
  }else if (cmFirst < 150){
    inFirst = "out";
  }
  if(cmSecond < 150 && inFirst != "in"){
    inSecond = "in";
  }else if (cmSecond < 150){
    inSecond = "out";
  }
  
  if(cmSecond < 150){
    inSecond = true;
  }
  */
  if(cmFirst < 150 || cmSecond < 150){
    idleCounter = 0;
    Serial.println(cmSecond);
    Serial.println("Turning on the lights");
    analogWrite(ledFirst, 255);
    analogWrite(ledSecond, 255);
    afterLife = maxLife;
    
    
    delay(1000);
  }else{
//    if(afterLife == 0 && inFirst == "out" || inSecond == "out"){
    if(afterLife == 0){
      analogWrite(ledFirst, 0);
      analogWrite(ledSecond, 0);
    }
    Serial.print("Idle Counter: ");
    Serial.println(idleCounter);
    idleCounter++;
    delay(1000);
  }
  if(afterLife != 0){
    afterLife--;
  }
  if(idleCounter > wait){
    delay(120000);
  }
}
long microsecondsToCentimeters(long microseconds)
{
  //the speed of the sound is 340 m/s or 29412 microseconds/sm, and hence the sound travels to the obstacle and back, we divide it by two
  return microseconds / 29.412 / 2;
}



