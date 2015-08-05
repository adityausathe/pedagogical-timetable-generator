
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
#include <time.h>
#include <string.h>
#include "input.h"

/*
 * It takes subject list pointers and standard list pointer as argument.
 * This function copies the credits of subjects from subject list to standard list.
 */
void std_sub(subject_list *th_sub_list, subject_list *lab_sub_list, standard_list *std_list) {
	
	int l;
	subject_info *p = th_sub_list->head;
	standard_info *q = std_list->head;
	subject_info *r = lab_sub_list->head;
	
	th_sub_list->head = p;
	lab_sub_list->head = r;
	while(std_list->head != NULL) {
		for(l = 0;l < std_list->head->subject_count;l++) {
			th_sub_list->head = p;
			while(th_sub_list->head != NULL) {
				if(strcmp(std_list->head->subject_ID[l], th_sub_list->head->subject_ID) == 0)
					std_list->head->subject_credits[l] = th_sub_list->head->credits;
				th_sub_list->head = th_sub_list->head->next;
			}
		}
		
		for(l = 0;l < std_list->head->lab_subject_count;l++) {
			lab_sub_list->head = r;
			while(lab_sub_list->head != NULL) {
				if(strcmp(std_list->head->lab_subject_ID[l], lab_sub_list->head->subject_ID) == 0)
					std_list->head->lab_subject_credits[l] = lab_sub_list->head->credits;
				lab_sub_list->head = lab_sub_list->head->next;
			}
		}
		std_list->head = std_list->head->next;
	}
	std_list->head = q;
	th_sub_list->head = p;
	lab_sub_list->head = r;
}

/*
 * It dynamically allocates a 3-D array ***rooms.
 * ***rooms is used in separate_std_tt(...) to create the timetable.
 * It takes standard list pointer and array size parameters as input and gives ***rooms as output.
 */

int ***init_rooms(standard_list *std_list, int working_days, int daily_slots, int classrooms) {
	
	int i, j, ***rooms, standard_count; 
	
	if(std_list->standard_count > (2 * classrooms))
		standard_count = 2 * classrooms;
	else
		standard_count = std_list->standard_count;

	rooms = (int ***)malloc(standard_count * sizeof(int **));
	for(i = 0;i < standard_count;i++) {
		rooms[i] = (int **)malloc(working_days * sizeof(int *));
		for(j = 0;j < (working_days);j++) 
			rooms[i][j] = (int *)malloc(daily_slots * sizeof(int));      
	}
	
	return rooms;

}

/*
 * This function allocates the time slots to the no of standards twice the no of classrooms( i.e. 2 * classrooms) regardless of overlapping.
 * It first allocates lab and then theory lectures.
 * It takes all the list pointers as arguments along with the rooms and other parameters.
 */

