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
#include <filesystem>
#include <map>

#include <list>
#include <proton.h>
#include <steamrt.h>

#include <vector>

class vlvproton {
public:
    static std::shared_ptr<vlvproton> getInstance(std::list<std::filesystem::path> base_dirs);
    static std::shared_ptr<vlvproton> getInstance();
    std::vector<std::string> get_available_protons();
    std::vector<std::string> get_available_steam_runtimes();
    void select(const std::string& key);
    void identify_proton_installs();
    void identify_steamrt_installs();

    std::shared_ptr<proton> get_selected_proton();
    std::shared_ptr<steamrt> get_selected_steamrt();
protected:
private:
    void identify_proton(const std::filesystem::directory_iterator& path);
    void identify_steamrt(const std::filesystem::directory_iterator& path);
    vlvproton(std::list<std::filesystem::path> base_dirs);
    bool                              is_dir_a_proton(std::filesystem::directory_entry);
    bool                              is_dir_a_steamrt(std::filesystem::directory_entry);
    static std::shared_ptr<vlvproton> mInstance;
    std::filesystem::path             m_baseDir;
    std::map<std::string, std::shared_ptr<proton>> mProtons;
    std::map<std::string, std::shared_ptr<steamrt>> mSteamRTs;
    std::string mSelectedProton;
    std::string mSelectedSteamRT;
};
