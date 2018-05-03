//
// Created by Daniel on 26.10.17.
//

#include <Actions/CommitAction.h>
#include "gtest/gtest.h"

#include "../../Actions/InitAction.h"

Storage *Storage::StorageInstance;

class InitTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        s = Storage::Instance();
    }

    virtual void TearDown() {
        Utils::stdoutFromCommand("rm -rf " + (fs::current_path() / ".vcs").string());
    }

    Storage *s;
};

TEST_F(InitTest, TestInit) {
    InitAction a(Command("init"));
    a.execute();
    EXPECT_EQ(s->directoryExists(fs::current_path() / ".vcs"), true);
}
