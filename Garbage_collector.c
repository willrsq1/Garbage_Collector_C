/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wruet-su <william.ruetsuquet@gmail.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/30 14:55:37 by wruet-su          #+#    #+#             */
/*   Updated: 2023/04/30 16:36:03 by wruet-su         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_to_be_freed		t_to_be_freed;
typedef	struct s_Garbage_collector	t_Garbage_collector;

void	*ft_calloc(size_t nmemb, t_Garbage_collector *Garbage);
void	ft_free_everything_lol(t_Garbage_collector *Garbage); //frees all
int		ft_free_this_data(t_Garbage_collector *Garbage, void *ptr); //frees one data

static void	ft_lst_malloc(t_Garbage_collector *Garbage, void *ptr);
static void	ft_bzero(void *s, size_t n);

typedef	struct s_Garbage_collector
{
	t_to_be_freed	*first_lst;
}		t_Garbage_collector;

typedef struct s_to_be_freed
{
	void			*data;
	t_to_be_freed	*next;
}		t_to_be_freed;

void	ft_free_everything_lol(t_Garbage_collector *Garbage)
{
	t_to_be_freed	*lst;
	t_to_be_freed	*temp;

	lst = Garbage->first_lst;
	if (!lst)
		return ;
	while (lst)
	{
		temp = lst->next;
		if (lst->data)
			free (lst->data);
		lst->data = NULL;
		free (lst);
		lst = temp;
	}
	Garbage->first_lst = NULL;
}

void	*ft_calloc(size_t nmemb, t_Garbage_collector *Garbage)
{
	void	*ptr;

	ptr = NULL;
	ptr = malloc(nmemb);
	if (!ptr)
	{
		write(2, "Malloc failed !!\n", 18);
		ft_free_everything_lol(Garbage);
		exit (1);
	}
	ft_lst_malloc(Garbage, ptr);
	ft_bzero(ptr, nmemb);
	return (ptr);
}

static void	ft_lst_malloc(t_Garbage_collector *Garbage, void *ptr)
{
	t_to_be_freed	*to_be_freed_lst;
	t_to_be_freed	*temp;

	to_be_freed_lst = malloc(sizeof(t_to_be_freed));
	if (!to_be_freed_lst)
	{
		write(2, "Eroor calloc\n", 14);
		ft_free_everything_lol(Garbage);
		exit (1);
	}
	to_be_freed_lst->data = NULL;
	to_be_freed_lst->data = ptr;
	to_be_freed_lst->next = NULL;
	if (Garbage->first_lst == NULL)
	{
		Garbage->first_lst = to_be_freed_lst;
		return ;
	}
	temp = Garbage->first_lst;
	while (temp && temp->next)
		temp = temp->next;
	temp->next = to_be_freed_lst;
}

static void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*str;

	str = (char *)s;
	i = 0;
	while (i < n)
	{
		str[i] = 0;
		i++;
	}
}

int	ft_free_this_data(t_Garbage_collector *Garbage, void *ptr)
{
	t_to_be_freed	*lst;
	t_to_be_freed	*temp;

	lst = Garbage->first_lst;
	if (!lst || !ptr)
		return (-1);
	while (lst && lst->next && lst->next->data != ptr)
		lst = lst->next;
	temp = lst;
	if (lst->data != ptr) //if first lst has the malloced address
		lst = lst->next;
	if (!lst)
		return (-1); //invalid pointer
	free (lst->data);
	temp->next = lst->next;
	if (lst == temp)
		Garbage->first_lst = lst->next;
	free(lst);
	return (0);
}

/* Run with leaks detector ! */

int	main()
{
	int					i;
	int					y;
	char				***example;
	t_Garbage_collector	Garbage;

	i = -1;
	Garbage.first_lst = NULL;
	example = ft_calloc(sizeof(char **) * 17072000, &Garbage);
	while (++i < 100)
	{
		example[i] = ft_calloc(sizeof(char *) * 100, &Garbage);
		y = -1;
		while (++y < 100)
			example[i][y] = ft_calloc(sizeof(char) * 10, &Garbage);
	}
	ft_free_this_data(&Garbage, example[3]); //you can free a data when it's not needed anymore
	ft_free_this_data(&Garbage, example[17][07]);
	ft_free_this_data(&Garbage, example[13][06]);
	ft_free_this_data(&Garbage, NULL); //crash test - no pointer
	ft_free_this_data(&Garbage, &Garbage); //crash test - pointer that isn't a malloc
	ft_free_everything_lol(&Garbage);
	example = ft_calloc(39012, &Garbage);
	ft_free_this_data(&Garbage, example);
	ft_free_everything_lol(&Garbage); //calling when there is nothing to free won't crash
	ft_free_everything_lol(&Garbage); //repeated calls won't crash either
	write(1, "Everything has been freed.\n", 28);
	return (0);
}