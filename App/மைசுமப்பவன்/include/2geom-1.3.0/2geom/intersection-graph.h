/**
 * \file
 * \brief Path intersection graph
 *//*
 * Authors:
 *   Krzysztof Kosiński <tweenk.pl@gmail.com>
 * 
 * Copyright 2015 Authors
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
 */

#ifndef SEEN_LIB2GEOM_INTERSECTION_GRAPH_H
#define SEEN_LIB2GEOM_INTERSECTION_GRAPH_H

#include <set>
#include <vector>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/intrusive/list.hpp>
#include <2geom/forward.h>
#include <2geom/pathvector.h>

namespace Geom {

/** @class PathIntersectionGraph
 * @brief Intermediate data for computing Boolean operations on paths.
 *
 * This class implements the Greiner-Hormann clipping algorithm,
 * with improvements inspired by Foster and Overfelt as well as some
 * original contributions.
 *
 * For the purposes of boolean operations, a shape is defined as a PathVector
 * using the "even-odd" rule, i.e., regions with odd winding are considered part
 * of the shape, whereas regions with even winding are not.
 *
 * For this reason, the two path-vectors are sometimes called "shapes" or "operands" of
 * the boolean operation. Each path-vector may contain several paths, which are called
 * either "paths" or "components" in the documentation.
 *
 * @ingroup Paths
 */
class PathIntersectionGraph
{
    // this is called PathIntersectionGraph so that we can also have a class for polygons,
    // e.g. PolygonIntersectionGraph, which is going to be significantly faster
public:
    /** @brief Construct a path intersection graph for two shapes described via their boundaries.
     *  The boundaries are passed as path-vectors.
     *
     *  @param a – the first operand, also referred to as operand A.
     *  @param b – the second operand, also referred to as operand B.
     *  @param precision – precision setting used for intersection calculations.
     */
    PathIntersectionGraph(PathVector const &a, PathVector const &b, Coord precision = EPSILON);

    /**
     * @brief Get the union of the shapes, A ∪ B.
     *
     * A point belongs to the union if and only if it belongs to at least one of the operands.
     *
     * @return A path-vector describing the union of the operands A and B.
     */
    PathVector getUnion();

    /**
     * @brief Get the intersection of the shapes, A ∩ B.
     *
     * A point belongs to the intersection if and only if it belongs to both shapes.
     *
     * @return A path-vector describing the intersection of the operands A and B.
     */
    PathVector getIntersection();

    /**
     * @brief Get the difference of the shapes, A ∖ B.
     *
     * A point belongs to the difference if and only if it belongs to A but not to B.
     *
     * @return A path-vector describing the difference of the operands A and B.
     */
    PathVector getAminusB();

    /**
     * @brief Get the opposite difference of the shapes, B ∖ A.
     *
     * A point belongs to the difference if and only if it belongs to B but not to A.
     *
     * @return A path-vector describing the difference of the operands B and A.
     */
    PathVector getBminusA();

    /**
     * @brief Get the symmetric difference of the shapes, A ∆ B.
     *
     * A point belongs to the symmetric difference if and only if it belongs to one of the two
     * shapes A or B, but not both. This is equivalent to the logical XOR operation: the elements
     * of A ∆ B are points which are in A XOR in B.
     *
     * @return A path-vector describing the symmetric difference of the operands A and B.
     */
    PathVector getXOR();

    /// Returns the number of intersections used when computing Boolean operations.
    std::size_t size() const;

    /**
     * @brief Get the geometric points where the two path-vectors intersect.
     *
     * Degenerate intersection points, where the shapes merely "kiss", are not retured.
     *
     * @param defective – whether to return only the defective crossings or only the true crossings.
     * @return If defective is true, returns a vector containing all defective intersection points,
     * i.e., points that are neither true transverse intersections nor degenerate intersections.
     * If defective is false, returns all true transverse intersections.
     */
    std::vector<Point> intersectionPoints(bool defective = false) const;

