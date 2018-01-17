#include <Arduino_FreeRTOS.h>

static void led_task(void* pvParameters);

task_info list_of_tasks[10];

void setup() 
{
  Serial.begin(115200);  
  
  byte num_tasks = setNewTasks(list_of_tasks);
  
  printTasksInfo(list_of_tasks, num_tasks);
}

void loop()
{
  PORTB = 0;
  PORTD = 0;
}

static void led_task(void* pvParameters)
{
  while(1)
  {
    if((byte)pvParameters < 8)
    {
      PORTD = 1 << (byte)pvParameters;
      PORTB = 0;
    }
    else  
    {
      PORTB = 1 << (byte)pvParameters - 8;
      PORTD = 0;
    }
  }
}

byte setNewTasks(task_info* list_of_tasks)
{
  printInfoInput("Set new collection of tasks");
  byte num_tasks = Serial.parseInt();
  
  byte counter = 0;
  while(counter < num_tasks)
  {
    Serial.println(F("Add new task"));
    if(addNewTask(&(list_of_tasks[counter])))
      xPeriodicTaskCreate(&(list_of_tasks[counter++]));
    else  
    {
      Serial.print(F("Task with id: "));
      Serial.print(counter);
      Serial.println(F(" cannot be added due to task missconfiguration"));
      Serial.println(F("Try again"));
    }
  }
  return num_tasks;
}

bool addNewTask(task_info* c_task)
{
  printInfoInput("Add start time of task");
  c_task->time_period = Serial.parseInt();
  c_task->tick_period = c_task->time_period;
  printInfoInput("Add time length of task");
  c_task->time_length = Serial.parseInt();
  printInfoInput("Add led pin");
  c_task->task_param = Serial.parseInt();
  pinMode((byte)c_task->task_param, OUTPUT);
  c_task->stack_depth = 42;
  c_task->task_function = led_task;
  return true;
}

void printInfoInput(char* text)
{
  Serial.println(text);
  while(!Serial.available());
}
