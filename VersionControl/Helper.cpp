//
// Created by Jannik Siebert on 26/10/2017.
//

#include "Helper.h"

const string Helper::getHeadCommitId() {
    try {
        fs::ifstream stream;
        stream.open(Storage::Instance()->HEAD_FILE());

        stringstream content;
        content << stream.rdbuf();

        string commitId;
        content >> commitId;

        return commitId;
    } catch (fs::filesystem_error const &exception) {
        cerr << "Error: Storage: " << exception.what() << endl;
    }

    return "";
}

const bool Helper::buildReplay(const vector<Commit> &chain, const fs::path &directory) {
    Storage storage = *Storage::Instance();

    // clear replay directory
    if (storage.directoryExists(directory)) {
        storage.remove(directory);
    }

    // create empty replay directory
    storage.createDirectory(directory);

    // reverse iterate over chain (first commit first)
    for (unsigned long i = chain.size(); i > 0; i--) {
        string currentId = chain.at(i - 1).getId();

        fs::path additionsDirectory = storage.ADDITIONS_DIRECTORY(currentId);
        fs::path diffsDirectory = storage.DIFFERENCES_DIRECTORY(currentId);
        fs::path deletionsFile = storage.DELETIONS_FILE(currentId);

        // apply additions todo: refactor to method (see deletion)
        if (storage.directoryExists(additionsDirectory)) {
            storage.copy(additionsDirectory, directory);
        }

        // treat changed files like new files
        if (storage.directoryExists(diffsDirectory)) {
            storage.copy(diffsDirectory, directory);
        }

        // apply deletions
        applyDeletions(currentId, directory);
    }

    return false;
}

// todo:
// move to Info class
const string Helper::loadAuthor() {
    try {
        fs::ifstream stream;
        stream.open(Storage::Instance()->AUTHOR_FILE());

        stringstream content;
        content << stream.rdbuf();

        string author;
        content >> author;

        return author;
    } catch (fs::filesystem_error const &exception) {
        cerr << "Error: Author: " << exception.what() << endl;
    }

    return "";
}

void Helper::applyDeletions(const string &commitId, const fs::path &directory) {
    Storage storage = *Storage::Instance();

    for (fs::path deletion : Commit::load(commitId).getDeletions()) {
        storage.remove(directory / deletion);
    }
}

bool Helper::commitDiff(Commit &commit, Diff &diff) {
    return commitDiff(commit, diff, Storage::Instance()->WORKING_DIRECTORY());
}

bool Helper::commitDiff(Commit &commit, Diff &diff, const fs::path &prefix) {
    Storage storage = *Storage::Instance();

    vector<fs::path> additions = diff.getAdditions();
    vector<fs::path> deletions = diff.getDeletions();
    vector<fs::path> differences = diff.getDifferences();

    // check if any empty
    if (additions.empty() && differences.empty() && deletions.empty()) {
        cout << "No files changed. Nothing to commit..." << endl;
        return false;
    }

    // save additions in commit changes directory
    if (!additions.empty()) {
        storage.copyFilesToDirectory(additions, storage.TEMPORARY_DIRECTORY(), prefix);
        storage.moveDirectory(storage.TEMPORARY_DIRECTORY(), storage.ADDITIONS_DIRECTORY(commit.getId()));
    }

    // save deletions in commit changes directory
    if (!deletions.empty()) {
        storage.writeToFile(deletions, storage.DELETIONS_FILE(commit.getId()));
    }

    // save differences in commit changes directory
    if (!differences.empty()) {
        storage.copyFilesToDirectory(differences, storage.TEMPORARY_DIRECTORY(), prefix);
        storage.moveDirectory(storage.TEMPORARY_DIRECTORY(), storage.DIFFERENCES_DIRECTORY(commit.getId()));
    }

    // save commit
    commit.save();

    // set commit as HEAD
    commit.makeHead();

    return true;
}

bool Helper::mergeCommitDiff(Commit &commit, Diff &diff) {
    return mergeCommitDiff(commit, diff, Storage::Instance()->MERGE_DIRECTORY_THEIRS());
}

bool Helper::mergeCommitDiff(Commit &commit, Diff &diff, const fs::path &prefix) {
    Storage storage = *Storage::Instance();

    vector<fs::path> additions = diff.getAdditions();

    // save additions in commit changes directory
    if (!additions.empty()) {
        storage.copyFilesToDirectory(additions, storage.TEMPORARY_DIRECTORY(), prefix);
        storage.moveDirectory(storage.TEMPORARY_DIRECTORY(), storage.ADDITIONS_DIRECTORY(commit.getId()));
    }

    // save commit
    commit.save();

    // set commit as HEAD
    commit.makeHead();

    return true;
}
