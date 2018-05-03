//
// Created by Jannik Siebert on 18/10/2017.
//

#ifndef VCS_ACTION_H
#define VCS_ACTION_H

#include <string>
#include <sstream>
#include <iostream>

#include <boost/optional.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/join.hpp>

#include "../Config.h"
#include "../Routing/Command.h"
#include "../Management/Storage.h"

using namespace std;

namespace b = boost;
namespace fs = boost::filesystem;
namespace alg = boost::algorithm;

class Action {
public:
    Action(const Command &command);

    const Command &getCommand() const;

    void execute();

    Command command;

private:
    void printIntroduction();
};


#endif //VCS_ACTION_H
