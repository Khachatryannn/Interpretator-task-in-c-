#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <fstream>
// #include "testing.cpp"



std::regex operators("\\+|\\-|%|\\*|/|=|\\?|:|!|\\(|\\)|,|\\{|\\}|->");
std::regex ident("^(?!number|dot|valid|word|none|array|foo|if|else|while|for|stop|equ|notequ|big|small|X)[a-zA-Z][a-zA-Z0-9_]{0,49}$");

std::regex valNum("^[0-9]+$");
std::regex valDot("^[0-9]+\\.[0-9]+$");
std::regex valWord("^\".*\"$");
std::regex valNone("^None$");
std::regex valValid("^(YES|NO)$");
std::regex valArray("^([0-9]+(\\.[0-9]+)?|\".*\"|None|YES|NO)(, [0-9]+(\\.[0-9]+)?|, \".*\"|, None|, YES|, NO)*$");

std::regex keyword("^(if|else|while|for|stop|equ|notequ|big|small|X)$");
std::regex type("^(number|dot|valid|word|none|array|foo)$"); // horinvac barer for types 

std::regex exprBool("^[0-9]+(\\.[0-9]+)?((equ|notequ|big|small)[0-9]+(\\.[0-9]+)?)+$");
std::regex exprNum("^[0-9]+(\\.[0-9]+)?([+\\-*/%][0-9]+(\\.[0-9]+)?)+$");
std::regex exprStr("^(\".*\")(\\+\".*\")+$");


std::vector<std::string> ids;

bool isValue(std::string& str){
	return ((std::regex_match(str, valNum)) || (std::regex_match(str, valDot)) || (std::regex_match(str, valWord)) || (std::regex_match(str, valNone)) || (std::regex_match(str, valValid)) || (std::regex_match(str, valArray)));
}

bool isExpr(std::string& str){
	return ((std::regex_match(str, exprBool)) || (std::regex_match(str, exprNum)) || (std::regex_match(str, exprStr)));
}

class Token {
	private:
		std::string name;
		std::string value;
		int posit;

	public:	
		Token () : name (), value (), posit () { };
		
		Token (std::string& a, std::string& b, int c) {
			name = a;
			value = b;
			posit = c;
		}

		Token (const Token& ob){
			name = ob.name;
			value = ob.value;
			posit = ob.posit;
		}

		Token (Token&& ob){
			name = ob.name;
			value = ob.value;
			posit = ob.posit;
			ob.name.clear();
			ob.value.clear();
			ob.posit = 0;
		}

		~Token () {}

		void setName(const std::string& str) {
			name = str;
		}

		void setValue(const std::string& str){
			value = str;
		}

		void setPosit(const int x){
			posit = x;
		}

		std::string getName () const {
			return name;
		}

		std::string getValue () const {
			return value;
		}

		int getPosit () const {
			return posit;
		}
};


class Lexer {
	private:
	std::vector<Token> tokensTable;
    
	public:
	void setTT(const Token& x){
		tokensTable.push_back(x);
	}
	std::vector<Token> getTokenTB() const {
		return tokensTable;
	}
    Token operator[](int i) {
		return tokensTable[i];
	}
};
	
bool syntaxType (Lexer& x);

std::vector<std::string> tokenisation (const std::string& a) {   //string@ bajanum a arandzin simvolneri @st probeli
        std::vector<std::string> tokens;
        
        for (int i = 0; i < a.size(); ++i){
                std::string val;
	
		while (a[i] == ' ' || a[i] == '\t' && a[i] != '\0'){
			++i;
		}
            while (a[i] != ' ' && a[i] != '\0'){
                        val.push_back(a[i]);
                        ++i;
                }
                tokens.push_back(val);
        }
	
        for (int i = 0; i < tokens.size(); ++i){
                tokens[i].erase(std::remove(tokens[i].begin(), tokens[i].end(), '\n'), tokens[i].end());
	//	std::cout << tokens[i] << std::endl; // for checking
        }
        return tokens;
}


