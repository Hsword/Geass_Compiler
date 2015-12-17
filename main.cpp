#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <stack>
using namespace std;
//lexical analysis
enum Tag
{
    NUM=256,REAL,ID,AND,OR,EQ,NE,GE,LE,STR,TYPE,KEY,THEN,ELSE,IFEND,WHILE,DO,WHEND,FOR,FOR_CHECK,FOR_DO,FOR_JUMP,FOR_END,CONTINUE,BREAK,RET,SSP,SPP,CALL,RETAX,RETT,FUN_S,FUN_E
};
string Tag_Str[]={
    "NUM","REAL","ID","AND","OR","EQ","NE","GE","LE","STR","TYPE","KEY","THEN","ELSE","IFEND","WHILE","DO","WHEND","FOR","FOR_C","FOR_D","FOR_J","FOR_E","CON","BRE","RET","SSP","SPP","CALL","RETAX","RETT","FUN_S","FUN_E"
};
#define NUM_SIZE 4
#define REAL_SIZE 8
#define CHAR_SIZE 1
const int k_w_len=8;
const int t_w_len=4;
string keywords[k_w_len]={"if","else","for","while","do","break","continue","return"};
string typewords[t_w_len]={"int","real","char","const"};
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
string symbols=",;+-*/(){}[]^?:!~";
bool issymbol(char c)
{
    for(int i=0;i<symbols.length();i++)
    {
        if(c==symbols[i])
            return true;
    }
    return false;
}
bool lexical_error=false;
string error_message="";
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
            else cout<<endl;
    }
}

/* 递归子程序语法分析*/
bool synax_error=false;
bool semantic_error=false;
void Semantic_Error(string msg);

//符号表设计
int curpid;
struct Synb
{
    int tid;//token_index
    int typ;
    /*
    i r c a
    1 2 3 4
    */
    int cat;
    /*
    c v f
    1 2 3
    */
    int addr;
    int pid;
    Synb(int tid)
    {
        this->tid=tid;
        this->pid=curpid;
        this->typ=this->cat=this->addr=0;
    }
};
int Data_Size(int typ)
{
    if(typ==1) return NUM_SIZE;
    else if(typ==2) return REAL_SIZE;
    else if(typ==3) return CHAR_SIZE;
    else return 0;
}
vector<Synb> Synbl;
int Tid_To_Sid(int tid);
vector<double> Consl;
int Vall=0;
struct Typeu
{
    int typ;
    vector<int> Tlen;
    /*eg:
    itp
    len1
    len2
    ...
    */
    Typeu(int typ)
    {
        this->typ=typ;
    }
};
vector<Typeu> Typel;
struct Declar_Unit
{
    int tid;
    vector<int> Array_Len_List;
    Declar_Unit(int tid)
    {
        this->tid=tid;
    }
    void Declar_Unit_Push(int len)
    {
        this->Array_Len_List.push_back(len);
    }
};

//四元式结构定义
struct Node{
    int Ein;   // 0: empty  1:identifier 2:num 3:arr 4.ret 5.fun
    int Addr;
    int Size;
    double Num;
    string Name;
};
// function list
struct Pfinf
{
    int Size;
    Node Ret;
};
vector<Pfinf> Pfinfl;
int mainpid;

void Synbl_Push_Name(int tid)
{
    for(int i=0;i<Synbl.size();i++)
    {
        if(Token_List[Synbl[i].tid]->get_lexeme_str()==Token_List[tid]->get_lexeme_str()&&Synbl[i].pid==curpid)
            return;
    }
    Synbl.push_back(Synb(tid));
    return ;
}
void Synbl_Push(Declar_Unit du,int typ,bool cat,bool arr,double val)   //typ:Type_Op   cat:Const_Type_Op   arr:Array_Op
{
    int tid=du.tid;
    for(int i=0;i<Synbl.size();i++)
    {
        if((Token_List[Synbl[i].tid]->get_lexeme_str()==Token_List[tid]->get_lexeme_str())&&Synbl[i].pid==curpid)
        {
            if(Synbl[i].typ!=0)
            {
                Semantic_Error("Variable redeclation confict!");
            }
            else
            {
                Synbl[i].pid=curpid;
                if(arr)
                {
                    Synbl[i].cat=2;
                    Synbl[i].typ=Typel.size();
                    Typel.push_back(Typeu(4));
                    Typel[Synbl[i].typ].Tlen.push_back(typ);
                    int sum=1;
                    for(int k=0;k<du.Array_Len_List.size();k++)
                    {
                        sum*=du.Array_Len_List[k];
                        Typel[Synbl[i].typ].Tlen.push_back(du.Array_Len_List[k]);
                    }
                    Synbl[i].addr=Vall;
                    Vall+=sum*Data_Size(typ);
                }
                else
                {
                    Synbl[i].typ=typ;
                    if(cat)
                    {
                        Synbl[i].cat=1;
                        Synbl[i].addr=Consl.size();
                        Consl.push_back(val);
                    }
                    else
                    {
                        Synbl[i].cat=2;
                        Synbl[i].addr=Vall;
                        Vall+=Data_Size(typ);
                    }
                }
            }
            return ;
        }
    }
}
int Synbl_Push_Fun_Typ(int tid,int typ)
{
    for(int i=0;i<Synbl.size();i++)
    {
        if(Token_List[Synbl[i].tid]->get_lexeme_str()==Token_List[tid]->get_lexeme_str())
        {
            if(Synbl[i].cat!=0)
            {
                Semantic_Error("Function redeclation confict!");
            }
            else
            {
                Synbl[i].cat=3;
                Synbl[i].tid=tid;
                Synbl[i].typ=typ;
                Synbl[i].pid=-1;
                Synbl[i].addr=Pfinfl.size();
                Pfinfl.push_back(Pfinf());
                return Synbl[i].addr;
            }

        }
    }
}
void Synbl_Push_Fun_Size(int tid,int Size)
{
    for(int i=0;i<Synbl.size();i++)
    {
        if(Token_List[Synbl[i].tid]->get_lexeme_str()==Token_List[tid]->get_lexeme_str())
        {
                Pfinfl[Synbl[i].addr].Size=Size;
                return ;
        }
    }
}
bool Const_Type_Op=false;
int Type_Op;
int Fun_Type_Op;
bool Array_Op=false;
bool Ret_Op=false;
void Show_Synbl()
{
    cout<<"Synbl:"<<endl;
    cout<<"name\ttyp\tcat\taddr\tpid\n";
    for(int i=0;i<Synbl.size();i++)
    {
        cout<<Token_List[Synbl[i].tid]->get_lexeme_str();
        cout<<"\t"<<Synbl[i].typ<<"\t"<<Synbl[i].cat<<"\t"<<Synbl[i].addr<<"\t"<<Synbl[i].pid<<endl;
    }
}

