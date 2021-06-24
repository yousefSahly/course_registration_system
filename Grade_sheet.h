#ifndef GRADE_SHEET_H_
#define GRADE_SHEET_H_

#include "Semester.h"
#include "list.h"

typedef struct grade_sheet_t *GradeSheet;

typedef enum{
    GRADE_SHEET_OK,
    GRADE_SHEET_MEMORY_ERROR,
    GRADE_SHEET_IS_EMPTY,
    GRADE_SHEET_INVALID_PARAMTER,
	GRADE_SHEET_SEMESTER_IS_EMPTY,
	GRADE_SHEET_COURSE_DOES_NOT_EXIST,
    GRADE_SHEET_SEMESTER_DOES_NOT_EXIST,
    GRADE_SHEET_SEMESTER_COURSE_DOESNT_EXIST,
    GRADE_SHEET_SHOULD_NEVER_REACH_HERE,
    GRADE_SHEET_SEMESTER_EMPTY,
    GRADE_SHEET_SEMESTER_ALREADY_EXIST
}GRADE_SHEETResult;
//---------------------------------------------------------------------------------------------------
//this function creates a new grade_sheet
//@param new_sheet is a pointer for grade_sheet that points to the new created grade_sheet
//return value: GRADE_SHEET_OK,
//              GRADE_SHET_MEMORY_ERROR;
GRADE_SHEETResult GradeSheetCreate(GradeSheet * new_sheet);
//---------------------------------------------------------------------------------------------------
//this func is to add a semester to the semesters list in the grade sheet of the student.
GRADE_SHEETResult GradeSheetAddSemester(GradeSheet sheet,int Semester_id);
//---------------------------------------------------------------------------------------------------
//this func is to add a grade of a course (the course is the id and the points which are saved in the course struct)
//in to the semester with the given semester_id. to the grade sheet of the connected student.
//if the semester doesn't exist then it's created.
GRADE_SHEETResult GradeSheetAddCourse(GradeSheet sheet,int course_id,int grade,
		                                                char* points, int semester_id);
//---------------------------------------------------------------------------------------------------
//this func is to remove a course from a semester with a given_id.
//if the course is not found then a relevant e num is returned.
GRADE_SHEETResult GradeSheetRemoveCourseFromSemester(GradeSheet sheet,int course_id,int semester_id);
//---------------------------------------------------------------------------------------------------
//this func updates a grade of a course with a given id. it updates the last grade that was taken.
GRADE_SHEETResult GradeSheetUpdateCourse(GradeSheet sheet,int course_id,int grade);
//---------------------------------------------------------------------------------------------------
//this func returns the list of the semesters in grade sheet of the connected student.
List GradeSheetGetList(GradeSheet sheet);
//---------------------------------------------------------------------------------------------------
//this func returns the size of the semesters list.
int GradeSheetGetSize(GradeSheet sheet);
//---------------------------------------------------------------------------------------------------
//this func calculates the total points X2 in the given grade sheet and returns them.
int GradeSheetTotalPointsX2(GradeSheet sheet);
//---------------------------------------------------------------------------------------------------
//this func calculates the total failed points X2 in the given grade sheet and returns them.
int GradeSheetFailedPointsX2(GradeSheet sheet);
//---------------------------------------------------------------------------------------------------
//this func calculates the effective grades X1 and points X2 of non-sport courses
// in the given grade sheet and returns them.
int GradeSheetEffectivePointX2AndGradeOther(GradeSheet sheet, int *sum2, int course_id,int*exist);
//---------------------------------------------------------------------------------------------------
//this func calculates the effective grades and points X2 of the sport courses
//in the given grade sheet and returns them.
int GradeSheetEffectivePointsAndGradesSportX2(GradeSheet sheet,int*sum2);
//---------------------------------------------------------------------------------------------------
//this func calculates the effective grades of the non-sport courses and points X2
//in the given grade sheet and returns them.
int GradeSheetEffectivePointsAndGradesOtherX2(GradeSheet sheet,int*sum2);
//---------------------------------------------------------------------------------------------------
//this func calculates the effective grades of all courses and points X2 in the given grade sheet and returns them.
int GradeSheetEffectivePointsAndGradesX2(GradeSheet sheet,int*total_points);
//---------------------------------------------------------------------------------------------------
//this func copies all the data in the given sheet to a new sheet.
GradeSheet copyGradeSheet(GradeSheet sheet);
//---------------------------------------------------------------------------------------------------
//this func searches for the last semester in the connected student grade sheet.
int GradeSheetFindLastSemester(GradeSheet sheet,int course_id);
//---------------------------------------------------------------------------------------------------
// this function deletes the semester with no courses and free's it from the list of semesters
GRADE_SHEETResult GradeSheetRemoveEmptySemester(GradeSheet sheet);
//---------------------------------------------------------------------------------------------------

//this function destroys the Grade_sheet
//@param sheet is the target sheet that we want to destroy
void GradeSheetDestroy(GradeSheet  sheet);


#endif /* GRADE_SHEET_H_ */
