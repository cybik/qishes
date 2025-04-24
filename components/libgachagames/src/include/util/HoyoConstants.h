/**
 * Created by cybik on 22-02-13.
 * The contents of this file are https://gitlab.com/KRypt0n_/an-anime-game-launcher/-/blob/main/src/ts/Constants.ts
 *  made into a C++/QT class.
 **/

#pragma once

#include <cppcodec/base64_rfc4648.hpp>
#include <string>

#define b64dec(X) cppcodec::base64_rfc4648::decode<std::string>(X)

// Constants for Hoyoverse properties at large.
namespace HoyoConstants {
    namespace placeholders {
        namespace uppercase
        {
            /* Anime */                 const std::string first = b64dec("R2Vuc2hpbg==");
            /* Game */                  const std::string second = b64dec("SW1wYWN0");
            /* Anime Game */            const std::string full = b64dec("R2Vuc2hpbiBJbXBhY3Q=");
            /* anAnimeCompany */        const std::string company = b64dec("bWlIb1lv");
            /* NOTAREALANIMECOMPANY */  const std::string company_alterego = b64dec("Q09HTk9TUEhFUkU=");
        }
    
        namespace lowercase
        {
            /* anime */                 const std::string first = b64dec("Z2Vuc2hpbg==");
            /* animecompany */          const std::string company = b64dec("bWlob3lv");
            /* animecompany */          const std::string company_os = b64dec("aG95b3ZlcnNl");
        }

    }
// https://hyp-api.mihoyo.com/hyp/hyp-connect/api/getAllGameBasicInfo?launcher_id=jGHBHlcOq1
    namespace uri {
        const std::string api = "https://sdk-os-static."+ placeholders::lowercase::company +".com/hk4e_global/mdk/launcher/api";
        const std::string api_os = "https://sdk-os-static."+ placeholders::lowercase::company_os +".com/hk4e_global/mdk/launcher/api";
        const std::string api_hyp = "https://sg-hyp-api."+ placeholders::lowercase::company_os +".com/hyp/hyp-connect/api/getAllGameBasicInfo";
        const std::string api_hyp_cn = "https://hyp-api."+ placeholders::lowercase::company +".com/hyp/hyp-connect/api/getAllGameBasicInfo";

        const std::string telemetry[2] = {
            b64dec("bG9nLXVwbG9hZC1vcy5taWhveW8uY29t"),
            b64dec("b3ZlcnNlYXVzcGlkZXIueXVhbnNoZW4uY29t"),
        };
        const std::string winetricks = "https://raw.githubusercontent.com/Winetricks/winetricks/master/src/winetricks";
    }
    const std::string versionUri = (uri::api_os+"/resource?key=gcStgarh&launcher_id=10&language=");
    const std::string backgroundUri = (uri::api_os+"/content?filter_adv=true&launcher_id=10&key=gcStgarh&language=");
    const std::string backgroundUri_hyp = (uri::api_hyp+"?launcher_id=VYTpXlbWo8&language=");
    const std::string backgroundUri_hyp_cn = (uri::api_hyp+"?launcher_id=jGHBHlcOq1");
}
