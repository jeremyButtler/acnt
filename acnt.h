/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' acnt SOF: Start Of File
'   - reads a account file for acnt and sets tallies
'   o header:
'     - defined variables; forward declarations, guards
'   o .h st01: 
'     - holds data for a single acount
'   o .h st02: file_acnt
'     - holds all accounts in a file
'   o fun01: blank_st_acnt
'     - blanks a st_acnt structure
'   o fun02: init_st_acnt
'     - initializes a st_acnt structure
'   o fun03: freeStack_st_acnt
'     - frees variables in a st_acnt structure
'   o fun04: freeHeap_st_acnt
'     - frees a st_acnt structure
'   o fun05: freeHeapAry_st_acnt
'     - frees an array of st_acnt structure
'   o fun06: setup_st_acnt
'     - allocates initial memory for a st_acnt structure
'   o fun07: mk_st_acnt
'     - makes a initialized and setup st_acnt structure
'   o fun08: mkAry_st_acnt
'     - makes an array of initialized and setup st_acnt
'   o fun09: reallocEntry_st_acnt
'     - resizes all arrays in a st_acnt struct
'   o fun10: reallocPar_st_acnt
'     - resizes parent account name array in st_acnt
'   o fun11: reallocChild_st_acnt
'     - resizes child account name array in st_acnt struct
'   o fun12: reallocAry_st_acnt
'     - resizes a st_acnt struct array
'   o .c fun13: swapEntry_st_acnt
'     - swaps two entries in a st_acnt strucutre
'   o .c fun14: sortDate_st_acnt
'     - sorts entry arrays in st_acnt structure by date
'   o fun15: findDate_st_acnt
'     - find closest entry index to date
'   o fun16: addNewPar_st_acnt
'     - add new parent id to st_acnt struct
'   o fun17: addNewChild_st_acnt
'     - add new child id to st_acnt struct
'   o fun18: readFile_acnt
'     - get account entries from a file
'   o fun19: addEntry_st_acnt
'     - adds entry to an account
'   o fun20: delIndex_st_acnt
'     - deletes an entry from a st_acnt struct by index
'   o fun21: pIndex_st_acnt
'     - prints entry at input index
'   o fun22: pEntries_st_acnt
'     - print all entries matching input info
'   o fun23: pSum_st_acnt
'     - print totals for all accounts
'   o fun24: write_st_acnt
'     - write to accounts file (by date)
'   o fun25: divy_st_acnt
'     - divide money into accounts by divy file
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - defined variables; forward declarations, guards
\-------------------------------------------------------*/

#ifndef ACCOUNTS_H
#define ACCOUNTS_H

/*keep these negative*/
#define def_memErr_acnt -1
#define def_noFILE_acnt -2
#define def_fileErr_acnt -4
#define def_downsize_acnt -8
#define def_missingEntry_acnt -16
#define def_notUniq_acnt -32

struct str_ptrAry; /*string pointer array*/
struct FILE;       /*avoid void*/

/*------------------------------------------------------\
| ST01: st_acnt
|   - holds data for a single acount
\------------------------------------------------------*/
typedef struct st_acnt
{
   /*parrent account names and the index to find them*/
   signed char *uniqParStr;   /*uniq parent accounts*/
   unsigned short numParUS;   /*number parent accounts*/
   unsigned short sizeParUS;  /*maximum parent accounts*/
   unsigned short *parToNumAryUS; /*parent index->entry*/
   unsigned short *numToParAryUS;
      /*entry index to parent index (get account name)*/

   /*child account names and index to find them*/
   signed char *uniqChildStr;  /*number unique accounts*/
   unsigned short numChildUS;  /*number unique accounts*/
   unsigned short sizeChildUS; /*maximum unique accounts*/
   unsigned short *numToChildAryUS; /*index for each id*/
   unsigned short *childToNumAryUS; /*index to child*/


   /*account label for each entry*/
   unsigned short *parAryUS;  /*parent account by index*/
   unsigned short *childAryUS; /*account by index*/

   /*date for each entry*/
   signed short *yearArySS;  /*year transaction made*/
   signed char *monthArySC;  /*month transaction made*/
   signed char *dayArySC;    /*day transaction made*/

   /*ballance change + totals*/
   float *tranAryF;          /*transaction amount*/

   /*using str_ptrAry; faster sorting, but more memory*/
   struct str_ptrAry *commentAryST;  /*reason why*/

   /*array length/size variables*/ 
   signed long sizeEntrySL;  /*maximum entrys in arrays*/
   signed long numEntrySL;   /*numer entrys in arrays*/
}st_acnt;

