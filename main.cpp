#include "scanner.h"
#include<sstream>
#include<stdio.h>
#include<string.h>

typedef enum {ProK,PheadK,DecK,TypeK,VarK,ProcDecK,StmLK,StmtK,ExpK} NodeKind;

typedef enum {ArrayK,CharK,IntegerK,RecordK,IdK}  DecKind;

typedef enum {IfK,WhileK,AssignK,ReadK,WriteK,CallK,ReturnK} StmtKind;

typedef enum {OpK,ConstK,VariK} ExpKind;

typedef enum {IdV,ArrayMembV,FieldMembV} VarKind; 

typedef enum {Void,Integer,Boolean} ExpType;

typedef enum {valparamType,varparamType} ParamType; 

//typedef LexType  TmlType;
int lineno;
int line1=0,line2=0;

struct Attr 
{
	struct
	{	
		int low;             
		int up;              
		DecKind   childtype; 
	} ArrayAttr;             
	
	struct
	{	
		ParamType  paramt;     
	} ProcAttr;               
	
	struct
	{
		LexType op;          						
		int val;		      
		VarKind  varkind;    
		ExpType type;        
	} ExpAttr;	             
	string type_name;                  
};                          
class TreeNode
{
public:
	TreeNode * child[3];
	TreeNode * sibling;  //兄弟节点
	int lineno;
	NodeKind nodekind;
	struct
	{
		DecKind dec;//nodekind=DecK时，为声明类型
		StmtKind stmt;//nodekind=StmtK时，为语句类型
		ExpKind exp;//nodekind=ExpK时，为表达式类型
	}kind;
	int idnum;
	string name[99];
	string * table[99];
	struct Attr attr;
	//string type_name;
};


class Tree
{
public:
	TreeNode * tree;
	TreeNode * newNode(NodeKind kind);
	TreeNode * newStmtNode(StmtKind kind);
	TreeNode * newExpNode(ExpKind kind);
	void printtree(TreeNode * root);
	void filetree(TreeNode * root);
	string temp_name;
	TreeNode * root;
	tokenNode *current;
public:
	TreeNode * parse(void);
	TreeNode * Program(void);  
	TreeNode * ProgramHead(void);
	TreeNode * DeclarePart(void);
	TreeNode * TypeDec(void);
	TreeNode * TypeDeclaration(void);
	TreeNode * TypeDecList(void);
	TreeNode * TypeDecMore(void);
	void  TypeId(TreeNode * t);
	void  TypeName(TreeNode * t);
	void  BaseType(TreeNode * t);
	void  StructureType(TreeNode * t);
	void  ArrayType(TreeNode * t);
	void  RecType(TreeNode * t);
	TreeNode * FieldDecList(void);
 
	TreeNode * FieldDecMore(void);
	void  IdList(TreeNode * t);
	void  IdMore(TreeNode * t);
	TreeNode * VarDec(void);
	TreeNode * VarDeclaration(void);
	TreeNode * VarDecList(void);
	TreeNode * VarDecMore(void);
	void  VarIdList(TreeNode * t);
	void  VarIdMore(TreeNode * t);
	TreeNode * ProcDec(void);
	TreeNode * ProcDeclaration(void);
	void  ParamList(TreeNode * t);
	TreeNode * ParamDecList(void);
	TreeNode * Param(void);
	TreeNode * ParamMore(void);
	void  FormList(TreeNode * t);
	void  FidMore(TreeNode * t);
	TreeNode * ProcDecPart(void);
	TreeNode * ProcBody(void);
	TreeNode * ProgramBody(void);
	TreeNode * StmList(void);
	TreeNode * StmMore(void);
	TreeNode * Stm(void);
	TreeNode * AssCall(void);
	TreeNode * AssignmentRest(void);
	TreeNode * ConditionalStm(void);
	TreeNode * LoopStm(void);
	TreeNode * InputStm(void);
	TreeNode * OutputStm(void);
	TreeNode * ReturnStm(void);
	TreeNode * CallStmRest(void);
	TreeNode * ActParamList(void);
	TreeNode * ActParamMore(void);
	TreeNode * Exp(void);
	TreeNode * Simple_exp(void);	
	TreeNode * Term(void);	
	TreeNode * Factor(void);
	TreeNode * Variable(void);
	void VariMore(TreeNode * t);
	TreeNode * Fieldvar(void);
	void FieldvarMore(TreeNode * t );
	void SyntaxError(char * message);
	void match(LexType expected);

//public:      
	//struct token1=new tokenNode();//
	
};
TreeNode * Tree::newNode(NodeKind kind)		
{
	TreeNode * t = new TreeNode();	
	int i;
	for (i=0;i<3;i++) 
		t->child[i] = NULL;	
	t->sibling = NULL; 
    t->nodekind=kind;
	t->lineno = lineno;
	t->idnum = 0;
	for(i=0;i<99;i++) 
	{
		t->name[i]="\0";
		t->table[i] = NULL;
	}		
	return t;
}

TreeNode * Tree::newStmtNode(StmtKind kind)
{
	TreeNode * t = new TreeNode();
	int i;	
	for (i=0;i<3;i++) 
		t->child[i] = NULL;
	t->sibling = NULL;	
	t->nodekind = StmtK;
	t->kind.stmt = kind;
	t->lineno = lineno;	
	t->idnum = 0;	
	for(i=0;i<99;i++) 
	{
		t->name[i]="\0";
		t->table[i] = NULL;
	}
	return t;
}
TreeNode * Tree::newExpNode(ExpKind kind)

