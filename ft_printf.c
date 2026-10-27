/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiyakos <ymiyakos@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 11:00:22 by ymiyakos          #+#    #+#             */
/*   Updated: 2026/10/27 11:07:22 by ymiyakos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	put_args(char spec, va_list *ap)
{
	if (spec == 'c')
		return (ft_putchar(va_arg(*ap, int)));
	else if (spec == 's')
		return (ft_putstr(va_arg(*ap, char *)));
	else if (spec == 'p')
		return (ft_putptr(ap));
	else if (spec == 'd')
		return (ft_putnbr(va_arg(*ap, int)));
	else if (spec == 'i')
		return (ft_putnbr(va_arg(*ap, int)));
	else if (spec == 'u')
		return (ft_putunbr(va_arg(*ap, unsigned int), "0123456789"));
	else if (spec == 'x')
		return (ft_putunbr(va_arg(*ap, unsigned int), "0123456789abcdef"));
	else if (spec == 'X')
		return (ft_putunbr(va_arg(*ap, unsigned int), "0123456789ABCDEF"));
	else if (spec == '%')
		return (ft_putchar('%'));
	return (-1);
}

static int	read_format(const char **format, va_list *ap)
{
	char	c;

	c = **format;
	++(*format);
	if (c != '%')
		return (ft_putchar(c));
	if (!**format)
		return (0);
	c = **format;
	(*format)++;
	return (put_args(c, ap));
}

int	ft_printf(const char *format, ...)
{
	int		total;
	int		result;
	va_list	ap;

	total = 0;
	if (!format)
		return (-1);
	va_start(ap, format);
	while (*format)
	{
		result = read_format(&format, &ap);
		if (result < 0 || total > INT_MAX - result)
		{
			total = -1;
			break ;
		}
		total += result;
	}
	va_end(ap);
	return (total);
}
