/* Source Citation:
 *  - Flushing fgets buffer: https://stackoverflow.com/a/38768287
 *
 */

// Project Header Files
#include "validation.h"

int main(int argc, char * argv[]) {

    // User Handle Buffer (No More than 16 characters
    char handleBuffer[MAX_HANDLE_SIZE];

    /* Capture User Input for user handle
     * Rules:
     *      - No spaces
     *      - Cannot be blank
     *      - No special characters
     *      - First character cannot be a number
     */
    printf("Hello there, please create a name for your online handle\n"
           "Inputs greater than 16 characters will be trimmed automatically.\n\n");
    do {
        printf(">>");
        if (fgets(handleBuffer, MAX_HANDLE_SIZE, stdin)) {
            // Find the newline
            char * newLine = strchr(handleBuffer, '\n');
            if (newLine) {
                *newLine = 0;
            } else {
                /*
                 * Clear up to the newline. This will prevent error spam when you input a huge line of
                 * invalid inputs like 40 '*' marks.
                 */
                scanf("%*[^\n]");
                scanf("%*c");
            }
        }
    } while(isValid(handleBuffer) == 0);

    printf("%s", handleBuffer);

    return 0;
}