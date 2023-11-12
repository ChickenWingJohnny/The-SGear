#pragma once

#include <ILI9341_T4.h>
#include <tgx.h>

#include "m_MenuItem.h"
#include "m_Menu.h"

#include "CreateImage.h"

using namespace tgx;

class m_SignalGenerator : public Menu {
    private:

        Image<RGB565>* mainImage;
        uint16_t text_box[300 * 56];
        Image<RGB565> TextBox;

        int Item1PosX = 64;
        int Item2PosX = 160;
        int Item3PosX = 256;
        fVec2 TextBoxPos = fVec2(160, 36);

        int Item1PosY = 135;
        int Item2PosY = 135;
        int Item3PosY = 135;

        int Item1Scale = 4;
        int Item2Scale = 4;
        int Item3Scale = 4;
        float TextBoxScale = 1;

        int Item1Rotation = 45;
        int Item2Rotation = 45;
        int Item3Rotation = 45;
        float TextBoxRotation = 0;

        String Item1Text = "MODULATION";
        String Item2Text = "OSCILATORS";
        String Item3Text = "FILTERS";
        iVec2 Item1TextPos = iVec2(Item1PosX, Item1PosY + 54);
        iVec2 Item2TextPos = iVec2(Item2PosX, Item2PosY + 78);
        iVec2 Item3TextPos = iVec2(Item3PosX, Item3PosY + 54);

        MenuItem Item1 = {};
        MenuItem Item2 = {};
        MenuItem Item3 = {};

        const RGB32 col1 = RGB32(0, 255, 0);
        const RGB32 col2 = RGB32(2, 20, 0);   
        const RGB32 col3 = RGB32(7, 70, 0);

        bool transitionINFlag = true;
        bool transitionOUTFlag = false;
        bool transitionOUTDone = false;
        uint TransitionButton = 0;

        double TransitionScale = 700;
        int timeTransOUT;
        bool TransitionPart2 = false;
        const int TRANSITION_MIN = 1;
        const int TRANSITION_MAX = 700;
        const int TRANSITION_MAX2 = 2.5;
        const int TRANSITION_ROT_MAX = 90;
        const double TRANSITION_RATE = 1.20;

        Menu* TransitionMenu1;
        Menu* TransitionMenu2;
        Menu* TransitionMenu3;

        Menu* TransitioningMenu;

        //The Main Menu Loop Without Inputs
        void Draw(){
            mainImage->fillScreenVGradient(RGB565_Black, col3);

            //Menu Items
            mainImage->blitScaledRotated(Item1.image, fVec2(Item1.size.x/2, Item1.size.y/2), fVec2(Item1.pos.x, Item1.pos.y), Item1.scale, Item1.rot);
            mainImage->blitScaledRotatedMasked(Item2.image, RGB565_Black, fVec2(Item2.size.x/2, Item2.size.y/2), fVec2(Item2.pos.x, Item2.pos.y), Item2.scale, Item2.rot, 1.0);
            mainImage->blitScaledRotated(Item3.image, fVec2(Item3.size.x/2, Item3.size.y/2), fVec2(Item3.pos.x, Item3.pos.y), Item3.scale, Item3.rot);

            //Menu Text
            CreateImage::placeText(mainImage, Item1.bottomText, Item1TextPos, RGB565_White, font_Roboto_Bold_14, 1.0);
            CreateImage::placeText(mainImage, Item2.bottomText, Item2TextPos, RGB565_White, font_Roboto_Bold_14, 1.0);
            CreateImage::placeText(mainImage, Item3.bottomText, Item3TextPos, RGB565_White, font_Roboto_Bold_14, 1.0);

            //Title
            mainImage->blitScaledRotated(TextBox, fVec2(TextBox.width()/2, TextBox.height()/2), TextBoxPos, TextBoxScale, TextBoxRotation);
        }

