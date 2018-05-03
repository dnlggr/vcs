//
// Created by Daniel on 25.10.17.
//

#include "GraphAction.h"

GraphAction::GraphAction(const Command &command) : Action(command) {}

void GraphAction::execute() {
    Action::execute();

    printAdvancedGraph();

    cout << endl;
}

void GraphAction::printAdvancedGraph() {
    Commit head = Commit::load(Helper::getHeadCommitId());

    cout << endl << " Current branch: '" << head.getId() << "'" << endl;

    if (head.getParent() == "BASE") {
        cout << endl << " o   " << head.getId() << endl;
        return;
    }

    vector<Commit> headChain = head.getSubChain();

    Commit base = headChain.back();


    // todo: column to commit map? for efficiency


    // get all leaves
    vector<Commit> leaves = getLeaves(base);

    // create subchain for each leaf containing only unique commits
    vector<vector<Commit>> chains;

    for (const Commit &leaf : leaves) {
        vector<Commit> chain;
        for (Commit commit = leaf;;) {
            chain.push_back(commit);

            if (commit.getParent() == "BASE") {
                break;
            }

            Commit parent = Commit::load(commit.getParent());
            if (parent.getChildren().size() > 1 && parent.getChildren().front() != commit.getId()) {
                // cancel if not first child
                break;
            }

            commit = parent;
        }
        chains.push_back(chain);
    }

    int columns = chains.size();
    vector<bool> columnActive(columns, false);
    int mergeColumn = -1;
    int columShift = 0;
    bool showCommitLine = true;

    // loop until all chains empty
    int index = getIndexOfFirstNonEmptyChain(chains);
    while (index != -1) {

        // get newest commit
        Commit newestCommit = chains.at(index).front();
        vector<Commit> newestChain = chains.at(index);

        for (int i = 0; i < chains.size(); i++) {
            if (chains.at(i).empty()) {
                continue;
            }

            Commit nextCommit = chains.at(i).front();

            if (difftime(nextCommit.getDateTime(), newestCommit.getDateTime()) > 0) {
                newestCommit = nextCommit;
                newestChain = chains.at(i);
                index = i;
            }
        }

        // remove commit
        if (showCommitLine) {
            chains.at(index).erase(chains.at(index).begin());
        }

        // deactivate other columns
        if (newestCommit.getChildren().size() > 1) {
            fill(columnActive.begin() + index + 1, columnActive.begin() + index + newestCommit.getChildren().size() + columShift, false);
        }

        // empty line
        for (int i = 0; i < columns; i++) {
            if (columnActive.at(i)) {
                cout << " | ";
            } else if (newestCommit.getChildren().size() > 1 && (index < i && i < newestCommit.getChildren().size())) {
                cout << "/  ";
            } else if (i == mergeColumn) {
                cout << "\\  ";
                mergeColumn = -1;
                columShift++;
            } else {
                cout << "   ";
            }
        }

        cout << endl;

        // activate column
        columnActive.at(index) = true;

        // set merge column
        if (!newestCommit.getRelative().empty()) {
            mergeColumn = index + 1;
        }

        // commit bubble
        for (int i = 0; i < columns; i++) {
            if (i == index) {
                cout << " o ";
            } else if (columnActive.at(i)) {
                cout << " | ";
            } else {
                cout << "   ";
            }
        }

        cout << "\t " << newestCommit.getId() << "\t " << newestCommit.getMessage() << endl;

        index = getIndexOfFirstNonEmptyChain(chains);
    }
}

// todo:
// move into commit class?
vector<Commit> GraphAction::getLeaves(Commit commit) {
    vector<string> children = commit.getChildren();

    if (children.empty()) {
        // is leaf
        return vector<Commit>{commit};
    }

    // not leaf
    vector<Commit> leaves;
    for (const string child : children) {
        vector<Commit> childLeaves = getLeaves(Commit::load(child));
        leaves.insert(leaves.end(), childLeaves.begin(), childLeaves.end());
    }

    return leaves;
}

int GraphAction::getIndexOfFirstNonEmptyChain(vector<vector<Commit>> doubleChain) {
    for (int i = 0; i < doubleChain.size(); i++) {
        if (!doubleChain.at(i).empty()) {
            return i;
        }
    }

    return -1;
}

void GraphAction::printNTimes(const string &content, const int &n) {
    for (int i = n; i > 0; i--) {
        cout << content;
    }
}
