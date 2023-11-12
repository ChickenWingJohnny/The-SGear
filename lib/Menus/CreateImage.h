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
        uint16_t dial[37 * 37];
        uint16_t transition_in[2*2];
        uint16_t transition_out[2*2];
        uint16_t back_arrow[32*32];
        uint16_t wave1[240 * 60];
        uint16_t wave2[240 * 60];
    }
        Image<RGB565> Item = Image<RGB565>(item, 16, 16);
        Image<RGB565> AltItem = Image<RGB565>(alt_item, 16, 16);
        Image<RGB565> Item2 = Image<RGB565>(item2, 24, 24);
        Image<RGB565> AltItem2 = Image<RGB565>(alt_item2, 24, 24);
        Image<RGB565> Dial = Image<RGB565>(dial, 37, 37);
        Image<RGB565> TransitionIn = Image<RGB565>(transition_in, 2, 2);
        Image<RGB565> TransitionOut = Image<RGB565>(transition_out, 2, 2);
        Image<RGB565> BackArrow = Image<RGB565>(back_arrow, 32, 32);
        Image<RGB565> WaveImage1 = Image<RGB565>(wave1, 240, 60);
        Image<RGB565> WaveImage2 = Image<RGB565>(wave2, 240, 60);

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

        void updateDialMenuItem(RGB565 outerColor, RGB565 innerColor){
            Dial.fillCircle(Dial.lx()/2, Dial.ly()/2, 18, outerColor, outerColor);
            Dial.fillCircle(Dial.lx()/2, Dial.ly()/2, 16, innerColor, innerColor);
            Dial.drawFastVLine(iVec2((Dial.lx()/2)-1, 0), 10, RGB565_White);
            Dial.drawFastVLine(iVec2(Dial.lx()/2, 0), 10, RGB565_White);
            Dial.drawFastVLine(iVec2((Dial.lx()/2)+1, 0), 10, RGB565_White);
        }

        //Fills the Screen.
        void updateTransitionIn(RGB565 transitionInColor){
            TransitionIn.fillScreen(transitionInColor);
        }
        void updateTransitionOut(RGB565 transitionOutColor){
            TransitionOut.fillScreen(transitionOutColor);
        }
        
        void updateBackArrow(RGB565 color){
            BackArrow.fillTriangle(iVec2(4, 10), iVec2(12, 2), iVec2(12, 18), color, color);
            BackArrow.fillRect(iVec2(13, 8), iVec2(9, 5), color);
            BackArrow.fillTriangle(iVec2(22, 8), iVec2(22, 13), iVec2(27, 13), color, color);

            BackArrow.fillRect(iVec2(23, 14), iVec2(5, 7), color);
            BackArrow.fillTriangle(iVec2(27, 21), iVec2(22, 21), iVec2(22, 26), color, color);
            
            BackArrow.fillRect(iVec2(10, 22), iVec2(12, 5), color);
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


        static void updateSineWaveImage1(RGB565 BackgroundColor, RGB565 LineColor){
            WaveImage1.fillScreen(BackgroundColor);
            for(int s = 2; s > -2; s--){
                for(int x = 0; x < WaveImage1.lx(); x++){
                    int y = (int)(28 * sin(((double)1/60) * PI * (double) x));
                    WaveImage1.drawPixel(x, 28+y+s, LineColor);
                }
            }
        }
        static void updateSquareWaveImage1(RGB565 BackgroundColor, RGB565 LineColor){
            WaveImage1.fillScreen(BackgroundColor);
            int period = 30;
            for(int i = 0; i < 8; i++){
                WaveImage1.drawFastVLine(iVec2((period*i)-1, 0), 60, LineColor);
                WaveImage1.drawFastVLine(iVec2((period*i), 0), 60, LineColor);
                WaveImage1.drawFastVLine(iVec2((period*i)+1, 0), 60, LineColor);
                if(i%2){
                    WaveImage1.drawFastHLine(iVec2((period*i), 0), period, LineColor);
                    WaveImage1.drawFastHLine(iVec2((period*i), 1), period, LineColor);
                    WaveImage1.drawFastHLine(iVec2((period*i), 2), period, LineColor);
                }
                else{
                    WaveImage1.drawFastHLine(iVec2((period*i), 58), period, LineColor);
                    WaveImage1.drawFastHLine(iVec2((period*i), 59), period, LineColor);
                    WaveImage1.drawFastHLine(iVec2((period*i), 60), period, LineColor);
                }
            }
            WaveImage1.drawFastVLine(iVec2(240-1, 0), 60, LineColor);
            WaveImage1.drawFastVLine(iVec2(240, 0), 60, LineColor);
            WaveImage1.drawFastVLine(iVec2(240+1, 0), 60, LineColor);
        }
        static void updateTriangleWaveImage1(RGB565 BackgroundColor, RGB565 LineColor){
            WaveImage1.fillScreen(BackgroundColor);
            int period = 30;
            for(int i = 0; i < 8; i+=2){
                WaveImage1.drawLine(iVec2((period*i)-1, 60), iVec2(period*(i+1)-1, 0), LineColor);
                WaveImage1.drawLine(iVec2((period*i), 60), iVec2(period*(i+1), 0), LineColor);
                WaveImage1.drawLine(iVec2((period*i)+1, 60), iVec2(period*(i+1)+1, 0), LineColor);

                WaveImage1.drawLine(iVec2((period*(i+1))-1, 0), iVec2(period*(i+2)-1, 60), LineColor);
                WaveImage1.drawLine(iVec2((period*(i+1)), 0), iVec2(period*(i+2), 60), LineColor);
                WaveImage1.drawLine(iVec2((period*(i+1))+1, 0), iVec2(period*(i+2)+1, 60), LineColor);
            }
        }
        static void updateSawtoothWaveImage1(RGB565 BackgroundColor, RGB565 LineColor){
            //Sawtooth Reverse can be made by flipping this image.
            WaveImage1.fillScreen(BackgroundColor);
            int period = 60;
            for(int i = 0; i < 4; i++){
                WaveImage1.drawLine(iVec2((period*i)-1, 60), iVec2(period*(i+1)-1, 0), LineColor);
                WaveImage1.drawLine(iVec2((period*i), 60), iVec2(period*(i+1), 0), LineColor);
                WaveImage1.drawLine(iVec2((period*i)+1, 60), iVec2(period*(i+1)+1, 0), LineColor);

                WaveImage1.drawFastVLine(iVec2((period*(i+1)-1), 0), 60, LineColor);
                WaveImage1.drawFastVLine(iVec2((period*(i+1)), 0), 60, LineColor);
                WaveImage1.drawFastVLine(iVec2((period*(i+1)+1), 0), 60, LineColor);
            }
        }
        static void updateArbitraryWaveImage1(){
            
        }

        static void updateSineWaveImage2(RGB565 BackgroundColor, RGB565 LineColor){
            WaveImage2.fillScreen(BackgroundColor);
            for(int s = 2; s > -2; s--){
                for(int x = 0; x < WaveImage2.lx(); x++){
                    int y = (int)(28 * sin(((double)1/60) * PI * (double) x));
                    WaveImage2.drawPixel(x, 28+y+s, LineColor);
                }
            }
        }
        static void updateSquareWaveImage2(RGB565 BackgroundColor, RGB565 LineColor){
            WaveImage2.fillScreen(BackgroundColor);
            int period = 30;
            for(int i = 0; i < 8; i++){
                WaveImage2.drawFastVLine(iVec2((period*i)-1, 0), 60, LineColor);
                WaveImage2.drawFastVLine(iVec2((period*i), 0), 60, LineColor);
                WaveImage2.drawFastVLine(iVec2((period*i)+1, 0), 60, LineColor);
                if(i%2){
                    WaveImage2.drawFastHLine(iVec2((period*i), 0), period, LineColor);
                    WaveImage2.drawFastHLine(iVec2((period*i), 1), period, LineColor);
                    WaveImage2.drawFastHLine(iVec2((period*i), 2), period, LineColor);
                }
                else{
                    WaveImage2.drawFastHLine(iVec2((period*i), 58), period, LineColor);
                    WaveImage2.drawFastHLine(iVec2((period*i), 59), period, LineColor);
                    WaveImage2.drawFastHLine(iVec2((period*i), 60), period, LineColor);
                }
            }
            WaveImage2.drawFastVLine(iVec2(240-1, 0), 60, LineColor);
            WaveImage2.drawFastVLine(iVec2(240, 0), 60, LineColor);
            WaveImage2.drawFastVLine(iVec2(240+1, 0), 60, LineColor);
        }
        static void updateTriangleWaveImage2(RGB565 BackgroundColor, RGB565 LineColor){
            WaveImage2.fillScreen(BackgroundColor);
            int period = 30;
            for(int i = 0; i < 8; i+=2){
                WaveImage2.drawLine(iVec2((period*i)-1, 60), iVec2(period*(i+1)-1, 0), LineColor);
                WaveImage2.drawLine(iVec2((period*i), 60), iVec2(period*(i+1), 0), LineColor);
                WaveImage2.drawLine(iVec2((period*i)+1, 60), iVec2(period*(i+1)+1, 0), LineColor);

                WaveImage2.drawLine(iVec2((period*(i+1))-1, 0), iVec2(period*(i+2)-1, 60), LineColor);
                WaveImage2.drawLine(iVec2((period*(i+1)), 0), iVec2(period*(i+2), 60), LineColor);
                WaveImage2.drawLine(iVec2((period*(i+1))+1, 0), iVec2(period*(i+2)+1, 60), LineColor);
            }
        }
        static void updateSawtoothWaveImage2(RGB565 BackgroundColor, RGB565 LineColor){
            //Sawtooth Reverse can be made by flipping this image.
            WaveImage2.fillScreen(BackgroundColor);
            int period = 60;
            for(int i = 0; i < 4; i++){
                WaveImage2.drawLine(iVec2((period*i)-1, 60), iVec2(period*(i+1)-1, 0), LineColor);
                WaveImage2.drawLine(iVec2((period*i), 60), iVec2(period*(i+1), 0), LineColor);
                WaveImage2.drawLine(iVec2((period*i)+1, 60), iVec2(period*(i+1)+1, 0), LineColor);

                WaveImage2.drawFastVLine(iVec2((period*(i+1)-1), 0), 60, LineColor);
                WaveImage2.drawFastVLine(iVec2((period*(i+1)), 0), 60, LineColor);
                WaveImage2.drawFastVLine(iVec2((period*(i+1)+1), 0), 60, LineColor);
            }
        }
        static void updateArbitraryWaveImage(){
            
        }

}