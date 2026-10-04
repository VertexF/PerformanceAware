#ifndef CPU_DATA_HDR
#define CPU_DATA_HDR

#include <Foundation/Log.hpp>

static constexpr uint32_t instructionsTotal = 5;

static constexpr uint32_t moveInstructionsTotal = 7;
static constexpr uint32_t pushInstructionsTotal = 3;
static constexpr uint32_t addInstructionsTotal = 3;
static constexpr uint32_t subInstructionsTotal = 3;
static constexpr uint32_t cmpInstructionsTotal = 3;

//NOTE - We have information here about the instructions being tested because we are seperated them into arrays.
//This reduces the complexity of searching through instructions, because if we are in say 'pushInstructions' and we find the first byte is 11111111 
//now the rest is going to be a register/memory instruction.

//Logic is the only expection to this rule, the comparison operations need the entire 2 bytes to work that out. We can build in a special case for 
//this instruction.

static const char* narrowRegisters[8]
{
    "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"
};

static const char* wideRegisters[8]
{
    "ax", "cx", "dx", "bx", "sp", "bp", "si", "di"
};

static const char** registerTable[2]
{
    narrowRegisters,
    wideRegisters
};

static const char* effectiveAddressCalculations[8]
{
    "bx + si",
    "bx + di",
    "bp + si",
    "bp + di",
    "si",
    "di",
    "bp",
    "bx",
};

struct Move
{
    const char* instructionName = "mov";

    uint8_t reg;
    bool d;
    bool w;

    enum MoveInstructionsType : uint8_t
    {
        IMMIDATE_TO_REG_MEMORY = 0b11000110, //immediate to register/memory - 1pad
        REG_MEMORY_TO_FROM_MEMORY = 0b10001000, //register/memory to/from register - 2pad
        IMMIDATE_TO_REG = 0b10110000, //immediate to register - 4pad
        MEMORY_TO_ACCUMULATOR = 0b10100000, //Memory to accumulator - 1pad
        ACCUMULATOR_TO_MEMORY = 0b10100010, //Accumulator to memory - 1pad
        REG_MEMORY_TO_SEG_REG = 0b10001110, //Register/ memory to segment register - 0pad
        SEG_REG_TO_REG_MEMORY = 0b10001100, //Segment register to register/memory - 0pad
    };

    MoveInstructionsType types;

    static constexpr uint8_t moveInstructions[moveInstructionsTotal] =
    {
        0b11000110, //immediate to register/memory - 1pad
        0b10001000, //register/memory to/from register - 2pad
        0b10110000, //immediate to register - 4pad
        0b10100000, //Memory to accumulator - 1pad
        0b10100010, //Accumulator to memory - 1pad
        0b10001110, //Register/ memory to segment register - 0pad
        0b10001100, //Segment register to register/memory - 0pad
    };

    static constexpr uint8_t moveInstructionsMask[moveInstructionsTotal] =
    {
        0b11111110, //immediate to register/memory - 1pad
        0b11111100, //register/memory to/from register - 2pad
        0b11110000, //immediate to register - 4pad
        0b11111110, //Memory to accumulator - 1pad
        0b11111110, //Accumulator to memory - 1pad
        0b11111111, //Register/ memory to segment register - 0pad
        0b11111111, //Segment register to register/memory - 0pad
    };
};

struct Push
{
    const char* instructionName = "push";
    uint8_t reg;
    bool RM[3];
    bool mod[2];

    static constexpr uint8_t pushInstructions[pushInstructionsTotal] =
    {
        0b11111111, //Register/memory - 0pad
        0b01010000, //Register/ - 3pad
        0b00000110, //Segment register - 0pad
    };

    static constexpr uint8_t pushInstructionsMask[pushInstructionsTotal] =
    {
        0b11111111, //Register/memory - 0pad
        0b11111000, //Register/ - 3pad
        0b11111111, //Segment register - 0pad
    };
};

