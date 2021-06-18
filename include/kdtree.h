/*
* @File: kdtree.h
* @Date:   2021-06-16 08:27:17
* @Last Modified time: 2021-06-16 08:52:55
*/

#ifndef _KDTREE_H
#define _KDTREE_H

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <queue>
#include <string>
#include <vector>

namespace kdtree {
    template<typename PointT, typename PointU>
    static float distance2(const PointT &p1, const PointU &p2) {
        float dist2 = 0;
        for (int i = 0; i < PointT::dim; ++i) {
            dist2 += (p1[i] - p2[i]) * (p1[i] - p2[i]);
        }
        return dist2;
    }

    struct Node {
        int axis;
        int idx;
        Node *leftChild;
        Node *rightChild;

        Node() : axis(-1), idx(-1), leftChild(nullptr), rightChild(nullptr) {}
    };

    template<typename PointT>
    class KdTree {
    public:

        std::vector<PointT> points;
        Node *root;

        Node *buildNode(int *indices, int n_points, int depth) {
            if (n_points <= 0) return nullptr;

            const int axis = depth % PointT::dim;

            std::sort(indices, indices + n_points, [&](const int idx1, const int idx2) {
                return points[idx1][axis] < points[idx2][axis];
            });

            const int mid = (n_points - 1) / 2;

            Node *node = new Node();
            node->axis = axis;
            node->idx = indices[mid];
            node->leftChild = buildNode(indices, mid, depth + 1);
            node->rightChild =
                    buildNode(indices + mid + 1, n_points - mid - 1, depth + 1);

            return node;
        }

        void destructNode(Node *node) {
            if (!node) return;
            if (node->leftChild) {
                destructNode(node->leftChild);
            }
            if (node->rightChild) {
                destructNode(node->rightChild);
            }
            delete node;
        }

        void toGraphVizNode(const Node *node, std::ofstream &stream) const {
            if (node->leftChild) {
                stream << node->idx << "->" << node->leftChild->idx
                       << " [label=" << node->axis << "]" << std::endl;
                toGraphVizNode(node->leftChild, stream);
            } else {
                const std::string nullNode = "null" + std::to_string(node->idx) + "0";
                stream << nullNode << " [label=\"\", shape=\"none\"]" << std::endl;

                stream << node->idx << "->" << nullNode << " [label=" << node->axis << "]"
                       << std::endl;
            }

            if (node->rightChild) {
                stream << node->idx << "->" << node->rightChild->idx
                       << " [label=" << node->axis << "]" << std::endl;
                toGraphVizNode(node->rightChild, stream);
            } else {
                const std::string nullNode = "null" + std::to_string(node->idx) + "1";
                stream << nullNode << " [label=\"\", shape=\"none\"]" << std::endl;

                stream << node->idx << "->" << nullNode << " [label=" << node->axis << "]"
                       << std::endl;
            }
        }

        template<typename PointU>
        void searchNearestNode(const Node *node, const PointU &queryPoint,
                               int &idx_nearest, float &minDist2) const {
            if (!node) return;

            const PointT &median = points[node->idx];

            const float dist2 = distance2(queryPoint, median);
            if (dist2 < minDist2) {
                idx_nearest = node->idx;
                minDist2 = dist2;
            }

            const bool isLower = queryPoint[node->axis] < median[node->axis];
            if (isLower) {
                searchNearestNode(node->leftChild, queryPoint, idx_nearest, minDist2);
            } else {
                searchNearestNode(node->rightChild, queryPoint, idx_nearest, minDist2);
            }

            const float dist_to_siblings = median[node->axis] - queryPoint[node->axis];
            if (minDist2 > dist_to_siblings * dist_to_siblings) {
                if (isLower) {
                    searchNearestNode(node->rightChild, queryPoint, idx_nearest, minDist2);
                } else {
                    searchNearestNode(node->leftChild, queryPoint, idx_nearest, minDist2);
                }
            }
        }

        using KNNQueue = std::priority_queue<std::pair<float, int>>;

        template<typename PointU>
        void searchKNearestNode(const Node *node, const PointU &queryPoint, int k,
                                KNNQueue &queue) const {
            if (!node) return;

            const PointT &median = points[node->idx];

            const float dist2 = distance2(queryPoint, median);
            queue.emplace(dist2, node->idx);

            if (queue.size() > k) {
                queue.pop();
            }

            const bool isLower = queryPoint[node->axis] < median[node->axis];
            if (isLower) {
                searchKNearestNode(node->leftChild, queryPoint, k, queue);
            } else {
                searchKNearestNode(node->rightChild, queryPoint, k, queue);
            }

            const float dist_to_siblings = median[node->axis] - queryPoint[node->axis];
            if (queue.top().first > dist_to_siblings * dist_to_siblings) {
                if (isLower) {
                    searchKNearestNode(node->rightChild, queryPoint, k, queue);
                } else {
                    searchKNearestNode(node->leftChild, queryPoint, k, queue);
                }
            }
        }

