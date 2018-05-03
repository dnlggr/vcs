//
// Created by Jannik Siebert on 18/10/2017.
//

#ifndef VCS_STORAGE_H
#define VCS_STORAGE_H

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include "../Config.h"
#include "../VersionControl/Commit.h"

using namespace std;

namespace b = boost;
namespace fs = boost::filesystem;
namespace alg = boost::algorithm;

class Storage {
public:
    static Storage *Instance();

    const fs::path WORKING_DIRECTORY() const;
    const fs::path STORAGE_DIRECTORY() const;

    const fs::path CHANGE_DIRECTORY() const;
    const fs::path COMMIT_DIRECTORY() const;
    const fs::path TEMPORARY_DIRECTORY() const;
    const fs::path REPLAY_DIRECTORY() const;

    const fs::path MERGE_DIRECTORY_OURS() const;
    const fs::path MERGE_DIRECTORY_THEIRS() const;
    const fs::path MERGE_FILE() const;

    const fs::path HEAD_FILE() const;
    const fs::path AUTHOR_FILE() const;

    const fs::path CHANGE_DIRECTORY(const string &commitId) const;
    const fs::path COMMIT_FILE(const string &commitId) const;

    const fs::path ADDITIONS_DIRECTORY(const string &commitId) const;
    const fs::path DIFFERENCES_DIRECTORY(const string &commitId) const;
    const fs::path DELETIONS_FILE(const string &commitId) const;

    bool createDirectory();
    bool createDirectory(const fs::path &path);
    bool directoryExists();
    bool directoryExists(const fs::path &path);
    bool fileExists(const fs::path &path);
    bool copy(const fs::path &source, const fs::path &destination);
    bool moveDirectory(const fs::path &source, const fs::path &destination);
    bool writeToFile(vector<fs::path> list, fs::path file);

    bool remove(const fs::path &path);

    void copyRepositoryToTemporaryDirectory();
    void copyFilesToDirectory(const vector<fs::path> &list, const fs::path &directory);
    void copyFilesToDirectory(const vector<fs::path> &list, const fs::path &directory, const fs::path &prefix);

    bool clearWorkingDirectory();

private:
    static Storage *StorageInstance;

    Storage();

    fs::path workingPath;
    fs::path storagePath;

};

#endif //VCS_STORAGE_H
