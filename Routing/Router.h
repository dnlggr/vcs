//
// Created by Jannik Siebert on 18/10/2017.
//

#ifndef VCS_ROUTER_H
#define VCS_ROUTER_H

#include <cstdlib>

#include "../Config.h"
#include "Command.h"

#include "../Actions/InitAction.h"
#include "../Actions/CommitAction.h"
#include "../Actions/AuthorAction.h"
#include "../Actions/ShowAction.h"
#include "../Actions/HelpAction.h"
#include "../Actions/GraphAction.h"
#include "../Actions/CheckoutAction.h"
#include "../Actions/MergeAction.h"

class Router {
    Command command;

public:
    Router(const Command &command);

    int execute();

};

#endif //VCS_ROUTER_H
