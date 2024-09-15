#include "../includes/ft_async.h"

void remove_from_list(t_async *async)
{
    t_async_manager *manager = async->manager;

    pthread_mutex_lock(&manager->list_mutex);

    t_async_list *node = manager->thread_list;
    t_async_list *prev_node = NULL;

    while (node != NULL)
    {
        if (node->current->id == async->id)
        {
            if (prev_node == NULL)
                manager->thread_list = node->next;
            else
                prev_node->next = node->next;
            
            free(node);
            break;
        }

        prev_node = node;
        node = node->next;
    }

    pthread_mutex_unlock(&manager->list_mutex);
    pthread_mutex_destroy(&async->async_mutex);
    free(async);
}

void stop_all_async_tasks(void)
{
    t_async_manager *manager = get_manager();

    pthread_mutex_lock(&manager->list_mutex);

    t_async_list *node = manager->thread_list;

    while (node != NULL)
    {
        pthread_mutex_lock(&node->current->async_mutex);
        node->current->stopped = true;
        pthread_mutex_unlock(&node->current->async_mutex);
        node = node->next;
    }
    
    pthread_mutex_unlock(&manager->list_mutex);

    // Wait for all threads to figure out that they should stop
    while (1)
    {
        pthread_mutex_lock(&manager->list_mutex);
        if (manager->thread_list == NULL)
        {
            pthread_mutex_unlock(&manager->list_mutex);
            break;
        }
        pthread_mutex_unlock(&manager->list_mutex);
        usleep(1000);
    }

    printf("All async tasks stopped\n");
}