void Semantic_Error(string msg)
{
    semantic_error=true;
    error_message=msg;
    cout<<error_message<<endl;
    exit(0);
}



stack<Node> Sem;
stack<int> Syn;
Node Tid_To_Node(int tid);
Node Num_To_Node(int num);
struct Middle_Code_Unit{
    int Operator;//
    Node Target1;//为零表示空
    Node Target2;//为零表示为空
    Node Result;//为零表示为空，为-1表示此处为跳转地址
};
Middle_Code_Unit Get_Ssp(int pid)
{
    Middle_Code_Unit tmp;         //PUSH SP   ADD SP, VALL
    tmp.Operator=SSP;
    tmp.Target1.Ein=2;
    tmp.Target1.Addr=pid;
    tmp.Target1.Size=NUM_SIZE;
    tmp.Target2.Ein=0;
    tmp.Result.Ein=0;
    return tmp;
}
Middle_Code_Unit Get_Spp()
{
    Middle_Code_Unit tmp;
    tmp.Operator=SPP;        //POP SP
    tmp.Target1.Ein=0;
    tmp.Target2.Ein=0;
    tmp.Result.Ein=0;
    return tmp;
}
Middle_Code_Unit Call(int tid)
{
    Middle_Code_Unit tmp;
    tmp.Operator=CALL;
    tmp.Target1.Ein=5;
    tmp.Target1.Name=Token_List[tid]->get_lexeme_str();
    tmp.Target2.Ein=0;
    tmp.Result.Ein=0;
    return tmp;
}
Middle_Code_Unit Fun_Start(int tid)
{
    Middle_Code_Unit tmp;
    tmp.Operator=FUN_S;
    tmp.Target1.Ein=5;
    tmp.Target1.Name=Token_List[tid]->get_lexeme_str();
    tmp.Target2.Ein=0;
    tmp.Result.Ein=0;
    return tmp;
}
Middle_Code_Unit Fun_End(int tid)
{
    Middle_Code_Unit tmp;
    tmp.Operator=FUN_E;
    tmp.Target1.Ein=5;
    tmp.Target1.Name=Token_List[tid]->get_lexeme_str();
    tmp.Target2.Ein=0;
    tmp.Result.Ein=0;
    return tmp;
}
Middle_Code_Unit Get_Retax(Node node)
{
    Middle_Code_Unit tmp;
    tmp.Operator=RETAX;     //MOV AX,[]
    tmp.Target1=node;
    tmp.Target2.Ein=0;
    tmp.Result.Ein=0;
    return tmp;
}
Middle_Code_Unit Get_Ret()
{
    Middle_Code_Unit tmp;
    tmp.Operator=RET;           //RET
    tmp.Target1.Ein=0;
    tmp.Target2.Ein=0;
    tmp.Result.Ein=0;
    return tmp;
}
Middle_Code_Unit Get_Rett(int typ)
{
    Middle_Code_Unit tmp;
    tmp.Operator=RETT;        //MOV [],AX
    tmp.Target1.Ein=1;
    tmp.Target1.Size=Data_Size(typ);
    tmp.Target1.Addr=Vall;
    Vall+=tmp.Target1.Size;
    Sem.push(tmp.Target1);
    tmp.Target2.Ein=0;
    tmp.Result.Ein=0;
    return tmp;
}
vector<Middle_Code_Unit> Middle_Code;
void Show_Middle_Code()
{
    cout<<"Middle Code List:"<<endl;
    for(int i=0;i<Middle_Code.size();i++)
    {
        if(Middle_Code[i].Operator<256)
        {
            printf("%c",Middle_Code[i].Operator);
        }
        else
        {
            cout<<Tag_Str[Middle_Code[i].Operator-256];
        }
        cout<<"\t";
        if(Middle_Code[i].Target1.Ein==0)
        {
            cout<<" ";
        }
        else if(Middle_Code[i].Target1.Ein==1)
        {
            cout<<"["<<Middle_Code[i].Target1.Addr<<"]";
        }
        else if(Middle_Code[i].Target1.Ein==2)
        {
            if(Middle_Code[i].Target1.Size<REAL_SIZE)
                printf("%d",int(Middle_Code[i].Target1.Num));
            else
                printf("%.2f",Middle_Code[i].Target1.Num);
        }
        else if(Middle_Code[i].Target1.Ein==2)
        {
            cout<<"[["<<Middle_Code[i].Target1.Addr<<"]]";
        }
        else
        {
            cout<<Middle_Code[i].Target1.Name;
        }
        cout<<"\t";
        if(Middle_Code[i].Target2.Ein==0)
        {
            cout<<" ";
        }
        else if(Middle_Code[i].Target2.Ein==1)
        {
            cout<<"["<<Middle_Code[i].Target2.Addr<<"]";
        }
        else if(Middle_Code[i].Target2.Ein==2)
        {
            if(Middle_Code[i].Target2.Size<REAL_SIZE)
                printf("%d",int(Middle_Code[i].Target2.Num));
            else
                printf("%.2f",Middle_Code[i].Target2.Num);
        }
        else
        {
            cout<<"[["<<Middle_Code[i].Target2.Addr<<"]]";
        }
        cout<<"\t";
        if(Middle_Code[i].Result.Ein==0)
        {
            cout<<" ";
        }
        else if(Middle_Code[i].Result.Ein==1)
        {
            cout<<"["<<Middle_Code[i].Result.Addr<<"]";
        }
        else if(Middle_Code[i].Result.Ein==2)
        {
            if(Middle_Code[i].Result.Size<REAL_SIZE)
                printf("%d",int(Middle_Code[i].Result.Num));
            else
                printf("%.2f",Middle_Code[i].Result.Num);
        }
        else
        {
            cout<<"[["<<Middle_Code[i].Result.Addr<<"]]";
        }
        cout<<endl;
    }
}
vector< vector< Middle_Code_Unit> > Block;
//翻译文法

