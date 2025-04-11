/******************************************************************
 *
 * workarounds.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 25-04-03, for qgachawishes.
 *
 ******************************************************************/

#include "workaround.h"
#include "jadeite.h"
#include "none.h"

std::unique_ptr<AWorkaround> Workaround::getWorkaround(Handler handler, std::string executable) {
    switch (handler) {
        case Jadeite: {
            return std::make_unique<JadeiteImpl>(executable);
        }
        default: return std::make_unique<NoneImpl>(executable);
    }
}
