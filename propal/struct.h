/* s_warehouse est la structure mere de toutes la base de donnees elle contient *
 * les differentes structures permetant d'acceder aux CTT et a les manipuler    *
 * jusqu'a leurs ecoulement dans les shelfs					*/

typedef	struct	s_warehouse 
{
	int	sh_nb;			// nb of shelf
	t_shelf	*sh_tab;		// tab of shelf
	t_sh_ht	**sh_ht;		// hash tab to search for a ref
	int	st_nb;			// nb of stacks
	t_stack	**st_tab;		// tab of stacks
	t_st_ht	**st_ht;		// hash tab to search for a ref
	t_bbkht	**bbkht;		// hash tab to search for a CTT by id
}		t_warehouse;



typedef	struct		s_CTT
{
	char		*id;		// the id of the container
	char		*ref;		// the ref of the product contained
	unsigned int	quantity;	// the quantity of product contained
	char		*type;		// the type of product contained
	char		*position;	// the position of the container int the wh
}			t_CTT;

/* cette structure contient un pointeur sur pointeur sur t_CTT pour le cas ou *
 * il y aurait plusieurs container sur le meme etage d'une etagere            */

typedef	struct	s_shelf			// a s_shelf is one shelving of a shelf
{
	char	*name;			// name of the shelf
	t_CTT	*containing;		// the array of container on this shelf
}		t_shelf;

/* meme chose */

typedef struct	s_stack			// a s_stack can be accessed only on the last
					// CTT added, it will be the t_CTT[quantity - 1]
{
	char	anomaly;
	char	*name;
	t_CTT	**containing;
	int	quantity;
}		t_stack;

/* l'idee de cette structure est de faciliter la recherche d'une reference en *
 * particulier								      */

typedef	struct	s_sh_ht			// standing for shelf_hash_tab
{
	char		*ref;		// the reference of the product
	unsigned int	quantity;	// the quantity remaining on shelves in tthe wh
	t_CTT		**position;	// the aray of container stocked on shelves
}		t_sh_ht;

/* meme chose */

typedef struct	s_st_ht			// standing for stack_hash_tab
{
	char		anomaly;
	unsigned int	quantity;
	t_CTT		**position;
}			t_st_ht;

/* tableau de hachage contenant l'ensemble des CTT present dans l'entrepot    */

typedef	struct	s_bbkht			//standing for big book_hash_tab
{
	char	*id;
	t_CTT	*container;
}		t_bbkht;
