/******************************************************************
 *
 * HoyoMetadata.h
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 25-04-23, for qgachawishes.
 *
 ******************************************************************/


#pragma once

#include <memory>

#include <httpclient/httpclient.h>
#include "HoyoConstants.h"

class HoyoMetadata {
public:
    static std::shared_ptr<HoyoMetadata>                                 get_instance();
    std::string                                                          getLocaleBackgroundUri() const;
    HoyoConstants::HoyoServerKey                                         getServerKey2(const QJsonObject& json_values);
    std::shared_ptr<std::map<HoyoConstants::HoyoServerKey, std::string>> getBackgrounds();
    void                                                                 bootstrap();
    std::string getBackground(HoyoConstants::HoyoServerKey key);

private:
    HoyoMetadata();
    static std::shared_ptr<HoyoMetadata>                                 m_instance;
    std::shared_ptr<HttpClient>                                          mHttpClient;
    std::shared_ptr<std::map<HoyoConstants::HoyoServerKey, std::string>> mBackgrounds;
};
