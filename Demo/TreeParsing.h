// TreeParsing.h
#ifndef TREEPARSING_H
#define TREEPARSING_H

#include <iostream>
#include <string>
#include <vector>

class TreeParsing {
public:
    class Node {
    public:
        Node(std::string val, Node* par);
        ~Node();

        void clearNode();
        Node* getChild(int index);
        Node* operator[](int index);
        void setValue(std::string str);
        std::string getValue() const;
        int size() const;
        


        Node* parent;
        std::string value;
        std::vector<Node*> children;
    };


    TreeParsing(std::string str);
    TreeParsing(const TreeParsing& other);
    TreeParsing(TreeParsing&& other);
    ~TreeParsing();

    void clearTree();

    // TreeParsing& operator=(const TreeParsing& other);
    TreeParsing& operator=(TreeParsing&& other);

    Node* getThis();
    Node* getLeafPtrStay(std::vector<int> path);
    Node* getLeafPtr(std::vector<int> path);
    std::string getLeaf(std::vector<int> path);

    void addChild(std::string val);



    void goToChildren(int index);
    void gpToLastChildren();
    Node* goToLeaf(std::vector<int> path);
    void goToParent();
    void goToRoot();

    void print() const;
    void print(Node* node, int level) const;

    operator Node*() const;

private:
    Node* root;
    Node* cursor;
};


#endif //TREEPARSING_H
