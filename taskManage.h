#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_TASKS 300 // 최대 할 일 개수

typedef struct {
  char taskName[50]; // 할 일 이름
  int targetYear;    // 목표 연도
  int targetMonth;   // 목표 월
  int targetDay;     // 목표 일
  long int d_Day;    // D-day 값
  char importance[10]; // 우선순위
  char taskState[10];  // 할 일 상태
} todoList;

todoList todoLists[MAX_TASKS]; // 할 일 목록 배열 선언

// 윤년 확인 함수
// 주어진 연도가 윤년인지 확인하는 함수입니다.
// 조건: 4의 배수이고 100의 배수가 아니거나, 400의 배수이면 윤년입니다.
int isLeapYear(int year) {
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// 전체 일수 계산 함수
// 주어진 날짜(y, m, d)까지의 총 일수를 계산합니다.
// 윤년인 경우 2월은 29일로 설정하여 계산합니다.
long totalday(int y, int m, int d) {
  int months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  long total = 0L;
  int i;

  // 윤년이면 2월을 29일로 설정
  if (isLeapYear(y)) {
    months[1] = 29;
  }

  // 연 단위 총 일수 계산
  total = (y - 1) * 365L + (y - 1) / 4 - (y - 1) / 100 + (y - 1) / 400;

  // 월 단위 일수 추가
  for (i = 0; i < m - 1; i++) {
    total += months[i];
  }

  // 일 단위 추가
  total += d;
  return total;
}

// D-day 계산 함수 by 한상규
// 현재 날짜와 목표 날짜의 차이를 계산하여 D-day 값을 반환합니다.
long D_day_count(int counts, int year, int month, int day) {
  long today_days = totalday(year, month, day); // 현재 날짜의 총 일수
  long target_days =
      totalday(todoLists[counts].targetYear, todoLists[counts].targetMonth,
               todoLists[counts].targetDay); // 목표 날짜의 총 일수
  long dates = target_days - today_days; // D-day 값 계산
  return dates;
}

// 1번 할 일 추가 함수
// 할 일 이름, 목표 날짜, 우선순위를 입력받고 할 일을 추가합니다.
void addTask(int counts) {
  // 현재 날짜 자동 설정
  time_t t = time(NULL);
  struct tm *today = localtime(&t);
  int year = today->tm_year + 1900;
  int month = today->tm_mon + 1;
  int day = today->tm_mday;
  int i;

  // task의 달성 상태를 미완료로 설정
  strcpy(todoLists[counts].taskState, "Incomplete");

  // 할 일 입력
  inputTask:
    printf("-------------------------------------------\n");
    printf("할 일을 입력해주세요[50자 내] : ");
    fgets(todoLists[counts].taskName, 50, stdin);
    todoLists[counts].taskName[strcspn(todoLists[counts].taskName, "\n")] =
        '\0'; // 개행 제거

  // 중복된 이름이 있는지 검사
  if(counts != 0){
    for(i = 0; i < counts; i++){
      if(strcmp(todoLists[counts].taskName, todoLists[i].taskName) == 0){
        printf("중복된 목표 이름입니다.\n");
        goto inputTask;
      }
    }
  }

  // 목표 날짜 입력
  char dateInput[20];
  printf("목표 날짜를 적어주세요[YYYY.MM.DD] : ");
  fgets(dateInput, sizeof(dateInput), stdin);

  // 날짜 유효성 검사 및 목표 날짜 설정
  while (sscanf(dateInput, "%d.%d.%d", &todoLists[counts].targetYear,
                &todoLists[counts].targetMonth, &todoLists[counts].targetDay) != 3 ||
         todoLists[counts].targetMonth > 12 ||
         todoLists[counts].targetDay > 31 ||
         todoLists[counts].targetMonth < 1 || todoLists[counts].targetDay < 1) {
      printf("잘못된 날짜입니다. 다시 입력해주세요\n");
      fgets(dateInput, sizeof(dateInput), stdin);
  }

  // 우선순위 입력
  printf("우선순위를 적어주세요[High, Middle, Low] : ");
  scanf("%s", todoLists[counts].importance);

  // 우선순위 유효성 확인
  while (strcmp(todoLists[counts].importance, "High") != 0 &&
         strcmp(todoLists[counts].importance, "Middle") != 0 &&
         strcmp(todoLists[counts].importance, "Low") != 0) {
    printf("잘못된 우선순위입니다. 다시 입력해주세요\n");
    scanf("%s", todoLists[counts].importance);
  }

  // D-day 계산
  todoLists[counts].d_Day = D_day_count(counts, year, month, day);
}

// 2번 할 일 제거 함수
// 할 일 목록에서 이름을 통해 특정 할 일을 제거합니다.
int removeTask(int counts){
  if (counts == 0) {
    printf("-------------------------------------------\n");
    printf("| 목표가 없습니다.\n");
    return 0;
  }
  int i;
  printf("-------------------------------------------\n");
  printf("삭제할 목표의 이름을 입력해주세요 : ");
  char searchTask[50];
  fgets(searchTask, 50, stdin);
  searchTask[strcspn(searchTask, "\n")] = '\0';
  for (i = 0; i < counts; i++) {
    if (strcmp(searchTask, todoLists[i].taskName) == 0) {
      strcpy(todoLists[i].taskName, ""); // 일치하는 이름의 할 일 제거
      return --counts;
    }
  }
  printf("| 목표를 찾을 수 없습니다.\n");
  return counts;
}

// 3번 할 일 출력 함수
// 현재 모든 할 일의 상태와 D-day 정보를 출력합니다.
int printTask(int counts) {
  if (counts == 0) {
    printf("-------------------------------------------\n");
    printf("| 목표가 없습니다.\n");
    return 0;
  }
  int i;
  printf("-------------------------------------------\n");
  for (i = 0; i < counts; i++) {
    if (strcmp(todoLists[i].taskName, "") != 0) {
      if (todoLists[i].d_Day > 0) {
        printf("| [%s] %s [%d.%d.%1d] D-%ld #%s\n", todoLists[i].taskState,
               todoLists[i].taskName, todoLists[i].targetYear,
               todoLists[i].targetMonth, todoLists[i].targetDay,
               todoLists[i].d_Day, todoLists[i].importance);
      } else if (todoLists[i].d_Day == 0) {
        printf("| [%s] %s [%d.%d.%1d] D-Day #%s\n", todoLists[i].taskState,
               todoLists[i].taskName, todoLists[i].targetYear,
               todoLists[i].targetMonth, todoLists[i].targetDay,
               todoLists[i].importance);
      } else if (todoLists[i].d_Day < 0) {
        printf("| [%s] %s [%d.%d.%1d] D+%ld #%s\n", todoLists[i].taskState,
               todoLists[i].taskName, todoLists[i].targetYear,
               todoLists[i].targetMonth, todoLists[i].targetDay,
               -todoLists[i].d_Day, todoLists[i].importance);
      }
    } else {
      break;
    }
  }
  return 0;
}

// 4번 할 일 수정 함수
// 이름을 통해 특정 할 일을 찾아 수정할 수 있도록 addTask 함수를 호출합니다.
int modifyTask(int counts) {
  if (counts == 0) {
    printf("-------------------------------------------\n");
    printf("| 목표가 없습니다.\n");
    return 0;
  }
  int i;
  printf("-------------------------------------------\n");
  printf("| 수정할 목표의 이름을 입력해주세요 : ");
  char searchTask[50];
  fgets(searchTask, 50, stdin);
  searchTask[strcspn(searchTask, "\n")] = '\0';
  for (i = 0; i < counts; i++) {
    if (strcmp(searchTask, todoLists[i].taskName) == 0) {
      addTask(i); // 해당 인덱스에서 addTask를 호출해 정보 수정
      return 0;
    }
  }
  printf("| 목표를 찾을 수 없습니다.\n");
  return 0;
}

// 5번 할 일 달성 체크 함수
// 이름을 통해 특정 할 일을 찾아서 완료 상태로 변경합니다.
int achieveTask(int counts) {
  if (counts == 0) {
    printf("-------------------------------------------\n");
    printf("| 목표가 없습니다.\n");
    return 0;
  }
  int i;
  printf("-------------------------------------------\n");
  printf("수정할 목표의 이름을 입력해주세요 : ");
  char searchTask[50];
  fgets(searchTask, 50, stdin);
  searchTask[strcspn(searchTask, "\n")] = '\0';
  for (i = 0; i < counts; i++) {
    if (strcmp(searchTask, todoLists[i].taskName) == 0) {
      if (strcmp(todoLists[i].taskState, "Complete") == 0) {
        printf("이미 완료한 목표입니다.\n");
        return 0;
      }
      strcpy(todoLists[i].taskState, "Complete"); // 상태를 'Complete'로 설정
      printf("목표가 완료되었습니다.\n");
      return 0;
    }
  }
  printf("목표를 찾을 수 없습니다.\n");
  return 0;
}

// 6번 할 일 날짜 동기화 함수
// 현재 날짜를 기준으로 모든 할 일의 D-day를 갱신합니다.
int initDday(int counts) {
  if (counts == 0) {
    printf("-------------------------------------------\n");
    printf("| 목표가 없습니다.\n");
    return 0;
  }
  int i;
  time_t t = time(NULL);
  struct tm *today = localtime(&t);
  int year, month, day;
  for (i = 0; i < counts; i++) {
    int year = today->tm_year + 1900;
    int month = today->tm_mon + 1;
    int day = today->tm_mday;
    todoLists[i].d_Day = D_day_count(i, year, month, day); // D-day 계산 함수 호출
  }
  return 0;
}
