//
// Copyright (c) 2021, Лев Воробьёв
//
// You may copy, modify, redistribute this file
// in terms of Boost Software License 1.0

#include <iostream>
#include <memory>
#include <csignal>
#include "pipe_listener.h"
using namespace ipc;

using event_t = unique_ptr<event, decltype(&event_free)>;

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        unique_ptr<event_base, decltype(&event_base_free)> e_base (event_base_new(), &event_base_free);
        pipe_listener listener(e_base.get());
        event_t e_sigint (evsignal_new(e_base.get(), SIGINT, [](evutil_socket_t, short, void *arg){
            auto sig = (event*) arg;
            auto base = event_get_base(sig);
            event_del(sig);
            event_base_loopexit(base, nullptr);
            }, event_self_cbarg()), &event_free);
        event_base_dispatch(e_base.get());
    } else {
        for (int i = 1; i < argc; ++i) {
            object_pipe::send_message(i,argv[i]);
        }
    }
    return EXIT_SUCCESS;
}