#include "kvstore.h"
#include <string>
#include <ctime>
using namespace std;

void KVStore::skipList::addLevel(){
    level* tmp=new level(top);
    if (top){
        top->above=tmp;
    }
    top=tmp;
}

void KVStore::skipList::reset(){
    level* tmp=top->below;
    while (tmp){
        delete top;
        top=tmp;
        tmp=top->below;
    }
    delete top;
    top=nullptr;
    addLevel();
}

KVStore::skipList::~skipList(){
    level* tmp=top->below;
    while (tmp){
        delete top;
        top=tmp;
        tmp=top->below;
    }
    delete top;
}

KVStore::skipList::skipList(){
    _size=0;
    addLevel();
}

uint64_t KVStore::skipList::size() {
    return _size;
}

KVStore::node* KVStore::skipList::search(uint64_t key) {
    level* height=top;
    node* l=height->head;
    node* r=height->tail;
    node* tmp;
    while (height){
        tmp=l->next;
        while (tmp!=r&&tmp->key<=key){
            if (tmp->key==key){
                return tmp;
            }
            l=tmp;
            tmp=tmp->next;
        }
        r=tmp;
        if (height->below){
            l=l->below;r=r->below;
        }
        height=height->below;
    }
    return nullptr;
}

string KVStore::skipList::get(uint64_t key) {
    node* tmp=search(key);
    if (!tmp)
        return "";
    while (tmp->below)
        tmp=tmp->below;
    return tmp->value;
}

bool  KVStore::skipList::del(uint64_t key) {
    node* tmp=search(key),*p;
    if (!tmp)
        return false;
    while (tmp){
        tmp->prev->next=tmp->next;
        tmp->next->prev=tmp->prev;
        p=tmp;
        tmp=tmp->below;
        delete p;
    }
    return true;
}

void  KVStore::skipList::put(uint64_t key, const string &s) {
    level* height=top;
    node* l=height->head;
    node* r=height->tail;
    node* tmp;
    while (height->below){
        tmp=l->next;
        while (tmp!=r&&tmp->key<=key){
            if (tmp->key==key){
                tmp->value=s;
            }
            l=tmp;
            tmp=tmp->next;
        }
        r=tmp;
        if (height->below){
            l=l->below;r=r->below;
        }
        height=height->below;
    }
    tmp=l->next;
    while (tmp!=r&&tmp->key<=key){
        if (tmp->key==key){
            tmp->value=s;
        }
        l=tmp;
        tmp=tmp->next;
    }
    l->next=new node(l,tmp,nullptr);
    ++_size;
    tmp->prev=l->next;
    tmp=tmp->prev;
    tmp->key=key;
    tmp->value=s;
    srand((int)time(0));
    node* p,*q;
    while (rand()&1){
        if (!height->above)
            addLevel();
        height=height->above;
        p=height->head;
        while (p->next!=height->tail&&p->next->key<key)
            p=p->next;
        q=new node(p,p->next,tmp);
        tmp=p;
        p->next->prev=tmp;
        p->next=tmp;
    }
}



KVStore::KVStore(const std::string &dir): KVStoreAPI(dir)
{
}

KVStore::~KVStore()
{
}

/**
 * Insert/Update the key-value pair.
 * No return values for simplicity.
 */
void KVStore::put(uint64_t key, const std::string &s)
{
    memTable.put(key,s);
}
/**
 * Returns the (string) value of the given key.
 * An empty string indicates not found.
 */
std::string KVStore::get(uint64_t key)
{
	return memTable.get(key);
}
/**
 * Delete the given key-value pair if it exists.
 * Returns false iff the key is not found.
 */
bool KVStore::del(uint64_t key)
{
	return memTable.del(key);
}

/**
 * This resets the kvstore. All key-value pairs should be removed,
 * including memtable and all sstables files.
 */
void KVStore::reset()
{
    memTable.reset();
}
