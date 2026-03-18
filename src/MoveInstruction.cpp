#include "MoveInstruction.hpp"

#include "Foundation/Log.hpp"
#include "Foundation/Assert.hpp"

bool handleMoveInstruction(const Move& move, const char* const data, StringBuffer& instructionStringBuffer, uint32_t& currentByte)
{
    switch (move.types)
    {
    case Move::IMMIDATE_TO_REG_MEMORY: //immediate to register/memory - 1pad
    {
        uint8_t modMask = 0b11000000; //mask
        uint8_t registerToRegesterMove = 0b11000000; //register to register move
        uint8_t noDisplacement = 0b00000000; //no displacement* (apart from when the R/M field is 110)
        uint8_t displacementBy8Bit = 0b01000000; //8-bit displacement for a effecitive address calculation
        uint8_t displacementBy16Bit = 0b10000000; //16-bit displacement for a effecitive address calculation
        uint8_t modField = data[currentByte] & modMask;
        if (modField == registerToRegesterMove)
        {
            VOID_ERROR("You can't do a register to register move with a immediate to register mov.\n");
        }
        else if (modField == noDisplacement)
        {
            uint8_t rmMask = 0b00000111;
            uint8_t rm = rmMask & data[currentByte];
            if (rm == 0b00000110)
            {
                VOID_ERROR("Direct Address mode not implemented");
            }
            const char* effectiveAddress = effectiveAddressCalculations[rm];
            bool isWide = move.w;

            if (isWide)
            {
                uint16_t lowValueMask = 0b0000000011111111;
                currentByte++;
                uint16_t lowValue = data[currentByte] & lowValueMask;
                currentByte++;
                uint16_t highValue = data[currentByte];

                highValue = highValue << 8;
                uint16_t value = lowValue | highValue;

                vprint(instructionStringBuffer.appendUseF("%s [%s], word %d\n", move.instructionName, effectiveAddress, value));
            }
            else
            {
                uint8_t value = data[++currentByte];

                vprint(instructionStringBuffer.appendUseF("%s [%s], byte %d\n", move.instructionName, effectiveAddress, value));
            }

            currentByte++;
            return true;
        }
        else if (modField == displacementBy8Bit)
        {
            uint8_t rmMask = 0b00000111;
            uint8_t rm = rmMask & data[currentByte];

            const char* effectiveAddress = effectiveAddressCalculations[rm];
            bool isWide = move.w;

            uint16_t displacementValue = data[++currentByte];

            if (isWide)
            {
                uint16_t lowValueMask = 0b0000000011111111;
                currentByte++;
                uint16_t lowValue = data[currentByte] & lowValueMask;
                currentByte++;
                uint16_t highValue = data[currentByte];

                highValue = highValue << 8;
                uint16_t value = lowValue | highValue;

                vprint(instructionStringBuffer.appendUseF("%s [%s + %d], word %d\n", move.instructionName, effectiveAddress, displacementValue, value));
            }
            else
            {
                uint8_t value = data[++currentByte];

                vprint(instructionStringBuffer.appendUseF("%s [%s + %d], byte %d\n", move.instructionName, effectiveAddress, displacementValue, value));
            }

            currentByte++;
            return true;
        }
        else if (modField == displacementBy16Bit)
        {
            uint8_t rmMask = 0b00000111;
            uint8_t rm = rmMask & data[currentByte];

            const char* effectiveAddress = effectiveAddressCalculations[rm];
            bool isWide = move.w;

            uint16_t dispLowMask = 0b0000000011111111;
            currentByte++;
            uint16_t lowDispLow = data[currentByte] & dispLowMask;
            currentByte++;
            uint16_t dispHigh = data[currentByte];

            dispHigh = dispHigh << 8;
            uint16_t displacementValue = lowDispLow | dispHigh;

            if (isWide)
            {
                uint16_t lowValueMask = 0b0000000011111111;
                currentByte++;
                uint16_t lowValue = data[currentByte] & lowValueMask;
                currentByte++;
                uint16_t highValue = data[currentByte];

                highValue = highValue << 8;
                uint16_t value = lowValue | highValue;

                vprint(instructionStringBuffer.appendUseF("%s [%s + %d], word %d\n", move.instructionName, effectiveAddress, displacementValue, value));
            }
            else
            {
                uint8_t value = data[++currentByte];

                vprint(instructionStringBuffer.appendUseF("%s [%s + %d], byte %d\n", move.instructionName, effectiveAddress, displacementValue, value));
            }

            currentByte++;
            return true;
        }
    }
    break;
    case Move::REG_MEMORY_TO_FROM_MEMORY: //register/memory to/from register - 2pad
    {
        uint8_t modMask = 0b11000000; //mask
        uint8_t registerToRegesterMove = 0b11000000; //register to register move
        uint8_t noDisplacement = 0b00000000; //no displacement* (apart from when the R/M field is 110)
        uint8_t displacementBy8Bit = 0b01000000; //8-bit displacement for a effecitive address calculation
        uint8_t displacementBy16Bit = 0b10000000; //16-bit displacement for a effecitive address calculation
        uint8_t modField = data[currentByte] & modMask;
        if (modField == registerToRegesterMove)
        {
            bool isWide = move.w;
            uint8_t firstMaskRegister = 0b00111000;
            uint8_t firstRegisterIndex = (data[currentByte] & firstMaskRegister) >> 3;
            const char* firstRegister = registerTable[isWide][firstRegisterIndex];

            uint8_t secondMaskRegister = 0b00000111;
            uint8_t secondRegisterIndex = (data[currentByte] & secondMaskRegister);
            const char* secondRegister = registerTable[isWide][secondRegisterIndex];

            if (move.d)
            {
                vprint(instructionStringBuffer.appendUseF("%s %s, %s\n", move.instructionName, firstRegister, secondRegister));
            }
            else
            {
                vprint(instructionStringBuffer.appendUseF("%s %s, %s\n", move.instructionName, secondRegister, firstRegister));
            }

            currentByte++;
            return true;
        }
        else if (modField == noDisplacement)
        {
            uint8_t rmMask = 0b00000111;
            uint8_t rm = rmMask & data[currentByte];
            if (rm == 0b00000110)
            {
                uint8_t maskRegister = 0b00111000;
                uint8_t registerIndex = (data[currentByte] & maskRegister) >> 3;
                const char* reg = registerTable[move.w][registerIndex];

                uint16_t lowValueMask = 0b0000000011111111;
                currentByte++;
                uint16_t lowValue = data[currentByte] & lowValueMask;
                currentByte++;
                uint16_t highValue = data[currentByte];

                highValue = highValue << 8;
                uint16_t value = lowValue | highValue;

                vprint(instructionStringBuffer.appendUseF("%s %s, [%d]\n", move.instructionName, reg, value));

                currentByte++;
                return true;
            }
            const char* effectiveAddress = effectiveAddressCalculations[rm];
            uint8_t maskRegister = 0b00111000;
            uint8_t registerIndex = (data[currentByte] & maskRegister) >> 3;
            const char* reg = registerTable[move.w][registerIndex];

            if (move.d)
            {
                vprint(instructionStringBuffer.appendUseF("%s %s, [%s]\n", move.instructionName, reg, effectiveAddress));
            }
            else
            {
                vprint(instructionStringBuffer.appendUseF("%s [%s], %s\n", move.instructionName, effectiveAddress, reg));
            }
            currentByte++;
            return true;
        }
        else if (modField == displacementBy8Bit)
        {
            uint8_t rmMask = 0b00000111;
            uint8_t rm = rmMask & data[currentByte];

            const char* effectiveAddress = effectiveAddressCalculations[rm];
            uint8_t maskRegister = 0b00111000;
            uint8_t registerIndex = (data[currentByte] & maskRegister) >> 3;
            const char* reg = registerTable[move.w][registerIndex];

            uint16_t value = data[++currentByte];

            if (move.d)
            {
                vprint(instructionStringBuffer.appendUseF("%s %s, [%s + %d]\n", move.instructionName, reg, effectiveAddress, value));
            }
            else
            {
                vprint(instructionStringBuffer.appendUseF("%s [%s + %d], %s\n", move.instructionName, effectiveAddress, value, reg));
            }

            currentByte++;
            return true;
        }
        else if (modField == displacementBy16Bit)
        {
            uint8_t rmMask = 0b00000111;
            uint8_t rm = rmMask & data[currentByte];

            const char* effectiveAddress = effectiveAddressCalculations[rm];
            uint8_t maskRegister = 0b00111000;
            uint8_t registerIndex = (data[currentByte] & maskRegister) >> 3;
            const char* reg = registerTable[move.w][registerIndex];

            uint16_t lowValueMask = 0b0000000011111111;
            currentByte++;
            uint16_t lowValue = data[currentByte] & lowValueMask;
            currentByte++;
            uint16_t highValue = data[currentByte];

            highValue = highValue << 8;
            uint16_t value = lowValue | highValue;


            if (move.d)
            {
                vprint(instructionStringBuffer.appendUseF("%s %s, [%s + %d]\n", move.instructionName, reg, effectiveAddress, value));
            }
            else
            {
                vprint(instructionStringBuffer.appendUseF("%s [%s + %d], %s\n", move.instructionName, effectiveAddress, value, reg));
            }

            currentByte++;
            return true;
        }
    }
    break;
    case Move::IMMIDATE_TO_REG: //immediate to register - 4pad
    {
        if (move.w)
        {
            const char* reg = registerTable[move.w][move.reg];
            uint16_t lowValueMask = 0b0000000011111111;
            uint16_t lowValue = data[currentByte] & lowValueMask;
            currentByte++;
            uint16_t highValue = data[currentByte];
            currentByte++;

            highValue = highValue << 8;
            uint16_t value = lowValue | highValue;

            vprint(instructionStringBuffer.appendUseF("%s %s, %d\n", move.instructionName, reg, value));
        }
        else
        {
            const char* reg = registerTable[move.w][move.reg];
            uint8_t value = data[currentByte];
            vprint(instructionStringBuffer.appendUseF("%s %s, %d\n", move.instructionName, reg, value));
        }

        currentByte++;
        return true;
    }
    break;
    case Move::MEMORY_TO_ACCUMULATOR: //Memory to accumulator - 1pad
    {
        if (move.w)
        {
            uint16_t lowValueMask = 0b0000000011111111;
            uint16_t lowValue = data[currentByte] & lowValueMask;
            currentByte++;
            uint16_t highValue = data[currentByte];

            highValue = highValue << 8;
            uint16_t value = lowValue | highValue;

            vprint(instructionStringBuffer.appendUseF("%s ax, [%d]\n", move.instructionName, value));
        }
        else
        {
            uint16_t value = data[currentByte];

            vprint(instructionStringBuffer.appendUseF("%s ax, [%d]\n", move.instructionName, value));
        }

        currentByte++;
        return true;
    }
    break;
    case Move::ACCUMULATOR_TO_MEMORY: //Accumulator to memory - 1pad
    {
        if (move.w)
        {
            uint16_t lowValueMask = 0b0000000011111111;
            uint16_t lowValue = data[currentByte] & lowValueMask;
            currentByte++;
            uint16_t highValue = data[currentByte];

            highValue = highValue << 8;
            uint16_t value = lowValue | highValue;

            vprint(instructionStringBuffer.appendUseF("%s [%d], ax\n", move.instructionName, value));
        }
        else
        {
            uint16_t value = data[currentByte];

            vprint(instructionStringBuffer.appendUseF("%s [%d], ax\n", move.instructionName, value));
        }
        currentByte++;
        return true;
    }
    break;
    case Move::REG_MEMORY_TO_SEG_REG: //Register/ memory to segment register - 0pad
        break;
    case Move::SEG_REG_TO_REG_MEMORY: //Segment register to register/memory - 0pad
        break;
    }

    return true;
}