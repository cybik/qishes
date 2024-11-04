/******************************************************************
 *
 * vlvproton.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 24-11-04 for qgachawishes.
 *
 ******************************************************************/

#include "vlvproton.h"

std::shared_ptr<vlvproton> vlvproton::mInstance = nullptr;

std::shared_ptr<vlvproton> vlvproton::getInstance() {
    if (!mInstance) mInstance = std::shared_ptr<vlvproton>(new vlvproton());
    return mInstance;
}

