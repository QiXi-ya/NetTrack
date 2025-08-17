#pragma once
#include<string>
using namespace std;


class FileControl
{
public:
	static int UnpackFilesFromOne(const string& packedFileName, const string& outputDir);
};

class chooseFile
{
private:
    // 将宽字符串转换为多字节字符串
    static string WideToMultiByte(const wstring& wstr);
public:
    // 选择文件对话框
    static string OpenFileDialog(const wstring& title, const vector<pair<wstring, wstring>>& filters);

    // 选择文件夹对话框
    static string OpenFolderDialog(const wstring& title);
};