Lexer tokensInfo (std::vector<std::string>& a) { //stugum e toxum amen simvol tuylatreli e tvyal lezvum , ete ayo irenic inch e nerkayacnum
	Lexer obj;
	int size = a.size();
    
	for (int i = 0; i < size; ++i){
		Token ob;
               if (std::regex_match(a[i], type)){
                        ob.setName("type");
                        ob.setValue(a[i]);
                        ob.setPosit(i);
		} else if (std::regex_match(a[i], operators)){
			ob.setName("operator");
			ob.setValue(a[i]);
			ob.setPosit(i);
                } else if (std::regex_match(a[i], ident) && ((i <= 1) || (i > 1 && (a[i - 1] == "=" || a[i - 1] == "-" || 
				a[i - 1] == "+" || a[i - 1] == "*" || a[i - 1] == "/" || a[i - 1] == "%") && (i > 1 && (obj[i - 2].getName() == "identifier" || 
				obj[i -2].getName() == "value")) || (i == 2  && a[1] == "->" && (a[0] == "while" || a[0] == "if"))) || (i == 4 && (a[3] == "equ" ||
				a[3] == "notequ" || a[3] == "big" || a[3] == "small")))){
			ob.setName("identifier");
			ob.setValue(a[i]);
			ob.setPosit(i);
                } else if (isValue(a[i])){
			std::string tmp = a[i];
                        int numb = 0;
                        for (int i = 0; i < tmp.size(); ++i){
				    if (tmp[i] == '"'){
					++numb;
                            }
                         }
                         if (numb > 2 && std::regex_match(a[i], exprStr)) {
                                 ob.setName("expression");
                         } else {
				ob.setName("value");
			}
			ob.setValue(a[i]);
			ob.setPosit(i);
                } else if (std::regex_match(a[i], keyword)){
			 ob.setName("keyword");
                         ob.setValue(a[i]);
                         ob.setPosit(i);
		} else if (isExpr(a[i])){
			ob.setName("expression");
			ob.setValue(a[i]);
			ob.setPosit(i);
		} else {
			std::cout << " non-existing token " << a[i] << std::endl;
			throw std::invalid_argument(" ");
                }

		obj.setTT(ob);
	}
	
	if (obj[0].getName() == "type" && obj[1].getName() == "identifier"){
		if(syntaxType(obj)){
			ids.push_back(obj[1].getValue());
		} else {
			std::cout << " wrong declaration of an identifier " <<  std::endl;
			throw std::invalid_argument("");
		
		}
	}
	return obj;
}

bool syntaxID(Lexer& x){
    std::vector<Token> res = x.getTokenTB();
	bool flag = false;
	int size = res.size();
	if(size > 0 && res[0].getName() == "identifier" && res[1].getValue() == "=" && res[size - 1].getValue() == ":"){
		int i = 2;
		if (i < size && res[i].getName() == "expression" || res[i].getName() == "value"){
			++i;
			if (i == size - 1 && res[i].getValue() == ":"){	
				flag = true;
			} else {
				return false;
			}
		} else if (i < size && res[i].getName() == "identifier"){
			auto search = std::find(ids.begin(), ids.end(), res[i].getValue());
			if (*search == res[i].getValue()){
				++i;
				for (i; i < size; ){
					if (i == size - 1 && res[i].getValue() == ":"){
						flag = true;
						++i;
					} else if (i < size && res[i].getValue() == "=" || res[i].getValue() == "+" || res[i].getValue() == "-" || res[i].getValue() == "*" || res[i].getValue() == "/" || res[i].getValue() == "%"){
						++i;
						if (res[i].getName() == "value" && res[i + 1].getValue() == ":"){
							++i;
						} else if (res[i].getName() == "identifier"){
							auto research = std::find(ids.begin(), ids.end(), res[i].getValue());
							if (*research == res[i].getValue()){
								++i;
							} else {
								std::cout << "assigning a value from a nonexistent variable" << std::endl;
								return false;
							}
						} else {
							return false;
						}
					} else {
						return false;
					}
				}
			} else {
				std::cout << "assigning a value from a nonexistent variable" << std::endl;
				return false;
			}
		} else {
			return false;
		}
	} else {
		return false;
	}
	
	return flag;
}

bool syntaxType (Lexer& x){
	std::vector<Token> res = x.getTokenTB();
	bool flag = false;
	int size = res.size();
	
	if (res[0].getName() == "type" && res[1].getName() == "identifier" && res[2].getValue() == "=" && res[size - 1].getValue() == ":"){
		int i = 3;
		if (i == size - 2 && res[i].getName() == "expression" && res[size - 1].getValue() == ":"){
			flag = true;
		} else if (i < size && res[i].getName() == "identifier"){
			auto search = std::find(ids.begin(), ids.end(), res[i].getValue());
			if (*search == res[i].getValue()){
				++i;
				for (i; i < size; ){
					if (i == size - 1 && res[i].getValue() == ":"){
						flag = true;
						++i;
					} else if (res[i].getValue() == "="){
						++i;
						if (res[i].getName() == "value" && res[i + 1].getValue() == ":" && i == size - 2){
							flag = true;
							++i;
						} else if (res[i].getName() == "identifier"){
							auto research = std::find(ids.begin(), ids.end(), res[i].getValue());
							if (*research == res[i].getValue()){
								++i;
							} else {
								std::cout << "assigning a value from a nonexistent variable" << std::endl;
								return 0;
							}
						}
					} else if (res[i].getValue() == "+" || res[i].getValue() == "-" || res[i].getValue() == "*" || res[i].getValue() == "/" || res[i].getValue() == "%"){
						++i;
						if (res[i].getName() == "value" || res[i].getName() == "identifier"){
							++i;
						}
					} else {
						return false;
					}
				}
			} else {
				std::cout << "assigning a value from a nonexistent variable" << std::endl;
				return false;
			}
		} else if (i < size && res[i].getName() == "value"){
			++i;
			for (i; i < size; ){
				if (res[i].getValue() == ":"){
					flag = true;
					++i;
				} else if (res[i].getValue() == "+" || res[i].getValue() == "-" || res[i].getValue() == "*" || res[i].getValue() == "/" || res[i].getValue() == "%"){
					++i;
					if (res[i].getName() == "value"){
						++i;
					} else if (res[i].getName() == "identifier"){
						auto research = std::find(ids.begin(), ids.end(), res[i].getValue());
						if (*research == res[i].getValue()){
							++i;
						} else {
							std::cout << "assigning a value from a nonexistent variable" << std::endl;
							return false;
						}
					} else {
						return false;
					}
				} else {
					return false;
				}
			}
		} else {
			return false;
		}
	} else {
		return false;
	}

	return flag;
}
					
				
					
