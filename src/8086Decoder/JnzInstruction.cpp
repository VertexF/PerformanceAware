#include "AddInstruction.hpp"

#include "Foundation/Assert.hpp"

enum JznInstructions : uint8_t
{
    JE = 0b01110100,
    JL = 0b01111100,
    JLE = 0b01111110,
    JB = 0b01110010,
    JBE = 0b01110110,
    JP = 0b01111010,
    JO = 0b01110000,
    JS = 0b01111000,
    JNZ = 0b01110101,
    JNL = 0b01111101,
    JG = 0b01111111,
    JNB = 0b01110011,
    JA = 0b01110111,
    JNP = 0b01111011,
    JNO = 0b01110001,
    JNS = 0b01111001,
    LOOP = 0b11100010,
    LOOPZ = 0b11100001,
    LOOPNZ = 0b11100000,
    JCXZ = 0b11100011,
};

void printJumpValue(const char* const data, StringBuffer& instructionStringBuffer, uint32_t& currentByte, const char* instructionName)
{
    uint32_t programPosition = currentByte;
    currentByte++;
    programPosition += data[currentByte];
    vprint(instructionStringBuffer.appendUseF("%s, 0x%x\n", instructionName, programPosition));
}

bool handleJnzInstruction(const char* const data, StringBuffer& instructionStringBuffer, uint32_t& currentByte)
{
    switch ((uint8_t)data[currentByte])
    {
        case JE:
        {
            printJumpValue(data, instructionStringBuffer, currentByte, "je");
        }
            return true;
        case JL:
        {
            printJumpValue(data, instructionStringBuffer, currentByte, "jl");
        }
            return true;
        case JLE:
        {
            printJumpValue(data, instructionStringBuffer, currentByte, "jle");
        }
            return true;
        case JB:
        {
            printJumpValue(data, instructionStringBuffer, currentByte, "jb");
        }
            return true;
        case JBE:
        {
            printJumpValue(data, instructionStringBuffer, currentByte, "jbe");
        }
            return true;
        case JP:
        {
            printJumpValue(data, instructionStringBuffer, currentByte, "jp");
        }
            return true;
        case JO:
        {
            printJumpValue(data, instructionStringBuffer, currentByte, "jo");
        }
            return true;
        case JS:
        {
            printJumpValue(data, instructionStringBuffer, currentByte, "js");
        }
            return true;
        case JNZ:
        {
            printJumpValue(data, instructionStringBuffer, currentByte, "jnz");
        }
            return true;
        case JNL:
        {
            printJumpValue(data, instructionStringBuffer, currentByte, "jnl");
        }
            return true;
        case JG:
        {
            printJumpValue(data, instructionStringBuffer, currentByte, "jg");
        }
            return true;
        case JNB:
        {
            printJumpValue(data, instructionStringBuffer, currentByte, "jnb");
        }
            return true;
        case JA:
        {
            printJumpValue(data, instructionStringBuffer, currentByte, "ja");
        }
            return true;
        case JNP:
        {
            printJumpValue(data, instructionStringBuffer, currentByte, "jnp");
        }
            return true;
        case JNO:
        {
            printJumpValue(data, instructionStringBuffer, currentByte, "jno");
        }
            return true;
        case JNS:
        {
            printJumpValue(data, instructionStringBuffer, currentByte, "jns");
        }
            return true;
        case LOOP:
        {
            printJumpValue(data, instructionStringBuffer, currentByte, "loop");
        }
            return true;
        case LOOPZ:
        {
            printJumpValue(data, instructionStringBuffer, currentByte, "loopz");
        }
            return true;
        case LOOPNZ:
        {
            printJumpValue(data, instructionStringBuffer, currentByte, "loopnz");
        }
            return true;
        case JCXZ:
        {
            printJumpValue(data, instructionStringBuffer, currentByte, "jcxz");
        }
            return true;
    }

    return false;
}