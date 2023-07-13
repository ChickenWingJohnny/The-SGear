#pragma once

#include <ILI9341_T4.h>
#include <tgx.h>

#include "m_MenuItem.h"
#include "m_Menu.h"

using namespace tgx;

class m_SignalGenerator : public Menu {
    private:

        Image<RGB565>* mainImage;

        uint16_t item[32 * 32];
        uint16_t alt_item[32 * 32];
        uint16_t item2[48 * 48];
        uint16_t alt_item2[48 * 48];
        uint16_t text_box[300 * 56];
        uint16_t transition_in[2*2];
        uint16_t transition_out[2*2];
        uint16_t transition_out2[240*320];

        Image<RGB565> SampleMenuItem;
        Image<RGB565> SampleAltItem;
        Image<RGB565> SampleMenuItem2;
        Image<RGB565> SampleAltItem2;
        Image<RGB565> TextBox;
        Image<RGB565> TransitionIn;
        Image<RGB565> TransitionOut;
        Image<RGB565> TransitionOut2;
        
        void createSampleMenuItem(RGB565 outerColor, RGB565 innerColor){
            SampleMenuItem.fillScreen(outerColor);
            SampleMenuItem.fillRect(2, 2, 28, 28, innerColor);
        }
        void createSampleAltItem(RGB565 outerColor, RGB565 innerColor){
            SampleAltItem.fillScreen(outerColor);
            SampleAltItem.fillRect(2, 2, 28, 28, innerColor);
        }

        void createSampleMenuItem2(RGB565 outerColor, RGB565 innerColor){
            SampleMenuItem2.fillRect(0, 0, 32, 32, outerColor);
            SampleMenuItem2.fillRect(16, 16, 32, 32, outerColor);
            SampleMenuItem2.fillRect(2, 2, 28, 28, innerColor);
            SampleMenuItem2.fillRect(18, 18, 28, 28, innerColor);
        }
        void createSampleAltItem2(RGB565 outerColor, RGB565 innerColor){
            SampleAltItem2.fillRect(0, 0, 32, 32, outerColor);
            SampleAltItem2.fillRect(16, 16, 32, 32, outerColor);
            SampleAltItem2.fillRect(2, 2, 28, 28, innerColor);
            SampleAltItem2.fillRect(18, 18, 28, 28, innerColor);
        }

        void createTransitionIn(RGB565 transitionInColor){
            TransitionIn.fillScreen(transitionInColor);
        }
        void createTransitionOut(RGB565 transitionOutColor){
            TransitionOut.fillScreen(transitionOutColor);
        }

        void placeText(Image<RGB565> tgtImg, String text, iVec2 middlePos, RGB565 col, const ILI9341_t3_font_t& font, float opacity){
            char chars[text.length()];
            text.toCharArray(chars, text.length() + 1);
            iBox2 textBoundingBox = tgtImg.measureText(chars, middlePos, font);
            iVec2 textAncPos(textBoundingBox.lx()/2.0, textBoundingBox.ly()/2.0);
            
            iVec2 pos(middlePos.x - textAncPos.x, middlePos.y + textAncPos.y);
            tgtImg.drawText(chars, pos, col, font, false, opacity);
        }
        void createTextBox(String text, int strokeWidth, RGB565 innerColor, RGB565 outerColor, RGB565 textColor, const ILI9341_t3_font_t& font){
            TextBox.fillScreen(outerColor);
            TextBox.fillRect(strokeWidth, strokeWidth, TextBox.lx() - (2*strokeWidth), TextBox.ly() - (2*strokeWidth), innerColor);
            placeText(TextBox, text, iVec2(TextBox.lx()/2, TextBox.ly()/2), textColor, font, 1.0);
        }

        String Item1Text = "MODULATION";
        String Item2Text = "OSCILATORS";
        String Item3Text = "FILTERS";

        int Item1PosX = 64;
        int Item2PosX = 160;
        int Item3PosX = 256;

        int Item1Scale = 2;
        int Item2Scale = 2;
        int Item3Scale = 2;

        int Item1Rotation = 45;
        int Item2Rotation = 45;
        int Item3Rotation = 45;

        MenuItem Item1 = {};
        MenuItem Item2 = {};
        MenuItem Item3 = {};

        RGB32 col1;
        RGB32 col2;
        RGB32 col3;

        bool TransitionINDone = false;
        bool TransitionOUTFlag = false;
        bool TransitionOUTDone = false;
        uint TransitionButton = 0;

