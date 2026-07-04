#include <iostream>
#include <fstream> 
#include <string>
#include <sstream>
#include <optional>
#include <vector>
#include "./tokenization.hpp"
#include "./parser.hpp"
#include "./generation.hpp"







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
   Parser parser(std::move(tokens));
   std::optional<NodeExit> tree = parser.parse();
   if(!tree.has_value()){
     std::cerr<<"No exit statement found"<<std::endl;
     exit(EXIT_FAILURE);
   }
   Generator generator(tree.value());

   {
    std::fstream file("out.asm",std::ios::out
    );
    file << generator.generate();
   }
   system("nasm -f elf64 out.asm -o out.o");
   system("ld -o out out.o");

    
    return 0;
}