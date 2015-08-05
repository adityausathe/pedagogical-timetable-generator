
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
#include "input.h"

/*
 * Three functions below initialises all the link lists.
 */

void init_subject_list(subject_list *sub_list) {
	sub_list->head = sub_list->tail = NULL;
}

void init_teacher_list(teacher_list *teach_list) {
	teach_list->head = teach_list->tail = NULL;
	teach_list->teacher_count = 0;
}

void init_standard_list(standard_list *std_list) {
	std_list->head = std_list->tail = NULL;
	std_list->standard_count = 0;
}

/*
 * Stores the subject information in a link_list from the input file of structures.
 * It takes a lab and theory subjects linklist pointers as input and creates the list.
 */

void input_subject(subject_list *lab_sub_list, subject_list *th_sub_list, char *sub_ip) {
	FILE *fp;
	subject_info **node, *tmp[128];
	int i = 0, j, flag = 0, cnt = 0, cred, num, k = 0, lab = 0, allocate = 0;
	char ch, token[32];
	
	fp = fopen(sub_ip, "r");
	if(fp == NULL) {
		perror("\n\n\n\n\t\t\tThere is some error while opening 'subject_input'\n\n\n\n");
		exit (1);
	}

	node = (subject_info **)malloc(8 * sizeof(subject_info *));
	tmp[0] = (subject_info *)malloc(sizeof(subject_info));
	
	while((ch = getc(fp)) != EOF) {
		if(ch == '$')
			lab = 1;
		if(ch == '"') {
			cnt++;
			flag = 0;
			if(cnt % 2) {
				flag = 1;
				continue;
			}
			else {
				token[i] = '\0';
				if(cnt == 2)					
					strcpy(tmp[k]->subject_ID, token);
				else if(cnt == 4)
					strcpy(tmp[k]->subject_name, token);
				else if(cnt == 6) {
					j = cred = 0;
					while(token[j] != '\0') {
						num = token[j] - '0';
						cred = 10 * cred + num;
						j++;
					}
					tmp[k]->credits = cred;
					tmp[k]->next = NULL;
					if(lab) {
						if(lab_sub_list->head) {
						lab_sub_list->tail->next = tmp[k];
						lab_sub_list->tail = tmp[k];
					} else
						lab_sub_list->tail = lab_sub_list->head = tmp[k];
					} else {
						if(th_sub_list->head) {
							th_sub_list->tail->next = tmp[k];
							th_sub_list->tail = tmp[k];
						} else
							th_sub_list->tail = th_sub_list->head = tmp[k];
					}
					allocate = 1;
				}
				flag = 0;
				i = 0;
			}
		}
		if(flag) {
			token[i] = ch;
			i++;
		} 
		if(cnt == 6 && allocate) {
			k++;
			allocate = 0;
			if(k == 8)
				node = (subject_info **)realloc(node, 2 * k * sizeof(subject_info *));
			tmp[k] = (subject_info *)malloc(sizeof(subject_info));
			cnt = 0;
		}
	}
	
	free(tmp[k]);
	fclose(fp);

}

/*
 * Stores the teacher information in a link list of structures - teacher_info.
 * It takes teacher link list as argument and creates it from file teacher_input.
 */

void input_teacher(teacher_list *teach_list, char *teach_ip) {
	FILE *fp;
	teacher_info **node, *tmp[128];
	int i = 0, j, flag = 0, cnt = 0, work, num, k = 0, sub_no = 0, spc_cnt = 1;
	char ch, token[32];

	fp = fopen(teach_ip, "r");
	if(fp == NULL) {
		perror("\n\n\n\n\t\t\tThere is some error while opening 'teacher_input'\n\n\n\n");
		exit (1);
	}
	node = (teacher_info **)malloc(8 * sizeof(teacher_info *));
	tmp[0] = (teacher_info *)malloc(sizeof(teacher_info));
	
	while(1) {
		ch = getc(fp);
		if((ch == '"' || ch == EOF)&& cnt == 6) {
				tmp[k]->next = NULL;
				tmp[k]->subject_count = sub_no;
				teach_list->teacher_count++;
				if(teach_list->head) {
					teach_list->tail->next = tmp[k];
					teach_list->tail = tmp[k];
				} else
					teach_list->tail = teach_list->head = tmp[k];
				k++;
				if(k == 8)
					node = (teacher_info **)realloc(node, 2 * k * sizeof(teacher_info *));
				tmp[k] = (teacher_info *)malloc(sizeof(teacher_info));
				cnt = sub_no = 0;
			}
		if(ch == EOF)
			break;
		if(ch == '"') {
			cnt++;
			flag = 0;
			if(cnt % 2) {
				flag = 1;
				continue;
			}
			else {
				token[i] = '\0';
				if(cnt == 2)		
					strcpy(tmp[k]->teacher_name, token);
				else if(cnt == 4)
					strcpy(tmp[k]->teacher_ID, token);
				else if(cnt == 6) {
					j = work = 0;
					while(token[j] != '\0') {
						num = token[j] - '0';
						work = 10 * work + num;
						j++;
					}
					tmp[k]->workload = work;
				}
				i = 0;
				continue;
			}
		}
		if(flag && (cnt != 6)) {
			token[i] = ch;
			i++;
		}
		if(cnt == 6) {
			if(ch == ' ' || ch == '\t' || ch == '\n') {
				if(!spc_cnt) {
					token[i] = '\0';
					strcpy(tmp[k]->subject_ID[sub_no], token);
					sub_no++;
					i = 0;
				}
				spc_cnt++;
			}else {
				token[i] = ch;
				i++;
				spc_cnt = 0;
			}
		}
	}
	fclose(fp);

}

