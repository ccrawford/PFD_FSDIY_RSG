// This is for a FSDIY G1000 Control Board v2.3
// Using a Arduino Mega Pro Mini.


// #define ENCODER_OPTIMIZE_INTERRUPTS
#define ENCODER_DO_NOT_USE_INTERRUPTS  // There are not many interrupts on the Mega. Keep this off or else the pins that *do* support interrupts break!

#include <Mux.h>
#include <Encoder.h>
#include "Button.h"

using namespace admux;
Mux muxSoft(Pin(33, INPUT_PULLUP, PinType::Digital), Pinset(41, 39, 37, 35)); //SIG pin, S0-S3 pins, enable pin: NO ENABLE PIN!!


// define mux names and set button status to not pressed (up to 16 inputs per mux) THE ORDER MATTERS!

// The names ("BTN_NAV_FF" etc) come from the RSG CommandMapping.ini file

String muxSofttxt[] = {"BTN_NAV_VOL_CLICK", "BTN_NAV_FF", "BTN_NAV_TOG", "BTN_ALT_SEL ", "BTN_SOFT_1", "BTN_SOFT_2", "BTN_SOFT_3", "BTN_SOFT_4", "BTN_SOFT_5", "BTN_SOFT_6", "BTN_SOFT_7", "BTN_SOFT_8", "BTN_SOFT_9", "BTN_SOFT_10", "BTN_SOFT_11", "BTN_SOFT_12"};
bool muxSoftstatus[] = {false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};

//rotary handler section
class RotaryHandler {
public:
  virtual  bool update() = 0;
};

class QuadPulseRotaryHandler : public RotaryHandler {
  Encoder enc;
  long state = -999;
  const char* up;
  const char* down;
  int pulsecount = 4; //*** set this for number of pulses per click on your encoders (4 most common)
  
public:
  QuadPulseRotaryHandler(const Encoder &enc, const char* up, const char* down) : enc{enc}, up{up}, down{down} {}
  QuadPulseRotaryHandler(const Encoder &enc, const char* up, const char* down, int pulsecount) : enc{enc}, up{up}, down{down}, pulsecount{pulsecount} {}
  bool update() {
    long new_state = enc.read()/pulsecount;
    if (new_state > state){ //turning clockwise
      Serial.write(up);
      state = new_state;
      return true;
    } else if (new_state < state) { //turning counterclockwise
      Serial.write(down);    
      state = new_state;
      return true;
    }
    return false;
  }
};



// define number of buttons
#define NUM_BUTTONS 29 //total number of buttons (not on muxes)
Button Buttons[NUM_BUTTONS];

// define number of encoders
#define NUM_ENCODERS 14 //total number of encoders
RotaryHandler *Encoders[NUM_ENCODERS];

// define encoders  (name, pin pair for A and B pins) if the direction is backwards, flip the pin pairs.
Encoder ENC_RANGE(15,17);
Encoder ENC_BARO(20,22);
Encoder ENC_CRS(24,26);
Encoder ENC_COM_OUTER(30,31);
Encoder ENC_COM_INNER(29,27);
Encoder ENC_NAV_OUTER(55,57);
Encoder ENC_NAV_INNER(59,61);
Encoder ENC_ALT_OUTER(51,53);
Encoder ENC_ALT_INNER(52,50);
Encoder ENC_HDG(54,56);
Encoder ENC_NAV_VOL(65,63);
Encoder ENC_COM_VOL(19,21);
Encoder ENC_FMS_INNER(2,4);
Encoder ENC_FMS_OUTER(45,43);


