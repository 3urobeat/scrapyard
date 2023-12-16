/*
 * File: symmetricCipher.c
 * Project: scrapyard
 * Created Date: 12.12.2023 21:30:36
 * Author: 3urobeat
 *
 * Last Modified: 16.12.2023 13:29:23
 * Modified By: 3urobeat
 */


// Compile with gcc and run: gcc ./symmetricCipher.c -o symmetricCipher && ./symmetricCipher

// Please note that this file was loosely put together for practice, don't judge the code quality. Thanks.


#include <stdio.h>


const char *key = "fwfywkoaz9djxwDWSztJ6UJozX68HV3RjrVRP3g6bgzsZ57iSWoPTcCqU7QTRcYi"; // The key to encrypt with (length must be >= input)

const int debug = 0; // Logs more information. Set 0 to disable, 1 to enable.


// XORs every bit inside aArr with the corresponding bit in bArr and writes the result into outArr
void xor_byte(int *outArr, int *aArr, int *bArr)
{

}


// Converts an int (ascii character) to a sequence of bits stored in an int array
void ascii_to_binary(int *outArr, int inChar)
{
    // Clear output array
    for (int i = 0; i < 8; i++)
    {
        outArr[i] = 0;
    }


    // Add bit after bit, starting from the right (= at index 7)
    int temp = inChar;

    for (int i = 7; i > 0; i--)
    {
        // Set bit to 1 if the number doesn't divide perfectly (= division with remainder)
        *(outArr + i) = (int) (temp % 2 == 1);

        // Divide temp by 2 for the next iteration. Decimals will be lost as temp is an int
        temp = temp / 2;

        // Break loop if we have reached 0
        if (temp == 0) break;
    }


    // Uncomment to see the result
    if (debug) {
        printf("%c (ascii %d): ", (char) inChar, inChar);
        for (int j = 0; j < 8; j++) printf("%d", *(outArr + j));
        printf("\n");
    }
}


// Entry point
int main()
{
    // Read input (byte per byte to prevent overflow)
    char         input[1024] = "";
    unsigned int inputLength = 0;

    printf("Please enter your text (max 1023 chars):\n");

    while (inputLength < 1024) { // Read until exceeding buffer or reaching newline
        char thisChar = (char) fgetc(stdin); // Read char from stdin

        // Break loop on linebreak (aka submit)
        if (thisChar == '\n') break;

        // Append char to input array
        input[inputLength] = thisChar;
        inputLength++;
    }

    input[inputLength + 1] = '\0'; // Make sure we have a null byte at the end


    // XOR each bit of input with key
    char output[1024] = "";
    char *p           = input; // Point to first byte of input

    int tempInputBitsArr[8];
    int tempKeyBitsArr[8];
    int tempOutputBitsArr[8];

    while (*p) { // Iterate through input until reaching null byte
        ascii_to_binary(tempInputBitsArr, (int) *p);
        ascii_to_binary(tempKeyBitsArr,   (int) key[p - input]); // Get char from key by calculating pointer offset

        xor_byte(tempOutputBitsArr, tempInputBitsArr, tempKeyBitsArr);

        p++;
    }


    // Exit with UNIX error code 0
    return 0;
}