    /**
     * @brief Get the geometric points located on path portions between consecutive intersections.
     *
     * These points were used for the winding number calculations which determined which path portions
     * lie inside the other shape and which lie outside.
     *
     * @return A vector containing all sample points used for winding calculations.
     */
    std::vector<Point> windingPoints() const {
        return _winding_points;
    }

    void fragments(PathVector &in, PathVector &out) const;


    bool valid() const { return _graph_valid; }

private:
    enum InOutFlag {
        INSIDE,
        OUTSIDE,
        BOTH
    };

    struct IntersectionVertex {
        boost::intrusive::list_member_hook<> _hook;
        boost::intrusive::list_member_hook<> _proc_hook;
        PathVectorTime pos; ///< Intersection time.
        Point p; ///< Geometric position of the intersection point; guarantees that endpoints are exact.
        IntersectionVertex *neighbor; ///< A pointer to the corresponding vertex on the other shape.
        /** Tells us whether the edge originating at this intersection lies inside or outside of
         *  the shape given by the other path-vector. The "edge originating" at this intersection is
         *  the portion of the path between this intersection and the next intersection, in the
         *  direction of increasing path time. */
        InOutFlag next_edge;
        unsigned which; ///< Index of the operand path-vector that this intersection vertex lies on.
        /** Whether the intersection is defective, which means that for some reason the paths
         *  neither cross transversally through each other nor "kiss" at a common tangency point.
         */
        bool defective;
    };

    typedef boost::intrusive::list
        < IntersectionVertex
        , boost::intrusive::member_hook
            < IntersectionVertex
            , boost::intrusive::list_member_hook<>
            , &IntersectionVertex::_hook
            >
        > IntersectionList;

    typedef boost::intrusive::list
        < IntersectionVertex
        , boost::intrusive::member_hook
            < IntersectionVertex
            , boost::intrusive::list_member_hook<>
            , &IntersectionVertex::_proc_hook
            >
        > UnprocessedList;

    /// Stores processed intersection information for a single path in an operand path-vector.
    struct PathData {
        IntersectionList xlist; ///< List of crossings on this particular path.
        std::size_t path_index; ///< Index of the path in its path-vector.
        int which; ///< Index of the path-vector (in PathIntersectionGraph::_pv) that the path belongs to.
        /** Whether this path as a whole is contained INSIDE or OUTSIDE relative to the other path-vector.
         *  The value BOTH means that some portions of the path are inside while others are outside.
         */
        InOutFlag status;

        PathData(int w, std::size_t pi)
            : path_index(pi)
            , which(w)
            , status(BOTH)
        {}
    };

    struct IntersectionVertexLess;
    typedef IntersectionList::iterator ILIter;
    typedef IntersectionList::const_iterator CILIter;

    PathVector _getResult(bool enter_a, bool enter_b);
    void _handleNonintersectingPaths(PathVector &result, unsigned which, bool inside);
    void _prepareArguments();
    bool _prepareIntersectionLists(Coord precision);
    void _assignEdgeWindingParities(Coord precision);
    void _assignComponentStatusFromDegenerateIntersections();
    void _removeDegenerateIntersections();
    void _verify();

    ILIter _getNeighbor(ILIter iter);
    PathData &_getPathData(ILIter iter);

    PathVector _pv[2]; ///< Stores the two operand path-vectors, A at _pv[0] and B at _pv[1].
    boost::ptr_vector<IntersectionVertex> _xs; ///< Stores all crossings between the two shapes.
    boost::ptr_vector<PathData> _components[2]; ///< Stores the crossing information for the operands.
    UnprocessedList _ulist; ///< Temporarily holds all unprocessed during a boolean operation.
    bool _graph_valid; ///< Whether all intersections are regular.
    /** Stores sample points located on paths of the operand path-vectors,
     *  between consecutive intersections.
     */
    std::vector<Point> _winding_points;

    friend std::ostream &operator<<(std::ostream &, PathIntersectionGraph const &);
};

std::ostream &operator<<(std::ostream &os, PathIntersectionGraph const &pig);

} // namespace Geom

#endif // SEEN_LIB2GEOM_PATH_GRAPH_H
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
