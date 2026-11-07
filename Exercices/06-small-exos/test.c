/* Compilation :
**   nasm -f elf64 -g -F dwarf atoi_base.s -o atoi_base.o
**   gcc -Wall -Wextra -g test.c atoi_base.o -o test
**   ./test
**
** Chaque test compare le resultat de ton asm avec une implementation C
** de reference (ref_atoi_base) qui suit le comportement attendu :
**   - base invalide (moins de 2 chars, doublons, '+', '-', espace) -> 0
**   - on saute les espaces blancs (isspace) au debut
**   - on accepte plusieurs '+' / '-' (chaque '-' inverse le signe)
**   - on s'arrete au premier char qui n'est pas dans la base
*/

#include <stdio.h>
#include <string.h>
#include <limits.h>

int	ft_atoi_base(char *str, char *base);
int	check_base(char *base);

/* ----------------------------------------------------------------- */
/* Implementation C de reference                                     */
/* ----------------------------------------------------------------- */

static int	ref_isspace(char c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

static int	ref_check_base(char *base)
{
	int	i;
	int	j;

	if (!base || strlen(base) < 2)
		return (0);
	for (i = 0; base[i]; i++)
	{
		if (base[i] == '+' || base[i] == '-' || ref_isspace(base[i]))
			return (0);
		for (j = i + 1; base[j]; j++)
			if (base[i] == base[j])
				return (0);
	}
	return (1);
}

static int	ref_index(char c, char *base)
{
	int	i;

	for (i = 0; base[i]; i++)
		if (base[i] == c)
			return (i);
	return (-1);
}

static int	ref_atoi_base(char *str, char *base)
{
	int	sign = 1;
	int	result = 0;
	int	len;
	int	d;

	if (!ref_check_base(base))
		return (0);
	len = (int)strlen(base);
	while (ref_isspace(*str))
		str++;
	while (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign = -sign;
		str++;
	}
	while ((d = ref_index(*str, base)) != -1)
	{
		result = result * len + d;
		str++;
	}
	return (result * sign);
}

/* ----------------------------------------------------------------- */
/* Harness                                                           */
/* ----------------------------------------------------------------- */

static int	g_pass = 0;
static int	g_fail = 0;

static void	show(const char *s)
{
	printf("\"");
	for (; *s; s++)
	{
		if (*s == '\n') printf("\\n");
		else if (*s == '\t') printf("\\t");
		else if (*s == '\v') printf("\\v");
		else if (*s == '\f') printf("\\f");
		else if (*s == '\r') printf("\\r");
		else printf("%c", *s);
	}
	printf("\"");
}

static void	test_atoi(char *str, char *base)
{
	int	expected = ref_atoi_base(str, base);
	int	got = ft_atoi_base(str, base);

	if (expected == got)
	{
		g_pass++;
		printf("[OK]   ft_atoi_base(");
		show(str);
		printf(", ");
		show(base);
		printf(") = %d\n", got);
	}
	else
	{
		g_fail++;
		printf("[KO]   ft_atoi_base(");
		show(str);
		printf(", ");
		show(base);
		printf(") = %d  (attendu %d)\n", got, expected);
	}
}

#define DEC "0123456789"
#define BIN "01"
#define OCT "01234567"
#define HEX "0123456789abcdef"
#define HEXU "0123456789ABCDEF"
#define WEIRD "poneyvif"

int	main(void)
{
	printf("\n=== ft_atoi_base : base invalide ===\n");
	test_atoi("42", "");
	test_atoi("42", "0");
	test_atoi("42", "0120");
	test_atoi("42", "01+");
	test_atoi("42", "01-");
	test_atoi("42", "01 ");
	test_atoi("42", "0\t1");

	//printf("\n=== ft_atoi_base : base 10 ===\n");
	//test_atoi("0", DEC);
	test_atoi("42", DEC);
	test_atoi("-42", DEC);
	test_atoi("+42", DEC);
	test_atoi("123456789", DEC);
	test_atoi("2147483647", DEC);
	test_atoi("-2147483648", DEC);

	printf("\n=== ft_atoi_base : espaces blancs ===\n");
	test_atoi("   42", DEC);
	test_atoi("\t\n\v\f\r 42", DEC);
	test_atoi("  -42", DEC);
	test_atoi("42  ", DEC);
	test_atoi(" - 42", DEC);

	printf("\n=== ft_atoi_base : signes multiples ===\n");
	test_atoi("--42", DEC);
	test_atoi("---42", DEC);
	test_atoi("+-42", DEC);
	test_atoi("-+42", DEC);
	test_atoi("+-+-+-42", DEC);
	test_atoi("-", DEC);
	test_atoi("+", DEC);

	printf("\n=== ft_atoi_base : arret sur char invalide ===\n");
	test_atoi("42abc", DEC);
	test_atoi("4 2", DEC);
	test_atoi("4-2", DEC);
	test_atoi("abc", DEC);
	test_atoi("", DEC);
	test_atoi("12", BIN);

	printf("\n=== ft_atoi_base : autres bases ===\n");
	test_atoi("101010", BIN);
	test_atoi("-101010", BIN);
	test_atoi("52", OCT);
	test_atoi("777", OCT);
	test_atoi("2a", HEX);
	test_atoi("ff", HEX);
	test_atoi("-ff", HEX);
	test_atoi("FF", HEX);
	test_atoi("FF", HEXU);
	test_atoi("7fffffff", HEX);
	test_atoi("-80000000", HEX);
	test_atoi("poney", WEIRD);
	test_atoi("vif", WEIRD);
	test_atoi("  --fi", WEIRD);

	printf("\n=== resultat : %d OK, %d KO ===\n", g_pass, g_fail);
	return (g_fail != 0);
}
