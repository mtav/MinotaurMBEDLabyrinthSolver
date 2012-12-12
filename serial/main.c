
/**************************************************

file: main.c
purpose: simple demo that receives characters from
the serial port and print them on the screen

**************************************************/

#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "rs232.h"

//~ #include <iostream>
//~ #include <string>
//~ #include <sstream>
//~ 
//~ using namespace std;

int main()
{
  int i, n,
      cport_nr=16,        /* /dev/ttyS0 (COM1 on windows) */
      bdrate=9600;       /* 9600 baud */
//      bdrate=115200;       /* 9600 baud */

  unsigned char buf[10000];


  if(OpenComport(cport_nr, bdrate))
  {
    printf("Can not open comport\n");

    return(0);
  }

	//~ string input = "";
 
	//~ while(1)
	//~ {
	 //~ cout << "Please enter a valid sentence (with spaces):\n>";
	 //~ getline(cin, input);
	 //~ cout << "You entered: " << input << endl << endl;
	//~ cprintf(cport_nr, input.c_str());
	 //~ }

//	cprintf(cport_nr, "hello world of something\n");
 
  while(1)
  {
    n = PollComport(cport_nr, buf, 10000-1);

    if(n > 0)
    {
      buf[n] = 0;   /* always put a "null" at the end of a string! */

      for(i=0; i < n; i++)
      {
        if(buf[i] < 32)  /* replace unreadable control-codes by dots */
        {
          buf[i] = '\n';
        }
      }

      printf("%s\n\n", (char *)buf);
    }

#ifdef _WIN32
    Sleep(100);  /* it's ugly to use a sleeptimer, in a real program, change the while-loop into a (interrupt) timerroutine */
#else
    usleep(100000);  /* sleep for 100 milliSeconds */
#endif
  }

  return(0);
}