{ 
	
	TreeNode * t = new TreeNode();	
	int i;
	for (i=0;i<3;i++) 
		t->child[i] = NULL;
	t->sibling = NULL;
	t->nodekind = ExpK;
	t->kind.exp = kind;
	t->lineno = lineno;
	t->attr.ExpAttr.varkind = IdV;
	t->attr.ExpAttr.type = Void;
	for(i=0;i<99;i++) 
	{
		t->name[i]="\0";
		t->table[i] = NULL;
	}
	return t;
}	
void Tree::printtree(TreeNode * root)
{   line1++;
	switch (root->nodekind)
	{   		
	case ProK : 
		cout<<"ProK";
		break;
	case PheadK:
		cout<<"PheadK  ";
		cout<<root->name[0];		
		break;
	case DecK:
		cout<<"DecK  ";	
		if (root->attr.ProcAttr.paramt==varparamType )
		{
			cout<<"var param:  ";		
		}
		if (root->attr.ProcAttr.paramt==valparamType)
		{
			cout<<"value param:  ";
			
		}
		switch(root->kind.dec)
		{ 
		case  ArrayK:			
			cout<<"ArrayK  ";
			cout<<root->attr.ArrayAttr.up<<"  ";
			cout<<root->attr.ArrayAttr.low;
			if (root->attr.ArrayAttr.childtype == CharK)
				cout<<"Chark  ";
			else if( root->attr.ArrayAttr.childtype == IntegerK)
				cout<<"IntegerK  ";
			break;
		case  CharK:
			cout<<"CharK  ";break;
		case  IntegerK:
			cout<<"IntegerK  ";break;
		case  RecordK:
			cout<<"RecordK  ";break;
		case  IdK:
			cout<<"IdK  ";
			cout<<root->attr.type_name;
			break;
		default: 
			cout<<"节点dec声明类型错误！行号为"<<line1;
		}
		if (root->idnum !=0)
			for (int i=0 ; i <= (root->idnum);i++)
			{
				cout<<root->name[i]<<"  ";		
			}
			else  
			{
				cout<<"变量类型有误,行号为"<<line1;
			}
			break;
	case TypeK:
		cout<<"TypeK  ";
		break;			
	case VarK:
		cout<<"VarK  ";
		break;
			
	case ProcDecK: 
		cout<<"ProcDecK  "<<root->name[0];			
		break;			
	case StmLK:
		cout<<"StmLk  ";break;			
		case StmtK:
			{ cout<<"StmtK  ";
			switch (root->kind.stmt)
			{ 
			case IfK:
				cout<<"If  ";break;
			case WhileK:
				cout<<"While  ";break;			
			case AssignK:
				cout<<"Assign  ";break;			
			case ReadK:
				cout<<"Read  ";
				cout<< root->name[0]<<"  ";				
				break;				
			case WriteK:
				cout<<"Write  ";break;				
			case CallK:
				cout<<"Call  ";
				cout<<root->name[0]<<"  ";
				break;				
			case ReturnK:
				cout<<"Return  ";break;				
			default: 
				cout<<"语法树语句类型节点错误！行号为"<<line1;	
			}
			};break;
	case ExpK: 
		{ 
		cout<<"ExpK  "<<endl;
		switch (root->kind.exp)
		{ 
			case OpK:
				{ 
				cout<<"Op  ";
				switch(root->attr.ExpAttr.op)
				{
				case EQ:   cout<< "="; break;
				case LT:   cout<<"<"; break;      
				case PLUS: cout<<"+"; break;   
				case MINUS:cout<<"-"; break;
				case TIMES:cout<<"*"; break;  
				case OVER: cout<<"/"; break;  
				default: 
					cout<<"语法树节点运算符单词错误！行号为"<<line1;
				}
						
				if(root->attr.ExpAttr.varkind==ArrayMembV)
				{
				cout<<"ArrayMember  ";
				cout<<root->name[0]<<"  ";
				}
				}
				break;
			case ConstK:
				cout<<"Const  ";
				switch(root->attr.ExpAttr.varkind)
				{
				case IdV:
					cout<<"Id  ";
					cout<<root->name[0]<<"  ";
					break;
				case FieldMembV:
					cout<<"FieldMember  ";
					cout<<root->name[0]<<"  ";
					break;
				case ArrayMembV:
					cout<<"ArrayMember  ";
					cout<<root->name[0]<<"  ";
					break;
				default: 
					cout<<"var变量错误！!行号为"<<line1;
				}					
					cout<<root->attr.ExpAttr.val<<"  ";
					break;
			case VariK:
				cout<<"Vari  ";
				switch(root->attr.ExpAttr.varkind)
				{
				case IdV:
					cout<<"Id  ";
					cout<<root->name[0]<<"  ";
					break;
				case FieldMembV:
					cout<<"FieldMember  ";
					cout<<root->name[0]<<"  ";
					break;
				case ArrayMembV:
					cout<<"ArrayMember  ";
					cout<<root->name[0]<<"  ";
					break;
				default: 
					cout<<"var变量错误！行号为"<<line1;
				}			
				break;
		default: 
				cout<<"语法树节点表达式类型错误！行号为"<<line1;
				}
			};break;
	default: 
		cout<<"语法树节点类型错误！行号为"<<line1;
		break;
   }  
   cout<<endl;
   for (int i=0;i<3;i++)
   {
	   if(root->child[i]!=NULL)
		   printtree(root->child[i]);  			
   }
   
   if(root->sibling!=NULL)
	   printtree( root->sibling);	
};

