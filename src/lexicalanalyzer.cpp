#include "lexicalanalyzer.h"
#include <sstream>
#include <cctype>
#include <unordered_set>
#include <regex>
#include <unordered_map>

bool isKeyword(const std::string& word) {
    std::unordered_set<std::string> keywords = {
        "if", "else", "switch", "case", "default", "for", "while", "do", "break", "continue", "goto", "main", "return",
        "int", "char", "float", "double", "void", "_Bool",
        "short", "long", "signed", "unsigned",
        "auto", "static", "extern", "register",
        "const", "volatile", "restrict",
        "struct", "union", "enum", "typedef",
        "sizeof", "inline", "_Alignas", "_Alignof", "_Atomic", "_Generic", "_Noreturn", "_Static_assert", "_Thread_local",
        "printf", "scanf", "gets", "puts", "fgets", "fputs",
        "malloc", "calloc", "realloc", "free",
        "exit", "atoi", "atof", "strlen", "strcpy", "strncpy", "strcat", "strcmp",
        "fopen", "fclose", "fread", "fwrite", "fprintf", "fscanf"

    };
    return keywords.count(word) > 0;
}

bool isNumber(const std::string& word) {
    std::regex numberRegex(R"(^\d+(\.\d+)?$)");
    return std::regex_match(word, numberRegex);
}

bool isIdentifier(const std::string& word) {
    if (word.empty() || isKeyword(word)) return false;
    if (!std::isalpha(word[0]) && word[0] != '_') return false;
    for (size_t i = 1; i < word.size(); ++i) {
        if (!std::isalnum(word[i]) && word[i] != '_') return false;
    }
    return true;
}

std::pair<std::string, std::string> getOperatorCategory(const std::string& op) {
    static std::unordered_map<std::string, std::string> operatorMap = {
        {"+", "Arithmetic Plus"}, {"-", "Arithmetic Minus"}, {"*", "Arithmetic Multiply"},
        {"/", "Arithmetic Divide"}, {"%", "Arithmetic Modulus"}, {"++", "Arithmetic Increment"},
        {"--", "Arithmetic Decrement"},

        {"==", "Relational Equal to"}, {"!=", "Relational Not equal to"},
        {">", "Relational Greater than"}, {"<", "Relational Less than"},
        {">=", "Relational Greater than or equal to"}, {"<=", "Relational Less than or equal to"},

        {"&&", "Logical AND"}, {"||", "Logical OR"}, {"!", "Logical NOT"},

        {"=", "Assignment"}, {"+=", "Assignment Add and assign"}, {"-=", "Assignment Subtract and assign"},
        {"*=", "Assignment Multiply and assign"}, {"/=", "Assignment Divide and assign"}, {"%=", "Assignment Modulus and assign"},

        {"&", "Bitwise AND"}, {"|", "Bitwise OR"}, {"^", "Bitwise XOR"},
        {"~", "Bitwise NOT"}, {"<<", "Bitwise Left shift"}, {">>", "Bitwise Right shift"}
    };


    auto it = operatorMap.find(op);
    if (it != operatorMap.end()) {
        return {"Operator:" + op, it->second};
    }
    return {"Operator:" + op, "Unknown"};
}

std::string getDelimiterType(char ch) {
    switch (ch) {
    case ';': return "Semicolon";
    case ',': return "Comma";
    case '(': return "Left Parenthesis";
    case ')': return "Right Parenthesis";
    case '{': return "Left Brace";
    case '}': return "Right Brace";
    case '[': return "Left Bracket";
    case ']': return "Right Bracket";
    default: return "Unknown";
    }
}

std::vector<std::pair<std::string, std::string>> LexicalAnalyzer::analyze(const std::string& input) {
    std::vector<std::pair<std::string, std::string>> tokens;
    std::string word = "";

    auto pushWordToken = [&](const std::string& w) {
        if (isKeyword(w)) {
            tokens.push_back({w, "Keyword:" + w});
        } else if (isNumber(w)) {
            tokens.push_back({w, "Number:" + w});
        } else if (isIdentifier(w)) {
            tokens.push_back({w, "Identifier:" + w});
        } else {
            tokens.push_back({w, "Unknown"});
        }
    };

    for (size_t i = 0; i < input.length(); ++i) {
        char ch = input[i];

        if (std::isspace(ch)) {
            if (!word.empty()) {
                pushWordToken(word);
                word.clear();
            }
        }
        else if (ispunct(ch)) {
            if (!word.empty()) {
                pushWordToken(word);
                word.clear();
            }

            std::string op(1, ch);
            // Check for multi-character operator
            if (i + 1 < input.length()) {
                std::string twoChar = op + input[i + 1];
                auto [cat, subcat] = getOperatorCategory(twoChar);
                if (subcat != "Unknown") {
                    tokens.push_back({twoChar, cat + ", " + subcat});
                    ++i; // Skip next character
                    continue;
                }
            }

            // Single character operator
            auto [cat, subcat] = getOperatorCategory(op);
            if (subcat != "Unknown") {
                tokens.push_back({op, cat + ", " + subcat});
            } else {
                std::string delimiterType = getDelimiterType(ch);
                if (delimiterType != "Unknown") {
                    tokens.push_back({op, "Delimiter:" + delimiterType});
                } else {
                    tokens.push_back({op, "Unknown"});
                }
            }
        }
        else {
            word += ch;
        }
    }

    if (!word.empty()) {
        pushWordToken(word);
    }

    return tokens;
}
