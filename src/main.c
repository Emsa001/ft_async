#include "ft_async.h"

int value = 0;

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

void end_main(void *arg) {
    printf("This was executed from main thread!\n");
}

void ft_wait_end(void *arg) {
    printf("Executed with ft_wait! - Wow this is so easy!\n");
    value++;
}

void main_thread_loop()
{
    int i = 0;
    while (1) {
        async_queue();
        printf("Main loop: %d\n", i);
        value++;

        if (i == 3)
            return ;

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
    async2->time = 3000;
    async2->end_main = &end_main;
    start_async(async2);

    t_async *async4 = new_async();
    async4->time = 1000;
    async4->start = &start;
    async4->process = &process;
    async4->end = &end;
    async4->end_main = &end_main;
    start_async(async4);

    ft_wait(1000, &ft_wait_end);

    main_thread_loop();

    printf("Main loop exited. Program terminated.\n");
    destroy_manager();
    return 0;
}