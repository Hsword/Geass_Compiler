#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include <cmath>
using namespace std;
enum Tag
{
    NUM=256,REAL,ID,MINUS,AND,OR,EQ,NE,GE,LE,STR,TYPE,KEY
};
const int k_w_len=8;
const int t_w_len=5;
string keywords[k_w_len]={"if","else","for","while","do","break","continue","return"};
string typewords[t_w_len]={"int","real","char","bool","const"};
vector<string> idwords;
vector<string> strwords;
int get_keyindex(string a)
{
    for(int i=0;i<k_w_len;i++)
    {
        if(keywords[i]==a)
            return i;
    }
    return -1;
}
int get_typeindex(string a)
{
    for(int i=0;i<t_w_len;i++)
    {
        if(typewords[i]==a)
            return i;
    }
    return -1;
}
int get_idindex(string a)
{
    int len=idwords.size();
    for(int i=0;i<len;i++)
    {
        if(idwords[i]==a)
            return i;
    }
    idwords.push_back(a);
    return len;
}
int get_strindex(string a)
{
    int len=strwords.size();
    for(int i=0;i<len;i++)
    {
        if(strwords[i]==a)
            return i;
    }
    strwords.push_back(a);
    return len;
}
class Token
{
    private:
        int tag;
    public:
        Token(){}
        Token(int i)
        {
            tag=i;
        }
        int get_tag()
        {
            return tag;
        }
        virtual int get_numvalue(){}
        virtual double get_realvalue(){}
        virtual int get_lexeme(){}
        virtual string get_lexeme_str(){}
};
class Num: public Token
{
    private:
        int value;
    public:
        Num(int t,int v):Token(t),value(v){};
        int get_numvalue()
        {
            return value;
        }
};
class Real: public Token
{
    private:
        double value;
    public:
        Real(int t,double v):Token(t),value(v){};
        double get_realvalue()
        {
            return value;
        }
};
class Word: public Token
{
    private:
        int lexeme;
    public:
        Word(int t,int v):Token(t),lexeme(v){};
        int get_lexeme()
        {
            return lexeme;
        }
        string get_lexeme_str()
        {
            if((this->get_tag())==ID) return idwords[lexeme];
            else if((this->get_tag())==TYPE) return typewords[lexeme];
            else if((this->get_tag())==KEY) return keywords[lexeme];
            else return strwords[lexeme];
        }
};
bool isletter(char c)
{
    if((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c=='_'))
        return true;
    else return false;
}
bool isdigit(char c)
{
    if(c>='0'&&c<='9')
        return true;
    else return false;
}
string symbols=",;+-*/(){}[]";
bool issymbol(char c)
{
    for(int i=0;i<symbols.length();i++)
    {
        if(c==symbols[i])
            return true;
    }
    return false;
}
bool error=false;
string buffer="";
string tmp="";
vector<Token*> Token_List;
void show_Token_List()
{
    int len=Token_List.size();
        for(int i=0;i<len;i++)
        {
            int tmp=Token_List[i]->get_tag();
            cout<<tmp<<"\t";
            if(tmp<NUM)
            {
                cout<<char(tmp)<<endl;
            }
            else if(tmp==NUM)
            {
                cout<<Token_List[i]->get_numvalue()<<endl;
            }
            else if(tmp==REAL)
            {
                cout<<Token_List[i]->get_realvalue()<<endl;
            }
            else if(tmp==ID||tmp==STR||tmp==KEY||tmp==TYPE)
            {
                cout<<Token_List[i]->get_lexeme_str()<<endl;
            }
    }
}

