#include <stdio.h>
#include <sys/time.h>   // getlocaltime
#include <time.h>       // localtime

int main(int argc, char* argv[]) {
    struct timeval getlocaltime_t;
    struct tm *local_time_t;
    char *buffer = new char[80];

    while(true) {
        gettimeofday(&getlocaltime_t, NULL);
        // printf("\tgettimeofday: %ld:%ld.\n", getlocaltime_t.tv_sec, getlocaltime_t.tv_usec);

        local_time_t = localtime(&getlocaltime_t.tv_sec);
        // printf("\tlocaltime: %s", asctime(local_time_t));

        strftime(buffer, 80, "%X.", local_time_t);
        printf("\tstrtime: %s%ld\n", buffer, getlocaltime_t.tv_usec);
    }


    return 0;
}
