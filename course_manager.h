
#ifndef COURSE_MANAGER_H_
#define COURSE_MANAGER_H_

#include "set.h"
#include "student.h"
#include "mtm_ex3.h"
#include "Grade_sheet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STUDENT_ID_NUM 1000000000
typedef struct course_manager_t *CourseManager;

typedef struct grade_t *Grade;

typedef struct relevant_frined_t *RelevantFriend;

typedef enum {
    COURSE_MANAGER_OUT_OF_MEMORY, // You should exit program after this error
    COURSE_MANAGER_INVALID_PARAMETERS,
    COURSE_MANAGER_NOT_LOGGED_IN,
    COURSE_MANAGER_ALREADY_LOGGED_IN,
    COURSE_MANAGER_STUDENT_DOES_NOT_EXIST,
    COURSE_MANAGER_STUDENT_ALREADY_EXISTS,
    COURSE_MANAGER_NOT_FRIEND,
    COURSE_MANAGER_ALREADY_FRIEND,
    COURSE_MANAGER_NOT_REQUESTED,
    COURSE_MANAGER_ALREADY_REQUESTED,
    COURSE_MANAGER_COURSE_DOES_NOT_EXIST,
    COURSE_MANAGER_SHOULD_NEVER_GET_HERE,
    COURSE_MANAGER_MEMORY_ERROR,
    COURSE_MANAGER_SET_IS_EMPTY,
    COURSE_MANAGER_STUDENT_GRADES_IS_EMPTY,
    COURSE_MANAGER_LIST_FRIENDS_IS_EMPTY,
    COURSE_MANAGER_OK
}COURSE_MANEGERResult;

//this func creates the course manger
CourseManager CourseMangerCreate();

//---------------------------------------------------------------------------------------------------
//this func is to add a student to the students set
COURSE_MANEGERResult CourseMangerAddStudent(CourseManager manager, int student_id, char* Fname, char* Lname);

//---------------------------------------------------------------------------------------------------
//this func is to remove a student from the students set.
COURSE_MANEGERResult CourseMangerRemoveStudent(CourseManager manager, int student_id);

//---------------------------------------------------------------------------------------------------
//this func turns on the connection_id flag. this flag is to know if a student is connected or not.
COURSE_MANEGERResult CourseMangerLogin(CourseManager manager ,int student_id);

//---------------------------------------------------------------------------------------------------
//this func resets the connection_id flag. so that we know that now a new student can login.
COURSE_MANEGERResult CourseMangerLogout(CourseManager manager);

//---------------------------------------------------------------------------------------------------
//this func adds the id of the connected student to the other_id's student's friend request in set.
//and adds the other_id to the connected student friend request out set.
COURSE_MANEGERResult CourseMangerFriendRequest(CourseManager manager, int other_id);

//---------------------------------------------------------------------------------------------------
//this func takes the other id. and checks if the other id is in the connected student's friend request in/out set.
//and if it's there, then we deal with it according to the action. and the other_id is deleted from the friend
//request int/out set in the two actions. in addition if the action was accept, then we have to add the other_id
//to the connected student's friends list and to the other as well.
COURSE_MANEGERResult CourseMangerHandleFriendRequest(CourseManager manager,int other_id, char* action);

//---------------------------------------------------------------------------------------------------
//this func deletes the other_id from the connected student friends set. and deletes the id of the connected
//student from the other_id student friends set.
COURSE_MANEGERResult CourseMangerUnFriend(CourseManager manager, int other_id);

//---------------------------------------------------------------------------------------------------
//this func adds a grade to the connected student grade sheet. if there is no grade sheet. then it's created.
COURSE_MANEGERResult CourseMangerAddGrade(CourseManager manager, int semester_id, int course_id,
                                          char* points, int grade);
//---------------------------------------------------------------------------------------------------
//this func deletes the last grade of the given course in the given semester.
COURSE_MANEGERResult CourseMangerRemoveGrade(CourseManager manager,int semester_id,int course_id);
//---------------------------------------------------------------------------------------------------
//this func updates the grade for a specific course.
COURSE_MANEGERResult CourseMangerUpdateGrade(CourseManager manager, int course_id, int new_grade);

//---------------------------------------------------------------------------------------------------
//this func prints all the grade sheet information of the connected student.
COURSE_MANEGERResult CourseMangerReportFull(CourseManager manager, FILE* output_channel);

//---------------------------------------------------------------------------------------------------
//nfs el eshe t2reban.. bs 3ala as3'ar shway
COURSE_MANEGERResult CourseMangerReportClean(CourseManager manager, FILE* output_channel);
//---------------------------------------------------------------------------------------------------
//this func prints the top amount grades from the connected student grade sheet
COURSE_MANEGERResult CourseMangerReportBest(CourseManager manager, int amount, FILE* output_channel);

//---------------------------------------------------------------------------------------------------
//this func prints the worst amount grades from the connected student grade sheet
COURSE_MANEGERResult CourseMangerReportWorst(CourseManager manager, int amount, FILE* output_channel);

//---------------------------------------------------------------------------------------------------
//this func searches in the connected student friends set for the top amount grades in the given
//course and prints the names of the grade's students.
COURSE_MANEGERResult CourseMangerReportReference(CourseManager manager,int course_id, int amount, FILE* output_channel);

//---------------------------------------------------------------------------------------------------
//this func is to handle the faculty request that the connected student have sent.
//it works according to the command rules.
COURSE_MANEGERResult CourseMangerReportFacultyRequest(CourseManager manager,int course_id, char* request, FILE* output_channel);
//---------------------------------------------------------------------------------------------------
//this func returns the list of the relevant friends in the manager.
List CourseManagerGetRelevanFriendsList(CourseManager manager);
//---------------------------------------------------------------------------------------------------
//this func returns the students set.
Set CourseManagerGetStudents(CourseManager manager);
//---------------------------------------------------------------------------------------------------
//this func returns the grades list taht is in the manager.
List CourseManagerGetGradesList(CourseManager manager);
//---------------------------------------------------------------------------------------------------
//returns the size of the students set.
int CourseManagerGetStudentsSize(CourseManager manager);
//---------------------------------------------------------------------------------------------------
//returns the connected id that is connected in the manager.
int CourseManagerGetConnectedId(CourseManager manager);
//---------------------------------------------------------------------------------------------------
//this func returns the student via id.
//and returns NULL if the student not returned.
Student CourseManagerGetStudentViaId(CourseManager manager,int student_id);
//---------------------------------------------------------------------------------------------------
//this func destroys all the relevant memory of the manager.
void CourseManagerDestroy(CourseManager course_manager);

#endif
