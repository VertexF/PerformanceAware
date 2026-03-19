#ifndef JNZ_INSTRUCTION_HDR
#define JNZ_INSTRUCTION_HDR

#include "Foundation/String.hpp"
#include "CPUData.hpp"

bool handleJnzInstruction(const char* const data, StringBuffer& instructionStringBuffer, uint32_t& currentByte);

#endif // !JNZ_INSTRUCTION_HDR
