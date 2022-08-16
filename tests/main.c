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
    test(catches, basic, 1,1,0,1,1);
    //test(&doesntCatch, &basic, 1,0,0,1,0); // unhandled exception

    test(catches, uncatchable, 1,0,1,1,1);
    //test(&doesntCatch, &uncatchable, 1,0,0,1,0); // unhandled exception

    test(catches, multipleThrows, 1,1,0,1,1);
    //test(&doesntCatch, &multipleThrows, 1,0,0,1,0); // unhandled exception

    test(catches, noThrow, 1,0,0,1,1);
    test(doesntCatch, noThrow, 1,0,0,1,1);

    printf("Tests completed");
    return 0;
}
