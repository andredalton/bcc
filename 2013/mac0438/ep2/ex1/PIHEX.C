#include <string.h>
#include <stdio.h>
#include <direct.h>
#include <windows.h>
#include <float.h>
#include "pihex.h"
#include "main.h"

#ifdef Sample
char Samplefn[]="Pihexsmp.txt";
#endif
#ifdef times
char timesfn[]="pitimes.txt";
#endif

char iniName[]=sIniName;
char otheriniName[]=sotherIniName;
extern char iniFileName[];
extern long communicating;
extern long cont;

extern HWND mainHwnd;
extern void FAR PASCAL OutputStr (
        HWND    hWnd,
        LPSTR   str);


unsigned long where[2];      //these are for the range currently being divided between
unsigned long m_start_pos[2];//the threads.
unsigned long progress[2];
unsigned long m_current_pos[2];
unsigned long m_end_pos[2];

long overheat_err;
long numclocks;
float fracdone;              //used by calc_main_status -- if a new range is just started and
                             //the last one isn't done, (i.e. with a multi-processor system)
                             //this can be negative.


extern long lastcommunicated;

long rangeswaiting;

typedef struct {
        long d[2];
        long dinc;
        float amul;
        long w[2];
        long winc;
        float xmul;
        long power[2];
        long powerinc;
        long termcount;
        long internal[6];
} polylogdat;

struct threaddat {
    long threadnum;
    long junk;
    unsigned long where[2];
    unsigned long start_pos[2];                         //start of this session's execution -- not start of subrange
    unsigned long current_pos[2];
    unsigned long end_pos[2];
    double pisum[4];
    long internal[8];
    polylogdat pldat;
};

extern void __pascal POWERINIT(void);
extern void __pascal POWERFUNC(polylogdat *dat);
extern void __pascal POLYLOGCONVOUT(polylogdat *dat);

long error;
char ErrMsg[200];

CRITICAL_SECTION not_calculating;  //just use one criticalsection for all special purposes
                                   // -- under 0.01% of time should be spent in them in total
                                   //anyway.                                                                                

void pihex_dropout(void)
{
    char cbuf[256];
    char buf[256];
    long thrd;

    WritePrivateProfileString("Main","where","0000000000000000",iniFileName);
    WritePrivateProfileString("Main","srt_pos","0000000000000000",iniFileName);
    WritePrivateProfileString("Main","cur_pos","0000000000000000",iniFileName);
    WritePrivateProfileString("Main","end_pos","0000000000000000",iniFileName);

    WritePrivateProfileString("Main","rangesw","0",iniFileName);

    for(thrd=0;;thrd++) {
        sprintf(cbuf,"Thread%d",thrd);
        if(GetPrivateProfileSection(cbuf,buf,256,iniFileName)==0) break;
        buf[0]=0;buf[1]=0;
        WritePrivateProfileSection(cbuf,buf,iniFileName);
    };

    remove("range0.ini");
    remove("range1.ini");
}

