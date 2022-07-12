#include "fastfetch.h"

#include <unistd.h>
#include <pthread.h>

const FFTitleResult* ffDetectTitle(FFinstance* instance)
{
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    static bool init = false;
    static FFTitleResult result;

    pthread_mutex_lock(&mutex);
    if(init)
    {
        pthread_mutex_unlock(&mutex);
        return &result;
    }
    init = true;

    ffStrbufInit(&result.userName);
    ffStrbufAppendS(&result.userName, instance->state.passwd->pw_name);

    ffStrbufInitA(&result.hostname, 256);
    gethostname(result.hostname.chars, result.hostname.allocated);
    ffStrbufRecalculateLength(&result.hostname);

    pthread_mutex_unlock(&mutex);
    return &result;
}

static inline void printTitlePart(FFinstance* instance, const FFstrbuf* content)
{
    if(!instance->config.pipe)
    {
        fputs(FASTFETCH_TEXT_MODIFIER_BOLT, stdout);
        ffPrintColor(&instance->config.mainColor);
    }

    ffStrbufWriteTo(content, stdout);

    if(!instance->config.pipe)
        fputs(FASTFETCH_TEXT_MODIFIER_RESET, stdout);
}

void ffPrintTitle(FFinstance* instance)
{
    const FFTitleResult* result = ffDetectTitle(instance);

    ffLogoPrintLine(instance);

    printTitlePart(instance, &result->userName);
    putchar('@');
    printTitlePart(instance, &result->hostname);
    putchar('\n');

    ffStrbufDestroy(&result->userName);
    ffStrbufDestroy(&result->hostname);
}
