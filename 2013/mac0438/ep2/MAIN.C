/*
Copyright (C) 1997-1999 Colin Percival.

This code, or parts of it, may be distributed and used freely for academic
purposes only, unless permission is obtained from the author.
*/

#include "Main.h"
#include "PiHex.h"
#include "Cpuid.h"

char AppClass[]=sAppClass;
char AppName[]=sAppName;
char IniFileName[256];

long _EXPORT FAR PASCAL WindowProc(HWND,UINT,WPARAM,LPARAM);

// Taken from prime.exe
static  char *  NEAR lines[NumLines] = {0};
static  char    NEAR linebuf[NumLines*LineSize];
static  int     NEAR charHeight = 0;
void FAR PASCAL OutputStr(HWND hWnd,LPSTR str);
void FAR PASCAL LineFeed (HWND hWnd);

HANDLE hCalcThread[MAXTHREADS];
DWORD CalcThreadId[MAXTHREADS];

int TRAY_ICON;
int NO_ICON;
int AUTO_CONT; //1 is yes, 0 is no
int service;
char UserName[128];
char Useremail[128];
long cpuspeed;
double daysworktoget;
#define cputype CPUTYPE
#define cpuvendor CPUVENDOR
long cpunum;
double hoursworkperday;
long connectmethod;
long computernum;
long lastcommunicated;

char proxyaddr[128];
long proxyport;

long autoconn;
long joined;
long PR_OPT;

long windowflashed;
long threads;

HWND mainHwnd;
long cont = FALSE;     //used to tell Calc_Thread to exit
//  cont = computing
long threadsactive = 0;
HANDLE AppInst;

long communicating = 0;
long wanttocommunicate = 0;

#ifdef times
FILE *timesfl;
#endif

long tscval[2];

float rdtsc(void);          //read the Pentium Time Stamp Counter
#pragma aux rdtsc =     \
        0x0F 0x31       \
        "mov tscval,eax"  \
        "mov tscval+4,edx"\
        "fild qword tscval"\
        modify [eax edx];

LARGE_INTEGER int64;

float fild(void);           //convert from int64 to float
#pragma aux fild = "fild qword int64";

struct thdat {
    long threadnum;
};

void tellserverhours(void)
{
    char cbuf[256];

    sprintf(cbuf,"change,%s,%s,%d,%d,%d,%d,%f,%f\ngetrtime\n",UserName,Useremail,cpuvendor,cputype,cpuspeed,cpunum,hoursworkperday,daysworktoget);
    spoolmsg(cbuf);
};

void updatehours(float cav)
{
    char cbuf[20];

    hoursworkperday=1.0/(0.99/hoursworkperday+0.01/cav);
    sprintf(cbuf,"%f",hoursworkperday);
    WritePrivateProfileString("Main","hoursperday",cbuf,IniFileName);
};

long cftblah;
double busytime;
double tottime;
long thrdpr;
long disablecomputing=-9;

UINT StatusTimer;

unsigned __stdcall cpufreetime(void * thd)
{
    unsigned long atc;
    unsigned long btc;
    unsigned long ctc;

    char cbuf[80];
    
    busytime=10000;
    tottime=20000;
    thrdpr=-3;
    disablecomputing=-9;

    do {
// btc-atc is 500ms + time it takes for a thread of priority 2 to get some cpu time.
// ctc-btc is 0ms + time it takes for a thread of priority thrdpr+1 to get some cpu time.

// if cpu is busy with 'real' work, btc-atc will be large
// if cpu is busy with 'idle' work (of priority above PiHex's), ctc-btc will be large.

        atc=GetTickCount();
        SetThreadPriority(GetCurrentThread(),2);
        Sleep(500*max(disablecomputing,1));
        btc=GetTickCount();
        SetThreadPriority(GetCurrentThread(),thrdpr+1);
        Sleep(0);
        ctc=GetTickCount();

        sprintf(cbuf,"%d,%d\n",btc-atc,ctc-btc);
//        OutputStr(mainHwnd,cbuf);

        if(atc+550>btc)
        {
            tottime+=ctc-atc;
            if(btc+25<ctc) busytime+=ctc-atc;
        };
        if(atc+525*max(disablecomputing,1)>btc) {
            disablecomputing=(disablecomputing-9)/2;
            if((disablecomputing>=-5)&&(disablecomputing<-1)) {
                StatusTimer=SetTimer(mainHwnd,tStatus,5000,NULL);
//                OutputStr(mainHwnd,"Restarted Timer");
            };
        }
        else {
            disablecomputing=min(disablecomputing+1,1000000000);
            if(disablecomputing==-1) {
                KillTimer(mainHwnd,StatusTimer);
//                OutputStr(mainHwnd,"Killed Timer");
            };
        };

        busytime-=busytime/100;
        tottime-=tottime/100;

        if(busytime*4>tottime*3)
        {
            busytime=10000;
            tottime=15000;
            thrdpr+=1;
            if(thrdpr>1) thrdpr=1;
        };            
        if(busytime*4<tottime)
        {
            busytime=10000;
            tottime=15000;
            thrdpr-=1;
            if(thrdpr<-3) thrdpr=-3;
        };            

        } while(cont&&(PR_OPT==PR_AUTO));
    return 0;
}

DWORD GetHighTime(void)
{
    SYSTEMTIME systime;
    FILETIME ftime;

    GetSystemTime(&systime);
    SystemTimeToFileTime(&systime,&ftime);

    return(ftime.dwHighDateTime);
};

void StatusProc(HWND Hwnd)                      //triggered by WM_TIMER
{
    WIN32_FIND_DATA temp;
    HANDLE hFind;
    
    calc_main_status();
    if(strlen(calc_Status)>0) {
        OutputStr(mainHwnd,calc_Status);
        LineFeed(mainHwnd);
    };

    if(lastcommunicated+daysworktoget*400<GetHighTime()) request_communication(0,0);
    // We have not communicated recently.  Communicate

    if(wanttocommunicate&&(!communicating)) OutputStr(mainHwnd,"PiHex would like to communicate with the server.");
    if(connectmethod/2==2) {
        hFind=FindFirstFile("output.txt",&temp);
        if(hFind!=INVALID_HANDLE_VALUE) {     //file exists
            OutputStr(mainHwnd,"Please send output.txt to the PiHex server (see readme.txt)\n");
            FindClose(hFind);
        };

        hFind=FindFirstFile("input.txt",&temp);
        if(hFind!=INVALID_HANDLE_VALUE) {     //file exists
            FindClose(hFind);
            processinput();
        };
    };

    if(calc_error||windowflashed||wanttocommunicate)
    {
        windowflashed=!windowflashed;
        FlashWindow(Hwnd,TRUE);
    }
    if(TRAY_ICON) {
    if(windowflashed) TrayMessage(NIM_MODIFY,calc_Tip,Hwnd,BlackIcon);
    else TrayMessage(NIM_MODIFY,calc_Tip,Hwnd,AppIcon);}
}

#ifdef Sample
FILE *SampleFile;
UINT SampleTimer;
void SampleProc()                               //triggered by WM_TIMER
{
    CONTEXT lpcontext;
    lpcontext.ContextFlags=CONTEXT_CONTROL; //find out where thread 0 is
    GetThreadContext(hCalcThread[0],&lpcontext);
    fprintf(SampleFile,"%X:%X\n",lpcontext.SegCs,lpcontext.Eip);
                                            //and write it to a file
}
#endif

