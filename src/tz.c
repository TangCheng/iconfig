/*
 * Copyright (C) 2012 Varphone Wong <varphone@qq.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public Licens
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-
 *
 */

#include <assert.h>
#include <regex.h>
#include <string.h>
#include "tz.h"

struct tzinfo
{
    const char* name;
    const char* short_name;
    struct {
        int tz_minuteswest;
	    int tz_dsttime;
    } timezone;
};

struct tzinfo posix_1003_1_tzinfo[] = {
    {"AfghanistanStandardTime-4:30", "UTC+4:30", {270, 0}},
    {"AlaskanStandardTime9DaylightTime", "UTC-9:00", {-540, 1}},
    {"ArabianStandardTime-4", "UTC+4:00", {240, 0}},
    {"ArabicStandardTime-3", "UTC+3:00", {180, 0}},
    {"ArabStandardTime-3", "UTC+3:00", {180, 0}},
    {"ArgentinaStandardTime3", "UTC-3:00", {-180, 0}},
    {"AtlanticStandardTime4DaylightTime", "UTC-4:00", {-240, 1}},
    {"AUSCentralStandardTime-9:30", "UTC+9:30", {570, 0}},
    {"AUSEasternStandardTime-10DaylightTime", "UTC+10:00", {600, 1}},
    {"AzerbaijanStandardTime-4DaylightTime", "UTC+4:00", {240, 1}},
    {"AzoresStandardTime1DaylightTime", "UTC-1:00", {-60, 1}},
    {"BahiaStandardTime3DaylightTime", "UTC-3:00", {-180, 1}},
    {"BangladeshStandardTime-6", "UTC+6:00", {360, 0}},
    {"CanadaCentralStandardTime6", "UTC-6:00", {-360, 0}},
    {"CapeVerdeStandardTime1", "UTC-1:00", {-60, 0}},
    {"CaucasusStandardTime-4DaylightTime", "UTC+4:00", {240, 1}},
    {"CenAustraliaStandardTime-9:30DaylightTime", "UTC+9:30", {570, 1}},
    {"CentralAmericaStandardTime6", "UTC-6:00", {-360, 0}},
    {"CentralAsiaStandardTime-6", "UTC+6:00", {360, 0}},
    {"CentralBrazilianStandardTime4DaylightTime", "UTC-4:00", {-240, 1}},
    {"CentralEuropeanStandardTime-1DaylightTime", "UTC+1:00", {60, 1}},
    {"CentralEuropeStandardTime-1DaylightTime", "UTC+1:00", {60, 1}},
    {"CentralPacificStandardTime-11", "UTC+11:00", {660, 0}},
    {"CentralStandardTime6DaylightTime", "UTC-6:00", {-360, 1}},
    {"CentralStandardTimeMexico6DaylightTime", "UTC-6:00", {-360, 1}},
    {"ChinaStandardTime-8", "UTC+8:00", {480, 0}},
    {"DatelineStandardTime12", "UTC-12:00", {-720, 0}},
    {"EAfricaStandardTime-3", "UTC+3:00", {180, 0}},
    {"EasternStandardTime5DaylightTime", "UTC-5:00", {-300, 1}},
    {"EAustraliaStandardTime-10", "UTC+10:00", {600, 0}},
    {"EEuropeStandardTime-2DaylightTime", "UTC+2:00", {120, 1}},
    {"EgyptStandardTime-2", "UTC+2:00", {120, 0}},
    {"EkaterinburgStandardTime-5DaylightTime", "UTC+5:00", {300, 1}},
    {"EkaterinburgStandardTime-6", "UTC+6:00", {360, 0}},
    {"ESouthAmericaStandardTime3DaylightTime", "UTC-3:00", {-180, 1}},
    {"FijiStandardTime-12", "UTC+12:00", {720, 0}},
    {"FijiStandardTime-12DaylightTime", "UTC+12:00", {720, 1}},
    {"FLEStandardTime-2DaylightTime", "UTC+2:00", {120, 1}},
    {"GeorgianStandardTime-4", "UTC+4:00", {240, 0}},
    {"GMTStandardTime0DaylightTime", "UTC-0:00", {-0, 1}},
    {"GreenlandStandardTime3DaylightTime", "UTC-3:00", {-180, 1}},
    {"GreenwichStandardTime0", "UTC-0:00", {-0, 0}},
    {"GTBStandardTime-2DaylightTime", "UTC+2:00", {120, 1}},
    {"HawaiianStandardTime10", "UTC-10:00", {-600, 0}},
    {"IndiaStandardTime-5:30", "UTC+5:30", {330, 0}},
    {"IranStandardTime-3:30DaylightTime", "UTC+3:30", {210, 1}},
    {"IsraelStandardTime-2", "UTC+2:00", {120, 0}},
    {"IsraelStandardTime-2DaylightTime", "UTC+2:00", {120, 1}},
    {"JordanStandardTime-2DaylightTime", "UTC+2:00", {120, 1}},
    {"KaliningradStandardTime-3", "UTC+3:00", {180, 0}},
    {"KamchatkaStandardTime-12DaylightTime", "UTC+12:00", {720, 1}},
    {"KoreaStandardTime-9", "UTC+9:00", {540, 0}},
    {"MagadanStandardTime-12", "UTC+12:00", {720, 0}},
    {"MauritiusStandardTime-4", "UTC+4:00", {240, 0}},
    {"<MexicoStandardTime2>7DaylightTime", "UTC-7:00", {-420, 1}},
    {"MexicoStandardTime6DaylightTime", "UTC-6:00", {-360, 1}},
    {"<Mid-AtlanticStandardTime>2DaylightTime", "UTC-2:00", {-120, 1}},
    {"MiddleEastStandardTime-2DaylightTime", "UTC+2:00", {120, 1}},
    {"MontevideoStandardTime3DaylightTime", "UTC-3:00", {-180, 1}},
    {"MoroccoStandardTime0", "UTC-0:00", {-0, 0}},
    {"MountainStandardTime7DaylightTime", "UTC-7:00", {-420, 1}},
    {"MountainStandardTimeMexico7DaylightTime", "UTC-7:00", {-420, 1}},
    {"MyanmarStandardTime-6:30", "UTC+6:30", {390, 0}},
    {"NamibiaStandardTime-1DaylightTime", "UTC+1:00", {60, 1}},
    {"NCentralAsiaStandardTime-7", "UTC+7:00", {420, 0}},
    {"NepalStandardTime-5:45", "UTC+5:45", {345, 0}},
    {"NewfoundlandStandardTime3:30DaylightTime", "UTC-3:30", {-210, 1}},
    {"NewZealandStandardTime-12DaylightTime", "UTC+12:00", {720, 1}},
    {"NorthAsiaEastStandardTime-8DaylightTime", "UTC+8:00", {480, 1}},
    {"NorthAsiaEastStandardTime-9", "UTC+9:00", {540, 0}},
    {"NorthAsiaStandardTime-7DaylightTime", "UTC+7:00", {420, 1}},
    {"NorthAsiaStandardTime-8", "UTC+8:00", {480, 0}},
    {"PacificSAStandardTime4DaylightTime", "UTC-4:00", {-240, 1}},
    {"PacificStandardTime8DaylightTime", "UTC-8:00", {-480, 1}},
    {"PacificStandardTimeMexico8DaylightTime", "UTC-8:00", {-480, 1}},
    {"PakistanStandardTime-5", "UTC+5:00", {300, 0}},
    {"ParaguayStandardTime4DaylightTime", "UTC-4:00", {-240, 1}},
    {"RomanceStandardTime-1DaylightTime", "UTC+1:00", {60, 1}},
    {"RussianStandardTime-3DaylightTime", "UTC+3:00", {180, 1}},
    {"RussianStandardTime-4", "UTC+4:00", {240, 0}},
    {"SAEasternStandardTime3", "UTC-3:00", {-180, 0}},
    {"SamoaStandardTime11", "UTC-11:00", {-660, 0}},
    {"SamoaStandardTime-13DaylightTime", "UTC+13:00", {780, 1}},
    {"SAPacificStandardTime5", "UTC-5:00", {-300, 0}},
    {"SAWesternStandardTime4", "UTC-4:00", {-240, 0}},
    {"SEAsiaStandardTime-7", "UTC+7:00", {420, 0}},
    {"SingaporeStandardTime-8", "UTC+8:00", {480, 0}},
    {"SouthAfricaStandardTime-2", "UTC+2:00", {120, 0}},
    {"SriLankaStandardTime-5:30", "UTC+5:30", {330, 0}},
    {"SriLankaStandardTime-6", "UTC+6:00", {360, 0}},
    {"SyriaStandardTime-2DaylightTime", "UTC+2:00", {120, 1}},
    {"TaipeiStandardTime-8", "UTC+8:00", {480, 0}},
    {"TasmaniaStandardTime-10DaylightTime", "UTC+10:00", {600, 1}},
    {"TokyoStandardTime-9", "UTC+9:00", {540, 0}},
    {"TongaStandardTime-13", "UTC+13:00", {780, 0}},
    {"TurkeyStandardTime-2DaylightTime", "UTC+2:00", {120, 1}},
    {"UlaanbaatarStandardTime-8", "UTC+8:00", {480, 0}},
    {"USEasternStandardTime5", "UTC-5:00", {-300, 1}},
    {"USEasternStandardTime5DaylightTime", "UTC-5:00", {-300, 1}},
    {"USMountainStandardTime7", "UTC-7:00", {-420, 0}},
    {"<UTC-02>2", "UTC-2:00", {-120, 0}},
    {"UTC0", "UTC-0:00", {0, 0}},
    {"<UTC-11>11", "UTC-11:00", {-660, 1}},
    {"<UTC+12>-12", "UTC+12:00", {720, 0}},
    {"VenezuelaStandardTime4:30", "UTC-4:30", {-270, 0}},
    {"VladivostokStandardTime-11", "UTC+11:00", {660, 0}},
    {"VladivostokStandardTime-10DaylightTime", "UTC+10:00", {600, 1}},
    {"WAustraliaStandardTime-8", "UTC+8:00", {480, 0}},
    {"WCentralAfricaStandardTime-1", "UTC+1:00", {60, 0}},
    {"WestAsiaStandardTime-5", "UTC+5:00", {300, 0}},
    {"WestPacificStandardTime-10", "UTC+10:00", {600, 0}},
    {"WEuropeStandardTime-1DaylightTime", "UTC+1:00", {60, 1}},
    {"YakutskStandardTime-10", "UTC+10:00", {600, 0}},
    {"YakutskStandardTime-9DaylightTime", "UTC+9:00", {540, 1}},
    {0, 0, {0, 0}},
};

