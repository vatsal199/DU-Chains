/*

Author: Dhameliya Vatsalkumar
Email: vatsal17137@gmail.com

*/

#include <iostream>
#include<string>
#include<vector>
#include<fstream>
#include<algorithm>
#include<string.h>
#include <cstddef>
#include<stdlib.h>
#include<cstdio>
using namespace std;
typedef pair< int , int> ii;
typedef vector<string> vs;
typedef vector< vs > vvs;
typedef vector<int> vi;
typedef vector< vi >  vvi;
class fileRead{
    vector<string> vs;
    public:
        fileRead(){
            vs.push_back("null");
        }
        string trim(string str)
        {
            char s[100];
            int j=0;
            bool flag = true;
            for(int i=0;i<str.length();i++){
                if((str[i]==' ' || str[i]== 9)&& flag){
                    continue;
                }else{
                    flag=false;
                    s[j++]=str[i];
                }
            }
            s[j]='\0';
            return s;
        }
        int getLength(){
            return vs.size();
        }
        string getStatement(int num){
            return vs[num];
        }
        void readInVector(){
            cout<<"Enter file path:";
            char path[300];
            scanf("%s",path);
            //ifstream fin("C:/Users/vatsa/Desktop/tp1.cpp");
            ifstream fin(path);
            string line;
            for(string line; getline( fin, line );){
               line = trim(line);
                if(line != "")
                    vs.push_back(line);
            }
        }
};
class DU{
        vvs DEF;
        vvs USE;
        int N;
        vs symbTbl;
        fileRead fr;
    public:
        DU(){}
        DU(fileRead f1){
            fr = f1;
            N = fr.getLength();
            DEF.assign(N+1,vs());
            USE.assign(N+1,vs());
        }
        vvs getDEF(){
            return DEF;
        }
        vvs getUSE(){
            return USE;
        }
        bool startWithPrintf(string str){
            if(str.substr(0,6)=="printf")
                return true;
            else
                return false;

        }
        int isDataType(string str){
            string s[]={"int","void","float","double","long int","long long int","bool","string","char"};
            bool flag=false;
            int matchLength;
            for(int i=0;i<9;i++){
                int len = s[i].length();
                if(str.substr(0,len)==s[i]){
                    matchLength = len;
                    flag=true;
                    break;
                }
            }
            if(!flag)
                return 0;
            else
                return matchLength;
        }
        bool isFunctionName(string str,int dtlen){
            int flag = 0;
            for(int i=dtlen;i<str.length();i++){
                if(flag==0 && str[i]=='(' ){
                        flag = 1;
                        continue;
                   }
                else if(flag==1 && str[i]==')'){
                    flag = 2;
                    break;
                }
            }
            if(flag == 2)
                return true;
            else
                return false;
        }
        bool startWithHash(string str){
            if(str[0] == '#')
                return true;
            else
                return false;
        }
        bool inItOpenCurli(string str){
            size_t found = str.find_first_of("{");
            if(found!=std::string::npos)
                return true;
            return false;
        }
        void generateSymbolTable(){
            for(int i=1;i<fr.getLength();i++){
                int line_num = i;
                string line = fr.getStatement(i);
                if(startWithHash(line)){
                    if(line.substr(0,7)=="#define"){
                        string s="";
                        for(int j=8;line[j]!=' ';j++)
                            s.push_back(line[j]);
                        symbTbl.push_back(s);
                        DEF[line_num].push_back(s);
                    }
                }
                else if(!startWithPrintf(line)){
                    int dtlen = isDataType(line);
                    if(dtlen!=0){
                        if(!isFunctionName(line,dtlen)){
                            int i=dtlen;
                            for(;line[i]==' ';i++);
                            string s="";
                            for(;line[i] != '=' && line[i] != ' ' && line[i] != ';';i++)
                                s.push_back(line[i]);
                            symbTbl.push_back(s);
                        }else{
                            int j=0;
                            while(line[j] != '(')
                                    j++;
                            j++;
                            for(;line[j]!=')';){
                                string s=line.substr(j);
                                int dtlen=isDataType(s);
                                j+=dtlen;
                                j++;
                                string tp="";
                                for(;line[j]!=' ' && line[j]!=',' && line[j]!=')';j++)
                                    tp.push_back(line[j]);
                                symbTbl.push_back(tp);
                                DEF[line_num].push_back(tp);
                                for(;line[j]==' ' || line[j]==',';j++);
                            }
                        }
                    }
                }
            }
        }
        bool startWithWhile(string str){
            if(str.substr(0,5)== "while")
                return true;
            else
                return false;
        }
        bool startWithFor(string str){
            if(str.substr(0,3)== "for")
                return true;
            else
                return false;
        }
        bool startWithReturn(string str){
            if(str.substr(0,6)== "return")
                return true;
            else
                return false;
        }
        bool startWithIf(string str){
            if(str.substr(0,2)== "if")
                return true;
            else
                return false;
        }
        bool startWithElse(string str){
            if(str.substr(0,4)== "else")
                return true;
            else
                return false;
        }
        bool startWithCloseCurli(string str){
            if(str.substr(0,1)== "}")
                return true;
            else
                return false;
        }
        bool startWithOpenCurli(string str){
            if(str.substr(0,1)== "{")
                return true;
            else
                return false;
        }
        void useOnly(string str,int line_num){
            for(int i=0;i<symbTbl.size();i++){
                size_t found = str.find(symbTbl[i]);
                if (found!=std::string::npos)
                    USE[line_num].push_back(symbTbl[i]);
            }
        }
        void find_DEF_USE(string str,int line_num){
            vector<ii> index;
            for(int i=0;i<symbTbl.size();i++){
                size_t found = str.find(symbTbl[i]);
                if (found!=std::string::npos){
                    index.push_back(ii(found,i));
                }
            }
            sort(index.begin(),index.end());
            bool flag = true;
            for(int i=0;i<index.size();i++){
                if(flag){
                    DEF[line_num].push_back(symbTbl[index[i].second]);
                    flag = false;
                }else{
                    USE[line_num].push_back(symbTbl[index[i].second]);
                }
            }
            int DEFfound = index[0].first;
            string serchString = symbTbl[index[0].second];
            size_t found = str.find(serchString,DEFfound+1);
            if (found!=std::string::npos){
                    USE[line_num].push_back(symbTbl[index[0].second]);
            }
        }
        void generateDU(){
            int i=1;
            string line=fr.getStatement(i);
            for(;!inItOpenCurli(line);i++){
                 line = fr.getStatement(i);
            }
            if(i==1)
                i=2;
            for(;i<fr.getLength();i++){
                line = fr.getStatement(i);
                if(startWithWhile(line)){
                    useOnly(line.substr(5),i);
                }
                else if(startWithFor(line)){
                    useOnly(line.substr(3),i);
                }
                else if(startWithIf(line)){
                    useOnly(line.substr(2),i);
                }
                else if(startWithReturn(line)){
                    useOnly(line.substr(6),i);
                }
                else if(startWithElse(line) || startWithCloseCurli(line) || startWithOpenCurli(line)){

                }else{
                    find_DEF_USE(line,i);
                }
            }
        }
};
class flowDiagram{
    vector< vector<int> > flowDig;
    int N;
    fileRead fr;
    public:
        flowDiagram(){}
        flowDiagram(fileRead f1){
            fr = f1;
            N=fr.getLength();
            flowDig.assign(N+1,vector<int>());
        }
        vvi getFlowDiagram(){
            return flowDig;
        }
        bool startWithPrintf(string str){
            string s="printf";
            for(int i=0;i<6;i++)
            {
                if(str[i] != s[i])
                    return false;
            }
            return true;
        }
        bool findSemicolon(string str){
            if(!startWithPrintf(str)){
                for(int i=str.length()-1;i>=0;i--){
                    if(str[i] == ';')
                        return true;
                }
            }
            return false;
        }
        bool isIntMain(string str){
            string s[]={"int","void","float","double","long int","long long int","bool","string","char"};
            bool flag = false;
            for(int i=0;i<9;i++){
                int len = s[i].length();
                if(str.substr(0,len) == s[i]){
                    flag = true;
                    break;
                }
            }
            if(!flag)
                return false;
            int flag1=0;
            for(int i=0;i<str.length();i++){
                if(flag1==0){
                    if(str[i]=='('){
                        flag1=1;
                        continue;
                       }
                }
                else if(flag1 == 1){
                    if(str[i]==')'){
                        flag1=2;
                        continue;
                       }
                }
            }
            if(flag1 == 2)
                return true;
            else
                return false;
        }
         bool inItOpenCurli(string str){
            size_t found = str.find_first_of("{");
            if(found!=std::string::npos)
                return true;
            return false;
        }
         bool inItCloseCurli(string str){
            size_t found = str.find_first_of("}");
            if(found!=std::string::npos)
                return true;
            return false;
        }
        int isWhileLoop(string str){
            if(str.substr(0,5) == "while"){
                if(inItOpenCurli(str))
                    return 2;
                else
                    return 1;
            }
            return 0;
        }
        int isForLoop(string str){
            if(str.substr(0,3) == "for"){
                if(inItOpenCurli(str))
                    return 2;
                else
                    return 1;
            }
            return 0;
        }

