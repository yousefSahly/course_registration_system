#include "Semester.h"
#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "Grade_sheet.h"
#define MAXIDNUM 1000000
#define COURSE_SPORTF 390000
#define COURSE_SPORTL 399999
struct grade_sheet_t{
	List semesters;
	int size;
};
//-----------------------------------------------------------------------------------
//this func is to check if a semester in in the grade sheet of the student.
//and if it's empty then a relevant enum is returned.
static GRADE_SHEETResult GradeSheetCheckList(GradeSheet sheet,int semester_id){
    assert(sheet !=NULL);
    ListElement temp=listGetFirst(sheet->semesters);
    if(temp==NULL){
    	return GRADE_SHEET_SEMESTER_EMPTY;
    }
   while(temp){
	   if(SemesterGetId(temp)==semester_id){
		   return GRADE_SHEET_SEMESTER_ALREADY_EXIST;
	   }
	   temp=listGetNext(sheet->semesters);
   }
   return GRADE_SHEET_SEMESTER_DOES_NOT_EXIST;
}
//----------------------------------------------------------------------------------------
int GradeSheetFindLastSemester(GradeSheet sheet,int course_id){
	assert(sheet!=NULL);
	int found=-1;
	List semesters=listCopy(sheet->semesters);
	ListElement semester=listGetFirst(semesters);
	int i=0,temp_semester_id=-1;
	while(i<sheet->size-1){
	      int index=SemesterFindLastCourse((Semester)semester,course_id,&found);
	      if(index==-1 || found==-1){
		     semester=listGetNext(semesters);
		     i++;
	      }
	      else if(found!=-1){
	    	  temp_semester_id=SemesterGetId(semester);
	          semester=listGetNext(semesters);
	          i++;
	      }
	}
	int index=SemesterFindLastCourse((Semester)semester,course_id,&found);
	if(index!=-1){
		int temp=SemesterGetId(semester);
		listDestroy(semesters);
		return temp;
	}
	semester=listGetFirst(semesters);
	while(semester){
		if(SemesterGetId(semester)==temp_semester_id){
			int temp=SemesterGetId(semester);
			listDestroy(semesters);
			return temp;
		}
        semester=listGetNext(semesters);
	}
	int temp=SemesterGetId(semester);
	listDestroy(semesters);
	return temp;
}
//----------------------------------------------------------------------------------------
GradeSheet copyGradeSheet(GradeSheet sheet){
	assert(sheet!=NULL);
	GradeSheet new_sheet=malloc(sizeof(*sheet));
	new_sheet->size=sheet->size;
	new_sheet->semesters=listCopy(sheet->semesters);
	return new_sheet;
}
//-----------------------------------------------------------------------------------
GRADE_SHEETResult GradeSheetCreate(GradeSheet * new_sheet){
	assert(new_sheet!=NULL);
	GradeSheet grade_sheet=malloc(sizeof(*grade_sheet));
	if(!grade_sheet){
		return GRADE_SHEET_MEMORY_ERROR;
	}
	grade_sheet->semesters= listCreate( copySemester, freeSemester);
	if (!grade_sheet->semesters){
		free(grade_sheet);
		return GRADE_SHEET_MEMORY_ERROR;
	}
	grade_sheet->size=0;
	*new_sheet=grade_sheet;
	return GRADE_SHEET_OK;
}
//-----------------------------------------------------------------------------------
GRADE_SHEETResult GradeSheetAddSemester(GradeSheet sheet,int Semester_id){
	assert(sheet!=NULL);
	Semester new_semester;
	if (Semester_id<=0){
		return GRADE_SHEET_INVALID_PARAMTER;
	}
	SEMESTERResult res= SemesterCreate(&new_semester,Semester_id);
	if (res==SEMESTER_MEMORY_ERROR){
		return GRADE_SHEET_MEMORY_ERROR;
	}
	GRADE_SHEETResult res3=GradeSheetCheckList( sheet, Semester_id);
	if(res3==GRADE_SHEET_SEMESTER_ALREADY_EXIST){
		SemesterDestroy(new_semester);
		return GRADE_SHEET_SEMESTER_ALREADY_EXIST;
	}
	ListResult res2=listInsertLast(sheet->semesters,new_semester);
	if (res2 ==LIST_NULL_ARGUMENT){
		SemesterDestroy(new_semester);
		return GRADE_SHEET_MEMORY_ERROR;
	}
	else if (res2 ==LIST_OUT_OF_MEMORY){
		SemesterDestroy(new_semester);
		return GRADE_SHEET_MEMORY_ERROR;
	}
	sheet->size++;
	int key=0;
	listSort(sheet->semesters,compareSemesters,&key);
	SemesterDestroy(new_semester);
	return GRADE_SHEET_OK;
}
//-----------------------------------------------------------------------------------
GRADE_SHEETResult GradeSheetAddCourse(GradeSheet sheet,int course_id,int grade,char* points, int semester_id1){
	assert(sheet!=NULL);
	assert(points!=NULL);
	ListElement semester=listGetFirst(sheet->semesters);
	if(!semester){
		return GRADE_SHEET_IS_EMPTY;
	}
	int i=0;
	while (i<sheet->size){
		if(SemesterGetId(semester)==semester_id1){
		SEMESTERResult res=SemesterAddCourse(semester,course_id,points,grade);
			if(res == SEMESTER_INVALID_PARAMETER){
				return GRADE_SHEET_INVALID_PARAMTER;
			}
			else if(res == SEMESTER_MEMORY_ERROR){
				return GRADE_SHEET_MEMORY_ERROR;
			}
			return GRADE_SHEET_OK;
		}
		semester=listGetNext(sheet->semesters);
     i++;
	}
	return GRADE_SHEET_SHOULD_NEVER_REACH_HERE; //should never get here;
}
//-----------------------------------------------------------------------------------
GRADE_SHEETResult GradeSheetRemoveCourseFromSemester(GradeSheet sheet,int course_id,int semester_id){
	assert(sheet!=NULL);
	ListElement semester=listGetFirst(sheet->semesters);
	if(!semester){
		return GRADE_SHEET_IS_EMPTY;
	}
	int i=0;
	if(sheet->size==0){
		return GRADE_SHEET_IS_EMPTY;
	}
	while (i<sheet->size){
		if(SemesterGetId(semester)==semester_id){
			SEMESTERResult res=SemesterRemoveCourse(semester, course_id);
			if (res==SEMESTER_INVALID_PARAMETER){
				return GRADE_SHEET_INVALID_PARAMTER;
			}
			else if (res==SEMESTER_IS_EMPTY){
				return GRADE_SHEET_SEMESTER_IS_EMPTY;
			}
			else if (res==SEMESTER_COURSE_DOES_NOT_EXIST ){
				return GRADE_SHEET_SEMESTER_COURSE_DOESNT_EXIST;
			}
			else if(res==SEMESTER_MEMORY_ERROR){
				return GRADE_SHEET_MEMORY_ERROR;
			}
			return GRADE_SHEET_OK;
		}
		semester=listGetNext(sheet->semesters);
      i++;
	}
	return GRADE_SHEET_SEMESTER_DOES_NOT_EXIST;
}
//----------------------------------------------------------------------------------
GRADE_SHEETResult GradeSheetRemoveEmptySemester(GradeSheet sheet){
	assert(sheet !=NULL);
	ListElement semester=listGetFirst(sheet->semesters);
	while(semester){
		if(SemesterGetSize(semester)==0){
			listRemoveCurrent(sheet->semesters);
			sheet->size--;
		}
		semester=listGetNext(sheet->semesters);
	}
	int key=0;
	listSort(sheet->semesters,compareSemesters,&key);
	return GRADE_SHEET_OK;
}
//----------------------------------------------------------------------------------
GRADE_SHEETResult GradeSheetUpdateCourse(GradeSheet sheet,int course_id,int grade){
	assert(sheet !=NULL);
    int i=0,found=-1,iterator=-1;
    ListElement temp_semester=listGetFirst(sheet->semesters);
    if(!temp_semester){
    		return GRADE_SHEET_IS_EMPTY;
    	}
    while(i<sheet->size){
         int res=SemesterFindLastCourse(temp_semester,course_id,&found);
         if(found==-1 || res==-1){
    	    temp_semester=listGetNext(sheet->semesters);
    	    i++;
         }
         else {
        	found=-1;
         	iterator=i;
    	    temp_semester=listGetNext(sheet->semesters);
    	    i++;
         }
    }
    if(iterator==-1){
    	return GRADE_SHEET_COURSE_DOES_NOT_EXIST;
    }
    i=0;
    temp_semester=listGetFirst(sheet->semesters);
    while(i<iterator){
    	temp_semester=listGetNext(sheet->semesters);
    	i++;
    }
    SEMESTERResult res2= SemesterUpdateCourse(temp_semester,course_id,grade);
    if(res2== SEMESTER_INVALID_PARAMETER){
    	return GRADE_SHEET_INVALID_PARAMTER;
    }
    else if(res2==SEMESTER_MEMORY_ERROR){
    	return GRADE_SHEET_MEMORY_ERROR;
    }
 return GRADE_SHEET_OK;
}
//----------------------------------------------------------------------------------------
List GradeSheetGetList(GradeSheet sheet){
	assert(sheet !=NULL);
	return sheet->semesters;
}
//----------------------------------------------------------------------------------------
int GradeSheetGetSize(GradeSheet sheet){
	assert(sheet !=NULL);
	return sheet->size;
}
//----------------------------------------------------------------------------------------
int GradeSheetTotalPointsX2(GradeSheet sheet){
	assert(sheet !=NULL);
	int key=0;
	listSort(sheet->semesters,compareSemesters,&key);
	int sum=0;
	ListElement semester=listGetFirst(sheet->semesters);
	while (semester){
		sum+=SemesterTotalPointsX2((Semester)semester);
		semester=listGetNext(sheet->semesters);
	}
	return sum;
}
//----------------------------------------------------------------------------------------
int GradeSheetFailedPointsX2(GradeSheet sheet){
	assert(sheet !=NULL);
	int key=0;
	listSort(sheet->semesters,compareSemesters,&key);
	int sum=0;
	ListElement temp=listGetFirst(sheet->semesters);
	while (temp){
		sum+=SemesterFailPointsX2((Semester)temp);
		temp=listGetNext(sheet->semesters);
	}
	return sum;
}
//----------------------------------------------------------------------------------------
int GradeSheetEffectivePointsAndGradesSportX2(GradeSheet sheet, int *sum2){
	assert(sum2!=NULL);
	assert(sheet !=NULL);
	*sum2=0;
	int key=0;
	int* hist=calloc(MAXIDNUM+1,sizeof(int));
	if(!hist){
		return -1;
	}
	listSort(sheet->semesters,compareSemesters,&key);
	int sum=0;
	int grade=0,points=0;
	ListElement semester=listGetFirst(sheet->semesters);
	while (semester){
		ListElement course=listGetFirst(SemesterGetList((Semester)semester));
		while (course){
			hist[courseGetId((Course)course)]=1;
			if (courseGetId((Course)course)>=COURSE_SPORTF && courseGetId((Course)course)<=COURSE_SPORTL){
				SEMESTERResult res=SemesterGetValidGrade((Semester)semester, courseGetId((Course)course), &grade,&points);
				if(res==SEMESTER_COURSE_DOES_NOT_EXIST){
					course=listGetNext(SemesterGetList((Semester)semester));
					continue;
				}
				else if(res==SEMESTER_IS_EMPTY){
				        semester=listGetNext(sheet->semesters);
				        for(int i=0;i<MAXIDNUM+1;i++){
				        	hist[i]=0;
				        }
				        if(semester==NULL){
				        	free(hist);
				        	return sum;
				        }
				        course=listGetFirst(SemesterGetList((Semester)semester));
				        continue;
				}
				sum+=grade*points;
				*sum2+=points;
			}
			while(hist[courseGetId((Course)course)]==1){
				 course=listGetNext(SemesterGetList((Semester)semester));
				 if(course==NULL){
					 semester=listGetNext(sheet->semesters);
					 for(int i=0;i<MAXIDNUM+1;i++){
					       hist[i]=0;
					 }
					 if(semester==NULL){
						 free(hist);
						 return sum;
					 }
					 course=listGetFirst(SemesterGetList((Semester)semester));
					 break;
			    }
			}
		}
	}
	free(hist);
	return sum;
}
//----------------------------------------------------------------------------------------
int GradeSheetEffectivePointsAndGradesOtherX2(GradeSheet sheet, int *sum2){
	assert(sum2!=NULL);
	assert(sheet !=NULL);
	*sum2=0;
	int key=0;
	int* hist=calloc(MAXIDNUM+1,sizeof(int));
	if(!hist){
		return -1;
	}
	listSort(sheet->semesters,compareSemesters,&key);
	int sum=0,temp_sum=0,temp_sum2=*sum2;
	int grade=0,points=0;
	List temp_list=listCopy(sheet->semesters);
	ListElement semester=listGetFirst(sheet->semesters);
	ListElement course=listGetFirst(SemesterGetList((Semester)semester));
	while (course){
		hist[courseGetId((Course)course)]=1;
		ListElement temp_semester=listGetFirst(temp_list);
		while (temp_semester){
			if (courseGetId((Course)course)<COURSE_SPORTF || courseGetId((Course)course)>COURSE_SPORTL){
				SEMESTERResult res=SemesterGetValidGrade((Semester)temp_semester, courseGetId((Course)course), &grade,&points);
				if(res==SEMESTER_COURSE_DOES_NOT_EXIST || res==SEMESTER_IS_EMPTY){
					temp_semester=listGetNext(temp_list);
					continue;
				}
				temp_sum=grade*points;
				temp_sum2=points;
			}
			temp_semester=listGetNext(temp_list);
		}
        sum+=temp_sum;
        *sum2+=temp_sum2;
		temp_sum=0;
		temp_sum2=0;
		while(hist[courseGetId((Course)course)]==1){
			 course=listGetNext(SemesterGetList((Semester)semester));
			 if(course==NULL){
				semester=listGetNext(sheet->semesters);
				if(semester==NULL){
					listDestroy(temp_list);
					free(hist);
					return sum;
				}
				course=listGetFirst(SemesterGetList((Semester)semester));
			 }
		}
	}
	listDestroy(temp_list);
	free(hist);
	return sum;
}
//--------------------------------------------------------------------------------------------
int GradeSheetEffectivePointX2AndGradeOther(GradeSheet sheet, int *sum2, int course_id,int*exist){
	assert(sum2!=NULL);
	assert(sheet !=NULL);
	*sum2=0;
	int key=0;
	listSort(sheet->semesters,compareSemesters,&key);
	if(exist[course_id]==1){
		return -1;
	}
	exist[course_id]=1;
    int temp_sum=0,temp_sum2=*sum2;
    int grade=0,points=0;
    int semester_id=GradeSheetFindLastSemester(sheet,course_id);
    List semesters=listCopy(sheet->semesters);
    ListElement semester=listGetFirst(semesters);
    while(semester){
    	if(SemesterGetId(semester)==semester_id){
           if (course_id<COURSE_SPORTF || course_id>COURSE_SPORTL){
               SemesterGetValidGrade(semester, course_id, &grade,&points);
               Course course=SemesterGetLastCourse(semester,course_id);
               if(!course){
            	  listDestroy(semesters);
        	      return 0;
               }
               temp_sum=grade;
               temp_sum2=points;
          }
          break;
       }
    	semester=listGetNext(semesters);
    }
    *sum2=temp_sum2;
	listDestroy(semesters);
    return temp_sum;
}
//----------------------------------------------------------------------------------------
int GradeSheetEffectivePointsAndGradesX2(GradeSheet sheet,int*total_points){
	assert(sheet !=NULL);
	assert(total_points !=NULL);
	int points_other=0,sum_other=0,points_sport=0,sum_sport=0;
	sum_other=GradeSheetEffectivePointsAndGradesOtherX2(sheet,&points_other);
	sum_sport=GradeSheetEffectivePointsAndGradesSportX2(sheet,&points_sport);
	*total_points=points_other+points_sport;
	return sum_other+sum_sport;
}
//----------------------------------------------------------------------------------------
void GradeSheetDestroy(GradeSheet  sheet){
	assert(sheet!=NULL);
	listDestroy(sheet->semesters);
	free(sheet);
}

