
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
 * It creates final printable timetable for each teacher.
 * It get saved in a file of teacher's name so he/she can know when and where is the lecture.
 * The output file is coded in .csv format so it can be opened in spreadsheet with the constraints given in the README.txt.
 */
 
void teacher_schedule(int ***std_tt, int ***rooms, teacher_list *teach_list, standard_list *std_list, int working_days, int daily_slots, 
			int classrooms, int day_start, int duration) {
	
	char token[512], teach_name[32], time_slots[1024], cell_content[32];
	FILE *fp;
	int holder, i, j, k, class, l, standard_count, start, end, had_lunch;
	float s, e;
	standard_info *p = std_list->head;
	teacher_info *q = teach_list->head, *r;
	
	if(std_list->standard_count > (2 * classrooms))
		standard_count = 2 * classrooms;
	else
		standard_count = std_list->standard_count;

	p = std_list->head;

	for(i = 0;i < std_list->standard_count;i++) {
		for(j = 0;j < working_days;j++) {
			for(k = 0;k < daily_slots;k++) {
				if(std_tt[i][j][k] > 0) {
					teach_list->head = q;					
					while(teach_list->head) {					
						if(strcmp(teach_list->head->teacher_ID, p->teacher_ID[(std_tt[i][j][k] % 100) / 10]) == 0) {
							holder = std_tt[i][j][k] % 1000;
							for(l = 0;l < standard_count;l++)
								if((rooms[l][j][k] / 1000) == (i + 1))
									class = (l % classrooms) + 1;
							strcpy(teach_list->head->schedule[j][k], p->subject_ID[(std_tt[i][j][k] % 100) / 10]);
							
							sprintf(token, " class %s room %d", p->standard_name, class);
							strcat(teach_list->head->schedule[j][k], token);
							break;
						}
						teach_list->head = teach_list->head->next;
					}
				}
			}
		}
		p = p->next;
	}
	r = teach_list->head = q;

	strcpy(time_slots, "DAY,");
	end = day_start;
	had_lunch = 1;

	for(i = 0;i < daily_slots;i++) {
		start = end;
		if((i == lunch ) && had_lunch) {
			end = start + 30;
			i--;
			had_lunch = 0;
		}else
			end = start + duration;	
		s = start / 60 + (float)(0.01 * (start % 60));
		e = end / 60 + (float)(0.01 * (end % 60));
		sprintf(token, "%.2f - %.2f,", s, e);
		strcat(time_slots, token);
	}
	strcat(time_slots, "\n");

	for(i = 0;i < teach_list->teacher_count;i++) {
		strcpy(teach_name, "Teacher-wise_schedule/");
		strcat(teach_name, r->teacher_name);
		strcat(teach_name, ".csv");
		fp = fopen(teach_name, "w");
		if(fp == NULL) {
			perror("\n\n\n\n\n\n\n\n\n\t\t\t\t\t\tDirectory -'Teacher-wise_schedule' not found.\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			exit (1);
		}
		sprintf(cell_content, ",,,*************, TEACHER NAME-, %s,************\n\n", r->teacher_name);
		fprintf(fp, "%s", cell_content);
		fprintf(fp, "%s", time_slots);

		for(j = 0;j < (working_days);j++) {
			switch(j) {
				case 0: {
					strcpy(cell_content, "MONDAY,");
					fprintf(fp, "%s", cell_content);
					break;
				}
				case 1: {
					strcpy(cell_content, "TUESDAY,");
					fprintf(fp, "%s", cell_content);
					break;
				}
				case 2: {
					strcpy(cell_content, "WEDNESDAY,");
					fprintf(fp, "%s", cell_content);
					break;
				}
				case 3: {
					strcpy(cell_content, "THURSDAY,");
					fprintf(fp, "%s", cell_content);
					break;
				}
				case 4: {
					strcpy(cell_content, "FRIDAY,");
					fprintf(fp, "%s", cell_content);
					break;
				}
				case 5: {
					strcpy(cell_content, "SATURDAY,");
					fprintf(fp, "%s", cell_content);
					break;
				}
			}
			had_lunch = 1;
			for(k = 0;k < daily_slots;k++) {
				if((k == lunch) && had_lunch) {
					k--;
					had_lunch = 0;					
					strcpy(cell_content, "LUNCH,");
					fprintf(fp, "%s", cell_content);
				}
				else if(r->schedule[j][k]) {
					fprintf(fp, "%s", r->schedule[j][k]);
					fputc(',', fp);
				} else
					fputc(',', fp);
			}
			fputc('\n', fp);
		}
		fclose(fp);
		r = r->next;
	}
}
