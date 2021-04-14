//
// Copyright (c) 2021, Лев Воробьёв
//
// You may copy, modify, redistribute this file
// in terms of Boost Software License 1.0

#ifndef IPC_PIPE_LISTENER_H
#define IPC_PIPE_LISTENER_H

#include "object_pipe.h"

namespace ipc {

    class pipe_listener : public message_listener {
        object_pipe _pipe;
    public:
        explicit pipe_listener(event_base *e_base);

        ~pipe_listener() override;

        pipe_listener(pipe_listener const&) = delete;
        pipe_listener(pipe_listener &&) = delete;

        pipe_listener &operator=(pipe_listener const&) = delete;
        pipe_listener &operator=(pipe_listener &&) = delete;

    private:
        void on_message(u_long id, const char *msg) override;
    };

}

#endif //IPC_PIPE_LISTENER_H
