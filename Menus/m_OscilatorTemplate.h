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

        bool transitionINFlag = true;
        bool transitionOUTFlag = false;
        bool transitionOUTDone = false;
        uint TransitionButton = 0;

        double TransitionScale = 700;
        const int TRANSITION_MIN = 1;
        const int TRANSITION_MAX = 700;
        const double TRANSITION_RATE = 1.20;

        int Item1PosX = 22;
        int Item2PosX = 220;
        int Item3PosX = 256;
        int Item1PosY = 218;
        int Item2PosY = 80;
        int Item3PosY = 135;
        int Dial1PosX = 40;
        int Dial2PosX = 160;
        int Dial3PosX = 280;
        int Dial1PosY = 160;
        int Dial2PosY = 200;
        int Dial3PosY = 160;
        
        int Item1Scale = 2;
        int Item2Scale = 2;
        int Item3Scale = 4;
        float Dial1Scale = 1.25;
        float Dial2Scale = 1.5;
        float Dial3Scale = 1.25;

        int Item1Rotation = 45;
        int Item2Rotation = 45;
        int Item3Rotation = 45;
        float Dial1Rotation = 0;
        float Dial2Rotation = 0;
        float Dial3Rotation = 0;

        String oscString;

        Menu* TransitionMenu1;
        Menu* TransitionMenu2;
        Menu* TransitionMenu3;
        Menu* TransitioningMenu;

        const RGB32 col1 = RGB32(0, 255, 0);
        const RGB32 col2 = RGB32(2, 20, 0);   
        const RGB32 col3 = RGB32(7, 70, 0);

        MenuItem Item1 = {};
        MenuItem Item2 = {};
        MenuItem Item3 = {};

        void Draw(){
            mainImage->fillScreen(RGB565_Black);
            
            CreateImage::placeText(mainImage, ("OSCILATOR " + oscString), iVec2(160, 30), RGB565_White, font_Roboto_Bold_36, 1.0);

            int x = -(int)(millis() * WAVE_SPEED) % WaveDisplay.lx();
            WaveDisplay.blitScaledRotated(WaveImage, fVec2(0, 0), fVec2(x, 0), 1.0F, 0.0F, 1.0F);
            //Serial.println(Dial1);
            mainImage->fillRect(iVec2(77, 57), iVec2(166, 96), col1);
            mainImage->fillRect(iVec2(80, 60), iVec2(160, 90), col2);
            CreateImage::placeText(mainImage, "Wave Shape", iVec2(140, 67), RGB565_White, font_Roboto_Bold_14, 1.0);
            mainImage->blit(WaveDisplay, 100, 80);

            mainImage->blitScaledRotated(Item1.image, fVec2(Item1.image.width()/2, Item1.image.height()/2), fVec2(Item1PosX, Item1PosY), Item1Scale, Item1Rotation);
            mainImage->blitScaledRotatedMasked(CreateImage::BackArrow, RGB565_Black, fVec2(CreateImage::BackArrow.width()/2, CreateImage::BackArrow.height()/2), fVec2(Item1PosX, Item1PosY), 1, 0, 1.0);
            mainImage->blitScaledRotated(Item2.image, fVec2(Item2.image.width()/2, Item2.image.height()/2), fVec2(Item2PosX, Item2PosY), Item2Scale, Item2Rotation);

            mainImage->blitScaledRotatedMasked(CreateImage::Dial, RGB565_Black, fVec2(CreateImage::Dial.lx()/2.0, CreateImage::Dial.ly()/2.0), fVec2(Dial1PosX, Dial1PosY), Dial1Scale, Dial1Rotation, 1.0);
            mainImage->blitScaledRotatedMasked(CreateImage::Dial, RGB565_Black, fVec2(CreateImage::Dial.lx()/2.0, CreateImage::Dial.ly()/2.0), fVec2(Dial2PosX, Dial2PosY), Dial2Scale, Dial2Rotation, 1.0);
            mainImage->blitScaledRotatedMasked(CreateImage::Dial, RGB565_Black, fVec2(CreateImage::Dial.lx()/2.0, CreateImage::Dial.ly()/2.0), fVec2(Dial3PosX, Dial3PosY), Dial3Scale, Dial3Rotation, 1.0);
        }

        void CreateSineWaveImage(){
            WaveImage.fillScreen(col2);
            for(int s = 2; s > -2; s--){
                for(int x = 0; x < WaveImage.lx(); x++){
                    int y = (int)(28 * sin(((double)1/60) * PI * (double) x));
                    WaveImage.drawPixel(x, 28+y+s, col1);
                }
            }
        }
        void CreateSquareWaveImage(){
            WaveImage.fillScreen(col2);
            RGB565 lineColor = col1;
            int period = 30;
            for(int i = 0; i < 8; i++){
                WaveImage.drawFastVLine(iVec2((period*i)-1, 0), 60, lineColor);
                WaveImage.drawFastVLine(iVec2((period*i), 0), 60, lineColor);
                WaveImage.drawFastVLine(iVec2((period*i)+1, 0), 60, lineColor);
                if(i%2){
                    WaveImage.drawFastHLine(iVec2((period*i), 0), period, lineColor);
                    WaveImage.drawFastHLine(iVec2((period*i), 1), period, lineColor);
                    WaveImage.drawFastHLine(iVec2((period*i), 2), period, lineColor);
                }
                else{
                    WaveImage.drawFastHLine(iVec2((period*i), 58), period, lineColor);
                    WaveImage.drawFastHLine(iVec2((period*i), 59), period, lineColor);
                    WaveImage.drawFastHLine(iVec2((period*i), 60), period, lineColor);
                }
            }
            WaveImage.drawFastVLine(iVec2(240-1, 0), 60, lineColor);
            WaveImage.drawFastVLine(iVec2(240, 0), 60, lineColor);
            WaveImage.drawFastVLine(iVec2(240+1, 0), 60, lineColor);
        }
        void CreateTriangleWaveImage(){
            WaveImage.fillScreen(col2);
            int period = 30;
            for(int i = 0; i < 8; i+=2){
                WaveImage.drawLine(iVec2((period*i)-1, 60), iVec2(period*(i+1)-1, 0), col1);
                WaveImage.drawLine(iVec2((period*i), 60), iVec2(period*(i+1), 0), col1);
                WaveImage.drawLine(iVec2((period*i)+1, 60), iVec2(period*(i+1)+1, 0), col1);

                WaveImage.drawLine(iVec2((period*(i+1))-1, 0), iVec2(period*(i+2)-1, 60), col1);
                WaveImage.drawLine(iVec2((period*(i+1)), 0), iVec2(period*(i+2), 60), col1);
                WaveImage.drawLine(iVec2((period*(i+1))+1, 0), iVec2(period*(i+2)+1, 60), col1);
            }
        }
        void CreateSawtoothWaveImage(){
            //Sawtooth Reverse can be made by flipping this image.
            WaveImage.fillScreen(col2);
            int period = 60;
            for(int i = 0; i < 4; i++){
                WaveImage.drawLine(iVec2((period*i)-1, 60), iVec2(period*(i+1)-1, 0), col1);
                WaveImage.drawLine(iVec2((period*i), 60), iVec2(period*(i+1), 0), col1);
                WaveImage.drawLine(iVec2((period*i)+1, 60), iVec2(period*(i+1)+1, 0), col1);

                WaveImage.drawFastVLine(iVec2((period*(i+1)-1), 0), 60, col1);
                WaveImage.drawFastVLine(iVec2((period*(i+1)), 0), 60, col1);
                WaveImage.drawFastVLine(iVec2((period*(i+1)+1), 0), 60, col1);
            }
        }
        void CreateArbitraryWaveImage(){
            
        }
        void selectWave(int type){
            switch (type)
            {
                case WAVEFORM_SINE:
                    CreateSineWaveImage();
                    break;
                case WAVEFORM_SQUARE:
                    CreateSquareWaveImage();
                    break;
                case WAVEFORM_TRIANGLE:
                    CreateTriangleWaveImage();
                    break;
                case WAVEFORM_SAWTOOTH:
                    CreateSawtoothWaveImage();
                default:
                    break;
            }
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
                    oscString = "0";
                    break;
                case 1:
                    Osc = &Synth->osc_1;
                    oscString = "1";
                    break;
                default:
                    break;
            }
        
            Item1.image = CreateImage::Item;
            Item2.image = CreateImage::Item;
            Item3.image = CreateImage::Item;

            Item1.size = iVec2(Item1.image.width(), Item1.image.height());
            Item2.size = iVec2(Item2.image.width(), Item2.image.height());
            Item3.size = iVec2(Item3.image.width(), Item3.image.height());

            Item1.pos = iVec2(Item1PosX, Item1PosY);
            Item2.pos = iVec2(Item2PosX, Item2PosY);
            Item3.pos = iVec2(Item3PosX, Item3PosY);

            Item1.scale = Item1Scale;
            Item2.scale = Item2Scale;
            Item3.scale = Item3Scale;

            Item1.rot = Item1Rotation;
            Item2.rot = Item2Rotation;
            Item3.rot = Item3Rotation;
        }
        
        void Link(Menu* Tm1, Menu* Tm2, Menu* Tm3){
            TransitionMenu1 = Tm1;
            TransitionMenu2 = Tm2;
            TransitionMenu3 = Tm3;
        }

        //Instantiates everything inside the menu. Pre-draws the images, and stores the TransitionINColor.
        void Setup(){
            selectWave(Osc->type);

            CreateImage::updateRectMenuItem(col1, col2);
            CreateImage::updateAltRectMenuItem(col2, col1);
            CreateImage::updateTransitionIn(col1);
            CreateImage::updateTransitionOut(TransitionColor());
            CreateImage::updateDialMenuItem(col1, col2);
            CreateImage::updateBackArrow(col1);
        }
        void Setup(RGB565 TransitionINColor){
            selectWave(Osc->type);

            CreateImage::updateRectMenuItem(col1, col2);
            CreateImage::updateAltRectMenuItem(col2, col1);
            CreateImage::updateTransitionIn(col1);
            CreateImage::updateTransitionOut(TransitionColor());
            CreateImage::updateDialMenuItem(col1, col2);
            CreateImage::updateBackArrow(col1);
        }

        //The flag for if the TransitionIN should Happen.
        bool TransitionINFlag(){
            return transitionINFlag;
        }
        //After the previous menu is destroyed, and the current menu is setup, transition in is called every
        //frame until it is done. Will implicitly stop drawing if it is done Transitioning in.
        void TransitionIN(){
            Draw();
            mainImage->blitScaledRotated(CreateImage::TransitionIn, fVec2(CreateImage::TransitionIn.width()/2, CreateImage::TransitionIn.height()/2), fVec2(mainImage->width()/2, mainImage->height()/2), TransitionScale, 45);
            TransitionScale /= TRANSITION_RATE;
            if(TransitionScale <= TRANSITION_MIN) {
                transitionINFlag = false;
            }
        }

        //Draws the static of the menu. Controls the inputs of the menu as well.
        //Pressed parameters should be whether the button is down or up.
        //Is Just Released parameters should be whether the button is JUST released.
        //Dial 1 passes through the analog parameters of the 3 dials.
        void Draw(
            bool B1Pressed, bool B2Pressed, bool B3Pressed, 
            bool B1IsJustReleased, bool B2IsJustReleased, bool B3IsJustReleased,
            int Dial1, int Dial2, int Dial3
        ){
            Dial1Rotation = 300*(Dial1/1024.0) - 210;
            Dial2Rotation = 300*(Dial2/1024.0) - 210;
            Dial3Rotation = 300*(Dial3/1024.0) - 210;

            if(B1Pressed){
                Item1.image = CreateImage::AltItem;
                CreateImage::updateBackArrow(col2);
            } else{
                Item1.image = CreateImage::Item;
                CreateImage::updateBackArrow(col1);
            }
            B2Pressed ? Item2.image = CreateImage::AltItem : Item2.image = CreateImage::Item;
            //B3Pressed ? Item3.image = CreateImage::AltItem : Item3.image = CreateImage::Item;

            Draw();

            if(B2IsJustReleased){
                Osc->type += 1;
                Osc->type > WAVEFORM_TRIANGLE_VARIABLE ? Osc->type = WAVEFORM_SINE : Osc->type = Osc->type;
                selectWave(Osc->type);
            }

            if(TransitionButton == 0 && TransitioningMenu == nullptr) {
                TransitionButton = B1IsJustReleased << 2; //| B3IsJustReleased;
                if(TransitionButton != 0) {
                    transitionOUTFlag = true;
                    //Serial.println(TransitionButton);
                    if(TransitionButton == 0b100){
                        Serial.println("Button 1 Released... Activating Transition for Button 1");
                        TransitioningMenu = TransitionMenu1;
                    }
                    // else if (TransitionButton == 0b001){
                    //     Serial.println("Button 3 Released... Activating Transition for Button 3");
                    //     TransitioningMenu = TransitionMenu3;
                    // }     
                }
            }
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
            if(TransitionScale > TRANSITION_MAX){
                mainImage->fillScreen(TransitionColor());
                Serial.println("Transition to ... Done.");
                transitionOUTDone = true;
            }

            if(!transitionOUTDone){
                if(TransitionButton >> 2){
                    Draw();
                    mainImage->blitScaledRotated(CreateImage::TransitionOut, fVec2(CreateImage::TransitionOut.width()/2, CreateImage::TransitionOut.height()/2), fVec2(Item1.pos.x, Item1.pos.y), TransitionScale, 45);
                    TransitionScale *= TRANSITION_RATE;
                }
            }
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