//This program is designed by Pardis Pashakhanloo & Mehran Akhavan Khaleghhi

//Include header files and needed .cpp files 
#include <iostream.h>
#include <vector.h>
#include "BitmapHandler.cpp"
#include "Convert.cpp"
#include "utilities.h"
#include <conio.h>
#include "Tests.cpp"

void finalize(char*);

int main()
{
        //Opens the file which contains the image location
        FILE * address;
        address = fopen("imgAd.txt", "r");
        char path[100];
        fgets(path, 100, address);
        fclose(address);
        
        finalize(path);
        
        return 0;
}


void finalize(char * path)     //Make the last - result - image

{
        BitmapHandler bh(path);
        if(!bh.checkFile())     //Print an error related to the bitmap file
        {
            cout << "Error; " << bh.getErrorReason() << endl;
            getch();
            return;
        }
        if(!bh.init())      //Print an error if initializing the bh - BitmapHandler - counters difficulties
        {
            cout << "Error; " << bh.getErrorReason() << endl;
            getch();
            return;
        }
        
        Test ts(&bh);
        
        //Do the steps for detecting faces in the image
        ts.init();      //Initialize Test
        ts.testFace();      //Run testFace -> detect skin based on H range and rg range
		ts.Median(3);      //Run Median filter on the image in ordet to decrease the noise 
        ts.createComponent();   //Create components out of the related parts
        ts.areaTest();      //Run area test in order to remove components which are smaller or larger than expected face area
		ts.percentageTest();  //Run percentage test in order to remove components which contain too little ro too much skin pixels
        ts.goldenRatio();   //Run Golden Ratio test in order to remove components which doesn't match this ratio
        ts.drawRectangle();     //Draw a blue rectangle around detected faces

        {       //Make the final bitmap file
			vector<vector<RGB> > tempColorTable;
			tempColorTable.resize(ts.matrix.size());
			bh.rewind();
			bh.skipNBytes(bh.offsetOfColorTable);
			for(int i = 0; i < bh.height; i++)
			{
				tempColorTable[i].resize(bh.width);
				for(int j = 0; j < bh.width; j++)
				{
					if(ts.matrix[i][j] == -3)  //-3 means rectangle pixel
					{
						tempColorTable[i][j].red = 0;
						tempColorTable[i][j].green = 0;
						tempColorTable[i][j].blue = 255;
						bh.skipNBytes(3);
					}
                    else    //Else copy the original image pixel
					{
						tempColorTable[i][j].blue = bh.readByte();
						tempColorTable[i][j].green = bh.readByte();
						tempColorTable[i][j].red = bh.readByte();
					}

				}
                bh.skipNBytes(bh.numOfPadding);
			
			}
			bh.write("result.bmp", tempColorTable);
        }
}
