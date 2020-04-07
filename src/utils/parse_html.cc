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

#include<gumbo.h>
#include<string>
#include<unordered_set>

#include"values.h"

using namespace std;

extern void format_url(string&);

void get_links_contents(GumboNode*node,unordered_set<string>&links,string&contents){
  if(node->type==GUMBO_NODE_TEXT){
    contents+=string(node->v.text.text);
    return;
  }else if(node->type==GUMBO_NODE_ELEMENT&&
	   node->v.element.tag!=GUMBO_TAG_SCRIPT&&
	   node->v.element.tag!=GUMBO_TAG_STYLE){
    GumboAttribute*href;
    if(node->v.element.tag==GUMBO_TAG_A&&
       (href=gumbo_get_attribute(&node->v.element.attributes,"href"))){
	string tmp_url=href->value;
	format_url(tmp_url);
	if(!tmp_url.empty()&&tmp_url.size()<=URL_SIZE)
	  links.insert(tmp_url);
    }
    string temp_contents="";
    GumboVector*children=&node->v.element.children;
    for(unsigned int i=0;i<children->length;++i){
      auto text=contents;
      get_links_contents((GumboNode*)children->data[i],links,contents);
      if(i!=0&&text!=contents)
	contents.append("\n");
    }
  }
}

void find_title(const GumboNode*root,string&title){
  title="";
  if(root->type!=GUMBO_NODE_ELEMENT||root->v.element.children.length<1){
    return;
  }
  const GumboVector*root_children=&root->v.element.children;
  GumboNode*head=nullptr;
  for(int i=0;i<root_children->length;++i){
    GumboNode*child=(GumboNode*)root_children->data[i];
    if(child->type==GUMBO_NODE_ELEMENT&&
       child->v.element.tag==GUMBO_TAG_HEAD){
      head=child;
      break;
    }
  }
  if(!head)return;
  GumboVector*head_children=&head->v.element.children;
  for(int i=0;i<head_children->length;++i){
    GumboNode*child=(GumboNode*)head_children->data[i];
    if(child->type==GUMBO_NODE_ELEMENT&&
       child->v.element.tag==GUMBO_TAG_TITLE){
      if(child->v.element.children.length!=1)
	return;
      GumboNode*title_=(GumboNode*)child->v.element.children.data[0];
      if(title_->type!=GUMBO_NODE_TEXT&&
	 title_->type!=GUMBO_NODE_WHITESPACE)
	return;
      title=string(title_->v.text.text);
      return;
    }
  }
}
