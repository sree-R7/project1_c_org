#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>
#include <map>
#include <istream>
#include <ostream>
#include <iterator>
#include <sstream>
#include <bitset>

//#include "myAssembler.h"

using namespace std;

void readlabel(string cline, ofstream &myfile);

void readline(string cline, ofstream &myfile);
void printline(string o, vector<string> passing);
void compare_opcode(string opcode, vector<string> vec, ofstream &myfile);
string convert_reg(string reg_name);

unsigned int countBits(unsigned int n);
string convert_imm(string imm_str);

void assembleTypeR(string op, string rs, string rt, string rd, string sh, string func, ofstream &myfile);

void assembleTypeI(string op, string rs, string rt, string imm, ofstream &myfile);

map<string, int> labelMap;
int label_line_count = 1; // we will increment this in readlabel
int line_count = 1;       // this will increment in realine
int label_count = 0;      // count number of labels
int main(int argc, char *argv[])
{

    /*               myAssembler *.s
     This command will have myAssembler read *.s and generate *.obj. If there is
     an existing *.obj file simply overwrite it.*/

    ofstream myfile;

    myfile.open("myAssembler.obj", ios::trunc); // If file is opened for output operations then previous content is deleted and replaced by the new one.

    string filename = argv[1];
    // cout << "filename:" << filename << endl;

    ifstream file;
    file.open(filename);

    if (!file.is_open()) // If the file does not exist the program crashes.
    {
        cout << "FILE DOES NOT EXIST/FAILED TO OPEN FILE!" << endl;
        exit(EXIT_FAILURE);
    }
    else
    {

        string lab;
        while (getline(file, lab))
        {

            char labelline[100];
            strcpy(labelline, lab.c_str());
            readlabel(lab, myfile);
        }

        // to parse second time reset pointer.
        file.clear();
        file.seekg(0, ios::beg);

        string line;
        while (getline(file, line))
        {
            char codeline[100];
            strcpy(codeline, line.c_str());
            readline(line, myfile);
        }
    }

    file.close();
    myfile.close();

    return 0;
}

void readline(string regular_line, ofstream &myfile)
{

    std::vector<char> writable(regular_line.begin(), regular_line.end());
    writable.push_back('\0'); // done because strtok needed char* type. not string
    // char delmiter = ',';

    char *pch;
    pch = strtok(&writable[0], " ,\t");
    //	add	$t1, $t2, $t3

    string opcode = pch; // will store mnemonic and labels
    vector<string> vec;  // vec holds everything but opcode
    while (pch != NULL)
    {

        vec.push_back(pch);
        pch = strtok(NULL, " , ( )");
    }
    vec.erase(vec.begin()); // removing opcode from vec
    /*cout << "opcode:" << opcode << endl;
    for (auto i : vec)
        cout << i << endl;*/

    // call function to convert opcode to hex bin here.
    compare_opcode(opcode, vec, myfile);
}

void readlabel(string label_line, ofstream &myfile)
{
    string labels = label_line;
    string delimit = ":";

    if (labels.find(delimit) != string::npos)
    {
        label_count++;
        labelMap[labels] = label_line_count + 1 - label_count;
    }
    label_line_count++;

    // cout << labelMap["L1:"] << endl;
    // cout << labelMap["L2:"] << endl;
}

// opcode-> add
// vec.at(0) -> $t1
// vect.at(1) -> $t2....