void Tree::filetree(TreeNode * root)
{   line2++;
	string out="yufafenxi.txt";
	ofstream outfile;
	outfile.open(out.c_str(),ofstream::app);
	if(!outfile)
	{
		cerr<<"error:文件打开失败！:"
			<<out<<endl;
		exit(0);
	}
	switch (root->nodekind)
	{   
		
	case ProK : 
		outfile<<"ProK";
		break;
	case PheadK:
		outfile<<"    PheadK  ";
		outfile<<root->name[0];	
		break;
	case DecK:
		outfile<<"        DecK  ";		
		if (root->attr.ProcAttr.paramt==varparamType )
		{
			outfile<<"var param:  ";
			
		}
		if (root->attr.ProcAttr.paramt==valparamType)
		{
			outfile<<"value param:  ";
			
		}
		switch(root->kind.dec)
		{ 
		case  ArrayK:		
			outfile<<"ArrayK  ";
			outfile<<root->attr.ArrayAttr.up<<"  ";
			outfile<<root->attr.ArrayAttr.low<<endl;
			if (root->attr.ArrayAttr.childtype == CharK)
				outfile<<"Chark  ";
			else if( root->attr.ArrayAttr.childtype == IntegerK)
				outfile<<"IntegerK  ";
			break;
		case  CharK:
			outfile<<"CharK  ";break;
		case  IntegerK:
			outfile<<"IntegerK  ";break;
		case  RecordK:
			outfile<<"RecordK  ";break;
		case  IdK:
			outfile<<"IdK  ";
			outfile<<root->attr.type_name<<"  ";
			break;
		default: 
			outfile<<"声明类型错误！行号为"<<line2;
		}
		if (root->idnum !=0)
			for (int i=0 ; i <= (root->idnum);i++)
			{
				outfile<<root->name[i]<<"  ";		
			}
			else  
			{
				outfile<<"变量类型有误！可能因为begin缺失！行号为"<<line2;
			}
			break;
		case TypeK:
			outfile<<"    TypeK  ";break;
			
		case VarK:
			outfile<<"    VarK  ";
			break;
			
		case ProcDecK: 
			outfile<<"    ProcDecK  "<<root->name[0];
			
			break;
			
		case StmLK:
			outfile<<"    StmLk  ";break;
			
		case StmtK:
			{ outfile<<"        StmtK  ";
			switch (root->kind.stmt)
			{ 
			case IfK:
				outfile<<"If  ";break;
			case WhileK:
				outfile<<"While  ";break;
				
			case AssignK:
				outfile<<"Assign  ";
				break;
				
			case ReadK:
				outfile<<"Read  ";
				outfile<< root->name[0]<<"  ";
				
				break;
				
			case WriteK:
				outfile<<"Write  ";break;
				
			case CallK:
				outfile<<"Call  ";
				outfile<<root->name[0]<<"  ";
				break;
				
			case ReturnK:
				outfile<<"Return  ";break;
				
			default: 
				outfile<<"语法树节点语句类型错误！行号为"<<line2;	
			}
			};break;
		case ExpK: 
			{ 
				
				switch (root->kind.exp)
				{ 
				case OpK:
					{   outfile<<"           ExpK  ";
						outfile<<"Op  ";
						switch(root->attr.ExpAttr.op)
						{ 
						case EQ:   outfile<< "="; break;
						case LT:   outfile<<"<"; break;      
						case PLUS: outfile<<"+"; break;   
						case MINUS:outfile<<"-"; break;
						case TIMES:outfile<<"*"; break;  
						case OVER: outfile<<"/"; break;  
						default: 
							outfile<<"运算符单词错误！行号为"<<line2;
			
						}
						
						if(root->attr.ExpAttr.varkind==ArrayMembV)
						{
							outfile<<"ArrayMember  ";
							outfile<<root->name[0]<<"  ";
						}
					};break;
				case ConstK:
					outfile<<"               ExpK  ";
					outfile<<"Const  ";
					switch(root->attr.ExpAttr.varkind)
					{
					case IdV:
						//outfile<<"Id  ";
						outfile<<root->name[0]<<"  ";
						break;
					case FieldMembV:
						outfile<<"FieldMember  ";
						outfile<<root->name[0]<<"  ";
						break;
					case ArrayMembV:
						outfile<<"ArrayMember  ";
						outfile<<root->name[0]<<"  ";
						break;
					default: 
						outfile<<"var type error!";
					}
					
					outfile<<root->attr.ExpAttr.val<<"  ";
					break;
				case VariK:
					 outfile<<"               ExpK  ";
						//outfile<<"Vari  ";
						switch(root->attr.ExpAttr.varkind)
						{
						case IdV:
							outfile<<root->name[0]<<"  ";
							outfile<<"IdV  ";
							break;
						case FieldMembV:
							outfile<<"FieldMemberV  ";
							outfile<<root->name[0]<<"  ";
							break;
						case ArrayMembV:
							outfile<<"ArrayMemberV  ";
							outfile<<root->name[0]<<"  ";
							break;
						default: 
							outfile<<"var类型错误!行号为"<<line2;
						}
						
						
						break;
						default: 
							outfile<<"变量类别错误！行号为"<<line2;
	
				}
			};break;
		default: 
			outfile<<"语法树节点表达式类型错误！行号为"<<line2;
			break;
   }
   
   outfile<<endl;
   outfile.close();
   for (int i=0;i<3;i++)
   {
	   if(root->child[i]!=NULL)
		   filetree(root->child[i]);  			
   }
   
   if(root->sibling!=NULL)
	   filetree( root->sibling);
   
   
}


