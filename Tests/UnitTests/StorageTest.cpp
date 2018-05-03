#include "gtest/gtest.h"
#include "../../Config.h"
#include "../../Utils.h"

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

Storage *Storage::StorageInstance;

class StorageTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        s = Storage::Instance();
        p1 = fs::current_path() / "StorageTestDirectory1";
        p2 = fs::current_path() / "StorageTestDirectory2";
        f1 = fs::current_path() / "StorageTestFile1";
        f2 = fs::current_path() / "StorageTestFile2";

        Utils::stdoutFromCommand("touch " + f1.string());
        Utils::stdoutFromCommand("touch " + f2.string());
    }
    virtual void TearDown() {
        Utils::stdoutFromCommand("rm -rf " + p1.string());
        Utils::stdoutFromCommand("rm -rf " + p2.string());
        Utils::stdoutFromCommand("rm -rf " + f1.string());
        Utils::stdoutFromCommand("rm -rf " + f2.string());
    }

    Storage *s;
    fs::path p1;
    fs::path p2;
    fs::path f1;
    fs::path f2;
};

TEST_F(StorageTest, TestThatExistingDirectoryIsFound) {
    bool exists = s->directoryExists("/");
    EXPECT_EQ(exists, true);
}

TEST_F(StorageTest, TestThatNotExistingDirectoryIsNotFound) {
    bool exists = s->directoryExists("/foobar");
    EXPECT_EQ(exists, false);
}

TEST_F(StorageTest, TestCreatingADirectory) {
    s->createDirectory(p1);
    bool exists = s->directoryExists(p1);
    EXPECT_EQ(exists, true);
}

TEST_F(StorageTest, TestRemovingADirectory) {
    s->createDirectory(p1);
    EXPECT_EQ(s->directoryExists(p1), true);
    s->remove(p1);
    EXPECT_EQ(s->directoryExists(p1), false);
}

TEST_F(StorageTest, TestCopyNotExistingDirectory) {
    bool ret = s->copy(p1, p2);
    EXPECT_EQ(ret, false);
}

TEST_F(StorageTest, TestCopyExistingDirectory) {
    s->createDirectory(p1);
    s->copy(p1, p2);
    EXPECT_EQ(s->directoryExists(p2), true);
}

TEST_F(StorageTest, TestCopyFile) {
    s->createDirectory(p1);
    s->copy(f1, p1);
    EXPECT_EQ(s->fileExists(p1 / "StorageTestFile1"), true);
}

TEST_F(StorageTest, TestRecursiveCopyDirectory) {
    s->createDirectory(p1);
    s->createDirectory(p1 / "foo");
    s->createDirectory(p1 / "foo" / "bar");
    s->copy(p1, p2);
    EXPECT_EQ(s->directoryExists(p2), true);
}

TEST_F(StorageTest, TestRecursiveCopyDirectoryWithContent) {
    s->createDirectory(p1);
    s->createDirectory(p1 / "foo");
    s->createDirectory(p1 / "foo" / "bar");
    Utils::stdoutFromCommand("mv " + f1.string() + " " + (p1 / "foo" / "bar").string());
    s->copy(p1, p2);
    EXPECT_EQ(s->fileExists(p2 / "foo" / "bar" / "StorageTestFile1"), true);
}

TEST_F(StorageTest, TestMoveDirectoryThatDoesNotExist) {
    EXPECT_EQ(s->moveDirectory(p1, p2), false);
}

TEST_F(StorageTest, TestMoveDirectoryReplacingExistingDirectory) {
    s->createDirectory(p1);
    Utils::stdoutFromCommand("mv " + f1.string() + " " + p1.string());
    s->createDirectory(p2);
    s->moveDirectory(p1, p2);
    EXPECT_EQ(s->fileExists(p2 / "StorageTestFile1"), true);
}