#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_list
{
	void			*data;
	struct s_list	*next;
}	t_list;

void	ft_list_remove_if(t_list **begin_list, void *data_ref, int (*cmp)(),
			void (*free_fct)(void *));

static int	g_free_count = 0;

static void	count_free(void *data)
{
	g_free_count++;
	free(data);
}

static t_list	*build_list(const char **words, int n)
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

static void	free_list(t_list *lst)
{
	t_list	*next;

	while (lst)
	{
		next = lst->next;
		free(lst->data);
		free(lst);
		lst = next;
	}
}

static void	print_list(t_list *lst)
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

static void	run(const char *title, const char **words, int n, char *to_remove,
		const char *expected, int expected_frees)
{
	t_list	*lst = build_list(words, n);

	g_free_count = 0;
	printf("== %s ==\n", title);
	printf("before : ");
	print_list(lst);
	ft_list_remove_if(&lst, to_remove, strcmp, count_free);
	printf("after  : ");
	print_list(lst);
	printf("expect : %s\n", expected);
	printf("frees  : %d (expected %d)\n\n", g_free_count, expected_frees);
	free_list(lst);
}

int	main(void)
{
	const char	*w1[] = {"one", "two", "three", "four"};
	const char	*w2[] = {"two", "two", "one", "two", "three", "two"};
	const char	*w3[] = {"same", "same", "same"};
	t_list		*empty = NULL;

	run("remove middle", w1, 4, "two", "one -> three -> four", 1);
	run("remove head", w1, 4, "one", "two -> three -> four", 1);
	run("remove last", w1, 4, "four", "one -> two -> three", 1);
	run("remove nothing (no match)", w1, 4, "five",
		"one -> two -> three -> four", 0);
	run("remove several (head, consecutive, last)", w2, 6, "two",
		"one -> three", 4);
	run("remove everything", w3, 3, "same", "(empty)", 3);

	printf("== NULL list ==\n");
	ft_list_remove_if(&empty, "x", strcmp, count_free);
	printf("no crash, list still %s\n", empty ? "NOT NULL (bad)" : "NULL");
	return (0);
}
