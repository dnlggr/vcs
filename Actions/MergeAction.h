//
// Created by Jannik Siebert on 26/10/2017.
//

#ifndef VCS_MERGEACTION_H
#define VCS_MERGEACTION_H

#include "Action.h"

#include "../VersionControl/Diff.h"
#include "../VersionControl/Helper.h"

class MergeAction : public Action {
public:
    MergeAction(const Command &command);

    void execute();

private:
    void mergeConflict(const string &theirCommitId, Diff &diff);
};


#endif //VCS_MERGEACTION_H
