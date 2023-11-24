#pragma once

#include <ILI9341_T4.h>
#include <tgx.h>

#include "CreateImage.h"
#include "s_Synthesizer.h"

#include "m_MenuItem.h"
#include "m_Menu.h"

using namespace tgx;

#define FFT_RESOLUTION 21
#define FFT_E 1.4142
#define FFT_ARRAY_SIZE 5
#define PEAK_RESOLUTION 64

class m_ViewWave : public Menu
{
    private:

        Image<RGB565>* mainImage;
        s_Synthesizer* synth;
        uint16_t text_box[240 * 56];
        Image<RGB565> TextBox;

        float fftValues[FFT_RESOLUTION][FFT_ARRAY_SIZE]; //Not A Circular Array, used to average the values
        float peakValues[PEAK_RESOLUTION]; //A Circular Array
        int writeIndex = 0;

        //The Main Menu Loop Without Inputs
        void Draw(){
            mainImage->fillScreen(RGB32_Black);
            //mainImage->fillScreenVGradient(RGB565_Black, col3);
            
            CreateImage::drawCylinderBackgroundHorizontal(mainImage, col3);

            //FFT Background
            mainImage->fillRect(iVec2(10, 45), iVec2(300, 180), col1);
            mainImage->fillRect(iVec2(12, 47), iVec2(300 - 4, 180-4), col2);

            //Wave View
            drawFFT();
            CreateImage::placeText(mainImage, "43 107 194 302 431 582 1k 1.3k 1.7k 2.2k 2.8k 3.6k 4.5k 7.3k 9.3k 12k 15k 18k 21k", iVec2(160, 215), RGB32_White, font_Righteous_AA2_8, 1.0f);


            // updatePeak();
            // drawPeak();

            
            //Title
            mainImage->blitScaledRotated(TextBox, fVec2(TextBox.width()/2, TextBox.height()/2), fVec2(160, 36), 1, 0);

            //Menu Items
            mainImage->blitScaledRotated(Item1.image, fVec2(Item1.size.x/2, Item1.size.y/2), fVec2(Item1.pos.x, Item1.pos.y), Item1Scale, Item1Rotation);
            mainImage->blitScaledRotatedMasked(CreateImage::BackArrow, RGB565_Black, fVec2(CreateImage::BackArrow.width()/2, CreateImage::BackArrow.height()/2), fVec2(Item1PosX, Item1PosY), 1, 0, 1.0);
            mainImage->blitScaledRotated(Item3.image, fVec2(Item3.size.x/2, Item3.size.y/2), fVec2(Item3.pos.x, Item3.pos.y), Item3Scale, Item3Rotation);
        }