/*class parse
{
private:
	
private:
	
	TreeNode * Program(void);  
	TreeNode * ProgramHead(void);
	TreeNode * DeclarePart(void);
	TreeNode * TypeDec(void);
	TreeNode * TypeDeclaration(void);
	TreeNode * TypeDecList(void);
	TreeNode * TypeDecMore(void);
	void  TypeId(TreeNode * t);
	void  TypeName(TreeNode * t);
	void  BaseType(TreeNode * t);
	void  StructureType(TreeNode * t);
	void  ArrayType(TreeNode * t);
	void  RecType(TreeNode * t);
	TreeNode * FieldDecList(void);
 
	TreeNode * FieldDecMore(void);
	void  IdList(TreeNode * t);
	void  IdMore(TreeNode * t);
	TreeNode * VarDec(void);
	TreeNode * VarDeclaration(void);
	TreeNode * VarDecList(void);
	TreeNode * VarDecMore(void);
	void  VarIdList(TreeNode * t);
	void  VarIdMore(TreeNode * t);
	TreeNode * ProcDec(void);
	TreeNode * ProcDeclaration(void);
	void  ParamList(TreeNode * t);
	TreeNode * ParamDecList(void);
	TreeNode * Param(void);
	TreeNode * ParamMore(void);
	void  FormList(TreeNode * t);
	void  FidMore(TreeNode * t);
	TreeNode * ProcDecPart(void);
	TreeNode * ProcBody(void);
	TreeNode * ProgramBody(void);
	TreeNode * StmList(void);
	TreeNode * StmMore(void);
	TreeNode * Stm(void);
	TreeNode * AssCall(void);
	TreeNode * AssignmentRest(void);
	TreeNode * ConditionalStm(void);
	TreeNode * LoopStm(void);
	TreeNode * InputStm(void);
	TreeNode * OutputStm(void);
	TreeNode * ReturnStm(void);
	TreeNode * CallStmRest(void);
	TreeNode * ActParamList(void);
	TreeNode * ActParamMore(void);
	TreeNode * Exp(void);
	 TreeNode * Simple_exp(void);	
	TreeNode * Term(void);	
	TreeNode * Factor(void);	
	TreeNode * Variable(void);
	void VariMore(TreeNode * t);
	TreeNode * Fieldvar(void);
	void FieldvarMore(TreeNode * t );
	void SyntaxError(char * message);
	void match(LexType expected);
    TreeNode * newnode(NodeKind s);
//public:      
	//struct token1=new tokenNode();//
	tokenNode *head,*current;
};*/



//56个子程序
void Tree::match(LexType expected)
{ 
      if (current->lex==expected)   
	  current=current->next;
      else 
      {
	  cout<<"匹配失败！"<<endl;
	  switch(current->lex)
	  {
	     case ENDFILE:
			 cout<<"ENDFILE"<<"---"<<current->lineno<<endl;break;
		 case ERROR:
			 cout<<"ERROR"<<"---"<<current->lineno<<endl;break;
		 case PROGRAM:
			 cout<<"PROGRAM"<<"---"<<current->lineno<<endl;break;
		 case PROCEDURE:
			 cout<<"PROCEDURE"<<"---"<<current->lineno<<endl;break;
		 case TYPE:
			 cout<<"TYPE"<<"---"<<current->lineno<<endl;break;	
		 case VAR:
			 cout<<"VAR"<<"---"<<current->lineno<<endl;	break;
		 case IF:
			 cout<<"IF"<<"---"<<current->lineno<<endl;break;
		case THEN:
			 cout<<"THEN"<<"---"<<current->lineno<<endl;break;
		case ELSE:
			 cout<<"ELSE"<<"---"<<current->lineno<<endl;break;
		case WHILE:
			 cout<<"WHILE"<<"---"<<current->lineno<<endl;break;
		case DO:
			 cout<<"DO"<<"---"<<current->lineno<<endl;break;
		case ENDWH:
			 cout<<"ENDWH"<<"---"<<current->lineno<<endl;break;
		case BEGIN:
			 cout<<"BEGIN"<<"---"<<current->lineno<<endl;break;
		case END:
			 cout<<"END"<<"---"<<current->lineno<<endl;break;
		case READ:
			 cout<<"READ"<<"---"<<current->lineno<<endl;break;
		case WRITE:
			 cout<<"WRITE"<<"---"<<current->lineno<<endl;break;
		case ARRAY:
			 cout<<"ARRAY"<<"---"<<current->lineno<<endl;break;
		case OF:
			 cout<<"OF"<<"---"<<current->lineno<<endl;break;
		case RECORD:
			 cout<<"RECORD"<<"---"<<current->lineno<<endl;break;
		case RETURN:
			 cout<<"RETURN"<<"---"<<current->lineno<<endl;break;
		case INTEGER:
			 cout<<"INTEGER"<<"---"<<current->lineno<<endl;break;
	    case CHAR:
			 cout<<"CHAR"<<"---"<<current->lineno<<endl;break;
		case ID:
			 cout<<"ID"<<"---"<<current->lineno<<endl;break;
	    case INTC:
			 cout<<"INTC"<<"---"<<current->lineno<<endl;break;
		case CHARC:
			 cout<<"CHARC"<<"---"<<current->lineno<<endl;break;
		case ASSIGN:
			 cout<<"ASSIGN"<<"---"<<current->lineno<<endl;break;
		case EQ:
			 cout<<"EQ"<<"---"<<current->lineno<<endl;break;
		case LT:
			 cout<<"LT"<<"---"<<current->lineno<<endl;break;
		case PLUS:
			 cout<<"PLUS"<<"---"<<current->lineno<<endl;break;
		case MINUS:
			 cout<<"MINUS"<<"---"<<current->lineno<<endl;break;
		case TIMES:
			 cout<<"TIMES"<<"---"<<current->lineno<<endl;break;
		case OVER:
			 cout<<"OVER"<<"---"<<current->lineno<<endl;break;
		case LPAREN:
			 cout<<"LPAREN"<<"---"<<current->lineno<<endl;break;
	    case RPAREN:
			 cout<<"RPAREN"<<"---"<<current->lineno<<endl;break;
		case DOT:
			 cout<<"DOT"<<"---"<<current->lineno<<endl;break;
		case COLON:
			 cout<<"COLON"<<"---"<<current->lineno<<endl;break;
		case SEMI:
			 cout<<"SEMI"<<"---"<<current->lineno<<endl;break;
		case COMMA:
			 cout<<"COMMA"<<"---"<<current->lineno<<endl;break;
		case LMIDPAREN:
			 cout<<"LMINPAREN"<<"---"<<current->lineno<<endl;break;
		case RMIDPAREN:
			 cout<<"RMIDPAREN"<<"---"<<current->lineno<<endl;break;
		case UNDERANGE:
			 cout<<"UNDERANGE"<<"---"<<current->lineno<<endl;break;
	  }
	  current=current->next;
      }     
}