/* 递归子程序语法分析*/
//by 何雪
int Token_List_Index;
void Conditional_Expression();
void Constant_Expression();
void Logical_Or_Expression();
void Expression();
void Logical_And_Expression();
void Inclusive_Or_Expression();
void And_Expression();
void Equality_Expression();
void Relational_Expression();
void Additive_Expression();
void Multiplicative_Expression();
void Unary_Expression();
void Primary_Expression();
void Assignment_Expression();
//by 王千阁
int TokenList_Index;
void Constant();
void Assignment_Operator();
void Unary_Operator();
void Type_Name();
void Type_Specifier();
void Declarator();
void Declarator_End();
void Declaration();
void Init_Declarator();
void Init_Declarator_End();
void Initializer();
void Initializer_List();
void Initializer_End();
void Compound_Statement();
void Statement();
void Expression_Statement();
void Selection_Statement();
void Iteration_Statement();
void Jump_Statement();
//
 void Identifier()
 {
    if(Token_List[Token_List_Index]->get_tag()!=ID)
        error=true;
    Token_List_Index++;
 }


void Constant_Expression()
{
    Conditional_Expression();
}

void Conditional_Expression()
{
    Logical_Or_Expression();
    while(Token_List[Token_List_Index]->get_tag()=='?')
    {
        Token_List_Index++;
        Expression();
        if(Token_List[Token_List_Index]->get_tag()==':')
        {
            Token_List_Index++;
            Conditional_Expression();
        }
        else
            error=true;
    }
}

void Logical_Or_Expression()
{
    Logical_And_Expression();
    while(Token_List[Token_List_Index]->get_tag()==OR)
    {
        Token_List_Index++;
        Logical_And_Expression();
    }
}

void Logical_And_Expression()
{
    Inclusive_Or_Expression();
    while(Token_List[Token_List_Index]->get_tag()==AND)
    {
        Token_List_Index++;
        Inclusive_Or_Expression();
    }
}

void Inclusive_Or_Expression()
{
    And_Expression();
    while(Token_List[Token_List_Index]->get_tag()=='^')
    {
        Token_List_Index++;
        And_Expression();
    }
}

void And_Expression()
{
    Equality_Expression();
    while(Token_List[Token_List_Index]->get_tag()=='&')
    {
        Token_List_Index++;
        Equality_Expression();
    }
}

void Equality_Expression()
{
    Relational_Expression();
    while(Token_List[Token_List_Index]->get_tag()==EQ || Token_List[Token_List_Index]->get_tag()==NE)
    {
        Token_List_Index++;
        Relational_Expression();
    }
}

void Relational_Expression()
{
    Additive_Expression();
    while(Token_List[Token_List_Index]->get_tag()==LE || Token_List[Token_List_Index]->get_tag()==GE || Token_List[Token_List_Index]->get_tag()=='>' || Token_List[Token_List_Index]->get_tag()=='<')
    {
        Token_List_Index++;
        Additive_Expression();
    }
}

void Additive_Expression()
{
    Multiplicative_Expression();
    while(Token_List[Token_List_Index]->get_tag()=='+' || Token_List[Token_List_Index]->get_tag()=='-')
    {
        Token_List_Index++;
        Multiplicative_Expression();
    }
}

void Multiplicative_Expression()
{
    Unary_Expression();
    while(Token_List[Token_List_Index]->get_tag()=='*' || Token_List[Token_List_Index]->get_tag()=='/' || Token_List[Token_List_Index]->get_tag()=='%')
    {
        Token_List_Index++;
        Unary_Expression();
    }
}

void Unary_Expression()
{
    if(Token_List[Token_List_Index]->get_tag()=='-' || Token_List[Token_List_Index]->get_tag()=='~' || Token_List[Token_List_Index]->get_tag()=='!')
    {
        Token_List_Index++;
        Unary_Expression();
    }
    else if(Token_List[Token_List_Index]->get_tag()=='(')
    {
        Token_List_Index++;
        Expression();
        if(Token_List[Token_List_Index]->get_tag()==')')
            Token_List_Index++;
        else
            error=true;
    }
    else if(Token_List[Token_List_Index]->get_tag()==ID)
    {
        Primary_Expression();
    }
    else if(Token_List[Token_List_Index]->get_tag()==NUM || Token_List[Token_List_Index]->get_tag()==REAL)
        Token_List_Index++;
    else
        error=true;
}

