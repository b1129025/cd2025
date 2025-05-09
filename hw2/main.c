#include <stdio.h>
#include <string.h>

// 定義 Token 類型
#define NUMBER_TOKEN 1
#define PLUS_TOKEN 2
#define LPAREN_TOKEN 3
#define RPAREN_TOKEN 4
#define EOF_TOKEN 5

// 掃描器狀態
typedef struct {
    int token;
    int value;
} Scanner;

Scanner scanner;

// 函數原型
void parse_S();
void parse_S_Prime();
void parse_E();

// 錯誤處理
void parseError() {
    printf("解析錯誤！\n");
    exit(1);
}

// 掃描器函數
void getNextToken() {
    char c;
    
    // 跳過空白字符
    do {
        c = getchar();
    } while (c == ' ' || c == '\t' || c == '\n');
    
    if (c >= '0' && c <= '9') {
        // 數字 token
        int num = 0;
        do {
            num = num * 10 + (c - '0');
            c = getchar();
        } while (c >= '0' && c <= '9');
        ungetc(c, stdin);
        
        scanner.token = NUMBER_TOKEN;
        scanner.value = num;
    } else if (c == '+') {
        scanner.token = PLUS_TOKEN;
    } else if (c == '(') {
        scanner.token = LPAREN_TOKEN;
    } else if (c == ')') {
        scanner.token = RPAREN_TOKEN;
    } else if (c == EOF) {
        scanner.token = EOF_TOKEN;
    } else {
        printf("無效字符: %c\n", c);
        parseError();
    }
}

// 解析函數，基於圖片中的文法：
// S → ES'
// S' → ε
// S' → +S
// E → num
// E → (S)

void parse_S() {
    // S → ES'
    parse_E();
    parse_S_Prime();
}

void parse_S_Prime() {
    // S' → ε | +S
    if (scanner.token == PLUS_TOKEN) {
        // S' → +S
        getNextToken();
        parse_S();
    }
    // 否則 S' → ε (不做任何事)
}

void parse_E() {
    // E → num | (S)
    if (scanner.token == NUMBER_TOKEN) {
        // E → num
        printf("發現數字: %d\n", scanner.value);
        getNextToken();
    } else if (scanner.token == LPAREN_TOKEN) {
        // E → (S)
        getNextToken();
        parse_S();
        if (scanner.token == RPAREN_TOKEN) {
            getNextToken();
        } else {
            printf("預期右括號\n");
            parseError();
        }
    } else {
        printf("預期數字或左括號\n");
        parseError();
    }
}

int main() {
    printf("請輸入表達式 (例如: 1+2+(3+4)): ");
    
    // 初始化掃描器，讀取第一個 token
    getNextToken();
    
    // 開始解析
    parse_S();
    
    // 檢查是否已消耗所有輸入
    if (scanner.token == EOF_TOKEN) {
        printf("解析成功！\n");
    } else {
        printf("表達式後有多餘輸入\n");
        parseError();
    }
    
    return 0;
}