#include "Main.h"
#include "code.h"
#include <cstddef>
#include <filesystem>

using namespace std;
using ::byte;

// 将字符串向量连接成单个字符串
string StringVectorConverter::join(const vector<string>& vec, char delimiter = '\x1F') {
    if (vec.empty()) return "";

    string result;
    // 预分配空间提高性能
    size_t total_size = 0;
    for (const auto& s : vec) {
        total_size += s.size();
    }
    total_size += vec.size() - 1; // 分隔符数量
    result.reserve(total_size);

    // 拼接字符串
    for (size_t i = 0; i < vec.size(); ++i) {
        if (i != 0) {
            result += delimiter;
        }
        result += vec[i];
    }

    return result;
}

// 将字符串分割成字符串向量
vector<string> StringVectorConverter::split(const string& str, char delimiter = '\x1F') {
    vector<string> result;
    if (str.empty()) return result;

    istringstream iss(str);
    string token;
    while (getline(iss, token, delimiter)) {
        if (!token.empty()) {  // 跳过空字符串
            result.push_back(move(token));
        }
    }

    return result;
}