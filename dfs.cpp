#include<iostream>
#include<stack>
using namespace std;
#define MAX 1024
struct node{
	int k;
	int p[MAX];
	int now[MAX];
};
node setNode(node n,int num){
	n.k=0;	
	for(int i=0;i<1024;i++){
		if(i>=num){
			n.p[i]=-1;
			n.now[i]=-1;
		}else{
			n.p[i]=0;
			n.now[i]=0;
		}
	}
	return n;
}
stack<node> mystack;
int main(){
	int num;
	cin>>num;
	node first;
	first=setNode(first,num);
	mystack.push(first);
	while(!mystack.empty()){
		node now=mystack.top();
		mystack.pop();
		if(now.k==num){
			for(int i=1;i<=num;i++)cout<<now.p[i]+1;
			cout<<endl;
		}else
		for(int i=0;i<num;i++){
			if(now.now[i]==0){
				node next=now;
				next.now[i]=1;
				next.p[now.k+1]=i;
				next.k+=1;
				mystack.push(next);
			}
		}
	}
	return 0;
} 
