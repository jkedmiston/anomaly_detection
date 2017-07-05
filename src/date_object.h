//mimics datetime object (has hour, minute, second, day, month, year attributes and 
//comparison operator. Mainly this lets one do comparisons on a standard
//calendar and not get into trouble with leap years by oversizing some parameters

#ifndef DATE_OBJECT_H
#define DATE_OBJECT_H
#include <vector>
#include <stdio.h>
#include <string>
#define SEC_PER_DAY 86400
#define SEC_PER_MIN 60
#define MIN_PER_HOUR 60
#define DAY_PER_MONTH 35
#define MONTH_PER_YEAR 12
class date_t
{
//mimics datetime object (has hour, minute, second, day, month, year attributes and 
//comparison operator. Mainly this lets one do comparisons on a standard
//calendar and not get into trouble with leap years by oversizing some parameters

 public:
  int m_hour;
  int m_min;
  int m_sec;
  int m_day;
  int m_month;
  int m_year;
  unsigned long long int m_t;
  date_t(){
    _init(0,0,0,0,0,0,0);
  }
  void print() const{
    printf("%d %d %d %d %d %d\n", m_hour, m_min, m_sec, m_day, m_month, m_year);
  }
  static date_t one_day(){
    date_t retval(0,0,0,1,0,0);
    return retval;
  }

  static date_t one_year(){
    date_t retval(0,0,0,0,0,1);
    return retval;
  }

  static date_t one_month(){
    date_t retval(0,0,0,0,1,0);
    return retval;
  }

  static date_t now(){
    date_t retval;
    return retval;
  }
  std::string __repr__() {
    char buff[300];
    sprintf(buff, "%d:%d:%d %d/%d/%d", m_hour, m_min, m_sec, m_day, m_month, m_year);
    std::string s = buff;
    return s;
  }
  date_t(int hour, int min, int sec, int day, int month, int year){
    init(hour, min, sec, day, month, year);
  }
  
  void _init(int hour, int min, int sec, int day, 
	     int month, int year, unsigned long long int tot){
    //bypasses the total_time computation
    m_hour = hour;
    m_min = min;
    m_sec = sec;
    m_day = day;
    m_month = month;
    m_year = year;
    m_t = tot;
  }
  
  void init(int hour, int min, int sec, int day, int month, int year){
    m_hour = hour;
    m_min = min;
    m_sec = sec;
    m_day = day;
    m_month = month;
    m_year = year;
    m_t = this->approximate_total_time();
  }
  
  date_t(std::vector<int>& date_array){
    init_from_vec(date_array);
  }
  
  void init_from_vec(std::vector<int>& date_array){
    init(date_array[0], date_array[1], date_array[2], 
	 date_array[3], date_array[4], date_array[5]);
  }

  void init_from_vec_full(std::vector<unsigned long long int>& date_array){
    _init(date_array[0], date_array[1], date_array[2], 
	 date_array[3], date_array[4], date_array[5], date_array[6]);
  }
  
  date_t(const date_t& other){
    this->copy(other);
  }
  
  void copy(const date_t& other){
    std::vector<unsigned long long int> copyvals = other.date_to_vec_full();
    this->init_from_vec_full(copyvals);
  }
  
  date_t& operator= (const date_t& source){
    if (this == &source)
      return *this;
    
    this->copy(source);
    
    return *this;
  }


  

  std::vector<unsigned long long int> date_to_vec_full() const{
    std::vector<unsigned long long int> retval(7);
    retval[0] = m_hour;     retval[1] = m_min;     retval[2] = m_sec;
    retval[3] = m_day;     retval[4] = m_month;     retval[5] = m_year;
    retval[6] = m_t;
    return retval;
  }

  std::vector<int> date_to_vec() const{
    std::vector<int> retval(6);
    retval[0] = m_hour;     retval[1] = m_min;     retval[2] = m_sec;
    retval[3] = m_day;     retval[4] = m_month;     retval[5] = m_year;

    return retval;
  }
  friend date_t operator+(const date_t& a, const date_t& b);
  friend date_t operator-(const date_t& a, const date_t& b);
  friend bool operator<(const date_t& arg1, const date_t& arg2);
  friend bool operator==(const date_t& arg1, const date_t& arg2);
  friend bool operator>(const date_t& arg1, const date_t& arg2);
  friend bool operator>=(const date_t& arg1, const date_t& arg2);

