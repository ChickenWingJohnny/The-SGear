#pragma once

#include <ILI9341_T4.h>
#include <tgx.h>

#include "CreateImage.h"
#include "s_Synthesizer.h"

#include "m_MenuItem.h"
#include "m_Menu.h"

using namespace tgx;

#define drawADSR_StartX 22
#define drawADSR_StartY 75
#define drawADSR_EndY 175

class m_VolumeEnvelope : public Menu
{
    private:

        Image<RGB565>* mainImage;
        s_Synthesizer* synth;
        uint16_t text_box[240 * 56];
        Image<RGB565> TextBox;

        //The Main Menu Loop Without Inputs
        void Draw(){
            mainImage->fillScreen(RGB32_Black);
            
            CreateImage::drawCylinderBackgroundHorizontal(mainImage, col3);

            //Menu Items
            mainImage->blitScaledRotated(Item1.image, fVec2(Item1.size.x/2, Item1.size.y/2), fVec2(Item1.pos.x, Item1.pos.y), Item1Scale, Item1Rotation);
            mainImage->blitScaledRotatedMasked(CreateImage::BackArrow, RGB565_Black, fVec2(CreateImage::BackArrow.width()/2, CreateImage::BackArrow.height()/2), fVec2(Item1PosX, Item1PosY), 1, 0, 1.0);
            mainImage->blitScaledRotated(Item2.image, fVec2(Item2.size.x/2, Item2.size.y/2), fVec2(Item2.pos.x, Item2.pos.y), Item2Scale, Item2Rotation);
            mainImage->blitScaledRotated(Item3.image, fVec2(Item3.size.x/2, Item3.size.y/2), fVec2(Item3.pos.x, Item3.pos.y), Item3Scale, Item3Rotation);

            //Dials
            mainImage->blitScaledRotatedMasked(CreateImage::Dial, RGB565_Black, fVec2(CreateImage::Dial.lx()/2.0, CreateImage::Dial.ly()/2.0), fVec2(Dial1PosX, Dial1PosY), Dial1Scale, Dial1Rotation, 1.0);
            mainImage->blitScaledRotatedMasked(CreateImage::Dial, RGB565_Black, fVec2(CreateImage::Dial.lx()/2.0, CreateImage::Dial.ly()/2.0), fVec2(Dial2PosX, Dial2PosY), Dial2Scale, Dial2Rotation, 1.0);
            mainImage->blitScaledRotatedMasked(CreateImage::Dial, RGB565_Black, fVec2(CreateImage::Dial.lx()/2.0, CreateImage::Dial.ly()/2.0), fVec2(Dial3PosX, Dial3PosY), Dial3Scale, Dial3Rotation, 1.0);

            //ADSR and Background
            mainImage->fillRect(drawADSR_StartX, drawADSR_StartY, 320-(drawADSR_StartX*2), drawADSR_EndY-drawADSR_StartY, col1);
            mainImage->fillRect(drawADSR_StartX+2, drawADSR_StartY+2, 320-(drawADSR_StartX*2)-4, drawADSR_EndY-drawADSR_StartY-4, col2);

            drawADSR();

            //Text
            CreateImage::placeText(mainImage, "A", iVec2(Dial1PosX, Dial1PosY), RGB565_White, font_Righteous_AA2_16, 1.0);
            CreateImage::placeText(mainImage, Dial2Text, iVec2(Dial2PosX, Dial2PosY), RGB565_White, font_Righteous_AA2_16, 1.0);
            CreateImage::placeText(mainImage, "R", iVec2(Dial3PosX, Dial3PosY), RGB565_White, font_Righteous_AA2_16, 1.0);

            CreateImage::placeText(mainImage, "Attack: " + String(AttackValue), iVec2(Dial1PosX+10, Dial1PosY-30), RGB565_White, font_Righteous_AA2_12, 1.0);
            if(!Dial2Sustain){
                CreateImage::placeText(mainImage, "Decay: " + String(DecayValue), iVec2(Dial2PosX, Dial2PosY-30), RGB565_White, font_Righteous_AA2_12, 1.0);
            }
            else{
                CreateImage::placeText(mainImage, "Sustain: " + String(SustainValue), iVec2(Dial2PosX, Dial2PosY-30), RGB565_White, font_Righteous_AA2_12, 1.0);
            }
            CreateImage::placeText(mainImage, "Release: " + String(ReleaseValue), iVec2(Dial3PosX-10, Dial3PosY-30), RGB565_White, font_Righteous_AA2_12, 1.0);

            //Title
            mainImage->blitScaledRotated(TextBox, fVec2(TextBox.width()/2, TextBox.height()/2), fVec2(160, 36), 1, 0);
        }

        void drawADSR(){
            //Draw Attack
            float EndAttackPosX = (drawADSR_StartX*2)+(65*AttackValue/250.0);
            float EndDecayPosX = (drawADSR_StartX*2)+EndAttackPosX+(65*DecayValue/250.0);
            float SustainPosY = drawADSR_EndY+(-100*SustainValue);
            float EndReleasePosX = 320-(drawADSR_StartX*2);
            float StartReleasePosX = EndReleasePosX-(65*ReleaseValue/250.0);

            mainImage->fillTriangle(iVec2(drawADSR_StartX*2, drawADSR_EndY-2), iVec2(EndAttackPosX, drawADSR_EndY-2), iVec2(EndAttackPosX, drawADSR_StartY), col1, col1);
            mainImage->fillTriangle(iVec2(EndAttackPosX, drawADSR_StartY), iVec2(EndAttackPosX, SustainPosY), iVec2(EndDecayPosX, SustainPosY), col1, col1);
            mainImage->fillRect(iVec2(EndAttackPosX,SustainPosY), iVec2(EndDecayPosX-EndAttackPosX, drawADSR_EndY-SustainPosY), col1);
            mainImage->fillRect(iVec2(EndDecayPosX-1,SustainPosY), iVec2(StartReleasePosX-EndDecayPosX+2, drawADSR_EndY-SustainPosY), col1);
            mainImage->fillTriangle(iVec2(StartReleasePosX, SustainPosY), iVec2(StartReleasePosX, drawADSR_EndY-2), iVec2(EndReleasePosX, drawADSR_EndY-2), col1, col1);
        }

