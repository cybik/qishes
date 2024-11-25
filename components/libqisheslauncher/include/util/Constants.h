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
            /* animecompany */          const std::string company_os = atob("aG95b3ZlcnNl");
        }

    }
// https://hyp-api.mihoyo.com/hyp/hyp-connect/api/getAllGameBasicInfo?launcher_id=jGHBHlcOq1
    namespace uri {
        const std::string api = "https://sdk-os-static."+ placeholders::lowercase::company +".com/hk4e_global/mdk/launcher/api";
        const std::string api_os = "https://sdk-os-static."+ placeholders::lowercase::company_os +".com/hk4e_global/mdk/launcher/api";
        const std::string api_hyp = "https://sg-hyp-api."+ placeholders::lowercase::company_os +".com/hyp/hyp-connect/api/getAllGameBasicInfo";
        const std::string api_hyp_cn = "https://hyp-api."+ placeholders::lowercase::company +".com/hyp/hyp-connect/api/getAllGameBasicInfo";
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
    const std::string versionUri = (uri::api_os+"/resource?key=gcStgarh&launcher_id=10&language=");
    const std::string backgroundUri = (uri::api_os+"/content?filter_adv=true&launcher_id=10&key=gcStgarh&language=");
    const std::string backgroundUri_hyp = (uri::api_hyp+"?launcher_id=VYTpXlbWo8&language=");
    const std::string backgroundUri_hyp_cn = (uri::api_hyp+"?launcher_id=jGHBHlcOq1");
}

#endif //QAGL_CONSTANTS_H
