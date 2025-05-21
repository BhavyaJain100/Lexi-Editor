#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <string>
#include <vector>
#include <utility>

class LexicalAnalyzer {
public:
    static std::vector<std::pair<std::string, std::string>> analyze(const std::string& input);
};

#endif // LEXICALANALYZER_H
