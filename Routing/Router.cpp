//
// Created by Jannik Siebert on 18/10/2017.
//

#include "Router.h"

Router::Router(const Command &command) : command(command) {
    // todo:
    // init actions here
    // maybe read storage here and create singleton for global usage
}

int Router::execute() {

    if (command.getAction() == "help") {
        HelpAction action = HelpAction(command);
        action.execute();
    } else if (command.getAction() == "init") {
        InitAction action = InitAction(command);
        action.execute();
    } else if (command.getAction() == "commit") {
        CommitAction action = CommitAction(command);
        action.execute();
    } else if (command.getAction() == "author") {
        AuthorAction action = AuthorAction(command);
        action.execute();
    } else if (command.getAction() == "show") {
        ShowAction action = ShowAction(command);
        action.execute();
    } else if (command.getAction() == "graph") {
        GraphAction action = GraphAction(command);
        action.execute();
    } else if (command.getAction() == "checkout") {
        CheckoutAction action = CheckoutAction(command);
        action.execute();
    } else if (command.getAction() == "merge") {
        MergeAction action = MergeAction(command);
        action.execute();
    } else {
        Action action = Action(command);
        action.execute();

        cout << "The action `" + command.getAction() + "` is not implemented." << endl;
    }

    return EXIT_SUCCESS;
}