//动作函数声明
void init();
void Pop_Sem();
void Pop_Syn();
void Push_Sem(int tid);
void Push_Syn(int op);
void Quat();
void Quat_a();//array address transfrom
void Unary_Quat();
void Assign();
//if else
void Genth();
void Genel();
void Genie();
//while
void Genwh();
void Gendo();
void Genwe();
//for
void Genfr();
void Genfc();
void Genfd();
void Genfj();
void Genfe();

void Continue();
void Break();

int Tid_To_Sid(int tid)
{
    for(int i=0;i<Synbl.size();i++)
    {
        if(Token_List[tid]->get_lexeme_str()==Token_List[Synbl[i].tid]->get_lexeme_str())
        {
            return i;
        }
    }
    return -1;
}
Node Num_To_Node(int num)
{
    Node node;
    node.Ein=2;
    node.Size=NUM_SIZE;
    node.Num=num;
    return node;
}
Node Tid_To_Node(int tid)
{
    Node node;
    if(Token_List[tid]->get_tag()==NUM)
    {
        node.Ein=2;
        node.Num=Token_List[tid]->get_numvalue();
        node.Size=NUM_SIZE;
    }
    else if(Token_List[tid]->get_tag()==REAL)
    {
        node.Ein=2;
        node.Num=Token_List[tid]->get_realvalue();
        node.Size=REAL_SIZE;
    }
    else if(Token_List[tid]->get_tag()==ID)
    {
        bool is_found=false;
        for(int i=0;i<Synbl.size();i++)
        {
            if(Token_List[tid]->get_lexeme_str()==Token_List[Synbl[i].tid]->get_lexeme_str())
            {
                if(Synbl[i].typ>=4)
                {
                    ///数组
                    node.Size=Data_Size(Typel[Synbl[i].typ].Tlen[0]);
                    node.Ein=2;
                    node.Num=Synbl[i].addr;
                }
                else
                {
                    node.Size=Data_Size(Synbl[i].typ);
                    if(Synbl[i].cat==1)
                    {
                        node.Ein=2;
                        node.Num=Consl[Synbl[i].addr];
                    }
                    else
                    {
                        node.Ein=1;
                        node.Addr=Synbl[i].addr;
                    }
                }
                is_found=true;
            }
        }
        if(!is_found)
            Semantic_Error("Variable was not declared.");
    }
    return node;
}
void init()
{
    while(!Sem.empty())
        Pop_Sem();
    while(!Syn.empty())
        Syn.pop();
}
void Pop_Sem()
{
    //cout<<"pop sem"<<endl;
    Sem.pop();
}
void Pop_Syn()
{
    //cout<<"pop syn"<<endl;
    Syn.pop();
}
void Push_Sem(int tid)
{
    //cout<<"push sem"<<endl;
    Sem.push(Tid_To_Node(tid));
}
void Push_Syn(int op)
{
    //cout<<"push syn"<<endl;
    Syn.push(op);
}
void Quat()
{
    Middle_Code_Unit T;
    T.Operator=Syn.top();
    Pop_Syn();
    T.Target2=Sem.top();
    Pop_Sem();
    T.Target1=Sem.top();
    Pop_Sem();
    T.Result=Node();
    T.Result.Ein=1;
    T.Result.Addr=Vall;
    T.Result.Size=max(T.Target1.Size,T.Target2.Size);
    Vall+=T.Result.Size;
    Sem.push(T.Result);
    Middle_Code.push_back(T);
}
void Quat_a()
{
    Middle_Code_Unit T;
    T.Operator=Syn.top();
    Pop_Syn();
    T.Target2=Sem.top();
    Pop_Sem();
    T.Target1=Sem.top();
    Pop_Sem();
    T.Result=Node();
    T.Result.Ein=1;
    T.Result.Addr=Vall;
    T.Result.Size=max(T.Target1.Size,T.Target2.Size);
    Vall+=T.Result.Size;
    Middle_Code.push_back(T);
    T.Result.Ein=3;
    Sem.push(T.Result);
}
void Unary_Quat()
{
    Middle_Code_Unit T;
    T.Operator=Syn.top();
    Pop_Syn();
    T.Target1=Sem.top();
    Pop_Sem();
    T.Result=Node();
    T.Result.Ein=1;
    T.Result.Addr=Vall;
    T.Result.Size=T.Target1.Size;
    Vall+=T.Result.Size;
    Sem.push(T.Result);
    T.Target2=Node();
    T.Target2.Ein=0;
    Middle_Code.push_back(T);
}
void Assign()
{
    Middle_Code_Unit T;
    T.Operator=Syn.top();
    Pop_Syn();
    T.Target1=Sem.top();
    Pop_Sem();
    T.Result=Sem.top();
    Pop_Sem();
    T.Target2=Node();
    T.Target2.Ein=0;
    if(T.Result.Size<T.Target1.Size)
    {
        //Show_Synbl();
        //cout<<T.Result.Size<<T.Target1.Size<<endl;
        Semantic_Error("Type mismatch in assign expression!");
    }
    if(T.Result.Ein==0||T.Result.Ein==2)
    {
        Semantic_Error("Left value error!");
    }
    Middle_Code.push_back(T);
}
//if else
void Genth()
{
    Middle_Code_Unit T;
    T.Operator=THEN;
    T.Target1=Sem.top();
    Pop_Sem();
    T.Target2=Node();
    T.Target2.Ein=0;
    T.Result=Node();
    T.Result.Ein=0;
    Middle_Code.push_back(T);
}
void Genel()
{
    Middle_Code_Unit T;
    T.Operator=ELSE;
    T.Target1=Node();
    T.Target1.Ein=0;
    T.Target2=Node();
    T.Target2.Ein=0;
    T.Result=Node();
    T.Result.Ein=0;
    Middle_Code.push_back(T);
}
void Genie()
{
    Middle_Code_Unit T;
    T.Operator=IFEND;
    T.Target1=Node();
    T.Target1.Ein=0;
    T.Target2=Node();
    T.Target2.Ein=0;
    T.Result=Node();
    T.Result.Ein=0;
    Middle_Code.push_back(T);
}
//while
void Genwh()
{
    Middle_Code_Unit T;
    T.Operator=WHILE;
    T.Target1=Node();
    T.Target1.Ein=0;
    T.Target2=Node();
    T.Target2.Ein=0;
    T.Result=Node();
    T.Result.Ein=0;
    Middle_Code.push_back(T);
}
void Gendo()
{
    Middle_Code_Unit T;
    T.Operator=DO;
    T.Target1=Node();
    T.Target1.Ein=0;
    T.Target2=Node();
    T.Target2.Ein=0;
    T.Result=Node();
    T.Result.Ein=0;
    Middle_Code.push_back(T);
}
void Genwe()
{
    Middle_Code_Unit T;
    T.Operator=WHEND;
    T.Target1=Node();
    T.Target1.Ein=0;
    T.Target2=Node();
    T.Target2.Ein=0;
    T.Result=Node();
    T.Result.Ein=0;
    Middle_Code.push_back(T);
}
//for
void Genfr()
{
    Middle_Code_Unit T;
    T.Operator=FOR;
    T.Target1=Node();
    T.Target1.Ein=0;
    T.Target2=Node();
    T.Target2.Ein=0;
    T.Result=Node();
    T.Result.Ein=0;
    Middle_Code.push_back(T);
}
void Genfc()
{
    Middle_Code_Unit T;
    T.Operator=FOR_CHECK;
    T.Target1=Sem.top();
    Pop_Sem();
    T.Target2=Node();
    T.Target2.Ein=0;
    T.Result=Node();
    T.Result.Ein=0;
    Middle_Code.push_back(T);
}
void Genfd()
{
    Middle_Code_Unit T;
    T.Operator=FOR_DO;
    T.Target1=Node();
    T.Target1.Ein=0;
    T.Target2=Node();
    T.Target2.Ein=0;
    T.Result=Node();
    T.Result.Ein=0;
    Middle_Code.push_back(T);
}
void Genfj()
{
    Middle_Code_Unit T;
    T.Operator=FOR_JUMP;
    T.Target1=Node();
    T.Target1.Ein=0;
    T.Target2=Node();
    T.Target2.Ein=0;
    T.Result=Node();
    T.Result.Ein=0;
    Middle_Code.push_back(T);
}
void Genfe()
{
    Middle_Code_Unit T;
    T.Operator=FOR_END;
    T.Target1=Node();
    T.Target1.Ein=0;
    T.Target2=Node();
    T.Target2.Ein=0;
    T.Result=Node();
    T.Result.Ein=0;
    Middle_Code.push_back(T);
}

