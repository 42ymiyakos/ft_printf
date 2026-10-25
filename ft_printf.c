/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiyakos <ymiyakos@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 11:00:22 by ymiyakos          #+#    #+#             */
/*   Updated: 2026/10/25 18:57:23 by ymiyakos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <stdint.h>

static int	put_args(char spec, va_list *ap)
{
	if (spec == 'c')
		return (ft_putchar(va_arg(*ap, int)));
	else if (spec == 's')
		return (ft_putstr(va_arg(*ap, char *)));
	else if (spec == 'p')
		return (ft_putnbr((uintptr_t)va_arg(*ap, void *)));
	else if (spec == 'd')
		return (ft_putnbr(va_arg(*ap, int)));
	else if (spec == 'i')
		return (ft_putnbr(va_arg(*ap, int)));
	else if (spec == 'u')
		return (ft_putnbr(va_arg(*ap, unsigned int)));
	else if (spec == 'x')
		return (ft_putunbr(va_arg(*ap, unsigned int), "0123456789abcdef"));
	else if (spec == 'X')
		return (ft_putunbr(va_arg(*ap, unsigned int), "0123456789ABCDEF"));
	else if (spec == '%')
		return (ft_putchar('%'));
}

int	ft_printf(const char *format, ...)
{
	int		total;
	int		result;
	va_list	ap;

	total = 0;
	result = 0;
	va_start(ap, format);
	while (*format)
	{
		if (*format != '%')
		{
			result = ft_putchar(*format);
			if (result < 0)
			{
				va_end(ap);
				return (-1);
			}
			total += result;
		}
		else if (*(format + 1))
		{
			result = put_args(*(++format), &ap);
			if (result < 0)
			{
				va_end(ap);
				return (-1);
			}
			total += result;
		}
		++format;
	}
	va_end(ap);
	return (total);
}
