schedule_generator: database.c main.c destroy.c generator.c remaining_standards.c print_tt.c teacher_schedule.c input.c
	mkdir Standard-wise_schedule
	mkdir Teacher-wise_schedule
	gcc -c database.c main.c destroy.c generator.c remaining_standards.c print_tt.c teacher_schedule.c input.c
	gcc -o schedule-generator database.o main.o destroy.o generator.o remaining_standards.o print_tt.o teacher_schedule.o input.o
	clear
	echo Directories created and Files compiled successfully.
