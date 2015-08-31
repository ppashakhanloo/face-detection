#include<iostream.h>

using namespace std;

int main()
{
    FILE * imgAd;
    imgAd = fopen ("imgAd.txt","w");
    
    // Get bmp file location and write it in a txt file
    
    cout<<"Please enter a destination to load a file, "<<endl<<endl<<"The file must be a bitmap image with following features: "<<endl;
    cout<<"BM"<<endl<<"Compression Method = 0"<<endl<<"Color Depth = 24 bits per pixel"<<endl<<endl;
    char path[100];
    cin>>(path);
    fputs(path, imgAd);
    fclose(imgAd);
    
    return 0;
}
