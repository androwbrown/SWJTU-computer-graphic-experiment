#include "WeilerAtherton.h"
#include <list>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cmath>

// Define a floating-point point structure for precision
struct PointFP {
    double x, y;
};

// Define a tolerance for float comparisons
const double EPSILON = 1e-6;

// Node structure using floating-point coordinates
struct Node {
    PointFP pt;
    bool isIntersection = false;
    bool isEntering = false;
    bool visited = false;
    double alpha = 0.0;
    Node* next = nullptr;
    Node* prev = nullptr;
    Node* corresponding = nullptr;
};

// Intersection function using floating-point arithmetic
bool getIntersection(PointFP p1, PointFP p2, PointFP p3, PointFP p4, PointFP& intersection, double& t) {
    double det = (p2.x - p1.x) * (p4.y - p3.y) - (p2.y - p1.y) * (p4.x - p3.x);

    if (std::abs(det) < EPSILON) {
        return false; // Parallel or collinear
    }

    t = ((p3.x - p1.x) * (p4.y - p3.y) - (p3.y - p1.y) * (p4.x - p3.x)) / det;
    double u = -((p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x)) / det;

    if (t >= -EPSILON && t <= 1.0 + EPSILON && u >= -EPSILON && u <= 1.0 + EPSILON) {
        intersection.x = p1.x + t * (p2.x - p1.x);
        intersection.y = p1.y + t * (p2.y - p1.y);
        return true;
    }
    return false;
}


