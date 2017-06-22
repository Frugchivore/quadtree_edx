#include <iostream>
#include <stdexcept>
#include <vector>
#include <stack>
#include <chrono>
#include <random>
#include <queue>
#include <map>
#include "interval.h"
#include "QuadTree.h"



void test_Interval1D();
void test_Interval2D();
void test_QuadTree();
std::map<std::string, double> test_timing_quadtree();
int collision_check(std::vector<TreeElement> items);

int main() {
    std::cout << "Running test_Interval1D..." << std::endl;
    try {
        test_Interval1D();
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << " in test_Interval1D" << std::endl;
        return -1;
    }
    std::cout << "done." << std::endl;

    std::cout << "Running test_Interval2D..." << std::endl;
    try {
        test_Interval2D();
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << " in test_Interval2D" << std::endl;
        return -1;
    }
    std::cout << "done." << std::endl;

    std::cout << "Running test_QuadTree..." << std::endl;
    try {
        test_QuadTree();
    }
    catch (const std::exception& ex) {
        std::cerr << ex.what() << " in test_QuadTree" << std::endl;
        return -1;
    }
    std::cout << "done." << std::endl;

    std::cout << "Running timing test" << std::endl;
    std::map<std::string, double> times;
    times = test_timing_quadtree();
    std::cout << "bounds timing: " << times["bounds"] << std::endl;
    std::cout << "fill tree timing: " << times["fill_tree"] << std::endl;
    std::cout << "parse tree timing: " << times["parse_tree"] << std::endl;
    std::cout << "total timing: " << times["bounds"] + times["fill_tree"] + times["parse_tree"] << std::endl;
    return 0;
}

void test_Interval1D() {
    Interval1D interval1(0, 10);
    Interval1D interval2(0, 10);

    bool threwn = false;
    try {
        Interval1D interv(10, 0);
    } catch(std::invalid_argument& exc) {
        threwn = true;
    }
    if (!threwn) {
        throw std::runtime_error("failed : imax > imin");
    }

    if (!(interval1 == interval2)) {
        throw std::runtime_error("failed : operator==");
    }
    Interval1D interval3(0.0, 11.0);
    if (!(interval1 != interval3)) {
        throw std::runtime_error("failed : operator!=");
    }

    Interval1D interval4(3.0, 5.0), interval5(-10, 10), interval6(5.0, 15.0), interval7(-2.0, 1);

    if (!(interval1.overlaps(interval2))) {
        throw std::runtime_error("failed : overlaps interval2");
    }

    if (!(interval1.overlaps(interval3))) {
        throw std::runtime_error("failed : overlaps interval3");
    }

    if (!(interval1.overlaps(interval4))) {
        throw std::runtime_error("failed : overlaps interval4");
    }

    if (!(interval1.overlaps(interval5))) {
        throw std::runtime_error("failed : overlaps interval5");
    }

    if (!(interval1.overlaps(interval6))) {
        throw std::runtime_error("failed : overlaps interval6");
    }

    if (!(interval1.overlaps(interval7))) {
        throw std::runtime_error("failed : overlaps interval7");
    }

    Interval1D interval8(11, 12);

    if (interval1.overlaps(interval8)) {
        throw std::runtime_error("failed : non-overlaps interval8");
    }
}

void test_Interval2D(){
    Interval2D interv1(0, 1, 2, 4);
    Interval2D interv2(0, 1, 2, 4);

    if (!(interv1 == interv2)) {
        throw std::runtime_error("failed : operator==");
    }

    Interval2D interv3(-1, 1, 2, 4);
    if (!(interv1 != interv3)) {
        throw std::runtime_error("failed : operator!=");
    }

    Interval2D interv4(0.0, 1.0, 0.0, 1.0), interv5(1.5, 2.5, 2, 4);
    if (interv1.overlaps(interv4)) {
        throw std::runtime_error("failed : non-overlaps interval4");
    }
    if (interv1.overlaps(interv5)) {
        throw std::runtime_error("failed : non-overlaps interval5");
    }

    Interval2D interv6(0.5, 1.5, 0, 3);
    if (!(interv1.overlaps(interv6))) {
        throw std::runtime_error("failed : overlaps interval6");
    }

}

struct Ball {
    double x;
    double y;
    double r;

    bool operator==(const Ball& rhs)
    {
        return x == rhs.x && y == rhs.y && r == rhs.r;
    }
};

Interval2D make_box(Ball b) {
    double xmin = b.x - b.r;
    double xmax = b.x + b.r;
    double ymin = b.y - b.r;
    double ymax = b.y + b.r;
    return Interval2D(xmin, xmax, ymin, ymax);
}