struct Add 
{
    const char* instructionName = "add";
    uint8_t reg;
    bool d;
    bool w; //use wide registers or/and wide data.
    bool s; //sign extend to 8bit to 16bit if w = 1

    enum AddInstructionsType : uint8_t
    {
        REG_MEMORY_WITH_TO_EITHER = 0b00000000, //Reg/memory with either register to either - 2pad
        IMMIDATE_TO_REG_MEMORY =    0b10000000, //immediate to register - 2pad
        IMMIDATE_TO_ACCUMULATOR =   0b00000100, //immediate to acculator - 1pad
    };

    AddInstructionsType types;

    static constexpr uint8_t addInstructionsMask[addInstructionsTotal]
    {
        0b11111100, //Reg/memory with either register to either - 2pad
        0b11111100, //immediate to register - 2pad
        0b11111110, //immediate to acculator - 1pad
    };

    static constexpr uint8_t addInstructions[addInstructionsTotal]
    {
        0b00000000, //Reg/memory with either register to either - 2pad
        0b10000000, //immediate to register - 2pad
        0b00000100, //immediate to acculator - 1pad
    };
};

struct Sub
{
    const char* instructionName = "sub";
    uint8_t reg;
    bool d;
    bool w; //use wide registers or/and wide data.
    bool s; //sign extend to 8bit to 16bit if w = 1

    enum SubInstructionsType : uint8_t
    {
        REG_MEMORY_WITH_TO_EITHER = 0b00101000, //Reg/memory with either register to either - 2pad
        IMMIDATE_FROM_REG_MEMORY =  0b10000000, //immediate from register - 2pad
        IMMIDATE_FROM_ACCUMULATOR = 0b00101100,   //immediate from acculator - 1pad
    };

    SubInstructionsType types;

    static constexpr uint8_t subInstructionsMask[subInstructionsTotal]
    {
        0b11111100, //Reg/memory with either register - 2pad
        0b11111100, //immediate from register - 2pad
        0b11111110, //immediate from acculator - 1pad
    };

    static constexpr uint8_t subInstructions[subInstructionsTotal]
    {
        0b00101000, //Reg/memory with either register - 2pad
        0b10000000, //immediate from register - 2pad
        0b00101100, //immediate from acculator - 1pad
    };
};

struct Cmp
{
    const char* instructionName = "cmp";
    uint8_t reg;
    bool d;
    bool w; //use wide registers or/and wide data.
    bool s; //sign extend to 8bit to 16bit if w = 1

    enum CpmInstructionsType : uint8_t
    {
        REG_MEMORY_AND_REG = 0b00111000, //Reg/memory and register - 2pad
        IMMIDATE_WITH_REG_MEMORY = 0b10000000, //immediate with register - 2pad
        IMMIDATE_WITH_ACCUMULATOR = 0b00111100,   //immediate with acculator - 1pad
    };

    CpmInstructionsType types;

    static constexpr uint8_t cmpInstructionsMask[cmpInstructionsTotal]
    {
        0b11111100, //Reg/memory and register - 2pad
        0b11111100, //immediate with register - 2pad
        0b11111110, //immediate with acculator - 1pad
    };

    static constexpr uint8_t cmpInstructions[cmpInstructionsTotal]
    {
        0b00111000, //Reg/memory and register - 2pad
        0b10000000, //immediate with register - 2pad
        0b00111100, //immediate with acculator - 1pad
    };
};

//These two arrays need to be coupled together. Meaning they need to match instruction type to instruction total.
static const uint8_t* instructionsList[instructionsTotal]
{
    Push::pushInstructions,
    Move::moveInstructions,
    Add::addInstructions,
    Sub::subInstructions,
    Cmp::cmpInstructions,
};

//These two arrays need to be coupled together. Meaning they need to match instruction type to instruction total.
static const uint8_t* instructionsListMask[instructionsTotal]
{
    Push::pushInstructionsMask,
    Move::moveInstructionsMask,
    Add::addInstructionsMask,
    Sub::subInstructionsMask,
    Cmp::cmpInstructionsMask,
};