#ifdef HAVE_REGEX_H
static int smatch(const char* pattern, const char* target)
{
    int err = 0;
    regex_t ctx = {0};
    err = regcomp(&ctx, pattern, 0);
    if (!err) {
        err = regexec(&ctx, target, 0, NULL, 0);
        regfree(&ctx);
    }
    return !err;
}
#else
static int smatch(const char* pattern, const char* target)
{
    return !strcmp(pattern, target);
}
#endif

tzinfo_t tz_lookup_by_name(const char* name, unsigned int flags)
{
    struct tzinfo* tzi = 0;
    if (flags & TZ_FLAG_POSIX) {
        tzi = posix_1003_1_tzinfo;
        while (tzi->name) {
            if (smatch(tzi->name, name))
                return tzi;
            tzi++;
        }
    }
    return 0;
}

tzinfo_t tz_lookup_by_short_name(const char* short_name, unsigned int flags)
{
    struct tzinfo* tzi = 0;
    if (flags & TZ_FLAG_POSIX) {
        tzi = posix_1003_1_tzinfo;
        while (tzi->name) {
            if (smatch(tzi->short_name, short_name))
                return tzi;
            tzi++;
        }
    }
    return 0;
}

tzinfo_t tz_lookup_by_full_and_short_name(const char* name, const char* short_name, unsigned int flags)
{
    struct tzinfo* tzi = 0;
    if (flags & TZ_FLAG_POSIX) {
        tzi = posix_1003_1_tzinfo;
        while (tzi->name) {
            if (smatch(tzi->name, name) &&
                smatch(tzi->short_name, short_name))
                return tzi;
            tzi++;
        }
    }
    return 0;
}

const char* tzinfo_name(tzinfo_t tzi)
{
    assert(tzi);
    return ((struct tzinfo*)tzi)->name;
}

const char* tzinfo_short_name(tzinfo_t tzi)
{
    assert(tzi);
    return ((struct tzinfo*)tzi)->short_name;
}

int tzinfo_minuteswest(tzinfo_t tzi)
{
    assert(tzi);
    return ((struct tzinfo*)tzi)->timezone.tz_minuteswest;
}

int tzinfo_dsttime(tzinfo_t tzi)
{
    assert(tzi);
    return ((struct tzinfo*)tzi)->timezone.tz_dsttime;
}
