#include "set.h"
#include "student.h"
#include "mtm_ex3.h"
#include "Grade_sheet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "course_manager.h"
#define COURSE_SPORTF 390000
#define COURSE_SPORTL 399999
#define MAXIDNUM 1000000
#define MAXGRADE 100
#define MAX_STUDENT_ID_NUM 1000000000
#define MINGRADE 0
#define NOT_CONNECTED -1
struct course_manager_t{
    Set students;
    int students_size;
    int connected_id;    //this is a log in flag
    List grades;
    List relevant_frineds;
};
//----------------------------------------------------------------------------------
//this two structs is for the reports
//this one is to hold an information of a grade that we want to use it later for the report.
struct grade_t{
    int course_id;
    int semester_id;
    int grade;
    int points;
};
//----------------------------------------------------------------------------------
//this one is to hold an information of a friend that we want to use it later for the reference report.
struct relevant_frined_t{
    int id;
    int grade;
    char* Fname;
    char* Lname;
};
//----------------------------------------------------------------------------------
static int compareClean(ListElement grade1,ListElement grade2,ListSortKey key){
	 int distance1 = abs((int)((Grade)grade1)->course_id - *(int*)key);
	 int distance2 = abs((int)((Grade)grade2)->course_id - *(int*)key);
	 int res= distance1 - distance2;
	 if(res==0){
	   distance1 = abs((int)((Grade)grade1)->semester_id);
       distance2 = abs((int)((Grade)grade2)->semester_id);
	   return distance1-distance2;
	}
	return res;
}
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
//------------------------------------------------------------------------------------
//this func is to copy the data of a relevant friend.
static ListElement CopyRelFriend(ListElement relevant_frined){
    assert(relevant_frined!=NULL);
    RelevantFriend new_relevant_frined=malloc(sizeof(*new_relevant_frined));
    if(!new_relevant_frined){
        return NULL;
    }
    new_relevant_frined->id=((RelevantFriend)relevant_frined)->id;
    new_relevant_frined->grade=((RelevantFriend)relevant_frined)->grade;
    new_relevant_frined->Fname=malloc(strlen(((RelevantFriend)relevant_frined)->Fname)+1);
    if(!new_relevant_frined->Fname){
    	free(new_relevant_frined);
    	return NULL;
    }
    new_relevant_frined->Lname=malloc(strlen(((RelevantFriend)relevant_frined)->Lname)+1);
    if(!new_relevant_frined->Lname){
      	free(new_relevant_frined);
       	free(new_relevant_frined->Fname);
       	return NULL;
    }
    strcpy(new_relevant_frined->Fname,((RelevantFriend)relevant_frined)->Fname);
    strcpy(new_relevant_frined->Lname,((RelevantFriend)relevant_frined)->Lname);
    return new_relevant_frined;
}
//------------------------------------------------------------------------------------
//this func is to free to data of a relevant friend.
static void freeRelFriend(ListElement relevant_friend){
	free(((RelevantFriend)relevant_friend)->Fname);
	free(((RelevantFriend)relevant_friend)->Lname);
    free(relevant_friend);
}
//------------------------------------------------------------------------------------
////this func is to compare the grades of two relevant friends.

