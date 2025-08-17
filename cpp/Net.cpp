#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Net.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include "Main.h"
#include <shlobj.h>
#include <iphlpapi.h>
#include <wininet.h>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "libcrypto.lib")
using namespace std;
namespace fs = std::filesystem;

// ========================= Net类实现 =========================

// 静态成员变量定义
// 
// 发送线程退出标志
bool isExit_send = false;
// 接收线程退出标志
bool isExit_recv = false;
//接收string线程退出标志
bool isExit_recvString = false;
// 发送线程运行状态
bool isSendRun = false;
// 接收线程运行状态
bool isRecvRun = false;
//接收字符串线程运行状态
bool isRecvStringRun = false;
// 接收线程对象
std::thread recvThread;
// 当前统计周期接收字节数
uint64_t currentRecvBytes = 0;
// 当前接收文件总大小
uint64_t currentFileSize = 0;
// 当前接收文件已接收字节数
uint64_t currentFileReceived = 0;
// 速度统计互斥锁
std::mutex recvSpeedMutex;
// 当前发送加密模式
int encryptModeSend = 0;
// 当前接收解密模式
int encryptModeRecv = 0;
// 当前发送加密密钥
std::string encryptKeySend = "000000";
// 当前接收解密密钥
std::string encryptKeyRecv = "000000";
// 保存接收到的加密字符串
std::vector<std::string> encryptedStringList;
// 保存接收文件端接收到的文件名
std::vector<std::wstring> receivedFileNameList;
// 发送端速度统计相关变量
uint64_t currentSendBytes = 0;
uint64_t currentSendFileSize = 0;
uint64_t currentSendFileSent = 0;
std::mutex sendSpeedMutex;
// 发送速度上限（MB/s），0表示不限制
int sendSpeedLimitMB = 0;

string GetCurrentTimeS() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X"); // 格式：YYYY-MM-DD HH:MM:SS
    return ss.str();
}

// ----------- 加密/解密设置 -----------
void Net::setEncryptKey(const std::string& key) { encryptKeySend = key; }
void Net::setDecryptKey(const std::string& key) { encryptKeyRecv = key; }
void Net::setEncryptMode(int mode) { encryptModeSend = mode; }
void Net::setDecryptMode(int mode) { encryptModeRecv = mode; }
void Net::setSendSpeedLimitMB(int limit) { sendSpeedLimitMB = limit; }

// ----------- AES加密辅助 -----------
void Net::aesEncrypt(std::vector<char>& buf, const std::string& key) {
    unsigned char aesKey[16] = {0};
    strncpy((char*)aesKey, key.c_str(), 16);
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    std::vector<unsigned char> out(buf.size() + AES_BLOCK_SIZE);
    int outlen1 = 0, outlen2 = 0;
    EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, aesKey, NULL);
    EVP_EncryptUpdate(ctx, out.data(), &outlen1, (unsigned char*)buf.data(), buf.size());
    EVP_EncryptFinal_ex(ctx, out.data() + outlen1, &outlen2);
    EVP_CIPHER_CTX_free(ctx);
    buf.assign(out.begin(), out.begin() + outlen1 + outlen2);
}

void Net::aesDecrypt(std::vector<char>& buf, const std::string& key) {
    unsigned char aesKey[16] = {0};
    strncpy((char*)aesKey, key.c_str(), 16);
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    std::vector<unsigned char> out(buf.size() + AES_BLOCK_SIZE);
    int outlen1 = 0, outlen2 = 0;
    EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, aesKey, NULL);
    EVP_DecryptUpdate(ctx, out.data(), &outlen1, (unsigned char*)buf.data(), buf.size());
    EVP_DecryptFinal_ex(ctx, out.data() + outlen1, &outlen2);
    EVP_CIPHER_CTX_free(ctx);
    buf.assign(out.begin(), out.begin() + outlen1 + outlen2);
}