/*------------------------------------------------------\
| Fun01: blank_st_acnt
|   - blanks a st_acnt structure
| Input:
|   - acntSTPtr:
|     o pointer to st_acnt structure to blank
| Output:
|   - Modifies:
|     o sets all values in acntSTPtr to 0/'\0'
\------------------------------------------------------*/
void
blank_st_acnt(
   struct st_acnt *acntSTPtr /*to blank*/
);

/*------------------------------------------------------\
| Fun02: init_st_acnt
|   - initializes a st_acnt structure
| Input:
|   - acntSTPtr:
|     o pointer to st_acnt structure to initialize
| Output:
|   - Modifies:
|     o sets all values in acntSTPtr to 0 and all
|       pointers to null
\------------------------------------------------------*/
void
init_st_acnt(
   struct st_acnt *acntSTPtr /*to initialize*/
);

/*------------------------------------------------------\
| Fun03: freeStack_st_acnt
|   - frees variables in a st_acnt structure
| Input:
|   - acntSTPtr:
|     o pointer to st_acnt structure to free variables
| Output:
|   - Frees:
|     o all arrays and initializes
\------------------------------------------------------*/
void
freeStack_st_acnt(
   struct st_acnt *acntSTPtr /*with vars to free*/
);

/*------------------------------------------------------\
| Fun04: freeHeap_st_acnt
|   - frees a st_acnt structure
| Input:
|   - acntSTPtr:
|     o pointer to st_acnt structure to free
| Output:
|   - Frees:
|     o acntSTPtr (you must set to 0/null)
\------------------------------------------------------*/
void
freeHeap_st_acnt(
   struct st_acnt *acntSTPtr /*to free*/
);

/*------------------------------------------------------\
| Fun05: freeHeapAry_st_acnt
|   - frees an array of st_acnt structure
| Input:
|   - acntSTPtr:
|     o pointer to st_acnt structure array to free
|   - lenSL:
|     o length of acntSTPtr array to free
| Output:
|   - Frees:
|     o acntSTPtr and all its elements; you set to 0/null
\------------------------------------------------------*/
void
freeHeapAry_st_acnt(
   struct st_acnt *acntSTPtr, /*array to free*/
   signed long lenSL          /*array length*/
);


/*------------------------------------------------------\
| Fun06: setup_st_acnt
|   - allocates initial memory for a st_acnt structure
| Input:
|   - acntSTPtr:
|     o pointer to st_acnt structure setup
| Output:
|   - Modifies:
|     o all arrays to have some memory
|   - Returns:
|     o 0 for no errors
|     o def_memErr_acnt for memory errors
\------------------------------------------------------*/
signed char
setup_st_acnt(
   struct st_acnt *acntSTPtr /*to setup*/
);

/*------------------------------------------------------\
| Fun07: mk_st_acnt
|   - makes a initialized and setup st_acnt structure
| Input:
| Output:
|   - Returns:
|     o pointer to an initialized and setup  st_acnt
|     o 0 for memory errors
\------------------------------------------------------*/
struct st_acnt *
mk_st_acnt(
);

/*------------------------------------------------------\
| Fun08: mkAry_st_acnt
|   - makes an array of initialized and setup st_acnt
| Input:
|   - lenSL:
|     o number of elements in array
| Output:
|   - Returns:
|     o pointer to an initialized and setup st_acnt
|       struct array
|     o 0 for memory errors
\------------------------------------------------------*/
st_acnt *
mkAry_st_acnt(
   signed long lenSL
);

/*------------------------------------------------------\
| Fun09: reallocEntry_st_acnt
|   - resizes all arrays in a st_acnt struct
| Input:
|   - acntSTPtr:
|     o ponter to st_acnt struct to resize
|   - lenSL:
|     o number to resize to
| Output:
|     o parAryUS, childAryUS, yearArySS, monthArySC,
|       dayArySC, tarnAryF, commentAryST, acntSortArySL,
|       and parSortArySL to be resized to lenSL
|     o sizeEntrySL to be same as lenSL
|   - Returns:
|     o 0 for no errors
|     o def_memErr_acnt for memory errors
|     o def_downsize_acnt if downsizing (not allowed)
\------------------------------------------------------*/
signed char
realloc_st_acnt(
   struct st_acnt *acntSTPtr, /*struct to resize*/
   signed long lenSL          /*new array sizes*/
);

