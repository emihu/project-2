#include <avr/eeprom.h>
#include <avr/io.h>
#include <util/delay.h>

int timer=0;
bool state=0;
int count = 0;

void setup()
{
  pinMode (A0, INPUT);
  Serial.begin(9600);
  
  TCCR0A=(1<<WGM01);    //Set the CTC mode   
  OCR0A=0xF9; //Value for ORC0A for 1ms 
  
  TIMSK0|=(1<<OCIE0A);   //Set the interrupt request
  sei(); //Enable interrupt
  
  TCCR0B|=(1<<CS01);    //Set the prescale 1/64 clock
  TCCR0B|=(1<<CS00);
}

ISR(TIMER0_COMPA_vect){    //This is the interrupt request
  timer++;
}

void timer_delay(uint8_t seconds) {
  int max_time = seconds*980;
   if(timer>= max_time){
    state=!state;
    timer=0;
     //Serial.println(count++);
  }
}



void loop()
{
  timer_delay(3);
  if (state) {
  int read = analogRead(A0);
  int celsius = (read*5.0 / 1024 - 0.5) * 100;
  float* address = 0;  
  if(celsius < 20) {
    address = new float;
  }
  eeprom_write_float(address, celsius);
  state = !state;
  
    //to test
  Serial.println("Value at address:");
  Serial.println(eeprom_read_float(address)); 
 }
}

