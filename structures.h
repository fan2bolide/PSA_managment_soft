/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bajeanno <bajeanno@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/14 01:54:29 by bajeanno          #+#    #+#             */
/*   Updated: 2022/08/14 01:54:29 by bajeanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "main_header.h"

#ifndef PSA_STRUCTURES_H
#define PSA_STRUCTURES_H

// defining CTT as a structure
struct CTT
{
	char *id;
	char *product_reference;
	unsigned int nb_pieces;
	enum {_FH, _FL, _52GS, _52GJ} ctt_type;
};

//defining stack nodes as list nodes but only rights to push front and delete an element
typedef struct stack_node
{
	struct stack_node *under;
	struct CTT *data;
}   stack_node;

typedef struct s_stack
{
	struct stack_node *top;
}	t_stack;

//defining shelf as a array of array of <struct CTT>
typedef struct s_shelf
{
	struct CTT **data;
}	t_shelf;

//defining row as an array of stacks or shelfs
typedef struct s_row
{
	enum row_type{shelf, stack} row_type;
	unsigned int row_size;
	t_stack *stacks;
	t_shelf *shelfs;
}	t_row;

struct allee //nom provisoire
{
	char *name;
	t_row *row_right;
	t_row *row_left;
};

#endif