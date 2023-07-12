volatile byte enableBits;    // 0 is disabled, 1 is enabled for the 6 lowest bits.
byte counter = 0;

void genBits(byte arr[], int bitShift) {
  for (int i = 0; i<16*6; i++) {
    if (i%6 <= 2) {
      arr[bitShift + i] = 1;
    }
  }
}

void clear(byte arr[]){
  for (int i=0; i < 16*6+13*6;i++){
      arr[i]=0;
  }
}

void genPSSignal(byte arr[], int ps) {
  for (int txr=0; txr<6; txr++){

      //genBits(prePhases[ps][txr], txr*ps);

      byte tmp[16*6 + 13*6];
      clear(tmp);
      // for (int a : tmp) cout<<a<<",";
      // cout<<"\n";
      genBits(tmp, txr*ps);

      for (int t=0; t<16*6+13*6; t++) {
        arr[t] += tmp[t] * (1<<txr);
        // phases[ps][t] += prePhases[ps][txr][t] * (1<<txr);
      }

  }
}

//int prePhases[13+1][6][13*6 + 16*6];
byte phases[16*6+13*6];

int f = 120; //in kHz
int clkspeed = 16000;
void setup()
{ 

  // {13 bit offset * 6 width}{16 wide burst * 6 bit width per pulse} | 6 txr nodes | 13 unique phase shifts + 1 for straight


  // for (int ps=0; ps<13+1; ps++) {

  // }
  genPSSignal(phases, 0);

  DDRB = B11111111;         // set all pins on port B to output

  
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

}


void loop()
{
  
}

ISR(TIMER2_COMPA_vect)
{
  // At 40kHz, every little piece of code matters.
  
  //PORTB = (1*phases[counter] + 2*phases[counter-1] + 4*phases[counter-2]) & enableBits;
  PORTB = phases[counter];
  counter++;
  if( counter >= 13*6 + 16*6)
    counter = 0;
}
