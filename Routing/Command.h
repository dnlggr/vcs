//
// Created by Jannik Siebert on 18/10/2017.
//

#ifndef VCS_COMMAND_H
#define VCS_COMMAND_H

#include <string>
#include <vector>

#include <boost/filesystem.hpp>

#include "../Config.h"

namespace fs = boost::filesystem;

class Command {
public:
    Command(int argc, char *argv[]);
    Command(string action);
    Command(string action, vector<string> arguments);

    int getArgc() const;
    void setArgc(int argc);

    char *const *getArgv() const;
    void setArgv(char *argv[]);

    const string &getAction() const;
    void setAction();

    const vector<string> &getArguments() const;
    void setArguments();

    const string &getPath() const;
    void setPath();
private:
    int argc;
    char **argv;

    string action;
    vector<string> arguments;

    string path;
};

#endif //VCS_COMMAND_H
