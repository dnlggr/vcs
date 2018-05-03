//
// Created by Jannik Siebert on 18/10/2017.
//

#include "Storage.h"

Storage::Storage() {
    Storage::workingPath = fs::current_path();
    Storage::storagePath = workingPath / Config::VCS_STORAGE_DIRECTORY;

    // todo:
    // check if .vcs exists (if not recursive check ../ )
}

// singleton

Storage *Storage::Instance() {
    if (!StorageInstance) {
        StorageInstance = new Storage();
    }

    return StorageInstance;
}

// getters

const fs::path Storage::WORKING_DIRECTORY() const {
    return workingPath;
}

const fs::path Storage::STORAGE_DIRECTORY() const {
    return storagePath;
}

const fs::path Storage::CHANGE_DIRECTORY() const {
    return STORAGE_DIRECTORY() / Config::VCS_CHANGE_DIRECTORY;
}

const fs::path Storage::COMMIT_DIRECTORY() const {
    return STORAGE_DIRECTORY() / Config::VCS_COMMIT_DIRECTORY;
}

const fs::path Storage::TEMPORARY_DIRECTORY() const {
    return STORAGE_DIRECTORY() / Config::VCS_TEMPORARY_DIRECTORY;
}

const fs::path Storage::REPLAY_DIRECTORY() const {
    return STORAGE_DIRECTORY() / Config::VCS_REPLAY_DIRECTORY;
}

const fs::path Storage::MERGE_DIRECTORY_OURS() const {
    return STORAGE_DIRECTORY() / Config::VCS_MERGE_DIRECTORY_OURS;
}

const fs::path Storage::MERGE_DIRECTORY_THEIRS() const {
    return STORAGE_DIRECTORY() / Config::VCS_MERGE_DIRECTORY_THEIRS;
}

const fs::path Storage::MERGE_FILE() const {
    return STORAGE_DIRECTORY() / Config::VCS_MERGE_FILE;
}

const fs::path Storage::HEAD_FILE() const {
    return STORAGE_DIRECTORY() / Config::VCS_HEAD_FILE;
}

const fs::path Storage::AUTHOR_FILE() const {
    return STORAGE_DIRECTORY() / Config::VCS_AUTHOR_FILE;
}

const fs::path Storage::CHANGE_DIRECTORY(const string &commitId) const {
    return STORAGE_DIRECTORY() / Config::VCS_CHANGE_DIRECTORY / commitId;
}

const fs::path Storage::COMMIT_FILE(const string &commitId) const {
    return STORAGE_DIRECTORY() / Config::VCS_COMMIT_DIRECTORY / commitId;
}


const fs::path Storage::ADDITIONS_DIRECTORY(const string &commitId) const {
    return CHANGE_DIRECTORY(commitId) / Config::VCS_ADDITIONS_DIRECTORY;
}

const fs::path Storage::DIFFERENCES_DIRECTORY(const string &commitId) const {
    return CHANGE_DIRECTORY(commitId) / Config::VCS_DIFFERENCES_DIRECTORY;
}

const fs::path Storage::DELETIONS_FILE(const string &commitId) const {
    return CHANGE_DIRECTORY(commitId) / Config::VCS_DELETIONS_FILE;
}

// file system operations

bool Storage::createDirectory() {
    return createDirectory(STORAGE_DIRECTORY());
}

bool Storage::createDirectory(const fs::path &path) {
    try {
        return fs::create_directory(path);
    } catch (fs::filesystem_error &exception) {
        cerr << "Error: Storage: Something went wrong." << endl;
    }

    return false;
}

bool Storage::directoryExists() {
    return directoryExists(STORAGE_DIRECTORY());
}

bool Storage::directoryExists(const fs::path &path) {
    try {
        return (fs::exists(path) && fs::is_directory(path));
    } catch (fs::filesystem_error &exception) {
        cerr << "Error: Storage: Something went wrong." << exception.what() << endl;
    }

    return false;
}

void Storage::copyRepositoryToTemporaryDirectory() {
    Storage::copy(WORKING_DIRECTORY(), TEMPORARY_DIRECTORY());
}

