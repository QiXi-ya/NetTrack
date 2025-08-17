#define  _CRT_SECURE_NO_WARNINGS
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <string>
#include <array>
#include <vector>
#include "NetCode.h"

using namespace std;


    // 编码：将IP和端口编码为8位Base64字符串
    string code::encode_ip_port(const string& ip, unsigned short port) {
        // 解析IP地址
        int a = 0, b = 0, c = 0, d = 0;
        if (sscanf(ip.c_str(), "%d.%d.%d.%d", &a, &b, &c, &d) != 4) return "";

        // 6字节数据：4字节IP + 2字节端口
        array<unsigned char, 6> data = {
            static_cast<unsigned char>(a),
            static_cast<unsigned char>(b),
            static_cast<unsigned char>(c),
            static_cast<unsigned char>(d),
            static_cast<unsigned char>((port >> 8) & 0xFF),
            static_cast<unsigned char>(port & 0xFF)
        };

        // Base64编码表
        constexpr char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        string out;
        int val = 0, valb = -6;
        for (unsigned char byteOS : data) {
            val = (val << 8) | byteOS;
            valb += 8;
            while (valb >= 0) {
                out.push_back(base64_chars[(val >> valb) & 0x3F]);
                valb -= 6;
            }
        }
        if (valb > -6)
            out.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
        while (out.size() < 8)
            out.push_back('='); // 补齐8位
        return out;
    }

    // 解码：将8位Base64字符串还原为IP和端口
    pair<string, unsigned short> code::decode_ip_port(const string& codeK) {
        // 只取前8位
        string code = codeK.substr(0, 8);

        // Base64解码表
        int T[256] = { 0 };
        constexpr char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        for (int i = 0; i < 64; ++i) T[(unsigned char)base64_chars[i]] = i;

        int val = 0, valb = -8;
        vector<unsigned char> out;
        for (char c : code) {
            if (c == '=') break;
            val = (val << 6) | T[(unsigned char)c];
            valb += 6;
            if (valb >= 0) {
                out.push_back((unsigned char)((val >> valb) & 0xFF));
                valb -= 8;
            }
        }
        if (out.size() != 6) return { "ERROR", 0 };
        char ipbuf[16];
        snprintf(ipbuf, sizeof(ipbuf), "%d.%d.%d.%d", out[0], out[1], out[2], out[3]);
        unsigned short port = (static_cast<unsigned short>(out[4]) << 8) | out[5];
        return { string(ipbuf), port };
    }