void Primary_Expression()
{
    if(Token_List[Token_List_Index]->get_tag()==ID)
    {
        Token_List_Index++;
        while(Token_List[Token_List_Index]->get_tag()=='[')
        {
            Token_List_Index++;
            Constant_Expression();
            if(Token_List[Token_List_Index]->get_tag()==']')
                Token_List_Index++;
            else error=true;
        }
    }
}


void Assignment_Expression()
{
    Primary_Expression();
    Assignment_Operator();
    Conditional_Expression();
}






void Constant()
{
     if (Token_List[Token_List_Index]->get_tag()==NUM)
        {
         Token_List_Index++;
        }
        if (Token_List[Token_List_Index]->get_tag()==REAL)
        {
         Token_List_Index++;
        }

}
void Expression()
{
    Assignment_Expression();
     while  (Token_List[Token_List_Index]->get_tag()==',')
        {
            Token_List_Index++;
            Assignment_Expression();
        }
}
void Assignment_Operator()
{
     if (Token_List[Token_List_Index]->get_tag()=='=')
        Token_List_Index++;
     else
     {
        error=true;
     }
}
void Unary_Operator()
{
     if (Token_List[Token_List_Index]->get_tag()=='-')
        Token_List_Index++;
    if (Token_List[Token_List_Index]->get_tag()=='~')
        Token_List_Index++;
    if (Token_List[Token_List_Index]->get_tag()=='!')
        Token_List_Index++;
    else
    {
       error=true;

    }
}
void Type_Name()
{
     if (Token_List[Token_List_Index]->get_tag()==TYPE&&Token_List[Token_List_Index]->get_lexeme_str()=="const")
     {
         Token_List_Index++;
         Type_Specifier();
     }
     else
     {
        Type_Specifier();
     }
}
void Type_Specifier()
{
     if (Token_List[Token_List_Index]->get_lexeme_str()=="int")
     Token_List_Index++;
        else if (Token_List[Token_List_Index]->get_lexeme_str()=="char")
          Token_List_Index++;
         else if (Token_List[Token_List_Index]->get_lexeme_str()=="real")
            Token_List_Index++;
          else
            {
                error=true;

            }
}
void Declarator()
{
    if (Token_List[Token_List_Index]->get_tag()=='(')
    {
        Token_List_Index++;
        Declarator();
        if (Token_List[Token_List_Index]->get_tag()==')')
            Token_List_Index++;
        else{
                error=true;

            }
    }
    else
    {
        Identifier();
        Declarator_End();
    }
}
void Declarator_End()
{
   while (Token_List[Token_List_Index]->get_tag()=='[')
   {
       Token_List_Index++;
       Constant_Expression();
       if (Token_List[Token_List_Index]->get_tag()==']')
            Token_List_Index++;
       else
       {
            error=true;

       }

   }
}
void Declaration()
{
    Type_Name();
    Init_Declarator();
    while (Token_List[Token_List_Index]->get_tag()==',')
    {
        Token_List_Index++;
        Init_Declarator();
    }
    if(Token_List[Token_List_Index]->get_tag()!=';')
        error=true;
    else
        Token_List_Index++;
}
void Init_Declarator()
{
    Declarator();
    Init_Declarator_End();
}
void Init_Declarator_End()
{
    if (Token_List[Token_List_Index]->get_tag()=='=')
    {
        Token_List_Index++;
        Initializer();
    }
}
void Initializer()
{
      if (Token_List[Token_List_Index]->get_tag()=='{')
     {
         Token_List_Index++;
         Initializer_List();
         Initializer_End();
     }
     else Constant_Expression();
}
void Initializer_List()
{
    Initializer();
    while(Token_List[Token_List_Index]->get_tag()==',')
    {
        Token_List_Index++;
        Initializer_List();
    }
}
void Initializer_End()
{
    if (Token_List[Token_List_Index]->get_tag()==',')
    {
        Token_List_Index++;
        if (Token_List[Token_List_Index]->get_tag()=='}')
            Token_List_Index++;
        else
            error=true;
    }
    else if (Token_List[Token_List_Index]->get_tag()=='}')
    {
        Token_List_Index++;
    }else
   error=true;


}
void Compound_Statement()
{
    if (Token_List[Token_List_Index]->get_tag()=='{')
        Token_List_Index++;
    while(Token_List[Token_List_Index]->get_tag()==TYPE)
    {
        Declaration();
    }
    while(Token_List[Token_List_Index]->get_tag()!='}')
    Statement();
    if(Token_List[Token_List_Index]->get_tag()=='}')
        Token_List_Index++;
}
void Statement()
{
    if (Token_List[Token_List_Index]->get_tag()==KEY&&(Token_List[Token_List_Index]->get_lexeme_str()=="while"||Token_List[Token_List_Index]->get_lexeme_str()=="for"))
        Iteration_Statement();
    else if(Token_List[Token_List_Index]->get_tag()==KEY&&Token_List[Token_List_Index]->get_lexeme_str()=="if")
        Selection_Statement();
    else if(Token_List[Token_List_Index]->get_tag()==KEY&&Token_List[Token_List_Index]->get_lexeme_str()=="return")
        Token_List_Index++;
    else  if(Token_List[Token_List_Index]->get_tag()=='{')
        Compound_Statement();
    else
        Expression_Statement();
}
void Expression_Statement()
{
    if(Token_List[Token_List_Index]->get_tag()!=';')
        Expression();
    if(Token_List[Token_List_Index]->get_tag()==';')
        Token_List_Index++;
}
void Selection_Statement()
{
    if(Token_List[Token_List_Index]->get_tag()==KEY&&Token_List[Token_List_Index]->get_lexeme_str()=="if")
        Token_List_Index++;
    else
       error=true;
    if (Token_List[Token_List_Index]->get_tag()=='(')
        Token_List_Index++;
    else
       error=true;
    Constant_Expression();
    if (Token_List[Token_List_Index]->get_tag()==')')
        Token_List_Index++;
    else
       error=true;
    Statement();
    if(Token_List[Token_List_Index]->get_tag()==KEY&&Token_List[Token_List_Index]->get_lexeme_str()=="else")
    {
        Token_List_Index++;
        Statement();
    }
}
void Iteration_Statement()
{
    if (Token_List[Token_List_Index]->get_tag()==KEY&&Token_List[Token_List_Index]->get_lexeme_str()=="while")
    {
        Token_List_Index++;
        if (Token_List[Token_List_Index]->get_tag()=='(')
        {
            Token_List_Index++;
        }
        else
        {
            error=true;
        }
        Constant_Expression();
        if (Token_List[Token_List_Index]->get_tag()!=')')
            error=true;
        Token_List_Index++;
        Statement();
    }
    else if (Token_List[Token_List_Index]->get_tag()==KEY&&Token_List[Token_List_Index]->get_lexeme_str()=="for")
    {
        Token_List_Index++;
        if (Token_List[Token_List_Index]->get_tag()=='(')
            Token_List_Index++;
        else
            error=true;
        if (Token_List[Token_List_Index]->get_tag()!=';')
            Expression();
        if (Token_List[Token_List_Index]->get_tag()==';')
            Token_List_Index++;
        else
            error=true;
        if (Token_List[Token_List_Index]->get_tag()!=';')
            Constant_Expression();
        if (Token_List[Token_List_Index]->get_tag()==';')
            Token_List_Index++;
        else
            error=true;
        if (Token_List[Token_List_Index]->get_tag()!=')')
            Expression();
        if (Token_List[Token_List_Index]->get_tag()==')')
            Token_List_Index++;
        else
           error=true;
        Statement();
    }
}
void Jump_Statement()
{
    if (Token_List[Token_List_Index]->get_tag()==KEY&&Token_List[Token_List_Index]->get_lexeme_str()=="continue")
        {
            Token_List_Index++;
            if (Token_List[Token_List_Index]->get_tag()==';')
                Token_List_Index++;
            return;
        }
    else if (Token_List[Token_List_Index]->get_tag()==KEY&&Token_List[Token_List_Index]->get_lexeme_str()=="break")
        {
            Token_List_Index++;
            if (Token_List[Token_List_Index]->get_tag()==';')
                Token_List_Index++;
            return;
        }
    else
        Statement();
}

