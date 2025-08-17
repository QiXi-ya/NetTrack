#pragma warning(disable: 4244)  // 禁用特定警告
#pragma warning(disable: 4566)
#pragma warning(disable: 4838)
#pragma warning(disable: 4267)
#define  _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <wincrypt.h>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <chrono>
#include <ctime>
#include <dpapi.h>
#include <fstream>
#pragma comment(lib, "crypt32.lib")
#include "AES.h"

using namespace std;


    // DPAPI加密函数
     std::vector<BYTE> SecureFileStorage::DPAPIEncrypt(const std::vector<BYTE>& data) {
        DATA_BLOB dataIn, dataOut;
        dataIn.pbData = const_cast<BYTE*>(data.data());
        dataIn.cbData = static_cast<DWORD>(data.size());

        if (!CryptProtectData(&dataIn, L"SecureFileStorage", nullptr, nullptr,
            nullptr, CRYPTPROTECT_LOCAL_MACHINE, &dataOut)) {
            throw std::runtime_error("DPAPI加密失败，错误代码: " + std::to_string(GetLastError()));
        }

        std::vector<BYTE> encrypted(dataOut.pbData, dataOut.pbData + dataOut.cbData);
        LocalFree(dataOut.pbData);
        return encrypted;
    }

    // DPAPI解密函数
     std::vector<BYTE> SecureFileStorage::DPAPIDecrypt(const std::vector<BYTE>& encrypted) {
        DATA_BLOB dataIn, dataOut;
        dataIn.pbData = const_cast<BYTE*>(encrypted.data());
        dataIn.cbData = static_cast<DWORD>(encrypted.size());

        if (!CryptUnprotectData(&dataIn, nullptr, nullptr, nullptr,
            nullptr, 0, &dataOut)) {
            throw std::runtime_error("DPAPI解密失败，错误代码: " + std::to_string(GetLastError()));
        }

        std::vector<BYTE> decrypted(dataOut.pbData, dataOut.pbData + dataOut.cbData);
        LocalFree(dataOut.pbData);
        return decrypted;
    }

    // 简单的混淆/反混淆函数
     void SecureFileStorage::ObfuscateData(std::vector<BYTE>& data) {
        const BYTE xorKey = 0xAA; // 可自定义的异或密钥
        const BYTE rotateBits = 3; // 旋转位数

        for (auto& byte : data) {
            // 异或混淆
            byte ^= xorKey;
            // 循环左移
            byte = (byte << rotateBits) | (byte >> (8 - rotateBits));
        }
    }

     void SecureFileStorage::DeobfuscateData(std::vector<BYTE>& data) {
        const BYTE xorKey = 0xAA;
        const BYTE rotateBits = 3;

        for (auto& byte : data) {
            // 循环右移
            byte = (byte >> rotateBits) | (byte << (8 - rotateBits));
            // 异或还原
            byte ^= xorKey;
        }
    }

    // 将多个字符串序列化为字节流
     std::vector<BYTE> SecureFileStorage::SerializeData(const std::vector<std::string>& data) {
        std::vector<BYTE> result;
        for (const auto& str : data) {
            // 添加长度前缀
            uint32_t length = static_cast<uint32_t>(str.size());
            result.insert(result.end(), reinterpret_cast<BYTE*>(&length),
                reinterpret_cast<BYTE*>(&length) + sizeof(length));
            // 添加字符串内容
            result.insert(result.end(), str.begin(), str.end());
        }
        return result;
    }

    // 从字节流反序列化多个字符串
     std::vector<std::string> SecureFileStorage::DeserializeData(const std::vector<BYTE>& data) {
        std::vector<std::string> result;
        size_t pos = 0;
        while (pos < data.size()) {
            if (pos + sizeof(uint32_t) > data.size()) {
                throw std::runtime_error("Invalid data format");
            }
            // 读取长度
            uint32_t length;
            memcpy(&length, &data[pos], sizeof(length));
            pos += sizeof(length);

            // 读取字符串内容
            if (pos + length > data.size()) {
                throw std::runtime_error("Invalid data format");
            }
            result.emplace_back(data.begin() + pos, data.begin() + pos + length);
            pos += length;
        }
        return result;
    }


    // 安全存储字符串到文件
     bool SecureFileStorage::SaveSecureData(const std::vector<std::string>& dataInput, const std::wstring& filePath) {
        try {
            // 1. 序列化数据
            std::vector<BYTE> rawData = SerializeData(dataInput);

            // 2. 应用混淆
            ObfuscateData(rawData);

            // 3. 使用DPAPI加密
            std::vector<BYTE> encrypted = DPAPIEncrypt(rawData);

            // 4. 二进制写入文件
            std::ofstream outFile(filePath, std::ios::binary);
            if (!outFile) {
                SetLastError(0x15);
                return false;
            }

            outFile.write(reinterpret_cast<const char*>(encrypted.data()), encrypted.size());
            return outFile.good();
        }
        catch (...) {
            SetLastError(0x17);
            return false;
        }
    }

    // 从文件安全读取字符串
     std::vector<std::string> SecureFileStorage::LoadSecureData(const std::wstring& filePath) {
        try {
            // 1. 二进制读取文件
            std::ifstream inFile(filePath, std::ios::binary | std::ios::ate);
            if (!inFile) {
                SetLastError(0x18);
                return {};
            }

            std::streamsize size = inFile.tellg();
            inFile.seekg(0, std::ios::beg);

            std::vector<BYTE> encrypted(size);
            if (!inFile.read(reinterpret_cast<char*>(encrypted.data()), size)) {
                SetLastError(0x19);
                return {};
            }

            // 2. 使用DPAPI解密
            std::vector<BYTE> obfuscated = DPAPIDecrypt(encrypted);

            // 3. 反混淆
            DeobfuscateData(obfuscated);

            // 4. 反序列化数据
            return DeserializeData(obfuscated);
        }
        catch (...) {
            SetLastError(0x20);
            return {};
        }
    }


    bool  AES::Encrypt(const std::vector<std::string>& dataInput, const wchar_t* filePath) {
        return SecureFileStorage::SaveSecureData(dataInput, filePath);
    }

    bool  AES::Decrypt(const wchar_t* filePath, std::vector<std::string>& output) {
        output = SecureFileStorage::LoadSecureData(filePath);
        return !output.empty();
    }