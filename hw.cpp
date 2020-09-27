#include <iostream>
#include <fstream>
#include <cstdio>
#include <cassert>
using namespace std;

// We'll store image info as globals; not great programming practice
// but ok for this short program.
int size;
bool **image;

void renderPixel(int x, int y, int radius)
{
    // added parameter radius to distinguish between R= 150 or 100
    if ( radius == 150 ){
        assert(x >= 0 && y >= 0 && x <= size && y <= size);

        image[x][y] = 1;
        image[y][x] = 1;
        image[y][size - x] = 1; // creating the second half of the circle based on requirements 
        image[x][size - y] = 1;

    }
    if ( radius == 100 ) {
        assert(x >= 0 && y >= 0 && x <= size && y <= size);
        image[x][y]= 1;
        image[y][x] = 1;
        image[size - y][x] = 1;    // creating the second half of the circle based on requirements 
        image[size - x][y] = 1;

    }
    // TODO: light up the pixel's symmetric counterpart
}

//Heavily used the reading material
//Used the MidpointCircle() formula provided in the reading material and tweaked minor things
//To make it work with what we needed
void rasterizeArc(int radius)
{
    int x = 150;    // changing the origin 
    int y = radius+x;
    int d = 1 - radius;
    int deltaE= 3;
    int deltaSE= -2 *radius+ 5;
    renderPixel (x,y,radius);   // added radius to renderpixel so it knows which circle its drawing so what if statement to execute
    while (y > x) {
        if (d < 0) {
            d += deltaE;
            deltaE += 2;
            deltaSE += 2;
        } else {

            d += deltaSE;
            deltaE += 2;
            deltaSE += 4;
            y--;
        }
        x++;

        renderPixel (x,y,radius);
    }

    // TODO: rasterize the arc using renderPixel to light up pixels
}

// You shouldn't need to change anything below this point.
//You did have to add another rasterizeArc() function call
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " circlesize\n";
        return 0;
    }

#ifdef _WIN32
    sscanf_s(argv[1], "%d", &size);
#else
    sscanf(argv[1], "%d", &size);
#endif
    if (size <= 0)
    {
        cout << "Image must be of positive size.\n";
        return 0;
    }

    // reserve image as 2d array
    image = new bool*[size+1];
    for (int i = 0; i <= size; i++) image[i] = new bool[size+1];

    rasterizeArc(100);
    rasterizeArc(150); // called it again for R = 150 

    char filename[50];
#ifdef _WIN32
    sprintf_s(filename, 50, "circle%d.ppm", size);
#else
    sprintf(filename, "circle%d.ppm", size);
#endif

    ofstream outfile(filename);
    outfile << "P3\n# " << filename << "\n";
    outfile << size+1 << ' ' << size+1 << ' ' << 1 << endl;

    for (int i = 0; i <= size; i++)
        for (int j = 0; j <= size; j++)
            outfile << image[size-i][j] << " 0 0\n";

    // delete image data
    for (int i = 0; i <= size; i++) delete [] image[i];
    delete [] image;

    return 0;
}
