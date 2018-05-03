//
// Created by Daniel on 23.10.17.
//

#include "AuthorAction.h"

AuthorAction::AuthorAction(const Command &command) : Action(command) {}

void AuthorAction::execute() {
    Action::execute();

    if (command.getArguments().empty()) {
        cout << "No author specified. Aborting..." << endl;
        return;
    }
    try {
        fs::ofstream stream;
        stream.open(Storage::Instance()->AUTHOR_FILE());

        string author = command.getArguments()[0];
        stream << author << endl;
        cout << "Changed author to '" << author << "'." << endl;
    } catch (fs::filesystem_error const &exception) {
        cerr << "Error: Storage: " << exception.what() << endl;
    }
}
