#include <stdio.h>
#include <stdlib.h>

typedef struct s_list
{
	void			*data;
	struct s_list	*next;
}	t_list;

void	ft_list_push_front(t_list **begin_list, void *data);

static void	print_list(const char *label, t_list *lst)
{
	printf("%s: ", label);
	while (lst)
	{
		printf("%s", (char *)lst->data);
		if (lst->next)
			printf(" -> ");
		lst = lst->next;
	}
	printf("\n");
}

static void	free_list(t_list *lst)
{
	t_list	*next;

	while (lst)
	{
		next = lst->next;
		free(lst);
		lst = next;
	}
}

int	main(void)
{
	t_list	*lst = NULL;
	t_list	*old_head;

	ft_list_push_front(&lst, "one");
	print_list("after push one  ", lst);
	printf("  head data = %s (expected one), head next = %p (expected (nil))\n",
		(char *)lst->data, (void *)lst->next);

	old_head = lst;
	ft_list_push_front(&lst, "two");
	print_list("after push two  ", lst);
	printf("  head data = %s (expected two), head next == old head ? %s\n",
		(char *)lst->data, lst->next == old_head ? "yes" : "NO");

	old_head = lst;
	ft_list_push_front(&lst, "three");
	print_list("after push three", lst);
	printf("  head data = %s (expected three), head next == old head ? %s\n",
		(char *)lst->data, lst->next == old_head ? "yes" : "NO");

	/* expected: three -> two -> one */
	free_list(lst);
	return (0);
}
