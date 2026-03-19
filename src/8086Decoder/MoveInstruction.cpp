#include "MoveInstruction.hpp"

#include "Foundation/Log.hpp"
#include "Foundation/Assert.hpp"

namespace 
{
    constexpr uint8_t modMask = 0b11000000; //mask
    constexpr uint8_t registerToRegesterMove = 0b11000000; //register to register move
    constexpr uint8_t noDisplacement = 0b00000000; //no displacement* (apart from when the R/M field is 110)
    constexpr uint8_t displacementBy8Bit = 0b01000000; //8-bit displacement for a effecitive address calculation
    constexpr uint8_t displacementBy16Bit = 0b10000000; //16-bit displacement for a effecitive address calculation
}

bool handleMoveInstruction(const Move& move, const char* const data, StringBuffer& instructionStringBuffer, uint32_t& currentByte)
{
    switch (move.types)
    {
    case Move::IMMIDATE_TO_REG_MEMORY: //immediate to register/memory - 1pad
    {
        uint8_t modField = data[currentByte] & modMask;
        if (modField == registerToRegesterMove)
        {
            VOID_ERROR("You can't do a register to register move with a immediate to register mov.\n");
        }
        else if (modField == noDisplacement)
        {
            const char* effectiveAddress = getEffectiveAddressCalculation(data[currentByte]);
            //We need to do this to move off the effective address calculation.
            ++currentByte;
            if (move.w)
            {
                uint16_t value = get16BitValue(data, currentByte);
                vprint(instructionStringBuffer.appendUseF("%s [%s], word %d\n", move.instructionName, effectiveAddress, value));
            }
            else
            {
                uint8_t value = data[currentByte];
                vprint(instructionStringBuffer.appendUseF("%s [%s], byte %d\n", move.instructionName, effectiveAddress, value));
            }

            return true;
        }
        else if (modField == displacementBy8Bit)
        {
            const char* effectiveAddress = getEffectiveAddressCalculation(data[currentByte]);
            uint16_t displacementValue = data[++currentByte];

            //We need to do this to move off the displacement value.
            ++currentByte;
            if (move.w)
            {
                uint16_t value = get16BitValue(data, currentByte);
                vprint(instructionStringBuffer.appendUseF("%s [%s + %d], word %d\n", move.instructionName, effectiveAddress, displacementValue, value));
            }
            else
            {
                uint8_t value = data[currentByte];
                vprint(instructionStringBuffer.appendUseF("%s [%s + %d], byte %d\n", move.instructionName, effectiveAddress, displacementValue, value));
            }

            return true;
        }
        else if (modField == displacementBy16Bit)
        {
            const char* effectiveAddress = getEffectiveAddressCalculation(data[currentByte]);
            uint16_t displacementValue = get16BitDisplacement(data, currentByte);

            //We need to do this to move off the displacement value.
            ++currentByte;
            if (move.w)
            {
                uint16_t value = get16BitValue(data, currentByte);
                vprint(instructionStringBuffer.appendUseF("%s [%s + %d], word %d\n", move.instructionName, effectiveAddress, displacementValue, value));
            }
            else
            {
                uint8_t value = data[++currentByte];
                vprint(instructionStringBuffer.appendUseF("%s [%s + %d], byte %d\n", move.instructionName, effectiveAddress, displacementValue, value));
            }

            return true;
        }
    }
    break;
    case Move::REG_MEMORY_TO_FROM_MEMORY: //register/memory to/from register - 2pad
    {
        uint8_t modField = data[currentByte] & modMask;
        if (modField == registerToRegesterMove)
        {
            const char* firstRegister = getRegister(data[currentByte], move.w, 0b00111000);
            const char* secondRegister = getRegister(data[currentByte], move.w, 0b00000111);

            if (move.d)
            {
                vprint(instructionStringBuffer.appendUseF("%s %s, %s\n", move.instructionName, firstRegister, secondRegister));
            }
            else
            {
                vprint(instructionStringBuffer.appendUseF("%s %s, %s\n", move.instructionName, secondRegister, firstRegister));
            }

            return true;
        }
        else if (modField == noDisplacement)
        {
            uint8_t rmMask = 0b00000111;
            uint8_t rm = rmMask & data[currentByte];
            if (rm == 0b00000110)
            {
                const char* reg = getRegister(data[currentByte], move.w, 0b00111000);
                uint16_t displacementValue = get16BitDisplacement(data, currentByte);

                vprint(instructionStringBuffer.appendUseF("%s %s, [%d]\n", move.instructionName, reg, displacementValue));

                return true;
            }
            const char* effectiveAddress = effectiveAddressCalculations[rm];
            const char* reg = getRegister(data[currentByte], move.w, 0b00111000);

            if (move.d)
            {
                vprint(instructionStringBuffer.appendUseF("%s %s, [%s]\n", move.instructionName, reg, effectiveAddress));
            }
            else
            {
                vprint(instructionStringBuffer.appendUseF("%s [%s], %s\n", move.instructionName, effectiveAddress, reg));
            }

            return true;
        }
        else if (modField == displacementBy8Bit)
        {
            const char* effectiveAddress = getEffectiveAddressCalculation(data[currentByte]);
            const char* reg = getRegister(data[currentByte], move.w, 0b00111000);
            uint16_t displacementValue = data[++currentByte];

            if (move.d)
            {
                vprint(instructionStringBuffer.appendUseF("%s %s, [%s + %d]\n", move.instructionName, reg, effectiveAddress, displacementValue));
            }
            else
            {
                vprint(instructionStringBuffer.appendUseF("%s [%s + %d], %s\n", move.instructionName, effectiveAddress, displacementValue, reg));
            }

            return true;
        }
        else if (modField == displacementBy16Bit)
        {
            const char* effectiveAddress = getEffectiveAddressCalculation(data[currentByte]);
            const char* reg = getRegister(data[currentByte], move.w, 0b00111000);
            uint16_t displacementValue = get16BitDisplacement(data, currentByte);

            if (move.d)
            {
                vprint(instructionStringBuffer.appendUseF("%s %s, [%s + %d]\n", move.instructionName, reg, effectiveAddress, displacementValue));
            }
            else
            {
                vprint(instructionStringBuffer.appendUseF("%s [%s + %d], %s\n", move.instructionName, effectiveAddress, displacementValue, reg));
            }

            return true;
        }
    }
    break;
    case Move::IMMIDATE_TO_REG: //immediate to register - 4pad
    {
        const char* reg = registerTable[move.w][move.reg];
        if (move.w)
        {
            uint16_t value = get16BitValue(data, currentByte);
            vprint(instructionStringBuffer.appendUseF("%s %s, %d\n", move.instructionName, reg, value));
        }
        else
        {
            uint8_t value = data[currentByte];
            vprint(instructionStringBuffer.appendUseF("%s %s, %d\n", move.instructionName, reg, value));
        }

        return true;
    }
    break;
    case Move::MEMORY_TO_ACCUMULATOR: //Memory to accumulator - 1pad
    {
        if (move.w)
        {
            uint16_t value = get16BitValue(data, currentByte);
            vprint(instructionStringBuffer.appendUseF("%s ax, [%d]\n", move.instructionName, value));
        }
        else
        {
            uint16_t value = data[currentByte];
            vprint(instructionStringBuffer.appendUseF("%s ax, [%d]\n", move.instructionName, value));
        }

        return true;
    }
    break;
    case Move::ACCUMULATOR_TO_MEMORY: //Accumulator to memory - 1pad
    {
        if (move.w)
        {
            uint16_t value = get16BitValue(data, currentByte);
            vprint(instructionStringBuffer.appendUseF("%s [%d], ax\n", move.instructionName, value));
        }
        else
        {
            uint16_t value = data[currentByte];
            vprint(instructionStringBuffer.appendUseF("%s [%d], ax\n", move.instructionName, value));
        }
        
        return true;
    }
    break;
    case Move::REG_MEMORY_TO_SEG_REG: //Register/ memory to segment register - 0pad
        VOID_ERROR("Not implememnt.\n");
        break;
    case Move::SEG_REG_TO_REG_MEMORY: //Segment register to register/memory - 0pad
        VOID_ERROR("Not implememnt.\n");
        break;
    }

    printBinary1(data[currentByte]);
    VOID_ERROR("Something has not parsed correctly at \n");
    return false;
}