/*------------------------------------------------------\
| Fun10: reallocPar_st_acnt
|   - resizes parent account name array in st_acnt struct
| Input:
|   - acntSTPtr:
|     o ponter to st_acnt struct to resize
|   - lenUS:
|     o number to resize to
| Output:
|   - Modifies:
|     o uniqParStr and numToParAryUS arrays in acntSTPtr
|       to be resized to lenSL
|     o sizeParUS in acntSTPtr to be same as lenUS
|   - Returns:
|     o 0 for no errors
|     o def_memErr_acnt for memory errors
|     o def_downsize_acnt if downsizing (not allowed)
\------------------------------------------------------*/
signed char
reallocPar_st_acnt(
   struct st_acnt *acntSTPtr, /*struct to resize*/
   unsigned short lenUS       /*new array sizes*/
);

/*------------------------------------------------------\
| Fun11: reallocChild_st_acnt
|   - resizes child account name array in st_acnt struct
| Input:
|   - acntSTPtr:
|     o ponter to st_acnt struct to resize
|   - lenUS:
|     o number to resize to
| Output:
|   - Modifies:
|     o uniqChildStr and acntIndexAryUS arrays in acntSTPtr
|       to be resized to lenSL
|     o sizeChildUS in acntSTPtr to be same as lenUS
|   - Returns:
|     o 0 for no errors
|     o def_memErr_acnt for memory errors
|     o def_downsize_acnt if downsizing (not allowed)
\------------------------------------------------------*/
signed char
reallocChild_st_acnt(
   struct st_acnt *acntSTPtr, /*struct to resize*/
   unsigned short lenUS       /*new array sizes*/
);

/*------------------------------------------------------\
| Fun12: reallocAry_st_acnt
|   - resizes a st_acnt struct array
| Input:
|   - acntSTPtr:
|     o ponter to st_acnt struct array to resize
|   - lenSL:
|     o number of st_acnt structs to resize to
|   - sizeSL:
|     o size of old array (for initialization and setup)
| Output:
|   - Frees:
|     o acntSTPtr on success (you must assign return
|       value to acntSTPtr) [this is from realloc]
|   - Returns:
|     o pointer to rellocated acntSTPtr structure array
|     o 0 for memory errors
\------------------------------------------------------*/
st_acnt *
reallocAry_st_acnt(
   struct st_acnt *acntSTPtr, /*struct array to resize*/
   signed long lenSL,         /*new array sizes*/
   signed long sizeSL         /*size of old array*/
);

/*-------------------------------------------------------\
| Fun15: findDate_st_acnt
|   - find closest entry index to date
| Input:
|   - acntSTPtr:
|     o st_acnt pointer to search for date
|   - yearSS:
|     o year to search for
|   - monthSC:
|     o month to search for
|   - daySC:
|     o day to search for
|   - parUS:
|     o number of parent account to find
|     o use numToParAryUS in acntSTPtr to get index of
|       parent id in uniqParAryStr in acntSTPtr
|   - childUS:
|     o number of child account to find
|     o use numToChhildAryUS in acntSTPtr to get index of
|      child id in uniqChildAryStr in acntSTPtr
| Output:
|   - Returns:
|     o index of first matc (>= 0) if found result
|     o negative (< 0) closest index to date/first index
|       with date (return * -1 = closest index)
|       * always before date if no match, only exception
|         is if there is no earier date
\-------------------------------------------------------*/
signed long
findDate_st_acnt(
   struct st_acnt *acntSTPtr, /*to search for date in*/
   signed short yearSS,       /*year to find*/
   signed char monthSC,       /*month to find*/
   signed char daySC,         /*day to find*/
   unsigned short parUS,      /*parent to find*/
   unsigned short childUS     /*child to find*/
);

