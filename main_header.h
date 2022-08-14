/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bajeanno <bajeanno@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/14 01:49:07 by bajeanno          #+#    #+#             */
/*   Updated: 2022/08/14 01:49:07 by bajeanno         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_HEADER_H_
#define MAIN_HEADER_H_

//includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "structures.h"

//prototypes
void create_data_structure(void *datas);
void launch_app(void *datas);
void save_data(void *datas);
void free_datas(void *datas);

#endif
