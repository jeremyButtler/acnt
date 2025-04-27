# Use:

Somewhat simple command line program to manage a bank
  account using a tsv file. Input can be from the
  program call or by a prompt interaction (use `-prompt`).

Target audience is me, but this might also apply to ordenary
  command line users (is there such a thing?) who just
  want to keep track of their bank accounts.

- What my target audience means:
  - I am not trying to manage multiple currencies, so
    acnt does handle different currencies
  - no double accounting (again simpiler)
  - rounding is only done to the nearest 100th of a dollar
    (a penny)
  - accounts are manged in a tsv file that can be
    manipulated with standard unix tools (sed, awk, and
    grep)
  - no GUI (to much work to build)

Why does it exist, because I am odd and wanted my own
  system. Excel/libreoffice spreadsheets are to clunky
  and ledger looked to complicated and I I did not like
  ledgers file system. Also, I want something that does
  modify my accounts for me (makes my life easier).

# License:

Dual licenses under Unlicense and MIT. pick the one that
  works for you.

# Install:

## Unix (Linux/Mac/BSD)

```
cd ~/Downloads;
git clone https://github.com/jeremybuttler/acnt;
cd acnt;

if [ ! -d "/usr/local/bin" ];
then
   sudo mkdir -p "/usr/local/bin";
fi

make -f mkfile.unix;
sudo make -f mkfile.unix install;
```

## Windows:

Warning: acnt is a command line tool, so it can only work
  in the windows terminal.

Need to test on windows, should work with the windows, but
  as not been tested yet. You will need the C++ build
  tools from visual studio.

Clone repository, unzip, and Open developler terminal.
  Make sure downloaded repo is saved as acnt in Downloads.

```
cd %HOMEPATH%\Downloads\acnt
nmake /F mkfile.win
```

Then copy acnt.exe to a convient place.

# Usage:

## Before running acnt

Before we use acnt, we should first go over how the acnt
  system works. The accounts are kept in a tsv file, which
  can use both tabs and spaces (your preference). However,
  just to warn you acnt prints tabs only.

This account tsv file (say account.tsv), has eight
  columns.

1. parent: the actual (physical) account the funds are in
   - ex: savings, checking, and CD's
2. child: how account within the parent account, this is
   you specific fund/goal/task
   - ex: rent, bills, groceries, savings, slush, ect...
3. the year of the transaction (earliest is 1900)
4. the month of the transaction (1-12)
5. the day of the transaction (1-31)
6. the amount of the transaction
   - positive (ex `100`) means funds were added
   - negative (ex `-100`) means funds were removed
7. the total amount in the account
   - acnt actually ignores this, so use `NA` if unsure,
   - acnt always re-finds this value, so do not worry
     about it
8. a comment about the transaction
   - actn only keeps track of this for printing, so it is
     for your personal use only
   - it is very poor form to leave blank values in a
     spread sheet (tsv is a simple spread sheet), so use
     `NA` if you do not want a comment

You will need to start acnt with an account file. I have
  provided a blank file called `account.tsv`. In it are
  the header and two transactions of zero dollars and
  zero cents. You can replace the header with want you
  want, but beaware that acnt will always write this
  header and no other when it saves or prints a file.

Just be warned, acnt sorts accounts by the order they
  first apear, not alphabetical. Also, after a couple
  rounds they will be sorted by date.

## Beware of ghost:

Before you run acnt you should be first warned of the
  ghost account. Unlike ghosts or or unicorns it is very
  reall.

So, what is the ghost account? it is a rounding system
  behind the scenes.

The reason ghost exist's is because I am not a fan of
  keeping track of cents. So, what I ended up often doing
  is rounding negative transactions (purchases) up, and
  postive transactions down. Over time this would
  accumlate into a ghost fund. Acnt follows this logic,
  and so, rounds any cents off of your transaction and
  adds it to a ghost account (see it is reall). So, do not
  be supprised to see this weird account named `ghost`
  popup all of a sudden.

You can disable the ghost account using `-no-ghost`. You
  can disable the ghost account setting during compile
  time by changing `def_ghost_defsAcnt 1` in defsAcnt.h
  to `def_ghost_defsAcnt 0` (then re-install). You can
  always renable a one time ghost account use at runtime
  using `-ghost`.

## Quick startup:

To get the help message for acnt, use `acnt -h`. The help
  message is rather long, but really centers around two
  sections. The first is file IO (input and output files).
  The second is a list of commands you can call from the
  command line. The final list of inputs is the input list
  for each command. The only required input for acnt is an
  account file to work on.

To get started, use the `account.tsv` file. It is just a
  blank file with two transactions of 0. You can manually
  add or modify these or use account to add new
  transactions.

To start an interactive sesssion
  do `acnt -file account.tsv -prompt`. You can then use
  `h` to get a list of commands you can use.

- To add a 100 dollar transaction to savings
  do `acnt -file account.tsv -add -amount 100 -par savings -child savings`
- To print a summary of all accounts
  do `acnt -file account.tsv`
- To print a list all transactions in your savings account
  do `acnt -file account.tsv -print -par savings`
- To print all saving funds accross all parent accounts
  do `acnt -file account.tsv -print -child savings`
- To print a summary of all saving funds accross all
  parent accounts
  do `acnt -file account.tsv -print hsum -child savings`
  - to get a non-separated tsv file use `-print sum`
    instead of `-print hsum`
