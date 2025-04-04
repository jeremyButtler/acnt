/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' mainAcnt SOF: Start Of File
'   - driver for acnt
'   o header:
'     - included libraries and line break variable
'   o fun01: pversion_mainAcnt
'     - prints version number for acnt
'   o fun02: phelp_mainAcnt
'     - prints help messager for acnt
'   o fun03: input_mainAcnt
'     - get user input provided with command line call
'   o main:
'     - driver function for account
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - included libraries and line break variable
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
   #include <time.h>
#endif

#include <stdio.h>

#include "genLib/endLine.h"
#include "genLib/base10str.h"
#include "genLib/ulCp.h"
#include "acnt.h"
#include "cmdAcnt.h"

/*.h files only*/
#include "defsAcnt.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidden Libraries:
!   - std #include <time.h>
!   - .c  #include "gentLib/numToStr.h"
!   - .c  #include "genLib/strAry.h"
!   - .c  #include "genLib/prtAry.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#define def_delCmd_mainAcnt 1
#define def_addCmd_mainAcnt 2
#define def_divyCmd_mainAcnt 4
#define def_pEntriesCmd_mainAcnt 8
#define def_sumCmd_mainAcnt 16
#define def_promptCmd_mainAcnt 32
#define def_mockCmd_mainAcnt 64

#define def_stdinBuff_mainAcnt 1024

/*-------------------------------------------------------\
| Fun01: pversion_mainAcnt
|   - prints version number for acnt
| Input:
|   - outFILE:
|     o FILE poitner to file to print version number to
| Output:
|   - Prints:
|     o version number to outFILE
\-------------------------------------------------------*/
void
pversion_mainAcnt(
   void *outFILE
){
   fprintf(
      (FILE *) outFILE,
      "acnt version: %i-%02i-%02i%s",
      def_year_defsAcnt,
      def_month_defsAcnt,
      def_day_defsAcnt,
      str_endLine
   );
} /*pverison_mainAcnt*/

