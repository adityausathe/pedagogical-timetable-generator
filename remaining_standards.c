
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

#include "input.h"

/*
 * It allots slots to all remaining standards (i.e. standard_count - (2 * classrooms)).
 * It first allots their labs and then lectures in the free slots of partially filled classrooms.
 * It takes all the list pointers as input, ***rooms and other paramters.
 * It get called only when the standards are more than twice the no. of claarooms.
 * So, it recieves 'standard_count'.
 * It returns a 3-D array pointer of newly allocated rooms.
 */
 
int ***remaining_standards(int ***rooms, subject_list *th_sub_list, subject_list *lab_sub_list, standard_list *std_list, 
		teacher_list *teach_list, int working_days, int classrooms, int daily_slots, int batch, int standard_count, int day_start, 
		int duration) {
	
	int ***remaining_rooms, slot_no, a[16], min, max, i, j, k, m, rnd, std_slots, std_no, daily_slot_count, sub_no, class, flag, flag1, 			day, hour, cnt, l, holder, batch_no, BIG, lab_duration[8], check, sub_no2, lunch_allot;     
	static int ccc = 0;

	time_t tt;
	srandom(time(&tt));
	standard_info *q, *LIST_HEAD, *LIST_HEAD2, *r = std_list->head;

	remaining_rooms = (int ***)malloc(standard_count * sizeof(int **));
	for(i = 0;i < standard_count;i++) {
		remaining_rooms[i] = (int **)malloc(working_days * sizeof(int *));
		for(j = 0;j < (working_days);j++) 
			remaining_rooms[i][j] = (int *)malloc(daily_slots * sizeof(int));      
	}
	for(i = 0;i < standard_count;i++) {
		for(j = 0;j < (working_days);j++) 
			for(k = 0;k < daily_slots;k++) {
				remaining_rooms[i][j][k] = 0;
			}
	}	

	for(i = 0;i < (2 * classrooms);i++)
		std_list->head = std_list->head->next;
	
	q = std_list->head;
	
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
							if(!remaining_rooms[i][j][rnd])
								flag = 1;
							break;
						}
						case 2: {
							if((!remaining_rooms[i][j][rnd] && !remaining_rooms[i][j][rnd + 1]) && !((rnd < lunch) 									&& ((rnd + 1) >= lunch)))
								flag = 2;
							break;
						}
						case 3: {
							if((!remaining_rooms[i][j][rnd] && !remaining_rooms[i][j][rnd + 1] && 									!remaining_rooms[i][j][rnd + 2]) && !((rnd < lunch) && ((rnd + 2) >= lunch)))
								flag = 3;
							break;
						}
						case 4: {
							if((!remaining_rooms[i][j][rnd] && !remaining_rooms[i][j][rnd + 1] && 				!remaining_rooms[i][j][rnd + 2] && !remaining_rooms[i][j][rnd + 3]) && !((rnd < lunch) && ((rnd + 3) >= lunch)))
								flag = 4;
							break;
						}
					}
					if(flag) {
						for(m = 1;m <= flag;m++) {
							remaining_rooms[i][j][rnd] = -(1000 * (2 * classrooms + std_no) + 100 * flag + 												std_slots);
							std_list->head->lab_slot_allotted[std_slots][hour] = 1000 * (2 * classrooms + i) + 
														10 * rnd + j;
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
		std_slots = sub_no = cnt = 0;
		j = l = 0;
		while((std_slots < std_list->head->sem_credits) && (l < classrooms)) {
			daily_slot_count = 0;
			
			for(k = 0;k < daily_slots;k++){
				cnt++;
				flag = flag1 = 0;
				rnd = k;

				if(daily_slot_count == daily_slots)
					break;
				if((remaining_rooms[i][j][rnd]) || (((rooms[l][j][rnd] > 0) || (rooms[l + classrooms][j][rnd] > 0)))) {
					continue;
				}
				if((std_list->head->subject_credits[sub_no] > std_list->head->credits_allotted[sub_no]) && 					(std_slots < std_list->head->sem_credits)) {
					for(check = 0;check < standard_count;check++) {
						holder = (remaining_rooms[check][j][rnd] / 1000) - (2 * classrooms);
						sub_no2 = (remaining_rooms[check][j][rnd] % 1000) / 10;
						if((holder > 0) && (check != i)) {
							LIST_HEAD = q;
							holder--;
							while(holder) {
								LIST_HEAD = LIST_HEAD->next;
								holder--;
							}
							if(strcmp(LIST_HEAD->teacher_ID[sub_no2], std_list->head->teacher_ID[sub_no]) == 0)
								flag1 = 1;
						}
					}
					for(check = 0;check < (2 * classrooms);check++) {
						holder = rooms[check][j][rnd] / 1000;
						sub_no2 = (rooms[check][j][rnd] % 1000) / 10;
						if((holder > 0) && (check != l)) {
							LIST_HEAD2 = r;
							holder--;
							while(holder) {
								LIST_HEAD2 = LIST_HEAD2->next;
								holder--;
							}
							if(strcmp(LIST_HEAD2->teacher_ID[sub_no2], std_list->head->teacher_ID[sub_no]) == 0)
								flag1 = 1;
						}
					}
					for(check = 0;check < daily_slots;check++)
						if((remaining_rooms[i][j][check] == (1000 *(2 * classrooms + std_no) + 10 * sub_no)))
							flag = 1;
					
					if(flag || flag1)
						continue;

					remaining_rooms[i][j][rnd] = rooms[l][j][rnd] = 1000 * (2 * classrooms + std_no) + 10 * sub_no;
					std_list->head->slot_allotted[sub_no][std_list->head->credits_allotted[sub_no]] = 
												1000 * ((l % classrooms)+ 1) + 10 * rnd + j;
					std_list->head->credits_allotted[sub_no]++;
					sub_no = (sub_no + 1) % std_list->head->subject_count;
					daily_slot_count++;
					std_slots++;
				}
				if(std_list->head->subject_credits[sub_no] == std_list->head->credits_allotted[sub_no])
					sub_no = (sub_no + 1) % std_list->head->subject_count;
			}
			j = (j+1) % working_days;
			lunch_allot = 1;
			if(j == 0)
				l++; 
		}
		if(std_slots < std_list->head->sem_credits) {
			standard_database(std_list);
			CONTROLLER(lab_sub_list, th_sub_list, teach_list, std_list, classrooms, working_days, daily_slots, batch, 
					day_start, duration);
		
			exit (1);		
		}

		std_no++;
		i++;
		class++;
		std_list->head = std_list->head->next;
	}
	std_list->head = r;
	
	return remaining_rooms;

}
