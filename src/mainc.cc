#include "node.h"
#include <fstream>
#include "purchase.h"

class sequential_operation_t
{
public:
  date_t m_time;
  sequential_operation_t(date_t t){
      m_time = t;
    }
  //  virtual ~sequential_operation_t(){
  //  }
  virtual void call()=0;
};

class befriend_t : public sequential_operation_t
{
public:
  node_t* m_n1;
  node_t* m_n2;
  befriend_t(date_t t, node_t* n1, node_t* n2):sequential_operation_t(t){
    m_n1 = n1;
    m_n2 = n2;
  }
  //  virtual ~befriend_t(){
  //  }
  virtual void call(){
    m_n1->append(m_n2);
  }
};

class unfriend_t: public sequential_operation_t
{
public:
  node_t* m_n1;
  node_t* m_n2;
  unfriend_t(date_t t, node_t* n1, node_t* n2): sequential_operation_t(t){
    m_n1 = n1;
    m_n2 = n2;
  }
  virtual void call(){
    m_n1->decouple(m_n2);
  }

};

typedef std::map<date_t, std::vector<sequential_operation_t*> > operation_map_td;



void build_friends(std::string friendfile, 
		   std::vector<node_t*>* allpeople, 
		   std::map<int, node_t*>* unique_people, 
		   operation_map_td* operation_map){

  std::ifstream myfile;
  myfile.open(friendfile.c_str());
  int linect = 0;
  int hour, minute, second, day, month, year;
  int id1, id2;
  
  while(myfile){
    std::string strInput;
    std::getline(myfile,strInput);
    sscanf(strInput.c_str(), "%d %d %d %d %d %d %d %d", &hour, &minute, &second, &day, &month, &year, &id1, &id2);
    
    date_t t(hour, minute, second, day, month, year);
    node_t* node_id1;
    node_t* node_id2;
    //make new nodes if not in global data
    if(unique_people->count(id1) == 0){
      //make new node
      node_id1 = new node_t(id1);
      (*unique_people)[id1] = node_id1;
      allpeople->push_back(node_id1);
    } else {
      node_id1 = unique_people->find(id1)->second;
    }
    
    if(unique_people->count(id2) == 0){
      //make new node
      node_id2 = new node_t(id2);
      (*unique_people)[id2] = node_id2;
      allpeople->push_back(node_id2);
    } else {
      node_id2 = unique_people->find(id2)->second;
    }
    

    //node_id1->append(node_id2);//2 way connection


    befriend_t* operation = new befriend_t(t, node_id1, node_id2);    
    (*operation_map)[t].push_back(operation);
    
    linect ++;
  }
  myfile.close();
  //printf("nlines %d %lud\n", linect, unique_people->size());
}


void subtract_unfriends(std::string unfriendfile, 
			std::vector<node_t*>* allpeople, 
			std::map<int, node_t*>* unique_people, 
			operation_map_td* operation_map){
  std::vector<std::string> retval;
  std::ifstream myfile;
  myfile.open(unfriendfile.c_str());
  int linect = 0;
  int hour, minute, second, day, month, year;
  int id1, id2;
  
  while(myfile){
    std::string strInput;
    std::getline(myfile,strInput);
    if(strInput.length() < 5){
      continue;
    }

    sscanf(strInput.c_str(), "%d %d %d %d %d %d %d %d", &hour, &minute, &second, &day, &month, &year, &id1, &id2);
    date_t t(hour, minute, second, day, month, year);
  
  
    node_t* node_id1;
    node_t* node_id2;
    //make new nodes if not in global data
    if(unique_people->count(id1) == 0){
      //make new node
      node_id1 = new node_t(id1);
      (*unique_people)[id1] = node_id1;
      allpeople->push_back(node_id1);
    } else {
      node_id1 = unique_people->find(id1)->second;
    }
    
    if(unique_people->count(id2) == 0){
      //make new node
      node_id2 = new node_t(id2);
      (*unique_people)[id2] = node_id2;
      allpeople->push_back(node_id2);
    } else {
      node_id2 = unique_people->find(id2)->second;
    }
    //node_id1->decouple(node_id2);
    unfriend_t* operation = new unfriend_t(t, node_id1, node_id2);    
    (*operation_map)[t].push_back(operation);

    //node_id1->append(node_id2);//2 way connection
    linect ++;
  }
  myfile.close();
  //printf("nlines %d %lud\n", linect, unique_people->size());
}