unsigned __stdcall CalcThread(struct thdat *threaddat)
{
    struct thdat *tmpdat;
    long curpr;
#ifdef times
    double st;
    int longst;
#endif


    threadsactive++;
    tmpdat=(struct thdat*)(((long)threaddat+15)&0xFFFFFFF0);     //round up to
    * (long *) tmpdat=* (long *) threaddat;                      //16 byte boundary.
    calc_thread_init(tmpdat);

    curpr=thrdpr;
    if(curpr>-3) SetThreadPriority(GetCurrentThread(),curpr); else
                 SetThreadPriority(GetCurrentThread(),-15);

    do {
        if(curpr!=thrdpr)
        {
            curpr=thrdpr;
            if(curpr>-3) SetThreadPriority(GetCurrentThread(),curpr); else
                         SetThreadPriority(GetCurrentThread(),-15);
        };


#ifdef times
        st=rdtsc();
#endif
        if(disablecomputing<1) calc_thread_dowork(tmpdat); else Sleep(500*(disablecomputing+1));
#ifdef times
        st=rdtsc()-st;
        longst=st;
        fprintf(timesfl,"%d\n",longst);
#endif
        } while(cont&&(tmpdat->threadnum+1));

    if(tmpdat->threadnum==-1) {
        calc_main_status();
        if(strlen(calc_Status)>0) {
            OutputStr(mainHwnd,calc_Status);
            LineFeed(mainHwnd);
        };}                            //if thread exited by itself, print any error 
                                       //message it put into calc_Status

    calc_thread_done(tmpdat);

    if(!cont) {
        threadsactive--;
        HeapFree(GetProcessHeap(),0,threaddat);
        return(0);
    };

    HeapFree(GetProcessHeap(),0,threaddat);
    
    do {Sleep(0);} while(calc_error&&cont);     //don't exit until user tells us to if there is an error

    if(threadsactive==1) {PostMessage(mainHwnd,WM_COMMAND,MENU_STOP,0);
                    //if this is last thread still active, stop calculating.

    do {Sleep(0);} while(cont);}  //don't exit until EndCalc actually gets called.

    threadsactive--;
    return(0);
} 

void CpuId(void)
{
    CPUID_PROC();


    if(strcmp(VENDOR,"GenuineIntel")==0) {CPUVENDOR=CPU_INTEL;}
    else
    if(strcmp(VENDOR,"AuthenticAMD")==0) {CPUVENDOR=CPU_AMD;}
    else
    if(strcmp(VENDOR,"CyrixInstead")==0) {CPUVENDOR=CPU_CYRIX;}
    else
    if(strcmp(VENDOR,"CentaurHauls")==0) {CPUVENDOR=CPU_IDT;}
    else
    CPUVENDOR=CPU_UNKNOWN;
};

void StartCalc(HWND hwnd)
{
    long thnum;
    struct thdat *threaddat;
    
    EnableMenuItem(GetMenu(hwnd),MENU_STOP,MF_ENABLED);
    EnableMenuItem(GetMenu(hwnd),MENU_START,MF_GRAYED);

    cont=TRUE;
    threadsactive=0;
    windowflashed=0;

#ifdef times
    timesfl=fopen(timesfn,"at");
#endif


    threads=calc_main_init();
    if(threads)          //if code supports multiple (calculating) threads...
    {
        threads=cpunum;
    }
    else threads=1;                                //otherwise, run just one.
    
    for(thnum=0;thnum<threads;thnum++) {           //for each thread...
    threaddat=HeapAlloc(GetProcessHeap(),0,ThreadDataSize+16);//get some memory
    threaddat->threadnum=thnum;                              //tell it its name
    hCalcThread[thnum]=(HANDLE)_beginthreadex(NULL,0,CalcThread,threaddat,
    0,(unsigned *)&CalcThreadId[thnum]);          //and create it
    }

    Sleep(1000);
    StatusTimer=SetTimer(hwnd,tStatus,5000,NULL);
#ifdef Sample
    SampleFile=fopen(Samplefn,"at");
    SampleTimer=SetTimer(hwnd,tSample,1,NULL);
#endif

    if(PR_OPT==PR_AUTO)
    SetThreadPriority( (HANDLE) _beginthreadex(NULL,0,cpufreetime,
    NULL,0,(unsigned *)&cftblah),2);
    else thrdpr=PR_OPT-PR_4;
}

void EndCalc(HWND hwnd)
{
    long thnum;
    long temp;

    EnableMenuItem(GetMenu(hwnd),MENU_START,MF_ENABLED);
    EnableMenuItem(GetMenu(hwnd),MENU_STOP,MF_GRAYED);

    if(threadsactive==0) return;        //if there's nothing to stop, exit

    cont=FALSE;                          //tell threads to exit.

    KillTimer(hwnd,StatusTimer);
#ifdef Sample
    KillTimer(hwnd,SampleTimer);
    fclose(SampleFile);
#endif

    for(thnum=0;thnum<threads;thnum++)
    {
        GetExitCodeThread(hCalcThread[thnum],(LPDWORD)&temp);
        if(temp==STILL_ACTIVE)
        SetThreadPriority(hCalcThread[thnum],THREAD_PRIORITY_HIGHEST);
    }   //set all the threads to highest priority, so that it will finish
        //even if some other programs are sucking up CPU cycles.

    do Sleep(0); while(threadsactive);


        //wait for all threads to finish
#ifdef times
    fclose(timesfl);
#endif
    calc_main_done();
    TrayMessage(NIM_MODIFY,calc_Tip,hwnd,AppIcon);              //fix the icon, in case it is black
}

