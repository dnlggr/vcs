//
// Created by Daniel on 27.10.17.
//

#include <Actions/CommitAction.h>
#include <Actions/CheckoutAction.h>
#include <Actions/MergeAction.h>
#include "gtest/gtest.h"

#include "../../Actions/InitAction.h"

namespace fs = boost::filesystem;

class MergeTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        s = Storage::Instance();
        InitAction a(Command("init"));
        a.execute();

        commits = fs::current_path() / ".vcs/**";
    }

    virtual void TearDown() {
        Utils::stdoutFromCommand("rm -rf " + (fs::current_path() / ".vcs").string());
        Utils::stdoutFromCommand("rm -rf " + (fs::current_path() / "ab_1").string());
        Utils::stdoutFromCommand("rm -rf " + (fs::current_path() / "ab_1.THEIRS").string());
        Utils::stdoutFromCommand("rm -rf " + (fs::current_path() / "a_2").string());
        Utils::stdoutFromCommand("rm -rf " + (fs::current_path() / "a_3").string());
        Utils::stdoutFromCommand("rm -rf " + (fs::current_path() / "a_4").string());
        Utils::stdoutFromCommand("rm -rf " + (fs::current_path() / "b_5").string());
        Utils::stdoutFromCommand("rm -rf " + (fs::current_path() / "b_6").string());
    }

    string getHead() {
        try {
            fs::ifstream stream;
            stream.open(fs::current_path() / ".vcs/HEAD");

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

    string fileContent(fs::path file) {
        try {
            fs::ifstream stream;
            stream.open(file);

            stringstream content;
            content << stream.rdbuf();

            string c;
            content >> c;

            return c;
        } catch (fs::filesystem_error const &exception) {
            cerr << "Error: Storage: " << exception.what() << endl;
        }

        return "";
    }

    Storage *s;
    fs::path commits;
    fs::path changes;
};

TEST_F(MergeTest, TestMergeWithConflicts) {
    // On *a* commit 1 2 3 4
    Utils::stdoutFromCommand("echo theirs > " + (fs::current_path() / "ab_1").string());
    CommitAction a(Command("commit"));
    a.execute();
    Utils::stdoutFromCommand("touch " + (fs::current_path() / "a_2").string());
    a.execute();
    string a_2 = getHead();
    Utils::stdoutFromCommand("touch " + (fs::current_path() / "a_3").string());
    a.execute();
    Utils::stdoutFromCommand("touch " + (fs::current_path() / "a_4").string());
    a.execute();
    string a_4 = getHead();

    // On *a* checkout 2
    vector<string> argsCheckout{a_2};
    CheckoutAction ca((Command("checkout", argsCheckout)));
    ca.execute();

    // On *b* commit 5 6
    Utils::stdoutFromCommand("echo ours > " + (fs::current_path() / "ab_1").string());
    a.execute();
    Utils::stdoutFromCommand("touch " + (fs::current_path() / "b_5").string());
    a.execute();
    Utils::stdoutFromCommand("touch " + (fs::current_path() / "b_6").string());
    a.execute();
    string b_6 = getHead();

    // merge a into b
    vector<string> argsMerge{a_4};
    MergeAction ma((Command("merge", argsMerge)));
    ma.execute();

    // check that their merge conflict exists
    EXPECT_EQ(s->fileExists(fs::current_path() / "ab_1.THEIRS"), true);
    string content = fileContent((fs::current_path() / "ab_1.THEIRS").string());
    EXPECT_EQ(content, "theirs");

    // check that our merge conflict exists
    content = fileContent((fs::current_path() / "ab_1").string());
    EXPECT_EQ(content, "ours");
    Utils::stdoutFromCommand("echo merged > " + (fs::current_path() / "ab_1").string());
    Utils::stdoutFromCommand("rm -rf " + (fs::current_path() / "ab_1.THEIRS").string());

    // merge commit
    a.execute();

    // check that merge commit has relations to both branches
    Commit mergeCommit = Commit::load(getHead());
    EXPECT_EQ(mergeCommit.getParent(), b_6);
    EXPECT_EQ(mergeCommit.getRelative(), a_4);

    // check that merge result was commited
    content = fileContent((fs::current_path() / "ab_1").string());
    EXPECT_EQ(content, "merged");
}
