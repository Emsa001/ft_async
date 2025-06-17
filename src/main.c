#include "ft_async.h"

// This function is called when the async task starts running in its thread.
void start(t_async *async) {
    printf("Started async task %d\n", async->id);
}

// This function is called periodically while the async task is running.
// The interval is set by async->process_time.
void process(t_async *async) {
    int time_left = async->time - async->time_elapsed;
    printf("Processing async task %d: %d\n", async->id, time_left);
}

// This function is called when the async task finishes (after async->time ms).
void end(t_async *async) {
    printf("Ended async task %d\n", async->id);
}

// This function is called from the main thread after the async task ends.
void end_main(void *arg) {
    printf("This runs in the main thread after async ends!\n");
}

void ft_wait_end(void *arg){
    printf("I am executed in main thread after 2 seconds!\n");
}

int main() {
    // Create a new async task structure
    t_async *async = new_async();
    // Set the total duration for the async task (in milliseconds)
    async->time = 2000; // Task duration in ms
    // Assign the function to call when the task starts
    async->start = &start;
    // Assign the function to call periodically while running
    async->process = &process;
    // Set the interval between process calls (in milliseconds)
    async->process_time = 100; // Process interval in ms
    // Assign the function to call when the task ends
    async->end = &end;
    // Assign the function to call in the main thread after the task ends (requires async_queue() to be called)
    async->end_main = &end_main;
    // Start the async task (runs in a separate thread)
    start_async(async);

    // Create a new async task, which will run ft_wait_end after 2 seconds in main thread without blocking it. (requires async_queue() to be called regularly)
    ft_wait(2000, &ft_wait_end);

    // Main thread loop: regularly call async_queue() to process main-thread callbacks
    for (int i = 0; i < 60; ++i) {
        async_queue();
        usleep(100000); // Sleep for 100ms
    }
    // Clean up all async resources and threads
    destroy_manager();
    return 0;
}

/*
============================
Explanation: Async Task, ft_wait, and Program Duration
============================

This code demonstrates two main features of ft_async:

1. Async Task Lifecycle
----------------------
- An async task is created and started in a separate thread.
- When the task starts, the `start` function is called (prints a message showing how long the thread will run).
- While the task is running, the `process` function is called every `process_time` milliseconds (here, every 100ms).
- The task runs for `time` milliseconds (here, 2000ms = 2 seconds).
- After 2 seconds, the `end` function is called in the async thread.
- When the async task ends, the `end_main` function is scheduled to run in the main thread (prints a message). This requires the main thread to call `async_queue()` regularly.
- After the main loop, `destroy_manager()` cleans up all async resources and threads.

Summary of async task:
- Lifetime: 2 seconds (2000ms)
- Calls `start` once at the beginning (in async thread)
- Calls `process` every 100ms (in async thread)
- Calls `end` once at the end (in async thread)
- Calls `end_main` once after ending (in main thread, via `async_queue()`)

2. ft_wait Function
-------------------
- `ft_wait(ms, func)` schedules a function (`func`) to be executed in the main thread after `ms` milliseconds.
- In this example, `ft_wait(2000, &ft_wait_end);` will call `ft_wait_end` in the main thread after 2 seconds.
- Like `end_main`, this also requires the main thread to call `async_queue()` regularly to process the callback.
- This is useful for scheduling delayed actions in the main thread without blocking it.

Key Point:
- Both async task callbacks (like `end_main`) and `ft_wait` scheduled functions are executed in the main thread, but only when `async_queue()` is called (typically in your main loop).


3. How long does the whole program run?
---------------------------------------
- The main loop runs 60 times, sleeping 100ms each iteration: 60 x 100ms = 6000ms = 6 seconds.
- Therefore, the entire program runs for about 6 seconds before exiting.
- Both the async task and the ft_wait callback are completed within the first 2 seconds, but the program continues running the main loop for the full 6 seconds.

*/