        void drawFFT(){
            //Draw FFT
            //https://forum.pjrc.com/index.php?threads/is-there-a-logarithmic-function-for-fft-bin-selection-for-any-given-of-bands.32677/
            RGB32 color;
            bool available = synth->getFFTAvailable();
            for(int i = 0; i < FFT_RESOLUTION - 1; i++){
                //Rainbow will vary by I, RAINBOW!!!
                color = RGB32(0, 0, 0);
                if(i < FFT_RESOLUTION/3) // i = 0 to 6
                    color = RGB32(255*cos(i/6.0 * PI/2), 255*sin(i/6.0 * PI/2),0);
                else if(i < 2*FFT_RESOLUTION/3) // i = 7 to 13
                    color = RGB32(0, 255*cos((i-7)/6.0 * PI/2), 255*sin((i-7)/6.0 * PI/2));
                else if(i < FFT_RESOLUTION) // i = 14 to 20
                    color = RGB32(255*sin((i-14)/6.0 * PI/2), 0, 255*cos((i-14)/6.0 * PI/2));

                
                float height = available ? min(3200*synth->getFFT(i*FFT_E, (i+1)*FFT_E), 75) : 0;
                fftValues[i][writeIndex % FFT_ARRAY_SIZE] = height;
                //Find the max value in the array (Helps with the jittering)
                float max = 0;
                for(int j = 0; j < FFT_ARRAY_SIZE; j++){
                    if(fftValues[i][j] > max)
                        max = fftValues[i][j];
                }
                
                //Draw the Bar!
                mainImage->fillRect(iVec2(i * 273 / FFT_RESOLUTION + 20 + i, round(204 - 32*log(max > 1 ? max : 1))), 
                                    iVec2(273 / FFT_RESOLUTION, round(32*log(max > 1 ? max : 1) + 5)), 
                                    color
                );
            }
            writeIndex++;
        }
        void updatePeak(){
            //Update the circular array
            peakValues[writeIndex % PEAK_RESOLUTION] = synth->getPeak();
            writeIndex++;
        }
        void drawPeak(){
            //similar to draw FFT, draw lines interpolating each peak from one point of the array, to the next
            int x = 0;
            for(int i = writeIndex - PEAK_RESOLUTION; i < writeIndex + PEAK_RESOLUTION - 1; i++){
                float P1 = peakValues[i % PEAK_RESOLUTION];
                float P2 = peakValues[(i + 1) % PEAK_RESOLUTION];
                mainImage->drawLine(x * (240.0/PEAK_RESOLUTION), 159 - 800*P1, (x + 1) * (240.0/PEAK_RESOLUTION), 159 - 800*P2, col1);
                mainImage->drawLine(x * (240.0/PEAK_RESOLUTION), 160 - 800*P1, (x + 1) * (240.0/PEAK_RESOLUTION), 160 - 800*P2, col1);
                mainImage->drawLine(x * (240.0/PEAK_RESOLUTION), 161 - 800*P1, (x + 1) * (240.0/PEAK_RESOLUTION), 161 - 800*P2, col1);
                x++;
            }
        }

        const int Item1PosX = 22;
        const int Item1PosY = 46;
        const int Item1Scale = 2;
        const int Item1Rotation = 45;

        const int Item3PosX = 298;
        const int Item3PosY = 46;
        const int Item3Scale = 2;
        const int Item3Rotation = 45;

        MenuItem Item1 = {};
        MenuItem Item3 = {};

        const RGB32 col1 = RGB32(0, 0, 255);
        const RGB32 col2 = RGB32(0, 2, 20);
        const RGB32 col3 = RGB32(0, 7, 70);

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

    public:
        m_ViewWave(Image<RGB565>* mI, s_Synthesizer* s){
            mainImage = mI;
            synth = s;
            TextBox = Image<RGB565>(text_box, 240, 56);

            Item1.image = CreateImage::Item;
            Item1.size.x = Item1.image.width();
            Item1.size.y = Item1.image.height();
            Item1.pos.x = Item1PosX;
            Item1.pos.y = Item1PosY;
            Item1.scale = Item1Scale;
            Item1.rot = Item1Rotation;

            Item3.image = CreateImage::Item;
            Item3.size.x = Item3.image.width();
            Item3.size.y = Item3.image.height();
            Item3.pos.x = Item3PosX;
            Item3.pos.y = Item3PosY;
            Item3.scale = Item3Scale;
            Item3.rot = Item3Rotation;
        }
        void Link(Menu* Tm1, Menu* Tm2, Menu* Tm3){
            TransitionMenu1 = Tm1;
            TransitionMenu2 = Tm2;
            TransitionMenu3 = Tm3;
        }

        void Setup(){
            Serial.println("Setup m_MainMenu");

            CreateImage::updateTransitionIn(col1);
            CreateImage::updateTransitionOut(TransitionColor());

            CreateImage::updateRectMenuItem(col1, col2);
            CreateImage::updateAltRectMenuItem(col2, col1);
            CreateImage::updateBackArrow(col1);
            CreateImage::createTextBox(&TextBox, "VIEW WAVE", 4, col2, col1, RGB565_White, font_Righteous_AA2_32);
        }
        void Setup(RGB565 TransitionINColor){
            Serial.println("Setup m_MainMenu");

            CreateImage::updateTransitionIn(TransitionINColor);
            CreateImage::updateTransitionOut(TransitionColor());

            CreateImage::updateRectMenuItem(col1, col2);
            CreateImage::updateAltRectMenuItem(col2, col1);
            CreateImage::updateBackArrow(col1);
            CreateImage::createTextBox(&TextBox, "VIEW WAVE", 4, col2, col1, RGB565_White, font_Righteous_AA2_32);
        }

