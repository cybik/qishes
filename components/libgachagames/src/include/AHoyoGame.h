/******************************************************************
 *
 * AHoyoGame.h
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 25-04-19, for qgachawishes.
 *
 ******************************************************************/

#pragma once

#include "AGame.h"

class AHoyoGame : public AGame {
public:
    void prefetchBackground() override;
};
