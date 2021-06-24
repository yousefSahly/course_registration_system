#ifndef SEMESTER_H_
#define SEMESTER_H_

#include "list.h"
#include "Course.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


typedef struct semester_t *Semester;

typedef enum {
    SEMESTER_SUCCESS,
    SEMESTER_INVALID_PARAMETER,
    SEMESTER_MEMORY_ERROR,
	SEMESTER_IS_EMPTY,
    SEMESTER_COURSE_ALREADY_EXIST,
	SEMESTER_COURSE_DOES_NOT_EXIST
}SEMESTERResult;


//this func creates a semester ADT to hold a set of the courses that are done in this
//semester. return values: SEMESTER_SUCCESS, SEMESTER_MEMORY_ERROR in case of an allocation
//problem.
SEMESTERResult SemesterCreate(Semester *semester, int id);

//------------------------------------------------------------------------------------------
// this function searches for the course with the id course_id and returns the last index where it
// appeared in the list of the courses
int SemesterFindLastCourse(Semester semester1,int course_id,int* found);
//------------------------------------------------------------------------------------------
//this func is to add a course that was done in that semester.
SEMESTERResult SemesterAddCourse(Semester semester1, int course_id,char * points,int grade);

//------------------------------------------------------------------------------------------
//this func is to remove a given semester from the courses set
//and frees all the relevant memory of the course
SEMESTERResult SemesterRemoveCourse(Semester semester1, int course_id);

//------------------------------------------------------------------------------------------
//this func updates a given grade to a given course.
SEMESTERResult SemesterUpdateCourse(Semester semester1,int course_id, int grade);
//------------------------------------------------------------------------------------------
// this function returns the course's points and grade with the id course_id which is the effictive
// grade for the semester
SEMESTERResult SemesterGetValidGrade(Semester semester1,int course_id, int* grade,int* points);
//------------------------------------------------------------------------------------------
//this func returns the id of the semester
int SemesterGetId(Semester semester1);
//------------------------------------------------------------------------------------------
//this func returns the number of courses in the semester
int SemesterGetSize(Semester semester1);
//-----------------------------------------------------------------------------------------
//this func is to return the courses list in every semester.
List SemesterGetList(Semester semester);
//-----------------------------------------------------------------------------------------
//this func searches for the last course with the given id that was taken in the given semester.
Course SemesterGetLastCourse(Semester semester,int course_id);
//-----------------------------------------------------------------------------------------
//this func searches for a max grade in course with a given id in a given semester.
Course SemesterGetMaxCourse(Semester semester1, int  course_id);
//-----------------------------------------------------------------------------------------
//this func compares between 2 semester ides according to a given key.
int compareSemesters(ListElement semester1,ListElement semester2, ListSortKey key );
//-----------------------------------------------------------------------------------
//the next two functions are for the list sort and list create.
void freeSemester(ListElement semester);
//-----------------------------------------------------------------------------------
// this function copy's the semester to a new semester with duplication
ListElement copySemester(ListElement semester);
//-----------------------------------------------------------------------------------------------
//this func calculates the effective grades and points X2 in a given semester and returns them.
int SemesterEffectiveGradesAndPointsX2(Semester semester,int*sum2);
//-----------------------------------------------------------------------------------------------
//this func calculates the total failed points X2 in a given semester and returns them.
int SemesterFailPointsX2(Semester semester);
//-----------------------------------------------------------------------------------------------
//this func calculates the total points X2 in a given semester and returns them.
int SemesterTotalPointsX2(Semester semester);
//------------------------------------------------------------------------------------------
//this func is to free all the relevant memory of the semester.
void SemesterDestroy(Semester semester1);

#endif /* Semester_H_ */
