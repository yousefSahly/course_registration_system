#ifndef STUDENT_H_
#define STUDENT_H_
#include "Grade_sheet.h"
#include "set.h"
#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

typedef struct student_t *Student;

typedef enum {
    STUDENT_OK,
    STUDENT_MEMORY_ERROR,
	STUDENT_NO_FRIENDS,
    STUDENT_INVALID_PARAMETER,
    STUDENT_FRIEND_DOES_NOT_EXIST,
	STUDENT_FRIEND_ALREADY_EXIST,
    STUDENT_NO_FRIEND_REQUEST,
    STUDENT_FRIEND_REQUEST_ALREADY_SENT,
	STUDENT_ELEMENT_ALREADY_EXIST,
    STUDENT_GRADE_SHEET_IS_IMPTY
}STUDENTResult;

//this func is to creat the student struct.
STUDENTResult StudentCreate(Student *student, int id, char* Fname, char* Lname);

//------------------------------------------------------------------------------------------
//this func is to add a friend to the friends set.
STUDENTResult StudentAddFriend(Student student, int other_id);

//------------------------------------------------------------------------------------------
//this func is to remove a friend from the friends set.
STUDENTResult StudentUnfriend(Student student, int other_id);

//------------------------------------------------------------------------------------------
//this func returns the id of the connected student.
int StudentGetId(Student student);

//------------------------------------------------------------------------------------------
//this func returns the first name of the connected student.
char* StudentGetFirstName(Student student);
//------------------------------------------------------------------------------------------
//this func returns the last name of the given student.
char* StudentGetLastName(Student student);
//-----------------------------------------------------------------------------------------
//this func returns he grade sheet that is in the student struct.
GradeSheet StudentGetGradeSheet(Student student);
//------------------------------------------------------------------------------------------
//this func returns the friends list that is in the student struct.
List StudentGetFriends(Student student);
//------------------------------------------------------------------------------------------
//this func returns the friend requests set that the student have received that is in the student struct.
Set StudentGetFriendRequestIn(Student student);
//------------------------------------------------------------------------------------------
//this func returns the friend requests set that the student have sent that is in the student struct.
Set StudentGetFriendRequestOut(Student student);
//-----------------------------------------------------------------------------------------
//the function free's a student we uuse it to listCreate
void freeStudent(SetElement student);
//-----------------------------------------------------------------------------------------
//this function copy's student to new student we use it to listCreate
SetElement copyStudent(SetElement student);
//-----------------------------------------------------------------------------------------
//this function compares two student we use it in listSort
int compareStudents(SetElement student1,SetElement student2);
//-----------------------------------------------------------------------------------------
//this function compares two friends we use it in listSort
int compareIntL(ListElement friend1 ,ListElement friend2,ListSortKey key);
//-----------------------------------------------------------------------------------------
//the function free's a friend we uuse it to listCreate
void freeIntL(ListElement friend);
//-----------------------------------------------------------------------------------------
//this function copy's friend to new student we use it to listCreate
ListElement copyIntL(ListElement friend);
//-----------------------------------------------------------------------------------------
// this function searches for a friend with the id id and returns true if found
bool StudentFindFriendViaId(Student student,int id);
//-----------------------------------------------------------------------------------------
//this function compares requests we use it to sort the set of requests
int compareIntS(SetElement request1 ,SetElement request2 );
//-----------------------------------------------------------------------------------------
// this funcion free's request
void freeIntS(SetElement request);
//-----------------------------------------------------------------------------------------
//this function copy's request to new request
SetElement copyIntS(SetElement request);
//------------------------------------------------------------------------------------------
//this func takes a grade for a specific course and the course details and a semester.
//and adds them to the student grade sheet.
STUDENTResult StudentAddNewGrade(Student student, int semester_id, int course_id, char* points, int grade);

//------------------------------------------------------------------------------------------
//this func frees all the relevant memory of a Student struct.
void StudentDestroy(Student student);


#endif