void read_params(std::string filename, int* network_dimension, int* n_purchases){
  std::ifstream myfile;
  myfile.open(filename.c_str());
  while(myfile){
    std::string strInput;
    std::getline(myfile,strInput);
    sscanf(strInput.c_str(), "%d %d", network_dimension, n_purchases);
    break;
  }
  myfile.close();
}

void read_paramsfile(std::string filename, int* network_dimension, int* n_purchases){
  std::string params  = filename + ".params";
  read_params(params, network_dimension, n_purchases);
}
void build_network(std::string filename, std::vector<node_t*>* allpeople, std::map<int, node_t*>* unique_people, int network_dimension){

  printf("building social network of degree %d...\n", network_dimension);
  std::string befriends = filename + ".befriends";
  std::string unfriends = filename + ".unfriends";

  //set up the operation to execute which are time sorted 
  //by virtue of std::map on date_t
  //set up friends/unfriend operations  
  
  operation_map_td operation;
  build_friends(befriends, allpeople, unique_people, &operation);
  subtract_unfriends(unfriends, allpeople, unique_people, &operation);
  
  //process in map order which is time sequential
  //so this will be an accurate most recent network
  //this is in case someone unfriends/friends frequently
  //like my conservative father in law to my liberal
  //sister(s) in law after the election
  operation_map_td::iterator it;
  for(it = operation.begin(); 
      it != operation.end(); 
      ++it){
    std::vector<sequential_operation_t*>* ops = &(it->second);
    std::vector<sequential_operation_t*>::iterator subit;
    for(subit = ops->begin(); subit != ops->end(); ++subit){
      (*subit)->call();
    }
  }

  //now set up nth order networks and store
  for(  std::vector<node_t*>::iterator node_it = allpeople->begin(); 
	node_it != allpeople->end(); 
	++node_it){
    node_t* node = *node_it;
    node->set_nth_order_network(network_dimension);
  }
  printf("done with social network\n");
}

void build_purchases(std::string filename, 
		     std::vector<node_t*>* allpeople, 
		     std::map<int, node_t*>* unique_people, 
		     std::vector<purchase_t>* purchase_list, 
		     int add_to_network){
  printf("building purchases...\n");
  std::string purchases = filename + ".purchases";
  std::ifstream myfile;
  myfile.open(purchases.c_str());

  int hour, minute, second, day, month, year;
  int id1;
  double amount;
  
  while(myfile){
    std::string strInput;
    std::getline(myfile,strInput);
    if(strInput.length() < 5){
      continue;
    }

    sscanf(strInput.c_str(), "%d %d %d %d %d %d %d %lf", &hour, &minute, &second, &day, &month, &year, &id1, &amount);
    
    node_t* node_id1;
    
    //make new nodes if not in global data
    if(unique_people->count(id1) == 0){
      //make new node
      node_id1 = new node_t(id1);
      (*unique_people)[id1] = node_id1;
      allpeople->push_back(node_id1);
    } else {
      node_id1 = unique_people->find(id1)->second;
    }
    
    purchase_t p(hour, minute, second, day, month, year, amount, node_id1);
    //p->m_node = node_id1;
    purchase_list->push_back(p);
    if(add_to_network){
      node_id1->add_purchase(p);
    }
    
  }
  myfile.close();
  printf("done with purchases\n");
}

