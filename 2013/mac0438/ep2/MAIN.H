#define MENU_START              101
#define MENU_STOP               102
#define MENU_EXIT               103
#define MENU_TRAY               104
#define MENU_AUTOCONT           105
#define MENU_PRIORITY           106
#define MENU_ABOUT              107
#define MENU_QUIT               108
#define MENU_COMMUNICATE        109
#define MENU_CPUINFO            110
#define MENU_JOIN               111
#define MENU_COMMSTOP           112
#define MENU_SERVICE            113
#define MENU_NOTRAY             114

#define PR_1                    101
#define PR_2                    102
#define PR_3                    103
#define PR_4                    104
#define PR_5                    105
#define PR_AUTO                 106

#define UNAME_Edit              101
#define EMAIL_Edit              102
#define CT_I4                   103
#define CT_I5                   104
#define CT_I6                   105
#define CT_C6                   106
#define CT_A5                   107
#define CT_A6                   108
#define CSPEED_Edit             109
#define CNUM_Edit               110
#define CM_NET                  111
#define CM_DIAL                 112
#define CM_SNEAK                113
#define PROXY_Check             114
#define PROXY_Config            115
#define AUTOCOM_Check           116
#define CHOURS_Edit             117
#define CDAYS_Edit              118

#define PROXY_Edit              101
#define PORT_Edit               102

#define WM_TRAY                 101

#define tStatus                 1
#define tSample                 2
#define tTray                   3

#define AppIcon                 1
#define BlackIcon               2

#define AppMenu                 1

#define AboutBox                101
#define PriorityBox             102
#define CPUINFOBOX              103
#define PROXYINFOBOX            104

#define NumLines        64
#define LineSize        128

#define _EXPORT __export

#include <windows.h>
#include <winbase.h>
#include <stdio.h>
#include <direct.h>
#include <malloc.h>
#include <process.h>
#include <io.h>
#include <winsock.h>
#include <ras.h>

extern long __pascal calc_main_init(void);
// This is called whenever Pihex/start calculating is selected from the
// menu, and also upon startup if auto-continue is selected.
// It should preform non-thread-specific initialization.
// Returns: non-zero if it supports multiple calculating threads
//          zero otherwise.

extern void __pascal calc_main_status(void);
extern char calc_Status[];
extern char calc_Tip[];
// calc_main_status is called approximately once every second.
// It should write into calc_Status and calc_Tip.

extern long calc_error;
// When calc_error is 1, the tray icon (if it exists) and the title bar
// will flash -- this gets the user's attention and gets them to deal with
// a problem.

extern void __pascal calc_main_done(void);
// This is called whenever either Pihex/stop calulating is selected from
// the menu or all of the calculating threads finish.
// calc_main_done should perform non-thread-specific shutdown.

extern void __pascal calc_thread_init(LPVOID dat);
extern void __pascal calc_thread_dowork(LPVOID dat);
extern void __pascal calc_thread_done(LPVOID dat);
// These functions get passed the location of a region of memory of size
// ThreadDatSize. The first doubleword serves two purposes: first, it
// contains the number of the thread, starting with 0. Second, if it is
// ever set to -1 (FFFFFFFF) when calc_thread_init or calc_thread_dowork
// return, calc_thread_done is called, and then the thread is halted.
// The only other circumstance under which the loop which calls
// calc_thread_dowork will end is when Pihex/stop calculating is selected
// from the menu.

extern void pihex_dropout(void);

extern void spoolmsg(char *dat);
// Provided by main.c;  writes a message to the spool file.

extern void request_communication(long startcomputing, long forceattempt);
// If this function is called, main.c will communicate ASAP.

#ifdef Sample
extern char Samplefn[];
#endif
// If the code is to be sampled, Samplefn must be defined.
// This will only sample the first thread (thread 0), although it shouldn't
// matter, since all the calculating threads are identical.

#ifdef times
extern char timesfn[];
#endif

extern char iniName[];
//The name of the .ini file.

extern char otheriniName[];

