//
// Created by n.soungadoy on 21/06/2017.
//

#ifndef QUADTREE_QUADTREE_H
#define QUADTREE_QUADTREE_H

#include <memory>
#include <vector>
#include "interval.h"

const int DEFAULT_MAX_SIZE = 5;
enum type { particle, edge, halfplane };

using Element = std::pair<type, int>;
using TreeElement = std::pair<Interval2D, Element>;

class QuadTree {

public:
    typedef std::shared_ptr<QuadTree> quadtree_ptr;
//    QuadTree();
//    QuadTree(int max_size, int level);
    QuadTree(Interval2D bounds, int max_size=DEFAULT_MAX_SIZE,  int level=0);

//    QuadTree(const QuadTree& obj);

    void Push(const TreeElement& elt);

    std::vector<TreeElement> GetElements();

    std::vector<quadtree_ptr> GetChildren();


private:
    bool Assign(const TreeElement& elt);

    bool Split();

    bool split_;
    int level_;
    int size_;
    int max_size_;
    Interval2D bounds_;
    std::vector<TreeElement> elements_;

    quadtree_ptr NE_;
    quadtree_ptr NW_;
    quadtree_ptr SE_;
    quadtree_ptr SW_;
};


#endif //QUADTREE_QUADTREE_H