long high32oftime64(char *fname)
{
    HANDLE f;
    FILETIME lastwrite;

    f=CreateFile(fname,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

    GetFileTime(f,NULL,NULL,&lastwrite);

    CloseHandle(f);

    return(lastwrite.dwHighDateTime);
};

UINT TrayTimer=0;
void TrayMessage(UINT message,LPCSTR prompt,HWND hwnd,UINT Icon)
{
    NOTIFYICONDATA tnd;

    switch (message) {
        case NIM_ADD:
            tnd.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP;
            break;
        case NIM_MODIFY:
            tnd.uFlags=NIF_TIP|NIF_ICON;
            break;
        case NIM_DELETE:
            tnd.uFlags=0;
        }
        
    lstrcpy(tnd.szTip,prompt);

    tnd.uID=1;
    tnd.cbSize=sizeof(tnd);
    tnd.hWnd=hwnd;
    tnd.uCallbackMessage=WM_TRAY;
    tnd.hIcon=LoadIcon(AppInst,MAKEINTRESOURCE(Icon));

    if((!Shell_NotifyIcon(message, &tnd))&&(message==NIM_ADD)&&(!TrayTimer))
        TrayTimer=SetTimer(hwnd,tTray,1000,NULL);
    else
    if(TrayTimer) {KillTimer(hwnd,TrayTimer);TrayTimer=0;}
}

BOOL FAR PASCAL About(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
    switch(msg) {
        case WM_INITDIALOG:return(TRUE);
        case WM_COMMAND:if(wParam==IDOK||wParam==IDCANCEL)
        {EndDialog(hwnd,TRUE);
            return(TRUE);}
        break;
    }
    return (FALSE);
}

BOOL FAR PASCAL Priority(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
    switch(msg) {
        case WM_INITDIALOG:
            CheckRadioButton(hwnd,PR_1,PR_AUTO,PR_OPT);
            return(TRUE);
        case WM_COMMAND:
        switch(wParam) {
            case IDOK:
                if(IsDlgButtonChecked(hwnd,PR_1)) PR_OPT=PR_1;
                if(IsDlgButtonChecked(hwnd,PR_2)) PR_OPT=PR_2;
                if(IsDlgButtonChecked(hwnd,PR_3)) PR_OPT=PR_3;
                if(IsDlgButtonChecked(hwnd,PR_4)) PR_OPT=PR_4;
                if(IsDlgButtonChecked(hwnd,PR_5)) PR_OPT=PR_5;
                if(IsDlgButtonChecked(hwnd,PR_AUTO)) PR_OPT=PR_AUTO;
                if(PR_OPT==PR_AUTO)
                {if(threadsactive) SetThreadPriority( (HANDLE) _beginthreadex(NULL,0,cpufreetime,
                NULL,0,(unsigned *)&cftblah),2);}
                else thrdpr=PR_OPT-PR_4;
            case IDCANCEL:
                EndDialog(hwnd,TRUE);
            break;
        }
        break;
    }
    return (FALSE);
}

extern long rangeswaiting;

long processmsg(char *dat)
{
    char *dat1,*temp;
    char cbuf[10];
    float time;
    FILE *f;

    if(strncmp(dat,"HTTP",4)==0)
    {
        dat1=strstr(dat,"\r\n\r\n");
        if(dat1!=NULL) dat1+=2;

        if(dat1==NULL) dat1=strstr(dat,"\n\n");
        if(dat1!=NULL) dat1+=2;

        if(dat1==NULL) {
            OutputStr(mainHwnd,"The server has sent back an invalid reply.\nThis should never occur.\n");
            return(1800);
        };
    }
    else dat1=dat;
    
    if(strncmp(dat1,"computernum,",12)==0) {
        sscanf(dat1+12,"%d",&computernum);
        sprintf(dat1,"%d",computernum);
        WritePrivateProfileString("Main","computernum",dat1,IniFileName);
    }
    else
    if(strncmp(dat1,"range,",6)==0) {
        f=fopen("range0.ini","r");
        if(f!=NULL) {
            fclose(f);
            f=fopen("range1.ini","wt");
        }
        else
        {
            f=fopen("range0.ini","wt");
        };

        *(dat1+22)=0;
        *(dat1+31)=0;
        *(dat1+40)=0;

        temp=dat1+strlen(dat1)+1;

        fprintf(f,"[Main]\nwhere=%s\nsrt_pos=0%s0000000\nend_pos=0%s0000000\nrnum=%s\ntime=%s\n",dat1+6,dat1+23,dat1+32,dat1+41,temp);
        fclose(f);

        rangeswaiting++;

        sprintf(cbuf,"%d",rangeswaiting);
        WritePrivateProfileString("Main","rangesw",cbuf,IniFileName);
    }
    else
    if(strncmp(dat1,"rtime",5)==0) {
        sscanf(dat1+6,"%f",&time);
        sprintf(cbuf,"%f",time*8.382);
        WritePrivateProfileString("Main","rtime",cbuf,IniFileName);
    }
    else
    if(strncmp(dat1,"ignoreme",8)==0) {
    }
    else
    if(strncmp(dat1,"OK",2)!=0) {
        OutputStr(mainHwnd,"I don't understand what the server just told me, so I'll just ignore it.\nThis should never occur.\n");
    };

    return(0);
}

void processinput(void)
{
    FILE *f;
    char dat[256];
    long temp;

    f=fopen("input.txt","rt");
    while(fgets(dat,256,f)!=NULL)
    {
        if(strncmp(dat,"date",4)==0) {
            // compare dates
            sscanf(dat,"datetime,%d",&temp);
            if(temp+2>high32oftime64("output.txt")) remove("output.txt");
        }
        else
        if(strlen(dat)>2) processmsg(dat);
    };
    fclose(f);
    remove("input.txt");
};

typedef DWORD APIENTRY (*TRasEnumConnectionsA)( LPRASCONNA, LPDWORD, LPDWORD );
typedef DWORD APIENTRY (*TRasGetConnectStatusA)( HRASCONN, LPRASCONNSTATUSA );

long modemconnected(void)
{
    RASCONN * connlist;
    RASCONNSTATUS status;
    long size;
    long entrynum;
    long temp;

    char cbuf[256];

    HINSTANCE hinstLib; 
    TRasEnumConnectionsA PRasEnumConnectionsA;
    TRasGetConnectStatusA PRasGetConnectStatusA;
     

    hinstLib=LoadLibrary("RASAPI32"); 
 
    /* If the handle is valid, try to get the function address. */ 

    if(hinstLib==NULL) {
        OutputStr(mainHwnd,"Could not load RAS DLL.\n");
        return(0);
    };

    PRasEnumConnectionsA = (TRasEnumConnectionsA) GetProcAddress(hinstLib,"RasEnumConnectionsA");
    PRasGetConnectStatusA = (TRasGetConnectStatusA) GetProcAddress(hinstLib,"RasGetConnectStatusA");

    /* If the function address is valid, call the function. */ 
 
    if((PRasEnumConnectionsA==NULL)||(PRasGetConnectStatusA==NULL)) {
        OutputStr(mainHwnd,"Could not get RAS functions.\n");
        return(0);
    };

    size=0;
    entrynum=0;
    (PRasEnumConnectionsA)(NULL,&size,&entrynum);

    connlist=malloc(size+sizeof(RASCONN));

    connlist->dwSize=sizeof(RASCONN);
    
    temp=(PRasEnumConnectionsA)(connlist,&size,&entrynum);

    if(temp) {
        OutputStr(mainHwnd,"Error in RasEnumConnections.\nThis should never happen.\n");
        sprintf(cbuf,"Error code: %d\n",temp);
        OutputStr(mainHwnd,cbuf);
    };

    if(entrynum==0) {
        OutputStr(mainHwnd,"No active dial-up connection found.\n");
        return(0);
    };

    status.dwSize=sizeof(RASCONNSTATUS);

    (PRasGetConnectStatusA)(connlist->hrasconn,&status);

    FreeLibrary(hinstLib);

    if(status.rasconnstate==RASCS_Connected) {
//        OutputStr(mainHwnd,"Found active RAS connection!!!\n");
        return(1);
    };
        
    OutputStr(mainHwnd,"No active dial-up connection found.\n");
    return(0);    
}


long attempt_communication(void)
{
    FILE *f;
    FILE *g;

    int err;
    WORD wVersionRequested; 
    WSADATA wsaData; 
    struct hostent *hp;
    SOCKET s;
    SOCKADDR_IN sn;
    long len;
    long temp;
    char sendstr[2560];
    char dat[256];
    char cbuf[80];

    if(connectmethod/2==2) {
        // sneakernet: append spool.txt into output.txt
        f=fopen("spool.txt","rt");
        g=fopen("output.txt","at");
        if((f!=NULL)&&(g!=NULL)) 
        while(fgets(dat,256,f)!=NULL)
        {
            temp=computernum;
            if(strncmp(dat,"newuser",6)==0) temp=-1;

            fprintf(g,"%d,%s",temp,&dat);
        };
        fclose(f);
        remove("spool.txt");

        fclose(g);
        temp=high32oftime64("output.txt");

        g=fopen("output.txt","at");
        fprintf(g,"-1,datetime,%d\n",temp);
        fclose(g);

        return(0);
    };

    if(connectmethod/2==1)
    if(modemconnected()==0) {
        return(120);
    };

    wVersionRequested = MAKEWORD(1, 1); 
    err = WSAStartup(wVersionRequested, &wsaData); 
    if(err!=0) {
        OutputStr(mainHwnd,"Winsock stack could not be found.\n");
        return(1800);
    };

#ifdef DEBUG_INET
    hp=malloc(sizeof(*hp));
    hp->h_addrtype=PF_INET;
    hp->h_length=4;
    temp=inet_addr("192.168.0.3");
    sn.sin_family=hp->h_addrtype;
    sn.sin_port = htons(80);
    memcpy(&sn.sin_addr,&temp,hp->h_length);
#else
    if(connectmethod%2==0) {
        hp=gethostbyname("pihex.cecm.sfu.ca");
        sn.sin_port = htons(80);
    }
    else
    {
        if(isdigit(proxyaddr[strlen(proxyaddr)-1])) {
            temp=inet_addr(proxyaddr);
            hp=gethostbyaddr(&temp,4,PF_INET);
        }
        else
        hp=gethostbyname(proxyaddr);
        sn.sin_port = htons(proxyport);
    };
    
    if(!hp) {
        OutputStr(mainHwnd,"PiHex server could not be found.\n");
        return(1800);
    };

    sn.sin_family=hp->h_addrtype;

    memcpy(&sn.sin_addr, hp->h_addr,hp->h_length);
#endif


#ifdef DEBUG
    g=fopen("comm.txt","at");
#endif

    f=fopen("spool.txt","rt");
    if(f!=NULL) 
    while(fgets(dat,256,f)!=NULL)
    {
        if(*(dat+strlen(dat)-1)=='\n') *(dat+strlen(dat)-1)=0;

        s=socket(hp->h_addrtype,SOCK_STREAM,0);
        if(s==INVALID_SOCKET) {
            fclose(f);
            OutputStr(mainHwnd,"Cannot create socket. This should never occur.\n");
#ifdef DEBUG
            fclose(g);
#endif
            return(1800);
        };

        err=connect(s,(SOCKADDR *) &sn,sizeof(sn));
        if(err!=0) {
            fclose(f);
            OutputStr(mainHwnd,"Could not connect to server.\n");
#ifdef DEBUG
            fclose(g);
#endif
            return(1800);
            };

        temp=computernum;
        if(strncmp(dat,"newuser",6)==0) temp=-1;

        if(connectmethod%2==0)
        sprintf(sendstr,"GET /sendmsg.cgi?%d,%s HTTP/1.1\r\n\r\n",temp,&dat);
        else
        sprintf(sendstr,"GET http://pihex.cecm.sfu.ca/sendmsg.cgi?%d,%s HTTP/1.0\r\nPragma: no-cache\r\nCache-Control: no-cache\r\n\r\n",temp,&dat);
        

#ifdef DEBUG
        fprintf(g,"Sending string:%s",sendstr);
#endif
//        OutputStr(mainHwnd,sendstr);

        send(s,sendstr,strlen(sendstr),0);

        len=recv(s,sendstr,2559,0);
        while((temp=recv(s,&sendstr[len],2559-len,0))>0) len+=temp;
        if(temp==SOCKET_ERROR) {
            temp=WSAGetLastError();
            if(temp!=10014) {
                fclose(f);
                OutputStr(mainHwnd,"Something bad happened.  Please contact cperciva@sfu.ca about this.\n");
                sprintf(cbuf,"Error in recv: %d\n",temp);
                OutputStr(mainHwnd,cbuf);
                return(1800);
            };
        };

        sendstr[len]=0;

        closesocket(s);

#ifdef DEBUG
        fprintf(g,"Received string:%s",sendstr);
#endif
//        OutputStr(mainHwnd,sendstr);
        temp=processmsg(sendstr);
        if(temp!=0) {
            fclose(f);
#ifdef DEBUG
            fclose(g);
#endif
            return(temp);
        };
    };

#ifdef DEBUG
    fclose(g);
#endif

    WSACleanup();
    fclose(f);
    remove("spool.txt");

    return(0);
};


void spoolmsg(char *dat)
{
    FILE *f;
    char *x;

    for(x=dat;*x!=0;x++) if(*x==' ') *x='_';

    f=fopen("spool.txt","at");
    fprintf(f,dat);
    fclose(f);
}

BOOL FAR PASCAL ProxyInfo(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
    char cbuf[128];

    switch(msg) {
        case WM_INITDIALOG:
            SetDlgItemText(hwnd,PROXY_Edit,proxyaddr);
            sprintf(cbuf,"%d",proxyport);
            SetDlgItemText(hwnd,PORT_Edit,cbuf);
            return(TRUE);

        case WM_COMMAND:
            if(wParam==IDOK) {
                GetDlgItemText(hwnd,PROXY_Edit,proxyaddr,128);

                GetDlgItemText(hwnd,PORT_Edit,cbuf,128);
                sscanf(cbuf,"%d",&proxyport);

                WritePrivateProfileString("Main","proxyaddr",proxyaddr,IniFileName);

                sprintf(cbuf,"%d",proxyport);
                WritePrivateProfileString("Main","proxyport",cbuf,IniFileName);

                EndDialog(hwnd,TRUE);
                return(TRUE);
            };

            if(wParam==IDCANCEL) {
                EndDialog(hwnd,TRUE);
                return(TRUE);
            };
        break;
    }
    return (FALSE);
}


BOOL FAR PASCAL CpuInfo(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
    char cbuf[128];
    FARPROC lpProc;

    switch(msg) {
        case WM_INITDIALOG:
            SetDlgItemText(hwnd,UNAME_Edit,UserName);
            SetDlgItemText(hwnd,EMAIL_Edit,Useremail);

            if(cpuvendor==CPU_INTEL)
            {
                if(cputype==5) CheckDlgButton(hwnd,CT_I5,1);
                if(cputype==6) CheckDlgButton(hwnd,CT_I6,1);
            }
            else
            if(cpuvendor==CPU_AMD) {
                if(cputype==5) CheckDlgButton(hwnd,CT_A5,1);
                if(cputype==6) CheckDlgButton(hwnd,CT_A6,1);
            }
            else
            if(cpuvendor==CPU_CYRIX) CheckDlgButton(hwnd,CT_C6,1);
            else
            CheckDlgButton(hwnd,CT_I4,1);

            sprintf(cbuf,"%d",cpuspeed);
            SetDlgItemText(hwnd,CSPEED_Edit,cbuf);
            
            sprintf(cbuf,"%d",cpunum);
            SetDlgItemText(hwnd,CNUM_Edit,cbuf);

            if(connectmethod%2==0) {
                CheckDlgButton(hwnd,PROXY_Check,0);
            }
            else CheckDlgButton(hwnd,PROXY_Check,1);

            if(connectmethod/2==0) CheckDlgButton(hwnd,CM_NET,1);
            else if(connectmethod/2==1) CheckDlgButton(hwnd,CM_DIAL,1);
            else {
                CheckDlgButton(hwnd,CM_SNEAK,1);
                CheckDlgButton(hwnd,PROXY_Check,0);
            };
            
            sprintf(cbuf,"%4.1f",hoursworkperday);
            SetDlgItemText(hwnd,CHOURS_Edit,cbuf);
            
            sprintf(cbuf,"%4.1f",daysworktoget);
            SetDlgItemText(hwnd,CDAYS_Edit,cbuf);

            if(autoconn) CheckDlgButton(hwnd,AUTOCOM_Check,1);
            else CheckDlgButton(hwnd,AUTOCOM_Check,0);
            return(TRUE);

//  TODO 6.1: Add proxy authentication?
        case WM_COMMAND:
            if(wParam==PROXY_Config) {
                lpProc=MakeProcInstance(ProxyInfo,hInst);
                DialogBox(AppInst,MAKEINTRESOURCE(PROXYINFOBOX),hwnd,lpProc);
                return(TRUE);
            };

            if(wParam==IDOK) {
                GetDlgItemText(hwnd,UNAME_Edit,UserName,75);
                GetDlgItemText(hwnd,EMAIL_Edit,Useremail,75);

                if(IsDlgButtonChecked(hwnd,CT_I5)) {cputype=5;cpuvendor=CPU_INTEL;};
                if(IsDlgButtonChecked(hwnd,CT_I6)) {cputype=6;cpuvendor=CPU_INTEL;};
                if(IsDlgButtonChecked(hwnd,CT_A5)) {cputype=5;cpuvendor=CPU_AMD;};
                if(IsDlgButtonChecked(hwnd,CT_A6)) {cputype=6;cpuvendor=CPU_AMD;};
                if(IsDlgButtonChecked(hwnd,CT_C6)) {cputype=5;cpuvendor=CPU_CYRIX;};
                if(IsDlgButtonChecked(hwnd,CT_I4)) {cputype=4;cpuvendor=CPU_INTEL;};

                GetDlgItemText(hwnd,CSPEED_Edit,cbuf,75);
                sscanf(cbuf,"%d",&cpuspeed);

                GetDlgItemText(hwnd,CNUM_Edit,cbuf,75);
                sscanf(cbuf,"%d",&cpunum);

                connectmethod=IsDlgButtonChecked(hwnd,CM_DIAL)*2+IsDlgButtonChecked(hwnd,CM_SNEAK)*4+IsDlgButtonChecked(hwnd,PROXY_Check);

                GetDlgItemText(hwnd,CHOURS_Edit,cbuf,75);
                hoursworkperday=atof(&cbuf);

                GetDlgItemText(hwnd,CDAYS_Edit,cbuf,75);
                daysworktoget=atof(&cbuf);

                if(hoursworkperday<0.1) hoursworkperday=0.1;

                if(daysworktoget<0.5) daysworktoget=0.5;

                if(IsDlgButtonChecked(hwnd,AUTOCOM_Check)) autoconn=1; else autoconn=0;

                WritePrivateProfileString("Main","UserName",UserName,IniFileName);
                WritePrivateProfileString("Main","Useremail",Useremail,IniFileName);

                sprintf(cbuf,"%f",hoursworkperday);
                WritePrivateProfileString("Main","hoursperday",cbuf,IniFileName);
                sprintf(cbuf,"%f",daysworktoget);
                WritePrivateProfileString("Main","dayswork",cbuf,IniFileName);
                sprintf(cbuf,"%d",connectmethod);
                WritePrivateProfileString("Main","connectmethod",cbuf,IniFileName);
                sprintf(cbuf,"%d",autoconn);
                WritePrivateProfileString("Main","Autocom",cbuf,IniFileName);

                sprintf(cbuf,"%d",cputype);
                WritePrivateProfileString("Main","cputype",cbuf,IniFileName);
                sprintf(cbuf,"%d",cpuspeed);
                WritePrivateProfileString("Main","cpuspeed",cbuf,IniFileName);
                sprintf(cbuf,"%d",cpuvendor);
                WritePrivateProfileString("Main","cpuvendor",cbuf,IniFileName);
                sprintf(cbuf,"%d",cpunum);
                WritePrivateProfileString("Main","cpunum",cbuf,IniFileName);

                EndDialog(hwnd,TRUE);
                return(TRUE);
            };
            if(wParam==IDCANCEL) {
                EndDialog(hwnd,FALSE);
                return(TRUE);
            };
        break;
    }
    return (FALSE);
}


//  Returns 0 if ID_OK, -1 if ID_CANCEL
int CreateUserInfoWindow()
{
    char cbuf[128];
    FARPROC lpProc;

    lpProc=MakeProcInstance(CpuInfo,hInst);
    if(DialogBox(AppInst,MAKEINTRESOURCE(CPUINFOBOX),mainHwnd,lpProc)==FALSE) {
        FreeProcInstance(lpProc);
        return(-1);
    };

    FreeProcInstance(lpProc);

    if(joined<1)
    if(computernum==0)
    sprintf(cbuf,"ignoreme\nnewuser,%s,%s,%d,%d,%d,%d,%f,%f\ngetrange\ngetrange\ngetrtime\n",UserName,Useremail,cpuvendor,cputype,cpuspeed,cpunum,hoursworkperday,daysworktoget);
    else
    sprintf(cbuf,"rejoin\nchange,%s,%s,%d,%d,%d,%d,%f,%f\ngetrange\ngetrange\ngetrtime\n",UserName,Useremail,cpuvendor,cputype,cpuspeed,cpunum,hoursworkperday,daysworktoget);
    else
    sprintf(cbuf,"change,%s,%s,%d,%d,%d,%d,%f,%f\ngetrtime\n",UserName,Useremail,cpuvendor,cputype,cpuspeed,cpunum,hoursworkperday,daysworktoget);

    spoolmsg(cbuf);

    return(0);
}

void GetCPUDat(void)
{
    SYSTEM_INFO sysinf;
    double TSCstart,TSCend;
    double startcount,endcount;
    double freq;

    CpuId();
    // Get vendor and version of cpu.

    GetSystemInfo(&sysinf);
    cpunum=sysinf.dwNumberOfProcessors;
    // Get # of cpus.

    // We want to compute the cpu speed in MHz.
    if(cputype<5) {cpuspeed=33; return;}
    // 386 and 486 computers don't have TSC, so they are a pain to compute.
    // We don't have many anyway, so just call them 33Mhz.

    if(!QueryPerformanceFrequency(&int64)) {cpuspeed=66; return;}
    // If counter is not available, guess 66MHz.

    freq=fild();

    TSCstart=rdtsc();
    QueryPerformanceCounter(&int64);
    startcount=fild();

    for(;;) {
        QueryPerformanceCounter(&int64);
        endcount=fild();
        if(endcount>startcount+max(5000,freq/20)) break;
    };

    TSCend=rdtsc();

    cpuspeed=(TSCend-TSCstart)*freq/(endcount-startcount)/1000000;
};

void NewUserAuto()
{
    char cbuf[128];

    GetCPUDat();

    sprintf(cbuf,"%d",cputype);
    WritePrivateProfileString("Main","cputype",cbuf,IniFileName);
    sprintf(cbuf,"%d",cpuspeed);
    WritePrivateProfileString("Main","cpuspeed",cbuf,IniFileName);
    sprintf(cbuf,"%d",cpuvendor);
    WritePrivateProfileString("Main","cpuvendor",cbuf,IniFileName);
    sprintf(cbuf,"%d",cpunum);
    WritePrivateProfileString("Main","cpunum",cbuf,IniFileName);

    sprintf(cbuf,"ignoreme\nnewuser,%s,%s,%d,%d,%d,%d,%f,%f\ngetrange\ngetrange\ngetrtime\n",UserName,Useremail,cpuvendor,cputype,cpuspeed,cpunum,hoursworkperday,daysworktoget);

    spoolmsg(cbuf);
}

long compaftercomm;

unsigned __stdcall commthread(void * junk)
{
    long delay;
    char str[256];

    if(connectmethod<4) OutputStr(mainHwnd,"Attempting to communicate with server\n");

    for(;;) {
        delay=attempt_communication();
        if(delay==0) {
            if(connectmethod<4) OutputStr(mainHwnd,"Finished communicating with server\n");
            if(compaftercomm==1) {
                compaftercomm=0;
                PostMessage(mainHwnd,WM_COMMAND,MENU_STOP,0);
                PostMessage(mainHwnd,WM_COMMAND,MENU_START,0);
//                StartCalc(mainHwnd);
            };
            lastcommunicated=GetHighTime();
            sprintf(str,"%d",lastcommunicated);
            WritePrivateProfileString("Main","lastcommunicated",str,IniFileName);
            break;
        };

        sprintf(str,"PiHex will try again in %d seconds.\n",delay);
        OutputStr(mainHwnd,str);

        for(;delay>0;delay--) {
            Sleep(1000);
            if(communicating==0) break;
        };

        if(communicating==0) break;
    };

    wanttocommunicate=0;
    communicating=0;

    EnableMenuItem(GetMenu(mainHwnd),MENU_COMMSTOP,MF_GRAYED);
    EnableMenuItem(GetMenu(mainHwnd),MENU_COMMUNICATE,MF_ENABLED);

    return(0);
}

long commblah;

void request_communication(long startcomputing,long forceattempt)
{
    if(startcomputing==1) compaftercomm=1;
    if(startcomputing==-1) compaftercomm=0;
    if(communicating==0)
    {
        if(!forceattempt) {
            if(autoconn==0) {
                wanttocommunicate=1;
                return;
            };
            if(lastcommunicated+daysworktoget*200>GetHighTime())
            // we have communicated recently, don't communicate again
            return;
        };

        EnableMenuItem(GetMenu(mainHwnd),MENU_COMMSTOP,MF_ENABLED);
        EnableMenuItem(GetMenu(mainHwnd),MENU_COMMUNICATE,MF_GRAYED);
        communicating=1;
        _beginthreadex(NULL,0,commthread,NULL,0,(unsigned *)&commblah);
    };
};

void JoinPiHex(void)
{
//  main/joined is either 0 or -1, since this menu item is grayed when joined.
//  1.  If joined=-1 then NewUserAuto, else CreateNewUserWindow.
//  2.  If CNUW succeeded, then communicate
//  3.  Once communication is done, start computing

    if(joined==-1) NewUserAuto();
    else
    {
        GetCPUDat();
        if(CreateUserInfoWindow()==-1)
        {
            EnableMenuItem(GetMenu(mainHwnd),MENU_STOP,MF_GRAYED);
            EnableMenuItem(GetMenu(mainHwnd),MENU_QUIT,MF_GRAYED);

            return;
        };
    };

    joined=1;
    WritePrivateProfileString("Main","joined","1",IniFileName);                

    EnableMenuItem(GetMenu(mainHwnd),MENU_JOIN,MF_GRAYED);

    EnableMenuItem(GetMenu(mainHwnd),MENU_STOP,MF_GRAYED);
    EnableMenuItem(GetMenu(mainHwnd),MENU_START,MF_GRAYED);

    OutputStr(mainHwnd,"PiHex will now attempt to contact the server to request some work...\n");
    request_communication(1,1);

    EnableMenuItem(GetMenu(mainHwnd),MENU_QUIT,MF_ENABLED);
}

//  If communicating, then do not allow MENU_COMMUNICATE
//  If cont == TRUE, do not allow MENU_START
//  If cont == FALSE, do not allow MENU_STOP
//  If joined = 1, do not allow MENU_JOIN
//  If joined != 1, do not allow MENU_QUIT

/*
 * WindowProc - handle messages for the main application window
 */
LONG _EXPORT FAR PASCAL WindowProc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam)
{
    FARPROC lpProc;
    struct _SYSTEM_POWER_STATUS powstat;
    char achBuf[40];

    RECT rect;

    switch(msg) {
    case WM_COMMAND:
        switch(LOWORD(wparam)) {
            case MENU_START:
                if(joined<1) JoinPiHex();
                else StartCalc(hwnd);
                return(0);
            case MENU_STOP:
                EndCalc(hwnd);
                return(0);
            case MENU_COMMUNICATE:
                request_communication(0,1);
                return(0);
            case MENU_COMMSTOP:
                EnableMenuItem(GetMenu(mainHwnd),MENU_COMMSTOP,MF_GRAYED);
                communicating=0;
                return(0);
            case MENU_QUIT:
                if(MessageBox(hwnd,"Are you sure you want to quit PiHex?\nThis means that this computer will never run PiHex again.","PiHex",MB_OKCANCEL)!=IDOK)
                    return(0);
            
                EndCalc(hwnd);
                pihex_dropout();
                spoolmsg("dropout\n");

                joined=0;
                WritePrivateProfileString("Main","joined","0",IniFileName);                

                EnableMenuItem(GetMenu(hwnd),MENU_JOIN,MF_ENABLED);
                EnableMenuItem(GetMenu(hwnd),MENU_QUIT,MF_GRAYED);

                OutputStr(mainHwnd,"A message has been spooled to notify the PiHex server that this machine is not going to run PiHex anymore.\n");
                OutputStr(mainHwnd,"Please allow PiHex to communicate with the server to relay this message.\n");

                request_communication(-1,1);
                return(0);
            case MENU_JOIN:
                JoinPiHex();
                return(0);
            case MENU_EXIT:
                goto done;
            case MENU_TRAY:
                TRAY_ICON^=MF_CHECKED;
                if(TRAY_ICON) NO_ICON=0;
                CheckMenuItem(GetMenu(hwnd),MENU_TRAY,TRAY_ICON);
                CheckMenuItem(GetMenu(hwnd),MENU_NOTRAY,NO_ICON);
                if(TRAY_ICON)
                TrayMessage(NIM_ADD,AppName,hwnd,AppIcon);
                else TrayMessage(NIM_DELETE,AppName,hwnd,AppIcon);
                return(0);
            case MENU_NOTRAY:
                if(TRAY_ICON) TrayMessage(NIM_DELETE,AppName,hwnd,AppIcon);
                NO_ICON^=MF_CHECKED;
                if(NO_ICON) TRAY_ICON=0;
                CheckMenuItem(GetMenu(hwnd),MENU_TRAY,TRAY_ICON);
                CheckMenuItem(GetMenu(hwnd),MENU_NOTRAY,NO_ICON);
                return(0);
            case MENU_AUTOCONT:
                AUTO_CONT^=MF_CHECKED;
                CheckMenuItem(GetMenu(hwnd),MENU_AUTOCONT,AUTO_CONT);
                return(0);
            case MENU_SERVICE:
                service^=MF_CHECKED;
                CheckMenuItem(GetMenu(hwnd),MENU_SERVICE,service);
                SetRegistry();
                MakeService95();
                return(0);                
            case MENU_PRIORITY:
                lpProc=MakeProcInstance(Priority,hInst);
                DialogBox(AppInst,MAKEINTRESOURCE(PriorityBox),hwnd,lpProc);
                FreeProcInstance(lpProc);
                return(0);
            case MENU_CPUINFO:
                if(joined<1) JoinPiHex();
                else CreateUserInfoWindow();
                return(0);
            case MENU_ABOUT:
                lpProc=MakeProcInstance(About,hInst);
                DialogBox(AppInst,MAKEINTRESOURCE(AboutBox),hwnd,lpProc);
                FreeProcInstance(lpProc);
                return(0);
        }
        break;

    case WM_TIMER:
        switch(wparam) {
            case tStatus:StatusProc(hwnd);return(0);
            case tTray:TrayMessage(NIM_ADD,AppName,hwnd,AppIcon);return(0);
#ifdef Sample
            case tSample:SampleProc();return(0);
#endif
        }
        break;

    case WM_TRAY:
        if(lparam==WM_LBUTTONDBLCLK)
        {
            if(IsWindowVisible(hwnd)) {
                ShowWindow(hwnd,SW_HIDE);
                SetForegroundWindow(hwnd);
            }
            else ShowWindow(hwnd,SW_RESTORE);
        }
        return(0);

    case WM_SIZE:
        if((wparam==SIZE_MINIMIZED)&&(TRAY_ICON||NO_ICON))
        {ShowWindow(hwnd,SW_HIDE);
        return(0);};
        break;

//Taken from prime.exe
//I have no idea how this works
    case WM_PAINT: {
        PAINTSTRUCT ps;
        RECT    r;
        int     y;
        int     i;
        HDC hdc;
        hdc = BeginPaint(hwnd, &ps);
        GetClientRect (hwnd, &r);
        if (lines[0] != NULL)
        for (y = r.bottom, i = 1; y > 0 && i < NumLines; i++) {
        y -= charHeight;
        TextOut (hdc, 0, y, lines[i], strlen (lines[i]));
        }
        EndPaint(hwnd, &ps);
        return(0);
        }

    case WM_CREATE:

        GetPrivateProfileString("Main","Window","",achBuf,40,IniFileName);
        if(strlen(achBuf)!=0) {
            sscanf(achBuf,"%d,%d,%d,%d",&rect.left,&rect.right,&rect.top,&rect.bottom);
            MoveWindow(hwnd,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,FALSE);
        };

        mainHwnd=hwnd;

        service=GetPrivateProfileInt("Main","Service",0,IniFileName);
        SetRegistry();
        if(service) MakeService95();

        TRAY_ICON=GetPrivateProfileInt("Main","Trayicon",8,IniFileName);
        NO_ICON=GetPrivateProfileInt("Main","Noicon",0,IniFileName);
        AUTO_CONT=GetPrivateProfileInt("Main","Autocontinue",8,IniFileName);
        joined=GetPrivateProfileInt("Main","Joined",0,IniFileName);

        GetPrivateProfileString("Main","hoursperday","24.0",Useremail,128,IniFileName);
        hoursworkperday=atof(Useremail);
        GetPrivateProfileString("Main","dayswork","7.0",Useremail,128,IniFileName);
        daysworktoget=atof(Useremail);

        GetPrivateProfileString("Main","UserName","",UserName,128,IniFileName);
        GetPrivateProfileString("Main","Useremail","",Useremail,128,IniFileName);

        cputype=GetPrivateProfileInt("Main","cputype",6,IniFileName);
        cpuspeed=GetPrivateProfileInt("Main","cpuspeed",300,IniFileName);
        cpuvendor=GetPrivateProfileInt("Main","cpuvendor",0,IniFileName);
        cpunum=GetPrivateProfileInt("Main","cpunum",0,IniFileName);

        connectmethod=GetPrivateProfileInt("Main","connectmethod",0,IniFileName);
        computernum=GetPrivateProfileInt("Main","computernum",0,IniFileName);

        autoconn=GetPrivateProfileInt("Main","autocom",1,IniFileName);
        rangeswaiting=GetPrivateProfileInt("Main","rangesw",0,IniFileName);

        GetPrivateProfileString("Main","proxyaddr","",proxyaddr,128,IniFileName);
        proxyport=GetPrivateProfileInt("Main","proxyport",8080,IniFileName);

        lastcommunicated=GetPrivateProfileInt("Main","lastcommunicated",0,IniFileName);

        if(TRAY_ICON) TrayMessage(NIM_ADD,AppName,hwnd,AppIcon);

        GetPrivateProfileString("Main","Priority","Auto",achBuf,40,IniFileName);
        if(strcmp(achBuf,"Auto"))
        PR_OPT=100+GetPrivateProfileInt("Main","Priority",PR_AUTO-100,IniFileName);
        else PR_OPT=PR_AUTO;

        CheckMenuItem(GetMenu(hwnd),MENU_NOTRAY,NO_ICON);
        CheckMenuItem(GetMenu(hwnd),MENU_TRAY,TRAY_ICON);
        CheckMenuItem(GetMenu(hwnd),MENU_AUTOCONT,AUTO_CONT);
        CheckMenuItem(GetMenu(hwnd),MENU_SERVICE,service);

        EnableMenuItem(GetMenu(mainHwnd),MENU_COMMSTOP,MF_GRAYED);

//  If main/joined is 0, then bring up userinfo box, and when user has finished entering data,
//     if ID_OK then communicate, start computing.
//  If main/joined is 1, then do whatever AUTO_CONT indicates.
//  If main/joined is -1, then get cpu info automatically, communicate, and start computing.
//
//  If user is not joined, then clicks user info or start computing, then
//     follow operations as with main/joined=0 at startup.

        if(joined<1) {
            if(joined==0) ShowWindow(hwnd,SW_SHOW);
                else ShowWindow(hwnd,SW_HIDE);
            JoinPiHex();
        }
        else
        if(AUTO_CONT)
        {
            EnableMenuItem(GetMenu(hwnd),MENU_JOIN,MF_GRAYED);

            if(!GetSystemPowerStatus(&powstat)) StartCalc(hwnd);
            // If we can't get the Power Status (eg, NT4.0, where the function is not
            // implemented) start anyway.
            else
            if((powstat.BatteryLifePercent>75)&&(powstat.ACLineStatus>0))
            StartCalc(hwnd);
            if(TRAY_ICON||NO_ICON) ShowWindow(hwnd,SW_HIDE);
            else ShowWindow(hwnd,SW_SHOW);
        }
        else {
            ShowWindow(hwnd,SW_SHOW);
            EnableMenuItem(GetMenu(hwnd),MENU_START,MF_ENABLED);
            EnableMenuItem(GetMenu(hwnd),MENU_STOP,MF_GRAYED);
        };
        return(0);

// Very important: When the user shuts down the machine, programs are NOT
// asked to exit -- we need to make sure we save our data!
    case WM_ENDSESSION:
        if(wparam) goto done;
        return(0);

    case WM_DESTROY:
done:   if(threadsactive) EndCalc(hwnd);
        sprintf(achBuf,"%d",NO_ICON);
        WritePrivateProfileString("Main","Noicon",achBuf,IniFileName);
        sprintf(achBuf,"%d",TRAY_ICON);
        WritePrivateProfileString("Main","Trayicon",achBuf,IniFileName);
        sprintf(achBuf,"%d",AUTO_CONT);
        WritePrivateProfileString("Main","Autocontinue",achBuf,IniFileName);
        sprintf(achBuf,"%d",service);
        WritePrivateProfileString("Main","service",achBuf,IniFileName);
        sprintf(achBuf,"%d",PR_OPT-100);
        if(PR_OPT==PR_AUTO)
        WritePrivateProfileString("Main","Priority","Auto",IniFileName);
        else WritePrivateProfileString("Main","Priority",achBuf,IniFileName);
        if(TRAY_ICON) TrayMessage(NIM_DELETE,AppName,hwnd,AppIcon);

        GetWindowRect(hwnd,&rect);
        sprintf(achBuf,"%d,%d,%d,%d",rect.left,rect.right,rect.top,rect.bottom);
        if((rect.left>0)&&(rect.left<3000)) WritePrivateProfileString("Main","Window",achBuf,IniFileName);
        
        PostQuitMessage(0);
    }
    return(DefWindowProc(hwnd,msg,wparam,lparam));
}


