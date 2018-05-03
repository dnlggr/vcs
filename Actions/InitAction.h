//
// Created by Jannik Siebert on 18/10/2017.
//

#ifndef VCS_INITACTION_H
#define VCS_INITACTION_H

#include "Action.h"

class InitAction : public Action {
public:
    InitAction(const Command &command);

    void execute();
};


#endif //VCS_INITACTION_H
