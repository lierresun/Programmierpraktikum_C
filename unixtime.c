#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define ERR() ({ fprintf(stderr, "Usage: %s <unix_time> <time_zone (opt.)>\n\r", argv[0]); exit(1); })

int is_leap(int y) { return (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0)); }

int main(int argc, char* argv[]) {
    const char* wdays[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
    int mdays[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    unsigned int sec = 0, min = 0, hour = 0; 
    unsigned int weekday = 0, day = 1, month = 0, year = 1970, temp = 0;
    
    double timezone = 1.0;
    unsigned long long int t = 1761851730;
    
    if (argc > 3) { ERR(); }
    if (argc >= 2 && (sscanf(argv[1], "%llu", &t) != 1 || t < 0)) { ERR(); }
    if (argc == 3 && (sscanf(argv[2], "%lf", &timezone) != 1)) { ERR(); }
    t += (unsigned long long int)(timezone * 3600.0);
    

    //!TODO
    sec   = t % 60ULL;  t /= 60ULL;
    min   = t % 60ULL;  t /= 60ULL;
    hour  = t % 24ULL;  t /= 24ULL;   

    weekday = (unsigned int)t;


    while (true) {
    unsigned int days_in_year = is_leap(year) ? 366U : 365U;
    if (t < days_in_year) break;
    t -= days_in_year;
    year++;
    }


    mdays[1] = is_leap(year) ? 29 : 28;
    month = 0;
    while (t >= (unsigned long long)mdays[month]) {
    t -= (unsigned long long)mdays[month];
    month++;
    }
    day = (unsigned int)t + 1;


    printf("%02d.%02d.%d; %s; %02d:%02d:%02d\n\r", 
           day, month + 1, year, wdays[(weekday + 4) % 7], hour, min, sec);

    return 0;
}
