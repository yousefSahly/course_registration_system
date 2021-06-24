#include "Course.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#define MAXIDNUM 1000000
#define MAXGRADE 100
struct course_t{
    int id;
    char *points;
    int grade;
};

//-----------------------------------------------------------------------------------
//this func is to check if the points is a legal number.
static int pointsCheck(char *points){
	if(*points=='.'){
		return -1;
	}
    while (*points !='\0'){
        if (*points == '.'){
        	points++;
            if (*(points)-'5'!=0 && *(points)-'0'!=0){
                return -1;
            }
        }
        if(*points-'0' < 0 || *points-'0' >9){
            return -1;
        }
        points++;
    }
    return 1;
}
//-----------------------------------------------------------------------------------
CourseResult createCourse(Course *course, int id, char *points,int grade) {
    assert(points !=NULL);
    assert(course !=NULL);
    if ( id<=0 || id >=MAXIDNUM || pointsCheck(points) !=1 || grade<0 || grade>MAXGRADE) {
        return COURSE_ILLEGAL_PARAMETER;
    }
    Course course1=calloc(1,sizeof(*course1));
    if (course1 == NULL) {
        return COURSE_MEMORY_ERROR;
    }
    char* points1 = calloc((strlen(points)+1), sizeof(*points1));
    if (points1 == NULL) {
        free(course1);
        return COURSE_MEMORY_ERROR;
    }
    course1->id=id;
    strcpy(points1, points);
    course1->points=points1;
    course1->grade= grade;
    *course=course1;
    return COURSE_OK;
}
//------------------------------------------------------------------------------------------
CourseResult CourseUpdateGrade(Course course, int grade){
    assert(course!=NULL);
    if (grade<0 || grade>MAXGRADE){
        return COURSE_ILLEGAL_PARAMETER;
    }
    course->grade=grade;
    return COURSE_OK;
}
//------------------------------------------------------------------------------------------
int courseGetGrade(Course course){
	assert(course!=NULL);
	return course->grade;
}
//------------------------------------------------------------------------------------------
int courseGetId(Course course){
	assert(course!=NULL);
	return course->id;
}
//-----------------------------------------------------------------------------------------
char* courseGetPoints(Course course){
	assert(course !=NULL);
	return course->points;
}
//-----------------------------------------------------------------------------------------
ListElement copyCourse(ListElement course){
	assert(course !=NULL);
    Course new_course = malloc(sizeof(*new_course));
    if (new_course== NULL) {
        return NULL;
    }
    char* points=malloc(strlen(((Course)course)->points)+1);
    if(!points){
    	free(new_course);
    	return NULL;
    }
    new_course->id=((Course)course)->id;
    strcpy(points,((Course)course)->points);
    new_course->points=points;
    new_course->grade=((Course)course)->grade;
    return new_course;
}
//------------------------------------------------------------------------------------------
void freeCourse(ListElement course) {
	assert(course!=NULL);
	destroyCourse((Course)course);
}
//------------------------------------------------------------------------------------------
// deallocate all relevant memory of course1 and stop using it.
void destroyCourse(Course course1) {
    assert(course1!=NULL);
    free(course1->points);
    free(course1);
}