TreeNode * Tree::parse(void)

{ 
  TreeNode * t=NULL;
  token *t1=new token();
	t1->readfile();
	current=t1->current;
  t = Program();
  if (current->lex!=DOT)
	  cout<<"语法成分有误！\n"<<endl;
  else
      cout<<"语法分析完成！\n"<<endl;
  return t;
}

TreeNode * Tree::Program()
{   
	TreeNode * root =newNode(ProK);	
	
	TreeNode * t=ProgramHead();
	TreeNode * q=DeclarePart();
	TreeNode * s=ProgramBody();
    
	if (t!=NULL) 
            root->child[0] = t;
	else 
            cout<<"无程序头！"<<endl;
	if (q!=NULL) 
            root->child[1] = q;
	if (s!=NULL)
            root->child[2] = s;
	else cout<<"程序体无！"<<endl;
        if (!(current->lex==DOT))
	    cout<<"文件意外结束\n"<<endl;
	return root;
}

TreeNode * Tree::ProgramHead()
{
	TreeNode *t = newNode(PheadK);

    match(PROGRAM);
    if (current->lex==ID)
        t->name[0]=current->grammerinfo;
    match(ID);
    return t;
}

TreeNode * Tree::DeclarePart()
{
    TreeNode * typeP = newNode(TypeK);    	 
    TreeNode * tp1 = TypeDec();
    if (tp1!=NULL)
        typeP->child[0] = tp1;
    else
	    typeP=NULL;
    TreeNode * varP = newNode(VarK);
    TreeNode * tp2 = VarDec();
    if (tp2 != NULL)
        varP->child[0] = tp2;
    else 
        varP=NULL;
    TreeNode *procP = ProcDec();
    if (procP==NULL)  {}
    if (varP==NULL)   { varP=procP; }	 
    if (typeP==NULL)  { typeP=varP; }
    if (typeP!=varP)
	typeP->sibling = varP;
    if (varP!=procP)
        varP->sibling = procP;
    return typeP;

}

TreeNode * Tree::TypeDec()
{    
	TreeNode *t = NULL;
    if (current->lex==TYPE)
        t = TypeDeclaration();
    else if ((current->lex==VAR)||(current->lex==PROCEDURE)      
            ||(current->lex==BEGIN)) {}
         else      
	     current=current->next;
    return t;
}

TreeNode * Tree::TypeDeclaration()
{
	match(TYPE);
    TreeNode * t = TypeDecList();
    if (t==NULL)
        cout<<"type declaration错误！!"<<endl;
    return t;
}

TreeNode *Tree::TypeDecList()
{
    TreeNode * t = newNode(DecK);
    if (t != NULL)
    {
	TypeId(t);                               
	match(EQ);  
	TypeName(t); 	
	match(SEMI); 	
        TreeNode * p = TypeDecMore();
	if (p!=NULL)
	    t->sibling = p;
    }
    return t;
}

TreeNode *Tree::TypeDecMore()
{
    TreeNode * t=NULL;
    if (current->lex==ID)
        t = TypeDecList();
    else if ((current->lex==VAR)||(current->lex==PROCEDURE)||(current->lex==BEGIN)) {}       
         else
	     current=current->next;
    return t;
}

void Tree::TypeId(TreeNode *t)
{
	if ((current->lex==ID)&&(t!=NULL))
    {
	t->name[(t->idnum)]=current->grammerinfo;
	t->idnum = t->idnum+1;
    }
    match(ID);
}

void Tree::TypeName(TreeNode *t)
{
	 if (t !=NULL)
   {
      if ((current->lex==INTEGER)||(current->lex==CHAR))    
          BaseType(t);
      else if ((current->lex==ARRAY)||(current->lex==RECORD))   
              StructureType(t);
      else if (current->lex==ID) 
           {
             t->kind.dec=IdK;
	         t->attr.type_name = current->grammerinfo;    
                 match(ID);  
           }
	   else
	       current=current->next;
   }
}
void Tree::BaseType(TreeNode *t)
{
       if (current->lex==INTEGER)
       { 
             match(INTEGER);
             t->kind.dec=IntegerK;
       }
       else if (current->lex==CHAR)     
             {
                 match(CHAR);
                 t->kind.dec= CharK;
             }
             else
                current=current->next;   
}

void Tree::StructureType(TreeNode *t)
{
	   if (current->lex==ARRAY)
       {
           ArrayType(t); 
       }          
       else if (current->lex==RECORD)     
            {
                 t->kind.dec = RecordK;
                 RecType(t);
            }
            else
                current=current->next;   
}

