/**
 * Created by cybik on 22-02-13.
 * The contents of this file are https://gitlab.com/KRypt0n_/an-anime-game-launcher/-/blob/main/src/ts/Constants.ts
 *  made into a C++/QT class.
 **/

#ifndef QAGL_CONSTANTS_H
#define QAGL_CONSTANTS_H

#include <string>
#include <QByteArray>

#define atob(X) QByteArray::fromBase64(X, QByteArray::Base64Encoding).toStdString()

namespace QAGL {
    namespace placeholders {
        namespace uppercase
        {
            /* Anime */                 const std::string first = atob("R2Vuc2hpbg==");
            /* Game */                  const std::string second = atob("SW1wYWN0");
            /* Anime Game */            const std::string full = atob("R2Vuc2hpbiBJbXBhY3Q=");
            /* anAnimeCompany */        const std::string company = atob("bWlIb1lv");
            /* NOTAREALANIMECOMPANY */  const std::string company_alterego = atob("Q09HTk9TUEhFUkU=");
        }
    
        namespace lowercase
        {
            /* anime */                 const std::string first = atob("Z2Vuc2hpbg==");
            /* animecompany */          const std::string company = atob("bWlob3lv");
        }
    }

    namespace uri {
        const std::string api = "https://sdk-os-static."+ placeholders::lowercase::company +".com/hk4e_global/mdk/launcher/api";
        namespace patch {
            const std::string origin = "https://notabug.org/Krock/dawn";
            const std::string additional= "https://dev.kaifa.ch/Maroxy/dawn";
        }
        const std::string launcher_upstream = "https://gitlab.com/KRypt0n_/an-anime-game-launcher";
        const std::string launcher_qagl = "https://gitlab.com/cybik/qt-anime-game-launcher";
        const std::string discord_upstream_qagl = "https://discord.gg/ck37X6UWBp";

        const std::string telemetry[2] = {
            atob("bG9nLXVwbG9hZC1vcy5taWhveW8uY29t"), atob("b3ZlcnNlYXVzcGlkZXIueXVhbnNoZW4uY29t"),
        };
        const std::string winetricks = "https://raw.githubusercontent.com/Winetricks/winetricks/master/src/winetricks";
    }
    const std::string versionUri = (uri::api+"/resource?key=gcStgarh&launcher_id=10&language=");
    const std::string backgroundUri = (uri::api+"/content?filter_adv=true&launcher_id=10&key=gcStgarh&language=");
}

#endif //QAGL_CONSTANTS_H
