#include <ILI9341_T4.h> 
#include <tgx.h>
#include <Bounce2.h>
#include <font_Roboto_Bold.h>

#include <MIDI.h>

#include "Menus/m_MainMenu.h"
#include "Menus/m_SignalGenerator.h"
#include "Menus/m_Oscilators.h"
#include "Menus/m_OscilatorTemplate.h"

#include "Menus/s_Synthesizer.h"

//#include "SGearFrame.h"
//#include "bg.h"

//
// DEFAULT WIRING USING SPI 0 ON TEENSY 4/4.1
//
#define PIN_SCK         13        // mandatory
#define PIN_MISO        12        // mandatory
#define PIN_MOSI        11        // mandatory
#define PIN_DC          36        // mandatory, can be any pin but using pin 10 (or 36 or 37 on T4.1) provides greater performance

#define PIN_CS          26        // optional (but recommended), can be any pin.  
#define PIN_RESET       28        // optional (but recommended), can be any pin. 
#define PIN_BACKLIGHT   29        // optional, set this only if the screen LED pin is connected directly to the Teensy.
#define PIN_TOUCH_IRQ   255       // optional. set this only if the touchscreen is connected on the same SPI bus
#define PIN_TOUCH_CS    255       // optional. set this only if the touchscreen is connected on the same spi bus
#define SPI_SPEED       40000000  // SPI speed

#define MIDISerial      Serial8

#define BUTTON1         30
#define BUTTON2         31
#define BUTTON3         32

#define BUTTON_DEBOUNCE_SPEED 25  //In ms.

#define DIAL1           A14
#define DIAL2           A15
#define DIAL3           A16

// namespace for draw graphics primitives
using namespace tgx;

// framebuffers
DMAMEM uint16_t ib[240 * 320];  // used for internal buffering
DMAMEM uint16_t fb[240 * 320];  // paint in this buffer

// the screen driver object
DMAMEM ILI9341_T4::ILI9341Driver tft(PIN_CS, PIN_DC, PIN_SCK, PIN_MOSI, PIN_MISO, PIN_RESET, PIN_TOUCH_CS, PIN_TOUCH_IRQ);

// two diff buffers
DMAMEM ILI9341_T4::DiffBuffStatic<6000> diff1;
DMAMEM ILI9341_T4::DiffBuffStatic<6000> diff2;

//Image that encapsulates framebuffer
DMAMEM Image<RGB565> im(fb, 320, 240);

//Midi Create Object
MIDI_CREATE_INSTANCE(HardwareSerial, MIDISerial, MIDI);

//Menu Pointers and Objects (Used for switching menus)
Menu* PrevMenu;
Menu* CurrentMenu;
Menu* NextMenu;

s_Synthesizer Synth;

m_MainMenu MainMenu(&im);
m_SignalGenerator SignalGenerator(&im);
m_Oscilators Oscilators(&im, &Synth);
m_OscilatorTemplate Oscilator0(&im, &Synth, 0);
m_OscilatorTemplate Oscilator1(&im, &Synth, 1);

//Button Objects
Bounce b1 = Bounce();
Bounce b2 = Bounce();
Bounce b3 = Bounce();

bool Transition_Switch1 = false;
bool Transition_Switch2 = false;
bool Transition_Switch3 = false;

//Dials
int Dial1Value;
int Dial2Value;
int Dial3Value;

RGB565 Transition_Color;

int TimeTransOUTDone;


void setup() {
  Serial.println("Beginning Setup.");

  Serial.println("Linking Menus...");
  MainMenu.Link(&SignalGenerator, &MainMenu, &MainMenu);
  SignalGenerator.Link(&MainMenu, &Oscilators, &SignalGenerator);
  Oscilators.Link(&Oscilator0, &SignalGenerator, &Oscilator1);
  Oscilator0.Link(&Oscilators, nullptr, nullptr);
  Oscilator1.Link(&Oscilators, nullptr, nullptr);
  Serial.println("Menus Linked!");

  Serial.println("Setting up Screen...");
  setUpScreen();
  Serial.println("Screen Set up!");

  // clear display black
  im.fillScreen(RGB565_Black);

  //Midi setup.
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn(MIDI_NoteOn);
  MIDI.setHandleNoteOff(MIDI_NoteOff);
  MIDI.setHandlePitchBend(MIDI_PitchBend);
  MIDI.setHandleControlChange(MIDI_ControlChange);
  
  //Button Setup
  Serial.println("Attaching and Activating Buttons...");
  b1.attach(BUTTON1, INPUT);
  b2.attach(BUTTON2, INPUT);
  b3.attach(BUTTON3, INPUT);
  b1.interval(BUTTON_DEBOUNCE_SPEED);
  b2.interval(BUTTON_DEBOUNCE_SPEED);
  b3.interval(BUTTON_DEBOUNCE_SPEED);
  Serial.println("Buttons Created!");
  
  //Initializing the Main loop.
  Serial.println("initializing Current Menu...");
  CurrentMenu = &Oscilator0;
  CurrentMenu->Setup();
  TimeTransOUTDone = millis();
  Transition_Color = CurrentMenu->TransitionColor();

  AudioMemory(20);
  Synth.Setup();

  Serial.println("Done!!");
  Serial.println("Beginnning Main Loop.");
}

