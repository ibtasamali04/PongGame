#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include "raylib.h"

const float SCREEN_WIDTH = 1920;
const float SCREEN_HEIGHT = 1080;
const float smallButtonWidth = 130.0f;
const float largeButtonWidth = 224.0f;
const float smallButtonFrameHeight = 185.0f / 3;
const float largeButtonFrameHeight = 318.0f / 3;
const float listButtonSpace = 50;
const float buttonWidth = 220;
const float buttonHeight = 60;
const int LOADING_DELAY_FRAMES = 15;

Rectangle listButtonRec1 = {SCREEN_WIDTH / 6 - buttonWidth / 2, SCREEN_HEIGHT / 2 + 1 * listButtonSpace, largeButtonWidth, largeButtonFrameHeight};
Rectangle listButtonRec2 = {SCREEN_WIDTH / 6 - buttonWidth / 2, SCREEN_HEIGHT / 2 + 3 * listButtonSpace, largeButtonWidth, largeButtonFrameHeight};
Rectangle listButtonRec3 = {SCREEN_WIDTH / 6 - buttonWidth / 2, SCREEN_HEIGHT / 2 + 5 * listButtonSpace, largeButtonWidth, largeButtonFrameHeight};
Rectangle backButtonRec = {5, 30, smallButtonWidth, smallButtonFrameHeight};
Rectangle guideButtonRec = {SCREEN_WIDTH - 245, 30, smallButtonWidth, smallButtonFrameHeight};
Rectangle musicButtonRec = {SCREEN_WIDTH - 120, 30, smallButtonWidth, smallButtonFrameHeight};

Rectangle ballButtonRec = {SCREEN_WIDTH - 850, 605, 200, 30};
Rectangle playerButtonRec = {SCREEN_WIDTH - 850, 685, 200, 30};
Rectangle scoreButtonRec = {SCREEN_WIDTH - 850, 765, 200, 30};
Rectangle resetButtonRec = {SCREEN_WIDTH - 810, 835, smallButtonWidth, smallButtonFrameHeight};

#endif