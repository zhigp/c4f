// Binary Search Tree
// avg/worst time:  O(lgn) / O(n)
// avg/worst space: O(n) / O(n)


#include <iostream>
#include <queue>
using namespace std;

#define NULL_INT            -999999
#define BSTINSERT_KEY_EXIST -1
#define BSTINSERT_NULL_KEY  -2
#define BSTROTATE_LSUB_NULL -1
#define BSTROTATE_RSUB_NULL -1

class BSNode{
public:
    int key=NULL_INT;
    BSNode *p=nullptr, *lsub=nullptr, *rsub=nullptr;
    BSNode(int k):key(k){}
};

class BSTree{
public:
    BSNode *root=nullptr;
    void InorderWalk(BSNode *x);
    void PreorderWalk(BSNode *x);
    void PostorderWalk(BSNode *x);
    void BreadthWalk(BSNode *x);
    BSNode* Search(BSNode *x, int k);
    BSNode* SearchR(BSNode *x, int k);
    BSNode* Min(BSNode *x);
    BSNode* Max(BSNode *x);
    BSNode* Successor(BSNode *x);
    BSNode* Predecessor(BSNode *x);
    int Insert(BSNode *x);
    int Transplant(BSNode* u, BSNode* v);
    int Delete(BSNode *x);
    int RotateL(BSNode *x);
    int RotateR(BSNode *x);
};

void BSTree::InorderWalk(BSNode *x){
    if(x==nullptr)
      return ;
    InorderWalk(x->lsub);
    cout << x->key << "  ";
    InorderWalk(x->rsub);
}

void BSTree::PreorderWalk(BSNode *x){
    if(x==nullptr)
      return ;
    cout << x->key << "  ";
    PreorderWalk(x->lsub);
    PreorderWalk(x->rsub);
}

void BSTree::PostorderWalk(BSNode *x){
    if(x==nullptr)
      return ;
    PostorderWalk(x->lsub);
    PostorderWalk(x->rsub);
    cout << x->key << "  ";
}

void BSTree::BreadthWalk(BSNode *x){
    if(x==nullptr)
      return ;
    queue<BSNode *> bsq;
    BSNode *p=root;
    bsq.push(p);
    while(!bsq.empty()){
        p=bsq.front();
        cout << p->key << "  ";
        bsq.pop();
        if(p->lsub!=0)
          bsq.push(p->lsub);
        if(p->rsub!=0)
          bsq.push(p->rsub);
    }
}

//recurrent
BSNode* BSTree::SearchR(BSNode *x, int k){
    if(k==x->key)
      return x;
    if(k<x->key && x->lsub!=nullptr)
      return SearchR(x->lsub, k);
    if(k>x->key && x->rsub!=nullptr)
      return SearchR(x->rsub, k);

    return nullptr;
}
//iterative
BSNode* BSTree::Search(BSNode *x, int k){
    while(k!=x->key){
        if(k<x->key && x->lsub!=nullptr)
          x=x->lsub;
        else if(k>x->key && x->rsub!=nullptr)
          x=x->rsub;
        else
          return nullptr;
    }
    return x;
}

BSNode* BSTree::Min(BSNode *x){
    while(x->lsub!=nullptr)
      x=x->lsub;
    return x;
}

BSNode* BSTree::Max(BSNode *x){
    while(x->rsub!=nullptr)
      x=x->rsub;
    return x;
}

BSNode* BSTree::Successor(BSNode *x){
//return the smallest element among which >x
    if(x->rsub!=nullptr)
      return Min(x->rsub);
    if(x->p==nullptr)
      return nullptr;
    
    BSNode *y=x->p;
    while(x==y->rsub){
        if(y->p==nullptr)
          return nullptr;
        x=y;
        y=y->p;
    }
    return y;
}

BSNode* BSTree::Predecessor(BSNode *x){
//return the biggest element among which <x
    if(x->lsub!=nullptr)
      return Max(x->lsub);
    if(x->p==nullptr)
      return nullptr;

    BSNode *y=x->p;
    while(x==y->lsub){
        if(y->p==nullptr)
          return nullptr;
        x=y;
        y=y->p;
    }
    return y;
}

int BSTree::Insert(BSNode *x){
    
    if(x->key==NULL_INT){
        cout << "ERR:Insert(): insert node without key value !!!" << endl;
        return BSTINSERT_NULL_KEY;
    }
    if(root==nullptr){
        root=x;
        return 0;
    }
    
    BSNode *y=root, *z=nullptr;
    while(y!=nullptr){
        z=y;
        if(x->key==y->key){
            cout << "ERR:Insert(): key already exists in tree !!!" << endl;
            return BSTINSERT_KEY_EXIST;
        }
        if(x->key<y->key)
          y=y->lsub;
        else
          y=y->rsub;
    }
    x->p=z;
    if(x->key<z->key)
      z->lsub=x;
    else
      z->rsub=x;
    return 0;
}

