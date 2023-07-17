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

        uint16_t item[16 * 16];
        uint16_t alt_item[16 * 16];
        uint16_t text_box[240 * 56];
        uint16_t transition_in[2*2];
        uint16_t transition_out[2*2];

        Image<RGB565> SampleMenuItem;
        Image<RGB565> SampleAltItem;
        Image<RGB565> TextBox;
        Image<RGB565> TransitionIn;
        Image<RGB565> TransitionOut;

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

        String Item1TopText = "SIGNAL";
        String Item1BottomText = "GENERATOR";
        String Item2TopText = "VOLUME";
        String Item2BottomText = "ENVELOPE";
        String Item3TopText = "VARIOUS";
        String Item3BottomText = "EFFECTS";

        int Item1PosX = 64;
        int Item2PosX = 160;
        int Item3PosX = 256;

        int Item1Scale = 4;
        int Item2Scale = 4;
        int Item3Scale = 4;

        int Item1Rotation = 45;
        int Item2Rotation = 45;
        int Item3Rotation = 45;

        MenuItem Item1 = {};
        MenuItem Item2 = {};
        MenuItem Item3 = {};

        RGB32 col1;
        RGB32 col2;
        RGB32 col3;

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
            SampleMenuItem = Image<RGB565>(item, 16, 16);
            SampleAltItem = Image<RGB565>(alt_item, 16, 16);
            TextBox = Image<RGB565>(text_box, 240, 56);
            TransitionIn = Image<RGB565>(transition_in, 2, 2);
            TransitionOut = Image<RGB565>(transition_out, 2, 2);


            Item1.image = SampleMenuItem;
            Item2.image = SampleMenuItem;
            Item3.image = SampleMenuItem;

            Item1.topText = Item1TopText;
            Item1.bottomText = Item1BottomText;
            Item2.topText = Item2TopText;
            Item2.bottomText = Item2BottomText;
            Item3.topText = Item3TopText;
            Item3.bottomText = Item3BottomText;

            Item1.size.x = SampleMenuItem.width();
            Item2.size.x = SampleMenuItem.width();
            Item3.size.x = SampleMenuItem.width();
            Item1.size.y = SampleMenuItem.height();
            Item2.size.y = SampleMenuItem.height();
            Item3.size.y = SampleMenuItem.height();

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

            col1 = RGB32(0, 0, 255);
            col2 = RGB32(0, 2, 20);
            col3 = RGB32(0, 7, 70);
        }
        void Link(Menu* Tm1, Menu* Tm2, Menu* Tm3){
            TransitionMenu1 = Tm1;
            TransitionMenu2 = Tm2;
            TransitionMenu3 = Tm3;
        }

        void Setup(){
            Serial.println("Setup m_MainMenu");

            CreateImage::createTransition(&TransitionIn, col1);
            CreateImage::createTransition(&TransitionOut, TransitionColor());

            CreateImage::createRectMenuItem(&SampleMenuItem, col1, col2, 16, 16, 1);
            CreateImage::createRectMenuItem(&SampleAltItem, col2, col1, 16, 16, 1);
            CreateImage::createTextBox(&TextBox, "MAIN MENU", 4, col2, col1, RGB565_White, font_Roboto_Bold_32);
        }
        void Setup(RGB565 TransitionINColor){ 
            Serial.println("Setup m_MainMenu");

            CreateImage::createTransition(&TransitionIn, TransitionINColor);
            CreateImage::createTransition(&TransitionOut, TransitionColor());

            CreateImage::createRectMenuItem(&SampleMenuItem, col1, col2, 16, 16, 1);
            CreateImage::createRectMenuItem(&SampleAltItem, col2, col1, 16, 16, 1);
            CreateImage::createTextBox(&TextBox, "MAIN MENU", 4, col2, col1, RGB565_White, font_Roboto_Bold_32);
        }

        //Transition IN should ONLY be Drawn if transitionINFlag is True.
        bool TransitionINFlag(){
            return transitionINFlag;
        }
        void TransitionIN(){
            Draw();
            mainImage->blitScaledRotated(TransitionIn, fVec2(TransitionOut.width()/2, TransitionOut.height()/2), fVec2(mainImage->width()/2, mainImage->height()/2), TransitionScale, 45);
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
            B1Pressed ? Item1.image = SampleAltItem : Item1.image = SampleMenuItem;
            B2Pressed ? Item2.image = SampleAltItem : Item2.image = SampleMenuItem;
            B3Pressed ? Item3.image = SampleAltItem : Item3.image = SampleMenuItem;

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
                    mainImage->blitScaledRotated(TransitionOut, fVec2(TransitionOut.width()/2, TransitionOut.height()/2), fVec2(Item1.pos.x, Item1.pos.y), TransitionScale, 45);
                    TransitionScale *= TRANSITION_RATE;
                } else if (TransitionButton >> 1) {
                    Draw();
                    mainImage->blitScaledRotated(TransitionOut, fVec2(TransitionOut.width()/2, TransitionOut.height()/2), fVec2(Item2.pos.x, Item2.pos.y), TransitionScale, 45);
                    TransitionScale *= TRANSITION_RATE;
                } else if (TransitionButton) {
                    Draw();
                    mainImage->blitScaledRotated(TransitionOut, fVec2(TransitionOut.width()/2, TransitionOut.height()/2), fVec2(Item3.pos.x, Item3.pos.y), TransitionScale, 45);
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
