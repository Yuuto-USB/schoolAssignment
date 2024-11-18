#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_TASKS 300 // Maximum number of tasks

typedef struct {
  char taskName[50];   // Task name
  int targetYear;      // Target year
  int targetMonth;     // Target month
  int targetDay;       // Target day
  long int d_Day;      // D-day value
  char importance[10]; // Priority
  char taskState[10];  // Task state
  char test[50];
} todoList;

todoList todoLists[MAX_TASKS]; // Array of task list
void printTaskForm(int i) {
  if (todoLists[i].d_Day > 0) {
    printf("| [%s] %s [%d.%d.%1d] D-%ld #%s\n", todoLists[i].taskState,
           todoLists[i].taskName, todoLists[i].targetYear,
           todoLists[i].targetMonth, todoLists[i].targetDay, todoLists[i].d_Day,
           todoLists[i].importance);
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
}

int isFullDay(int month, int day){
	if(day==31){
		if(month == 2 || month == 4 || month == 6 || month == 9 || month == 11){
			return 1;
		}
		else{
			return 0;
		}
	}
}

// Leap year check function
// Function to check if the given year is a leap year
// Condition: If divisible by 4 and not divisible by 100, or divisible by 400, it is a leap year
int isLeapYear(int year) {
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int isLeap(int year, int month, int day){
	if(month == 2 && day == 29){
		if(isLeapYear(year)){
			return 0;
		}
		else{
			return 1;
		}
	}
	else{
		return 0;
	}
}

// Function to calculate total days
// Calculates the total number of days up to a given date (y, m, d)
// In the case of a leap year, February is set to 29 days
long totalday(int y, int m, int d) {
  int months[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  long total = 0L;
  int i;

  // If it's a leap year, set February to 29 days
  if (isLeapYear(y)) {
    months[1] = 29;
  }

  // Calculate total days for the years
  total = (y - 1) * 365L + (y - 1) / 4 - (y - 1) / 100 + (y - 1) / 400;

  // Add the days for the months
  for (i = 0; i < m - 1; i++) {
    total += months[i];
  }

  // Add the days for the given day
  total += d;
  return total;
}

// D-day calculation function
// Calculates the difference between the current date and target date to return the D-day value
long D_day_count(int counts, int year, int month, int day) {
  long today_days = totalday(year, month, day); // Total days for the current date
  long target_days =
      totalday(todoLists[counts].targetYear, todoLists[counts].targetMonth,
               todoLists[counts].targetDay); // Total days for the target date
  long dates = target_days - today_days;     // Calculate the D-day value
  return dates;
}

// Add task function
// Takes the task name, target date, and priority and adds a new task
void addTask(int counts) {
  // Automatically set the current date
  time_t t = time(NULL);
  struct tm *today = localtime(&t);
  int year = today->tm_year + 1900;
  int month = today->tm_mon + 1;
  int day = today->tm_mday;
  int i;

// Task input
inputTask:
  printf("-------------------------------------------\n");
  printf("Please enter the task [50 characters max] : ");
  fgets(todoLists[counts].taskName, 50, stdin);
  todoLists[counts].taskName[strcspn(todoLists[counts].taskName, "\n")] =
      '\0'; // Remove newline

  // Check if there is a duplicate name
  if (counts != 0) {
    for (i = 0; i < counts; i++) {
      if (strcmp(todoLists[counts].taskName, todoLists[i].taskName) == 0) {
        printf("Duplicate task name.\n");
        goto inputTask;
      }
    }
  }

  // Enter target date
  char dateInput[20];
  printf("Please enter the target date [YYYY.MM.DD] : ");
  fgets(dateInput, sizeof(dateInput), stdin);

  // Validate date input and set the target date
  while (sscanf(dateInput, "%d.%d.%d", &todoLists[counts].targetYear,
                &todoLists[counts].targetMonth,
                &todoLists[counts].targetDay) != 3 ||
         todoLists[counts].targetMonth > 12 ||
         todoLists[counts].targetDay > 31 ||
         todoLists[counts].targetMonth < 1 || todoLists[counts].targetDay < 1 || isFullDay(todoLists[counts].targetMonth, todoLists[counts].targetDay) == 1 || isLeap(todoLists[counts].targetYear, todoLists[counts].targetMonth, todoLists[counts].targetDay) == 1) {
    printf("Invalid date. Please enter again.\n");
    fgets(dateInput, sizeof(dateInput), stdin);
  }

  // Enter priority
  printf("Please enter the priority [High, Middle, Low] : ");
  scanf("%s", todoLists[counts].importance);

  // Validate priority input
  while (strcmp(todoLists[counts].importance, "High") != 0 &&
         strcmp(todoLists[counts].importance, "Middle") != 0 &&
         strcmp(todoLists[counts].importance, "Low") != 0) {
    printf("Invalid priority. Please enter again.\n");
    scanf("%s", todoLists[counts].importance);
  }
  //Set the task's status to incomplete
  strcpy(todoLists[counts].taskState, "Incomplete");


  // Calculate D-day
  todoLists[counts].d_Day = D_day_count(counts, year, month, day);
}

// Remove task function
// Takes the name of the task and removes it
int removeTask(int counts) {
  if (counts == 0) {
    printf("-------------------------------------------\n");
    printf("| No tasks available.\n");
    return counts;
  }

  int i, j;
  char searchTask[50];
  printf("-------------------------------------------\n");
  printf("Please enter the name of the task to delete : ");
  fgets(searchTask, sizeof(searchTask), stdin);
  searchTask[strcspn(searchTask, "\n")] = '\0';

  for (i = 0; i < counts; i++) {
    if (strcmp(searchTask, todoLists[i].taskName) == 0) {
      // Shift the array forward to remove the task
      for (j = i; j < counts - 1; j++) {
        todoLists[j] = todoLists[j + 1];
      }
      counts--; // Decrease the task count
      printf("Task has been deleted.\n");
      return counts;
    }
  }

  printf("| Task not found..\n");
  return counts;
}

// Print task function
// Prints the current state and D-day of all tasks
int printTask(int counts) {
  if (counts <= 0) {
    printf("-------------------------------------------\n");
    printf("| No tasks available.\n");
    return 0;
  }
  int i;
  printf("-------------------------------------------\n");
  for (i = 0; i < counts; i++) {
    if (strcmp(todoLists[i].taskName, "") != 0) {
      printTaskForm(i);
    }
  }
  return 0;
}

// Modify task function
// Allows modification of a specific task by name using the addTask function
int modifyTask(int counts) {
  if (counts == 0) {
    printf("-------------------------------------------\n");
    printf("| No tasks available.\n");
    return 0;
  }
  int i;
  printf("-------------------------------------------\n");
  printf("| Please enter the name of the task to modify : ");
  char searchTask[50];
  fgets(searchTask, 50, stdin);
  searchTask[strcspn(searchTask, "\n")] = '\0';
  for (i = 0; i < counts; i++) {
    if (strcmp(searchTask, todoLists[i].taskName) == 0) {
      addTask(i); // Call addTask to modify the task
      return 0;
    }
  }
  printf("| Task not found.\n");
  return 0;
}

// Achieve task function
// Marks a task as complete by finding it by name
int achieveTask(int counts) {
  if (counts == 0) {
    printf("-------------------------------------------\n");
    printf("| No tasks available.\n");
    return 0;
  }
  int i;
  printf("-------------------------------------------\n");
  printf("Please enter the name of the task to mark as complete : ");
  char searchTask[50];
  fgets(searchTask, 50, stdin);
  searchTask[strcspn(searchTask, "\n")] = '\0';
  for (i = 0; i < counts; i++) {
    if (strcmp(searchTask, todoLists[i].taskName) == 0) {
      if (strcmp(todoLists[i].taskState, "Complete") == 0) {
        printf("This task is already completed.\n");
        return 0;
      }
      strcpy(todoLists[i].taskState, "Complete"); // Set the state to 'Complete'
      printf("Task has been completed.\n");
      return 0;
    }
  }
  printf("Task not found.\n");
  return 0;
}

// Sync task dates function
// Updates the D-day for all tasks based on the current date
int initDday(int counts) {
  if (counts == 0) {
    printf("-------------------------------------------\n");
    printf("| No tasks available.\n");
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
    todoLists[i].d_Day =
        D_day_count(i, year, month, day); // Call D-day calculation function
  }
  return 0;
}

// 7. printImportance
int printImportance(int counts) {
  if (counts == 0) {
    printf("-------------------------------------------\n");
    printf("| No tasks available.\n");
    return 0;
  }
  int i;
  printf("-------------------------------------------\n");
  printf("     [ Importance : High ]\n");
  // High importance task print
  for (i = 0; i < counts; i++) {
    if (strcmp(todoLists[i].taskName, "") != 0 && strcmp(todoLists[i].importance, "High") == 0) {
      printTaskForm(i);
    }
  }

  printf("\n     [ Importance : Middle ]\n");
  // Middle importance task print
  for (i = 0; i < counts; i++) {
    if (strcmp(todoLists[i].taskName, "") != 0 && strcmp(todoLists[i].importance, "Middle") == 0) {
      printTaskForm(i);
    }
  }

  printf("\n     [ Importance : Low ]\n");
  // Low importance task print
  for (i = 0; i < counts; i++) {
    if (strcmp(todoLists[i].taskName, "") != 0 && strcmp(todoLists[i].importance, "Low") == 0) {
      printTaskForm(i);
    }
  }
  return 0;
}
