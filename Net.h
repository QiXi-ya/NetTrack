#pragma once
#include <string>
#include <vector>
#include <thread>
#include <mutex>

class Net {
public:

    // 加密/解密设置
    static void setEncryptKey(const std::string& key); // 设置加密密钥
    static void setDecryptKey(const std::string& key); // 设置解密密钥
    static void setEncryptMode(int mode); // 设置加密模式（0:不加密, 1:普通异或, 2:带密钥异或, 3:AES）
    static void setDecryptMode(int mode); // 设置解密模式（0:不加密, 1:普通异或, 2:带密钥异或, 3:AES）

    // 线程控制
    static void startSendThread(std::string ip, unsigned short port); // 启动发送线程
    static void stopSendThread(); // 停止发送线程
    static void startRecvThread(unsigned short port, const std::string& save_dir); // 启动接收线程
    static void stopRecvThread(); // 停止接收线程

    // 文件与字符串传输
    static void SendFile(const std::string& ip, unsigned short port, const std::string& filepath); // 发送文件到指定IP和端口
    static void RecvFile(unsigned short port, const std::string& save_dir); // 持续接收文件
    // 发送加密字符串，返回错误码（0成功，其他见NetErrorTable）
    static int SendXorString(const std::string ip, unsigned short port, const std::string str); // 发送加密字符串（使用当前加密模式和密钥）
    static void RecvStringLoop(unsigned short port); // 持续接收字符串详细
	static void startRecvStringThread(unsigned short port); // 启动接收字符串线程

    // 文件夹打包与解包
    static bool packFolder(const std::string& folderPath, const std::string& outputFile); // 打包文件夹为单一文件
    static std::string unpackFolder(const std::string& inputFile, const std::string& outputFolder); // 解包文件夹

    // 工具与状态
    static std::string get_filename_only(const std::string& path); // 获取文件名（不含路径）
    static std::string GetSendSpeedString(); // 获取当前发送速度字符串（KB/s, MB/s）
    static std::string GetSendRemainTimeString(); // 获取当前文件发送剩余时间字符串（s, m, h）
    static std::string getSendStatus(); // 获取当前发送状态（SEND, LOCK, ERROR）
	static int GetTransferProgress(); //获取当前传输进度百分比（0 - 100）

    // 弹出并删除第一个接收到的文件名
    static std::wstring popReceivedFileName(); // 返回并删除第一个文件名，没有则返回"NULL"
    // 追加函数：弹出并删除第一个加密字符串
    static std::string popEncryptedString(); // 返回并删除第一个加密字符串，没有则返回"NULL"
    // 返回用户下载文件夹位置
    static std::wstring GetDownloadFolderPath(); // 获取下载文件夹路径
    static bool CheckInternetConnection(); // 检查网络连接状态
    static std::string get_local_ip(); // 获取设备IP
    //退出所有
    static void exit();
    static void setSendSpeedLimitMB(int limit); // 设置发送速度上限（MB/s），0表示不限制

private:
    // 加密/解密辅助
    static void encryptBuffer(std::vector<char>& buf, int mode, const std::string& key); // 按模式加密缓冲区
    static void decryptBuffer(std::vector<char>& buf, int mode, const std::string& key); // 按模式解密缓冲区
    static void aesEncrypt(std::vector<char>& buf, const std::string& key); // 使用AES加密缓冲区
    static void aesDecrypt(std::vector<char>& buf, const std::string& key); // 使用AES解密缓冲区


    static void SendWorker(std::string ip, unsigned short port); // 发送线程工作函数
    static void RecvWorker(unsigned short port, const std::string& save_dir); // 接收线程工作函数
};
