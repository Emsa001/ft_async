#include "ft_async.h"

// Add async task to the list
void add_to_list(t_async *async)
{
    t_async_manager *manager = async->manager;

    pthread_mutex_lock(&manager->list_mutex); // important !!!

    t_async_list *node = manager->thread_list;
    t_async_list *new_node = (t_async_list *)calloc(1, sizeof(t_async_list));

    new_node->current = async;
    new_node->next = NULL;

    if (node == NULL)
        manager->thread_list = new_node;
    else
    {
        while (node->next != NULL)
            node = node->next;
        node->next = new_node;
    }

    pthread_mutex_unlock(&manager->list_mutex);

    // I know I know.... But trust me - it must be like this
    pthread_mutex_lock(&manager->manager_mutex);
    pthread_mutex_lock(&async->async_mutex);
    async->id = manager->thread_count;
    pthread_mutex_unlock(&async->async_mutex);
    manager->thread_count++;
    pthread_mutex_unlock(&manager->manager_mutex);
}

t_async *new_async()
{
    t_async_manager *manager = get_manager();

    t_async *async = (t_async *)malloc(sizeof(t_async));
    bzero(async, sizeof(t_async));

    async->id = -1;
    async->manager = manager;

    async->start = NULL;
    async->process = NULL;
    async->end = NULL;
    async->end_main = NULL;

    async->arg = NULL;

    async->time = 10 * 1000;
    async->time_elapsed = 0;
    async->process_time = 100;
    async->stopped = false;

    return async;
}
