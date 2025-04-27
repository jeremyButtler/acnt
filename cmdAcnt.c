/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' cmdAcnt SOF: Start Of File
'   - interactive command interface for acnt
'   o header:
'     - included libraries
'   o fun01: addEntry_cmdAcnt
'     - gets entry from user and adds to account
'   o fun02: delEntry_cmdAcnt
'     - deletes entry to remove from a st_acnt
'   o fun03: pEntries_cmdAcnt
'     - prints entries for an account
'   o fun04: divy_cmdAcnt
'     - runs divy command from user input
'   o fun05: write_cmdAcnt
'     - save account to file
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "cmdAcnt.h"
#include <stdio.h>
#include <time.h>

#include "acnt.h"

#include "genLib/endLine.h"
#include "genLib/base10str.h"
#include "genLib/ulCp.h"
#include "genLib/strAry.h"

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\
! Hidenn libraries:
!   - .c  #include "genLib/numToStr.h"
!   - .c  #include "genLib/ptrAry.h"
\%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/*-------------------------------------------------------\
| Fun01: addEntry_cmdAcnt
|   - gets entry from user and adds to account
| Input:
|   - acntSTPtr:
|     o pointer to st_acnt struct to add entry to
|   - persisionUC:
|     o number decimals to print (prentend/mock delete)
|   - ghostBl:
|     o apply rounding (up for negative; down for postive)
|       * 1 apply rounding
|       * 0 do not round
| Output:
|   - Modifies:
|     o acntSTPtr to have user input
|   - Prints:
|     o prompts to stdout
|     o gets user input form stdin
|   - Returns:
|     o 0 for no errors
|     o 1 for memory error
\-------------------------------------------------------*/
signed char
addEntry_cmdAcnt(
   struct st_acnt *acntSTPtr,
   unsigned char percisionUC, /*amount of percision*/
   signed char ghostBl         /*1: do ghost rounding*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun01 TOC:
   '   - gets entry from user and adds to account
   '   o fun01 sec02:
   '     - get amount of transaction
   '   o fun01 sec03:
   '     - get parent account for transaction
   '   o fun01 sec05:
   '     - get year of transaction
   '   o fun01 sec06:
   '     - get month of transaction
   '   o fun01 sec07:
   '     - get day of transaction
   '   o fun01 sec08:
   '     - get comment
   '   o fun01 sec09:
   '     - get if apply ghost account
   '   o fun01 sec10:
   '     - add entry
   '   o fun01 sec11:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   float amountF = 0;

   signed char parStr[def_lenStr_strAry];
   signed char childStr[def_lenStr_strAry];
   signed char commentStr[def_inBuffLen_cmdAcnt];

   signed short yearSS = 0;
   signed char monthSC = 0;
   signed char daySC = 0;

   signed long indexSL = 0;

   signed char inStr[def_inBuffLen_cmdAcnt];

   signed char *tmpStr = 0;
   signed char tmpSC = 0;
   signed short tmpSS = 0;

   time_t timeRaw;
   struct tm *timeSTPtr;

   /*get time*/
   time(&timeRaw);
   timeSTPtr = localtime(&timeRaw);
   yearSS = (signed short) timeSTPtr->tm_year + 1900;
   monthSC = (signed char) timeSTPtr->tm_mon + 1;
   daySC = (signed char) timeSTPtr->tm_mday;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec02:
   ^   - get amount of transaction
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpStr = inStr;
   *tmpStr = ' ';

   while(*tmpStr != '\0')
   { /*Loop: get amount input*/
      printf("amount (q to quite):%s", str_endLine);

      tmpStr =
         (signed char *)
         fgets(
            (char *) inStr,
            def_inBuffLen_cmdAcnt,
            stdin
         );

      if(
            ! tmpStr
         || *tmpStr == '\0'
         || *tmpStr == '\n'
         || *tmpStr == '\r'
      ){ /*If: nothing input*/
         printf(
            "nothing input for amount%s",
            str_endLine
         );

         tmpStr = inStr;
         *tmpStr = ' ';
         continue;
      } /*If: nothing input*/

      else if(
             (tmpStr[0] & ~32) == 'Q'
          && tmpStr[1] < 33
      ) goto quite_fun01_sec10;

      tmpStr +=
         strToF_base10str(
            inStr,
            &amountF
         );

      if(*tmpStr == '\0')
         break;
      else if(*tmpStr == '\n')
         break;
      else if(*tmpStr == '\r')
         break;
      else
      { /*Else: invalid input*/         
         printf(
            "%s is non-numeric  or to large%s",
            inStr,
            str_endLine
         );
         continue;
      } /*Else: invalid input*/         
   } /*Loop: get amount input*/

   printf(
      "amount set to %0.2f%s",
      amountF,
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec03:
   ^   - get parent account for transaction
   ^   o fun01 sec03 sub01:
   ^     - prompt user and check if got input
   ^   o fun01 sec03 sub02:
   ^     - check if quiting or listing parent accounts
   ^   o fun01 sec03 sub03:
   ^     - check if index for account
   ^   o fun01 sec03 sub04:
   ^     - else account name input
   ^   o fun01 sec03 sub05:
   ^     - echo parent account name back to user
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec03 Sub01:
   *   - prompt user and check if got input
   \*****************************************************/

   tmpStr = inStr;
   *tmpStr = ' ';

   while(*tmpStr != '\0')
   { /*Loop: get parent account*/
     printf("parent account; q quite; l list; i index:");
     printf("%s", str_endLine);

      tmpStr =
         (signed char *)
         fgets(
            (char *) inStr,
            def_inBuffLen_cmdAcnt,
            stdin
         );

      if(
            ! tmpStr
         || *tmpStr == '\0'
         || *tmpStr == '\n'
         || *tmpStr == '\r'
      ){ /*If: nothing input*/
         printf(
            "nothing input for parent account%s",
             str_endLine
         );
         tmpStr = inStr;
         *tmpStr = ' ';
         continue;
      } /*If: nothing input*/

      /**************************************************\
      * Fun01 Sec03 Sub02:
      *   - check if quiting or listing parent accounts
      \**************************************************/

      else if(
             (tmpStr[0] & ~32) == 'Q'
          && tmpStr[1] < 33
      ) goto quite_fun01_sec10;

      else if(
             (tmpStr[0] & ~32) == 'L'
          && tmpStr[1] < 33
      ){ /*Else If: wanted list*/
         for(
            indexSL = 0;
            indexSL < (signed long) acntSTPtr->numParUS;
            ++indexSL
         ){ /*Loop: print parent account ids*/
             printf(
                "  %li: %s parent account%s",
                indexSL,
                get_strAry(
                   acntSTPtr->uniqParStr,
                   indexSL
                ),
                str_endLine
             );
         }  /*Loop: print parent account ids*/
      }  /*Else If: wanted list*/

      /**************************************************\
      * Fun01 Sec03 Sub03:
      *   - check if index for account
      \**************************************************/

      else if(
             (tmpStr[0] & ~32) == 'I'
          && tmpStr[1] < 33
      ){ /*Else If: index input*/

         while(*tmpStr < 33)
         { /*Loop: get to index*/
            if(*tmpStr == '\0')
               break;

            ++tmpStr;
         } /*Loop: get to index*/

         tmpSS = tmpStr - inStr;
         tmpStr = inStr + 2;

         tmpStr +=
            strToSL_base10str(
               tmpStr,
               &indexSL
            );


         if(*tmpStr > 32)
         { /*If: non-numeric value*/
            printf(
               "%s is non-numeric or to large%s",
               &inStr[tmpSS],
               str_endLine
            );

            continue;
         } /*If: non-numeric value*/


         else if(
            indexSL > (signed long) acntSTPtr->numParUS
         ){ /*Else If: index does not exist*/
            printf(
               "no parent account at index %li%s",
               indexSL,
               str_endLine
            );

            continue;
         } /*Else If: index does not exist*/


         else
         { /*Else: valid index*/
            cpStr_ulCp(
               parStr,
               get_strAry(
                  acntSTPtr->uniqParStr,
                  indexSL
               )
            );

            break;
         } /*Else: valid index*/
      } /*Else If: index input*/

      /**************************************************\
      * Fun01 Sec03 Sub04:
      *   - else account name input
      \**************************************************/

      else
      { /*Else: input parent account name*/
         tmpSS = endLine_ulCp(inStr);

         if(tmpSS >= def_lenStr_strAry - 1)
         { /*If: account name out of bounds*/
            printf(
               "parent account name %s is to long%s",
               inStr,
               str_endLine
            );
            continue;
         } /*If: account name out of bounds*/

         cpWhite_ulCp(
            parStr,
            inStr
         );

         break;
      } /*Else: input parent account name*/
   } /*Loop: get parent account*/

   /*****************************************************\
   * Fun01 Sec03 Sub05:
   *   - echo parent account name back to user
   \*****************************************************/

   printf(
      "parent account set to %s%s",
      parStr,
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec04:
   ^   - get child account for transaction
   ^   o fun01 sec04 sub01:
   ^     - prompt user and check if got input
   ^   o fun01 sec04 sub02:
   ^     - check if quiting or listing child accounts
   ^   o fun01 sec04 sub03:
   ^     - check if index for account
   ^   o fun01 sec04 sub04:
   ^     - else account name input
   ^   o fun01 sec04 sub05:
   ^     - echo child account name back to user
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun01 Sec04 Sub01:
   *   - prompt user and check if got input
   \*****************************************************/

   tmpStr = inStr;
   *tmpStr = ' ';

   while(*tmpStr != '\0')
   { /*Loop: get child account*/
     printf("child account; q quite; l list; i index:");
     printf("%s", str_endLine);

      tmpStr =
         (signed char *)
         fgets(
            (char *) inStr,
            def_inBuffLen_cmdAcnt,
            stdin
         );

      if(
            ! tmpStr
         || *tmpStr == '\0'
         || *tmpStr == '\n'
         || *tmpStr == '\r'
      ){ /*If: nothing input*/
         printf(
            "nothing input for child account%s",
            str_endLine
         );
         tmpStr = inStr;
         *tmpStr = ' ';
         continue;
      } /*If: nothing input*/

      /**************************************************\
      * Fun01 Sec04 Sub02:
      *   - check if quiting or listing child accounts
      \**************************************************/

      else if(
             (tmpStr[0] & ~32) == 'Q'
          && tmpStr[1] < 33
      ) goto quite_fun01_sec10;

      else if(
             (tmpStr[0] & ~32) == 'L'
          && tmpStr[1] < 33
      ){ /*Else If: wanted list*/
         for(
            indexSL = 0;
            indexSL < (signed long) acntSTPtr->numChildUS;
            ++indexSL
         ){ /*Loop: print parent account ids*/
             printf(
                "  %li: %s child account%s",
                indexSL,
                get_strAry(
                   acntSTPtr->uniqChildStr,
                   indexSL
                ),
                str_endLine
             );
         }  /*Loop: print parent account ids*/
      }  /*Else If: wanted list*/

      /**************************************************\
      * Fun01 Sec04 Sub03:
      *   - check if index for account
      \**************************************************/

      else if(
             (tmpStr[0] & ~32) == 'I'
          && tmpStr[1] < 33
      ){ /*Else If: index input*/

         while(*tmpStr < 33)
         { /*Loop: get to index*/
            if(*tmpStr == '\0')
               break;

            ++tmpStr;
         } /*Loop: get to index*/

         tmpSS = tmpStr - inStr;
         tmpStr = inStr + 2;

         tmpStr +=
            strToSL_base10str(
               tmpStr,
               &indexSL
            );


         if(*tmpStr > 32)
         { /*If: non-numeric value*/
            printf(
               "%s is non-numeric or to large%s",
               &inStr[tmpSS],
               str_endLine
            );

            continue;
         } /*If: non-numeric value*/


         else if(
            indexSL > (signed long) acntSTPtr->numChildUS
         ){ /*Else If: index does not exist*/
            printf(
               "no child account at index %li%s",
               indexSL,
               str_endLine
            );

            continue;
         } /*Else If: index does not exist*/


         else
         { /*Else: valid index*/
            cpStr_ulCp(
               childStr,
               get_strAry(
                  acntSTPtr->uniqChildStr,
                  indexSL
               )
            );

            break;
         } /*Else: valid index*/
      } /*Else If: index input*/

      /**************************************************\
      * Fun01 Sec04 Sub04:
      *   - else account name input
      \**************************************************/

      else
      { /*Else: input parent account name*/
         tmpSS = endLine_ulCp(inStr);

         if(tmpSS >= def_lenStr_strAry - 1)
         { /*If: account name out of bounds*/
            printf(
               "child account name %s is to long%s",
               inStr,
               str_endLine
            );
            continue;
         } /*If: account name out of bounds*/

         cpWhite_ulCp(
            childStr,
            inStr
         );

         break;
      } /*Else: input parent account name*/

   } /*Loop: get child account*/
   
   /*****************************************************\
   * Fun01 Sec04 Sub05:
   *   - echo child account name back to user
   \*****************************************************/

   printf(
      "child account set to %s%s",
      childStr,
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec05:
   ^   - get year of transaction
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpStr = inStr;
   *tmpStr = ' ';

   while(*tmpStr != '\0')
   { /*Loop: get year*/
      printf(
         "year [default %i] (q to quite):%s",
         yearSS,
         str_endLine
      );

      tmpStr =
         (signed char *)
         fgets(
            (char *) inStr,
            def_inBuffLen_cmdAcnt,
            stdin
         );

      if(
            ! tmpStr
         || *tmpStr == '\0'
         || *tmpStr == '\n'
         || *tmpStr == '\r'
      ) break; /*using default year*/

      else if(
             tmpStr[0] == 'q'
          && tmpStr[1] < 33
      ) goto quite_fun01_sec10;

      tmpStr +=
         strToSS_base10str(
            inStr,
            &tmpSS
         );

      if(*tmpStr == '\0')
         ;
      else if(*tmpStr == '\n')
         ;
      else if(*tmpStr == '\r')
         ;
      else
      { /*Else: invalid input*/         
         printf(
            "%s is non-numeric  or to large%s",
            inStr,
            str_endLine
         );
         continue;
      } /*Else: invalid input*/         


      if(tmpSS < 1900)
      { /*Else If: negative year input*/
         fprintf(
            stdout,
            "first allowed year is 1900%s",
            str_endLine
         );
         continue;
      } /*Else If: negative year input*/

      yearSS = tmpSS;
   } /*Loop: get year*/

   printf(
      "year set to %i%s",
      yearSS,
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec06:
   ^   - get month of transaction
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpStr = inStr;
   *tmpStr = ' ';

   while(*tmpStr != '\0')
   { /*Loop: get monnth*/
      printf(
         "month [default %i] (q to quite):%s",
         monthSC,
         str_endLine
      );

      tmpStr =
         (signed char *)
         fgets(
            (char *) inStr,
            def_inBuffLen_cmdAcnt,
            stdin
         );

      if(
            ! tmpStr
         || *tmpStr == '\0'
         || *tmpStr == '\n'
         || *tmpStr == '\r'
      ) break; /*using default month*/

      else if(
             tmpStr[0] == 'q'
          && tmpStr[1] < 33
      ) goto quite_fun01_sec10;

      tmpStr +=
         strToSC_base10str(
            inStr,
            &tmpSC
         );

      if(*tmpStr == '\0')
         ;
      else if(*tmpStr == '\n')
         ;
      else if(*tmpStr == '\r')
         ;
      else
      { /*Else: invalid input*/         
         printf(
            "%s is non-numeric  or to large%s",
            inStr,
            str_endLine
         );
         continue;
      } /*Else: invalid input*/         

      if(tmpSC > 12)
      { /*Else: invalid input*/         
         printf(
            "only 12 months in a year%s",
            str_endLine
         );
         continue;
      } /*Else: invalid input*/         

      else if(tmpSC <= 0)
      { /*Else: invalid input*/         
         printf(
            "first month starts at 1%s",
            str_endLine
         );
         continue;
      } /*Else: invalid input*/         

      monthSC = tmpSC;
   } /*Loop: get monnth*/


   printf(
      "month set to %i%s",
      monthSC,
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec07:
   ^   - get day of transaction
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpStr = inStr;
   *tmpStr = ' ';

   while(*tmpStr != '\0')
   { /*loop: get day of transaction*/
      printf(
         "day [default %i] (q to quite):%s",
         daySC,
         str_endLine
      );

      tmpStr =
         (signed char *)
         fgets(
            (char *) inStr,
            def_inBuffLen_cmdAcnt,
            stdin
         );

      if(
            ! tmpStr
         || *tmpStr == '\0'
         || *tmpStr == '\n'
         || *tmpStr == '\r'
      ) break; /*using default day*/

      else if(
             tmpStr[0] == 'q'
          && tmpStr[1] < 33
      ) goto quite_fun01_sec10;

      tmpStr +=
         strToSC_base10str(
            inStr,
            &tmpSC
         );

      if(*tmpStr == '\0')
         ;
      else if(*tmpStr == '\n')
         ;
      else if(*tmpStr == '\r')
         ;
      else
      { /*else: invalid input*/         
         printf(
            "%s is non-numeric  or to large%s",
            inStr,
            str_endLine
         );
         continue;
      } /*else: invalid input*/         

      if(tmpSC > 31)
      { /*else: invalid input*/         
         printf(
            "maximum of 31 days in a month%s",
            str_endLine
         );
         continue;
      } /*else: invalid input*/         

      else if(tmpSC <= 0)
      { /*Else: invalid input*/         
         printf(
            "first day starts at 1%s",
            str_endLine
         );
         continue;
      } /*Else: invalid input*/         

      daySC = tmpSC;
   } /*loop: get day of transaction*/


   printf(
      "day set to %i%s",
      daySC,
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec08:
   ^   - get comment
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpStr = inStr;
   *tmpStr = ' ';

   while(*tmpStr != '\0')
   { /*loop: get amount input*/

      printf(
         "comment [NA] (q to quite):%s",
         str_endLine
      );

      tmpStr =
         (signed char *)
         fgets(
            (char *) inStr,
            def_inBuffLen_cmdAcnt,
            stdin
         );

      if(
            ! tmpStr
         || *tmpStr == '\0'
         || *tmpStr == '\n'
         || *tmpStr == '\r'
      ){ /*If: no comment input*/
         commentStr[0] = 'N';
         commentStr[1] = 'A';
         break;
      } /*If: no comment input*/

      else if(
             tmpStr[0] == 'q'
          && tmpStr[1] < 33
      ) goto quite_fun01_sec10;

      cpLine_ulCp(
         commentStr,
         inStr
      );

      tmpStr = commentStr;
      while(*tmpStr != '\0')
      { /*Loop: remove white space*/
         if(*tmpStr < 32)
            *tmpStr = '_';
         ++tmpStr;
      } /*Loop: remove white space*/

   } /*loop: get amount input*/


   printf(
      "comment set to: %s%s",
      commentStr,
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec09:
   ^   - get if apply ghost account
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpStr = inStr;
   *tmpStr = ' ';

   while(*tmpStr != '\0')
   { /*loop: get amount input*/

      if(ghostBl)
         printf(
            "ghost rouding (y = yes; or n = no) [yes]:%s",
            str_endLine
         );
      else
         printf(
            "ghost rouding (y = yes; or n = no) [no]:%s",
            str_endLine
         );

      tmpStr =
         (signed char *)
         fgets(
            (char *) inStr,
            def_inBuffLen_cmdAcnt,
            stdin
         );

      if(
            ! tmpStr
         || *tmpStr == '\0'
         || *tmpStr == '\n'
         || *tmpStr == '\r'
      ) break;

      else if(
             tmpStr[0] == 'q'
          && tmpStr[1] < 33
      ) goto quite_fun01_sec10;

      else if(tmpStr[0] == 'y')
      { /*Else If: yes input*/
         ghostBl = 1;
         break;
      } /*Else If: yes input*/

      else if(tmpStr[0] == 'n')
      { /*Else If: no input*/
         ghostBl = 0;
         break;
      } /*Else If: no input*/

      else
      { /*Else: unkown input*/
         printf("unkown value;%s", str_endLine);
         printf(
            "input: y (yes), n (no) or q (quite)%s",
            str_endLine
         );
      } /*Else: unkown input*/
   } /*loop: get amount input*/


   if(ghostBl)
      printf(
         "applying ghost rouding to transcation%s",
         str_endLine
      );
   else
      printf(
         "not doing ghost rouding%s",
         str_endLine
      );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec10:
   ^   - add entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   printf(
      "getting changes for new entry%s",
      str_endLine
   );

   if(
      addEntry_st_acnt(
         acntSTPtr,
         parStr,
         childStr,
         yearSS,
         monthSC,
         daySC,
         amountF,
         commentStr,
         ghostBl,
         percisionUC,
         stdout
     )
   ) goto memErr_fun01_sec10;

   while(0 == 0)
   { /*Loop: confirm changes*/
      printf(
         "ok wih changes [y for yes; n for no]%s",
         str_endLine
      );
      
      tmpStr =
         (signed char *)
         fgets(
            (char *) inStr,
            def_inBuffLen_cmdAcnt,
            stdin
         );

      if(
            ! tmpStr
         || *tmpStr == '\0'
         || *tmpStr == '\n'
         || *tmpStr == '\r'
      ){ /*If: nothing input*/
         printf(
            "no input; input y for yes and n for no%s",
            str_endLine
         );

         continue;
      }  /*If: nothing input*/


      if(
            inStr[0] == 'y'
         && inStr[1] < 33
      ){ /*If: user said yes*/
         printf("adding account%s", str_endLine);

         if(
            addEntry_st_acnt(
               acntSTPtr,
               parStr,
               childStr,
               yearSS,
               monthSC,
               daySC,
               amountF,
               commentStr,
               ghostBl,
               percisionUC,
               0
           )
         ) goto memErr_fun01_sec10;

         break;
      } /*If: user said yes*/

      else if(
            inStr[0] == 'n'
         && inStr[1] < 33
      ){ /*Else If: user said no*/
         printf(
            "not adding account%s",
            str_endLine
         );
         goto quite_fun01_sec10;
      } /*Else If: user said no*/

      else
         printf(
            "input y to add entry; n to quite%s",
            str_endLine
         );
   } /*Loop: confirm changes*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun01 Sec11:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   memErr_fun01_sec10:;
      printf(
         "memory error adding entry%s",
         str_endLine
      );
      return 1;
   quite_fun01_sec10:;
      return 0;
} /*addEntry_cmdAcnt*/

/*-------------------------------------------------------\
| Fun02: delEntry_cmdAcnt
|   - deletes entry to remove from a st_acnt
| Input:
|   - acntSTPtr:
|     o pointer to st_acnt struct to remove entry from
|   - persisionUC:
|     o number decimals to print (prentend/mock delete)
| Output:
|   - Modifies:
|     o acntSTPtr to have target entry removed
|   - Prints:
|     o prompts to stdout
|     o gets user input form stdin
\-------------------------------------------------------*/
void
delEntry_cmdAcnt(
   struct st_acnt *acntSTPtr,
   unsigned char percisionUC  /*amount of percision*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun02 TOC:
   '   - deletes entry to remove from a st_acnt
   '   o fun02 sec01:
   '     - variable declarations
   '   o fun02 sec02:
   '     - get index to delete
   '   o fun02 sec03:
   '     - delete entry
   '   o fun02 sec04:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char inStr[def_inBuffLen_cmdAcnt];

   signed char *tmpStr = 0;

   signed long indexSL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec02:
   ^   - get index to delete
   ^   o fun02 sec02 sub01:
   ^     - get user input and check if valid
   ^   o fun02 sec02 sub02:
   ^     - convert input and check if valid number
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun02 Sec02 Sub01:
   *   - get user input and check if valid
   \*****************************************************/

   tmpStr = inStr;
   *tmpStr = ' ';

   while(*tmpStr != '\0')
   { /*Loop: get amount input*/
      printf(
         "index to delete (q to quite):%s",
         str_endLine
      );

      tmpStr =
         (signed char *)
         fgets(
            (char *) inStr,
            def_inBuffLen_cmdAcnt,
            stdin
         );

      if(
            ! tmpStr
         || *tmpStr == '\0'
         || *tmpStr == '\r'
         || *tmpStr == '\n'
      ){  /*Else If: at end of string*/
         printf(
            "nothing input%s",
            str_endLine
         );

         continue;
      } /*Else If: at end of string*/

      else if(
             tmpStr[0] == 'q'
          && tmpStr[1] < 33
      ) goto quite_fun02_sec04;

     /*****************************************************\
     * Fun02 Sec02 Sub02:
     *   - convert input and check if valid number
     \*****************************************************/

      tmpStr +=
         strToSL_base10str(
            inStr,
            &indexSL
         );

      if(
            *tmpStr == '\0'
         || *tmpStr == '\r'
         || *tmpStr == '\n'
      ){ /*If: valid input*/

         if(indexSL >= acntSTPtr->numEntrySL)
         { /*If: out of boundes*/
            printf(
               "%li > number entries in account (%li)%s",
               indexSL + 1,
               acntSTPtr->numEntrySL,
               str_endLine
            );

            continue;
         } /*If: out of boundes*/

         else if(indexSL < 0)
         { /*Else If: negative index*/
            printf(
               "input index must be 0 or greater%s",
               str_endLine
            );

            continue;
         } /*Else If: negative index*/

         else
            break; /*valid input*/
      }  /*If: valid input*/

      else
      { /*Else: invalid input*/         
         printf(
            "%s is non-numeric  or to large%s",
            inStr,
            str_endLine
         );
         continue;
      } /*Else: invalid input*/         
   } /*Loop: get amount input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec03:
   ^   - delete entry
   ^   o fun02 sec02 sub01:
   ^     - get user input and check if valid
   ^   o fun02 sec02 sub02:
   ^     - convert input and check if valid number
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   printf(
      "getting effect of deletion%s",
      str_endLine
   );

   delIndex_st_acnt(
      acntSTPtr,
      indexSL,
      percisionUC,
      stdout
   );

   while(0 == 0)
   { /*Loop: confirm changes*/
      printf(
         "ok wih deletion [y for yes; n for no]%s",
         str_endLine
      );
      
      tmpStr =
         (signed char *)
         fgets(
            (char *) inStr,
            def_inBuffLen_cmdAcnt,
            stdin
         );

      if(*inStr == 'y')
      { /*If: user said yes*/
         printf(
            "deleting index %li%s",
            indexSL,
            str_endLine
         );

         delIndex_st_acnt(
             acntSTPtr,
             indexSL,
             percisionUC,
             0
         );

         break;
      } /*If: user said yes*/

      else if(inStr[0] == 'n')
      { /*Else If: user said no*/
         printf(
            "not deleting index %li%s",
            indexSL,
            str_endLine
         );
         goto quite_fun02_sec04;
      } /*Else If: user said no*/

      else
         printf(
            "input y to delete entry; n to quite%s",
            str_endLine
         );
   } /*Loop: confirm changes*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun02 Sec04:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return;

   quite_fun02_sec04:;
      return;
} /*delEntry_cmdAcnt*/

/*-------------------------------------------------------\
| Fun03: pEntries_cmdAcnt
|   - prints entries for an account
| Input:
|   - acntSTPtr:
|     o pointer to st_acnt struct to print entries for
|   - persisionUC:
|     o number decimals to print (prentend/mock delete)
| Output:
|   - Prints:
|     o targeted account entries to stdout
|     o gets user input form stdin
\-------------------------------------------------------*/
signed char
pEntries_cmdAcnt(
   struct st_acnt *acntSTPtr,
   unsigned char percisionUC  /*amount of percision*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun03 TOC:
   '   - prints entries matching input in accounts
   '   o fun03 sec01:
   '     - variable declarations
   '   o fun03 sec02:
   '     - initialize arrys
   '   o fun03 sec03:
   '     - get parent account
   '   o fun03 sec04:
   '     - get child account for transaction
   '   o fun03 sec05:
   '     - get year of transaction
   '   o fun03 sec06:
   '     - get month range to search for
   '   o fun03 sec07:
   '     - get day of transaction
   '   o fun03 sec08:
   '     - get if summing entries
   '   o fun03 sec09:
   '     - print entries
   '   o fun03 sec10:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char parStr[def_lenStr_strAry];
   signed char *inParStr = 0;
   signed char childStr[def_lenStr_strAry];
   signed char *inChildStr = 0;

   signed char inStr[def_inBuffLen_cmdAcnt];

   signed short yearArySS[2];
   signed char monthArySC[2];
   signed char dayArySC[2];

   signed char sumBl = 0;
   signed char *tmpStr = 0;
   signed short tmpSS = 0;

   /*for getting date ranges from user*/
   signed short startSS = 0;
   signed short endSS = 0;
   signed char startSC = 0;
   signed char endSC = 0;

   signed long indexSL = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec02:
   ^   - initialize arrys
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   yearArySS[0] = 0;
   yearArySS[1] = 0;

   monthArySC[0] = 0;
   monthArySC[1] = 0;

   dayArySC[0] = 0;
   dayArySC[1] = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec03:
   ^   - get parent account
   ^   o fun03 sec03 sub01:
   ^     - prompt user and check if got input
   ^   o fun03 sec03 sub02:
   ^     - check if quiting or listing parent accounts
   ^   o fun03 sec03 sub03:
   ^     - check if index for account
   ^   o fun03 sec03 sub04:
   ^     - else account name input
   ^   o fun03 sec03 sub05:
   ^     - echo parent account name back to user
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec03 Sub01:
   *   - prompt user and check if got input
   \*****************************************************/

   tmpStr = inStr;
   *tmpStr = ' ';
   parStr[0] = '\0';

   while(*tmpStr != '\0')
   { /*Loop: get parent account*/
     printf("parent account; q quite; l list; i index;");
     printf(" - to skip:");
     printf("%s", str_endLine);

      tmpStr =
         (signed char *)
         fgets(
            (char *) inStr,
            def_inBuffLen_cmdAcnt,
            stdin
         );

      if(
            ! tmpStr
         || *tmpStr == '\0'
         || *tmpStr == '\n'
         || *tmpStr == '\r'
      ){ /*If: nothing input*/
         printf(
            "nothing input for parent account%s",
             str_endLine
         );
         tmpStr = inStr;
         *tmpStr = ' ';
         continue;
      } /*If: nothing input*/

      /**************************************************\
      * Fun03 Sec03 Sub02:
      *   - check if quiting or listing parent accounts
      \**************************************************/

      else if(
             (tmpStr[0] & ~32) == 'Q'
          && tmpStr[1] < 33
      ) goto quite_fun03_sec10;

      else if(
             tmpStr[0] == '-'
          && tmpStr[1] < 33
      ){ /*Else If: skipping parent entry*/
         parStr[0] = '\0';
         break;
      }  /*Else If: skipping parent entry*/

      else if(
             (tmpStr[0] & ~32) == 'L'
          && tmpStr[1] < 33
      ){ /*Else If: wanted list*/
         for(
            indexSL = 0;
            indexSL < (signed long) acntSTPtr->numParUS;
            ++indexSL
         ){ /*Loop: print parent account ids*/
             printf(
                "  %li: %s parent account%s",
                indexSL,
                get_strAry(
                   acntSTPtr->uniqParStr,
                   indexSL
                ),
                str_endLine
             );
         }  /*Loop: print parent account ids*/
      }  /*Else If: wanted list*/

      /**************************************************\
      * Fun03 Sec03 Sub03:
      *   - check if index for account
      \**************************************************/

      else if(
             (tmpStr[0] & ~32) == 'I'
          && tmpStr[1] < 33
      ){ /*Else If: index input*/

         while(*tmpStr < 33)
         { /*Loop: get to index*/
            if(*tmpStr == '\0')
               break;

            ++tmpStr;
         } /*Loop: get to index*/

         tmpSS = tmpStr - inStr;
         tmpStr = inStr + 2;

         tmpStr +=
            strToSL_base10str(
               tmpStr,
               &indexSL
            );


         if(*tmpStr > 32)
         { /*If: non-numeric value*/
            printf(
               "%s is non-numeric or to large%s",
               &inStr[tmpSS],
               str_endLine
            );

            continue;
         } /*If: non-numeric value*/


         else if(
            indexSL > (signed long) acntSTPtr->numParUS
         ){ /*Else If: index does not exist*/
            printf(
               "no parent account at index %li%s",
               indexSL,
               str_endLine
            );

            continue;
         } /*Else If: index does not exist*/


         else
         { /*Else: valid index*/
            cpStr_ulCp(
               parStr,
               get_strAry(
                  acntSTPtr->uniqParStr,
                  indexSL
               )
            );

            break;
         } /*Else: valid index*/
      } /*Else If: index input*/

      /**************************************************\
      * Fun03 Sec03 Sub04:
      *   - else account name input
      \**************************************************/

      else
      { /*Else: input parent account name*/
         tmpSS = endLine_ulCp(inStr);

         if(tmpSS >= def_lenStr_strAry - 1)
         { /*If: account name out of bounds*/
            printf(
               "parent account name %s is to long%s",
               inStr,
               str_endLine
            );
            continue;
         } /*If: account name out of bounds*/

         cpWhite_ulCp(
            parStr,
            inStr
         );

         break;
      } /*Else: input parent account name*/
   } /*Loop: get parent account*/

   /*****************************************************\
   * Fun03 Sec03 Sub05:
   *   - echo parent account name back to user
   \*****************************************************/

   if(parStr[0] != '\0')
   { /*If: parent account added*/
      inParStr = parStr;

      printf(
         "parent account set to %s%s",
         parStr,
         str_endLine
      );
   } /*If: parent account added*/

   else
   { /*Else: no parent added*/
      inParStr = 0;

      printf(
         "searchign all parent accounts%s",
         str_endLine
      );
   } /*Else: no parent added*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec04:
   ^   - get child account for transaction
   ^   o fun03 sec04 sub01:
   ^     - prompt user and check if got input
   ^   o fun03 sec04 sub02:
   ^     - check if quiting or listing child accounts
   ^   o fun03 sec04 sub03:
   ^     - check if index for account
   ^   o fun03 sec04 sub04:
   ^     - else account name input
   ^   o fun03 sec04 sub05:
   ^     - echo child account name back to user
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec04 Sub01:
   *   - prompt user and check if got input
   \*****************************************************/

   tmpStr = inStr;
   *tmpStr = ' ';
   childStr[0] = '\0';

   while(*tmpStr != '\0')
   { /*Loop: get child account*/
     printf("child account; q quite; l list; i index;");
     printf(" - to skip:");
     printf("%s", str_endLine);

      tmpStr =
         (signed char *)
         fgets(
            (char *) inStr,
            def_inBuffLen_cmdAcnt,
            stdin
         );

      if(
            ! tmpStr
         || *tmpStr == '\0'
         || *tmpStr == '\n'
         || *tmpStr == '\r'
      ){ /*If: nothing input*/
         printf(
            "nothing input for child account%s",
            str_endLine
         );
         tmpStr = inStr;
         *tmpStr = ' ';
         continue;
      } /*If: nothing input*/

      /**************************************************\
      * Fun03 Sec04 Sub02:
      *   - check if quiting or listing child accounts
      \**************************************************/

      else if(
             (tmpStr[0] & ~32) == 'Q'
          && tmpStr[1] < 33
      ) goto quite_fun03_sec10;

      else if(
             tmpStr[0] == '-'
          && tmpStr[1] < 33
      ){ /*Else If: skipping parent entry*/
         childStr[0] = '\0';
         break;
      }  /*Else If: skipping parent entry*/

      else if(
             (tmpStr[0] & ~32) == 'L'
          && tmpStr[1] < 33
      ){ /*Else If: wanted list*/
         for(
            indexSL = 0;
            indexSL < (signed long) acntSTPtr->numChildUS;
            ++indexSL
         ){ /*Loop: print parent account ids*/
             printf(
                "  %li: %s child account%s",
                indexSL,
                get_strAry(
                   acntSTPtr->uniqChildStr,
                   indexSL
                ),
                str_endLine
             );
         }  /*Loop: print parent account ids*/
      }  /*Else If: wanted list*/

      /**************************************************\
      * Fun03 Sec04 Sub03:
      *   - check if index for account
      \**************************************************/

      else if(
             (tmpStr[0] & ~32) == 'I'
          && tmpStr[1] < 33
      ){ /*Else If: index input*/

         while(*tmpStr < 33)
         { /*Loop: get to index*/
            if(*tmpStr == '\0')
               break;

            ++tmpStr;
         } /*Loop: get to index*/

         tmpSS = tmpStr - inStr;
         tmpStr = inStr + 2;

         tmpStr +=
            strToSL_base10str(
               tmpStr,
               &indexSL
            );


         if(*tmpStr > 32)
         { /*If: non-numeric value*/
            printf(
               "%s is non-numeric or to large%s",
               &inStr[tmpSS],
               str_endLine
            );

            continue;
         } /*If: non-numeric value*/


         else if(
            indexSL > (signed long) acntSTPtr->numChildUS
         ){ /*Else If: index does not exist*/
            printf(
               "no child account at index %li%s",
               indexSL,
               str_endLine
            );

            continue;
         } /*Else If: index does not exist*/


         else
         { /*Else: valid index*/
            cpStr_ulCp(
               childStr,
               get_strAry(
                  acntSTPtr->uniqChildStr,
                  indexSL
               )
            );

            break;
         } /*Else: valid index*/
      } /*Else If: index input*/

      /**************************************************\
      * Fun03 Sec04 Sub04:
      *   - else account name input
      \**************************************************/

      else
      { /*Else: input parent account name*/
         tmpSS = endLine_ulCp(inStr);

         if(tmpSS >= def_lenStr_strAry - 1)
         { /*If: account name out of bounds*/
            printf(
               "child account name %s is to long%s",
               inStr,
               str_endLine
            );
            continue;
         } /*If: account name out of bounds*/

         cpWhite_ulCp(
            childStr,
            inStr
         );

         break;
      } /*Else: input parent account name*/
   } /*Loop: get child account*/
   
   /*****************************************************\
   * Fun03 Sec04 Sub05:
   *   - echo child account name back to user
   \*****************************************************/

   if(childStr[0] != '\0')
   { /*If: childe account added*/
      inChildStr = childStr;

      printf(
         "child account set to %s%s",
         childStr,
         str_endLine
      );
   } /*If: child account added*/

   else
   { /*Else: no child added*/
      inChildStr = 0;

      printf(
         "searchign all child accounts%s",
         str_endLine
      );
   } /*Else: no child added*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec05:
   ^   - get year range to search for
   ^   o fun03 sec05 sub01:
   ^     - get input and do initial checks
   ^   o fun03 sec05 sub02:
   ^     - get starting year or starting,ending year
   ^   o fun03 sec05 sub03:
   ^     - get ending year (no starting)
   ^   o fun03 sec05 sub04:
   ^     - echo input
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec05 Sub01:
   *   - get input and do initial checks
   \*****************************************************/

   tmpStr = inStr;
   *tmpStr = ' ';

   while(*tmpStr != '\0')
   { /*Loop: get year*/
      yearArySS[0] = 0;
      yearArySS[1] = 0;

      printf(
        "year [all] (start,end; ,end; start; q quite;%s",
        str_endLine
      );

      printf(
        "  use 0 for any year%s",
        str_endLine
      );

      tmpStr =
         (signed char *)
         fgets(
            (char *) inStr,
            def_inBuffLen_cmdAcnt,
            stdin
         );

      if(
            ! tmpStr
         || *tmpStr == '\0'
         || *tmpStr == '\n'
         || *tmpStr == '\r'
      ) break; /*using all years*/

      else if(
             tmpStr[0] == 'q'
          && tmpStr[1] < 33
      ) goto quite_fun03_sec10;

      /**************************************************\
      * Fun03 Sec05 Sub02:
      *   - get starting year or starting,ending year
      \**************************************************/

      if(*tmpStr != ',')
      { /*If: have first year*/
         tmpStr +=
            strToSS_base10str(
               inStr,
               &startSS
            );

          if(startSS < 0)
          { /*If: negative year input*/
             fprintf(
                stdout,
                "year must be at least 0; any year%s",
                str_endLine
             );
             continue;
          } /*If: negative year input*/

          else if(
                ! tmpStr
             || *tmpStr == '\0'
             || *tmpStr == '\n'
             || *tmpStr == '\r'
          ){ /*Else If: only starting year input*/
             yearArySS[0] = startSS;
             break;
          }  /*Else If: only starting year input*/

          else if(*tmpStr != ',')
          { /*Else If: invalid input*/
             printf(
                "starting year is %s non-numeric%s",
                inStr,
                str_endLine
             );

             continue;
          } /*Else If: invalid input*/

          else
          { /*Else: have ending year*/
             tmpStr +=
                strToSS_base10str(
                   inStr,
                   &endSS
                );

              if(
                    ! tmpStr
                 && *tmpStr != '\0'
                 && *tmpStr != '\n'
                 && *tmpStr != '\r'
              ){ /*If: invalid input*/
                 printf(
                    "end year is non-numeric %s",
                    str_endLine
                 );

                 continue;
              }  /*If: invalid input*/

              else if(endSS < 0)
              { /*Else If: negative year input*/
                 fprintf(
                    stdout,
                    "year must be at least 0; any year%s",
                    str_endLine
                 );
                 continue;
              } /*Else If: negative year input*/

              else
              { /*Else: have year range*/
                 yearArySS[0] = startSS;
                 yearArySS[1] = endSS;
                 break;
              } /*Else: have year range*/
          } /*Else: have ending year*/
      } /*If: have first year*/

      /**************************************************\
      * Fun03 Sec05 Sub03:
      *   - get ending year (no starting)
      \**************************************************/

      else
      { /*Else: only ending year*/
         tmpStr = inStr + 1;
         tmpStr +=
            strToSS_base10str(
               tmpStr,
               &endSS
            );

            if(
                  ! tmpStr
               && *tmpStr != '\0'
               && *tmpStr != '\n'
               && *tmpStr != '\r'
            ){ /*If: invalid input*/
               printf(
                  "ending year is non-numeric%s",
                  str_endLine
               );

               continue;
            }  /*If: invalid input*/

            else if(endSS < 0)
            { /*Else If: negative year input*/
               fprintf(
                  stdout,
                  "year must be at least 0; any year%s",
                  str_endLine
               );
               continue;
            } /*Else If: negative year input*/

            else
            { /*Else: valid year input*/
               yearArySS[1] = endSS;
               break;
            } /*Else: valid year input*/
      } /*Else: only ending year*/
   } /*Loop: get year*/

   /*****************************************************\
   * Fun03 Sec05 Sub04:
   *   - echo input
   \*****************************************************/

   if(yearArySS[0] <= 0)
      yearArySS[0] = 0;

   if(yearArySS[1] <= 0)
      yearArySS[1] = 0;

   if(
         yearArySS[0] == 0
      && yearArySS[1] == 0
   ){ /*If: all year search*/
      printf(
         "searching for all years%s",
         str_endLine
      );
   } /*If: all year search*/

   else if(yearArySS[0] == 0)
   { /*Else If: all starting years*/
      printf(
         "searching for all years at or before %i%s",
         yearArySS[1],
         str_endLine
      );
   } /*Else If: all starting years*/

   else if(yearArySS[1] == 0)
   { /*Else If: all ending years*/
      printf(
         "searching for all years at or after %i%s",
         yearArySS[0],
         str_endLine
      );
   } /*Else If: all ending years*/

   else
   { /*Else If: strict range*/
      printf(
         "searching for all years bewtween %i and  %i%s",
         yearArySS[0],
         yearArySS[1],
         str_endLine
      );
   } /*Else If: strict range*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec06:
   ^   - get month range to search for
   ^   o fun03 sec06 sub01:
   ^     - get input and do initial checks
   ^   o fun03 sec06 sub02:
   ^     - get starting month or starting,ending month
   ^   o fun03 sec06 sub03:
   ^     - get ending month (no starting)
   ^   o fun03 sec06 sub04:
   ^     - echo input month range back to user
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec06 Sub01:
   *   - get input and do initial checks
   \*****************************************************/

   tmpStr = inStr;
   *tmpStr = ' ';

   while(*tmpStr != '\0')
   { /*Loop: get month*/
      monthArySC[0] = 0;
      monthArySC[1] = 0;

      printf(
         "month (start,end; start; ,end; q to quite):%s",
         str_endLine
      );

      printf(
        "  use 0 for any month%s",
        str_endLine
      );

      tmpStr =
         (signed char *)
         fgets(
            (char *) inStr,
            def_inBuffLen_cmdAcnt,
            stdin
         );

      if(
            ! tmpStr
         || *tmpStr == '\0'
         || *tmpStr == '\n'
         || *tmpStr == '\r'
      ) break;

      else if(
             tmpStr[0] == 'q'
          && tmpStr[1] < 33
      ) goto quite_fun03_sec10;

      /**************************************************\
      * Fun03 Sec06 Sub02:
      *   - get starting month or starting,ending month
      \**************************************************/

      if(*tmpStr != ',')
      { /*If: have first month*/
         tmpStr +=
            strToSC_base10str(
               inStr,
               &startSC
            );

          if(startSC > 12)
          { /*If: month out of range*/
             printf(
                "only 12 months in a year%s",
                str_endLine
             );

             continue;
          } /*If: invalid input*/

          else if(startSC < 0)
          { /*Else If: month out of range*/
             printf(
                "month must be 0 (any month) or >%s",
                str_endLine
             );

             continue;
          } /*Else If: invalid input*/

          else if(
                ! tmpStr
             || *tmpStr == '\0'
             || *tmpStr == '\n'
             || *tmpStr == '\r'
          ){ /*Else If: only start input*/
             monthArySC[0] = startSC;
             break;
          }  /*Else If: only start input*/

          else if(*tmpStr != ',')
          { /*If: invalid input*/
             printf(
                "starting month is %s non-numeric%s",
                inStr,
                str_endLine
             );

             continue;
          } /*If: invalid input*/

          else
          { /*Else: have ending month*/
             tmpStr +=
                strToSC_base10str(
                   inStr,
                   &endSC
                );

              if(
                    ! tmpStr
                 && *tmpStr != '\0'
                 && *tmpStr != '\n'
                 && *tmpStr != '\r'
              ){ /*If: invalid input*/
                 printf(
                    "end month is non-numeric %s",
                    str_endLine
                 );

                 continue;
              }  /*If: invalid input*/

              else if(endSC > 12)
              { /*Else If: month out of range*/
                 printf(
                    "only 12 months in a year%s",
                    str_endLine
                 );

                 continue;
              } /*Else If: invalid input*/

              else if(endSC < 0)
              { /*Else If: month out of range*/
                 printf(
                    "month must be 0 (any month) or >%s",
                    str_endLine
                 );

                 continue;
              } /*Else If: invalid input*/

              else
              { /*Else: valid month range input*/
                 monthArySC[0] = startSC;
                 monthArySC[1] = endSC;
                 break;
              } /*Else: valid month range input*/
          } /*Else: have ending month*/
      } /*If: have first month*/

      /**************************************************\
      * Fun03 Sec06 Sub03:
      *   - get ending month (no starting)
      \**************************************************/

      else
      { /*Else: only ending month*/
         tmpStr = inStr + 1;
         tmpStr +=
            strToSC_base10str(
               tmpStr,
               &endSC
            );

         if(
               ! tmpStr
            && *tmpStr != '\0'
            && *tmpStr != '\n'
            && *tmpStr != '\r'
         ){ /*If: invalid input*/
            printf(
               "ending month is non-numeric%s",
               str_endLine
            );

            continue;
         }  /*If: invalid input*/

         else if(endSC < 0)
         { /*Else If: month out of range*/
            printf(
               "month must be 0 (any month) or >%s",
               str_endLine
            );

            continue;
         } /*Else If: invalid input*/

         else if(endSC > 12)
         { /*Else: invalid input*/         
            printf(
               "only 12 months in a year%s",
               str_endLine
            );
            continue;
         } /*Else: invalid input*/         

         else
         { /*Else: valid month input*/
            monthArySC[1] = endSC;
            break;
         } /*Else: valid month input*/
      } /*Else: only ending month*/
   } /*Loop: get month*/

   /*****************************************************\
   * Fun03 Sec06 Sub04:
   *   - echo input month range back to user
   \*****************************************************/

   if(monthArySC[0] <= 0)
      monthArySC[0] = 0;

   if(monthArySC[1] <= 0)
      monthArySC[1] = 0;


   if(
         monthArySC[0] == 0
      && monthArySC[1] == 0
   ){ /*If: all month search*/
      printf(
         "searching for all months%s",
         str_endLine
      );
   } /*If: all month search*/

   else if(monthArySC[0] == 0)
   { /*Else If: all starting months*/
      printf(
         "searching for all months at or before %i%s",
         monthArySC[1],
         str_endLine
      );
   } /*Else If: all starting months*/

   else if(monthArySC[1] == 0)
   { /*Else If: all ending months*/
      printf(
         "searching for all months at or after %i%s",
         monthArySC[0],
         str_endLine
      );
   } /*Else If: all ending months*/

   else
   { /*Else If: strict range*/
      printf(
         "searching for all months bewtween %i and  %i%s",
         monthArySC[0],
         monthArySC[1],
         str_endLine
      );
   } /*Else If: strict range*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec07:
   ^   - get day of transaction
   ^   o fun03 sec07 sub01:
   ^     - get input and do initial checks
   ^   o fun03 sec07 sub02:
   ^     - get starting day or starting,ending day
   ^   o fun03 sec07 sub03:
   ^     - get ending day (no starting)
   ^   o fun03 sec07 sub04:
   ^     - echo input day range back to user
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun03 Sec07 Sub01:
   *   - get input and do initial checks
   \*****************************************************/

   tmpStr = inStr;
   *tmpStr = ' ';

   while(*tmpStr != '\0')
   { /*Loop: get day*/
      dayArySC[0] = 0;
      dayArySC[1] = 0;

      printf(
         "day (start,end; start; ,end; q to quite):%s",
         str_endLine
      );

      printf(
        "  use 0 for any day%s",
        str_endLine
      );

      tmpStr =
         (signed char *)
         fgets(
            (char *) inStr,
            def_inBuffLen_cmdAcnt,
            stdin
         );

      if(
            ! tmpStr
         || *tmpStr == '\0'
         || *tmpStr == '\n'
         || *tmpStr == '\r'
      ) break;

      else if(
             tmpStr[0] == 'q'
          && tmpStr[1] < 33
      ) goto quite_fun03_sec10;

      /**************************************************\
      * Fun03 Sec07 Sub02:
      *   - get starting day or starting,ending day
      \**************************************************/

      if(*tmpStr != ',')
      { /*If: have first day*/
         tmpStr +=
            strToSC_base10str(
               inStr,
               &startSC
            );

          if(startSC > 31)
          { /*If: day out of range*/
             printf(
                "only 31 days in a month%s",
                str_endLine
             );

             continue;
          } /*If: invalid input*/

          else if(startSC < 0)
          { /*Else: invalid input*/         
             printf(
                "day must be 0 (any day) or >%s",
                str_endLine
             );
             continue;
          } /*Else: invalid input*/         

          else if(
                ! tmpStr
             || *tmpStr == '\0'
             || *tmpStr == '\n'
             || *tmpStr == '\r'
          ){ /*Else If: only starting day input*/
             dayArySC[0] = startSC;
             break;
          }  /*Else If: only starting day input*/

          else if(*tmpStr != ',')
          { /*Else If: invalid input*/
             printf(
                "starting day is %s non-numeric%s",
                inStr,
                str_endLine
             );

             continue;
          } /*Else If: invalid input*/

          else
          { /*Else: have ending day*/
             tmpStr +=
                strToSC_base10str(
                   inStr,
                   &endSC
                );

              if(
                    ! tmpStr
                 && *tmpStr != '\0'
                 && *tmpStr != '\n'
                 && *tmpStr != '\r'
              ){ /*If: invalid input*/
                 printf(
                    "end day is non-numeric %s",
                    str_endLine
                 );

                 continue;
              }  /*If: invalid input*/

              else if(endSC > 31)
              { /*Else If: day out of range*/
                 printf(
                    "only 31 days in a month%s",
                    str_endLine
                 );

                 continue;
              } /*Else If: invalid input*/

              else if(endSC < 0)
              { /*Else: invalid input*/         
                 printf(
                    "day must be 0 (any day) or >%s",
                    str_endLine
                 );
                 continue;
              } /*Else: invalid input*/         

              else
              { /*Else: valid day range input*/
                 dayArySC[0] = startSC;
                 dayArySC[1] = endSC;
                 break;
              } /*Else: valid day range input*/
          } /*Else: have ending day*/
      } /*If: have first day*/

      /**************************************************\
      * Fun03 Sec07 Sub03:
      *   - get ending day (no starting)
      \**************************************************/

      else
      { /*Else: only ending day*/
         tmpStr = inStr + 1;
         tmpStr +=
            strToSC_base10str(
               tmpStr,
               &endSC
            );

         if(
               ! tmpStr
            && *tmpStr != '\0'
            && *tmpStr != '\n'
            && *tmpStr != '\r'
         ){ /*If: invalid input*/
            printf(
               "ending day is non-numeric%s",
               str_endLine
            );
   
            continue;
         }  /*If: invalid input*/
   
         else if(endSC > 31)
         { /*Else: invalid input*/         
            printf(
               "only 31 days in a month%s",
               str_endLine
            );
            continue;
         } /*Else: invalid input*/         
   
         else if(endSC < 0)
         { /*Else: invalid input*/         
            printf(
               "day must be 0 (any day) or >%s",
               str_endLine
            );
            continue;
         } /*Else: invalid input*/         
   
         else
         { /*Else: valid ending day*/
            dayArySC[1] = endSC;
            break;
         } /*Else: valid ending day*/
      } /*Else: only ending day*/
   } /*Loop: get day*/

   /*****************************************************\
   * Fun03 Sec07 Sub04:
   *   - echo input day range back to user
   \*****************************************************/

   if(dayArySC[0] <= 0)
      dayArySC[0] = 0;

   if(dayArySC[1] <= 0)
      dayArySC[1] = 0;


   if(
         dayArySC[0] == 0
      && dayArySC[1] == 0
   ){ /*If: all day search*/
      printf(
         "searching for all days%s",
         str_endLine
      );
   } /*If: all day search*/

   else if(dayArySC[0] == 0)
   { /*Else If: all starting days*/
      printf(
         "searching for all days at or before %i%s",
         dayArySC[1],
         str_endLine
      );
   } /*Else If: all starting days*/

   else if(dayArySC[1] == 0)
   { /*Else If: all ending days*/
      printf(
         "searching for all days at or after %i%s",
         dayArySC[0],
         str_endLine
      );
   } /*Else If: all ending days*/

   else
   { /*Else If: strict range*/
      printf(
         "searching for all days bewtween %i and  %i%s",
         dayArySC[0],
         dayArySC[1],
         str_endLine
      );
   } /*Else If: strict range*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec08:
   ^   - get if summing entries
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpStr = inStr;
   *tmpStr = ' ';

   while(*tmpStr != '\0')
   { /*Loop: find if summing entries*/
      dayArySC[0] = 0;
      dayArySC[1] = 0;

      printf(
         "print sums [no]; (yes/tsv/no): q to quite):%s",
         str_endLine
      );

      tmpStr =
         (signed char *)
         fgets(
            (char *) inStr,
            def_inBuffLen_cmdAcnt,
            stdin
         );

      if(
            ! tmpStr
         || *tmpStr == '\0'
         || *tmpStr == '\n'
         || *tmpStr == '\r'
      ) break;

      else if(
             tmpStr[0] == 'q'
          && tmpStr[1] < 33
      ) goto quite_fun03_sec10;

      else if(*tmpStr == 'y')
      { /*Else If: printing sums*/
         sumBl = 2;
         break;
      } /*Else If: printing sums*/

      else if(*tmpStr == 't')
      { /*Else If: printing sums*/
         sumBl = 1;
         break;
      } /*Else If: printing sums*/

      else if(*tmpStr == 'n')
      { /*Else If: printing sums*/
         sumBl = 0;
         break;
      } /*Else If: printing sums*/

      else
         fprintf(
            stdout,
            "input y (yes) or n (no)%s",
            str_endLine
         );
   } /*Loop: find if summing entries*/

   if(sumBl)
      fprintf(
         stdout,
         "summing found entries%s",
         str_endLine
      );
   if(sumBl)
      fprintf(
         stdout,
         "printing (not summing) found entries%s",
         str_endLine
      );
 
   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec09:
   ^   - print entries
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpSS =
      pEntries_st_acnt(
         acntSTPtr,
         inParStr,
         inChildStr,
         yearArySS,
         monthArySC,
         dayArySC,
         percisionUC,
         sumBl,
         stdout
      );

    if(tmpSS == def_memErr_acnt)
       return 1;

    else if(tmpSS)
    { /*Else: could not find entry*/
       printf(
          "no entries for:%s",
          str_endLine
       );

       if(inParStr)
          printf(
             "  parent: %s%s",
             parStr,
             str_endLine
          );
       else
          printf(
             "  parent: all%s",
             str_endLine
          );

       if(inChildStr)
          printf(
             "  child: %s%s",
             childStr,
             str_endLine
          );
       else
          printf(
             "  child: all%s",
             str_endLine
          );

       printf("  spanning years:");

       if(yearArySS[0] == 0)
          printf(" all,");
       else
          printf(" %i,", yearArySS[0]);

       if(yearArySS[1] == 0)
          printf("all%s", str_endLine);
       else
          printf("%i%s", yearArySS[1], str_endLine);


       printf("  spanning months:");

       if(monthArySC[0] == 0)
          printf(" all,");
       else
          printf(" %i,", monthArySC[0]);

       if(monthArySC[1] == 0)
          printf("all%s", str_endLine);


       printf("  spanning days:");

       if(dayArySC[0] == 0)
          printf(" all,");
       else
          printf(" %i,", dayArySC[0]);

       if(dayArySC[1] == 0)
          printf("all%s", str_endLine);
    } /*Else: could not find entry*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun03 Sec10:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   quite_fun03_sec10:;
      return 0;
} /*pEntries_cmdAcnt*/

/*-------------------------------------------------------\
| Fun04: divy_cmdAcnt
|   - runs divy command from user input
| Input:
|   - acntSTPtr:
|     o pointer to st_acnt struct to print entries for
|   - divyFileStr:
|     o path to divy file to use (use 0 for no divy file)
|   - ghostBl:
|     o apply rounding (up for negative; down for postive)
|       * 1 apply rounding
|       * 0 do not round
|   - persisionUC:
|     o number decimals to print (prentend/mock delete)
| Output:
|   - Prints:
|     o divy to stdout
|     o gets input from stdin
|   - Modifies:
|     o acntSTPtr to have divy
|   - Returns:
|     o 0 for no errors
|     o 1 for memory error
\-------------------------------------------------------*/
signed char
divy_cmdAcnt(
   struct st_acnt *acntSTPtr,
   signed char *divyFileStr, /*path to divy file to use*/
   signed char ghostBl,      /*apply ghost account*/
   unsigned char percisionUC /*percision to print at*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun04 TOC:
   '   - runs divy command from user input
   '   o fun04 sec01:
   '     - variable declartions
   '   o fun04 sec02:
   '     - get amount to divy
   '   o fun04 sec03:
   '     - get comment
   '   o fun04 sec05:
   '     - get divy file and do divy
   '   o fun04 sec06:
   '     - apply divy
   '   o fun04 sec07:
   '     - return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec01:
   ^   - variable declartions
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0;
   signed long errSL = 0;

   signed char inStr[def_inBuffLen_cmdAcnt];
   signed char commentStr[def_inBuffLen_cmdAcnt];

   signed char divyBuffStr[def_inBuffLen_cmdAcnt];
   signed char *divyContentsHeapStr = 0;
      /*holds entire divy file*/

   signed char *tmpStr = 0;
   unsigned long indexUL = 0;

   float amountF = 0;

   FILE *divyFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec02:
   ^   - get amount to divy
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpStr = inStr;
   *tmpStr = ' ';

   while(0 == 0)
   { /*Loop: get amount input*/
      printf("amount or q to quite:%s", str_endLine);

      tmpStr =
         (signed char *)
         fgets(
            (char *) inStr,
            def_inBuffLen_cmdAcnt,
            stdin
         );

      if(
            ! tmpStr
         || *tmpStr == '\0'
         || *tmpStr == '\n'
         || *tmpStr == '\r'
      ){ /*If: nothing input*/
         printf(
            "nothing input for amount%s",
            str_endLine
         );
         tmpStr = inStr;
         *tmpStr = ' ';
         continue;
      } /*If: nothing input*/

      else if(
             (tmpStr[0] & ~32) == 'Q'
          && tmpStr[1] < 33
      ) goto quite_fun04_sec07;

      tmpStr +=
         strToF_base10str(
            inStr,
            &amountF
         );

      if(*tmpStr == '\0')
         break;
      else if(*tmpStr == '\n')
         break;
      else if(*tmpStr == '\r')
         break;
      else
      { /*Else: invalid input*/         
         printf(
            "%s is non-numeric  or to large%s",
            inStr,
            str_endLine
         );
         continue;
      } /*Else: invalid input*/         
   } /*Loop: get amount input*/

   printf(
      "amount for divy set to %0.2f%s",
      amountF,
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec03:
   ^   - get comment
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpStr = inStr;
   *tmpStr = ' ';

   while(*tmpStr != '\0')
   { /*loop: get amount input*/

      printf(
         "comment [NA] (q to quite):%s",
         str_endLine
      );

      tmpStr =
         (signed char *)
         fgets(
            (char *) inStr,
            def_inBuffLen_cmdAcnt,
            stdin
         );

      if(
            ! tmpStr
         || *tmpStr == '\0'
         || *tmpStr == '\n'
         || *tmpStr == '\r'
      ){ /*If: no comment input*/
         commentStr[0] = 'N';
         commentStr[1] = 'A';
         break;
      } /*If: no comment input*/

      else if(
             tmpStr[0] == 'q'
          && tmpStr[1] < 33
      ) goto quite_fun04_sec07;

      cpLine_ulCp(
         commentStr,
         inStr
      );

      tmpStr = commentStr;
      while(*tmpStr != '\0')
      { /*Loop: remove white space*/
         if(*tmpStr < 32)
            *tmpStr = '_';
         ++tmpStr;
      } /*Loop: remove white space*/

   } /*loop: get amount input*/


   printf(
      "comment set to: %s%s",
      commentStr,
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec04:
   ^   - get if apply ghost account
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   tmpStr = inStr;
   *tmpStr = ' ';

   while(*tmpStr != '\0')
   { /*loop: get amount input*/

      if(ghostBl)
         printf(
            "ghost rouding (y = yes; or n = no) [yes]:%s",
            str_endLine
         );
      else
         printf(
            "ghost rouding (y = yes; or n = no) [no]:%s",
            str_endLine
         );

      tmpStr =
         (signed char *)
         fgets(
            (char *) inStr,
            def_inBuffLen_cmdAcnt,
            stdin
         );

      if(
            ! tmpStr
         || *tmpStr == '\0'
         || *tmpStr == '\n'
         || *tmpStr == '\r'
      ) break;

      else if(
             tmpStr[0] == 'q'
          && tmpStr[1] < 33
      ) goto quite_fun04_sec07;

      else if(tmpStr[0] == 'y')
      { /*Else If: yes input*/
         ghostBl = 1;
         break;
      } /*Else If: yes input*/

      else if(tmpStr[0] == 'n')
      { /*Else If: no input*/
         ghostBl = 0;
         break;
      } /*Else If: no input*/

      else
      { /*Else: unkown input*/
         printf("unkown value;%s", str_endLine);
         printf(
            "input: y (yes), n (no) or q (quite)%s",
            str_endLine
         );
      } /*Else: unkown input*/
   } /*loop: get amount input*/


   if(ghostBl)
      printf(
         "applying ghost rouding to transcation%s",
         str_endLine
      );
   else
      printf(
         "not doing ghost rouding%s",
         str_endLine
      );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec05:
   ^   - get divy file
   ^   o fun04 sec05 sub01:
   ^     - check if input divy file vaild
   ^   o fun04 sec05 sub02:
   ^     - get user input and check if valid
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun04 Sec05 Sub02:
   *   - check if input divy file is valid
   \*****************************************************/

   if(divyFileStr)
   { /*If: have divy file*/
      divyFILE =
         fopen(
            (char *) divyFileStr,
            "r"
         );

      if(! divyFILE)
      { /*If: could not open file*/
         printf(
            "Could not open divy file (%s)%s",
            divyFileStr,
            str_endLine
         );

         divyBuffStr[0] = '\0';
      } /*If: could not open file*/

      else
      { /*Else: could open divy file*/
         cpStr_ulCp(
           divyBuffStr,
           divyFileStr
         );

         fclose(divyFILE);
         divyFILE = 0;
      } /*Else: could open divy file*/
   } /*If: have divy file*/

   /*****************************************************\
   * Fun04 Sec05 Sub02:
   *   - get user input and check if valid
   \*****************************************************/

   getDivyFile_fun04_sec05_sub02:;

   tmpStr = inStr;
   *tmpStr = ' ';

   while(*tmpStr != '\0')
   { /*Loop: get amount input*/
      if(divyBuffStr[0] != '\0')
         printf(
            "divy file to use or q to quite [%s]:%s",
            divyBuffStr,
            str_endLine
         );
      else
         printf(
            "divy file to use or q to quite:%s",
            str_endLine
         );

      tmpStr =
         (signed char *)
         fgets(
            (char *) inStr,
            def_inBuffLen_cmdAcnt,
            stdin
         );

      if(
            ! tmpStr
         || *tmpStr == '\0'
         || *tmpStr == '\r'
         || *tmpStr == '\n'
      ){ /*If: no input*/
         if(divyFileStr[0] == '\0')
         { /*If: no divy file*/
            printf(
               "no divy file input%s",
               str_endLine
            );

            continue;
         } /*If: no divy file*/

         else
            break; /*else if applying default divy*/
      }  /*If: no input*/

      else if(
             tmpStr[0] == 'q'
          && tmpStr[1] < 33
      ) goto quite_fun04_sec07;

     /*****************************************************\
     * Fun04 Sec05 Sub03:
     *   - open divy file
     \*****************************************************/

      divyFILE =
         fopen(
            (char *) inStr,
            "r"
         );
      
      if(! divyFILE)
      { /*If: could not open divy file*/
         printf(
            "unable to open %s%s",
            inStr,
            str_endLine
         );

         continue;
      } /*If: could not open divy file*/

      else
      { /*Else: have divy file*/
         fclose(divyFILE);
         divyFILE  = 0;

         cpStr_ulCp(
           divyBuffStr,
           inStr
         );

         break;
      } /*Else: have divy file*/
   } /*Loop: get amount input*/

   printf(
      "using %s to divy%s",
      divyBuffStr,
      str_endLine
   );
      

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec06:
   ^   - apply divy
   ^   o fun04 sec06 sub01:
   ^     - do mock divy run
   ^   o fun04 sec06 sub02:
   ^     - apply divy
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun04 Sec06 Sub04:
   *   - do mock divy run
   \*****************************************************/

   divyFILE =
      fopen(
         (char *) divyBuffStr,
         "r"
      );
 
   fseek(
     divyFILE,
     0,
     SEEK_END
   );

   indexUL = ftell(divyFILE);

   fseek(
     divyFILE,
     0,
     SEEK_SET
   );

   if(divyContentsHeapStr)
      free(divyContentsHeapStr);
   divyContentsHeapStr = 0;

   divyContentsHeapStr =
      malloc( (indexUL + 8) * sizeof(signed char) );
   if(! divyContentsHeapStr)
     goto memErr_fun04_sec07;

   indexUL =
      fread(
         (char *) divyContentsHeapStr,
         sizeof(signed char),
         indexUL,
         divyFILE
      );

   divyContentsHeapStr[indexUL] = '\0';

   fclose(divyFILE);
   divyFILE = 0;

   errSL =
      divy_st_acnt(
         acntSTPtr,
         amountF,
         divyContentsHeapStr,
         commentStr,
         ghostBl,
         percisionUC,
         stdout
      );

   if(errSL == def_memErr_acnt)
       goto memErr_fun04_sec07;

   else if(errSL == def_missingEntry_acnt)
   { /*Else If: missing entry*/
      printf(
         "nothing in %s%s",
         divyBuffStr,
         str_endLine
      );

      goto getDivyFile_fun04_sec05_sub02;
   } /*Else If: missing entry*/

   else if(errSL)
   { /*Else If: missing entry*/
      printf(
         "invalid entry in divy file at lien %li%s",
         errSL,
         str_endLine
      );

      goto getDivyFile_fun04_sec05_sub02;
   } /*Else If: missing entry*/

   /*****************************************************\
   * Fun04 Sec06 Sub05:
   *   - apply divy
   \*****************************************************/

   while(0 == 0)
   { /*Loop: confirm changes*/
      printf(
         "ok wih divy [y for yes; n for no]%s",
         str_endLine
      );
      
      tmpStr =
         (signed char *)
         fgets(
            (char *) inStr,
            def_inBuffLen_cmdAcnt,
            stdin
         );

      if(
            ! tmpStr
         || *tmpStr == '\0'
         || *tmpStr == '\n'
         || *tmpStr == '\r'
      ){ /*If: nothing input*/
         printf(
            "no input; input y for yes and n for no%s",
            str_endLine
         );

         continue;
      }  /*If: nothing input*/


      if(inStr[0] == 'y')
      { /*If: user said yes*/
         printf("apply divy%s", str_endLine);

         divy_st_acnt(
            acntSTPtr,
            amountF,
            divyContentsHeapStr,
            commentStr,
            ghostBl,
            percisionUC,
            0
         );

         break;
      } /*If: user said yes*/

      else if(inStr[0] == 'n')
      { /*Else If: user said no*/
         printf("not apply divy%s", str_endLine);
         goto quite_fun04_sec07;
      } /*Else If: user said no*/

      else
         printf(
            "input y to add entry; n to quite%s",
            str_endLine
         );
   } /*Loop: confirm changes*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun04 Sec07:
   ^   - return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_fun04_sec07;

   quite_fun04_sec07:;
      errSC = 0;
      goto ret_fun04_sec07;

   memErr_fun04_sec07:;
      errSC = 1;
      goto ret_fun04_sec07;

   ret_fun04_sec07:;
      if(
            divyFILE
         && divyFILE != stdin
         && divyFILE != stdout
         && divyFILE != stderr
      ) fclose(divyFILE);
      divyFILE = 0;

      if(divyContentsHeapStr)
         free(divyContentsHeapStr);
      divyContentsHeapStr = 0;

      return errSC;
} /*divy_cmdAcnt*/

/*-------------------------------------------------------\
| Fun05: write_cmdAcnt
|   - save account to file
| Input:
|   - acntSTPtr:
|     o pointer to st_acnt struct to print entries for
|   - outFileStr:
|     o path to file to save accounts to
|   - persisionUC:
|     o number decimals to print (prentend/mock delete)
| Output:
|   - Prints:
|     o accounts to input file
|     o gets input from stdin
|   - Returns:
|     o 0 for no errors
|     o 1 for memory error
\-------------------------------------------------------*/
signed char
write_cmdAcnt(
   struct st_acnt *acntSTPtr,
   signed char *outFileStr,  /*path to output file*/
   unsigned char percisionUC /*percision to print at*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun05 TOC:
   '   - save account to file
   '   o fun05 sec01:
   '     - variable declarations
   '   o fun05 sec02:
   '     - get output file
   '   o fun05 sec03:
   '     - save output to file
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed char errSC = 0;
   signed char outBuffStr[def_inBuffLen_cmdAcnt];

   signed char inStr[def_inBuffLen_cmdAcnt];
   signed char *tmpStr = 0;

   FILE *outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec02:
   ^   - get output file
   ^   o fun05 sec02 sub01:
   ^     - get output file
   ^   o fun05 sec02 sub02:
   ^     - check if file already exits
   ^   o fun05 sec02 sub03:
   ^     - write accounts to output file
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun05 Sec02 Sub01:
   *  - get output file
   \*****************************************************/

   getOutFile_fun05_sec02_sub01:;

   if(outFileStr)
      cpStr_ulCp(
         outBuffStr,
         outFileStr
      );

   else
      outBuffStr[0] = '\0';


   tmpStr = inStr;
   *tmpStr = ' ';

   while(0 == 0)
   { /*Loop: get amount input*/
      if(outBuffStr[0] != '\0')
         printf(
            "output file or q to quite [%s]:%s",
            outBuffStr,
            str_endLine
         );
      else
         printf(
            "output file to use or q to quite:%s",
            str_endLine
         );

      tmpStr =
         (signed char *)
         fgets(
            (char *) inStr,
            def_inBuffLen_cmdAcnt,
            stdin
         );

      if(
            ! tmpStr
         || *tmpStr == '\0'
         || *tmpStr == '\r'
         || *tmpStr == '\n'
      ){ /*If: no input*/
         if(outFileStr[0] == '\0')
         { /*If: no out file*/
            printf(
               "no output file input%s",
               str_endLine
            );

            continue;
         } /*If: no out file*/

         else
            break; /*else if using default output file*/
      }  /*If: no input*/

      else if(
             tmpStr[0] == 'q'
          && tmpStr[1] < 33
      ) goto quite_fun05_sec03;

      else
      { /*Else: user input something*/
         cpLine_ulCp(
            outBuffStr,
            inStr
         );

         break;
      } /*Else: user input something*/
   } /*Loop: get amount input*/

   /*****************************************************\
   * Fun05 Sec02 Sub02:
   *  - check if file already exits
   \*****************************************************/

   outFILE =
      fopen(
         (char *) outBuffStr,
         "r"
      );
   
   if(outFILE)
   { /*If: could open out file*/
      fclose(outFILE);
      outFILE = 0;

      while(0 == 0)
      { /*Loop: confirm overwrite*/
         printf(
            "file %s alreay exists; overwrite (y/n)%s",
            outBuffStr,
            str_endLine
         );
         
         tmpStr =
            (signed char *)
            fgets(
               (char *) inStr,
               def_inBuffLen_cmdAcnt,
               stdin
            );

         if(
               ! tmpStr
            || *tmpStr == '\0'
            || *tmpStr == '\n'
            || *tmpStr == '\r'
         ){ /*If: nothing input*/
            printf(
               "no input; input y for yes and n for no%s",
               str_endLine
            );

            continue;
         }  /*If: nothing input*/


         if(inStr[0] == 'y')
            break;

         else if(inStr[0] == 'n')
         { /*Else If: user said no*/
            printf("not saving file%s", str_endLine);
            goto quite_fun05_sec03;
         } /*Else If: user said no*/

         else
            printf(
               "input y to add entry; n to quite%s",
               str_endLine
            );
      } /*Loop: confirm overwrite*/
   } /*If: could open out file*/

   /*****************************************************\
   * Fun05 Sec02 Sub03:
   *  - write accounts to output file
   \*****************************************************/

   outFILE =
      fopen(
         (char *) outBuffStr,
         "w"
      );

   if(! outFILE)
   { /*If: could not open output file*/
      printf(
         "unable to open %s%s",
         outBuffStr,
         str_endLine
      );

      goto getOutFile_fun05_sec02_sub01;
   } /*If: could not open output file*/

   errSC =
      write_st_acnt(
         acntSTPtr,
         percisionUC,
         outFILE
      );

   if(errSC)
      goto memErr_fun05_sec03;

   fclose(outFILE);
   outFILE = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun05 Sec03:
   ^   - save output to file
   ^   o fun05 sec02 sub01:
   ^     - check if input output file vaild
   ^   o fun05 sec02 sub02:
   ^     - get user input and check if valid
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errSC = 0;
   goto ret_fun05_sec03;

   memErr_fun05_sec03:;
      errSC = 1;
      goto ret_fun05_sec03;

   quite_fun05_sec03:;
      errSC = 0;
      goto ret_fun05_sec03;

   ret_fun05_sec03:;
      if(
            outFILE 
         && outFILE != stdin
         && outFILE != stdout
         && outFILE != stderr
      ) fclose(outFILE);
      outFILE = 0;

      return errSC;
} /*write_cmdAcnt*/
