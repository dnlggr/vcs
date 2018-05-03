//
// Created by Jannik Siebert on 22/10/2017.
//

#include "CommitAction.h"

CommitAction::CommitAction(const Command &command) : Action(command) {}

void CommitAction::execute() {
    Action::execute();

    Storage storage = *Storage::Instance();

    string message;
    if (!command.getArguments().empty()) {
        message = command.getArguments()[0];
    }

    if (storage.fileExists(storage.HEAD_FILE())) {
        // at least one other commit exists

        // load HEAD
        Commit head = Commit::load(Helper::getHeadCommitId());

        // build chain
        vector<Commit> chain = head.getSubChain();

        // build replay/
        Helper::buildReplay(chain, storage.REPLAY_DIRECTORY());

        // diff changes
        Diff diff(storage.REPLAY_DIRECTORY(), storage.WORKING_DIRECTORY());

        // todo:
        // real .vcs location instead of execute path
        vector<fs::path> additions = diff.getAdditions();
        vector<fs::path> deletions = diff.getDeletions();
        vector<fs::path> differences = diff.getDifferences();

        // create new commit (with parent = HEAD)
        Commit newCommit;

        // check if merge active
        if (storage.fileExists(storage.MERGE_FILE())) {
            string theirCommitId = readMergeFile();

            if (message.empty()) {
                message = "Merge branch with '" + theirCommitId + "'";
            }

            newCommit = Commit(head.getId(), theirCommitId, Helper::loadAuthor(), message);
        } else {
            newCommit = Commit(head.getId(), Helper::loadAuthor(), message);
        }

        // save diff to commit
        if (!Helper::commitDiff(newCommit, diff)) { return; }

        // add children to old HEAD
        head.addChild(newCommit.getId());

        // print additions
        Commit::printChangeWithoutSeparator(additions, "+", "added");

        // print deletions
        Commit::printChangeWithoutSeparator(deletions, "-", "removed");

        // print diffs
        Commit::printChangeWithoutSeparator(differences, "~", "changed");

        // delete merge file
        if (storage.fileExists(storage.MERGE_FILE())) {
            storage.remove(storage.MERGE_FILE());
        }
    } else {
        // no commits yet

        // create base commit (with parent = BASE)
        Commit baseCommit("BASE", Helper::loadAuthor(), message);

        // copy all files to additions directory
        storage.copyRepositoryToTemporaryDirectory();

        // save commit to commits/0
        baseCommit.save();

        // move tmp/ directory to changes/0/
        storage.moveDirectory(storage.TEMPORARY_DIRECTORY(), storage.ADDITIONS_DIRECTORY(baseCommit.getId()));

        // set base commit as HEAD
        baseCommit.makeHead();

        // print message
        cout << "All files successfully commited." << endl;
    }
}

string CommitAction::readMergeFile() {
    try {
        fs::ifstream stream;
        stream.open(Storage::Instance()->MERGE_FILE());

        stringstream content;
        content << stream.rdbuf();

        string mergeId;
        content >> mergeId;

        return mergeId;
    } catch (fs::filesystem_error const &exception) {
        cerr << "Error: Author: " << exception.what() << endl;
    }

    return "";
}
