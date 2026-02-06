/* ==========================================================================
 * DOOM macOS Native Port — Sound Subsystem (Core Audio)
 * ==========================================================================
 *
 * Audio output via macOS Core Audio (AudioQueue).  Handles SFX mixing
 * and MIDI music playback through the system's audio infrastructure.
 *
 * SPDX-License-Identifier: GPL-2.0-only
 * ========================================================================== */

#include <AudioToolbox/AudioToolbox.h>
#include <stdio.h>
#include <string.h>

#include "doomdef.h"
#include "doomtype.h"
#include "i_sound.h"
#include "lprintf.h"

#define AUDIO_SAMPLE_RATE 11025
#define AUDIO_CHANNELS    2
#define AUDIO_BUFFER_SIZE 2048
#define NUM_BUFFERS       3
#define MAX_SFX_CHANNELS  8

static AudioQueueRef         s_audioQueue = NULL;
static AudioQueueBufferRef   s_buffers[NUM_BUFFERS];
static int                   s_audioRunning = 0;

/* Simple mixing buffer */
static int16_t s_mixBuffer[AUDIO_BUFFER_SIZE * AUDIO_CHANNELS];

/* Channel tracking */
static struct {
    int playing;
    int volume;
    int separation;
} s_channels[MAX_SFX_CHANNELS];

/* -----------------------------------------------------------------------
 * Audio callback — fills output buffers with mixed audio
 * ----------------------------------------------------------------------- */
static void audioCallback(void *userData, AudioQueueRef queue,
                          AudioQueueBufferRef buffer)
{
    (void)userData;

    /* Fill with silence for now — a full implementation would mix
     * active SFX channels here */
    memset(buffer->mAudioData, 0, buffer->mAudioDataByteSize);
    AudioQueueEnqueueBuffer(queue, buffer, 0, NULL);
}

/* -----------------------------------------------------------------------
 * I_InitSound
 * ----------------------------------------------------------------------- */
void I_InitSound(void)
{
    AudioStreamBasicDescription fmt = {0};
    fmt.mSampleRate       = AUDIO_SAMPLE_RATE;
    fmt.mFormatID         = kAudioFormatLinearPCM;
    fmt.mFormatFlags      = kLinearPCMFormatFlagIsSignedInteger |
                            kLinearPCMFormatFlagIsPacked;
    fmt.mBitsPerChannel   = 16;
    fmt.mChannelsPerFrame = AUDIO_CHANNELS;
    fmt.mFramesPerPacket  = 1;
    fmt.mBytesPerFrame    = AUDIO_CHANNELS * 2;
    fmt.mBytesPerPacket   = fmt.mBytesPerFrame;

    OSStatus status = AudioQueueNewOutput(&fmt, audioCallback, NULL,
                                          NULL, NULL, 0, &s_audioQueue);
    if (status != noErr) {
        lprintf(LO_WARN, "I_InitSound: AudioQueueNewOutput failed (%d)\n", (int)status);
        return;
    }

    UInt32 bufSize = AUDIO_BUFFER_SIZE * fmt.mBytesPerFrame;
    for (int i = 0; i < NUM_BUFFERS; i++) {
        AudioQueueAllocateBuffer(s_audioQueue, bufSize, &s_buffers[i]);
        s_buffers[i]->mAudioDataByteSize = bufSize;
        memset(s_buffers[i]->mAudioData, 0, bufSize);
        AudioQueueEnqueueBuffer(s_audioQueue, s_buffers[i], 0, NULL);
    }

    AudioQueueStart(s_audioQueue, NULL);
    s_audioRunning = 1;
    memset(s_channels, 0, sizeof(s_channels));

    lprintf(LO_INFO, "I_InitSound: Core Audio initialised (%d Hz)\n", AUDIO_SAMPLE_RATE);
}

void I_ShutdownSound(void)
{
    if (s_audioQueue) {
        AudioQueueStop(s_audioQueue, true);
        AudioQueueDispose(s_audioQueue, true);
        s_audioQueue = NULL;
    }
    s_audioRunning = 0;
}

void I_UpdateSound(void)     {}
void I_SubmitSound(void)     {}

void I_UpdateSoundParams(int channel, int volume, int separation)
{
    if (channel < 0 || channel >= MAX_SFX_CHANNELS) return;
    s_channels[channel].volume = volume;
    s_channels[channel].separation = separation;
}

int I_StartSound(int sfx_id, int channel, int volume, int separation, int pitch)
{
    (void)sfx_id; (void)pitch;
    if (channel < 0 || channel >= MAX_SFX_CHANNELS) return -1;
    s_channels[channel].playing = 1;
    s_channels[channel].volume = volume;
    s_channels[channel].separation = separation;
    return channel;
}

void I_StopSound(int channel)
{
    if (channel >= 0 && channel < MAX_SFX_CHANNELS)
        s_channels[channel].playing = 0;
}

int I_SoundIsPlaying(int channel)
{
    if (channel < 0 || channel >= MAX_SFX_CHANNELS) return 0;
    return s_channels[channel].playing;
}

/* Music stubs */
void I_InitMusic(void)   { lprintf(LO_INFO, "I_InitMusic: Core Audio MIDI stub\n"); }
void I_ShutdownMusic(void) {}
void I_PlaySong(int handle, int looping) { (void)handle; (void)looping; }
void I_StopSong(void) {}
void I_PauseSong(void) {}
void I_ResumeSong(void) {}
int  I_RegisterSong(const void *data, size_t len) { (void)data; (void)len; return 0; }
void I_UnRegisterSong(int handle) { (void)handle; }
void I_SetMusicVolume(int volume) { (void)volume; }