void Storage::copyFilesToDirectory(const vector<fs::path> &list, const fs::path &directory) {
    copyFilesToDirectory(list, directory, WORKING_DIRECTORY());
}

void Storage::copyFilesToDirectory(const vector<fs::path> &list, const fs::path &directory, const fs::path &prefix) {
    for (fs::path file : list) {
        Storage::copy(prefix / file, directory / file);
    }
}

// todo: fails of source is a file
bool Storage::copy(const fs::path &source, const fs::path &destination) {
    // todo:
    // change from cerr to throw exception
    // refactor this whole method!

    try {
        // todo:
        // think about recursive does parent exist method
        if (!fs::exists(source.parent_path()) && !Storage::createDirectory(source.parent_path())) {
            cerr << "Error: Storage: Could not create source parent." << endl;
            return false;
        }

        if (!fs::exists(destination.parent_path()) && !Storage::createDirectory(destination.parent_path())) {
            cerr << "Error: Storage: Could not create destination parent." << endl;
            return false;
        }

        if (!fs::is_directory(source) && !fs::is_directory(destination)) {
            fs::copy_file(source, destination, fs::copy_option::overwrite_if_exists);
            return true;
        }

        if (!fs::is_directory(source) && fs::is_directory(destination)) {
            fs::copy_file(source, destination / source.filename(), fs::copy_option::overwrite_if_exists);
            return true;
        }

        if (!directoryExists(source)) {
            cerr << "Error: Storage: Directory does not exist. " << source << endl;
            return false;
        }

        if (!fs::exists(destination) && !createDirectory(destination)) {
            cerr << "Error: Storage: Could not create destination. " << destination << endl;
            return false;
        }
    } catch (fs::filesystem_error const &exception) {
        cerr << "Error: Storage: " << exception.what() << endl;
        return false;
    }

    for (fs::directory_iterator file(source); file != fs::directory_iterator(); file++) {
        try {
            fs::path current(file->path());
            // todo: !important
            // change from ends with to full path compare

            if (fs::is_directory(current)) {
                // found storage directory (ignore)
                if (alg::ends_with(current, STORAGE_DIRECTORY())) {
                    continue;
                }

                // found directory (recursive)
                if (!copy(current, destination / current.filename())) {
                    return false;
                }
            } else {
                // found file (copy)
                fs::copy_file(current, destination / current.filename(), fs::copy_option::overwrite_if_exists);
            }
        } catch (fs::filesystem_error const &exception) {
            cerr << "Error: Storage: " << exception.what() << endl;
            return false;
        }
    }

    return true;
}

bool Storage::moveDirectory(const fs::path &source, const fs::path &destination) {
    try {
        if (!directoryExists(destination.parent_path())) {
            createDirectory(destination.parent_path());
        }

        fs::rename(source, destination);
        return true;
    } catch (fs::filesystem_error const &exception) {
        cerr << "Error: Storage: " << exception.what() << endl;
    }

    return false;
}

bool Storage::fileExists(const fs::path &path) {
    try {
        return fs::is_regular_file(path);
    } catch (fs::filesystem_error const &exception) {
        cerr << "Error: Storage: " << exception.what() << endl;
    }

    return false;
}

bool Storage::writeToFile(vector<fs::path> fileList, fs::path file) {
    try {
        if (!directoryExists(file.parent_path())) {
            createDirectory(file.parent_path());
        }

        fs::ofstream stream;
        stream.open(file);

        for (fs::path file : fileList) {
            stream << file.string() << endl;
        }
    } catch (fs::filesystem_error const &exception) {
        cerr << "Error: Storage: " << exception.what() << endl;
        return false;
    }

    return true;
}

bool Storage::remove(const fs::path &path) {
    try {
        fs::remove_all(path);
        return true;
    } catch (fs::filesystem_error const &exception) {
        cerr << "Error: Storage: " << exception.what() << endl;
    }

    return false;
}

bool Storage::clearWorkingDirectory() {
    if (!directoryExists(WORKING_DIRECTORY())) {
        return false;
    }

    for (fs::path path : b::make_iterator_range(fs::directory_iterator(WORKING_DIRECTORY()))) {
        if (path != STORAGE_DIRECTORY()) {
            remove(path);
        }
    }

    return true;
}
