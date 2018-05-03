//
// Created by Daniel on 26.10.17.
//

#include <Actions/CommitAction.h>
#include "gtest/gtest.h"

#include "../../Actions/InitAction.h"

namespace fs = boost::filesystem;

class CommitTest : public ::testing::Test {
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
        Utils::stdoutFromCommand("rm -rf " + (fs::current_path() / "bar").string());
        Utils::stdoutFromCommand("rm -rf " + (fs::current_path() / "baz").string());
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

TEST_F(CommitTest, TestInitialCommit) {
    Utils::stdoutFromCommand("touch " + (fs::current_path() / "foo").string());
    CommitAction a(Command("commit"));
    a.execute();

    fs::path path = fs::current_path() / ".vcs/changes" / getHead();

    EXPECT_EQ(s->directoryExists(path / "add"), true);
    EXPECT_EQ(s->directoryExists(path / "diff"), false);
    EXPECT_EQ(s->directoryExists(path / "del"), false);

    EXPECT_EQ(s->fileExists(path / "add" / "foo"), true);
}

TEST_F(CommitTest, TestEmptyCommit) {
    CommitAction a(Command("commit"));
    a.execute();

    int count = 0;
    for (fs::path p : boost::make_iterator_range(fs::directory_iterator(fs::current_path() / ".vcs/changes"))) {
        count++;
    }

    EXPECT_EQ(count, 1);
}

TEST_F(CommitTest, TestDeletingCommit) {
    Utils::stdoutFromCommand("touch " + (fs::current_path() / "foo").string());
    CommitAction a(Command("commit"));
    a.execute();

    Utils::stdoutFromCommand("rm " + (fs::current_path() / "foo").string());
    a.execute();

    fs::path path = fs::current_path() / ".vcs/changes" / getHead();

    EXPECT_EQ(s->fileExists(path / "del"), true);
    EXPECT_EQ(s->directoryExists(path / "add"), false);
    EXPECT_EQ(s->directoryExists(path / "diff"), false);

    EXPECT_EQ(fileContent(path / "del"), "foo");
}

TEST_F(CommitTest, TestAddingCommit) {
    CommitAction a(Command("commit"));
    a.execute();

    Utils::stdoutFromCommand("touch " + (fs::current_path() / "foo").string());
    a.execute();

    fs::path path = fs::current_path() / ".vcs/changes" / getHead();

    EXPECT_EQ(s->fileExists(path / "del"), false);
    EXPECT_EQ(s->directoryExists(path / "add"), true);
    EXPECT_EQ(s->directoryExists(path / "diff"), false);

    EXPECT_EQ(s->fileExists(path / "add" / "foo"), true);
}

TEST_F(CommitTest, TestChangingCommit) {
    Utils::stdoutFromCommand("touch " + (fs::current_path() / "foo").string());
    CommitAction a(Command("commit"));
    a.execute();

    Utils::stdoutFromCommand("echo lol > " + (fs::current_path() / "foo").string());
    a.execute();

    fs::path path = fs::current_path() / ".vcs/changes" / getHead();

    EXPECT_EQ(s->fileExists(path / "del"), false);
    EXPECT_EQ(s->directoryExists(path / "add"), false);
    EXPECT_EQ(s->directoryExists(path / "diff"), true);

    EXPECT_EQ(s->fileExists(path / "diff" / "foo"), true);
}

TEST_F(CommitTest, TestAddingDeletingChangingCommit) {
    Utils::stdoutFromCommand("touch " + (fs::current_path() / "foo").string());
    Utils::stdoutFromCommand("touch " + (fs::current_path() / "bar").string());
    CommitAction a(Command("commit"));
    a.execute();

    Utils::stdoutFromCommand("rm " + (fs::current_path() / "foo").string());            // delete
    Utils::stdoutFromCommand("echo lol > " + (fs::current_path() / "bar").string());    // change
    Utils::stdoutFromCommand("touch " + (fs::current_path() / "baz").string());         // add
    a.execute();

    fs::path path = fs::current_path() / ".vcs/changes" / getHead();

    EXPECT_EQ(s->fileExists(path / "del"), true);
    EXPECT_EQ(s->directoryExists(path / "add"), true);
    EXPECT_EQ(s->directoryExists(path / "diff"), true);

    EXPECT_EQ(fileContent(path / "del"), "foo");
    EXPECT_EQ(s->fileExists(path / "diff" / "bar"), true);
    EXPECT_EQ(s->fileExists(path / "add" / "baz"), true);
}