long main_newrange(void)
{
    char rangeini[200];
    char cBuf[20];
    long temp;
    HANDLE hFind;
    WIN32_FIND_DATA Findtemp;

    getcwd(rangeini,200);
    strcat(rangeini,"\\range*.ini");

    hFind=FindFirstFile((LPCTSTR) &rangeini,&Findtemp);
    if(hFind==INVALID_HANDLE_VALUE) {
        request_communication(1,0);
        error=1;
        return(0);
    };

    // The file exists now
    getcwd(rangeini,200);
    strcat(rangeini,"\\");
    strcat(rangeini,Findtemp.cFileName);
    FindClose(hFind);

    GetPrivateProfileString("Main","Where","0000000000000000",&cBuf,20,rangeini);
    if(strncmp(cBuf,"0000000000000000",16)==0) // another thread read the file first
    return(1);

    sscanf(cBuf,"%08lX",&where[1]);
    sscanf(&cBuf[8],"%08lX",&where[0]);
   
    temp=progress[0];
    progress[0]-=m_end_pos[0];
    if(temp<progress[0]) progress[1]=progress[1]-m_end_pos[1]-1;               //borrow
    else progress[1]=progress[1]-m_end_pos[1];
                                                            //progress is -(the amount left to do in old range)

    GetPrivateProfileString("Main","srt_pos","0000000000000000",cBuf,20,rangeini);
    sscanf(cBuf,"%08lX",&m_start_pos[1]);
    sscanf(&cBuf[8],"%08lX",&m_start_pos[0]);
     
    m_current_pos[0]=m_start_pos[0];
    m_current_pos[1]=m_start_pos[1];

    GetPrivateProfileString("Main","end_pos","0000000000000000",cBuf,20,rangeini);
    sscanf(cBuf,"%08lX",&m_end_pos[1]);
    sscanf(&cBuf[8],"%08lX",&m_end_pos[0]);

    temp=progress[0];
    progress[0]+=m_start_pos[0];
    if(temp>progress[0]) progress[1]=progress[1]+m_start_pos[1]+1;               //carry
    else progress[1]=progress[1]+m_start_pos[1];

    sprintf(cBuf,"%08lX%08lX",where[1],where[0]);
    WritePrivateProfileString("Main","where",cBuf,iniFileName);

    sprintf(cBuf,"%08lX%08lX",m_start_pos[1],m_start_pos[0]);
    WritePrivateProfileString("Main","srt_pos",cBuf,iniFileName);

    sprintf(cBuf,"%08lX%08lX",m_current_pos[1],m_current_pos[0]);
    WritePrivateProfileString("Main","cur_pos",cBuf,iniFileName);

    sprintf(cBuf,"%08lX%08lX",m_end_pos[1],m_end_pos[0]);
    WritePrivateProfileString("Main","end_pos",cBuf,iniFileName);  //write range info into PiHex.ini

    GetPrivateProfileString("Main","rnum","0",cBuf,20,rangeini);
    WritePrivateProfileString("Main","rnum",cBuf,iniFileName);

    remove(&rangeini);                       //delete Range.ini, so we don't start this range again.

    return(1);
};
    
long __pascal calc_main_init(void)
{
    char cBuf[20];

    InitializeCriticalSection(&not_calculating);

    POWERINIT();
    
    error=0;
    overheat_err=0;

    GetPrivateProfileString("Main","where","0000000000000000",cBuf,20,iniFileName);
    sscanf(cBuf,"%08lX",&where[1]);
    sscanf(&cBuf[8],"%08lX",&where[0]);

    GetPrivateProfileString("Main","srt_pos","0000000000000000",cBuf,20,iniFileName);
    sscanf(cBuf,"%08lX",&m_start_pos[1]);
    sscanf(&cBuf[8],"%08lX",&m_start_pos[0]);

    GetPrivateProfileString("Main","cur_pos","0000000000000000",cBuf,20,iniFileName);
    sscanf(cBuf,"%08lX",&m_current_pos[1]);
    sscanf(&cBuf[8],"%08lX",&m_current_pos[0]);

    progress[0]=m_current_pos[0];
    progress[1]=m_current_pos[1];

    GetPrivateProfileString("Main","end_pos","0000000000000000",cBuf,20,iniFileName);
    sscanf(cBuf,"%08lX",&m_end_pos[1]);
    sscanf(&cBuf[8],"%08lX",&m_end_pos[0]);

    return(1);
};

extern DWORD GetHighTime(void);
extern void updatehours(float cav);
extern void tellserverhours(void);