        void resetItemPositions(){
            Item1.bottomText = Item1Text;
            Item2.bottomText = Item2Text;
            Item3.bottomText = Item3Text;

            Item1.image = CreateImage::Item;
            Item2.image = CreateImage::Item2;
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

            TextBoxPos = fVec2(160, 36);

            Item1TextPos = iVec2(Item1PosX, Item1PosY + 54);
            Item2TextPos = iVec2(Item2PosX, Item2PosY + 78);
            Item3TextPos = iVec2(Item3PosX, Item3PosY + 54);
        }

    public:
        m_SignalGenerator(Image<RGB565>* mI){
            mainImage = mI;
            TextBox = Image<RGB565>(text_box, 300, 56);

            resetItemPositions();
        }
        void Link(Menu* Tm1, Menu* Tm2, Menu* Tm3){
            TransitionMenu1 = Tm1;
            TransitionMenu2 = Tm2;
            TransitionMenu3 = Tm3;
        }
        void Setup(){
            Serial.println("Setup m_SignalGenerator");

            resetItemPositions();

            CreateImage::updateTransitionIn(col1);
            CreateImage::updateTransitionOut(TransitionColor());

            CreateImage::updateRectMenuItem(col1, col2);
            CreateImage::updateAltRectMenuItem(col2, col1);
            CreateImage::updateDiamondMenuItem(col1, col2);
            CreateImage::updateAltDiamondMenuItem(col2, col2);
            CreateImage::createTextBox(&TextBox, "SIGNAL GENERATOR", 4, col2, col1, RGB565_White, font_Roboto_Bold_28);
        }
        void Setup(RGB565 TransitionINColor){
            Serial.println("Setup m_SignalGenerator");

            resetItemPositions();

            CreateImage::updateTransitionIn(TransitionINColor);
            CreateImage::updateTransitionOut(TransitionColor());

            CreateImage::updateRectMenuItem(col1, col2);
            CreateImage::updateAltRectMenuItem(col2, col1);
            CreateImage::updateDiamondMenuItem(col1, col2);
            CreateImage::updateAltDiamondMenuItem(col2, col1);
            CreateImage::createTextBox(&TextBox, "SIGNAL GENERATOR", 4, col2, col1, RGB565_White, font_Roboto_Bold_28);
        }

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