static uint32_t instructionListSize[instructionsTotal]
{
    pushInstructionsTotal,
    moveInstructionsTotal,
    addInstructionsTotal,
    subInstructionsTotal,
    cmpInstructionsTotal,
};

enum instructionType : uint8_t
{
    PUSH,
    MOV,
    ADD,
    SUB,
    CMP,
    COUNT,
};

struct OpCodes
{
    instructionType type;
    Push push;
    Move move;
    Add add;
    Sub sub;
    Cmp cmp;
};

static void printBinary1(char num)
{
    for (char i = sizeof(char) * 8 - 1; i >= 0; i--)
    {
        vprint("%d", (num >> i) & 1);
    }
    vprint("\n");
}

static void printBinary1(uint16_t num)
{
    for (char i = sizeof(uint16_t) * 8 - 1; i >= 0; i--)
    {
        vprint("%d", (num >> i) & 1);
    }
    vprint("\n");
}

static const char* getRegister(uint8_t currentByte, bool is16BitWide, uint8_t registerMask) 
{
    if (registerMask == 0b00111000)
    {
        uint8_t registerIndex = (currentByte & registerMask) >> 3;
        return registerTable[is16BitWide][registerIndex];
    }
    else if (registerMask == 0b00000111)
    {
        uint8_t registerIndex = (currentByte & registerMask);
        return registerTable[is16BitWide][registerIndex];
    }
    return "Invalid mask";
}

static uint16_t get16BitDisplacement(const char* const data, uint32_t& currentByte) 
{
    uint16_t lowDisplacementMask = 0b0000000011111111;
    currentByte++;
    uint16_t lowDisplacement = data[currentByte] & lowDisplacementMask;
    currentByte++;
    uint16_t highDisplacement = data[currentByte];

    highDisplacement = highDisplacement << 8;
    return lowDisplacement | highDisplacement;
}

static uint16_t get16BitValue(const char* const data, uint32_t& currentByte)
{
    uint16_t lowValueMask = 0b0000000011111111;
    uint16_t lowValue = data[currentByte] & lowValueMask;
    currentByte++;
    uint16_t highValue = data[currentByte];

    highValue = highValue << 8;
    return lowValue | highValue;
}

static const char* getEffectiveAddressCalculation(uint8_t byte)
{
    uint8_t rmMask = 0b00000111;
    uint8_t rm = rmMask & byte;
    return effectiveAddressCalculations[rm];
}

static int16_t signExtend(const char* const data, uint32_t &currentByte, bool isSigned)
{
    int16_t value;
    //If the s has been set I'm going to assume it's a signed number because how can you sign extend a non-signed number...
    if (isSigned)
    {
        uint8_t signExtendMask = 0b10000000;
        bool isNegative = (data[currentByte] & signExtendMask);
        if (isNegative)
        {
            uint16_t nonNegativeMask = 0b1111111111111111;
            value = data[currentByte] & nonNegativeMask;
        }
        else
        {
            uint16_t nonNegativeMask = 0b0000000011111111;
            value = data[currentByte] & nonNegativeMask;
        }
    }
    else
    {
        value = get16BitValue(data, currentByte);
    }

    return value;
}

static int8_t signExtend8Bit(const char* const data, uint32_t& currentByte, bool isSigned)
{
    int8_t value;
    //If the s has been set I'm going to assume it's a signed number because how can you sign extend a non-signed number...
    if (isSigned)
    {
        uint8_t signExtendMask = 0b10000000;
        bool isNegative = (data[currentByte] & signExtendMask);
        if (isNegative)
        {
            uint8_t nonNegativeMask = 0b11111111;
            value = data[currentByte] & nonNegativeMask;
        }
        else
        {
            uint16_t nonNegativeMask = 0b00000000;
            value = data[currentByte] & nonNegativeMask;
        }
    }
    else
    {
        value = data[currentByte];
    }

    return value;
}

#endif // !CPU_DATA_HDR
