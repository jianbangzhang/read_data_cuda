#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <algorithm>

// 转换为小写
std::string to_lower(const std::string& str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

// 去除前后空白
std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    return str.substr(start, end - start + 1);
}

int main() {
    std::string filename = "input.txt"; // 输入文件名
    std::ifstream infile(filename);

    if (!infile.is_open()) {
        std::cerr << "无法打开文件：" << filename << std::endl;
        return 1;
    }

    std::unordered_set<std::string> seen_lower;
    std::string line;

    std::cout << "处理后的输出为：" << std::endl;

    while (std::getline(infile, line)) {
        std::string trimmed = trim(line);

        if (trimmed.empty()) continue; // 跳过空行

        std::string lowered = to_lower(trimmed);
        if (seen_lower.find(lowered) == seen_lower.end()) {
            seen_lower.insert(lowered);
            std::cout << trimmed << std::endl;
        }
    }

    infile.close();
    return 0;
}