        //Main Menu Loop with Inputs.
        void Draw(  bool B1Pressed, bool B2Pressed, bool B3Pressed, 
                    bool B1IsJustReleased, bool B2IsJustReleased, bool B3IsJustReleased,
                    int Dial1, int Dial2, int Dial3
        ){
            
            B1Pressed ? Item1.image = CreateImage::AltItem : Item1.image = CreateImage::Item;
            B2Pressed ? Item2.image = CreateImage::AltItem2 : Item2.image = CreateImage::Item2;
            B3Pressed ? Item3.image = CreateImage::AltItem : Item3.image = CreateImage::Item;

            Draw();

            //Bitshifted bools to make it easier :)
            //Could move Transition out here???
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
                        Serial.println("Button 2 Released... Activating Transition for Button 2");
                        TransitioningMenu = TransitionMenu2;
                        timeTransOUT = millis();
                    }
                    else if (TransitionButton == 0b001){
                        Serial.println("Button 3 Released... Activating Transition for Button 3");
                        TransitioningMenu = TransitionMenu3;
                    }     
                }
            }
        }

        bool TransitionOUTFlag(){
            return transitionOUTFlag;
        }
        bool TransitionOUTDone(){
            return transitionOUTDone;
        }
        void TransitionOUT(){
            int timeElapsed = millis() - timeTransOUT;
            //Serial.println("Scale: " + TransitionScale);
            // if((TransitionButton & 0b001 || TransitionButton >> 2)) {
            //     if(TransitionScale > TRANSITION_MAX){
            //         mainImage->fillScreen(TransitionColor());
            //         Serial.println("Transition to ... Done.");
            //         transitionOUTDone = true;
            //     }
            // }
            // else if(TransitionButton >> 1){
            //     if(TransitionScale == TRANSITION_MAX2 && TransitionRotation == TRANSITION_ROT_MAX){
            //         Serial.println("Transition to Oscilators Done.");
            //         transitionOUTDone = true;
            //     }
            // }

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
                } else if (TransitionButton >> 1) {
                    //TODO: MAKE THIS A COOL TRANSITION :)
                    if(timeElapsed < 500 && !TransitionPart2){
                        Item3TextPos.x += (timeElapsed/150.0);
                        Item3.pos.x += (timeElapsed/150.0);
                        Item2TextPos.y += (timeElapsed/150.0);
                        Item1TextPos.x -= (timeElapsed/150.0);
                        Item1.pos.x -= (timeElapsed/150.0);
                        TextBoxPos.y -= (timeElapsed/150.0);

                        Item2.pos = iVec2(Item2PosX, 20*(-timeElapsed/1000.0) + Item2PosY);
                        Item2.rot = 180 * (timeElapsed/1000.0) + Item2Rotation;
                        Item2.scale = 3 * (timeElapsed/1000.0) + Item2Scale;
                        Draw();
                    } else if(timeElapsed > 500 && !TransitionPart2) {
                        CreateImage::updateTransitionIn(col2);
                        CreateImage::updateTransitionOut(col1);
                        timeTransOUT = millis();
                        TransitionPart2 = true;
                    } else if (timeElapsed < 500 && TransitionPart2) {
                        mainImage->fillScreenVGradient(RGB565_Black, col3);

                        int Osc0x = 60*(-timeElapsed/1000.0) + 130; //to 100 in 0.5 seconds
                        int Osc1x = 60*(timeElapsed/1000.0) + 190; //to 220 in 0.5 seconds

                        //Items should be 5.5 scale

                        mainImage->blitScaledRotated(CreateImage::TransitionOut, fVec2(CreateImage::TransitionOut.lx()/2, CreateImage::TransitionOut.ly()/2), fVec2(Osc0x, 120), 44.0F, 45);
                        mainImage->blitScaledRotated(CreateImage::TransitionOut, fVec2(CreateImage::TransitionOut.lx()/2, CreateImage::TransitionOut.ly()/2), fVec2(Osc1x, 120), 44.0F, 45);
                        mainImage->blitScaledRotated(CreateImage::TransitionIn, fVec2(CreateImage::TransitionIn.lx()/2, CreateImage::TransitionIn.ly()/2), fVec2(Osc0x, 120), 40.0F, 45);
                        mainImage->blitScaledRotated(CreateImage::TransitionIn, fVec2(CreateImage::TransitionIn.lx()/2, CreateImage::TransitionIn.ly()/2), fVec2(Osc1x, 120), 40.0F, 45);
                    }
                    else{
                        transitionOUTDone = true;
                    }
                    
                    //mainImage->blitScaledRotated(CreateImage::TransitionOut, fVec2(CreateImage::TransitionOut.width()/2, CreateImage::TransitionOut.height()/2), fVec2(Item1.pos.x, Item1.pos.y), TransitionScale, 45);
                    //TransitionScale *= TRANSITION_RATE;
                } else if (TransitionButton) {
                    Draw();
                    mainImage->blitScaledRotated(CreateImage::TransitionOut, fVec2(CreateImage::TransitionOut.width()/2, CreateImage::TransitionOut.height()/2), fVec2(Item3.pos.x, Item3.pos.y), TransitionScale, 45);
                    TransitionScale *= TRANSITION_RATE;
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
            Serial.println("Destroying m_SignalGenerator");

            // free(mainImage);

            // free(item);
            // free(text_box);
            // free(transition_in);
            // free(transition_out);

            // free(MenuItem);
            // free(TextBox);
            // free(TransitionIn);
            // free(TransitionOut);

            transitionINFlag = true;
            transitionOUTFlag = false;
            TransitionPart2 = false;
            transitionOUTDone = false;
            TransitionButton = 0;
            TransitioningMenu = nullptr;
        }
};
