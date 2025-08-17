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
    // �����ַ���ת��Ϊ���ֽ��ַ���
    static string WideToMultiByte(const wstring& wstr);
public:
    // ѡ���ļ��Ի���
    static string OpenFileDialog(const wstring& title, const vector<pair<wstring, wstring>>& filters);

    // ѡ���ļ��жԻ���
    static string OpenFolderDialog(const wstring& title);
};