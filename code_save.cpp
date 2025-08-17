#include "Main.h"
#include "code.h"
#include <cstddef>
#include <filesystem>

using namespace std;
using ::byte;

// ���ַ����������ӳɵ����ַ���
string StringVectorConverter::join(const vector<string>& vec, char delimiter = '\x1F') {
    if (vec.empty()) return "";

    string result;
    // Ԥ����ռ��������
    size_t total_size = 0;
    for (const auto& s : vec) {
        total_size += s.size();
    }
    total_size += vec.size() - 1; // �ָ�������
    result.reserve(total_size);

    // ƴ���ַ���
    for (size_t i = 0; i < vec.size(); ++i) {
        if (i != 0) {
            result += delimiter;
        }
        result += vec[i];
    }

    return result;
}

// ���ַ����ָ���ַ�������
vector<string> StringVectorConverter::split(const string& str, char delimiter = '\x1F') {
    vector<string> result;
    if (str.empty()) return result;

    istringstream iss(str);
    string token;
    while (getline(iss, token, delimiter)) {
        if (!token.empty()) {  // �������ַ���
            result.push_back(move(token));
        }
    }

    return result;
}