void compare_opcode(string opcode, vector<string> vec, ofstream &myfile)
{
    string op, rs, rt, rd, sh, func, imm, imm_temp, label_branch;

    // int imm_int = 0;
    string colon = ":";

    if (opcode == "add")
    {
        op = "000000";
        // cout << "In " << opcode << "rs is:" << vec.at(1) << endl;

        rs = convert_reg(vec.at(1)); // vec.at(1);
        rd = convert_reg(vec.at(0)); // vec.at(0);
        rt = convert_reg(vec.at(2)); // vec.at(2);
        sh = "00000";
        func = "100000";
        assembleTypeR(op, rs, rt, rd, sh, func, myfile);
    }
    if (opcode == "sub")
    {
        op = "000000";
        // cout << "In " << opcode << "rs is:" << vec.at(1) << endl;

        rs = convert_reg(vec.at(1)); // vec.at(1);
        rd = convert_reg(vec.at(0)); // vec.at(0);
        rt = convert_reg(vec.at(2)); // vec.at(2);
        sh = "00000";
        func = "100010";
        assembleTypeR(op, rs, rt, rd, sh, func, myfile);
    }
    if (opcode == "lw") // rt, imm(rs)
    {
        // cout << "In " << opcode << "rt is:" << vec.at(0) << endl;
        //  cout << "In " << opcode << "imm is:" << vec.at(1) << endl;
        // cout << "In " << opcode << "rs is:" << vec.at(2) << endl;
        op = "100011"; // 23 in Hex
        rt = convert_reg(vec.at(0));
        imm = convert_imm(vec.at(1));
        rs = convert_reg(vec.at(2));
        assembleTypeI(op, rs, rt, imm, myfile);
    }

    if (opcode.find(colon) != string::npos)
    {
        line_count--;
        // label_count++;
    }

    if (opcode == "beq")
    {
        op = "000100";
        rt = convert_reg(vec.at(1));
        rs = convert_reg(vec.at(0));
        label_branch = vec.at(2);
        label_branch.append(":");
        // into vec 3, you gotta insert string version of int length.
        // cout << "line of beq: " << line_count << endl;

        int diff = 0;

        if (labelMap[label_branch] > line_count) // if we are skipping instructions ie: label to jump to is later...
        {
            diff = (labelMap[label_branch] - line_count) - 1; // MIPS online
        }
        else
        {
            // diff = (labelMap[label_branch] - 1) - (line_count + 1); // yours
            diff = -((line_count - labelMap[label_branch])) - 1; // mips online
        }
        string to_imm = to_string(diff);
        // cout << "beq:" << to_imm << endl;
        imm = convert_imm(to_imm);
        assembleTypeI(op, rs, rt, imm, myfile);

        // cout << "beq diff:" << diff << endl;
    }

    if (opcode == "bne")
    {
        op = "000101";
        rt = convert_reg(vec.at(1));
        rs = convert_reg(vec.at(0));
        label_branch = vec.at(2);
        label_branch.append(":");
        // into vec 3, you gotta insert string version of int length.
        // cout << "line of beq: " << line_count << endl;

        int diff = 0;

        if (labelMap[label_branch] > line_count) // if we are skipping instructions ie: label to jump to is later...
        {
            diff = (labelMap[label_branch] - line_count) - 1; // MIPS online
        }
        else
        {
            diff = -(line_count - labelMap[label_branch]) - 1; // mips online
        }
        string to_imm = to_string(diff);
        imm = convert_imm(to_imm);
        assembleTypeI(op, rs, rt, imm, myfile);

        // cout << "bne diff:" << diff << endl;
    }
    line_count++;
    //  return;
}

void assembleTypeR(string op, string rs, string rt, string rd, string sh, string func, ofstream &myfile)
{
    // concatenate op rs rt rd sh func
    string conc;
    conc.append(op);
    conc.append(rs);
    conc.append(rt);
    conc.append(rd);
    conc.append(sh);
    conc.append(func);
    bitset<32> set(conc);
    // cout << "conc:" << conc << endl;

    myfile << std::setw(8) << std::setfill('0') << hex << set.to_ulong() << endl;
}

void assembleTypeI(string op, string rs, string rt, string imm, ofstream &myfile)
{
    // concatenate op rs rt rd sh func
    string conc;
    conc.append(op);
    conc.append(rs);
    conc.append(rt);
    conc.append(imm);
    bitset<32> set(conc);
    // cout << "conc:" << conc << endl;

    myfile << std::setw(8) << std::setfill('0') << hex << set.to_ulong() << endl;
}

