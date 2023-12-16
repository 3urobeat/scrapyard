/*
 * File: symmetricCipher.c
 * Project: scrapyard
 * Created Date: 12.12.2023 21:30:36
 * Author: 3urobeat
 *
 * Last Modified: 16.12.2023 20:13:07
 * Modified By: 3urobeat
 */


// Manual implementation of a symmetric cipher using a XOR operation on every bit, like you'd do on paper.
// Please note that this file was loosely put together for practice, don't judge the code quality. Thanks.

// Compile with gcc and run: gcc ./symmetricCipher.c -o symmetricCipher && ./symmetricCipher


#include <stdio.h> // Needed for stdin stdout stuff

#define MAX_INPUT 1023 // Max amount of chars
#define ARR_SIZE  8    // The size of all int arrays representing one byte as bits
#define DEBUG     0    // Logs more information. Set 0 to disable, 1 to enable.

// The key to encrypt with (length must be >= input)
const char *key = "fwJYzgj@C92oR92nV4ifKGnyQcKGWqJWTna7FvM&m^YReyTg4^38GQwemT^%NiC%Dk73xW@Mqp6t%RmfkZQR%3LwrGDeYPqBjMRtMe2#TPCaU8*hGU*W!MJPk6zQ@cCF4Hfywkoaz9djxwDWSztJ6UJ7xj!8Af7jqCyySNmRqqMKyKYwX@*2BRffB*rLhdK%D*Gd%GAf%D5CW47kvz5CpNWYfHB4zQnGBsW6zrxzgar@UBPz*qQZoN!YZ5tvQ$VkvApCRA7iZ5DSk45a@*EK4yCH@tq9Gcc45KZy7Gyi9Bf@2MS#HgHR!i3qCZ5B96%Wuj2RM97TTjSPpcjh4wTM9h%9eP!Tj3YVTTsJS@!tsnXu24mzNRueYeHvfuyvo!YKjK54NbD&Qt&UxWG$8U2iUBvhwZsvwfPuqpgtiB@kM@UE5gQH^cqWGnwoCmv%eN2bFpRQtrnKiZXs%mJZqjNe6dTdo8n6Nw54qE$aDp9EZxe*XEtvs4#S7xs%933aMPobSrkv3De4yuYwSXjrBc@tM&vnbKzQGhy4&A$QKBxmQpuNiJU%Mcon9wqJgtTs@Rf!n@C8SSJVe228RLw@DdpCMfUs%dKC@UmjeSJ6NkAaDhav9!Y*$Nqgw3EXQhriBVhY@@qVi8qyQ4DSWu@zKNNHAugEPmFcABXTgSdVHQbBv!UcQhNy&K^ADTnbkvp3udt$HwTetc%KDvc2Bg4%PgLtU9q*YV6s5K9cJXr3Gksv@PctN9ZTK%kmWx3oZ9T3fFYya*CS#$W!zpFyKRGq$S*piNBozX68HV3RjrVRP3g6bgzsZ57iSWoPTcCqU7QTRcYihRA2Z!ZCPi79Un#btELWcyMjnbQKaCA@yRm&XKf!MeerUMykGnYSzfMUZXEH@4mJW9@LSD*!7MJKgsC*%o%YaM&%bnrUAv9%Jr$LcCkZUU9@Q8whV4ebk^L%BhZkZiopeToYjRCgDH%KvdBkbzfRpwsvFADXS92ptr9tQWt@Z$7D5GUNwQrzA$oz@zFSMTS*jnJ8Jyb3Uw5qQ%HGPc&E4Xn9@gy33#UEPbiM*Lz$YoKzgseFjS%k2FHTMFMQwVX3";


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


// Reads a message from stdin and encodes it with key
void encode()
{
    // Read input (byte per byte to prevent overflow)
    char         input[MAX_INPUT + 1] = "";
    unsigned int inputLength = 0;

    printf("Please enter your text (max %d chars):\n", MAX_INPUT);

    while (inputLength < MAX_INPUT + 1) // Read until exceeding buffer or reaching newline
    {
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

    while (*p) // Iterate through input until reaching null byte
    {
        if (DEBUG) printf("\n");

        int index = p - input; // Specify index by calculating pointer offset (yes, I could have used a for loop but I wanted to do pointer arithmetic)

        // Manual way
        ascii_to_binary(tempInputBitsArr, (int) *p);
        ascii_to_binary(tempKeyBitsArr,   (int) key[index]);

        xor_byte(tempOutputBitsArr, tempInputBitsArr, tempKeyBitsArr);

        output[index] = (char) binary_to_ascii(tempOutputBitsArr);

        // Control using built-in XOR operator
        int control = ((int) *p) ^ ((int) key[index]); // XOR using ^ operator

        if (DEBUG) printf("Control: %d XOR %d = %d\n", (int) *p, (int) key[index], control);

        p++;
    }


    // Print result as a series of ints to avoid having to deal with unprintable chars
    printf("\nResult:\n");

    p = output; // Update p to point to first byte of output

    while (*p)
    {
        printf("%d ", (int) *p);
        p++;
    }

    printf("\n");
}


// Reads a series of spaced ints from stdin and decodes it with key to chars
void decode()
{
    // Read input which is a number of ints, separated with a space (byte per byte to prevent overflow, submit to input arr on space)
    char         input[MAX_INPUT + 1] = "";
    unsigned int inputLength = 0;

    int charBuffer = 0;

    printf("Please enter your text (max %d chars as ints, spaced apart):\n", MAX_INPUT);

    while (inputLength < MAX_INPUT + 1) // Read until exceeding buffer or reaching newline
    {
        char thisChar = (char) fgetc(stdin); // Read char from stdin

        // Submit to input array on space or newline, otherwise add to charBuffer
        if (thisChar == ' ' || thisChar == '\n') {
            if (DEBUG) printf("Input: Appending int %d\n", charBuffer);

            // Append char to input array
            input[inputLength] = (char) charBuffer;
            inputLength++;

            // Reset buffer
            charBuffer = 0;

        } else {

            // Multiply charBuffer with 10 to "make room" for the next digit
            charBuffer *= 10;

            // Add digit to charBuffer. Subtract 48 because '0' corresponds to ascii decimal 48
            charBuffer += (int) thisChar - 48;
        }

        // Break loop on linebreak (aka submit)
        if (thisChar == '\n') break;
    }


    // XOR each bit of input with key
    char output[MAX_INPUT + 1] = "";
    char *p = input; // Point to first byte of input

    int tempInputBitsArr[ARR_SIZE];
    int tempKeyBitsArr[ARR_SIZE];
    int tempOutputBitsArr[ARR_SIZE];

    while (*p) // Iterate through input until reaching null byte
    {
        if (DEBUG) printf("\n");

        int index = p - input; // Specify index by calculating pointer offset (yes, I could have used a for loop but I wanted to do pointer arithmetic)

        // Manual way
        ascii_to_binary(tempInputBitsArr, (int) *p);
        ascii_to_binary(tempKeyBitsArr,   (int) key[index]);

        xor_byte(tempOutputBitsArr, tempInputBitsArr, tempKeyBitsArr);

        output[index] = (char) binary_to_ascii(tempOutputBitsArr);

        // Control using built-in XOR operator
        int control = ((int) *p) ^ ((int) key[index]); // XOR using ^ operator

        if (DEBUG) printf("Control: %d XOR %d = %d\n", (int) *p, (int) key[index], control);

        p++;
    }


    // Print result
    printf("\nResult: %s\n", output);
}


// Entry point
int main()
{
    encode();
    printf("\n");
    decode();

    // Exit with UNIX error code 0
    return 0;
}
