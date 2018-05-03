//
// Created by Jannik Siebert on 18/10/2017.
//

#include "Command.h"

Command::Command(int argc, char *argv[]) {
    Command::setArgc(argc);
    Command::setArgv(argv);

    Command::setAction();
    Command::setArguments();

    Command::setPath();
}

// argc

int Command::getArgc() const {
    return argc;
}

void Command::setArgc(int argc) {
    Command::argc = argc;
}

// argv

char *const *Command::getArgv() const {
    return argv;
}

void Command::setArgv(char *argv[]) {
    Command::argv = argv;
}

// action

const string &Command::getAction() const {
    return action;
}

void Command::setAction() {
    if (argc <= 1) {
        action = "help";
    } else {
        action = argv[1];
    }
}

// arguments

const vector<string> &Command::getArguments() const {
    return arguments;
}


void Command::setArguments() {
    if (argc >= 3) {
        arguments = vector<string>(&argv[2], &argv[argc]);
    }
}

// path

const string &Command::getPath() const {
    return path;
}

void Command::setPath() {
    if (argc >= 1) {
        Command::path = argv[0];
    }
}

Command::Command(string action) : action(action) {}
Command::Command(string action, vector<string> arguments) : action(action), arguments(arguments) {}
