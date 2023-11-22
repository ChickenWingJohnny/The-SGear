#pragma once

#include <ILI9341_T4.h>
#include <tgx.h>

#include "CreateImage.h"

#include "m_MenuItem.h"
#include "m_Menu.h"

using namespace tgx;

class m_MainMenu : public Menu {
    private:

        Image<RGB565>* mainImage;
        uint16_t text_box[240 * 56];
        Image<RGB565> TextBox;

        //The Main Menu Loop Without Inputs
        void Draw(){
            Item1.pos.y = 145 + (5 * sin(0.5 * PI * millis() / 1000.0));
            Item2.pos.y = 145 + (5 * sin(0.25*PI + (0.5 * PI * millis() / 1000.0)));
            Item3.pos.y = 145 + (5 * sin(0.5*PI + (0.5 * PI * millis() / 1000.0)));

            mainImage->fillScreenVGradient(RGB565_Black, col3);

            //Menu Items
            mainImage->blitScaledRotated(Item1.image, fVec2(Item1.size.x/2, Item1.size.y/2), fVec2(Item1.pos.x, Item1.pos.y), Item1Scale, Item1Rotation);
            mainImage->blitScaledRotated(Item2.image, fVec2(Item2.size.x/2, Item2.size.y/2), fVec2(Item2.pos.x, Item2.pos.y), Item2Scale, Item2Rotation);
            mainImage->blitScaledRotated(Item3.image, fVec2(Item3.size.x/2, Item3.size.y/2), fVec2(Item3.pos.x, Item3.pos.y), Item3Scale, Item3Rotation);

            //Menu Text
            CreateImage::placeText(mainImage, Item1.topText, iVec2(Item1.pos.x, Item1.pos.y - 54), RGB565_White, font_Roboto_Bold_14, 1.0F);
            CreateImage::placeText(mainImage, Item1.bottomText, iVec2(Item1.pos.x, Item1.pos.y + 54), RGB565_White, font_Roboto_Bold_14, 1.0F);
            CreateImage::placeText(mainImage, Item2.topText, iVec2(Item2.pos.x, Item2.pos.y - 54), RGB565_White, font_Roboto_Bold_14, 1.0F);
            CreateImage::placeText(mainImage, Item2.bottomText, iVec2(Item2.pos.x, Item2.pos.y + 54), RGB565_White, font_Roboto_Bold_14, 1.0F);
            CreateImage::placeText(mainImage, Item3.topText, iVec2(Item3.pos.x, Item3.pos.y - 54), RGB565_White, font_Roboto_Bold_14, 1.0F);
            CreateImage::placeText(mainImage, Item3.bottomText, iVec2(Item3.pos.x, Item3.pos.y + 54), RGB565_White, font_Roboto_Bold_14, 1.0F);

            //Title
            mainImage->blitScaledRotated(TextBox, fVec2(TextBox.width()/2, TextBox.height()/2), fVec2(160, 36), 1, 0);
        }

        const String Item1TopText = "SIGNAL";
        const String Item1BottomText = "GENERATOR";
        const String Item2TopText = "VOLUME";
        const String Item2BottomText = "ENVELOPE";
        const String Item3TopText = "VARIOUS";
        const String Item3BottomText = "EFFECTS";

        const int Item1PosX = 64;
        const int Item2PosX = 160;
        const int Item3PosX = 256;

        const int Item1Scale = 4;
        const int Item2Scale = 4;
        const int Item3Scale = 4;

        const int Item1Rotation = 45;
        const int Item2Rotation = 45;
        const int Item3Rotation = 45;

        MenuItem Item1 = {};
        MenuItem Item2 = {};
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
        m_MainMenu(Image<RGB565>* mI){
            mainImage = mI;
            TextBox = Image<RGB565>(text_box, 240, 56);

            Item1.image = CreateImage::Item;
            Item2.image = CreateImage::Item;
            Item3.image = CreateImage::Item;

            Item1.topText = Item1TopText;
            Item1.bottomText = Item1BottomText;
            Item2.topText = Item2TopText;
            Item2.bottomText = Item2BottomText;
            Item3.topText = Item3TopText;
            Item3.bottomText = Item3BottomText;

            Item1.size.x = Item1.image.width();
            Item2.size.x = Item2.image.width();
            Item3.size.x = Item3.image.width();
            Item1.size.y = Item1.image.height();
            Item2.size.y = Item2.image.height();
            Item3.size.y = Item3.image.height();

            Item1.pos.x = Item1PosX;
            Item2.pos.x = Item2PosX;
            Item3.pos.x = Item3PosX;
            //Pos Y is not constant.

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

        void Setup(){
            Serial.println("Setup m_MainMenu");

            CreateImage::updateTransitionIn(col1);
            CreateImage::updateTransitionOut(TransitionColor());

            CreateImage::updateRectMenuItem(col1, col2);
            CreateImage::updateAltRectMenuItem(col2, col1);
            CreateImage::createTextBox(&TextBox, "MAIN MENU", 4, col2, col1, RGB565_White, font_Righteous_AA2_32);
        }
        void Setup(RGB565 TransitionINColor){
            Serial.println("Setup m_MainMenu");

            CreateImage::updateTransitionIn(TransitionINColor);
            CreateImage::updateTransitionOut(TransitionColor());

            CreateImage::updateRectMenuItem(col1, col2);
            CreateImage::updateAltRectMenuItem(col2, col1);
            CreateImage::createTextBox(&TextBox, "MAIN MENU", 4, col2, col1, RGB565_White, font_Righteous_AA2_32);
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
            B1Pressed ? Item1.image = CreateImage::AltItem : Item1.image = CreateImage::Item;
            B2Pressed ? Item2.image = CreateImage::AltItem : Item2.image = CreateImage::Item;
            B3Pressed ? Item3.image = CreateImage::AltItem : Item3.image = CreateImage::Item;
            
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
                        Serial.println("Button 2 Released... Activating Transition for Button 2");
                        TransitioningMenu = TransitionMenu2;
                    }
                    else if (TransitionButton == 0b001){
                        Serial.println("Button 3 Released... Activating Transition for Button 3");
                        TransitioningMenu = TransitionMenu3;
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
                    Draw();
                    mainImage->blitScaledRotated(CreateImage::TransitionOut, fVec2(CreateImage::TransitionOut.width()/2, CreateImage::TransitionOut.height()/2), fVec2(Item2.pos.x, Item2.pos.y), TransitionScale, 45);
                    TransitionScale *= TRANSITION_RATE;
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
            Serial.println("Destroying m_MainMenu");

            // free(mainImage);

            // free(item);
            // free(text_box);
            // free(transition_in);
            // free(transition_out);

            // free(SampleMenuItem);
            // free(TextBox);
            // free(TransitionIn);
            // free(TransitionOut);

            transitionINFlag = true;
            transitionOUTFlag = false;
            transitionOUTDone = false;
            TransitionButton = 0;
            TransitioningMenu = nullptr;
        }
};
