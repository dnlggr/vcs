//
// Created by Jannik Siebert on 19/10/2017.
//

#include "Config.h"

const fs::path Config::VCS_STORAGE_DIRECTORY = ".vcs";

const fs::path Config::VCS_CHANGE_DIRECTORY = "changes";
const fs::path Config::VCS_COMMIT_DIRECTORY = "commits";

const fs::path Config::VCS_TEMPORARY_DIRECTORY = "tmp";
const fs::path Config::VCS_REPLAY_DIRECTORY = "replay";

const fs::path Config::VCS_ADDITIONS_DIRECTORY = "add";
const fs::path Config::VCS_DIFFERENCES_DIRECTORY = "diff";
const fs::path Config::VCS_DELETIONS_FILE = "del";

const fs::path Config::VCS_HEAD_FILE = "HEAD";
const fs::path Config::VCS_AUTHOR_FILE = "author";

const fs::path Config::VCS_MERGE_DIRECTORY_OURS = "ours";
const fs::path Config::VCS_MERGE_DIRECTORY_THEIRS = "theirs";
const fs::path Config::VCS_MERGE_FILE = "MERGE";

const string Config::VCS_SINGLE_SEPARATOR = "─────────────────────────────────────────────";
const string Config::VCS_DOUBLE_SEPARATOR = "═════════════════════════════════════════════";

const bool Config::VCS_LOGGING_VERBOSE = false;
