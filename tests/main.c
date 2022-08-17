#include "main.h"

#define THROW_TEST() throw(EID_TEST, "test exception"); failThrow()
#define THROW_UNCATCHABLE() throw(420, "catch me if you can"); failThrow()

void basic(void)
{
    THROW_TEST();
}

void uncatchable(void)
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
    test(catches,     false, basic, 1,1,0,1,1,0);
    test(doesntCatch, true,  basic, 1,0,0,1,0,0);
    test(rethrows,    true,  basic, 1,1,0,1,0,1);

    test(catches,     false, uncatchable, 1,0,1,1,1,0);
    test(doesntCatch, true,  uncatchable, 1,0,0,1,0,0);
    test(rethrows,    true,  uncatchable, 1,0,1,1,0,1);

    test(catches,     false, multipleThrows, 1,1,0,1,1,0);
    test(doesntCatch, true,  multipleThrows, 1,0,0,1,0,0);
    test(rethrows,    true,  multipleThrows, 1,1,0,1,0,1);

    test(catches,     false, noThrow, 1,0,0,1,1,0);
    test(doesntCatch, false, noThrow, 1,0,0,1,1,0);
    test(rethrows,    false, noThrow, 1,0,0,1,1,0);

    printf("Tests completed");
    return 0;
}