// ----------- 加密/解密函数 -----------
void Net::encryptBuffer(std::vector<char>& buf, int mode, const std::string& key) {
    if (mode == 1) {
        for (auto& c : buf) c ^= 0xAA;
    } else if (mode == 2) {
        for (size_t i = 0; i < buf.size(); ++i) buf[i] ^= key[i % key.size()];
    } else if (mode == 3) {
        aesEncrypt(buf, key);
    }
}

void Net::decryptBuffer(std::vector<char>& buf, int mode, const std::string& key) {
    if (mode == 1) {
        for (auto& c : buf) c ^= 0xAA;
    } else if (mode == 2) {
        for (size_t i = 0; i < buf.size(); ++i) buf[i] ^= key[i % key.size()];
    } else if (mode == 3) {
        aesDecrypt(buf, key);
    }
}

// ----------- 工具函数 -----------
std::string Net::get_filename_only(const std::string& path) {
    size_t pos = path.find_last_of("\\/");
    if (pos == std::string::npos) return path;
    return path.substr(pos + 1);
}

// ----------- 文件夹打包 -----------
bool Net::packFolder(const std::string& folderPath, const std::string& outputFile) {
    try {
        std::ofstream outFile(outputFile, std::ios::binary);
        if (!outFile) {
            std::cerr << "无法创建输出文件: " << outputFile << std::endl;
            return false;
        }
        // 写入文件头标识（长度为3）
        const char header[] = "NPK";
        outFile.write(header, 3);
        // 写入根目录项
        char dirMarker = 'D';
        outFile.write(&dirMarker, 1);
        std::string rootName = fs::path(folderPath).filename().string();
        uint32_t rootLen = rootName.size();
        outFile.write(reinterpret_cast<const char*>(&rootLen), sizeof(rootLen));
        outFile.write(rootName.c_str(), rootLen);
        // 遍历文件夹
        for (const auto& entry : fs::recursive_directory_iterator(folderPath)) {
            std::string relativePath = fs::relative(entry.path(), folderPath).string();
            std::replace(relativePath.begin(), relativePath.end(), '\\', '/');
            if (entry.is_directory()) {
                char dirMarker = 'D';
                outFile.write(&dirMarker, 1);
                uint32_t pathLength = relativePath.size();
                outFile.write(reinterpret_cast<const char*>(&pathLength), sizeof(pathLength));
                outFile.write(relativePath.c_str(), pathLength);
            }
            else if (entry.is_regular_file()) {
                char fileMarker = 'F';
                outFile.write(&fileMarker, 1);
                uint32_t pathLength = relativePath.size();
                outFile.write(reinterpret_cast<const char*>(&pathLength), sizeof(pathLength));
                outFile.write(relativePath.c_str(), pathLength);
                uint64_t fileSize = entry.file_size();
                outFile.write(reinterpret_cast<const char*>(&fileSize), sizeof(fileSize));
                std::ifstream inFile(entry.path(), std::ios::binary);
                if (!inFile) {
                    std::cerr << "无法打开文件: " << entry.path() << std::endl;
                    return false;
                }
                std::vector<char> buffer(fileSize);
                inFile.read(buffer.data(), fileSize);
                encryptBuffer(buffer, encryptModeSend, encryptKeySend);
                outFile.write(buffer.data(), fileSize);
                inFile.close();
            }
        }
        outFile.close();
        return true;
    }
    catch (const std::exception& e) {
        std::cerr << "打包错误: " << e.what() << std::endl;
        return false;
    }
}

