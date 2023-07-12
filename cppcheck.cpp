// Online C++ compiler to run C++ program online
#include <iostream>
#include <bitset>

const int TXLength = 6*16 + 13*8;
const int c = 343; // speed of sound in m/s
const int pitch = 0.01; // distance between TXR nodes in m


int phases[TXLength];

// Seperate function used for different signal patterns
void genChannelBits(int arr[], int bitShift){
  for (int i = 0; i<16*6; i++) {
    if (i%6 <= 2) {
      arr[bitShift + i] = 1;
    }
  }
}

void clear(int arr[]){
  for (int i=0; i < 16*6+13*6;i++){
      arr[i]=0;
  }
}

void genCompositeSignal(int arr[], int bitDelay){
  for(int txr=0; txr<8; txr++){
    int tmp[TXLength];
    clear(tmp);
    genChannelBits(tmp, txr*bitDelay);
    for(int i=0; i<TXLength; i++){
      arr[i] += tmp[i]<<txr;
    }
  }
}

int main() {
    // Write C++ code here
    genCompositeSignal(phases, 2);
    for(int i=0; i<TXLength; i++){
        std::cout << std::bitset<8>(phases[i]) << "\n";
    }
    return 0;
}