        double TransitionScale = 200;
        double TransitionRotation = 0;
        const int TRANSITION_MIN = 1;
        const int TRANSITION_MAX = 200;
        const int TRANSITION_MAX2 = 2.5;
        const int TRANSITION_ROT_MAX = 90;
        const double TRANSITION_RATE = 1.20;

        Menu* TransitionMenu1;
        Menu* TransitionMenu2;
        Menu* TransitionMenu3;

        Menu* TransitioningMenu;

    public:
        m_SignalGenerator(Image<RGB565>* mI){
            mainImage = mI;
            SampleMenuItem = Image<RGB565>(item, 32, 32);
            SampleAltItem = Image<RGB565>(alt_item, 32, 32);
            SampleMenuItem2 = Image<RGB565>(item2, 48, 48);
            SampleAltItem2 = Image<RGB565>(alt_item2, 48, 48);
            TextBox = Image<RGB565>(text_box, 300, 56);
            TransitionIn = Image<RGB565>(transition_in, 2, 2);
            TransitionOut = Image<RGB565>(transition_out, 2, 2);
            TransitionOut2 = Image<RGB565>(transition_out2, 320, 240);

            Item1.bottomText = Item1Text;
            Item2.bottomText = Item2Text;
            Item3.bottomText = Item3Text;

            Item1.image = SampleMenuItem;
            Item2.image = SampleMenuItem2;
            Item3.image = SampleMenuItem;

            Item1.size.x = SampleMenuItem.width();
            Item2.size.x = SampleMenuItem2.width();
            Item3.size.x = SampleMenuItem.width();
            Item1.size.y = SampleMenuItem.height();
            Item2.size.y = SampleMenuItem2.height();
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
            createTransitionIn(col1);
            createTransitionOut(TransitionColor());

            createSampleMenuItem(col1, col2);
            createSampleAltItem(col2, col1);
            createSampleMenuItem2(col1, col2);
            createSampleAltItem2(col2, col1);
            createTextBox("SIGNAL GENERATOR", 4, col2, col1, RGB565_White, font_Roboto_Bold_18);
        }
        void Setup(RGB565 TransitionINColor){
            createTransitionIn(TransitionINColor);
            createTransitionOut(TransitionColor());

            createSampleMenuItem(col1, col2);
            createSampleAltItem(col2, col1);
            createSampleMenuItem2(col1, col2);
            createSampleAltItem2(col2, col1);
            createTextBox("SIGNAL GENERATOR", 4, col2, col1, RGB565_White, font_Roboto_Bold_28);
        }

        void TransitionIN(){
            if(!TransitionINDone){
                mainImage->blitScaledRotated(TransitionIn, fVec2(TransitionOut.width()/2, TransitionOut.height()/2), fVec2(mainImage->width()/2, mainImage->height()/2), TransitionScale, 45);
                TransitionScale /= TRANSITION_RATE;
                if(TransitionScale <= TRANSITION_MIN) {
                    TransitionINDone = true;
                }
            }
        }

        bool IsTransitionINDone(){
            return TransitionINDone;
        }

