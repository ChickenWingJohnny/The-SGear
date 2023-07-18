#pragma once

#include <ILI9341_T4.h>
#include <tgx.h>

using namespace tgx;

//Stores common shapes that can be used across menus.
namespace CreateImage {

    namespace {
        uint16_t item[16 * 16];
        uint16_t alt_item[16 * 16];
        uint16_t item2[24 * 24];
        uint16_t alt_item2[24 * 24];
        uint16_t transition_in[2*2];
        uint16_t transition_out[2*2];
    }
        Image<RGB565> Item = Image<RGB565>(item, 16, 16);
        Image<RGB565> AltItem = Image<RGB565>(alt_item, 16, 16);
        Image<RGB565> Item2 = Image<RGB565>(item2, 24, 24);
        Image<RGB565> AltItem2 = Image<RGB565>(alt_item2, 24, 24);
        Image<RGB565> TransitionIn = Image<RGB565>(transition_in, 2, 2);
        Image<RGB565> TransitionOut = Image<RGB565>(transition_out, 2, 2);

        //Simple Box with outline.
        void updateRectMenuItem(RGB565 outerColor, RGB565 innerColor){
            Item.fillScreen(outerColor);
            Item.fillRect(1, 1, 14, 14, innerColor);
        }

        void updateAltRectMenuItem(RGB565 outerColor, RGB565 innerColor){
            AltItem.fillScreen(outerColor);
            AltItem.fillRect(1, 1, 14, 14, innerColor);
        }

        //DoubleSquare forming a diamond like shape
        void updateDiamondMenuItem(RGB565 outerColor, RGB565 innerColor){
            Item2.fillRect(0, 0, 16, 16, outerColor);
            Item2.fillRect(8, 8, 16, 16, outerColor);
            Item2.fillRect(1, 1, 14, 14, innerColor);
            Item2.fillRect(9, 9, 14, 14, innerColor);
        }
        void updateAltDiamondMenuItem(RGB565 outerColor, RGB565 innerColor){
            AltItem2.fillRect(0, 0, 16, 16, outerColor);
            AltItem2.fillRect(8, 8, 16, 16, outerColor);
            AltItem2.fillRect(1, 1, 14, 14, innerColor);
            AltItem2.fillRect(9, 9, 14, 14, innerColor);
        }

        //Fills the Screen.
        void updateTransitionIn(RGB565 transitionInColor){
            TransitionIn.fillScreen(transitionInColor);
        }
        void updateTransitionOut(RGB565 transitionOutColor){
            TransitionOut.fillScreen(transitionOutColor);
        }
        

        // //Simple Box with outline.
        // static void createRectMenuItem(Image<RGB565>* im, RGB565 outerColor, RGB565 innerColor, int w, int h, int strokeWidth){
        //     im->fillScreen(outerColor);
        //     im->fillRect(strokeWidth, strokeWidth, w-(strokeWidth*2), h-(strokeWidth*2), innerColor);
        // }

        // //DoubleSquare forming a diamond like shape
        // static void createDiamondMenuItem(Image<RGB565>* im, RGB565 outerColor, RGB565 innerColor, int w, int h, int strokeWidth){
        //     im->fillRect(0, 0, w*2/3, h*2/3, outerColor);
        //     im->fillRect(w/3, h/3, w*2/3, h*2/3, outerColor);
        //     im->fillRect(strokeWidth, strokeWidth, (w*2/3)-(strokeWidth*2), (h*2/3)-(strokeWidth*2), innerColor);
        //     im->fillRect(w/3 + strokeWidth, h/3 + strokeWidth, (w*2/3)-(strokeWidth*2), (h*2/3)-(strokeWidth*2), innerColor);
        // }

        // //Fills the Screen.
        // static void createTransition(Image<RGB565>* im, RGB565 transitionInColor){
        //     im->fillScreen(transitionInColor);
        // }
        
        static void placeText(Image<RGB565>* im, String text, iVec2 middlePos, RGB565 col, const ILI9341_t3_font_t& font, float opacity){
            char chars[text.length()];
            text.toCharArray(chars, text.length() + 1);
            iBox2 textBoundingBox = im->measureText(chars, middlePos, font);
            iVec2 textAncPos(textBoundingBox.lx()/2.0, textBoundingBox.ly()/2.0);           
            iVec2 pos(middlePos.x - textAncPos.x, middlePos.y + textAncPos.y);
            im->drawText(chars, pos, col, font, false, opacity);
        }

        //Creates a Text box that perfectly enraps the text.
        static void createTextBox(Image<RGB565>* im, String text, int strokeWidth, RGB565 innerColor, RGB565 outerColor, RGB565 textColor, const ILI9341_t3_font_t& font){
            im->fillScreen(outerColor);
            im->fillRect(strokeWidth, strokeWidth, im->lx() - (2*strokeWidth), im->ly() - (2*strokeWidth), innerColor);
            placeText(im, text, iVec2(im->lx()/2, im->ly()/2), textColor, font, 1.0);
        }

}