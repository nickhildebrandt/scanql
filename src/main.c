#include <stdio.h>

/**
 * main - Program entry point
 * @argc: Number of command line arguments
 * @argv: Array of command line argument strings
 *
 * Return: 0 on success, non-zero on usage error
 */
int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <SQL-String>\n", argv[0]);
        return 1;
    }
    printf("Received SQL String: %s\n", argv[1]);
    return 0;
}
