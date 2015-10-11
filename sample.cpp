#include <iostream>

using namespace std;

void foo( int start, int size )
{
  int i = start;
  int j;
  while( i < start + 2 * size * size )
  {
    for( j = 0; j < size; j++ )
    {
      cout << i << " ";
      i++;
    }
    if( j % size == 0 )
    {
      j = 0;
      i+= size;
      cout << endl;
    }
  }
}

int main( int argc, char** argv )
{
  int n = 20;
  int a[n*n];

  for( int i = 0; i < n*n; i++ )
  {
    a[i] = i;
  }

  foo( a[0], n/2 );

  return 0;
}


