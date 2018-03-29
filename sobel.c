#include <stdio.h>                          /* Sobel.c */
#include <math.h>

int pic[256][256];
int outpicx[256][256];
int outpicy[256][256];
int maskx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
int masky[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
double ival[256][256],ival2[256][256],ival3[256][256],maxival,maxival2,maxival3;

main(argc,argv)
int argc;
char **argv;
{
    int i,j,p,q,mr,sum1,sum2;
    double threshold;
    FILE *fo1, *fo2, *fo3, *fp1, *fopen();
    char *foobar;
    int cols = 256, rows = 256;
    int threshold1 = 40, threshold2 = 100;

    // input file
    argc--; argv++;
    foobar = *argv;
    fp1=fopen(foobar,"rb");

    // magnitude output file
  	argc--; argv++;
  	foobar = *argv;
  	fo1=fopen(foobar,"wb");

    // low threshold output file
  	argc--; argv++;
  	foobar = *argv;
  	fo2=fopen(foobar,"wb");

    // high threshold output file
  	argc--; argv++;
  	foobar = *argv;
  	fo3=fopen(foobar,"wb");

    // formatting the output files for pgm
  	fprintf(fo1,"P5\n");
    fprintf(fo1,"%d %d\n", rows, cols);
    fprintf(fo1,"255\n");
    fprintf(fo2,"P5\n");
    fprintf(fo2,"%d %d\n", rows, cols);
    fprintf(fo2,"255\n");
    fprintf(fo3,"P5\n");
    fprintf(fo3,"%d %d\n", rows, cols);
    fprintf(fo3,"255\n");

    // scanning the input picture into an array pixel by pixel
    for (i=0;i<256;i++)
    {
    	for (j=0;j<256;j++)
        {
	        pic[i][j]  =  getc (fp1);
	        pic[i][j]  &= 0377;
        }
    }

    // convolution
    mr = 1;
    for (i=mr;i<256-mr;i++)
    {
    	for (j=mr;j<256-mr;j++)
      	{
	        sum1 = 0;
	        sum2 = 0;
	        for (p=-mr;p<=mr;p++)
	        {
	            for (q=-mr;q<=mr;q++)
	            {
	               sum1 += pic[i+p][j+q] * maskx[p+mr][q+mr];
	               sum2 += pic[i+p][j+q] * masky[p+mr][q+mr];
	            }
	        }
	        outpicx[i][j] = sum1;
	        outpicy[i][j] = sum2;
      	}
    }

    // root of squares to find the magnitude
    maxival = 0;
    for (i=mr;i<256-mr;i++)
    {
    	for (j=mr;j<256-mr;j++)
        {
         	ival[i][j]=sqrt((double)((outpicx[i][j]*outpicx[i][j]) + (outpicy[i][j]*outpicy[i][j])));
         	if (ival[i][j] > maxival)
            	maxival = ival[i][j];
        }
    }

    // print the output to the new output files from the magnitude array
    // depending on what threshold
    for (i=0;i<256;i++)
    {
    	for (j=0;j<256;j++)
        {
            ival[i][j] = (ival[i][j] / maxival) * 255;
         	fprintf(fo1,"%c",(char)((int)(ival[i][j])));
         	fprintf(fo2,"%c",(char)((int)((ival[i][j] > threshold1) ? 255 : 0)));
         	fprintf(fo3,"%c",(char)((int)((ival[i][j] > threshold2) ? 255 : 0)));
        }
    }

}