void Findini(void)
{
    char dir[128];
    char winfname[128];
    char curfname[128];
    char s[256];
    char sect[80];
    char value[80];
    char *tempc;
    FILE *f;
    FILE *g;
    long temp;

    GetCurrentDirectory(128,dir);
    sprintf(curfname,"%s\\%s",dir,iniName);
    GetWindowsDirectory(dir,128);
    sprintf(winfname,"%s\\%s",dir,iniName);

    if(GetPrivateProfileInt("Main","Trayicon",77,curfname)==77) {
    if(GetPrivateProfileInt("Main","Trayicon",77,winfname)!=77)
    {
        f=fopen(winfname,"rt");
        do
        {
            fgets(s,80,f);
            if(s[0]=='[') sscanf(s,"[%[^]]",sect);
            else
            if((tempc=strstr(s,"="))!=NULL)
            {
                (*tempc)=0;
                GetPrivateProfileString(sect,s,"",value,80,winfname);
                WritePrivateProfileString(sect,s,value,curfname);
            };
        } while(!feof(f));
        fclose(f);
        remove(winfname);
  };};

    GetCurrentDirectory(128,dir);
    sprintf(curfname,"%s\\%s",dir,iniName);
    GetCurrentDirectory(128,dir);
    sprintf(winfname,"%s\\%s",dir,otheriniName);

    if(GetPrivateProfileInt("Main","Trayicon",77,curfname)==77) {
    if(GetPrivateProfileInt("Main","Trayicon",77,winfname)!=77)
    {
        f=fopen(winfname,"rt");
        do
        {
            fgets(s,80,f);
            if(s[0]=='[') sscanf(s,"[%[^]]",sect);
            else
            if((tempc=strstr(s,"="))!=NULL)
            {
                (*tempc)=0;
                GetPrivateProfileString(sect,s,"",value,80,winfname);
                WritePrivateProfileString(sect,s,value,curfname);
            };
        } while(!feof(f));
        fclose(f);
        remove(winfname);
  };};

    sprintf(IniFileName,"%s",curfname);

    if(GetPrivateProfileInt("Main","PiHexVer",510,IniFileName)==510)
        if(GetPrivateProfileInt("Main","Autocom",666,IniFileName)==666) {
            // This really was version 5.10 or earlier

            // move all output into output.txt
            g=fopen("spool.txt","at");
            f=fopen("output0.txt","rt");
            if(f!=NULL) { while(fgets(s,256,f)!=NULL) fprintf(g,"output,%s",s);
            fclose(f); remove("output0.txt");};
            f=fopen("output1.txt","rt");
            if(f!=NULL) { while(fgets(s,256,f)!=NULL) fprintf(g,"output,%s",s);
            fclose(f); remove("output1.txt");};
            fclose(g);

            // kill the range that the computer was working on.
            WritePrivateProfileString("Thread0",NULL,NULL,IniFileName);
            WritePrivateProfileString("Thread1",NULL,NULL,IniFileName);
            WritePrivateProfileString("Thread2",NULL,NULL,IniFileName);
            WritePrivateProfileString("Thread3",NULL,NULL,IniFileName);

            WritePrivateProfileString("Main","output0",NULL,IniFileName);
            WritePrivateProfileString("Main","output1",NULL,IniFileName);
            WritePrivateProfileString("Main","current",NULL,IniFileName);
            WritePrivateProfileString("Main","where",NULL,IniFileName);
            WritePrivateProfileString("Main","srt_pos",NULL,IniFileName);
            WritePrivateProfileString("Main","cur_pos",NULL,IniFileName);
            WritePrivateProfileString("Main","end_pos",NULL,IniFileName);

            for(temp=0;temp<150;temp++) {
                sprintf(s,"range%d.ini",temp);
                remove(s);
            };
        }

    WritePrivateProfileString("Main","PiHexVer","600",IniFileName);
}

