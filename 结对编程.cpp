#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <cctype>  // 确保包含 cctype 头文件，用于 ::tolower

struct AnalysisResult {
    int wordCount;
    int lineCount;
    int charCount;
    std::unordered_map<std::string, int> wordFrequency;
};

AnalysisResult analyzeText(const std::string& text) {
    AnalysisResult result = {0, 0, 0, {}};
    std::istringstream iss(text);
    std::string line;

    while (std::getline(iss, line)) {
        result.lineCount++;
        result.charCount += line.length() + 1; // 加上换行符
        std::istringstream lineStream(line);
        std::string word;
        while (lineStream >> word) {
            result.wordCount++;
            // 转换为小写以忽略大小写
            std::transform(word.begin(), word.end(), word.begin(), [](unsigned char c) { return std::tolower(c); });
            // 移除非字母字符
            word.erase(std::remove_if(word.begin(), word.end(), [](unsigned char c) { return !std::isalpha(c); }), word.end());
            if (!word.empty()) {
                result.wordFrequency[word]++;
            }
        }
    }

    return result;
}

int main() {
    system("chcp 65001"); 
    std::string filePath;
    std::cout << "请输入要分析的文本文件路径: ";
    std::cin >> filePath;

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "无法打开文件！" << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string text = buffer.str();

    AnalysisResult result = analyzeText(text);

    std::cout << "单词数量: " << result.wordCount << std::endl;
    std::cout << "行数: " << result.lineCount << std::endl;
    std::cout << "字符数量: " << result.charCount << std::endl;

    std::string targetWord;
    std::cout << "请输入要统计出现次数的单词 (输入 'q' 跳过): ";
    std::cin >> targetWord;
    if (targetWord != "q") {
        std::transform(targetWord.begin(), targetWord.end(), targetWord.begin(), [](unsigned char c) { return std::tolower(c); });
        // 移除非字母字符
        targetWord.erase(std::remove_if(targetWord.begin(), targetWord.end(), [](unsigned char c) { return !std::isalpha(c); }), targetWord.end());
        auto it = result.wordFrequency.find(targetWord);
        if (it != result.wordFrequency.end()) {
            std::cout << "单词 '" << targetWord << "' 出现的次数: " << it->second << std::endl;
        } else {
            std::cout << "单词 '" << targetWord << "' 未在文本中出现。" << std::endl;
        }
    }

    std::cout << "各单词出现频率统计：" << std::endl;
    for (const auto& pair : result.wordFrequency) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }

    file.close();
    return 0;
}    
