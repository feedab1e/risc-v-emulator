#include <iostream>
#include <map>
#include <cstdint>
#include <climits>
#include <type_traits>
#include "rv32i-ima/isa/formats.hpp"
#include "rv32i-ima/isa/instructions.hpp"
#include "rv32i-ima/machine.hpp"


int main(int argc, char **argv) {
  using namespace rv32i;
  using namespace format;
  using namespace instructions;
  machine<
    rv32i_default_formats,
    instruction_set<add, csrrw, csrrs>
  > m;
  m.program = {0x8000'0000};
  m.step();
  return 0;
}
