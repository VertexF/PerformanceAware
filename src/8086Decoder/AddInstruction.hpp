#ifndef ADD_INSTRUCTION_HDR
#define ADD_INSTRUCTION_HDR

#include "Foundation/String.hpp"
#include "CPUData.hpp"

bool handleAddInstruction(const Add& move, const char* const data, StringBuffer& instructionStringBuffer, uint32_t& currentByte);

#endif // !ADD_INSTRUCTION_HDR
