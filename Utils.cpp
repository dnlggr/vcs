//
// Created by Daniel on 23.10.17.
//

#include "Utils.h"

// inspired by https://www.jeremymorgan.com/tutorials/c-programming/how-to-capture-the-output-of-a-linux-command-in-c/
string Utils::stdoutFromCommand(string command) {
    string out;
    char buffer[256];

    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe) {
        // todo: cerr instead of throw?
        throw runtime_error("popen() failed");
    }
    try {
        while(!feof(pipe)) {
            if (fgets(buffer, 256, pipe) != NULL) {
                out.append(buffer);
            }
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }

    pclose(pipe);
    return out;
}
