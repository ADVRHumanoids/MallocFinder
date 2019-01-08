#ifndef __CATCH_MALLOC_H__
#define __CATCH_MALLOC_H__

#include <malloc.h>
#include <exception>
#include <iostream>
#include <functional>
#include <execinfo.h>
#include <atomic>

namespace XBot { namespace Utils {
    
    class MallocFinder
    {
    public:
        
        
        static void SetThrowOnMalloc(bool throw_on_malloc)
        {
            _throw_on_malloc = throw_on_malloc;
        }
        
        static bool ThrowsOnMalloc()
        {
            return _throw_on_malloc;
        }
        
        static void SetThrowOnFree(bool throw_on_free)
        {
            _throw_on_free = throw_on_free;
        }
        
        static bool ThrowsOnFree()
        {
            return _throw_on_free;
        }
        
        static void Enable()
        {
            _is_enabled = true;
        }
        
        static void Disable()
        {
            _is_enabled = false;
        }
        
        static bool IsEnabled()
        {
            return _is_enabled;
        }
        
        static void OnMalloc()
        {
            _f_malloc();
        }
        
        static void OnFree()
        {
            _f_free();
        }
        
        static void SetOnMalloc(std::function<void(void)> f)
        {
            _f_malloc = f;
        }
        
        static void SetOnFree(std::function<void(void)> f)
        {
            _f_free = f;
        }
        
        static void PrintBacktrace()
        {
            void *bt[32];
            int nentries;

            nentries = backtrace (bt, sizeof(bt)/sizeof(bt[0]));
            
            backtrace_symbols_fd (bt, nentries, fileno(stdout));
        }
        
    private:
        
        static std::atomic<bool> _is_enabled;
        static std::atomic<bool> _throw_on_malloc;
        static std::atomic<bool> _throw_on_free;
        static std::function<void(void)> _f_malloc;
        static std::function<void(void)> _f_free;
        
    };
    
    
} }

#pragma GCC diagnostic pop


#define MALLOC_FINDER_INIT void init_malloc_hook()\
{\
  prev_malloc_hook = __malloc_hook;\
  prev_free_hook = __free_hook;\
  __malloc_hook = testing_malloc;\
  __free_hook = testing_free;\
}\
void(*volatile __malloc_initialize_hook)(void) = init_malloc_hook;



#endif