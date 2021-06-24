CC = gcc
OBJS = main.o course_manager.o Course.o Semester.o student.o Grade_sheet.o
ADTS = course_manager.o Course.o Semester.o student.o Grade_sheet.o 
EXEC = mtm_cm
DEBUG_FLAG = -g -DNDEBUG
CFLAGS = -std=c99 -Wall -Werror -pedantic-errors $(DEBUG_FLAG)
LINK = -L. -lmtm 
OBJS_TESTS = course_manager_test Course_test Semester_test Grade_sheet_test student_test List_mtm_test
O_TESTS = student_test.o Semester_test.o Grade_sheet_test.o Course_test.o course_manager_test.o
O_FILES = $(O_TESTS) $(OBJS)
TEST_PATH=./tests/
LIST_PATH=./list_mtm/

TESTS = tests

$(EXEC) : $(OBJS)
		$(CC) $(DEBUG_FLAG) $(OBJS) -o $@ $(LINK)
$(TESTS) : $(OBJS_TESTS)
	
main.o: main.c mtm_ex3.h course_manager.h set.h student.h Grade_sheet.h \
 Semester.h list.h Course.h
		$(CC) -c  $(CFLAGS) main.c $(LINK)
		
#adts_o
course_manager.o: course_manager.c set.h student.h Grade_sheet.h \
 Semester.h list.h Course.h mtm_ex3.h course_manager.h
		$(CC) -c  $(CFLAGS) $*.c $(LINK)
	
Course.o: Course.c Course.h list.h
		$(CC) -c  $(CFLAGS) $*.c $(LINK)

Grade_sheet.o: Grade_sheet.c Semester.h list.h Course.h Grade_sheet.h
		$(CC) -c $(CFLAGS) $*.c $(LINK)

student.o: student.c student.h Grade_sheet.h Semester.h list.h Course.h \
 set.h
		$(CC) -c  $(CFLAGS) $*.c $(LINK)

Semester.o: Semester.c list.h Course.h Semester.h
		$(CC) -c  $(CFLAGS) $*.c $(LINK)
		
#list_o
list_mtm.o : list_mtm/list_mtm.c list_mtm/list_mtm.h
		$(CC) -c $(CFLAGS) list_mtm/list_mtm.c
			
Semester_test : Semester_test.o	Semester.o
		$(CC) $(ADTS) Semester_test.o  $(DEBUG_FLAG) $(LINK) -o $@	

		
#Adts_tests_run
course_manager_test : course_manager_test.o course_manager.o
		$(CC) course_manager_test.o $(ADTS) $(DEBUG_FLAG) -o $@ $(LINK)

	
Course_test : Course_test.o Course.o
		$(CC) Course_test.o $(ADTS) $(DEBUG_FLAG)  -o $@ $(LINK)
	
				
student_test : student_test.o student.o 
		$(CC) student_test.o $(ADTS) $(DEBUG_FLAG) -o $@ $(LINK)

	
Grade_sheet_test : Grade_sheet_test.o Grade_sheet.o
		$(CC) Grade_sheet_test.o $(ADTS) $(DEBUG_FLAG) -o $@ $(LINK)
#list_test
List_mtm_test  :list_mtm.o  list_mtm_test.o 
		$(CC) list_mtm_test.o list_mtm.o $(DEBUG_FLAG) -o $@ 
		
#tests o
course_manager_test.o: course_manager_test.c set.h student.h \
 Grade_sheet.h Semester.h list.h Course.h mtm_ex3.h course_manager.h \
 test_utilities.h
		$(CC) -c  $(CFLAGS) course_manager.o $(TEST_PATH)course_manager_test.c $(LINK) -o $@ 

Course_test.o: Course_test.c Course.h list.h test_utilities.h
		$(CC) -c  $(CFLAGS) $(TEST_PATH)Course_test.c 	
	
Grade_sheet_test.o: Grade_sheet_test.c Semester.h list.h Course.h \
 Grade_sheet.h test_utilities.h
		$(CC) -c  $(CFLAGS) $(TEST_PATH)Grade_sheet_test.c 

Semester_test.o: Semester_test.c list.h Course.h Semester.h \
 test_utilities.h
		$(CC) -c  $(CFLAGS) $(TEST_PATH)Semester_test.c 
	
student_test.o: student_test.c student.h Grade_sheet.h Semester.h list.h \
 Course.h set.h test_utilities.h
		$(CC) -c  $(CFLAGS) $(TEST_PATH)student_test.c 	
		
list_mtm_test.o    : $(TEST_PATH)list_mtm_test.c $(TEST_PATH)test_utilities.h ./list_mtm/list_mtm.h
		$(CC) -c  $(CFLAGS)  $(TEST_PATH)list_mtm_test.c 

clean:
		rm -f  $(O_FILES) $(EXEC)