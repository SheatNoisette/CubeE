// sound.cpp: SDL3_mixer based audio

#include "cube.h"

VARP(soundvol, 0, 255, 255);
VARP(musicvol, 0, 128, 255);
bool nosound = false;

#define MAXCHAN 32
#define SOUNDFREQ 22050

struct soundloc {
    vec loc;
    bool inuse;
} soundlocs[MAXCHAN];

#define MAXVOL 1.0f

static MIX_Mixer* mixer = NULL;
static MIX_Track* music_track = NULL;
static MIX_Audio* mod = NULL;
static MIX_Track* sound_tracks[MAXCHAN];

void stopsound()
{
    if (nosound)
        return;
    if (mod) {
        if (music_track)
            MIX_StopTrack(music_track, 0);
        MIX_DestroyAudio(mod);
        mod = NULL;
    };
};

VAR(soundbufferlen, 128, 1024, 4096);

void initsound()
{
    memset(soundlocs, 0, sizeof(soundloc) * MAXCHAN);
    if (!MIX_Init()) {
        conoutf("sound init failed (SDL_mixer): %s", SDL_GetError());
        nosound = true;
        return;
    };
    SDL_AudioSpec spec;
    spec.format = SDL_AUDIO_S16;
    spec.channels = 2;
    spec.freq = SOUNDFREQ;
    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);
    if (!mixer) {
        conoutf("sound init failed (SDL_mixer): %s", SDL_GetError());
        nosound = true;
        return;
    };
    for (int i = 0; i < MAXCHAN; i++) {
        sound_tracks[i] = MIX_CreateTrack(mixer);
    };
    music_track = MIX_CreateTrack(mixer);
};

void music(char* name)
{
    if (nosound)
        return;
    stopsound();
    if (soundvol && musicvol) {
        string sn;
        strcpy_s(sn, "packages/");
        strcat_s(sn, name);
        mod = MIX_LoadAudio(mixer, path(sn), true);
        if (mod) {
            MIX_SetTrackAudio(music_track, mod);
            MIX_SetTrackGain(music_track, (musicvol * MAXVOL) / 255);
            SDL_PropertiesID props = SDL_CreateProperties();
            SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
            MIX_PlayTrack(music_track, props);
            SDL_DestroyProperties(props);
        };
    };
};

COMMAND(music, ARG_1STR);

vector<MIX_Audio*> samples;

cvector snames;

int registersound(char* name)
{
    loopv(snames) if (strcmp(snames[i], name) == 0) return i;
    snames.add(newstring(name));
    samples.add(NULL);
    return samples.length() - 1;
};

COMMAND(registersound, ARG_1EST);

void cleansound()
{
    if (nosound)
        return;
    stopsound();
    for (int i = 0; i < MAXCHAN; i++) {
        if (sound_tracks[i])
            MIX_DestroyTrack(sound_tracks[i]);
    };
    if (music_track)
        MIX_DestroyTrack(music_track);
    loopv(samples) if (samples[i]) MIX_DestroyAudio(samples[i]);
    if (mixer)
        MIX_DestroyMixer(mixer);
    MIX_Quit();
};

VAR(stereo, 0, 1, 1);

void updatechanvol(int chan, vec* loc)
{
    float vol = soundvol / 255.0f, pan = 0.5f;
    if (loc) {
        vdist(dist, v, *loc, player1->o);
        vol -= (dist * 3 * soundvol / 255) / 255.0f;
        if (vol < 0)
            vol = 0;
        if (stereo && (v.x != 0 || v.y != 0)) {
            float yaw = -atan2(v.x, v.y) - player1->yaw * (PI / 180.0f);
            pan = 0.5f * sin(yaw) + 0.5f;
        };
    };
    MIX_SetTrackGain(sound_tracks[chan], vol);
    MIX_StereoGains gains = { 1.0f - pan, pan };
    MIX_SetTrackStereo(sound_tracks[chan], &gains);
};

void newsoundloc(int chan, vec* loc)
{
    assert(chan >= 0 && chan < MAXCHAN);
    soundlocs[chan].loc = *loc;
    soundlocs[chan].inuse = true;
};

void updatevol()
{
    if (nosound)
        return;
    loopi(MAXCHAN) if (soundlocs[i].inuse)
    {
        if (MIX_TrackPlaying(sound_tracks[i]))
            updatechanvol(i, &soundlocs[i].loc);
        else
            soundlocs[i].inuse = false;
    };
};

void playsoundc(int n)
{
    addmsg(0, 2, SV_SOUND, n);
    playsound(n);
};

int soundsatonce = 0, lastsoundmillis = 0;

void playsound(int n, vec* loc)
{
    if (nosound)
        return;
    if (!soundvol)
        return;
    if (lastmillis == lastsoundmillis)
        soundsatonce++;
    else
        soundsatonce = 1;
    lastsoundmillis = lastmillis;
    if (soundsatonce > 5)
        return;
    if (n < 0 || n >= samples.length()) {
        conoutf("unregistered sound: %d", n);
        return;
    };

    if (!samples[n]) {
        sprintf_sd(buf)("packages/sounds/%s.wav", snames[n]);
        samples[n] = MIX_LoadAudio(mixer, path(buf), true);
        if (!samples[n]) {
            conoutf("failed to load sample: %s", buf);
            return;
        };
    };

    int chan = -1;
    loopi(MAXCHAN) if (!MIX_TrackPlaying(sound_tracks[i]))
    {
        chan = i;
        break;
    };
    if (chan < 0)
        return;

    MIX_SetTrackAudio(sound_tracks[chan], samples[n]);
    if (loc)
        newsoundloc(chan, loc);
    updatechanvol(chan, loc);
    MIX_PlayTrack(sound_tracks[chan], 0);
};

void sound(int n) { playsound(n, NULL); };
COMMAND(sound, ARG_1INT);