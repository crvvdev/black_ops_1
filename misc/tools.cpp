#include "..\includes.hpp"

namespace Tools
{
void Patch(void *dst, const void *src, size_t size)
{
    DWORD oldProtection = NULL;
    if (VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtection))
    {
        memcpy(dst, src, size);
        VirtualProtect(dst, size, oldProtection, &oldProtection);
    }
}
} // namespace Tools