void SetRegistry(void)
{
    HKEY hkey;
    DWORD temp;
    char cbuf[256];

    RegCreateKeyEx(HKEY_LOCAL_MACHINE,"Software\\Microsoft\\Windows\\CurrentVersion\\RunServices",0,NULL,
                    REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hkey,&temp);

    if(service) {
        GetModuleFileName(NULL,cbuf,255);
        RegSetValueEx(hkey,"PiHex",0,REG_SZ,cbuf,strlen(cbuf)+1);
    } else RegDeleteValue(hkey,"PiHex");

    RegCloseKey (hkey);
};    

void MakeService95(void)
{
    HMODULE hlib;
    DWORD (__stdcall *proc)(DWORD, DWORD);
    DWORD   rc;

/* Call RegisterServiceProcess in the Kernel */

    hlib=LoadLibrary("KERNEL32.DLL");
    if(!hlib) {OutputStr(mainHwnd,"Could not load KERNEL32.DLL\n"); return;};
    proc=(DWORD(__stdcall *)(DWORD,DWORD)) GetProcAddress(hlib,"RegisterServiceProcess");
    if(proc==NULL) OutputStr(mainHwnd,"Unable to find RegisterServiceProcess\n");
    else {
        if(service) rc=(*proc)(NULL,1);
        else rc=(*proc)(NULL,0);
        if(!rc) OutputStr(mainHwnd,"RegisterServiceProcess failed\n");
    }
    FreeLibrary(hlib);
};    