bool syntaxFor(Lexer& x){
	std::vector<Token> res = x.getTokenTB();
	int size = res.size();
	bool flag = false;

	if (res[0].getValue() == "for" && res[size - 1].getValue() == "{") {
		int i = 1;
		if (res[i].getValue() == "->" && res[size - 1 - i].getValue() == "->"){
			++i;
			if(res[i].getValue() == "X" && res[size - 1 - i].getName() == "expression"){
				++i;
				if (i == size/2){
					flag = true;
				} else {
					std::cout << "extra symbols has been used or too few symbols are present" << std::endl;
					return false;
				}
			} else {
				std::cout << "X is not used /or an expression" << std::endl;
				return false;
			}
		} else {
			std::cout << " -> is not used" << std::endl;
			return false;
		}
	} else {
		std::cout << " { operator is not used" << std::endl;
	}
	return flag;
}		



bool syntaxWhileIf (Lexer& x){
	std::vector<Token> res = x.getTokenTB();
	int size = res.size();
	bool flag = false;

	if ((res[0].getValue() == "while" || res[0].getValue() == "if") && res[size - 1].getValue() == "{"){
		int i = 1;
		if (res[i].getValue() == "->"){
			++i;
			if (res[i].getName() == "identifier"){
				auto search = std::find(ids.begin(), ids.end(), res[i].getValue());
				if (search != ids.end() && *search == res[i].getValue()){
					++i;
					if(res[i].getValue() == "equ" || res[i].getValue() == "notequ" || res[i].getValue() == "big" || res[i].getValue() == "small"){
						++i;
						if (res[i].getName() == "identifier"){
							auto research = std::find(ids.begin(), ids.end(), res[i].getValue());
							if(*research == res[i].getValue()){
								++i;
								if(i == size - 1){
									flag = true;
								} else {
									std::cout << "used more often or less symbols" << std::endl;
									return false;
								}
							} else {
								std::cout << "assigning a value from a non-existent variable" << std::endl;
								return false;
							}
						} else if (res[i].getName() == "value" && (std::regex_match(res[i].getValue(), valNum) || std::regex_match(res[i].getValue(), valDot) || std::regex_match(res[i].getValue(), valWord))){
							++i;
							if (i == size - 1){
								flag = true;
							} else {
								std::cout << "used more often or less symbols" << std::endl;
								return false;
							}
						} else {
							std::cout << "chi karox linel hamematutyun, ete tiv, bar kam popoxakan che" << std::endl;
							return false;
						}
					} else {
						std::cout << " absense of operation" << std::endl; // gorcoxutyan bacakayutyun
					}
				} else {
					std::cout << "assigning a value from a nonexistent variable" << std::endl;
					return false;
				}
			} else if (res[i].getName() == "value" && (std::regex_match(res[i].getValue(), valNum) || std::regex_match(res[i].getValue(), valDot) || std::regex_match(res[i].getValue(), valWord))){
				++i;
				if(res[i].getValue() == "equ" || res[i].getValue() == "notequ" || res[i].getValue() == "big" || res[i].getValue() == "small"){
						++i;
						if (res[i].getName() == "identifier"){
							auto research = std::find(ids.begin(), ids.end(), res[i].getValue());
                                                        if(research != ids.end() && *research == res[i].getValue()){
                                                             ++i;
                                                             if(i == size - 1){
                                                                  flag = true;
                                                              } else {
                                                                  std::cout << "ogtagorcvac en havelyal kam pakas simvolner" << std::endl;
                                                                  return false;
                                                              }
                                                         } else {
                                                                 std::cout << "assigning a value from a nonexistent variable" << std::endl;
                                                                 return false;
                                                         }
                                                 } else if (res[i].getName() == "value" && (std::regex_match(res[i].getValue(), valNum) || std::regex_match(res[i].getValue(), valDot) || std::regex_match(res[i].getValue(), valWord))){
                                                         ++i;
                                                         if (i == size - 1){
                                                                 flag = true;
                                                         } else {
                                                                 std::cout << "ogtagorcvac en havelyal kam pakas simvolner" << std::endl;
                                                                 return false;
                                                         }
                                                 } else {
                                                         std::cout << "chi karox linel hamematutyun, ete tiv, bar kam popoxakan che" << std::endl;
                                                         return false;
                                                 }
                                         } else {
                                                 std::cout << "gorcoxutyan bacakayutyun" << std::endl;
                                         }
                       } else {
                                 std::cout << "chi karox linel hamematutyun, ete tiv, bar kam popoxakan che" << std::endl;
                                 return false;
                       }
		} else {
			std::cout << "ogtagorcvac che -> operator@" << std::endl;
			return false;
		}
	} else {
		std::cout << "ogtagorcvac che { operator@" << std::endl;
		return false;
	}

	return flag;
}

		
bool syntaxElse(Lexer& x){
	std::vector<Token>  res = x.getTokenTB();
	bool flag = false;
	int size = res.size();
	if (size == 3 && res[0].getValue() == "}" && res[1].getValue() == "else" && res[2].getValue() == "{"){
		flag = true;
	} else {
		std::cout << "ogtagorcvac che '{' " << std::endl;
	}

	return flag;
}

	