// ----------- 文件夹解包 -----------
string Net::unpackFolder(const std::string& inputFile, const std::string& outputFolder) {
    try {
        std::ifstream inFile(inputFile, std::ios::binary);
        if (!inFile) {
            std::cerr << "无法打开输入文件: " << inputFile << std::endl;
            return "false";
        }
        char header[4] = { 0 };
        inFile.read(header, 3);
        if (std::string(header) != "NPK") {
            std::cerr << "无效的打包文件格式" << std::endl;
            return "false";
        }
        // 读取根目录名
        char marker;
        inFile.read(&marker, 1);
        if (marker != 'D') {
            std::cerr << "无效的根目录标记" << std::endl;
            return "false";
        }
        uint32_t rootLen;
        inFile.read(reinterpret_cast<char*>(&rootLen), sizeof(rootLen));
        std::vector<char> rootBuffer(rootLen + 1);
        inFile.read(rootBuffer.data(), rootLen);
        rootBuffer[rootLen] = '\0';
        std::string rootName = rootBuffer.data();
        fs::path rootPath = fs::path(outputFolder) / rootName;
        fs::create_directories(rootPath);
        // 解包内容
        while (inFile) {
            char marker;
            inFile.read(&marker, 1);
            if (inFile.eof()) break;
            uint32_t pathLength;
            inFile.read(reinterpret_cast<char*>(&pathLength), sizeof(pathLength));
            std::vector<char> pathBuffer(pathLength + 1);
            inFile.read(pathBuffer.data(), pathLength);
            pathBuffer[pathLength] = '\0';
            std::string relativePath = pathBuffer.data();
            fs::path fullPath = rootPath / relativePath;
            if (marker == 'D') {
                fs::create_directories(fullPath);
            }
            else if (marker == 'F') {
                uint64_t fileSize;
                inFile.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));
                fs::create_directories(fullPath.parent_path());
                std::ofstream outFile(fullPath, std::ios::binary);
                if (!outFile) {
                    std::cerr << "无法创建文件: " << fullPath << std::endl;
                    return "false";
                }
                std::vector<char> buffer(fileSize);
                inFile.read(buffer.data(), fileSize);
                decryptBuffer(buffer, encryptModeRecv, encryptKeyRecv);
                outFile.write(buffer.data(), fileSize);
                outFile.close();
            }
            else {
                std::cerr << "无效的标记: " << marker << std::endl;
                return "false";
            }
        }
        inFile.close();
        remove(inputFile.c_str());
        receivedFileNameList.push_back(fs::path(rootName).wstring());
        return rootPath.string();
    }
    catch (const std::exception& e) {
        std::cerr << "解包错误: " << e.what() << std::endl;
        remove(inputFile.c_str());
        return "false";
    }
}

// ----------- 文件发送 -----------
void Net::SendFile(const string& ip, unsigned short port, const string& filepath) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int timeout = 1000;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) != 0) {
        closesocket(sock);
        WSACleanup();
        return;
    }

    // 打开文件并获取文件大小
    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        closesocket(sock);
        WSACleanup();
        return;
    }

    file.seekg(0, std::ios::end);
    uint64_t filesize = file.tellg();
    file.seekg(0, std::ios::beg);

    string filename = get_filename_only(filepath);
    uint32_t namelen = filename.size();
    uint8_t mode = (uint8_t)encryptModeSend;

    // 发送加密模式和文件信息
    send(sock, (char*)&mode, sizeof(mode), 0);
    send(sock, (char*)&namelen, sizeof(namelen), 0);
    send(sock, filename.c_str(), namelen, 0);
    send(sock, (char*)&filesize, sizeof(filesize), 0);

    // 初始化发送统计信息
    {
        std::lock_guard<std::mutex> lock(sendSpeedMutex);
        currentSendFileSize = filesize;
        currentSendFileSent = 0;
        currentSendBytes = 0;
    }

    // 文件传输
    vector<char> buffer(4096);
    uint64_t sent = 0;
    auto lastTime = std::chrono::steady_clock::now();
    uint64_t bytesSentThisSecond = 0;
    auto lastUpdateTime = lastTime;

    while (sent < filesize) {
        size_t to_read = min<size_t>(buffer.size(), filesize - sent);
        file.read(buffer.data(), to_read);

        // 加密数据
        encryptBuffer(buffer, encryptModeSend, encryptKeySend);

        int bytesSent = send(sock, buffer.data(), to_read, 0);
        if (bytesSent <= 0) break;

        sent += bytesSent;
        bytesSentThisSecond += bytesSent;

        // 更新发送进度
        {
            std::lock_guard<std::mutex> lock(sendSpeedMutex);
            currentSendFileSent = sent;
            currentSendBytes = bytesSentThisSecond; // 实时更新当前速度
        }

        // 限速处理
        auto now = std::chrono::steady_clock::now();
        if (sendSpeedLimitMB > 0) {
            double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime).count() / 1000.0;
            if (elapsed < 1.0 && bytesSentThisSecond >= sendSpeedLimitMB * 1024 * 1024) {
                std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>((1.0 - elapsed) * 1000)));
                lastTime = std::chrono::steady_clock::now();
                bytesSentThisSecond = 0;
            }
        }

        // 每秒重置计数器
        if (std::chrono::duration_cast<std::chrono::seconds>(now - lastUpdateTime).count() >= 1) {
            bytesSentThisSecond = 0;
            lastUpdateTime = now;
        }
    }

    // 传输完成后的清理
    file.close();
    closesocket(sock);
    WSACleanup();

    // 如果是临时打包文件，删除它
    if (filename == "netPack.npk") {
        remove("netPack.npk");
    }

    // 更新上传历史记录
    for (int i = lastUploadMax - 1; i > 0; --i) {
        lastUpload[i] = lastUpload[i - 1];
    }
    string kl = GetCurrentTimeS() + "   " + sendFileList[0].displayPath;
    lastUpload[0] = kl;
}

