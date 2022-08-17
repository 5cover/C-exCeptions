#include "main.h"
#include <signal.h>

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

void signalHandler(int sigId)
{
    if(sigId == SIGABRT)
    {
        printf("Is this correct? (y/n) ");
        char input = '\0';
        while(input != 'n' && input != 'y')
        {
            if(input != '\0')
            {
                printf("Invalid input (y/n) ");
            }
            scanf("%c", &input);
            while(getchar() != '\n');

        }
        assert(input == 'y');
        exit(EXIT_SUCCESS);
    }
}

int main(void)
{
    signal(SIGABRT, signalHandler);

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

    printf("\nAutomatic tests completed. Running unhandled exception test...\n");

    THROW_TEST();

    return 0;
}
