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
#include "Event.h"

/* Timestamp (in ms since device was turned on) of the last sonar state change. 
 * Used for debouncing. */
static uint32 s_LastSonarChange;

/* TRUE until music has been played at least once. */
static boolean s_FirstRun;

/* TRUE if there is currently an object in the sonar's sensing area, FALSE 
 * otherwise */
static boolean s_SonarActive;

/* The current volume of the MP3 player */
static uint16 s_Volume;

/* The interface to the MP3 player daughter board */
static MP3Player s_Player;

/* Initialize I/O pins and state variables */
void Event::init()
{    
    /* Update bookkeeping variables */
    s_FirstRun         = TRUE;
    s_SonarActive      = FALSE;
    s_Volume           = DEFAULT_VOLUME;
    s_LastSonarChange  = 0;
    
    /* Initialize the MP3 player */
    s_Player.begin();
    s_Player.setBitRate(MP3_BIT_RATE);
    s_Player.setVolume(MP3_MAX_VOLUME, MP3_MAX_VOLUME);
    MSG_0("Initializing event module");
}

/* Called when an object enters the sonar sensing area */
void Event::onSonarActive()
{
    /* Stop prematurely if the sonar is already active, or if not enough time
     * has passed since the last sonar state change. Enforce the time limit to
     * debounce the potentially noisy analog signal. */
    uint32 Delta = millis() - s_LastSonarChange;
    if (s_SonarActive || (Delta < MIN_SONAR_TIME_THRESH))
    {
        return;
    }

    /* Update sonar bookkeeping variables */
    s_LastSonarChange = millis();
    s_SonarActive = TRUE;

    if (s_FirstRun)
    {
        s_FirstRun = FALSE;
        s_Player.playTrack(MP3_TRACK_NUMBER);
    }
    else
    {
        s_Player.resumeDataStream();
    }
    
    MSG_1("Sonar activated, delta (ms): ", Delta);
}

/* Called when an object leaves the sonar sensing area */
void Event::onSonarDeactive()
{
    /* Stop prematurely if the sonar is not active, or if not enough time has
     * passed since the last sonar state change. Enforce the time limit to
     * debounce the potentially noisy analog signal. */
    uint32 Delta = millis() - s_LastSonarChange;
    if (!s_SonarActive || (Delta < MIN_SONAR_TIME_THRESH))
    {
        return;
    }

    /* Update sonar bookkeeping variables */
    s_LastSonarChange = millis();
    s_SonarActive = FALSE;
    
    s_Player.pauseDataStream();
    MSG_1("Sonar deactivated, delta (ms): ", Delta);
}
