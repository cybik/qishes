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

class steam_proton {
public:
    static std::shared_ptr<steam_proton> getInstance();

    void try_setup();
    void try_run(   const std::string& target_executable,
                    const std::list<std::string>& arguments = {},
                    std::map<std::string, std::string> env_overrides = {}
    );
    void                                          select(const std::string& key);

protected:
private:
    steam_proton();
    static std::shared_ptr<steam_proton>          mInstance;
    std::shared_ptr<vlvproton>                    mProton;
    std::string                                   mSelected;
    std::shared_ptr<QProcess>                     mProcess;
    std::shared_ptr<QProcessEnvironment>          mProcessEnvironment;
    std::shared_ptr<QStringList>                  mArguments;

    std::map<std::string, std::string>            mSetupEnvironment;
};
