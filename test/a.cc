#include"db/sql_db_operation.hpp"
int main(){
  using namespace amigao;
  DBOperationInterface*db_op=new SqlDBOperation();
}
    virtual void add_dict_table(std::string dict_name,
				std::map<std::string,std::string>&word_no)=0;
    virtual void add_dict_table(std::string dict_name,
				std::string dict_file_path)=0;
    virtual void add_dict_table(std::string dict_name,
				std::vector<std::string>&vec)=0;
    virtual void add_dict_table(std::string dict_name)=0;
virtual void op_website_nice(std::string url,
				 std::string nice)=0;
    virtual void op_visited_url(std::string url,
				std::string&title,
				std::string&contents,
				std::string&html)=0;
    virtual void op_reverse(std::string url,
				  std::map<std::string,int>&weight)=0;
    virtual long get_url_no(std::string url)=0;
    virtual short get_website_nice(std::string url)=0;
    virtual void rm_dict_table(std::string dict_name)=0;
    virtual bool if_exist(std::string select,
			  std::string from,
			  std::string column,
			  std::string value)=0;
    virtual void op_dict_manager(std::string dict_name,
				 std::string status,
				 std::string dict_no,
				 std::string comments)=0;
    virtual void rm_dict_manager(std::string dict_name)=0;
    virtual std::priority_queue<PQueueNode*>*search_sentence(std::string)=0;
    virtual void op_strategy_manager(std::string strategy_name,
				     std::string status,
				     std::string comment)=0;
    virtual void rm_strategy_manger(std::string strategy_name)=0;