/*-------------------------------------------------------\
| Fun16: addNewPar_st_acnt
|   - add new parent id to st_acnt struct
| Input:
|   - parStr:
|     o c-string with new parent id to add
|   - acntSTPtr:
|     o pointer to st_acnt struct to add parent to
| Output:
|   - Returns:
|     o index of parent id in parToNumAryUS
|     o def_memErr_acnt (< 0) for memory error
\-------------------------------------------------------*/
signed int
addNewPar_st_acnt(
   signed char *parStr,       /*parent account id to add*/
   struct st_acnt *acntSTPtr  /*add parent id to*/
);

/*-------------------------------------------------------\
| Fun17: addNewChild_st_acnt
|   - add new child id to st_acnt struct
| Input:
|   - childStr:
|     o c-string with new child id to add
|   - acntSTPtr:
|     o pointer to st_acnt struct to add child to
| Output:
|   - Returns:
|     o index of child id in childToNumAryUS
|     o def_memErr_acnt (< 0) for memory error
\-------------------------------------------------------*/
signed int
addNewChild_st_acnt(
   signed char *childStr,     /*child account id to add*/
   struct st_acnt *acntSTPtr  /*add child id to*/
);

/*-------------------------------------------------------\
| Fun18: readFile_acnt
|   - get account entries from a file
| Input:
|   - acntSTPtr:
|     o pointer to initialized and setup st_acnt struct to
|       hold accounts
|   - acntFILE:
|     o FILE pointer to file with accounts
| Output:
|   - Returns:
|     o 0 for no errors
|     o def_memErr_acnt for memory errors
|     o def_noFILE_acnt if empty/no file
|     o (line << 4 | def_fileErr_acnt) for file errors
|       - do (error >> 4) to get line number
\-------------------------------------------------------*/
signed long
readFile_acnt(
   struct st_acnt *acntSTPtr, /*holds entries from file*/
   void *acntFILE             /*file with accounts*/
);

/*-------------------------------------------------------\
| Fun19: addEntry_st_acnt
|   - adds entry to an account
| Input:
|   - acntSTPtr:
|     o pointer to add account entry to
|   - parStr:
|     o c-string with parent account name
|   - childStr:
|     o c-string with child account name
|   - yearSS:
|     o year of transaction
|   - monthSC:
|     o month of transaction
|   - daySC:
|     o day of transaction
|   - amountF:
|     o amount of transaction
|   - commentStr:
|     o c-string with comment to add (0/null for NA)
|   - ghostBl:
|     o round up (negative) or down (positive) and add
|       rounded value to ghost account
|       * 1 do rounding
|   - persisionUC:
|     o number decimals to print (prentend/mock delete)
|       * 0 do not round
|   - outFILE:
|     o 0: add entry (reall change)
|     o FILE pointer: to print pretened change to
|       * does not apply the change
| Output:
|   - Prints:
|     o if outFILE; print potentail change to outFILE
|   - Modifies:
|     o parAryUS, childAryUS, yearArySS, monthArySC,
|       dayArySC, tranAryF, commentAryST in acntSTPtr
|       to have new entry (sorted by date)
|   - Returns:
|     o 0 for no errors
|     o def_memErr_acnt for memory errors
|     o def_missingEntry_acnt if child account is missing
|       (mock run only), year is not in range (>= 2000),
|       month is not in range, or day is not in range
\-------------------------------------------------------*/
signed char
addEntry_st_acnt(
   struct st_acnt *acntSTPtr, /*holds entries from file*/
   signed char *parStr,       /*parent account name*/
   signed char *childStr,     /*child account name*/
   signed short yearSS,       /*year of transaction*/
   signed short monthSC,      /*month of transaction*/
   signed short daySC,        /*day of transaction*/
   float amountF,             /*amount of transaction*/
   signed char *commentStr,   /*comment to add to entry*/
   signed char ghostBl,       /*add to ghost account*/
   unsigned char percisionUC, /*amount of percision*/
   void *outFILE              /*0 reall; FILE pretend*/
);

