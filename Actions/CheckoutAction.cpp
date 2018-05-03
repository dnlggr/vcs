//
// Created by Daniel on 25.10.17.
//

#include "CheckoutAction.h"

CheckoutAction::CheckoutAction(const Command &command) : Action(command) {

}

void CheckoutAction::execute() {
    Action::execute();

    Storage storage = *Storage::Instance();

    if (command.getArguments().empty()) {
        cout << "No commit to checkout specified. Nothing to checkout..." << endl;
        return;
    }

    string commitId;
    Commit commit;

    try {
        commitId = command.getArguments().at(0);
        commit = Commit::load(commitId);
    } catch (...) {
        cout << "Could not checkout specified commit. Aborting..." << endl;
        return;
    }

    if (commit.getId() != commitId) {
        cout << "Could not checkout specified commit. Aborting..." << endl;
        return;
    }

    // build chain
    vector<Commit> chain = commit.getSubChain();

    // build replay/
    Helper::buildReplay(chain, storage.REPLAY_DIRECTORY());

    // clean working directory
    storage.clearWorkingDirectory();

    // apply checked out commit to working directory
    storage.copy(storage.REPLAY_DIRECTORY(), storage.WORKING_DIRECTORY());

    // set HEAD to checked out commit
    commit.makeHead();

    cout << "Successfully checked out commit '" << commit.getId() << "'." << endl;
}
