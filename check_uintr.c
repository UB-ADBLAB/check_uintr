#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <cpuid.h>

int main(int argc, char *argv[]) { 
  unsigned int eax, ebx, ecx, edx;

  __get_cpuid_count(0, 0, &eax, &ebx, &ecx, &edx);
  if (eax < 10) {
    printf("no uintr support: cpuid leaf 0 indicates maximum "
           "input value for CPUID is 0x%08x < \n", eax);
    return 1;
  }

  char s[13];
  *(unsigned*)&s[0] = eax;
  *(unsigned*)&s[4] = ecx;
  *(unsigned*)&s[8] = ebx;
  s[12] = '\0';
  if (!strcmp(s, "GenuineIntel")) {
    printf("no uintr support: vendor identifier is not GeniuneIntel: \"%s\"\n", s);
    return 1;
  }

  __get_cpuid_count(7, 0, &eax, &ebx, &ecx, &edx);
  if (!(edx & 0x20)) {
    printf("no uintr support: cpuid leaf 7, subleaf 0, EDX bit 5 is zero\n");
    return 1;
  }

  printf("found uintr support\n");
  
  bool warn = false;
  __get_cpuid_count(0xd, 1u, &eax, &ebx, &ecx, &edx);
  if (!(eax & 0xb)) {
    printf("warning: no xsave support: cpuid leaf 0xd, subleaf 1, eax bits 0, 1, 3 are all zero\n");
    warn = true;
  }

  if (!(ecx & 0x4000)) {
    printf("warning: no xsave support for uintr: cpuid leaf 0xd, subleaf 1, ecx bit 14 is zero\n");
    warn = true;
  }

  if (warn) {
    printf("warning: the patched kernel will not work without xsave support for uintr.\n");
    printf("         please consider using the uintr device driver linked through our codebase\n");
  } else {
	printf("OK: the patched kernel should work\n");
  }

  return (warn) ? 2 : 0;
}
