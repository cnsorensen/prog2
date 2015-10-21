#include <iostream>
#include "quadtree.h"
using namespace std;

quadtree::quadtree()
{
    root = nullptr;
}

quadtree::~quadtree()
{ }
void quadtree::insert( int start, int size, unsigned char *image, int tolerance, node *&Tree, int fullsize )
{
  int i = start, j;
  //start max and min values to the first value checked in quad for comparisons
  int max = int(image[i]), min = int(image [i]);

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
  //quits when it runs into start position of quadrant below  
 while( i < start + (size*(fullsize )))
  {
    for( j = 0; j < size; j++ )
    {
          //typecast char to int and compare to max value in quad
          if ( int(image[i]) > max )
            max = int(image[i]);
          //typecast char to int and compare to min value in quad
          if ( int(image[i]) < min )
            min = int(image[i]);        
            i++;
    }
    i+= fullsize - size;
  }
  //store average value for later
  Tree->avg = (max + min)/2;
  
  //difference greater than +/- tolerance value ( 2*tolerance )
  if ( ( max - min ) > 2*tolerance )
  {
    //not a leaf node, can use flag later to draw white lines
    Tree->leaf = false;
    
    //Top left starting coordinate with appropriate size
    insert ( start, size/2, image, tolerance, Tree->child[0], fullsize );
     
    //Top right starting coordinate with appropriate size
    insert ( (start + size/2), size/2, image, tolerance, Tree->child[1], fullsize );
    
    //Bottom left starting coordinate with size
    insert ( (start + (fullsize * size/2) ), size/2, image, tolerance, Tree->child[2], fullsize );
    
    //Bottom right starting coordinate with size
    insert (( start + (fullsize * size/2) + size/2), size/2, image, tolerance, Tree->child[3], fullsize );
  }
  
  //else tolerance value was good for this quad and it's a leaf node
  else
    Tree->leaf = true;
}


void quadtree::fillArr( unsigned char *compressed, node *&Tree, int index, int size, int fullsize )
{
    int i, j; 
    if ( Tree == nullptr )
        return;
      
    //if the node isn't a leaf, recurse down for values
    if ( Tree-> leaf == false )
    {
        //fill compressed image array for Q1 of current section
        fillArr( compressed, Tree->child[0], index, size/2, fullsize );
        
        //fill compressed image array for Q2 of current section
        fillArr( compressed, Tree->child[1], (index + size/2), size/2, fullsize );
        
        //fill compressed image array for Q3 of current section
        fillArr( compressed, Tree->child[2], (index + ( fullsize * size/2)), size/2, fullsize );
        
        //fill compressed image array for Q4 of current section
        fillArr( compressed, Tree->child[3], ( index + (fullsize * size/2) + size/2), size/2, fullsize );
    }
   
   else if ( Tree->leaf == true )
   {
     i = index;
   //Same algorithm from insert funciton
   //traverses width of a quadrant in a 2-D fashion
   //quits when it hits start position of quadrant below
      while( i < index + (size * (fullsize ) ) )
      {
        for( j = 0; j < size; j++ )
        {
            //set array values for rows within current box
            compressed[i] = Tree->avg;        
               i++;
         }
        //incriment down a full row and back width of current box
        i+= fullsize - size;
    }
  }
}

