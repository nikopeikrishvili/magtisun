MagtiSun - Version: 0.0.2 Beta BUILD 4
----

MagtiSun is MagtiFun Library and SMS Sender for CLI Applications. Softwear is written for educational purposes and is distributed in the hope that it will be useful for anyone interested in this field.

###Usage
```
magtisun [-u <username>] [-p <password>] [-i] [-h]
options are:
 -u <username>       # magtifun user name
 -p <password>       # magtifun password
 -i                  # get and print user information
 -h                  # prints version and usage\n");
```

###Compilation
Compilation of this project is possible with Makefile (just type 'make' in terminal).

###Dependencies
Make sure you have [curl development library](https://github.com/bagder/curl) installed on your computer to compile magtisun.

###Todo
- add invisible password input
- get and print user information

###Simple CLI interface
Here is an example output of MagtiSun usage:
```
[kala@build-box magtisun]$ ./magtisun
====================================================
MagtiSun-SMS: 0.0.2 Beta Build 4 (May  4 2015)
====================================================
2015.05.04-00:56:47 - [WARN] Username and/or Password is not given
2015.05.04-00:56:47 - [INFO] Check usage with parameter -h
2015.05.04-00:56:47 - [INPUT] Enter Username: kala13x
2015.05.04-00:57:04 - [INPUT] Enter Password: ********
2015.05.04-00:57:07 - [INPUT] Enter Number: 598998592
2015.05.04-00:57:12 - [INPUT] Enter Text: dzudzu
2015.05.04-00:57:16 - [LIVE] Sending message...
2015.05.04-00:57:17 - [LIVE] Message sent
```

###Get more info from blog
- [http://off-sec.com/](http://off-sec.com)
