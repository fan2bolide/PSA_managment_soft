/* s_warehouse est la structure mere de toutes la base de donnees elle contient *
 * les differentes structures permetant d'acceder aux CTT et a les manipuler    *
 * jusqu'a leurs ecoulement dans les shelfs					*/

typedef	struct		s_warehouse 
{
	char		*wh_name;			// name of wh
	int		sector_nb;			// nb of sector
	t_sector	*sectors			// array of sectors
	t_bbkht		**bbkht;			// hash tab to search for a CTT by id
	char		***ref_ht;
	char		***ctt_id_ht;
}			t_warehouse;

typedef	struct	s_sector
{
	int	sh_row_nb;				// nb of shelf row in the sector
	t_sh_rw	*sh_rws;				// array of shelf row
	int	st_row_nb;				// nb of stack row in the sector
	t_st_rw	*st_rws;				// array of stack row
}		t_sector;

typedef struct	s_shelf_row
{
	int	row_ln;					// lenght of the row
	char	*row_name;				// name of the row
	t_shelf	*shelfs;				// array of shelfs
}		t_sh_rw;

typedef	struct		s_shelf
{
	char		*name;				// name of the shelf
	t_shelving	*shelf;				// array of shelvings
}			t_shelf;

typedef	struct				s_shelving	// a s_shelf is one shelving of a shelf
{
	char				*name;		// name of the shelf
	int				*quantity;	// quantity of pieces
	t_CTT				*containing;	// the container on this shelf
	enum {_FH, _FL, _52GS, _52GJ}	ctt_type;	// type possibly putable in the shelving
	int				distri;
}					t_shelving;

typedef	struct				s_CTT
{
	char				*id;		// the id of the container
	char				*ref;		// the ref of the product contained
	unsigned int			*quantity;	// the quantity of product contained
	char				*type;		// the type of product contained
	char				*position;	// the position of the container in the wh	
	enum {_FH, _FL, _52GS, _52GJ}	ctt_type;	// type of the CTT
}					t_CTT;

typedef	struct	s_bbkht					// standing for big book_hash_tab
{
	char	*id;					// id of container
	t_CTT	*container;				// ptr on container
}		t_bbkht;