void newrange(struct threaddat *dat)
{
    char Thname[20];
    char cBuf[20];
    unsigned long temp;
    unsigned long pisum1[4];
    char outstr[256];
    FILE *f;

    long starttime;
    long endtime;
    float idealtime;
    float cav;

    POLYLOGCONVOUT(&dat->pldat);

    pisum1[3]=dat->pisum[0];                    //convert from double to long
    pisum1[2]=dat->pisum[1];
    pisum1[1]=dat->pisum[2];
    pisum1[0]=dat->pisum[3];

    if((where[1]|where[0])!=0)
    {
        sprintf(outstr,"output,%08lX%08lX,%08lX%08lX,%08lX%08lX,%08lX%08lX%08lX%08lX\n"
        ,dat->where[1],dat->where[0],dat->start_pos[1],dat->start_pos[0]
        ,dat->current_pos[1],dat->current_pos[0]
        ,pisum1[3],pisum1[2],pisum1[1],pisum1[0]);

        spoolmsg(outstr);
    };

    sprintf(Thname,"Thread%d",dat->threadnum);

    endtime=GetHighTime();
    starttime=GetPrivateProfileInt(Thname,"stime",0,iniFileName);
    sprintf(cBuf,"%d",endtime);
    WritePrivateProfileString(Thname,"stime",cBuf,iniFileName);
    if(starttime>0) {
        cav=24./(endtime-starttime);
        GetPrivateProfileString("Main","rtime","0",cBuf,20,iniFileName);
        sscanf(cBuf,"%f",&idealtime);
        cav=cav*idealtime;
        if(idealtime>0) updatehours(cav);
    };

    temp=GetPrivateProfileInt(Thname,"lastr",0,iniFileName);
    if(temp!=0) {
            sprintf(outstr,"donerange,%d\n",temp);
            tellserverhours();
            spoolmsg(outstr);
            spoolmsg("getrange\n");
            WritePrivateProfileString(Thname,"lastr","0",iniFileName);

            rangeswaiting--;
            sprintf(cBuf,"%d",rangeswaiting);
            WritePrivateProfileString("Main","rangesw",cBuf,iniFileName);

            lastcommunicated=0;
            WritePrivateProfileString("Main","lastcommunicated","0",iniFileName);
        };


    
    dat->pisum[0]=0;
    dat->pisum[1]=0;
    dat->pisum[2]=0;
    dat->pisum[3]=0;
    
    if(m_current_pos[1]==m_end_pos[1]&&m_current_pos[0]==m_end_pos[0]) {
        if(main_newrange()==0) {
            sprintf(Thname,"Thread%d",dat->threadnum);
            sprintf(cBuf,"%08lX%08lX",dat->current_pos[1],dat->current_pos[0]);
            WritePrivateProfileString(Thname,"cur_pos",cBuf,iniFileName);
            dat->threadnum=-1;
            return;};};

    dat->where[0]=where[0];
    dat->where[1]=where[1];
    dat->current_pos[0]=m_current_pos[0];
    dat->current_pos[1]=m_current_pos[1];
    dat->start_pos[0]=m_current_pos[0];
    dat->start_pos[1]=m_current_pos[1];

    temp=m_current_pos[0];
    m_current_pos[0]=(m_current_pos[0]&(0xF0000000))+(1<<28);
    if(m_current_pos[0]<temp) m_current_pos[1]++;

    if((m_current_pos[1]>m_end_pos[1])||((m_current_pos[1]==m_end_pos[1])&&
    m_current_pos[0]>m_end_pos[0]))
    {m_current_pos[0]=m_end_pos[0];m_current_pos[1]=m_end_pos[1];};
    
    dat->end_pos[0]=m_current_pos[0];
    dat->end_pos[1]=m_current_pos[1];
    
    sprintf(Thname,"Thread%d",dat->threadnum);

    if((m_current_pos[1]==m_end_pos[1])&&(m_current_pos[0]==m_end_pos[0]))
    GetPrivateProfileString("Main","rnum","0",cBuf,20,iniFileName);
    else
    sprintf(cBuf,"0");

    WritePrivateProfileString(Thname,"lastr",cBuf,iniFileName);

    sprintf(cBuf,"%08lX%08lX",m_current_pos[1],m_current_pos[0]);
    WritePrivateProfileString("Main","cur_pos",cBuf,iniFileName);

    sprintf(cBuf,"%08lX%08lX",dat->where[1],dat->where[0]);
    WritePrivateProfileString(Thname,"where",cBuf,iniFileName);

    sprintf(cBuf,"%08lX%08lX",dat->current_pos[1],dat->current_pos[0]);
    WritePrivateProfileString(Thname,"cur_pos",cBuf,iniFileName);

    sprintf(cBuf,"%08lX%08lX",dat->end_pos[1],dat->end_pos[0]);
    WritePrivateProfileString(Thname,"end_pos",cBuf,iniFileName);
}

void __pascal calc_progress(struct threaddat *dat)
{
    unsigned long temp;
    EnterCriticalSection(&not_calculating);
    if(overheat_err) {
        sprintf(ErrMsg,"Error: CPU appears to be overheating.");error=1;
        dat->threadnum=-1;
    };

    if(rangeswaiting<1) request_communication(0,0);

    temp=progress[0];
    progress[0]+=16;
    if(progress[0]<temp) progress[1]++;         //carry
    if(dat->current_pos[0]==dat->end_pos[0]
    &&dat->current_pos[1]==dat->end_pos[1])     //if we've finished this sub-range...
    newrange(dat);                                                //get another.
    LeaveCriticalSection(&not_calculating);
}

