
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

typedef struct subject_info {
	char subject_name[32];
	int credits;
	char subject_ID[8];
	char teacher_ID[16][8];
	int free_teacher;
	int teacher_count;
	int standard_count;
	struct subject_info *next;
}subject_info;

typedef struct subject_list {
	subject_info *head;
	subject_info *tail;
}subject_list;

typedef struct teacher_info {
	char teacher_name[32];
	char teacher_ID[8];
	char subject_ID[16][8];
	int subject_count;
	int workload;
	char schedule[8][16][32];
	struct teacher_info *next;
}teacher_info;

typedef struct teacher_list {
	teacher_info *head;
	teacher_info *tail;
	int teacher_count;
}teacher_list;

typedef struct standard_info {
	char standard_name[32];
	char subject_ID[16][8], lab_subject_name[8][16], lab_subject_ID[8][8] ;
	int subject_credits[16], lab_subject_credits[8];
	int subject_count, lab_subject_count;		
	char teacher_ID[16][8];
	int slot_allotted[16][16], lab_slot_allotted[8][8];
	int credits_allotted[16];
	int sem_credits;
	struct standard_info *next;
}standard_info;

typedef struct standard_list {
	standard_info *head;
	standard_info *tail;
	int standard_count;
}standard_list;

/* GLOBAL VARIABLES*/

char input_file[8][16];
int lunch;

/* function declarations: */

void input_standard(standard_list *std_list, char *std_ip);
void input_teacher(teacher_list *teach_list, char *teach_ip);
void input_subject(subject_list *lab_sub_list, subject_list *th_sub_list, char *sub_ip);
void traverse_std(standard_list *l);
void traverse_teach(teacher_list *l);
void traverse_sub(subject_list *l);
void init_subject_list(subject_list *sub_list);
void init_teacher_list(teacher_list *teach_list);
void init_standard_list(standard_list *std_list);
void std_sub(subject_list *, subject_list *, standard_list *);
void superposition(int ***, int ***, subject_list *, subject_list *, standard_list *, teacher_list *, int, int, int, int, int, int);
void separate_std_tt(int ***, subject_list *, subject_list *, standard_list *, int, int, int, int);
void slot_separator(int ***, int ***, subject_list *, standard_list *, int, int, int, int);
int ***remaining_standards(int ***, subject_list *, subject_list *, standard_list *, teacher_list *, int, int, int, int, int, int, int);
int ***init_std_tt(standard_list *, int, int, int);
int ***init_rooms(standard_list *, int, int, int);
void print_tt(int ***, int ***, subject_list *, standard_list *, int, int, int, int, int);
void subject_teacher_database(subject_list *, subject_list *, teacher_list *);
void standard_database(standard_list *);
void CONTROLLER(subject_list *, subject_list *, teacher_list *, standard_list *, int, int, int, int, int, int);
void teacher_schedule(int ***, int ***, teacher_list *, standard_list *, int, int, int, int, int);
void destroy_database(subject_list *, subject_list *, teacher_list *, standard_list *);
void DELAY(void);

