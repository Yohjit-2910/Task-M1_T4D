// Documentation
// Name : Yohjit Chopra
// Roll No. 2110994798
// Distinction Task

// Define constants for the pins used in the program
#define Soil_moisture_sensor 2
#define PIR_sensor_1 3
#define PIR_sensor_2 4

#define Soil_Moisture_LED 8
#define PIR_LED_2 10
#define timer_LED 6

// Declare variables to keep track of the state of the LEDs
volatile bool PIR_LED_1_state = false;
volatile bool Soil_Moisture_LED_state = false;
volatile bool PIR_LED_2_state = false;
volatile bool timer_LED_state = false;

int i = 2; // A variable to track the timer value

// Function to set up the timer for the LED blink
void starttimmerfunc()
  {
     // Reset timer registers
  	TCCR1A = 0;
    TCCR1B = 0;
    TCNT1  = 0;
    // Set the timer compare value to 31249, which gives us a 2ms interval
    OCR1A = 31249; //2ms
    // Set the timer mode to CTC and prescaler to 1024
    TCCR1B |= (1 << WGM12);
    TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10);
    TIMSK1 |= (1 << OCIE1A);
  }
  
// Interrupt service routine for the timer compare interrupt
ISR(TIMER1_COMPA_vect){
    // Print the current timer value to the serial monitor
  Serial.print("Led Timmer: ");
  Serial.print(i);
  Serial.println(" milli seconds");
  // Toggle the state of the LED connected to pin 6
  digitalWrite(timer_LED, digitalRead(timer_LED) ^ 1);
  // Increment the timer value by 2ms
  i = i+2;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(Soil_Moisture_LED, OUTPUT);
  pinMode(PIR_LED_2, OUTPUT);
  pinMode(timer_LED, OUTPUT);
  
  pinMode(PIR_sensor_1, INPUT);
  pinMode(PIR_sensor_2, INPUT);
  pinMode(Soil_moisture_sensor, INPUT);
  
  Serial.begin(9600);
  starttimmerfunc();
  
  // Enable interrupts for the pins connected to the sensors
  noInterrupts();
  PCIFR |= bit(PCIE2);
  PCICR |= bit(PCIE2);
  PCMSK2 |= bit (PCINT18);
  PCMSK2 |= bit (PCINT19);
  PCMSK2 |= bit (PCINT20);
  interrupts();
  
}

void loop() {
 
}

ISR(PCINT2_vect)
{
  if(digitalRead(Soil_moisture_sensor) == HIGH) 
  {
    // Check if the soil moisture sensor has detected a rise in moisture level
    // Toggle the state of the LED connected to pin 8
    Soil_Moisture_LED_state = !Soil_Moisture_LED_state;
    digitalWrite(Soil_Moisture_LED, Soil_Moisture_LED_state);
    Serial.println("Soil Moisture Detected");
  }
  if(digitalRead(PIR_sensor_1) == HIGH) 
  {
    // Check if the PIR sensor connected to pin 3 has detected motion
    // Toggle the state of the LED
    PIR_LED_1_state = !PIR_LED_1_state;
    digitalWrite(LED_BUILTIN, PIR_LED_1_state);
    Serial.println("PIR-1 Motion Detected ");
  }
  if(digitalRead(PIR_sensor_2) == HIGH) 
  {
    PIR_LED_2_state = !PIR_LED_2_state;
  	digitalWrite(PIR_LED_2, PIR_LED_2_state);
  	Serial.println("PIR-2 Motion Detected ");
  }  
}





