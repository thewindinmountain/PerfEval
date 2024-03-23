#include<stdio.h>
#include <time.h>

#define ONE         a += 1;
#define TWO         ONE ONE
#define FOUR        TWO TWO
#define TEN         FOUR FOUR TWO
#define HUNDRED     TEN TEN TEN TEN TEN TEN TEN TEN TEN TEN 
#define THOUSAND    HUNDRED HUNDRED HUNDRED HUNDRED HUNDRED HUNDRED HUNDRED HUNDRED HUNDRED HUNDRED
#define TENTHOUSAND THOUSAND THOUSAND THOUSAND THOUSAND THOUSAND THOUSAND THOUSAND THOUSAND THOUSAND THOUSAND 

int main()
{
    struct timespec begin,end;
    register long long a = 0;
    register int i = 1000000;
    clock_gettime(CLOCK_REALTIME,&begin);
    do{
        TENTHOUSAND
        i--;
    } while (i);
    clock_gettime(CLOCK_REALTIME,&end);
    printf("%f\n", (10000000000)  / (end.tv_sec - begin.tv_sec + (end.tv_nsec - begin.tv_nsec)/1.0e9) / 1000000000);
    return a == 0;
}