void Continue()
{
    Middle_Code_Unit T;
    T.Operator=CONTINUE;
    T.Target1=Node();
    T.Target1.Ein=0;
    T.Target2=Node();
    T.Target2.Ein=0;
    T.Result=Node();
    T.Result.Ein=0;
    Middle_Code.push_back(T);
}
void Break()
{
    Middle_Code_Unit T;
    T.Operator=BREAK;
    T.Target1=Node();
    T.Target1.Ein=0;
    T.Target2=Node();
    T.Target2.Ein=0;
    T.Result=Node();
    T.Result.Ein=0;
    Middle_Code.push_back(T);
}



//by 何雪
int Token_List_Index;
void Conditional_Expression();
void Constant_Expression();
void Logical_Or_Expression();
void Expression();
void Logical_And_Expression();
void Inclusive_Or_Expression();
void Exclusive_Or_Expression();
void And_Expression();
void Equality_Expression();
void Relational_Expression();
void Additive_Expression();
void Multiplicative_Expression();
void Unary_Expression();
void Primary_Expression();
void Assignment_Expression();
//
double Constant();
double Real_Constant();
int Integer_Constant();
//by 王千阁
int TokenList_Index;
void Assignment_Operator();
void Unary_Operator();
void Type_Name();
int Type_Specifier();
Declar_Unit Declarator();
void Declaration();
void Init_Declarator();
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
void Return_Statement();
void Oringinal_Statements();
int Function_Call_Expression();