void flag_purchases(std::string flagged_filename, 
		    std::vector<node_t*>& allpeople, 
		    std::vector<purchase_t>& purchase_list,
		    int n_purchases,
		    date_t start_time){
  printf("flagging purchases...\n");
  int nout;
  double mean;
  double sd;
  std::vector<purchase_t> flagged;
  std::vector<std::vector<double> > flagged_mean_sd;
  
  for(  std::vector<purchase_t>::iterator pit = purchase_list.begin();
	pit != purchase_list.end();
	++pit){
    
    node_t* node = (*pit).m_node;
    double amount = (*pit).m_amount;
    
    //compute mean sd
    node->get_purchase_mean_and_sd(n_purchases, 
				   start_time, 
				   &mean, 
				   &sd, 
				   &nout);
    #ifdef DEBUG
    printf("mean: %15.5e %15.5e\n", mean, sd);
    #endif
    if(nout < 2){
      //don't flag anything
      continue;
    } else {

      if(amount > mean + 3 * sd){
	//flag
	flagged.push_back(*pit);
	std::vector<double> a(2);
	a[0] = mean;
	a[1] = sd;
	flagged_mean_sd.push_back(a);
      }
    }
  }




  //record flagged purchases
  FILE* pf = fopen(flagged_filename.c_str(), "w");
  std::vector<std::vector<double> >::iterator dit = flagged_mean_sd.begin();
  for(std::vector<purchase_t>::iterator pit = flagged.begin();
      pit != flagged.end();
      ++pit, ++dit){
    purchase_t* p = &(*pit);
    //printf("purchase %s\n", p->__repr__().c_str());
    std::vector<double> mean_sd = (*dit);
    fprintf(pf, "%s %d %f %f %f\n", p->m_date_of_purchase.__repr__().c_str(), p->m_node->m_id, (mean_sd)[0], (mean_sd)[1], p->m_amount);
  }
  fclose(pf);
  printf("wrote %s\n", flagged_filename.c_str());
}



int main(int argc, char* argv[]){
  //read in stream_log.json

  std::vector<node_t*> allpeople;
  int network_dimension;
  int n_purchases;
  

  //initial set up from batch file
  std::string inputfile_name_base = "batch_log";
  std::map<int, node_t*> unique_people;
  
  read_paramsfile(inputfile_name_base, &network_dimension, &n_purchases);
  
  //set up the nth order networks for all people
  build_network(inputfile_name_base, 
		&allpeople, 
		&unique_people, 
		network_dimension);
  


  //add purchase history
  //include network as argument because we
  //might add to network:
  //will add more nodes here if someone has no friends
  std::vector<purchase_t> old_purchases;
  build_purchases(inputfile_name_base, 
		  &allpeople, 
		  &unique_people, 
		  &old_purchases, 
		  1);
  

  //////////////////////////////////////////////////////////
  //now process new streaming data
  std::string stream_file_base = "stream_log";
  build_network(stream_file_base, 
		&allpeople, 
		&unique_people, 
		network_dimension);
  

  //the network is fixed here 
  //we compare new purchases against the past purchases
  //and update the network after flagging
  //@last argument: don't add to network, just read in purchase data
  std::vector<purchase_t> recent_purchases;
  build_purchases(stream_file_base, 
		  &allpeople, 
		  &unique_people, 
		  &recent_purchases, 
		  0);

  //now flag recent purchases
  std::string flagged_file_base = "flagged_purchases.flagged";
  
  date_t start_time;
  if(old_purchases.size() > 0){
    start_time = old_purchases[old_purchases.size() - 1].m_date_of_purchase;
  }
  
  flag_purchases(flagged_file_base, 
		 allpeople, 
		 recent_purchases, 
		 n_purchases, 
		 start_time);


  //update from stream file
  recent_purchases.clear();
  build_purchases(stream_file_base, 
		  &allpeople, 
		  &unique_people, 
		  &recent_purchases, 
		  1);
  //from here to the start of the streaming section
  //would go in some sort of loop as new data comes in
  //
  
}