bool isInside(const PointFP& p, const std::vector<PointFP>& polygon) {
    if (polygon.empty()) return false;
    bool inside = false;
    int n = polygon.size();
    for (int i = 0, j = n - 1; i < n; j = i++) {
        if (((polygon[i].y > p.y) != (polygon[j].y > p.y)) &&
            (p.x < (polygon[j].x - polygon[i].x) * (p.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x)) {
            inside = !inside;
        }
    }
    return inside;
}

double signedArea(const std::vector<PointFP>& polygon) {
    double area = 0.0;
    int n = polygon.size();
    if (n < 3) return 0.0;
    for (int i = 0; i < n; ++i) {
        PointFP p1 = polygon[i];
        PointFP p2 = polygon[(i + 1) % n];
        area += (p1.x * p2.y - p2.x * p1.y);
    }
    return area / 2.0;
}

// buildLinkedList function converting POINT to PointFP
Node* buildLinkedList(const std::vector<POINT>& polygon, std::vector<Node*>& all_nodes, std::vector<PointFP>& fp_polygon) {
    if (polygon.empty()) return nullptr;
    fp_polygon.clear();
    for(const auto& p : polygon) {
        fp_polygon.push_back({(double)p.x, (double)p.y});
    }
    
    Node* head = new Node{fp_polygon[0]};
    all_nodes.push_back(head);
    Node* current = head;
    for (size_t i = 1; i < fp_polygon.size(); ++i) {
        Node* newNode = new Node{fp_polygon[i]};
        all_nodes.push_back(newNode);
        current->next = newNode;
        newNode->prev = current;
        current = newNode;
    }
    current->next = head;
    head->prev = current;
    return head;
}


std::vector<std::vector<POINT>> weilerAthertonClip(const std::vector<POINT>& subjectPolygon, const std::vector<POINT>& clipPolygon) {
    std::vector<std::vector<POINT>> resultIntPolygons;
    std::vector<Node*> all_nodes;

    if (subjectPolygon.empty() || clipPolygon.size() < 3) {
        return resultIntPolygons;
    }

    std::vector<PointFP> subjectPolygonFP, clipPolygonFP;
    Node* subjList = buildLinkedList(subjectPolygon, all_nodes, subjectPolygonFP);
    Node* clipList = buildLinkedList(clipPolygon, all_nodes, clipPolygonFP);

    std::vector<Node*> intersections;

    // S为源裁剪矩形
    Node* s_current = subjList;
    do {
        Node* s_next = s_current->next;
        std::vector<Node*> current_edge_intersections;

        Node* c_current = clipList;
        do {
            Node* c_next = c_current->next;
            PointFP intersection_pt;
            double t;
       //找交点，并分别插入CP和SP
            if (getIntersection(s_current->pt, s_next->pt, c_current->pt, c_next->pt, intersection_pt, t)) {
                //intersection_pt为交点的引用
                Node* s_intersect_node = new Node{intersection_pt, true};
                all_nodes.push_back(s_intersect_node);
                s_intersect_node->alpha = t;

                Node* c_intersect_node = new Node{intersection_pt, true};
                all_nodes.push_back(c_intersect_node);

                s_intersect_node->corresponding = c_intersect_node;
                c_intersect_node->corresponding = s_intersect_node;

                current_edge_intersections.push_back(s_intersect_node);
                
                double dx_c = c_next->pt.x - c_current->pt.x;
                double dy_c = c_next->pt.y - c_current->pt.y;

                if (std::abs(dx_c) > std::abs(dy_c)) {
                    c_intersect_node->alpha = (dx_c != 0) ? (intersection_pt.x - c_current->pt.x) / dx_c : 0.0;
                } else {
                    c_intersect_node->alpha = (dy_c != 0) ? (intersection_pt.y - c_current->pt.y) / dy_c : 0.0;
                }

                Node* temp = c_current;
                while(temp->next != c_next && temp->next->isIntersection && temp->next->alpha < c_intersect_node->alpha) {
                    temp = temp->next;
                }
                c_intersect_node->next = temp->next;
                c_intersect_node->prev = temp;
                temp->next->prev = c_intersect_node;
                temp->next = c_intersect_node;

                intersections.push_back(s_intersect_node);
            }
            c_current = c_next;
        } while (c_current != clipList);

        std::sort(current_edge_intersections.begin(), current_edge_intersections.end(), [](const Node* a, const Node* b) {
            return a->alpha < b->alpha;
        });

        for (Node* intersect_node : current_edge_intersections) {
            intersect_node->next = s_current->next;
            intersect_node->prev = s_current;
            s_current->next->prev = intersect_node;
            s_current->next = intersect_node;
            s_current = intersect_node;
        }

        s_current = s_next;
    } while (s_current != subjList);

    // 没有交点的情况
    if (intersections.empty()) {
        if (!subjectPolygonFP.empty() && isInside(subjectPolygonFP[0], clipPolygonFP)) {
            resultIntPolygons.push_back(subjectPolygon);
        } else if (!clipPolygonFP.empty() && isInside(clipPolygonFP[0], subjectPolygonFP)) {
            resultIntPolygons.push_back(clipPolygon);
        }
        for(auto n : all_nodes) delete n;
        return resultIntPolygons;
    }

    // 标记节点为入点或出点
    bool subject_is_cw = signedArea(subjectPolygonFP) > 0;
    for (Node* s_intersect : intersections) {
        Node* s_prev = s_intersect->prev;
        Node* s_start = s_intersect;
        while(s_prev->isIntersection && s_prev != s_start) s_prev = s_prev->prev;//找到交点紧邻的前面那个点
        Node* s_next = s_intersect->next;
        while(s_next->isIntersection && s_next != s_start) s_next = s_next->next;//找到交点紧邻的后面那个点
          //c是交点对应在裁剪矩形（CP）中的那个点
        Node* c_intersect = s_intersect->corresponding;
        //同上
        Node* c_prev = c_intersect->prev;
        Node* c_start = c_intersect;
        while(c_prev->isIntersection && c_prev != c_start) c_prev = c_prev->prev;
        Node* c_next = c_intersect->next;
        while(c_next->isIntersection && c_next != c_start) c_next = c_next->next;
        
        double cross_product = (s_next->pt.x - s_prev->pt.x) * (c_next->pt.y - c_prev->pt.y) -
                               (s_next->pt.y - s_prev->pt.y) * (c_next->pt.x - c_prev->pt.x);
        
        if (subject_is_cw) {
            s_intersect->isEntering = (cross_product < 0);
        } else {
            s_intersect->isEntering = (cross_product > 0);
        }
    }

    // 从原多边形入点开始，追踪
    for (Node* start_node : intersections) {
        if (start_node->isEntering && !start_node->visited) {
            std::vector<PointFP> new_poly_fp;
            Node* current = start_node;
            bool forward = true; 
            
            do {
                if (current->visited) {
                    // Already visited in this path, indicates a cycle. Break to prevent infinite loop.
                    break;
                }
                current->visited = true;
                if (current->corresponding) {
                    current->corresponding->visited = true;
                }

                new_poly_fp.push_back(current->pt);

                Node* next_node = current->next;
                while (next_node && next_node != current && !next_node->isIntersection) {
                    new_poly_fp.push_back(next_node->pt);
                    next_node = next_node->next;
                }
                
                //如果形成了环，说明找到了一个裁剪后多边形，直接退出
                if (next_node == start_node) {
                    current = next_node;
                    break;
                }
                
                // 继续
                current = next_node->corresponding;
                if (current == nullptr) { 
                    break;
                }

            } while (current != start_node && current != nullptr);

             if(new_poly_fp.size() > 2) {
                std::vector<POINT> new_poly_int;
                for(const auto& p_fp : new_poly_fp){
                    new_poly_int.push_back({(long)round(p_fp.x), (long)round(p_fp.y)});
                }
                resultIntPolygons.push_back(new_poly_int);
             }
        }
    }
    
    for(auto n : all_nodes) delete n;
    return resultIntPolygons;
}