/*
 * Stores the information about the standards in a link list of struct standard_info.
 * It takes empty standard link list pointer as argument and creates list from input file standard_input.
 */

void input_standard(standard_list *std_list, char *std_ip) {
	FILE *fp;
	standard_info **node, *tmp[128];
	int i = 0, j, flag = 0, cnt = 0, cred, num, k = 0, sub_no = 0, spc_cnt = 1, lab = 0, lab_no = 0;
	char ch, token[32];
	fp = fopen(std_ip, "r");
	if(fp == NULL) {
		perror("\n\n\n\n\t\t\tThere is some error while opening 'standard_input'\n\n\n\n");
		exit (1);
	}

	node = (standard_info **)malloc(8 * sizeof(standard_info *));
	tmp[0] = (standard_info *)malloc(sizeof(standard_info));

	while(1) {
		ch = getc(fp);
		if((ch == '"' || ch == EOF)&& cnt == 2) {
				tmp[k]->next = NULL;
				tmp[k]->sem_credits = 0;
				tmp[k]->subject_count = sub_no;
				tmp[k]->lab_subject_count = lab_no;
				if(std_list->head) {
					std_list->tail->next = tmp[k];
					std_list->tail = tmp[k];
					std_list->standard_count++;
				} else {
					std_list->tail = std_list->head = tmp[k];
					std_list->standard_count++;
				}
				k++;
				if(k == 8)
					node = (standard_info **)realloc(node, 2 * k * sizeof(standard_info *));
				tmp[k] = (standard_info *)malloc(sizeof(standard_info));
				cnt = sub_no = lab = lab_no = 0;
			}
		if(ch == EOF)
			break;
		if(ch == '"') {
			cnt++;
			flag = 0;
			if(cnt % 2) {
				flag = 1;
				continue;
			}
			else {
				token[i] = '\0';
				if(cnt == 2)
					strcpy(tmp[k]->standard_name, token);
				i = 0;
				continue;
			}
		}
		if(flag && (cnt != 2)) {
			token[i] = ch;
			i++;
		}
		if(ch == '$') {
			lab = 1;
			continue;
		}
		if(cnt == 2) {
			if(!lab) {
				if(ch == ' ' || ch == '\t' || ch == '\n') {
					if(!spc_cnt) {
						token[i] = '\0';
						strcpy(tmp[k]->subject_ID[sub_no], token);
						tmp[k]->credits_allotted[sub_no] = 0;
						sub_no++;
						i = 0;
					}
					spc_cnt++;
				}else {
					token[i] = ch;
					i++;
					spc_cnt = 0;
				}
			}else if(lab) {
				if(ch == ' ' || ch == '\t' || ch == '\n') {
					if(!spc_cnt) {
						token[i] = '\0';
						strcpy(tmp[k]->lab_subject_ID[lab_no], token);
						lab_no++;
						i = 0;
					}
					spc_cnt++;
				}else {
					token[i] = ch;
					i++;
					spc_cnt = 0;
				}
			}
		}
	}
	fclose(fp);
}

/* 
 * Assigns the teachers available for a particular subjects.
 * The assigned value get stored in subjects' structure.
 * It takes two list pointers theory subjects and teachers' list as arguments.
 */

void teach_sub(subject_list *th_sub_list, teacher_list *teach_list) {
	
	int l;
	subject_info *p = th_sub_list->head;
	teacher_info *q = teach_list->head;

	while(th_sub_list->head != NULL) {
		th_sub_list->head->teacher_count = th_sub_list->head->free_teacher = 0;
		th_sub_list->head = th_sub_list->head->next;
	}
	th_sub_list->head = p;

	while(q != NULL) {
		for(l = 0;l < q->subject_count;l++) {
			th_sub_list->head = p;
			while(th_sub_list->head != NULL) {
				if(strcmp(q->subject_ID[l], th_sub_list->head->subject_ID) == 0)
					strcpy(th_sub_list->head->teacher_ID[th_sub_list->head->teacher_count++], q->teacher_ID);
				th_sub_list->head = th_sub_list->head->next;
			}
		}
		q = q->next;
	}
	th_sub_list->head = p;

}

/*
 * Assigns a teacher to each subject of each class randomly.
 * It takes subjects' list and teachers' list as arguments.
 */

void assign_teacher2std(subject_list *sub_list, standard_list *std_list, teacher_list *teach_list) {
	
	int l;	
	standard_info *p = std_list->head;
	subject_info *q = sub_list->head;
	teacher_info *r = teach_list->head;

	while(std_list->head != NULL) {
		for(l = 0;l < std_list->head->subject_count;l++) {
			sub_list->head = q;
			while(sub_list->head != NULL) {
				if(strcmp(std_list->head->subject_ID[l], sub_list->head->subject_ID) == 0) {
					std_list->head->sem_credits += sub_list->head->credits;
					if(sub_list->head->free_teacher == sub_list->head->teacher_count)
						sub_list->head->free_teacher = 0;
					strcpy(std_list->head->teacher_ID[l], sub_list->head->teacher_ID[sub_list->head->free_teacher++]);
					teach_list->head = r;
					
					while(teach_list->head != NULL) {
						if(strcmp(std_list->head->teacher_ID[l], teach_list->head->teacher_ID) == 0) {
							teach_list->head->workload -= sub_list->head->credits;
							
						}
						teach_list->head = teach_list->head->next;
					}

				}
				sub_list->head = sub_list->head->next;
			}
		}
		std_list->head = std_list->head->next;
	}
	teach_list->head = r;
	std_list->head = p;
	sub_list->head = q;

}