        //The Main Menu Loop.
        void Draw(  bool B1Pressed, bool B2Pressed, bool B3Pressed, 
                    bool B1IsJustReleased, bool B2IsJustReleased, bool B3IsJustReleased,
                    int Dial1, int Dial2, int Dial3
        ){
            
            Item1.pos.y = 135 + (5 * sin(0.5 * PI * millis() / 1000.0));
            Item2.pos.y = 135 + (5 * sin(0.25*PI + (0.5 * PI * millis() / 1000.0)));
            Item3.pos.y = 135 + (5 * sin(0.5*PI + (0.5 * PI * millis() / 1000.0)));

            mainImage->fillScreenVGradient(RGB565_Black, col3);

            B1Pressed ? Item1.image = SampleAltItem : Item1.image = SampleMenuItem;
            B2Pressed ? Item2.image = SampleAltItem2 : Item2.image = SampleMenuItem2;
            B3Pressed ? Item3.image = SampleAltItem : Item3.image = SampleMenuItem;

            //Menu Items
            mainImage->blitScaledRotated(Item1.image, fVec2(Item1.size.x/2, Item1.size.y/2), fVec2(Item1.pos.x, Item1.pos.y), Item1Scale, Item1Rotation);
            mainImage->blitScaledRotatedMasked(Item2.image, RGB565_Black, fVec2(Item2.size.x/2, Item2.size.y/2), fVec2(Item2.pos.x, Item2.pos.y), Item2Scale, Item2Rotation, 1.0);
            mainImage->blitScaledRotated(Item3.image, fVec2(Item3.size.x/2, Item3.size.y/2), fVec2(Item3.pos.x, Item3.pos.y), Item3Scale, Item3Rotation);

            //Menu Text
            placeText(*mainImage, Item1.bottomText, iVec2(Item1.pos.x, Item1.pos.y + 54), RGB565_White, font_Roboto_Bold_14, 1.0);
            placeText(*mainImage, Item2.bottomText, iVec2(Item2.pos.x, Item2.pos.y + 72), RGB565_White, font_Roboto_Bold_14, 1.0);
            placeText(*mainImage, Item3.bottomText, iVec2(Item3.pos.x, Item3.pos.y + 54), RGB565_White, font_Roboto_Bold_14, 1.0);

            //Title
            mainImage->blitScaledRotated(TextBox, fVec2(TextBox.width()/2, TextBox.height()/2), fVec2(160, 36), 1, 0);

            //Bitshifted bools to make it easier :)
            //Could move Transition out here???
            if(TransitionButton == 0 && TransitioningMenu == nullptr) {
                TransitionButton = B1IsJustReleased << 2 | B2IsJustReleased << 1 | B3IsJustReleased;
                if(TransitionButton != 0) {
                    TransitionOUTFlag = true;
                    //Serial.println(TransitionButton);
                    if(TransitionButton == 0b100)       TransitioningMenu = TransitionMenu1;
                    else if (TransitionButton == 0b010) {
                        TransitioningMenu = TransitionMenu2;
                        mainImage->blitBackward(TransitionOut2, iVec2(0,0));
                    }
                    else if (TransitionButton == 0b001) TransitioningMenu = TransitionMenu3;
                
                }
            }
        }

        void TransitionOUT(){
            //Serial.println("Flag: " + TransitionOUTFlag && !TransitionOUTDone);
            if(TransitionOUTFlag && !TransitionOUTDone){
                if(TransitionButton >> 2 ){
                        mainImage->blitScaledRotated(TransitionOut, fVec2(TransitionOut.width()/2, TransitionOut.height()/2), fVec2(Item1.pos.x, Item1.pos.y), TransitionScale, 45);
                        TransitionScale *= TRANSITION_RATE;
                } else if (TransitionButton >> 1) {
                    //TODO: MAKE THIS A COOL TRANSITION :)
                        TransitionScale += 0.005;
                        if(TransitionScale > TRANSITION_MAX2) TransitionScale = TRANSITION_MAX2;
                        TransitionRotation += 0.5;
                        mainImage->blitScaledRotated(TransitionOut2, fVec2(Item2.pos.x, Item2.pos.y), fVec2(Item2.pos.x, Item2.pos.y), TransitionScale, TransitionRotation);
                } else if (TransitionButton) {
                        mainImage->blitScaledRotated(TransitionOut, fVec2(TransitionOut.width()/2, TransitionOut.height()/2), fVec2(Item3.pos.x, Item3.pos.y), TransitionScale, 45);
                        TransitionScale *= TRANSITION_RATE;
                }
                //Serial.println("Scale: " + TransitionScale);
                if((TransitionButton & 0b001 || TransitionButton >> 2) && TransitionScale > TRANSITION_MAX) {
                    mainImage->fillScreen(TransitionColor());
                    TransitionOUTFlag = false;
                    TransitionOUTDone = true;
                }
                else if(TransitionScale == TRANSITION_MAX2 && TransitionRotation == TRANSITION_ROT_MAX){
                    TransitionOUTFlag = false;
                }
            }
        }
        bool IsTransitioningOUT(){
            return TransitionOUTFlag;
        }
        bool IsTransitionOUTDone(){
            return TransitionOUTDone;
        }
        RGB565 TransitionColor(){
            return col1;
        }

        Menu* MenuToTransitionTo(){
            return TransitioningMenu;
        }

        void Destruct(){
            // free(mainImage);

            // free(item);
            // free(text_box);
            // free(transition_in);
            // free(transition_out);

            // free(SampleMenuItem);
            // free(TextBox);
            // free(TransitionIn);
            // free(TransitionOut);

            TransitionINDone = false;
            TransitionOUTFlag = false;
            TransitionOUTDone = false;
            TransitionButton = 0;
            TransitioningMenu = nullptr;
        }
};
