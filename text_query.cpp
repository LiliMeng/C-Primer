#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <memory>
#include <set>
#include <fstream>

using namespace std;
using line_no = std::vector<std::string>::size_type;

string make_plural(size_t ctr, const string &word,
                               const string &ending)
                               
{
    return (ctr>1) ? word + ending : word;
    }


class QueryResult;
class TextQuery {
public:
    
    TextQuery(std::ifstream&);
    QueryResult query(const std::string&) const;
    private:
        
    	std::shared_ptr<std::vector<std::string>> file;
    	std::map<std::string,
    	         std::shared_ptr<std::set<line_no>>> wm;   //a map from string to shared_ptr<set>
    	       };
    	       
TextQuery::TextQuery(ifstream &is):file(new vector<string>)     //TextQuery constructor
{
    string text;
    while (getline(is, text)){
 	file->push_back(text);
 	int n=file->size() -1;
 	istringstream line(text);
 	string word;
 	while (line >> word) {
 		auto &lines = wm[word];
 		if(!lines)
 		   lines.reset(new set<line_no>);
 		lines->insert(n);
 		}
 		}
 		}
   
class QueryResult {                                        
friend std::ostream& print(std::ostream&, const QueryResult&);
public:
    QueryResult (std::string s,
                 std::shared_ptr<std::set<line_no>> p,
                 std::shared_ptr<std::vector<std::string>> f):
    	sought(s), lines(p),file(f) { }

private:
    std::string sought; //data member, a string that is the word whose results it represents;
    std::shared_ptr<std::set<line_no>> lines; // a shared_ptr to the set of line numbers on which this word appears;
    std::shared_ptr<std::vector<std::string>> file; // a shared_ptr to the vector containing the input file;
    };
    
QueryResult
TextQuery::query(const string &sought) const
{       // we'll return a pointer to this set if we don't find sought


    
	static shared_ptr<set<line_no>> nodata(new set<line_no>);
	auto loc = wm.find(sought);
	if (loc == wm.end())
		return QueryResult(sought, nodata, file);
    else
        return QueryResult(sought, loc->second, file);
 }




ostream &print(ostream & os, const QueryResult &qr)
{
	os<<qr.sought<<"occurs "<<qr.lines->size()<<""
	  <<make_plural(qr.lines->size(),"time","s")<<endl;
	for (auto num: *qr.lines)
		os<<"\t (line)"<<num+1<<")"
		  <<*(qr.file->begin()+num)<<endl;
        return os;
        }
        
        
void runQueries(ifstream &infile)
{
	TextQuery tq(infile);
	while (true) {
		cout << "enter word to look for, or q to quit: ";
		string s;
		if (!(cin>>s)||s=="q") break;
		print(cout,tq.query(s))<<endl;
		}
}
int main()
{
  
   
   ifstream infile("UBC");
   runQueries(infile);
   
   
return 0;
}