// ----------- 文件接收（持续接收） -----------
void Net::RecvFile(unsigned short port, const string& save_dir) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    int timeout = 1000;
    setsockopt(listenSock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
    setsockopt(listenSock, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(listenSock, (sockaddr*)&addr, sizeof(addr));
    listen(listenSock, 1);

    while (!isExit_recv) {
        SOCKET clientSock = accept(listenSock, nullptr, nullptr);
        if (clientSock == INVALID_SOCKET) continue;

        setsockopt(clientSock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
        setsockopt(clientSock, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));

        // 读取加密模式
        uint8_t mode = 0;
        recv(clientSock, (char*)&mode, sizeof(mode), MSG_WAITALL);
        encryptModeRecv = mode;

        // 读取文件信息
        uint32_t namelen = 0;
        recv(clientSock, (char*)&namelen, sizeof(namelen), MSG_WAITALL);

        vector<char> namebuf(namelen);
        recv(clientSock, namebuf.data(), namelen, MSG_WAITALL);

        string filename(namebuf.begin(), namebuf.end());
        uint64_t filesize = 0;
        recv(clientSock, (char*)&filesize, sizeof(filesize), MSG_WAITALL);

        // 文件接收
        string fullpath = save_dir + "\\" + filename;
        std::ofstream outfile(fullpath, std::ios::binary);
        vector<char> buffer(4096);
        uint64_t received = 0;

        while (received < filesize) {
            size_t to_recv = min<size_t>(buffer.size(), filesize - received);
            int ret = recv(clientSock, buffer.data(), to_recv, MSG_WAITALL);
            if (ret <= 0) break;

            // 解密处理
            decryptBuffer(buffer, encryptModeRecv, encryptKeyRecv);

            outfile.write(buffer.data(), ret);
            received += ret;
        }

        outfile.close();
        closesocket(clientSock);
        {
            std::lock_guard<std::mutex> lock(recvSpeedMutex);
            currentFileSize = 0;
            currentFileReceived = 0;
        }
        if (filename == "netPack.npk") {
            // 解包到 save_dir
            string backinf = unpackFolder(fullpath, save_dir);
            if (backinf != "false") {
                string save = save_dir + backinf;
                receivedFileNameList.push_back(std::wstring(save.begin(), save.end()));
            }
        }
        else {
            string save = save_dir + filename;
            receivedFileNameList.push_back(std::wstring(save.begin(), save.end()));
        }
    }

    closesocket(listenSock);
    WSACleanup();
}

// ----------- 线程控制 -----------
void Net::startSendThread(std::string ip, unsigned short port) {
    while (isSendRun) std::this_thread::sleep_for(std::chrono::milliseconds(100));
    isExit_send = false;
    std::thread t(Net::SendWorker, ip, port);
    t.detach();
}

void Net::stopSendThread() { isExit_send = true; while (isSendRun) { api_sleep(100); } }

void Net::startRecvThread(unsigned short port, const std::string& save_dir) {
    while (isRecvRun) std::this_thread::sleep_for(std::chrono::milliseconds(100));
    isExit_recv = false;
    recvThread = std::thread(Net::RecvWorker, port, save_dir);
    recvThread.detach();
}

void Net::stopRecvThread() { isExit_recv = true; while (isRecvRun) { api_sleep(100); }}

// ----------- 发送/接收工作线程 -----------
void Net::SendWorker(std::string ip, unsigned short port) {
    isSendRun = true;
    int clock = 0;
    while (!isExit_send)
    {
        if (!sendFileList.empty())// && clock == 10)
        {
			SendFile(ip, port, sendFileList[0].filePath);
			sendFileList.erase(sendFileList.begin());
            clock = 0;
        }
        else
        {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    isSendRun = false;
}

void Net::RecvWorker(unsigned short port, const std::string& save_dir) {
    isRecvRun = true;
    RecvFile(port, save_dir);
    isRecvRun = false;
}

// ----------- 速度与剩余时间 -----------
// 获取当前发送速度字符串（KB/s, MB/s）
std::string Net::GetSendSpeedString() {
    std::lock_guard<std::mutex> lock(sendSpeedMutex);

    double speed = currentSendBytes / 1024.0; // KB/s
    char buf[64];

    if (speed < 1024) {
        sprintf(buf, "%.2f KB/s", speed);
    }
    else {
        sprintf(buf, "%.2f MB/s", speed / 1024.0);
    }

    return std::string(buf);
}

// 获取当前文件发送剩余时间字符串（s, m, h）
std::string Net::GetSendRemainTimeString() {
    std::lock_guard<std::mutex> lock(sendSpeedMutex);

    // 如果没有文件在传输或传输已完成
    if (currentSendFileSize == 0 || currentSendFileSent >= currentSendFileSize) {
        return "0s";
    }

    // 如果刚开始传输还没有速度数据
    if (currentSendBytes == 0) {
        return "Calculating...";
    }

    // 计算剩余字节和预计时间
    uint64_t remain = currentSendFileSize - currentSendFileSent;
    double seconds = remain / (currentSendBytes * 1.0);

    // 处理极端情况（速度非常慢时）
    if (seconds > 86400 * 365) { // 超过1年
        return "Long time";
    }

    // 转换为小时、分钟、秒
    int h = static_cast<int>(seconds) / 3600;
    int m = (static_cast<int>(seconds) % 3600) / 60;
    int s = static_cast<int>(seconds) % 60;

    char buf[64] = "";
    if (h > 0) sprintf(buf, "%dh ", h);
    if (m > 0) sprintf(buf + strlen(buf), "%dm ", m);
    if (s > 0 || (h == 0 && m == 0)) sprintf(buf + strlen(buf), "%ds", s);

    return std::string(buf);
}

// 获取下载文件夹路径
std::wstring Net::GetDownloadFolderPath() {
    PWSTR path = nullptr;
    if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Downloads, 0, NULL, &path))) {
        std::wstring result(path);
        CoTaskMemFree(path);
        return result;
    }
    return L"";
}

