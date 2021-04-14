//
// Copyright (c) 2021, Лев Воробьёв
//
// You may copy, modify, redistribute this file
// in terms of Boost Software License 1.0

#include "stat.h"
#include <system_error>
using namespace std;

bool ipc::stat(const char *node, mode_t mode) {
    struct stat st{};
    return lstat(node, &st) == 0 && (st.st_mode & S_IFMT) == mode;
}

void ipc::raise_system_error(const char *msg) {
    throw system_error(errno, system_category(), msg);
}
