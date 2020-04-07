#include<string>

unsigned short str2sum(const std::string &str){
  unsigned short num=0;
  for(size_t i=0;i<str.size();++i){
    short tmp=(str[i]-'0')<<i;
    num+=tmp;
  }
  return num;
}