int BSTree::Transplant(BSNode *u, BSNode *v){
    if(u->p==nullptr)
      root=v;
    else if(u==u->p->lsub)
      u->p->lsub=v;
    else
      u->p->rsub=v;
    
    if(v!=nullptr)
      v->p=u->p;
    
    return 0;
}

int BSTree::Delete(BSNode *x){
    if(x->lsub==nullptr && x->rsub==nullptr)
      Transplant(x, static_cast<BSNode *>(nullptr));
    else if(x->lsub==nullptr && x->rsub!=nullptr)
      Transplant(x, x->rsub);
    else if(x->lsub!=nullptr && x->rsub==nullptr)
      Transplant(x, x->lsub);
    else{
        if(x->key%2){
            BSNode *y=Min(x->rsub);
            if(y->p!=x){
                Transplant(y, y->rsub);
                y->rsub=x->rsub;
                y->rsub->p=y;
            }
            Transplant(x, y);
            y->lsub=x->lsub;
            y->lsub->p=y;
        }
        else{
            BSNode *y=Max(x->lsub);
            if(y->p!=x){
                Transplant(y, y->lsub);
                y->lsub=x->lsub;
                y->lsub->p=y;
            }
            Transplant(x, y);
            y->rsub=x->rsub;
            y->rsub->p=y;
        }
    }

    return 0;
}

int BSTree::RotateL(BSNode *x){
    if(x->rsub==nullptr){
        cout << "ERR:RotateL(): right subtree of x is null !!!" << endl;
        return BSTROTATE_RSUB_NULL;
    }

    BSNode *y=x->rsub;
// 1.turn y's lsub into x's rsub 
    if(y->lsub!=nullptr)
      y->lsub->p=x;
    x->rsub=y->lsub;
// 2.link x's parent and y
    y->p=x->p;
    if(x->p==nullptr)
      root=y;
    else if(x==x->p->lsub)
      x->p->lsub=y;
    else
      x->p->rsub=y;
// 3.turn x into y's lsub
    x->p=y;
    y->lsub=x;

    return 0;
}

int BSTree::RotateR(BSNode *x){
    if(x->lsub==nullptr){
        cout << "ERR:RotateR(): left subtree of x is null !!!" << endl;
        return BSTROTATE_LSUB_NULL;
    }
    BSNode *y=x->lsub;
// 1.turn y's rsub into x's lsub
    if(y->rsub!=nullptr)
      y->rsub->p=x;
    x->lsub=y->rsub;
// 2.link x's parent and y
    y->p=x->p;
    if(x->p==nullptr)
      root=y;
    else if(x==x->p->lsub)
      x->p->lsub=y;
    else
      x->p->rsub=y;
// 3.turn x into y's rsub
    x->p=y;
    y->rsub=x;

    return 0;
}


int main(){

    BSTree btree;
    BSNode a(100),b(80),c(125),d(55),e(89),f(110),g(136),h(44),i(67),j(82),k(93),l(103),m(118),n(129),o(140);
    btree.Insert(&a); btree.Insert(&b); btree.Insert(&c); btree.Insert(&d); btree.Insert(&e); btree.Insert(&f); 
    btree.Insert(&g); btree.Insert(&h); btree.Insert(&i); btree.Insert(&j); btree.Insert(&k); btree.Insert(&l);
    btree.Insert(&m); btree.Insert(&n); btree.Insert(&o);

    cout << endl; 
    cout << "Insert sequence:\t\t\t" << "100-80-125-55-89-110-136-44-67-82-93-103-118-129-140" << endl << endl;
    cout << "Min:\t\t\t\t\t" << btree.Min(btree.root)->key << endl; 
    cout << "Max:\t\t\t\t\t" << btree.Max(btree.root)->key << endl;
    cout << "Successor of j(82):\t\t\t" << btree.Successor(&j)->key << endl;
    cout << "Predecessor of j(82):\t\t\t" << btree.Predecessor(&j)->key << endl;
    //return nullptr if not found
    cout << "Search g(136):\t\t\t\t" << "(ptr): " << btree.Search(btree.root, 136) << endl << endl;
    
    cout << "InorderWalk:\t\t\t\t"; btree.InorderWalk(btree.root); cout << endl;
    cout << "PreorderWalk:\t\t\t\t"; btree.PreorderWalk(btree.root); cout << endl;
    cout << "PostorderWalk:\t\t\t\t"; btree.PostorderWalk(btree.root); cout << endl;
    cout << "BreadthWalk:\t\t\t\t"; btree.BreadthWalk(btree.root); cout << endl << endl;

    btree.RotateL(&c);
    cout << "BreadthWalk after LeftRotate c(125):\t"; btree.BreadthWalk(btree.root); cout << endl;
    btree.RotateR(&b);
    cout << "BreadthWalk after RightRotate b(80):\t"; btree.BreadthWalk(btree.root); cout << endl << endl;

    btree.Delete(&b);
    cout << "BreadthWalk after delete b(80):\t\t"; btree.BreadthWalk(btree.root); cout << endl;
    btree.Delete(&c);
    cout << "BreadthWalk after delete c(125):\t"; btree.BreadthWalk(btree.root); cout << endl << endl;

    return 0;
}
