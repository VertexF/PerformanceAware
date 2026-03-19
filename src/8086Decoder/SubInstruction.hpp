#ifndef SUB_INSTRUCTION_HDR
#define SUB_INSTRUCTION_HDR

#include "Foundation/String.hpp"
#include "CPUData.hpp"

bool handleSubInstruction(const Sub& sub, const char* const data, StringBuffer& instructionStringBuffer, uint32_t& currentByte);

#endif // !SUB_INSTRUCTION_HDR
