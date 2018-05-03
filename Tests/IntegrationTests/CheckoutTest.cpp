//
// Created by Daniel on 26.10.17.
//

#include <Actions/CommitAction.h>
#include <Actions/CheckoutAction.h>
#include "gtest/gtest.h"

#include "../../Actions/InitAction.h"

namespace fs = boost::filesystem;

class CheckoutTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        s = Storage::Instance();
        InitAction a(Command("init"));
        a.execute();

        commits = fs::current_path() / ".vcs/**";
    }

    virtual void TearDown() {
        Utils::stdoutFromCommand("rm -rf " + (fs::current_path() / ".vcs").string());
        Utils::stdoutFromCommand("rm -rf " + (fs::current_path() / "foo").string());
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

TEST_F(CheckoutTest, TestCheckout) {
    Utils::stdoutFromCommand("echo before > " + (fs::current_path() / "foo").string());
    CommitAction a(Command("commit"));
    a.execute();
    string firstCommit = getHead();

    Utils::stdoutFromCommand("echo after > " + (fs::current_path() / "foo").string());
    a.execute();

    string content = fileContent((fs::current_path() / "foo").string());
    EXPECT_EQ(content, "after");

    vector<string> args{firstCommit};
    CheckoutAction ca((Command("checkout", args)));
    ca.execute();

    content = fileContent((fs::current_path() / "foo").string());
    EXPECT_EQ(content, "before");
}
