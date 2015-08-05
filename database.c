
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
 * This file consists of functions which control the whole process, from input to output generation.
 * It lacks main() function.
 */

#include "input.h"

/*
 * This function calls the input functions for reading subject data and standard data
 * It takes empty subject list(both theory and lab) and teacher list pointers as arguments and creates them. 
 */
void subject_teacher_database(subject_list *lab_sub_list, subject_list *th_sub_list, teacher_list *teach_list) {
	
	init_subject_list(lab_sub_list);
	init_subject_list(th_sub_list);
	init_teacher_list(teach_list);
	input_subject(lab_sub_list, th_sub_list, input_file[1]);
	input_teacher(teach_list, input_file[2]);
	teach_sub(th_sub_list, teach_list);	
}

/*
 * This function calls the input functions for reading standard data.
 * This is separared from the above function because it may be recursively called in future.
 * It takes an empty standard list pointer as argument and creates it.
 */
void standard_database(standard_list *std_list) {
	
	init_standard_list(std_list);
	input_standard(std_list, input_file[3]);
	
}

/*
 * This is control function it handles all the function calls to the generating functions.
 * As this is heart function it takes all list pointers as arguments along with daily available slots, institute starting time, lecture       	 duration, working days per week, no of batches for lab,etc; from main() function.
 */
void CONTROLLER(subject_list *lab_sub_list, subject_list *th_sub_list, teacher_list *teach_list, standard_list *std_list, int classrooms, int 			working_days, int daily_slots, int batch, int day_start, int duration) {
	
	int ***rooms, ***remaining_rooms, ***std_tt, residual_rooms, total_lectures = 0;
	static int count = 0;
	char choice[16];
	standard_info *p = std_list->head;
	
	assign_teacher2std(th_sub_list, std_list, teach_list);

	while(p) {
		total_lectures += p->sem_credits;
		p = p->next;
	}

	/*
 	 * This condition checks the feasibility of the whole timetable process i.e. Is it possible to generate a table from given data by
	   merely doing some calculations which assumes 80% of slots can be used to generate the timetable.
	 */
 
	system("clear");

	if((total_lectures > ((((daily_slots - 1) * classrooms * working_days) * 8) / 10)) && !count) {
		printf("\n\n\n\n\n\n\t\t\tWARNING!!!\n\n\t\t\tThe classrooms or number of daily slots may be insufficient,\n\t\t\tso chances of success are LESS.\n\n\t\t\tEnter 'continue' to continue or 'abort' to try again\n\n\t\t\tRECOMMENDED: Enter 'continue': It may take 20 sec to run\n\t\t\t");
		scanf("%s", choice);
		if(strcmp(choice, "abort") == 0)		
			exit (1);
	}

	count++;
	
	//DELAY();

	system("clear");

	if(count == 3000) {
		printf("\n\n\n\n\n\n\n\n\t\t\tSORRY It seems that it is hard to fomulate a timetable from given data.\n\t\t\tPlease either increase the daily slots or try one more time by changing the order of satndards in input file\n\t\t\tIf still it is not working, sorry, but you have to reduce subjects of some standard in input file for standards.\n\n\n\n\n\n\n\n");
		
		exit (1);
	}
  	
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\tGENERATING......\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	DELAY();

	std_sub(th_sub_list, lab_sub_list, std_list);
	rooms = init_rooms(std_list, working_days, daily_slots, classrooms);
	
	separate_std_tt(rooms, th_sub_list, lab_sub_list, std_list, working_days, daily_slots, classrooms, batch);
	
	std_tt = init_std_tt(std_list, working_days, daily_slots, classrooms);
	
	slot_separator(rooms, std_tt,th_sub_list, std_list, working_days, daily_slots, classrooms, 0);
	
	superposition(rooms, std_tt, th_sub_list, lab_sub_list, std_list, teach_list, working_days, daily_slots, classrooms, batch, day_start, 				duration);
	
	system("clear");	
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t\tALMOST THERE......\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

	slot_separator(rooms, std_tt, th_sub_list, std_list, working_days, daily_slots, classrooms, 1);
	
	if(std_list->standard_count > ( 2 * classrooms)) {
		residual_rooms = std_list->standard_count - ( 2 * classrooms);
		remaining_rooms = remaining_standards(rooms, th_sub_list, lab_sub_list, std_list, teach_list, working_days, classrooms, 					daily_slots, batch, residual_rooms, day_start, duration);
		slot_separator(remaining_rooms, std_tt, th_sub_list, std_list, working_days, daily_slots, classrooms, 2);
	}

	print_tt(std_tt, rooms, th_sub_list, std_list, working_days, daily_slots, classrooms, day_start, duration);
	
	teacher_schedule(std_tt, rooms, teach_list, std_list, working_days, daily_slots, classrooms, day_start, duration);

	system("clear");	
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\tSCHEDULE HAS BEEN GENEARATED AND SAVED SUCCESSFULLY!!!\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");

	destroy_database(lab_sub_list, th_sub_list, teach_list, std_list);
}

/*
 * As this algorithm uses recursion for timetable formulation.
 * The processor needs some time lag between two recursion in order forget the previous result and start generating new result.
 * The need of this function I feel through my experience while running this program.
 */

void DELAY(void) {
	int i, j;
	for(i = 0;i < 10000000;i++) {
		j = i;
		i = j;
		i++;
	}
}
