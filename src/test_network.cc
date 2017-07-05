#include "node.h"
#include "purchase.h"
#include "date_object.h"

int main(){  //printf("hi\n");
  
  //std::map<int, node_t*> m;
  //m[1] = new node_t(1);
  node_t** as = new node_t*[20];
  for(int i = 0; i < 20; ++i){
    node_t* a = new node_t(i);
    as[i] = a;
  }
  
  as[0]->append(as[4]);
  as[0]->append(as[5]);
  as[0]->append(as[6]);
  as[0]->append(as[7]);
  
  as[1]->append(as[4]);
  as[1]->append(as[5]);
  as[1]->append(as[6]);
  as[1]->append(as[7]);

  as[6]->append(as[8]);
  as[6]->append(as[9]);
  
  as[8]->append(as[19]);
  as[19]->append(as[18]);

  purchase_t p1(10, 11, 8, 29, 9, 1981, 34.3);
  purchase_t p2(10, 11, 19, 29, 9, 1981, 40.3);
  purchase_t p3(10, 11, 13, 29, 9, 1981, 403.3);
  purchase_t p4(10, 11, 13, 29, 9, 1980, 13.3);

  purchase_t p5(13, 11, 13, 29, 9, 1983, 14.3);
  purchase_t p6(15, 11, 13, 29, 9, 1984, 15.3);
  purchase_t p7(19, 11, 13, 29, 9, 1989, 17.3);

  purchase_t p8(18, 11, 13, 29, 9, 1983, 11.3);
  purchase_t p9(19, 11, 13, 29, 9, 1984, 12.3);
  purchase_t p10(20, 11, 13, 29, 9, 1989, 18.3);
  
  std::vector<purchase_t> purchases(10);
  purchases[0] = p1;
  purchases[1] = p2;
  purchases[2] = p3;
  purchases[3] = p4;
  purchases[4] = p5;
  purchases[5] = p6;
  purchases[6] = p7;
  purchases[7] = p8;
  purchases[8] = p9;
  purchases[9] = p10;
  
  double psum = 0;
  for(int i = 0; i < (int)purchases.size(); ++i){
    psum += purchases[i].m_amount;
  }
  psum /= purchases.size();
  
  as[1]->add_purchase(p1);
  as[1]->add_purchase(p2);
  as[1]->add_purchase(p3);
  as[1]->add_purchase(p4);

  as[4]->add_purchase(p5);
  as[4]->add_purchase(p6);
  as[4]->add_purchase(p7);

  as[18]->add_purchase(p8);
  as[18]->add_purchase(p9);
  as[18]->add_purchase(p10);
  
  as[1]->set_nth_order_network(4);
  print_network(as[1]->m_nodemap_network);
  
  date_t start_time(9, 1, 1, 1, 1, 2000);
  int nout;

  double mean, sd;
  as[1]->get_purchase_mean_and_sd(3, start_time, &mean, &sd, &nout);
  if(fabs(mean - (18.3 + 17.3 + 12.3 ) / 3.) > 1e-7){
    printf("error\n");
    throw 2;
  } else {
    printf("mean at order 3 ok\n");
  }
  as[1]->get_purchase_mean_and_sd(10, start_time, &mean, &sd, &nout);
  if(fabs(mean - psum) > 1e-7){
    throw 2;
  } else {
    printf("total mean ok\n");
  }

  as[1]->set_nth_order_network(1);
  as[1]->get_purchase_mean_and_sd(10, start_time, &mean, &sd, &nout);
  if(fabs(mean - (p1.m_amount + p2.m_amount + p3.m_amount + p4.m_amount + p5.m_amount + p6.m_amount + p7.m_amount)/7) > 1e-7){
    throw 2;
  } else {
    printf("local mean ok\n");
  }
  for(int i = 0; i < 20; ++i){
    delete as[i];
  }
  delete [] as;
}
