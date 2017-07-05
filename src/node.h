#ifndef NODE_H
#define NODE_H
//functions/classes for forming networks
//node represents a particular person (integer id)
#include <iostream>
#include <stdio.h>
#include <map>
#include "misc.h"
#include "date_object.h"
#include "purchase.h"
#include <math.h>
class node_t;

template <typename T>
class LinkedList;

template <typename T>
class LinkedListObject;

/*
  general linked list object, a shell structure around the data
 */
template <typename T>
class LinkedListObject
{
 public:

  LinkedListObject(T* data){
    this->m_data = data;
    this->m_next = NULL;
    this->m_last = NULL;
  }
  
  ~LinkedListObject(){
    //std::cout<<"linked_list.h: linked list object deletion"<<std::endl;
  }
  std::string __repr__(){
    std::string s = "LinkedListObject. Data: " + this->m_data->__repr__();
    return s;
  }
  void copy(const LinkedListObject<T>& other){
    this->m_data = other.m_data;
    this->m_next = other.m_next;
    this->m_last = other.m_last;
  }
  
  // copy constructor
  LinkedListObject(const LinkedListObject<T>& other){
    this->copy(other);
  }

  // assignment operator
  LinkedListObject<T>& operator= (const LinkedListObject<T>& source){
    if (this == &source)
      return *this;
    
    this->copy(source);
    
    return *this;
  }

  LinkedListObject<T>* m_next;
  LinkedListObject<T>* m_last;
  T* m_data;    
};

template <typename T>
class LinkedList
{
  typedef LinkedListObject<T> obj_td;
  
 public:
  LinkedListObject<T>* m_head;
  int m_size;
  void toArray(T*** arr, int* sz){
    *sz = this->m_size;
    (*arr) = new T*[*sz];
    obj_td* head = this->m_head;
    for(int i = 0; i < this->m_size; ++i){
      (*arr)[i] = head->m_data;
      head = head->m_next;
    }
  }
  int contains(T* obj){
    obj_td* head = this->m_head;
    for(int i = 0; i < this->m_size; ++i){
      if(obj == head->m_data){
	return 1;
      }
      head = head->m_next;
    }
    return 0;

  }
  LinkedList(){
    this->m_size = 0;
    this->m_head = NULL;
  }
  
  obj_td* getHead(){
    return this->m_head;
  }

  /* make a new shell object around the object and add this to the list */
  void addItem(T* object){
    obj_td* newobject = new obj_td (object);
    
    if(this->getSize()==0){
      this->m_head = newobject;
      this->m_head->m_next = NULL;
      this->m_head->m_last = NULL;
    } else {
      this->m_head->m_last = newobject;
      newobject->m_next    = this->m_head;
      this->m_head         = newobject;
    }
    
    this->m_size ++;
  }
  
  /* destructor, delete the linked list elements*/
  ~LinkedList(){
    obj_td* head = this->getHead();
    for(int i = 0; i < this->getSize(); ++i){
      obj_td* next = head->m_next;
      delete head;
      head = next;
    }
    this->m_size = 0;
    this->m_head = NULL;
  }
  
  void deleteFull(){
    std::cout<<"linked_list.h: deleteFull"<<std::endl;
    obj_td* head = this->getHead();
    for(int i = 0; i < this->getSize(); ++i){
      obj_td* next = head->m_next;
      delete head->m_data;
      head = next;
    }
    delete this;
  }
  
  /* the number of elements in the list */
  int getSize(){
    return this->m_size;
  }

  //dump the whole list to a string
  std::string printList(){
    std::string s;
    obj_td* head = this->getHead();
    for(int i = 0; i < this->getSize(); ++i){
      s += head->__repr__();
      s += "\n";
      head = head->m_next;
    }
    return s;
  }
  
  std::string __repr__(){
    std::cout<<"repr "<<" "<<this->getSize()<<std::endl;
    std::string s = "LinkedList, size:" + this->getSize();
    return s;
  }