        template<typename PointU>
        void sphericalRangeSearchNode(const Node *node, const PointU &queryPoint,
                                      float r, std::vector<int> &list) const {
            if (!node) return;

            const PointT &median = points[node->idx];

            const float dist2 = distance2(queryPoint, median);
            if (dist2 < r * r) {
                list.push_back(node->idx);
            }

            const bool isLower = queryPoint[node->axis] < median[node->axis];
            if (isLower) {
                sphericalRangeSearchNode(node->leftChild, queryPoint, r, list);
            } else {
                sphericalRangeSearchNode(node->rightChild, queryPoint, r, list);
            }

            const float dist_to_siblings = median[node->axis] - queryPoint[node->axis];
            if (r * r > dist_to_siblings * dist_to_siblings) {
                if (isLower) {
                    sphericalRangeSearchNode(node->rightChild, queryPoint, r, list);
                } else {
                    sphericalRangeSearchNode(node->leftChild, queryPoint, r, list);
                }
            }
        }

        template<typename PointU>
        bool inRect(const PointT &median, const PointU &queryPoint, float width, float height) const {
            return median[0] <= queryPoint[0] + width / 2 && median[0] >= queryPoint[0] - width / 2
                   && median[1] <= queryPoint[1] + height / 2 && median[1] >= queryPoint[1] - height / 2;
        }

        template<typename PointU>
        void OrthoWindowRangeSearchNode(const Node *node, const PointU &queryPoint,
                                        float width, float height, std::vector<int> &list) const {
            if (!node) return;

            const PointT &median = points[node->idx];

            if (inRect(median, queryPoint, width, height)) {
                list.push_back(node->idx);
            }

            if (node->axis == 0) {
                if (queryPoint[node->axis] - width / 2 < median[node->axis]) {
                    OrthoWindowRangeSearchNode(node->leftChild, queryPoint, width, height, list);
                }
                if (queryPoint[node->axis] + width / 2 >= median[node->axis]) {
                    OrthoWindowRangeSearchNode(node->rightChild, queryPoint, width, height, list);
                }
            } else {
                if (queryPoint[node->axis] - height / 2 < median[node->axis]) {
                    OrthoWindowRangeSearchNode(node->leftChild, queryPoint, width, height, list);
                }
                if (queryPoint[node->axis] + height / 2 >= median[node->axis]) {
                    OrthoWindowRangeSearchNode(node->rightChild, queryPoint, width, height, list);
                }
            }
        }

    public:
        KdTree() : root(nullptr) {}

        KdTree(std::initializer_list<PointT> init) : root(nullptr), points(init) {}

        KdTree(const std::vector<PointT> &points) : root(nullptr), points(points) {}

        ~KdTree() { destructTree(); }

        void buildTree() {
            std::vector<int> indices(points.size());
            std::iota(indices.begin(), indices.end(), 0);

            root = buildNode(indices.data(), points.size(), 0);
        }

        void destructTree() { destructNode(root); }

        void toGraphviz(const std::string &filename) const {
            std::ofstream file(filename);
            if (!file) {
                std::cerr << "failed to create " << filename << std::endl;
            }

            file << "digraph {" << std::endl;
            toGraphVizNode(root, file);
            file << "}" << std::endl;

            file.close();
        }

        template<typename PointU>
        int searchNearest(const PointU &queryPoint) const {
            int idx_nearest;
            float minDist2 = std::numeric_limits<float>::max();
            searchNearestNode(root, queryPoint, idx_nearest, minDist2);
            return idx_nearest;
        }


        template<typename PointU>
        std::vector<int> searchKNearest(const PointU &queryPoint, int k) const {
            KNNQueue queue;
            searchKNearestNode(root, queryPoint, k, queue);

            std::vector<int> ret(queue.size());
            for (int & i : ret) {
                i = queue.top().second;
                queue.pop();
            }
            return ret;
        }

        template<typename PointU>
        std::vector<int> sphericalRangeSearch(const PointU &queryPoint,
                                              float r) const {
            std::vector<int> ret;
            sphericalRangeSearchNode(root, queryPoint, r, ret);
            return ret;
        }

        template<typename PointU>
        std::vector<int> OrthoRangeSearch(const PointU &queryPoint,
                                          float wdith, float height) const {
            std::vector<int> ret;
            OrthoWindowRangeSearchNode(root, queryPoint, wdith, height, ret);
            return ret;
        }

    };

}
#endif