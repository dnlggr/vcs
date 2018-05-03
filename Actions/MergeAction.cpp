//
// Created by Jannik Siebert on 26/10/2017.
//

#include "MergeAction.h"

MergeAction::MergeAction(const Command &command) : Action(command) {}

void MergeAction::execute() {
    Action::execute();

    Storage storage = *Storage::Instance();

    // todo:
    // check if files ready to commit -> abort

    if (command.getArguments().empty()) {
        cout << "No commit to merge specified. Nothing to merge..." << endl;
        return;
    }

    // get their commit
    string theirCommitId;
    Commit theirCommit;

    try {
        theirCommitId = command.getArguments().at(0);
        theirCommit = Commit::load(theirCommitId);
    } catch (...) {
        cout << "Could not merge specified commit. Aborting..." << endl;
        return;
    }

    if (theirCommit.getId() != theirCommitId) {
        cout << "Could not merge specified commit. Aborting..." << endl;
        return;
    }

    // get our commit (HEAD)
    if (!storage.fileExists(storage.HEAD_FILE())) {
        cout << "Could not merge specified commit. Aborting..." << endl;
        return;
    }

    // todo:
    // backup working dir?

    // build replay OURS
    Commit ourCommit = Commit::load(Helper::getHeadCommitId());
    vector<Commit> ourChain = ourCommit.getSubChain();
    Helper::buildReplay(ourChain, storage.MERGE_DIRECTORY_OURS());

    // build replay THEIRS
    vector<Commit> theirChain = theirCommit.getSubChain();
    Helper::buildReplay(theirChain, storage.MERGE_DIRECTORY_THEIRS());

    // compare OURS vs THEIRS
    Diff diff(storage.MERGE_DIRECTORY_OURS(), storage.MERGE_DIRECTORY_THEIRS());

    vector<fs::path> additions = diff.getAdditions();

    // copy THEIRS into OURS
    Commit::printChangeWithoutSeparator(additions, "+", "added in merge");

    // todo:
    // save empty merge commits as well
    if (diff.getAdditions().empty() && diff.getDifferences().empty() && diff.getDeletions().empty()) {
        cout << endl << "Nothing to merge." << endl;
        return;
    }

    // check for conflict
    if (!diff.getDifferences().empty()) {
        Commit::printChangeWithoutSeparator(diff.getDifferences(), "~", "modified in a branch");

        // handle conflict
        mergeConflict(theirCommitId, diff);

        cout << endl << "MERGE CONFLICT: Original files have a '*.THEIRS' counterpart." << endl;
        cout << "Resolve conflicts manually and run 'vcs commit' to complete merge." << endl;
        return;
    }

    // make merge commit message
    stringstream message;
    message << "Merge branch with '" << theirCommit.getId() << "'";

    // create merge commit
    Commit mergeCommit(ourCommit.getId(), theirCommit.getId(), Helper::loadAuthor(), message.str());

    // save diff to commit
    Helper::mergeCommitDiff(mergeCommit, diff);

    // add children to old HEAD
    ourCommit.addChild(mergeCommit.getId());

    // apply additions
    storage.copy(storage.ADDITIONS_DIRECTORY(mergeCommit.getId()), storage.WORKING_DIRECTORY());
}

void MergeAction::mergeConflict(const string &theirCommitId, Diff &diff) {
    Storage storage = *Storage::Instance();

    // create MERGE file
    try {
        fs::ofstream stream;
        stream.open(storage.MERGE_FILE());

        stream << theirCommitId << endl;
    } catch (fs::filesystem_error const &exception) {
        cerr << "Error: Merge: " << exception.what() << endl;
        return;
    }

    // todo: check MERGE file in commit action

    // apply additions
    storage.copyFilesToDirectory(diff.getAdditions(), storage.WORKING_DIRECTORY(), storage.MERGE_DIRECTORY_THEIRS());

    // create .THEIRS files
    for (const fs::path changedFile : diff.getDifferences()) {
        storage.copy(storage.MERGE_DIRECTORY_THEIRS() / changedFile, storage.WORKING_DIRECTORY() / fs::path(changedFile.string() + ".THEIRS"));
    }
}