void Tree::ArrayType(TreeNode *t)
{
     match(ARRAY);
     match(LMIDPAREN);
     if (current->lex==INTC)
	 t->attr.ArrayAttr.low = atoi(current->grammerinfo.c_str());
     match(INTC);
     match(UNDERANGE);
     if (current->lex==INTC)
	 t->attr.ArrayAttr.up = atoi(current->grammerinfo.c_str());
     match(INTC);
     match(RMIDPAREN);
     match(OF);
     BaseType(t);
     t->attr.ArrayAttr.childtype = t->kind.dec;
     t->kind.dec =ArrayK;
}

void Tree::RecType(TreeNode * t)
{
    TreeNode * p = NULL;
    match(RECORD);
    p = FieldDecList();
    if (p!=NULL)
        t->child[0] = p;
    else
        cout<<"record body木有!"<<endl;         
    match(END);
}

TreeNode * Tree::FieldDecList()
{
    TreeNode * t = newNode(DecK);
    TreeNode * p = NULL;
    if (t != NULL)
    {
        if ((current->lex==INTEGER)||(current->lex==CHAR))
        {
                    BaseType(t);
	            IdList(t);
				
	            match(SEMI);
				//current=current->next;
	            p = FieldDecMore();
        }
	else if (current->lex==ARRAY) 
             {
	            ArrayType(t);
	            IdList(t);			
	            match(SEMI);
				//current=current->next;
	            p = FieldDecMore();
             }
             else
             {
		    current=current->next;
		    cout<<"type name木有"<<endl;
             }
        t->sibling = p;
    }	    
    return t;	
}

TreeNode * Tree::FieldDecMore()
{
    TreeNode * t = NULL;   
    if (current->lex==INTEGER||current->lex==CHAR||current->lex==ARRAY)
	 t = FieldDecList();
    else if (current->lex==END) {}
	 else
             current=current->next;
    return t;	
}

void Tree::IdList(TreeNode * t)
{
    if (current->lex==ID)
    {
	t->name[(t->idnum)] = current->grammerinfo;
	t->idnum = t->idnum + 1;
    }
    IdMore(t);
}

void Tree::IdMore(TreeNode * t)
{
    if (current->lex==COMMA)
    {
        match(COMMA);
        IdList(t);
    }
    else if (current->lex==SEMI) {}
         else
	     current=current->next;	
}

TreeNode * Tree::VarDec()
{
    TreeNode * t = NULL;
    if (current->lex==VAR)
        t = VarDeclaration();
    else if ((current->lex==PROCEDURE)||(current->lex==BEGIN)) {}
	 else
	     current=current->next;
    return t;
}

TreeNode * Tree::VarDeclaration()
{
    match(VAR);
    TreeNode * t = VarDecList();
    if (t==NULL)
	cout<<"var declaration木有!"<<endl;
    return t;
}

TreeNode * Tree::VarDecList()
{
    TreeNode * t = newNode(DecK);
    TreeNode * p = NULL;
    if (t != NULL)
    {
	TypeName(t);
	VarIdList(t);
	
	match(SEMI);
        p = VarDecMore();
	t->sibling = p;
    }
    return t;
}

TreeNode * Tree::VarDecMore()
{
    TreeNode * t =NULL;
    if ((current->lex==INTEGER)||(current->lex==CHAR)||(current->lex==ARRAY)||(current->lex==RECORD)||(current->lex==ID))
    	t = VarDecList();
    else if ((current->lex==PROCEDURE)||(current->lex==BEGIN))
	     {}
	 else
             current=current->next;
    return t;
}

void Tree::VarIdList(TreeNode * t)
{
    if (current->lex==ID)
    {
        t->name[(t->idnum)] = current->grammerinfo;
	t->idnum = t->idnum + 1;
        match(ID);
    }
    else 
    {
	cout<<"varid错误！当前单词不为ID!"<<endl;
	current=current->next;
    }
    VarIdMore(t);
}


void Tree::VarIdMore(TreeNode * t)
{
    if (current->lex==COMMA)
    {   
        match(COMMA);
        VarIdList(t);
    }
    else if (current->lex==SEMI)  {}
         else
             current=current->next;	
}

TreeNode * Tree::ProcDec()
{
    TreeNode * t = NULL;
    if (current->lex==PROCEDURE)
        t = ProcDeclaration();
    else if (current->lex==BEGIN) {}
         else
	     current=current->next;
    return t;
}

TreeNode * Tree::ProcDeclaration()
{
    TreeNode * t = newNode(ProcDecK);
    match(PROCEDURE);
    if (current->lex==ID)
    {
        t->name[0] = current->grammerinfo;
	t->idnum = t->idnum+1;
	match(ID);
    }
    match(LPAREN);
    ParamList(t);
    match(RPAREN);
    match(SEMI);
    t->child[1] = ProcDecPart();
    t->child[2] = ProcBody();
    return t;
}

void Tree::ParamList(TreeNode * t)     
{
    TreeNode * p = NULL;
    if ((current->lex==INTEGER)||(current->lex==CHAR)||(current->lex==ARRAY)||(current->lex==RECORD)||(current->lex==ID)||(current->lex==VAR))
    {
        p = ParamDecList();
        t->child[0] = p;
    } 
    else if (current->lex==RPAREN) {} 
	 else
	     current=current->next;
}

TreeNode * Tree::ParamDecList()
{
    TreeNode * t = Param();
    TreeNode * p = ParamMore();
    if (p!=NULL)
        t->sibling = p;
    return t;
}

