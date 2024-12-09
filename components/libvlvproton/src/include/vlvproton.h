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

#include <vector>

class vlvproton {
public:
    static std::shared_ptr<vlvproton> getInstance(const std::filesystem::path& base_dir);
    static std::shared_ptr<vlvproton> getInstance();
    std::vector<std::string> get_available_protons();
    void select(const std::string& key);
    void identify_installs();

    std::shared_ptr<proton> get_selected_proton();
protected:
private:
    void identify(const std::filesystem::directory_iterator& path);
    vlvproton(const std::filesystem::path& base_dir);
    bool                              is_dir_a_proton(std::filesystem::directory_entry);
    static std::shared_ptr<vlvproton> mInstance;
    std::filesystem::path             m_baseDir;
    std::map<std::string, std::shared_ptr<proton>> mProtons;
    std::string mSelectedProton;
};
