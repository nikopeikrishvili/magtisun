/*-----------------------------------------------------
Name: info.c
Date: 2015.05.03
Auth: kala13x (a.k.a 7th Ghost)
Desc: Get and print additional information
-----------------------------------------------------*/


#include "stdinc.h"
#include "info.h"


/*---------------------------------------------
| Get application version
---------------------------------------------*/
const char* get_version()
{
    static char verstr[128];
    sprintf(verstr, "%s Build %d (%s)", VERSION, BUILD, __DATE__);
    return verstr;
}


/*---------------------------------------------
| Greet users
---------------------------------------------*/
void greet(void) 
{
    printf("====================================================\n");
    printf("MagtiSun-SMS: %s\n", get_version());
    printf("====================================================\n");
} 


/*---------------------------------------------
| Print usage
---------------------------------------------*/
void usage(void) 
{
    printf("Usage: magtisun [-l] [-o] [-i] [-h]\n");
    printf("options are:\n");
    printf("   -l                  # do login\n");
    printf("   -o                  # logout from app\n");
    printf("   -i                  # get and print user information\n");
    printf("   -h                  # prints version and usage\n");
    printf("\n");
}
