#ifndef _LOG_H_
#define _LOG_H_
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<errno.h>

void writelog(const char *fileName, const char *log)
{
        time_t tDate;
        struct tm *eventTime;
        time(&tDate);
        eventTime = localtime(&tDate);
        int iYear = eventTime->tm_year;
        int iMon = eventTime->tm_mon + 1;
        int iDay = eventTime->tm_mday;
        int iHour = eventTime->tm_hour;
        int iMin = eventTime->tm_min;
        int iSec = eventTime->tm_sec;
        printf("tm_isdst = %d\n", eventTime->tm_isdst);
        char sDate[16];
        sprintf(sDate,"%04d-%02d-%02d", iYear, iMon, iDay);
        char sTime[16];
        sprintf(sTime,"%02d:%02d:%02d", iHour, iMin, iSec);
        char s[1024];
        sprintf(s,"%s %s %s\n", sDate, sTime, log);


        FILE *p = fopen(fileName, "a+");
        if(p == NULL)
        {
                printf("write log %s error: %s\n",fileName, strerror(errno));
        }
        else
        {
                fputs(s, p);
                fclose(p);
        }
        return;

}

#endif/*_LOG_H_*/