/*
 * WinMain - initialization, message loop
 */

int PASCAL WinMain(HANDLE this_inst,HANDLE prev_inst,LPSTR cmdline,int cmdshow)
{
    MSG         msg;
    WNDCLASS    wc;
    HWND        hwnd;

    char cbuf[256];

    GetModuleFileName(NULL,cbuf,255);
    strrchr(cbuf,'\\')[1]=0;
    SetCurrentDirectory(cbuf);

    if(strlen(cmdline)>0) SetCurrentDirectory(cmdline);

//  TODO 6.1: Add 'no icon' option.

//  TODO 6.1: Add 'windows 9x service' option.

    Findini();

    AppInst = this_inst;

//  TODO 6.1: If another copy is running, tell that copy to restore itself (in case it is hidden).
//  We don't need to tell it -- we can restore it ourselves.  Gee, what a lack of security!

    if(hwnd=FindWindow(AppClass,AppName)) {
        ShowWindow(hwnd,SW_SHOW);
        SetForegroundWindow(hwnd);
        return(FALSE);
    };
        

//  TODO 6.1: Add code to cpufreetime so that if less than 5% of cpu time is idle, we don't use any.
//            This is necessary since the win32 scheduler schedules idle threads even when high
//            priority threads want CPU time.
//  It might work now;  I need to wait for Steven to get back from the US to find out if it still lags Quake.
//  No, it still doesn't work.

    SetPriorityClass(GetCurrentProcess(),IDLE_PRIORITY_CLASS);
    // Set priority to use time slices only when no other programs
    // want them.

    SetThreadPriority(GetCurrentThread(),2);
    // Set thread priority so that interface thread will be responsive.

    SetThreadAffinityMask(GetCurrentThread(),1);
    // Only run this thread on cpu 0.

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = (LPVOID) WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = sizeof( DWORD );
    wc.hInstance = this_inst;
    wc.hIcon = LoadIcon(this_inst,MAKEINTRESOURCE(AppIcon));
    wc.hCursor = LoadCursor( NULL, IDC_ARROW );
    wc.hbrBackground = GetStockObject( WHITE_BRUSH );
    wc.lpszMenuName = MAKEINTRESOURCE(AppMenu);
    wc.lpszClassName = AppClass;

    if(!RegisterClass(&wc)) return(FALSE);

    hwnd = CreateWindow(
        AppClass,             /* class */
        AppName,                /* caption */
        WS_OVERLAPPEDWINDOW,    /* style */
        CW_USEDEFAULT,          /* init. x pos */
        CW_USEDEFAULT,          /* init. y pos */
        CW_USEDEFAULT,          /* init. x size */
        CW_USEDEFAULT,          /* init. y size */
        NULL,                   /* parent window */
        NULL,                   /* menu handle */
        this_inst,              /* program handle */
        NULL                    /* create parms */
        );
                    
    if( !hwnd ) return( FALSE );

    UpdateWindow( hwnd );
   
    while( GetMessage( &msg, NULL, NULL, NULL ) ) {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }

    return( msg.wParam );

} /* WinMain */

