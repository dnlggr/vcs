//
// Created by Daniel on 18.10.17.
//

#ifndef VCS_COMMIT_H
#define VCS_COMMIT_H

#include <utility>
#include <vector>
#include <ctime>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "../Config.h"
#include "../Management/Storage.h"

using namespace std;

namespace b = boost;
namespace ar = boost::archive;
namespace fs = boost::filesystem;
namespace sr = boost::serialization;
namespace uuid = boost::uuids;

class Commit {
public:
    Commit() {};
    Commit(const string &id, const string &parent, const string &relative, const string &author, const string &date, const string &message, const vector<string> &children);
    Commit(const string &parent, const string &author, const string &message);
    Commit(const string &parent, const string &relative, const string &author, const string &message);

    const string &getId() const;
    const string &getParent() const;
    const string &getRelative() const;
    const string &getAuthor() const;
    const string &getDate() const;
    const time_t & getDateTime() const;
    const string &getMessage() const;

    const vector<string> &getChildren() const;
    void addChild(const string &childId);

    void save();
    static Commit load(string id);

    vector<fs::path> getAdditions();
    vector<fs::path> getDeletions();
    vector<fs::path> getDifferences();

    void printMetaData();
    void printChanges();
    static void printChangeWithoutSeparator(const vector<fs::path> &updates, const string &symbol, const string &description);
    static void printChange(const vector<fs::path> &updates, const string &symbol, const string &description, const bool &useSeparator = true);

    bool makeHead();

    vector<Commit> getSubChain();

private:
    string id;
    string parent;
    string relative;
    string author;
    string date;
    string message;

    vector<string> children;

    friend class sr::access;
    template<class Archive>
    void serialize(Archive &archive, const unsigned int version) {
        archive & id;
        archive & parent;
        archive & relative;
        archive & author;
        archive & date;
        archive & message;
        archive & children;
    }
};


#endif //VCS_COMMIT_H
