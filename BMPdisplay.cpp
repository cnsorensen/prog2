/*
	***** BMPdisplay.cpp*****

Read in and display a BMP image file using OpenGL graphics, in 24-bit color and 8-bit monochrome.

Author: John M. Weiss, Ph.D.
Class: CSC300 Data Structures, Fall 2015 (sample code for PA#2)

To compile:	g++ BMPdisplay.cpp BMPload.cpp -lglut -lGLU -lGL

Modifications:
*/

// include files
#include <iostream>
using namespace std;
#include "quadtree.h"
// the GLUT header automatically includes gl.h and glu.h
#include <GL/glut.h>

// symbolic constants
const int EscapeKey = 27;
const int Space = 32;
const int Q = 113;
const int Plus = 43;
const int Minus = 45;

// globals to track values needed
int tolerance = 0;
int totalNodes = 0;
int totalLeaves = 0;

// typedefs
typedef unsigned char byte;

// global vars
int ScreenWidth  = 1024;
int ScreenHeight =  768;

//global tree instance
quadtree tree;

int nrows, ncols;   // image dimensions
bool lines = false; // track display for compressed image or quad lines image


byte* BMPimage;		    // array of RGB pixel values (range 0 to 255)
byte* image;            // array of bytes to store monochrome image (for quadtree encoding)
byte* compressed;       // array of bytes holding colored compressed image values
byte* quads;            // array of bytes holding colored image and lines drawn over the top

// OpenGL callback function prototypes
void display( void );
void reshape( int w, int h );
void keyboard( unsigned char key, int x, int y );

// other function prototypes
void initOpenGL( const char *filename, int nrows, int ncols );
bool LoadBmpFile( const char* filename, int &nrows, int &ncols, byte* &image );
void displayColor( int x, int y, int w, int h, byte *image );
void displayMonochrome( int x, int y, int w, int h, byte *compressed );

/******************************************************************************/

// main() function
int main( int argc, char *argv[] )
{
    if ( argc < 2 )
    {
        cerr << "Usage: BMPdisplay image.bmp\n";
        return -1;
    }

    // read image file
    if ( !LoadBmpFile( argv[1], nrows, ncols, BMPimage ) )
    {
        cerr << "Error: unable to load " << argv[1] << endl;
        return -1;
    }
    cerr << "reading " << argv[1] << ": " << nrows << " x " << ncols << endl;
  
    //values for outputting compression stats
    int L2;
    int Nsquared = nrows * ncols;
  
    // convert 24-bit color BMP image to 8-bit monochrome image
    image = new byte [nrows * ncols ];
    byte* BMPptr = BMPimage, *imageptr = image;
    for ( int row = 0; row < nrows; row++ )
        for ( int col = 0; col < ncols; col++ )
        {
            *imageptr++ = 0.30 * BMPptr[0] + 0.59 * BMPptr[1] + 0.11 * BMPptr[2] + 0.5;
            BMPptr += 3;
        }

    // initialize quads array to same values of image array
    quads = new byte [nrows * ncols];
    for ( int i= 0; i < nrows*ncols; i++)
        quads[i] = image[i];
        
        
    //instantiate a tree and begin insertion based on tolerance value and image array
    tolerance = atoi(argv[2]);

    tree.insert( 0, nrows, image, tolerance, tree.root, nrows, quads );

    //make and fill compressed array from average values of image array stored in tree
    compressed = new byte [nrows * ncols];

    //fill the compressed array with average tree values and count node types
    tree.fillArr( compressed, tree.root, 0, nrows, nrows, totalLeaves, totalNodes );
    L2 = 2 * totalLeaves;
    
    //Output compression values based off of original user input
    cout << "Original image size : " << Nsquared << endl
         << "Leaves in compression tree for original values : " << totalLeaves << endl 
         << "Compression factor from original values : " << 100 * L2 / Nsquared << '%' 
         << " of the original image."<< endl;
    
    // perform various OpenGL initializations
    glutInit( &argc, argv );
    initOpenGL( argv[1], nrows, ncols );

    // go into OpenGL/GLUT main loop, never to return
    glutMainLoop();
    
    //free up memory before final program quit
    delete [] image;
    delete [] compressed;
    delete [] quads;

    tree.~quadtree( );

    return 0;
}

