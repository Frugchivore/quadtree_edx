//
// Created by n.soungadoy on 21/06/2017.
//

#ifndef QUADTREE_INTERVAL_H
#define QUADTREE_INTERVAL_H

#include <stdexcept>
#include <sstream>

class Interval1D {
public:
    Interval1D(double imin, double imax) {
        if (imax < imin) {
            std::ostringstream oss;
            oss << "imax(" << imax << ") less than imin(" << imin << ")";
            throw std::invalid_argument("imax less than imin");
        }

        if (imin == imax) {
            std::ostringstream oss;
            oss << "imax(" << imax << ") equal imin(" << imin << ")";
            throw std::invalid_argument(oss.str());
        }

        m_imax = imax;
        m_imin = imin;
    };

    inline bool overlaps(const Interval1D& other) const {
        if (*this == other) {
            return true;
        }
        if (other.m_imax <= m_imax && other.m_imax >= m_imin) {
            return true;
        }
        if (other.m_imin >= m_imin && other.m_imin <= m_imax ) {
            return true;
        }
        if (other.m_imin <= m_imin && other.m_imax >= m_imax) {
            return true;
        }

        return false;
    }

    double imin() { return m_imin; }
    double imax() { return m_imax; }

    friend bool operator==(const Interval1D& lhs, const Interval1D& rhs) {
        if (lhs.m_imax == rhs.m_imax &&
                lhs.m_imin == rhs.m_imin) {
            return true;
        }
        return false;
    }

    friend bool operator!=(const Interval1D& lhs, const Interval1D& rhs) {
        return !(lhs == rhs);
    }


private:
    double m_imin;
    double m_imax;
};

class Interval2D {
public:
//    Interval2D() : m_intervalX(0,0), m_intervalY(0,0) {}
    Interval2D(double xmin, double xmax, double ymin, double ymax) : m_intervalX(xmin, xmax), m_intervalY(ymin, ymax) {}

    inline bool overlaps(const Interval2D& other) const {
        if (other.m_intervalX.overlaps(m_intervalX) &&
                other.m_intervalY.overlaps(m_intervalY)) {
            return true;
        }

        return false;
    }

    double xmin() { return m_intervalX.imin(); }
    double xmax() { return m_intervalX.imax(); }
    double ymin() { return m_intervalY.imin(); }
    double ymax() { return m_intervalY.imax(); }

    friend bool operator==(const Interval2D& lhs, const Interval2D& rhs) {
        if (lhs.m_intervalX == rhs.m_intervalX && lhs.m_intervalY == rhs.m_intervalY) {
            return true;
        }
        return false;
    }

    friend bool operator!=(const Interval2D& lhs, const Interval2D& rhs) {
        return !(lhs == rhs);
    }
private:
    Interval1D m_intervalX;
    Interval1D m_intervalY;
};


#endif //QUADTREE_INTERVAL_H