// 检查网络连接状态
bool Net::CheckInternetConnection() {
    DWORD dwFlags = 0;
    return InternetGetConnectedState(&dwFlags, 0) != 0;
}

// 获取设备IP
std::string Net::get_local_ip() {
    ULONG flags = GAA_FLAG_INCLUDE_PREFIX;
    ULONG family = AF_INET;
    ULONG bufLen = 15000;
    std::vector<char> buffer(bufLen);
    PIP_ADAPTER_ADDRESSES pAddresses = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(buffer.data());
    if (GetAdaptersAddresses(family, flags, NULL, pAddresses, &bufLen) != NO_ERROR) return "";
    for (PIP_ADAPTER_ADDRESSES curr = pAddresses; curr; curr = curr->Next) {
        // 排除虚拟网卡
        std::wstring name;
        if (curr->FriendlyName) name = curr->FriendlyName;
        if (name.find(L"VMware") != std::wstring::npos ||
            name.find(L"Virtual") != std::wstring::npos ||
            name.find(L"Hyper-V") != std::wstring::npos ||
            name.find(L"Loopback") != std::wstring::npos) continue;
        if (curr->OperStatus != IfOperStatusUp) continue;
        for (PIP_ADAPTER_UNICAST_ADDRESS ua = curr->FirstUnicastAddress; ua; ua = ua->Next) {
            SOCKADDR_IN* sa_in = reinterpret_cast<SOCKADDR_IN*>(ua->Address.lpSockaddr);
            char ip[32] = {0};
            inet_ntop(AF_INET, &sa_in->sin_addr, ip, sizeof(ip));
            std::string ipstr(ip);
            if (ipstr.find("192.168.") == 0 || ipstr.find("10.") == 0 || ipstr.find("172.") == 0) {
                return ipstr;
            }
        }
    }
    // fallback: 返回第一个非回环地址且不是虚拟网卡
    for (PIP_ADAPTER_ADDRESSES curr = pAddresses; curr; curr = curr->Next) {
        std::wstring name;
        if (curr->FriendlyName) name = curr->FriendlyName;
        if (name.find(L"VMware") != std::wstring::npos ||
            name.find(L"Virtual") != std::wstring::npos ||
            name.find(L"Hyper-V") != std::wstring::npos ||
            name.find(L"Loopback") != std::wstring::npos) continue;
        if (curr->OperStatus != IfOperStatusUp) continue;
        for (PIP_ADAPTER_UNICAST_ADDRESS ua = curr->FirstUnicastAddress; ua; ua = ua->Next) {
            SOCKADDR_IN* sa_in = reinterpret_cast<SOCKADDR_IN*>(ua->Address.lpSockaddr);
            char ip[32] = {0};
            inet_ntop(AF_INET, &sa_in->sin_addr, ip, sizeof(ip));
            std::string ipstr(ip);
            if (ipstr != "127.0.0.1") return ipstr;
        }
    }
    return "127.0.0.1";
}

