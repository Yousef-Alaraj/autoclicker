#include <Carbon/Carbon.h>
#include <CoreGraphics/CoreGraphics.h>
#include <chrono>
#include <iostream>
#include <thread>

std::atomic<bool> is_clicking(false);
std::atomic<bool> keep_running(true);

void clicker_loop() {
    CGEventRef event = CGEventCreate(NULL);

    if (!event) {
        std::cout << "CGEventCreate Failed\n";
        return;
    }

    CGPoint cursor = CGEventGetLocation(event);
    CGEventRef down =
        CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, cursor, kCGMouseButtonLeft);
    CGEventPost(kCGHIDEventTap, down);

    CGEventRef up = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp, cursor, kCGMouseButtonLeft);
    CGEventPost(kCGHIDEventTap, up);

    CFRelease(up);
    CFRelease(down);
    CFRelease(event);

    if (CGEventSourceKeyState(kCGEventSourceStateHIDSystemState, kVK_Escape)) {
        return;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void clicker_worker() {
    while (keep_running) {
        if (is_clicking)
            clicker_loop();
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
}

int main() {
    std::thread worker(clicker_worker);

    bool previous_toggle_pressed = false;
    bool previous_escape_pressed = false;

    while (keep_running) {
        bool current_toggle_pressed =
            CGEventSourceKeyState(kCGEventSourceStateHIDSystemState, kVK_ANSI_S);
        if (!previous_toggle_pressed && current_toggle_pressed) {
            is_clicking = is_clicking ^ 1;
        }

        bool current_escape_pressed =
            CGEventSourceKeyState(kCGEventSourceStateHIDSystemState, kVK_Escape);
        if (!previous_escape_pressed && current_escape_pressed) {
            keep_running = false;
        }

        previous_toggle_pressed = current_toggle_pressed;
        previous_escape_pressed = current_escape_pressed;

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    worker.join();

    return 0;
}
