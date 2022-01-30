
/************************************************************************
 * Tree.C:
 * Maintains a parse tree that will allow for manipulation of arithmeteic 
 * expressions.
 ***********************************************************************/

#include <CC/iostream.h>

class Node {
/* 
 * Each node represents either an integer constant, a unary operator
 * or a binary operator.  One class, three uses.
 *
 * This is an abstract base class.
 */
    int use;
protected:
    Node() { use = 1; }                          // use count
    friend class Tree;
    friend ostream& operator<< 
	   (ostream&, const Tree&);
    virtual void print (ostream&) const = 0;
    virtual ~Node() {}
};

class Tree {
/*
 * To avoid copying trees, we will need the Tree class to represent a 
 * pointer to a tree.
 *             Tree       Node
 *            +----+     +----+
 *            |  --|---->|    |
 *            +----+     +----+
 */
    friend class Node;
    friend ostream& operator<< 
	   (ostream&, const Tree&);
    Node* p;                         // pointer to Node only data
public:
    Tree(int);                       // given int constant e.g. 5
    Tree(char*,Tree);                // given char ptr and Tree e.g. -5
    Tree(char*,Tree,Tree);           // given char ptr + 2 Trees e.g. 4+6
    Tree(const Tree& t) { p = t.p; ++p->use; }     // copy constructor
    ~Tree() { if (--p->use == 0) delete p; }       // destructor
    void operator=(const Tree& t);                 
};

ostream&
operator<<(ostream& o, const Tree& t)
/*
 * To print a Tree, call print member of Node pointed to by p.
 */
{
    t.p->print (o);  
    return o;
}

class IntNode: public Node {
/*
 * A node that contains an integer constant.
 */
    friend class Tree;
    int n;
    IntNode (int k): n (k) {}
    void print (ostream& o) const { o << n; }
};

class UnaryNode: public Node {
/*
 * A node that contains a Tree (operand) and a char pointer (operator).
 */
    friend class Tree;
    char* op;
    Tree opnd;
    UnaryNode (char* a, Tree b): op (a), opnd (b) {}
    void print (ostream& o) const { o << "(" << op << opnd << ")"; }
};

class BinaryNode: public Node {
/*
 * A node that contains two Trees (operands) and a char pointer (operator).
 */
    friend class Tree;
    char* op;
    Tree left;
    Tree right;
    BinaryNode (char* a, Tree b, Tree c): op (a), left (b), right (c) {}
    void print (ostream& o) const { o << "(" << left << op << right << ")"; }
};

Tree::Tree(int n)
{ p = new IntNode (n); }

Tree::Tree(char* op, Tree t)
{ p = new UnaryNode (op, t); }

Tree::Tree(char* op, Tree left, Tree right)
{ p = new BinaryNode (op, left, right); }

void 
Tree::operator=(const Tree& t)
{
    ++t.p->use;
    if  (--p->use == 0)
	   delete p;
    p = t.p;
}

/*-------------------------------------------------------------------------
 * Main program.
 *-----------------------------------------------------------------------*/

main()
{
   Tree t ("*", Tree("-",5), Tree("+",3,4));

   cout << t << "\n";
   t = Tree("*", t, t);
   cout << t << "\n";
}
