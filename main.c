#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "mtm_ex3.h"
#include "course_manager.h"
#define DONT_CARE " \t\n"
#define COMMENT '#'
#define NEW_LINE '\r'
#define NO_FILES 1 //the program name
#define ONE_FILE 3 //2 parameters are needed to define input or output files
#define TWO_FILES 5 //4 parameters are needed to define input and output files
#define GET_NEXT_PARAMETER 2
#define ACCEPT 1
#define DECLINE 0

#define INPUT_FLAG "-i"
#define OUTPUT_FLAG "-o"
MtmErrorCode SWITCH_COURSE_MANAGER_RESULT(COURSE_MANEGERResult result) {
	switch (result) {
	case COURSE_MANAGER_OUT_OF_MEMORY:
		return MTM_OUT_OF_MEMORY;
		break;
	case COURSE_MANAGER_INVALID_PARAMETERS:
		return MTM_INVALID_PARAMETERS;
		break;
	case COURSE_MANAGER_NOT_LOGGED_IN:
		return MTM_NOT_LOGGED_IN;
		break;
	case COURSE_MANAGER_ALREADY_LOGGED_IN:
		return MTM_ALREADY_LOGGED_IN;
		break;
	case COURSE_MANAGER_STUDENT_DOES_NOT_EXIST:
		return MTM_STUDENT_DOES_NOT_EXIST;
		break;
	case COURSE_MANAGER_STUDENT_ALREADY_EXISTS:
		return MTM_STUDENT_ALREADY_EXISTS;
		break;
	case COURSE_MANAGER_NOT_FRIEND:
		return MTM_NOT_FRIEND;
		break;
	case COURSE_MANAGER_ALREADY_FRIEND:
		return MTM_ALREADY_FRIEND;
		break;
	case COURSE_MANAGER_NOT_REQUESTED:
		return MTM_NOT_REQUESTED;
		break;
	case COURSE_MANAGER_ALREADY_REQUESTED:
		return MTM_ALREADY_REQUESTED;
		break;
	case COURSE_MANAGER_COURSE_DOES_NOT_EXIST:
		return MTM_COURSE_DOES_NOT_EXIST;
		break;
	case COURSE_MANAGER_MEMORY_ERROR:
		return MTM_OUT_OF_MEMORY;
		break;
	//case COURSE_MANAGER_OK:
		//return MTM_OK;
		//break;
	case COURSE_MANAGER_LIST_FRIENDS_IS_EMPTY:
	    return MTM_NOT_FRIEND;
	    break;
	case COURSE_MANAGER_SET_IS_EMPTY:
		return MTM_STUDENT_DOES_NOT_EXIST;
		break;
	case COURSE_MANAGER_STUDENT_GRADES_IS_EMPTY:
		return MTM_COURSE_DOES_NOT_EXIST;
		break;
		// this point must not be reached
	default:
		return MTM_OUT_OF_MEMORY;
		break;
	}
}
//-------------------------------------------------------------------------------------
//void removeSpaces(char buffer[]);
COURSE_MANEGERResult analyzeStudentAdd(CourseManager manager) {
	char* student_id = strtok(NULL, DONT_CARE);
	assert(student_id!=NULL);
	int  id=atoi(student_id);
	char * first_name= strtok(NULL, DONT_CARE);
	char* last_name=strtok(NULL,DONT_CARE);
	return CourseMangerAddStudent(manager, id,first_name,last_name);
}
//-------------------------------------------------------------------------------------
COURSE_MANEGERResult analyzeStudentRemove(CourseManager manager) {
	char* student_id = strtok(NULL, DONT_CARE);
	assert(student_id!=NULL);
	int id=atoi(student_id);
	return CourseMangerRemoveStudent(manager, id);
}
//-------------------------------------------------------------------------------------
COURSE_MANEGERResult analyzeStudentLogIn(CourseManager manager) {
	char* student_id=strtok(NULL,DONT_CARE);
	int id=atoi(student_id);
	return CourseMangerLogin(manager,id);
}
//-------------------------------------------------------------------------------------
COURSE_MANEGERResult analyzeStudentLogOut(CourseManager manager) {

	return CourseMangerLogout(manager);
}
//-------------------------------------------------------------------------------------
COURSE_MANEGERResult analyzeStudentFriendRequest(CourseManager manager) {
	char* friend_id=strtok(NULL,DONT_CARE);
	int other_id=atoi(friend_id);
	return CourseMangerFriendRequest(manager,other_id);
}
//-------------------------------------------------------------------------------------
COURSE_MANEGERResult analyzeStudentHandleRequest(CourseManager manager) {
	char* friend_id=strtok(NULL,DONT_CARE);
	int other_id=atoi(friend_id);
	char* action=strtok(NULL,DONT_CARE);
	return CourseMangerHandleFriendRequest(manager,other_id,action);
}
//-------------------------------------------------------------------------------------
COURSE_MANEGERResult analyzeStudentUnFriend(CourseManager manager) {
	char* friend_id=strtok(NULL,DONT_CARE);
	int other_id=atoi(friend_id);
	return CourseMangerUnFriend(manager,other_id);
}
//-------------------------------------------------------------------------------------
COURSE_MANEGERResult analyzeGradeSheetAdd(CourseManager manager) {
	char* semester_id = strtok(NULL, DONT_CARE);
	int semester=atoi(semester_id);
	char * course = strtok(NULL, DONT_CARE);
	int course_id = atoi(course);
	char* points = strtok(NULL, DONT_CARE);
	char* grade = strtok(NULL, DONT_CARE);
	int course_grade = atoi(grade);
	return CourseMangerAddGrade(manager,semester,course_id, points, course_grade);
}
//-------------------------------------------------------------------------------------
COURSE_MANEGERResult analyzeGradeSheetRemove(CourseManager manager) {
	char* semester_id = strtok(NULL, DONT_CARE);
	int semester=atoi(semester_id);
	char * course = strtok(NULL, DONT_CARE);
	int course_id = atoi(course);
	return CourseMangerRemoveGrade(manager,semester,course_id);
}
//-------------------------------------------------------------------------------------
COURSE_MANEGERResult analyzeGradeSheetUpdate(CourseManager manager) {
	char * course = strtok(NULL, DONT_CARE);
	int course_id = atoi(course);
	char* grade =strtok(NULL,DONT_CARE);
	int new_grade=atoi(grade);
	return CourseMangerUpdateGrade(manager,course_id,new_grade);
}
//-------------------------------------------------------------------------------------
COURSE_MANEGERResult analyzeReportFull(CourseManager manager,FILE* output_channel) {

	return CourseMangerReportFull(manager, output_channel);
}
//-------------------------------------------------------------------------------------
COURSE_MANEGERResult analyzeReportClean(CourseManager manager,FILE* output_channel) {

	return CourseMangerReportClean(manager, output_channel);
}
//-------------------------------------------------------------------------------------
COURSE_MANEGERResult analyzeReportBest(CourseManager manager,FILE* output_channel) {
	char* amount=strtok(NULL,DONT_CARE);
	int amount_grades=atoi(amount);
	return CourseMangerReportBest(manager,amount_grades,output_channel);
}
//-------------------------------------------------------------------------------------
COURSE_MANEGERResult analyzeReportWorst(CourseManager manager,FILE* output_channel) {
	char* amount=strtok(NULL,DONT_CARE);
	int amount_grades=atoi(amount);
	return CourseMangerReportWorst(manager,amount_grades,output_channel);
}
//-------------------------------------------------------------------------------------
COURSE_MANEGERResult analyzeReportReference(CourseManager manager,FILE* output_channel) {
	char* course=strtok(NULL,DONT_CARE);
	int course_id=atoi(course);
	char* amount=strtok(NULL,DONT_CARE);
	int amount_grades=atoi(amount);
	return CourseMangerReportReference(manager,course_id,amount_grades,output_channel);
}
//-------------------------------------------------------------------------------------
COURSE_MANEGERResult analyzeReportFacultyRequest(CourseManager manager,FILE* output_channel) {
	char* course=strtok(NULL,DONT_CARE);
	int course_id=atoi(course);
	char* request=strtok(NULL,DONT_CARE);
	return CourseMangerReportFacultyRequest(manager,course_id,request,output_channel);
}
//-------------------------------------------------------------------------------------
COURSE_MANEGERResult analyzeReport(FILE* output_channel, CourseManager manager) {
	char* secondWord = strtok(NULL, DONT_CARE);
	assert(secondWord);
	if (strcmp(secondWord, "full") == 0) {
		return (analyzeReportFull(manager, output_channel));
	}
	if (strcmp(secondWord, "clean") == 0) {
		return (analyzeReportClean(manager, output_channel));
	}
	if (strcmp(secondWord, "best") == 0) {
		return (analyzeReportBest(manager, output_channel));
	}
	if (strcmp(secondWord, "worst") == 0) {
		return (analyzeReportWorst(manager, output_channel));
	}
	if (strcmp(secondWord, "reference") == 0) {
		return (analyzeReportReference(manager, output_channel));
	}
	if (strcmp(secondWord, "faculty_request") == 0) {
		return (analyzeReportFacultyRequest(manager, output_channel));
	}
	return COURSE_MANAGER_OK;
}
//-------------------------------------------------------------------------------------
COURSE_MANEGERResult analyzeGradeSheet(CourseManager manager) {
	char* secondWord = strtok(NULL, DONT_CARE);
	assert(secondWord);
	if (strcmp(secondWord, "add") == 0) {
		return (analyzeGradeSheetAdd(manager));
	}
	if (strcmp(secondWord,"remove") == 0) {
		return (analyzeGradeSheetRemove(manager));
	}
	if (strcmp(secondWord,"update") == 0) {
		return (analyzeGradeSheetUpdate(manager));
	}
	// this point must not be reached
	return COURSE_MANAGER_OK;
}
//-------------------------------------------------------------------------------------
COURSE_MANEGERResult analyzeStudent(CourseManager manager) {
	char* secondWord = strtok(NULL, DONT_CARE);
	assert(secondWord);
	if (strcmp(secondWord, "add") == 0) {
		return (analyzeStudentAdd(manager));
	}
	if (strcmp(secondWord, "remove") == 0) {
		return (analyzeStudentRemove(manager));
	}
	if(strcmp(secondWord,"login")==0){
		return (analyzeStudentLogIn(manager));
	}
	if(strcmp(secondWord,"logout")==0){
		return (analyzeStudentLogOut(manager));
	}
	if(strcmp(secondWord,"friend_request")==0){
		return (analyzeStudentFriendRequest(manager));
	}
	if(strcmp(secondWord,"handle_request")==0){
		return (analyzeStudentHandleRequest(manager));
	}
	if(strcmp(secondWord,"unfriend")==0){
		return (analyzeStudentUnFriend(manager));
	}
	// this point must not be reached
	return COURSE_MANAGER_OK;
}
//-------------------------------------------------------------------------------------
// analyzes the input line
int analyzeLine(char* buffer, FILE* output,CourseManager manager) {
	char* firstWord = strtok(buffer, DONT_CARE);
	// empty line or comment
	if ((firstWord == NULL) || (firstWord[0] == COMMENT)
			|| (firstWord[0] == '\n') || (firstWord[0] == NEW_LINE)) {
		return 0;
	}
	COURSE_MANEGERResult result;
	if (strcmp(firstWord, "student") == 0) {
		result = analyzeStudent(manager);
	}
	if (strcmp(firstWord, "grade_sheet") == 0) {
		result = analyzeGradeSheet(manager);
	}
	if (strcmp(firstWord, "report") == 0) {
		result = analyzeReport(output, manager);
	}
	if (result == COURSE_MANAGER_OK) { // commands were performed successfully
		return 0;
	} else {
		mtmPrintErrorMessage(stderr, SWITCH_COURSE_MANAGER_RESULT(result));
	}
	if (result == COURSE_MANAGER_OUT_OF_MEMORY || result== COURSE_MANAGER_MEMORY_ERROR) {    // must exit
		return -1;
	}
	return 0;
}
//-------------------------------------------------------------------------------------
/* checks if the parameters argc and argv are valid and open input and output
 * files if needed, returns the opened input and output files via the given
 * pointers and returns 1. if parameters are not valid or files could not be
 * opened it returns -1 */
