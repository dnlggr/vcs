//
// Created by Daniel on 25.10.17.
//

#ifndef VCS_GRAPHACTION_H
#define VCS_GRAPHACTION_H

#include <sstream>
#include <algorithm>

#include "Action.h"

#include <VersionControl/Helper.h>

class GraphAction : public Action {
public:
    GraphAction(const Command &command);

    void execute();

private:
    void printAdvancedGraph();
    vector<Commit> getLeaves(Commit commit);

    int getIndexOfFirstNonEmptyChain(vector<vector<Commit>> doubleChain);
    void printNTimes(const string &content, const int &n);
};

#endif //VCS_GRAPHACTION_H
