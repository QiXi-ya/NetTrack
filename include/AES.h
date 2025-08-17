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

    // DPAPI解密函数
    static std::vector<BYTE> DPAPIDecrypt(const std::vector<BYTE>& encrypted);

    // 简单的混淆/反混淆函数
    static void ObfuscateData(std::vector<BYTE>& data);

    static void DeobfuscateData(std::vector<BYTE>& data);

    // 将多个字符串序列化为字节流
    static std::vector<BYTE> SerializeData(const std::vector<std::string>& data);

    // 从字节流反序列化多个字符串
    static std::vector<std::string> DeserializeData(const std::vector<BYTE>& data);
public:
    // 安全存储字符串到文件
    static bool SaveSecureData(const std::vector<std::string>& dataInput, const std::wstring& filePath);

    // 从文件安全读取字符串
    static std::vector<std::string> LoadSecureData(const std::wstring& filePath);
};