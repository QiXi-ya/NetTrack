#include "Main.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <cstddef>
using ::byte;

using namespace std;

const vector<string> CheckFileList =
{
    "config.dll",
    "ts.dll",
    "Record_upload.dll",
    "Record_download.dll",
    "ofstartimage.dll",
    "NetTrackHt.dll",
    "NetTrack_imgPack.dll",
    "config.ini",
    "NetTrackImgManager.dll"
};

const int number = 8;

bool isFile(string filePath)
{
    ifstream sK(filePath);
    if (sK.is_open())
    {
        sK.close();
        return true;
    }
    else
    {
        return false;
    }
}

int CheckFile()
{
    bool isFileK[number];

    for (int i = 0; i < number; i++)
    {
        isFileK[i] = true;
    }

    for (int i = 0; i < number; i++)
    {
        isFileK[i] = isFile(CheckFileList[i]);
        if (!isFileK[i]) return i;
    }
}