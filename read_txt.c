#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINES 1024
#define MAX_LINE_LEN 256

// 去掉前后空格
void trim(char *str) {
    int start = 0, end = strlen(str) - 1;

    while (isspace((unsigned char)str[start])) start++;
    while (end >= start && isspace((unsigned char)str[end])) end--;

    if (start > 0 || end < (int)strlen(str) - 1) {
        memmove(str, str + start, end - start + 1);
        str[end - start + 1] = '\0';
    }
}

// 转换为小写（原地）
void to_lowercase(char *str) {
    while (*str) {
        *str = tolower((unsigned char)*str);
        str++;
    }
}

// 判断是否重复（忽略大小写）
int is_duplicate(char lines[][MAX_LINE_LEN], int count, const char *new_line) {
    char lower1[MAX_LINE_LEN], lower2[MAX_LINE_LEN];
    strcpy(lower2, new_line);
    to_lowercase(lower2);

    for (int i = 0; i < count; ++i) {
        strcpy(lower1, lines[i]);
        to_lowercase(lower1);
        if (strcmp(lower1, lower2) == 0) {
            return 1; // 重复
        }
    }
    return 0;
}

int main() {
    FILE *fp = fopen("input.txt", "r");
    if (!fp) {
        perror("无法打开文件 input.txt");
        return 1;
    }

    char raw_line[MAX_LINE_LEN];
    char lines[MAX_LINES][MAX_LINE_LEN];
    int count = 0;

    while (fgets(raw_line, sizeof(raw_line), fp)) {
        trim(raw_line);
        if (strlen(raw_line) == 0) continue; // 跳过空行

        if (!is_duplicate(lines, count, raw_line)) {
            strncpy(lines[count], raw_line, MAX_LINE_LEN - 1);
            lines[count][MAX_LINE_LEN - 1] = '\0';
            count++;
        }

        if (count >= MAX_LINES) break; // 防止数组溢出
    }

    fclose(fp);

    printf("处理后的内容：\n");
    for (int i = 0; i < count; ++i) {
        printf("%s\n", lines[i]);
    }

    return 0;
}

