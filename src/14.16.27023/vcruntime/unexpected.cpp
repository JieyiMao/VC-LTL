//
// unexpected.cpp
//
//      Copyright (c) Microsoft Corporation. All rights reserved.
//
// The unexpected handler
//
#include <eh.h>
#include <vcruntime_internal.h>
#include <corecrt_internal.h>
#include <winapi_thunks.h>

static __forceinline unexpected_handler __cdecl get_unexpected_or_default(
    __acrt_ptd const* const ptd
    ) noexcept
{
#ifdef _ATL_XP_TARGETING
	if (__LTL_GetOsMinVersion() < 0x00060000)
		return ptd->XP_msvcrt._unexpected ? (unexpected_handler)ptd->XP_msvcrt._unexpected : &terminate;
	else
#endif
		return ptd->VistaOrLater_msvcrt._unexpected ? (unexpected_handler)ptd->VistaOrLater_msvcrt._unexpected : &terminate;
}

extern "C" unexpected_handler __cdecl _get_unexpected() noexcept
{
    return get_unexpected_or_default(__acrt_getptd());
}

#if 0 //由 set_unexpected.asm 转发
extern "C" unexpected_handler __cdecl set_unexpected(
    unexpected_handler const new_handler
    ) noexcept
{
    __vcrt_ptd* const ptd = __vcrt_getptd();

    unexpected_handler const old_handler = get_unexpected_or_default(ptd);

    ptd->_unexpected = new_handler;

    return old_handler;
}
#endif

#if 0 //由 unexpected.asm 转发
extern "C" void __cdecl unexpected() noexcept(false)
{
    unexpected_handler const handler = __vcrt_getptd()->_unexpected;
    if (handler)
    {
        handler();
    }

    terminate();
}
#endif