        //Transition IN should ONLY be Drawn if transitionINFlag is True.
        bool TransitionINFlag(){
            return transitionINFlag;
        }
        void TransitionIN(){
            Draw();
            mainImage->blitScaledRotated(CreateImage::TransitionIn, fVec2(CreateImage::TransitionIn.width()/2, CreateImage::TransitionIn.height()/2), fVec2(mainImage->width()/2, mainImage->height()/2), TransitionScale, 45);
            TransitionScale /= TRANSITION_RATE;
            if(TransitionScale <= TRANSITION_MIN) {
                transitionINFlag = false;
            }
        }

        //Main Menu Loop with Button Presses.
        void Draw(  bool B1Pressed, bool B2Pressed, bool B3Pressed, 
                    bool B1IsJustReleased, bool B2IsJustReleased, bool B3IsJustReleased,
                    int Dial1, int Dial2, int Dial3
        ){
            if(B1Pressed){
                Item1.image = CreateImage::AltItem;
                CreateImage::updateBackArrow(col2);
            }
            else{
                Item1.image = CreateImage::Item;
                CreateImage::updateBackArrow(col1);
            }

            if(B3Pressed){
                Item3.image = CreateImage::AltItem;
            }
            else{
                Item3.image = CreateImage::Item;
            }
            
            Draw();

            //Bitshifted bools to make it easier :)
            if(TransitionButton == 0 && TransitioningMenu == nullptr) {
                TransitionButton = B1IsJustReleased << 2 | B2IsJustReleased << 1 | B3IsJustReleased;
                if(TransitionButton != 0) {
                    transitionOUTFlag = true;
                    //Serial.println(TransitionButton);
                    if(TransitionButton == 0b100){
                        Serial.println("Button 1 Released... Activating Transition for Button 1");
                        TransitioningMenu = TransitionMenu1;
                    }
                    else if (TransitionButton == 0b010){
                        // Serial.println("Button 2 Released... Activating Transition for Button 2");
                        // TransitioningMenu = TransitionMenu2;
                    }
                    else if (TransitionButton == 0b001){
                        Serial.println("This should change the view to FFT/PEAK");
                        //TransitioningMenu = TransitionMenu3;
                    }
                }
            }
        }

        //Transition should ONLY be Drawn if transitionINFlag is True.
        bool TransitionOUTFlag(){
            return transitionOUTFlag;
        }
        bool TransitionOUTDone(){
            return transitionOUTDone;
        }
        void TransitionOUT(){
            if(TransitionScale > TRANSITION_MAX) {
                mainImage->fillScreen(TransitionColor());
                transitionOUTDone = true;
            }
            if(!transitionOUTDone){
                if(TransitionButton >> 2){
                    Draw();
                    mainImage->blitScaledRotated(CreateImage::TransitionOut, fVec2(CreateImage::TransitionOut.width()/2, CreateImage::TransitionOut.height()/2), fVec2(Item1.pos.x, Item1.pos.y), TransitionScale, 45);
                    TransitionScale *= TRANSITION_RATE;
                } else if (TransitionButton >> 1) {

                } else if (TransitionButton) {

                }
            }
        }

        RGB565 TransitionColor(){
            return col1;
        }

        Menu* MenuToTransitionTo(){
            return TransitioningMenu;
        }

        void Destruct(){
            Serial.println("Destroying m_MainMenu");

            transitionINFlag = true;
            transitionOUTFlag = false;
            transitionOUTDone = false;
            TransitionButton = 0;
            TransitioningMenu = nullptr;
        }
};
