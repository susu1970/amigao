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
// Author: susu1970@yandex.com 
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
		   # <-> ##
		   
 */

//pass text-to-be-crypted by const-reference,return cryped string
string crypt(const string&text){
  string tmp="";
  for(int i=0;i<text.size();++i){
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
    else
      tmp+=text[i];
  }
  return tmp;
}
//pass text-to-be-decrypted by const-reference,return decryped string
string decrypt(const string&text){
  if(text.empty())return "";
  string tmp="";
  int i=0,j=1;
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
      else
	return "";
      ++i;++j;
    }else
      tmp+=text[i];
  }
  return tmp+text[i];
}
