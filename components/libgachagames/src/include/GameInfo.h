/******************************************************************
 *
 * gameinfo.h
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 25-04-10, for qgachawishes.
 *
 ******************************************************************/


#pragma once

#include <string>
#include <workaround.h>

class GameInfo {
public:
    typedef enum {
        // Unpatched
        Genshin,
        Nap,
        Launcher,

        // Jadeite
        HonkaiSR,
        Honkai3rd,

        // UnrealEngine - more guesswork needed
        UnrealEngineMeta,
        WutheringWaves,

        /** Unsupported for now */
        InfinityNikki,
        Strinova,

        Unknown         /** NYANEEEEEEEEEEEEEEH */
    } ExeType;
    GameInfo(std::string exe, std::string label, ExeType exeType, Workaround::Handler workaround)
        : target_exe(exe), target_label(label), target_exeType(exeType), selected_workaround(workaround) {}
    ExeType get_exetype();
    std::string get_label();
    Workaround::Handler get_workaround();
private:
    std::string target_exe;
    std::string target_label;
    ExeType target_exeType;
    Workaround::Handler selected_workaround;
};
