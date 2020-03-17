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

extern void format_url(string&);

/*
  split given url,get host url address and page path on that host
 */
void parse_url(string url,string&host_url,string&page_path){
  host_url="";
  page_path="/";
  format_url(url);
  if(url.empty())
    return;
  size_t i=0;//work subscript
  for(;i<url.size()&&url[i]!='/';++i);
  host_url=url.substr(0,i);
  if(i<url.size())
    page_path=url.substr(i,url.size());
}
