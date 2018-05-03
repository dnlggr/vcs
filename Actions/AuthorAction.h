//
// Created by Daniel on 23.10.17.
//

#ifndef VCS_AUTHORACTION_H
#define VCS_AUTHORACTION_H

#include "Action.h"

class AuthorAction : public Action {
public:
    AuthorAction(const Command &command);

    void execute();
};


#endif //VCS_AUTHORACTION_H
