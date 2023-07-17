#pragma once

#include <ILI9341_T4.h> 
#include <tgx.h>

#include "s_Synthesizer.h"

#define WAVE_SPEED         0.03 //Pixels per ms

using namespace tgx;

class m_OscilatorTemplate : public Menu {
    private:
        Image<RGB565>* mainImage;
        s_Synthesizer* Synth;
        Oscilator* Osc;

        uint16_t wave[240 * 60]; //Actual resolution is 120 x 60
        uint16_t wave_display[120 * 60]; //Actual resolution is 120 x 60
        uint16_t transition_in[2*2];

        Image<RGB565> WaveImage;
        Image<RGB565> WaveDisplay;
        Image<RGB565> TransitionIn;

        bool transitionINFlag = true;
        bool transitionOUTFlag = false;
        bool transitionOUTDone = false;
        uint TransitionButton = 0;

        double TransitionScale = 700;
        const int TRANSITION_MIN = 1;
        const int TRANSITION_MAX = 700;
        const double TRANSITION_RATE = 1.20;

        Menu* TransitionMenu1;
        Menu* TransitionMenu2;
        Menu* TransitionMenu3;
        Menu* TransitioningMenu;

        RGB32 col1;
        RGB32 col2;
        RGB32 col3;

        MenuItem Item1 = {};
        MenuItem Item2 = {};
        MenuItem Item3 = {};

        void CreateMenuItemImage(){
            
        }
        void CreateDialImage(){

        }
        void createTransitionIn(RGB565 transitionInColor){
            TransitionIn.fillScreen(transitionInColor);
        }
        void CreateSineWaveImage(){
            WaveImage.fillScreen(RGB565_Black);
            for(int s = 2; s > -2; s--){
                for(int x = 0; x < WaveImage.lx(); x++){
                    int y = (int)(28 * sin(((double)1/60) * PI * (double) x));
                    WaveImage.drawPixel(x, 28+y+s, col1);
                }
            }
        }
        void CreateSquareWaveImage(){
            WaveImage.fillScreen(RGB565_Black);

        }
        void CreateTriangleWaveImage(){

        }
        void CreateSawtoothWaveImage(){
            //Sawtooth Reverse can be made by flipping this image.
        }
        void CreateArbitraryWaveImage(){
            
        }

    public:
        m_OscilatorTemplate(Image<RGB565>* im, s_Synthesizer* s, int OscControl){
            mainImage = im;
            Synth = s;

            WaveImage = Image<RGB565>(wave, 240, 60);
            WaveDisplay = Image<RGB565>(wave_display, 120, 60);

            switch (OscControl){
                case 0:
                    Osc = &Synth->osc_0;
                    break;
                case 1:
                    Osc = &Synth->osc_1;
                    break;
                default:
                    break;
            }

            col1 = RGB32(0, 0, 255);
            col2 = RGB32(0, 2, 20);
            col3 = RGB32(0, 7, 70);
        }
        
        void Link(Menu* Tm1, Menu* Tm2, Menu* Tm3){
            TransitionMenu1 = Tm1;
            TransitionMenu2 = Tm2;
            TransitionMenu3 = Tm3;
        }

        //Instantiates everything inside the menu. Pre-draws the images, and stores the TransitionINColor.
        void Setup(){
            CreateSineWaveImage();

            createTransitionIn(col1);
            //createTransitionOut(TransitionColor());
        }
        void Setup(RGB565 TransitionINColor){

        }

        //The flag for if the TransitionIN should Happen.
        bool TransitionINFlag(){
            return transitionINFlag;
        }
        //After the previous menu is destroyed, and the current menu is setup, transition in is called every
        //frame until it is done. Will implicitly stop drawing if it is done Transitioning in.
        void TransitionIN(){
            Draw();
            mainImage->blitScaledRotated(TransitionIn, fVec2(TransitionIn.width()/2, TransitionIn.height()/2), fVec2(mainImage->width()/2, mainImage->height()/2), TransitionScale, 45);
            TransitionScale /= TRANSITION_RATE;
            if(TransitionScale <= TRANSITION_MIN) {
                transitionINFlag = false;
            }
        }
        void Draw(){
            // int x = -(int)(millis() * WAVE_SPEED) % WaveDisplay.lx();
            // WaveDisplay.blit(WaveImage, iVec2(x, 0));
            // Serial.println(x);
            // mainImage->blit(WaveDisplay, iVec2(80, 60));
            //mainImage->blit(WaveImage, iVec2(80, 60));
        }

        //Draws the static of the menu. Controls the inputs of the menu as well.
        //Pressed parameters should be whether the button is down or up.
        //Is Just Released parameters should be whether the button is JUST released.
        //Dial 1 passes through the analog parameters of the 3 dials.
        void Draw(
            bool B1IsPressed, bool B2IsPressed, bool B3IsPressed, 
            bool B1IsJustReleased, bool B2IsJustReleased, bool B3IsJustReleased,
            int Dial1, int Dial2, int Dial3
        ){
            int x = -(int)(millis() * WAVE_SPEED) % WaveDisplay.lx();
            WaveDisplay.blitScaledRotated(WaveImage, fVec2(0, 0), fVec2(x, 0), 1.0F, 0.0F, 1.0F);
            //Serial.println(Dial1);
            mainImage->blit(WaveDisplay, 100, 80);
            //mainImage->blit(WaveImage, iVec2(80, 60));
        }

        //The flag for if the TransitionOUT should happen.
        bool TransitionOUTFlag(){
            return transitionOUTFlag;
        }
        //The flag for if the TransitionOUT is done.
        bool TransitionOUTDone(){
            return transitionOUTDone;
        }
        //Once the "Back button" is Pressed then released, TransitionOUT will be called every frame until it's done.
        //Will implicitly stop drawing if it is done Transitioning out.
        void TransitionOUT(){

        }
        
        //The Transition OUT color of the current menu INTO the next menu.
        //Should be stored temporarily, and then discarded once setup is called with this.
        RGB565 TransitionColor(){
            return col1;
        }

        //The Menu address pointing to what menu to transition to.
        Menu* MenuToTransitionTo(){
            return TransitioningMenu;
        }

        //This simply should free all the variables that were inside setup() and reset everything (menuwise)
        void Destruct(){
            Serial.println("Destroying m_OscilatorTemplate");

            transitionINFlag = true;
            transitionOUTFlag = false;
            transitionOUTDone = false;
            TransitionButton = 0;
            TransitioningMenu = nullptr;
        }
};