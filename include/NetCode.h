#pragma once
#include <string>
#include <array>
#include <vector>
using namespace std;
/// <summary>
/// 八位字符编码类
/// </summary>
class code
{
public:
    // 编码：将IP和端口编码为8位Base64字符串
    static string encode_ip_port(const string& ip, unsigned short port);

    // 解码：将8位Base64字符串还原为IP和端口
    static pair<string, unsigned short> decode_ip_port(const string& codeK);
};