string convert_imm(string imm_str)
{
    // need to sign extend imm_str
    int imm_unex = stoi(imm_str);
    // convert dec to binary
    // cout << "imm_unex: " << imm_unex << endl;
    string binary_imm = bitset<16>(imm_unex).to_string();

    /*
    AUX stuff , might need later to sign extend
    unsigned b = countBits(imm_unex); // number of bits representing the number in x

    int r;                 // resulting sign-extended number
    int m = 1U << (b - 1); // mask can be pre-computed if b is fixed

    // x = x & ((1U << b) - 1); // (Skip this if bits in x above position b are already zero.)
    r = (imm_unex ^ m) - m;*/

    std::string binary = std::bitset<16>(imm_unex).to_string(); // to binary
    // cout << "binary:" << binary << endl;
    return binary;
}

unsigned int countBits(unsigned int n)
{
    unsigned int count = 0;
    while (n)
    {
        count++;
        n >>= 1;
    }
    return count;
}

string convert_reg(string reg_at) //
{
    string reg_num_binary;

    // t0 - t9
    if (reg_at == "$t0")
    {
        reg_num_binary = "01000"; // 8
    }
    if (reg_at == "$t1")
    {
        reg_num_binary = "01001"; // 9
    }
    if (reg_at == "$t2")
    {
        reg_num_binary = "01010"; // 10
    }
    if (reg_at == "$t3")
    {
        reg_num_binary = "01011"; // 11
    }
    if (reg_at == "$t4")
    {
        reg_num_binary = "01100"; // 12
    }
    if (reg_at == "$t5")
    {
        reg_num_binary = "01011"; // 13
    }
    if (reg_at == "$t6")
    {
        reg_num_binary = "01110"; // 14
    }
    if (reg_at == "$t7")
    {
        reg_num_binary = "01111"; // 15
    }
    if (reg_at == "$t8")
    {
        reg_num_binary = "11000"; // 24
    }
    if (reg_at == "$t9")
    {
        reg_num_binary = "11001"; // 25
    }

    ////////$zero
    if (reg_at == "$zero" || reg_at == "$0")
    {
        reg_num_binary = "00000"; // 0
    }
    //////// $at
    if (reg_at == "$at")
    {
        reg_num_binary = "00001"; // 1
    }
    //////// v0-v1
    if (reg_at == "$v0")
    {
        reg_num_binary = "00010"; // 2
    }
    if (reg_at == "$v1")
    {
        reg_num_binary = "00011"; // 3
    }
    //////// a0-a3
    if (reg_at == "$a0")
    {
        reg_num_binary = "00100"; // 4
    }
    if (reg_at == "$a1")
    {
        reg_num_binary = "00101"; // 5
    }
    if (reg_at == "$a2")
    {
        reg_num_binary = "00110"; // 6
    }
    if (reg_at == "$a3")
    {
        reg_num_binary = "00111"; // 7
    }
    ///////// s0- s7
    if (reg_at == "$s0")
    {
        reg_num_binary = "10000"; // 16
    }
    if (reg_at == "$s1")
    {
        reg_num_binary = "10001"; // 17
    }
    if (reg_at == "$s2")
    {
        reg_num_binary = "10010"; // 18
    }
    if (reg_at == "$s3")
    {
        reg_num_binary = "10011"; // 19
    }
    if (reg_at == "$s4")
    {
        reg_num_binary = "10100"; // 20
    }
    if (reg_at == "$s5")
    {
        reg_num_binary = "10101"; // 21
    }
    if (reg_at == "$s6")
    {
        reg_num_binary = "10110"; // 22
    }
    if (reg_at == "$s7")
    {
        reg_num_binary = "10111"; // 23
    }

    ////// $gp
    if (reg_at == "$gp")
    {
        reg_num_binary = "11100"; // 28
    }

    ///// sp, fp, ra,
    if (reg_at == "$sp")
    {
        reg_num_binary = "11101"; // 29
    }
    if (reg_at == "$fp")
    {
        reg_num_binary = "11110"; // 30
    }
    if (reg_at == "$ra")
    {
        reg_num_binary = "11111"; // 31
    }

    return reg_num_binary;
}
