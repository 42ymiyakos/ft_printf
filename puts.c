/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   puts.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiyakos <ymiyakos@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 13:31:08 by ymiyakos          #+#    #+#             */
/*   Updated: 2026/10/25 17:18:39 by ymiyakos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <unistd.h>

int	ft_putchar(char c)
{
	return (write(1, &c, 1));
}

int	ft_putstr(const char *s)
{
	if (!s)
		return (write(1, "(null)", 6));
	return (write(1, s, ft_strlen(s)));
}

int	ft_putunbr(unsigned long unbr, const char *base)
{
	size_t	base_len;
	int		total;
	int		result;

	base_len = ft_strlen(base);
	total = 0;
	if (unbr >= base_len)
	{
		result = ft_putunbr(unbr / base_len, base);
		if (result < 0)
			return (-1);
		total += result;
	}
	result = ft_putchar(base[unbr % base_len]);
	if (result < 0)
		return (-1);
	return (total + result);
}

int	ft_putnbr(long long nbr)
{
	int		total;
	int		result;

	total = 0;
	if (nbr < 0)
	{
		result = ft_putchar('-');
		if (result < 0)
			return (-1);
		nbr *= -1;
		++total;
	}
	result = ft_putunbr(nbr, "0123456789");
	if (result < 0)
		return (-1);
	return (total + result);
}
