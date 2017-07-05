#include "purchase.h"
#include "node.h"
//class node_t;
purchase_t::purchase_t(int hour, int min, int sec, int day, int month, int year, double amount, node_t* n){
  m_date_of_purchase.init(hour, min, sec, day, month, year);
  m_amount  = amount;
  //m_id = n->m_id;
  m_node = n;
}

