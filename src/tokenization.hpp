#pragma once
#include <string>
#include <vector>
#include <optional>

enum class TokenType {
    exit,
    int_lit,
    semi
};

struct Token {
    TokenType type;
    std::optional<std::string> value{};
};

class Tokenizer {
public:
    inline explicit Tokenizer(std::string src) : m_src(std::move(src)) {}
    //explicit prevents the compiler from doing automatic implicit conversions to your type.

    inline std::vector<Token> tokenize() {

        std::string buff;
        std::vector<Token> tokens;
        while(peak().has_value()){
           
            if(std::isalpha(peak().value())){
                buff.push_back(consume());
                while(peak().has_value() && std::isalnum(peak().value())){
                    buff.push_back(consume());


                }
                if(buff=="exit"){
                    tokens.push_back(Token{TokenType::exit});
                    buff.clear();
                    continue;
                }else {
                    std::cerr<<"You messed up"<<std::endl;
                    exit(EXIT_FAILURE);
                }

            }
            else if(std::isdigit(peak().value())){
                buff.push_back(consume());
                while(peak().has_value() && std::isdigit(peak().value())){
                    buff.push_back(consume());

                }
                tokens.push_back(Token{TokenType::int_lit,buff});
                buff.clear();
                continue;
            }
            else if(peak().value()==';'){
                consume();
                tokens.push_back(Token{TokenType::semi});

            }else if(std::isspace(peak().value())){
                consume();
                continue;
            }else {
                std::cerr<<"You messed up"<<std::endl;
                exit(EXIT_FAILURE);

            }

        }

        

        return tokens;
    }

private:
    std::optional<char> peak(int ahead=0) const {
        if(m_idx+ahead >=m_src.length()){
            return {};
        }else {
            return m_src.at(m_idx+ahead);
        }

    }

    char consume(){
            return m_src.at(m_idx++);
    }
    const std::string m_src;
    int m_idx=0;
};