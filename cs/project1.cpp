#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <filesystem>
#include <map>
#include <set>
#include <cmath>

using namespace std;
int address = 0; //주소 
map<string,int> label_address; // label의 주소 매핑
map<string,string> iformat_opcode = {     //iformat 연산 코드 매핑
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
    {"mul", "000010"},
    {"jr", "001000"}
};

map<string,string> jformat_opcode = {   //jformat 연산 코드 매핑     
    {"j", "000010"},
    {"jal", "000011"}
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

struct iformat{         // iformat 형태
    string opcode;
    string rs;
    string rt;
    string immediate;

    string immediate_to_binary(string immediate){ 
        int immediate_int;
        if(immediate[0] == '0' && (immediate[1] == 'x' || immediate[1] == 'X')){ //16진수 인지 판별하기 
            immediate_int = stoi(immediate, nullptr, 16);
        }
        else immediate_int = stoi(immediate);
        string ret_binary = "0000000000000000";
        string binary = "";
        
        if(immediate_int < 0) {
            immediate_int = (1 << 16) + immediate_int; // 음수 처리 (2의 보수)
        }

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
        if(label_address.find(immediate) != label_address.end()){  //immediate가 label일 경우와 상수일 경우를 구분한다.
            this->immediate = label_address[immediate];
        }
        this->immediate = immediate_to_binary(immediate);
    }

    string to_binary(){
        string binary = opcode + rs + rt + immediate;
        return binary;
    }

};

struct rformat{     // rformat 형태
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
        if(shamt != "00000"){
            this -> shamt = shamt_to_binary(shamt);
        }
        else{
            this->shamt = shamt;
        }
        this->funct = rformat_function[funct];
    }

    string shamt_to_binary(string shamt){ // 인자로 들어온 shamt를 2진수 형태로 변환
        string ret_binary = "00000";
        string binary = ""; 
        int immediate_int = stoi(shamt); 
        immediate_int = pow(2,immediate_int);
        for(int i = 0; i<5; i++){
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
    string to_binary(){
        string binary = opcode + rs + rt + rd + shamt + funct;
        return binary;
    }
};

struct jformat{
    string opcode;
    string address;

    jformat(string opcode, string address){
        this->opcode = jformat_opcode[opcode];
        this->address = address_to_binary(address);
    }
    string address_to_binary(string label){
        string ret_binary = "00000000000000000000000000"; // 26비트
        if(label_address.find(label) != label_address.end()){
            int label_int = label_address[label]/4;
            string binary = "";
            for(int i = 0; i<26; i++){
                binary = to_string(label_int % 2) + binary;
                label_int /= 2;
            }
            int len = ret_binary.length() - binary.length();
            for(int i = 0; i<binary.length(); i++){
                ret_binary[i+len] = binary[i];
            }

        }
        return ret_binary;
        
    }
    string to_binary(){
        string binary = opcode + address;
        return binary;
    }
};

vector<string> tokenize(string line) {  // line별로 토큰화 하는 함수
    vector<string> tokens;
    string token;

    for(size_t i = 0; i < line.size(); i++) {
        char ch = line[i];
        if(ch == '#') {
            break;
        }

        if(isspace(ch) || ch == ','|| ch == '('|| ch == ')') {
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
    string prev_label;
    while (getline(file, line)) {
        vector<string> tokens = tokenize(line);
        if(tokens.size() == 0){     //공백일 경우 스킵
            continue;
        }
        for(int i = 0; i < tokens.size(); i++) {
            if(tokens[i].back() == ':') {  // 레이블 주소
                string label = tokens[i].substr(0, tokens[i].size() - 1); // ':' 제거x
                label_address[label] = address;
                prev_label = label;
                //cout << label << " " << address << endl;
            }
        }
        if(tokens.size() == 1){ // token size가 1일경우 계산 스킵 
            continue;
        }
        address += 4;
    } 
    // 두번째 탐색 과정
    address = 0;
    file.clear();
    file.seekg(0);
    while (getline(file, line)) {
        vector<string> tokens = tokenize(line);
        
        string binary_inst;
        if((tokens.size() == 1 && tokens[0].back() == ':') || tokens.size() == 0){    //label만 있을경우 스킵
            continue;
        } 
        for(int i = 0; i<tokens.size(); i++) {
            if(tokens[i].back() == ':'){
                string label = tokens[0].substr(0, tokens[0].size() - 1);
                if(label_address[label] <= address){
                address = label_address[label];
                continue;
            }
            }  //label : opcode~~형태
            //iformat 처리
            if(iformat_opcode.find(tokens[i]) != iformat_opcode.end()) {
                if(tokens[i] == "lw" || tokens[i] == "sw"){
                    iformat inst = iformat(tokens[i], tokens[i+3], tokens[i+1], tokens[i+2]); // opcode, rs, rt, immediate
                    binary_inst = inst.to_binary(); // 
                }
                else if(tokens[i] == "bne" || tokens[i] == "beq"){
                    iformat inst = iformat(tokens[i], tokens[i+1], tokens[i+2], to_string((abs((label_address[tokens[i+3]] - address))/4)-1));
                    binary_inst = inst.to_binary();
                }
                else if(tokens[i] == "lui"){
                    iformat inst = iformat(tokens[i], "$zero", tokens[i+1], tokens[i+2]);
                    binary_inst = inst.to_binary();  
                }
                else{
                    iformat inst = iformat(tokens[i], tokens[i+2], tokens[i+1], tokens[i+3]); 
                    binary_inst = inst.to_binary();
                }
            }   
            //rformat 처리
            else if(rformat_function.find(tokens[i]) != rformat_function.end()) {
                if(tokens[i] == "sll" || tokens[i] == "srl"){
                    rformat inst = rformat("000000", "$zero", tokens[i+2], tokens[i+1],tokens[i+3],tokens[i]);
                    binary_inst = inst.to_binary();
                }
                else if(tokens[i] == "jr"){
                    rformat inst = rformat("000000", tokens[i+1], "$zero", "$zero","00000",tokens[i]); // opcode, rs, rt, rd, shamt, funct
                    binary_inst = inst.to_binary();
                }
                else if(tokens[i] == "mul"){
                    rformat inst = rformat("011100", tokens[i+2], tokens[i+3], tokens[i+1],"00000",tokens[i]); 
                    binary_inst = inst.to_binary();
                }
                else{
                    rformat inst = rformat("000000", tokens[i+2], tokens[i+3], tokens[i+1],"00000",tokens[i]); 
                    binary_inst = inst.to_binary();
                }
            }
            //jformat 처리
            else if(jformat_opcode.find(tokens[i]) != jformat_opcode.end()) {
                jformat inst = jformat(tokens[i], tokens[i+1]); // opcode, adress
                binary_inst = inst.to_binary();
            }
        }
        
        //output << binary_inst  << ' ' << address << endl;
        output << binary_inst << endl;
        address += 4;
    }   
}

int main() {
    try {
        filesystem::path current_path = filesystem::current_path();
        cout << "이전 작업 디렉토리: " << current_path << endl;
        
        // cs 폴더로 이동
        filesystem::current_path(current_path / "cs");
        cout << "현재 작업 디렉토리: " << filesystem::current_path() << endl;
    } catch (const filesystem::filesystem_error& e) {
        cerr << "작업 디렉토리 변경 중 오류 발생: " << e.what() << endl;
        return 1;
    }
   
    ifstream file("input.asm");
    if (!file.is_open()) {
        cerr << "파일을 열 수 없습니다: " << endl;
        return 1;
    }
    ofstream output("output.bin");
    parse_line(file, output);
    //cout << label_address.size() << endl;
    file.close(); 

    return 0;
}
