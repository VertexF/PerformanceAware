#include "AddInstruction.hpp"

#include "Foundation/Assert.hpp"

namespace
{
    constexpr uint8_t modMask = 0b11000000; //mask
    constexpr uint8_t registerToRegesterMove = 0b11000000; //register to register add
    constexpr uint8_t noDisplacement = 0b00000000; //no displacement* (apart from when the R/M field is 110)
    constexpr uint8_t displacementBy8Bit = 0b01000000; //8-bit displacement for a effecitive address calculation
    constexpr uint8_t displacementBy16Bit = 0b10000000; //16-bit displacement for a effecitive address calculation
}

bool handleSubInstruction(const Sub& sub, const char* const data, StringBuffer& instructionStringBuffer, uint32_t& currentByte)
{
    switch (sub.types) 
    {
    case Sub::REG_MEMORY_WITH_TO_EITHER://Reg/memory with either register to either - 2pad
    {
        uint8_t modField = data[currentByte] & modMask;
        if (modField == registerToRegesterMove)
        {
            const char* firstRegister = getRegister(data[currentByte], sub.w, 0b00111000);
            const char* secondRegister = getRegister(data[currentByte], sub.w, 0b00000111);

            if (sub.d)
            {
                vprint(instructionStringBuffer.appendUseF("%s %s, %s\n", sub.instructionName, firstRegister, secondRegister));
            }
            else
            {
                vprint(instructionStringBuffer.appendUseF("%s %s, %s\n", sub.instructionName, secondRegister, firstRegister));
            }

            return true;
        }
        else if (modField == noDisplacement)
        {
            uint8_t rmMask = 0b00000111;
            uint8_t rm = rmMask & data[currentByte];
            if (rm == 0b00000110)
            {
                const char* reg = getRegister(data[currentByte], sub.w, 0b00111000);
                uint16_t displacementValue = get16BitDisplacement(data, currentByte);

                vprint(instructionStringBuffer.appendUseF("%s %s, [%d]\n", sub.instructionName, reg, displacementValue));

                return true;
            }
            const char* effectiveAddress = effectiveAddressCalculations[rm];
            const char* reg = getRegister(data[currentByte], sub.w, 0b00111000);

            if (sub.d)
            {
                vprint(instructionStringBuffer.appendUseF("%s %s, [%s]\n", sub.instructionName, reg, effectiveAddress));
            }
            else
            {
                vprint(instructionStringBuffer.appendUseF("%s [%s], %s\n", sub.instructionName, effectiveAddress, reg));
            }

            return true;
        }
        else if (modField == displacementBy8Bit)
        {
            const char* effectiveAddress = getEffectiveAddressCalculation(data[currentByte]);
            const char* reg = getRegister(data[currentByte], sub.w, 0b00111000);
            currentByte++;
            uint16_t displacementValue = data[currentByte];

            if (sub.d)
            {
                vprint(instructionStringBuffer.appendUseF("%s %s, [%s + %d]\n", sub.instructionName, reg, effectiveAddress, displacementValue));
            }
            else
            {
                vprint(instructionStringBuffer.appendUseF("%s [%s + %d], %s\n", sub.instructionName, effectiveAddress, displacementValue, reg));
            }

            return true;
        }
        else if (modField == displacementBy16Bit)
        {
            const char* effectiveAddress = getEffectiveAddressCalculation(data[currentByte]);
            const char* reg = getRegister(data[currentByte], sub.w, 0b00111000);
            uint16_t displacementValue = get16BitDisplacement(data, currentByte);

            if (sub.d)
            {
                vprint(instructionStringBuffer.appendUseF("%s %s, [%s + %d]\n", sub.instructionName, reg, effectiveAddress, displacementValue));
            }
            else
            {
                vprint(instructionStringBuffer.appendUseF("%s [%s + %d], %s\n", sub.instructionName, effectiveAddress, displacementValue, reg));
            }

            return true;
        }
        break;
    }
    case Sub::IMMIDATE_FROM_REG_MEMORY://immediate to register - 2pad
    {
        uint8_t modField = data[currentByte] & modMask;

        if (modField == registerToRegesterMove)
        {
            const char* reg = getRegister(data[currentByte], sub.w, 0b00000111);

            currentByte++;
            if (sub.w)
            {
                uint16_t value;
                //If the s has been set I'm going to assume it's a signed number because how can you sign extend a non-signed number...
                value = signExtend(data, currentByte, sub.s);
                vprint(instructionStringBuffer.appendUseF("%s %s, %d\n", sub.instructionName, reg, value));

                return true;
            }
            else
            {
                uint8_t value = data[currentByte];
                vprint(instructionStringBuffer.appendUseF("%s %s, %d\n", sub.instructionName, reg, value));
                return true;
            }
        }
        else if (modField == noDisplacement)
        {
            uint8_t rmMask = 0b00000111;
            uint8_t rm = rmMask & data[currentByte];
            if (rm == 0b00000110)
            {
                const char* reg = getRegister(data[currentByte], sub.w, 0b00111000);
                uint16_t displacementValue = get16BitDisplacement(data, currentByte);

                //We need to do this to add off the displacementValue.
                ++currentByte;
                if (sub.w)
                {
                    uint16_t value = signExtend(data, currentByte, sub.s);
                    vprint(instructionStringBuffer.appendUseF("%s [%d], word %d\n", sub.instructionName, displacementValue, value));
                }
                else
                {
                    uint8_t value = data[currentByte];
                    vprint(instructionStringBuffer.appendUseF("%s [%d], byte %d\n", sub.instructionName, displacementValue, value));
                }

                return true;
            }
            const char* effectiveAddress = effectiveAddressCalculations[rm];
            //We need to do this to add off the effective address calculation.
            ++currentByte;
            if (sub.w)
            {
                uint16_t value = signExtend(data, currentByte, sub.s);
                vprint(instructionStringBuffer.appendUseF("%s [%s], word %d\n", sub.instructionName, effectiveAddress, value));
            }
            else
            {
                uint8_t value = data[currentByte];
                vprint(instructionStringBuffer.appendUseF("%s [%s], byte %d\n", sub.instructionName, effectiveAddress, value));
            }

            return true;
        }
        else if (modField == displacementBy8Bit)
        {
            uint8_t rmMask = 0b00000111;
            uint8_t rm = rmMask & data[currentByte];
            if (rm == 0b00000110)
            {
                const char* reg = getRegister(data[currentByte], sub.w, 0b00111000);
                uint16_t displacementValue = get16BitDisplacement(data, currentByte);

                //We need to do this to add off the displacementValue.
                ++currentByte;
                if (sub.w)
                {
                    uint16_t value = signExtend(data, currentByte, sub.s);
                    vprint(instructionStringBuffer.appendUseF("%s [%d], word %d\n", sub.instructionName, displacementValue, value));
                }
                else
                {
                    uint8_t value = data[currentByte];
                    vprint(instructionStringBuffer.appendUseF("%s [%d], byte %d\n", sub.instructionName, displacementValue, value));
                }

                return true;
            }
            const char* effectiveAddress = effectiveAddressCalculations[rm];
            uint16_t displacementValue = data[++currentByte];

            //We need to do this to add off the displacement value.
            ++currentByte;
            if (sub.w)
            {
                uint16_t value = signExtend(data, currentByte, sub.s);
                vprint(instructionStringBuffer.appendUseF("%s [%s + %d], word %d\n", sub.instructionName, effectiveAddress, displacementValue, value));
            }
            else
            {
                uint8_t value = data[currentByte];
                vprint(instructionStringBuffer.appendUseF("%s [%s + %d], byte %d\n", sub.instructionName, effectiveAddress, displacementValue, value));
            }

            return true;
        }
        else if (modField == displacementBy16Bit)
        {
            uint8_t rmMask = 0b00000111;
            uint8_t rm = rmMask & data[currentByte];
            if (rm == 0b00000110)
            {
                const char* reg = getRegister(data[currentByte], sub.w, 0b00111000);
                uint16_t displacementValue = get16BitDisplacement(data, currentByte);

                //We need to do this to add off the displacementValue.
                ++currentByte;
                if (sub.w)
                {
                    uint16_t value = signExtend(data, currentByte, sub.s);
                    vprint(instructionStringBuffer.appendUseF("%s [%d], word %d\n", sub.instructionName, displacementValue, value));
                }
                else
                {
                    uint8_t value = data[currentByte];
                    vprint(instructionStringBuffer.appendUseF("%s [%d], byte %d\n", sub.instructionName, displacementValue, value));
                }

                return true;
            }
            const char* effectiveAddress = effectiveAddressCalculations[rm];
            uint16_t displacementValue = get16BitDisplacement(data, currentByte);

            //We need to do this to add off the displacement value.
            ++currentByte;
            if (sub.w)
            {
                uint16_t value = signExtend(data, currentByte, sub.s);
                vprint(instructionStringBuffer.appendUseF("%s [%s + %d], word %d\n", sub.instructionName, effectiveAddress, displacementValue, value));
            }
            else
            {
                uint8_t value = data[currentByte];
                vprint(instructionStringBuffer.appendUseF("%s [%s + %d], byte %d\n", sub.instructionName, effectiveAddress, displacementValue, value));
            }

            return true;
        }
    }
    break;
    case Sub::IMMIDATE_FROM_ACCUMULATOR://immediate to acculator - 1pad
    {
        if (sub.w)
        {
            uint16_t value = get16BitValue(data, currentByte);
            vprint(instructionStringBuffer.appendUseF("%s ax, %d\n", sub.instructionName, value));
        }
        else
        {
            uint8_t value = data[currentByte];
            vprint(instructionStringBuffer.appendUseF("%s al, %d\n", sub.instructionName, value));
        }
        return true;
    }
    break;
    }

    printBinary1(data[currentByte]);
    VOID_ERROR("Something has not parsed correctly at in the add parsing.\n");
    return false;
}