#pragma once

#include <ILI9341_T4.h> 
#include <tgx.h>

using namespace tgx;

class Menu {
    public:
        //Everything Essential.
        //All menus must have a setup, TransitionIN and OUT and the OUT Color, Draw, 
        //  And a check if the transition into and out of the menu is done.

        //You really shouldn't use a menu pointer and this. This is simply here as a reference.
        virtual void Link(Menu* Tm1, Menu* Tm2, Menu* Tm3);

        //Instantiates everything inside the menu. Pre-draws the images, and stores the TransitionINColor.
        virtual void Setup();
        virtual void Setup(RGB565 TransitionINColor);

        //After the previous menu is destroyed, and the current menu is setup, transition in is called every
        //frame until it is done. Will implicitly stop drawing if it is done Transitioning in.
        virtual void TransitionIN();
        //The flag for if the TransitionIN is done.
        virtual bool IsTransitionINDone();

        //Draws the static of the menu. Controls the inputs of the menu as well.
        //Pressed parameters should be whether the button is down or up.
        //Is Just Released parameters should be whether the button is JUST released.
        //Dial 1 passes through the analog parameters of the 3 dials.
        virtual void Draw(
            bool B1IsPressed, bool B2IsPressed, bool B3IsPressed, 
            bool B1IsJustReleased, bool B2IsJustReleased, bool B3IsJustReleased,
            int Dial1, int Dial2, int Dial3
        );

        //Once the "Back button" is Pressed then released, TransitionOUT will be called every frame until it's done.
        //Will implicitly stop drawing if it is done Transitioning out.
        virtual void TransitionOUT();
        //The flag for if the TransitionOUT is ready to happen.
        virtual bool IsTransitioningOUT();
        //The flag for if the TransitionOUT is done.
        virtual bool IsTransitionOUTDone();
        //The Transition OUT color of the current menu INTO the next menu.
        //Should be stored temporarily, and then discarded once setup is called with this.
        virtual RGB565 TransitionColor();

        //The Menu address pointing to what menu to transition to.
        virtual Menu* MenuToTransitionTo();

        //This simply should free all the variables that were inside setup() and reset everything (menuwise)
        virtual void Destruct();
};