int getInputAndOutputChanels(int argc, char** argv, FILE** input, FILE** output) {
	if ((argc != NO_FILES) && (argc != ONE_FILE) && (argc != TWO_FILES)) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
		return -1;
	}
	//  recognizes which files are entered
	int inputIndex = -1;
	int outputIndex = -1;
	for (int i = 1; i < argc; i += GET_NEXT_PARAMETER) {
		if (strcmp(argv[i], "-i") == 0) {
			inputIndex = i + 1; // index of input file  (the input file getten after the flag
		} else if (strcmp(argv[i], "-o") == 0) {
			outputIndex = i + 1; // index of output file (the output file getten after o
		}
	}
	// if user enters arguments and "-i" or "-o" appears twice or none of them
	if ((argc == TWO_FILES) && ((inputIndex == -1) || (outputIndex == -1))) {
		mtmPrintErrorMessage(stderr, MTM_INVALID_COMMAND_LINE_PARAMETERS);
		return -1;
	}
	*input = stdin;    // default - in case of 0 parameters entered
	*output = stdout;    // default - in case of 0 parameters entered
	if (inputIndex != -1) {    // if input file was entered
		*input = fopen(argv[inputIndex], "r");
		if (*input == NULL) {
			mtmPrintErrorMessage(stderr, MTM_CANNOT_OPEN_FILE);
			return -1;
		}
	}
	if (outputIndex != -1) {    // if output file was entered
		*output = fopen(argv[outputIndex], "w");
		if (*output == NULL) {
			if (*input != stdin) {
				fclose(*input);
			}
			mtmPrintErrorMessage(stderr, MTM_CANNOT_OPEN_FILE);
			return -1;
		}
	}
	return 0;
}
//-------------------------------------------------------------------------------------
// closes a program in case of memory outrun
void closeProgram(CourseManager manager, FILE* input, FILE* output) {
	assert(input);
	assert(output);
	CourseManagerDestroy(manager);
	if (input != stdin) {
		fclose(input);
	}
	if (output != stdout) {
		fclose(output);
	}
}
//-------------------------------------------------------------------------------------
// returns 0 if commands were performed successfully, otherwise, returns -1
int main(int argc, char** argv) {
    FILE* input = stdin;
	FILE* output = stdout;
	int result = getInputAndOutputChanels(argc, argv, &input, &output);
	if (result == -1) {
		return -1;
	}
	//creates a new yad3
	CourseManager manager=CourseMangerCreate();
	if (manager == NULL) {    // if creation fails
		closeProgram(manager, input, output);
		mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
		return -1;
	}
	// gets commands from user/file
	char buffer[MAX_LEN];
	while (fgets(buffer, MAX_LEN, input) != NULL) {
		//removeSpaces(buffer);
		result = analyzeLine(buffer, output,manager);
		if (result == -1) {    // in case of a memory outrun
			closeProgram(manager, input, output);
			mtmPrintErrorMessage(stderr, MTM_OUT_OF_MEMORY);
			return -1;
		}
	}
	closeProgram(manager, input, output);
	return 0;
}