int Identifier()
 {
    if(Token_List[Token_List_Index]->get_tag()!=ID)
        synax_error=true;
    Token_List_Index++;
    return Token_List_Index-1;
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
        Genth();
        Expression();
        if(Token_List[Token_List_Index]->get_tag()==':')
        {
            Token_List_Index++;
            Genel();
            Conditional_Expression();
            Genie();
        }
        else
            synax_error=true;
    }
}

void Logical_Or_Expression()
{
    Logical_And_Expression();
    while(Token_List[Token_List_Index]->get_tag()==OR)
    {
        Push_Syn(OR);
        Token_List_Index++;
        Logical_And_Expression();
        Quat();
    }
}

void Logical_And_Expression()
{
    Inclusive_Or_Expression();
    while(Token_List[Token_List_Index]->get_tag()==AND)
    {
        Push_Syn(AND);
        Token_List_Index++;
        Inclusive_Or_Expression();
        Quat();
    }
}

void Inclusive_Or_Expression()
{
    Exclusive_Or_Expression();
    while(Token_List[Token_List_Index]->get_tag()=='|')
    {
        Push_Syn('|');
        Token_List_Index++;
        Exclusive_Or_Expression();
        Quat();
    }
}

void Exclusive_Or_Expression()
{
    And_Expression();
    while(Token_List[Token_List_Index]->get_tag()=='^')
    {
        Push_Syn('^');
        Token_List_Index++;
        And_Expression();
        Quat();
    }
}

void And_Expression()
{
    Equality_Expression();
    while(Token_List[Token_List_Index]->get_tag()=='&')
    {
        Push_Syn('&');
        Token_List_Index++;
        Equality_Expression();
        Quat();
    }
}

void Equality_Expression()
{
    Relational_Expression();
    while(Token_List[Token_List_Index]->get_tag()==EQ || Token_List[Token_List_Index]->get_tag()==NE)
    {
        Push_Syn(Token_List[Token_List_Index]->get_tag());
        Token_List_Index++;
        Relational_Expression();
        Quat();
    }
}

void Relational_Expression()
{
    Additive_Expression();
    while(Token_List[Token_List_Index]->get_tag()==LE || Token_List[Token_List_Index]->get_tag()==GE || Token_List[Token_List_Index]->get_tag()=='>' || Token_List[Token_List_Index]->get_tag()=='<')
    {
        Push_Syn(Token_List[Token_List_Index]->get_tag());
        Token_List_Index++;
        Additive_Expression();
        Quat();
    }
}

void Additive_Expression()
{
    Multiplicative_Expression();
    while(Token_List[Token_List_Index]->get_tag()=='+' || Token_List[Token_List_Index]->get_tag()=='-')
    {
        Push_Syn(Token_List[Token_List_Index]->get_tag());
        Token_List_Index++;
        Multiplicative_Expression();
        Quat();
    }
}

void Multiplicative_Expression()
{
    Unary_Expression();
    while(Token_List[Token_List_Index]->get_tag()=='*' || Token_List[Token_List_Index]->get_tag()=='/' || Token_List[Token_List_Index]->get_tag()=='%')
    {
        Push_Syn(Token_List[Token_List_Index]->get_tag());
        Token_List_Index++;
        Unary_Expression();
        Quat();
    }
}

void Unary_Expression()
{
    if(Token_List[Token_List_Index]->get_tag()=='-' || Token_List[Token_List_Index]->get_tag()=='~' || Token_List[Token_List_Index]->get_tag()=='!')
    {
        Push_Syn(Token_List[Token_List_Index]->get_tag());
        Unary_Operator();
        Unary_Expression();
        Unary_Quat();
    }
    else if(Token_List[Token_List_Index]->get_tag()=='(')
    {
        Token_List_Index++;
        Constant_Expression();
        if(Token_List[Token_List_Index]->get_tag()==')')
            Token_List_Index++;
        else
            synax_error=true;
    }
    else if(Token_List[Token_List_Index]->get_tag()==ID)
    {
        if(Token_List_Index+1==Token_List.size())
        {
            synax_error=true;
        }
        else if(Token_List[Token_List_Index+1]->get_tag()=='(')
        {
            int typ=Function_Call_Expression();
            Middle_Code.push_back(Get_Rett(typ));
        }
        else
        {
            Primary_Expression();
        }
    }
    else
    {
        Constant();
    }
}

