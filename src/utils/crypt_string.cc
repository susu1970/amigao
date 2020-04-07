// Copyright 2020 amigao. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Author: 758293230@qq.com
//

#include<string>

using namespace std;
/*
 *(de)crypt rules: 
                   ' <-> #0
                   " <-> #1
		   \ <-> #2
		   | <-> #3
		   & <-> #4
		   ( <-> #5
		   ) <-> #6
		   , <-> #7
		   \n <-> #8
		   ; <-> #9
		   `  <-> #a
		   # <-> ##
		   
 */

//pass text-to-be-crypted by const-reference,return cryped string
static inline char dec2hex_char(short int n) {
 if ( 0 <= n && n <= 9 ) {
  return char( short('0') + n );
 } else if ( 10 <= n && n <= 15 ) {
  return char( short('A') + n - 10 );
 } else {
  return char(0);
 }
}
 
static inline short int hex_char2dec(char c) {
 if ( '0'<=c && c<='9' ) {
  return short(c-'0');
 } else if ( 'a'<=c && c<='f' ) {
  return ( short(c-'a') + 10 );
 } else if ( 'A'<=c && c<='F' ) {
  return ( short(c-'A') + 10 );
 } else {
  return -1;
 }
}
 
string crypt(const string &URL){
 string result = "";
 for ( size_t i=0; i<URL.size(); i++ ) {
  char c = URL[i];
  if (
   ( '0'<=c && c<='9' ) ||
   ( 'a'<=c && c<='z' ) ||
   ( 'A'<=c && c<='Z' ) //||
   //   c=='/' || c=='.'
   ) {
   result += c;
  } else {
   int j = (short int)c;
   if ( j < 0 ) {
    j += 256;
   }
   int i1, i0;
   i1 = j / 16;
   i0 = j - i1*16;
   //   result += '%';
   result += '#';
   result += dec2hex_char(i1);
   result += dec2hex_char(i0);
  }
 }
 return result;
}
string decrypt(const string &URL) {
 string result = "";
 for ( size_t i=0; i<URL.size(); i++ ) {
  char c = URL[i];
  //  if ( c != '%' ) {
  if (c != '#' ) {
   result += c;
  } else {
   char c1 = URL[++i];
   char c0 = URL[++i];
   int num = 0;
   num += hex_char2dec(c1) * 16 + hex_char2dec(c0);
   result += char(num);
  }
 }
 return result;
}
/*
 *below (de)crypt codes are outdated
string crypt(const string&text){
  string tmp="";
  for(size_t i=0;i<text.size();++i){
    if(text[i]=='\'')
      tmp+="#0";
    else if(text[i]=='"')
      tmp+="#1";
    else if(text[i]=='\\')
      tmp+="#2";
    else if(text[i]=='#')
      tmp+="##";
    else if(text[i]=='|')
      tmp+="#3";
    else if(text[i]=='&')
      tmp+="#4";
    else if(text[i]=='(')
      tmp+="#5";
    else if(text[i]==')')
      tmp+="#6";
    else if(text[i]==',')
      tmp+="#7";
    else if(text[i]=='\n')
      tmp+="#8";
    else if(text[i]==';')
      tmp+="#9";
    else if(text[i]=='`')
      tmp+="#a";
    else
      tmp+=text[i];
  }
  return tmp;
}
//pass text-to-be-decrypted by const-reference,return decryped string
string decrypt(const string&text){
  if(text.empty())return "";
  string tmp="";
  size_t i=0,j=1;
  for(;j<text.size();++i,++j){
    if(text[i]=='#'){
      if(text[j]=='#')
	tmp+="#";
      else if(text[j]=='0')
	tmp+="'";
      else if(text[j]=='1')
	tmp+="\"";
      else if(text[j]=='2')
	tmp+="\\";
      else if(text[j]=='3')
	tmp+="|";
      else if(text[j]=='4')
	tmp+="&";
      else if(text[j]=='5')
	tmp+="(";
      else if(text[j]=='6')
	tmp+=")";
      else if(text[j]=='7')
	tmp+=",";
      else if(text[j]=='8')
	tmp+="\n";
      else if(text[j]=='9')
	tmp+=";";
      else if(text[j]=='a')
	tmp+="`";
      else
	return "";
      ++i;++j;
    }else
      tmp+=text[i];
  }
  return tmp+text[i];
}
*/
