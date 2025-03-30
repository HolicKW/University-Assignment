#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <filesystem>
#include <map>
#include <set>

using namespace std;


map<string,string> iformat_opcode = {     //iformat 함수 매핑
    {"addi", "001000"},
    {"andi", "001100"},
    {"ori", "001101"},
    {"xori", "001110"},
    {"lw", "100011"},
    {"sw", "101011"},
    {"lui", "001111"},
    {"slti", "001010"},
    {"beq", "000100"},
    {"bne", "000101"},
    
};
map<string,string> rformat_function = {     //rformat 함수 매핑
    {"add", "100000"},
    {"sub", "100010"},
    {"and", "100100"},
    {"or", "100101"},
    {"xor", "100110"},
    {"nor", "100111"},
    {"slt", "101010"},
    {"sll", "000000"},
    {"srl", "000010"},
    {"jr", "001000"}
};

// 레지스터 주소 매핑
map<string, string> registers = {
    {"$zero", "00000"},  // $0
    {"$v0", "00010"},    // $2
    {"$v1", "00011"},    // $3
    {"$a0", "00100"},    // $4
    {"$a1", "00101"},    // $5
    {"$a2", "00110"},    // $6
    {"$a3", "00111"},    // $7
    {"$t0", "01000"},    // $8
    {"$t1", "01001"},    // $9
    {"$t2", "01010"},    // $10
    {"$t3", "01011"},    // $11
    {"$t4", "01100"},    // $12
    {"$t5", "01101"},    // $13
    {"$t6", "01110"},    // $14
    {"$t7", "01111"},    // $15
    {"$s0", "10000"},    // $16
    {"$s1", "10001"},    // $17
    {"$s2", "10010"},    // $18
    {"$s3", "10011"},    // $19
    {"$s4", "10100"},    // $20
    {"$s5", "10101"},    // $21
    {"$s6", "10110"},    // $22
    {"$s7", "10111"},    // $23
    {"$t8", "11000"},    // $24
    {"$t9", "11001"},    // $25
    {"$gp", "11100"},    // $28
    {"$sp", "11101"},    // $29
    {"$fp", "11110"},    // $30
    {"$ra", "11111"}     // $31
};

struct iformat{
    string opcode;
    string rs;
    string rt;
    string immediate;

    string immediate_to_binary(string immediate){
        string ret_binary = "0000000000000000";
        string binary = "";
        int immediate_int = stoi(immediate);
        for(int i = 0; i<16; i++){
            binary = to_string(immediate_int % 2) + binary;
            immediate_int /= 2;
            if(immediate_int == 0){
                break;
            }
        }
        int len = ret_binary.length() - binary.length();
        for(int i = 0; i<binary.length(); i++){
            ret_binary[i+len] = binary[i];
        }
        return ret_binary;
    }
    iformat(string opcode, string rs, string rt, string immediate){

        this->opcode = iformat_opcode[opcode];
        this->rs = registers[rs];
        this->rt = registers[rt];
        this->immediate = immediate_to_binary(immediate);
    }

    string to_binary(){
        string binary = opcode + rs + rt + immediate;
        return binary;
    }

};

struct rformat{
    string opcode;
    string rs;
    string rt;
    string rd;
    string shamt;
    string funct;

    rformat(string opcode, string rs, string rt, string rd, string shamt, string funct){
        this->opcode = opcode;
        this->rs = registers[rs];
        this->rt = registers[rt];
        this->rd = registers[rd];
        this->shamt = shamt;
        this->funct = rformat_function[funct];
    }

    string to_binary(){
        string binary = opcode + rs + rt + rd + shamt + funct;
        return binary;
    }
};

vector<string> tokenize(string line) {  // line별로 토큰화 하는 함수
    vector<string> tokens;
    string token;

    for(size_t i = 0; i < line.size(); i++) {
        char ch = line[i];
        if(line[i] == '#') {
            break;
        }

        if(isspace(ch) || ch == ',') {
            if(!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }

        }
        else{
            token += ch;
        }
    }
    if(!token.empty()) {
        tokens.push_back(token);
    }
    return tokens;
}

void parse_line(ifstream& file, ofstream& output) {
    string line;
    while (getline(file, line)) {
        vector<string> tokens = tokenize(line);
        string binary_inst;
        for(int i = 0; i<tokens.size(); i++) {
            if(iformat_opcode.find(tokens[i]) != iformat_opcode.end()) {
                iformat inst = iformat(tokens[i], tokens[i+2], tokens[i+1], tokens[i+3]); // opcode, rs, rt, immediate
                binary_inst = inst.to_binary();
            }   
            else if(rformat_function.find(tokens[i]) != rformat_function.end()) {
                rformat inst = rformat("000000", tokens[i+2], tokens[i+3], tokens[i+1],"00000",tokens[i]); // opcode, rs, rt, rd, shamt, funct
                binary_inst = inst.to_binary();
            }
        }
        
        output << binary_inst << endl;
    }   
}

int main() {
    /*filesystem::current_path(filesystem::current_path());
    cout << "현재 작업 디렉토리: " << filesystem::current_path() << endl;*/
    cout << "현재 작업 디렉토리: " << filesystem::current_path() << endl;
    ifstream file("input.asm");

    if (!file.is_open()) {
        cerr << "파일을 열 수 없습니다: " << endl;
        return 1;
    }
    ofstream output("output.bin");
    parse_line(file, output);
    file.close();

    return 0;
}