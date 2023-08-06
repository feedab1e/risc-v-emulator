#include "rv32i-ima/isa/formats.hpp"
#include "rv32i-ima/isa/instructions.hpp"
#include "rv32i-ima/machine.hpp"


int main(int argc, char **argv) {
  using namespace rv32i;
  using namespace format;
  using namespace instructions;
  machine<
    rv32i_default_formats,
    instruction_set<LUI, AUIPC, ADD, SLT, SLTU, AND, OR, XOR, csrrw, csrrs>
  > m;
  m.program = {0x8765'4000 | 0b0110111 | 6 << 7};
  //TODO: test auipc
  //TODO: test add
  //TODO: test slt/sltu
  //TODO: test and/or/xor
  m.step();
  return 0;
}