/*-------------------------------------------------------\
| Fun20: delIndex_st_acnt
|   - deletes an entry from a st_acnt struct by index
| Input:
|   - acntSTPtr:
|     o pointer to delete account entry from
|   - indexSL:
|     o index to delete
|   - persisionUC:
|     o number decimals to print (prentend/mock delete)
|   - outFILE:
|     o 0: delete entry (reall change)
|     o FILE pointer: to print pretened change to
|       * does not apply the change
| Output:
|   - Prints:
|     o if outFILE; print potentail change to outFILE
|   - Modifies:
|     o parAryUS, childAryUS, yearArySS, monthArySC,
|       dayArySC, tranAryF, commentAryST in acntSTPtr
|       to have entry deleted
|     o acntSortArySL and parSortArySL to have entry
|       deleted
|   - Returns:
|     o -1 if could not delete index
\-------------------------------------------------------*/
signed char
delIndex_st_acnt(
   struct st_acnt *acntSTPtr, /*holds entries from file*/
   signed long indexSL,       /*index to delete*/
   unsigned char percisionUC, /*amount of percision*/
   void *outFILE              /*0 reall; FILE pretend*/
);

/*-------------------------------------------------------\
| Fun21: pIndex_st_acnt
|   - prints entry at input index
| Input:
|   - acntSTPtr:
|     o pointer to print entry at input index
|   - indexSL:
|     o index to print
|   - persisionUC:
|     o number decimals to print (prentend/mock delete)
|   - outFILE:
|     o file to print index to (0/null for stdout)
| Output:
|   - Prints:
|     o entry at index to outFILE
|   - Returns:
|     o 0 if no error
|     o def_missingEntry_acnt if no index (out bounds)
\-------------------------------------------------------*/
signed char
pIndex_st_acnt(
   struct st_acnt *acntSTPtr, /*accounts*/
   signed long indexSL,       /*index of entry to print*/
   unsigned char percisionUC, /*amount of percision*/
   void *outFILE              /*file to print to*/
);

/*-------------------------------------------------------\
| Fun22: pEntries_st_acnt
|   - print all entries matching input info
| Input:
|   - acntSTPtr:
|     o pointer to print account entries from
|   - parStr:
|     o c-string with parent account name (use 0 to skip)
|   - childStr:
|     o c-string with child account name (use 0 to skip)
|   - yearSS:
|     o year of transaction (use 0 to skip)
|     o 2 element array [first year, last year]
|   - monthSC:
|     o month of transaction to find (use 0 to skip)
|     o 2 element array [first month, last month]
|   - daySC:
|     o day of transaction to find (use 0 to skip)
|     o 2 element array [first day, last day]
|   - persisionUC:
|     o number decimals to print (prentend/mock delete)
|   - sumBl:
|     o print sums of searched entries instead of entries
|   - outFILE:
|     o FILE pointer to print search to
| Output:
|   - Prints:
|     o all matching accounts to stdout
|   - Returns:
|     o 0 for no errors
|     o def_memErr_acnt for memory errors
|     o def_missingEntry_acnt if no entries
\-------------------------------------------------------*/
signed char
pEntries_st_acnt(
   struct st_acnt *acntSTPtr, /*holds entries from file*/
   signed char *parStr,       /*parent account to find*/
   signed char *childStr,     /*child accout to find*/
   signed short yearArySS[],  /*year to find*/
   signed char monthArySC[],  /*month to find*/
   signed char dayArySC[],    /*day to find*/
   unsigned char percisionUC, /*amount of percision*/
   signed char sumBl,         /*print sums, not entries*/
   void *outFILE              /*print accounts to*/
);

/*-------------------------------------------------------\
| Fun23: pSum_st_acnt
|   - print totals for all accounts
| Input:
|   - acntSTPtr:
|     o pointer to print account sums from
|   - persisionUC:
|     o number decimals to print (prentend/mock delete)
|   - outFILE:
|     o FILE pointer to print sums to | Output:
| Output:
|   - Prints:
|     o sums for all accounts to outFILE
|   - Returns:
|     o 0 for no errors
|     o def_memErr_acnt for memory errors
\-------------------------------------------------------*/
signed char
pSum_st_acnt(
   struct st_acnt *acntSTPtr, /*has entries to get sums*/
   unsigned char percisionUC, /*amount of percision*/
   void *outFILE              /*print account sums to*/
);

