#include "node.h"

void node_t::get_purchase_mean_and_sd(int n_purchases, 
				      date_t start_time,
				      double* mean, 
				      double* sd, 
				      int* nout){
  //@start_time: initial guess for how far back to go
  //@n_purchases: target number of purchases to achieve
  //requires nth order network to be already set
  
  std::vector<purchase_t> local_purchases;

  //get the last n purchases in this nodes' currently set 
  //nth order network
  get_n_purchases(this->m_nodemap_network, 
		  start_time, 
		  n_purchases,
		  &local_purchases,
		  nout);
  
  compute_purchase_stats(local_purchases, mean, sd);
    
}

void node_t::get_purchases_after(date_t probetime, 
				 int exhausted_flag, 
				 std::map<date_t, std::vector<purchase_t> >* purchase_map){
  //assumes m_purchases is sorted (done in add_purchase)
  //simply add all purchases with no checks if exhausted_flag
  if(exhausted_flag == 1){
    std::vector<purchase_t>::reverse_iterator it;
    for (it = m_purchases.rbegin(); 
	 it != m_purchases.rend(); 
	 ++it ) { 
      purchase_t* p = &(*it);
      (*purchase_map)[p->m_date_of_purchase].push_back(*p);
    }
    return ;
  } 
    
  //else
  //check each purchase for probetime
  std::vector<purchase_t>::reverse_iterator it;
  int count = 0;
  for (it = m_purchases.rbegin(); 
       it != m_purchases.rend(); 
       ++it ) { 
    purchase_t* p = &(*it);
    if(p->m_date_of_purchase >= probetime){
      //add to list
      (*purchase_map)[p->m_date_of_purchase].push_back(*p);
      count ++;
    } else {
      //done
      return;
    }
  }
    
}

void node_t::get_n_purchases_after(date_t probetime, int* exhausted_flag, int* n){
  //number of purchases after probed time
  //@exhausted_flag : 1 if there are no remaining purchases
  //@n: number of purchases found
  //assumes m_purchases is sorted (done in add_purchase)

    
  std::vector<purchase_t>::reverse_iterator it;
  int count = 0;
  for (it = m_purchases.rbegin(); 
       it != m_purchases.rend(); 
       ++it ) { 
    purchase_t* p = &(*it);
    if(p->m_date_of_purchase >= probetime){
      //add to list
      count ++;
    } else {
      break;
    }
  }
  
  *n = count;
  
  if(count == m_npurchases){
    *exhausted_flag = 1;
  } else {
    *exhausted_flag = 0;
  }
    
}

//add purchase to id's list
//make sure this list is in proper time order 
//(later elements are more recent)
void node_t::add_purchase(purchase_t& newp){
  //most of the time just add to end
  
  newp.set_id(this->m_id);
  
  if(m_npurchases == 0){
    m_purchases.push_back(newp);
    m_npurchases += 1;
    return;
  }
  
  std::vector<purchase_t>::reverse_iterator it;
  for (it = m_purchases.rbegin(); 
       it != m_purchases.rend(); 
       ++it ) { 
    purchase_t* oldp = &(*it);//.base();
    if(newp < *oldp){
      //if newp is older than oldp due to time asynchronization in the stream input
      continue;
    } else {
      //newp is newer than oldp -- insert it
      m_purchases.insert(it.base(), newp);
      m_npurchases += 1;
      return;
    }
  }
  
  m_purchases.insert(m_purchases.begin(), newp);
  m_npurchases+=1;
  
}

void node_t::print_purchases(){
    std::vector<purchase_t>::iterator it = m_purchases.begin();
    for(int i = 0; i < m_npurchases; ++i, ++it){
      std::cout<<(*it)<<std::endl;
    }
  }


void copy_list_to_map(llist_td* list_to_copy_from, 
		      node_imap_td* to_copy_to,
		      int layer){
  //insert [int:node*] pair. 
  //duplicate entries will not change anything
  //
  llobj_td* values = list_to_copy_from->getHead();
  int nvalues = list_to_copy_from->getSize();
  for(int j = 0; j < nvalues; ++j){
    node_t* node = values->m_data;

    node->m_layer = minfunc(node->m_layer, layer);

    (*to_copy_to)[node->m_id] = node;
    values = values->m_next;
  }
}