// following lines taken from prime.exe
/*************************************************************************/
/*                      Output Text to Main Window                       */
/*************************************************************************/

void FAR PASCAL LineFeed (
        HWND    hWnd)
{
        char    *p;
        SIZE s;

        if (lines[0] != NULL) {
                p = lines[NumLines-1];
                memmove (&lines[1], &lines[0], (NumLines-1)*sizeof(char *));
                lines[0] = p;
                *p = 0;
        }

        if (charHeight == 0)
        {
            GetTextExtentPoint(GetDC(hWnd),"A",1,&s);
            charHeight=s.cy;
        }
          
        ScrollWindow (hWnd, 0, -charHeight, NULL, NULL);
        UpdateWindow (hWnd);
}

void FAR PASCAL OutputStr (
        HWND    hWnd,
        LPSTR   str)
{
        char    *p;
        int     i;

        if (lines[0] == NULL) {
                for (i = 0; i < NumLines; i++) {
                        lines[i] = &linebuf[i*LineSize];
                        *lines[i] = 0;
                }
        }
        p = lines[0] + strlen (lines[0]);
        for ( ; *str; str++) {
                if (*str == '\n') *p = 0, LineFeed (hWnd), p = lines[0];
                else *p++ = *str;
        }
        *p = 0;

}


