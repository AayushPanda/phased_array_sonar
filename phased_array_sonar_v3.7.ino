// Rewritten for Arduino Mega 2560
// byte results in faster execution

// (6 bits per pulse * 16 cycles per burst) + (13 max bits of offset per TXR * 8 TXR)
const int TXLength = 6*16 + 13*8;
const int c = 343; // speed of sound in m/s
const int pitch = 0.01; // distance between TXR nodes in m

byte phases[TXLength];

// Seperate function used for different signal patterns
void genChannelBits(byte arr[], int bitShift){
  for (int i = 0; i<16*6; i++) {
    if (i%6 <= 2) {
      arr[bitShift + i] = 1;
    }
  }
}

void genCompositeSignal(byte arr[], int bitDelay){
  for(int txr=0; txr<8; txr++){
    byte tmp[TXLength];
    for(int i=0; i<TXLength;i++){
        tmp[i] = 0;
    }
    genChannelBits(tmp, txr*bitDelay);
    for(int i=0; i<TXLength; i++){
      arr[i] += tmp[i]<<txr;
    }
  }
}

int f = 120; // Interrupt frequency in kHz
int clkspeed = 16000; // 16MHz clock speed on Atmega2560

void setup(){ 
  
  DDRA = 0xFF;         // set all pins on port A to output
  
  // Use Timer2 for interrupts.
  // 16MHz and 40kHz makes 199 for OCR2A in CTC mode
  // 16MHz/40kHz/2 = 200. minus 1 is 199.
  
  TCCR2A = 0;               // clear settings
  TCCR2B = 0;               // clear settings
  TCNT2 = 0;                // counter start at 0
  OCR2A = clkspeed/f/2 - 1; // count 199 with CTC mode
  TCCR2A = _BV(WGM21);      // CTC mode
  TCCR2B = _BV(CS20);       // start clock, no prescaler
  TIMSK2 |= (1 << OCIE2A);  // enable interrupt

  // Interrupts occur at 25/6us intervals, so 6 interrupts per 40khz cycle
  genCompositeSignal(phases, 1);
}

void loop(){}

byte counter = 0;

ISR(TIMER2_COMPA_vect){
  PORTA = phases[counter];
  counter++;
  if( counter >= 13*6 + 16*6)
    counter = 0;
}
