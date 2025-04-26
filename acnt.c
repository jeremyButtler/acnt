/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' acnt SOF: Start Of File
'   - reads a account file for acnt and sets tallies
'   o header:
'     - included libraries
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
|   - included libraries
\-------------------------------------------------------*/

#ifdef PLAN9
   #include <u.h>
   #include <libc.h>
#else
   #include <stdlib.h>
#endif

#include "acnt.h"
#include <stdio.h>
#include <time.h>

#include "genLib/endLine.h"
#include "genLib/base10str.h"
#include "genLib/numToStr.h"
#include "genLib/ulCp.h"
#include "genLib/strAry.h"
#include "genLib/ptrAry.h"

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
){ /*blank_st_acnt*/
   if(! acntSTPtr)
      return;

   /*most accounts are not worth blanking, since I use
   `  a length variable to find them, so do  a lazy
   `  delete (mark length as 0 [no entries])
   */

   acntSTPtr->numParUS = 0;
   acntSTPtr->numChildUS = 0;
   acntSTPtr->numEntrySL = 0;
} /*blank_st_acnt*/

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
){
   if(! acntSTPtr)
      return;

   acntSTPtr->uniqParStr = 0;
   acntSTPtr->parToNumAryUS = 0;
   acntSTPtr->numToParAryUS = 0;
   acntSTPtr->sizeParUS = 0;

   acntSTPtr->uniqChildStr = 0;
   acntSTPtr->childToNumAryUS = 0;
   acntSTPtr->numToChildAryUS = 0;
   acntSTPtr->sizeChildUS = 0;

   acntSTPtr->parAryUS = 0;
   acntSTPtr->childAryUS = 0;

   acntSTPtr->yearArySS = 0;
   acntSTPtr->monthArySC = 0;
   acntSTPtr->dayArySC = 0;
   acntSTPtr->tranAryF = 0;
   acntSTPtr->commentAryST = 0;

   acntSTPtr->sizeEntrySL = 0;

   blank_st_acnt(acntSTPtr);
} /*init_st_acnt*/

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
){
   if(! acntSTPtr)
      return;

   if(acntSTPtr->uniqParStr)
     free(acntSTPtr->uniqParStr);
   if(acntSTPtr->parToNumAryUS)
     free(acntSTPtr->parToNumAryUS);
   if(acntSTPtr->numToParAryUS)
     free(acntSTPtr->numToParAryUS);

   if(acntSTPtr->uniqChildStr)
      free(acntSTPtr->uniqChildStr);
   if(acntSTPtr->childToNumAryUS)
      free(acntSTPtr->childToNumAryUS);
   if(acntSTPtr->numToChildAryUS)
      free(acntSTPtr->numToChildAryUS);

   if(acntSTPtr->parAryUS)
      free(acntSTPtr->parAryUS);
   if(acntSTPtr->childAryUS)
      free(acntSTPtr->childAryUS);

   if(acntSTPtr->yearArySS)
      free(acntSTPtr->yearArySS);
   if(acntSTPtr->monthArySC)
      free(acntSTPtr->monthArySC);
   if(acntSTPtr->dayArySC)
      free(acntSTPtr->dayArySC);

  if(acntSTPtr->tranAryF)
     free(acntSTPtr->tranAryF);
  if(acntSTPtr->commentAryST)
     freeHeap_str_ptrAry(acntSTPtr->commentAryST);

   init_st_acnt(acntSTPtr);
} /*freeStack_st_acnt*/

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
){
   if(! acntSTPtr)
      return;

   freeStack_st_acnt(acntSTPtr);
   free(acntSTPtr);
} /*freeHeap_st_acnt*/

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
){
   signed long slElm = 0;

   for(
     slElm = 0;
     slElm < lenSL;
     ++slElm
   ) freeStack_st_acnt(&acntSTPtr[lenSL]);

   free(acntSTPtr);
} /*freeHeapAry_st_acnt*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun06 TOC:
   '   - allocates initial memory for a st_acnt structure
   '   o fun06 sec01:
   '     - declarations + check input + initialize
   '   o fun06 sec02:
   '     - allocate memory
   '   o fun06 sec03:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec01:
   ^   - declarations + check input + initialize
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   #define def_size_fun06 16
   /*keeping this small so all values can have the same
   `  number, will resize later
   */

   if(! acntSTPtr)
      return 0;

   freeStack_st_acnt(acntSTPtr);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec02:
   ^   - allocate memory
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   acntSTPtr->uniqParStr = mk_strAry(def_size_fun06);
   if(! acntSTPtr->uniqParStr)
      goto memErr_fun06_sec03;

   acntSTPtr->parToNumAryUS =
      malloc(def_size_fun06 * sizeof(unsigned short));
   if(! acntSTPtr->parToNumAryUS)
      goto memErr_fun06_sec03;

   acntSTPtr->numToParAryUS =
      malloc(def_size_fun06 * sizeof(unsigned short));
   if(! acntSTPtr->numToParAryUS)
      goto memErr_fun06_sec03;

   acntSTPtr->sizeParUS = def_size_fun06;


   acntSTPtr->uniqChildStr = mk_strAry(def_size_fun06);
   if(! acntSTPtr->uniqChildStr)
      goto memErr_fun06_sec03;

   acntSTPtr->childToNumAryUS =
      malloc(def_size_fun06 * sizeof(unsigned short));
   if(! acntSTPtr->childToNumAryUS)
      goto memErr_fun06_sec03;

   acntSTPtr->numToChildAryUS =
      malloc(def_size_fun06 * sizeof(unsigned short));
   if(! acntSTPtr->numToChildAryUS)
      goto memErr_fun06_sec03;

   acntSTPtr->sizeChildUS = def_size_fun06;


   acntSTPtr->parAryUS =
      malloc(def_size_fun06 * sizeof(unsigned short));
   if(! acntSTPtr->parAryUS)
      goto memErr_fun06_sec03;

   acntSTPtr->childAryUS =
      malloc(def_size_fun06 * sizeof(unsigned short));
   if(! acntSTPtr->childAryUS)
      goto memErr_fun06_sec03;


   acntSTPtr->yearArySS =
      malloc(def_size_fun06 * sizeof(signed short));
   if(! acntSTPtr->yearArySS)
      goto memErr_fun06_sec03;

   acntSTPtr->monthArySC =
      malloc(def_size_fun06 * sizeof(signed char));
   if(! acntSTPtr->monthArySC)
      goto memErr_fun06_sec03;

   acntSTPtr->dayArySC =
      malloc(def_size_fun06 * sizeof(signed char));
   if(! acntSTPtr->dayArySC)
      goto memErr_fun06_sec03;


   acntSTPtr->tranAryF =
      malloc(def_size_fun06 * sizeof(float));
   if(! acntSTPtr->tranAryF)
      goto memErr_fun06_sec03;


   acntSTPtr->commentAryST= mk_str_ptrAry(def_size_fun06);
   if(! acntSTPtr->commentAryST)
      goto memErr_fun06_sec03;


   blank_st_acnt(acntSTPtr);
   acntSTPtr->sizeEntrySL = def_size_fun06;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun06 Sec03:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   memErr_fun06_sec03:;
      return def_memErr_acnt;
} /*setup_st_acnt*/

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
){
   struct st_acnt *retHeapST = 0;

   retHeapST = malloc(sizeof(struct st_acnt));
   if(! retHeapST)
      goto memErr_fun07;

   init_st_acnt(retHeapST);

   if(setup_st_acnt(retHeapST))
      goto memErr_fun07;

   return retHeapST;

   memErr_fun07:;
      return 0;
} /*mk_st_acnt*/

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
){
   struct st_acnt *retHeapST = 0;
   signed long slElm = 0;

   retHeapST = malloc(lenSL * sizeof(struct st_acnt));

   if(! retHeapST)
      goto memErr_fun07;

   for(
      slElm = 0;
      slElm < lenSL;
      ++slElm
   ){ /*Loop: initialize and setup memory*/
      init_st_acnt(retHeapST);

      if(setup_st_acnt(retHeapST))
         goto memErr_fun07;
   }  /*Loop: initialize and setup memory*/


   return retHeapST;

   memErr_fun07:;
      if(retHeapST)
         freeHeapAry_st_acnt(
            retHeapST,
            slElm
         ); /*free setup structs*/
      retHeapST = 0;

      return 0;
} /*mkAry_st_acnt*/

/*------------------------------------------------------\
| Fun09: reallocEntry_st_acnt
|   - resizes entry arrays in a st_acnt struct
| Input:
|   - acntSTPtr:
|     o ponter to st_acnt struct to resize
|   - lenSL:
|     o number to resize to
| Output:
|   - Modifies:
|     o parAryUS, childAryUS, yearArySS, monthArySC,
|       dayArySC, tranAryF, commentAryST, to be resized
|       to lenSL
|     o sizeEntrySL to be same as lenSL
|   - Returns:
|     o 0 for no errors
|     o def_memErr_acnt for memory errors
|     o def_downsize_acnt if downsizing (not allowed)
\------------------------------------------------------*/
signed char
reallocEntry_st_acnt(
   struct st_acnt *acntSTPtr, /*struct to resize*/
   signed long lenSL          /*new array sizes*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun09 TOC:
   '   - resize all arrays in a st_acnt struct
   '   o fun09 sec01:
   '     - variable declarations
   '   o fun09 Sec02:
   '     - check if can resize
   '   o fun09 Sec03:
   '     - resize arrays
   '   o fun09 Sec04:
   '     - set size + return results
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec01:
   ^   - varaible declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed long slIndex = 0;
   signed char *swapSCPtr = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec02:
   ^   - check if can resize
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! acntSTPtr)
      return 0;

   if(lenSL < acntSTPtr->sizeEntrySL)
      goto downsize_fun09_sec04;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec03:
   ^   - resize arrays
   ^   o fun09 sec03 sub01:
   ^     - parent/child account index reallocation
   ^   o fun09 sec03 sub02:
   ^     - date reallocaton
   ^   o fun09 sec03 sub03:
   ^     - ammount and comment reallocaton
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun09 Sec03 Sub01:
   *   - parent/child account index reallocation
   \*****************************************************/

   swapSCPtr =
      (signed char *)
      realloc(
         acntSTPtr->parAryUS,
         lenSL * sizeof(unsigned short)
      );

   if(! swapSCPtr)
      goto memErr_fun09_sec04;
   acntSTPtr->parAryUS = (unsigned short *) swapSCPtr;


   swapSCPtr =
      (signed char *)
      realloc(
         acntSTPtr->childAryUS,
         lenSL * sizeof(unsigned short)
      );

   if(! swapSCPtr)
      goto memErr_fun09_sec04;
   acntSTPtr->childAryUS = (unsigned short *) swapSCPtr;

   /*****************************************************\
   * Fun09 Sec03 Sub02:
   *   - date reallocaton
   \*****************************************************/

   swapSCPtr =
      (signed char *)
      realloc(
          acntSTPtr->yearArySS,
          lenSL * sizeof(signed short)
      );

   if(! swapSCPtr)
      goto memErr_fun09_sec04;
   acntSTPtr->yearArySS = (signed short *) swapSCPtr;


   swapSCPtr =
      realloc(
         acntSTPtr->monthArySC,
         lenSL * sizeof(signed char)
      );
   if(! swapSCPtr)
      goto memErr_fun09_sec04;
   acntSTPtr->monthArySC = swapSCPtr;


   swapSCPtr =
      realloc(
         acntSTPtr->dayArySC,
         lenSL * sizeof(signed char)
      );
   if(! swapSCPtr)
      goto memErr_fun09_sec04;
   acntSTPtr->dayArySC = swapSCPtr;

   /*****************************************************\
   * Fun09 Sec03 Sub03:
   *   - ammount and comment reallocaton
   \*****************************************************/

   swapSCPtr =
      (signed char *)
      realloc(
         acntSTPtr->tranAryF,
         lenSL * sizeof(float)
      );
   if(! swapSCPtr)
      goto memErr_fun09_sec04;
   acntSTPtr->tranAryF = (float *) swapSCPtr;


   if(
      resize_str_ptrAry(
         acntSTPtr->commentAryST,
         lenSL
      )
   ) goto memErr_fun09_sec04;

   /*****************************************************\
   * Fun09 Sec03 Sub04:
   *   - blank reallocs
   \*****************************************************/

   for(
      slIndex = acntSTPtr->numEntrySL;
      slIndex < lenSL;
      ++slIndex
   ){ /*Loop: blank new allocs*/
      acntSTPtr->parAryUS[slIndex] = 0;
      acntSTPtr->childAryUS[slIndex] = 0;
      acntSTPtr->yearArySS[slIndex] = 0;
      acntSTPtr->monthArySC[slIndex] = 0;
      acntSTPtr->dayArySC[slIndex] = 0;
      acntSTPtr->tranAryF[slIndex] = 0;
   }  /*Loop: blank new allocs*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun09 Sec04:
   ^   - set size + return results
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   acntSTPtr->sizeEntrySL = lenSL;

   return 0;

   memErr_fun09_sec04:;
      return def_memErr_acnt;
   downsize_fun09_sec04:;
      return def_downsize_acnt;
} /*reallocEntry_st_acnt*/

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
){

   signed char *swapSCPtr = 0;

   if(! acntSTPtr)
      return 0;

   if(lenUS < acntSTPtr->sizeParUS)
      goto downsize_fun10; /*down sizing*/


   swapSCPtr =
      realloc_strAry(
         acntSTPtr->uniqParStr,
         lenUS
      );
   if(! swapSCPtr)
      goto memErr_fun10;
   acntSTPtr->uniqParStr = swapSCPtr;

   swapSCPtr =
      (signed char *)
      realloc(
         acntSTPtr->parToNumAryUS,
         lenUS * sizeof(unsigned short)
      );
   if(! swapSCPtr)
      goto memErr_fun10;
   acntSTPtr->parToNumAryUS =(unsigned short *) swapSCPtr;

   swapSCPtr =
      (signed char *)
      realloc(
         acntSTPtr->numToParAryUS,
         lenUS * sizeof(unsigned short)
      );
   if(! swapSCPtr)
      goto memErr_fun10;
   acntSTPtr->numToParAryUS =(unsigned short *) swapSCPtr;


   acntSTPtr->sizeParUS = lenUS;

   return 0;

   memErr_fun10:;
      return def_memErr_acnt;
   downsize_fun10:;
      return def_downsize_acnt;
} /*reallocPar_st_acnt*/

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
|     o uniqChildStr and numToChildAryUS arrays in
|       acntSTPtr to be resized to lenSL
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
){

   signed char *swapSCPtr = 0;

   if(! acntSTPtr)
      return 0;

   if(lenUS < acntSTPtr->sizeChildUS)
      goto downsize_fun11; /*down sizing*/


   swapSCPtr =
      realloc_strAry(
         acntSTPtr->uniqChildStr,
         lenUS
      );
   if(! swapSCPtr)
      goto memErr_fun11;
   acntSTPtr->uniqChildStr = swapSCPtr;

   swapSCPtr =
      (signed char *)
      realloc(
         acntSTPtr->childToNumAryUS,
         lenUS * sizeof(unsigned short)
      );
   if(! swapSCPtr)
      goto memErr_fun11;
   acntSTPtr->childToNumAryUS =
      (unsigned short *) swapSCPtr;

   swapSCPtr =
      (signed char *)
      realloc(
         acntSTPtr->numToChildAryUS,
         lenUS * sizeof(unsigned short)
      );
   if(! swapSCPtr)
      goto memErr_fun11;
   acntSTPtr->numToChildAryUS =
      (unsigned short *) swapSCPtr;


   acntSTPtr->sizeChildUS = lenUS;

   return 0;

   memErr_fun11:;
      return def_memErr_acnt;
   downsize_fun11:;
      return def_downsize_acnt;
} /*reallocChild_st_acnt*/

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
){
   struct st_acnt *swapST = 0;

   swapST =
      realloc(
         acntSTPtr,
         lenSL * sizeof(struct st_acnt)
      );

   if(! swapST)
      goto memErr_fun12;

   acntSTPtr = swapST; 

   if(sizeSL > 0)
      --sizeSL;

   while(sizeSL < lenSL)
   { /*Loop: initialize and set up*/
      init_st_acnt(&acntSTPtr[sizeSL]);

      if( setup_st_acnt(&acntSTPtr[sizeSL]) )
         goto memErr_fun12;
   } /*Loop: initialize and set up*/

   return acntSTPtr;

   memErr_fun12:;
      return 0;
} /*reallocAry_st_acnt*/