        bool isLoop(string str,int line_num){
            if(isWhileLoop(str) != 0){
                if(isWhileLoop(str) == 2){
                    vector<int> tp;
                    int tpline=line_num+1;
                    tp.push_back(1);
                    while(tp.size()!=0){
                        string tpstr=fr.getStatement(tpline);
                        if(inItOpenCurli(tpstr)){
                             tp.push_back(1);
                        }
                        if(inItCloseCurli(tpstr)){
                             tp.pop_back();
                        }
                        tpline++;
                    }
                    flowDig[tpline-1].push_back(line_num);
                }
                return true;
            }
            else if(isForLoop(str) != 0){
                if(isForLoop(str) == 2){
                    vector<int> tp;
                    int tpline=line_num+1;
                    tp.push_back(1);
                    while(tp.size()!=0){
                        string tpstr=fr.getStatement(tpline);
                        if(inItOpenCurli(tpstr)){
                             tp.push_back(1);
                        }
                        if(inItCloseCurli(tpstr)){
                             tp.pop_back();
                        }
                        tpline++;
                    }
                    flowDig[tpline-1].push_back(line_num);
                }
                return true;
            }
            else
                return false;
        }
        bool isIF(string str,int line_num){
            if(str.substr(0,2) == "if"){
                int tpline=line_num;
                vector<int> state;
                if(inItOpenCurli(str)){
                   tpline++;
                   string tpstr = fr.getStatement(tpline);
                   while(!inItCloseCurli(tpstr)){
                        tpline++;
                        tpstr = fr.getStatement(tpline);
                   }
                   state.push_back(tpline);
                }else{
                    tpline++;
                    state.push_back(tpline);
                }
                tpline++;
                state.push_back(tpline+1);
                if(inItOpenCurli(fr.getStatement(tpline))){
                        //cout<<"5";
                    tpline++;
                    string tpstr = fr.getStatement(tpline);
                    while(!inItCloseCurli(tpstr)){
                        tpline++;
                        tpstr = fr.getStatement(tpline);
                   }
                   state.push_back(tpline);
                }else{
                    state.push_back(tpline+1);
                }
                flowDig[line_num].push_back(line_num+1);
                flowDig[line_num].push_back(state[1]);
                flowDig[state[0]].push_back(state[2]+1);
                flowDig[state[1]-1].push_back(state[1]);
                flowDig[state[0]].push_back(-1);
                flowDig[state[2]].push_back(state[2]+1);
                flowDig[state[2]].push_back(-1);
                return true;
            }
            else
                return false;
        }
        bool isReturn(string str){
            if(str.substr(0,6) == "return")
                return true;
            else
                return false;
        }
        bool onlyCurli(string str){
            if(str.length() == 1){
                if(str[0]=='{' || str[0]=='}')
                    return true;
            }
            return false;
        }
        bool ifButNotElse(string str,int line_num){
            if(str.substr(0,2) == "if"){
                int ifEndLine;
                if(inItOpenCurli(str)){
                    vector< int > tp;
                    tp.push_back(1);
                    int tpline = line_num + 1;
                    while(tp.size() != 0){
                         string tpstr = fr.getStatement(tpline);
                         if(inItOpenCurli(tpstr))
                            tp.push_back(1);
                         else if(inItCloseCurli(tpstr))
                            tp.pop_back();
                         tpline++;
                    }
                    ifEndLine = tpline - 1;
                    string tempstr = fr.getStatement(tpline);
                    if(tempstr.substr(0,4) == "else"){
                        return false;
                    }else{
                        flowDig[line_num].push_back(line_num+1);
                        flowDig[line_num].push_back( ifEndLine+1);
                        flowDig[line_num].push_back(-1);
                        return true;
                    }

                }else{
                    string tpline =fr.getStatement(line_num+2);
                    if(tpline.substr(0,4) == "else")
                        return false;
                    else{
                        flowDig[line_num].push_back(line_num+1);
                        flowDig[line_num].push_back(line_num+2);
                        flowDig[line_num].push_back(-1);
                        return true;
                    }
                }
            }
            return false;
        }
        void modigyFlowDig(){
            for(int i=0;i<flowDig.size();i++){
                for(int j=0;j<flowDig[i].size();j++){
                    if(flowDig[i][j] == -1){
                        int k=flowDig[i].size();
                        k=k-j;
                        while(k--){
                            flowDig[i].pop_back();
                        }
                    }
                }
            }
        }
        bool startWithHash(string str){
            if(str[0] == '#')
                return true;
            else
                return false;
        }
        void generateFlow(){
            for(int i=1;i<N;i++){
                string line= fr.getStatement(i);
               // cout<<"\n"<<line;
                if(startWithHash(line)){
                    flowDig[i].push_back(i+1);
                }
                else if(isLoop(line,i)){
                    flowDig[i].push_back(i+1);
                }
                else if(ifButNotElse(line,i)){
                }
                else if(isIF(line,i)){
                }
                else if(isReturn(line)){
                    flowDig[i].push_back(-1);
                }
                else if(findSemicolon(line)){
                    flowDig[i].push_back(i+1);
                }
                else if(isIntMain(line)){
                    flowDig[i].push_back(i+1);
                }
                else if(onlyCurli(line)){
                    flowDig[i].push_back(i+1);
                }
            }
            modigyFlowDig();
        }
};
class DUChain{
    vs temp;
    vs completed;
    vvs DEF;
    vvs USE;
    vvi flowDig;
    fileRead fr;
    DU du;
    flowDiagram fd;
    vector<bool> defineComplete;
    int N;
    public:
     DUChain(fileRead f1,DU d1,flowDiagram fd1){
        fr = f1;
        du = d1;
        fd = fd1;
        DEF = du.getDEF();
        USE = du.getUSE();
        flowDig = fd.getFlowDiagram();
        N = fr.getLength();
        defineComplete.assign(N,false);
     }

