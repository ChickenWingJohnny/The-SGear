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

        uint16_t item[16 * 16];
        uint16_t alt_item[16 * 16];
        uint16_t item2[24 * 24];
        uint16_t alt_item2[24 * 24];
        uint16_t text_box[300 * 56];
        uint16_t transition_in[2*2];
        uint16_t transition_out[2*2];
        uint16_t transition_out2[240*320];

        Image<RGB565> MenuItem;
        Image<RGB565> AltItem;
        Image<RGB565> MenuItem2;
        Image<RGB565> AltItem2;
        Image<RGB565> TextBox;
        Image<RGB565> TransitionIn;
        Image<RGB565> TransitionOut;
        Image<RGB565> TransitionOut2;

        String Item1Text = "MODULATION";
        String Item2Text = "OSCILATORS";
        String Item3Text = "FILTERS";

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
        double TransitionRotation = 0;
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
            Item1.pos.y = 135 + (5 * sin(0.5 * PI * millis() / 1000.0));
            Item2.pos.y = 135 + (5 * sin(0.25*PI + (0.5 * PI * millis() / 1000.0)));
            Item3.pos.y = 135 + (5 * sin(0.5*PI + (0.5 * PI * millis() / 1000.0)));

            mainImage->fillScreenVGradient(RGB565_Black, col3);

            //Menu Items
            mainImage->blitScaledRotated(Item1.image, fVec2(Item1.size.x/2, Item1.size.y/2), fVec2(Item1.pos.x, Item1.pos.y), Item1Scale, Item1Rotation);
            mainImage->blitScaledRotatedMasked(Item2.image, RGB565_Black, fVec2(Item2.size.x/2, Item2.size.y/2), fVec2(Item2.pos.x, Item2.pos.y), Item2Scale, Item2Rotation, 1.0);
            mainImage->blitScaledRotated(Item3.image, fVec2(Item3.size.x/2, Item3.size.y/2), fVec2(Item3.pos.x, Item3.pos.y), Item3Scale, Item3Rotation);

            //Menu Text
            CreateImage::placeText(mainImage, Item1.bottomText, iVec2(Item1.pos.x, Item1.pos.y + 54), RGB565_White, font_Roboto_Bold_14, 1.0);
            CreateImage::placeText(mainImage, Item2.bottomText, iVec2(Item2.pos.x, Item2.pos.y + 72), RGB565_White, font_Roboto_Bold_14, 1.0);
            CreateImage::placeText(mainImage, Item3.bottomText, iVec2(Item3.pos.x, Item3.pos.y + 54), RGB565_White, font_Roboto_Bold_14, 1.0);

            //Title
            mainImage->blitScaledRotated(TextBox, fVec2(TextBox.width()/2, TextBox.height()/2), fVec2(160, 36), 1, 0);
        }

    public:
        m_SignalGenerator(Image<RGB565>* mI){
            mainImage = mI;
            MenuItem = Image<RGB565>(item, 16, 16);
            AltItem = Image<RGB565>(alt_item, 16, 16);
            MenuItem2 = Image<RGB565>(item2, 24, 24);
            AltItem2 = Image<RGB565>(alt_item2, 24, 24);
            TextBox = Image<RGB565>(text_box, 300, 56);
            TransitionIn = Image<RGB565>(transition_in, 2, 2);
            TransitionOut = Image<RGB565>(transition_out, 2, 2);
            TransitionOut2 = Image<RGB565>(transition_out2, 320, 240);

            Item1.bottomText = Item1Text;
            Item2.bottomText = Item2Text;
            Item3.bottomText = Item3Text;

            Item1.image = MenuItem;
            Item2.image = MenuItem2;
            Item3.image = MenuItem;

            Item1.size.x = MenuItem.width();
            Item2.size.x = MenuItem2.width();
            Item3.size.x = MenuItem.width();
            Item1.size.y = MenuItem.height();
            Item2.size.y = MenuItem2.height();
            Item3.size.y = MenuItem.height();

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

            col1 = RGB32(0, 255, 0);
            col2 = RGB32(2, 20, 0);
            col3 = RGB32(7, 70, 0);
        }
        void Link(Menu* Tm1, Menu* Tm2, Menu* Tm3){
            TransitionMenu1 = Tm1;
            TransitionMenu2 = Tm2;
            TransitionMenu3 = Tm3;
        }
        void Setup(){
            Serial.println("Setup m_SignalGenerator");

            CreateImage::createTransition(&TransitionIn, col1);
            CreateImage::createTransition(&TransitionOut, TransitionColor());

            CreateImage::createRectMenuItem(&MenuItem, col1, col2, 16, 16, 1);
            CreateImage::createRectMenuItem(&AltItem, col2, col1, 16, 16, 1);
            CreateImage::createDiamondMenuItem(&MenuItem2, col1, col2, 24, 24, 1);
            CreateImage::createDiamondMenuItem(&AltItem2, col2, col2, 24, 24, 1);
            CreateImage::createTextBox(&TextBox, "SIGNAL GENERATOR", 4, col2, col1, RGB565_White, font_Roboto_Bold_18);
        }
        void Setup(RGB565 TransitionINColor){
            Serial.println("Setup m_SignalGenerator");

            CreateImage::createTransition(&TransitionIn, TransitionINColor);
            CreateImage::createTransition(&TransitionOut, TransitionColor());

            CreateImage::createRectMenuItem(&MenuItem, col1, col2, 16, 16, 1);
            CreateImage::createRectMenuItem(&AltItem, col2, col1, 16, 16, 1);
            CreateImage::createDiamondMenuItem(&MenuItem2, col1, col2, 24, 24, 1);
            CreateImage::createDiamondMenuItem(&AltItem2, col2, col2, 24, 24, 1);
            CreateImage::createTextBox(&TextBox, "SIGNAL GENERATOR", 4, col2, col1, RGB565_White, font_Roboto_Bold_18);
        }

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

        //Main Menu Loop with Inputs.
        void Draw(  bool B1Pressed, bool B2Pressed, bool B3Pressed, 
                    bool B1IsJustReleased, bool B2IsJustReleased, bool B3IsJustReleased,
                    int Dial1, int Dial2, int Dial3
        ){
            
            B1Pressed ? Item1.image = AltItem : Item1.image = MenuItem;
            B2Pressed ? Item2.image = AltItem2 : Item2.image = MenuItem2;
            B3Pressed ? Item3.image = AltItem : Item3.image = MenuItem;

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
                        mainImage->blitBackward(TransitionOut2, iVec2(0,0));
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
                    mainImage->blitScaledRotated(TransitionOut, fVec2(TransitionOut.width()/2, TransitionOut.height()/2), fVec2(Item1.pos.x, Item1.pos.y), TransitionScale, 45);
                    TransitionScale *= TRANSITION_RATE;
                } else if (TransitionButton >> 1) {
                    //TODO: MAKE THIS A COOL TRANSITION :)
                    // TransitionScale += 0.5;
                    // TransitionRotation += 1;
                    // if(TransitionScale > TRANSITION_MAX2) TransitionScale = TRANSITION_MAX2;
                    // if(TransitionRotation > TRANSITION_ROT_MAX) TransitionRotation = TRANSITION_ROT_MAX;
                    
                    Draw();
                    mainImage->blitScaledRotated(TransitionOut, fVec2(TransitionOut.width()/2, TransitionOut.height()/2), fVec2(Item1.pos.x, Item1.pos.y), TransitionScale, 45);
                    TransitionScale *= TRANSITION_RATE;

                    mainImage->blitScaledRotated(TransitionOut2, fVec2(Item2.pos.x, Item2.pos.y), fVec2(Item2.pos.x, Item2.pos.y), TransitionScale, TransitionRotation);
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
            transitionOUTDone = false;
            TransitionButton = 0;
            TransitioningMenu = nullptr;
        }
};
