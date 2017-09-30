Nathaniel Bates
4/15/2017
COP 3402

HW 4: Compiler With Procedure and Else Implemented

In order to run the program on Eustis, just compile the compiler.c,
LexicalAnalyzer.c, and P-Machine.c files together as so:

gcc compiler.c LexicalAnalyzer.c P-Machine.c

There is a header file included with those 3 files which is included in
the zip file submission.

After compilation, to run it simply enter the following as 2 separate strings:

./a.out <filename>.txt -l-a-v

If there are no flags, then the program will just run and if there is
an error, it will print it out.  Also, if there is no expected output
(print instruction to screen) or expected input (scanf for user input),
and there are no errors, it will just show:

"No errors, program syntactically correct.".

/*Very Important*/
The program is expecting a single string for the flags.  So spaces in
between the flags will result in not all of them being read.
-l-v will work, but -l -v will only display the l portion.

Also, if the program has a read statement and/or a write statement in it
and you are printing out the contents of the virtual machine, it will
print the stack up until the read/write statement. It will then execute
the statement, then continue printing the stack.  After it is finished,
it will also print each of the registers used and values when the SIO3
instruction (print to screen) executes, basically consolidates the write
statements so you can see them all together at the end.

We used the inputProgram.txt file in order to show the errors we are checking for.
The valid_wirth.txt program is included as we recievied an email regarding submitting
that as a test case.  We included the NestedProcedures.txt program from the pdf, as these
were the 3 files we primarily used to test the compiler.

Included in the zip files is as follows:
ReadMe.txt
compiler.c
LexicalAnalyzer.c
P-Machine.c
header.h
errors.pdf
UserManual.pdf
inputProgram.txt
outputProgram(n > 10).txt
outputProgram(n < 10).txt
NestedProcedures.txt
NestedProcedures_output.txt
valid_wirth.txt (Program from page 311 in book)
valid_wirth_output.txt (Program from page 311 in book)
