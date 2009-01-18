/*****************************************************************************
 * clock.h: clocks synchronisation
 *****************************************************************************
 * Copyright (C) 2008 the VideoLAN team
 * Copyright (C) 2008 Laurent Aimar
 * $Id$
 *
 * Authors: Laurent Aimar < fenrir _AT_ videolan _DOT_ org >
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#if defined(__PLUGIN__) || defined(__BUILTIN__) || !defined(__LIBVLC__)
# error This header file can only be included from LibVLC.
#endif

#ifndef _INPUT_CLOCK_H
#define _INPUT_CLOCK_H 1

#include <vlc_common.h>
#include <vlc_input.h> /* FIXME Needed for input_clock_t */

/** @struct input_clock_t
 * This structure is used to manage clock drift and reception jitters
 *
 * XXX input_clock_GetTS can be called from any threads. All others functions
 * MUST be called from one and only one thread.
 */

/**
 * This function creates a new input_clock_t.
 * You must use input_clock_Delete to delete it once unused.
 */
input_clock_t *input_clock_New( int i_rate );

/**
 * This function destroys a input_clock_t created by input_clock_New.
 */
void           input_clock_Delete( input_clock_t * );

/**
 * This function will update a input_clock_t with a new clock reference point.
 */
void    input_clock_Update( input_clock_t *, vlc_object_t *p_log,
                            bool b_can_pace_control, mtime_t i_clock, mtime_t i_system );
/**
 * This function will reset the drift of a input_clock_t.
 *
 * The actual jitter estimation will not be reseted by it.
 */
void    input_clock_Reset( input_clock_t * );

/**
 * This functions will return a deadline used to control the reading speed.
 */
mtime_t input_clock_GetWakeup( input_clock_t * );

/**
 * This functions allows to change the actual reading speed.
 */
void    input_clock_ChangeRate( input_clock_t *, int i_rate );

/**
 * This function allows to change the pause status.
 */
void    input_clock_ChangePause( input_clock_t *, bool b_paused, mtime_t i_date );

/**
 * This function allows to rebase the original system value date.
 * It can be called only imediatly after a input_clock_Update call.
 * FIXME ugly
 */
void    input_clock_ChangeSystemOrigin( input_clock_t *, mtime_t i_system );

/**
 * This function converts a timestamp from stream clock to system clock.
 *
 * If pi_rate is provided it will be field with the rate value used for
 * the conversion.
 * If i_ts_bound is not INT64_MAX, the value will be invalidated if not
 * before mdate() + i_pts_delay + i_ts_bound.
 */
mtime_t input_clock_GetTS( input_clock_t *, int *pi_rate, mtime_t i_ts, mtime_t i_ts_bound );

/**
 * This function returns the current rate.
 */
int input_clock_GetRate( input_clock_t * );

/**
 * This function returns current clock state or VLC_EGENERIC if there is not a
 * reference point.
 */
int input_clock_GetState( input_clock_t *,
                          mtime_t *pi_stream_start, mtime_t *pi_system_start,
                          mtime_t *pi_stream_duration, mtime_t *pi_system_duration );

/**
 * This function allows the set the minimal configuration for the jitter estimation algo.
 */
void input_clock_SetJitter( input_clock_t *,
                            mtime_t i_pts_delay, int i_cr_average );

#endif

