#include "httprange.h"

HttpRange::HttpRange() :
    null(true),
    lowRange(0),
    highRange(0),
    size(-1),
    rxRange("range:\\s+(\\w+)=(\\d*)-(\\d*)", Qt::CaseInsensitive)
{
}

HttpRange::HttpRange(QString range) :
    null(true),
    lowRange(0),
    highRange(0),
    size(-1),
    rxRange("range:\\s+(\\w+)=(\\d*)-(\\d*)", Qt::CaseInsensitive)
{
    if (rxRange.indexIn(range) != -1) {
        unit = rxRange.cap(1);

        if (rxRange.cap(2).isEmpty()) {
            lowRange = -1;
        } else {
            lowRange = rxRange.cap(2).toLong();
        }

        if (rxRange.cap(3).isEmpty()) {
            highRange = -1;
        } else {
            highRange = rxRange.cap(3).toLong();
        }


        if ((highRange != -1) && (lowRange > highRange)) {
            // invalid range
            lowRange = -1;
            highRange = -1;
        } else {
            // range is valid
            null = false;
        }
    }
}

long HttpRange::getStartByte() const {

    if (!isNull() && (size > 0)) {
        if ((lowRange >= 0) && (lowRange < size)) {
            return lowRange;
        }

        if ((lowRange == -1) && (highRange > 0)) {
            if ((size - highRange) >= 0) {
                return size - highRange;
            } else {
                return 0;
            }
        }
    }

    return -1;
}

long HttpRange::getEndByte() const {

    if (!isNull() && (size > 0)) {
        if (highRange == -1)  {
            return size - 1;
        }

        if ((lowRange == -1) && (highRange > 0)) {
            return size - 1;
        }

        if (highRange < size) {
            return highRange;
        } else {
            return size - 1;
        }
    }

    return -1;
}

long HttpRange::getLength() const {
    long start = getStartByte();
    if (start != -1) {
        long end = getEndByte();
        if (end != -1) {
            return end - start + 1;
        }
    }
    return -1;
}
