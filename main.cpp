#include<bits/stdc++.h>
using namespace std;

ifstream in ("mergeheap.in");
ofstream out ("mergeheap.out");


/// Structura ce defineste nodul
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

/// Merge a 2 arbori binomiali
Nod* mergeBinomialTrees(Nod *b1, Nod *b2)
{
    // daca b1 > b2
    if (b1->valoare > b2->valoare)
        swap(b1, b2);

    // Facem ca arborele de valoare mai mare
    // sa fie fiul celui cu valoare mai mica

    b2->sibling = b1->child;
    b1->child = b2;
    b1->grad++;

    return b1;
}

/// Functie pentru reuniune a doua heap-uri binomiale
list<Nod*> unionBionomialHeap(list<Nod*> heap1, list<Nod*> heap2)
{
    // heap_nou va contine heap-ul dupa merge
    list<Nod*> heap_nou;
    list<Nod*>::iterator i1 = heap1.begin();
    list<Nod*>::iterator i2 = heap2.begin();
    while (i1!=heap1.end() && i2!=heap2.end())
    {
        // daca gradul lui heap1 <= gradul lui heap2
        if((*i1)->grad <= (*i2)->grad)
        {
            heap_nou.push_back(*i1);
            i1++;
        }
        // altfel (grad (heap1) > grad(heap2))
        else
        {
            heap_nou.push_back(*i2);
            i2++;
        }
    }

    // verificam daca mai sunt elemente in heap1
    while (i1 != heap1.end())
    {
        heap_nou.push_back(*i1);
        i1++;
    }

    // verificam daca mai sunt elemente in heap2
    while (i2!=heap2.end())
    {
        heap_nou.push_back(*i2);
        i2++;
    }
    return heap_nou;
}

// rearanjam heap-ul ca sa fie
// in ordine crescatoare de grad si
// sa nu existe doi arbori cu acelasi grad
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
        // daca decat un element trebuie procesat
        if (it2 == binomial_heap.end())
            it1++;

        // Daca grad(it1) < grad(it2) se trece mai departe
        else if ((*it1)->grad < (*it2)->grad)
        {
            it1++;
            it2++;
            if(it3!=binomial_heap.end())
                it3++;
        }

        // daca gradul a trei arbori consecutivi din heap
        // este acelasi se trece mai departe
        // (trebuie ca numai ultimii doi sa fie reuniti
        // daca dupa ei este un arbore de grad mai mare,
        // astfel se pastreaza ordinea crescatoare de grad)
        else if (it3!=binomial_heap.end() &&
                (*it1)->grad == (*it2)->grad &&
                (*it1)->grad == (*it3)->grad)
        {
            it1++;
            it2++;
            it3++;
        }

        // daca gradul a doi arbori din heap este egal
        // (am trecut de verificarea de 3 consecutivi deci
        // e clar ca dupa ei urmeaza un arbore de grad mai mare)
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

/// functie de insert arbore binomial in heap binomial
list<Nod*> insertArboreinHeap(list<Nod*> binomial_heap,Nod *tree)
{
    // heap nou ce va fi returnat
    list<Nod*> heap_nou;

    // inseram arborele in heap
    heap_nou.push_back(tree);

    // facem reuniune
    heap_nou = unionBionomialHeap(binomial_heap,heap_nou);

    // si adjust pentru a uni arborii de acelasi
    // grad daca este cazul
    return adjust(heap_nou);
}

/// functie de inserare de valoare in heap
// cea de mai inainte introduce un arbore
// iar aceasta transforma o valoare intr-un
// arbore de grad 0 si apeleaza functia de
// insert ca arbore
list<Nod*> insert(list<Nod*> binomial_heap, int valoare)
{
    Nod *temp = newNod(valoare);
    return insertArboreinHeap(binomial_heap,temp);
}


/// stergere minim din arbore binomial
list<Nod*> deleteMinimDinArbore(Nod *tree)
{
    list<Nod*> heap;
    Nod *temp = tree->child;
    Nod *a;

    // transformam arborele in heap
    while (temp)
    {
        a = temp;
        temp = temp->sibling;
        a->sibling = NULL;
        heap.push_front(a);
    }
    return heap;
}



/// functie pentru cautarea minimului in heap
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

/// stergere minim din heap
// folosesc functia de getMin pentru a gasi minimul
// si pe cea de deleteMinimDinArbore pentru a
// scoate minimul din arborele ce contine valoarea
// minima
list<Nod*> deleteMinim(list<Nod*> binomial_heap)
{
    list<Nod*> heap_nou,lo;
    Nod *temp;

    // temp va contine pointer la elementul cu valoarea minima
    // din heap
    temp = getMin(binomial_heap);
    list<Nod*>::iterator it;
    it = binomial_heap.begin();
    while (it != binomial_heap.end())
    {
        if (*it != temp)
        {
            // formam un alt heap din toti arborii
            // fara cel ce contine minimul
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
    cout << "\nElement minim din heap "
         << temp->valoare << "\n";
    cout<<endl;
    binomial_heap = deleteMinim(binomial_heap);
    printHeap(binomial_heap);


    return 0;
}