int main()
{
    freopen("test.in","r",stdin);

    while(getline(cin,tmp))
    {
        buffer=buffer+tmp+"\n";
    }
    cout<<buffer<<endl;
    int buffer_size=buffer.length();
    int lexemeBegin=0,forword=0;
    while(lexemeBegin<buffer_size&&!error)
    {
        char c=buffer[forword];
        //cout<<"c=="<<c<<endl;
        while(c==' '||c=='\n'||c=='\t')
        {
            forword++;
            lexemeBegin++;
            if(forword>=buffer_size||lexemeBegin>=buffer_size)
                break;
            c=buffer[forword];
        }
        if(c=='<'||c=='>'||c=='='||c=='!')
        {
            forword++;
            if(forword<buffer_size&&buffer[forword]=='=')
            {
                    switch (c)
                    {
                                case '<':
                                    Token_List.push_back(new Token(LE));
                                    break;
                                case '>':
                                    Token_List.push_back(new Token(GE));
                                    break;
                                case '!':
                                    Token_List.push_back(new Token(NE));
                                    break;
                                case '=':
                                    Token_List.push_back(new Token(EQ));
                                    break;
                    }
                    forword++;
                    lexemeBegin+=2;
            }
            else
            {
                    Token_List.push_back(new Token(c));
                    lexemeBegin++;
            }
        }
        else if(isletter(c))
        {
            while(true)
            {
                forword++;
                if(forword>=buffer_size) break;
                c=buffer[forword];
                if(!(isletter(c)||isdigit(c))) break;
            }
            //cout<<"tokenlis push back="<<buffer.substr(lexemeBegin,forword-lexemeBegin)<<endl;
            //cout<<"now buffer[forword] is "<<buffer[forword]<<endl;
            string tmp=buffer.substr(lexemeBegin,forword-lexemeBegin);
            int index=get_keyindex(tmp);
            if(index!=-1)  Token_List.push_back(new Word(KEY,index));
            else
            {
                index=get_typeindex(tmp);
                if(index!=-1)   Token_List.push_back(new Word(TYPE,index));
                else
                    Token_List.push_back(new Word(ID,get_idindex(tmp)));
            }
            lexemeBegin=forword;
        }
        else if(isdigit(c))
        {
            //number
            //cout<<"number="<<c<<endl;
            bool endofinput=false;
            double ans=c-'0';
            while(true)
            {
                forword++;
                if(forword>=buffer_size)
                {
                    endofinput=true;
                    break;
                }
                c=buffer[forword];
                if(!isdigit(c))
                    break;
                ans=ans*10+c-'0';
            }
            if(endofinput)
            {
                Token_List.push_back(new Num(NUM,ans));
                lexemeBegin=forword;
            }
            else
            {
                if(c=='.')
                {
                    forword++;
                    if(forword>=buffer_size||!isdigit(buffer[forword]))
                        error=true;
                    else
                    {
                        double w=10;
                        ans+=(buffer[forword]-'0')/w;
                        while(true)
                        {
                            forword++;
                            w*=10;
                            if(forword>=buffer_size)
                            {
                                endofinput=true;
                                break;
                            }
                            c=buffer[forword];
                            if(!isdigit(c))
                                break;
                            ans=ans+(c-'0')/w;
                        }
                        if(endofinput)
                        {
                            Token_List.push_back(new Real(REAL,ans));
                            lexemeBegin=forword;
                        }
                        else
                        {
                            if(c=='e'||c=='E')
                            {
                                double ord=0;
                                forword++;
                                if(forword>=buffer_size)
                                {
                                    error=true;
                                }
                                else
                                {
                                    c=buffer[forword];
                                    bool ispos=true;
                                    if(c=='+'||c=='-')
                                    {
                                        if(c=='-')  ispos=false;
                                        forword++;
                                        if(forword>=buffer_size||!isdigit(buffer[forword]))
                                            error=true;
                                        else
                                        {
                                            c=buffer[forword];
                                            ord=c-'0';
                                        }
                                    }
                                    else if(!isdigit(c))
                                    {
                                        error=true;
                                    }
                                    else
                                    {
                                        ord=c-'0';
                                    }
                                    if(!error)
                                    {
                                        while(true)
                                        {
                                             forword++;
                                             if(forword>=buffer_size)   break;
                                             c=buffer[forword];
                                             if(!isdigit(c)) break;
                                             ord=ord*10+c-'0';
                                        }
                                        if(!ispos) ord=-ord;
                                        ans=ans*pow(10,ord);
                                        Token_List.push_back(new Real(REAL,ans));
                                        lexemeBegin=forword;
                                    }
                                }
                            }
                            else
                            {
                                Token_List.push_back(new Real(REAL,ans));
                                lexemeBegin=forword;
                            }
                        }
                    }
                }
                else if(c=='e'||c=='E')
                {
                    double ord=0;
                    forword++;
                    if(forword>=buffer_size)
                    {
                        error=true;
                    }
                    else
                    {
                        c=buffer[forword];
                        bool ispos=true;
                        if(c=='+'||c=='-')
                        {
                            if(c=='-')  ispos=false;
                            forword++;
                            if(forword>=buffer_size||!isdigit(buffer[forword]))
                                error=true;
                            else
                            {
                                c=buffer[forword];
                                ord=c-'0';
                            }
                        }
                        else if(!isdigit(c))
                        {
                            error=true;
                        }
                        else
                        {
                            ord=c-'0';
                        }
                        if(!error)
                        {
                            while(true)
                            {
                                 forword++;
                                 if(forword>=buffer_size)   break;
                                 c=buffer[forword];
                                 if(!isdigit(c)) break;
                                 ord=ord*10+c-'0';
                            }
                            if(!ispos) ord=-ord;
                            ans=ans*pow(10,ord);
                            Token_List.push_back(new Real(REAL,ans));
                            lexemeBegin=forword;
                        }
                    }
                }
                else
                {
                    Token_List.push_back(new Num(NUM,int(ans)));
                    lexemeBegin=forword;
                }
            }
        }
        else if(c=='\'')
        {
            if((forword+2>=buffer_size)||(buffer[forword+2]!='\''))
            {
                error=true;
            }
            else
            {
                Token_List.push_back(new Num(NUM,buffer[forword+1]));
                forword+=3;
                lexemeBegin+=3;
            }
        }
        else if(c=='"')
        {
            do
            {
                forword++;
                if(forword>=buffer_size)
                {
                    error=true;
                    break;
                }
                c=buffer[forword];
            }while(buffer[forword]!='"');
            if(!error)
            {
                Token_List.push_back(new Word(STR,get_strindex(buffer.substr(lexemeBegin+1,forword-lexemeBegin-1))));
                forword++;
                lexemeBegin=forword;
            }
        }
        else if(issymbol(c))
        {
            Token_List.push_back(new Token(c));
            forword++;
            lexemeBegin++;
        }
    }
    if(error)
    {
        cout<<"Lexical analysis error!"<<endl;
    }
    else
    {
        show_Token_List();
        cout<<"Lexical analysis accepted!"<<endl;

        //语法分析开始
        Token_List_Index=0;
        Compound_Statement();
        if(error)
        {
            cout<<"Synax analysis error!"<<endl;
        }
        else
        {
            cout<<"Synax analysis accepted!"<<endl;
        }
    }
    return 0;
}
