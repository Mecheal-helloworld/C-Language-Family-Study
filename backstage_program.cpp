#include <iostream>
#include <time.h>
#include <windows.h>
#include<pthread.h>
#include "Windows.h"

#define SERVICE_NAME_1 "HealthReport"
#define BUFLEN 255

static volatile int g_run = 1;
SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hServiceStatusHandle;
FILE* log;
char tmpBuf[BUFLEN];

void* thr_routine( void* arg )
{
	int sign;

	sign = *( int* )arg;

	while ( g_run )
	{
		time_t now_time=time(NULL);
		tm*  now_tm = localtime( &now_time );
    	int now_hour = now_tm -> tm_hour;
    	log = fopen("E:\\Users\\ASUS\\AppData\\Local\\backstage\\log.txt", "a+");
    	fprintf(log, "point: now hour is %d\n",now_hour);
    	fclose(log);
    	
    	if( sign && (now_hour==7||now_hour==8||now_hour==12||now_hour==13||now_hour==18||now_hour==19))
		{
    		system("python E:\\Users\\ASUS\\AppData\\Local\\backstage\\python\\health_up.py");
    		log = fopen("E:\\Users\\ASUS\\AppData\\Local\\backstage\\log.txt", "a+");
    		strftime(tmpBuf, BUFLEN, "%Y-%m-%d %H:%M:%S",now_tm);
    		fprintf(log, "do: run health_up.py at %s\n",tmpBuf);
    		fclose(log);
    		sign = 0;
		}
		else if(now_hour!=7&&now_hour!=8&&now_hour!=12&&now_hour!=13&&now_hour!=18&&now_hour!=19)sign = 1;
		
    	Sleep(6000000);
	}

	return NULL;
}

void WINAPI HealthReportServiceHandler(DWORD fdwControl)
{ 
    //获取本地时间  
    time_t now_time=time(NULL);
	tm* now_tm = localtime(&now_time);
    switch(fdwControl) 
    {
        case SERVICE_CONTROL_STOP:
        case SERVICE_CONTROL_SHUTDOWN:
        ServiceStatus.dwWin32ExitCode = 0; 
        ServiceStatus.dwCurrentState  = SERVICE_STOPPED; 
        ServiceStatus.dwCheckPoint    = 0; 
        ServiceStatus.dwWaitHint      = 0;

		//add you quit code here
		g_run = 0;
		log = fopen("E:\\Users\\ASUS\\AppData\\Local\\backstage\\log.txt", "a+");
		strftime(tmpBuf, BUFLEN, "%Y-%m-%d %H:%M:%S",now_tm);
        fprintf(log, "stop: this server stop at %s\n",tmpBuf);
		fclose(log);

        break; 
        default:
            return; 
    };
    if (!SetServiceStatus(hServiceStatusHandle,  &ServiceStatus)) 
    { 
        DWORD nError = GetLastError();
    } 
}

void WINAPI service_HealthReport(int argc, char** argv) 
{       
    ServiceStatus.dwServiceType        = SERVICE_WIN32; 
    ServiceStatus.dwCurrentState       = SERVICE_START_PENDING; 
    ServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE; 
    ServiceStatus.dwWin32ExitCode      = 0; 
    ServiceStatus.dwServiceSpecificExitCode = 0; 
    ServiceStatus.dwCheckPoint         = 0; 
    ServiceStatus.dwWaitHint           = 0;  
    hServiceStatusHandle = RegisterServiceCtrlHandler(SERVICE_NAME_1, HealthReportServiceHandler); 
    if (hServiceStatusHandle==0) 
    {
        DWORD nError = GetLastError();
    }  
    //add your init code here
    g_run = 1;

    int sign = 1;
    
    time_t now_time=time(NULL);
    tm* now_tm = localtime( &now_time );
    log = fopen("E:\\Users\\ASUS\\AppData\\Local\\backstage\\log.txt", "a+");
    strftime(tmpBuf, BUFLEN, "%Y-%m-%d %H:%M:%S",now_tm);
    fprintf(log, "start: this server start at %s\n",tmpBuf);
    fclose(log);
    //add your service thread here
    
    if ( pthread_create( 0, NULL, thr_routine, &sign ) != 0 )
	{
		time_t error_time=time(NULL);
		tm* error_tm = localtime(&error_time);
		log = fopen("E:\\Users\\ASUS\\AppData\\Local\\backstage\\log.txt", "a+");
		strftime(tmpBuf, BUFLEN, "%Y-%m-%d %H:%M:%S",error_tm);
		fprintf(log, "error: thread create error at %s\n",tmpBuf);
		fclose(log);
		DWORD nError = GetLastError();
	}

    // Initialization complete - report running status 
    ServiceStatus.dwCurrentState       = SERVICE_RUNNING; 
    ServiceStatus.dwCheckPoint         = 0; 
    ServiceStatus.dwWaitHint           = 9000;  
    if(!SetServiceStatus(hServiceStatusHandle, &ServiceStatus)) 
    { 
        DWORD nError = GetLastError();
    } 

}

int main (int argc, const char *argv[])
{
    SERVICE_TABLE_ENTRY ServiceTable[2];

    ServiceTable[0].lpServiceName = SERVICE_NAME_1;
    ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)service_HealthReport;

    ServiceTable[1].lpServiceName = NULL;
    ServiceTable[1].lpServiceProc = NULL;
    // 启动服务的控制分派机线程
    StartServiceCtrlDispatcher(ServiceTable); 
    return 0;
}