bool syntaxAnalize(Lexer& x){
	std::vector<Token> res = x.getTokenTB();
	bool flag = false;
	if (res[0].getName() == "type"){
		flag = syntaxType(x);
	} else if (res[0].getName() == "identifier"){
		flag = syntaxID(x);
	} else if (res[0].getValue() == "for"){
		flag = syntaxFor(x);
	} else if (res[0].getValue() == "while" || res[0].getValue() == "if"){
		flag = syntaxWhileIf(x);
	} else if (res.size() > 1 && res[0].getValue() == "}"){
		flag = syntaxElse(x);
	} else if (res.size() == 1 && res[0].getValue() == "}"){
		flag = true;
	} else {
		std::cout << "tox@ sxal e sksvum" << std::endl;
		return false;
	}

	return flag;
}


// int main () {
// 	bool flag = true;
// 	std::string str;
// 	std::stack<char> s;
// 	std::cout << "Enter the file name" << std::endl;
// 	std::cin >> str;
// 	std::ifstream read;
// 	read.open(str);	
	
// 	if (!read.is_open()){
// 		throw std::invalid_argument("Error file isn't open");
// 	}
	
// while (std::getline(read, str)) {
//         for (char ch : str) {
//             if (ch == '(' || ch == '{') {
//                 s.push(ch);
//             } else if (ch == ')' || ch == '}') {
//                 if (s.empty() || (ch == ')' && s.top() != '(') || (ch == '}' && s.top() != '{')) {
//                     flag = false;
//                     return 0;
//                 } else {
//                     s.pop();
//                 }
//             }
//         }
//     }

//     if (!s.empty()) {
//         flag = false;
//     }	
// 	read.clear();
// 	read.seekg(0, std::ios::beg);
	
// 	if (flag){
// 		while (std::getline(read, str)){
// 			if (str.empty()){
// 				continue;
// 			}
// 			std::vector<std::string> tmp = tokenisation(str);
// 			Lexer x = tokensInfo(tmp);
// 			std::vector<Token> result = x.getTokenTB();
		
// 			std::cout << syntaxAnalize(x) << std::endl;
// 		}

// 		return 0;
// 	} else {
// 		std::cout << "cragrum pakagcer@ sxal en trvac" << std::endl;
// 		throw std::invalid_argument("");
// 	}

// 	return 0;
// }		
	
int main() {
    bool flag = true;
    std::string str;
    std::stack<char> s;

    std::cout << "Enter the file name: ";
    std::cin >> str;

    std::ifstream read;
    read.open(str);

    try {
        if (!read.is_open()) {
            throw std::runtime_error("Error: Unable to open file.");
        }

        while (std::getline(read, str)) {
            for (char ch : str) {
                if (ch == '(' || ch == '{') {
                    s.push(ch);
                } else if (ch == ')' || ch == '}') {
                    if (s.empty() || (ch == ')' && s.top() != '(') || (ch == '}' && s.top() != '{')) {
                        flag = false;
                        break;
                    } else {
                        s.pop();
                    }
                }
            }

        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    if (!s.empty()) {
        flag = false;
    }

    read.close(); // Closing the file after reading

    if (flag) {
        std::cout << "File processed successfully." << std::endl;
    } else {
        std::cout << "Error , not allowed parentheses or braces." << std::endl;
        return 1;
    }

    return 0;
}
