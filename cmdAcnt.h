/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' cmdAcnt SOF: Start Of File
'   - interactive command interface for acnt
'   o header:
'     - guards
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

/*-------------------------------------------------------\
| Header:
|   - guards
\-------------------------------------------------------*/

#ifndef INTERACTIVE_COMMAND_LINE_ACCOUNT_H
#define INTERACTIVE_COMMAND_LINE_ACCOUNT_H

#define def_inBuffLen_cmdAcnt 1024

struct st_acnt;

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
);

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
);

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
);
 
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
);

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
   signed char *outFileStr,  /*path to divy file to use*/
   unsigned char percisionUC /*percision to print at*/
);

#endif
