//
// Created by Jannik Siebert on 18/10/2017.
//

#ifndef VCS_HELP_H
#define VCS_HELP_H

#include "Action.h"

class HelpAction : public Action {
public:
    HelpAction(const Command &command);

    void execute();

};


#endif //VCS_HELP_H
