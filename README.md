# timetable-genearator


---------------------------------------------------------------------------------------------------------------
- I've tried to solve the problem by simple recursive algorithm.
- I wanted to solve it by using 'Genetic Algorithm' concept.
- But as it is new for me so, I more focused on developing a familiar recursive algorithm.
- This program takes three files as input(command line arguments) containing standard lists, subject lists 
  and teacher information.

# FILEWISE DESCRIPTION:

- In file input.c, it creates lists from the given input files.
- In file generator.c, it creates a complete schedule of standards = (2 * classrooms) i.e. twice the number
  of classrooms.
- In file remaining_standards.c, it creates schedule of remainig standards by filling the free slots in the
  prior allotted classrooms. 
- In file print_tt.c, it decodes the information stored about the subject and and corresponding standard 
  and exports all the information to .csv files which is readable. The files are in the name of each standard
  and are separate.
- The file teacher_schedule.c, it does same function as print_tt.c but it produces timetable for each teacher
  in separate file in the name of that teacher.

---------------------------------------------------------------------------------------------------------------
# INPUT FILES:

- The input file format is shown in sample files given.
- The '"' symbol should be necessarily used.
- While entering subject_initials take care of 'spaces'.
- Please read instructions given in input files.
- A demo screenshot of terminal based input is attached - 'terminal-input.png'. Go through that.
- While opening .csv files the required setting is mentioned in the screenshot 'spreadsheet-setting.png'.
- The input should be given strictly as follows. '$' symbol specifies lab subjects. 
  The lab hours are actually the no of lecture slots required to take the lab. 
  Strictly follow the format Don't use spaces in ID. Shown in 'subject_ip_coep'.
- The names and other data should be given in double qoutes as shown below. subject_ID as shown in 'teacher_ip_coep'. 
  Strictly follow the format Don't use spaces in ID.
- The standard names should be given in double qoutes as shown below. subject_ID as shown in 'standard_ip_coep'. 

# LABS:

- For labs only required slots are alloted as per lab subjects and no. of batches.
  As the lab allotmant will be another problem which couuld be better solved manually. 
- There are many parameters about lab allotment, i.e. how many no. of batches can attend lab simultanously.
  So, I've only given the required lab slots per standards the user has to arrange the batches as per their 
  convinience.

#CAUTION:

- The formed schedule files should be copied to other directory as running the program again overwrite the 
  previous files.
- If the failure warning continues after running program many times then try changing the order of input 
  standards in the standard-input file.
- If still it is not working, then reduce some subjects from some standard so it will give partial schedule.
----------------------------------------------------------------------------------------------------------------
# IMPORTANT:
	 A HALF HOUR BUFFER LUNCH TIME IS NEEDED.
- So, some lectures might run half hour extra after the regular institute timings.
- Rooms numbers are arbitrary. You can assign room names by editing the output file.
- Output file contains: room 1, room 2, etc.
----------------------------------------------------------------------------------------------------------------

# NOTE:

--FOR GIVEN SAMPLE INPUT FILES, ENTER TERMINAL BASED DATA AS GIVEN IN THE SCEENSHOT.(It will fit that well.)