// ----------- 状态查询 -----------
std::string Net::getSendStatus() {
    if (isSendRun) return "SEND";
    if (encryptModeSend > 0) return "LOCK";
    return "ERROR";
}

// ----------- 其它传输相关函数 -----------
// 错误代码对照表
std::vector<std::string> NetErrorTable = {
    "Success", // 0
    "WSAStartup failed", // 1
    "Socket creation failed", // 2
    "Connect failed", // 3
    "Send failed", // 4
    "Recv failed", // 5
    "File open failed", // 6
    "Unknown error" // 7
};

// 修改SendXorString返回值
int Net::SendXorString(const std::string ip, unsigned short port, const std::string str) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) return 1;
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // 设置超时时间为1秒
    int timeout = 1;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
    if (sock == INVALID_SOCKET) { WSACleanup(); return 2; }
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);
    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) != 0) {
        closesocket(sock); WSACleanup(); return 3;
    }
    uint32_t strlen = str.size();
    std::vector<char> buf(str.begin(), str.end());
    encryptBuffer(buf, encryptModeSend, encryptKeySend);
    if (send(sock, (char*)&strlen, sizeof(strlen), 0) != sizeof(strlen)) {
        closesocket(sock); WSACleanup(); return 4;
    }
    if (send(sock, buf.data(), strlen, 0) != strlen) {
        closesocket(sock); WSACleanup(); return 4;
    }
    closesocket(sock);
    WSACleanup();
	if (debug) cout << "\nsend success :" << str << "   to   " << ip << ":" << port << endl;
    return 0;
}

