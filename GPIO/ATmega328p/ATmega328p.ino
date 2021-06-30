
/**
 * SIZE: 860 Bytes
 */

  //pin assignments
const uint8_t RED = 7;
const uint8_t BLUE = 6;
const uint8_t GREEN = 5;
const uint8_t BUTTON = 2;

  //timer interrupt
const uint8_t TCNT2_COMP = 125;
const uint16_t COUNTER_GOAL = 375;
uint16_t counter = 0;

  //pwm signal (changed from analog read of the pot on pin A2)
uint8_t DUTY_CYCLE_COMP = 75;



void setup() {
    /**
     * DIGITAL I/O
     * set Data Direction Register for Port-D to 10100000 so 5 and 7 are output, rest are input
     */
  DDRD |= 1 << RED;
  DDRD |= 1 << GREEN;

    /**
     * HARDWARE INTERRUPTS
     * set Pin Change Interrupt Control Register to 00000100 so that Pin Change Interrupt Enable 2 listens (PCINT 23 ... 16)
     * set Pin Change Mask Register 2 to 00000100 so arduino pin 2 can interrupt
     */
  PCICR |= 1 << PCIE2;
  PCMSK2 |= 1 << BUTTON;



    /**
     * TIMER
     * clear Timer/Counter Control Register 2A of any preset functionality
     * set Timer/Counter Control Register 2B to 00000100 for a 256 prescalar
     * reset Timer/Counter Register
     * set Output Compare Register 2A to the desired clock pulses --[ (desired_seconds * clock_Hz) / prescalar = desired_pulses ]--
     * set Timer/Counter 2 Interrupt Mask to 00000010 to enable it
     */
  TCCR2A = 0;
  TCCR2B |= 1 << CS22;
  TCNT2 = 0;
  OCR2A = TCNT2_COMP;
  TIMSK2 |= 1 << OCIE2A;


    /**
     * ANALOG READ
     * set the ADC Multiplexer Selection Register to 01000010 for internal reference voltage (0100) and arduino pin A2 (0010) see tables 23.3 and 23.4 in datasheet
     * set the ADC Control and Status Register to 10000011  for enable (10000) and 8-bit prescalar (0011)
     */
  ADMUX = 0;
  ADMUX |= 1 << REFS0;
  ADMUX |= 1 << MUX1;
  ADCSRA = 0;
  ADCSRA |= 1 << ADEN;
  ADCSRA |= 1 << ADPS0;
  ADCSRA |= 1 << ADPS1;


    /**
     * FAST PWM (counter goes to 255, set OCR to percent of overflow to define duty cycle)
     * set Timer/Counter Control Register 0A to 10000011 for Non-Inverted (1000) Fast PWM (0011) on output A
     * set cs bits of Timer/Counter Control Register 0B to 011 for a prescalar of 64 --> 976.6Hz
     * set Output Compare Register for Timer 0 output A to the desired duty cycle (0-255)
     */
  TCCR0A |= 1 << COM0A1; 
  TCCR0A |= 1 << WGM01;
  TCCR0A |= 1 << WGM00;
  TCCR0B |= 1 << CS01; 
  TCCR0B |= 1 << CS00;
  OCR0A = DUTY_CYCLE_COMP;
  
}//end setup()





void loop() {
  
    /**
     * Take an analog reading of what's on port A2
     */
  uint8_t x = 0; //dummy counter
  const uint8_t samples = 50;
  uint16_t sum = 0;
  for(int i = 0; i < samples; i++){
    ADCSRA |= 1 << ADSC; 
    while(ADCSRA >> ADSC & (1 << ADSC) >> ADSC){
      //just passing time while ADSC bit is still high, drops low after sample rate
      x++;
    }
    sum += ADC; //grab the value from the converter
  }
  uint16_t reading = sum / samples;
    /** 
     * use these lines to get a voltage value instead 
     * //float adc_voltage_conversion = 5.0/1023.0;
     * //float voltage = sum * adc_voltage_conversion / samples;
     */
  
  
  
    /**
     * Set PWM duty cycle comparator goal according to the analog read
     */
  DUTY_CYCLE_COMP = 255 - reading/4; //pot has 1023 on left and 0 on right so flip that to get duty cycle
  OCR0A = DUTY_CYCLE_COMP;


    /**
     * reset multiplexer for next loop
     */
  ADMUX = 0; 
  ADMUX |= 1 << REFS0;
  ADMUX |= 1 << MUX1;
}//end loop()




ISR (PCINT2_vect){
    /**
     * if port reads high on pin button is connected to turn LED on, else turn it off
     */
  if(PIND >> BUTTON & (1 << BUTTON) >> BUTTON){
    PORTD |= 1 << GREEN;
  }
  else{
    PORTD &= ~(1 << GREEN);
  }
}//end interrupt service routine





ISR (TIMER2_COMPA_vect){
  counter++;
  if(counter == COUNTER_GOAL){
      /**
       * Toggle the On/Off state of the LED's when the counter hits the goal
       * reset for the next 3/4 of a second
       */
    PORTD ^= 1 << RED;
    DDRD ^= 1 << BLUE; //lose on/off control with pwm so toggling whether it's an input or output
    counter = 0;
    TCNT2 = 0; //reset timer
  }
}//end interrupt service routine
