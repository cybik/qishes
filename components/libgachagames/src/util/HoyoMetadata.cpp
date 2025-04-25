/******************************************************************
 *
 * HoyoMetadata.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 25-04-23, for qgachawishes.
 *
 ******************************************************************/

#include "util/HoyoMetadata.h"

#include <iostream>

#include "QJsonDocument"
#include "QJsonArray"
#include "QJsonObject"


std::shared_ptr<HoyoMetadata> HoyoMetadata::m_instance = nullptr;

std::shared_ptr<HoyoMetadata> HoyoMetadata::get_instance() {
    if (m_instance == nullptr) {
        m_instance = std::shared_ptr<HoyoMetadata>(new HoyoMetadata());
    }
    return m_instance;
}

std::string HoyoMetadata::getLocaleBackgroundUri() const {
    // if behind the Chinese mainland firewall, return mihoyo-based url
    return (HoyoConstants::backgroundUri_hyp + "en-us");
}

HoyoConstants::HoyoServerKey HoyoMetadata::getServerKey2(const QJsonObject& json_values) {
    /**
     *   loc_gen_glb, // 5TIVvvcwtM
     *   loc_jp_glb, // g0mMIvshDb
     *   loc_kr_glb, // uxB4MC7nzC
     *   loc_en_sea_glb, // bxPTXSET5t
     *   loc_cn_glb // wkE5P5WsIf
     **/
    if (json_values["biz"] == "nap_global") return HoyoConstants::loc_nap;
    else if (json_values["biz"] == "hkrpg_global") return HoyoConstants::loc_bhsr;
    else if (json_values["biz"] == "hk4e_global") return HoyoConstants::loc_hk4e;
    else if (json_values["id"] == "5TIVvvcwtM") return HoyoConstants::loc_3rd_gen_glb;
    else if (json_values["id"] == "g0mMIvshDb") return HoyoConstants::loc_3rd_jp_glb;
    else if (json_values["id"] == "uxB4MC7nzC") return HoyoConstants::loc_3rd_kr_glb;
    else if (json_values["id"] == "bxPTXSET5t") return HoyoConstants::loc_3rd_en_sea_glb;
    else if (json_values["id"] == "wkE5P5WsIf") return HoyoConstants::loc_3rd_cn_glb;
    return HoyoConstants::loc_void;
}

std::shared_ptr<std::map<HoyoConstants::HoyoServerKey, std::string>> HoyoMetadata::getBackgrounds() {
    auto bg = mHttpClient->get_sync(getLocaleBackgroundUri().c_str());
    auto background = std::make_shared<QJsonDocument>(QJsonDocument::fromJson(bg));
    auto mo = std::make_shared<std::map<HoyoConstants::HoyoServerKey, std::string>>();
    if (    !background->isNull()
        && (*background)["data"].isObject()
        && (*background)["data"]["game_info_list"].isArray()
    ) {
        for (auto elem: (*background)["data"]["game_info_list"].toArray()) {
            if (!elem.isUndefined()
                && elem.toObject()["game"].isObject()
                && elem.toObject()["backgrounds"].isArray()
            ) {
                if (elem.toObject()["game"].toObject()["biz"].isString()) {
                    getServerKey2(elem.toObject()["game"].toObject());
                    mo->insert(std::make_pair(
                        getServerKey2(elem.toObject()["game"].toObject()),
                        elem.toObject()["backgrounds"]
                            .toArray()
                            .at(0)
                            .toObject()["background"]
                            .toObject()["url"]
                            .toString()
                        .toStdString()
                    ));
                }
            }
        }
    }
    return mo;
}


std::string HoyoMetadata::getBackground(HoyoConstants::HoyoServerKey key) {
    if (mBackgrounds->contains(key)) return mBackgrounds->at(key);
    return "";
}


void HoyoMetadata::bootstrap() {
    mBackgrounds = getBackgrounds();
    for (auto elem: *mBackgrounds) {
        std::cout << elem.first << " :: " << elem.second << std::endl;
    }
}

HoyoMetadata::HoyoMetadata() {
    mHttpClient = std::make_shared<HttpClient>();
    mHttpClient->setGlobalTimeout(std::chrono::milliseconds(5000));
}