/*-------------------------------------------------------\
| Fun02: phelp_mainAcnt
|   - prints help messager for acnt
| Input:
|   - outFILE:
|     o FILE poitner to file to print help message to
| Output:
|   - Prints:
|     o help message to outFILE
\-------------------------------------------------------*/
void
phelp_mainAcnt(
   void *outFILE
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - prints help messager for acnt
   '   o fun02 sec02:
   '     - usage entry
   '   o fun02 sec03:
   '     - input entry
   '   o fun02 sec04:
   '     - output entry
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - variable declarations and general setup
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed short yearSS = 0;
   signed char monthSC = 0;
   signed char daySC = 0;

   time_t rawTime;
   struct tm *timeSTPtr;

   time(&rawTime);
   timeSTPtr = localtime(&rawTime);
   yearSS = (signed short) timeSTPtr->tm_year + 1900;
   monthSC = (signed short) timeSTPtr->tm_mon + 1;
   daySC = (signed short) timeSTPtr->tm_mday;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - usage entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "acnt -file account.tsv <command type> <options>%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "  - runs commands on account.tsv%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - input entry
   ^   o fun02 sec03 sub01:
   ^     - input block header
   ^   o fun02 sec03 sub02:
   ^     - file IO
   *   o fun02 sec03 sub03:
   *     - commands
   *   o fun02 sec03 sub04:
   *     - input values
   *   o fun02 sec03 sub05:
   *     - help and version
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec03 Sub01:
   *   - input block header
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "Input:%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec03 Sub02:
   *   - file IO
   *   o fun02 sec03 sub02 cat01:
   *     - account file + file io header
   *   o fun02 sec03 sub02 cat02:
   *     - output file
   *   o fun02 sec03 sub02 cat04:
   *     - divy file
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec03 Sub02 cat01:
   +   - account file + file io header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  file io:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    -file account.tsv: [Required]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o account file to work on%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec03 Sub02 cat02:
   +   - output file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -out file.tsv: [Optional; stdout]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o file to save account (-no-pretend) to%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec03 Sub02 cat03:
   +   - print output file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -p-out print-output.tsv: [Optional; stdout]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o output file for -print and -sum%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o -pretend is also printed to this file%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec03 Sub02 cat04:
   +   - divy file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -divy-file divyFile.tsv: [Optional; none]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o divy file to divide accounts by%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec03 Sub03:
   *   - commands
   *   o fun02 sec03 sub03 cat01:
   *     - commands header
   *   o fun02 sec03 sub03 cat02:
   *     - add command 
   *   o fun02 sec03 sub03 cat03:
   *     - delete command
   *   o fun02 sec03 sub03 cat04:
   *     - divy command
   *   o fun02 sec03 sub03 cat05:
   *     - sum command
   *   o fun02 sec03 sub03 cat06:
   *     - print entries command
   *   o fun02 sec03 sub03 cat07:
   *     - interactive command
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec03 Sub03 Cat01:
   +   - commands header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  commands: [default -sum]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    - commands to apply to accounts:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    - can do a single add, del, or print command"
   );

   fprintf(
      (FILE *) outFILE,
      "%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    - prompt/sum commands run after add,del,print"
   );

   fprintf(
      (FILE *) outFILE,
      "%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec03 Sub03 Cat02:
   +   - add command
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -add amount: [Optional; NA]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o add entry with amount to account file%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o postiive = increase,negative = decrease%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec03 Sub03 Cat03:
   +   - delete command
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -del index: [Optional; NA]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o delete entry in -file at index%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec03 Sub03 Cat04:
   +   - divy command
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -divy amount: [Optional; amount]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o divide funds using -divy-file divy.tsv%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o postiive = increase,negative = decrease%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec03 Sub03 Cat05:
   +   - sum command
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -sum: [Optional; NA]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o print totals for all accounts%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec03 Sub03 Cat06:
   +   - print entries command
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -print: [Optional; NA]%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o print accounts by account values%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec03 Sub03 Cat07:
   +   - interactive command
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_prompt_defsAcnt)
      fprintf(
         (FILE *) outFILE, "    -prompt: [Yes]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -prompt: [No]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "      o run interactive (prompt) session%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o disable with \"-no-prompt\"%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec03 Sub04:
   *   - input values
   *   o fun02 sec03 sub04 cat01:
   *     - parent account + input values header
   *   o fun02 sec03 sub04 cat02:
   *     - child account
   *   o fun02 sec03 sub04 cat03:
   *     - amount
   *   o fun02 sec03 sub04 cat04:
   *     - year
   *   o fun02 sec03 sub04 cat05:
   *     - month
   *   o fun02 sec03 sub04 cat06:
   *     - day
   *   o fun02 sec03 sub04 cat07:
   *     - comment
   *   o fun02 sec03 sub04 cat08:
   *     - ghost
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec03 Sub04 Cat01:
   +   - parent account + input values header
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "  command input:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "    - input for account commands:%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -par \"account\":%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o parent account to target (-add, -print)%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec03 Sub04 Cat02:
   +   - child account
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -child \"account\":%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o child account to target (-add, -print)%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec03 Sub04 Cat03:
   +   - amount
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -amount \"account\":%s",
      str_endLine
   );

   fprintf(
     (FILE *) outFILE,
     "      o transaction amount (-add or -divy)%s",
     str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o postiive = increase,negative = decrease%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec03 Sub04 Cat04:
   +   - year
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -year %i:%s",
      yearSS,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o year of transaction (-add)%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -start-year %i:%s",
      yearSS,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o starting year to search for (-print)%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -end-year %i:%s",
      yearSS,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o ending year to search for (-print)%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec03 Sub04 Cat05:
   +   - month
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -month %i:%s",
      monthSC,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o month (as number) of transaction (-add)%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -start-month %i:%s",
      monthSC,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o starting month to search for (-print)%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -end-month %i:%s",
      monthSC,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o ending month to search for (-print)%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec03 Sub04 Cat06:
   +   - day
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -day %i:%s",
      daySC,
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o day of month of transaction (-add)%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -start-day all:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o starting day to search for (-print)%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "    -end-day all:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o ending day to search for (-print)%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec03 Sub04 Cat07:
   +   - comment
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   fprintf(
      (FILE *) outFILE,
      "    -note:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o comment (note) to add (-add, -divy)%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec03 Sub04 Cat08:
   +   - ghost
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_ghost_defsAcnt)
      fprintf(
         (FILE *) outFILE,
         "    -ghost: [Yes]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -ghost: [No]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "      o round positive transcations down and%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        negative trasancations up%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o amount lost in rounding is save to a%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "        ghost account%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o disable with \"-no-ghost\"%s",
      str_endLine
   );


   fprintf(
      (FILE *) outFILE,
      "        (-add, -print, -divy)%s",
      str_endLine
   );

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun02 Sec03 Sub04 Cat09:
   +   - mock
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(def_mock_defsAcnt)
      fprintf(
         (FILE *) outFILE,
         "    -pretend: [Yes]%s",
         str_endLine
      );
   else
      fprintf(
         (FILE *) outFILE,
         "    -pretend: [No]%s",
         str_endLine
      );

   fprintf(
      (FILE *) outFILE,
      "      o run commands, but do not apply results%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "      o disable with \"-no-pretend\"%s",
      str_endLine
   );

   /*****************************************************\
   * Fun02 Sec03 Sub05:
   *   - help and version
   \*****************************************************/

   fprintf(
      (FILE *) outFILE,
      "  -h: print this help message and exit%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  -v: print version number and exit%s",
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec04:
   ^   - output
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   fprintf(
      (FILE *) outFILE,
      "Output:%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - prints output to -out if non-prompt mode%s",
      str_endLine
   );

   fprintf(
      (FILE *) outFILE,
      "  - for prompt; runs input commands%s",
      str_endLine
   );
} /*phelp_mainAcnt*/

/*-------------------------------------------------------\
| Fun03: input_mainAcnt
|   - get user input provided with command line call
| Input:
|   - numArgSI:
|     o number arguments user input
|   - argAryStr:
|     o arguments input
|   - acntFileStrPtr,
|     o c-string pointer to point to account file path
|   - outFileStrPtr, 
|     o c-stirng pointer to point to output file path
|   - printFileStrPtr:
|     o c-stirng pointer to point to output file for
|       search, and sum printing
|   - divyFileStrPtr,
|     o c-string pointer to point to divy file path
|   - cmdFlagsSCPtr,   
|     o signed char to pointer to hold command to run
|       * def_addCmd_mainAcnt to do addition command
|       * def_delCmd_mainAcnt to do deletion command
|       * def_divyCmd_mainAcnt to do divy command
|       * def_pEntriesCmd_mainAcnt to print entries
|       * def_sumCmd_mainAcnt to print sums
|       * def_promptCmd_mainAcnt to run prompt session
|   - parStrPtr,     
|     o c-stirng pointer to piont to parent account name
|   - childStrPtr,   
|     o c-stirng pointer to piont to child account name
|   - addAmountFPtr,           
|     o float pointer to get amount of add transaction
|   - divyAmountFPtr,           
|     o float pointer to get amount of divy transaction
|   - yearSSPtr,     
|     o signed short ponter to get year
|   - monthSCPtr,     
|     o signed char ponter to get month
|   - daySCPtr,       
|     o signed char ponter to get day of transaction
|   - commentStrPtr, 
|     o c-string pointer to point to comment
|   - ghostBlPtr      
|     o signed char pointer to tell if using ghost account
|       * 1: use ghost account
|       * 9: do not use ghost account
|   - yearArySS:
|     o 2 element short array with start and end year
|   - monthArySC:
|     o 2 elmement char array with start and end month
|   - monthArySS:
|     o 2 elmement char array with start and end day
|   - indexSLPtr:
|     o signed long pointer to get index to delete
| Output:
|   - Prints:
|     o if requested; help or version to stdout
|     o errors to sdtderr
|   - Returns:
|     o 0 for no errors
|     o 1 if printed help message or version number
|     o 2 if had error
\-------------------------------------------------------*/
signed char
input_mainAcnt(
   int numArgsSI,               /*number arguments input*/
   char *argAryStr[],           /*user input*/
   signed char **acntFileStrPtr,/*gets account file*/
   signed char **outFileStrPtr, /*gets output file*/
   signed char **divyFileStrPtr,/*gets divy file*/
   signed char **printFileStrPtr, /*search/sum print*/
   signed char *cmdFlagsSCPtr,   /*holds command to run*/
   signed char **parStrPtr,     /*parent account*/
   signed char **childStrPtr,   /*child account*/
   float *addAmountFPtr,        /*amount to apply*/
   float *divyAmountFPtr,       /*amount to apply*/
   signed short *yearSSPtr,     /*year to use*/
   signed char *monthSCPtr,     /*month to use*/
   signed char *daySCPtr,       /*day to use*/
   signed char **commentStrPtr, /*gets comment*/
   signed char *ghostBlPtr,     /*1: use ghost account*/
   signed short yearArySS[],    /*year print start/end*/
   signed char monthArySC[],    /*month print start/end*/
   signed char dayArySC[],      /*day print start/end*/
   signed long *indexSLPtr      /*index for delete*/
){ /*****************************************************\
   ' Fun03 TOC:
   '   - get user input provided with command line call
   '   o fun03 sec01:
   '     - variable declarations and input check 
   '   o fun03 sec02:
   '     - get user input
   '   o fun03 sec03:
   '     - return result
   \*****************************************************/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations and input check 
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0;
   signed int siArg = 1;
   signed char *tmpStr = 0;
   signed char sumDefBl = 1;

   if(numArgsSI <= 1)
      goto phelp_fun03_sec03;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - get user input
   ^   o fun03 sec02 Sub01:
   ^     - file io + start loop
   ^   o fun03 sec02 sub02:
   ^     - commands
   ^   o fun03 sec02 sub03:
   ^     - input for commands
   ^   o fun03 sec02 sub04:
   ^     - help message
   ^   o fun03 sec02 sub05:
   ^     - version number
   ^   o fun03 sec02 sub06:
   ^     - unkown input
   ^   o fun03 sec02 sub07:
   ^     - move to next argument
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec02 Sub01:
   *   - file io + start loop
   \*****************************************************/

   while(siArg < numArgsSI)
   { /*Loop: get user input*/

      if(
         ! eqlNull_ulCp(
            (signed char *) "-file",
            (signed char *) argAryStr[siArg]
         )
      ){ /*If: account tsv provided*/
         ++siArg;
         *acntFileStrPtr = (signed char *)
            argAryStr[siArg];
      }  /*If: account tsv provided*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-out",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: output file*/
         ++siArg;
         *outFileStrPtr = (signed char *)
            argAryStr[siArg];
      }  /*Else If: output file*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-p-out",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: output file*/
         ++siArg;
         *printFileStrPtr = (signed char *)
            argAryStr[siArg];
      }  /*Else If: output file*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-divy-file",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: divy tsv file*/
         ++siArg;
         *divyFileStrPtr = (signed char *)
            argAryStr[siArg];

         if(sumDefBl)
         { /*If: need to remove default sum cmd*/
            *cmdFlagsSCPtr &= (~def_sumCmd_mainAcnt);
            sumDefBl = 0;
         } /*If: need to remove default sum cmd*/
      }  /*Else If: divy tsv file*/

      /**************************************************\
      * Fun03 Sec02 Sub02:
      *   - commands
      *   o fun03 sec02 sub02 cat01:
      *     - add command
      *   o fun03 sec02 sub02 cat02:
      *     - del command
      *   o fun03 sec02 sub02 cat03:
      *     - divy command
      *   o fun03 sec02 sub02 cat04:
      *     - sum command
      *   o fun03 sec02 sub02 cat05:
      *     - print a set of entries command
      *   o fun03 sec02 sub02 cat06:
      *     - prompt command
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub02 Cat01:
      +   - add command
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-add",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: user adding account entry*/
         *cmdFlagsSCPtr |= def_addCmd_mainAcnt;
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToF_base10str(
               tmpStr,
               addAmountFPtr
            );

         if(*tmpStr != '\0')
         { /*If: input is non-numeric or to large*/
            fprintf(
               stderr,
               "-add %s is non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If: input is non-numeric or to large*/

         if(sumDefBl)
         { /*If: need to remove default sum cmd*/
            *cmdFlagsSCPtr &= (~def_sumCmd_mainAcnt);
            sumDefBl = 0;
         } /*If: need to remove default sum cmd*/
      }  /*Else If: user adding account entry*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-no-add",
            (signed char *) argAryStr[siArg]
         )
      ) *cmdFlagsSCPtr &= (~def_addCmd_mainAcnt);

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub02 Cat02:
      +   - delete command
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-del",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: deleting an index*/
         *cmdFlagsSCPtr |= def_delCmd_mainAcnt;
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSL_base10str(
               tmpStr,
               indexSLPtr 
            );

         if(*tmpStr != '\0')
         { /*If: non-numeric input*/
            fprintf(
               stderr,
               "-del %s is non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If: non-numeric input*/

         if(sumDefBl)
         { /*If: need to remove default sum cmd*/
            *cmdFlagsSCPtr &= (~def_sumCmd_mainAcnt);
            sumDefBl = 0;
         } /*If: need to remove default sum cmd*/
      }  /*Else If: deleting an index*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-no-del",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: not removing entry*/
         *cmdFlagsSCPtr &= (~def_delCmd_mainAcnt);
         *indexSLPtr = 0;
      }  /*Else If: not removing entry*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub02 Cat03:
      +   - divy command
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-divy",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: wants to do a divy*/
         *cmdFlagsSCPtr |= def_divyCmd_mainAcnt;
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToF_base10str(
               tmpStr,
               divyAmountFPtr
            );

         if(*tmpStr != '\0')
         { /*If: input is non-numeric or to large*/
            fprintf(
               stderr,
               "-divy %s is non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If: input is non-numeric or to large*/

         if(sumDefBl)
         { /*If: need to remove default sum cmd*/
            *cmdFlagsSCPtr &= (~def_sumCmd_mainAcnt);
            sumDefBl = 0;
         } /*If: need to remove default sum cmd*/
      }  /*Else If: wants to do a divy*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-no-divy",
            (signed char *) argAryStr[siArg]
         )
      ) *cmdFlagsSCPtr &= (~def_divyCmd_mainAcnt);

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub02 Cat04:
      +   - sum command
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-sum",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: user wanted a summary*/
         *cmdFlagsSCPtr |= def_sumCmd_mainAcnt;

         if(sumDefBl)
            sumDefBl = 0;
      }  /*Else If: user wanted a summary*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-no-sum",
            (signed char *) argAryStr[siArg]
         )
      ) *cmdFlagsSCPtr &= (~def_sumCmd_mainAcnt);

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub02 Cat05:
      +   - print a set of entries command
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-print",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: user wanted to search for entries*/
         *cmdFlagsSCPtr |= def_pEntriesCmd_mainAcnt;

         if(sumDefBl)
         { /*If: need to remove default sum cmd*/
            *cmdFlagsSCPtr &= (~def_sumCmd_mainAcnt);
            sumDefBl = 0;
         } /*If: need to remove default sum cmd*/
      }  /*Else If: user wanted to search for entries*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-no-print",
            (signed char *) argAryStr[siArg]
         )
      ) *cmdFlagsSCPtr &= (~def_pEntriesCmd_mainAcnt);

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub02 Cat06:
      +   - prompt command
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-prompt",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: user wanted interative session*/
         *cmdFlagsSCPtr |= def_promptCmd_mainAcnt;

         if(sumDefBl)
         { /*If: need to remove default sum cmd*/
            *cmdFlagsSCPtr &= (~def_sumCmd_mainAcnt);
            sumDefBl = 0;
         } /*If: need to remove default sum cmd*/
      }  /*Else If: user wanted interative session*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-no-prompt",
            (signed char *) argAryStr[siArg]
         )
      ) *cmdFlagsSCPtr &= (~def_promptCmd_mainAcnt);

      /**************************************************\
      * Fun03 Sec02 Sub03:
      *   - input for commands
      *   o fun03 sec02 sub03 cat01:
      *     - parent account
      *   o fun03 sec02 sub03 cat02:
      *     - child account
      *   o fun03 sec02 sub03 cat03:
      *     - amount to divy/add input
      *   o fun03 sec02 sub03 cat04:
      *     - year
      *   o fun03 sec02 sub03 cat05:
      *     - year search start
      *   o fun03 sec02 sub03 cat06:
      *     - year search end
      *   o fun03 sec02 sub03 cat07:
      *     - month
      *   o fun03 sec02 sub03 cat08:
      *     - month search start
      *   o fun03 sec02 sub03 cat09:
      *     - month search end
      *   o fun03 sec02 sub03 cat10:
      *     - day
      *   o fun03 sec02 sub03 cat11:
      *     - day search start
      *   o fun03 sec02 sub03 cat12:
      *     - day search end
      *   o fun03 sec02 sub03 cat13:
      *     - comment
      *   o fun03 sec02 sub03 cat14:
      *     - ghost setting
      *   o fun03 sec02 sub03 cat15:
      *     - mock (pretend) setting
      \**************************************************/

      /*++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub03 Cat01:
      +  - parent account
      \++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-par",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: parent account input*/
         ++siArg;
         *parStrPtr = (signed char *) argAryStr[siArg];
      }  /*Else If: parent account input*/

      /*++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub03 Cat02:
      +  - child account
      \++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-child",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: child account input*/
         ++siArg;
         *childStrPtr = (signed char *) argAryStr[siArg];
      }  /*Else If: child account input*/

      /*++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub03 Cat04:
      +  - year
      \++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-year",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: year input*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSS_base10str(
               tmpStr,
               yearSSPtr
            );

         if(*tmpStr != '\0')
         { /*If: input is non-numeric or to large*/
            fprintf(
               stderr,
               "-year %s is non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If: input is non-numeric or to large*/
      }  /*Else If: year input*/

      /*++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub03 Cat05:
      +  - year search start
      \++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-start-year",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: staring year for search input*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSS_base10str(
               tmpStr,
               &yearArySS[0]
            );

         if(*tmpStr != '\0')
         { /*If: input is non-numeric or to large*/
            fprintf(
               stderr,
               "-start-year %s; non-numeric/to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If: input is non-numeric or to large*/
      }  /*Else If: staring year for search input*/

      /*++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub03 Cat06:
      +  - year search end
      \++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-end-year",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: ending year for search input*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSS_base10str(
               tmpStr,
               &yearArySS[1]
            );

         if(*tmpStr != '\0')
         { /*If: input is non-numeric or to large*/
            fprintf(
               stderr,
               "-end-year %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If: input is non-numeric or to large*/
      }  /*Else If: ending year for search input*/

      /*++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub03 Cat07:
      +  - month
      \++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-month",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: month input*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSC_base10str(
               tmpStr,
               monthSCPtr
            );

         if(*tmpStr != '\0')
         { /*If: input is non-numeric or to large*/
            fprintf(
               stderr,
               "-month %s is non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If: input is non-numeric or to large*/
      }  /*Else If: month input*/

      /*++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub03 Cat08:
      +  - month search start
      \++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-start-month",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: staring month for search input*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSC_base10str(
               tmpStr,
               &monthArySC[0]
            );

         if(*tmpStr != '\0')
         { /*If: input is non-numeric or to large*/
            fprintf(
               stderr,
               "-start-month %s; non-numeric/to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If: input is non-numeric or to large*/
      }  /*Else If: staring month for search input*/

      /*++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub03 Cat09:
      +  - month search end
      \++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-end-month",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: ending month for search input*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSC_base10str(
               tmpStr,
               &monthArySC[1]
            );

         if(*tmpStr != '\0')
         { /*If: input is non-numeric or to large*/
            fprintf(
               stderr,
               "-end-month %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If: input is non-numeric or to large*/
      }  /*Else If: ending month for search input*/

      /*++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub03 Cat10:
      +  - day
      \++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-day",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: day input*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSC_base10str(
               tmpStr,
               daySCPtr
            );

         if(*tmpStr != '\0')
         { /*If: input is non-numeric or to large*/
            fprintf(
               stderr,
               "-day %s is non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If: input is non-numeric or to large*/
      }  /*Else If: day input*/

      /*++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub03 Cat11:
      +  - day search start
      \++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-start-day",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: staring day for search input*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSC_base10str(
               tmpStr,
               &dayArySC[0]
            );

         if(*tmpStr != '\0')
         { /*If: input is non-numeric or to large*/
            fprintf(
               stderr,
               "-start-day %s; non-numeric/to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If: input is non-numeric or to large*/
      }  /*Else If: staring day for search input*/

      /*++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub03 Cat12:
      +  - day search end
      \++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-end-day",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: ending day for search input*/
         ++siArg;
         tmpStr = (signed char *) argAryStr[siArg];

         tmpStr +=
            strToSC_base10str(
               tmpStr,
               &dayArySC[1]
            );

         if(*tmpStr != '\0')
         { /*If: input is non-numeric or to large*/
            fprintf(
               stderr,
               "-end-day %s; non-numeric or to large%s",
               argAryStr[siArg],
               str_endLine
            );
            goto err_fun03_sec03;
         } /*If: input is non-numeric or to large*/
      }  /*Else If: ending day for search input*/

      /*++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub03 Cat13:
      +  - comment
      \++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-note",
            (signed char *) argAryStr[siArg]
         )
      ){ /*Else If: comment (note) input*/
         ++siArg;
         *commentStrPtr= (signed char *) argAryStr[siArg];
      }  /*Else If: comment (note) input*/

      /*++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub03 Cat14:
      +  - ghost setting
      \++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-ghost",
            (signed char *) argAryStr[siArg]
         )
      ) *ghostBlPtr = 1;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-no-ghost",
            (signed char *) argAryStr[siArg]
         )
      ) *ghostBlPtr = 0;

      /*++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun03 Sec02 Sub03 Cat14:
      +  - mock (pretend) setting
      \++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-pretend",
            (signed char *) argAryStr[siArg]
         )
      ) *cmdFlagsSCPtr |= def_mockCmd_mainAcnt;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-no-pretend",
            (signed char *) argAryStr[siArg]
         )
      ) *cmdFlagsSCPtr &= (~def_mockCmd_mainAcnt);

      /**************************************************\
      * Fun03 Sec02 Sub04:
      *   - help message
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-h",
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--h",
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "help",
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-help",
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--help",
            (signed char *) argAryStr[siArg]
         )
      ) goto phelp_fun03_sec03;


      /**************************************************\
      * Fun03 Sec02 Sub05:
      *   - version number
      \**************************************************/

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-v",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--v",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "version",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "-version",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec03;

      else if(
         ! eqlNull_ulCp(
            (signed char *) "--version",
            (signed char *) argAryStr[siArg]
         )
      ) goto pversion_fun03_sec03;

      /**************************************************\
      * Fun03 Sec02 Sub06:
      *   - unkown input
      \**************************************************/

      else
      { /*Else: unkown input*/
         fprintf(
            stderr,
            "%s is not recognized%s",
            argAryStr[siArg],
            str_endLine
         );
         goto err_fun03_sec03;
      } /*Else: unkown input*/

      /**************************************************\
      * Fun03 Sec02 Sub07:
      *   - move to next argument
      \**************************************************/

      ++siArg;
   } /*Loop: get user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_fun03_sec03;

   phelp_fun03_sec03:;
      phelp_mainAcnt(stdout);
      errSC = 1;
      goto ret_fun03_sec03;

   pversion_fun03_sec03:;
      pversion_mainAcnt(stdout);
      errSC = 1;
      goto ret_fun03_sec03;

   err_fun03_sec03:;
      errSC = 2;
      goto ret_fun03_sec03;

   ret_fun03_sec03:;
      return errSC;
} /*input_mainAcnt*/

