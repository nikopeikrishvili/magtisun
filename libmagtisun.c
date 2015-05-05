/*---------------------------------------------------------------------------

 MagtiSun is commandline application which makes you able to send sms
 with terminal via magtifun sms api. You also can implement libmagtisun 
 in another cli applications to send sms with terminal.

 Copyright (c) 2015 Sun Dro (a.k.a. 7th Ghost)
 Web: http://off-sec.com/ ; E-Mail: kala0x13@gmail.com

 This is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.

 This software is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

---------------------------------------------------------------------------*/


/* C includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

/* Local includes */
#include "libmagtisun.h"

/* Dependency includes */
#include <curl/curl.h>


/*---------------------------------------------
| Get library version
---------------------------------------------*/
const char* msl_get_version()
{
    static char verstr[128];
    sprintf(verstr, "%s Build %d (%s)", MSLVERSION, MSLBUILD, __DATE__);
    return verstr;
}


/*---------------------------------------------
| Initialise magtisun login variables
---------------------------------------------*/
void msl_init(MagtiSunLib* msl)
{
    /* Used variables */
    FILE* fp;
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    char* tmp;

    /* Clear values */
    bzero(msl->usr, sizeof(msl->usr));
    bzero(msl->pwd, sizeof(msl->pwd));
    bzero(msl->num, sizeof(msl->num));
    bzero(msl->txt, sizeof(msl->txt));
    msl->login = 0;
    msl->info = 0;
    msl->logged = 0;

    /* Open templorary file */
    fp = fopen(LOGIN_FILE, "r");
    if (fp != NULL) 
    {
        /* Get status */
        while ((read = getline(&line, &len, fp)) != -1) 
        {
            /* Find user in file */
            if(strstr(line, "user") != NULL) 
            {
                /* Get user info */
                tmp = strtok(line, ":");
                strcpy(msl->usr, tmp);
                tmp = strtok(NULL, ":");
                strcpy(msl->pwd, tmp);
                msl->logged = 1;
            }
        }
        fclose(fp);
    }
}


/*---------------------------------------------
| Read login information
---------------------------------------------*/
void msl_cli_init(MagtiSunLib* msl) 
{
    /* Get username */
    printf("[INPUT] Enter Username: ");
    scanf("%s", msl->usr);

    /* Get password (invisible) */
    char* pwd = getpass("[INPUT] Enter Password: ");
    strcpy(msl->pwd, pwd);
}


/*---------------------------------------------
| Read sms information
---------------------------------------------*/
void msl_init_sms(MagtiSunLib* msl)
{
    /* Get number */
    printf("[INPUT] Enter Number: ");
    scanf("%s", msl->num);

    /* Get sms text */
    printf("[INPUT] Enter Text: ");
    scanf("%s", msl->txt);
}


/*---------------------------------------------
| Write function callback
---------------------------------------------*/
size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    /* Write in file */
    return fwrite(ptr, size, nmemb, stream);
}


/*---------------------------------------------
| Check status in response
---------------------------------------------*/
int msl_check_status(char *fname) 
{
    /* Used variables */
    FILE* fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    int ret = -1;

    /* Open templorary file */
    fp = fopen(fname, "r");
    if (fp != NULL) 
    {
        /* Get status */
        while ((read = getline(&line, &len, fp)) != -1) 
        {
            /* Find success str in response */
            if(strstr(line, "success") != NULL) 
            {
                ret = 1;
                break;
            }
        }
        fclose(fp);
    }

    return ret;
}


/*---------------------------------------------
| Get information about user
---------------------------------------------*/
int msl_get_info(MagtiSunLib* msl) 
{
    printf("[TODO] Get infotmation is added to TODO list\n");
    exit(0);
}


/*---------------------------------------------
| Remove templorary file for login session
---------------------------------------------*/
void msl_logout() 
{
    remove(LOGIN_FILE);
    exit(1);
}


/*---------------------------------------------
| Create templorary file for login session
---------------------------------------------*/
int msl_login(MagtiSunLib* msl) 
{
    /* Remove existing fole */
    remove(LOGIN_FILE);

    /* User input info */
    msl_cli_init(msl);

    /* Open file pointer */
    FILE *fp = fopen(LOGIN_FILE, "a");
    if (fp == NULL) return 0;

    /* Write key in file */
    fprintf(fp, "%s:%s:user", msl->usr, msl->pwd);

    /* Close file and return */
    fclose(fp);
    return 1;
}

/*---------------------------------------------
| Authorise and send sms
---------------------------------------------*/
int msl_send(MagtiSunLib* msl) 
{
    /* Used variables */
    FILE *fp;
    CURL *curl;
    CURLcode res;
    char val[512];
    char url[128];
    int ret = -1;

    /* Initialise curl */
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    /* Check curl */
    if (curl) 
    {
        /* Open output fileponter */
        fp = fopen(DISCARD_FILE, "w");
        if (fp != NULL) 
        {
            /* Get ready for login */
            strcpy(url, "http://www.magtifun.ge/index.php?page=11&lang=ge");
            sprintf(val, "user=%s&password=%s", msl->usr, msl->pwd);

            /* Set curl options for login */
            curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
            curl_easy_setopt(curl, CURLOPT_URL, &url);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, &val);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(val));
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);
            curl_easy_setopt(curl, CURLOPT_POST, 1);
            curl_easy_setopt(curl, CURLOPT_COOKIESESSION, 1);
            curl_easy_setopt(curl, CURLOPT_COOKIEJAR, COOCKIE_LOGIN);
            curl_easy_setopt(curl, CURLOPT_COOKIEFILE, COOCKIE_FILE);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

            /* Send post request to magtifun */
            res = curl_easy_perform(curl);
            fclose(fp);

            /* Open response fileponter */
            if (res == CURLE_OK) fp = fopen(RESPONSE_FILE, "w");
            if (fp != NULL) 
            {
                /* Get ready for send */
                strcpy(url, "http://www.magtifun.ge/scripts/sms_send.php");
                sprintf(val, "recipients=%s&message_body=%s", msl->num, msl->txt);

                /* Set curl options for send */
                curl_easy_setopt(curl, CURLOPT_URL, &url);
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, &val);
                curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(val));
                curl_easy_setopt(curl, CURLOPT_COOKIESESSION, 1);
                curl_easy_setopt(curl, CURLOPT_COOKIEJAR, COOCKIE_SEND);
                curl_easy_setopt(curl, CURLOPT_COOKIEFILE, COOCKIE_FILE);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

                /* Send post request to magtifun */
                res = curl_easy_perform(curl);
                fclose(fp);

                /* Check everything is ok */
                if(res == CURLE_OK) 
                    ret = msl_check_status(RESPONSE_FILE);
            }

            /* Cleanup */
            curl_easy_cleanup(curl);
            remove(RESPONSE_FILE);
            remove(COOCKIE_LOGIN);
            remove(COOCKIE_SEND);
        }
    }

    /* Global cleanup curl */
    curl_global_cleanup();

    return ret;
}