#include "stdafx.h"
#include "op_node.h"
#include "params.h"
using namespace std;

string Line2Eqn(vector<node>& line,string& eqnForm,params& p)
{
	vector<string> eqn_floatstack;
	vector<string> eqn_boolstack;
	vector<string> form_floatstack;
	vector<string> form_boolstack;
	string s1,s1f,s2,s2f,b1,b1f,b2,b2f;
	for(unsigned int i=0;i<line.size();++i)
	{
		if(line.at(i).on){
			if(line.at(i).type=='n'){
				eqn_floatstack.push_back(to_string(static_cast<long double>(line.at(i).value)));
				form_floatstack.push_back("n");
			}
			else if(line.at(i).type=='v'){
				eqn_floatstack.push_back(line.at(i).varname);
				form_floatstack.push_back(line.at(i).varname);
			}
			else{
				string sop;
				//string sop2; // for protected functions
				//char tmp = line.at(i).type;
				if(eqn_floatstack.size() >= line[i].arity_float && eqn_boolstack.size()>=line[i].arity_bool){
					
					switch (line.at(i).type){
					case 'l':
						sop="log";
						break;
					case 's':
						sop="sin";
						break;
					case 'c':
						sop="cos";
						break;
					case 'e':
						sop="exp";
						break;
					case 'q':
						sop="sqrt";
						break;
					case '{':
						sop = "<=";
						break;
					case '}':
						sop = ">=";
						break;
					case 'i':
						sop = "IF-THEN";
						break;
					case 't':
						sop = "IF-THEN-ELSE";
						break;
					case '&':
						sop = "&&";
						break;
					case '|':
						sop = "||";
						break;
					case '=':
						sop="==";
						break;
					default:
						sop = string(&line[i].type);
						sop = sop[0];
						//cout << "error";
					}
					if(line[i].arity_float>=1){
						s1 = eqn_floatstack.back(); eqn_floatstack.pop_back();
						s1f = form_floatstack.back(); form_floatstack.pop_back();
					
						if(line[i].arity_float==2){
							s2 = eqn_floatstack.back(); eqn_floatstack.pop_back();
							s2f = form_floatstack.back(); form_floatstack.pop_back();
						}
					}
					// get boolean values
					if(line[i].arity_bool>=1){
						b1 = eqn_boolstack.back(); eqn_boolstack.pop_back();
						b1f = form_boolstack.back(); form_boolstack.pop_back();
					
						if(line[i].arity_bool==2){
							b2 = eqn_boolstack.back(); eqn_boolstack.pop_back();
							b2f = form_boolstack.back(); form_boolstack.pop_back();
						}
					}
					
					if(line[i].return_type=='f'){
						if(line[i].arity_float==1 && line[i].arity_bool==0){
							eqn_floatstack.push_back(sop + "("+s1+")");
							form_floatstack.push_back(sop + "("+s1f+")");
						}
						else if(line[i].arity_float==1 && line[i].arity_bool==1){
							eqn_floatstack.push_back(sop + "("+b1+","+s1+")");
							form_floatstack.push_back(sop + "("+b1f+","+s1f+")");
						}
						else if (line[i].arity_float==2 && line[i].arity_bool==0){
							eqn_floatstack.push_back("("+s2+sop+s1+")");
							form_floatstack.push_back("("+s2f+sop+s1f+")");
						}
						else if (line[i].arity_float==2 && line[i].arity_bool==1){
							eqn_floatstack.push_back(sop + "("+b1+","+s2+","+s1+")");
							form_floatstack.push_back(sop + "("+b1f+","+s2f+","+s1f+")");
						}
						else
							cout << "missed something";
					}
					else if (line[i].return_type=='b'){
						if(line[i].arity_float==2 && line[i].arity_bool==0){
							eqn_boolstack.push_back("("+s2+sop+s1+")");
							form_boolstack.push_back("("+s2f+sop+s1f+")");
						}
						else if(line[i].arity_float==0 && line[i].arity_bool==2){
							eqn_boolstack.push_back("("+b2+sop+b1+")");
							form_boolstack.push_back("("+b2f+sop+b1f+")");
						}
						else if(line[i].arity_float==0 && line[i].arity_bool==1){
							eqn_boolstack.push_back(sop + "("+b1+")");
							form_boolstack.push_back(sop + "("+b1f+")");
						}
						else
							cout << "missed something";
					}
					else
							cout << "missed something";
					
				}					//cout <<"arity screwed up.\n";
			}	
		}
	}
	//if ((eqn_floatstack.empty() && !p.classification) || (eqn_boolstack.empty() && p.classification) )
	if (eqn_floatstack.empty())	
	{
		eqnForm = "unwriteable";
		//cout << "equation stack empty.\n";
		return "unwriteable";
	}
	/*if (p.classification){
		eqnForm = form_boolstack.back();
		return eqn_boolstack.back();
	}
	else{*/
		eqnForm = form_floatstack.back();
		return eqn_floatstack.back();
	//}
}