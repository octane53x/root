CRAB LANGUAGE


OBJECTS:

OBJ Primitives: (Handled by compiler)

Type: A member of Thing
Char (8 bit): Memory space, used by Bool, Str
Block (64 bit): Memory space, used by Ref, Array

OBJ Library: (Compiled into primitives)

Thing: Base class for all objects, inherited automatically
Ref: Block with link to existing object, created with ampersand (Obj&)
Array<T>: Subsequent objects, created with square brackets (Obj[])
Bool: Char with value restriction to 0 or 1
Num: Fraction using Array of Block that replaces both integer and floating point
Str: Array of Char
Queue<T>: Iterable linked list
Map<T, U>: Ordered tree with log(N) access
Umap<T, U>: Hash table with constant time access


FUNCTIONS:

pass(): Do nothing
exit(): Terminate program
print(Str msg): Output msg to console
err(Str msg): Print stack trace and msg, then exit
assert(Bool condition, Str msg): Check condition, if false throw err


FUNCTIONAL DIFFERENCES FROM C++:

- NO POINTERS: Use Ref. User not allowed to manipulate memory addresses
- NO CASTING: Assignment operator must be inherent or implemented
    Downcasting handled in engine by referencing the type
- block keyword: Cannot override this function


NOTES:

- Variable and function names starting with triple underscore are reserved
    by system calls
