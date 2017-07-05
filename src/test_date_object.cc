#include "date_object.h"
#include <iostream>
#include <stdio.h>
#include <map>
#include <stdlib.h>
void test_operators(){
  date_t a(1, 34, 39, 4, 2, 1981);
  date_t b(1, 33, 29, 4, 2, 1981);
  date_t c(1, 33, 29, 4, 2, 1981);

  std::vector<int> b_data = b.date_to_vec();
  std::vector<int> b_data_copy;

  for(int i = 0; i < 6; ++i){
    b_data_copy = b_data;
    b_data_copy[i] += 1;
    date_t a(b_data_copy);
    if (b < a){
      printf("b < a test ok \n");
    } else {
      printf("b < a test failed \n");
    }
    if ( a < b) {
      printf("a < b test failed\n");
    } else {
      printf("a < b test ok\n");      
    }
  }


  if(b == c){
    printf("b == c test ok\n");
  } else {
    printf("b == c test failed \n");
    }


}
int main(){
  //map check
  test_operators();

  
  std::map<date_t, int> m;

  date_t a(1, 34, 39, 4, 2, 1981);
  date_t b(1, 33, 29, 4, 2, 1981);
  int nsamples = 10000;
  date_t** as = new date_t*[nsamples];
  for(int i = 0; i < nsamples; ++i){
    int hour = rand() % 24;
    int day = rand() % 31;
    int minute = rand() % 60;
    int second = rand() % 60;
    int month = rand() % 12;
    int year = rand() % 2020;
    date_t* a = new date_t(hour, minute, second, day, month, year);
    as[i] = a;		
    m[(*a)] = i;
  }

  std::vector<date_t> dates(nsamples);
  std::map<date_t, int>::iterator it;
  int kk = 0;
  for(it = m.begin(); it != m.end(); ++it, ++kk){
    dates[kk] = (it->first);
  }

  for(int j = 0; j < nsamples - 1; ++j){
    if ( (dates[j]) < (dates[j + 1])){
      //printf("d[n] < d[n+1] ok\n");
      int kk = (dates[j+1].checkless(dates[j]));
      if(kk==1){
	dates[j+1].checkless(dates[j]);
	dates[j].print();
	dates[j+1].print();
	throw 2;
      }
    } else {
      printf("fail\n");
      throw 2;
    }
    if ( (dates[j + 1]) < (dates[j])){
      printf("fail\n");
      throw 2;
    } else {
      //printf("d[n + 1] !< d[n] ok\n");

    }
    
  }
  printf("map<date_t, ..> ordering ok \n");
}
