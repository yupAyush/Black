#include <iostream>
#include <fstream> 
#include <string>
#include <sstream>
#include <optional>
#include <vector>
#include "./tokenization.hpp"





std::string tokens_to_asm(const std::vector<Token>& tokens ){
  std::stringstream output ;
  output <<"global _start\n_start:\n";
  for(int i=0;i<tokens.size();i++){
    const Token& token = tokens.at(i);
    if(token.type==TokenType::exit){
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
   Tokenizer tokenizer(std::move(contents));

   std::vector<Token> tokens=tokenizer.tokenize();
   
  
  
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