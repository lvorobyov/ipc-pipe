//
// Copyright (c) 2021, Лев Воробьёв
//
// You may copy, modify, redistribute this file
// in terms of Boost Software License 1.0

#include <iostream>
#include <csignal>
#include <cstdlib>
#include <cassert>
#include "object_pipe.h"
using namespace ipc;

int main() {
    signal(SIGABRT, [](int) { exit(EXIT_FAILURE); });
    try {
        static event_base *e_base = event_base_new();
        signal(SIGINT, [](int sig) { event_base_loopbreak(e_base); });
        object_pipe pipe(e_base);
        assert(object_pipe::exists());
        class : public message_listener {
        public:
            void on_message(u_long id, const char *msg) override {
                printf("%lu: %s\n", id, msg);
            }
        } listener;
        pipe.add_listener(&listener);
        event_base_dispatch(e_base);
        pipe.remove_listener(&listener);
        event_base_free(e_base);
    } catch (exception const &ex) {
        cerr << ex.what() << endl;
        return EXIT_FAILURE;
    }
    assert(!object_pipe::exists());
    return EXIT_SUCCESS;
}