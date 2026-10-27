/*
** ft_printf tester
**   ./test_ft_printf          通常テスト (printf と戻り値・出力を比較)
**   ./test_ft_printf --big    + writeの上限テスト (約2GiBのメモリを使う)
**
** 出力は stdout を一時ファイルへ dup2 して捕獲する。
** printf はバッファされるので、捕獲の前後で fflush が必須。
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include "ft_printf.h"

typedef struct s_cap
{
	char	*buf;
	size_t	len;
}	t_cap;

static int	g_total;
static int	g_fail;
static int	g_tmp_fd = -1;
static int	g_saved_fd = -1;

/* ---------- capture ---------- */

static void	cap_begin(void)
{
	if (g_tmp_fd < 0)
	{
		FILE	*f = tmpfile();

		if (!f)
		{
			perror("tmpfile");
			exit(2);
		}
		g_tmp_fd = fileno(f);
	}
	fflush(stdout);
	if (ftruncate(g_tmp_fd, 0) < 0 || lseek(g_tmp_fd, 0, SEEK_SET) < 0)
		exit(2);
	g_saved_fd = dup(1);
	dup2(g_tmp_fd, 1);
}

static t_cap	cap_end(void)
{
	t_cap	c;
	off_t	size;

	fflush(stdout);
	dup2(g_saved_fd, 1);
	close(g_saved_fd);
	size = lseek(g_tmp_fd, 0, SEEK_END);
	c.len = (size_t)size;
	c.buf = malloc(c.len + 1);
	if (!c.buf || pread(g_tmp_fd, c.buf, c.len, 0) != (ssize_t)c.len)
		exit(2);
	c.buf[c.len] = '\0';
	return (c);
}

/* ---------- report ---------- */

static void	put_escaped(const t_cap *c)
{
	size_t	i;

	fputc('"', stderr);
	for (i = 0; i < c->len; i++)
	{
		unsigned char	ch = (unsigned char)c->buf[i];

		if (ch == '\n')
			fputs("\\n", stderr);
		else if (ch < 32 || ch >= 127)
			fprintf(stderr, "\\x%02x", ch);
		else
			fputc(ch, stderr);
	}
	fputc('"', stderr);
}

static void	report(const char *expr, int cmp_out,
				int r1, const t_cap *a, int r2, const t_cap *b)
{
	int	ok;

	g_total++;
	ok = (r1 == r2);
	if (cmp_out)
		ok = ok && a->len == b->len && memcmp(a->buf, b->buf, a->len) == 0;
	if (ok)
		return ;
	g_fail++;
	fprintf(stderr, "[KO] %s\n", expr);
	fprintf(stderr, "     printf   : ret=%d out=", r1);
	put_escaped(a);
	fprintf(stderr, "\n     ft_printf: ret=%d out=", r2);
	put_escaped(b);
	fputc('\n', stderr);
}

/* printf と ft_printf を同じ引数で呼び、戻り値と出力を比較 */
#define T(...) do { \
	cap_begin(); \
	int r1_ = printf(__VA_ARGS__); \
	t_cap a_ = cap_end(); \
	cap_begin(); \
	int r2_ = ft_printf(__VA_ARGS__); \
	t_cap b_ = cap_end(); \
	report(#__VA_ARGS__, 1, r1_, &a_, r2_, &b_); \
	free(a_.buf); free(b_.buf); \
} while (0)

/* 戻り値だけ比較 (出力が実装依存のもの用) */
#define T_RET(...) do { \
	cap_begin(); \
	int r1_ = printf(__VA_ARGS__); \
	t_cap a_ = cap_end(); \
	cap_begin(); \
	int r2_ = ft_printf(__VA_ARGS__); \
	t_cap b_ = cap_end(); \
	report(#__VA_ARGS__, 0, r1_, &a_, r2_, &b_); \
	free(a_.buf); free(b_.buf); \
} while (0)

