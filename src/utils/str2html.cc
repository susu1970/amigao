#include<string>

using namespace std;

string str2html(const string&str){
  string res="";
  for(size_t i=0;i<str.size();++i){
    switch(str[i]){
    case '`':
      res+="&acute;";break;
    case '<':
      res+="&lt;";break;
    case '>':
      res+="&gt;";break;
    case '&':
      res+="&amp";break;
    case '"':
      res+="&quot;";break;
    case '^':
      res+="&circ;";break;
    case '/':
      res+="&frasl;";break;
    case '\'':
      res+="&apos;";break;
    default:
      res+=str[i];break;
    }
  }
  return res;
}
