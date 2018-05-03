//
// Created by Jannik Siebert on 18/10/2017.
//

#include "Action.h"

Action::Action(const Command &command) : command(command) {
    Action::command = command;
}

void Action::execute() {
    if (Config::VCS_LOGGING_VERBOSE) {
        printIntroduction();
    }
}

const Command &Action::getCommand() const {
    return command;
}

void Action::printIntroduction() {
    string action = command.getAction();
    string arguments = alg::join(command.getArguments(), " ");

    cout << "╔═══════════════════════════════════╗" << endl;
    cout << "║  VCS                      v0.1.0  ║" << endl;
    cout << "╟───────────────────────────────────╢" << endl;
    cout << "║  by Daniel Egger, Jannik Siebert  ║" << endl;
    cout << "╚═══════════════════════════════════╝" << endl << endl;
    cout << "  - Action:     " << action << endl;
    cout << "  - Arguments:  " << arguments << endl << endl;
}
