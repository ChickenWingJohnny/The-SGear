#pragma once

#include <ILI9341_T4.h> 
#include <tgx.h>

#include "s_Synthesizer.h"

using namespace tgx;

class m_Oscilators : public Menu {
    private:
        Image<RGB565>* mainImage;
        s_Synthesizer* Synth;

        int Item1PosX = 100;
        int Item2PosX = 160;
        int Item3PosX = 220;
        int Item1PosY = 120;
        int Item2PosY = 220;
        int Item3PosY = 120;
        int Dial1PosX = 40;
        int Dial3PosX = 280;
        int Dial1PosY = 120;
        int Dial3PosY = 120;

        float Item1Scale = 5.5;
        float Item2Scale = 2;
        float Item3Scale = 5.5;
        float Dial1Scale = 1.25;
        float Dial3Scale = 1.25;

        int Item1Rotation = 45;
        int Item2Rotation = 45;
        int Item3Rotation = 45;
        float Dial1Rotation = 0;
        float Dial3Rotation = 0;

        int Osc0Type;
        int Osc1Type;

        MenuItem Item1 = {};
        MenuItem Item2 = {};
        MenuItem Item3 = {};

        const RGB32 col1 = RGB32(0, 255, 0);
        const RGB32 col2 = RGB32(2, 20, 0);   
        const RGB32 col3 = RGB32(7, 70, 0);

        const RGB32 lineColor = col1;

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

        void Draw(){
            //Background
            mainImage->fillScreenVGradient(RGB565_Black, col3);

            //Lines.
            mainImage->drawFastVLine(iVec2(160-1, 0), 60, lineColor);
            mainImage->drawFastVLine(iVec2(160, 0), 60, lineColor);
            mainImage->drawFastVLine(iVec2(160+1, 0), 60, lineColor);

            mainImage->drawLine(iVec2(160-1, 60), iVec2(Item1PosX-1, Item1PosY), lineColor);
            mainImage->drawLine(iVec2(160, 60), iVec2(Item1PosX, Item1PosY), lineColor);
            mainImage->drawLine(iVec2(160+1, 60), iVec2(Item1PosX+1, Item1PosY), lineColor);
            mainImage->drawLine(iVec2(160-1, 60), iVec2(Item3PosX-1, Item3PosY), lineColor);
            mainImage->drawLine(iVec2(160, 60), iVec2(Item3PosX, Item3PosY), lineColor);
            mainImage->drawLine(iVec2(160+1, 60), iVec2(Item3PosX+1, Item3PosY), lineColor);

            mainImage->fillCircle(iVec2(160, 63), 5, lineColor, lineColor);

            mainImage->drawFastVLine(iVec2(130-1, 80), 10, lineColor);
            mainImage->drawFastVLine(iVec2(130, 80), 10, lineColor);
            mainImage->drawFastVLine(iVec2(130+1, 80), 10, lineColor);
            mainImage->drawFastHLine(iVec2(130, 90-1), 10, lineColor);
            mainImage->drawFastHLine(iVec2(130, 90), 10, lineColor);
            mainImage->drawFastHLine(iVec2(130, 90+1), 10, lineColor);

            mainImage->drawFastVLine(iVec2(190-1, 80), 10, lineColor);
            mainImage->drawFastVLine(iVec2(190, 80), 10, lineColor);
            mainImage->drawFastVLine(iVec2(190+1, 80), 10, lineColor);
            mainImage->drawFastHLine(iVec2(180, 90-1), 10, lineColor);
            mainImage->drawFastHLine(iVec2(180, 90), 10, lineColor);
            mainImage->drawFastHLine(iVec2(180, 90+1), 10, lineColor);

            mainImage->drawLine(iVec2(Item1PosX-1, Item1PosY), iVec2(160-1, 180), lineColor);
            mainImage->drawLine(iVec2(Item1PosX, Item1PosY), iVec2(160, 180), lineColor);
            mainImage->drawLine(iVec2(Item1PosX+1, Item1PosY), iVec2(160+1, 180), lineColor);
            mainImage->drawLine(iVec2(Item3PosX-1, Item3PosY), iVec2(160-1, 180), lineColor);
            mainImage->drawLine(iVec2(Item3PosX, Item3PosY), iVec2(160, 180), lineColor);
            mainImage->drawLine(iVec2(Item3PosX+1, Item3PosY), iVec2(160+1, 180), lineColor);

            mainImage->fillCircle(iVec2(160, 177), 5, lineColor, lineColor);

            mainImage->drawFastVLine(iVec2(160-1, 180), 60, lineColor);
            mainImage->drawFastVLine(iVec2(160, 180), 60, lineColor);
            mainImage->drawFastVLine(iVec2(160+1, 180), 60, lineColor);

            //Main Images
            mainImage->blitScaledRotated(Item1.image, fVec2(Item1.size.x/2.0, Item1.size.y/2.0), fVec2(Item1.pos.x, Item1.pos.y), Item1.scale, Item1.rot);
            mainImage->blitScaledRotated(Item3.image, fVec2(Item3.size.x/2.0, Item3.size.y/2.0), fVec2(Item3.pos.x, Item3.pos.y), Item3.scale, Item3.rot);

            mainImage->blitScaledRotatedMasked(CreateImage::Dial, RGB565_Black, fVec2(CreateImage::Dial.lx()/2.0, CreateImage::Dial.ly()/2.0), fVec2(Dial1PosX, Dial1PosY), Dial1Scale, Dial1Rotation, 1.0);
            mainImage->blitScaledRotatedMasked(CreateImage::Dial, RGB565_Black, fVec2(CreateImage::Dial.lx()/2.0, CreateImage::Dial.ly()/2.0), fVec2(Dial3PosX, Dial3PosY), Dial3Scale, Dial3Rotation, 1.0);
        }

    public:
        m_Oscilators(Image<RGB565>* im, s_Synthesizer* s){
            mainImage = im;
            Synth = s;

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
            Osc0Type = Synth->osc_0.type;
            Osc1Type = Synth->osc_1.type;
            CreateImage::updateRectMenuItem(col1, col2);
            CreateImage::updateAltRectMenuItem(col2, col1);
            CreateImage::updateDialMenuItem(col1, col2);
        }
        void Setup(RGB565 TransitionINColor){
            Osc0Type = Synth->osc_0.type;
            Osc1Type = Synth->osc_1.type;
            CreateImage::updateRectMenuItem(col1, col2);
            CreateImage::updateAltRectMenuItem(col2, col1);
            CreateImage::updateDialMenuItem(col1, col2);
        }

        //The flag for if the TransitionIN should Happen.
        bool TransitionINFlag(){
            return transitionINFlag;
        }
        //After the previous menu is destroyed, and the current menu is setup, transition in is called every
        //frame until it is done. Will implicitly stop drawing if it is done Transitioning in.
        void TransitionIN(){
            transitionINFlag = false;
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
            Dial1Rotation = 300*(Dial1/1024.0) - 210;
            Dial3Rotation = 300*(Dial3/1024.0) - 210;
            Serial.println(Dial1Rotation);
            Draw();
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
            Serial.println("Destroying m_Oscilators");

            transitionINFlag = true;
            transitionOUTFlag = false;
            transitionOUTDone = false;
            TransitionButton = 0;
            TransitioningMenu = nullptr;
        }
};