/*------------------------------------------------------\
| Fun13: swapEntry_st_acnt
|   - swaps two entries in a st_acnt strucutre
| Input:
|   - acntSTPtr:
|     o ponter to st_acnt struct array to resize
|   - firstSL:
|     o first index to swap
|   - secSL:
|     o second index to swap
| Output:
|   - Modifies:
|     o parAryUS, childAryUS, yearArySS, monthArySC,
|       dayArySC tranAryF, and commentAryST in acntSTPtr
|       to have indexes firstSL and secSL swaped
\------------------------------------------------------*/
void
swapEntry_st_acnt(
   struct st_acnt *acntSTPtr, /*has entries to swap*/
   signed long firstSL,       /*first index to swap*/
   signed long secSL          /*second index to swap*/
){
   float swapF = 0;

   acntSTPtr->parAryUS[firstSL] ^=
      acntSTPtr->parAryUS[secSL];
   acntSTPtr->parAryUS[secSL] ^=
      acntSTPtr->parAryUS[firstSL];
   acntSTPtr->parAryUS[firstSL] ^=
      acntSTPtr->parAryUS[secSL];

   acntSTPtr->childAryUS[firstSL] ^=
      acntSTPtr->childAryUS[secSL];
   acntSTPtr->childAryUS[secSL] ^=
      acntSTPtr->childAryUS[firstSL];
   acntSTPtr->childAryUS[firstSL] ^=
      acntSTPtr->childAryUS[secSL];

   acntSTPtr->yearArySS[firstSL] ^=
      acntSTPtr->yearArySS[secSL];
   acntSTPtr->yearArySS[secSL] ^=
      acntSTPtr->yearArySS[firstSL];
   acntSTPtr->yearArySS[firstSL] ^=
      acntSTPtr->yearArySS[secSL];

   acntSTPtr->monthArySC[firstSL] ^=
      acntSTPtr->monthArySC[secSL];
   acntSTPtr->monthArySC[secSL] ^=
      acntSTPtr->monthArySC[firstSL];
   acntSTPtr->monthArySC[firstSL] ^=
      acntSTPtr->monthArySC[secSL];

   acntSTPtr->dayArySC[firstSL] ^=
      acntSTPtr->dayArySC[secSL];
   acntSTPtr->dayArySC[secSL] ^=
      acntSTPtr->dayArySC[firstSL];
   acntSTPtr->dayArySC[firstSL] ^=
      acntSTPtr->dayArySC[secSL];

   swapF = acntSTPtr->tranAryF[firstSL];
   acntSTPtr->tranAryF[firstSL] =
      acntSTPtr->tranAryF[secSL];
   acntSTPtr->tranAryF[secSL] = swapF;

   swap_str_ptrAry(
      acntSTPtr->commentAryST,
      (unsigned long) firstSL,
      (unsigned long) secSL
   );
} /*swapEntry_st_acnt*/

