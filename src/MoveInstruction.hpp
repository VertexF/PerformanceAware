#ifndef MOVE_INSTRUCTION_HDR
#define MOVE_INSTRUCTION_HDR

#include "Foundation/String.hpp"

#include "CPUData.hpp"

bool handleMoveInstruction(const Move &move, const char* const data, StringBuffer& instructionStringBuffer, uint32_t &currentByte);

#endif // !MOVE_INSTRUCTION_HDR
