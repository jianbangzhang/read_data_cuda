#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <algorithm>

// 抽象基类
class TextProcessor {
public:
    virtual void processLine(const std::string& line) = 0;
    virtual void printResult() const = 0;
    virtual ~TextProcessor() = default;
};

// 工具函数：字符串小写
std::string to_lower(const std::string& str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

// 工具函数：去前后空白
std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    return str.substr(start, end - start + 1);
}

// 具体实现类
class FileCleaner : public TextProcessor {
private:
    std::unordered_set<std::string> seen;
    std::vector<std::string> results;

public:
    void processLine(const std::string& line) override {
        std::string trimmed = trim(line);
        if (trimmed.empty()) return;

        std::string lowered = to_lower(trimmed);
        if (seen.find(lowered) == seen.end()) {
            seen.insert(lowered);
            results.push_back(trimmed);
        }
    }

    void printResult() const override {
        for (const auto& line : results) {
            std::cout << line << std::endl;
        }
    }

    // 读取文件并处理
    void loadFile(const std::string& filename) {
        std::ifstream infile(filename);
        if (!infile.is_open()) {
            std::cerr << "无法打开文件: " << filename << std::endl;
            return;
        }

        std::string line;
        while (std::getline(infile, line)) {
            processLine(line);
        }

        infile.close();
    }
};

// 主程序
int main() {
    std::string filename = "input.txt"; // 输入文件名
    FileCleaner cleaner;
    cleaner.loadFile(filename);
    std::cout << "处理后的内容：" << std::endl;
    cleaner.printResult();
    return 0;
}

