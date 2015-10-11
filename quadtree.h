#include <iostream>
using namespace std;


typedef unsigned char byte;
class quadtree
{
    public:
//functions
    insert ( int start, int size, byte *image, int tolerance );
    /*store average color value:
        if (int (max - min) > tolerance )
            bool variable = false  and split into quads
        else
            bool variable = true   and this is a leaf*/
    
    
    //moveToArray (recursive )
        
    

    private:
    struct node
    {
        //data
        /*average color value*/
        bool leaf = true;
        node *child1, *child2, *child3, *child4;
    };
    
    node *root = nullptr;
};
