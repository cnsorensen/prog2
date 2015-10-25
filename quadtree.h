#include <iostream>

using namespace std;
#ifndef _Quadtree_H_
#define _Quadtree_H_

typedef unsigned char byte;

struct node
{   //data
    int avg;
    bool leaf;
    node *child[4];
};

class quadtree
{
    public:
//functions
    quadtree( );
    //insert to tree from 1-D char *image array 
    void insert( int start, int size, byte *image, int tolerance, node *&Tree, int fullsize, byte *quads );
    //fill compressed image array with simplified values for output
    void fillArr( byte *compressed, node *&Tree, int index, int size, int fullsize, int &leaves, int &nodes );
    ~quadtree();
    
    //compiler was giving errors with root as private data
    //I made root public and errors went away
    node *root;
};

#endif
