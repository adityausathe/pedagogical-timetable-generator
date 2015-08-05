
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
 * This controls CONTROLLER(...) and other functions.
 * It also accepts input files as command line arguments.
 * It computes lunch time, period duration, etc terms.
 * It accepts some user fed data.
 */

int main(int argc, char *argv[]){
	
	int daily_slots , working_days, i, j, k, classrooms, done = 0, batch, start_hr, start_min, close_hr, 
		close_min, day_start, duration;
	static int flag = 1;
	subject_list th_subjects, lab_subjects;
	teacher_list teachers;
	standard_list standards;

	if(flag) {
		for(i = 0;i < 4;i++)
			strcpy(input_file[i], argv[i]);
		flag = 0;
	}

	system("clear");
	printf("\n\t\t\t\t\t\t\t\tSCHEDULE - GENERATOR\n");
	printf("\n\n\t*************************************************************************************************************************\n\n");

	printf("\n\n\n\t\t\t>> ENTER OPENING TIME OF SCHOOL (24 hr format) in\n\t\t\t\thour minutes\n\t\t\t\t");
	scanf("%d %d", &start_hr, &start_min);
	printf("\n\n\n\t\t\t>> ENTER CLOSING TIME OF SCHOOL (24 hr format) in\n\t\t\t\thour minutes\n\t\t\t\t");
	scanf("%d %d", &close_hr, &close_min);
	printf("\n\n\n\t\t\t>> ENTER NUMBER OF CLASSROOMS WHICH ARE FULLY AVAILABLE.\n\t\t\t\t");
	scanf("%d", &classrooms);
	printf("\n\n\n\t\t\t>> ENTER NUMBER OF WORKING DAYS IN A WEEK.\n\t\t\t\t");
	scanf("%d", &working_days);
	printf("\n\n\n\t\t\t>> ENTER LECTURE DURATION IN MINUTES.\n\t\t\t\t");
	scanf("%d", &duration);
	printf("\n\n\n\t\t\t>> ENTER NUMBER OF LAB BATCHES.\n\t\t\t\t");
	scanf("%d", &batch);

	printf("\n\n\t*************************************************************************************************************************\n\n");
	DELAY();
	system("clear");
	if(start_hr <= 12)
		lunch = ((12 - start_hr) * 60) / duration;
	else
		lunch = 0;
	
	day_start = 60 * start_hr + start_min;
	daily_slots = (int)((close_hr - start_hr) * 60) / duration;

	subject_teacher_database(&lab_subjects, &th_subjects, &teachers);

	standard_database(&standards);

	CONTROLLER(&lab_subjects, &th_subjects, &teachers, &standards, classrooms, working_days, daily_slots, batch, day_start, 			duration);
	
	destroy_database(&lab_subjects, &th_subjects, &teachers, &standards);
	
	system("clear");	

	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\tSCHEDULE HAS BEEN GENEARATED AND SAVED SUCCESSFULLY!!!\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

	return 0;
}
