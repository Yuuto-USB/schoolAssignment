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
    printf("File opening error");
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

  printf("This is the Todo List program.\n");

  while (select != 9) {
    printf("---------------------- [Task : %d] ------------------- \n", counts);
    printf("1. Add Task | 2. Remove Task | 3. Print Task | 4. Modify Task | "
           "5. Change Task Status | 6. Sync Dates | 7. Importance Print | 8. Clear Log | 9. Exit\n");
    scanf("%d", &select);
    getchar();
    switch (select) {
    case 1:
      addTask(counts);
      counts++;
      break;
    case 2:
      counts = removeTask(counts);
      break;
    case 3:
      printTask(counts);
      break;
    case 4:
      modifyTask(counts);
      break;
    case 5:
      achieveTask(counts);
      break;
    case 6:
      initDday(counts);
      printf("The D-days have been synchronized.\n");
      break;
    case 7:
    	printImportance(counts);
    	break;
    case 8:
    	for(i=0; i<100; i++){
    		printf("\n");
		}
		break;
    }
  }

  fp = fopen("todoList.csv", "w");
  if (fp == NULL) {
    printf("File opening error");
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
  printf("The program has ended.");
  fclose(fp);
  return 0;
}
