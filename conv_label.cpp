#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <bitset>

using namespace std;

unordered_map<string, string> format_type = {
    {"add", "R"}, {"sub", "R"}, {"mul", "R"}, {"div", "R"}, {"and", "R"},
    {"or", "R"}, {"xor", "R"}, {"sll", "R"}, {"srl", "R"}, {"sra", "R"},
    {"slt", "R"}, {"addi", "I"}, {"andi", "I"}, {"ori", "I"},
    {"lw", "I"}, {"sw", "S"}, {"beq", "B"}, {"bne", "B"},
    {"lui", "U"}, {"jal", "J"}
};

unordered_map<string, string> opcode_map = {
    {"add", "0110011"}, {"sub", "0110011"}, {"addi", "0010011"},
    {"lw", "0000011"}, {"sw", "0100011"}, {"beq", "1100011"},
    {"bne", "1100011"}, {"jal", "1101111"}
};

unordered_map<string, string> funct3_map = {
    {"add", "000"}, {"sub", "000"}, {"addi", "000"}, {"lw", "010"},
    {"sw", "010"}, {"beq", "000"}, {"bne", "001"}
};

unordered_map<string, string> funct7_map = {{"add", "0000000"}, {"sub", "0100000"}};

unordered_map<string, string> register_map;
unordered_map<string, int> label_map; 

void init_register_map() {
    for (int i = 0; i < 32; i++) {
        register_map["x" + to_string(i)] = bitset<5>(i).to_string();
    }
}


string convert_to_machine_code(vector<string> tokens, int current_pc) {
    string opcode = opcode_map[tokens[0]];
    string type = format_type[tokens[0]];
    string funct3 = funct3_map[tokens[0]];
    string rd, rs1, rs2, imm, funct7;

    if (type == "R") {
        rd = register_map[tokens[1]];
        rs1 = register_map[tokens[2]];
        rs2 = register_map[tokens[3]];
        funct7 = funct7_map[tokens[0]];
        return funct7 + rs2 + rs1 + funct3 + rd + opcode;
    }
    else if (type == "I") {
        rd = register_map[tokens[1]];
        rs1 = register_map[tokens[2]];
        imm = bitset<12>(stoi(tokens[3])).to_string();
        return imm + rs1 + funct3 + rd + opcode;
    }
    else if (type == "S") {
        rs1 = register_map[tokens[3]];
        rs2 = register_map[tokens[1]];
        imm = bitset<12>(stoi(tokens[2])).to_string();
        return imm.substr(0, 7) + rs2 + rs1 + funct3 + imm.substr(7, 5) + opcode;
    }
    else if (type == "B") {
        rs1 = register_map[tokens[1]];
        rs2 = register_map[tokens[2]];

        if (label_map.find(tokens[3]) == label_map.end()) {
            
            cerr << "Error: Label '" << tokens[3] << "' not found!" << endl;
            exit(1);
        }
        int offset = label_map[tokens[3]] - current_pc; 
        imm = bitset<13>(offset).to_string(); 
        cout<<imm<<endl;

        return imm[0] + imm.substr(2, 6) + rs2 + rs1 + funct3 + imm.substr(8, 4) + imm[1] + opcode;
    }
    else if (type == "U") {
        rd = register_map[tokens[1]];
        imm = bitset<20>(stoi(tokens[2])).to_string();
        return imm + rd + opcode;
    }
    else if (type == "J") {
        rd = register_map[tokens[1]];
        if (label_map.find(tokens[2]) == label_map.end()) {
            cerr << "Error: Label '" << tokens[2] << "' not found!" << endl;
            exit(1);
        }
        
        int offset = label_map[tokens[2]] - current_pc;
        cout<<current_pc<<" "<<label_map[tokens[2]]<<endl;
        imm = bitset<21>(offset).to_string();
        cout<<"jal"<<" "<<imm<<endl;
        return imm[0] + imm.substr(10, 10) + imm[9] + imm.substr(2, 8) + rd + opcode;
    }
    return "";
}

int main() {
    init_register_map();
    ifstream asm_file("input.asm");
    ofstream mc_file("output.mc");
    string line;
    vector<string> instructions;
    int address = 0x0;

    // **First Pass: Store label locations**
    while (getline(asm_file, line)) {
        istringstream iss(line);
        vector<string> tokens;
        string word;
        while (iss >> word) tokens.push_back(word);
       

        if (!tokens.empty() && tokens[0].back() == ':') {
           
            string label = tokens[0].substr(0, tokens[0].size() - 1);
            label_map[label] = address; 
            instructions.push_back(line);  
            address += 4;   
        } else {
            instructions.push_back(line);
            address += 4;
        }
    }
    asm_file.close();
    cout << "First pass completed!" << endl;
    for(auto i:label_map){
        cout<<i.first<<" "<<i.second<<endl;
    }
    

    address = 0x0;
    for (const auto &instr : instructions) {
        istringstream iss(instr);
        vector<string> tokens;
        string word;
        while (iss >> word) tokens.push_back(word);
        cout<<tokens.size()<<endl;
        for(int i=0;i<tokens.size();i++){
            cout<<tokens[i]<<" ";
        }
        cout<<endl;

        if (tokens.empty()) continue;  
        
       
        if (tokens[0].back() == ':') {
            
            tokens.erase(tokens.begin());

        }

        string machine_code = convert_to_machine_code(tokens, address);
        if (!machine_code.empty()) {
            mc_file << "0x" << hex << address << " 0x" 
                    << bitset<32>(stoul(machine_code, 0, 2)).to_ulong() 
                    << " , " << instr << endl;
            address += 4;
        }
    }

    mc_file.close();
    return 0;
}
