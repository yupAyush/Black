#include <iostream>
#include <fstream> 
#include <string>
#include <sstream>
#include <optional>
#include <vector>

enum class TokenType {
        _return ,
        int_lit ,
        semi
};

struct Token {
        TokenType type;
        std::optional<std::string> value {}; // this value may or may not exists

};
std::vector<Token> tokenize(const std::string& str){
    std::string buff;
    std::vector<Token> tokens ;
   for(int i=0;i<str.length();i++){
      char c = str.at(i);
      if(std::isalpha(c)){
          buff.push_back(c);
          i++;
          while(i<str.length() && std::isalnum(str.at(i))){
            buff.push_back(str.at(i));
            i++;
          }
          i--;
          if(buff=="return"){
            tokens.push_back(Token{TokenType::_return});
            buff.clear();
            continue;
          }else{
            std:: cerr<<"You messed up!"<<std::endl;
            exit( EXIT_FAILURE);
          }

      }
      else if(std::isdigit(c)){
        buff.push_back(c);            
        i++;
        while(i < str.length() &&std::isdigit(str.at(i))){
            buff.push_back(str.at(i));
            i++;
        }
        i--;
        tokens.push_back(Token{TokenType::int_lit,buff});
        buff.clear();

      }
      else if(c==';'){
        tokens.push_back(Token{TokenType::semi});
      }
      else if(std::isspace(c)){
        continue;
      }else {
        std:: cerr<<"You messed up!"<<std::endl;
        exit( EXIT_FAILURE);
      }
     
   }

   return tokens ;
};


std::string tokens_to_asm(const std::vector<Token>& tokens ){
  std::stringstream output ;
  output <<"global _start\n_start:\n";
  for(int i=0;i<tokens.size();i++){
    const Token& token = tokens.at(i);
    if(token.type==TokenType::_return){
       if(i+1 <tokens.size() && tokens.at(i+1).type==TokenType::int_lit){
        if(i+2 <tokens.size() && tokens.at(i+2).type==TokenType::semi){
          output <<"   mov rax, 60\n";
          output <<"   mov rdi, "<<tokens.at(i+1).value.value()<<"\n";
          output <<"   syscall";
        }
       }

    }


  }
  return output.str();

}

int main(int argc,char* argv[]){

   


    if(argc!=2) {
        std::cerr<<"Incorrect input"<<std::endl;
        std::cerr<<"black <input.bk>"<<std::endl;
        return EXIT_FAILURE;
    }
    
   std::string contents;
   {
        std::fstream input(argv[1],std::ios::in);
        std::stringstream contents_stream ; //empty string buffer
        contents_stream << input.rdbuf(); //dumpinng file into the string buffer
        contents=contents_stream.str(); // converts string buffer into string 

        
   } // it closes autmatically 

   std::vector<Token> tokens=tokenize(contents);
   
  
  
   std::cout<<tokens_to_asm(tokens);

   {
    std::fstream file("out.asm",std::ios::out
    );
    file << tokens_to_asm(tokens);
   }
   system("nasm -f elf64 out.asm -o out.o");
   system("ld -o out out.o");

    
    return 0;
}