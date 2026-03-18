#ifndef CPU_DATA_HDR
#define CPU_DATA_HDR

#include <Foundation/Log.hpp>

static constexpr uint32_t instructionsTotal = 2;

static constexpr uint32_t moveInstructionsTotal = 7;
static constexpr uint32_t pushInstructionsTotal = 3;

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

//These two arrays need to be coupled together. Meaning they need to match instruction type to instruction total.
static const uint8_t* instructionsList[instructionsTotal]
{
    Push::pushInstructions,
    Move::moveInstructions,
};

//These two arrays need to be coupled together. Meaning they need to match instruction type to instruction total.
static const uint8_t* instructionsListMask[instructionsTotal]
{
    Push::pushInstructionsMask,
    Move::moveInstructionsMask,
};

static uint32_t instructionListSize[instructionsTotal]
{
    pushInstructionsTotal,
    moveInstructionsTotal,
};

enum instructionType : uint8_t
{
    PUSH,
    MOV,
    COUNT,
};

struct OpCodes
{
    instructionType type;
    Push push;
    Move move;
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

#endif // !CPU_DATA_HDR
