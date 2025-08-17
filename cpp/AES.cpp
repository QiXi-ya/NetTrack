#pragma warning(disable: 4244)  // �����ض�����
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


    // DPAPI���ܺ���
     std::vector<BYTE> SecureFileStorage::DPAPIEncrypt(const std::vector<BYTE>& data) {
        DATA_BLOB dataIn, dataOut;
        dataIn.pbData = const_cast<BYTE*>(data.data());
        dataIn.cbData = static_cast<DWORD>(data.size());

        if (!CryptProtectData(&dataIn, L"SecureFileStorage", nullptr, nullptr,
            nullptr, CRYPTPROTECT_LOCAL_MACHINE, &dataOut)) {
            throw std::runtime_error("DPAPI����ʧ�ܣ��������: " + std::to_string(GetLastError()));
        }

        std::vector<BYTE> encrypted(dataOut.pbData, dataOut.pbData + dataOut.cbData);
        LocalFree(dataOut.pbData);
        return encrypted;
    }

    // DPAPI���ܺ���
     std::vector<BYTE> SecureFileStorage::DPAPIDecrypt(const std::vector<BYTE>& encrypted) {
        DATA_BLOB dataIn, dataOut;
        dataIn.pbData = const_cast<BYTE*>(encrypted.data());
        dataIn.cbData = static_cast<DWORD>(encrypted.size());

        if (!CryptUnprotectData(&dataIn, nullptr, nullptr, nullptr,
            nullptr, 0, &dataOut)) {
            throw std::runtime_error("DPAPI����ʧ�ܣ��������: " + std::to_string(GetLastError()));
        }

        std::vector<BYTE> decrypted(dataOut.pbData, dataOut.pbData + dataOut.cbData);
        LocalFree(dataOut.pbData);
        return decrypted;
    }

    // �򵥵Ļ���/����������
     void SecureFileStorage::ObfuscateData(std::vector<BYTE>& data) {
        const BYTE xorKey = 0xAA; // ���Զ���������Կ
        const BYTE rotateBits = 3; // ��תλ��

        for (auto& byte : data) {
            // ������
            byte ^= xorKey;
            // ѭ������
            byte = (byte << rotateBits) | (byte >> (8 - rotateBits));
        }
    }

     void SecureFileStorage::DeobfuscateData(std::vector<BYTE>& data) {
        const BYTE xorKey = 0xAA;
        const BYTE rotateBits = 3;

        for (auto& byte : data) {
            // ѭ������
            byte = (byte >> rotateBits) | (byte << (8 - rotateBits));
            // ���ԭ
            byte ^= xorKey;
        }
    }

    // ������ַ������л�Ϊ�ֽ���
     std::vector<BYTE> SecureFileStorage::SerializeData(const std::vector<std::string>& data) {
        std::vector<BYTE> result;
        for (const auto& str : data) {
            // ��ӳ���ǰ׺
            uint32_t length = static_cast<uint32_t>(str.size());
            result.insert(result.end(), reinterpret_cast<BYTE*>(&length),
                reinterpret_cast<BYTE*>(&length) + sizeof(length));
            // ����ַ�������
            result.insert(result.end(), str.begin(), str.end());
        }
        return result;
    }

    // ���ֽ��������л�����ַ���
     std::vector<std::string> SecureFileStorage::DeserializeData(const std::vector<BYTE>& data) {
        std::vector<std::string> result;
        size_t pos = 0;
        while (pos < data.size()) {
            if (pos + sizeof(uint32_t) > data.size()) {
                throw std::runtime_error("Invalid data format");
            }
            // ��ȡ����
            uint32_t length;
            memcpy(&length, &data[pos], sizeof(length));
            pos += sizeof(length);

            // ��ȡ�ַ�������
            if (pos + length > data.size()) {
                throw std::runtime_error("Invalid data format");
            }
            result.emplace_back(data.begin() + pos, data.begin() + pos + length);
            pos += length;
        }
        return result;
    }


    // ��ȫ�洢�ַ������ļ�
     bool SecureFileStorage::SaveSecureData(const std::vector<std::string>& dataInput, const std::wstring& filePath) {
        try {
            // 1. ���л�����
            std::vector<BYTE> rawData = SerializeData(dataInput);

            // 2. Ӧ�û���
            ObfuscateData(rawData);

            // 3. ʹ��DPAPI����
            std::vector<BYTE> encrypted = DPAPIEncrypt(rawData);

            // 4. ������д���ļ�
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

    // ���ļ���ȫ��ȡ�ַ���
     std::vector<std::string> SecureFileStorage::LoadSecureData(const std::wstring& filePath) {
        try {
            // 1. �����ƶ�ȡ�ļ�
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

            // 2. ʹ��DPAPI����
            std::vector<BYTE> obfuscated = DPAPIDecrypt(encrypted);

            // 3. ������
            DeobfuscateData(obfuscated);

            // 4. �����л�����
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