static int compareGradesForRelFriends(ListElement relevant_friend, ListElement relevant_friend2, ListSortKey key){
	assert(relevant_friend!=NULL);
	assert(relevant_friend2!=NULL);
	assert(key!=NULL);
    int distance1 = abs((int)((RelevantFriend)relevant_friend)->grade - *(int*)key);
    int distance2 = abs((int)((RelevantFriend)relevant_friend2)->grade - *(int*)key);
    int res = distance1-distance2;
    if(res==0){
        distance1 = abs((int)((RelevantFriend)relevant_friend)->id);
        distance1 = abs((int)((RelevantFriend)relevant_friend2)->id);
        int res2=distance1-distance2;
        return res2;
    }
    return res;
}
//-----------------------------------------------------------------------------------------------------------
//the next two functions is to decide if to accept the friend request or not.
static COURSE_MANEGERResult CourseMangerHandleFriendRequestIn( CourseManager manager,int other_id, char* action){
	 assert(manager!=NULL);
     assert(action!=NULL);
	 int key=0;
	 Student student=CourseManagerGetStudentViaId(manager,manager->connected_id);
	    ListElement friend=listGetFirst(StudentGetFriends((Student)student));
	    while(friend){
	         if (*((int*)friend)==other_id){
	             return COURSE_MANAGER_ALREADY_FRIEND;
	         }
	         friend=listGetNext(StudentGetFriends((Student)student));
	    }
	    bool check=setIsIn(StudentGetFriendRequestIn(student),(SetElement)&other_id);
	    if (check==false) {
	       return COURSE_MANAGER_NOT_REQUESTED;
	    }
	    if (strcmp(action, "accept")!=0 && strcmp(action, "reject")!=0){
	           return COURSE_MANAGER_INVALID_PARAMETERS;
	    }
	    if (strcmp(action, "accept")==0){
	        listInsertLast(StudentGetFriends(student), (ListElement)&other_id);
	        ListResult res= listSort(StudentGetFriends(student),compareIntL,&key);
	        if(res==LIST_OUT_OF_MEMORY){
	            return COURSE_MANAGER_MEMORY_ERROR;
	        }
	        setRemove(StudentGetFriendRequestIn(student),(SetElement)&other_id);
	    }
	    if(strcmp(action, "reject")==0){
	        setRemove(StudentGetFriendRequestIn(student),(SetElement)&other_id);
	    }
	    return COURSE_MANAGER_OK;
}
//----------------------------------------------------------------------------------------------------------
static COURSE_MANEGERResult CourseMangerHandelFriendRequestOut(CourseManager manager,int other_id, char* action){
	 assert(manager!=NULL);
	 assert(action!=NULL);
	 int key=0;
	 Student student=CourseManagerGetStudentViaId(manager,other_id);
	    bool check=setIsIn(StudentGetFriendRequestOut(student),&(manager->connected_id));
	    if (check==false) {
	        return COURSE_MANAGER_NOT_REQUESTED;
	    }
	    if (strcmp(action, "accept")!=0 && strcmp(action, "reject")!=0){
	           return COURSE_MANAGER_INVALID_PARAMETERS;
	    }
	    if (strcmp(action, "accept")==0){
	       listInsertLast(StudentGetFriends(student), &(manager->connected_id));
	       ListResult res= listSort(StudentGetFriends(student),compareIntL,&key);
	       if(res==LIST_OUT_OF_MEMORY){
	          return COURSE_MANAGER_MEMORY_ERROR;
	       }
	       setRemove(StudentGetFriendRequestOut(student),&(manager->connected_id));
	    }

	 return COURSE_MANAGER_OK;
}
//-----------------------------------------------------------------------------------
//
static  COURSE_MANEGERResult copyFriendsManager(CourseManager manager, int course_grade,int student_id,char* fname,char*lname){
	assert(manager!=NULL);
	RelevantFriend relevant_friend=malloc(sizeof(*relevant_friend));
	if(!relevant_friend){
	   return COURSE_MANAGER_MEMORY_ERROR;
	}
	relevant_friend->grade=course_grade;
	relevant_friend->id=student_id;
	relevant_friend->Fname=malloc(sizeof(char)*(strlen(fname)+1));
	if(!relevant_friend->Fname){
		free(relevant_friend);
		return COURSE_MANAGER_OUT_OF_MEMORY;
	}
	relevant_friend->Lname=malloc(sizeof(char)*(strlen(lname)+1));
	if(!relevant_friend->Lname){
		free(relevant_friend);
		free(relevant_friend->Fname);
		return COURSE_MANAGER_OUT_OF_MEMORY;
	}
	strcpy(relevant_friend->Fname,fname);
	strcpy(relevant_friend->Lname,lname);
	ListResult res=listInsertLast(manager->relevant_frineds,relevant_friend);
	if(res==LIST_OUT_OF_MEMORY){
	  free(relevant_friend->Lname);
	  free(relevant_friend->Fname);
	  free(relevant_friend);
	  return COURSE_MANAGER_MEMORY_ERROR;
	}
	free(relevant_friend->Fname);
	free(relevant_friend->Lname);
	free(relevant_friend);
	return COURSE_MANAGER_OK;
}
//----------------------------------------------------------------------------------
//this func searches in the students friends list for the relevant friends that are taking the
//course that the student have ordered a reference for.
static COURSE_MANEGERResult findRelevantFriends(CourseManager manager, int course_id){
    assert(manager!=NULL);
	int temp_max_grade=0;
    Student student=CourseManagerGetStudentViaId(manager,manager->connected_id);
    List friends=StudentGetFriends(student);
    ListElement friend_elmnt=listGetFirst(friends);
    while(friend_elmnt){
          Student friend=CourseManagerGetStudentViaId(manager,*((int*)friend_elmnt));
          GradeSheet friend_grade_sheet=StudentGetGradeSheet((Student)friend);
          if(GradeSheetGetSize(friend_grade_sheet)==0){
             friend_elmnt=listGetNext(friends);
             temp_max_grade=0;
             continue;
          }
          List friend_semesters=GradeSheetGetList(friend_grade_sheet);
          ListElement friend_semester=listGetFirst(friend_semesters);
          Course friend_course;
          while(friend_semester){
        	   friend_course=SemesterGetMaxCourse(friend_semester,course_id);
               if(!friend_course){
                  friend_semester=listGetNext(friend_semesters);
                  continue;
        	   }
               if(courseGetGrade(friend_course)>temp_max_grade){
            	   temp_max_grade=courseGetGrade(friend_course);
               }
               friend_semester=listGetNext(friend_semesters);
          }
        COURSE_MANEGERResult res=copyFriendsManager(manager,temp_max_grade,
                                                    StudentGetId(friend),StudentGetFirstName(friend),StudentGetLastName(friend));
        if(res==COURSE_MANAGER_MEMORY_ERROR) {
            return COURSE_MANAGER_MEMORY_ERROR;
        }
        temp_max_grade=0;
        friend_elmnt=listGetNext(friends);
    }
    return COURSE_MANAGER_OK;
}
//----------------------------------------------------------------------------------
//this func copies the data of a grade of a grade struct.
//and creates a new one. this func is for the sort of the list grades.
static ListElement copyGrade(ListElement grade){
    assert(grade!=NULL);
    Grade new_grade=malloc(sizeof(*new_grade));
    if(!new_grade){
        return NULL;
    }
    new_grade->grade=((Grade)grade)->grade;
    new_grade->course_id=((Grade)grade)->course_id;
    new_grade->semester_id=((Grade)grade)->semester_id;
    new_grade->points=((Grade)grade)->points;
    return new_grade;
}
//------------------------------------------------------------------------------------
//this func is to free the data of a grade.
static void freeGrade(ListElement grade){
    free(grade);
}
//------------------------------------------------------------------------------------
//this func is to compare between two grades. if equals then compares two semesters. if equals
//then compares two courses ides.
//this func is for the report.
static int compareGrades(ListElement grade, ListElement grade2, ListSortKey key){
    int distance1 = abs((int)((Grade)grade)->grade- *(int*)key);
    int distance2 = abs((int)((Grade)grade2)->grade - *(int*)key);
    int res= distance1 - distance2;
    if(res==0){
        distance1 = abs((int)((Grade)grade)->semester_id);
        distance2 = abs((int)((Grade)grade2)->semester_id);
        int res2=distance1-distance2;
        if(res2==0){
            distance1 = abs((int)((Grade)grade)->course_id);
            distance2 = abs((int)((Grade)grade2)->course_id);
            return distance1-distance2;
        }
        return res2;
    }
    return res;
}
//-------------------------------------------------------------------------------------------------
//this func is to fill the details of the grade in the grade struct.
static COURSE_MANEGERResult CoureManagerCopy(CourseManager manager,int grade ,int course_id,int semester_id,int points){
	assert(manager!=NULL);
	Grade grade1=malloc(sizeof(*grade1));
	if(!grade1){
	   return COURSE_MANAGER_MEMORY_ERROR;
    }
	grade1->grade=grade;
	grade1->semester_id=semester_id;
	grade1->course_id=course_id;
	grade1->points=points;
    ListResult res=listInsertLast(manager->grades,grade1);
	if (res==LIST_OUT_OF_MEMORY){
	   free(grade1);
	   return COURSE_MANAGER_MEMORY_ERROR;
	}
	free(grade1);
	return COURSE_MANAGER_OK;
}
//------------------------------------------------------------------------------------
//this func searhces for the effective grades for the student. using functions from before.
static COURSE_MANEGERResult CourseMangerEffectiveGrades(CourseManager manager){
    assert(manager!=NULL);
    int grade_sport=0, grade=0, points=0,temp=0,sport_points=0;
    int* exist=calloc(MAXIDNUM+1,sizeof(int));
    if(!exist){
    	return COURSE_MANAGER_OUT_OF_MEMORY;
    }
    Student student=CourseManagerGetStudentViaId(manager,manager->connected_id);
    GradeSheet sheet=StudentGetGradeSheet((Student)student);
    List semesters=GradeSheetGetList((GradeSheet)sheet);
    ListElement semester=listGetFirst(semesters);
    while(semester){
         ListElement course=listGetFirst(SemesterGetList((Semester)semester));
         while(course){
              if(courseGetId((Course)course)>=COURSE_SPORTF && courseGetId((Course)course)<=COURSE_SPORTL){
                  SemesterGetValidGrade((Semester)semester,courseGetId((Course)course),&grade_sport,&sport_points);
                  COURSE_MANEGERResult res=CoureManagerCopy(manager,grade_sport ,courseGetId((Course)course),
                		                     SemesterGetId(semester),sport_points);
                  if(res==COURSE_MANAGER_MEMORY_ERROR){
      	        	 free(exist);
                	 return COURSE_MANAGER_MEMORY_ERROR;
                  }
             }
             else {
                  grade=GradeSheetEffectivePointX2AndGradeOther((GradeSheet)sheet,
                		                       &points,courseGetId((Course)course),exist);
                  while(grade==-1){
                	  course=listGetNext(SemesterGetList((Semester)semester));
                	  if(course==NULL){
                	        semester=listGetNext(semesters);
                	        if(semester==NULL){
                	        	free(exist);
                	        	return COURSE_MANAGER_OK;
                	        }
                	        course=listGetFirst(SemesterGetList((Semester)semester));
                	  }
                	  if(courseGetId((Course)course)>=COURSE_SPORTF && courseGetId((Course)course)<=COURSE_SPORTL){
                	     SemesterGetValidGrade((Semester)semester,courseGetId((Course)course),&grade_sport,&sport_points);
                	     COURSE_MANEGERResult res=CoureManagerCopy(manager,grade_sport ,courseGetId((Course)course),
                	                  		                     SemesterGetId(semester),sport_points);
                	     if(res==COURSE_MANAGER_MEMORY_ERROR){
                	        free(exist);
                	        return COURSE_MANAGER_MEMORY_ERROR;
                	    }
                	     break;
                	  }
                	  else{
                	  grade=GradeSheetEffectivePointX2AndGradeOther((GradeSheet)sheet,
                	                  		                       &points,courseGetId((Course)course),exist);
                	  }
                  }
                  if(courseGetId((Course)course)>=COURSE_SPORTF && courseGetId((Course)course)<=COURSE_SPORTL){
                	  course=listGetNext(SemesterGetList((Semester)semester));
                	  continue;
                  }
                  temp=GradeSheetFindLastSemester(sheet,courseGetId((Course)course));
                  COURSE_MANEGERResult res=CoureManagerCopy(manager,grade,courseGetId((Course)course),temp,points);
                  if(res==COURSE_MANAGER_MEMORY_ERROR){
      	        	 free(exist);
                     return COURSE_MANAGER_MEMORY_ERROR;
                  }
            }
            course=listGetNext(SemesterGetList((Semester)semester));
        }
        semester=listGetNext(semesters);
    }
	free(exist);
    return COURSE_MANAGER_OK;
}
//-----------------------------------------------------------------------------------------------
//this func check if the student is in the system.
static int CheckIStudent(CourseManager manager, int student_id){
	assert(manager!=NULL);
    int found=0;
    SetElement temp=setGetFirst(manager->students);
    while (temp){
        if (StudentGetId((Student)temp)==student_id){
            found=1;
            return found;
        }
        temp=setGetNext(manager->students);
    }
    return found;
}
//-----------------------------------------------------------------------------------------------
CourseManager CourseMangerCreate(){
    CourseManager manager1 =malloc(sizeof(*manager1));
    if (!manager1){
        return NULL;
    }
    Set student_res=setCreate(copyStudent,freeStudent,compareStudents);
    if (!student_res){
        free(manager1);
        return NULL;
    }
    manager1->grades=listCreate(copyGrade, freeGrade);
    if(!manager1->grades){
        free(manager1);
        setDestroy(student_res);
        return NULL;
    }
    manager1->relevant_frineds=listCreate(CopyRelFriend, freeRelFriend);
    if(!manager1->relevant_frineds){
        free(manager1);
        setDestroy(student_res);
        listDestroy(manager1->grades);
        return NULL;
    }
    manager1->students_size=0;
    manager1->connected_id=-1;   //when the flag is -1. this means that no student is connected.
    manager1->students=student_res;
    return manager1;
}
//-----------------------------------------------------------------------------------------------
Student CourseManagerGetStudentViaId(CourseManager manager,int student_id){
	assert(manager!=NULL);
	SetElement student=setGetFirst(manager->students);
	if(student==NULL){
		return NULL;
	}
	while(student){
		if(StudentGetId((Student)student)==student_id){
			return student;
		}
		student=setGetNext(manager->students);
	}
	return NULL;
}
//-----------------------------------------------------------------------------------------------
COURSE_MANEGERResult CourseMangerAddStudent(CourseManager manager, int student_id, char* Fname, char* Lname){
    assert(manager!=NULL);
    assert(Fname!=NULL);
    assert(Lname!=NULL);
    int found=CheckIStudent(manager,student_id);
    if(found==1){
        return COURSE_MANAGER_STUDENT_ALREADY_EXISTS;
    }
    Student student;
    STUDENTResult res= StudentCreate(&student,student_id,Fname,Lname);
    if (res==STUDENT_INVALID_PARAMETER){
        return COURSE_MANAGER_INVALID_PARAMETERS;
    }
    else if (res==STUDENT_MEMORY_ERROR){
        return COURSE_MANAGER_MEMORY_ERROR;
    }
    SetResult res2= setAdd(manager->students, student);
    if (res2==SET_NULL_ARGUMENT){
        StudentDestroy(student);
        return COURSE_MANAGER_INVALID_PARAMETERS;
    }
    else if(res2==SET_OUT_OF_MEMORY){
        StudentDestroy(student);
        return COURSE_MANAGER_MEMORY_ERROR;
    }
    manager->students_size++;
    StudentDestroy(student);
    return COURSE_MANAGER_OK;
}
//-----------------------------------------------------------------------------------------------
COURSE_MANEGERResult CourseMangerRemoveStudent(CourseManager manager, int student_id){
    assert(manager!=NULL);
    if (student_id <=0 || student_id >=MAX_STUDENT_ID_NUM){
        return COURSE_MANAGER_STUDENT_DOES_NOT_EXIST;
    }
    Student student=CourseManagerGetStudentViaId(manager,student_id);
    if (!student){
        return COURSE_MANAGER_STUDENT_DOES_NOT_EXIST;
    }
    setRemove(manager->students,student);
    //here we delete all the relevant memory of the student from the world!
    SetElement student2=setGetFirst(manager->students);
    if (!student2){
        manager->students_size--;
        return COURSE_MANAGER_OK;
    }
    while (student2){
        ListElement friend=listGetFirst(StudentGetFriends((Student)student2));
        while(friend){
            if(*((int*)friend)==student_id){
                listRemoveCurrent(StudentGetFriends((Student)student2));
            }
            friend=listGetNext(StudentGetFriends((Student)student2));
        }
        bool check=setIsIn(StudentGetFriendRequestIn(student2),(SetElement)&student_id);
        if (check==true){
            setRemove(StudentGetFriendRequestIn(student2), (SetElement)&student_id);
        }
        check=setIsIn(StudentGetFriendRequestOut(student2),(SetElement)&student_id);
        if (check==true) {
            setRemove(StudentGetFriendRequestOut(student2), (SetElement) &student_id);
        }
        student2=setGetNext(manager->students);
    }
    manager->students_size--;
    return COURSE_MANAGER_OK;
}
//-----------------------------------------------------------------------------------------------
COURSE_MANEGERResult CourseMangerLogin(CourseManager manager ,int student_id){
    assert(manager!=NULL);
    int found=0;
    if (manager->connected_id !=NOT_CONNECTED){
       return COURSE_MANAGER_ALREADY_LOGGED_IN;
    }
    if (student_id <=0 || student_id >=MAX_STUDENT_ID_NUM){
        return COURSE_MANAGER_STUDENT_DOES_NOT_EXIST;
    }
    found=CheckIStudent(manager, student_id);
    if (found==0){
        return COURSE_MANAGER_STUDENT_DOES_NOT_EXIST;
    }
    manager->connected_id=student_id;
    return COURSE_MANAGER_OK;
}
//-----------------------------------------------------------------------------------------------
COURSE_MANEGERResult CourseMangerLogout(CourseManager manager){
    assert(manager!=NULL);
    if (manager->connected_id==NOT_CONNECTED){
        return COURSE_MANAGER_NOT_LOGGED_IN;
    }
    manager->connected_id=NOT_CONNECTED;
    return COURSE_MANAGER_OK;
}
//-----------------------------------------------------------------------------------------------
COURSE_MANEGERResult CourseMangerFriendRequest(CourseManager manager, int other_id){
    assert(manager!=NULL);
    int found=0;
    if (manager->connected_id==NOT_CONNECTED){
        return COURSE_MANAGER_NOT_LOGGED_IN;
    }
    if (other_id <=0 || other_id >=MAX_STUDENT_ID_NUM){
        return COURSE_MANAGER_STUDENT_DOES_NOT_EXIST;
    }
    found=CheckIStudent(manager,other_id);
    if (found==0){
        return COURSE_MANAGER_STUDENT_DOES_NOT_EXIST;
    }
    if(manager->connected_id==other_id){
    	return COURSE_MANAGER_ALREADY_FRIEND;
    }
    Student student=CourseManagerGetStudentViaId(manager,manager->connected_id);
    bool res= StudentFindFriendViaId(student,other_id);
    if(res==true){
    	return COURSE_MANAGER_ALREADY_FRIEND;
    }
    //here we've confirmed that they are not friends.
     bool check=setIsIn(StudentGetFriendRequestOut(student),(SetElement)&other_id);
     if (check==true) {
        return COURSE_MANAGER_ALREADY_REQUESTED;
     }
     setAdd(StudentGetFriendRequestOut(student),(SetElement)&other_id);
     student=CourseManagerGetStudentViaId(manager,other_id);
     //here we've confirmed that they are not friends.
    check=setIsIn(StudentGetFriendRequestIn(student),&(manager->connected_id));
    if (check==true) {
          return COURSE_MANAGER_ALREADY_REQUESTED;
    }
    setAdd(StudentGetFriendRequestIn(student),&(manager->connected_id));
    return COURSE_MANAGER_OK;
}
//-----------------------------------------------------------------------------------------------
COURSE_MANEGERResult CourseMangerHandleFriendRequest(CourseManager manager,int other_id, char* action){
    assert(manager!=NULL);
    assert(action!=NULL);
    int found=0;
    if (manager->connected_id==NOT_CONNECTED){
         return COURSE_MANAGER_NOT_LOGGED_IN;
    }
    if (other_id <=0 || other_id >=MAX_STUDENT_ID_NUM){
        return COURSE_MANAGER_STUDENT_DOES_NOT_EXIST;
    }
    found=CheckIStudent(manager, other_id);
    if (found==0){
       return COURSE_MANAGER_STUDENT_DOES_NOT_EXIST;
    }
    COURSE_MANEGERResult res2=CourseMangerHandleFriendRequestIn( manager, other_id, action);
    if(res2== COURSE_MANAGER_ALREADY_FRIEND){
        	return COURSE_MANAGER_ALREADY_FRIEND;
    }
    else if(res2==COURSE_MANAGER_NOT_REQUESTED){
    	return COURSE_MANAGER_NOT_REQUESTED;
    }
    else if(res2==COURSE_MANAGER_INVALID_PARAMETERS){
        return COURSE_MANAGER_INVALID_PARAMETERS;
    }
    COURSE_MANEGERResult res= CourseMangerHandelFriendRequestOut( manager,other_id, action);
    if(res== COURSE_MANAGER_NOT_REQUESTED){
    	return COURSE_MANAGER_NOT_REQUESTED;
    }
    else if(res==COURSE_MANAGER_INVALID_PARAMETERS){
        return COURSE_MANAGER_INVALID_PARAMETERS;
    }
    return COURSE_MANAGER_OK;
}
//-----------------------------------------------------------------------------------------------
COURSE_MANEGERResult CourseMangerUnFriend(CourseManager manager, int other_id){
    assert(manager!=NULL);
    int found=0,key=0,flag1=-1;
    if (manager->connected_id==NOT_CONNECTED){
        return COURSE_MANAGER_NOT_LOGGED_IN;
    }
    if (other_id <=0 || other_id >=MAX_STUDENT_ID_NUM){
        return COURSE_MANAGER_STUDENT_DOES_NOT_EXIST;
    }
    found=CheckIStudent(manager, other_id);
    if (found==0){
    	return COURSE_MANAGER_STUDENT_DOES_NOT_EXIST;
    }
    Student student=CourseManagerGetStudentViaId(manager,manager->connected_id);
    ListElement friend=listGetFirst(StudentGetFriends(student));
    while(friend){
           if (*((int*)friend)==other_id){
               flag1=1;
               listRemoveCurrent(StudentGetFriends(student));
               ListResult res=listSort(StudentGetFriends(student),compareIntL,&key);
               if(res==LIST_OUT_OF_MEMORY){
                 return COURSE_MANAGER_MEMORY_ERROR;
               }
          }
          friend=listGetNext(StudentGetFriends(student));
   }
   if(flag1==-1){
      return COURSE_MANAGER_NOT_FRIEND;
   }
   student=CourseManagerGetStudentViaId(manager,other_id);
   friend=listGetFirst(StudentGetFriends(student));
   while(friend){
         if (*((int*)friend)==manager->connected_id){
            listRemoveCurrent(StudentGetFriends(student));
            ListResult res= listSort(StudentGetFriends(student),compareIntL,&key);
            if(res==LIST_OUT_OF_MEMORY){
               return COURSE_MANAGER_MEMORY_ERROR;
            }
         }
         friend=listGetNext(StudentGetFriends(student));
   }
   return COURSE_MANAGER_OK;
}
//-----------------------------------------------------------------------------------------------
COURSE_MANEGERResult CourseMangerAddGrade(CourseManager manager, int semester_id, int course_id,
                                          char* points, int grade){
    assert(manager!=NULL);
    assert(points!=NULL);
    //here I should check input correction.
    if (manager->connected_id==NOT_CONNECTED){
        return COURSE_MANAGER_NOT_LOGGED_IN;
    }
    SetElement student=setGetFirst(manager->students);
    while(student) {
        if (StudentGetId((Student) student) == manager->connected_id) {
            STUDENTResult res=StudentAddNewGrade((Student)student,semester_id, course_id,points, grade);
            if (res==STUDENT_INVALID_PARAMETER){
                return COURSE_MANAGER_INVALID_PARAMETERS;
            }
            else if(res==STUDENT_MEMORY_ERROR){
                return COURSE_MANAGER_MEMORY_ERROR;
            }
            break;
        }
        student=setGetNext(manager->students);
    }
    return COURSE_MANAGER_OK;
}
//-----------------------------------------------------------------------------------------------
COURSE_MANEGERResult CourseMangerUpdateGrade(CourseManager manager, int course_id, int new_grade){
    assert(manager!=NULL);
    if (manager->connected_id==NOT_CONNECTED){
        return COURSE_MANAGER_NOT_LOGGED_IN;
    }
    SetElement student=setGetFirst(manager->students);
    while(student) {
        if (StudentGetId((Student) student) == manager->connected_id) {
            GRADE_SHEETResult res=GradeSheetUpdateCourse(StudentGetGradeSheet((Student)student),course_id,new_grade);
            if(res==GRADE_SHEET_IS_EMPTY){
                return COURSE_MANAGER_COURSE_DOES_NOT_EXIST;
            }
            else if(res==GRADE_SHEET_COURSE_DOES_NOT_EXIST){
                return COURSE_MANAGER_COURSE_DOES_NOT_EXIST;
            }
            else if(res==GRADE_SHEET_INVALID_PARAMTER){
                return COURSE_MANAGER_INVALID_PARAMETERS;
            }
            else if(res==GRADE_SHEET_MEMORY_ERROR){
                return COURSE_MANAGER_MEMORY_ERROR;
            }
            break;
        }
        student=setGetNext(manager->students);
    }
    if(course_id>=MAXIDNUM || course_id<=0 || new_grade<MINGRADE || new_grade>MAXGRADE){
    	 return COURSE_MANAGER_INVALID_PARAMETERS;
    }
    return COURSE_MANAGER_OK;
}
//-----------------------------------------------------------------------------------------------
COURSE_MANEGERResult CourseMangerRemoveGrade(CourseManager manager,int semester_id,int course_id){
    assert(manager!=NULL);
    if (manager->connected_id==NOT_CONNECTED){
        return COURSE_MANAGER_NOT_LOGGED_IN;
    }
    SetElement student=setGetFirst(manager->students);
    while(student) {
        if (StudentGetId((Student) student) == manager->connected_id) {
            GRADE_SHEETResult res=GradeSheetRemoveCourseFromSemester(StudentGetGradeSheet((Student)student)
                    ,course_id,semester_id);
            if (res==GRADE_SHEET_IS_EMPTY){
                return COURSE_MANAGER_COURSE_DOES_NOT_EXIST;
            }
            else if(res==GRADE_SHEET_INVALID_PARAMTER){
                return COURSE_MANAGER_COURSE_DOES_NOT_EXIST;
            }
            else if(res==GRADE_SHEET_SEMESTER_DOES_NOT_EXIST){
                return COURSE_MANAGER_COURSE_DOES_NOT_EXIST;
            }
            else if(res==GRADE_SHEET_SEMESTER_COURSE_DOESNT_EXIST){
                return COURSE_MANAGER_COURSE_DOES_NOT_EXIST;
            }
            else if(res==GRADE_SHEET_MEMORY_ERROR){
                return COURSE_MANAGER_MEMORY_ERROR;
            }
            else if(res==GRADE_SHEET_SEMESTER_DOES_NOT_EXIST){
            	return COURSE_MANAGER_COURSE_DOES_NOT_EXIST;
            }
            GradeSheetRemoveEmptySemester(StudentGetGradeSheet((Student)student));
            break;
        }
        student=setGetNext(manager->students);
    }
    return COURSE_MANAGER_OK;
}
//-----------------------------------------------------------------------------------------------
COURSE_MANEGERResult CourseMangerReportFull(CourseManager manager, FILE* output_channel){
    int effective_points_x2=0, effective_grade_sum_x2=0;
    int total_points=0;
    assert(manager!=NULL);
    if (manager->connected_id==NOT_CONNECTED){
        return COURSE_MANAGER_NOT_LOGGED_IN;
    }
    Student student=CourseManagerGetStudentViaId(manager,manager->connected_id);
    mtmPrintStudentInfo(output_channel,StudentGetId((Student)student),
    		StudentGetFirstName((Student)student), StudentGetLastName((Student)student));
    GradeSheet sheet=StudentGetGradeSheet((Student)student);
    List semesters=GradeSheetGetList((GradeSheet)sheet);
    ListElement semester=listGetFirst(semesters);
    while(semester){
          ListElement course=listGetFirst(SemesterGetList((Semester)semester));
          while(course){
               //--------------this part is for mtmPrintGradeInfo-----------------
               mtmPrintGradeInfo(output_channel, courseGetId((Course)course),
            		   pointsX2(courseGetPoints((Course)course)),courseGetGrade((Course)course));
               course=listGetNext(SemesterGetList((Semester)semester));
         }
         effective_grade_sum_x2=SemesterEffectiveGradesAndPointsX2((Semester)semester,
        		                                                    &effective_points_x2);
         //-----------this part is for mtmPrintSemesterInfo---------------------
         mtmPrintSemesterInfo(output_channel,SemesterGetId((Semester)semester),
         SemesterTotalPointsX2((Semester)semester), SemesterFailPointsX2((Semester)semester),
		                                             effective_points_x2,effective_grade_sum_x2);
         semester=listGetNext(semesters);
   }
   //--------------this part is for mtmPrintSummary---------------------------
    if(GradeSheetGetSize(sheet)==0){
    	mtmPrintSummary(output_channel,0,0,0,0);
       return COURSE_MANAGER_OK;
    }
   int total_grades_x2= GradeSheetEffectivePointsAndGradesX2((GradeSheet)sheet,&total_points);
   mtmPrintSummary(output_channel,GradeSheetTotalPointsX2((GradeSheet)sheet),
                         GradeSheetFailedPointsX2((GradeSheet)sheet),total_points, total_grades_x2);
   return COURSE_MANAGER_OK;
}
//-----------------------------------------------------------------------------------------------
COURSE_MANEGERResult CourseMangerReportClean(CourseManager manager, FILE* output_channel){
    assert(manager!=NULL);
    int total_points=0,key=MINGRADE;
    if (manager->connected_id==NOT_CONNECTED){
        return COURSE_MANAGER_NOT_LOGGED_IN;
    }
    COURSE_MANEGERResult res=CourseMangerEffectiveGrades(manager);
    if(res==COURSE_MANAGER_MEMORY_ERROR){
        return COURSE_MANAGER_MEMORY_ERROR;
    }
    List new=listCopy(manager->grades);
    listClear(manager->grades);
    listSort(new,compareClean,&key);
    Student student=CourseManagerGetStudentViaId(manager,manager->connected_id);
    //--------here is the part for printing the student info-------
    mtmPrintStudentInfo(output_channel, StudentGetId(student),
                       StudentGetFirstName(student), StudentGetLastName(student));
    ListElement grade=listGetFirst(new);
    while(grade){
         mtmPrintGradeInfo(output_channel,((Grade)grade)->course_id, ((Grade)grade)->points,
                                                                                      ((Grade)grade)->grade);
         grade=listGetNext(new);
   }
   GradeSheet sheet=StudentGetGradeSheet(student);
   //-----------mtm Summary print part--------
   if(GradeSheetGetSize(sheet)==0){
	   mtmPrintCleanSummary(output_channel,0,0);
	   listDestroy(new);
      return COURSE_MANAGER_OK;
   }
   int effective_grade_sum_x2= GradeSheetEffectivePointsAndGradesX2((GradeSheet)sheet,&total_points);
   mtmPrintCleanSummary(output_channel,total_points, effective_grade_sum_x2);
   listDestroy(new);
   return COURSE_MANAGER_OK;
}
//-----------------------------------------------------------------------------------------------
COURSE_MANEGERResult CourseMangerReportBest(CourseManager manager, int amount, FILE* output_channel){
    assert(manager!=NULL);
    int key=MAXGRADE;
    if (manager->connected_id==NOT_CONNECTED){
        return COURSE_MANAGER_NOT_LOGGED_IN;
    }
    if (amount<=0){
        return COURSE_MANAGER_INVALID_PARAMETERS;
    }
    Student student=CourseManagerGetStudentViaId(manager,manager->connected_id);
    GradeSheet sheet=StudentGetGradeSheet(student);
    if(GradeSheetGetSize(sheet)==0){
        return COURSE_MANAGER_OK;
    }
    COURSE_MANEGERResult res =CourseMangerEffectiveGrades(manager);
    if(res==COURSE_MANAGER_MEMORY_ERROR){
        return COURSE_MANAGER_MEMORY_ERROR;
    }
    List new=listCopy(manager->grades);
    listClear(manager->grades);
    listSort(new,compareGrades,&key);
    ListElement grade=listGetFirst(new);
    while(grade){
        if(amount==0){
            break;
        }
        mtmPrintGradeInfo(output_channel,((Grade)grade)->course_id, ((Grade)grade)->points,
                                                                                   ((Grade)grade)->grade);
        amount--;
        grade= listGetNext(new);
    }
    listDestroy(new);
    return COURSE_MANAGER_OK;
}
//-----------------------------------------------------------------------------------------------
COURSE_MANEGERResult CourseMangerReportWorst(CourseManager manager, int amount, FILE* output_channel){
    assert(manager!=NULL);
    int key=MINGRADE;
    if (manager->connected_id==NOT_CONNECTED){
        return COURSE_MANAGER_NOT_LOGGED_IN;
    }
    if (amount<=0){
        return COURSE_MANAGER_INVALID_PARAMETERS;
    }
    Student student=CourseManagerGetStudentViaId(manager,manager->connected_id);
    GradeSheet sheet=StudentGetGradeSheet(student);
    if(GradeSheetGetSize(sheet)==0){
       return COURSE_MANAGER_OK;
    }
    COURSE_MANEGERResult res=CourseMangerEffectiveGrades(manager);
    if(res==COURSE_MANAGER_MEMORY_ERROR){
        return COURSE_MANAGER_MEMORY_ERROR;
    }
    List new=listCopy(manager->grades);
    listClear(manager->grades);
    listSort(new,compareGrades,&key);
    ListElement grade= listGetFirst(new);
    while(grade){
        if(amount==0){
            break;
        }
        mtmPrintGradeInfo(output_channel,((Grade)grade)->course_id,((Grade)grade)->points,
                          ((Grade)grade)->grade);
        amount--;
        grade= listGetNext(new);
    }
    listDestroy(new);
    return COURSE_MANAGER_OK;
}
//-----------------------------------------------------------------------------------------------
COURSE_MANEGERResult CourseMangerReportReference(CourseManager manager,int course_id, int amount, FILE* output_channel){
    assert(manager!=NULL);
    int key=MAXGRADE;
    if (manager->connected_id==NOT_CONNECTED){
        return COURSE_MANAGER_NOT_LOGGED_IN;
    }
    if (amount<=0){
        return COURSE_MANAGER_INVALID_PARAMETERS;
    }
    if(course_id <=0 || course_id >=MAXIDNUM){
       return COURSE_MANAGER_OK;
    }
    COURSE_MANEGERResult res=findRelevantFriends(manager,course_id);
    if(res==COURSE_MANAGER_MEMORY_ERROR){
        return COURSE_MANAGER_MEMORY_ERROR;
    }
    List new=listCopy(manager->relevant_frineds);
    listClear(manager->relevant_frineds);
    listSort(new,compareGradesForRelFriends,&key);
    ListElement relevant_friend=listGetFirst(new);
    while(relevant_friend && amount>0){
        mtmPrintStudentName(output_channel,((RelevantFriend)relevant_friend)->Fname,
            ((RelevantFriend)relevant_friend)->Lname);
        amount--;
        relevant_friend=listGetNext(new);
    }
    listDestroy(new);
    return COURSE_MANAGER_OK;
}
//-----------------------------------------------------------------------------------------------
COURSE_MANEGERResult CourseMangerReportFacultyRequest(CourseManager manager,int course_id, char* request, FILE* output_channel){
    assert(manager!=NULL);
    int found=0;
    char* response="your request was rejected";
    if (manager->connected_id==NOT_CONNECTED){
        return COURSE_MANAGER_NOT_LOGGED_IN;
    }
    if (strcmp(request, "remove_course")==0){
        Student student=CourseManagerGetStudentViaId(manager,manager->connected_id);
        GradeSheet sheet=StudentGetGradeSheet(student);
        List semesters=GradeSheetGetList(sheet);
        ListElement semester1=listGetFirst(semesters);
        while(semester1){
              List courses=SemesterGetList(semester1);
              ListElement course1=listGetFirst(courses);
              while(course1){
                   if (courseGetId((Course)course1)==course_id){
                       found=1;
                       break;
                   }
                   course1=listGetNext(courses);
             }
             semester1=listGetNext(semesters);
       }
       if (found==0){
    	   return COURSE_MANAGER_COURSE_DOES_NOT_EXIST;
       }
       else{
       	  mtmFacultyResponse(output_channel, response);
       	  return COURSE_MANAGER_OK;
       }
    }
    if (strcmp(request, "remove_course")!=0 && strcmp(request, "register_course")!=0 && strcmp(request, "cancel_course")!=0){
        return COURSE_MANAGER_INVALID_PARAMETERS;
    }
    else if((strcmp(request, "cancel_course")==0) ||(strcmp(request, "register_course")==0)){
        mtmFacultyResponse(output_channel, response);
    }
    return COURSE_MANAGER_OK;
}
//----------------------------------------------------------------------------------------------------------------
int CourseManagerGetConnectedId(CourseManager manager){
    assert(manager!=NULL);
    return manager->connected_id;
}
//----------------------------------------------------------------------------------------------------------------
int CourseManagerGetStudentsSize(CourseManager manager){
    assert(manager!=NULL);
    return manager->students_size;
}
//----------------------------------------------------------------------------------------------------------------
List CourseManagerGetGradesList(CourseManager manager){
    assert(manager!=NULL);
    return manager->grades;
}
//----------------------------------------------------------------------------------------------------------------
Set CourseManagerGetStudents(CourseManager manager){
    assert(manager!=NULL);
    return manager->students;
}
//----------------------------------------------------------------------------------------------------------------
List CourseManagerGetRelevanFriendsList(CourseManager manager){
    assert(manager!=NULL);
    return manager->relevant_frineds;
}
//---------------------------------------------------------------------------------------------------------------------
void CourseManagerDestroy(CourseManager manager){
	setDestroy(manager->students);
	listDestroy(manager->grades);
	listDestroy(manager->relevant_frineds);
	free(manager);
}
