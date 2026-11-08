#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_list
{
	void			*data;
	struct s_list	*next;
}	t_list;

void	ft_list_sort(t_list **begin_list, int (*cmp)());

static int	g_fail = 0;

/* ---------- comparateurs ---------- */

/* strcmp renvoie n'importe quel entier, pas seulement -1/0/1 : bon test. */
static int	cmp_str(const char *a, const char *b)
{
	return (strcmp(a, b));
}

/* data contient directement un entier (cast), pas un pointeur : verifie
** que ft_list_sort ne presuppose rien sur le type de data. */
static int	cmp_int(void *a, void *b)
{
	long	x = (long)a;
	long	y = (long)b;

	if (x < y)
		return (-1000);
	if (x > y)
		return (1000);
	return (0);
}

/* ---------- helpers ---------- */

static t_list	*build_str(const char **words, int n)
{
	t_list	*head = NULL;
	t_list	*tail = NULL;
	t_list	*node;
	int		i;

	for (i = 0; i < n; i++)
	{
		node = malloc(sizeof(t_list));
		node->data = strdup(words[i]);
		node->next = NULL;
		if (tail)
			tail->next = node;
		else
			head = node;
		tail = node;
	}
	return (head);
}

static t_list	*build_int(const long *vals, int n)
{
	t_list	*head = NULL;
	t_list	*tail = NULL;
	t_list	*node;
	int		i;

	for (i = 0; i < n; i++)
	{
		node = malloc(sizeof(t_list));
		node->data = (void *)vals[i];
		node->next = NULL;
		if (tail)
			tail->next = node;
		else
			head = node;
		tail = node;
	}
	return (head);
}

static void	free_list(t_list *lst, int free_data)
{
	t_list	*next;

	while (lst)
	{
		next = lst->next;
		if (free_data)
			free(lst->data);
		free(lst);
		lst = next;
	}
}

static void	print_str(t_list *lst)
{
	if (!lst)
		printf("(empty)");
	while (lst)
	{
		printf("%s", (char *)lst->data);
		if (lst->next)
			printf(" -> ");
		lst = lst->next;
	}
	printf("\n");
}

/* Compte les noeuds avec une borne : si la liste boucle sur elle-meme
** (next mal raccorde), on s'arrete au lieu de tourner a l'infini. */
static int	count_bounded(t_list *lst, int max)
{
	int	n = 0;

	while (lst && n <= max)
	{
		n++;
		lst = lst->next;
	}
	return (n);
}

static void	check(int ok, const char *what)
{
	printf("  [%s] %s\n", ok ? "OK" : "KO", what);
	if (!ok)
		g_fail++;
}

/* Verifie : meme nombre de noeuds, pas de cycle, ordre croissant selon cmp,
** et que l'ensemble des data est preserve (rien perdu, rien duplique). */
static void	verify(t_list *lst, void **orig, int n, int (*cmp)())
{
	int		i;
	int		j;
	int		found;
	int		*used;
	t_list	*cur;

	check(count_bounded(lst, n) == n, "nombre de noeuds inchange, pas de cycle");
	cur = lst;
	i = 1;
	while (cur && cur->next)
	{
		if (cmp(cur->data, cur->next->data) > 0)
		{
			check(0, "ordre croissant");
			break ;
		}
		cur = cur->next;
		i++;
	}
	if (!cur || !cur->next)
		check(1, "ordre croissant");
	used = calloc(n, sizeof(int));
	cur = lst;
	found = 1;
	while (cur && found)
	{
		found = 0;
		for (j = 0; j < n; j++)
		{
			if (!used[j] && orig[j] == cur->data)
			{
				used[j] = 1;
				found = 1;
				break ;
			}
		}
		cur = cur->next;
	}
	check(found, "chaque data d'origine est present exactement une fois");
	free(used);
}

static void	**snapshot(t_list *lst, int n)
{
	void	**s = malloc(sizeof(void *) * (n ? n : 1));
	int		i;

	for (i = 0; i < n && lst; i++)
	{
		s[i] = lst->data;
		lst = lst->next;
	}
	return (s);
}

static void	run_str(const char *title, const char **words, int n,
		const char *expected)
{
	t_list	*lst = build_str(words, n);
	void	**orig = snapshot(lst, n);

	printf("== %s ==\n", title);
	printf("before : ");
	print_str(lst);
	ft_list_sort(&lst, cmp_str);
	printf("after  : ");
	print_str(lst);
	printf("expect : %s\n", expected);
	verify(lst, orig, n, cmp_str);
	printf("\n");
	free(orig);
	free_list(lst, 1);
}

static void	run_int(const char *title, const long *vals, int n)
{
	t_list	*lst = build_int(vals, n);
	void	**orig = snapshot(lst, n);

	printf("== %s (%d entiers) ==\n", title, n);
	ft_list_sort(&lst, cmp_int);
	verify(lst, orig, n, cmp_int);
	printf("\n");
	free(orig);
	free_list(lst, 0);
}

int	main(void)
{
	const char	*w_mid[] = {"banana", "apple", "cherry"};
	const char	*w_sorted[] = {"a", "b", "c", "d"};
	const char	*w_rev[] = {"d", "c", "b", "a"};
	const char	*w_dup[] = {"b", "a", "b", "c", "a", "b"};
	const char	*w_same[] = {"x", "x", "x"};
	const char	*w_one[] = {"solo"};
	const char	*w_two[] = {"z", "y"};
	const char	*w_pref[] = {"abc", "ab", "abcd", "a"};
	t_list		*empty = NULL;
	long		big[200];
	long		rev[50];
	int			i;

	run_str("liste vide", NULL, 0, "(empty)");
	run_str("un seul element", w_one, 1, "solo");
	run_str("deux elements inverses", w_two, 2, "y -> z");
	run_str("desordre simple", w_mid, 3, "apple -> banana -> cherry");
	run_str("deja triee", w_sorted, 4, "a -> b -> c -> d");
	run_str("triee a l'envers", w_rev, 4, "a -> b -> c -> d");
	run_str("doublons", w_dup, 6, "a -> a -> b -> b -> b -> c");
	run_str("tous identiques", w_same, 3, "x -> x -> x");
	run_str("prefixes (strcmp sur longueurs differentes)", w_pref, 4,
		"a -> ab -> abc -> abcd");

	srand(42);
	for (i = 0; i < 200; i++)
		big[i] = rand() % 1000 - 500;
	run_int("aleatoire avec negatifs", big, 200);
	for (i = 0; i < 50; i++)
		rev[i] = 50 - i;
	run_int("inverse, pire cas pour un tri par insertion/bulle", rev, 50);

	printf("== begin_list NULL / liste vide ==\n");
	ft_list_sort(NULL, cmp_str);
	ft_list_sort(&empty, cmp_str);
	check(empty == NULL, "pas de crash, liste toujours NULL");
	printf("\n");

	if (g_fail)
		printf("RESULTAT : %d test(s) KO\n", g_fail);
	else
		printf("RESULTAT : tout est OK\n");
	return (g_fail != 0);
}
