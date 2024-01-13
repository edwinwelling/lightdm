/*
 * Copyright (C) 2010-2011 Robert Ancell.
 * Author: Robert Ancell <robert.ancell@canonical.com>
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version. See http://www.gnu.org/copyleft/gpl.html the full text of the
 * license.
 */
#include <stdint.h>
#include "io.h"


// In case the EOF is not hit, this function reads exactly the
// count number of bytes.
ssize_t rigid_read (int fd, void *buf, size_t count) {
    size_t received = 0;
    ssize_t result = 0;

    while (received < count) {
        result = read(fd, (uint8_t*) buf + received, count - received);

        if (result == -1) {  
            return -1;
        }

        if (result == 0) {
            // function promise to exactly to read the count number
            // of bytes, in case it reach the EOF, it doesn;t
            // fulfill this promise: it returns the number of received
            // so far.
            break;
        }

        received += result;
    }

    return received;
}