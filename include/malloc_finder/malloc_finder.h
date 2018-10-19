#ifndef __CATCH_MALLOC_H__
#define __CATCH_MALLOC_H__

#include <malloc.h>
#include <exception>
#include <iostream>
#include <functional>

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
        
    private:
        
        static bool _is_enabled;
        static bool _throw_on_malloc;
        static bool _throw_on_free;
        static std::function<void(void)> _f_malloc;
        static std::function<void(void)> _f_free;
        
    };
    
    bool MallocFinder::_is_enabled = false;  
    bool MallocFinder::_throw_on_malloc = false;
    bool MallocFinder::_throw_on_free = false;
    std::function<void(void)> MallocFinder::_f_malloc = [](){ printf("Malloc was called!\n");};
    std::function<void(void)> MallocFinder::_f_free = [](){ printf("Free was called!\n");};
    
} }

/* Declare a function pointer into which we will store the default malloc */
static void * (* prev_malloc_hook)(size_t, const void *);
static void   (* prev_free_hook)(void *, const void *);

/* Ignore deprecated __malloc_hook */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

static void * testing_malloc(size_t size, const void * caller);
static void  testing_free(void * addr, const void * caller);

class MallocHookGuard
{
public:
    
    MallocHookGuard()
    {
        __malloc_hook = prev_malloc_hook;
        __free_hook = prev_free_hook;
    }
    
    ~MallocHookGuard()
    {
        __malloc_hook = testing_malloc;
        __free_hook = testing_free;
    }
};

/* Custom malloc */
static void * testing_malloc(size_t size, const void * caller)
{
    (void)caller;

    /* Set the malloc implementation to the default malloc hook so that we can call it
    * (otherwise, infinite recursion) */
    MallocHookGuard hook_guard;

    if (XBot::Utils::MallocFinder::IsEnabled()) {

        XBot::Utils::MallocFinder::OnMalloc();

        if(XBot::Utils::MallocFinder::ThrowsOnMalloc())
        {
            throw std::runtime_error("ThrowOnMalloc is enabled");
        }

    }

    // Execute the requested malloc.
    void * mem = malloc(size);
    
    // Set the malloc hook back to this function, so that we can intercept future mallocs.
    return mem;
}


/* Custom free */
static void  testing_free(void * addr, const void * caller)
{
    (void)caller;

    /* Set the malloc implementation to the default malloc hook so that we can call it
    * (otherwise, infinite recursion) */
    __free_hook = prev_free_hook;

    if (XBot::Utils::MallocFinder::IsEnabled()) {

        XBot::Utils::MallocFinder::OnFree();

        if(XBot::Utils::MallocFinder::ThrowsOnFree())
        {
            throw std::runtime_error("ThrowOnFree is enabled");
        }

    }

    // Execute the requested malloc.
    free(addr);
    
    // Set the malloc hook back to this function, so that we can intercept future mallocs.
    __free_hook = testing_free;

    
}


/// Function to be called when the malloc hook is initialized.
void init_malloc_hook()
{
  // Store the default malloc.
  prev_malloc_hook = __malloc_hook;
  prev_free_hook = __free_hook;
  // Set our custom malloc to the malloc hook.
  __malloc_hook = testing_malloc;
  __free_hook = testing_free;
}
#pragma GCC diagnostic pop

/// Set the hook for malloc initialize so that init_malloc_hook gets called.
void(*volatile __malloc_initialize_hook)(void) = init_malloc_hook;

#endif