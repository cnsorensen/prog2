#include <iostream>
#include "quadtree.h"
using namespace std;

quadtree::quadtree()
{
    root = nullptr;
}

quadtree::~quadtree()
{ }
void quadtree::insert( int start, int size, unsigned char *image, int tolerance, node *&Tree )
{
  int i = start;
  //start max and min values to the first value checked in quad for comparisons
  int j, max = image[i] - '0', min = image [i] - '0';

  if ( Tree == nullptr )
  {
      //establish the current node
      Tree = new node;
      //set it's children to null
      Tree->child[0] = nullptr;
      Tree->child[1] = nullptr;
      Tree->child[2] = nullptr;
      Tree->child[3] = nullptr;
    }
    
  //traverses width of a quadrant in a 2-D fashion
  //quits when it hits start position of quadrant below  
  while( i < start + 2 * size * size )
  {
    for( j = 0; j < size; j++ )
    {
    if ( j == 0 )
      //typecast char to int and compare to max value in quad
      if ( (image[i] - '0') > max )
        max = (image[i]-'0');
      //typecast char to int and compare to min value in quad
      if ( (image[i]-'0') < min )
        min = (image[i]-'0');        
      if( j % size == 0 )
      {
        //set j back to 0 to (box width - 1)
        j = 0;
        //drop position down 1 row (2-d view)
        i+= size;
      }
    }
  }
  //store average value for later
  Tree->avg = (max + min)/2;
  
  //difference greater than +/- tolerance value ( 2*tolerance )
  if ( ( max - min ) > 2*tolerance )
  {
    //not a leaf node, can use flag later to draw white lines
    Tree->leaf = false;
    
    //Top left starting coordinate with appropriate size
    insert ( start, size/2, image, tolerance, Tree->child[0] );
     
    //Top right starting coordinate with appropriate size
    insert ( (start + size/2), size/2, image, tolerance, Tree->child[1] );
    
    //Bottom left starting coordinate with size
    insert ( (start + (size * size/2) ), size/2, image, tolerance, Tree->child[2] );
    
    //Bottom right starting coordinate with size
    insert (( start + (size * size/2) + size/2), size/2, image, tolerance, Tree->child[3] );
  }
  
  //else tolerance value was good for this quad and it's a leaf node
  else
    Tree->leaf = true;
}


void quadtree::fillArr( unsigned char *compressed, node *&Tree, int index, int size )
{
    int i, j; 
    if ( Tree == nullptr )
        return;
      
    //if the node isn't a leaf, recurse down for values
    if ( Tree-> leaf == false )
    {
        //fill compressed image array for Q1 of current section
        fillArr( compressed, Tree->child[0], index, size/2 );
        
        //fill compressed image array for Q2 of current section
        fillArr( compressed, Tree->child[1], (index + size/2), size/2 );
        
        //fill compressed image array for Q3 of current section
        fillArr( compressed, Tree->child[2], (index + ( size * size/2)), size/2 );
        
        //fill compressed image array for Q4 of current section
        fillArr( compressed, Tree->child[3], (index + ( size * size/2) + size/2), size/2 );
    }
   
   else if ( Tree->leaf == true )
   {
     i = index;
   //Same algorithm from insert funciton
   //traverses width of a quadrant in a 2-D fashion
   //quits when it hits start position of quadrant below
      while( i < index + 2 * size * size )
      {
        for( j = 0; j < size; j++ )
        {
            compressed[i] = Tree->avg;        
            if( j % size == 0 )
            {
               //resets j count from 0 to (box width - 1)
               j = 0;
               //drops position down 1 row ( 2-D view)
               i+= size;
            }
         }
    }
  }
}