/*------------------------------------------------------\
| Fun14: sortDate_st_acnt
|   - sorts entry arrays in st_acnt structure by date
| Input:
|   - acntSTPtr:
|     o ponter to st_acnt struct array to sort
| Output:
|   - Modifies:
|     o parAryUS, childAryUS, yearArySS, monthArySC,
|       dayArySC tranAryF, and commentAryST in acntSTPtr
|       to be sorted by date and then by account
\------------------------------------------------------*/
void
sortDate_st_acnt(
   struct st_acnt *acntSTPtr /*struct to sort*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun14 TOC:
   '   - sorts entries in st_acnt struct by date
   '   o fun14 sec01:
   '     - variable declerations
   '   o fun14 sec02:
   '     - find the number of rounds to sort for
   '   o fun14 sec03:
   '     - sort the arrays
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec01:
   ^   - variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*Number of sorting rounds*/
   signed long subSL = 0;
   signed long nextSL = 0;
   signed long lastSL = 0;
   signed long onSL = 0;

   /*Variables to incurment loops*/
   signed long slIndex = 0;
   signed long slElm = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec02:
   ^   - find the max search value (number rounds to sort)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(acntSTPtr->numEntrySL < 2)
      return; /*nothing to do*/

   /*Recursion formuia: h[0] = 1, h[n] = 3 * h[n - 1] +1*/
   subSL = 1; /*Initialzie first array*/

   while(subSL < acntSTPtr->numEntrySL - 1)
      subSL = (3 * subSL) + 1;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun14 Sec03:
   ^   - sort arrays
   ^   o fun14 sec03 sub01:
   ^      - start sorting loops
   ^   o fun14 sec03 sub02:
   ^      - check if need to swap elements
   ^   o fun14 sec03 sub03:
   ^      - check back elements for swap
   ^   o fun14 sec03 sub04:
   ^      - move to next set to check
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun14 Sec03 Sub01:
   *    - start sorting loops
   \*****************************************************/

   while(subSL > 0)
   { /*Loop: all rounds*/
      for(
         slIndex = 0;
         slIndex <= subSL;
         ++slIndex
      ){ /*Loop: though sub array*/
         slElm = slIndex;

         for(
            slElm = slIndex;
            slElm + subSL < acntSTPtr->numEntrySL;
            slElm += subSL
         ){ /*Loop: swap elements in subarray*/
            nextSL = slElm + subSL;

            /********************************************\
            * Fun14 Sec03 Sub02:
            *    - check if need to swap elements
            \********************************************/

            if(
                 acntSTPtr->yearArySS[slElm]
               < acntSTPtr->yearArySS[nextSL]
            ) ; /*If: already positioned*/

            else if(
                 acntSTPtr->yearArySS[slElm]
               > acntSTPtr->yearArySS[nextSL]
            ) goto swapFor_fun14;

            else if(
                 acntSTPtr->monthArySC[slElm]
               < acntSTPtr->monthArySC[nextSL]
            ) ; /*Else If: month is positioned*/

            else if(
                 acntSTPtr->monthArySC[slElm]
               > acntSTPtr->monthArySC[nextSL]
            ) goto swapFor_fun14; /*month needs swaping*/

            else if(
                 acntSTPtr->dayArySC[slElm]
               < acntSTPtr->dayArySC[nextSL]
            ) ; /*Else If: day is positioned*/

            else if(
                 acntSTPtr->dayArySC[slElm]
               > acntSTPtr->dayArySC[nextSL]
            ) goto swapFor_fun14; /*day needs swaping*/

            else if(
                 acntSTPtr->childAryUS[slElm]
               < acntSTPtr->childAryUS[nextSL]
            ) ; /*child account already positioned*/

            else if(
                 acntSTPtr->childAryUS[slElm]
               > acntSTPtr->childAryUS[nextSL]
            ) goto swapFor_fun14;

            else if(
                  acntSTPtr->parAryUS[slElm]
               <= acntSTPtr->parAryUS[nextSL]
            ) ; /*parent account already positioned*/

            else
            { /*Else: need to swap an element*/
               swapFor_fun14:;

               swapEntry_st_acnt(
                  acntSTPtr,
                  slElm,
                  nextSL
               );

               /*****************************************\
               * Fun14 Sec03 Sub03:
               *    - check back elements for swap
               \*****************************************/

               lastSL = slElm;
               onSL = slElm;

               while(lastSL >= subSL)
               { /*loop: move swapped element back*/
                  lastSL -= subSL;

                  if(
                       acntSTPtr->yearArySS[onSL]
                     < acntSTPtr->yearArySS[lastSL]
                  ) ;

                  else if(
                       acntSTPtr->yearArySS[onSL]
                     > acntSTPtr->yearArySS[lastSL]
                  ) break; /*If: already positioned*/

                  else if(
                       acntSTPtr->monthArySC[onSL]
                     < acntSTPtr->monthArySC[lastSL]
                  ) ;

                  else if(
                       acntSTPtr->monthArySC[onSL]
                     > acntSTPtr->monthArySC[lastSL]
                  ) break; /*month is positioned*/

                  else if(
                       acntSTPtr->dayArySC[onSL]
                     < acntSTPtr->dayArySC[lastSL]
                  ) ;

                  else if(
                       acntSTPtr->dayArySC[onSL]
                     > acntSTPtr->dayArySC[lastSL]
                  ) break; /*day is positioned*/

                  else if(
                       acntSTPtr->childAryUS[onSL]
                     < acntSTPtr->childAryUS[lastSL]
                  ) ;

                  else if(
                       acntSTPtr->childAryUS[onSL]
                     > acntSTPtr->childAryUS[lastSL]
                  ) break; /*child account positioned*/

                  else if(
                        acntSTPtr->parAryUS[onSL]
                     >= acntSTPtr->parAryUS[lastSL]
                  ) break; /*parent account positioned*/

                  swapEntry_st_acnt(
                     acntSTPtr,
                     onSL,
                     lastSL
                  );

                  onSL = lastSL;
               } /*Loop: move swapped element back*/
            } /*Eles: need to swap elements*/
         } /*Loop: swap elements in subarray*/
      } /*Loop: though sub array*/

      /**************************************************\
      * Fun14 Sec03 Sub04:
      *    - move to next set to check
      \**************************************************/

      subSL = (subSL - 1) / 3; /*Move to next round*/
   } /*Loop: all rounds*/
} /*sortDate_st_acnt*/

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
){
   signed long midSL = 0;
   signed long rightSL = acntSTPtr->numEntrySL - 1;
   signed long leftSL = 0;

   while(leftSL <= rightSL)
   { /*Loop: find kmers*/
      midSL = (leftSL + rightSL) >> 1;

      if(yearSS > acntSTPtr->yearArySS[midSL])
         leftSL = midSL + 1;
         /*year is to far ahead*/

      else if(yearSS < acntSTPtr->yearArySS[midSL])
         rightSL = midSL - 1;
         /*year is to far behind*/

      else if(monthSC > acntSTPtr->monthArySC[midSL])
         leftSL = midSL + 1;
         /*month is to far ahead*/

      else if(monthSC < acntSTPtr->monthArySC[midSL])
         rightSL = midSL - 1;
         /*month is to far behind*/

      else if(daySC > acntSTPtr->dayArySC[midSL])
         leftSL = midSL + 1;
         /*day is to far ahead*/

      else if(daySC < acntSTPtr->dayArySC[midSL])
         rightSL = midSL - 1;
         /*day is to far behind*/

      else if(parUS < acntSTPtr->parAryUS[midSL])
         rightSL = midSL - 1;
         /*parent is far behind*/

      else if(parUS > acntSTPtr->parAryUS[midSL])
         leftSL = midSL + 1;
         /*parent is ahead*/

      else if(childUS < acntSTPtr->childAryUS[midSL])
         rightSL = midSL - 1;
         /*child is far behind*/

      else if(childUS > acntSTPtr->childAryUS[midSL])
         leftSL = midSL + 1;
         /*child is ahead*/

      else
      { /*Else: found match*/
         if(! midSL)
            return midSL; /*earlist possible match*/

         else if(
               yearSS == acntSTPtr->yearArySS[midSL - 1]
            && monthSC == acntSTPtr->monthArySC[midSL - 1]
            && daySC == acntSTPtr->dayArySC[midSL - 1]
         ) rightSL = midSL - 1; /*not at first match*/

         else
            return midSL; /*earlist possible match*/
      } /*Else: found match*/
   } /*Loop: find kmers*/

   if(! midSL)
      return midSL * -1;

   if( yearSS > acntSTPtr->yearArySS[midSL] )
      ;
   else if( yearSS < acntSTPtr->yearArySS[midSL] )
      --midSL; /*ended on later date*/
   else if( monthSC > acntSTPtr->monthArySC[midSL] )
      ;
   else if( monthSC < acntSTPtr->monthArySC[midSL] )
      --midSL; /*ended on later date*/
   else if( daySC < acntSTPtr->dayArySC[midSL] )
      --midSL; /*ended on later date*/

   return midSL * -1; /*nothing found*/
} /*findDate_st_acnt*/

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
){
   unsigned short usIndex = 0;
   signed int indexSI = 0;

   indexSI =
      find_strAry(
         acntSTPtr->uniqParStr,
         parStr,
         acntSTPtr->numParUS
      );

    if(indexSI >= 0)
       return indexSI; /*id already in array*/


   if(acntSTPtr->numParUS >= acntSTPtr->sizeParUS)
   { /*If: need to resize array*/
      if(
         reallocPar_st_acnt(
            acntSTPtr,
            acntSTPtr->numParUS << 1
         )
      ) goto memErr_fun16_sec05;
   } /*If: need to resize array*/


   add_strAry(
      parStr,
      acntSTPtr->uniqParStr,
      acntSTPtr->numParUS
   ); /*add parent id to unique parent id array*/

   indexSI = (signed int) acntSTPtr->numParUS;

   /*assign index on first come basis*/
   acntSTPtr->parToNumAryUS[acntSTPtr->numParUS] =
      acntSTPtr->numParUS;
   ++acntSTPtr->numParUS;


   /*this next bit is inefficent, but I am counting on
   `  new parent ids to be added rarely to entries
   */
   sortSyncUS_strAry(
      acntSTPtr->uniqParStr,
      acntSTPtr->parToNumAryUS,
      acntSTPtr->numParUS
   );


   /*this is for a quick index to parent id lookup*/
   for(
      usIndex = 0;
      usIndex < acntSTPtr->numParUS;
      ++usIndex
   ){ /*Loop: update entry index to par index*/
      acntSTPtr->numToParAryUS[
         acntSTPtr->parToNumAryUS[usIndex]
      ] = usIndex;
   }  /*Loop: update entry index to par index*/


   indexSI =
      find_strAry(
         acntSTPtr->uniqParStr,
         parStr,
         acntSTPtr->numParUS
      );

   return indexSI;

   memErr_fun16_sec05:;
      return def_memErr_acnt;
} /*addNewPar_st_acnt*/

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
){
   unsigned short usIndex = 0;
   signed int indexSI = 0;

   indexSI =
      find_strAry(
         acntSTPtr->uniqChildStr,
         childStr,
         acntSTPtr->numChildUS
      );

   if( indexSI >= 0)
       return indexSI;


   if(acntSTPtr->numChildUS >= acntSTPtr->sizeChildUS)
   { /*If: need to resize array*/
      if(
         reallocChild_st_acnt(
            acntSTPtr,
            acntSTPtr->numChildUS << 1
         )
      ) goto memErr_fun16_sec05;
   } /*If: need to resize array*/


   add_strAry(
      childStr,
      acntSTPtr->uniqChildStr,
      acntSTPtr->numChildUS
   ); /*add child id to unique child id array*/

   indexSI = (signed int) acntSTPtr->numChildUS;

   /*assign index on first come basis*/
   acntSTPtr->childToNumAryUS[acntSTPtr->numChildUS] =
      acntSTPtr->numChildUS;
   ++acntSTPtr->numChildUS;


   /*this next bit is inefficent, but I am counting on
   `  new child ids to be added rarely to entries
   */
   sortSyncUS_strAry(
      acntSTPtr->uniqChildStr,
      acntSTPtr->childToNumAryUS,
      acntSTPtr->numChildUS
   );


   /*this is for a quick index to child id lookup*/
   for(
      usIndex = 0;
      usIndex < acntSTPtr->numChildUS;
      ++usIndex
   ){ /*Loop: update entry index to child index*/
      acntSTPtr->numToChildAryUS[
         acntSTPtr->childToNumAryUS[usIndex]
      ] = usIndex;
   }  /*Loop: update entry index to child index*/

   indexSI =
      find_strAry(
         acntSTPtr->uniqChildStr,
         childStr,
         acntSTPtr->numChildUS
      );

   return indexSI;

   memErr_fun16_sec05:;
      return def_memErr_acnt;
} /*addNewChild_st_acnt*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun18 TOC:
   '   - get account entries from a file
   '   o fun18 sec01:
   '     - variable declarations
   '   o fun18 sec05:
   '     - sort and return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*maximum expected size is 247, it handles three
   `  strings of length 64, a full date (8 bytes)
   `  and well over 18 bytes for two numbers
   */
   #define def_buffSize_fun18 247

   signed char buffStr[def_buffSize_fun18 + 9];
   signed char *tmpStr = 0;
   unsigned short posUS = 0;
   unsigned short endUS = 0;
   signed long indexSL = 0;
   signed long lineSL = 0;

   signed char indexColBl = 0; /*index column in header*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec02:
   ^   - read file
   ^   o fun18 sec02 sub01:
   ^     - move past header and check if index column
   ^   o fun18 sec02 sub02:
   ^     - move past initial white space + allocate + loop
   ^   o fun18 sec02 sub04:
   ^     - read in parent account id
   ^   o fun18 sec02 sub05:
   ^     - read in child account id
   ^   o fun18 sec02 sub06:
   ^     - get year
   ^   o fun18 sec02 sub07:
   ^     - get month
   ^   o fun18 sec02 sub08:
   ^     - get day
   ^   o fun18 sec02 sub09:
   ^     - get transaction amount
   ^   o fun18 sec02 sub01:
   ^     - skip total (re-calculate)
   ^   o fun18 sec02 sub11:
   ^     - get comment
   ^   o fun18 sec02 sub12:
   ^     - move to next line
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun18 Sec02 Sub01:
   *   - move past header and check if index column
   \*****************************************************/

   tmpStr =
      (signed char *)
      fgets(
         (char *) buffStr,
         def_buffSize_fun18,
         (FILE *) acntFILE
      );

   if(! tmpStr)
      goto noFile_fun18_sec05; /*nothing in file*/


   while(*tmpStr < 33)
   { /*Loop: move past white space*/
      if(*tmpStr == '\0')
         goto noFile_fun18_sec05; /*nothing in file*/
      ++tmpStr;
   } /*Loop: move past white space*/

   if(
         (tmpStr[0] & ~32) == 'I'
      && (tmpStr[1] & ~32) == 'N'
      && (tmpStr[2] & ~32) == 'D'
      && (tmpStr[3] & ~32) == 'E'
      && (tmpStr[4] & ~32) == 'X'
      && tmpStr[5] < 33
   ) indexColBl = 1;
     /*first column has index's*/


   ++lineSL;

   /*****************************************************\
   * Fun18 Sec02 Sub02:
   *   - move past initial white space + allocate + loop
   \*****************************************************/

   while(
      fgets(
         (char *) buffStr,
         def_buffSize_fun18,
         (FILE *) acntFILE
      )
   ){ /*Loop: read in file*/


      /*move past any initial white space*/
      posUS = 0;

      while(buffStr[posUS] < 33)
      { /*Loop: find start of entry*/
         if(buffStr[posUS] == '\0')
            break;
         ++posUS;
      } /*Loop: find start of entry*/

      if(buffStr[posUS] < 33)
         continue; /*blank line*/

      /*make sure have enough memory for this entry*/
      if(acntSTPtr->numEntrySL >= acntSTPtr->sizeEntrySL)
      { /*If: need more memory for transactions*/
         if(
            reallocEntry_st_acnt(
               acntSTPtr,
               acntSTPtr->numEntrySL << 1
            )
         ) goto memErr_fun18_sec05;
      } /*If: need more memory for transactions*/

      /**************************************************\
      * Fun18 Sec02 Sub03:
      *   - move past index column (if have one)
      \**************************************************/

      if(indexColBl)
      { /*If: have index column*/
         while(buffStr[posUS] > 32)
            ++posUS;

         while(buffStr[posUS] < 33)
         { /*Loop: move to parent entry*/
            if(buffStr[posUS] == '\0')
               break;
            ++posUS;
         } /*Loop: move to parent entry*/

         if(buffStr[posUS] < 33)
            goto fileErr_fun18_sec05;
            /*line only had index entry*/
      } /*If: have index column*/
     
      /**************************************************\
      * Fun18 Sec02 Sub04:
      *   - read in parent account id
      *   o fun18 sec02 sub04 cat01:
      *     - find length of parent account id
      *   o fun18 sec02 sub04 cat02:
      *     - find parent account and copy if new
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun18 Sec02 Sub04 Cat01:
      +   - find length of parent account id
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      /*find end of parent account entry*/
      endUS = posUS;
      while(buffStr[endUS] > 32)
         ++endUS;


      if(endUS - posUS > def_lenStr_strAry)
         goto fileErr_fun18_sec05;
         /*longer than allocated memory*/


      if(buffStr[endUS] == 32)
         ; /*if space*/

      else if(buffStr[endUS] == '\t')
         ; /*if tab*/

      else
         goto fileErr_fun18_sec05;
         /*else end of line or weird asscii*/


      buffStr[endUS] = '\0';

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun18 Sec02 Sub04 Cat02:
      +   - find parent account and copy if new
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(endUS - posUS >= def_lenStr_strAry - 1)
         goto fileErr_fun18_sec05;
         /*account name to long*/

      indexSL =
         addNewPar_st_acnt(
            &buffStr[posUS],
            acntSTPtr
         );

      if(indexSL == def_memErr_acnt)
         goto memErr_fun18_sec05;

      /*add parent account index to entry*/
      acntSTPtr->parAryUS[acntSTPtr->numEntrySL] =
         acntSTPtr->parToNumAryUS[indexSL];

      posUS = endUS + 1;

      /**************************************************\
      * Fun18 Sec02 Sub05:
      *   - read in child account id
      *   o fun18 sec02 sub05 cat01:
      *     - find length of chile account id
      *   o fun18 sec02 sub05 cat02:
      *     - find chile account and copy if new
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun18 Sec02 Sub05 Cat01:
      +   - find length of child account id
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      while(buffStr[posUS] < 33)
      { /*Loop: find start of child account id*/
         if(buffStr[posUS] == '\0')
            goto fileErr_fun18_sec05;
         ++posUS;
      } /*Loop: find start of child account id*/


      /*find end of child account entry*/
      endUS = posUS;
      while(buffStr[endUS] > 32)
         ++endUS;


      if(endUS - posUS > def_lenStr_strAry)
         goto fileErr_fun18_sec05;
         /*longer than allocated memory*/


      if(buffStr[endUS] == 32)
         ; /*if space*/

      else if(buffStr[endUS] == '\t')
         ; /*if tab*/

      else
         goto fileErr_fun18_sec05;
         /*else end of line or weird asscii*/


      buffStr[endUS] = '\0';

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun18 Sec02 Sub05 Cat02:
      +   - find child account and copy if new
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      /*find if already have child account*/
      indexSL =
         addNewChild_st_acnt(
            &buffStr[posUS],
            acntSTPtr
         );

      if(indexSL == def_memErr_acnt)
         goto memErr_fun18_sec05;

      /*add child account index to entry*/
      acntSTPtr->childAryUS[acntSTPtr->numEntrySL] =
         acntSTPtr->childToNumAryUS[indexSL];

      posUS = endUS + 1;

      /**************************************************\
      * Fun18 Sec02 Sub06:
      *   - get year
      \**************************************************/

      while(buffStr[posUS] < 33)
      { /*Loop: find start of year*/
         if(buffStr[posUS] == '\0')
            goto fileErr_fun18_sec05;
         ++posUS;
      } /*Loop: find start of year*/


      posUS +=
         strToSS_base10str(
            &buffStr[posUS],
            &acntSTPtr->yearArySS[acntSTPtr->numEntrySL]
         ); /*convert string to number*/


      if(buffStr[posUS] == 32)
         ; /*if space*/

      else if(buffStr[posUS] == '\t')
         ; /*if tab*/

      else
         goto fileErr_fun18_sec05;
         /*else end of line or weird asscii*/

      /**************************************************\
      * Fun18 Sec02 Sub07:
      *   - get month
      \**************************************************/

      while(buffStr[posUS] < 33)
      { /*Loop: find start of year*/
         if(buffStr[posUS] == '\0')
            goto fileErr_fun18_sec05;
         ++posUS;
      } /*Loop: find start of year*/


      posUS +=
         strToSC_base10str(
            &buffStr[posUS],
            &acntSTPtr->monthArySC[acntSTPtr->numEntrySL]
         ); /*convert string to number*/


      if(buffStr[posUS] == 32)
         ; /*if space*/

      else if(buffStr[posUS] == '\t')
         ; /*if tab*/

      else
         goto fileErr_fun18_sec05;
         /*else end of line or weird asscii*/

      /**************************************************\
      * Fun18 Sec02 Sub08:
      *   - get day
      \**************************************************/

      while(buffStr[posUS] < 33)
      { /*Loop: find start of year*/
         if(buffStr[posUS] == '\0')
            goto fileErr_fun18_sec05;
         ++posUS;
      } /*Loop: find start of year*/


      posUS +=
         strToSC_base10str(
            &buffStr[posUS],
            &acntSTPtr->dayArySC[acntSTPtr->numEntrySL]
         ); /*convert string to number*/


      if(buffStr[posUS] == 32)
         ; /*if space*/

      else if(buffStr[posUS] == '\t')
         ; /*if tab*/

      else
         goto fileErr_fun18_sec05;
         /*else end of line or weird asscii*/

      /**************************************************\
      * Fun18 Sec02 Sub09:
      *   - get transaction amount
      \**************************************************/

      while(buffStr[posUS] < 33)
      { /*Loop: find start of year*/
         if(buffStr[posUS] == '\0')
            goto fileErr_fun18_sec05;
         ++posUS;
      } /*Loop: find start of year*/


      posUS +=
         strToF_base10str(
            &buffStr[posUS],
            &acntSTPtr->tranAryF[acntSTPtr->numEntrySL]
         ); /*convert string to number*/


      if(buffStr[posUS] == 32)
         ; /*if space*/

      else if(buffStr[posUS] == '\t')
         ; /*if tab*/

      else
         goto fileErr_fun18_sec05;
         /*else end of line or weird asscii*/

      /**************************************************\
      * Fun18 Sec02 Sub10:
      *   - skip total column (re-calcuate)
      \**************************************************/

      while(buffStr[posUS] < 33)
      { /*Loop: find start of year*/
         if(buffStr[posUS] == '\0')
            goto fileErr_fun18_sec05;
         ++posUS;
      } /*Loop: find start of year*/


      while(buffStr[posUS] > 32)
      { /*Loop: find start of year*/
         if(buffStr[posUS] == '\0')
            goto fileErr_fun18_sec05;
         ++posUS;
      } /*Loop: find start of year*/

      /*at this point there are no more required entries*/

      /**************************************************\
      * Fun18 Sec02 Sub11:
      *   - get comment
      \**************************************************/

      while(buffStr[posUS] < 33)
      { /*Loop: find start of year*/
         if(buffStr[posUS] == '\0')
            goto fileErr_fun18_sec05;
         ++posUS;
      } /*Loop: find start of year*/

      if(buffStr[posUS] == '\0')
      { /*If: no comment*/
         if(
            add_str_ptrAry(
               (signed char *) "NA",
               acntSTPtr->commentAryST,
               acntSTPtr->numEntrySL
            )
         ) goto memErr_fun18_sec05;
      } /*If: no comment*/

      else
      { /*Else: have comment*/
         if(
            add_str_ptrAry(
               &buffStr[posUS],
               acntSTPtr->commentAryST,
               acntSTPtr->numEntrySL
            )
         ) goto memErr_fun18_sec05;

         /*remove carradge returns and new lines at end*/
         if(
             acntSTPtr->commentAryST->strAry[
                acntSTPtr->numEntrySL
             ][
                acntSTPtr->commentAryST->lenAryUI[
                   acntSTPtr->numEntrySL
                ] - 2
             ] == '\r'
         ){ /*If: copied carriage return*/
             acntSTPtr->commentAryST->strAry[
                acntSTPtr->numEntrySL
             ][
                acntSTPtr->commentAryST->lenAryUI[
                   acntSTPtr->numEntrySL
                ] - 2
             ] = '\0';
         }  /*If: copied carriage return*/

         else if(
             acntSTPtr->commentAryST->strAry[
                acntSTPtr->numEntrySL
             ][
                acntSTPtr->commentAryST->lenAryUI[
                   acntSTPtr->numEntrySL
                ] - 1
             ] == '\n'
         ){ /*Else If: copied new line*/
             acntSTPtr->commentAryST->strAry[
                acntSTPtr->numEntrySL
             ][
                acntSTPtr->commentAryST->lenAryUI[
                   acntSTPtr->numEntrySL
                ] - 1
             ] = '\0';
         }  /*Else If: copied new line*/
      } /*Else: have comment*/

      /**************************************************\
      * Fun18 Sec02 Sub12:
      *   - move to next line
      \**************************************************/

      /*move to next entry for account*/
      ++acntSTPtr->numEntrySL;
   }  /*Loop: read in file*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun18 Sec05:
   ^   - sort and return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   sortDate_st_acnt(acntSTPtr);

   return 0;

   memErr_fun18_sec05:;
      return def_memErr_acnt;

   noFile_fun18_sec05:;
      return def_noFILE_acnt;

   fileErr_fun18_sec05:;
      return (lineSL << 4) | def_fileErr_acnt;
} /*readFile_acnt*/

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
|       * 0 do not round
|   - persisionUC:
|     o number decimals to print (prentend/mock delete)
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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun19 TOC:
   '   - adds entry to an account
   '   o fun19 sec01:
   '     - variable declarations
   '   o fun19 sec02:
   '     - deal with missing entries
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*these values are for pretend addition only*/
   signed char tranStr[32];/*transaction amount to print*/
   signed char totalStr[32]; /*total to print; mock only*/

   signed int parIndexSI = 0;
   signed int childIndexSI = 0;
   unsigned short parUS = 0; /*for sanity*/
   unsigned short childUS = 0; /*for sanity*/
   unsigned short ghostUS = 0; /*for sanity*/

   float addTotalF = 0;       /*total for added entry*/
   float totalF = 0;          /*accountFtotal; mock only*/

   float ghostF = 0;
   float ghostTotalF = 0;
   signed long ghostIndexSL = 0;

   /*this is needed for true delete*/
   signed long slElm = 0;     /*element on in loop*/
   signed long indexSL = 0;
   signed long slSwap = 0;    /*index to swap with*/
   signed char pBl = 1;    /*marks if need to print add*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec02:
   ^   - deal with missing entries
   ^   o fun19 sec02 sub01:
   ^     - deal with missing entries
   ^   o fun19 sec02 sub02:
   ^     - accountFfor ghost account
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun19 Sec02 Sub01:
   *   - deal with missing entries
   \*****************************************************/

   if(! childStr)
      goto missingEntry_fun19_sec05;

   if(! parStr)
      parStr = (signed char *) "NA";

   if(! commentStr)
      commentStr = (signed char *) "NA";

   if(yearSS < 2000)
      goto missingEntry_fun19_sec05;

   if(
         ! monthSC
      || monthSC > 12
   ) goto missingEntry_fun19_sec05;

   if(
         ! daySC
      || daySC > 31
   ) goto missingEntry_fun19_sec05;

   while(commentStr[indexSL] != '\0')
   { /*Loop: remove white space*/
      if(commentStr[indexSL] < 33)
         commentStr[indexSL] = '_';
      ++indexSL;
   } /*Loop: remove white space*/

   indexSL = 0;

   /*****************************************************\
   * Fun19 Sec02 Sub02:
   *   - accountFfor ghost account
   \*****************************************************/

   if(ghostBl)
   { /*If: applying ghost rounding*/

      ghostF = amountF - ((signed int) amountF);

      if(ghostF < 0)
         ghostF *= -1;
 
      amountF = (signed int) amountF;

      if(amountF < 0)
         --amountF; /*round up*/

      ghostTotalF = ghostF;

      ghostIndexSL = 
         find_strAry(
            acntSTPtr->uniqChildStr,
            (signed char *) "ghost",
            acntSTPtr->numChildUS
         ); /*see if have ghost accountFentry*/
            /*is -1 if no ghost accountFentry*/
   } /*If: applying ghost rounding*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec03:
   ^   - do mock addition (if file input)
   ^   o fun19 sec03 sub01:
   ^     - check if entry from same as insert + start loop
   ^   o fun19 sec03 sub02:
   ^     - entry from same parent accountFas deleted entry
   ^   o fun19 sec03 sub03:
   ^     - check if added accountF(print if time)
   ^   o fun19 sec03 sub04:
   ^     - print non-deleted entry + end loop
   ^   o fun19 sec03 sub05:
   ^     - print added entry line (let user see feedback)
   ^   o fun19 sec03 sub06:
   ^     - print ghost entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun19 Sec03 Sub01:
   *   - check if entry from same as insert + start loop
   \*****************************************************/

   if(outFILE)
   { /*If: mock add*/
      parIndexSI =
         find_strAry(
            acntSTPtr->uniqParStr,
            parStr,
            acntSTPtr->numParUS
         );
      if(parIndexSI < 0)
         goto pNewEntry_fun19_sec03_sub05;
         /*new accountF*/
      parUS =
         (signed int)
         acntSTPtr->parToNumAryUS[parIndexSI];

      childIndexSI =
         find_strAry(
            acntSTPtr->uniqChildStr,
            childStr,
            acntSTPtr->numChildUS
         );
      if(childIndexSI < 0)
         goto pNewEntry_fun19_sec03_sub05;
         /*new accountF*/
      childUS =
         (signed int)
         acntSTPtr->childToNumAryUS[childIndexSI];

      ghostIndexSL =
         find_strAry(
            acntSTPtr->uniqChildStr,
            (signed char *) "ghost",
            acntSTPtr->numChildUS
         );
      if(ghostIndexSL < 0)
         ghostUS = (unsigned short) -1;
         /*user should never have 62k child accounts*/
      else
         ghostUS =
            (signed int)
            acntSTPtr->childToNumAryUS[ghostIndexSL];

      totalF = 0;

      /*printing header for user*/
      fprintf(
         (FILE *) outFILE,
         "par\tchild\tyear\tmonth\tday\tamount\ttotal"
      );

      fprintf(
         (FILE *) outFILE,
         "\treason%s",
         str_endLine
      );

      for(
         slElm = 0;
         slElm < acntSTPtr->numEntrySL;
         ++slElm
      ){ /*Loop: print current entries*/

         if(acntSTPtr->parAryUS[slElm] != parUS)
            continue; /*if different accountF*/

         /***********************************************\
         * Fun19 Sec03 Sub02:
         *   - entry from same child accountFas deleted
         \***********************************************/

         if(
               ghostBl
            && acntSTPtr->childAryUS[slElm] == ghostUS
         ){ /*If: ghost account entry*/
             if(yearSS < acntSTPtr->yearArySS[slElm])
                continue; /*If: entry is after insert*/ 
             else if(yearSS > acntSTPtr->yearArySS[slElm])
                ; /*If: entry is before insert*/ 
             else if(
                  monthSC
                < acntSTPtr->monthArySC[slElm]
             ) continue; /*If: entry is after insert*/ 
             else if(
                  monthSC
                > acntSTPtr->monthArySC[slElm]
             ) continue; /*If: entry is before insert*/ 
             else if(
                  daySC
                < acntSTPtr->dayArySC[slElm]
             ) continue; /*If: entry is after insert*/ 

             ghostTotalF += acntSTPtr->tranAryF[slElm];

             continue;
         }  /*If: ghost account entry*/

         else if(acntSTPtr->childAryUS[slElm] != childUS)
            continue; /*if different accountF*/
            /*different account*/
               
         /***********************************************\
         * Fun19 Sec03 Sub03:
         *   - check if added accountF(print if time)
         \***********************************************/

         if(yearSS > acntSTPtr->yearArySS[slElm])
            ; /*If: entry is before insert*/ 

         else if(yearSS < acntSTPtr->yearArySS[slElm])
            goto insertEntry_fun19_sec03_sub03;

         else if(monthSC > acntSTPtr->monthArySC[slElm])
            ; /*Else If: entry is before insert by moth*/ 

         else if(monthSC < acntSTPtr->monthArySC[slElm])
            goto insertEntry_fun19_sec03_sub03;

         else if(daySC < acntSTPtr->dayArySC[slElm])
         { /*Else If: entry is earlier*/
            insertEntry_fun19_sec03_sub03:;
               if(pBl)
               { /*If: need to print new entry*/
                  pBl = 0; /*mark entry is printed*/

                  totalF += amountF;
                  addTotalF = totalF;

                  /*floats are not 100% accurate,
                  `   so need to do some rounding
                  */
                  double_numToStr(
                     tranStr,
                     amountF,
                     percisionUC
                  ); /*convert transaction to string*/

                  double_numToStr(
                     totalStr,
                     addTotalF,
                     percisionUC
                  ); /*covert total to string*/
                  
                  fprintf(
                     (FILE *) outFILE,
                     "%s\t%s\t%i\t%02i\t%02i\t%s\t%s",
                     parStr,
                     childStr,
                     yearSS,
                     monthSC,
                     daySC,
                     tranStr,
                     totalStr
                  );

                  fprintf(
                     (FILE *) outFILE,
                     "\t%s%s",
                     commentStr,
                     str_endLine
                  );
               } /*If: need to print new entry*/
         } /*Else If: entry is earlier*/

         /*else entry comes after*/

         /***********************************************\
         * Fun19 Sec03 Sub04:
         *   - print non-added entry + end loop
         \***********************************************/

         totalF += acntSTPtr->tranAryF[slElm];

         /*floats are not 100% accurate, so need to do
         `   some rountFng
         */
         double_numToStr(
            tranStr,
            acntSTPtr->tranAryF[slElm],
            percisionUC
         ); /*convert transaction to string*/

         double_numToStr(
            totalStr,
            totalF,
            percisionUC
         ); /*covert total to string*/
         
         fprintf(
            (FILE *) outFILE,
            "%s\t%s\t%i\t%02i\t%02i\t%s\t%s\t%s%s",
            parStr,
            childStr,
            acntSTPtr->yearArySS[slElm],
            acntSTPtr->monthArySC[slElm],
            acntSTPtr->dayArySC[slElm],
            tranStr,
            totalStr,
            acntSTPtr->commentAryST->strAry[slElm],
            str_endLine
         );
      } /*Loop: print current entries*/

      /**************************************************\
      * Fun19 Sec03 Sub05:
      *   - print added entry
      *   o fun19 sec03 sub05 cat01:
      *     - make sure printed inserted entry
      *   o fun19 sec03 sub05 cat02:
      *     - printed added entries in
      \**************************************************/

      pNewEntry_fun19_sec03_sub05:;

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun19 Sec03 Sub05 Cat01:
      +   - make sure printed inserted entry
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(pBl)
      { /*If: need to print new entry*/
         pBl = 0; /*mark entry is printed*/

         totalF += amountF;
         addTotalF = totalF;

         /*floats are not 100% accurate,
         `   so need to do some rounding
         */
         double_numToStr(
            tranStr,
            amountF,
            percisionUC
         ); /*convert transaction to string*/

         double_numToStr(
            totalStr,
            addTotalF,
            percisionUC
         ); /*covert total to string*/
         
         totalF += acntSTPtr->tranAryF[indexSL];

         fprintf(
            (FILE *) outFILE,
            "%s\t%s\t%i\t%02i\t%02i\t%s\t%s",
            parStr,
            childStr,
            yearSS,
            monthSC,
            daySC,
            tranStr,
            totalStr
         );

         fprintf(
            (FILE *) outFILE,
            "\t%s%s",
            commentStr,
            str_endLine
         );
      } /*If: need to print new entry*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun19 Sec03 Sub05 Cat02:
      +   - printed added entries in
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      fprintf(
         (FILE *) outFILE,
         "%s",
         str_endLine
      ); /*clear break after summary*/

      /*floats are not 100% accurate, so need to do
      `   some rountFng
      */
      double_numToStr(
         tranStr,
         amountF,
         percisionUC
      ); /*convert transaction to string*/

      double_numToStr(
         totalStr,
         addTotalF,
         percisionUC
      ); /*covert total to string*/
      
      totalF += acntSTPtr->tranAryF[indexSL];

      fprintf(
         (FILE *) outFILE,
         "added: %s\t%s\t%i\t%02i\t%02i\t%s\t%s\t%s%s",
         parStr,
         childStr,
         yearSS,
         monthSC,
         daySC,
         tranStr,
         totalStr,
         commentStr,
         str_endLine
      );

      /**************************************************\
      * Fun19 Sec03 Sub06:
      *   - print ghost entry
      \**************************************************/

      if(ghostF != 0)
      { /*If: have ghost accountFtransaction*/ 
         /*floats are not 100% accurate, so need to do
         `   some rountFng
         */
         double_numToStr(
            tranStr,
            ghostF,
            percisionUC
         ); /*convert transaction to string*/

         double_numToStr(
            totalStr,
            ghostTotalF,
            percisionUC
         ); /*covert total to string*/
         
         totalF += acntSTPtr->tranAryF[indexSL];

         fprintf(
            (FILE *) outFILE,
            "added: %s\t%s\t%i\t%02i\t%02i\t%s\t%s\t%s%s",
            parStr,
            "ghost",
            yearSS,
            monthSC,
            daySC,
            tranStr,
            totalStr,
            commentStr,
            str_endLine
         );
      } /*If: have ghost accountFtransaction*/ 

      fprintf(
         (FILE *) outFILE,
         "%s",
         str_endLine
      ); /*clear break after added entreis*/
   } /*If: mock add*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec04:
   ^   - do reall delete (else no file input)
   ^   o fun19 sec04 sub01:
   ^     - allocate memory and add parent/child accountF
   ^   o fun19 sec04 sub02:
   ^     - find insertion index
   ^   o fun19 sec04 sub03:
   ^     - insert new entry
   ^   o fun19 sec04 sub04:
   ^     - insert new ghost entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun19 Sec04 Sub01:
   *   - allocate memory and add parent/child accountF
   \*****************************************************/
 
   else
   { /*Else: no file; doing real addition*/
      if(
            (acntSTPtr->numEntrySL + 2) /*+1 for ghost*/
         >= acntSTPtr->sizeEntrySL
      ){ /*If: need to resize*/
         if(
            reallocEntry_st_acnt(
               acntSTPtr,
               acntSTPtr->sizeEntrySL
                  + ( 8 + (acntSTPtr->sizeEntrySL << 1) )
            )
         ) goto memErr_fun19_sec05;
      } /*If: need to resize*/


      /*only addNewPar_st_acnt and addNewChild_st_acnt
      `  only adds parent/child if parent/child is new,
      `  so no need to check if accountFalready exists
      */
      parIndexSI =
         addNewPar_st_acnt(
            parStr,
            acntSTPtr
         );

      if( parIndexSI == def_memErr_acnt )
         goto memErr_fun19_sec05;

      parIndexSI =
         (signed int)
         acntSTPtr->parToNumAryUS[parIndexSI];


      childIndexSI =
         addNewChild_st_acnt(
            childStr,
            acntSTPtr
         );

      if( childIndexSI == def_memErr_acnt )
         goto memErr_fun19_sec05;

      childIndexSI =
         (signed int)
         acntSTPtr->childToNumAryUS[childIndexSI];


      if(ghostBl && ghostF != 0)
      { /*If: using the ghost accountF*/
         indexSL =
            addNewChild_st_acnt(
               (signed char *) "ghost",
               acntSTPtr
            );

         if( indexSL == def_memErr_acnt )
            goto memErr_fun19_sec05;

         ghostUS = acntSTPtr->childToNumAryUS[indexSL];
      } /*If: using the ghost accountF*/

      /**************************************************\
      * Fun19 Sec04 Sub02:
      *   - find insertion index
      \**************************************************/

      indexSL =
         findDate_st_acnt(
            acntSTPtr,
            yearSS,
            monthSC,
            daySC,
            (unsigned short) parIndexSI,
            (unsigned short) childIndexSI
         );

      if(indexSL < 0)
         indexSL *= -1;


      parUS = (unsigned short) parIndexSI;
      childUS = (unsigned short) childIndexSI;

      /*one of these will often be the case; get inset
      `  index
      */
      if(yearSS < acntSTPtr->yearArySS[indexSL]) 
         ;
      else if(yearSS > acntSTPtr->yearArySS[indexSL]) 
         ++indexSL;
      else if(monthSC < acntSTPtr->monthArySC[indexSL]) 
         ;
      else if(monthSC > acntSTPtr->monthArySC[indexSL]) 
         ++indexSL;
      else if(daySC < acntSTPtr->dayArySC[indexSL]) 
         ;
      else if(daySC > acntSTPtr->dayArySC[indexSL]) 
         ++indexSL;
      else if(parUS < acntSTPtr->parAryUS[indexSL]) 
         ;
      else if(parUS > acntSTPtr->parAryUS[indexSL]) 
         ++indexSL;
      else if(childUS < acntSTPtr->childAryUS[indexSL]) 
         ;
      else if(childUS > acntSTPtr->childAryUS[indexSL]) 
         ++indexSL;
      else
      { /*Else: might be match, find end of match*/
         while(
               yearSS == acntSTPtr->yearArySS[indexSL]
            && monthSC == acntSTPtr->monthArySC[indexSL]
            && daySC == acntSTPtr->dayArySC[indexSL]
            && parUS == acntSTPtr->parAryUS[indexSL]
            && childUS == acntSTPtr->childAryUS[indexSL]
         ) ++indexSL;
      } /*Else: might be match, find end of match*/

      /**************************************************\
      * Fun19 Sec04 Sub03:
      *   - insert new entry
      \**************************************************/

      if(ghostBl && ghostF != 0)
         slSwap = acntSTPtr->numEntrySL + 1;
      else
         slSwap = acntSTPtr->numEntrySL;

      for(
         slElm = acntSTPtr->numEntrySL - 1;
         slElm >= indexSL;
         --slElm
      ){ /*Loop: move later entries up*/
         swapEntry_st_acnt(
            acntSTPtr,
            slSwap,
            slElm
         );

         --slSwap;
      } /*Loop: move later entries up*/


      acntSTPtr->parAryUS[indexSL] = parUS;
      acntSTPtr->childAryUS[indexSL] = childUS;
      acntSTPtr->yearArySS[indexSL] = yearSS;
      acntSTPtr->monthArySC[indexSL] = monthSC;
      acntSTPtr->dayArySC[indexSL] = daySC;
      acntSTPtr->tranAryF[indexSL] = amountF;

      if(
         add_str_ptrAry(
            commentStr,
            acntSTPtr->commentAryST,
            indexSL
         )
      ) goto memErr_fun19_sec05;

      ++acntSTPtr->numEntrySL; /*account for insert*/

      /**************************************************\
      * Fun19 Sec04 Sub04:
      *   - insert new ghost entry
      \**************************************************/

      if(ghostF != 0)
      { /*If: adding ghost accountFentry*/
         ++indexSL;
         acntSTPtr->parAryUS[indexSL] = parUS;
         acntSTPtr->childAryUS[indexSL] = ghostUS;
         acntSTPtr->yearArySS[indexSL] = yearSS;
         acntSTPtr->monthArySC[indexSL] = monthSC;
         acntSTPtr->dayArySC[indexSL] = daySC;
         acntSTPtr->tranAryF[indexSL] = ghostF;

         if(
            add_str_ptrAry(
               commentStr,
               acntSTPtr->commentAryST,
               indexSL
            )
         ) goto memErr_fun19_sec05;

         ++acntSTPtr->numEntrySL; /*accountFfor insert*/
      } /*If: adding ghost accountFentry*/

   } /*Else: no file; doing real addition*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun19 Sec05:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   memErr_fun19_sec05:;
      return def_memErr_acnt;

   missingEntry_fun19_sec05:;
      return def_missingEntry_acnt;
} /*addEntry_st_acnt*/

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
|   - Returns:
|     o -1 if could not delete index
\-------------------------------------------------------*/
signed char
delIndex_st_acnt(
   struct st_acnt *acntSTPtr, /*holds entries from file*/
   signed long indexSL,       /*index to delete*/
   unsigned char percisionUC, /*amount of percision*/
   void *outFILE              /*0 reall; FILE pretend*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun20 TOC:
   '   - deletes an entry from a st_acnt struct by index
   '   o fun20 sec01:
   '     - variable declarations
   '   o fun20 sec02:
   '     - check if can delete + get acount name
   '   o fun20 sec03:
   '     - do mock delete (if file input)
   '   o fun20 sec04:
   '     - do reall delete (else no file input)
   '   o fun20 sec05:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun20 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*these values are for pretend delete only*/
   signed char *parStr = 0;   /*parent account name*/
   signed char *childStr = 0; /*child accont name*/
   signed char tranStr[32];/*transaction amount to print*/
   signed char totalStr[32]; /*total to print; mock only*/

   float delTotalF = 0;       /*total for deleted entry*/
   signed char foundDelBl = 0;
      /*tells when to stop delTotalF in mock delete*/
   float totalF = 0;          /*account total; mock only*/
   unsigned short usChild = 0; /*for getting account name*/

   /*this is needed for true delete*/
   signed long slElm = 0;     /*element on in loop*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun20 Sec02:
   ^   - check if can delete + get acount name
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(indexSL >= acntSTPtr->numEntrySL)
      goto noIndex_fun20_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun20 Sec03:
   ^   - do mock delete (if file input)
   ^   o fun20 sec03 sub01:
   ^     - get account; check if deleted entry + loop
   ^   o fun20 sec03 sub02:
   ^     - entry from same parent account as deleted entry
   ^   o fun20 sec03 sub03:
   ^     - entry from same child account as deleted entry
   ^   o fun20 sec03 sub04:
   ^     - print non-deleted entry + end loop
   ^   o fun20 sec03 sub05:
   ^     - print deleted entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun20 Sec03 Sub01:
   *   - get account; check if deleted entry + print loop
   \*****************************************************/

   if(outFILE)
   { /*If: mock delete*/

      usChild = acntSTPtr->parAryUS[indexSL];
      usChild = acntSTPtr->numToParAryUS[usChild];
      parStr =
         get_strAry(
            acntSTPtr->uniqParStr,
            usChild
         );

      usChild = acntSTPtr->childAryUS[indexSL];
      usChild = acntSTPtr->numToChildAryUS[usChild];
      childStr =
         get_strAry(
            acntSTPtr->uniqChildStr,
            usChild
         );

      /*printing header for user*/
      fprintf(
         (FILE *) outFILE,
         "par\tchild\tyear\tmonth\tday\tamount\ttotal"
      );

      fprintf(
         (FILE *) outFILE,
         "\treason%s",
         str_endLine
      );

      for(
         slElm = 0;
         slElm < acntSTPtr->numEntrySL;
         ++slElm
      ){ /*Loop: print current entries*/

         if(slElm == indexSL)
         { /*If: found deletion item*/
            foundDelBl = 0;
            continue; /*index deleting*/
         } /*If: found deletion item*/

         /***********************************************\
         * Fun20 Sec03 Sub02:
         *   - entry from same parent account as deleted
         \***********************************************/

         if(
               acntSTPtr->parAryUS[slElm]
            != acntSTPtr->parAryUS[indexSL]
         ) continue; /*if different account*/

         /***********************************************\
         * Fun20 Sec03 Sub03:
         *   - entry from same child account as deleted
         \***********************************************/

         if(
               acntSTPtr->childAryUS[slElm]
            != acntSTPtr->childAryUS[indexSL]
         ) continue; /*if different account*/
               
         /***********************************************\
         * Fun20 Sec03 Sub04:
         *   - print non-deleted entry + end loop
         \***********************************************/

         totalF += acntSTPtr->tranAryF[slElm];

         if(! foundDelBl)
            delTotalF = totalF;
            /*updated deletion entry total*/

         /*floats are not 100% accurate, so need to do
         `   some rounding
         */
         double_numToStr(
            tranStr,
            acntSTPtr->tranAryF[slElm],
            percisionUC
         ); /*convert transaction to string*/

         double_numToStr(
            totalStr,
            totalF,
            percisionUC
         ); /*covert total to string*/
         
         fprintf(
            (FILE *) outFILE,
            "%s\t%s\t%i\t%02i\t%02i\t%s\t%s\t%s%s",
            parStr,
            childStr,
            acntSTPtr->yearArySS[slElm],
            acntSTPtr->monthArySC[slElm],
            acntSTPtr->dayArySC[slElm],
            tranStr,
            totalStr,
            acntSTPtr->commentAryST->strAry[slElm],
            str_endLine
         );
      } /*Loop: print current entries*/

      /**************************************************\
      * Fun20 Sec03 Sub05:
      *   - print deleted entry
      \**************************************************/

      double_numToStr(
         tranStr,
         acntSTPtr->tranAryF[indexSL],
         percisionUC
      ); /*convert transaction to string*/

      double_numToStr(
         totalStr,
         delTotalF,
         percisionUC
      ); /*convert transaction to string*/

      fprintf(
         (FILE *) outFILE,
         "\ndelete: %s\t%s\t%i\t%02i\t%02i\t%s\t%s\t%s%s",
         parStr,
         childStr,
         acntSTPtr->yearArySS[indexSL],
         acntSTPtr->monthArySC[indexSL],
         acntSTPtr->dayArySC[indexSL],
         tranStr,
         totalStr,
         acntSTPtr->commentAryST->strAry[indexSL],
         str_endLine
      );
   } /*If: mock delete*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun20 Sec04:
   ^   - do reall delete (else no file input)
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   else
   { /*Else: no file; doing real delete*/
      for(
         slElm = indexSL + 1;
         slElm < acntSTPtr->numEntrySL;
         ++slElm
      ){ /*Loop: delete account*/

         swapEntry_st_acnt(
            acntSTPtr,
            indexSL,
            slElm
         );

         ++indexSL;
      } /*Loop: delete account*/

      /*subtract transaction from account (lazy delete)*/
      --acntSTPtr->numEntrySL;
      --acntSTPtr->commentAryST->lenSL;
   } /*Else: no file; doing real delete*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun20 Sec05:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   noIndex_fun20_sec05:;
      return -1;
} /*delEntry_st_acnt*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun21 TOC:
   '   - prints entry at input index
   '   o fun21 sec01:
   '     - variable declarations
   '   o fun21 sec02:
   '     - check if have valid index and get total
   '   o fun21 sec03:
   '     - print entry
   '   o fun21 sec04:
   '     - return result
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun21 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed long slElm = 0;
   float totalF = 0;
   unsigned short parUS = 0;
   unsigned short childUS = 0;

   signed char tranStr[32];
   signed char totalStr[32];

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun21 Sec02:
   ^   - check if have valid index and get total
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(indexSL < 0)
      goto noIndex_fun21;
      /*negative number*/

   if(acntSTPtr->numEntrySL <= indexSL)
      goto noIndex_fun21;
      /*out of bounds*/
  
   if(! outFILE)
      outFILE = stdout;

   parUS = acntSTPtr->parAryUS[indexSL];
   childUS = acntSTPtr->childAryUS[indexSL];
   
   for(
      slElm = 0;
      slElm < indexSL;
      ++slElm
   ){ /*Loop: find total*/

      if(parUS != acntSTPtr->parAryUS[slElm])
         continue;
      else if(parUS != acntSTPtr->childAryUS[slElm])
         continue;
      else
         totalF += acntSTPtr->tranAryF[slElm];

   } /*Loop: find total*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun21 Sec03:
   ^   - print entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   parUS = acntSTPtr->numToParAryUS[parUS];
   childUS = acntSTPtr->numToChildAryUS[childUS];

   double_numToStr(
      tranStr,
      acntSTPtr->tranAryF[indexSL],
      percisionUC
   ); /*convert transaction to string*/

   double_numToStr(
      totalStr,
      totalF,
      percisionUC
   ); /*convert transaction to string*/

   fprintf(
      (FILE *) outFILE,
      "%s\t%s\t%i\t%02i\t%02i\t%s\t%s\t%s%s",
      get_strAry(
         acntSTPtr->uniqParStr,
         parUS
      ),
      get_strAry(
         acntSTPtr->uniqChildStr,
         childUS
      ),
      acntSTPtr->yearArySS[indexSL],
      acntSTPtr->monthArySC[indexSL],
      acntSTPtr->dayArySC[indexSL],
      tranStr,
      totalStr,
      acntSTPtr->commentAryST->strAry[indexSL],
      str_endLine
   );

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun21 Sec04:
   ^   - return result
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   return 0;

   noIndex_fun21:;
     return def_missingEntry_acnt;
} /*pIndex_st_acnt*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun22 TOC:
   '   - print all entries matching input info
   '   o fun22 sec02:
   '     - memory allocation
   '   o fun22 sec03:
   '     - get parent/child index and get starting index
   '   o fun22 sec04:
   '     - print entries
   '   o fun22 sec06:
   '     - clean up and return 
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   signed char errSC = 0;
   signed char *tmpParStr = 0;
   signed char *tmpChildStr = 0;

   signed int parIndexSI = 0;   /*parent index*/
   signed int childIndexSI = 0; /*child index*/

   signed long indexSL = 0;
   signed long slTotal = 0; /*index at in total*/
   float totalF = 0;

   float *totalHeapAryF = 0;
   float *parTotalHeapAryF = 0;
   signed char *hitChildHeapAryBl = 0;
   signed char *hitHeapAryBl = 0;
      /*for printing sums; marks parent hits*/

   signed char tranStr[32]; /*for printing transaction*/
   signed char totalStr[32]; /*for printing total*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun22 Sec02:
   ^   - memory allocation
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(! outFILE)
      outFILE = stdout;

   /*bit inefficent, but am realing on user not having
   `   many accounts
   */
   if(sumBl)
   { /*If: printing a list of sums*/
      hitHeapAryBl =
         calloc(
            acntSTPtr->numParUS,
            sizeof(signed char)
         );
      if(! hitHeapAryBl)
         goto memErr_fun22_sec06;

      parTotalHeapAryF =
         calloc(
            acntSTPtr->numParUS,
            sizeof(float)
         );
      if(! parTotalHeapAryF)
         goto memErr_fun22_sec06;

   } /*If: printing a list of sums*/

   indexSL = acntSTPtr->numParUS;
   indexSL *= acntSTPtr->numChildUS;
   indexSL += acntSTPtr->numChildUS;

   if(sumBl)
   { /*If: printing sums*/
      hitChildHeapAryBl =
         calloc(indexSL, sizeof(signed char));
      if(! hitHeapAryBl)
         goto memErr_fun22_sec06;
   } /*If: printing sums*/

   totalHeapAryF =
      calloc(
         indexSL,
         sizeof(float)
      ); /*maximum memory possible*/

   if(! totalHeapAryF)
      goto memErr_fun22_sec06;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun22 Sec03:
   ^   - get parent/child index and get starting index
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(parStr)
   { /*If: have parend id*/
      parIndexSI =
         find_strAry(
            acntSTPtr->uniqParStr,
            parStr,
            acntSTPtr->numParUS
         ); /*find parent string*/


      if(parIndexSI < 0)
         goto noEntries_fun22_sec06;

      parIndexSI = acntSTPtr->parToNumAryUS[parIndexSI];
   } /*If: have parend id*/


   if(childStr)
   { /*If: have parend id*/
      childIndexSI =
         find_strAry(
            acntSTPtr->uniqChildStr,
            childStr,
            acntSTPtr->numChildUS
         ); /*find parent string*/


      if(childIndexSI < 0)
         goto noEntries_fun22_sec06;

      childIndexSI =
         acntSTPtr->childToNumAryUS[childIndexSI];
   } /*If: have parend id*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun22 Sec04:
   ^   - print entries
   ^   o fun22 sec04 sub01:
   ^     - check if entry has parent and child account
   ^   o fun22 sec04 sub02:
   ^     - check if entry is in date range
   ^   o fun22 sec04 sub03:
   ^     - print entires in range/right account
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   /*****************************************************\
   * Fun22 Sec04 Sub01:
   *   - check if entry has parent and child account
   \*****************************************************/

   /*printing header for user*/
   if(sumBl)
      fprintf(
         (FILE *) outFILE,
         "parent\tchild\tamount%s",
         str_endLine
      );
   else
   { /*Else: print entry header*/
      fprintf(
         (FILE *) outFILE,
         "index\tpar\tchild\tyear\tmonth\tday\tamount"
      );

      fprintf(
         (FILE *) outFILE,
         "\total\treason%s",
         str_endLine
      );
   } /*Else: print entry header*/

   indexSL = 0;

   for(
      ;
      indexSL < acntSTPtr->numEntrySL;
      ++indexSL
   ){ /*Loop: print current entries*/

      if(
            parStr
         &&
                parIndexSI
            !=  (signed int) acntSTPtr->parAryUS[indexSL]
      ) continue; /*if different account*/

      if(
            childStr
         &&
               childIndexSI
            != (signed int) acntSTPtr->childAryUS[indexSL]
      ) continue; /*if different account*/


      /**************************************************\
      * Fun22 Sec04 Sub02:
      *   - check if entry is in date range
      *   o fun22 sec04 sub02 cat01:
      *     - check year ending range (if supplied)
      *   o fun22 sec04 sub02 cat02:
      *     - check month ending range (if supplied)
      *   o fun22 sec04 sub02 cat03:
      *     - check day range (if supplied)
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun22 Sec04 Sub02 Cat01:
      +   - check year ending range (if supplied)
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(
           yearArySS[1]
        && yearArySS[1] < acntSTPtr->yearArySS[indexSL]
      ) continue;

      else if(
         yearArySS[0] > acntSTPtr->yearArySS[indexSL]
      ) continue;

      else if(
         yearArySS[1] > acntSTPtr->yearArySS[indexSL]
      ) ; /*year is before range end, so can skip month
          `   and day checks (is in range)
          */

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun22 Sec04 Sub02 Cat02:
      +   - check month ending range (if supplied)
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
           monthArySC[1]
        && monthArySC[1] < acntSTPtr->monthArySC[indexSL]
      ) continue;

      else if(
         monthArySC[0] > acntSTPtr->monthArySC[indexSL]
      ) continue;

      else if(
         monthArySC[1] > acntSTPtr->monthArySC[indexSL]
      ) ; /*month is within range, no need for day check*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun22 Sec04 Sub02 Cat03:
      +   - check day range (if supplied)
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      else if(
            dayArySC[1]
         && dayArySC[1] < acntSTPtr->dayArySC[indexSL]
      ) continue;

      else if(dayArySC[0] > acntSTPtr->dayArySC[indexSL])
         continue; /*no way to know if more entiries*/

      /***********************************************\
      * Fun22 Sec04 Sub04:
      *   - print entires in range/right account
      \***********************************************/

      if(! parStr)
         tmpParStr =
            get_strAry(
               acntSTPtr->uniqParStr,
               acntSTPtr->numToParAryUS[
                  acntSTPtr->parAryUS[indexSL]
               ]
            );
      else
         tmpParStr = parStr;

      if(! childStr)
         tmpChildStr =
            get_strAry(
               acntSTPtr->uniqChildStr,
               acntSTPtr->numToChildAryUS[
                  acntSTPtr->childAryUS[indexSL]
               ]
            );
      else
         tmpChildStr = childStr;


      if(sumBl)
      { /*If: printing sums (not entries)*/
         slTotal = acntSTPtr->parAryUS[indexSL];
         hitHeapAryBl[slTotal] = 1;
         parTotalHeapAryF[slTotal] +=
            acntSTPtr->tranAryF[indexSL];

         totalHeapAryF[slTotal] +=
            acntSTPtr->tranAryF[indexSL];
      } /*If: printing sums (not entries)*/

      slTotal = acntSTPtr->parAryUS[indexSL];
      slTotal *= acntSTPtr->numChildUS;
      slTotal += acntSTPtr->childAryUS[indexSL];
      totalHeapAryF[slTotal] +=
         acntSTPtr->tranAryF[indexSL];

      if(sumBl)
         hitChildHeapAryBl[slTotal] = 1;
      else
      { /*Else: printing entries*/
         /*floats are not 100% accurate, so need to do
         `   some rounding
         */
         double_numToStr(
            tranStr,
            acntSTPtr->tranAryF[indexSL],
            percisionUC
         ); /*convert transaction to string*/

         double_numToStr(
            totalStr,
            totalHeapAryF[slTotal],
            percisionUC
         ); /*covert total to string*/
         
         fprintf(
            (FILE *) outFILE,
            "%li\t%s\t%s\t%i\t%02i\t%02i\t%s\t%s\t%s%s",
            indexSL,
            tmpParStr,
            tmpChildStr,
            acntSTPtr->yearArySS[indexSL],
            acntSTPtr->monthArySC[indexSL],
            acntSTPtr->dayArySC[indexSL],
            tranStr,
            totalStr,
            acntSTPtr->commentAryST->strAry[indexSL],
            str_endLine
         );
      } /*Else: printing entries*/

      tmpParStr = 0;
      tmpChildStr = 0;
   } /*Loop: print current entries*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun22 Sec05:
   ^   - print sums if requested
   ^   o fun22 sec05 sub01:
   ^     - print child account sums (if sumrequested)
   ^   o fun22 sec05 sub02:
   ^     - print parent account sums (if sum requested)
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   /*****************************************************\
   * Fun22 Sec05 Sub01:
   *   - print child account sums (if sum requested)
   \*****************************************************/

   if(sumBl)
   { /*If: printing a summary*/
      for(
         childIndexSI = 0;
         childIndexSI <
            (signed int) acntSTPtr->numChildUS;
         ++childIndexSI
      ){ /*Loop: print sum for each child account*/

         for(
            parIndexSI = 0;
            parIndexSI <
               (signed long) acntSTPtr->numParUS;
            ++parIndexSI
          ){ /*Loop: though all parent accounts*/

             indexSL = (signed long) parIndexSI;
             indexSL *=
                (signed long) acntSTPtr->numChildUS;
             indexSL += (signed long) childIndexSI;

             if(! hitChildHeapAryBl[indexSL])
                continue; /*account not in hit list*/

             double_numToStr(
                totalStr,
                totalHeapAryF[indexSL],
                percisionUC
             ); /*covert total to string*/

             fprintf(
                (FILE *) outFILE,
                "%s\t%s\t%s%s",
                get_strAry(
                   acntSTPtr->uniqParStr,
                   acntSTPtr->numToParAryUS[parIndexSI]
                ),
                get_strAry(
                   acntSTPtr->uniqChildStr,
                   acntSTPtr->numToChildAryUS[
                      childIndexSI
                   ]
                ),
                totalStr,
                str_endLine
             ); /*print account name and total*/
          }  /*Loop: though all parent accounts*/
      }  /*Loop: print sum for each child account*/

      free(totalHeapAryF);
      totalHeapAryF = 0;
      free(hitChildHeapAryBl);
      hitChildHeapAryBl = 0;

      /**************************************************\
      * Fun22 Sec05 Sub02:
      *   - print parent account sums (if sum requested)
      \**************************************************/

      slTotal = 0;

      for(
         parIndexSI = 0;
         parIndexSI < (signed long) acntSTPtr->numParUS;
         ++parIndexSI
       ){ /*Loop: print sums*/
          if(! hitHeapAryBl[parIndexSI])
             continue; /*parent not in found list*/

          totalF += parTotalHeapAryF[parIndexSI];

          double_numToStr(
             totalStr,
             parTotalHeapAryF[parIndexSI],
             percisionUC
          ); /*covert total to string*/

          fprintf(
             (FILE *) outFILE,
             "%s\tNA\t%s%s",
             get_strAry(
                acntSTPtr->uniqParStr,
                acntSTPtr->numToParAryUS[parIndexSI]
             ),
             totalStr,
             str_endLine
          ); /*print account name and total*/
       }  /*Loop: print sums*/

      free(parTotalHeapAryF);
      parTotalHeapAryF = 0;
      free(hitHeapAryBl);
      hitHeapAryBl = 0;

      double_numToStr(
         totalStr,
         totalF,
         percisionUC
      ); /*covert total to string*/

      fprintf(
         (FILE *) outFILE,
         "total\tNA\t%s%s",
         totalStr,
         str_endLine
      );
   } /*If: printing a summary*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun22 Sec06:
   ^   - clean up and return 
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   errSC = 0;
   goto ret_fun22_sec06;

   memErr_fun22_sec06:;
      errSC = def_memErr_acnt;
      goto ret_fun22_sec06;

   noEntries_fun22_sec06:;
      errSC = def_missingEntry_acnt;
      goto ret_fun22_sec06;

   ret_fun22_sec06:;
      if(totalHeapAryF)
         free(totalHeapAryF);
      totalHeapAryF = 0;

      if(parTotalHeapAryF)
         free(parTotalHeapAryF);
      parTotalHeapAryF = 0;

      if(hitHeapAryBl)
         free(hitHeapAryBl);
      hitHeapAryBl = 0;

      if(hitChildHeapAryBl)
         free(hitChildHeapAryBl);
      hitChildHeapAryBl = 0;

      return errSC;
} /*pEntries_st_acnt*/

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
){
   signed char errSC = 0;
   signed char totalStr[32];  /*total to print*/

   /*for printing*/
   unsigned short parUS = 0;
   unsigned short childUS = 0;

   signed long indexSL = 0;
   signed long slTotal = 0; /*index at in total*/

   float parSumF = 0;       /*for parent account sum*/
   float totalF = 0;        /*total for all accounts*/
   float *totalAryF = 0;
   signed char *pAryBl = 0; /*marks if had sum*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun23 Sec02:
   ^   - memory allocation
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*bit inefficent, but am realing on user not having
   `   many accounts
   */
   slTotal = acntSTPtr->numParUS;
   slTotal *= acntSTPtr->numChildUS;
   slTotal += acntSTPtr->numChildUS;

   totalAryF =
      calloc(
         slTotal,
         sizeof(float)
      ); /*maximum memory possible*/

   if(! totalAryF)
      goto memErr_fun23_sec05;


   pAryBl =
      calloc(
         slTotal,
         sizeof(signed char)
      ); /*maximum memory possible*/

   if(! pAryBl)
      goto memErr_fun23_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun23 Sec03:
   ^   - sum entries
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   for(
      indexSL = 0;
      indexSL < acntSTPtr->numEntrySL;
      ++indexSL
   ) { /*Loop: print current entries*/
      slTotal =
         (signed long) acntSTPtr->parAryUS[indexSL];
      slTotal *= (signed long) acntSTPtr->numChildUS;
      slTotal +=
         (signed long) acntSTPtr->childAryUS[indexSL];
      totalAryF[slTotal] += acntSTPtr->tranAryF[indexSL];

      pAryBl[slTotal] = 1;
   } /*Loop: print current entries*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun23 Sec04:
   ^   - print sums
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   fprintf(
      (FILE *) outFILE,
      "parent\tchild\ttotal%s",
      str_endLine
   ); /*print header*/


   /*I prefere child accounts first because parent
   `   accounts are physical accounts (were money is at),
   `   whereas child accounts are funds for a goal.
   `   So, a child account can span multiple parents,
   `   but a parent can not span multiple child
   */
   for(
      childUS = 0;
      childUS < acntSTPtr->numChildUS;
      ++childUS
   ){ /*Loop: check child sums*/

      for(
         parUS = 0;
         parUS < acntSTPtr->numParUS;
         ++parUS
      ){ /*Loop: print account sums*/

         slTotal = (signed long) parUS;
         slTotal *= (signed long) acntSTPtr->numChildUS;
         slTotal += (signed long) childUS;

         if(! pAryBl[slTotal])
            continue; /*account does not exist*/
         
         double_numToStr(
            totalStr,
            totalAryF[slTotal],
            percisionUC
         ); /*covert total to string*/

         fprintf(
            (FILE *) outFILE,
            "%s\t%s\t%s%s",
            get_strAry(
               acntSTPtr->uniqParStr,
               acntSTPtr->numToParAryUS[parUS]
            ),
            get_strAry(
               acntSTPtr->uniqChildStr,
               acntSTPtr->numToChildAryUS[childUS]
            ),
            totalStr,
            str_endLine
         ); /*print account name and total*/

      }  /*Loop: print account sums*/

   }  /*Loop: check child sums*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun23 Sec05:
   ^   - print parent totals
   ^   o fun23 sec05 sub01:
   ^     - setup for parent total printing + print header
   ^   o fun23 sec05 sub02:
   ^     - print parent totals (loop)
   ^   o fun23 sec05 sub03:
   ^     - print final parent total (not printed in loop)
   ^   o fun23 sec05 sub04:
   ^     - print total for all parent accounts
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   /*****************************************************\
   * Fun23 Sec05 Sub01:
   *   - setup for parent total printing + print header
   \*****************************************************/

   slTotal = (signed long) acntSTPtr->numParUS;
   slTotal *= (signed long) acntSTPtr->numChildUS;
   slTotal += (signed long) acntSTPtr->numChildUS;

   indexSL = 0;
   parSumF = 0;
   parUS = 0;
   childUS = 0;
   errSC = 0; /*re-using to mark if have total*/

   /*****************************************************\
   * Fun23 Sec05 Sub02:
   *   - print parent totals (loop)
   \*****************************************************/

   while(indexSL < slTotal)
   { /*Loop: print totals for each parent account*/

      if(childUS >= acntSTPtr->numChildUS)
      { /*If: end of parent account*/
         if(errSC)
         { /*If: have total to print*/
            double_numToStr(
               totalStr,
               parSumF,
               percisionUC
            ); /*covert total to string*/

            fprintf(
               (FILE *) outFILE,
               "%s\tNA\t%s%s",
               get_strAry(
                  acntSTPtr->uniqParStr,
                  acntSTPtr->numToParAryUS[parUS]
               ),
               totalStr,
               str_endLine
            ); /*print account name and total*/
         } /*If: have total to print*/

         ++parUS;
         parSumF = 0;
         errSC = 0;
         childUS = 0;
      } /*If: end of parent account*/

      if(! pAryBl[indexSL])
         goto nextAcnt_fun23_sec05_sub02;
         /*this account does not exist*/

      parSumF += totalAryF[indexSL];
      totalF += totalAryF[indexSL];
      errSC = 1;

      nextAcnt_fun23_sec05_sub02:;
         ++indexSL;
         ++childUS;
   } /*Loop: print totals for each parent account*/

   /*****************************************************\
   * Fun23 Sec05 Sub03:
   *   - print final parent total (not printed in loop)
   \*****************************************************/

   if(errSC)
   { /*If: have final, unprinted total*/
      double_numToStr(
         totalStr,
         parSumF,
         percisionUC
      ); /*covert total to string*/

      fprintf(
         (FILE *) outFILE,
         "%s\tNA\t%s%s",
         get_strAry(
            acntSTPtr->uniqParStr,
            acntSTPtr->numToParAryUS[parUS]
         ),
         totalStr,
         str_endLine
      ); /*print account name and total*/
   } /*If: have final, unprinted total*/

   /*****************************************************\
   * Fun23 Sec05 Sub04:
   *   - print total for all parent accounts
   \*****************************************************/

   double_numToStr(
      totalStr,
      totalF,
      percisionUC
   ); /*covert total to string*/

   fprintf(
      (FILE *) outFILE,
      "total\tNA\t%s%s",
      totalStr,
      str_endLine
   ); /*print account name and total*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun23 Sec05:
   ^   - clean up and return 
   \>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

   errSC = 0;
   goto ret_fun23_sec05;

   memErr_fun23_sec05:;
      errSC = def_memErr_acnt;
      goto ret_fun23_sec05;

   ret_fun23_sec05:;
      if(totalAryF)
         free(totalAryF);
      totalAryF = 0;

      if(pAryBl)
         free(pAryBl);
      pAryBl = 0;

      return errSC;
} /*pSum_st_acnt*/

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
){
   signed char amountStr[32];
   signed char totalStr[32];

   signed long slIndex = 0;
   unsigned short childIndexUS = 0;
   unsigned short parIndexUS = 0;

   signed long slTotal = 0; /*index of current total*/
   float *totalAryF = 0;
   signed long sizeTotalSL = 0;


   sizeTotalSL = acntSTPtr->numParUS;
   sizeTotalSL *= acntSTPtr->numChildUS;
   sizeTotalSL += acntSTPtr->numChildUS;

   /*allocate total sum memory*/
   totalAryF =
      calloc(
         sizeTotalSL,
         sizeof(float)
      ); /*maximum memory possible*/

   if(! totalAryF)
      return def_memErr_acnt;


   /*print header*/
   fprintf(
      (FILE *) outFILE,
      "parent\taccount\tyear\tmonth\tday\tamount\ttotal"
   );
   fprintf(
      (FILE *) outFILE,
      "\treason%s",
      str_endLine
   );


   /*print account entries to files*/
   while(slIndex < acntSTPtr->numEntrySL)
   { /*Loop: print entries*/
      parIndexUS = acntSTPtr->parAryUS[slIndex];
      parIndexUS = acntSTPtr->numToParAryUS[parIndexUS];

      childIndexUS = acntSTPtr->childAryUS[slIndex];
      childIndexUS =
         acntSTPtr->numToChildAryUS[childIndexUS];

      slTotal = parIndexUS;
      slTotal *= acntSTPtr->numChildUS;
      slTotal += childIndexUS;


      totalAryF[slTotal] += acntSTPtr->tranAryF[slIndex];

      double_numToStr(
         amountStr,
         acntSTPtr->tranAryF[slIndex],
         percisionUC
      );

      double_numToStr(
         totalStr,
         totalAryF[slTotal],
         percisionUC
      );
         

      fprintf(
         (FILE *) outFILE,
         "%s\t%s\t%i\t%02i\t%02i\t%s\t%s\t%s%s",
         get_strAry(
            acntSTPtr->uniqParStr,
            parIndexUS
         ),
         get_strAry(
            acntSTPtr->uniqChildStr,
            childIndexUS
         ),
         acntSTPtr->yearArySS[slIndex],
         acntSTPtr->monthArySC[slIndex],
         acntSTPtr->dayArySC[slIndex],
         amountStr,
         totalStr,
         acntSTPtr->commentAryST->strAry[slIndex],
         str_endLine
      ); /*print the entry*/

      ++slIndex;
   } /*Loop: print entries*/


   if(totalAryF)
      free(totalAryF);
   totalAryF = 0;
   return 0;
} /*write_st_acnt*/

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
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun25 TOC:
   '   - divide money into accounts by divy file
   '   o fun25 sec01:
   '     - variable declarations
   '   o fun25 sec02:
   '     - get date and memory allcoation
   '   o fun25 sec05:
   '     - clean up and return
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun25 Sec01:
   ^   - variable declarations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   signed long lineSL = 0;
   signed long slIndex = 0;

   float percTranF = 0;  /*percent to transfer*/
   float minTranF = 0;   /*minimum amount to transfer*/
   float maxTranF = 0;   /*maximum amount to transfer*/

   signed char totalStr[32];
   signed char tranStr[32];

   float *swapFPtr = 0;
   float *totalHeapAryF = 0;
   float *transferHeapAryF = 0;
   float *ghostHeapAryF = 0;   /*ghost; gets change*/
   float extraF = amountF; /*extra after divy*/
   signed long sizeTotalSL = 0; /*number accounts*/
   signed long numParSL = 0;
   float tmpF = 0;

   signed char parStr[def_lenStr_strAry];
   signed long parSL = 0;
   signed char *pParStr = 0;

   signed char childStr[def_lenStr_strAry];
   signed long childSL = 0;
   signed char *pChildStr = 0;

   signed long lenSL = 0;

   /*for getting date; everything is on stack*/
   time_t rawTime;
   struct tm *timeSTPtr;
   signed short yearSS = 0;
   signed char monthSC = 0;
   signed char daySC = 0;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun25 Sec02:
   ^   - get date and memory allcoation
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   time(&rawTime);
   timeSTPtr = localtime(&rawTime);
   yearSS = (signed short) timeSTPtr->tm_year + 1900;
   monthSC = (signed char) timeSTPtr->tm_mon + 1;
   daySC = (signed char) timeSTPtr->tm_mday;


   numParSL = (signed long) acntSTPtr->numParUS;

   if(outFILE)
   { /*If: doing pretend print*/
      sizeTotalSL = acntSTPtr->numParUS;
      sizeTotalSL *= acntSTPtr->numChildUS;
      sizeTotalSL += acntSTPtr->numChildUS;
   } /*If: doing pretend print*/

   else
      sizeTotalSL = (signed long) acntSTPtr->numParUS;


   transferHeapAryF =
      calloc(
         acntSTPtr->numParUS,
         sizeof(float)
      ); /*actual amount to transfer*/
   if(! transferHeapAryF)
      goto memErr_fun25_sec05;


   /*allocate total sum memory*/
   totalHeapAryF =
      calloc(
         sizeTotalSL,
         sizeof(float)
      ); /*total for account*/
   if(! totalHeapAryF)
      goto memErr_fun25_sec05;


   ghostHeapAryF =
      calloc(
         acntSTPtr->numParUS,
         sizeof(float)
      ); /*gets rounded off cents*/
   if(! ghostHeapAryF)
      goto memErr_fun25_sec05;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun25 Sec03:
   ^   - get divy totals
   ^   o fun25 sec03 sub01:
   ^     - get past header + start loop
   ^   o fun25 sec03 sub02:
   ^     - get parent account id
   ^   o fun25 sec03 sub03:
   ^     - get child account id
   ^   o fun25 sec03 sub04:
   ^     - get divy percentage (if one)
   ^   o fun25 sec03 sub05:
   ^     - get minimum transaction number
   ^   o fun25 sec03 sub06:
   ^     - get maximum transaction number
   ^   o fun25 sec03 sub07:
   ^     - add entry (non-mock)/print (mock) if new entry
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun25 Sec03 Sub01:
   *   - get past header + start loop
   \*****************************************************/

   /*get past header*/
   slIndex = endLine_ulCp(divyStr);

   if(divyStr[slIndex] == '\0')
      goto noDivy_fun25_sec05;

   /*get off line endings*/
   if(divyStr[slIndex] == '\r')
     ++slIndex;
   if(divyStr[slIndex] == '\n')
     ++slIndex;
   if(divyStr[slIndex] == '\r')
     ++slIndex;

   if(divyStr[slIndex] == '\0')
      goto noDivy_fun25_sec05;

   ++lineSL;

   if(! amountF)
      goto ranOut_fun25_sec03_sub07_cat06;
      /*no funds to divide*/

   while(divyStr[slIndex] != '\0')
   { /*Loop: get divy values*/

      percTranF = 0;
      minTranF = 0;
      maxTranF = 0;

      ++lineSL;

      /**************************************************\
      * Fun25 Sec03 Sub02:
      *   - get parent account id
      \**************************************************/

      lenSL = 0;

      while(divyStr[slIndex] > 32)
      { /*Loop: copy parent id*/
         if(lenSL >= def_lenStr_strAry)
            goto entryErr_fun25_sec05;

         parStr[lenSL++] = divyStr[slIndex++];
      } /*Loop: copy parent id*/

      parStr[lenSL] = '\0';

      parSL =
         find_strAry(
            acntSTPtr->uniqParStr,
            parStr,
            acntSTPtr->numParUS
         );


      while(
            divyStr[slIndex] == '\t'
         || divyStr[slIndex] == ' '
      ) ++slIndex;


      if(divyStr[slIndex] < 32)
         goto entryErr_fun25_sec05;

      /**************************************************\
      * Fun25 Sec03 Sub03:
      *   - get child account id
      \**************************************************/

      lenSL = 0;

      while(divyStr[slIndex] > 32)
      { /*Loop: copy parent id*/
         if(lenSL >= def_lenStr_strAry)
            goto entryErr_fun25_sec05;

         childStr[lenSL++] = divyStr[slIndex++];
      } /*Loop: copy parent id*/

      childStr[lenSL] = '\0';

      childSL =
         find_strAry(
            acntSTPtr->uniqChildStr,
            childStr,
            acntSTPtr->numChildUS
         );


      while(
            divyStr[slIndex] == '\t'
         || divyStr[slIndex] == ' '
      ) ++slIndex;


      if(divyStr[slIndex] < 32)
         goto entryErr_fun25_sec05;

      /**************************************************\
      * Fun25 Sec03 Sub04:
      *   - get divy percentage (if one)
      \**************************************************/

      if(
           ( (divyStr[slIndex] & ~32) == 'N' )
        && ( (divyStr[slIndex + 1] & ~32) == 'A' )
      ){ /*If: no percentage*/
         while(divyStr[slIndex] > 32)
            ++slIndex;
         goto skipDivyCol_fun25_sec03_sub04;
      } /*If: no percentage*/


      slIndex +=
         strToF_base10str(
            &divyStr[slIndex],
            &percTranF
         );
 
      if(divyStr[slIndex] > 32)
         goto entryErr_fun25_sec05;


      skipDivyCol_fun25_sec03_sub04:;
         while(
               divyStr[slIndex] == '\t'
            || divyStr[slIndex] == ' '
         ) ++slIndex;


        if(divyStr[slIndex] < 32)
           goto addEntry_fun25_sec03_sub07;
           /*end of divy entry*/

      /**************************************************\
      * Fun25 Sec03 Sub05:
      *   - get minimum transaction number
      \**************************************************/

      if(
           ( (divyStr[slIndex] & ~32) == 'N' )
        && ( (divyStr[slIndex + 1] & ~32) == 'A' )
      ){ /*If: no minimum value*/
         while(divyStr[slIndex] > 32)
            ++slIndex;
         goto skipMinCol_fun25_sub05;
      }  /*If: no minimum value*/

      slIndex +=
         strToF_base10str(
            &divyStr[slIndex],
            &minTranF
         );
 
      if(divyStr[slIndex] > 32)
         goto entryErr_fun25_sec05;


      skipMinCol_fun25_sub05:;
         while(
               divyStr[slIndex] == '\t'
            || divyStr[slIndex] == ' '
         ) ++slIndex;


        if(divyStr[slIndex] < 32)
           goto addEntry_fun25_sec03_sub07;
           /*end of divy entry*/

      /**************************************************\
      * Fun25 Sec03 Sub06:
      *   - get maximum transaction number
      \**************************************************/

      if(
           ( (divyStr[slIndex] & ~32) == 'N' )
        && ( (divyStr[slIndex + 1] & ~32) == 'A' )
      ) goto addEntry_fun25_sec03_sub07;
        /*no other columns to grab*/


      slIndex +=
         strToF_base10str(
            &divyStr[slIndex],
            &maxTranF
         );

      /*end of divy entry*/

      /**************************************************\
      * Fun25 Sec03 Sub07:
      *   - add entry (non-mock)/print (mock) if new entry
      *   o fun25 sec03 sub07 cat01:
      *     - get total for transaction
      *   o fun25 sec03 sub07 cat02:
      *     - deal with adding a new parent account
      '   o fun25 sec03 sub07 cat03:
      '     - reall; add transfer to array
      *   o fun25 sec03 sub07 cat04:
      *     - mock transfer; get total and transfer
      *   o fun25 sec03 sub07 cat05:
      *     - mock; new par/child (print transfer)
      *   o fun25 sec03 sub07 cat06:
      *     - check if still have divy amount remaining
      \**************************************************/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun25 Sec03 Sub07 Cat01:
      +   - get total for transaction
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      addEntry_fun25_sec03_sub07:;

         slIndex += endLine_ulCp(&divyStr[slIndex]);

         /*get off line endings*/
         if(divyStr[slIndex] == '\r')
           ++slIndex;
         if(divyStr[slIndex] == '\n')
           ++slIndex;
         if(divyStr[slIndex] == '\r')
           ++slIndex;

         /*find percentage to transfer*/
         percTranF *= amountF;

         /*check if transaction is in bounds*/
         if(percTranF < minTranF)
            percTranF = minTranF;

         else if(! maxTranF)
            ; /*no maximum*/

         else if(percTranF > maxTranF)
            percTranF = maxTranF;

         extraF -= percTranF;

         if(extraF < 0)
            percTranF += extraF;
            /*account for over divy*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun25 Sec03 Sub07 Cat02:
         +   - deaL with adding a new parent account
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         if(parSL < 0)
         { /*If: have parent account*/

            if(outFILE)
               goto newAcnt_fun25_sec03_sub07_cat05;
               /*mock transfer*/

            else
            { /*Else If: reall divy for new parent*/
               ++numParSL;

               if(numParSL >= sizeTotalSL)
               { /*If: need more memory*/
                  swapFPtr =
                     realloc(
                        totalHeapAryF,
                        (numParSL + 32) * sizeof(float)
                     );
                  if(! swapFPtr)
                     goto memErr_fun25_sec05;
                  totalHeapAryF = swapFPtr;

                  swapFPtr =
                     realloc(
                        transferHeapAryF,
                        (numParSL + 32) * sizeof(float)
                     );
                  if(! swapFPtr)
                     goto memErr_fun25_sec05;
                  transferHeapAryF = swapFPtr;

                  swapFPtr =
                     realloc(
                        ghostHeapAryF,
                        (numParSL + 32) * sizeof(float)
                     );
                  if(! swapFPtr)
                     goto memErr_fun25_sec05;
                  ghostHeapAryF = swapFPtr;

                  sizeTotalSL = numParSL + 32;

                  for(
                     lenSL = numParSL - 1;
                     lenSL < sizeTotalSL;
                     ++lenSL
                  ){ /*Loop: intialize new values*/
                     transferHeapAryF[lenSL] = 0;
                     totalHeapAryF[lenSL] = 0;
                     ghostHeapAryF[lenSL] = 0;
                  }  /*Loop: intialize new values*/
               } /*If: need more memory*/
            } /*Else If: reall divy for new parent*/
         } /*If: new parent account*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun25 Sec03 Sub07 Cat03:
         +   - reall; add transfer to array
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         if(! outFILE)
         { /*If: actual divy*/
             if(ghostBl)
             { /*If: rounding (ghost account)*/
                tmpF = percTranF;
                percTranF =  ((int) percTranF);
                tmpF -= percTranF;
             } /*If: rounding (ghost account)*/

             if(
                addEntry_st_acnt(
                    acntSTPtr,
                    parStr,
                    childStr,
                    yearSS,
                    monthSC,
                    daySC,
                    percTranF,
                    commentStr,
                    ghostBl,
                    percisionUC,
                    0 /*read transaction*/
                )
             ) goto memErr_fun25_sec05;

             parSL =
                find_strAry(
                   acntSTPtr->uniqParStr,
                   parStr,
                   acntSTPtr->numParUS
                );
             parSL = acntSTPtr->parToNumAryUS[parSL];

             if(ghostBl && tmpF != 0)
                ghostHeapAryF[parSL] += tmpF;

             transferHeapAryF[parSL] += percTranF;
             transferHeapAryF[parSL] += tmpF;
         } /*If: actual divy*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun25 Sec03 Sub07 Cat04:
         +   - mock transfer; get total and transfer
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         else if(childSL >= 0)
         { /*Else If: mock divy with existing account*/
            parSL =
               find_strAry(
                  acntSTPtr->uniqParStr,
                  parStr,
                  acntSTPtr->numParUS
               );
            parSL = acntSTPtr->parToNumAryUS[parSL];
            childSL = acntSTPtr->childToNumAryUS[childSL];

            if(ghostBl)
            { /*If: rounding (ghost account)*/
               ghostHeapAryF[parSL] += percTranF;
               ghostHeapAryF[parSL] -= ((int) percTranF);
            } /*If: rounding (ghost account)*/

            transferHeapAryF[parSL] += percTranF;

            parSL *= acntSTPtr->numChildUS;
            parSL += childSL;

            totalHeapAryF[parSL] += percTranF;
         } /*Else If: mock divy with existing account*/

         /*++++++++++++++++++++++++++++++++++++++++++++++\
         + Fun25 Sec03 Sub07 Cat05:
         +   - mock; new par/child (print transfer)
         \++++++++++++++++++++++++++++++++++++++++++++++*/

         else
         { /*Else: mock print with new child account*/
            newAcnt_fun25_sec03_sub07_cat05:;

            double_numToStr(
               totalStr,
               percTranF,
               percisionUC
            );

            if(ghostBl)
               percTranF = (int) percTranF;

            double_numToStr(
               tranStr,
               percTranF,
               percisionUC
            );

            fprintf(
               (FILE *) outFILE,
               "Pretend Total: %s %s has %s%s",
               parStr,
               childStr,
               totalStr,
               str_endLine
            ); /*account total*/

            fprintf(
               (FILE *) outFILE,
               "Pretend Transfer: %s to %s%s",
               parStr,
               totalStr,
               str_endLine
            ); /*transfer amount*/
         } /*Else: mock print with new child account*/

       /*++++++++++++++++++++++++++++++++++++++++++++++++\
       + Fun25 Sec03 Sub07 Cat06:
       +   - check if still have divy amount remaining
       \++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(extraF <= 0)
      { /*If: ran out of funds*/

         ranOut_fun25_sec03_sub07_cat06:;

         while(divyStr[slIndex] < 33)
         { /*Loop: get past white space*/

            if(divyStr[slIndex] == '\0')
               break;
            ++slIndex;

         } /*Loop: get past white space*/

         lenSL = slIndex;

         lenSL = endLine_ulCp(&divyStr[slIndex]);
         parSL = (signed long) divyStr[slIndex];

         if(divyStr[slIndex] != '\0')
         { /*If: not end of divy file*/

            fprintf(
               stderr,
               "ran out of funds on divy file line %li%s",
               lineSL,
               str_endLine
            );

            fprintf(
               stderr,
               "  %s%s",
               &divyStr[slIndex],
               str_endLine
            ); /*line ran out on*/
         } /*If: not end of divy file*/

         break; /*nothing else to divy*/
      } /*If: ran out of funds*/
   } /*Loop: get divy values*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun25 Sec04:
   ^   - print final tallies (reall and mock)
   ^   o fun25 sec04 sub01:
   ^     - get totals for all parent accounts
   ^   o fun25 sec04 sub02:
   ^     - reall print transfer amounts + add ghost
   ^   o fun25 sec04 sub03:
   ^     - mock; print totals
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   /*****************************************************\
   * Fun25 Sec04 Sub01:
   *   - reall print transfer amounts + add ghost entries
   *   o fun25 sec04 sub01 cat01:
   *     - print reall totals and tansfers
   *   o fun25 sec04 sub01 cat02:
   *     - reall; print untansfered funds
   \*****************************************************/

   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun25 Sec04 Sub01 Cat01:
   +   - print reall totals and tansfers
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   if(! outFILE)
   { /*If: doing reall divy*/
      for(
         slIndex = 0;
         slIndex < acntSTPtr->numEntrySL;
         ++slIndex
      ){ /*Loop: get totals*/
         totalHeapAryF[acntSTPtr->parAryUS[slIndex]] +=
            acntSTPtr->tranAryF[slIndex];
      }  /*Loop: get totals*/


      for(
         slIndex = 0;
         slIndex < numParSL;
         ++slIndex
      ){ /*Loop: print accoutns + add ghost accounts*/

         pParStr =
            get_strAry(
               acntSTPtr->uniqParStr,
               acntSTPtr->numToParAryUS[slIndex]
            );

         totalHeapAryF[slIndex] += ghostHeapAryF[slIndex];

         if(transferHeapAryF[slIndex])
         { /*If: transfer funds*/
            double_numToStr(
               tranStr,
               transferHeapAryF[slIndex],
               percisionUC
            );

            double_numToStr(
               totalStr,
               totalHeapAryF[slIndex],
               percisionUC
            );

            fprintf(
               stdout,
               "Transfer: %s to %s for a total of %s%s",
               tranStr,
               pParStr,
               totalStr,
               str_endLine
            );
         } /*If: transfer funds*/

         if(ghostHeapAryF[slIndex] == 0)
            continue;

         if(
            addEntry_st_acnt(
                acntSTPtr,
                pParStr,
                (signed char *) "ghost",
                yearSS,
                monthSC,
                daySC,
                ghostHeapAryF[slIndex],
                commentStr,
                0,
                percisionUC,
                0 /*read transaction*/
            )
         ) goto memErr_fun25_sec05;
           /*print ghost entries*/

      }  /*Loop: print accoutns + add ghost accounts*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun25 Sec04 Sub01 Cat02:
      +   - reall; print untansfered funds
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(extraF > 0)
         fprintf(
            stdout,
            "%0.2f untransfered%s",
            extraF,
            str_endLine
         );
   } /*If: doing reall divy*/

   /*****************************************************\
   * Fun25 Sec04 Sub02:
   *   - mock; print totals
   *   o fun25 sec04 sub02 cat01:
   *     - mock; print new account totals
   *   o fun25 sec04 sub02 cat02:
   *     - mock; print transer amounts
   *   o fun25 sec04 sub02 cat03:
   *     - mock; print untransfered funds
   \*****************************************************/
   
   /*++++++++++++++++++++++++++++++++++++++++++++++++++++\
   + Fun25 Sec04 Sub02 Cat01:
   +   - mock; print new account totals
   \++++++++++++++++++++++++++++++++++++++++++++++++++++*/

   else
   { /*Else: printing out mock transfer*/

      for(
         slIndex = 0;
         slIndex < acntSTPtr->numEntrySL;
         ++slIndex
      ){ /*Loop: get totals*/
         parSL = acntSTPtr->parAryUS[slIndex];
         parSL *= acntSTPtr->numChildUS;
         parSL += acntSTPtr->childAryUS[slIndex];

         if(totalHeapAryF[parSL] != 0)
            totalHeapAryF[parSL] +=
               acntSTPtr->tranAryF[slIndex];
      }  /*Loop: get totals*/

      slIndex = 0; 

      for(
         parSL = 0;
         parSL < (signed long) acntSTPtr->numParUS;
         ++parSL
      ){ /*Loop: print parent account totals*/

         pParStr =
            get_strAry(
               acntSTPtr->uniqParStr,
               acntSTPtr->numToParAryUS[parSL]
            );

         for(
            childSL = 0;
            childSL < (signed long) acntSTPtr->numChildUS;
            ++childSL
         ){ /*Loop: print out account totals*/
            if(totalHeapAryF[slIndex] == 0)
            { /*If: account not affected by divy*/
               ++slIndex;
               continue;
            } /*If: account not affected by divy*/

            pChildStr =
               get_strAry(
                  acntSTPtr->uniqChildStr,
                  acntSTPtr->numToChildAryUS[childSL]
               );

            double_numToStr(
               totalStr,
               totalHeapAryF[slIndex],
               percisionUC
            );

            fprintf(
               (FILE *) outFILE,
               "Pretend: %s %s account will have %s%s",
               pParStr,
               pChildStr,
               totalStr,
               str_endLine
            ); 

            ++slIndex;
         }  /*Loop: print out account totals*/
      }  /*Loop: print parent account totals*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun25 Sec04 Sub02 Cat02:
      +   - mock; print transer amounts
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      for(
         parSL = 0;
         parSL < acntSTPtr->numParUS;
         ++parSL
      ){ /*Loop: print transer amounts*/

         if(transferHeapAryF[parSL])
         { /*If: transfer funds*/
            pParStr =
               get_strAry(
                  acntSTPtr->uniqParStr,
                  acntSTPtr->numToParAryUS[parSL]
               );

            double_numToStr(
               tranStr,
               transferHeapAryF[parSL],
               percisionUC
            );

            fprintf(
               stdout,
               "Pretend Transfer: %s to %s%s",
               tranStr,
               pParStr,
               str_endLine
            );
         } /*If: transfer funds*/
      }  /*Loop: print transfer amounts*/

      /*+++++++++++++++++++++++++++++++++++++++++++++++++\
      + Fun25 Sec04 Sub02 Cat03:
      +   - mock; print untransfered funds
      \+++++++++++++++++++++++++++++++++++++++++++++++++*/

      if(extraF > 0)
         fprintf(
            (FILE *) outFILE,
            "%0.2f untransfered%s",
            extraF,
            str_endLine
         );
   } /*Else: printing out mock transfer*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Fun25 Sec05:
   ^   - clean up and return
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   lineSL = 0;
   goto ret_fun25_sec05;

   memErr_fun25_sec05:;
     lineSL = def_memErr_acnt;
     goto ret_fun25_sec05;

   noDivy_fun25_sec05:;
     lineSL = def_missingEntry_acnt;
     goto ret_fun25_sec05;

   entryErr_fun25_sec05:;
     goto ret_fun25_sec05;

   ret_fun25_sec05:;
      if(totalHeapAryF)
         free(totalHeapAryF);
      totalHeapAryF = 0;

      if(transferHeapAryF)
         free(transferHeapAryF);
      transferHeapAryF = 0;

      if(ghostHeapAryF)
         free(ghostHeapAryF);
      ghostHeapAryF = 0;

      return lineSL;
} /*divy_st_acnt*/

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
