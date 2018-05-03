//
// Created by Daniel on 23.10.17.
//

#include "Diff.h"

Diff::Diff(const fs::path &oldDirectory, const fs::path &newDirectory) : oldDirectory(oldDirectory), newDirectory(newDirectory) {
    detectFileChanges();
}

void Diff::detectFileChanges() {
    // initialize
    vector<fs::path> additions;
    vector<fs::path> deletions;
    vector<fs::path> diffs;

    // execute diff command. Todo: enable recursive diffs
    string command = "diff -q " + string(oldDirectory.c_str()) + " " + string(newDirectory.c_str());
    istringstream result(Utils::stdoutFromCommand(command));

    // iterate over changes
    string line;
    while (getline(result, line)) {
        detectAdditions(line, additions);
        detectDeletions(line, deletions);
        detectDiffs(line, diffs);
    }

    Diff::additions = additions;
    Diff::deletions = deletions;
    Diff::diffs = diffs;
}

vector<fs::path> Diff::getAdditions() {
    return additions;
}

vector<fs::path> Diff::getDeletions() {
    return deletions;
}

vector<fs::path> Diff::getDifferences() {
    return diffs;
}

void Diff::detectDiffs(const string &line, vector<fs::path> &diffs) {
    string diffMarkerStart = "and ";
    string diffMarkerEnd = " differ";
    if (line.find(diffMarkerEnd) != string::npos && line.find(diffMarkerStart) != string::npos) {
        unsigned long start = line.find(diffMarkerStart) + diffMarkerStart.length();
        unsigned long end = line.find(diffMarkerEnd);
        string absolutePath = line.substr(start, (end - start));
        fs::path relativePath = fs::path(absolutePath).lexically_relative(newDirectory);
        diffs.push_back(relativePath);
    }
}

void Diff::detectAdditions(const string &line, vector<fs::path> &additions) {
    string additionMarker = "Only in " + string(newDirectory.c_str()) + ": ";
    if (line.find(additionMarker) != string::npos) {
        fs::path file = line.substr(additionMarker.size(), line.size());
        if (Storage::Instance()->WORKING_DIRECTORY() / file != Storage::Instance()->STORAGE_DIRECTORY()) {
            additions.push_back(file);
        }
    }
}

void Diff::detectDeletions(const string &line, vector<fs::path> &deletions) {
    string deletionMarker = "Only in " + string(oldDirectory.c_str()) + ": ";
    if (line.find(deletionMarker) != string::npos) {
        fs::path file = line.substr(deletionMarker.size(), line.size());
        if (Storage::Instance()->WORKING_DIRECTORY() / file != Storage::Instance()->STORAGE_DIRECTORY()) {
            deletions.push_back(file);
        }
    }
}
