#include<stdio.h>
#include<fcntl.h>
#include<iostream>
#include<string>
#include<stdlib.h>
#include<string>
#include<map>
#include<fstream>
#include<regex>
#include<unistd.h>
#include<assert.h>

using namespace std;

string str1="fdsklfjsdlkjfskdf kldsf< \tmeta http-equiv=\"content-type\" content=\"text/html;charset   =       '   GB2312'\"> </meta> kldsfjkljfdsklfj";
string str2="fdsklfjsdlkjfskdf kldsf< \tmeta http-equiv=\"content-type\" content=\"text/html;charset   =       \"   GB2312\"'\"/> </meta> kldsfjkljfdsklfj";
string str3="fdsklfjsdlkjfskdf kldsf< \tmeta http-equiv=\"conte/ >nt-type\" content=\"text/html;charset   =       '   GB2312'\"> </meta> kldsfjkljfdsklfj";
string str4="fdsklfjsdlkjfskdf kldsf< \tmeta http-equiv=\"conte<   /meta  >nt/-type\" content=\"text/html;charset   =       '   GB2312'\"> <    /meta    \t> kldsfjkljfdsklfj";
string str5="fdsklfjsdlkjfskdf kldsf \tmeta http-equiv=\"conte<   /meta  >nt-t/>ype\" content=\"text/html;charset   =       '   GB2312'\"> <    /meta    \t> kldsfjkljfdsklfj";
string str6="fdsklfjsdlkjfskdf kldsf \tmeta http-equiv=\"conte<   /meta  >nt-typ<  /meta >e\" content=\"text/html;charset   =       '   GB2312'\"> <    /meta    \t> kldsfjkljfdsklfj";
string str7="fdsklfjsdlkjfskdf kldsf< \tmeta http-equiv=\"content-type/\" content</meta=\"text/html;charset   =       '   GB2312'\"> </meta> kldsfjkljfdsklfj";
//<meta http-equiv="content-type" content="text/html;charset=GB2312"/>
//   

int main(int argc,char**argv){
  static string gbk_pattern="<\\s*(meta){1}\\s*.*[(/>)(<\\s*/meta\\s*>)]{0}(charset)+\\s*={1}\\s*\"?'?\\s*(((gB)+)|((Gb)+)|((gb)+)|((GB)+)).*[(/>)(<\\s*/meta\\s*>)]{1}";
  static regex reg((gbk_pattern));
  assert(regex_search(str1,reg));
  assert(regex_search(str2,reg));  
  assert(regex_search(str3,reg));
  assert(regex_search(str4,reg));
  assert(regex_search(str5,reg));
  assert(regex_search(str6,reg));
  assert(regex_search(str7,reg));

}
