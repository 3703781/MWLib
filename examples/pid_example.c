/**
 * @file    pid_example.c
 * @author  Miaow
 * @date    2019/09/03
 */
 
#include "string.h"
#include "stdarg.h"
#include "utils.h"
#include "ff.h"
#include "pid.h"

PID_InfoTypeDef PidInfo;

int f_myprintf(FIL* fp, const char* format, ...)
{
  va_list aptr;
  char buffer[255];
  uint32_t bw;

  va_start(aptr, format);
  vsprintf(buffer, format, aptr);
  va_end(aptr);
  f_write(fp, buffer, strlen(buffer), &bw);

  return bw;
}


int main()
{
  uint32_t i = 0;
  uint32_t j = 0;
  char buffer[255];
  FIL outputDataFile;
  FATFS fileSystem;
  UTILS_InitDelay();
  UTILS_InitUart(115200);

  f_mount(&fileSystem, "0", 1);
  
  printf("Start simulating!\r\n");
  for (j = 0; j < 10; j++)
  {
    sprintf(buffer, "0:/data%f.csv", (float)j / 20.0f);
    f_open(&outputDataFile, buffer, FA_OPEN_ALWAYS | FA_WRITE);
    f_printf(&outputDataFile, "count,target,error,output\r\n");
    PID_Init(&PidInfo, 0.2f, 0.1f, 0.2f);
    PidInfo.kp = (float)j / 20.0f;
    for (i = 0; i < 400; i++)
    {
      PID_Realize(&PidInfo, 200.0f, PidInfo.outputValue);
      f_myprintf(&outputDataFile, "%d,%f,%f,%f\r\n", i, 200.0f, PidInfo.error, PidInfo.outputValue);
    }
    f_close(&outputDataFile);
  }
  printf("End\r\n");

  f_unmount("0");

  while (1);
}
