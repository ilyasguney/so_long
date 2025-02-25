/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iguney <iguney@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 22:41:10 by iguney            #+#    #+#             */
/*   Updated: 2025/02/23 01:57:36 by iguney           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	map_read(t_data *data)
{
	int	fd;
	int	i;

	i = -1;
	data->vertical = 0;
	data->horizontal = 0;
	calculate_gnl(data);
	fd = open(data->path, O_RDONLY);
	if (fd == -1)
		return (error("File does not open!", data));
	if (data->vertical != 0)
	{
		data->map = malloc(sizeof(char *) * data->vertical);
		if (!data->map)
			return (close(fd), error("Allocation Failed!", data));
		while (++i < data->vertical)
		{
			data->map[i] = get_next_line(fd);
			if (!data->map)
				return (close(fd), error("Get Line Failed!", data));
		}
		data->horizontal = s_strlen(data->map[0]);
	}
	close(fd);
}

void	map_includes(t_data *data)
{
	int		i;
	int		j;
	char	*keys;

	i = 0;
	j = 0;
	keys = "10PCE";
	if (data->vertical <= 0)
		return (error("The map is empty!", data));
	while (i < data->vertical)
	{
		j = 0;
		while (j < data->horizontal)
		{
			if (!s_strchr(keys, data->map[i][j]))
				return (map_error("Invalid map input!", data));
			j++;
		}
		i++;
	}
}

void	map_valid(t_data *data)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (j < data->horizontal)
	{
		if (data->map[0][j] != '1' || data->map[data->vertical - 1][j] != '1')
			return (map_error("The map conditions must be closed!", data));
		j++;
	}
	while (i < data->vertical)
	{
		if (data->map[i][0] != '1' || data->map[i][data->horizontal - 1] != '1')
			return (map_error("The map conditions must be closed!", data));
		else if (data->horizontal != s_strlen(data->map[i]))
			return (map_error("All map lines must be same!", data));
		i++;
	}
	data->player = malloc(sizeof(t_data));
	if (!data->player)
		return (map_error("Allocation Failed!", data));
}

void	map_requirements(t_data *data)
{
	data->check_p = 0;
	data->check_c = 0;
	data->check_e = 0;
	data->i = -1;
	while (++data->i < data->vertical)
	{
		data->j = -1;
		while (++data->j < data->horizontal)
		{
			if (data->map[data->i][data->j] == 'P')
			{
				data->check_p++;
				data->player->x = data->j;
				data->player->y = data->i;
			}
			if (data->map[data->i][data->j] == 'C')
				data->check_c++;
			if (data->map[data->i][data->j] == 'E')
				data->check_e++;
		}
	}
	if (data->check_p != 1 || data->check_c == 0 || data->check_e != 1)
		return (player_error(("Invalid character input!"), data));
}

void	map_copy(t_data *data)
{
	int	fd;
	int	i;

	i = 0;
	fd = open(data->path, O_RDONLY);
	if (fd == -1)
		return (player_error("File does not open!", data));
	data->map_reachable = malloc(sizeof(char *) * data->vertical);
	if (!data->map_reachable)
		return (close(fd), reachable_error("Allocation Failed!", data));
	while (i < data->vertical)
	{
		data->map_reachable[i] = get_next_line(fd);
		if (!data->map_reachable)
			return (close(fd), reachable_error("Get Line Failed!", data));
		i++;
	}
}