void separate_std_tt(int ***rooms, subject_list *sub_list, subject_list *lab_sub_list, standard_list *std_list, int working_days, int 				daily_slots, int classrooms, int batch) {

	int slot_no, a[16], min, max, i, j, k, m, rnd, std_slots, std_no, daily_slot_count, sub_no, class, flag, flag1, hour, cnt, 			l, holder, batch_no, BIG, lab_duration[8], check, sub_no2, lunch_break = 3, standard_count, j_cnt, residue;     
	
	time_t tt;
	srandom(time(&tt));
	standard_info *q = std_list->head, *LIST_HEAD;

	if(std_list->standard_count > (2 * classrooms))
		standard_count = 2 * classrooms;
	else
		standard_count = std_list->standard_count;
	for(i = 0;i < standard_count;i++)
		for(j = 0;j < (working_days);j++) 
			for(k = 0;k < daily_slots;k++) {
				rooms[i][j][k] = 0;
			}
	
	i = j = k = 0;
	std_no = 1;
	while(std_no <= standard_count) {
		for(l = 0;l < 8;l++)
			lab_duration[l] = 0;
		for(l = 0;l < std_list->head->lab_subject_count;l++)
			lab_duration[std_list->head->lab_subject_credits[l]]++;
		for(l = 0;l < 8;l++) {
			if(lab_duration[l] == 0)
				continue;
			if(batch > lab_duration[l])
				BIG = batch;
			else
				BIG = lab_duration[l];
			min = 1;
			std_slots = hour = 0;
			j = 0;
			while(std_slots < BIG) {
				max = daily_slots - l;
				cnt = flag = 0;
				while(!flag) {
					if(cnt == 8)
						break;
					cnt++;
					rnd = random() % max;
					switch(l) {
						case 1: {
							if(!rooms[i][j][rnd])
								flag = 1;
							break;
						}
						case 2: {
							if((!rooms[i][j][rnd] && !rooms[i][j][rnd + 1]) && 
								!((rnd < lunch) && ((rnd + 1) >= lunch)))
								flag = 2;
							break;
						}
						case 3: {
							if((!rooms[i][j][rnd] && !rooms[i][j][rnd + 1] && !rooms[i][j][rnd + 2]) &&
								 !((rnd < lunch) && ((rnd + 2) >= lunch)))
								flag = 3;
							break;
						}
						case 4: {
							if((!rooms[i][j][rnd] && !rooms[i][j][rnd + 1] && !rooms[i][j][rnd + 2] && 
								!rooms[i][j][rnd + 3]) && !((rnd < lunch) && ((rnd + 3) >= lunch)))
								flag = 4;
							break;
						}
					}
					if(flag) {
						for(m = 1;m <= flag;m++) {
							rooms[i][j][rnd] = -(1000 * std_no + 100 * flag + std_slots);
							std_list->head->lab_slot_allotted[std_slots][hour] = 1000 * i + 10 * rnd + j;
							rnd++;
							hour++;
						}
						std_slots++;
					}
				}
				j = (j+1) % working_days;
			}
		}
		std_no++;
		i++;
		std_list->head = std_list->head->next;
	}
	
	std_list->head = q;
	i = j = k = 0;
	std_no = class = 1;

	while(class <= standard_count) {
		min = std_list->head->sem_credits / working_days;
		max = daily_slots;
		std_slots = sub_no = 0;
		j = 0;

		while(std_slots < std_list->head->sem_credits) {
			daily_slot_count = cnt = 0;
			for(k = 0;k < min;k++) {
				cnt++;
				flag = flag1 = 0;
				if(cnt == 16)
					break;

				rnd = random() % max;
				if(daily_slot_count == daily_slots)
					break;
				if(rooms[i][j][rnd]) {
					k--;
					continue;
				}
				if((std_list->head->subject_credits[sub_no] > std_list->head->credits_allotted[sub_no]) && 					(std_slots < std_list->head->sem_credits)) {
					for(check = 0;check < standard_count;check++) {
						holder = rooms[check][j][rnd] / 1000;
						sub_no2 = (rooms[check][j][rnd] % 1000) / 10; 
						if((holder > 0) && (check != i)) {
							LIST_HEAD = q;
							holder--;
							while(holder) {
								LIST_HEAD = LIST_HEAD->next;
								holder--;
							}
							if(strcmp(LIST_HEAD->teacher_ID[sub_no2], std_list->head->teacher_ID[sub_no]) == 0) {
								flag1 = 1;
							}
						}
					}
					for(check = 0;check < daily_slots;check++)
						if(rooms[i][j][check] == (1000 * std_no + 10 * sub_no))
							flag = 1;
					if(flag || flag1)
						continue;
					rooms[i][j][rnd] = 1000 * std_no + 10 * sub_no;
					std_list->head->slot_allotted[sub_no][std_list->head->credits_allotted[sub_no]] = 1000 * ((i % 														classrooms) + 1) + 10 * rnd + j;
					std_list->head->credits_allotted[sub_no]++;
					sub_no = (sub_no + 1) % std_list->head->subject_count;
					daily_slot_count++;
					std_slots++;
				}
				if(std_list->head->subject_credits[sub_no] == std_list->head->credits_allotted[sub_no])
					sub_no = (sub_no + 1) % std_list->head->subject_count;
			}
			j = (j+1) % working_days;
		}
		std_no++;
		i++;
		class++;
		std_list->head = std_list->head->next;
	}
	std_list->head = q;
	
}

