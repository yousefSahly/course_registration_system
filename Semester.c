#include "list.h"
#include "Course.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "Semester.h"
#define MAXIDNUM 1000000
#define MAXGRADE 100
#define FAILDGRADE 55
struct semester_t{
    int semeseter_id;
    List courses;
    int coursesSize;
};
//-----------------------------------------------------------------------------------
//this func is to check if the points is a legal number.

static int pointsX2(char *points) {
    int sum = 0;
    while (*points != '\0') {
        if (*points == '.') {
           if (*(++points)-'5'==0 || *(points)-'0'==0){
                int r = (*(points) - '0') / 5;
                sum += r;
               break;
           }
        }
        if (*points-'0' >= 0 && *points-'0' <=9) {
            sum += (*points - '0') * 2;
        }
        points++;
    }
    return sum;
}
//--------------------------------------------------------------------------------------
//this func searches for the course with the given id in the semester that have the max grade.
static int SemesterFindMaxCourse(Semester semester1, int  course_id, int*res){
    assert(semester1 !=NULL);
    assert(res!=NULL);
    int max=0;
    int flag=0,i=0,index_max=0;
    if(semester1->coursesSize==0 ){
        return -1;
    }
    if(course_id <=0 || course_id>=MAXIDNUM){
        return -2;
    }
    ListElement temp_course=listGetFirst(semester1->courses);
    if(temp_course==NULL){
        return -3;
    }
    while(temp_course){
        if(courseGetId(temp_course)==course_id){
            flag=1;
            if(courseGetGrade(temp_course)>max){
                max=courseGetGrade(temp_course);
                index_max=i;
            }
        }
        i++;
        temp_course=listGetNext(semester1->courses);
    }
    if (flag==0){
        return -1;
    }
    *res=max;
    return index_max;
}
//-----------------------------------------------------------------------------------
SEMESTERResult SemesterCreate(Semester *semester, int id){
    assert(semester !=NULL);
    if (id<=0){
        return SEMESTER_INVALID_PARAMETER;
    }
    Semester semester1=calloc(1,sizeof(*semester1));
    if (semester1 == NULL) {
        return SEMESTER_MEMORY_ERROR;
    }
    semester1->courses=listCreate(copyCourse,freeCourse);
    if (!semester1->courses){
    	free(semester1);
        return SEMESTER_MEMORY_ERROR;
    }
    semester1->semeseter_id=id;
    semester1->coursesSize=0;
    *semester=semester1;
    return SEMESTER_SUCCESS;
}
//------------------------------------------------------------------------------------------
int SemesterFindLastCourse(Semester semester1,int course_id,int* found){
	assert(semester1 !=NULL);
	assert(found!=NULL);
	int j=-1;
	List courses=listCopy(semester1->courses);
    ListElement temp=listGetFirst(courses);
    if(!temp){
    	listDestroy(courses);
	        return -1;
	 }
    int index=0;
	while(index<semester1->coursesSize){
		if(courseGetId(temp)==course_id){
			*found=1;
			j=index;
		}
        temp=listGetNext(courses);
        index++;
	}
	listDestroy(courses);
	return j;
}
//----------------------------------------------------------------------------------------------
SEMESTERResult SemesterAddCourse(Semester semester1, int course_id,char * points,int grade){
    assert(semester1 !=NULL);
    assert(points !=NULL);
    if(course_id<=0 || course_id>=MAXIDNUM){
    	return SEMESTER_INVALID_PARAMETER;
    }
    Course new_course;
    CourseResult res=createCourse(&new_course,course_id,points,grade);
    if(res== COURSE_MEMORY_ERROR){
    	return SEMESTER_MEMORY_ERROR;
    }
   	else if(res==COURSE_ILLEGAL_PARAMETER ){
   		return SEMESTER_INVALID_PARAMETER;
   	}
   	ListResult	res2=listInsertLast(semester1->courses,new_course);
    if (res2 ==LIST_NULL_ARGUMENT){
   	   destroyCourse(new_course);
   	   return SEMESTER_INVALID_PARAMETER;
    }
    if (res2== LIST_OUT_OF_MEMORY){
       destroyCourse(new_course);
       return SEMESTER_MEMORY_ERROR;
    }
    destroyCourse(new_course);
    semester1->coursesSize++;
    return SEMESTER_SUCCESS;
}
//--------------------------------------------------------------------------------------
SEMESTERResult SemesterRemoveCourse(Semester semester1, int course_id){
    assert(semester1 !=NULL);
    if (course_id>=MAXIDNUM || course_id<=0){
        return SEMESTER_INVALID_PARAMETER;
    }
    ListElement temp=listGetFirst(semester1->courses);
    if(!temp){
        return SEMESTER_IS_EMPTY;
    }
    int i=0,found=-1;
   int index= SemesterFindLastCourse(semester1,course_id,&found);
   if(found==-1){
	   return SEMESTER_COURSE_DOES_NOT_EXIST;
   }
   if(index==-1){
	   return SEMESTER_IS_EMPTY;
   }
   temp=listGetFirst(semester1->courses);
   while(i<index){
	  temp= listGetNext(semester1->courses);
	   i++;
   }
   ListResult res=listRemoveCurrent(semester1->courses);
   if (res==LIST_INVALID_CURRENT){
      return SEMESTER_INVALID_PARAMETER;
   }
   else if (res==LIST_NULL_ARGUMENT){
	   return SEMESTER_MEMORY_ERROR;
   }
   semester1->coursesSize--;
   return SEMESTER_SUCCESS;
}
//--------------------------------------------------------------------------------------
SEMESTERResult SemesterUpdateCourse(Semester semester1,int course_id, int grade){
    assert(semester1 !=NULL);
    if(course_id<=0 || course_id>=MAXIDNUM){
    	return SEMESTER_INVALID_PARAMETER;
    }
    if(grade<0 || grade>MAXGRADE){
        return SEMESTER_INVALID_PARAMETER;
    }
    if(semester1->coursesSize==0){
        return SEMESTER_IS_EMPTY;
    }
    ListElement temp=listGetFirst(semester1->courses);
    if(temp==NULL){
        return SEMESTER_MEMORY_ERROR;
    }
    int i=0,found=-1;
    int index= SemesterFindLastCourse(semester1,course_id,&found);
    if(found==-1){
 	   return SEMESTER_COURSE_DOES_NOT_EXIST;
    }
    temp=listGetFirst(semester1->courses);
    while(i<index){
 	  temp= listGetNext(semester1->courses);
 	   i++;
    }
    CourseUpdateGrade(temp,grade);
    return SEMESTER_SUCCESS;
}
//--------------------------------------------------------------------------------------
SEMESTERResult SemesterGetValidGrade(Semester semester1,int course_id, int* grade,int* points){
     assert(semester1 !=NULL);
     assert(grade !=NULL);
     assert(points !=NULL);
     if(course_id<=0 || course_id>=MAXIDNUM){
     	return SEMESTER_INVALID_PARAMETER;
     }
     int found=-1,i=0;
     ListElement temp=listGetFirst(semester1->courses);
     if(temp==NULL){
       return SEMESTER_IS_EMPTY;
     }
     int index= SemesterFindLastCourse(semester1,course_id,&found);
     if(found==-1){
	   return SEMESTER_COURSE_DOES_NOT_EXIST;
     }
     temp=listGetFirst(semester1->courses);
     while(i<index){
	   temp= listGetNext(semester1->courses);
	   i++;
     }
     *grade=courseGetGrade(temp);
     *points=pointsX2(courseGetPoints(temp));
     return SEMESTER_SUCCESS;
}
//--------------------------------------------------------------------------------------
int SemesterGetId(Semester semester1){
	assert(semester1 !=NULL);
    return semester1->semeseter_id;
}
//--------------------------------------------------------------------------------------
int SemesterGetSize(Semester semester1){
	assert(semester1 !=NULL);
	return semester1->coursesSize;
}
//--------------------------------------------------------------------------------------
List SemesterGetList(Semester semester){
	assert(semester !=NULL);
	return semester->courses;
}
//--------------------------------------------------------------------------------------
Course SemesterGetLastCourse(Semester semester,int course_id){
	assert(semester !=NULL);
	int found=-1,i=0;
	int index=SemesterFindLastCourse(semester,course_id,&found);
	if(found==-1){
		return NULL;
	}
    ListElement course=listGetFirst(semester->courses);
    while(i<index){
    	course=listGetNext(semester->courses);
    	i++;
    }
    return course;
}
//--------------------------------------------------------------------------------------
Course SemesterGetMaxCourse(Semester semester1, int  course_id){
	assert(semester1 !=NULL);
	int i=0,max=0;
	int index=SemesterFindMaxCourse(semester1,course_id, &max);
	if(index==-1 || index==-2 || index==-3){
		return NULL;
	}
	ListElement course=listGetFirst(semester1->courses);
	while(i<index){
	    course=listGetNext(semester1->courses);
	    i++;
	 }
    return course;
}
//-----------------------------------------------------------------------------------------------
int SemesterTotalPointsX2(Semester semester){
    assert(semester!=NULL);
    int sum=0;
    List courses=SemesterGetList(semester);
    ListElement course=listGetFirst(courses);
    while(course){
        sum+=pointsX2(courseGetPoints((Course)course));
        course=listGetNext(courses);
    }
    return sum;
}
//-----------------------------------------------------------------------------------------------
int SemesterFailPointsX2(Semester semester){
    assert(semester!=NULL);
    int sum=0;
    List temp=SemesterGetList(semester);
    ListElement temp2=listGetFirst(temp);
    while(temp2){
        if(courseGetGrade((Course)temp2)< FAILDGRADE){
            sum+=pointsX2(courseGetPoints((Course)temp2));
        }
        temp2=listGetNext(temp);
    }
    return sum;
}
//-----------------------------------------------------------------------------------------------
int SemesterEffectiveGradesAndPointsX2(Semester semester,int*sum2){
    assert(semester!=NULL);
    assert(sum2!=NULL);
    *sum2=0;
    int sum=0;
    int* hist=calloc(MAXIDNUM+1,sizeof(int));
    if(!hist){
    	return -1;
    }
    List courses=listCopy(semester->courses);
    ListElement temp2=listGetFirst(courses);
    while(temp2){
    	hist[courseGetId(temp2)]=1;
        Course course=SemesterGetLastCourse(semester,courseGetId((Course)temp2));
        if (!course){
            temp2=listGetNext(courses);
            continue;
        }
        sum+=courseGetGrade(course)*pointsX2(courseGetPoints(course));
        *sum2+=pointsX2(courseGetPoints(course));
        while(hist[courseGetId(temp2)]==1){
        	 temp2=listGetNext(courses);
        	 if(!temp2){
        		 listDestroy(courses);
        		 free(hist);
        		 return sum;
        	 }
        }
    }
    listDestroy(courses);
    free(hist);
    return sum;
}
//-----------------------------------------------------------------------------------
ListElement copySemester(ListElement semester) {
    assert(semester !=NULL);
    Semester new_semester=malloc(sizeof(*new_semester));
    if(!semester){
    	return NULL;
    }
    new_semester->semeseter_id=((Semester)semester)->semeseter_id;
    new_semester->coursesSize=((Semester)semester)->coursesSize;
    new_semester->courses=listCopy(((Semester)semester)->courses);
	return new_semester ;
}
//-----------------------------------------------------------------------------------

void freeSemester(ListElement semester){
	assert(semester!=NULL);
    SemesterDestroy((Semester)semester);
}
//-----------------------------------------------------------------------------------

int compareSemesters(ListElement semester1,ListElement semester2, ListSortKey key ){
	assert(semester1!=NULL);
	assert(semester2!=NULL);
	assert(key!=NULL);
	int distance1 = abs(((Semester)semester1)->semeseter_id - *(int*)key);
	int distance2 = abs(((Semester)semester2)->semeseter_id - *(int*)key);
	return distance1 - distance2;
}
//--------------------------------------------------------------------------------------
void SemesterDestroy(Semester semester1){
    assert(semester1 !=NULL);
    listDestroy(semester1->courses);
    free(semester1);
}