/*-------------------------------------------------------\
| Fun24: write_st_acnt
|   - write to accounts file (by date)
| Input:
|   - acntSTPtr:
|     o pointer to write
|   - persisionUC:
|     o number decimals to print
|   - outFILE:
|     o FILE pointer: to print acntSTPtr to
| Output:
|   - Prints:
|     o entires in acnTSTPtr to outFILE
|   - Returns:
|     o 0 for no errors
|     o def_memErr_acnt for memory errors
\-------------------------------------------------------*/
signed char
write_st_acnt(
   struct st_acnt *acntSTPtr, /*accounts to print out*/
   unsigned char percisionUC, /*percision to print at*/
   void *outFILE              /*right accounts to*/
);

/*-------------------------------------------------------\
| Fun25: divy_st_acnt
|   - divide money into accounts by divy file
| Input:
|   - acntSTPtr:
|     o pointer to divy funds into
|   - amountF:
|     o amount to divy between accounts
|   - divyStr:
|     o c-string with divy infromation
|       * column 1: parent account name
|       * column 2: child account name
|       * column 3: percentage to divy (or NA)
|       * column 4: minimum amount to transfer (or NA)
|         - can be left out if percent column
|       * column 5: maximum amount to transfer (or NA)
|         - can be left out if percent/min column
|       * column 6 to nth: ignored
|     o first row is headedr (ignored)
|   - commentStr:
|     o c-string with comment to add (0/null for NA)
|   - ghostBl:
|     o 1: round down, but print unrounded transfer
|     o 0: do extact transaction (no rounding)
|   - persisionUC:
|     o number decimals to print (prentend/mock delete)
|   - outFILE:
|     o 0: divy entry (reall change)
|     o FILE pointer: to print pretened change to
|       * does not apply the change
| Output:
|   - Prints:
|     o if outFILE; print potentail change to outFILE
|     o amount to transfer to parent accounts to
|       stdout or if mock run, to outFILE
|     o prints extra after transfer to stdout/outFILE
|     o if can not complete divy, prints line ended
|       to stderr
|   - Modifies:
|     o parAryUS, childAryUS, yearArySS, monthArySC,
|       dayArySC, tranAryF, commentAryST in acntSTPtr
|       to have new entries from divy (sorted by date)
|   - Returns:
|     o 0 for no errors
|     o def_memErr_acnt for memory errors
|     o def_missingEntry_acnt if divy string is blank or
|       just header
|     o number of problematic line in divy file
\-------------------------------------------------------*/
signed long
divy_st_acnt(
   struct st_acnt *acntSTPtr, /*accounts to add divy to*/
   float amountF,             /*amount to divy*/
   signed char *divyStr,      /*each line is divy value*/
   signed char *commentStr,   /*user comment*/
   signed char ghostBl,       /*round transaction down*/
   unsigned char percisionUC, /*percision to print at*/
   void *outFILE              /*0 reall; FILE pretend*/
);

#endif

/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the public domain
:   is inconvient / not possible, this code is under the
:   MIT license.
: 
: Public domain:
: 
: This is free and unencumbered software released into the
:   public domain.
: 
: Anyone is free to copy, modify, publish, use, compile,
:   sell, or distribute this software, either in source
:   code form or as a compiled binary, for any purpose,
:   commercial or non-commercial, and by any means.
: 
: In jurisdictions that recognize copyright laws, the
:   author or authors of this software dedicate any and
:   all copyright interest in the software to the public
:   domain. We make this dedication for the benefit of the
:   public at large and to the detriment of our heirs and
:   successors. We intend this dedication to be an overt
:   act of relinquishment in perpetuity of all present and
:   future rights to this software under copyright law.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO
:   EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM,
:   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
:   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
:   IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
:   DEALINGS IN THE SOFTWARE.
: 
: For more information, please refer to
:   <https://unlicense.org>
: 
: MIT License:
: 
: Copyright (c) 2025 jeremyButtler
: 
: Permission is hereby granted, free of charge, to any
:   person obtaining a copy of this software and
:   associated documentation files (the "Software"), to
:   deal in the Software without restriction, including
:   without limitation the rights to use, copy, modify,
:   merge, publish, distribute, sublicense, and/or sell
:   copies of the Software, and to permit persons to whom
:   the Software is furnished to do so, subject to the
:   following conditions:
: 
: The above copyright notice and this permission notice
:   shall be included in all copies or substantial
:   portions of the Software.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
:   EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
:   FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
:   AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
:   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
:   USE OR OTHER DEALINGS IN THE SOFTWARE.
\=======================================================*/
