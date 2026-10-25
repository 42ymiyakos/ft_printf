/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymiyakos <ymiyakos@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/25 10:55:14 by ymiyakos          #+#    #+#             */
/*   Updated: 2026/10/25 16:34:48 by ymiyakos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stddef.h>

int		ft_printf(const char *format, ...);

// puts
int		ft_putchar(char c);
int		ft_putstr(const char *s);
int		ft_putunbr(unsigned long unbr, const char *base);
int		ft_putnbr(long long nbr);

// utils
size_t	ft_strlen(const char *s);

#endif