void Primary_Expression()
{
    if(Token_List[Token_List_Index]->get_tag()==ID)
    {
        Push_Sem(Token_List_Index);
        Token_List_Index++;
        if(Token_List[Token_List_Index]->get_tag()=='[')
        {
            int sid=Tid_To_Sid(Token_List_Index-1);
            if(sid==-1)
                    Semantic_Error("Variable was not declared.");
             int tpid=Synbl[sid].typ;
            bool first_level=true;
            int k=2;
            while(Token_List[Token_List_Index]->get_tag()=='[')
            {
                Push_Syn('+');
                Token_List_Index++;
                Constant_Expression();
                if(first_level)
                    first_level=false;
                else
                    Quat();
                if(Token_List[Token_List_Index]->get_tag()==']')
                {
                    Token_List_Index++;
                    if(k<Typel[tpid].Tlen.size())
                    {
                        Push_Syn('*');
                        Sem.push(Num_To_Node(Typel[tpid].Tlen[k]));
                        k++;
                    }
                    if(k!=Typel[tpid].Tlen.size())
                        Quat();
                    else
                    {
                        Push_Syn('*');
                        Sem.push(Num_To_Node(4));
                        Quat();
                        Quat_a();
                    }
                }
                else synax_error=true;
            }
            if(k!=Typel[tpid].Tlen.size())
                Semantic_Error("Array's dimension mismatch!");
        }
    }
}
double Constant()
{
    if(Token_List[Token_List_Index]->get_tag()==NUM )
        return Integer_Constant();
    else
        return Real_Constant();
}
int Integer_Constant()
{
    if(Token_List[Token_List_Index]->get_tag()!=NUM)
        synax_error=true;
    Push_Sem(Token_List_Index);
    Token_List_Index++;
    return Token_List[Token_List_Index-1]->get_numvalue();
}
double Real_Constant()
{
    if(Token_List[Token_List_Index]->get_tag()!=REAL)
        synax_error=true;
    Push_Sem(Token_List_Index);
    Token_List_Index++;
    return Token_List[Token_List_Index-1]->get_realvalue();
}

