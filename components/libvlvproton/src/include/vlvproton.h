/******************************************************************
 *
 * vlvproton.h
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 24-11-04 for qgachawishes.
 *
 ******************************************************************/


#pragma once
#include <memory>


class vlvproton {
public:
    static std::shared_ptr<vlvproton> getInstance();
protected:
private:
    static std::shared_ptr<vlvproton> mInstance;
};
