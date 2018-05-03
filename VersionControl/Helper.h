//
// Created by Jannik Siebert on 26/10/2017.
//

#ifndef VCS_COMMITHELPER_H
#define VCS_COMMITHELPER_H

#include <sstream>

#include "Commit.h"
#include "Diff.h"

class Helper {
public:
    static const string getHeadCommitId();

    static const string loadAuthor();

    static const bool buildReplay(const vector<Commit> &chain, const fs::path &directory);

    static void applyDeletions(const string &commitId, const fs::path &directory);

    static bool commitDiff(Commit &commit, Diff &diff);
    static bool commitDiff(Commit &commit, Diff &diff, const fs::path &prefix);

    static bool mergeCommitDiff(Commit &commit, Diff &diff);
    static bool mergeCommitDiff(Commit &commit, Diff &diff, const fs::path &prefix);
};


#endif //VCS_COMMITHELPER_H
