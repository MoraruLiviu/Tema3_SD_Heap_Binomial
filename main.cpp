#include<bits/stdc++.h>
using namespace std;

ifstream in ("mergeheap.in");
ofstream out ("mergeheap.out");


/// Definition of the node
struct Nod
{
    int valoare, grad;
    Nod *child, *sibling;
};

Nod* newNod(int valoare)
{
    Nod *temp = new Nod;
    temp->valoare = valoare;
    temp->grad = 0;
    temp->child = temp->sibling = NULL;
    return temp;
}

/// Merge 2 binomial trees
Nod* mergeBinomialTrees(Nod *b1, Nod *b2)
{
    // if b1 > b2
    if (b1->valoare > b2->valoare)
        swap(b1, b2);

    // We make the higher value tree be the
    // son of the lower value one

    b2->sibling = b1->child;
    b1->child = b2;
    b1->grad++;

    return b1;
}

/// Function that unites 2 binomial heaps
list<Nod*> unionBionomialHeap(list<Nod*> heap1, list<Nod*> heap2)
{
    // heap_nou will contaain the new heap after merging
    list<Nod*> heap_nou;
    list<Nod*>::iterator i1 = heap1.begin();
    list<Nod*>::iterator i2 = heap2.begin();
    while (i1!=heap1.end() && i2!=heap2.end())
    {
        // if the degree of heap1 <= the degree of heap2
        if((*i1)->grad <= (*i2)->grad)
        {
            heap_nou.push_back(*i1);
            i1++;
        }
        // otherwise (degree (heap1) > degree (heap2))
        else
        {
            heap_nou.push_back(*i2);
            i2++;
        }
    }

    // we check if there are any more elements in heap1
    while (i1 != heap1.end())
    {
        heap_nou.push_back(*i1);
        i1++;
    }

    //  we check if there are any more elements in heap2
    while (i2!=heap2.end())
    {
        heap_nou.push_back(*i2);
        i2++;
    }
    return heap_nou;
}

// we rearrange the heaps so that
// they are ordered ascending by degree
// and that there are no two trees with the same degree
list<Nod*> adjust(list<Nod*> binomial_heap)
{
    if (binomial_heap.size() <= 1)
        return binomial_heap;
    list<Nod*> heap_nou;
    list<Nod*>::iterator it1,it2,it3;
    it1 = it2 = it3 = binomial_heap.begin();

    if (binomial_heap.size() == 2)
    {
        it2 = it1;
        it2++;
        it3 = binomial_heap.end();
    }
    else
    {
        it2++;
        it3=it2;
        it3++;
    }
    while (it1 != binomial_heap.end())
    {
        // if only 1 element needs to be processed
        if (it2 == binomial_heap.end())
            it1++;

        // If grad(it1) < grad(it2) we move on
        else if ((*it1)->grad < (*it2)->grad)
        {
            it1++;
            it2++;
            if(it3!=binomial_heap.end())
                it3++;
        }

        // if the degrees of 3 consecutive trees
        // are the same we move on
        // (only the last 2 trees of the same degree must be united
        // so that the ascending order based on the degree is kept)
        else if (it3!=binomial_heap.end() &&
                (*it1)->grad == (*it2)->grad &&
                (*it1)->grad == (*it3)->grad)
        {
            it1++;
            it2++;
            it3++;
        }

        // if the degree of 2 trees is the same
        else if ((*it1)->grad == (*it2)->grad)
        {
            *it1 = mergeBinomialTrees(*it1,*it2);
            it2 = binomial_heap.erase(it2);
            if(it3 != binomial_heap.end())
                it3++;
        }
    }
    return binomial_heap;
}

/// function that inserts a tree in the heap
list<Nod*> insertArboreinHeap(list<Nod*> binomial_heap,Nod *tree)
{
    // new heap
    list<Nod*> heap_nou;

    // we enter the tree in the new heap
    heap_nou.push_back(tree);

    // we unite the 2 heaps
    heap_nou = unionBionomialHeap(binomial_heap,heap_nou);

    // and merge if there's multiple trees with the same degree
    return adjust(heap_nou);
}

/// function for inserting a value in the heap
// it makes the value in a tree and adds the tree
// to the heap
list<Nod*> insert(list<Nod*> binomial_heap, int valoare)
{
    Nod *temp = newNod(valoare);
    return insertArboreinHeap(binomial_heap,temp);
}


/// delete the minimum of a tree
list<Nod*> deleteMinimDinArbore(Nod *tree)
{
    list<Nod*> heap;
    Nod *temp = tree->child;
    Nod *a;

    // turn the tree into a heapp
    while (temp)
    {
        a = temp;
        temp = temp->sibling;
        a->sibling = NULL;
        heap.push_front(a);
    }
    return heap;
}



/// function to find the minimum from a heap
Nod* getMin(list<Nod*> binomial_heap)
{
    list<Nod*>::iterator it = binomial_heap.begin();
    Nod *temp = *it;
    while (it != binomial_heap.end())
    {
        if ((*it)->valoare < temp->valoare)
            temp = *it;
        it++;
    }
    return temp;
}

/// delete the minimum from the heap
list<Nod*> deleteMinim(list<Nod*> binomial_heap)
{
    list<Nod*> heap_nou,lo;
    Nod *temp;

    // temp will point to the minimum element in the heap
    temp = getMin(binomial_heap);
    list<Nod*>::iterator it;
    it = binomial_heap.begin();
    while (it != binomial_heap.end())
    {
        if (*it != temp)
        {
            // we make a new heap without the tree
            // that has the minimum
            heap_nou.push_back(*it);
        }
        it++;
    }
    lo = deleteMinimDinArbore(temp);
    heap_nou = unionBionomialHeap(heap_nou,lo);
    heap_nou = adjust(heap_nou);
    return heap_nou;
}

list<Nod*> Build (list<Nod*> binomial_heap, long long N)
{
    long a;
    for (long long i=0; i<N;i++)
    {
        in>>a;
        binomial_heap = insert(binomial_heap,a);
    }
    return binomial_heap;
}

void printArbore(Nod *a)
{
    while (a)
    {
        out << a->valoare <<endl;
        printArbore(a->child);
        a = a->sibling;
    }
}


void printHeap(list<Nod*> binomial_heap)
{
    list<Nod*> ::iterator it;
    it = binomial_heap.begin();
    while (it != binomial_heap.end())
    {
        printArbore(*it);
        it++;
    }
}

int main()
{
    long long N;
    in>>N;
    list<Nod*> binomial_heap;

    binomial_heap = Build(binomial_heap ,N);
    Nod *temp = getMin(binomial_heap);
    cout << "\nThe minimum element in the heap "
         << temp->valoare << "\n";
    cout<<endl;
    binomial_heap = deleteMinim(binomial_heap);
    printHeap(binomial_heap);


    return 0;
}

