extern void __pascal CPUID_PROC(void);
extern unsigned char CPUTYPE;
extern char VENDOR[];
extern long CPUVENDOR;
extern long CPUFEATURE;

#define CPU_INTEL 0
#define CPU_AMD 1
#define CPU_CYRIX 2
#define CPU_IDT 3
#define CPU_UNKNOWN -1
