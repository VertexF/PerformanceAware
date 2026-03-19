#ifndef CMP_INSTRUCTION_HDR
#define CMP_INSTRUCTION_HDR

#include "Foundation/String.hpp"
#include "CPUData.hpp"

bool handleCmpInstruction(const Cmp& cmp, const char* const data, StringBuffer& instructionStringBuffer, uint32_t& currentByte);

#endif // !CMP_INSTRUCTION_HDR