void compute_purchase_stats(std::vector<purchase_t>& purchases, double* meanout, double* sdout){
  std::vector<purchase_t>::iterator it;
  double sum = 0;
  
  int nvals = purchases.size();
  for(it = purchases.begin(); it != purchases.end(); ++it){
#ifdef DEBUG
    printf("node.cc: compute_purchase: %s \n", (*it).__repr__().c_str());
#endif
    sum += (*it).get_purchase_value();
  }
  
  double mean = sum / nvals;
  
  //sd from insight equation FAQ 
  double sd = 0;
  double tmp;
  for(it = purchases.begin(); it != purchases.end(); ++it){
    tmp = ((*it).get_purchase_value() - mean);
    sd += (tmp * tmp);
  }
  sd = sd / nvals;
  sd = sqrt(sd);

  *meanout = mean;
  *sdout = sd;
  
}

//recursive on network, then network of network for n = 2, etc. 
void get_nth_degree_network(node_t* node, int n, int recursion_depth, node_imap_td** nodemap){
  if (recursion_depth == 1){
    (*nodemap) = new node_imap_td;
  }
			    
  if ( n == 1 ) {
    //add nodes network to nodemap{}
    copy_list_to_map(node->m_network, *nodemap, recursion_depth);
  } else {
    //iterate over the 1st order network, node->network
    //process get neighbors of neighbors on next iteration
    llist_td* network = node->m_network;
    llobj_td* head = network->getHead();
    int nnetwork = network->getSize();
    for(int j = 0; j < nnetwork; ++j){
      get_nth_degree_network(head->m_data, 
			     n - 1, 
			     recursion_depth + 1, 
			     nodemap);
      head = head->m_next;
    }
    
  }
  
}


void get_n_purchases(node_imap_td* nodemap, 
		     date_t start_time, 
		     int n, 
		     std::vector<purchase_t>* allpurchases, 
		     int* nout){
  //get the most recent n purchases in the network
  //iterative process where the time is decremented further
  //and further back until n purchases are found or the list is exhausted
  
  int total_n = 0;
  int exhausted_count = 0;
  int local_n;
  int exhausted_flag;

  //FIXME
  //certain data may need larger or smaller decrements in time
  //date_t one_day = date_t::one_day();
  //date_t one_year = date_t::one_year();
  date_t one_month = date_t::one_month();
  date_t probe_time = start_time;
  date_t one_inc = one_month;


  int iter = 0;
  std::vector<int> exhausted_record(nodemap->size());
  int kk = 0;
  node_imap_td::iterator it;
  while( exhausted_count != (int)nodemap->size() and total_n < n){
    probe_time = probe_time - one_inc;
    total_n = 0;
    exhausted_count = 0;
    kk = 0;
    for(it = nodemap->begin(); it != nodemap->end(); it++, kk++){
      node_t* node = it->second;
      node->get_n_purchases_after(probe_time, &exhausted_flag, &local_n);
      exhausted_count += exhausted_flag;
      exhausted_record[kk] = exhausted_flag;
      total_n += local_n;
    }

#ifdef DEBUG
    printf("iter probe time: %d %s %s %d %d %ld\n", total_n, start_time.__repr__().c_str(), probe_time.__repr__().c_str(), iter, exhausted_count, nodemap->size());
#endif
    
    if(probe_time.m_year < 0){
      printf("void get_n_purchases:: an error has occurred\n");
      throw 20;
    }
    iter ++;
  }

  //get purchases until probe_time
  //std::map is already sorted
  std::map<date_t, std::vector<purchase_t> > purchase_map;
  kk = 0;

  for(it = nodemap->begin(); it != nodemap->end(); it++, kk++){
    node_t* node = it->second;
    node->get_purchases_after(probe_time, 
			      exhausted_record[kk], 
			      &purchase_map);
    
    exhausted_count += exhausted_flag;
  }
  
  //now take in the first n purchases from the sorted map
  allpurchases->clear();
  std::map<date_t, std::vector<purchase_t> >::reverse_iterator rit;
  int ct = 0;
  for(rit = purchase_map.rbegin(); 
      rit != purchase_map.rend(); 
      ++rit){
    std::vector<purchase_t>* purchases = &(rit->second);
    std::vector<purchase_t>::iterator vecit;
    for(vecit = purchases->begin(); vecit != purchases->end(); ++vecit){
      allpurchases->push_back( *vecit );
      //printf("node.cc purchase %s\n", (*vecit).__repr__().c_str());
      ct ++;
      if(ct == n){
	*nout = ct;
	return;
      }
    }
  }
  *nout = ct;
  
}




void print_network(node_imap_td* nodemap){
  node_imap_td::iterator it;
  for(it = nodemap->begin(); it != nodemap->end(); it++){
    printf("first: %d ", it->first);
    printf("second: %s \n", it->second->__repr__().c_str());
  }
  
}

