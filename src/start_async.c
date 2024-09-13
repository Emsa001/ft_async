#include "ft_async.h"

// Function to run async process function
static void run_async_loop(t_async *current, t_async_manager *manager)
{
    while (1)
    {
        pthread_mutex_lock(&current->async_mutex);
        if (current->stopped || (current->time_elapsed >= current->time && current->time != -1))
        {
            pthread_mutex_unlock(&current->async_mutex);
            break;
        }
        pthread_mutex_unlock(&current->async_mutex);

        usleep(current->process_time * 1000);

        if (current->process)
            current->process(current);
        
        // pthread_mutex_lock(&current->async_mutex);
        if(current->time != -1)
            current->time_elapsed += current->process_time;
        // pthread_mutex_unlock(&current->async_mutex);

    }
}

static void *run_async(void *arg)
{
    t_async *current = (t_async *)arg;
    if (current->start)
        current->start(current);

    run_async_loop(current, current->manager);

    if (current->end)
        current->end(current);

    remove_from_list(current);
    return NULL;
}

// Add async task to the list
void start_async(t_async *async)
{
    pthread_t thread_id;

    pthread_mutex_init(&async->async_mutex, NULL);

    add_to_list(async);

    pthread_mutex_lock(&async->async_mutex);
    async->thread_id = thread_id;
    pthread_mutex_unlock(&async->async_mutex);

    pthread_create(&thread_id, NULL, run_async, async);
	pthread_detach(thread_id); // VERY IMPORTANT
}
