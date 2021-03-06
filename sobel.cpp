// sobel.cpp : sobel edge detection
// Tested Environment: Darwin OSX X86_64 with g++ 4.2.1
//
// NAME:	  Shulang Lei
// SID:		  200253624
// DATE:	  Nov 4th, 2013

#include <cstring>
#include <cstdarg>
#include <cstdlib>
#include <math.h>
#include <stdio.h>

/* image */

typedef unsigned char uchar_t;

#define HEIGHT  420
#define WIDTH   560	

uchar_t	inputimage[HEIGHT][WIDTH];
uchar_t outputimage[HEIGHT][WIDTH];
uchar_t outputimage1[HEIGHT][WIDTH];
uchar_t outputimage2[HEIGHT][WIDTH];
uchar_t outputimage3[HEIGHT][WIDTH];

void
ProcessImage();

void
ReadImage(char*, uchar_t[HEIGHT][WIDTH]);

void
WriteImage(char*, uchar_t[HEIGHT][WIDTH]);

/* given pixel postion x,y, a 3x3 filter and an image, returns its new gray level  */
uchar_t
apply3x3(int x, int y, float[3][3], uchar_t[HEIGHT][WIDTH]);

/* error */
void
error(char*, ...);

void
debug(char*, ...);

/* main function */

int main(int argc, char *argv[])
{
	/* initialization */
	memset( inputimage, 0, HEIGHT*WIDTH*sizeof(uchar_t) );
	memset( outputimage, 0, HEIGHT*WIDTH*sizeof(uchar_t) );

        /* command line arguments */
	if(argc != 2) {
		error((char *)"USAGE: %s <image.raw>\n",  argv[0]);
	}


	/* read image */
	ReadImage( argv[1], inputimage );

	/* process image */
	ProcessImage();

	/* write image */
	WriteImage((char*)"hsobel_out.raw", outputimage );
	WriteImage((char*)"vsobel_out.raw", outputimage1 );
	WriteImage((char*)"gradient_out.raw", outputimage2 );
	WriteImage((char*)"edge_map_out.raw", outputimage3 );
		
	return 0;
}


/* image */

void
ProcessImage()
{
  //sobel y operator
  float h_sobel_filter[3][3] = 
  {  
    {-1,-2,-1},
    {0,0,0},
    {1,2,1}
  };

  //sobel x operator
  float v_sobel_filter[3][3] = 
  {  
    {-1,0,1},
    {-2,0,2},
    {-1,0,1}
  };


  //apply sobel y
  for(int i=0; i < HEIGHT; i++)
    for(int j=0; j < WIDTH; j++)
      outputimage[i][j] = apply3x3(j,i, h_sobel_filter, inputimage);

  //apply sobel x
  for(int i=0; i < HEIGHT; i++)
    for(int j=0; j < WIDTH; j++)
      outputimage1[i][j] = apply3x3(j,i, v_sobel_filter, inputimage);

  //get gradient 
  for(int i=0; i < HEIGHT; i++)
    for(int j=0; j < WIDTH; j++)
      outputimage2[i][j] = sqrt(pow(outputimage[i][j], 2) + pow(outputimage1[i][j], 2));

  //apply threshold
  for(int i=0; i < HEIGHT; i++)
    for(int j=0; j < WIDTH; j++) {
      if (outputimage2[i][j] > 128) 
        outputimage3[i][j] = 255;
      else
        outputimage3[i][j] = 0;
    }
}

void
ReadImage(char *pszPath, uchar_t image[HEIGHT][WIDTH])
{

	FILE *pRAW;

	if( ( pRAW = fopen( pszPath, "rb" ) ) == NULL )
		error((char *)"ERROR: Could not open image \"%s\".\n", pszPath);

	if( fread( image, sizeof(uchar_t), HEIGHT*WIDTH, pRAW ) <
		( HEIGHT*WIDTH ) )
	{
		error((char *)"ERROR: Could not read image \"%s\".\n", pszPath);
	}

	fclose(pRAW);
}

void
WriteImage(char *pszPath, uchar_t image[HEIGHT][WIDTH])
{

	FILE *pRAW;

	if( ( pRAW = fopen( pszPath, "wb" ) ) == NULL )
		error((char*)"ERROR: Could not create image \"%s\".\n", pszPath);

	if( fwrite( image, sizeof(uchar_t), HEIGHT*WIDTH, pRAW) <
		( HEIGHT*WIDTH) )
	{
		error((char*)"ERROR: Could not write image \"%s\".\n", pszPath);
	}

	fclose(pRAW);
}

/* error */
void 
error(char *psz, ...)
{
	va_list ap;
	va_start( ap, psz );
	vfprintf( stderr, psz, ap );
	va_end(ap);
	exit(-1);
}

/* given pixel postion x,y, a 3x3 filter and an image, returns its new gray level  */
uchar_t
apply3x3(int x, int y, float filter[3][3], uchar_t img[HEIGHT][WIDTH])
{
  //apply filters
  float sum = 0;
  if (y-1>=0 && x-1>=0)
    sum += img[y-1][x-1]*filter[0][0];
  if (y-1>=0)
    sum += img[y-1][x]*filter[0][1];
  if (x+1<WIDTH)
    sum += img[y-1][x+1]*filter[0][2];
  if (x-1>=0)
    sum += img[y][x-1]*filter[1][0];
  sum += img[y][x]*filter[1][1];
  if (x+1<WIDTH)
    sum += img[y][x+1]*filter[1][2];
  if (x-1>=0 && y+1<HEIGHT)
    sum += img[y+1][x-1]*filter[2][0];
  if (y+1<HEIGHT)
    sum += img[y+1][x]*filter[2][1];
  if (y+1<HEIGHT && x+1<WIDTH)
    sum += img[y+1][x+1]*filter[2][2];

  //clamp result
  if (sum > 255) /* clamp down */
    return 255;
  if (sum < 0) /* clamp up */
    return 0;
  return sum; 
}

/*debug*/
void debug(char *psz, ...)
{
	va_list ap;

	va_start( ap, psz );
	vfprintf( stdout, psz, ap );
	va_end(ap);
}
