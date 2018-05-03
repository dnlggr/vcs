//
// Created by Jannik Siebert on 22/10/2017.
//

#ifndef VCS_COMMITACTION_H
#define VCS_COMMITACTION_H

#include "Action.h"

#include "../VersionControl/Diff.h"
#include "../VersionControl/Helper.h"

class CommitAction : public Action {
public:
    CommitAction(const Command &command);

    void execute();

private:
    string readMergeFile();

};


#endif //VCS_COMMITACTION_H
