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

#include"values.h"

using namespace std;

/*
  function:
    give a source url address,cut it's meaningless part and simplify it,include cut the 'www.' part
*/
void format_url(string&url){
  if(url.empty()||url.size()>URL_SIZE){//URL_SIZE is the maximum size of url address
    url="";
    return;
  }
  //to lowercase
  size_t i=0,j=0,k=0,g=0,h=0;//work subscript
  size_t len=url.size();
  for(;i<len;++i)
    if(url[i]>='A'&&url[i]<='Z')
      url[i]+=32;
  //filte meaningless character
  for(i=0;i<len&&
	(url[i]==' '||
	 url[i]=='\t');++i);
  //cut the meaningless tail characters
  for(j=len-1;j>=i;--j){
    if(url[j]==' '||url[j]=='\t'||url[j]=='/'||url[j]=='\n')
      len=j;//update tail subscript
    else
      break;
  }
  //now the effective part of url is url.substr(i,len-i),if legal
  string http="http";
  //try to match the 'http' prefix
  for(j=i,g=j;j<len&&k<http.size()&&url[j]==http[k];++j,++k);//backup 'j' to 'g'
  if(k!=http.size())
    j=g;//not match,recovery j
  else{//found 'http' prefix
    if(j<len&&url[j]=='s')++j;//match the 'https' prefix
    string tmp_str="://";
    //try to match the '://' prefix
    for(k=0;j<len&&k<tmp_str.size()&&url[j]==tmp_str[k];++j,++k);
    if(k!=tmp_str.size())
      j=g;//not match,recovery
  }
  string www="www.";
  //try to match the 'www.' prefix
  for(k=0,g=j;j<len&&k<www.size()&&url[j]==www[k];++j,++k);
  if(k!=www.size()){//represent failed
    j=g;//recovery j
  }
  i=j;//update start subscript
  string javascript="javascript";
  //try to match the 'javascript'
  for(k=0;j<len&&k<javascript.size()&&url[j]==javascript[k];++j,++k);
  if(i==len||
     (k==javascript.size()&&len-i==javascript.size())||
      (j<len&&url[j]==':'))
    url="";
  else
    url=url.substr(i,len-i);
  if(url.size()==0||
     !(
       (url[0]>='a'&&url[0]<='z')||
       (url[0]>='A'&&url[0]<='Z')||
       (url[0]>='0'&&url[0]<='9')
       ))
    url="";
}
