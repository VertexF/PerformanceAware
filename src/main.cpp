#include <Foundation/Array.hpp>
#include <Foundation/Memory.hpp>
#include <Foundation/Time.hpp>
#include <Foundation/Log.hpp>

int main() 
{
    //Init services
    MemoryService::instance()->init(void_giga(1ull), void_mega(8));
    timeServiceInit();

    HeapAllocator* allocator = &MemoryService::instance()->systemAllocator;
    StackAllocator scratchAllocator = MemoryService::instance()->scratchAllocator;

	return 0;
}