  void deleteParticle(T* p){
    obj_td* obj = this->getHead();
    obj_td* prevobj;
    int found = 0;
    if(obj->m_data == p){
      this->m_head = obj->m_next;
      this->m_size --;
      delete obj;
      found = 1;
    } else {
      for(int i = 0; i < this->getSize() - 1; ++i){
	prevobj = obj;
	obj = obj->m_next;
	if(obj->m_data == p){
	  prevobj->m_next = obj->m_next;
	  delete obj;
	  this->m_size --;
	  found = 1;
	  break;
	}
      }
    }
    if (found == 0){
      printf("error in delete particle\n");
    }
  }
};

typedef std::map<int, LinkedList<node_t>*> node_map_td;
typedef std::map<int, node_t*> node_imap_td;



typedef LinkedListObject<node_t> llobj_td;
typedef LinkedList<node_t> llist_td;
typedef LinkedListObject<purchase_t> llo_purchase_td;
typedef LinkedList<purchase_t> ll_purchase_td;


//recursive function to get the nth degree connections off of node
//a map is used so that duplicate ids are not double counted
void get_nth_degree_network(node_t* node, 
			    int n, 
			    int recursion_depth, 
			    node_imap_td** nodemap);

//get the last n purchases within a network
//start_time is decremented by one month every iteration
//until the target number is reached
//or all network purchases are exhausted
void get_n_purchases(node_imap_td* nodemap, 
		     date_t start_time, 
		     int n, 
		     std::vector<purchase_t>* purchases, 
		     int* nout);

//mean and standard deviation from an array of purchases
void compute_purchase_stats(std::vector<purchase_t>& purchases, 
			    double* meanout, 
			    double* sdout);

class node_t
{
private:
  std::vector<purchase_t> m_purchases;
  int m_npurchases;
  
public:
  int m_id;
  int m_layer;
  

  //for reuse of a network 
  int m_current_network_order;
  node_imap_td* m_nodemap_network; //nth order network for reuse
  
  node_imap_td* m_inetwork;//1st order network with unique ids
  
  llist_td* m_network;
  

  void print_purchases();
  
  //record purchase to this node
  void add_purchase(purchase_t& p);
  
  //get the number of purchases after the probed time
  void get_n_purchases_after(date_t probetime,
			     int* exhausted_flag,
			     int* n);
  
  //mean and sd from last n_purchases in a network
  //nout is given as output in case less than n_purchases are found
  void get_purchase_mean_and_sd(int n_purchases, 
				date_t start_time, 
				double* mean, 
				double* sd, 
				int* nout);
  

  //generate network off of this node and store it in memory for
  //    later use off of streaming data
  void set_nth_order_network(int n){
    if(n == this->m_current_network_order){
      //pass
    } else {
      delete this->m_nodemap_network;
      get_nth_degree_network(this, n, 1, &this->m_nodemap_network);
      this->m_current_network_order = n;
    }
  }

  //get purchase objects after arg @probetime in a sorted map
  void get_purchases_after(date_t probetime, 
			   int exhausted_flag, 
			   std::map<date_t, std::vector<purchase_t> >* purchase_map);

  //
  std::string __repr__(){
    char buff[100];
    sprintf(buff, "node_t: %d %d", this->m_id, this->m_layer);
    std::string repr = buff;
    return repr;
  }

  //
  node_t(int id){
    m_id = id;
    m_layer = 1e9;
    m_network = new llist_td;
    m_inetwork = new node_imap_td;
    m_npurchases = 0;
    this->append(this);
    this->m_nodemap_network = new node_imap_td;
    this->m_current_network_order = 0;
  }

  void reset_layer(){
    m_layer = 1e9;
  }

  //remove two nodes' connections
  void decouple(node_t* n){
    if(m_inetwork->count(n->m_id)){
      m_network->deleteParticle(n);
      m_inetwork->erase(n->m_id);
    } 
    if(n->m_inetwork->count(this->m_id)){
      n->m_network->deleteParticle(this);
      n->m_inetwork->erase(this->m_id);
    }
  }
  
  //associate two nodes
  void append(node_t* n){
    //two way
    m_network->addItem(n);
    if(n != this){
      n->m_network->addItem(this);
    }
    
    (*m_inetwork)[n->m_id] = n;
    (*n->m_inetwork)[this->m_id] = this;
  }
  
  ~node_t(){
    delete m_network;
    delete m_inetwork;
  }

};






template class LinkedListObject<node_t>;
template class LinkedList<node_t>;


//for debugging
void print_network(node_imap_td* nodemap);


#endif
