#include <Carbon/Carbon.h>
#include <CoreGraphics/CoreGraphics.h>
#include <chrono>
#include <iostream>
#include <thread>

int main() {

    for (int i = 0; i < 1e9; i++) {
        CGEventRef event = CGEventCreate(NULL);

        if (!event) {
            std::cout << "CGEventCreate Failed\n";
            return 0;
        }
        CGPoint cursor = CGEventGetLocation(event);
        CGEventRef down =
            CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, cursor, kCGMouseButtonLeft);
        CGEventPost(kCGHIDEventTap, down);

        CGEventRef up =
            CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp, cursor, kCGMouseButtonLeft);
        CGEventPost(kCGHIDEventTap, up);

        CFRelease(up);
        CFRelease(down);
        CFRelease(event);

        if (CGEventSourceKeyState(kCGEventSourceStateHIDSystemState, kVK_Escape)) {
            return 0;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}