void setup() {

  // start serial connection
  Serial.begin(115200);

  muxSoft.enabled(false);

  uint8_t i = 0;
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_FMS_INNER, "ENC_FMS_INNER_UP\n", "ENC_FMS_INNER_DN\n",2};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_FMS_OUTER, "ENC_FMS_OUTER_UP\n", "ENC_FMS_OUTER_DN\n",2};   
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_NAV_VOL, "ENC_NAV_VOL_UP\n", "ENC_NAV_VOL_DN\n",4};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_COM_VOL, "ENC_COM_VOL_UP\n", "ENC_COM_VOL_DN\n",4};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_NAV_OUTER, "ENC_NAV_OUTER_UP\n", "ENC_NAV_OUTER_DN\n",2};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_NAV_INNER, "ENC_NAV_INNER_UP\n", "ENC_NAV_INNER_DN\n",2};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_HDG, "ENC_HDG_UP\n", "ENC_HDG_DN\n",4};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_ALT_OUTER, "ENC_ALT_OUTER_UP\n", "ENC_ALT_OUTER_DN\n",2};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_ALT_INNER, "ENC_ALT_INNER_UP\n", "ENC_ALT_INNER_DN\n",2};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_COM_OUTER, "ENC_COM_OUTER_UP\n", "ENC_COM_OUTER_DN\n",2};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_COM_INNER, "ENC_COM_INNER_UP\n", "ENC_COM_INNER_DN\n",2};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_RANGE, "ENC_RANGE_UP\n", "ENC_RANGE_DN\n",4};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_BARO, "ENC_BARO_UP\n", "ENC_BARO_DN\n",2};
  Encoders[i++] = new QuadPulseRotaryHandler{ENC_CRS, "ENC_CRS_UP\n", "ENC_CRS_DN\n",2};



 // init non-mux buttons
  i = 0;
  // Auto repeat buttons are the first 4
  Buttons[i++].initialize("BTN_PAN_LEFT", 9, 10, true);  
  Buttons[i++].initialize("BTN_PAN_UP", 11, 10, true);  
  Buttons[i++].initialize("BTN_PAN_RIGHT", 5, 10, true);  
  Buttons[i++].initialize("BTN_PAN_DN", 7, 10, true);
    
  Buttons[i++].initialize("BTN_FMS", 49, 10);
  Buttons[i++].initialize("BTN_MENU", 69, 10);  
  Buttons[i++].initialize("BTN_PROC", 8, 10);  
  Buttons[i++].initialize("BTN_ENT", 67, 10);  
  Buttons[i++].initialize("BTN_DIRECT", 16, 10);  
  Buttons[i++].initialize("BTN_FPL", 14, 10);  
  Buttons[i++].initialize("BTN_CLR", 12, 10);  
  Buttons[i++].initialize("BTN_PAN_SYNC", 3, 10);  
  Buttons[i++].initialize("BTN_COM_VOL_CLICK", 23, 10);  
  Buttons[i++].initialize("BTN_COM_FF", 25, 10);  
  Buttons[i++].initialize("BTN_COM_TOG", 28, 10);  
  Buttons[i++].initialize("BTN_CRS_SYNC", 18, 10);  


  Buttons[i++].initialize("BTN_AP", 60, 10);
  Buttons[i++].initialize("BTN_FD",40 , 10);
  Buttons[i++].initialize("BTN_ALT", 42, 10);
  Buttons[i++].initialize("BTN_HDG", 62, 10);
  Buttons[i++].initialize("BTN_NAV", 32, 10);
  Buttons[i++].initialize("BTN_BC", 46, 10);
  Buttons[i++].initialize("BTN_NOSE_UP", 47, 10);
  Buttons[i++].initialize("BTN_APR", 34, 10);
  Buttons[i++].initialize("BTN_VS", 36, 10);
  Buttons[i++].initialize("BTN_FLC", 38, 10);
  Buttons[i++].initialize("BTN_NOSE_DN", 48, 10);
  Buttons[i++].initialize("BTN_ALT_SEL", 44, 10);  // Is this VNV?
  Buttons[i++].initialize("BTN_HDG_SYNC", 58, 10);
  //Buttons[i++].initialize("BTN_NAV_TOG", , 10);
  
}

unsigned long lastKeepAlive = 0;
unsigned long lastKeepAliveLong = 0;
unsigned long lastHeld = 0;

void loop() {

  for (uint8_t i = 0; i < NUM_ENCODERS; ++i) {
    // If one of the encoders is changing, short circuit the other checks to avoid chatter.
    if (Encoders[i]->update()) continue;
  }
  
// keep alive for RSG connection
  if(millis() - lastKeepAliveLong > 3000 ) {
    Serial.write("\\####RealSimGear#RealSimGear-G1000XFD1#1#3.1.9#756E6B776F06E/\n"); // 3.1.9 = latest firmware; 756E6B776F06E = RANDOM ID
    lastKeepAliveLong = millis();
    lastKeepAlive = millis();
  }
  if(millis() - lastKeepAlive > 100 ) {
    Serial.write("\n"); 
    lastKeepAlive = millis();
  }
  
// check for non-mux buttons pressed/released
  for(uint8_t i=0; i<NUM_BUTTONS; i++){
    Buttons[i].update();
  }
  //end of non-mux button check
 
  // check for muxSoft buttons pressed/released 
  byte dataSoft;
  for(byte i=0; i< muxSoft.channelCount(); i++){
      dataSoft = muxSoft.read(i); //get value from selected channel
      if(dataSoft == LOW && muxSoftstatus[i] == false) {
        muxSoftstatus[i] = true;
        Serial.write(String(muxSofttxt[i] + "=1\n").c_str());
       }
      else if (dataSoft == HIGH && muxSoftstatus[i] == true) {
        muxSoftstatus[i] = false;  
        Serial.write(String(muxSofttxt[i] + "=0\n").c_str());
      }
  }
  // end of muxSoft buttons pressed/released 

  
}