        const int Item1PosX = 22;
        const int Item1PosY = 36;
        const int Item1Scale = 2;
        const int Item1Rotation = 45;

        const int Item2PosX = 200;
        const int Item2PosY = 200;
        const int Item2Scale = 1;
        const int Item2Rotation = 45;

        const int Item3PosX = 298;
        const int Item3PosY = 36;
        const int Item3Scale = 2;
        const int Item3Rotation = 45;

        int Dial1PosX = 30;
        int Dial1PosY = 210;
        float Dial1Scale = 1.25;
        float Dial1Rotation = 0;

        int Dial2PosX = 160;
        int Dial2PosY = 210;
        float Dial2Scale = 1.25;
        float Dial2Rotation = 0;
        bool Dial2Sustain = false;
        String Dial2Text = "D";

        int Dial3PosX = 290;
        int Dial3PosY = 210;
        float Dial3Scale = 1.25;
        float Dial3Rotation = 0;

        int AttackValue; //0.0-250.0
        int DecayValue; //0.0-250.0
        float SustainValue; //0.0-1.0
        int ReleaseValue; //0.0-250.0


        MenuItem Item1 = {};
        MenuItem Item2 = {};
        MenuItem Item3 = {};

        const RGB32 col1 = RGB32(255, 0, 0);
        const RGB32 col2 = RGB32(20, 0, 2);
        const RGB32 col3 = RGB32(70, 0, 7);

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
        m_VolumeEnvelope(Image<RGB565>* mI, s_Synthesizer* s){
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

            Item2.image = CreateImage::Item;
            Item2.size.x = Item2.image.width();
            Item2.size.y = Item2.image.height();
            Item2.pos.x = Item2PosX;
            Item2.pos.y = Item2PosY;
            Item2.scale = Item2Scale;
            Item2.rot = Item2Rotation;

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
            Serial.println("Setup m_VolumeEnvelope");

            CreateImage::updateTransitionIn(col1);
            CreateImage::updateTransitionOut(TransitionColor());

            CreateImage::updateRectMenuItem(col1, col2);
            CreateImage::updateAltRectMenuItem(col2, col1);
            CreateImage::updateBackArrow(col1);
            CreateImage::updateDialMenuItem(col1, col2);
            CreateImage::createTextBox(&TextBox, "VOLUME ENVELOPE", 4, col2, col1, RGB565_White, font_Righteous_AA2_24);
        }
        void Setup(RGB565 TransitionINColor){
            Serial.println("Setup m_VolumeEnvelope");

            CreateImage::updateTransitionIn(TransitionINColor);
            CreateImage::updateTransitionOut(TransitionColor());

            CreateImage::updateRectMenuItem(col1, col2);
            CreateImage::updateAltRectMenuItem(col2, col1);
            CreateImage::updateBackArrow(col1);
            CreateImage::updateDialMenuItem(col1, col2);
            CreateImage::createTextBox(&TextBox, "VOLUME ENVELOPE", 4, col2, col1, RGB565_White, font_Righteous_AA2_24);
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
            Dial1Rotation = 300*(Dial1/1024.0)-210;
            Dial2Rotation = 300*(Dial2/1024.0)-210;
            Dial3Rotation = 300*(Dial3/1024.0)-210;
            
            if(B1Pressed){
                Item1.image = CreateImage::AltItem;
                CreateImage::updateBackArrow(col2);
            }
            else{
                Item1.image = CreateImage::Item;
                CreateImage::updateBackArrow(col1);
            }
            if(B2Pressed){
                Item2.image = CreateImage::AltItem;
            }
            else{
                Item2.image = CreateImage::Item;
            }
            if(B2IsJustReleased){
                if(Dial2Sustain){
                    Dial2Text = "D";
                }
                else{
                    Dial2Text = "S";
                }
                Dial2Sustain = !Dial2Sustain;
            }
            if(B3Pressed){
                Item3.image = CreateImage::AltItem;
            }
            else{
                Item3.image = CreateImage::Item;
            }
            
            AttackValue = 250 + 250*(Dial1/1024.0);
            if(!Dial2Sustain){
                DecayValue = 250 + 250*(Dial2/1024.0);
                SustainValue = synth->sustain;
            }
            else{
                DecayValue = synth->decay;
                SustainValue = 1 + (Dial2/1024.0);
            }
            ReleaseValue = 250 + 250*(Dial3/1024.0);

            synth->updateADSR(AttackValue, DecayValue, SustainValue, ReleaseValue);

            Draw();

            //Bitshifted bools to make it easier :)
            if(TransitionButton == 0 && TransitioningMenu == nullptr) {
                TransitionButton = B1IsJustReleased << 2;
                if(TransitionButton != 0) {
                    //Serial.println(TransitionButton);
                    if(TransitionButton == 0b100){
                        Serial.println("Button 1 Released... Activating Transition for Button 1");
                        TransitioningMenu = TransitionMenu1;
                        transitionOUTFlag = true;
                    }
                    //No other transitions needed :)
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
