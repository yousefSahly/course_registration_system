#include "student.h"
#include "Grade_sheet.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#define MAXIDNUM 1000000
#define MAX_GRADE 100
#define MAX_STUDENT_ID_NUM 1000000000
struct student_t{
    int id;
    char* first_name;
    char* last_name;
    List friends;
    Set friend_request_in;
    Set friend_request_out;
    GradeSheet student_sheet;
};
//-----------------------------------------------------------------------------------
//this func is to check if the points is a legal number.
static int pointsCheck(char *points){
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
//---------------------------------------------------------------------------------------------
//this func is to compare between two elements of a list.
int compareIntL(ListElement friend1 ,ListElement friend2,ListSortKey key){
	assert(friend1!=NULL);
	assert(friend2!=NULL);
	assert(key!=NULL);
	int distance1 = abs(*(int*)friend1 - *(int*)key);
	int distance2 = abs(*(int*)friend2 - *(int*)key);
	return distance1 - distance2;
}
//---------------------------------------------------------------------------------------------
//this func is to free an element of a list.
void freeIntL(ListElement friend){
	assert(friend!=NULL);
	free(friend);
}
//---------------------------------------------------------------------------------------------
//this func is to copy an element of a list.
ListElement copyIntL(ListElement friend){
	assert(friend!=NULL);
	int* new_friend=malloc(sizeof(int));
	if(!new_friend){
		return NULL;
	}
	*new_friend=*(int*)friend;
    return new_friend;
}
//---------------------------------------------------------------------------------------------
//this func is to compare between two elements of a set.

int compareIntS(SetElement request1 ,SetElement request2 ){
	assert(request1!=NULL);
	assert(request2!=NULL);
	return (*(int*)request1)-(*(int*)request2);
}
//---------------------------------------------------------------------------------------------
//this func is to free an element of a set.

void freeIntS(SetElement request){
	assert(request!=NULL);
	free(request);
}
//---------------------------------------------------------------------------------------------
//this func is to copy an element of a set.

SetElement copyIntS(SetElement request){
	assert(request!=NULL);
	int* new_request=malloc(sizeof(int));
	if(!new_request){
		return NULL;
	}
	*new_request=*(int*)request;
    return new_request;
}
//---------------------------------------------------------------------------------------------
STUDENTResult StudentCreate(Student *student, int id, char* Fname, char* Lname){
    assert(student !=NULL);
    assert(Fname !=NULL);
    assert(Lname !=NULL);
    if (id <=0 || id >=MAX_STUDENT_ID_NUM){
        return STUDENT_INVALID_PARAMETER;
    }
    Student student1=malloc(sizeof(*student1));
    if (!student1){
        return STUDENT_MEMORY_ERROR;
    }
    char* Fname1=malloc((strlen(Fname)+1)* sizeof(*Fname1));
    if (!Fname1){
        free(student1);
        return STUDENT_MEMORY_ERROR;
    }
    char* Lname1=malloc((strlen(Lname)+1)*sizeof(*Lname1));
    if (!Lname1){
        free(student1);
        free(Fname);
        return STUDENT_MEMORY_ERROR;
    }
    GradeSheet new_sheet;
    GRADE_SHEETResult res= GradeSheetCreate(&new_sheet);
    if (res == GRADE_SHEET_MEMORY_ERROR){
        free(Lname1);
        free(Fname1);
        free(student1);
        return STUDENT_MEMORY_ERROR;
    }
    student1->student_sheet=new_sheet;
    strcpy(Lname1, Lname);
    strcpy(Fname1, Fname);
    student1->id=id;
    student1->first_name=Fname1;
    student1->last_name=Lname1;
    student1->friend_request_in=setCreate(copyIntS,freeIntS,compareIntS);
    if (!student1->friend_request_in){
        free(student1->first_name);
        free(student1->last_name);
        free(student1);
        GradeSheetDestroy(student1->student_sheet);
        return STUDENT_MEMORY_ERROR;
    }
    student1->friend_request_out=setCreate(copyIntS,freeIntS,compareIntS);
    if (!student1->friend_request_out){
    	 free(student1->first_name);
    	 free(student1->last_name);
         free(student1);
         GradeSheetDestroy(student1->student_sheet);
        setDestroy(student1->friend_request_in);
        return STUDENT_MEMORY_ERROR;
    }
    student1->friends=listCreate(copyIntL,freeIntL);
    if (!student1->friends){
    	 free(student1->first_name);
    	 free(student1->last_name);
         free(student1);
         GradeSheetDestroy(student1->student_sheet);
        setDestroy(student1->friend_request_in);
        setDestroy(student1->friend_request_out);
        return STUDENT_MEMORY_ERROR;
    }
    *student=student1;
    return STUDENT_OK;
}
//-----------------------------------------------------------------------------------------------
int StudentGetId(Student student){
	assert(student !=NULL);
	return student->id;
}
//-----------------------------------------------------------------------------------------------
char* StudentGetFirstName(Student student){
	assert(student !=NULL);
    return student->first_name;
}
//-----------------------------------------------------------------------------------------------
char* StudentGetLastName(Student student){
	assert(student !=NULL);
	return student->last_name;
}
//-----------------------------------------------------------------------------------------
GradeSheet StudentGetGradeSheet(Student student){
	assert(student !=NULL);
	return student->student_sheet;
}
//------------------------------------------------------------------------------------------
List StudentGetFriends(Student student){
	assert(student !=NULL);
	return student->friends;
}
//------------------------------------------------------------------------------------------
Set StudentGetFriendRequestIn(Student student){
	assert(student !=NULL);
	return student->friend_request_in;
}
//------------------------------------------------------------------------------------------
Set StudentGetFriendRequestOut(Student student){
	assert(student !=NULL);
	return student->friend_request_out;
}
//------------------------------------------------------------------------------------------
bool StudentFindFriendViaId(Student student,int id){
	ListElement friend=listGetFirst(student->friends);
	while(friend){
	      if (*((int*)friend)==id){
	            return true;
	      }
	      friend=listGetNext(student->friends);
	}
	return false;
}
//-----------------------------------------------------------------------------------------------
STUDENTResult StudentAddNewGrade(Student student,int semester_id,
		                                         int course_id, char* points, int grade){
	assert(student !=NULL);
	assert(points !=NULL);
	if(semester_id<=0 || course_id<=0
			|| course_id>=MAXIDNUM || grade<0 || grade>MAX_GRADE || pointsCheck(points)==-1){
		return STUDENT_INVALID_PARAMETER;
	}
	GRADE_SHEETResult res=GradeSheetAddSemester(student->student_sheet,semester_id);
	if(res==GRADE_SHEET_INVALID_PARAMTER){
		return STUDENT_INVALID_PARAMETER;
	}
	else if(res==GRADE_SHEET_MEMORY_ERROR){
		return STUDENT_MEMORY_ERROR;
	}
	else if(res==GRADE_SHEET_SEMESTER_ALREADY_EXIST){
	GRADE_SHEETResult res2=GradeSheetAddCourse(student->student_sheet,course_id,grade,points,semester_id);
	if(res2==GRADE_SHEET_INVALID_PARAMTER){
		return STUDENT_INVALID_PARAMETER;
	}
	else if(res2==GRADE_SHEET_MEMORY_ERROR){
		return STUDENT_MEMORY_ERROR;
	}
		return STUDENT_OK;
	}
	res=GradeSheetAddCourse(student->student_sheet,course_id,grade,points,semester_id);
	if(res==GRADE_SHEET_INVALID_PARAMTER){
			return STUDENT_INVALID_PARAMETER;
	}
	else if(res==GRADE_SHEET_MEMORY_ERROR){
			return STUDENT_MEMORY_ERROR;
	}
	return STUDENT_OK;
}
//----------------------------------------------------------------------------------------------
 int compareStudents(SetElement student1,SetElement student2){
	assert(student1!=NULL);
	assert(student2!=NULL);
	return ((Student)student1)->id-((Student)student2)->id;
}
//---------------------------------------------------------------------------------------------
 SetElement copyStudent(SetElement student){
	assert(student!=NULL);
	Student new_student;
	StudentCreate(&new_student,((Student)student)->id,
			((Student)student)->first_name,((Student)student)->last_name);
	listDestroy(new_student->friends);
	new_student->friends=listCopy(((Student)student)->friends);
	setDestroy(new_student->friend_request_in);
	new_student->friend_request_in=setCopy(((Student)student)->friend_request_in);
	setDestroy(new_student->friend_request_out);
	new_student->friend_request_out=setCopy(((Student)student)->friend_request_out);
	GradeSheetDestroy(new_student->student_sheet);
	new_student->student_sheet=copyGradeSheet(((Student)student)->student_sheet);
    return new_student;
}
//---------------------------------------------------------------------------------------------
void freeStudent(SetElement student){
	assert(student!=NULL);
	StudentDestroy((Student)student);
}
//-------------------------------------------------------------------------------------------------
void StudentDestroy(Student student){
GradeSheetDestroy(student->student_sheet);
listDestroy(student->friends);
setDestroy(student->friend_request_in);
setDestroy(student->friend_request_out);
free(student->last_name);
free(student->first_name);
free(student);
return;
}
