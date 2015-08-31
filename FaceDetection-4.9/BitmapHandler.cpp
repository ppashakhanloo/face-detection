//This file is designed to read the bitmap file
//and extract all the information about the image

#include <iostream>
#include <stdio.h>
#include <vector>
#include "utilities.h"
#define Byte unsigned char

using namespace std;

struct BitmapHeader //Contains the information of the first bitmap header
{
        Byte BM[2];
        Byte size[4];
        Byte unused[4];
        Byte colorTableOffset[4];
};

struct BitmapDIBHeader  //Contains the information of the second bitmap header
{
        Byte DIBSize[4];
        Byte width[4];
        Byte height[4];
        Byte numOfColorPlanes[2];
        Byte bitsPerPixel[2];
        Byte compress[4];
        Byte rawData[4];
        Byte horizontalResolution[4];
        Byte verticalResolution[4];
        Byte numOfColorInPalette[4];
        Byte importantColors[4];
};

class BitmapHandler
{
        public:
                BitmapHandler(){}

                BitmapHandler(char* address)    //Constructor
                {
                        file = fopen(address, "rb");
                }

                bool open(char* address)
                {
                        file = fopen(address, "rb");
                        if(!file)
                        {
                            this->errorReason = "Can not open file";
                            return false;
                        }
                        return true;
                }
                
                ~BitmapHandler()    //Destructor
                {
                        fclose(file);
                }
                
                bool init()
                {
                        fread(&bitHeader, sizeof(BitmapHeader), 1, file);
                        fread(&bitDib, sizeof(BitmapDIBHeader), 1,file);
                        offsetOfColorTable = convertByte2Int(bitHeader.colorTableOffset);
                        width = convertByte2Int(bitDib.width);
                        height = convertByte2Int(bitDib.height);
                        
                        compress = convertByte2Int(bitDib.compress);
                        bitsPerPixel = convertByte2Int(bitDib.bitsPerPixel);
                        BM = convertByte2Int(bitHeader.BM);
                        
                        numOfPadding = width % 4 == 0 ? 0:4 - (width*3) % 4;
                        
                        //Show all the important - IOW used - information of the image on the screen
                        cout << "num of padding: " << numOfPadding << endl;
                        cout << "offset: "<< offsetOfColorTable << endl;
                        cout << "width: " << width << endl;
                        cout << "height: " << height << endl;
                        cout << "compression method: " << compress << endl;
                        cout << "color depth: " << bitsPerPixel << endl;
						if(bitHeader.BM[0] != 'B' || bitHeader.BM[1] != 'M')
						{
                            this->errorReason = "File is not in bmp format";
                            return false;
                        }
						if(offsetOfColorTable != 54)
						{
                            this->errorReason = "File Corrupted";
                            return false;
                        }
						if(compress != 0)
						{
							this->errorReason = "File is compresed";
							return false;
                        }
                        if(bitsPerPixel != 24)
                        {
                            this->errorReason = "Color Depth is not 24";
                            return false;
                        }
                        
                        //Read R, G, and B parameters of each pixel and save it in a 2d vector
                        colorTable.resize(height);
                        for(int i = 0 ;i < height; i++)
                                colorTable[i].resize(width);
                        for(int i= 0 ; i < height; i++)
                        {
                                for(int j = 0; j < width; j++)
                                {
                                        colorTable[i][j].blue = readByte();
                                        colorTable[i][j].green = readByte();
                                        colorTable[i][j].red = readByte();
                                }
                                if(numOfPadding)
                                        skipNBytes(numOfPadding);
                        }
                        return true;
                }
                
                bool checkFile()
                {
                    if(!file)
                    {
                        this->errorReason = "Can not open file";
                        return false;
                    }
                    return true;
                    
                }

                inline void close()
                {
                        fclose(file);
                }

                inline void rewind()
                {
                        fseek(file, 0, SEEK_SET);   //Equals to rewind(file);
                }

                Byte readByte()
                {
                        Byte byte;
                        fread(&byte, sizeof(Byte), 1, file);
                        return byte;
                }

                inline void skipNBytes(int n)
                {
                        fseek(file, n, SEEK_CUR);
                }
                
                int readInt()
                {
                        int num;
                        fread(&num, sizeof(int), 1, file);
                        return num;
                }

                inline int convertByte2Int(Byte byte[])
                {
                        return *(int*)byte;
                }

				void write(char* address, vector<vector<RGB> > &colorTable)     //Convert a 2d vector to a bitmap file which can be previewed
				{
					FILE* newFile;
					newFile = fopen(address, "wb");
					fwrite(&bitHeader, sizeof(BitmapHeader), 1, newFile);
					fwrite(&bitDib, sizeof(BitmapDIBHeader), 1, newFile);
					for(int i = 0; i < colorTable.size(); i++)
					{
						for(int j = 0; j < colorTable[i].size(); j++)
						{
							fwrite(&colorTable[i][j].blue, sizeof(Byte), 1, newFile);
							fwrite(&colorTable[i][j].green, sizeof(Byte), 1, newFile);
							fwrite(&colorTable[i][j].red, sizeof(Byte), 1, newFile);

						}
						for(int k = 0; k < numOfPadding; k++)
						{
							Byte byte = '0';
							fwrite(&byte, sizeof(Byte), 1, newFile);
						}
					}
					fclose(newFile);
				}

                inline string getErrorReason()
				{
					return this->errorReason;
				}
				
                int offsetOfColorTable;
                int width;
                int height;
                int numOfPadding;
                int compress;
                int BM;
                int bitsPerPixel;
                vector<vector<RGB> > colorTable;
                BitmapHeader bitHeader;
                BitmapDIBHeader bitDib;
        
        private:
                string errorReason;
                FILE* file;
};
