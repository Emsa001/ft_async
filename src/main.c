#include "ft_async.h"

void start(t_async *async) {
    printf("start thread %d\n", async->id);
}

void end(t_async *async) {
    printf("end thread: %d\n", async->id);
}

void process(t_async *async) {
    int time_left = async->time - async->time_elapsed;
    printf("process thread %d: %d\n", async->id, time_left);
}

void main_thread_loop()
{
    int i = 0;
    while (1) {
        if (i == 5)
            break;

        sleep(1);
        i++;
    }
}

int main()
{
    t_async *async = new_async();
    async->time = -1; // Infinite
    async->start = &start;
    async->process = &process;
    async->end = &end;
    start_async(async);

    t_async *async2 = new_async();
    async2->time = 10000;
    async2->start = &start;
    async2->process = &process;
    async2->process_time = 10;
    async2->end = &end;
    start_async(async2);

    t_async *async4 = new_async();
    async4->time = 1000;
    async4->start = &start;
    async4->process = &process;
    async4->end = &end;
    start_async(async4);

    main_thread_loop();

    printf("Main loop exited. Program terminated.\n");
    destroy_manager();
    return 0;
}