/******************************************************************************/

// various commands to initialize OpenGL and GLUT
void initOpenGL( const char *filename, int nrows, int ncols )
{
    glutInitDisplayMode( GLUT_RGBA | GLUT_SINGLE );	        // 32-bit graphics and single buffering

    ScreenWidth = 2 * ncols;
    ScreenHeight = nrows;
    glutInitWindowSize( ScreenWidth, ScreenHeight );   // initial window size
    glutInitWindowPosition( 100, 50 );			            // initial window  position
    glutCreateWindow( filename );			                // window title
    
    glClearColor( 0.0, 0.0, 0.0, 0.0 );			            // use black for glClear command
    
    // callback routines
    glutDisplayFunc( display );				// how to redisplay window
    glutReshapeFunc( reshape );				// how to resize window
    glutKeyboardFunc( keyboard );			// how to handle key presses
}

/******************************************************************************/
/*                          OpenGL callback functions                         */
/******************************************************************************/

// callback function that tells OpenGL how to redraw window
void display( void )
{
    // clear the display
    glClear( GL_COLOR_BUFFER_BIT );

    //if spacebar pressed an even number of times
    if ( !lines )
    {   
        // display image in color and monochrome
        displayColor( 0, 0, ncols, nrows, image );
        displayMonochrome( ncols, 0, ncols, nrows, compressed );
    }
    //if spacebar pressed an odd number of times
    if ( lines )
    {
        // display image in color and where tree quads are in monochrome
        displayColor( 0, 0, ncols, nrows, image );
        displayMonochrome( ncols, 0, ncols, nrows, quads );    
    } 
    
    // flush graphical output
    glFlush();
}

/******************************************************************************/

// callback function that tells OpenGL how to resize window
// note that this is called when the window is first created
void reshape( int w, int h )
{
    // store new window dimensions globally
    ScreenWidth = w;
    ScreenHeight = h;

    // how to project 3-D scene onto 2-D
    glMatrixMode( GL_PROJECTION );  // use an orthographic projection
    glLoadIdentity();				// initialize transformation matrix
    gluOrtho2D( 0.0, w, 0.0, h );
    glViewport( 0, 0, w, h );		// adjust viewport to new window
}

/******************************************************************************/

// callback function that tells OpenGL how to handle keystrokes
void keyboard( unsigned char key, int x, int y )
{   
    switch ( key )
    {

            // Escape quits program
        case EscapeKey:
            exit( 0 );
            break;
            // 'q' key quits program
        case Q:
            exit( 0 );
            break;
            // space bar displays or removes quad lines
        case Space:
            lines = !lines;
            break;
            // '+' key increments tolerance value and reforms compressed and quad images
        case Plus:
            tolerance += 1;
            for (int i = 0; i < nrows*ncols; i++)
                quads [i] = image[i];
            tree.insert( 0, nrows, image, tolerance, tree.root, nrows, quads );
            tree.fillArr( compressed, tree.root, 0, nrows, nrows, totalLeaves, totalNodes );
            break;
            // '-' key decrements tolerance value and reforms compressed and quad images
        case Minus:
            if ( tolerance > 0 )
                tolerance -= 1;
            for ( int i = 0; i < nrows*ncols; i++)
                quads[i] = image[i];
            tree.insert( 0, nrows, image, tolerance, tree.root, nrows, quads );
            tree.fillArr( compressed, tree.root, 0, nrows, nrows, totalLeaves, totalNodes );
            break;
            
        default:    //any other key press does nothing
            break;
    }
    glutPostRedisplay();
}

/******************************************************************************/

// display 24-bit color image
void displayColor( int x, int y, int w, int h, byte* image )
{
    glRasterPos2i( x, y );
    glDrawPixels( w, h, GL_RGB, GL_UNSIGNED_BYTE, BMPimage );
}

// display 8-bit monochrome image
void displayMonochrome( int x, int y, int w, int h, byte* image )
{
    glRasterPos2i( x, y );
    glDrawPixels( w, h, GL_LUMINANCE, GL_UNSIGNED_BYTE, image );
}

/*****************************************************************************/
