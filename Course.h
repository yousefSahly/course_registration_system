#ifndef COURSE_H
#define COURSE_H
#include "list.h"
#include <string.h>

typedef struct course_t *Course;

typedef enum {
    COURSE_OK ,
    COURSE_MEMORY_ERROR ,
    COURSE_GRADE_DOES_EXIST,
    COURSE_IS_EMPTY,
    COURSE_ILLEGAL_PARAMETER
} CourseResult;

//------------------------------------------------------------------------------------------
// create a new course with the details of the parameters id , points , grades list
// id and points should be copied into the new crated course by duplication.
// points or course must not be NULL. (handled by assert).
// course is an output parameter which gets the created new course.
// return value : COURSE_OK ,
//                COURSE_MEMORY_ERROR  if there are memory problems
//                COURSE_ILLEGAL_PARAMETER if points < 0
CourseResult createCourse(Course *course, int id, char *points, int grade);
//------------------------------------------------------------------------------------------
//this func returns the grade of the course
int courseGetGrade(Course course);
//------------------------------------------------------------------------------------------
//this func returns the id of the course
int courseGetId(Course course);
//------------------------------------------------------------------------------------------
//this func returns the points of the course in a string
char* courseGetPoints(Course course);
//------------------------------------------------------------------------------------------
//this func updates a grade in a the course
//return values: COURSE_OK, COURSE_ILLEGAL_PARAMETER if grade is illegal.
CourseResult CourseUpdateGrade(Course course, int grade);
//------------------------------------------------------------------------------------------
// deallocate all relevant memory of course1 and stop using it.
void destroyCourse(Course course1);
//-----------------------------------------------------------------------------------------
//this func is and the next one are for the listCreate.
void freeCourse(ListElement course);
//-----------------------------------------------------------------------------------------
ListElement copyCourse(ListElement course);

#endif // COURSE_H
