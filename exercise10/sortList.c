// A (partial) implementation of selection sort,
// using lots of pointer arithmetic.

#include <stdio.h>

// Swap the values pointed to by a and b.
void swap( int *a, int *b )
{
  // Implement this funciton, easy
  int temp = *a;
  *a = *b;
  *b = temp;
}

// Return a pointer to the element of the given array with the smallest value.
int *findSmallest( int *list, int len )
{
  int *smallest = list;
  for ( int i = 1; i < len; i++ ) {
     
    // Fill in the body of this loop, without
    // declaring any more variables.  Just use
    // the smallest pointer to keep up with the
    // smallest value you've found so far.
    if(*(list + i) < *smallest) {
      smallest = list + i;
    } 
  }
  return smallest;
}

// Print the contents of the given list.
void printList( int *list, int len )
{
  while ( len ) {
    // Fill in the body of this loop, without
    // declaring any more variables.  Modify the parameters as you
    // iterate, to move through the list and exit the loop when you've
    // printed everything.
    printf("%d ", *list);
    list++;
    len--;
  }

  printf( "\n" );
}

int main()
{
  // A list of random-ish values.
  int list[] = { 808, 250, 74, 659, 931, 273, 545, 879, 924, 710, 441, 166 };
  int len = sizeof( list ) / sizeof( list[ 0 ] );

  // For each index, find the smallest item from the remaining
  // (unsorted) portion of the list.
  for ( int i = 0; i < len; i++ ) {

    // Here's a clever trick.  Call findSmallest() with a pointer to
    // the start of the unsorted part of list.  It's just getting an offset
    // into our list array, but it can use as if it was a whole (shorter) array.
    int *p = findSmallest( list + i, len - i );

    // Swap the smallest item into the first position in the unsorted part of the
    // list.
    swap( list + i, p );

    // Now, the sorted part of the list is a little bit longer and the
    // unsorted part is a little shorter.
  }

  printList( list, len );
}