/* printf と比較できないものは期待値を直接渡す */
#define T_EXPECT(expected, ...) do { \
	cap_begin(); \
	int r2_ = ft_printf(__VA_ARGS__); \
	t_cap b_ = cap_end(); \
	t_cap a_ = { "", 0 }; \
	report("(expect " #expected ") " #__VA_ARGS__, 0, (expected), &a_, r2_, &b_); \
	free(b_.buf); \
} while (0)

/* ---------- tests ---------- */

static void	test_plain(void)
{
	T("");
	T("abc");
	T("hello, world\n");
	T("tab\there");
}

static void	test_char(void)
{
	T("%c", 'a');
	T("%c", '\n');
	T("%c", '\0');
	T("%c", 255);
	T("[%c%c%c]", 'a', 'b', 'c');
}

static void	test_string(void)
{
	char	*null_str = NULL;
	char	longstr[5001];

	memset(longstr, 'x', 5000);
	longstr[5000] = '\0';
	T("%s", "hello");
	T("%s", "");
	T("%s", null_str);
	T("[%s][%s]", "a", "b");
	T("%s", longstr);
}

static void	test_ptr(void)
{
	int		x = 42;
	void	*null_ptr = NULL;
	void	*heap = malloc(1);

	T("%p", (void *)&x);
	T("%p", heap);
	T("%p", null_ptr);
	T("%p", (void *)1);
	T("%p", (void *)-1L);
	T("%p %p", (void *)&x, null_ptr);
	free(heap);
}

static void	test_int(void)
{
	T("%d", 0);
	T("%d", 1);
	T("%d", -1);
	T("%d", 42);
	T("%d", -42);
	T("%d", INT_MAX);
	T("%d", INT_MIN);
	T("%i", 0);
	T("%i", -2147483647);
	T("%i", INT_MIN);
	T("%i", INT_MAX);
}

static void	test_unsigned(void)
{
	T("%u", 0);
	T("%u", 1);
	T("%u", 42u);
	T("%u", -1);
	T("%u", INT_MAX);
	T("%u", INT_MIN);
	T("%u", UINT_MAX);
}

static void	test_hex(void)
{
	T("%x", 0);
	T("%x", 9);
	T("%x", 10);
	T("%x", 255);
	T("%x", 4096);
	T("%x", -1);
	T("%x", INT_MIN);
	T("%x", UINT_MAX);
	T("%X", 0);
	T("%X", 255);
	T("%X", 0xabcdef);
	T("%X", -1);
	T("%X", UINT_MAX);
}

static void	test_percent(void)
{
	T("%%");
	T("%%%%");
	T("100%%");
	T("%%d");
	T("a%%b%%c");
	T("%c%%", 'x');
}

static void	test_mixed(void)
{
	int		x = 7;
	char	*null_str = NULL;

	T("%c%s%d%i%u%x%X%p%%", 'a', "bc", 1, -2, 3u, 255, 255, (void *)&x);
	T("%s %s %s", "a", null_str, "c");
	T("%d%d%d", 1, 2, 3);
	T("%x%X", 0xff, 0xff);
	T("start %d middle %s end", 42, "str");
	T("%d", 1); /* 直前の呼び出しの影響が残らないか */
	T("%s%s", "", "");
}

static void	test_trailing_percent(void)
{
	/* 出力は実装依存なので戻り値のみ比較 */
	T_RET("abc%");
	T_RET("%");
	T_RET("%d%", 1);
}

static void	test_return_value(void)
{
	T_EXPECT(-1, NULL);
	T_EXPECT(0, "");
	T_EXPECT(3, "abc");
	T_EXPECT(11, "%d", INT_MIN);
	T_EXPECT(1, "%c", '\0');
	T_EXPECT(5, "%p", (void *)NULL);
	T_EXPECT(6, "%s", (char *)NULL);
}

