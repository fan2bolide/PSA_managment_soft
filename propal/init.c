t_warehouse	*ft_init(int nb_shelf, int nb_stack)
{
	t_warehouse	*wh;

	wh = malloc(sizeof (s_warehouse));
	if (!warehouse)
		return (NULL);
	wh->shelf_nb = nb_shelf;
	wh->shelf_tab = ft_shelf_tab_constructor(nb_shelf);
	wh->shelf_hash_tab = ft_shelf_hash_tab_constructor();
	wh->stack_nb = nb_stack;
	wh->stack_tab = ft_stack_tab_constructor(nb_stack);
	wh->stack_hash_tab = ft_stack_hash_tab_constructor();
	return (wh);
}


