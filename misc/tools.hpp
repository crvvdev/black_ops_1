#pragma once

#define DEBUG_OUT(fmt, ...)                                                                                            \
    {                                                                                                                  \
        DWORD dwWritten = 0;                                                                                           \
        char buffer[1024]{};                                                                                           \
        sprintf_s(buffer, fmt, __VA_ARGS__);                                                                           \
        WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), buffer, strlen(buffer), &dwWritten, nullptr);                   \
    }

namespace Tools
{
void Patch(void *Address, const void *Bytes, size_t Len);
} // namespace Tools