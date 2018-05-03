//
// Created by Daniel on 18.10.17.
//

#include "Commit.h"

// Todo: This constructor can probably be removed.
Commit::Commit(const string &id, const string &parent, const string &relative, const string &author, const string &date, const string &message, const vector<string> &children) : id(id), parent(parent), relative(relative), author(
        move(author)), date(move(date)), message(move(message)), children(children) {}

Commit::Commit(const string &parent, const string &author, const string &message) : parent(parent), author(
        move(author)), message(move(message)) {

    uuid::random_generator randomGenerator;
    id = uuid::to_string(randomGenerator());

    // local time. Todo: use UTC.
    time_t now = time(0);
    date = ctime(&now);
}

Commit::Commit(const string &parent, const string &relative, const string &author, const string &message) : Commit(parent, author, message) {
    this->relative = relative;
}

const string &Commit::getId() const {
    return id;
}

const string &Commit::getParent() const {
    return parent;
}

const string &Commit::getRelative() const {
    return relative;
}

const string &Commit::getAuthor() const {
    return author;
}

const string &Commit::getDate() const {
    return date;
}

const string &Commit::getMessage() const {
    return message;
}

const vector<string> &Commit::getChildren() const {
    return children;
}

void Commit::addChild(const string &childId) {
    children.push_back(childId);
    save();
}

const time_t & Commit::getDateTime() const {
    const char *timeString = const_cast<char*>(date.c_str());

    struct tm t;
    strptime(timeString, "%c", &t);

    time_t dateTime = mktime(&t);

    return dateTime;
}

void Commit::save() {
    fs::path filename(Storage::Instance()->COMMIT_FILE(id));

    fs::ofstream stream(filename);

    ar::text_oarchive archive(stream);

    archive << *this;
}

Commit Commit::load(string id) {
    Commit commit;

    fs::path filename(Storage::Instance()->COMMIT_FILE(id));

    fs::ifstream stream(filename);

    ar::text_iarchive archive(stream);

    archive >> commit;

    return commit;
}

vector<fs::path> Commit::getAdditions() {
    fs::path additionsDirectory = Storage::Instance()->ADDITIONS_DIRECTORY(id);

    if (!Storage::Instance()->directoryExists(additionsDirectory)) {
        return vector<fs::path>();
    }

    vector<fs::path> additions;
    for (const fs::path &path : b::make_iterator_range(fs::directory_iterator(additionsDirectory))) {
        additions.push_back(path.lexically_relative(Storage::Instance()->ADDITIONS_DIRECTORY(id)));
    }

    return additions;
}

vector<fs::path> Commit::getDeletions() {
    fs::path deletionsFile = Storage::Instance()->DELETIONS_FILE(id);

    if (!Storage::Instance()->fileExists(deletionsFile)) {
        return vector<fs::path>();
    }

    fs::ifstream stream;
    stream.open(deletionsFile);

    string line;
    vector<fs::path> deletions;

    // iterate over file
    while (getline(stream, line)) {
        deletions.push_back(line);  // todo: weird
    }

    return deletions;
}

vector<fs::path> Commit::getDifferences() {
    fs::path differencesDirectory = Storage::Instance()->DIFFERENCES_DIRECTORY(id);

    if (!Storage::Instance()->directoryExists(differencesDirectory)) {
        return vector<fs::path>();
    }

    vector<fs::path> differences;
    for (const fs::path &path : b::make_iterator_range(fs::directory_iterator(differencesDirectory))) {
        differences.push_back(path.lexically_relative(Storage::Instance()->DIFFERENCES_DIRECTORY(id)));
    }

    return differences;
}


void Commit::printMetaData() {
    cout << "Commit:  " << id << endl;

    cout << Config::VCS_DOUBLE_SEPARATOR << endl;

    cout << "Date:\t " << date;
    cout << "Author:\t " << author << endl;

    if (parent != "BASE") {
        cout << "Parent:\t " << parent << endl;
    } else {
        cout << "Parent:\t " << "This is the first commit." << endl;
    }

    if (!relative.empty()) {
        cout << "Merge:\t " << relative << endl;
    }

    cout << Config::VCS_SINGLE_SEPARATOR << endl;

    if (message.empty()) {
        cout << "\t  <No message specified>" << endl;
    } else {
        cout << message << endl;
    }
}

void Commit::printChanges() {
    printChange(getAdditions(), "+", "added");
    printChange(getDeletions(), "-", "removed");
    printChange(getDifferences(), "~", "changed");
}

void Commit::printChangeWithoutSeparator(const vector<fs::path> &updates, const string &symbol, const string &description) {
    printChange(updates, symbol, description, false);
}

void Commit::printChange(const vector<fs::path> &updates, const string &symbol, const string &description, const bool &useSeparator) {
    if (!updates.empty()) {
        if (useSeparator) {
            cout << Config::VCS_SINGLE_SEPARATOR << endl;
        }

        cout << "Files and directories " << description << ":" << endl;
        for (const fs::path &update : updates) {
            cout << " " << symbol << " " << update << endl;
        }
    }
}

bool Commit::makeHead() {
    try {
        fs::ofstream stream;
        stream.open(Storage::Instance()->HEAD_FILE());

        stream << id << endl;

        return true;
    } catch (fs::filesystem_error const &exception) {
        cerr << "Error: Storage: " << exception.what() << endl;
    }

    return false;
}

vector<Commit> Commit::getSubChain() {
    vector<Commit> chain = vector<Commit>();

    for (Commit i = *this; ; i = Commit::load(i.getParent())) {
        chain.push_back(i);

        // todo:
        // move base to config
        if (i.getParent() == "BASE") { break; }
    }

    return chain;
}
