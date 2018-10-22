#include <malloc_finder/malloc_finder.h>

namespace XBot { namespace Utils {

bool MallocFinder::_is_enabled = false;  
bool MallocFinder::_throw_on_malloc = false;
bool MallocFinder::_throw_on_free = false;
std::function<void(void)> MallocFinder::_f_malloc = [](){ printf("Malloc was called!\n");};
std::function<void(void)> MallocFinder::_f_free = [](){ printf("Free was called!\n");};

} }