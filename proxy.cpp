#include "proxy.h"

void Proxy::Init(HMODULE hProxy)
{
    ProxyModuleHandle = hProxy;
    
    OriginalModuleHandle = LoadLibraryW(L"d3d9-2.dll");
    if (OriginalModuleHandle == nullptr)
    {
        MessageBoxW(nullptr, L"Cannot load original d3d9.dll library", L"Proxy", MB_ICONERROR);
        ExitProcess(0);
    }

#define RESOLVE(fn) Original##fn = GetProcAddress(OriginalModuleHandle, #fn)
    RESOLVE(Direct3DCreate9);
#undef RESOLVE
}

#ifdef _WIN64
// 64位平台使用函数指针调用
void FakeDirect3DCreate9()                            { ((void(*)())Proxy::OriginalDirect3DCreate9)(); }
#else
// 32位平台使用原有的内联汇编
__declspec(naked) void FakeDirect3DCreate9()                            { __asm { jmp [Proxy::OriginalDirect3DCreate9] } }
#endif