void test_QuadTree() {
//    typedef std::shared_ptr<QuadTree> quadtree_ptr ;

    Interval2D bounds(0.0, 10.0, 0.0, 10.0);
    QuadTree tree(bounds, 2);
    Ball b1 = {2.5, 2.5, 1.0};
    TreeElement elt1(make_box(b1), Element(particle, 1));
    tree.Push(elt1);

    Ball b2 = {7.5, 2.5, 1.0};
    TreeElement elt2(make_box(b2), Element(particle, 2));
    tree.Push(elt2);

    // Expect no children yet and 2 elements.
    std::vector<TreeElement> elements = tree.GetElements();

    if (elements.size() != 2) {
        throw std::runtime_error("failed : GetElements expecting 2 elements");
    }

    std::vector<QuadTree::quadtree_ptr> children = tree.GetChildren();

    if (children.size() != 0) {
        throw std::runtime_error("failed : GetChildren expecting 0 children");
    }

    Ball b3 = {2.5, 7.5, 1.0};
    TreeElement elt3(make_box(b3), Element(particle, 3));
    tree.Push(elt3);

    children = tree.GetChildren();

    if (children.size() != 4) {
        throw std::runtime_error("failed : GetChildren expecting 4 children");
    }

    Ball b4 = {5.0, 7.5, 1.0};
    TreeElement elt4(make_box(b4), Element(particle, 4));
    tree.Push(elt4);

    // elt4 should be in several node at the same time.

}



std::map<std::string, double> test_timing_quadtree() {
    std::map<std::string, double> times;
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elapsed;

    std::default_random_engine generatorX, generatorY;
    generatorX.seed(511412);
    generatorY.seed(17099766);
    std::uniform_real_distribution<double> distributionX(0.0, 400);
    std::uniform_real_distribution<double> distributionY(0.0, 400);
    auto Xposition = std::bind(distributionX, generatorX);
    auto Yposition = std::bind(distributionY, generatorY);

    typedef std::pair<int,Ball> Item;
    std::vector<Item> particles;

    int num_particle = 20000;
    std::cout << "number of particles: " << num_particle << std::endl;
    int max_size = 55;
    // Create the particles
    for (int i=0; i < num_particle; ++i) {
        Ball b = { Xposition(), Yposition(), 1.2};
        particles.push_back(Item(i, b));
    }

    //find bounds of the map.
    start = std::chrono::high_resolution_clock::now();
    double xmin = std::numeric_limits<double>::infinity();
    double xmax = -std::numeric_limits<double>::infinity();
    double ymin = std::numeric_limits<double>::infinity();
    double ymax = -std::numeric_limits<double>::infinity();

    for (std::vector<Item>::const_iterator particle_iter = particles.begin(); particle_iter != particles.end(); particle_iter++) {
        double x = particle_iter->second.x;
        double y = particle_iter->second.y;
        double r = particle_iter->second.r;

        if (x+r > xmax) xmax = x+r;
        if (x-r < xmin) xmin = x-r;
        if (y+r > ymax) ymax = y+r;
        if (y-r < ymin) ymin = y-r;
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = end-start;
    times["bounds"] = elapsed.count();
    // create tree
    QuadTree tree(xmin, xmax, ymin, ymax, max_size);

    // fill tree
    start = std::chrono::high_resolution_clock::now();
    for (std::vector<Item>::const_iterator particle_iter = particles.begin(); particle_iter != particles.end(); particle_iter++) {
        TreeElement item(make_box(particle_iter->second), Element(particle, particle_iter->first));
        tree.Push(item);
    }

    end = std::chrono::high_resolution_clock::now();
    elapsed = end-start;
    times["fill_tree"] = elapsed.count();

    //parse tree
    start = std::chrono::high_resolution_clock::now();

    std::queue<QuadTree> parse_queue;
    parse_queue.push(tree);
    int collisions_count = 0;
    while (!parse_queue.empty()) {
        QuadTree curr = parse_queue.front();
        parse_queue.pop();

        std::vector<TreeElement> items = curr.GetElements();
        collisions_count += collision_check(items);
        std::vector<std::shared_ptr<QuadTree> > children = curr.GetChildren();
        std::vector<std::shared_ptr<QuadTree>>::const_iterator child_iter;
        for (child_iter = children.begin(); child_iter != children.end(); child_iter++) {
            parse_queue.push(**child_iter);
        }
    }
    end = std::chrono::high_resolution_clock::now();
    elapsed = end-start;
    times["parse_tree"] = elapsed.count();

    std::cout << collisions_count << " collisions accounted for." << std::endl;
    return times;
}

int collision_check(std::vector<TreeElement> items) {
    int count = 0;
    for (int i = 0; i < items.size(); i++) {
        for(int j = i+1; j < items.size(); j++) {
            TreeElement& first = items.at(i);
            TreeElement& second = items.at(j);

            if (first.first.overlaps(second.first)) {
                count++;
            }

        }
    }
    return count;
}