// Rewritten for Arduino Mega 2560
// uint8_t results in faster execution

#include <avr/sleep.h>
#include <avr/power.h>

// 6 bits per wave
const int TXLength = 6;
const int c = 343; // speed of sound in m/s
const int pitch = 0.01; // distance between TXR nodes in m

uint8_t phases[TXLength];

// Seperate function used for different signal patterns
void genChannelBits(uint8_t arr[], int bitShift){
  for (int i = 0; i<TXLength; i++) {
    if (i%6 <= 2){
        arr[(bitShift + i)%TXLength] = 1;
    }
  }
}

void genCompositeSignal(uint8_t arr[], int bitDelay){
  for(int txr=0; txr<8; txr++){
    uint8_t tmp[TXLength];
    for(int i=0; i<TXLength;i++){
        tmp[i] = 0;
    }
    genChannelBits(tmp, (txr*bitDelay)%6);
    for(int i=0; i<TXLength; i++){
      arr[i] += tmp[i]<<txr;
    }
  }
}

int f = 120; // Interrupt frequency in kHz
int clkspeed = 16000; // 16MHz clock speed on Atmega2560

void setup(){ 

  DDRA = 0xFF;         // set all pins on port A to output
  PORTA = 0x00;        // set all pins on port A to 0 (low)

  // Use Timer2 for interrupts.
  // 16MHz and 40kHz makes 199 for OCR2A in CTC mode
  // 16MHz/40kHz/2 = 200. minus 1 is 199.
  noInterrupts();           // disable all interrupts
  TCCR2A = 0;               // clear settings
  TCCR2B = 0;               // clear settings
  TCNT2 = 0;                // counter start at 0
  OCR2A = 1; // count 199 with CTC mode
  TCCR2A = 1 << WGM21;      // CTC mode
  TCCR2B = 1 << CS20;       // start clock, no prescaler (0b00000001)
  TIMSK2 |= (1 << OCIE2A);  // enable interrupt on compare match A
  interrupts();

  // disable everything unneeded to prevent jitter
  ADCSRA = 0;  // ADC
  power_adc_disable ();
  power_spi_disable();
  power_twi_disable();
  power_timer0_disable();
  power_timer1_disable();
  power_timer3_disable();
  power_timer4_disable();
  power_timer5_disable();
  power_usart1_disable();
  power_usart2_disable();
  power_usart3_disable();

  // Interrupts occur at 25/6us intervals, so 6 interrupts per 40khz cycle
  genCompositeSignal(phases, 1);
}

void loop(){}

uint8_t counter = 0;

ISR(TIMER2_COMPA_vect){
  PORTA = phases[counter];
  counter++;
  if(counter == TXLength){
    counter = 0;
  }
}