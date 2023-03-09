#pragma once

#include <queue>
#include <rack.hpp>
#include <thread>

using namespace rack;

/**
 * Notifier is a service that runs a persistent background thread with a locked queue for notifications.
 *
 * As notificaitons are added to the queue, they are popped off and placed on a stack.
 *
 * As notifications reach zero opacity, they are removed from the stack.
 *
 * All notifications are rendered.
 *
 * Assume we have a Notification widget.
 *
 */

struct Notification {};

struct Notifier {
    std::thread thread;
    std::mutex mutex;

    std::queue<Notification*> queue;
    std::vector<Notification*> stack;
};