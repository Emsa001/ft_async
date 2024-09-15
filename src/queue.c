/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: escura <escura@student.42wolfsburg.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/15 12:49:01 by escura            #+#    #+#             */
/*   Updated: 2024/09/15 14:19:45 by escura           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_async.h"

void add_to_functions_queue(t_async *async)
{
    t_async_manager *manager = get_manager();

    pthread_mutex_lock(&manager->functions_queue_mutex);

    t_function *new = (t_function *)calloc(1, sizeof(t_function));
    t_function *tmp = manager->functions_queue;

    new->func = async->end_main;
    new->arg = async->arg;
    new->next = NULL;

    if (tmp == NULL)
    {
        manager->functions_queue = new;
        pthread_mutex_unlock(&manager->functions_queue_mutex);
        return;
    }

    while (tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = new;

    pthread_mutex_unlock(&manager->functions_queue_mutex);
}

void async_queue()
{
    t_async_manager *manager = get_manager();

    pthread_mutex_lock(&manager->functions_queue_mutex);

    t_function *tmp = manager->functions_queue;
    t_function *next = NULL;

    while (tmp != NULL)
    {
        next = tmp->next;
        tmp->func(tmp->arg);
        free(tmp);
        tmp = next;
    }
    manager->functions_queue = NULL;

    pthread_mutex_unlock(&manager->functions_queue_mutex);
}