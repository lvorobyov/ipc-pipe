//
// Copyright (c) 2021, Лев Воробьёв
//
// You may copy, modify, redistribute this file
// in terms of Boost Software License 1.0

#ifndef IPC_STAT_H
#define IPC_STAT_H

#include <sys/stat.h>

namespace ipc {
    bool stat(const char *node, mode_t mode);

    void raise_system_error(const char *msg);
}

#endif //IPC_STAT_H
