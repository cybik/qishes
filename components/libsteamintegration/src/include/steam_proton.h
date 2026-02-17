/******************************************************************
 *
 * steam_proton.h
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 24-11-29, for qgachawishes.
 *
 ******************************************************************/

#pragma once

#include <memory>

#include <vlvproton.h>

#include <QProcess>

#include <workaround.h>

class steam_proton {
public:
    static std::shared_ptr<steam_proton> getInstance();

    void try_setup();
    void try_run(   const std::string& target_executable,
                    const Workaround::Handler workaround,
                    const std::list<std::string>& arguments = {},
                    const std::map<std::string, std::string>& env_overrides = {},
                    const QString prefix = ""
    );
    void                                          select(const std::string& key);
    void                                          select_steamrt(const std::string& key);
    static std::string get_compat_dir_path();
    static std::string get_compat_c_drive();

protected:
private:
    steam_proton();
    static std::shared_ptr<steam_proton>          mInstance;
    std::shared_ptr<vlvproton>                    mProton;
    std::shared_ptr<vlvproton>                    mSteamRt;
    std::string                                   mSelected;
    std::shared_ptr<QProcess>                     mProcess;
    std::shared_ptr<QProcessEnvironment>          mProcessEnvironment;

    std::map<std::string, std::string>            mSetupEnvironment;
};
