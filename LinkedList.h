#ifndef LDL_h
#define LDL_h
#include "Structs.h"
template <class T>
struct Nodo {
    T* dato;
    Nodo<T>* sig = NULL;
    Nodo<T>* prev = NULL;

    T* getDato() {
        return dato;
    }
};
template <class T>
class LinkedList { // Lista Ligada
private:
    Nodo<T>* head, * tail;
    int size;
public:
    LinkedList() {
        head = NULL;
        tail = NULL;
        size = 0;
    }
    Nodo<T>* getHead() {
        return head;
    }
    Nodo<T>* getTail() {
        return tail;
    }
    int getSize() {
        return size;
    }
    T* get(int pos) {
        Nodo<T>* aux = head;
        if (aux == NULL) { // si esta vacia
            return NULL;
        }
        else if (pos > size) { // si esta fuera de rango
            return NULL;
        }
        else if (pos == size) { // si es el ultimo nodo
            return tail->getDato();
        }
        else if (pos == 0) { // si es el primer nodo
            return aux->getDato();
        }
        else {
            for (int i = 0; i < pos; i++) {
                aux = aux->sig; // busca hasta encontrar el nodo
            }
            return aux->getDato();
        }
    }
    void set(int pos, T* newDato) {
        Nodo<T>* aux = head;
        if (aux != NULL && pos <= size) {
            if (pos == 0) {
                aux->dato = newDato;
            }
            else {
                for (int i = 0; i < pos; i++) {
                    if (aux->sig != NULL) aux = aux->sig; // busca hasta encontrar el nodo
                }
                aux->dato = newDato;
            }
        }
    }
    void add(T* dato) {
        Nodo<T>* aux = new Nodo<T>;
        aux->dato = dato;
        aux->sig = NULL;

        if (head == NULL) { // si la lista esta vacia
            head = aux;
            tail = aux;
            aux->prev = NULL;
        }
        else {
            aux->prev = tail; // asigna como prev al tail antiguo
            tail->sig = aux; // el tail antiguo asigna como sig al nuevo nodo
            tail = aux; // el nuevo nodo se vuelve el nuevo tail
            aux->sig = NULL; // se define como NULL el sig del nuevo tail
        }
        size++;
    }
    void swap(T* dato1, T* dato2, int pos1, int pos2) {
        T* datoAux;
        datoAux = dato1;
        this->set(pos1, dato2);
        this->set(pos2, datoAux);
    }
    void del(int pos) {
        Nodo<T>* aux = head;
        if (aux != NULL) { // si la lista esta vacia, no hay nada que eliminar
            if (pos < size) { // si el pos esta fuera de rango, no hay nada que eliminar
                if (pos == 0) { // si la pos es 0, se elimina el head
                    head = aux->sig;
                    aux->prev = NULL;
                    delete aux;
                    size--;
                }
                else if (pos == (size - 1)) { // si la pos es el tamaño, se elimina el tail
                    for (int i = 0; i < (pos - 1); i++) { // me detengo uno antes del ultimo
                        aux = aux->sig;
                    }
                    tail = aux;
                    delete aux->sig;
                    aux->sig = NULL;
                    size--;
                }
                else { // en cualquier otro escenario, se busca el nodo en la pos y se elimina normal
                    for (int i = 0; i < pos; i++) {
                        aux = aux->sig;
                    }
                    aux->prev->sig = aux->sig;
                    aux->sig->prev = aux->prev;
                    delete aux;
                    size--;
                }
            }
        }
    }
    void deleteList() {
        Nodo<T>* aux = head;
        while (aux != NULL) {
            T* temp = aux->getDato();
            aux = aux->sig;
            delete temp;
        }
        head = tail = NULL;
        size = 0;
    }
    // funciones exclusivas a los carnets
    Nodo<T>* getMiddle(Nodo<T>* start, Nodo<T>* last) { // se utiliza en la busqueda binaria
        if (start == NULL) return NULL; // la lista esta vacia
        Nodo<T>* slow = start;
        if (start->sig == NULL) return slow; // si la lista solo tiene un elemento
        Nodo<T>* fast = start->sig;
        while (fast != last) // recorremos la lista hasta llegar
        {                       // al nodo que este en medio
            fast = fast->sig;      // tomando en consideracion
            if (fast != last)       // como punto de salida
            {                       // el nodo de parametro
                slow = slow->sig;
                fast = fast->sig;
            }
        }
        return slow;
    }
    T* binarySearch(int dato) { // funciona como un get usando el numero de carnet
        Nodo<Carne>* start = this->head; // primero va a buscar el valor de enmedio entre el head y el tail (comienzo y fin)
        Nodo<Carne>* last = this->tail; // luego, seguira partiendo en dos hasta encontrar el valor
        do {
            Nodo<Carne>* mid = getMiddle(start, last);
            
            if (mid == NULL) return NULL; // Si la lista esta vacia
            
            if (mid->dato->carnetID == dato) { // Si el dato a buscar se encuentra en el nodo de enmedio
                return mid->dato;
            }
            else if (mid->dato->carnetID < dato) { // Si el dato es mayor, busca del lado derecho (desde middle hasta tail)
                start = mid->sig;
            }
            else { // Si es menor, busca del lado izquierdo (desde head hasta middle)
                last = mid;
            }
        } while (last == NULL || last != start); // sigue buscando en la lista hasta encontrar el valor o terminar el recorrido
        if (start->dato->carnetID == dato) return start->dato; // una ultima comparacion por si el dato se encuentra en el tail
        return NULL; // cuando haya terminado el recorrido y no haya encontrado el resultado
    }
    void assortedInsert(Carne* dato) { // para acomodar un carnet segun su ID de forma ascendente
        Nodo<Carne>* aux = new Nodo<Carne>;
        aux->dato = dato;
        Nodo<Carne>* first = head;
        if (first == NULL) { // para cuando la lista es vacia
            aux->prev = NULL;
            aux->sig = first;
            head = aux; // intercambian lugar
            tail = aux;
        }
        else if (first->dato->carnetID >= aux->dato->carnetID) { // el nodo nuevo se convierte en el head
            aux->prev = NULL;
            aux->sig = first;
            head = aux;
        }
        else { // busca el nodo que estara antes del nuevo, osea el lugar en donde vamos a insertarlo
            Nodo<Carne>* temp = head;
            while (temp->sig != NULL && temp->sig->dato->carnetID < aux->dato->carnetID) {
                temp = temp->sig; // busca hasta que llegue al final de la lista o el valor siguiente sea mayor
            }                       // ahora inserta el nuevo nodo en los espacios indicados:
            aux->sig = temp->sig;
            if (aux->sig != NULL) {
                aux->sig->prev = aux; // antes del siguiente de temp (el nodo que estaba originalmente)
            }
            else {
                tail = aux; // si se agrega hasta el final, se reasigna el tail
            }
            aux->prev = temp;
            temp->sig = aux; // y despues de temp
        }
        size++; // se suma al size para grabar que incremento la lista
    }
};
#endif // !LDL_h