- To acutally save an account you will need to
  do `acnt -file account.tsv -add -amount 100 -par savings -child savings -out account.tsv`
  - notice, we are saving to the same account we are
    opening. If you use a differnt file, then you will
    have a new file.
  - always have an older account file just in case you
    wipe our your new file

**WARNING**:
  never do `acnt -file account.tsv > account.tsv`, which
  will first blank the input/output account.tsv file
  before account can open it. Instead
  do `acnt -file account.tsv -out account.tsv`.

If you want to check the result of an trasanction before
  making it, you can use the `-pretend` option (does not
  apply to `-prompt`), which will print out a fake
  transaction. The saved file will be the original input
  file. For
  example `acnt -file account.tsv -add -amount 100 -par savings -child savings -out accunt.tsv -pretend`
  would print a rough change to stdout (terminal) and then
  do nothing else.
  
## An additional trick:

I do not like dividing up pay checks, and so, acnt has an
 auto divy system. You will need to provided a divy file
 `-divy-file divy.tsv` and the `-divy` flag. The
  `example-divy.tsv` is an example of a divy file. The
  divy file is a tsv file that has six columns.

1. parent: the parent account to add the sums into
2. child: the child account to add the sums into
3. percent: the percentage of the divy this account gets
   - use `NA` for no percent (min column is used instead)
4. min: the minimum amount (hard value, not percent) this
   account gets
   - use `NA` for no minimum
5. max: the maximum amount (hard value, not percent) this
   account gets
   - use `NA` for no maximum
6. comment: personal comment for you (this is ignored)

If you run out of money, acnt will complete the
  transaction, but also warn you that it could not
  complete the divy. In this case the first accounts
  listed will get the funds. So, make sure the important
  accounts come first in the divy file.

You can do a divy for a 100 dollars
  with `acnt -file account.tsv -divy-file divy.tsv -divy -amount 100`.

## Acnt Command list:

To work from the command line you will use one of six
  commands.

1. -prompt: start an interactive session
   - type `h` and enter for help message
   - type `q` to quite
   - type `add` to add an entry
   - type `divy` to divides funds by a divy file
   - type `del` to delete an entry
   - type `sum` to print a summary
   - type `search` to print entries from a search
   - type `save` to save your modified account file
     - c89 does not support file detection or completion,
       so you need to manually provide the file name
2. -sum: list total in each account in `-file account.tsv`
   - default option if no other command is used
   - save to file with `-p-out file.tsv`
3. -print: print entries in `-file account.tsv` that fit
   the input search range
   - save to file with `-p-out file.tsv`
   - inputing no additional parameters will print all
     entries, so you should provide at least one of:
     - -par: parent account to search for
     - -child: child account to search for
     - -start-year: first year in range to search for
     - -end-year: last year in range to search for
     - -start-month: first month in range to search for
     - -end-month: last month in range to search for
     - -start-day: first day in range to search for
     - -end-day: last day in range to search for
4. -add: add a new entry to `-file account.tsv` file
   - can not be used with -divy
   - Required options:
     - -amount: amount to enter
     - -par: parent account to add to
     - -child: child account to add to
   - Optional options:
     - -year: year of transcaction (current year)
     - -month: month of transcaction (current month)
     - -day: day of transcaction (current day)
     - -note: comment about entry
     - -no-ghost: do not round amount and add rounded off
       sum to ghost account (`-ghost` enables)
5. -divy: divy `-amount` into accounts
   using `-divy-file divy.tsv`
   - can not be used with -add
   - Required:
     - -divy-file: divy file to divide ammount with
     - -amount: amount to divy up
   - Optional:
     - -year: year of transcaction (current year)
     - -month: month of divy (current month)
     - -day: day of divy (current day)
     - -note: comment to add to all entries
     - -no-ghost: do not round amount and add rounded off
       sum to ghost account (`-ghost` enables)
6. -del: remove entry by index from `-file account.tsv`
   - beware, the index can be different then the row
     number in `-file account.tsv`, use -print to get the
     actual index's
7. -out: file to save `-file account.tsv` entries to
   - default is stdout (terminal)

## The acnt order:

Acnt applies commands in a certain order. The first
  command run is always delete, then add or divy (can
  not run both), then print, then sum, and if not
  using `-prompt` the write command. The very last command
  to fire is the `-prompt` command.

# Updates:

- 2025-04-26:
  - added `-print hsum` to print divider between parent
    child account sums, account totals, and total sums
    when printing
- 2025-04-25:
  - fixed bug in `acnt file account.tsv -print sum` would
    wrongly sum sub-accounts
- 2025-04-06:
  - add in sum command for print `-print sum`
  - changed `-sum` to print all entries as tsv
    - no longer break between child account totals and
      parent account totals
    - parent account totals sum marked with `NA`
   - added in all accounts total for `-sum` and the
     `-print-sum` commands
- 2025-04-04:
  - acnt now reconginizes, and ignores the index column at
    start made from `acnt -file account.tsv -print`
- 2025-04-03:
  - fixed minor error message issue when forgot parent or
    child account with add
  - addin index header for `-print`
  - removed `-amount value` flag adn changed `-add`
    and `-divy` to take in the transaction amount, so
    you can use both add and divy in same run
