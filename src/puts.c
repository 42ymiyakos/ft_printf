/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   puts.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiyakos <ymiyakos@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 13:31:08 by ymiyakos          #+#    #+#             */
/*   Updated: 2026/10/27 11:20:53 by ymiyakos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <unistd.h>
#include <stdint.h>

int	ft_putchar(char c)
{
	return (write(1, &c, 1));
}

int	ft_putstr(const char *s)
{
	size_t		remaining;
	size_t		max_write_count;
	long long	written_total;
	size_t		min;
	int			result;

	if (!s)
		return (write(1, "(null)", 6));
	remaining = ft_strlen(s);
	if (remaining > INT_MAX)
		return (-1);
	max_write_count = INT_MAX - 4095;
	written_total = 0;
	while (remaining > 0)
	{
		min = remaining;
		if (remaining > max_write_count)
			min = max_write_count;
		result = write(1, s + written_total, min);
		if (result < 0)
			return (-1);
		written_total += result;
		remaining -= result;
	}
	return (written_total);
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

int	ft_putptr(va_list *ap)
{
	uintptr_t	ptr;
	int			total;
	int			result;

	ptr = (uintptr_t)va_arg(*ap, void *);
	if (!ptr)
		return (ft_putstr("(nil)"));
	total = 0;
	result = ft_putstr("0x");
	if (result < 0)
		return (-1);
	total += result;
	result = ft_putunbr(ptr, "0123456789abcdef");
	if (result < 0)
		return (-1);
	total += result;
	return (total);
}
