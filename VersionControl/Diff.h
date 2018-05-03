//
// Created by Daniel on 23.10.17.
//

#ifndef VCS_DIFF_H
#define VCS_DIFF_H

#include <vector>

#include <iostream>
#include <sstream>

#include <boost/filesystem.hpp>

#include "../Config.h"
#include "../Utils.h"
#include "../Management/Storage.h"

using namespace std;

namespace fs = boost::filesystem;

class Diff {
public:
    Diff(const fs::path &oldDirectory, const fs::path &newDirectory);

    vector<fs::path> getAdditions();
    vector<fs::path> getDeletions();
    vector<fs::path> getDifferences();

private:
    fs::path oldDirectory;
    fs::path newDirectory;

    vector<fs::path> additions;
    vector<fs::path> deletions;
    vector<fs::path> diffs;

    void detectFileChanges();
    void detectAdditions(const string &line, vector<fs::path> &additions);
    void detectDeletions(const string &line, vector<fs::path> &deletions);
    void detectDiffs(const string &line, vector<fs::path> &diffs);
};


#endif //VCS_DIFF_H
