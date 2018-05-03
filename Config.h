//
// Created by Jannik Siebert on 19/10/2017.
//

#ifndef VCS_CONFIG_H
#define VCS_CONFIG_H

#include <string>

#include <boost/filesystem.hpp>
#include "Management/Storage.h"

using namespace std;

namespace fs = boost::filesystem;

class Config {
public:
    static const fs::path VCS_STORAGE_DIRECTORY;

    static const fs::path VCS_CHANGE_DIRECTORY;
    static const fs::path VCS_COMMIT_DIRECTORY;

    static const fs::path VCS_TEMPORARY_DIRECTORY;
    static const fs::path VCS_REPLAY_DIRECTORY;

    static const fs::path VCS_ADDITIONS_DIRECTORY;
    static const fs::path VCS_DIFFERENCES_DIRECTORY;
    static const fs::path VCS_DELETIONS_FILE;

    static const fs::path VCS_HEAD_FILE;
    static const fs::path VCS_AUTHOR_FILE;

    static const fs::path VCS_MERGE_DIRECTORY_OURS;
    static const fs::path VCS_MERGE_DIRECTORY_THEIRS;
    static const fs::path VCS_MERGE_FILE;

    static const string VCS_SINGLE_SEPARATOR;
    static const string VCS_DOUBLE_SEPARATOR;

    static const bool VCS_LOGGING_VERBOSE;

};


#endif //VCS_CONFIG_H