/* stdout を /dev/full に向けて write を失敗させる */
static void	expect_write_fail(const char *label, int ret)
{
	g_total++;
	if (ret == -1)
		return ;
	g_fail++;
	fprintf(stderr, "[KO] write fail: %s -> ret=%d (expected -1)\n", label, ret);
}

static void	test_write_error(void)
{
	int		full = open("/dev/full", O_WRONLY);
	int		saved;
	int		x = 1;
	int		r[8];

	if (full < 0)
	{
		fprintf(stderr, "skip: /dev/full not available\n");
		return ;
	}
	fflush(stdout);
	saved = dup(1);
	dup2(full, 1);
	r[0] = ft_printf("abc");
	r[1] = ft_printf("%c", 'a');
	r[2] = ft_printf("%s", "hello");
	r[3] = ft_printf("%d", 12345);
	r[4] = ft_printf("%x", 0xdeadbeef);
	r[5] = ft_printf("%u", UINT_MAX);
	r[6] = ft_printf("%p", (void *)&x);
	r[7] = ft_printf("ok%dok", 1);
	dup2(saved, 1);
	close(saved);
	close(full);
	expect_write_fail("abc", r[0]);
	expect_write_fail("%c", r[1]);
	expect_write_fail("%s", r[2]);
	expect_write_fail("%d", r[3]);
	expect_write_fail("%x", r[4]);
	expect_write_fail("%u", r[5]);
	expect_write_fail("%p", r[6]);
	expect_write_fail("ok%dok", r[7]);
}

/* write の1回あたりの上限 (INT_MAX - 4095) 周りと戻り値の上限 */
static void	big_case(const char *label, char *buf, size_t len, int expected, int args)
{
	int	r;

	g_total++;
	buf[len] = '\0';
	if (args == 2)
		r = ft_printf("%s%s", buf, buf);
	else
		r = ft_printf("%s", buf);
	buf[len] = 'a';
	if (r == expected)
		return ;
	g_fail++;
	fprintf(stderr, "[KO] big: %s -> ret=%d (expected %d)\n", label, r, expected);
}

static void	test_big(void)
{
	size_t	max_write = (size_t)INT_MAX - 4095;
	size_t	size = (size_t)INT_MAX + 2;
	char	*buf = malloc(size);
	int		devnull = open("/dev/null", O_WRONLY);
	int		saved;

	if (!buf || devnull < 0)
	{
		fprintf(stderr, "skip: cannot allocate %zu bytes or open /dev/null\n", size);
		free(buf);
		return ;
	}
	memset(buf, 'a', size - 1);
	fflush(stdout);
	saved = dup(1);
	dup2(devnull, 1);
	big_case("INT_MAX-4095 (1回で収まる境界)", buf, max_write, (int)max_write, 1);
	big_case("INT_MAX-4094 (2周目に1byte)", buf, max_write + 1, (int)max_write + 1, 1);
	big_case("INT_MAX", buf, INT_MAX, INT_MAX, 1);
	big_case("INT_MAX+1 (ft_putstr上限超え)", buf, (size_t)INT_MAX + 1, -1, 1);
	big_case("%s%s 合計がINT_MAXを超える", buf, (size_t)INT_MAX / 2 + 1, -1, 2);
	big_case("%s%s 合計がちょうどINT_MAX", buf, (size_t)INT_MAX / 2, INT_MAX - 1, 2);
	dup2(saved, 1);
	close(saved);
	close(devnull);
	free(buf);
}

int	main(int argc, char **argv)
{
	test_plain();
	test_char();
	test_string();
	test_ptr();
	test_int();
	test_unsigned();
	test_hex();
	test_percent();
	test_mixed();
	test_trailing_percent();
	test_return_value();
	test_write_error();
	if (argc > 1 && strcmp(argv[1], "--big") == 0)
		test_big();
	printf("%d / %d passed\n", g_total - g_fail, g_total);
	return (g_fail != 0);
}