  bool checkless(date_t& arg2){
    bool retval;
    if(this->m_year > arg2.m_year){
      retval = false;
    } else if(this->m_year < arg2.m_year){
      retval = true;
    } else if (this->m_month > arg2.m_month){
    retval = false;
    } else if (this->m_month < arg2.m_month){
    retval = true;
    } else if (this->m_day > arg2.m_day){
    retval = false;
    } else if (this->m_day < arg2.m_day){
      retval = true;
    } else if (this->m_hour > arg2.m_hour){
      retval = false;
    } else if (this->m_hour < arg2.m_hour){
      retval = true;
    } else if (this->m_min > arg2.m_min){
      retval = false;
    } else if (this->m_min < arg2.m_min){
      retval = true;
    } else if (this->m_sec > arg2.m_sec){
      retval = false;
    } else if (this->m_sec < arg2.m_sec){
      retval = true;
    } else {
      //must be equal
      retval = false;
    }
  
  return retval;

    
}
  unsigned long long int approximate_total_time() const{
    //overcount days in month and days in year to avoid problems with leap years
    //this is just a rough sorting function
    unsigned long long int t1 = m_sec + m_min * SEC_PER_MIN + m_hour * MIN_PER_HOUR * SEC_PER_MIN + m_day * SEC_PER_DAY;
    unsigned long long int t2 = (unsigned long long)(m_month) * DAY_PER_MONTH * SEC_PER_DAY;
    unsigned long long int t3 = (unsigned long long)(m_year) * (long long)(MONTH_PER_YEAR * DAY_PER_MONTH * SEC_PER_DAY);
    
    return t1 + t2 + t3;
  }

};
inline bool operator>=(const date_t& arg1, const date_t& arg2){
  return arg1.m_t >= arg2.m_t;
}

inline date_t operator+(const date_t& a, const date_t& b){
  date_t retval(a.m_hour + b.m_hour,
		a.m_min + b.m_min,
		a.m_sec + b.m_sec,
		a.m_day + b.m_day,
		a.m_month + b.m_month,
		a.m_year + b.m_year);
  //IMPROVE ME
  //need logics to keep e.g. hours less than 24 and increment day if 
  //over 24, etc. 
  /*if(retval.m_hour > 24){
    retval.m_hour 
    }*/
  return retval;
}

inline date_t operator-(const date_t& a, const date_t& b){
  date_t retval(a.m_hour - b.m_hour,
		a.m_min - b.m_min,
		a.m_sec - b.m_sec,
		a.m_day - b.m_day,
		a.m_month - b.m_month,
		a.m_year - b.m_year);
  //IMPROVE ME
  //need logics to keep e.g. hours less than 24 and increment day if 
  //over 24, etc. 
  /*if(retval.m_hour > 24){
    retval.m_hour 
    }*/
  return retval;
}

inline bool operator<(const date_t& arg1, const date_t& arg2){
  return arg1.m_t < arg2.m_t;
  bool retval;
  if(arg1.m_year < arg2.m_year){
    retval = true;
  } else if (arg1.m_month < arg2.m_month){
    retval = true;
  } else if (arg1.m_day < arg2.m_day){
    retval = true;
  } else if (arg1.m_hour < arg2.m_hour){
    retval = true;
  } else if (arg1.m_min < arg2.m_min){
    retval = true;
  } else if (arg1.m_sec < arg2.m_sec){
    retval = true;
  } else {
    retval = false;
  }
  return retval;
}
inline bool operator==(const date_t& arg1, const date_t& arg2){
  bool date1 = (arg1.m_year == arg2.m_year) && (arg1.m_month == arg2.m_month) && (arg1.m_day == arg2.m_day);
  bool time1 = (arg1.m_hour == arg2.m_hour) && (arg1.m_min == arg2.m_min) && (arg1.m_sec == arg2.m_sec);
  return time1 && date1;
}

#endif
