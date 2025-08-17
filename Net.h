#pragma once
#include <string>
#include <vector>
#include <thread>
#include <mutex>

class Net {
public:

    // ����/��������
    static void setEncryptKey(const std::string& key); // ���ü�����Կ
    static void setDecryptKey(const std::string& key); // ���ý�����Կ
    static void setEncryptMode(int mode); // ���ü���ģʽ��0:������, 1:��ͨ���, 2:����Կ���, 3:AES��
    static void setDecryptMode(int mode); // ���ý���ģʽ��0:������, 1:��ͨ���, 2:����Կ���, 3:AES��

    // �߳̿���
    static void startSendThread(std::string ip, unsigned short port); // ���������߳�
    static void stopSendThread(); // ֹͣ�����߳�
    static void startRecvThread(unsigned short port, const std::string& save_dir); // ���������߳�
    static void stopRecvThread(); // ֹͣ�����߳�

    // �ļ����ַ�������
    static void SendFile(const std::string& ip, unsigned short port, const std::string& filepath); // �����ļ���ָ��IP�Ͷ˿�
    static void RecvFile(unsigned short port, const std::string& save_dir); // ���������ļ�
    // ���ͼ����ַ��������ش����루0�ɹ���������NetErrorTable��
    static int SendXorString(const std::string ip, unsigned short port, const std::string str); // ���ͼ����ַ�����ʹ�õ�ǰ����ģʽ����Կ��
    static void RecvStringLoop(unsigned short port); // ���������ַ�����ϸ
	static void startRecvStringThread(unsigned short port); // ���������ַ����߳�

    // �ļ��д������
    static bool packFolder(const std::string& folderPath, const std::string& outputFile); // ����ļ���Ϊ��һ�ļ�
    static std::string unpackFolder(const std::string& inputFile, const std::string& outputFolder); // ����ļ���

    // ������״̬
    static std::string get_filename_only(const std::string& path); // ��ȡ�ļ���������·����
    static std::string GetSendSpeedString(); // ��ȡ��ǰ�����ٶ��ַ�����KB/s, MB/s��
    static std::string GetSendRemainTimeString(); // ��ȡ��ǰ�ļ�����ʣ��ʱ���ַ�����s, m, h��
    static std::string getSendStatus(); // ��ȡ��ǰ����״̬��SEND, LOCK, ERROR��
	static int GetTransferProgress(); //��ȡ��ǰ������Ȱٷֱȣ�0 - 100��

    // ������ɾ����һ�����յ����ļ���
    static std::wstring popReceivedFileName(); // ���ز�ɾ����һ���ļ�����û���򷵻�"NULL"
    // ׷�Ӻ�����������ɾ����һ�������ַ���
    static std::string popEncryptedString(); // ���ز�ɾ����һ�������ַ�����û���򷵻�"NULL"
    // �����û������ļ���λ��
    static std::wstring GetDownloadFolderPath(); // ��ȡ�����ļ���·��
    static bool CheckInternetConnection(); // �����������״̬
    static std::string get_local_ip(); // ��ȡ�豸IP
    //�˳�����
    static void exit();
    static void setSendSpeedLimitMB(int limit); // ���÷����ٶ����ޣ�MB/s����0��ʾ������

private:
    // ����/���ܸ���
    static void encryptBuffer(std::vector<char>& buf, int mode, const std::string& key); // ��ģʽ���ܻ�����
    static void decryptBuffer(std::vector<char>& buf, int mode, const std::string& key); // ��ģʽ���ܻ�����
    static void aesEncrypt(std::vector<char>& buf, const std::string& key); // ʹ��AES���ܻ�����
    static void aesDecrypt(std::vector<char>& buf, const std::string& key); // ʹ��AES���ܻ�����


    static void SendWorker(std::string ip, unsigned short port); // �����̹߳�������
    static void RecvWorker(unsigned short port, const std::string& save_dir); // �����̹߳�������
};
