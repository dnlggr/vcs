//
// Created by Jannik Siebert on 18/10/2017.
//

#include "HelpAction.h"

HelpAction::HelpAction(const Command &command) : Action(command) {}

void HelpAction::execute() {
    Action::execute();
}
