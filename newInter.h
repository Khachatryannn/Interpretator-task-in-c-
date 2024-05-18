#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <fstream>

class Interpretator {
    private:

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

    public:
    //Regex definitions

    bool isValue(std::string& str) {}
    bool iseExpr(std::string& str) {}

    std::vector<std::string> tokenisation (const std::string& a) {}
    Lexer tokensInfo(std::vector<std::string>& a) {}
    bool syntaxID(Lexer& x) {}
    bool syntaxType(Lexer& x) {}
    bool syntaxFor(Lexer& x) {}
    bool syntaxWhileIf(Lexer& x) {}
    bool syntaxElse(Lexer& x) {}

    bool syntaxAnalize(Lexer& x) {}


};