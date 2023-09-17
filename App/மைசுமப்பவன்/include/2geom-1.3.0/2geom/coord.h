/** @file
 * @brief Integral and real coordinate types and some basic utilities
 *//*
 * Authors:
 *   Nathan Hurst <njh@mail.csse.monash.edu.au>
 *   Krzysztof Kosiński <tweenk.pl@gmail.com>
 * Copyright 2006-2015 Authors
 *
 * This library is free software; you can redistribute it and/or
 * modify it either under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation
 * (the "LGPL") or, at your option, under the terms of the Mozilla
 * Public License Version 1.1 (the "MPL"). If you do not alter this
 * notice, a recipient may use your version of this file under either
 * the MPL or the LGPL.
 *
 * You should have received a copy of the LGPL along with this library
 * in the file COPYING-LGPL-2.1; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 * You should have received a copy of the MPL along with this library
 * in the file COPYING-MPL-1.1
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY
 * OF ANY KIND, either express or implied. See the LGPL or the MPL for
 * the specific language governing rights and limitations.
 *
 */

#ifndef LIB2GEOM_SEEN_COORD_H
#define LIB2GEOM_SEEN_COORD_H

#include <cmath>
#include <limits>
#include <string>
#include <functional>
#include <boost/operators.hpp>
#include <2geom/forward.h>

namespace Geom {

/** @brief 2D axis enumeration (X or Y).
 * @ingroup Primitives */
enum Dim2 { X=0, Y=1 };

/** @brief Get the other (perpendicular) dimension.
 * @ingroup Primitives */
inline constexpr Dim2 other_dimension(Dim2 d) { return Dim2(int(d) ^ 1); }

// TODO: make a smarter implementation with C++11
template <typename T>
struct D2Traits {
    using D1Value = typename T::D1Value;
    using D1Reference = typename T::D1Reference;
    using D1ConstReference = typename T::D1ConstReference;
};

/** @brief Axis extraction functor.
 * For use with things such as Boost's transform_iterator.
 * @ingroup Utilities */
template <Dim2 D, typename T>
struct GetAxis {
    using result_type = typename D2Traits<T>::D1Value;
    using argument_type = T;
    typename D2Traits<T>::D1Value operator()(T const &a) const {
        return a[D];
    }
};

/** @brief Floating point type used to store coordinates.
 * @ingroup Primitives */
using Coord = double;

/** @brief Type used for integral coordinates.
 * @ingroup Primitives */
using IntCoord = int;

/** @brief Default "acceptably small" value.
 * @ingroup Primitives */
constexpr Coord EPSILON = 1e-6;

/** @brief Get a value representing infinity.
 * @ingroup Primitives */
inline constexpr Coord infinity() { return std::numeric_limits<Coord>::infinity(); }

/** @brief Nearness predicate for values.
 * @ingroup Primitives */
inline constexpr bool are_near(Coord a, Coord b, double eps=EPSILON) { return std::abs(a-b) <= eps; }
inline constexpr bool rel_error_bound(Coord a, Coord b, double eps=EPSILON) { return std::abs(a) <= eps*b; }

/** @brief Numerically stable linear interpolation.
 * @ingroup Primitives */
inline constexpr Coord lerp(Coord t, Coord a, Coord b) {
    return (1 - t) * a + t * b;
}

/** @brief Traits class used with coordinate types.
 * Defines point, interval and rectangle types for the given coordinate type.
 * @ingroup Utilities */
template <typename C>
struct CoordTraits {
    using PointType = D2<C>;
    using IntervalType = GenericInterval<C>;
    using OptIntervalType = GenericOptInterval<C>;
    using RectType = GenericRect<C>;
    using OptRectType = GenericOptRect<C>;

    using IntervalOps =
      boost::equality_comparable< IntervalType
    , boost::orable< IntervalType
      >>;

    using RectOps =
      boost::equality_comparable< RectType
    , boost::orable< RectType
    , boost::orable< RectType, OptRectType
      >>>;
};

// NOTE: operator helpers for Rect and Interval are defined here.
// This is to avoid increasing their size through multiple inheritance.

template<>
struct CoordTraits<IntCoord> {
    using PointType = IntPoint;
    using IntervalType = IntInterval;
    using OptIntervalType = OptIntInterval;
    using RectType = IntRect;
    using OptRectType = OptIntRect;

    using IntervalOps =
      boost::equality_comparable< IntInterval
    , boost::additive< IntInterval
    , boost::additive< IntInterval, IntCoord
    , boost::orable< IntInterval
      >>>>;

    using RectOps =
      boost::equality_comparable< IntRect
    , boost::orable< IntRect
    , boost::orable< IntRect, OptIntRect
    , boost::additive< IntRect, IntPoint
      >>>>;
};

template<>
struct CoordTraits<Coord> {
    using PointType = Point;
    using IntervalType = Interval;
    using OptIntervalType = OptInterval;
    using RectType = Rect;
    using OptRectType = OptRect;

    using IntervalOps =
      boost::equality_comparable< Interval
    , boost::equality_comparable< Interval, IntInterval
    , boost::additive< Interval
    , boost::multipliable< Interval
    , boost::orable< Interval
    , boost::arithmetic< Interval, Coord
      >>>>>>;

    using RectOps =
      boost::equality_comparable< Rect
    , boost::equality_comparable< Rect, IntRect
    , boost::orable< Rect
    , boost::orable< Rect, OptRect
    , boost::additive< Rect, Point
    , boost::multipliable< Rect, Affine
      >>>>>>;
};

/** @brief Convert coordinate to shortest possible string.
 * @return The shortest string that parses back to the original value.
 * @relates Coord */
std::string format_coord_shortest(Coord x);

/** @brief Convert coordinate to human-readable string.
 * Unlike format_coord_shortest, this function will not omit a leading zero
 * before a decimal point or use small negative exponents. The output format
 * is similar to Javascript functions.
 * @relates Coord */
std::string format_coord_nice(Coord x);

/** @brief Parse coordinate string.
 * When using this function in conjunction with format_coord_shortest()
 * or format_coord_nice(), the value is guaranteed to be preserved exactly.
 * @relates Coord */
Coord parse_coord(std::string const &s);

} // namespace Geom

#endif // LIB2GEOM_SEEN_COORD_H

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0)(inline-open . 0)(case-label . +))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:fileencoding=utf-8:textwidth=99 :
