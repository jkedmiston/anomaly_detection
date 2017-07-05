#ifndef PURCHASE_H
#define PURCHASE_H
//basic object for storing date of purchase, amount, and person (node)

#include "date_object.h"
#include <iostream>

class node_t;

class purchase_t
{						
 public:
  date_t m_date_of_purchase;
  double m_amount;
  int m_id;
  node_t* m_node;
  purchase_t(){
    m_amount = -1000;
  }

  purchase_t(int hour, int min, int sec, int day, int month, int year, double amount){
    m_date_of_purchase.init(hour, min, sec, day, month, year);
    m_amount  = amount;
    m_id = -1;
    m_node = NULL;
  }

  purchase_t(int hour, int min, int sec, int day, int month, int year, double amount, node_t* n);

  purchase_t(date_t& date, double amount){
    m_date_of_purchase = date;
    m_amount = amount;
    m_id = -1;
    m_node = NULL;
  }
  
  void set_id(int id){
    m_id = id;
  }
  
  std::string __repr__(){
    std::string s = m_date_of_purchase.__repr__();
    char buff[100];
    sprintf(buff, "%s %f", s.c_str(), m_amount);
    //s += std::string(m_amount);
    return buff;
  }

  purchase_t(const purchase_t& other){
    this->copy(other);
  }
  
  void copy(const purchase_t& other){
    this->m_date_of_purchase = other.m_date_of_purchase;
    this->m_amount = other.m_amount;
    this->m_id = other.m_id;
    this->m_node = other.m_node;
  }
  
  purchase_t& operator= (const purchase_t& source){
    if (this == &source)
      return *this;
    
    this->copy(source);
    
    return *this;
  }

  double get_purchase_value(){
    return this->m_amount;
  }
  
  friend bool operator<(const purchase_t& arg1, const purchase_t& arg2);
  friend std::ostream& operator<<(std::ostream &out, purchase_t& arg1);
};

inline bool operator<(const purchase_t& arg1, const purchase_t& arg2){
  return arg1.m_date_of_purchase < arg2.m_date_of_purchase;
}
inline std::ostream& operator<<(std::ostream& out, purchase_t& arg1){
  char buff[300];
  sprintf(buff, "%.2d:%.2d:%.2d %.2d/%.2d/%.4d: %5.2f id: %d", arg1.m_date_of_purchase.m_hour, arg1.m_date_of_purchase.m_min, arg1.m_date_of_purchase.m_sec, arg1.m_date_of_purchase.m_day, arg1.m_date_of_purchase.m_month, arg1.m_date_of_purchase.m_year, arg1.m_amount, arg1.m_id);
  out << "purchase_t: " << buff;
  return out;
}

#endif
