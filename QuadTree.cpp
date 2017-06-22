//
// Created by n.soungadoy on 21/06/2017.
//

#include "interval.h"
#include <vector>
#include <memory>
#include "QuadTree.h"


QuadTree::QuadTree(double xmin, double xmax, double ymin, double ymax, int max_size, int level) :
        bounds_(xmin, xmax, ymin, ymax) {
    max_size_ = max_size;
    level_ = level;
    size_ = 0;
    split_ = false;
}

QuadTree::QuadTree(Interval2D bounds, int max_size, int level) : bounds_(bounds) {
    max_size_ = max_size;
    level_ = level;
    size_ = 0;
    split_ = false;
}

bool QuadTree::Assign(const TreeElement& elt) {

    if (NE_->bounds_.overlaps(elt.first)) {
        NE_->Push(elt);
    }
    if (NW_->bounds_.overlaps(elt.first)) {
        NW_->Push(elt);
    }
    if (SE_->bounds_.overlaps(elt.first)) {
        SE_->Push(elt);
    }
    if (SW_->bounds_.overlaps(elt.first)) {
        SW_->Push(elt);
    }
    return true;
}

bool QuadTree::Split() {
    double xmin = bounds_.xmin();
    double xmax = bounds_.xmax();
    double ymin = bounds_.ymin();
    double ymax = bounds_.ymax();

    double xmid = xmin + (xmax-xmin)/2;
    double ymid = ymin + (ymax-ymin)/2;

    Interval2D boundsNE = Interval2D(xmid, xmax, ymid, ymax);
    NE_ = std::make_shared<QuadTree>(boundsNE, max_size_, level_ + 1);
    Interval2D boundsNW = Interval2D(xmin, xmid, ymid, ymax);
    NW_ = std::make_shared<QuadTree>(boundsNW, max_size_, level_ + 1);
    Interval2D boundsSE = Interval2D(xmid, xmax, ymin, ymid);
    SE_ = std::make_shared<QuadTree>(boundsSE, max_size_, level_ + 1);
    Interval2D boundsSW = Interval2D(xmin, xmid, ymin, ymid);
    SW_ = std::make_shared<QuadTree>(boundsSW, max_size_, level_ + 1);

    split_ = true;
}


void QuadTree::Push(const TreeElement& element) {
    size_++;
    if (!split_) {
        elements_.push_back(element);
        if (size_ > max_size_) {

            Split();
            while(!elements_.empty()) {
                TreeElement element = elements_.back();
                elements_.pop_back();
                Assign(element);
            }
        }
    } else {
        Assign(element);
    }

}

std::vector<TreeElement> QuadTree::GetElements() {
    return elements_;
}

std::vector<std::shared_ptr<QuadTree> > QuadTree::GetChildren() {
    std::vector<QuadTree::quadtree_ptr> children;
    if (split_) {
        children.push_back(NE_);
        children.push_back(NW_);
        children.push_back(SE_);
        children.push_back(SW_);
    }
    return children;
}




