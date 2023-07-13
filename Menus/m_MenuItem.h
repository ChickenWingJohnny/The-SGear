#pragma once

#include <ILI9341_T4.h> 
#include <tgx.h>

using namespace tgx;

struct MenuItem {
  //Image Reference.
  Image<RGB565> image;
  //Image Size, X and Y
  iVec2 size;
  //Position, X and Y
  iVec2 pos;
  //Rotation
  double rot;
  //Scale
  double scale;
  //Top Text
  String topText;
  //Bottom Text
  String bottomText;
};
