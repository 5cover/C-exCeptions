#include "main.h"

#define THROW_TEST() throw(EID_TEST, "test exception"); failThrow()
#define THROW_UNCATCHABLE() throw(420, "catch me if you can"); failThrow()

void basic(void)
{
    THROW_TEST();
}
void tcatchAll(void)
{
    THROW_UNCATCHABLE();
}
void multipleThrows(void)
{
    THROW_TEST();
    THROW_UNCATCHABLE();
}
void noThrow(void)
{

}
int main(void)
{
    test(&basic, 1,1,0,1);
    test(&tcatchAll, 1,0,1,1);
    test(&multipleThrows, 1,1,0,1);
    test(&noThrow, 1,0,0,1);

    //test(&unhandled, 1,0,0,1);

    printf("Tests completed");
    return 0;
}
