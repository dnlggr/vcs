//
// Created by Daniel on 23.10.17.
//

#include "ShowAction.h"

ShowAction::ShowAction(const Command &command) : Action(command) {}

void ShowAction::execute() {
    Action::execute();

    if (command.getArguments().empty()) {
        cout << "No commit specified. Aborting..." << endl;
        return;
    }

    b::optional<Commit> commit = getCommit();

    if (commit) {
        commit.get().printMetaData();
        commit.get().printChanges();

        cout << Config::VCS_DOUBLE_SEPARATOR << endl << endl;

    } else {
        cout << "Could not load specified commit. Aborting..." << endl;
        return;
    }
}

b::optional <Commit> ShowAction::getCommit() {
    string commitId;
    b::optional<Commit> commit;

    try {
        commitId = command.getArguments().at(0);
        commit = Commit::load(commitId);
    } catch (...) {
        return b::optional<Commit>();
    }

    if (commit->getId() != commitId) {
        return b::optional<Commit>();
    }

    return commit;
}
