#pragma once
#include <string>
#include <array>
#include <vector>
using namespace std;
/// <summary>
/// ��λ�ַ�������
/// </summary>
class code
{
public:
    // ���룺��IP�Ͷ˿ڱ���Ϊ8λBase64�ַ���
    static string encode_ip_port(const string& ip, unsigned short port);

    // ���룺��8λBase64�ַ�����ԭΪIP�Ͷ˿�
    static pair<string, unsigned short> decode_ip_port(const string& codeK);
};