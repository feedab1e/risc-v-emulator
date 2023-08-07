#include "rv32i-ima/isa/formats.hpp"
#include "rv32i-ima/isa/instructions.hpp"
#include "rv32i-ima/machine.hpp"
#include <fstream>
#include <filesystem>

std::vector<unsigned int> readBinaryFile(std::string fileName)
{
  // Open the binary file in input mode
  std::ifstream inputFile(fileName.c_str(), std::ios::binary | std::ios::in);


  // Check if the file is opened successfully
  if (!inputFile.is_open()) {
    throw std::runtime_error ("Failed to open the binary file.");
  }


  // Move the pointer to the end of the file to get the file size
  inputFile.seekg(0, inputFile.end);
  size_t fileSize = inputFile.tellg();
  inputFile.seekg(0, inputFile.beg);


  // Read the binary file into a vector buffer
  std::vector<uint32_t> buf(fileSize);
  inputFile.read(reinterpret_cast<char *>(buf.data()), fileSize);


  // Close the file
  inputFile.close();
  return buf;
}


int main(int argc, char **argv) {
  using namespace rv32i;
  using namespace format;
  using namespace instructions;
  machine<
    rv32i_default_formats,
    instruction_set<LUI, AUIPC, ADDI, ADD, SLT, SLTIU, SLTU, ANDI, AND, ORI, OR, XORI, XOR, SLLI, SLL, SRLI_SRAI, SRL, SRA, SUB , csrrw, csrrs, JAL, JALR, BEQ, BNE, BGE, BLT, BGEU, BLTU,
                      LB, LH, LW, LBU, LHU,
                      SB, SH, SW>

  > m;
  //m.program = {0x8765'4000 | 0b0110111 | 6 << 7};
  std::cout<<std::filesystem::current_path()<<std::endl;
  m.program = readBinaryFile(std::filesystem::current_path()/"../test/bin/rv32ui-p-add.bin");
  //TODO: test auipc
  //TODO: test add
  //TODO: test slt/sltu
  //TODO: test and/or/xor
  //TODO: test sll, srl, sra, sub
  //TODO: test addi/nop
  for(int i = 0; i < 100; i++){
    m.step();
  }
  return 0;
}
