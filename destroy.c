
/*****************************************************************************
 * Copyright (C) Aditya Sathe satheaditya2014@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

/*
 * This function free the dynamically allocated memories at the end of program.
 * It takes all the list pointer as arguments and free them and returns nothing.
 */

#include "input.h"

void destroy_database(subject_list *lab_sub_list, subject_list *th_sub_list, teacher_list *teach_list, standard_list *std_list) {
	
	subject_info *p;
	teacher_info *q;
	standard_info *r;
	while(lab_sub_list->head) {
		p = lab_sub_list->head->next;
		free(lab_sub_list->head);
		lab_sub_list->head = p;
	}
	while(th_sub_list->head) {
		p = th_sub_list->head->next;
		free(th_sub_list->head);
		th_sub_list->head = p;
	}
	while(teach_list->head) {
		q = teach_list->head->next;
		free(teach_list->head);
		teach_list->head = q;
	}
	while(std_list->head) {
		r = std_list->head->next;
		free(std_list->head);
		std_list->head = r;
	}
}
