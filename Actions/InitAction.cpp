//
// Created by Jannik Siebert on 18/10/2017.
//

#include "InitAction.h"

InitAction::InitAction(const Command &command) : Action(command) {}

void InitAction::execute() {
    Action::execute();

    Storage storage = *Storage::Instance();
    
    if (storage.directoryExists()) {
        cout << "VCS is already initialized." << endl;
        return;
    }

    cout << "VCS is being initialized. Please wait..." << endl;

    bool success = true;
    success &= storage.createDirectory();
    success &= storage.createDirectory(storage.COMMIT_DIRECTORY());
    success &= storage.createDirectory(storage.CHANGE_DIRECTORY());

    try {
        fs::ofstream stream;
        stream.open(storage.AUTHOR_FILE());

        stream << "<Author>" << endl;
    } catch (fs::filesystem_error const &exception) {
        success = false;
    }

    if (success) {
        cout << "VCS was successfully initialized..." << endl;
    } else {
        cout << "Something went wrong setting up the file system. Aborting..." << endl;
    }
}
