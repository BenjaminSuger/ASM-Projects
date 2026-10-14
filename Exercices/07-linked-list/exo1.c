#include <stdio.h>

typedef struct s_list
{
	void			*data;
	struct s_list	*next;
}	t_list;

int	ft_list_size(t_list *begin_list);

int	main(void)
{
	char	*s1 = "one";
	char	*s2 = "two";
	char	*s3 = "three";
	char	*s4 = "four";
	t_list	n4 = {s4, NULL};
	t_list	n3 = {s3, &n4};
	t_list	n2 = {s2, &n3};
	t_list	n1 = {s1, &n2};
	t_list	*lst = &n1;

	printf("ft_list_size(lst)  = %d (expected 4)\n", ft_list_size(lst));
	printf("ft_list_size(NULL) = %d (expected 0)\n", ft_list_size(NULL));
	return (0);
}
