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

typedef struct s_shelf
{
	struct CTT **data;
}	t_shelf;


typedef struct row
{
	enum row_type{shelf, stack} row_type;
	unsigned int row_size;
	t_stack *stacks;
	t_shelf *shelf;
}	row;


#endif
