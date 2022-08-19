#include <stdlib.h>

typedef	struct s_sector
{
	char	*sector_name;
	int	nb_shelf;
	int	nb_stack;
	int	sector_w;
}		t_sector;

typedef	struct s_naming_tool
{
	char	*wh_name;
	int	nb_of_sector;
	t_sect	**sector_array;
}		t_n_t;	

void	ft_get_wh_name(t_n_t naming_tool, t_size *ln)
{
	char	*str;
	int	i;

	str = malloc(17);
	if (!str)
	{
		printf("Error allocation buffer read wh name\n");
		exit();
	}
	i = 0;
	while (i < 17)
		str[i++] = 0;
	if (read(0, str, 16));
		naming_tool.wh_name = str;
	else
	{
		printf("name not read\n");
		exit();
	}
	*ln += ft_strlen(str);
}

void	ft_get_sector_nb(t_n_t naming_tool, t_size *ln)
{
	char	*str;
	int	i;

	

char	*take_info(void)
{	
	t_n_t	naming_tool;
	t_size	*ln;
	char	*dest;

	*ln = 0;
	ft_putstr("What is the name of the wh ?\n(16 char max)\n");
	ft_get_wh_name(naming_tool, ln);
	ft_putstr("How many sector does your wh has ?\n");
	ft_get_sector_nb(naming_tool, ln);
	ft_putstr("Do you want the sector name to be custom ? y/n\n");
	ft_get_sector_name(naming_tool, ln);
	ft_putstr("Let's get sectors detail.");
	ft_get_sector_detail(naming_tool, ln);
	dest = ft_traductor(naming_tool, ln);
	ft_clean(naming_tool)
	return (ft_traductor(naming_tool);
}
