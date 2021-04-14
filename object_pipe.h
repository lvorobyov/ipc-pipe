//
// Copyright (c) 2021, Лев Воробьёв
//
// You may copy, modify, redistribute this file
// in terms of Boost Software License 1.0

#ifndef IPC_SWITCHING_PIPE_H
#define IPC_SWITCHING_PIPE_H

#include <event2/event.h>
#include <list>

namespace ipc {
    using namespace std;

    class message_listener {
    public:
        virtual ~message_listener() = default;
        virtual void on_message(u_long id, const char *msg) = 0;
    };

    class object_pipe {
    private:
        static const char *fifo;
        event_base *e_base;
        event *e_read;
        list<message_listener *> _listeners;

        static bool stat(mode_t mode);

        static bool is_file();

        static void create(mode_t mode);

        event *read_pipe_event(event_base *base);

    public:
        explicit object_pipe(event_base *base);

        virtual ~object_pipe();

        object_pipe(object_pipe const&) = delete;
        object_pipe(object_pipe &&) = delete;

        object_pipe &operator=(object_pipe const&) = delete;
        object_pipe &operator=(object_pipe &&) = delete;

        static bool exists();

        void add_listener(message_listener *listener);

        void remove_listener(message_listener *listener);

        static void send_message(u_long id, const char *msg);

    private:
        static void on_read(evutil_socket_t sock, short flags, void *arg);

        void on_read(evutil_socket_t sock);

        void on_message(u_long a, const char *msg);

        void clear_events() const;
    };

}

#endif //IPC_SWITCHING_PIPE_H
