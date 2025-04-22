#include <cuda_runtime.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cstring>

#define MAX_LINES 1024
#define MAX_LINE_LENGTH 256

// 转小写工具（设备端）
__device__ void to_lower(char* str) {
    for (int i = 0; i < MAX_LINE_LENGTH; i++) {
        char c = str[i];
        if (c == '\0') break;
        if (c >= 'A' && c <= 'Z') {
            str[i] = c + 32;
        }
    }
}

// 字符串比较（设备端）
__device__ bool str_equal(const char* a, const char* b) {
    for (int i = 0; i < MAX_LINE_LENGTH; ++i) {
        if (a[i] != b[i]) return false;
        if (a[i] == '\0') return true;
    }
    return true;
}

// 核函数：并行判断是否重复
__global__ void mark_unique(char lines[][MAX_LINE_LENGTH], int line_count, bool* keep_flags) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx >= line_count) return;

    // 跳过空行
    if (lines[idx][0] == '\0') {
        keep_flags[idx] = false;
        return;
    }

    // 转为小写（就地）
    to_lower(lines[idx]);

    // 和前面所有行比较
    for (int i = 0; i < idx; ++i) {
        if (lines[i][0] == '\0') continue;
        if (str_equal(lines[i], lines[idx])) {
            keep_flags[idx] = false;
            return;
        }
    }

    keep_flags[idx] = true;
}

int main() {
    // 读取文件
    std::ifstream infile("input.txt");
    std::vector<std::string> host_lines;
    std::string line;

    while (std::getline(infile, line) && host_lines.size() < MAX_LINES) {
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        if (!line.empty()) {
            host_lines.push_back(line.substr(0, MAX_LINE_LENGTH - 1));
        }
    }
    infile.close();
    int line_count = host_lines.size();

    // 拷贝数据到平面内存
    char h_lines[MAX_LINES][MAX_LINE_LENGTH] = {0};
    for (int i = 0; i < line_count; ++i) {
        std::strncpy(h_lines[i], host_lines[i].c_str(), MAX_LINE_LENGTH - 1);
    }

    // 分配 GPU 内存
    char (*d_lines)[MAX_LINE_LENGTH];
    bool* d_keep;
    cudaMalloc(&d_lines, sizeof(h_lines));
    cudaMalloc(&d_keep, sizeof(bool) * line_count);

    // 拷贝到设备
    cudaMemcpy(d_lines, h_lines, sizeof(h_lines), cudaMemcpyHostToDevice);

    // 启动核函数
    int threadsPerBlock = 256;
    int blocks = (line_count + threadsPerBlock - 1) / threadsPerBlock;
    mark_unique<<<blocks, threadsPerBlock>>>(d_lines, line_count, d_keep);

    // 拷贝结果回主机
    bool h_keep[MAX_LINES];
    cudaMemcpy(h_keep, d_keep, sizeof(bool) * line_count, cudaMemcpyDeviceToHost);

    // 输出
    std::cout << "去重并忽略大小写后的结果：" << std::endl;
    for (int i = 0; i < line_count; ++i) {
        if (h_keep[i]) {
            std::cout << host_lines[i] << std::endl;
        }
    }

    // 清理
    cudaFree(d_lines);
    cudaFree(d_keep);

    return 0;
}

