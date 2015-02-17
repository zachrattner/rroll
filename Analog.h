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

#ifndef ANALOG_H
#define ANALOG_H

#include "Types.h"

/* Number of consecutive ADC samples to filter */
#define ANALOG_HIST_LEN  5

/* Structure representing the ADC history of an analog sensor */
typedef struct
{
    /* Stores the next index to write */
    uint8 idx;
    
    /* Cumulative number of writes into the buffer, up to ANALOG_HIST_LEN. Used
     * for filtering the first (ANALOG_HIST_LEN - 1) readings. */
    uint8 num_writes;
    
    /* The raw ADC values, on [0, 1024). Maps linearly from 0 to 5 V */
    uint16 val[ANALOG_HIST_LEN];
} analog_hist_t;

/* Global structure storing the analog sonar readings */
extern analog_hist_t SonarReadings;

/* Manages reading from analog input pins */
class Analog
{
    public:
        /* Initialize the given history structure */
        static void init(analog_hist_t* hist);
        
        /* Perform an ADC conversion on the specified pin, perform filtering, 
         * and return the filtered result. */
        static uint16 read(analog_hist_t* hist, uint8 pin);
};

#endif /* !ANALOG_H */