/*
 * This function folds the the no of standard list(2 * classrooms) and superimpose two standards in a same classroom.
 * It does all the adjustments with the overlapped lectures.
 * It produces final non overlapped timetable of (2 * classrooms) # of standards.
 * It takes all the list pointers as argument along with the 3-D array rooms which contains alloted slots to different standards.
 */

void superposition(int ***rooms, int ***std_tt, subject_list *sub_list, subject_list *lab_sub_list, standard_list *std_list, teacher_list *teach_list, int working_days, int daily_slots, int classrooms, int batch, int day_start, int duration) {
	
	int i, j, k, l, m, n, flag = 1, day, cnt, slot_no = 0, free_slot_no, full_slot_no, ***free_slots, ***full_slots, classrm_diff, 			equality = 1, check = 1, check1, check2 = 1, residue = 0, argc, holder, sub_no, flag1 = 1, sub_no2, standard_count;
	static int ccc = 1;
	standard_info *p, *q, *list_start, *LIST_HEAD, *LIST_HEAD2;
	
	if(std_list->standard_count > (2 * classrooms))
		standard_count = 2 * classrooms;
	else
		standard_count = std_list->standard_count;
	
	free_slots =(int ***)malloc(standard_count * sizeof(int **)); 
	full_slots = (int ***)malloc(standard_count * sizeof(int **));
	for(i = 0;i < standard_count;i++) {
		free_slots[i] = (int **)malloc(working_days * standard_count * sizeof(int *)); 
		full_slots[i] = (int **)malloc(working_days * standard_count * sizeof(int *));
		for(j = 0;j < (working_days * standard_count);j++) {
			free_slots[i][j] = (int *)malloc(daily_slots * sizeof(int));
			full_slots[i][j] = (int *)malloc(daily_slots * sizeof(int));
		}
	}
	for(i = 0;i < standard_count;i++)
		for(j = 0;j < (working_days * standard_count);j++)
			for(k = 0;k < daily_slots;k++) {
				full_slots[i][j][k] = free_slots[i][j][k] = -1;}
	/*
	 * Checks the overlapping and keep the records of overlapped slots.
	 * It also keep the record of free slots in corresponding two standards.
	 */

	cnt = classrooms;
	for(i = 0;i < classrooms;i++) {
		if(cnt < standard_count)
			j = classrooms + i;
		else
			continue;
		free_slot_no = full_slot_no = 0;
		for(day = 0;day < working_days;day++) {
			for(k = 0;k < daily_slots;k++) {
				if((rooms[i][day][k] < 0) ^ (rooms[j][day][k] < 0)) {
					if((rooms[i][day][k] < 0) && (rooms[j][day][k] <= 0))
						free_slots[i][day][k] = rooms[i][day][k];
					else if((rooms[j][day][k] < 0) && (rooms[i][day][k] <= 0))
						free_slots[i][day][k] = rooms[j][day][k];
				}
				if((rooms[i][day][k] == 0) && (rooms[j][day][k] == 0)){
					free_slots[i][day][k] = rooms[i][day][k];
				}
				else if((rooms[i][day][k] > 0) && (rooms[j][day][k] > 0)){
					full_slots[i][day][k] = rooms[i][day][k];
					full_slots[j][day][k] = rooms[j][day][k];
				}
			}
		}
		cnt++;
	}
	
	list_start = std_list->head;
	for(day = 0;day < working_days;day++) {
		p = q = list_start;
		for(classrm_diff = 1;classrm_diff < classrooms;classrm_diff++)
			q = q->next;
			cnt = classrooms;
		for(i = 0;i < classrooms;i++) {
			if(cnt >= standard_count)
				break;
			j = i;
			k = 0;
			for(k = 0;k < daily_slots;k++) {
				if(k == 4)
					continue;
				l = 0;
				if(full_slots[i][day][k] != -1) {
					while((free_slots[j][day][l] == -1) && (l < daily_slots))
						l++;
					if(l < daily_slots) {
					while(l < daily_slots) {
						if((free_slots[j][day][l] < -1) && flag) {
							if((-free_slots[j][day][l] / 1000) == (i + 1))
								equality = 0;
							else if((-free_slots[j][day][l] / 1000) == (i + 1 + classrooms))
								equality = 1;
						}
						if(equality % 2) {
							if(!flag) {
								for(check1 = 0;check1 < daily_slots;check1++) {
									if(rooms[j][day][check1] == rooms[i][day][k]) {
										check = 0;
										break;
									}
									else
										check = 1;
								}
								for(check1 = 0;check1 < standard_count;check1++) {
									if(((rooms[check1][day][l] / 1000) == (i + 1)) || 
										((std_tt[check1][day][l] / 1000) == -(i + 1))) {				
										check2 = 0;
										break;
									}
									else
										check2 = 1;
								}
							}
							for(check1 = 0;check1 < standard_count;check1++) {
								holder = rooms[check1][day][l] / 1000;
								sub_no = (rooms[check1][day][l] % 1000) / 10;
								sub_no2 = (full_slots[i][day][k] % 1000) / 10;
								if((holder > 0) && (check1 != j)) {
									LIST_HEAD = LIST_HEAD2 = list_start;
									holder--;
									while(holder) {
										LIST_HEAD = LIST_HEAD->next;
										holder--;
									}
									holder = 0;
									while(holder < i) {
										LIST_HEAD2 = LIST_HEAD2->next;
										holder++;
									}
									if(strcmp(LIST_HEAD->teacher_ID[sub_no], LIST_HEAD2->teacher_ID[sub_no2]) == 0){
										flag1 = 0;
										break;
									}else
										flag1 = 1;
								}
							}
							if(check && flag1 && check2) {
								m = (full_slots[i][day][k] % 1000) / 10;
								n = (full_slots[i][day][k] % 1000) % 10;
								std_list->head = p;
								std_list->head->slot_allotted[m][n] = 1000 * ((j % classrooms) + 1)+ 10 * day
													+ l;
								rooms[j][day][l] = 1000 * (i + 1) + 10 * m + n;
								rooms[i][day][k] = 0;
								free_slots[j][day][l] = -1;
								equality++;
								check = flag = flag1 = 1;
								residue = 0;
								break;
							}else {
								l++;
								while((free_slots[j][day][l] == -1) && (l < daily_slots))
									l++;
							}	
						}
						else {
							if(!flag) {
								for(check1 = 0;check1 < daily_slots;check1++) {
									if(rooms[j][day][check1] == rooms[i + classrooms][day][k]) {
										check = 0;
										break;
									}
									else
										check = 1;
								}
								for(check1 = 0;check1 < standard_count;check1++) {
									if(((rooms[check1][day][l] / 1000) == (i + classrooms + 1)) || 
										((std_tt[check1][day][l] / 1000) == -(i + classrooms + 1))) {
										check2 = 0;
										break;
									}
									else
										check2 = 1;
								}
							}
							for(check1 = 0;check1 < standard_count;check1++) {
								holder = rooms[check1][day][l] / 1000;
								sub_no = (rooms[check1][day][l] % 1000) / 10;
								sub_no2 = (full_slots[i + classrooms][day][k] % 1000) / 10;
								if((holder > 0) && (check1 != j)) {
									LIST_HEAD = LIST_HEAD2 = list_start;
									holder--;
									while(holder) {
										LIST_HEAD = LIST_HEAD->next;
										holder--;
									}
									holder = 0;
									while(holder < (i + classrooms)) {
										LIST_HEAD2 = LIST_HEAD2->next;
										holder++;
									}
									if(strcmp(LIST_HEAD->teacher_ID[sub_no], 
										LIST_HEAD2->teacher_ID[sub_no2]) == 0){
										flag1 = 0;
										break;
									}else
										flag1 = 1;
								}
							}				
							if(check && flag1 && check2) {
								m = (full_slots[i + classrooms][day][k] % 1000) / 10;
								n = (full_slots[i + classrooms][day][k] % 1000) % 10;
								std_list->head = q;
								std_list->head->slot_allotted[m][n] = 1000 * ((j % classrooms) + 1) + 10 * day
													+ l;
								rooms[j][day][l] = 1000 * (i + 1 + classrooms) + 10 * m + n;
								rooms[i + classrooms][day][k] = 0;
								free_slots[j][day][l] = -1;
								equality++;
								check = flag = flag1 = 1;
								residue = 0;
								break;
							}else {
								l++;
								while((free_slots[j][day][l] == -1) && (l < daily_slots))
									l++;
								
							}
						}
					}
					}
					if(l >= daily_slots) {
						residue++;
						if(residue == (daily_slots * classrooms)) {
							residue = 0;
							standard_database(std_list);
							CONTROLLER(lab_sub_list, sub_list, teach_list, std_list, classrooms, working_days, 										daily_slots, batch, day_start, duration);
							exit(1);
						}
						check = check2 = flag = 0;
						k--;
						j = (j + 1) % classrooms;
					}
				}
			}
			cnt++;
			q = q->next;
			p = p->next;
		}
	}

	std_list->head = list_start;
	
}

