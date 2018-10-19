#include <gtest/gtest.h>
#include <malloc_finder/malloc_finder.h>

using namespace XBot::Utils;

class TestMallocFinder: public ::testing::Test {
    

protected:

     TestMallocFinder(){
         
     }

     virtual ~TestMallocFinder() {
     }

     virtual void SetUp() {
         
     }

     virtual void TearDown() {
     }
     
     
};

TEST_F(TestMallocFinder, apiExample)
{
    /* This malloc won't be detected */
    malloc(1);
    
    /* Inside the following section, malloc/free will be detected */
    MallocFinder::Enable();
    malloc(1);
    MallocFinder::Disable();
    
    /* Customize callback that is triggered by malloc/free detection with a lambda */
    MallocFinder::SetOnMalloc( [](){ std::cout << "my malloc callback" << std::endl; } );
    MallocFinder::Enable();
    malloc(1);
    MallocFinder::Disable();
    
    /* Throw on malloc */
    MallocFinder::SetThrowOnMalloc(true);
    
    malloc(1); // does not throw
    
    MallocFinder::Enable();
    malloc(1); // throws
    MallocFinder::Disable();
};



int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}