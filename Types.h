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

#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>       /* Integer types */
#include <Arduino.h>      /* Arduino APIs */
#include <SPI.h>          /* For communicating with the MP3 player shield */
#include <SdFat.h>        /* For decoding MP3 files off the SD card */
#include <SdFatUtil.h>    /* For interfacing with the SD card */
#include <SFEMP3Shield.h> /* For interfacing with the MP3 player shield */

/* ADC limits -- maps linearly from 0 to 5 V */
#define ADC_MIN 0
#define ADC_MAX 1023

/* Enable debug statements over the serial port */
/* #define DEBUG */

/* Debug statements with 0 through 3 variables to print */
#ifdef DEBUG
#include <Serial.h>

/* The serial baud rate defaults to 9600 on the Arduino IDE */
#define SERIAL_BAUD_RATE 9600

/* A message with 0 parameters to the format string */
#define MSG_0(str)          Serial.println(str)

/* A message with 1 parameter to the format string */
#define MSG_1(str, x)       Serial.print(str);\
                            Serial.println(x)
                      
/* A message with 2 parameters to the format string */
#define MSG_2(str, x, y)    Serial.print(str);\
                            Serial.print(x);\
                            Serial.print(" ");\
                            Serial.println(y)
                      
/* A message with 3 parameters to the format string */
#define MSG_3(str, x, y, z) Serial.print(str);\
                            Serial.print(x);\
                            Serial.print(" ");\
                            Serial.print(y);\
                            Serial.print(" ");\
                            Serial.println(z)
#else
/* Strip debug statements in release mode */
#define MSG_0(str)
#define MSG_1(str, x)
#define MSG_2(str, x, y)
#define MSG_3(str, x, y, z)
#endif /* DEBUG */

/* MP3 player */
#define MP3_TRACK_NUMBER 1
#define MP3_MAX_VOLUME   0
#define MP3_MED_VOLUME   50
#define MP3_BIT_RATE     192 /* kbps */
typedef SFEMP3Shield     MP3Player;

/*
 * Sonar parameters:
 * The ADC reads 0 through 1023, which maps linearly from 0 to 5 V. The sonar
 * unit maps linearly 9.8 mV of output per inch from the sensor to the remote 
 * object being detected.
 * 
 * 9.8 mV = 1 in
 * 5000 mV = 1023 ADC
 * 1 mV = (1023 / 5000) ADC
 * 1 in  = ((1023 * 9.8) / 5000) ADC
 * 
 * This macro converts inches to ADC units so they can be compared to sonar 
 * readings from the ADC. Do not covert ADC units to inches, since that 
 * direction loses precision.
 **/
#define IN_TO_ADC(in) ((in * 50127) / 25000)

/* Consider the sonar to be active when an object comes within this range of 
 * the sensor */
#define SONAR_DISTANCE_THRESH IN_TO_ADC(72)

/* Disallow sonar state changes within this many ms of each other */
#define MIN_SONAR_TIME_THRESH 1000

/* Default volume */
#define DEFAULT_VOLUME 50

/* Pin numbers for I/O ports */
#define SONAR_PIN 0

/* Integer types */
typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef int8_t   int8;
typedef int16_t  int16;
typedef int32_t  int32;
typedef int64_t  int64;

/* Makeshift bool type */
typedef uint8 boolean;

#ifndef NULL
#define NULL 0
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#endif /* !TYPES_H */