void loop() {
  //Update the MIDI, SYNTH, BUTTONS, DIALS, and SCREEN
  bool readVal = MIDI.read();
  while(readVal){
    //The MIDI handles will be taking care of updating everything.
    readVal = MIDI.read();
  }
  Synth.Play();
  updateButtons();
  updateDials();

  readVal = MIDI.read();
  while(readVal){
    //The MIDI handles will be taking care of updating everything.
    readVal = MIDI.read();
  }
  Synth.Play();

  updateScreen();
}

void setUpScreen(){
  while (!tft.begin(SPI_SPEED));

  tft.setScroll(0);
  tft.setRotation(1);                  // Rotation is the screen rotation: Rotates CW
  tft.setFramebuffer(ib);              // set 1 internal framebuffer -> activate float buffering
  tft.setDiffBuffers(&diff1, &diff2);  // set the 2 diff buffers => activate differential updates 
  tft.setDiffGap(4);                   // use a small gap for the diff buffers
  tft.setRefreshRate(120);             // around 120hz for the display refresh rate 
  tft.setVSyncSpacing(1);              // set framerate = refreshrate/2 (and enable vsync at the same time) 

  // make sure backlight is on
  if (PIN_BACKLIGHT != 255) {
      pinMode(PIN_BACKLIGHT, OUTPUT);
      digitalWrite(PIN_BACKLIGHT, HIGH);
  }
}

void updateButtons(){
  b1.update();
  b2.update();
  b3.update();
}

void updateDials(){
  Dial1Value = -analogRead(DIAL1);
  Dial2Value = -analogRead(DIAL2);
  Dial3Value = -analogRead(DIAL3);

  Serial.println(Dial1Value);
}

void updateScreen(){
  // Serial.println("In Flag" + CurrentMenu->TransitionINFlag());
  // Serial.println("Out Flag" + CurrentMenu->TransitionOUTFlag());
  // Serial.println("Out Done" + CurrentMenu->TransitionOUTDone());
  if(millis()-TimeTransOUTDone < 1000){
    //If you want a loading screen, have it here :)
  }
  //IF TransIN, then render TransIN.
  else if(CurrentMenu->TransitionINFlag()){
    CurrentMenu->TransitionIN();
    //im.blitMasked(SGearFrame, RGB565_Black, iVec2(0,0), 1.0f);
  }
  //If TransOUT, then render TransOUT
  else if(CurrentMenu->TransitionOUTFlag()){
    //If the Time Transitioning out == 1000, reset everything.
    CurrentMenu->TransitionOUT();
    //im.blitMasked(SGearFrame, RGB565_Black, iVec2(0,0), 1.0f);
  }
  //Neither? Then draw with inputs.
  else if(!CurrentMenu->TransitionINFlag() && !CurrentMenu->TransitionOUTFlag()){
    CurrentMenu->Draw(
      !b1.read(), !b2.read(), !b3.read(),
      b1.rose(), b2.rose(), b3.rose(),
      Dial1Value, Dial2Value, Dial3Value
    );
    //im.blit(bg, iVec2(0, 10 -(millis() / 30)%15));
    //im.blitMasked(SGearFrame, RGB565_Black, iVec2(0,0), 1.0f);
  }

  //If The transition Out is done, then set up the next menu.
  if(CurrentMenu->TransitionOUTDone() && millis()-TimeTransOUTDone > 1000){
      TimeTransOUTDone = millis();

      if(CurrentMenu->MenuToTransitionTo() != nullptr){
        NextMenu = CurrentMenu->MenuToTransitionTo();
        Transition_Color = CurrentMenu->TransitionColor();
      }
      PrevMenu = CurrentMenu;
      CurrentMenu->Destruct();
      CurrentMenu = NextMenu;
      NextMenu = nullptr;
      CurrentMenu->Setup(Transition_Color);
  }

  //Overlay the FPS and Update the screen
  tft.overlayFPS(fb, 0, 0xFFFF, 0, 0.3f); 
  tft.update(fb, false);
}

void MIDI_NoteOn(byte channel, byte pitch, byte velocity){
  // Serial.print("Added ");
  // Serial.println(pitch);
  if(21 <= pitch && pitch <= 108) Synth.AddNote(pitch);
}
void MIDI_NoteOff(byte channel, byte pitch, byte velocity){
  // Serial.print("Removed ");
  // Serial.println(pitch);
  if(21 <= pitch && pitch <= 108) Synth.RemoveNote(pitch);
}
void MIDI_PitchBend(byte channel, int bend){
  Serial.print("Bend ");
  Serial.println(bend);
  Synth.bend = bend;
}
void MIDI_ControlChange(byte channel, byte control, byte value){

}