//
// Copyright (c) 2021, Лев Воробьёв
//
// You may copy, modify, redistribute this file
// in terms of Boost Software License 1.0

#include <iostream>
#include "pipe_listener.h"

using namespace ipc;

pipe_listener::pipe_listener(event_base *e_base) : _pipe(e_base) {
    // создать канал для переключения пулов
    _pipe.add_listener(this);
}

pipe_listener::~pipe_listener() {
    // отключить именованный канал
    _pipe.remove_listener(this);
}

void pipe_listener::on_message(u_long id, const char *msg) {
    cout << id << ": " << msg << endl;
}
