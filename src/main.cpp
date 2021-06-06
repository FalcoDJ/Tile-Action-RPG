#include <iostream>

#ifdef DEBUG_APPLICATION

struct AllocationMetrics
{
    uint32_t TotalAllocated = 0;
    uint32_t TotalFreed = 0;

    uint32_t CurrentUsage() { return TotalAllocated - TotalFreed; }
};

static AllocationMetrics s_AllocationMetrics;

void* operator new(size_t size)
{
    s_AllocationMetrics.TotalAllocated += size;
    return malloc(size);
}

void operator delete(void* memory, size_t size)
{
    s_AllocationMetrics.TotalFreed += size;
    free(memory);
}

static void PrintMemoryUsage()
{
    std::cout << "Memory Usage: " << s_AllocationMetrics.CurrentUsage() << " bytes\n";
}

#endif

#include "Engine.hpp"

int main(int argc, char const *argv[])
{
    Engine demo;
    if (demo.Construct(demo.Canvas_Size.x, demo.Canvas_Size.y, demo.Canvas_Scale.x, demo.Canvas_Scale.y))
    demo.Start();

    #ifdef DEBUG_APPLICATION
        PrintMemoryUsage();
    #endif

    return 0;
}
