
t_shelf	*ft_shelf_tab_constructor(int nb_shelf)
{
	t_shelf	*dest;
	int	i;

	dest = malloc(sizeof(t_shelf) * nb);
	i = 0;
	while (i < nb_shelf)
	{
		(dest[i]).name = ft_create_shelf_name(i);
		(dest[i]).containing = NULL;
		i++;
	}
	return (dest);
}

t_stack	*ft_stack_tab_constructor(int nb_stack)
{
	t_stack *dest;
	int	i;

	dest = malloc(sizeof(t_stack) * nb);
	i = 0;
	while (i < nb_stack)
	{
		dest[i].name = ft_create_stack_name(i);
		i++;
	}
	return (dest);
}

ft_shelf_

t_warehouse	*ft_init(int nb_shelf, int nb_stack)
{
	t_warehouse	*wh;

	wh = malloc(sizeof (s_warehouse));
	if (!warehouse)
		return (NULL);
	wh->sh_nb = nb_shelf;
	wh->sh_tab = ft_shelf_tab_constructor(nb_shelf);
	wh->sh_ht = ft_shelf_hash_tab_constructor();
	wh->st_nb = nb_stack;
	wh->st_tab = ft_stack_tab_constructor(nb_stack);
	wh->st_ht = ft_stack_hash_tab_constructor();
	wh->bbkht = ft_bbkht_constructor()
	return (wh);
}


