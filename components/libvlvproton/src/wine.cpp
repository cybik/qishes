//
// Created by cybik on 24-12-23.
//

#include <iostream>
#include <ostream>
#include <wine.h>
#include <algorithm>


std::string wine::resolve_executable_path(const std::string & target_exec) {
    if (!target_exec.contains(":\\")) {
        return target_exec;
    }
    std::string processed_exec_path = target_exec.substr(target_exec.find_first_of(":")+1);
    std::ranges::replace(processed_exec_path, '\\', '/'); // initial
    processed_exec_path.insert(0, "/pfx/drive_c/");
    processed_exec_path.insert(0, std::getenv("STEAM_COMPAT_DATA_PATH"));
    size_t pos;
    while (( pos = processed_exec_path.find("//")) != std::string::npos) {
        processed_exec_path.replace(pos, 2, "/");
    }
    return processed_exec_path.substr(0, processed_exec_path.find_last_of("/"));
}