/*-------------------------------------------------------\
| Main:
|   - driver function for account
| Input:
|    - numArgsSI:
|      o number input arguments
|    - argAryStr:
|      o c-string array with user input
| Output:
|   - Prints:
|     o acnt operations/results to `-out`
|     o error messages to stderr
|   - Returns:
|     o 0 for no errors
|     o 1 for errors
\-------------------------------------------------------*/
int
main(
   int numArgsSI,    /*number arguments input*/
   char *argAryStr[] /*user input*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main TOC:
   '   - driver function for account
   '   o main sec01:
   '     - variable declarations
   '   o main sec02:
   '     - initialize, get user input, check user input
   '   o main sec03:
   '     - run input non-prompt commands
   '   o main sec04:
   '     - run prompt
   '   o main sec05:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0;  /*for returned error values*/
   signed long errSL = 0;  /*for returned error values*/

   signed char *acntFileStr = 0; /*accounts to process*/
   signed char *outFileStr = 0;  /*no input*/
   signed char *printFileStr = (signed char *) "-";
   signed char *divyFileStr = 0; /*divy file to use*/
   signed char *commentStr = (signed char *) "NA";
      /*comment for transaction*/

   signed char *parStr = 0;   /*parent account*/
   signed char *childStr = 0; /*child account*/

   signed char cmdFlagsSC = def_sumCmd_mainAcnt;
   float addAmountF = 0;         /*amount to transfer*/
   float divyAmountF = 0;        /*amount to transfer*/
   signed char ghostBl = def_ghost_defsAcnt;
   signed long delIndexSL = 0;

   signed short yearSS = 0;   /*for adding/divy*/
   signed short yearArySS[2]; /*for searches*/
   signed char monthSC = 0;   /*for adding/divy*/
   signed char monthArySC[2]; /*for searches*/
   signed char daySC = 0;     /*for adding/divy*/
   signed char dayArySC[2];   /*for searches*/

   unsigned char percisionUC = def_percison_defsAcnt;
     /*2 decimals of percision during print*/

   signed char *divyBuffHeapStr = 0; /*holds divy file*/
   unsigned long lenDivyUL = 0;      /*length of divy*/
   signed long tmpSL = 0;

   struct st_acnt acntStackST;  /*holds accounts*/
   FILE *stFILE = 0;  /*input file to save*/
   FILE *tmpFILE = 0; /*temporary file*/
   FILE *pOutFILE = 0;

   signed char buffStr[def_stdinBuff_mainAcnt];
      /*prompt buffer*/

   time_t timeRaw;   /*holds time*/
   struct tm *timeSTPtr; /*has time variables*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
   ^ Main Sec02:
   ^   - initialize, get user input, check user input
   ^   o main sec02 sub01:
   ^     - initialize varaibles
   ^   o main sec02 sub02:
   ^     - get input
   ^   o main sec02 sub03:
   ^     - setup memory
   ^   o main sec02 sub04:
   ^     - read divy file
   ^   o main sec02 sub05:
   ^     - read account file
   ^   o main sec02 sub06:
   ^     - open print output file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
 
   /*****************************************************\
   * Main Sec02 Sub01:
   *   - initialize varaibles
   \*****************************************************/

   /*initialize structures*/
   init_st_acnt(&acntStackST);

   if(def_prompt_defsAcnt)
      cmdFlagsSC |= def_promptCmd_mainAcnt;
   if(def_mock_defsAcnt)
      cmdFlagsSC |= def_mockCmd_mainAcnt;

   /*set up date, add, divy, and search commands*/
   time(&timeRaw);
   timeSTPtr = localtime(&timeRaw);

   yearSS = (signed short) timeSTPtr->tm_year + 1900;
   yearArySS[0] = 0; /*any year*/
   yearArySS[1] = 0;

   monthSC = (signed short) timeSTPtr->tm_mon + 1;
   monthArySC[0] = 0; /*any month*/
   monthArySC[1] = 0;

   daySC = (signed short) timeSTPtr->tm_mday + 1;
   dayArySC[0] = 0; /*any day*/
   dayArySC[1] = 0;

   /*****************************************************\
   * Main Sec02 Sub02:
   *   - get input
   \*****************************************************/

   errSC =
      input_mainAcnt(
         numArgsSI,
         argAryStr,
         &acntFileStr,  /*accounts to process*/
         &outFileStr,   /*output file*/
         &divyFileStr,  /*divy file to use*/
         &printFileStr, /*sum/search print output file*/
         &cmdFlagsSC,   /*commands to run*/
         &parStr,       /*parent account*/
         &childStr,     /*child account*/
         &addAmountF,   /*amount to transfer*/
         &divyAmountF,  /*amount to transfer*/
         &yearSS,       /*year for add/divy*/
         &monthSC,      /*month for add/divy*/
         &daySC,        /*day for add/divy*/
         &commentStr,   /*comment for transaction*/
         &ghostBl,      /*round and add to ghost account*/
         yearArySS,     /*year (start/end) for searches*/
         monthArySC,    /*month (start/end) for searches*/
         dayArySC,      /*day (start/end) for searches*/
         &delIndexSL    /*index to delete*/
   );

   if(errSC)
   { /*If: had error*/
      --errSC; /*convert help/version print to no error*/
      goto ret_main_sec05;
   } /*If: had error*/

   /*****************************************************\
   * Main Sec02 Sub03:
   *   - setup memory
   \*****************************************************/

   if( setup_st_acnt(&acntStackST) )
   { /*If: had memory error*/
     fprintf(
        stderr,
        "memory error setting up account structure%s",
        str_endLine
     );
     goto err_main_sec05;
   } /*If: had memory error*/

   /*****************************************************\
   * Main Sec02 Sub04:
   *   - read divy file
   *   o main sec02 sub04 cat01:
   *     - open divy file
   *   o main sec02 sub04 cat02:
   *     - get file length and allocate memory
   *   o main sec02 sub04 cat03:
   *     - read in file
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub04 Cat01:
   +   - open divy file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(divyFileStr)
   { /*If: input divy file*/
      stFILE =
         fopen(
            (char *) divyFileStr,
            "r"
         );

      if(! divyFileStr)
      { /*If: could not open file*/
         fprintf(
            stderr,
            "could not open -divy %s%s",
            divyFileStr,
            str_endLine
         );
         goto err_main_sec05;
      } /*If: could not open file*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Main Sec02 Sub04 Cat02:
      +   - get file length and allocate memory
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      fseek(
         stFILE,
         0,
         SEEK_END
      );

      lenDivyUL = ftell(stFILE);

      fseek(
         stFILE,
         0,
         SEEK_SET
      );

      divyBuffHeapStr =
         malloc((lenDivyUL + 9) * sizeof(signed char));
      if(! divyBuffHeapStr)
      { /*If: memory error*/
         fprintf(
            stderr,
            "ran out of memory reading -divy %s%s",
            divyFileStr,
            str_endLine
         );
         goto err_main_sec05;
      } /*If: memory error*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Main Sec02 Sub04 Cat03:
      +   - read in file
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      lenDivyUL =
         fread(
            (char *) divyBuffHeapStr,
            sizeof(signed char),
            lenDivyUL,
            stFILE
         );

      fclose(stFILE);
      divyBuffHeapStr[lenDivyUL] = '\0';
   } /*If: input divy file*/

   else if(cmdFlagsSC & def_divyCmd_mainAcnt)
   { /*Else If: no divy file, but requested divy*/
      fprintf(
         stderr,
         "-divy needs a divy file from -divy-file%s",
         str_endLine
      );

      goto err_main_sec05;
   } /*Else If: no divy file, but requested divy*/

   stFILE = 0;

   /*****************************************************\
   * Main Sec02 Sub05:
   *   - read account file
   *   o main sec02 sub05 cat01:
   *     - open account file
   *   o main sec02 sub04 cat02:
   *     - read account file
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub05 Cat01:
   +   - open account file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(
         ! acntFileStr
      || *acntFileStr == '-'
   ) stFILE = stdin;

   else
   { /*Else: user input account file*/
      stFILE =
        fopen(
           (char *) acntFileStr,
           "r"
        );

      if(! stFILE)
      { /*If: could not open account file*/
         fprintf(
            stderr,
            "could not open -file %s%s",
            acntFileStr,
            str_endLine
         );
         goto err_main_sec05;
      } /*If: could not open account file*/
   } /*Else: user input account file*/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec02 Sub04 Cat02:
   +   - read account file
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   tmpSL =
      readFile_acnt(
         &acntStackST,
         stFILE
      );

   if(stFILE != stdin)
      fclose(stFILE);
   stFILE = 0;

   if(tmpSL)
   { /*If: had error*/
      if(tmpSL == def_memErr_acnt)
      { /*If: memory error*/
         fprintf(
            stderr,
            "memory error read -file %s%s",
            acntFileStr,
            str_endLine
         );
         goto err_main_sec05;
      } /*If: memory error*/

      else if(tmpSL == def_noFILE_acnt)
      { /*Else If: empty file*/
         fprintf(
            stderr,
            "nothing in -file %s%s",
            acntFileStr,
            str_endLine
         );
         goto err_main_sec05;
      } /*Else If: empty file*/

      else
      { /*Else: bad line*/
         fprintf(
            stderr,
            "line %li in -file %s is invalid%s",
            (tmpSL >> 4),
            acntFileStr,
            str_endLine
         );
         goto err_main_sec05;
      } /*Else: bad line*/
   } /*If: had error*/

   /*****************************************************\
   * Main Sec02 Sub06:
   *   - open print output file
   \*****************************************************/

   if(
         ! printFileStr
      || *printFileStr == '-'
   ) pOutFILE = stdout;

   else
   { /*Else: user input output file*/
      pOutFILE =
        fopen(
           (char *) printFileStr,
           "w"
        );

      if(! pOutFILE)
      { /*If: could not open output file*/
         fprintf(
            stderr,
            "could not open -p-out %s%s",
            printFileStr,
            str_endLine
         );
         goto err_main_sec05;
      } /*If: could not open output file*/
   } /*Else: user input output file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec03:
   ^   - run input non-prompt commands
   ^   o main sec03 sub01:
   ^     - delete requested entry
   ^   o main sec03 sub02:
   ^     - add entry
   ^   o main sec03 sub03:
   ^     - divy entry
   ^   o main sec03 sub04:
   ^     - search and print accounts
   ^   o main sec03 sub05:
   ^      - print account sums
   ^   o main sec03 sub06:
   ^     - print additions and deletions to account
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec03 Sub01:
   *   - delete requested entry
   \*****************************************************/

   if(cmdFlagsSC & def_delCmd_mainAcnt)
   { /*If: doing deletion*/
      
      if(cmdFlagsSC & def_mockCmd_mainAcnt)
         tmpFILE = stFILE; /*pretend deletion*/
      else
         tmpFILE = 0;      /*real deletion*/

      if(
         delIndex_st_acnt(
            &acntStackST,
            delIndexSL,
            percisionUC,
            tmpFILE
         )
      ){ /*If: could not delete index*/
         fprintf(
            stderr,
            "%li is not in -file %s; could not delete%s",
            delIndexSL,
            acntFileStr,
            str_endLine
         );
         goto err_main_sec05;
      }  /*If: could not delete index*/

      tmpFILE = 0;
   } /*If: doing deletion*/

   /*****************************************************\
   * Main Sec03 Sub02:
   *   - add entry
   \*****************************************************/

   if(cmdFlagsSC & def_addCmd_mainAcnt)
   { /*If: doing addition*/
      
      if(cmdFlagsSC & def_mockCmd_mainAcnt)
         tmpFILE = stFILE; /*pretend addition*/
      else
         tmpFILE = 0;      /*real addition*/

      errSC =
         addEntry_st_acnt(
            &acntStackST,
            parStr,
            childStr,
            yearSS,
            monthSC,
            daySC,
            addAmountF,
            commentStr,
            ghostBl,
            percisionUC,
            tmpFILE
         );

      tmpFILE = 0;

      if(errSC == def_memErr_acnt)
      { /*If: memory error*/
         fprintf(
            stderr,
            "memory error adding entry to -file %s%s",
            acntFileStr,
            str_endLine
         );
         goto err_main_sec05;
      } /*If: memory error*/

      else if(errSC)
      { /*Else If: date error (or child error for mock)*/
         fprintf(
            stderr,
            "likely missing -par or -child, but also%s",
            str_endLine
         );

         fprintf(
            stderr,
            "  could be bad date%s",
            str_endLine
         );

         goto err_main_sec05;
      } /*Else If: date error (or child error for mock)*/
   } /*If: doing addition*/

   /*****************************************************\
   * Main Sec03 Sub03:
   *   - divy entry
   \*****************************************************/

   if(cmdFlagsSC & def_divyCmd_mainAcnt)
   { /*If: doing addition*/
      
      if(cmdFlagsSC & def_mockCmd_mainAcnt)
         tmpFILE = stFILE; /*pretend addition*/
      else
         tmpFILE = 0;      /*real addition*/

      errSL =
         (unsigned long)
         divy_st_acnt(
            &acntStackST,
            divyAmountF,
            divyBuffHeapStr,
            commentStr,
            ghostBl,
            percisionUC,
            tmpFILE
         );

      if(divyBuffHeapStr)
         free(divyBuffHeapStr);
      divyBuffHeapStr = 0;

      tmpFILE = 0;

      if(errSL == def_memErr_acnt)
      { /*If: memory error*/
         fprintf(
            stderr,
            "memory error diving to -file %s%s",
            acntFileStr,
            str_endLine
         );
         goto err_main_sec05;
      } /*If: memory error*/

      else if(errSL == def_missingEntry_acnt)
      { /*Else If: empty file*/
         fprintf(
            stderr,
            "nothing in -divy %s%s",
            divyFileStr,
            str_endLine
         );
         goto err_main_sec05;
      } /*Else If: empty file*/

      else if(errSL)
      { /*Else If: bad line*/
         fprintf(
            stderr,
            "line %li in -divy %s is invalid%s",
            errSL,
            divyFileStr,
            str_endLine
         );
         goto err_main_sec05;
      } /*Else If: bad line*/
   } /*If: doing addition*/

   /*****************************************************\
   * Main Sec03 Sub04:
   *   - search and print accounts
   \*****************************************************/

   if(cmdFlagsSC & def_pEntriesCmd_mainAcnt)
   { /*If: doing search*/
      errSC =
         pEntries_st_acnt(
            &acntStackST,
            parStr,
            childStr,
            yearArySS,
            monthArySC,
            dayArySC,
            percisionUC,
            pOutFILE
         );

      if(errSC == def_memErr_acnt)
      { /*If: memory error*/
         fprintf(
            stderr,
            "memory error; entry search for -file %s%s",
            acntFileStr,
            str_endLine
         );
         goto err_main_sec05;
      } /*If: memory error*/

      else if(errSC)
      { /*Else If: no entries found*/
         fprintf(
            stderr,
            "-file %s has no entries in search range%s",
            acntFileStr,
            str_endLine
         );
         goto err_main_sec05;
      } /*If: no entries found*/
   }  /*Else If: doing search*/

   /*****************************************************\
   * Main Sec03 Sub05:
   *   - print account sums
   \*****************************************************/

   if(cmdFlagsSC & def_sumCmd_mainAcnt)
   { /*If: printing sums*/
      errSC =
         pSum_st_acnt(
            &acntStackST,
            percisionUC,
            pOutFILE
         );

      if(errSC == def_memErr_acnt)
      { /*If: memory error*/
         fprintf(
            stderr,
            "memory error; printing sums for -file %s%s",
            acntFileStr,
            str_endLine
         );
         goto err_main_sec05;
      } /*If: memory error*/
   } /*If: printing sums*/

   if(stFILE && stFILE != stdout)
      fclose(stFILE);
   stFILE = 0;

   /*****************************************************\
   * Main Sec03 Sub06:
   *   - print additions and deletions to account
   \*****************************************************/

   if(cmdFlagsSC & def_promptCmd_mainAcnt)
      ; /*if user running interactive session; let the
        `  save the modifications
        */

   else if(
         outFileStr
      && (*outFileStr == '-' || *outFileStr)
   ) goto pchange_main_sec03_sub06;

   else if(
         ! (cmdFlagsSC & def_delCmd_mainAcnt)
      && ! (cmdFlagsSC & def_addCmd_mainAcnt)
      && ! (cmdFlagsSC & def_divyCmd_mainAcnt)
   ) ; /*made no modifications to account*/

   else if(cmdFlagsSC & def_sumCmd_mainAcnt)
      ; /*wanted a summary*/

   else if(cmdFlagsSC & def_pEntriesCmd_mainAcnt)
      ; /*wanted a search and print*/

   else if(cmdFlagsSC & def_mockCmd_mainAcnt)
      ; /*doing pretend (mock) change*/

   else
   { /*Else: modified account file*/
      pchange_main_sec03_sub06:;

      if(
            ! outFileStr
         || outFileStr[0] == '\0'
         || outFileStr[0] == '-'
      ) stFILE = stdout;

      else
      { /*Else: user provided output file*/
         stFILE =
            fopen(
              (char *) outFileStr,
              "w"
            );

         if(! stFILE)
         { /*If: had file error*/
            fprintf(
               stderr,
               "unable to open -out %s%s",
               outFileStr,
               str_endLine
            );
            goto err_main_sec05;
         } /*If: had file error*/
      } /*Else: user provided output file*/


      /*write output to file*/
      if(
         write_st_acnt(
            &acntStackST,
            percisionUC,
            stFILE
         )
      ){ /*If: memory error*/
         fprintf(
           stderr,
           "memory error saving -out %s%s",
           outFileStr,
           str_endLine
         );
         goto err_main_sec05;
      }  /*If: memory error*/


      if(stFILE != stdout)
        fclose(stFILE);
      stFILE = 0;
   } /*Else: modified account file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec04:
   ^   - run prompt
   ^   o main sec04 sub01:
   ^     - get input + start loop
   ^   o main sec04 sub02:
   ^     - print help message (if requested)
   ^   o main Sec04 Sub03:
   ^     - check if need to quite
   ^   o main Sec04 Sub04:
   ^     - check if deleting entry
   ^   o main Sec04 Sub05:
   ^     - check if adding entry
   ^   o main Sec04 Sub06:
   ^     - check if dividing up funds (divy)
   ^   o main Sec04 Sub07:
   ^     - check if searching for and printing entries
   ^   o main Sec04 Sub08:
   ^     - check if printing sums (account totals)
   ^   o main Sec04 Sub09:
   ^     - check if saving file
   ^   o main Sec04 Sub10:
   ^     - unkown input
   ^   o main Sec04 Sub11:
   ^     - Removing any trailing file IO
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Main Sec04 Sub01:
   *   - get input + start loop
   ^   o main sec04 sub01 cat01:
   ^     - print prompt, get input, + start loop
   ^   o main sec04 sub01 cat02:
   ^     - check if read in all user inptut
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Main Sec04 Sub01 Cat01:
   +   - print prompt, get input, + start loop
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! (cmdFlagsSC & def_promptCmd_mainAcnt) )
      goto done_main_sec05;
      /*not doing interactive session*/

   do{ /*Loop: get input*/
      printf(
         "acnt interactive: h for help; q to quite%s",
         str_endLine
      );

      parStr =
         (signed char *)
         fgets(
            (char *) buffStr,
            def_stdinBuff_mainAcnt,
            stdin
         ); /*get user input*/

      if(! parStr)
         continue; /*nothing input*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Main Sec04 Sub01 Cat02:
      +   - check if read in all user inptut
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      parStr = buffStr;

      while(
            *parStr != '\n'
         && *parStr != '\r'
         && *parStr != '\0'
      ) ++parStr;


      if(*parStr == '\0')
      { /*If: over 1024 bytes*/
         printf(
            "input is longer than 1023 characters%s",
            str_endLine
         );

         while(*parStr == '\0')
         { /*Loop: till get end of input*/
            parStr =
               (signed char *)
               fgets(
                  (char *) buffStr,
                  def_stdinBuff_mainAcnt,
                  stdin
               ); /*get user input*/

            if(! parStr)
               continue; /*nothing input*/

            while(
                  *parStr != '\n'
               && *parStr != '\r'
               && *parStr != '\0'
            ) ++parStr;
         } /*Loop: till get end of input*/

         continue; /*invalid input*/
      } /*If: over 1024 bytes*/


      *parStr = '\0'; /*remove new line*/

      /**************************************************\
      * Main Sec04 Sub02:
      *   - print help message (if requested)
      \**************************************************/

      if(
            buffStr[0] == 'h'
         && buffStr[1] == '\0'
      ){ /*If: wanted help message*/
         printf(
            "add: to add a new entry%s",
            str_endLine
         );

         printf(
            "del: to delete an entry%s",
            str_endLine
         );

         printf(
            "divy: to divide entrys between accoutns%s",
            str_endLine
         );

         printf(
            "search: to search for account entries%s",
            str_endLine
         );

         printf(
            "sum: to print totals for all accounts%s",
            str_endLine
         );

         printf(
            "save: to save your accounts%s",
            str_endLine
         );

         printf(
            "q: to quite%s",
            str_endLine
         );

         printf(
            "h: to print this help message%s",
            str_endLine
         );
      } /*If: wanted help message*/

      /**************************************************\
      * Main Sec04 Sub03:
      *   - check if need to quite
      \**************************************************/

      else if(
            buffStr[0] == 'q'
         && buffStr[1] == '\0'
      ) break; /*finished*/
        
      /**************************************************\
      * Main Sec04 Sub04:
      *   - check if deleting entry
      \**************************************************/

      else if(
            buffStr[0] == 'd'
         && buffStr[1] == 'e'
         && buffStr[2] == 'l'
         && buffStr[3] == '\0'
      ){ /*Else If: deleting an entry*/
         delEntry_cmdAcnt(
            &acntStackST,
            percisionUC
         );
      }  /*Else If: deleting an entry*/

      /**************************************************\
      * Main Sec04 Sub05:
      *   - check if adding entry
      \**************************************************/

      else if(
            buffStr[0] == 'a'
         && buffStr[1] == 'd'
         && buffStr[2] == 'd'
         && buffStr[3] == '\0'
      ){ /*Else If: adding new entry*/
         if(
            addEntry_cmdAcnt(
               &acntStackST,
               percisionUC,
               ghostBl
            )
         ) goto err_main_sec05;
           /*already printed error message*/
      }  /*Else If: adding new entry*/

      /**************************************************\
      * Main Sec04 Sub06:
      *   - check if dividing up funds (divy)
      \**************************************************/

      else if(
            buffStr[0] == 'd'
         && buffStr[1] == 'i'
         && buffStr[2] == 'v'
         && buffStr[3] == 'y'
         && buffStr[4] == '\0'
      ){ /*Else If: dividing entries*/
         if(
            divy_cmdAcnt(
               &acntStackST,
               divyFileStr,
               percisionUC,
               ghostBl
            )
         ) goto err_main_sec05;
           /*already printed error message*/
      }  /*Else If: dividing entries*/

      /**************************************************\
      * Main Sec04 Sub07:
      *   - check if searching for and printing entries
      \**************************************************/

      else if(
            buffStr[0] == 's'
         && buffStr[1] == 'e'
         && buffStr[2] == 'a'
         && buffStr[3] == 'r'
         && buffStr[4] == 'c'
         && buffStr[5] == 'h'
         && buffStr[6] == '\0'
      ){ /*Else If: searching for entries*/
         pEntries_cmdAcnt(
            &acntStackST,
            percisionUC
         );
      }  /*Else If: searching for entries*/

      /**************************************************\
      * Main Sec04 Sub08:
      *   - check if printing sums (account totals)
      \**************************************************/

      else if(
            buffStr[0] == 's'
         && buffStr[1] == 'u'
         && buffStr[2] == 'm'
         && buffStr[3] == '\0'
      ){ /*Else If: printing sums (totals)*/
         pSum_st_acnt(
            &acntStackST,
            percisionUC,
            stdout
         );
      }  /*Else If: printing sums (totals)*/

      /**************************************************\
      * Main Sec04 Sub09:
      *   - check if saving file
      \**************************************************/

      else if(
            buffStr[0] == 's'
         && buffStr[1] == 'a'
         && buffStr[2] == 'v'
         && buffStr[3] == 'e'
         && buffStr[4] == '\0'
      ){ /*Else If: saving file*/
         if(
             write_cmdAcnt(
               &acntStackST,
               outFileStr,
               percisionUC
             )
         ) goto err_main_sec05;
           /*error message printed already*/
      }  /*Else If: saving file*/

      /**************************************************\
      * Main Sec04 Sub10:
      *   - unkown input
      \**************************************************/

      else
      { /*Else: unkown input*/
         printf(
            "%s is not recongnized%s",
            buffStr,
            str_endLine
         );
      } /*Else: unkown input*/

      /**************************************************\
      * Main Sec04 Sub11:
      *   - remove trailing file IO
      \**************************************************/

   } while(1 == 1); /*Loop: get input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec05:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   done_main_sec05:;
      errSC = 0;
      goto ret_main_sec05;

   err_main_sec05:;
      errSC = 1;
      goto ret_main_sec05;

   ret_main_sec05:;
      freeStack_st_acnt(&acntStackST);

      if(divyBuffHeapStr)
         free(divyBuffHeapStr);
      divyBuffHeapStr = 0;

      if(
            stFILE
         && stFILE != stdin
         && stFILE != stdout
         && stFILE != stderr
      ) fclose(stFILE);
      stFILE = 0;

      if(
            pOutFILE
         && pOutFILE != stdin
         && pOutFILE != stdout
         && pOutFILE != stderr
      ) fclose(pOutFILE);
      pOutFILE = 0;

      return errSC;
} /*main*/
