#pragma once
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class AES
{
public:
    static bool Encrypt(const std::vector<std::string>& dataInput, const wchar_t* filePath);
    static bool Decrypt(const wchar_t* filePath, std::vector<std::string>& output);
};

class SecureFileStorage
{
private:
    static std::vector<BYTE> DPAPIEncrypt(const std::vector<BYTE>& data);

    // DPAPI���ܺ���
    static std::vector<BYTE> DPAPIDecrypt(const std::vector<BYTE>& encrypted);

    // �򵥵Ļ���/����������
    static void ObfuscateData(std::vector<BYTE>& data);

    static void DeobfuscateData(std::vector<BYTE>& data);

    // ������ַ������л�Ϊ�ֽ���
    static std::vector<BYTE> SerializeData(const std::vector<std::string>& data);

    // ���ֽ��������л�����ַ���
    static std::vector<std::string> DeserializeData(const std::vector<BYTE>& data);
public:
    // ��ȫ�洢�ַ������ļ�
    static bool SaveSecureData(const std::vector<std::string>& dataInput, const std::wstring& filePath);

    // ���ļ���ȫ��ȡ�ַ���
    static std::vector<std::string> LoadSecureData(const std::wstring& filePath);
};