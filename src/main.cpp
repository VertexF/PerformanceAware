#include <Foundation/Array.hpp>
#include <Foundation/Memory.hpp>
#include <Foundation/Time.hpp>
#include <Foundation/Log.hpp>
#include <Foundation/File.hpp>
#include <Foundation/HashMap.hpp>
#include <Foundation/String.hpp>
 
#include "CPUData.hpp"
#include "MoveInstruction.hpp"
#include "GroupInstructions.hpp"

int main() 
{
    //Init services
    MemoryService::instance()->init(void_giga(1ull), void_mega(8));
    timeServiceInit();

    HeapAllocator* allocator = &MemoryService::instance()->systemAllocator;
    StackAllocator scratchAllocator = MemoryService::instance()->scratchAllocator;
    FileReadResult fileInput = fileReadBinary(BaseFilePath"\\computer_enhance\\perfaware\\part1\\listing_0040_challenge_movs", &scratchAllocator);
    char* data = fileInput.data;

    uint32_t currentByte = 0;

    StringBuffer instructionStringBuffer;
    instructionStringBuffer.init(2048, allocator);
    instructionStringBuffer.append("bits 16\n\n");

    OpCodes test1
    {
        .type = COUNT
    };

    while (currentByte < fileInput.size)
    {
        groupInstructions(test1, data[currentByte]);

        currentByte++;

        //Now we know the category of instruction we can loop.
        //This starts with the second byte.
        switch (test1.type)
        {
        case PUSH:
            break;
        case MOV:
            if (handleMoveInstruction(test1.move, data, instructionStringBuffer, currentByte))
            {
                continue;
            }
            break;
        }
    }

    fileWriteText("InstructionDecode.txt", instructionStringBuffer.data, instructionStringBuffer.currentSize);

    instructionStringBuffer.shutdown();

    MemoryService::instance()->shutdown();

	return 0;
}