void Assignment_Expression()
{
    Primary_Expression();
    Assignment_Operator();
    Constant_Expression();
    Assign();
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
     {
        Push_Syn('=');
        Token_List_Index++;
     }
     else
     {
        synax_error=true;
     }
}
void Unary_Operator()
{
     if (Token_List[Token_List_Index]->get_tag()=='-')
        Token_List_Index++;
    else if (Token_List[Token_List_Index]->get_tag()=='~')
        Token_List_Index++;
    else if (Token_List[Token_List_Index]->get_tag()=='!')
        Token_List_Index++;
    else
    {
       synax_error=true;
    }
}
void Type_Name()
{
     if (Token_List[Token_List_Index]->get_tag()==TYPE&&Token_List[Token_List_Index]->get_lexeme_str()=="const")
     {
         Const_Type_Op=true;
         Token_List_Index++;
         Type_Specifier();
     }
     else
     {
        Type_Specifier();
     }
}
int Type_Specifier()
{
    if (Token_List[Token_List_Index]->get_lexeme_str()=="int")
    {
        Token_List_Index++;
        Type_Op=1;
    }
    else if (Token_List[Token_List_Index]->get_lexeme_str()=="real")
    {
        Token_List_Index++;
        Type_Op=2;
     }
     else if (Token_List[Token_List_Index]->get_lexeme_str()=="char")
     {
            Token_List_Index++;
            Type_Op=3;
     }
    else
    {
            synax_error=true;
    }
    return Type_Op;
}
Declar_Unit Declarator()
{
    Declar_Unit du(Identifier());
    if(Const_Type_Op&&Token_List[Token_List_Index]->get_tag()=='[')
    {
        Semantic_Error("const array is not allowed!");
    }
    while (Token_List[Token_List_Index]->get_tag()=='[')
    {
       Token_List_Index++;
       Array_Op=true;
       du.Declar_Unit_Push(Integer_Constant());
       if (Token_List[Token_List_Index]->get_tag()==']')
            Token_List_Index++;
       else
       {
            synax_error=true;
       }
   }
   return du;
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
    Const_Type_Op=false;
    if(Token_List[Token_List_Index]->get_tag()!=';')
        synax_error=true;
    else
        Token_List_Index++;
}
void Init_Declarator()
{
    bool is_assign=false;
    Declar_Unit du=Declarator();
    if(Const_Type_Op&&Token_List[Token_List_Index]->get_tag()!='=')
    {
        Semantic_Error("const init-declarator needs initializer.");
    }
    if(Array_Op&&Token_List[Token_List_Index]->get_tag()=='=')
    {
        Semantic_Error("array init-declarator has no initializer.");
    }
    double val;
    if (Token_List[Token_List_Index]->get_tag()=='=')
    {
        Token_List_Index++;
        val=Constant();
        is_assign=true;
        //Initializer();
    }
    Synbl_Push_Name(du.tid);
    Synbl_Push(du,Type_Op,Const_Type_Op,Array_Op,val);   //typ:Type_Op   cat:Const_Type_Op   arr:Array_Op
    Array_Op=false;
    if(is_assign&&!Const_Type_Op)
    {
        Node tmp=Sem.top();
        Sem.pop();
        Push_Sem(du.tid);
        Sem.push(tmp);
        Push_Syn('=');
        Assign();
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
            synax_error=true;
    }
    else if (Token_List[Token_List_Index]->get_tag()=='}')
    {
        Token_List_Index++;
    }else
   synax_error=true;


}
void Compound_Statement()
{
    if (Token_List[Token_List_Index]->get_tag()=='{')
        Token_List_Index++;
    else
        synax_error=true;
    while(Token_List[Token_List_Index]->get_tag()==TYPE)
    {
        Declaration();
    }
    while(Token_List[Token_List_Index]->get_tag()!='}')
        Statement();
    Token_List_Index++;
}
void Area_Statement()
{
    if (Token_List[Token_List_Index]->get_tag()=='{')
        Token_List_Index++;
    else
        synax_error=true;
    while(Token_List[Token_List_Index]->get_tag()!='}')
        Statement();
    Token_List_Index++;
}
void Area_Jump_Statement()
{
        if (Token_List[Token_List_Index]->get_tag()=='{')
        Token_List_Index++;
    else
        synax_error=true;
    while(Token_List[Token_List_Index]->get_tag()!='}')
        Jump_Statement();
    Token_List_Index++;
}
void Statement()
{
    if (Token_List[Token_List_Index]->get_tag()==KEY&&Token_List[Token_List_Index]->get_lexeme_str()=="return")
        Return_Statement();
    else if (Token_List[Token_List_Index]->get_tag()==KEY&&(Token_List[Token_List_Index]->get_lexeme_str()=="while"||Token_List[Token_List_Index]->get_lexeme_str()=="for"))
        Iteration_Statement();
    else if(Token_List[Token_List_Index]->get_tag()==KEY&&Token_List[Token_List_Index]->get_lexeme_str()=="if")
        Selection_Statement();
    else  if(Token_List[Token_List_Index]->get_tag()=='{')
        Area_Statement();
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
       synax_error=true;
    if (Token_List[Token_List_Index]->get_tag()=='(')
        Token_List_Index++;
    else
       synax_error=true;
    Constant_Expression();
    Genth();
    if (Token_List[Token_List_Index]->get_tag()==')')
        Token_List_Index++;
    else
       synax_error=true;
    Area_Statement();
    if(Token_List[Token_List_Index]->get_tag()==KEY&&Token_List[Token_List_Index]->get_lexeme_str()=="else")
    {
        Genel();
        Token_List_Index++;
        Area_Statement();
        Genie();
    }
}
void Selection_Jump_Statement()
{
    if(Token_List[Token_List_Index]->get_tag()==KEY&&Token_List[Token_List_Index]->get_lexeme_str()=="if")
        Token_List_Index++;
    else
       synax_error=true;
    if (Token_List[Token_List_Index]->get_tag()=='(')
        Token_List_Index++;
    else
       synax_error=true;
    Constant_Expression();
    Genth();
    if (Token_List[Token_List_Index]->get_tag()==')')
        Token_List_Index++;
    else
       synax_error=true;
    Area_Jump_Statement();
    if(Token_List[Token_List_Index]->get_tag()==KEY&&Token_List[Token_List_Index]->get_lexeme_str()=="else")
    {
        Genel();
        Token_List_Index++;
        Area_Jump_Statement();
        Genie();
    }
}
void Iteration_Statement()
{
    if (Token_List[Token_List_Index]->get_tag()==KEY&&Token_List[Token_List_Index]->get_lexeme_str()=="while")
    {
        Genwh();
        Token_List_Index++;
        if (Token_List[Token_List_Index]->get_tag()=='(')
        {
            Token_List_Index++;
        }
        else
        {
            synax_error=true;
        }
        Constant_Expression();
        Gendo();
        if (Token_List[Token_List_Index]->get_tag()!=')')
            synax_error=true;
        Token_List_Index++;
        Area_Jump_Statement();
        Genwe();
    }
    else if (Token_List[Token_List_Index]->get_tag()==KEY&&Token_List[Token_List_Index]->get_lexeme_str()=="for")
    {
        Token_List_Index++;
        if (Token_List[Token_List_Index]->get_tag()=='(')
            Token_List_Index++;
        else
            synax_error=true;
        if (Token_List[Token_List_Index]->get_tag()!=';')
            Expression();
        if (Token_List[Token_List_Index]->get_tag()==';')
            Token_List_Index++;
        else
            synax_error=true;
        Genfr();
        if (Token_List[Token_List_Index]->get_tag()!=';')
            Constant_Expression();
        if (Token_List[Token_List_Index]->get_tag()==';')
            Token_List_Index++;
        else
            synax_error=true;
        Genfc();
        Genfd();
        if (Token_List[Token_List_Index]->get_tag()!=')')
            Expression();
        if (Token_List[Token_List_Index]->get_tag()==')')
            Token_List_Index++;
        else
           synax_error=true;
        Genfj();
        Area_Jump_Statement();
        Genfe();
    }
}
void Jump_Statement()
{
    if (Token_List[Token_List_Index]->get_tag()==KEY&&Token_List[Token_List_Index]->get_lexeme_str()=="continue")
    {
            Token_List_Index++;
            if (Token_List[Token_List_Index]->get_tag()==';')
                Token_List_Index++;
            else synax_error=true;
            Continue();
    }
    else if (Token_List[Token_List_Index]->get_tag()==KEY&&Token_List[Token_List_Index]->get_lexeme_str()=="break")
    {
            Token_List_Index++;
            if (Token_List[Token_List_Index]->get_tag()==';')
                Token_List_Index++;
            else synax_error=true;
            Break();
    }
    else if (Token_List[Token_List_Index]->get_tag()==KEY&&(Token_List[Token_List_Index]->get_lexeme_str()=="while"||Token_List[Token_List_Index]->get_lexeme_str()=="for"))
        Iteration_Statement();
    else if (Token_List[Token_List_Index]->get_tag()==KEY&&Token_List[Token_List_Index]->get_lexeme_str()=="return")
        Return_Statement();
    else if(Token_List[Token_List_Index]->get_tag()==KEY&&Token_List[Token_List_Index]->get_lexeme_str()=="if")
        Selection_Statement();
    else  if(Token_List[Token_List_Index]->get_tag()=='{')
        Area_Jump_Statement();
    else
        Expression_Statement();
}

