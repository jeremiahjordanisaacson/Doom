/* ==========================================================================
 * DOOM Windows Native Port — Sound Subsystem (DirectSound)
 * ==========================================================================
 *
 * DirectSound-based audio for SFX and music playback.  Replaces the
 * SDL_mixer-based audio from the shared engine.
 *
 * SFX:   Primary buffer + secondary buffers (one per channel)
 * Music: MIDI playback via midiOut* or secondary buffer for MUS->WAV
 *
 * SPDX-License-Identifier: GPL-2.0-only
 * ========================================================================== */

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dsound.h>
#include <mmsystem.h>
#include <stdio.h>

#include "doomdef.h"
#include "doomtype.h"
#include "i_sound.h"
#include "s_sound.h"
#include "w_wad.h"
#include "lprintf.h"

extern HWND g_hWnd;

/* DirectSound objects */
static LPDIRECTSOUND       s_pDS = NULL;
static LPDIRECTSOUNDBUFFER s_pPrimary = NULL;

#define MAX_CHANNELS 8

static struct {
    LPDIRECTSOUNDBUFFER buffer;
    int playing;
    int id;
} s_channels[MAX_CHANNELS];

/* -----------------------------------------------------------------------
 * I_InitSound — Initialise DirectSound
 * ----------------------------------------------------------------------- */
void I_InitSound(void)
{
    HRESULT hr;

    hr = DirectSoundCreate(NULL, &s_pDS, NULL);
    if (FAILED(hr)) {
        lprintf(LO_WARN, "DirectSoundCreate failed (0x%08lx)\n", hr);
        return;
    }

    hr = IDirectSound_SetCooperativeLevel(s_pDS, g_hWnd, DSSCL_PRIORITY);
    if (FAILED(hr)) {
        lprintf(LO_WARN, "SetCooperativeLevel failed (0x%08lx)\n", hr);
    }

    /* Create primary buffer */
    DSBUFFERDESC dsBD = {0};
    dsBD.dwSize  = sizeof(DSBUFFERDESC);
    dsBD.dwFlags = DSBCAPS_PRIMARYBUFFER;

    hr = IDirectSound_CreateSoundBuffer(s_pDS, &dsBD, &s_pPrimary, NULL);
    if (SUCCEEDED(hr)) {
        /* Set primary buffer format: 11025 Hz, 16-bit, mono (DOOM standard) */
        WAVEFORMATEX wfx = {0};
        wfx.wFormatTag      = WAVE_FORMAT_PCM;
        wfx.nChannels       = 2;
        wfx.nSamplesPerSec  = 11025;
        wfx.wBitsPerSample  = 16;
        wfx.nBlockAlign     = wfx.nChannels * wfx.wBitsPerSample / 8;
        wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
        IDirectSoundBuffer_SetFormat(s_pPrimary, &wfx);
    }

    memset(s_channels, 0, sizeof(s_channels));
    lprintf(LO_INFO, "I_InitSound: DirectSound initialised\n");
}

/* -----------------------------------------------------------------------
 * I_ShutdownSound
 * ----------------------------------------------------------------------- */
void I_ShutdownSound(void)
{
    for (int i = 0; i < MAX_CHANNELS; i++) {
        if (s_channels[i].buffer) {
            IDirectSoundBuffer_Stop(s_channels[i].buffer);
            IDirectSoundBuffer_Release(s_channels[i].buffer);
            s_channels[i].buffer = NULL;
        }
    }
    if (s_pPrimary) { IDirectSoundBuffer_Release(s_pPrimary); s_pPrimary = NULL; }
    if (s_pDS)      { IDirectSound_Release(s_pDS);            s_pDS = NULL; }
}

/* -----------------------------------------------------------------------
 * I_UpdateSound — Mix audio for the current frame
 * ----------------------------------------------------------------------- */
void I_UpdateSound(void)
{
    /* DirectSound handles mixing in hardware/software automatically.
     * We just need to check for completed buffers. */
    for (int i = 0; i < MAX_CHANNELS; i++) {
        if (s_channels[i].buffer && s_channels[i].playing) {
            DWORD status;
            IDirectSoundBuffer_GetStatus(s_channels[i].buffer, &status);
            if (!(status & DSBSTATUS_PLAYING)) {
                s_channels[i].playing = 0;
            }
        }
    }
}

/* -----------------------------------------------------------------------
 * I_SubmitSound — Submit mixed audio to output device
 * ----------------------------------------------------------------------- */
void I_SubmitSound(void)
{
    /* Not needed for DirectSound — buffers play independently */
}

/* -----------------------------------------------------------------------
 * I_UpdateSoundParams — Update channel volume/pan (called by engine)
 * ----------------------------------------------------------------------- */
void I_UpdateSoundParams(int channel, int volume, int separation)
{
    if (channel < 0 || channel >= MAX_CHANNELS) return;
    if (!s_channels[channel].buffer) return;

    /* Convert DOOM volume (0-15) to DirectSound dB (0 to -10000) */
    LONG dsVol = (volume == 0) ? DSBVOLUME_MIN : (LONG)(-2000 + volume * 133);
    IDirectSoundBuffer_SetVolume(s_channels[channel].buffer, dsVol);

    /* Convert separation (0-255, 128=center) to DirectSound pan */
    LONG dsPan = (LONG)((separation - 128) * 78); /* approx -10000..10000 */
    IDirectSoundBuffer_SetPan(s_channels[channel].buffer, dsPan);
}

/* -----------------------------------------------------------------------
 * I_StartSound — Start playing a sound effect
 * ----------------------------------------------------------------------- */
int I_StartSound(int sfx_id, int channel, int volume, int separation, int pitch)
{
    (void)pitch; /* TODO: pitch shifting via frequency adjustment */

    if (channel < 0 || channel >= MAX_CHANNELS) return -1;
    if (!s_pDS) return -1;

    /* TODO: Load sound data from WAD lump, create secondary buffer,
     * lock/fill/unlock, then play.  For now this is a stub. */

    return channel;
}

/* -----------------------------------------------------------------------
 * I_StopSound — Stop a playing channel
 * ----------------------------------------------------------------------- */
void I_StopSound(int channel)
{
    if (channel < 0 || channel >= MAX_CHANNELS) return;
    if (s_channels[channel].buffer) {
        IDirectSoundBuffer_Stop(s_channels[channel].buffer);
        s_channels[channel].playing = 0;
    }
}

/* -----------------------------------------------------------------------
 * I_SoundIsPlaying — Check if a channel is still playing
 * ----------------------------------------------------------------------- */
int I_SoundIsPlaying(int channel)
{
    if (channel < 0 || channel >= MAX_CHANNELS) return 0;
    return s_channels[channel].playing;
}

/* -----------------------------------------------------------------------
 * I_InitMusic / I_ShutdownMusic / I_PlaySong / I_StopSong
 *
 * Stubs for MIDI music playback.  A full implementation would use
 * the Windows MIDI API (midiOutOpen, midiStreamOut) or convert
 * MUS format to MIDI on the fly.
 * ----------------------------------------------------------------------- */
void I_InitMusic(void)   { lprintf(LO_INFO, "I_InitMusic: Windows MIDI stub\n"); }
void I_ShutdownMusic(void) {}
void I_PlaySong(int handle, int looping) { (void)handle; (void)looping; }
void I_StopSong(void) {}
void I_PauseSong(void) {}
void I_ResumeSong(void) {}
int  I_RegisterSong(const void *data, size_t len) { (void)data; (void)len; return 0; }
void I_UnRegisterSong(int handle) { (void)handle; }
void I_SetMusicVolume(int volume) { (void)volume; }