void Net::RecvStringLoop(unsigned short port) {
    while (isRecvStringRun)
    {
        api_sleep(100);
    }
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);
    SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    // 设置超时时间为1秒
    int timeout = 1;
    setsockopt(listenSock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
    setsockopt(listenSock, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(listenSock, (sockaddr*)&addr, sizeof(addr));
    listen(listenSock, 1);
    printf("[RecvStringLoop] Listening on port %d...\n", port);
    isRecvStringRun = true;
    while (!isExit_recvString) {
        sockaddr_in clientAddr{};
        int addrLen = sizeof(clientAddr);
        SOCKET clientSock = accept(listenSock, (sockaddr*)&clientAddr, &addrLen);
        if (clientSock == INVALID_SOCKET) continue;
        // 设置超时时间为1秒
        setsockopt(clientSock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
        setsockopt(clientSock, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
        char ipstr[32] = {0};
        inet_ntop(AF_INET, &clientAddr.sin_addr, ipstr, sizeof(ipstr));
        uint32_t strlen = 0;
        int ret = recv(clientSock, (char*)&strlen, sizeof(strlen), MSG_WAITALL);
        if (ret != sizeof(strlen) || strlen == 0) {
            closesocket(clientSock);
            continue;
        }
        std::vector<char> buf(strlen+1);
        ret = recv(clientSock, buf.data(), strlen, MSG_WAITALL);
        if (ret != strlen) {
            closesocket(clientSock);
            continue;
        }
        buf[strlen] = '\0';
        // 解密字符串
        decryptBuffer(buf, encryptModeRecv, encryptKeyRecv);
        std::string receivedStr(buf.begin(), buf.begin() + strlen);
        encryptedStringList.push_back(receivedStr);
        printf("[RecvStringLoop] From %s:%d, Length: %u, Content: %s\n", ipstr, ntohs(clientAddr.sin_port), strlen, receivedStr.c_str());
        closesocket(clientSock);
    }
    closesocket(listenSock);
    WSACleanup();
    isRecvStringRun = false;
}

void Net::startRecvStringThread(unsigned short port) {
    while (isRecvStringRun) std::this_thread::sleep_for(std::chrono::milliseconds(100));
    isExit_recvString = false;
    std::thread t(Net::RecvStringLoop, port);
    t.detach();
}

// 弹出并删除第一个接收到的文件名
std::wstring Net::popReceivedFileName() {
    if (receivedFileNameList.empty()) return L"NULL";
    std::wstring result = receivedFileNameList.front();
    receivedFileNameList.erase(receivedFileNameList.begin());
    return result;
}

// 弹出并删除第一个加密字符串
std::string Net::popEncryptedString() {
    if (encryptedStringList.empty()) return "NULL";
    std::string result = encryptedStringList.front();
    encryptedStringList.erase(encryptedStringList.begin());
    return result;
}

// 获取当前文件发送进度百分比（0-100）
int Net::GetTransferProgress() {
    std::lock_guard<std::mutex> lock(sendSpeedMutex);
    if (currentSendFileSize == 0) return 0;
    double percent = (double)currentSendFileSent * 100.0 / (double)currentSendFileSize;
    if (percent >= 100.0) percent = 100.0;
    if (percent < 0.0) percent = 0.0;
    return static_cast<int>(percent + 0.5); // 四舍五入
}

void Net::exit()
{
	isExit_send = isExit_recv = isExit_recvString = true;
    while (isRecvRun && !isSendRun && !isRecvStringRun)
    {
        api_sleep(100); 
    }
}