/*
 * It allocates a 3-D array which stores the timetable of all the standards in saparate blocks of array.
 */

int ***init_std_tt(standard_list *std_list, int working_days, int daily_slots, int classrooms) {

	int i, j, ***std_tt;
	std_tt = (int ***)malloc(std_list->standard_count * sizeof(int **));
	for(i = 0;i < std_list->standard_count;i++) {
		std_tt[i] = (int **)malloc(working_days * sizeof(int *));
		for(j = 0;j < (working_days);j++) 
			std_tt[i][j] = (int *)malloc(daily_slots * sizeof(int));      
	}
	return std_tt;
}

/*
 * It separates the combined timetable which is stored in ***rooms and stores it systematically in ***std_tt.
 * It takes given list pointers and ***rooms and ***std_tt for separate rooms alocation.
 */

void slot_separator(int ***rooms, int ***std_tt, subject_list *sub_list, standard_list *std_list, int working_days, int daily_slots, int 			classrooms, int reset) {
	
	int i, j, k, l, temp, flag, standard_count;
	standard_info *p = std_list->head, *q;

	if(std_list->standard_count > (2 * classrooms))
		standard_count = 2 * classrooms;
	else
		standard_count = std_list->standard_count;
	if(reset != 2) {
		for(i = 0;i < standard_count;i++)
			for(j = 0;j < (working_days);j++)
				for(k = 0;k < (daily_slots);k++) {
					if(reset) {
						if((rooms[i][j][k] / 1000) > 0)
							std_tt[(rooms[i][j][k] / 1000) - 1][j][k] = rooms[i][j][k];
					}
					else if((rooms[i][j][k] / 1000) < 0)
						std_tt[-(rooms[i][j][k] / 1000) - 1][j][k] = rooms[i][j][k];
				}
		
	}else {
		for(i = 0;i < (std_list->standard_count - (2 * classrooms));i++)
			for(j = 0;j < (working_days);j++) 
				for(k = 0;k < (daily_slots);k++) {
					if((rooms[i][j][k] / 1000) > 0)
						std_tt[(rooms[i][j][k] / 1000) - 1][j][k] = rooms[i][j][k];
					else if((rooms[i][j][k] / 1000) < 0)
						std_tt[-(rooms[i][j][k] / 1000) - 1][j][k] = rooms[i][j][k];
				}
		
	}
	
}
