#define MAXTHREADS         256
#define ThreadDataSize       768

#ifdef DEBUG
#define sAppClass                "PiHexDbgClass"
#define sAppName                "PiHex Beta"
#define sIniName                   "Pihexdbg.ini"
#define sotherIniName                   "Pihexdbg.ini"
#else
#define sotherIniName                   "Pihexdbg.ini"
#define sIniName                   "Pihex.ini"
#define sAppClass                "PiHexClass 6.00"
#define sAppName                "PiHex 6.00"
#endif
