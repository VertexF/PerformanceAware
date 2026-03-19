#include "GroupInstructions.hpp"

#include "Foundation/Assert.hpp"

namespace 
{
    constexpr uint8_t wMask = 0b00000001;
    constexpr uint8_t dMask = 0b00000010;
    constexpr uint8_t sMask = 0b00000010;

    constexpr uint8_t opCodeSpecial = 0b10000000;
    constexpr uint8_t specialOpcodeMask = 0b00111000;

    enum SpecialOpCodes
    {
        ADD_OP = 0b00000000,
        SUB_OP = 0b00101000,
        CMP_OP = 0b00111000,
    };
}

void groupInstructions(OpCodes& opcode, const char* const data, uint32_t currentByte)
{
    //This loop is looking getting the instruction category, so mov or push etc.
    //First we need to loop through all the possible instruction categories.
    for (uint32_t instructionIndex = 0; instructionIndex < instructionsTotal; ++instructionIndex)
    {
        //Then we need to get the instruction sub-categories size. So the total amount of mov instructions. 
        uint32_t sizeOfInstructionList = instructionListSize[instructionIndex];
        for (uint32_t currentInstructionlist = 0; currentInstructionlist < sizeOfInstructionList; ++currentInstructionlist)
        {
            //Here we double index into the instruction list to get the actual binary for the instruction + the padding.
            uint8_t byte = data[currentByte];
            uint8_t instruction = instructionsList[instructionIndex][currentInstructionlist];
            uint8_t instructionMask = instructionsListMask[instructionIndex][currentInstructionlist];
            uint8_t instructionByte = byte & instructionMask;

            if ((byte & instructionByte) == instruction)
            {
                if (opCodeSpecial == instructionByte)
                {
                    currentByte++;
                    uint8_t opCode = (data[currentByte] & specialOpcodeMask);
                    switch (opCode) 
                    {
                    case ADD_OP:
                        instructionIndex = 2;
                        break;
                    case SUB_OP:
                        instructionIndex = 3;
                        break;
                    case CMP_OP:
                        instructionIndex = 4;
                        break;
                    }
                }

                //Then we set the tag for our union we will use later to discard a lot of extra instructions.
                switch (instructionIndex)
                {
                case 0: //push
                    opcode.type = PUSH;
                    break;
                case 1: //move
                    opcode.type = MOV;
                    switch ((Move::MoveInstructionsType)instruction)
                    {
                        case Move::IMMIDATE_TO_REG_MEMORY: //immediate to register/memory - 1pad
                        {
                            opcode.move.types = Move::IMMIDATE_TO_REG_MEMORY;
                            opcode.move.w = (byte & wMask);
                        }
                        return;
                        case Move::REG_MEMORY_TO_FROM_MEMORY: //register/memory to/from register - 2pad
                        {
                            opcode.move.types = Move::REG_MEMORY_TO_FROM_MEMORY;
                            opcode.move.d = (byte & dMask);
                            opcode.move.w = (byte & wMask);
                        }
                        return;
                        case Move::IMMIDATE_TO_REG: //immediate to register - 4pad
                        {
                            opcode.move.types = Move::IMMIDATE_TO_REG;

                            uint8_t mask = 0b00001000;
                            opcode.move.w = (byte & mask);

                            mask = 0b00000111;
                            opcode.move.reg = (byte & mask);
                        }
                        return;
                        case Move::MEMORY_TO_ACCUMULATOR: //Memory to accumulator - 1pad
                        {
                            opcode.move.types = Move::MEMORY_TO_ACCUMULATOR;
                            opcode.move.w = (byte & wMask);
                        }
                        return;
                        case Move::ACCUMULATOR_TO_MEMORY: //Accumulator to memory - 1pad
                        {
                            opcode.move.types = Move::ACCUMULATOR_TO_MEMORY;
                            opcode.move.w = (byte & wMask);
                        }
                        return;
                        case Move::REG_MEMORY_TO_SEG_REG: //Register/ memory to segment register - 0pad
                            opcode.move.types = Move::REG_MEMORY_TO_SEG_REG;
                            return;
                        case Move::SEG_REG_TO_REG_MEMORY: //Segment register to register/memory - 0pad
                            opcode.move.types = Move::SEG_REG_TO_REG_MEMORY;
                            return;
                    }
                    break;
                case 2: //add
                    opcode.type = ADD;
                    switch ((Add::AddInstructionsType)instruction)
                    {
                        case Add::REG_MEMORY_WITH_TO_EITHER: //Reg/memory with either register to either - 2pad
                        {
                            opcode.add.types = Add::REG_MEMORY_WITH_TO_EITHER;
                            opcode.add.d = (byte & dMask);
                            opcode.add.w = (byte & wMask);
                        }
                        return;
                        case Add::IMMIDATE_TO_REG_MEMORY: //immediate to register - 2pad
                        {
                            opcode.add.types = Add::IMMIDATE_TO_REG_MEMORY;
                            opcode.add.s = (byte & sMask);
                            opcode.add.w = (byte & wMask);
                        }
                        return;
                        case Add::IMMIDATE_TO_ACCUMULATOR: //immediate to acculator - 1pad
                        {
                            opcode.add.types = Add::IMMIDATE_TO_ACCUMULATOR;
                            opcode.add.w = (byte & wMask);
                        }
                    }
                    break;
                    case 3: //sub
                        opcode.type = SUB;
                        switch ((Sub::SubInstructionsType)instruction)
                        {
                            case Sub::REG_MEMORY_WITH_TO_EITHER: //Reg/memory with either register to either - 2pad
                            {
                                opcode.sub.types = Sub::REG_MEMORY_WITH_TO_EITHER;
                                opcode.sub.d = (byte & dMask);
                                opcode.sub.w = (byte & wMask);
                            }
                            return;
                            case Sub::IMMIDATE_FROM_REG_MEMORY: //immediate to register - 2pad
                            {
                                opcode.sub.types = Sub::IMMIDATE_FROM_REG_MEMORY;
                                opcode.sub.s = (byte & sMask);
                                opcode.sub.w = (byte & wMask);
                            }
                            return;
                            case Sub::IMMIDATE_FROM_ACCUMULATOR: //immediate to acculator - 1pad
                            {
                                opcode.sub.types = Sub::IMMIDATE_FROM_ACCUMULATOR;
                                opcode.sub.w = (byte & wMask);
                            }
                            return;
                        }
                    break;

                    case 4: //cmp
                        opcode.type = CMP;
                        switch ((Cmp::CpmInstructionsType)instruction)
                        {
                        case Cmp::REG_MEMORY_AND_REG: //Reg/memory with either register to either - 2pad
                        {
                            opcode.cmp.types = Cmp::REG_MEMORY_AND_REG;
                            opcode.cmp.d = (byte & dMask);
                            opcode.cmp.w = (byte & wMask);
                        }
                        return;
                        case Cmp::IMMIDATE_WITH_REG_MEMORY: //immediate to register - 2pad
                        {
                            opcode.cmp.types = Cmp::IMMIDATE_WITH_REG_MEMORY;
                            opcode.cmp.s = (byte & sMask);
                            opcode.cmp.w = (byte & wMask);
                        }
                        return;
                        case Cmp::IMMIDATE_WITH_ACCUMULATOR: //immediate to acculator - 1pad
                        {
                            opcode.cmp.types = Cmp::IMMIDATE_WITH_ACCUMULATOR;
                            opcode.cmp.w = (byte & wMask);
                        }
                        return;
                        }
                        break;
                }
            }
        }
    }
}