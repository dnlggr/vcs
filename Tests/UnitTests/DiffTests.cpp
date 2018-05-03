#include "../../VersionControl/Diff.h"
#include "gtest/gtest.h"
#include "../../Config.h"
#include "../../Utils.h"

namespace fs = boost::filesystem;

class DiffTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        s = Storage::Instance();
        A = fs::current_path() / "A";
        B = fs::current_path() / "B";

        Utils::stdoutFromCommand("mkdir " + A.string());
        Utils::stdoutFromCommand("mkdir " + B.string());

        Utils::stdoutFromCommand("touch " + (A / "only_A_1").string());
        Utils::stdoutFromCommand("touch " + (A / "only_A_2").string());
        Utils::stdoutFromCommand("touch " + (B / "only_B").string());
        Utils::stdoutFromCommand("touch " + (A / "both_same").string());
        Utils::stdoutFromCommand("touch " + (B / "both_same").string());
        Utils::stdoutFromCommand("touch " + (A / "both_diff").string());
        Utils::stdoutFromCommand("touch " + (B / "both_diff").string());
        Utils::stdoutFromCommand("echo foo > " + (B / "both_diff").string());
    }
    virtual void TearDown() {
        Utils::stdoutFromCommand("rm -rf " + A.string());
        Utils::stdoutFromCommand("rm -rf " + B.string());
    }

    Storage *s;
    fs::path A;
    fs::path B;
};

TEST_F(DiffTest, TestAdditions) {
    Diff d = Diff(A, B);

    EXPECT_EQ(d.getAdditions().size(), 1);
}

TEST_F(DiffTest, TestDeletions) {
    Diff d = Diff(A, B);

    EXPECT_EQ(d.getDeletions().size(), 2);
}

TEST_F(DiffTest, TestDiffs) {
    Diff d = Diff(A, B);

    EXPECT_EQ(d.getDifferences().size(), 1);
}

TEST_F(DiffTest, TestOnSameDirectory) {
    Diff d = Diff(A, A);

    EXPECT_EQ(d.getAdditions().size(), 0);
    EXPECT_EQ(d.getDeletions().size(), 0);
    EXPECT_EQ(d.getDifferences().size(), 0);
}

TEST_F(DiffTest, TestOnNotExistingDirectory) {
    Diff d = Diff(A, (B / "not_here"));

    EXPECT_EQ(d.getDifferences().size(), 0);
    EXPECT_EQ(d.getDeletions().size(), 0);
    EXPECT_EQ(d.getAdditions().size(), 0);
}