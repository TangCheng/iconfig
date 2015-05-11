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

#ifndef LIBTZ_TZ_H
#define LIBTZ_TZ_H

#include <sys/time.h>

typedef void* tzinfo_t;     ///< Timezone information.

#define TZ_FLAG_POSIX    0x0000001      ///< Posix timezone format.

/// Lookup timezone information by full name.
/// @param [in] name            Full name of timezone.
/// @param [in] flags           Additional flags.
/// @return a tzinfo object if found.
tzinfo_t tz_lookup_by_name(const char* name, unsigned int flags);

/// Lookup timezone information by short name.
/// @param [in] name            Short name of timezone.
/// @param [in] flags           Additional flags.
/// @return a tzinfo object if found.
tzinfo_t tz_lookup_by_short_name(const char* short_name, unsigned int flags);

/// Lookup timezone information by full and short name.
/// @param [in] name            Full name of timezone.
/// @param [in] short_name      Short name of timezone.
/// @param [in] flags           Additional flags.
/// @return a tzinfo object if found.
tzinfo_t tz_lookup_by_full_and_short_name(const char* name, const char* short_name, unsigned int flags);

/// Get full name.
/// @param [in] tzi             Timezone information object.
/// @return string pointer.
const char* tzinfo_name(tzinfo_t tzi);

/// Get short name.
/// @param [in] tzi             Timezone information object.
/// @return string pointer.
const char* tzinfo_short_name(tzinfo_t tzi);

/// Get timezone.
/// @param [in] tzi             Timezone information object.
/// @return long int timezone value.
int tzinfo_minuteswest(tzinfo_t tzi);

/// Get daylight.
/// @param [in] tzi             Timezone information object.
/// @return int daylight value.
int tzinfo_dsttime(tzinfo_t tzi);

#endif
