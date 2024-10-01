#pragma once

#include <stdint.h>
#include <cmath>
#include <stdio.h>

void printHex(const uint8_t* buf, uint16_t size);

static inline constexpr int pow10[] = {1, 10, 100, 1000, 10000, 100000, 1000000};

// This function prints a float like printf("%*.*f", val) would, but without actually using the printf float implementation, just integers 
// same format as printf, numDigits is including decimals and decimal point, numDecimals just the fraction part
static void printFloat(float val, int numDigits, int numDecimals) {
    // for float without decimals, just do direct integer printing
    if (numDecimals == 0) {
        printf("%*d", numDigits, (int)val);
        return;
    }

    if (numDecimals > 6) {
        return; // only does 6 decimals max right now
    }

    bool isNegative = (val < -0.001f);              // save negative 
    val = std::abs(val);                            // work with absolute value from now on
    
    int factor = pow10[numDecimals];                // Factor to shift decimals for extracting the fractional part
    val = val * factor + 0.5;                       // Apply rounding for the fractional part
    
    // Separate integer and fractional parts
    int intPart = (int)(val / factor);
    int fracPart = (int)((int)val % factor);

    // Calculate total length, needed for the integer and fractional part
    int intLen = 1, tempInt = intPart;
    
    // Count digits in the integer part
    if (intPart != 0) {
        intLen = 0;
        while (tempInt != 0) {
            tempInt /= 10;
            intLen++;
        }
    }

    // Calculate total length including decimal point and fractional part
    int totalLength = intLen + 1 + numDecimals;

    // Print leading spaces if total digits required are more than actual digits
    for (int i = totalLength; i < numDigits - isNegative; i++) {
        putchar(' ');
    }

    if (isNegative) {
        putchar('-');
    }

    // Print number
    printf("%d", intPart);                          
    putchar('.');
    printf("%0*d", numDecimals, fracPart); 
}