void __pascal calc_thread_init(struct threaddat *dat)
{
    char Thname[20];
    char cBuf[20];
    unsigned long temp;
    EnterCriticalSection(&not_calculating);
    _control87(_PC_64|_RC_DOWN,_MCW_PC|_MCW_RC);        //set precision to 64 bits, rounding to down.
    sprintf(Thname,"Thread%d",dat->threadnum);
    
    GetPrivateProfileString(Thname,"where","0000000000000000",cBuf,20,iniFileName);
    sscanf(cBuf,"%08lX",&dat->where[1]);
    sscanf(&cBuf[8],"%08lX",&dat->where[0]);

    GetPrivateProfileString(Thname,"cur_pos","0000000000000000",cBuf,20,iniFileName);
    sscanf(cBuf,"%08lX",&dat->current_pos[1]);
    sscanf(&cBuf[8],"%08lX",&dat->current_pos[0]);

    GetPrivateProfileString(Thname,"end_pos","0000000000000000",cBuf,20,iniFileName);
    sscanf(cBuf,"%08lX",&dat->end_pos[1]);
    sscanf(&cBuf[8],"%08lX",&dat->end_pos[0]);

    dat->pisum[0]=0;
    dat->pisum[1]=0;
    dat->pisum[2]=0;
    dat->pisum[3]=0;
    dat->start_pos[0]=dat->current_pos[0];
    dat->start_pos[1]=dat->current_pos[1];

    temp=progress[0];                     //adjust progress to account for the fact that
    progress[0]-=dat->end_pos[0];         //this sub-range isn't finished yet.
    if(temp<progress[0]) progress[1]--;   //borrow
    progress[1]-=dat->end_pos[1];

    temp=progress[0];
    progress[0]+=dat->current_pos[0];
    if(temp>progress[0]) progress[1]++;   //carry
    progress[1]+=dat->current_pos[1];

    LeaveCriticalSection(&not_calculating);
}

long calc_error;
char calc_Status[200];
char calc_Tip[200];
long oldprogress=0;
long oldtcount;
extern void calcfracdone(void);
void __pascal calc_main_status(void)
{
    WIN32_FIND_DATA temp;
    HANDLE hFind;
    char outputname[20];
    char cBuf[20];
    float tempf;
    long templ;

    calcfracdone();

    templ=GetTickCount();
    if(oldprogress!=0)
    {
        tempf=progress[0]-oldprogress;
        tempf=tempf*numclocks/1000./(templ-oldtcount);
        sprintf(calc_Status,"%f%% done,%fMHz",fracdone,tempf);
    } else
    {
        sprintf(calc_Status,"%f%% done",fracdone);
    };
    oldtcount=templ;
    oldprogress=progress[0];
    sprintf(calc_Tip,"PiHex %f%% done",fracdone);

    if(error) {sprintf(calc_Status,"%s",ErrMsg);calc_error=1;return;};
    calc_error=0;
};

void __pascal calc_thread_done(struct threaddat *dat)
{
    char Thname[20];
    char cBuf[120];
    unsigned long pisum1[4];
    FILE *f;
    char outstr[256];

    if(dat->threadnum==-1) return;

    EnterCriticalSection(&not_calculating);

    POLYLOGCONVOUT(&dat->pldat);

    pisum1[3]=dat->pisum[0];
    pisum1[2]=dat->pisum[1];
    pisum1[1]=dat->pisum[2];
    pisum1[0]=dat->pisum[3];


    if((where[1]|where[0])!=0)
    {
        sprintf(outstr,"output,%08lX%08lX,%08lX%08lX,%08lX%08lX,%08lX%08lX%08lX%08lX\n"
        ,dat->where[1],dat->where[0],dat->start_pos[1],dat->start_pos[0]
        ,dat->current_pos[1],dat->current_pos[0]
        ,pisum1[3],pisum1[2],pisum1[1],pisum1[0]);

        spoolmsg(outstr);
    };

    sprintf(Thname,"Thread%d",dat->threadnum);

    sprintf(cBuf,"%08lX%08lX",dat->current_pos[1],dat->current_pos[0]);
    WritePrivateProfileString(Thname,"cur_pos",cBuf,iniFileName);

    LeaveCriticalSection(&not_calculating);
};

void __pascal calc_main_done(void)
{
    char cBuf[20];

    sprintf(cBuf,"%d",rangeswaiting);
    WritePrivateProfileString("Main","rangesw",cBuf,iniFileName);

    DeleteCriticalSection(&not_calculating);
}

