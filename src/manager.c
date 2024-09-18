#include "ft_async.h"

t_async_manager *get_manager()
{
    static t_async_manager *manager = NULL;
    
    if (manager == NULL)
    {
        manager = (t_async_manager *)malloc(sizeof(t_async_manager));
        bzero(manager, sizeof(t_async_manager));

        manager->thread_list = NULL;
        manager->thread_count = 0;
        manager->stop_all = false;
        pthread_mutex_init(&manager->manager_mutex, NULL);
        pthread_mutex_init(&manager->list_mutex, NULL);
        pthread_mutex_init(&manager->functions_queue_mutex, NULL);
    }

    return manager;
}

void destroy_manager()
{
    t_async_manager *manager = get_manager();

    stop_all_async_tasks();

    pthread_mutex_destroy(&manager->manager_mutex);
    pthread_mutex_destroy(&manager->list_mutex);

    free(manager);
    manager = NULL;
}
