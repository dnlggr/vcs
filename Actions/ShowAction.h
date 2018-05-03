//
// Created by Daniel on 23.10.17.
//

#ifndef VCS_SHOWACTION_H
#define VCS_SHOWACTION_H

#include "Action.h"

class ShowAction : public Action {
public:
    ShowAction(const Command &command);

    void execute();

private:
    b::optional<Commit> getCommit();

};


#endif //VCS_SHOWACTION_H
