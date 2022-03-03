#include "customProtocol.h"

/**
 * @brief Error function
 *
 * @param msg Error message
 */
void error(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

void timeout(void)
{
    int timeout_sec = 3;
    int timeout_milli = timeout_sec * 1000;
    // printf("Timeout: %d seconds\n", timeout_sec);
    int elapsed_time = clock() * 1000 / CLOCKS_PER_SEC;
    int end_time = elapsed_time + timeout_milli;
    while (elapsed_time <= end_time)
    {
        elapsed_time = clock() * 1000 / CLOCKS_PER_SEC;
    }
    // printf("Timeout reached\n");
}
