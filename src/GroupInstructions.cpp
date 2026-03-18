#include "GroupInstructions.hpp"

#include "Foundation/Assert.hpp"

void groupInstructions(OpCodes& opcode, uint8_t byte) 
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
            uint8_t instruction = instructionsList[instructionIndex][currentInstructionlist];
            uint8_t instructionMask = instructionsListMask[instructionIndex][currentInstructionlist];
            uint8_t instructionByte = byte & instructionMask;

            if ((byte & instructionByte) == instruction)
            {
                //Then we set the tag for our union we will use later to discard a lot of extra instructions.
                switch (instructionIndex)
                {
                case 0:
                    opcode.type = PUSH;
                    break;
                case 1:
                    opcode.type = MOV;
                    switch ((Move::MoveInstructionsType)instruction)
                    {
                    case Move::IMMIDATE_TO_REG_MEMORY: //immediate to register/memory - 1pad
                    {
                        opcode.move.types = Move::IMMIDATE_TO_REG_MEMORY;
                        uint8_t wMask = 0b00000001;
                        opcode.move.w = (byte & wMask);
                    }
                    break;
                    case Move::REG_MEMORY_TO_FROM_MEMORY: //register/memory to/from register - 2pad
                    {
                        opcode.move.types = Move::REG_MEMORY_TO_FROM_MEMORY;
                        uint8_t dMask = 0b00000010;
                        opcode.move.d = (byte & dMask);

                        uint8_t wMask = 0b00000001;
                        opcode.move.w = (byte & wMask);
                    }
                    break;
                    case Move::IMMIDATE_TO_REG: //immediate to register - 4pad
                    {
                        opcode.move.types = Move::IMMIDATE_TO_REG;

                        uint8_t mask = 0b00001000;
                        opcode.move.w = (byte & mask);

                        mask = 0b00000111;
                        opcode.move.reg = (byte & mask);
                    }
                    break;
                    case Move::MEMORY_TO_ACCUMULATOR: //Memory to accumulator - 1pad
                    {
                        opcode.move.types = Move::MEMORY_TO_ACCUMULATOR;
                        uint8_t wMask = 0b00000001;
                        opcode.move.w = (byte & wMask);
                    }
                    break;
                    case Move::ACCUMULATOR_TO_MEMORY: //Accumulator to memory - 1pad
                    {
                        opcode.move.types = Move::ACCUMULATOR_TO_MEMORY;
                        uint8_t wMask = 0b00000001;
                        opcode.move.w = (byte & wMask);
                    }
                    break;
                    case Move::REG_MEMORY_TO_SEG_REG: //Register/ memory to segment register - 0pad
                        opcode.move.types = Move::REG_MEMORY_TO_SEG_REG;
                        break;
                    case Move::SEG_REG_TO_REG_MEMORY: //Segment register to register/memory - 0pad
                        opcode.move.types = Move::SEG_REG_TO_REG_MEMORY;
                        break;
                    }
                    break;
                default:
                    VOID_ERROR("Instruction not supported");
                }

                break;
            }
        }
    }
}