void Oringinal_Statements()
{
    mainpid=-1;
    curpid=-1;
    while(Token_List_Index<Token_List.size()&&Token_List[Token_List_Index]->get_tag()==TYPE)
    {
        Vall=0;
        Fun_Type_Op=Type_Specifier();
        int tid=Identifier();
        Middle_Code.push_back(Fun_Start(tid));
        Synbl_Push_Name(tid);
        curpid=Synbl_Push_Fun_Typ(tid,Type_Op);
        if(Token_List[tid]->get_lexeme_str()=="main")
            mainpid=curpid;
        if(Token_List[Token_List_Index]->get_tag()!='(')
            synax_error=true;
        else
            Token_List_Index++;
        if(Token_List_Index==Token_List.size())
            synax_error=true;
        //params
        if(Token_List[Token_List_Index]->get_tag()!=')')
            synax_error=true;
        else
            Token_List_Index++;
        if(Token_List_Index==Token_List.size())
            synax_error=true;
        Compound_Statement();
        Middle_Code.push_back(Fun_End(tid));
        Synbl_Push_Fun_Size(tid,Vall);
    }
    if(Token_List_Index<Token_List.size())
        synax_error=true;
    if(mainpid==-1)
        Semantic_Error("No main function!");
    for(int i=0;i<Middle_Code.size();i++)
    {
        if(Middle_Code[i].Operator==SSP)
        {
            Middle_Code[i].Target1.Num=Pfinfl[Middle_Code[i].Target1.Addr].Size;
        }
    }
}
int Function_Call_Expression()
{
    //SSP SIZE
    int tid=Identifier();
    int sid=Tid_To_Sid(tid);
    if(sid==-1)
        Semantic_Error("Undeclared function!");
    Middle_Code.push_back(Get_Ssp(curpid));
    if(Token_List[Token_List_Index]->get_tag()!='(')
        synax_error=true;
    else
        Token_List_Index++;
    if(Token_List_Index==Token_List.size())
        synax_error=true;
    //params
    if(Token_List[Token_List_Index]->get_tag()!=')')
        synax_error=true;
    else
        Token_List_Index++;
    if(Token_List_Index==Token_List.size())
        synax_error=true;
    //CALL
    Middle_Code.push_back(Call(tid));
    //SPP
    Middle_Code.push_back(Get_Spp());
    return Synbl[sid].typ;
}
void Return_Statement()
{
    Token_List_Index++;
    Constant_Expression();
    Node tmp=Sem.top();
    Sem.pop();
    if(tmp.Size>Data_Size(Fun_Type_Op))
        Semantic_Error("Function return type mismatch!");
    Middle_Code.push_back(Get_Retax(tmp));
    Middle_Code.push_back(Get_Ret());
    if(Token_List[Token_List_Index]->get_tag()!=';')
        synax_error=true;
    Token_List_Index++;
}
int main()
{
    freopen("test.in","r",stdin);
    freopen("test.out","w",stdout);

    while(getline(cin,tmp))
    {
        buffer=buffer+tmp+"\n";
    }
    cout<<buffer<<endl;
    int buffer_size=buffer.length();
    int lexemeBegin=0,forword=0;
    while(lexemeBegin<buffer_size&&!lexical_error)
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
                {
                    Token_List.push_back(new Word(ID,get_idindex(tmp)));
                    //Synbl_Push_Name(Token_List.size()-1);
                }
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
                        lexical_error=true;
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
                                    lexical_error=true;
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
                                            lexical_error=true;
                                        else
                                        {
                                            c=buffer[forword];
                                            ord=c-'0';
                                        }
                                    }
                                    else if(!isdigit(c))
                                    {
                                        lexical_error=true;
                                    }
                                    else
                                    {
                                        ord=c-'0';
                                    }
                                    if(!lexical_error)
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
                        lexical_error=true;
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
                                lexical_error=true;
                            else
                            {
                                c=buffer[forword];
                                ord=c-'0';
                            }
                        }
                        else if(!isdigit(c))
                        {
                            lexical_error=true;
                        }
                        else
                        {
                            ord=c-'0';
                        }
                        if(!lexical_error)
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
                lexical_error=true;
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
                    lexical_error=true;
                    break;
                }
                c=buffer[forword];
            }while(buffer[forword]!='"');
            if(!lexical_error)
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
        else if(c=='&')
        {
            if((forword+1>=buffer_size)||(buffer[forword+1]!='&'))
            {
                Token_List.push_back(new Token(c));
                forword++;
                lexemeBegin++;
            }
            else
            {
                Token_List.push_back(new Token(AND));
                forword+=2;
                lexemeBegin+=2;
            }
        }
        else if(c=='|')
        {
            if((forword+1>=buffer_size)||(buffer[forword+1]!='|'))
            {
                Token_List.push_back(new Token(c));
                forword++;
                lexemeBegin++;
            }
            else
            {
                Token_List.push_back(new Token(OR));
                forword+=2;
                lexemeBegin+=2;
            }
        }
    }
    if(lexical_error)
    {
        error_message="Lexical analysis error!";
        cout<<error_message<<endl;
    }
    else
    {
        show_Token_List();
        cout<<"Lexical analysis accepted!"<<endl;

        //语法分析开始
        error_message="Synax analysis error!";
        Token_List_Index=0;
        /* Init
        */
        init();
        for(int i=0;i<=3;i++)
            Typel.push_back(Typeu(i));
        //Compound_Statement();
        Oringinal_Statements();
        if(synax_error)
        {
            cout<<error_message<<endl;
        }
        else
        {
            cout<<"Synax analysis and Semantic analysis accepted!"<<endl;
            Show_Synbl();
            Show_Middle_Code();
        }
    }
    return 0;
}
