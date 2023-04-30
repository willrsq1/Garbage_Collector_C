# Garbage_Collector_C
Garbage Collector for mallocs in language C.

Only 4 functions - 1 file - are needed !

![feazfzaefa](https://user-images.githubusercontent.com/117649637/235355641-1a6637c0-6249-4ac7-bba9-a06798afadeb.jpg)

Secures ALL Mallocs.
Initializes ALL Malloced bits to '\0'.
Stores everything that has been Malloced in a chained list.
Upon calling, frees everything that has been previously Malloced.

A main is available to try it (and show how to use it).

I ran the tests with Valgrind.

If you wish to free something before using the function, simply set the freed point to NULL to avoid crashes.

Enjoy !