        bool isInUse(int line_num,string str){
            for(int i=0;i<USE[line_num].size();i++){
                if(USE[line_num][i] == str){
                    return true;
                }
            }
            return false;
        }
         bool isInDef(int line_num,string str){
            for(int i=0;i<DEF[line_num].size();i++){
                if(DEF[line_num][i] == str){
                    return true;
                }
            }
            return false;
        }
        void allFalse(bool trace[]){
            for(int i=0;i<N;i++)
                trace[i] = false;
        }
        void solve(){
            int line_num=1;
            bool trace[N];
            allFalse(trace);
            for(int i=1;i<=N;i++){
                for(int j=0;j<DEF[i].size();j++){
                        if(!defineComplete[i]){
                            cout<<endl;
                            completed.push_back(DEF[i][j]);
                            line_num = i;
                            allFalse(trace);
                            defineComplete[line_num] = true;
                            executeAlgo(line_num+1,DEF[i][j],fr.getStatement(line_num),trace);
                        }
                }
            }
        }
        string removeCurli(string str){
            bool flag=false;
            int pos;
            for(int i=str.length()-1;i>=0;i--){
                if(str[i] == '{'){
                    pos=i;
                    flag = true;
                    break;
                }
            }
            if(flag){
                string s="";
                for(int i=0;i<str.length();i++){
                    if(i==pos)
                        continue;
                    s.push_back(str[i]);
                }
                return s;
            }
            else{
                return str;
            }
        }
        void executeAlgo(int line_num,string variable,string defination,bool trace[]){
            if(!trace[line_num]){
                trace[line_num] = true;
                if(line_num == N)
                    return;
                if(isInUse(line_num,variable)){
                    string USEstatement = fr.getStatement(line_num);
                    USEstatement = removeCurli(USEstatement);
                    cout<<"\n[ "<<variable<<" , "<<removeCurli(defination)<<" , "<<USEstatement<<" ]";
                }
                bool flag = false;
                if(isInDef(line_num,variable)){
                    if(!defineComplete[line_num]){
                        flag = true;
                        defination = fr.getStatement(line_num);
                        defineComplete[line_num] = true;
                    }
                }
                for(int i=0;i<flowDig[line_num].size();i++){
                    if(flag){
                         bool trace1[N];
                         allFalse(trace1);
                         executeAlgo(flowDig[line_num][i],variable,defination,trace1);
                    }else{
                        executeAlgo(flowDig[line_num][i],variable,defination,trace);
                    }
                }
            }
            return;
        }
};
int main()
{
    fileRead fr;
    fr.readInVector();
    DU du(fr);
    du.generateSymbolTable();
    du.generateDU();
    flowDiagram fd(fr);
    fd.generateFlow();
    DUChain duc(fr,du,fd);
    duc.solve();
    return 0;
}
