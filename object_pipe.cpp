//
// Copyright (c) 2021, Лев Воробьёв
//
// You may copy, modify, redistribute this file
// in terms of Boost Software License 1.0

#include <functional>
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include "object_pipe.h"
#include "stat.h"

using namespace ipc;

const char *object_pipe::fifo = "/tmp/ipc-pipe.fifo";

bool object_pipe::stat(mode_t mode) {
    return ipc::stat(fifo, mode);
}

bool object_pipe::is_file() {
    return stat(S_IFREG);
}

bool object_pipe::exists() {
    return stat(S_IFIFO);
}

void object_pipe::create(mode_t mode) {
    if (is_file())
        throw logic_error("it is regular file");
    unlink(fifo);
    if (mkfifo(fifo, mode) == -1)
        raise_system_error("mkfifo");
}

event *object_pipe::read_pipe_event(event_base *base) {
    if (!exists())
        create(0622);
    evutil_socket_t sock = open(fifo, O_RDONLY | O_NONBLOCK, 0);
    if (sock == -1)
        raise_system_error("open");
    return event_new(base, sock, EV_READ | EV_PERSIST, on_read, this);
}

object_pipe::object_pipe(event_base *base) : e_base(base) {
    e_read = read_pipe_event(base);
    event_add(e_read, nullptr);
}

typedef struct {
    u_long id;
    char msg[8];
} message_t;

void object_pipe::on_read(evutil_socket_t sock, short flags, void *arg) {
    static_cast<object_pipe *>(arg)->on_read(sock);
}

void object_pipe::on_read(evutil_socket_t sock) {
    message_t msg;
    ssize_t len;
    if ((len = read(sock, &msg, sizeof(msg))) <= 0) {
        if (len < 0 && errno != EAGAIN) {
            raise_system_error("read");
        }
        ::close(sock);
        clear_events();
        e_read = read_pipe_event(e_base);
        event_add(e_read, nullptr);
    } else {
        on_message(msg.id, msg.msg);
    }
}

void object_pipe::send_message(u_long id, const char *msg) {
    if (!exists())
        create(0644);
    evutil_socket_t sock = open(fifo, O_WRONLY, 0);
    message_t message{id};
    memset(message.msg, 0, 8);
    strncpy(message.msg, msg, 7);
    if (write(sock, &message, sizeof(message)) < 0)
        raise_system_error("write");
    ::close(sock);
}

object_pipe::~object_pipe() {
    clear_events();
    unlink(fifo);
}

void object_pipe::clear_events() const {
    event_del(e_read);
    event_free(e_read);
}

void object_pipe::add_listener(message_listener *listener) {
    _listeners.push_back(listener);
}

void object_pipe::remove_listener(message_listener *listener) {
    _listeners.remove(listener);
}

void object_pipe::on_message(u_long id, const char *msg) {
    for_each(_listeners.begin(), _listeners.end(), [id, msg](auto && a) { a->on_message(id, msg); });
}

