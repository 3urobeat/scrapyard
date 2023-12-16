/*
 * File: symmetricCipher.c
 * Project: scrapyard
 * Created Date: 12.12.2023 21:30:36
 * Author: 3urobeat
 *
 * Last Modified: 16.12.2023 15:46:15
 * Modified By: 3urobeat
 */


// Manual implementation of a symmetric cipher using a XOR operation on every bit, like you'd do on paper.
// Please note that this file was loosely put together for practice, don't judge the code quality. Thanks.

// Compile with gcc and run: gcc ./symmetricCipher.c -o symmetricCipher && ./symmetricCipher


#include <stdio.h> // Needed for stdin stdout stuff

#define MAX_INPUT 1023 // Max amount of chars
#define ARR_SIZE  8    // The size of all int arrays representing one byte as bits
#define DEBUG     1    // Logs more information. Set 0 to disable, 1 to enable.

const char *key = "fwfywkoaz9djxwDWSztJ6UJozX68HV3RjrVRP3g6bgzsZ57iSWoPTcCqU7QTRcYi"; // The key to encrypt with (length must be >= input)


// Converts a sequence of bits stored in an int array to an int (ascii character)
int binary_to_ascii(int *inArr)
{
    int result = 0;

    for (int i = 0; i < ARR_SIZE; i++)
    {
        // Calculate exponent like an idiot (pow() for plebs)
        int exp = 1;
        for (int j = 0; j < ARR_SIZE - i - 1; j++) exp *= 2;

        // Multiply bit with exponent and add to result
        result += *(inArr + i) * exp;
    }

    // Log result if debug
    if (DEBUG) {
        for (int j = 0; j < ARR_SIZE; j++) printf("%d", *(inArr + j));
        printf(": %c (ascii %d)\n", (char) result, result);
    }

    return result;
}


// XORs every bit inside aArr with the corresponding bit in bArr and writes the result into outArr
void xor_byte(int *outArr, int *aArr, int *bArr)
{
    for (int i = 0; i < ARR_SIZE; i++)
    {
        int a = *(aArr + i);
        int b = *(bArr + i);

        *(outArr + i) = (!a && b) || (a && !b); // XOR written as a logical expression
    }

    // Log result if debug
    if (DEBUG) {
        printf("XOR result: ");
        for (int j = 0; j < ARR_SIZE; j++) printf("%d", *(outArr + j));
        printf("\n");
    }
}


// Converts an int (ascii character) to a sequence of bits stored in an int array
void ascii_to_binary(int *outArr, int inChar)
{
    // Clear output array
    for (int i = 0; i < ARR_SIZE; i++)
    {
        outArr[i] = 0;
    }


    // Add bit after bit, starting from the right (= at index 7)
    int temp = inChar;

    for (int i = ARR_SIZE - 1; i > 0; i--)
    {
        // Set bit to 1 if the number doesn't divide perfectly (= division with remainder)
        *(outArr + i) = (int) (temp % 2 == 1);

        // Divide temp by 2 for the next iteration. Decimals will be lost as temp is an int
        temp = temp / 2;

        // Break loop if we have reached 0
        if (temp == 0) break;
    }


    // Log result if debug
    if (DEBUG) {
        printf("%c (ascii %d): ", (char) inChar, inChar);
        for (int j = 0; j < ARR_SIZE; j++) printf("%d", *(outArr + j));
        printf("\n");
    }
}


// Entry point
int main()
{
    // Read input (byte per byte to prevent overflow)
    char         input[MAX_INPUT + 1] = "";
    unsigned int inputLength = 0;

    printf("Please enter your text (max %d chars):\n", MAX_INPUT);

    while (inputLength < MAX_INPUT + 1) { // Read until exceeding buffer or reaching newline
        char thisChar = (char) fgetc(stdin); // Read char from stdin

        // Break loop on linebreak (aka submit)
        if (thisChar == '\n') break;

        // Append char to input array
        input[inputLength] = thisChar;
        inputLength++;
    }

    input[inputLength + 1] = '\0'; // Make sure we have a null byte at the end


    // XOR each bit of input with key
    char output[MAX_INPUT + 1] = "";
    char *p = input; // Point to first byte of input

    int tempInputBitsArr[ARR_SIZE];
    int tempKeyBitsArr[ARR_SIZE];
    int tempOutputBitsArr[ARR_SIZE];

    while (*p) { // Iterate through input until reaching null byte
        if (DEBUG) printf("\n");

        int index = p - input; // Specify index by calculating pointer offset (yes, I could have used a for loop but I wanted to do pointer arithmetic)

        // Manual way
        ascii_to_binary(tempInputBitsArr, (int) *p);
        ascii_to_binary(tempKeyBitsArr,   (int) key[index]);

        xor_byte(tempOutputBitsArr, tempInputBitsArr, tempKeyBitsArr);

        output[index] = (char) binary_to_ascii(tempOutputBitsArr);

        // Control using built-in XOR operator
        int control = ((int) *p) ^ ((int) key[index]); // XOR using ^ operator

        printf("Control: %d XOR %d = %d\n", (int) *p, (int) key[index], control);

        p++;
    }


    // Exit with UNIX error code 0
    return 0;
}
