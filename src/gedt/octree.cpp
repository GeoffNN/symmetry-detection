#include "gedt/octree.h"


OCTree::OCTree(Mesh &mesh, const BoundingBox &boundingBox, int maxDepth)
        : grid(boundingBox, std::pow(2, maxDepth)), mesh(mesh), maxDepth(maxDepth),
          voxelValues(grid.size, grid.size, grid.size) {


}


void OCTree::computeMesh(const list<Mesh::FaceHandle> &potentialFaces) {
    root = unique_ptr<Node>(new Node(this, mesh, grid.bb, potentialFaces, maxDepth));
}

//
//OCTree::Node::Node(OCTree *tree, Mesh &mesh, const BoundingBox &boundingBox,
//                   const list<Mesh::FaceHandle> &potentialFaces,
//                   int maxDepth,
//                   int depth) :
//        Node(tree, mesh, boundingBox, potentialFaces, maxDepth, depth, nullptr) {
//
//
//}


OCTree::Node::Node(OCTree *tree, Mesh &mesh, const BoundingBox &boundingBox,
                   const list<Mesh::FaceHandle> &potentialFaces,
                   int maxDepth,
                   int depth, Node *parent) :
        mesh(mesh), boundingBox(boundingBox), depth(depth), parent(parent) {

    list<Mesh::FaceHandle> faces;
//    std::cout << "Testing for " << boundingBox << std::endl;
    for (auto f : potentialFaces) {
//        displayFace(mesh, f);
        if (boxContainsFace(f, boundingBox)) {
//            std::cout << " OK";
            faces.push_back(f);
        }
//        std::cout << std::endl;
    }

//    std::cout << std::endl;
//    std::cout << std::endl;

    facesNumber = faces.size();
    if (depth < maxDepth) {
        vector<BoundingBox> subdivisions = boundingBox.subdivisions();
        for (int i = 0; i < subdivisions.size(); i++) {
            children.push_back(new Node(tree, mesh, subdivisions[i], faces, maxDepth, depth + 1, this));
        }
    } else {
        tree->voxelValues(tree->grid.coordsFromPoint(boundingBox.getCenter())) = int(facesNumber);
    }
}

OCTree::Node::~Node() {
    for (auto c : children) {
        delete (c);
    }
}

bool OCTree::Node::boxContainsFace(Mesh::FHandle f, const BoundingBox &bb) {
    double r = (bb.getMax() - bb.getCenter()).norm();
    vector<Vec> points;
    for (Mesh::FaceVertexIter it = mesh.fv_iter(f); it.is_valid(); it++) {
        points.push_back(mesh.point(*it));
    }
    return FaceIntersection::faceSphereIntersection(points, bb.getCenter(), r);
}