TreeNode * Tree::ParamMore()
{
    TreeNode * t = NULL;
    if (current->lex==SEMI)
    {   
        match(SEMI);
        t = ParamDecList();
	if (t==NULL)
           cout<<"param declaration木有!"<<endl;
    }
    else if (current->lex==RPAREN)  {} 
	 else
	     current=current->next;
    return t;
}

TreeNode * Tree::Param()
{
    TreeNode * t = newNode(DecK);
    if ((current->lex==INTEGER)||(current->lex==CHAR)||(current->lex==ARRAY)||(current->lex==RECORD)|| (current->lex==ID))
    {
         t->attr.ProcAttr.paramt = valparamType;
	 TypeName(t);
	 FormList(t);
    }
    else if (current->lex==VAR)
         {
             match(VAR);
             t->attr.ProcAttr.paramt = varparamType;
	     TypeName(t);
	     FormList(t);
	 }
         else
             current=current->next;
    return t;
}

void Tree::FormList(TreeNode * t)
{
    if (current->lex==ID)
    {
	t->name[(t->idnum)] = current->grammerinfo;
	t->idnum = t->idnum + 1;
	match(ID);
    }
    FidMore(t);   
}


void Tree::FidMore(TreeNode * t)
{      
    if (current->lex==COMMA)
    {
        match(COMMA);
	FormList(t);
    }
    else if ((current->lex==SEMI)||(current->lex==RPAREN))  
             {}
         else
	     current=current->next;	  
}

TreeNode * Tree::ProcDecPart()
{
    TreeNode * t = DeclarePart();
    return t;
}

TreeNode * Tree::ProcBody()
{
    TreeNode * t = ProgramBody();
    if (t==NULL)
	cout<<"program body木有!"<<endl;
    return t;
}

TreeNode * Tree::ProgramBody()
{
    TreeNode * t = newNode(StmLK);
	
    match(BEGIN);
	if(t!=NULL)
     t->child[0] = StmList();
    match(END);
    return t;
}

TreeNode * Tree::StmList()
{
    TreeNode * t = Stm();
    TreeNode * p = StmMore();
	if(t!=NULL){
      if (p!=NULL)
	  {t->sibling = p;}}
    return t;
}

TreeNode * Tree::StmMore()
{
    TreeNode * t = NULL;
    if ((current->lex==END)||(current->lex==ENDWH)) {}
    else if (current->lex==SEMI)
	{    
         match(SEMI);
	     t = StmList();
	 }
	 else
	     current=current->next;
    return t;
}

TreeNode * Tree::Stm()
{
    TreeNode * t = NULL;
    if (current->lex==IF)
        t = ConditionalStm();
    else if (current->lex==WHILE)         
	     t = LoopStm();
    else if (current->lex==READ)  
	     t = InputStm();	 
    else if (current->lex==WRITE)   
	     t = OutputStm();	 
    else if (current->lex==RETURN)  
	     t = ReturnStm();	 
    else if (current->lex==ID)
         {
             temp_name = current->grammerinfo;
			 match(ID);
             t = AssCall();
         }
	 else
	     current=current->next;
    return t;
}

TreeNode * Tree::AssCall()
{
    TreeNode * t = NULL;
    if ((current->lex==ASSIGN)||(current->lex==LMIDPAREN)||(current->lex==DOT))
	     t = AssignmentRest();
    else if (current->lex==LPAREN)
	     t = CallStmRest();
	 else 
	     current=current->next;
    return t;
}

TreeNode * Tree::AssignmentRest()
{
    TreeNode * t = newStmtNode(AssignK);
	 TreeNode * c = newExpNode(VariK);
    c->name[0] = temp_name;
    c->idnum = c->idnum+1;
    VariMore(c);
    t->child[0] = c;
		
    match(ASSIGN);
	  
    t->child[1] = Exp(); 	
    return t;
}

TreeNode * Tree::ConditionalStm()
{
    TreeNode * t = newStmtNode(IfK);
    match(IF);
    t->child[0] = Exp();
    match(THEN);
	tokenNode *xls=current;
    if (t!=NULL)  
	{t->child[1] = StmList();}
	current=xls->next->next->next->next->next;
	tokenNode *xls1=current;
    if(current->lex==ELSE)
    {
	match(ELSE);   
	t->child[2] = StmList();
    }
	current=xls1->next->next->next->next->next->next;
    match(FI);
    return t;
}

TreeNode * Tree::LoopStm()
{
    TreeNode * t = newStmtNode(WhileK);
    match(WHILE);
    t->child[0] = Exp();
    match(DO);
    t->child[1] = StmList();
    return t;
}

TreeNode * Tree::InputStm()
{
    TreeNode *  t = newStmtNode(ReadK);
    match(READ);
    match(LPAREN);
    if (current->lex==ID)	
    {
	t->name[0] =current->grammerinfo;
        t->idnum = t->idnum+1;
    }
    match(ID);
    match(RPAREN);
    return t;
}

TreeNode * Tree::OutputStm()
{
    TreeNode * t = newStmtNode(WriteK);
    match(WRITE);
    match(LPAREN);
    t->child[0] = Exp();
    match(RPAREN);
    return t;
}

TreeNode * Tree::ReturnStm()
{
    TreeNode * t = newStmtNode(ReturnK);
    match(RETURN);
    return t;
}

