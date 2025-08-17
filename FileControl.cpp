#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <thread>
#include <Windows.h>
#include "FileControl.h"
#include <ShlObj_core.h>

using namespace std;
namespace fs = filesystem;

// �����������packedFileName�лָ��ļ���outputDir
int FileControl::UnpackFilesFromOne(const string& packedFileName, const string& outputDir) {
    ifstream in(packedFileName, ios::binary);
    if (!in) {
        cerr << "�޷��������ļ�: " << packedFileName << endl;
        return -1;
    }

    // �жϽ��Ŀ¼
    fs::path outDirPath;
    if (outputDir.empty()) {
        outDirPath = fs::current_path();
    }
    else {
        outDirPath = outputDir;
        if (!fs::exists(outDirPath)) {
            fs::create_directories(outDirPath);
        }
    }

    while (in.peek() != EOF) {
        int nameLen = 0;
        in.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
        if (in.gcount() != sizeof(nameLen)) break;

        string filename(nameLen, '\0');
        in.read(&filename[0], nameLen);

        streamsize fileSize = 0;
        in.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));

        vector<char> buffer(static_cast<size_t>(fileSize));
        in.read(buffer.data(), fileSize);

        ofstream out(outDirPath / filename, ios::binary);
        out.write(buffer.data(), fileSize);
    }
    in.close();
    return 1;
}


    // �����ַ���ת��Ϊ���ֽ��ַ���
    string chooseFile::WideToMultiByte(const wstring& wstr) {
        if (wstr.empty()) return "";

        int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), nullptr, 0, nullptr, nullptr);
        string strTo(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, nullptr, nullptr);
        return strTo;
    }

    // ѡ���ļ��Ի���
    string chooseFile::OpenFileDialog(const wstring& title, const vector<pair<wstring, wstring>>& filters) {
        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        if (FAILED(hr)) return "ERROR";

        IFileOpenDialog* pFileOpen = nullptr;
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
            IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
        if (FAILED(hr)) {
            CoUninitialize();
            return "ERROR";
        }

        // ���ñ���
        pFileOpen->SetTitle(title.c_str());

        // �����ļ�������
        vector<COMDLG_FILTERSPEC> filterSpecs;
        for (const auto& filter : filters) {
            filterSpecs.push_back({ filter.first.c_str(), filter.second.c_str() });
        }

        if (!filterSpecs.empty()) {
            pFileOpen->SetFileTypes(static_cast<UINT>(filterSpecs.size()), filterSpecs.data());
        }

        // ��ʾ�Ի���
        hr = pFileOpen->Show(NULL);
        if (FAILED(hr)) {
            pFileOpen->Release();
            CoUninitialize();
            return ""; // �û�ȡ��
        }

        IShellItem* pItem = nullptr;
        hr = pFileOpen->GetResult(&pItem);
        if (FAILED(hr)) {
            pFileOpen->Release();
            CoUninitialize();
            return "ERROR";
        }

        PWSTR pszFilePath = nullptr;
        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
        pItem->Release();
        pFileOpen->Release();
        CoUninitialize();

        if (FAILED(hr)) return "ERROR";

        wstring wPath(pszFilePath);
        CoTaskMemFree(pszFilePath);
        return WideToMultiByte(wPath);
    }

    // ѡ���ļ��жԻ���
    string chooseFile::OpenFolderDialog(const wstring& title) {
        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        if (FAILED(hr)) return "ERROR";

        IFileOpenDialog* pFileOpen = nullptr;
        hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
            IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
        if (FAILED(hr)) {
            CoUninitialize();
            return "ERROR";
        }

        // ����Ϊ�ļ���ģʽ
        DWORD dwOptions;
        pFileOpen->GetOptions(&dwOptions);
        pFileOpen->SetOptions(dwOptions | FOS_PICKFOLDERS);

        // ���ñ���
        pFileOpen->SetTitle(title.c_str());

        // ��ʾ�Ի���
        hr = pFileOpen->Show(NULL);
        if (FAILED(hr)) {
            pFileOpen->Release();
            CoUninitialize();
            return ""; // �û�ȡ��
        }

        IShellItem* pItem = nullptr;
        hr = pFileOpen->GetResult(&pItem);
        if (FAILED(hr)) {
            pFileOpen->Release();
            CoUninitialize();
            return "ERROR";
        }

        PWSTR pszFilePath = nullptr;
        hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
        pItem->Release();
        pFileOpen->Release();
        CoUninitialize();

        if (FAILED(hr)) return "ERROR";

        wstring wPath(pszFilePath);
        CoTaskMemFree(pszFilePath);
        return WideToMultiByte(wPath);
    }
