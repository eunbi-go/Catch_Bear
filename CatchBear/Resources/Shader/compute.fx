#ifndef _COMPUTE_FX_
#define _COMPUTE_FX_

#include "params.fx"

// UAV: Unordered Access View
// compute shader 전용으로 사용하는 레지스터

// RWTexture2D: Read, Write 모두 가능
RWTexture2D<float4> g_rwtex_0 : register(u0);

// 쓰레드 그룹당 쓰레드 개수
// max : 1024 (CS_5.0)
// - 하나의 쓰레드 그룹은 하나의 다중처리기에서 실행
[numthreads(1024, 1, 1)]    // 1024 * 1(x) * 1(y) 가 총 스레드(일감)의 개수 
void CS_Main(int3 threadIndex : SV_DispatchThreadID)
{
    if (threadIndex.y % 2 == 0)
        g_rwtex_0[threadIndex.xy] = float4(1.f, 0.f, 0.f, 1.f);
    else
        g_rwtex_0[threadIndex.xy] = float4(0.f, 1.f, 0.f, 1.f);
}

#endif