TreeNode * Tree::CallStmRest()
{
    TreeNode * t=newStmtNode(CallK);
    match(LPAREN);
	/*if(t!=NULL)
		t->child[0]= ActParamList();
	    t->name[0]=temp_name;
        t->idnum = t->idnum+1;*/
    TreeNode * c = newExpNode(VariK); 
    c->name[0] = temp_name;
    c->idnum = c->idnum+1;
    t->child[0] = c;
    t->child[1] = ActParamList();
    match(RPAREN);
    return t;
}

TreeNode * Tree::ActParamList()
{
    TreeNode * t = NULL;
    if (current->lex==RPAREN)  {}
    else if ((current->lex==ID)||(current->lex==INTC))
	 {
	     t = Exp();
             if (t!=NULL)
	         t->sibling = ActParamMore();
         }
	 else
             current=current->next;
    return t;
}

TreeNode * Tree::ActParamMore()
{
    TreeNode * t = NULL;
    if (current->lex==RPAREN)  {}
    else if (current->lex==COMMA)
         {
	     match(COMMA);
	     t = ActParamList();
	 }
	 else	
	     current=current->next;
    return t;
}

TreeNode * Tree::Exp()
{
    TreeNode * t = Simple_exp();
    if ((current->lex==LT)||(current->lex==EQ)) 
    {
        TreeNode * p = newExpNode(OpK);
	p->child[0] = t;
        p->attr.ExpAttr.op = current->lex;	
        t = p;
        match(current->lex);
        if (t!=NULL)
            t->child[1] = Simple_exp();
    }
    return t;
}

TreeNode * Tree::Simple_exp()
{
    TreeNode * t = Term();
    while ((current->lex==PLUS)||(current->lex==MINUS))
    {
	TreeNode * p = newExpNode(OpK);
	p->child[0] = t;
        p->attr.ExpAttr.op = current->lex;
        t = p;
        match(current->lex);
        t->child[1] = Term();
    }
    return t;
}

TreeNode * Tree::Term()
{
    TreeNode * t = Factor();
    while ((current->lex==TIMES)||(current->lex==OVER))
    {
	TreeNode * p = newExpNode(OpK);
	p->child[0] = t;
        p->attr.ExpAttr.op = current->lex;
        t = p;	
        match(current->lex);
        t->child[1] = Factor();    
    }
    return t;
}

TreeNode * Tree::Factor()
{
    TreeNode * t = NULL;
    if (current->lex==INTC) 
    {
        t = newExpNode(ConstK);
        t->attr.ExpAttr.val = atoi(current->grammerinfo.c_str());
        match(INTC);
    }
    else if (current->lex==ID)  	  
	     t = Variable();
    else if (current->lex==LPAREN) 
	 {
             match(LPAREN);					
             t = Exp();
             match(RPAREN);					
         }
         else 			
             current=current->next;	  
    return t;
}

TreeNode * Tree::Variable()
{
    TreeNode * t = newExpNode(VariK);
    if (current->lex==ID)
    {
	t->name[0] = current->grammerinfo;
        t->idnum = t->idnum+1;
    }
    match(ID);
    VariMore(t);
    return t;
}
	
void Tree::VariMore(TreeNode * t)
{
        if ((current->lex==ASSIGN)||(current->lex==TIMES)||(current->lex==EQ)||(current->lex==LT)
			||(current->lex==PLUS)||(current->lex==MINUS)||(current->lex==OVER)||(current->lex==RPAREN)||(current->lex==RMIDPAREN)
			||(current->lex==SEMI)||(current->lex==COMMA)||(current->lex==THEN)||(current->lex==ELSE)
			||(current->lex==FI)||(current->lex==DO)||(current->lex==ENDWH)||(current->lex==END))    {}
	else if (current->lex==LMIDPAREN)
             {
	         match(LMIDPAREN);
	         t->child[0] = Exp();
             t->attr.ExpAttr.varkind = ArrayMembV;
			 t->child[0]->attr.ExpAttr.varkind = IdV;
	     }
	else if (current->lex==DOT)
             {
	         match(DOT);
	         t->child[0] = Fieldvar();
             t->attr.ExpAttr.varkind = FieldMembV;
			 t->child[0]->attr.ExpAttr.varkind = IdV;
	     }
	     else
	         current=current->next;
}

TreeNode * Tree::Fieldvar()
{
    TreeNode * t = newExpNode(VariK);
    if (current->lex==ID)
    {
	t->name[0] = current->grammerinfo;
        t->idnum = t->idnum+1;
    }	
    match(ID);	
    FieldvarMore(t);
    return t;
}

void Tree::FieldvarMore(TreeNode * t)
{
    if ((current->lex==ASSIGN)||(current->lex==TIMES)||(current->lex==EQ)||(current->lex==LT)
			||(current->lex==PLUS)||(current->lex==MINUS)||(current->lex==OVER)||(current->lex==RPAREN)
			||(current->lex==SEMI)||(current->lex==COMMA)||(current->lex==THEN)||(current->lex==ELSE)
			||(current->lex==FI)||(current->lex==DO)||(current->lex==ENDWH)||(current->lex==END))
       {}
    else if (current->lex==LMIDPAREN)
         { 
	     match(LMIDPAREN);
	     t->child[0] = Exp();
             t->child[0]->attr.ExpAttr.varkind = ArrayMembV;
	     match(RMIDPAREN);
	 }
	 else
	     current=current->next;
};


int main()
{
	Tree r;
    TreeNode *xx;
	string yufa="yufafenxi.txt";
	ofstream outfile;
	outfile.open(yufa.c_str(),ofstream::trunc);
	outfile.close();
	xx=r.parse();
	r.filetree(xx);
	outfile.open(yufa.c_str(),ofstream::app);
	cout<<"语法分析结束,请到文件中查看\n"<<endl;
	system("pause");
	return 0;
}
