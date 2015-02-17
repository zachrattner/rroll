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

#include <SPI.h>          /* For communicating with the MP3 player shield */
#include <SdFat.h>        /* For decoding MP3 files off the SD card */
#include <SdFatUtil.h>    /* For interfacing with the SD card */
#include <SFEMP3Shield.h> /* For interfacing with the MP3 player shield */
#include <Serial.h>       /* For printing debug messages */

#include "Types.h"
#include "Analog.h"
#include "Event.h"

/* Initialize the serial debugger, event manager, and I/O pins */
void setup()
{    
    #ifdef DEBUG
    Serial.begin(SERIAL_BAUD_RATE);
    #endif
    
    /* Initialize the analog sensor history for filtering */
    Analog::init(&SonarReadings);
    
    /* Initialize pin I/O modes */
    Event::init();
}

/* Monitor the sonar sensor and trigger entry/exit conditions*/
void loop()
{
    /* Read the sensor */
    uint16 Sonar = Analog::read(&SonarReadings, SONAR_PIN);
    
    /* Determine if some action needs to be done */
    if (Sonar < SONAR_DISTANCE_THRESH)
    {
        Event::onSonarActive();
    }
    else
    {
        Event::onSonarDeactive();            
    }

    /* Wait 1 ms */
    delay(1);
}
