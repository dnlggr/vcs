//
// Created by Daniel on 26.10.17.
//

#include "gtest/gtest.h"
#include "../../Utils.h"

TEST(ShellTest, testCommandExecutionAndReturnValue) {
    string ret = Utils::stdoutFromCommand("pwd");
    string pwd = fs::current_path().string() + "\n";

    EXPECT_EQ(ret, pwd);
}