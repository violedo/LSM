#pragma once

#include "kvstore_api.h"
using namespace std;

class KVStore : public KVStoreAPI {
	// You can add your implementation here
private:

    struct node{
        uint64_t key=0;
        string value="";
        node* prev,*next,*below;
        node(node* prev= nullptr,node* next= nullptr,node* below= nullptr){
            this->prev=prev;
            this->next=next;
            this->below=below;
        }
    };
    struct level{
        level *below,*above= nullptr;
        node* head,*tail;

        level(level* below= nullptr){
            this->below=below;
            head=new node(nullptr,tail, nullptr);
            tail=new node(head, nullptr,nullptr);
            head->next=tail;
            if (below){
                head->below=below->head;
                tail->below=below->tail;
            }
        }
        ~level(){
            node* p,*q;
            p=head;
            while (p!=tail){
                q=p->next;
                delete p;
                p=q;
            }
            delete p;
        }
    };

    class skipList{
    private:
        uint64_t _size;
        level* top= nullptr;
        node* search(uint64_t key);
        void addLevel();

    public:
        skipList();
        ~skipList();
        uint64_t size();
        void put(uint64_t key,const string &s);
        string get(uint64_t key);
        bool del(uint64_t key);
        void reset();
    } memTable;



public:
	KVStore(const std::string &dir);

	~KVStore();

	void put(uint64_t key, const std::string &s) override;

	std::string get(uint64_t key) override;

	bool del(uint64_t key) override;

	void reset() override;

};
