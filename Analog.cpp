/******************************************************************************
 * This file is part of rroll, an Arduino-based device that plays Rick Astley's 
 * "Never Gonna Give You Up" whenever an object enters a predefined area.
 *
 * rroll was released in January 2013 by Zach Rattner (info@zachrattner.com).
 *
 * rroll is free software: you can redistribute it and/or modify
 * it under the terms of the BSD 2-Clause license.
 *
 * Copyright (c) 2013, Zach Rattner
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/

#include "Types.h"
#include "Analog.h"

// Global structure storing the analog sonar readings
analog_hist_t SonarReadings;

// Initialize the given history structure
void Analog::init(analog_hist_t* hist)
{
    uint8 idx; // Loop iterator
    
    // NULL pointer check
    if (!hist)
    {
        return;
    }

    /* Zero out the history structure */
    hist->idx        = 0;
    hist->num_writes = 0;
    for (idx = 0; idx < ANALOG_HIST_LEN; idx++)
    {
        hist->val[idx] = 0;
    }
}

/* Perform an ADC conversion on the specified pin, perform filtering, 
 * and return the filtered result. */
uint16 Analog::read(analog_hist_t* hist, uint8 pin)
{
    uint8  idx = 0; // Loop iterator
    uint16 avg = 0; // Temporary variable to calculate the average value
    
    // NULL pointer check
    if (!hist)
    {
        return 0;
    }
    
    /* Read the pin */
    hist->val[hist->idx] = analogRead(pin);
    
    /* Record the write */
    if (hist->num_writes < ANALOG_HIST_LEN)
    {
        hist->num_writes++;
    }
    
    /* Sum the valid readings */
    for (idx = 0; idx < hist->num_writes; idx++)
    {
        avg += hist->val[idx];
    }
    
    /* Divide the sum to find the average */
    avg /= hist->num_writes;
    
    /* Record the write */
    hist->idx++;
    if (hist->idx >= ANALOG_HIST_LEN)
    {
        hist->idx = 0;
    }
    
    return avg;
}
