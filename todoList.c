#include "taskManage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LENGTH 300

int main() {
  FILE *fp;
  char line[MAX_LENGTH];
  int counts = 0, i, select = 1, printSelect;

  fp = fopen("todoList.csv", "r");
  if (fp == NULL) {
    printf("파일 열기 오류");
    fclose(fp);
    return 1;
  }

  while (fgets(line, sizeof(line), fp) != NULL && counts < MAX_TASKS) {

    line[strcspn(line, "\n")] = '\0';

    if (sscanf(line, "%[^,],%d,%d,%d,%ld,%[^,],%[^,]",
               todoLists[counts].taskName, &todoLists[counts].targetYear,
               &todoLists[counts].targetMonth, &todoLists[counts].targetDay,
               &todoLists[counts].d_Day, todoLists[counts].importance,
               todoLists[counts].taskState) == 7) {
      counts++;
    }
  }
  fclose(fp);
  if (counts != 0) {
    initDday(counts);
  }

  printf("Todo리스트 프로그램입니다.\n");

  while (select != 7) {
    printf("-------------------------------------------\n");
    printf("1번 목표 입력 | 2번 목표 삭제 | 3번 목표 출력 | 4번 목표 수정 | "
           "5번 목표 상태 "
           "바꾸기 | 6번 날짜 동기화 | 7번 종료\n");
    scanf("%d", &select);
    getchar();
    switch (select) {
    case 1:
      addTask(counts++);
      break;
    case 2:
      counts = removeTask(counts);
      break;
    case 3:
      printTask(counts++);
      break;
    case 4:
      modifyTask(counts);
      break;
    case 5:
      achieveTask(counts);
      break;
    case 6:
      initDday(counts);
      printf("디데이가 동기화되었습니다.\n");
      break;
    }
  }

  fp = fopen("todoList.csv", "w");
  if (fp == NULL) {
    printf("파일 열기 오류");
    fclose(fp);
    return 1;
  }
  for (i = 0; i < counts; i++) {
    if (strcmp(todoLists[i].taskName, "") != 0) {
      fprintf(fp, "%s,%d,%d,%d,%ld,%s,%s\n", todoLists[i].taskName,
              todoLists[i].targetYear, todoLists[i].targetMonth,
              todoLists[i].targetDay, todoLists[i].d_Day,
              todoLists[i].importance, todoLists[i].taskState);
    }
  }
  printf("-------------------------------------------\n");
  printf("프로그램이 종료되었습니다.");
  fclose(fp);
  return 0;
}
