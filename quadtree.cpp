#include <iostream>
#include "quadtree.h"
using namespace std;

void quadtree::insert( int start, int size, char *image, int tolerance )
{
  int i = start;
  int j, max = min = image [i];
  while( i < start + 2 * size * size )
  {
    for( j = 0; j < size; j++ )
    {
      if ( int (image [i]) > max )
        max = int (image [i]);
      if ( int (image [i]) < min )
        min = int (image [i]);
      if ( ( max - min ) > tolerance )
        //leaf = false, break and split into quads
        
      if( j % size == 0 )
      {
        j = 0;
        i+= size;
        cout << endl;
      }
    }

  }
}

/*int main( int argc, char** argv )
{
  int n = 20;
  int a[n*n];

  for( int i = 0; i < n*n; i++ )
  {
    a[i] = i;
  }

  foo( a[0], n/2 );

  return 0;
}*/
