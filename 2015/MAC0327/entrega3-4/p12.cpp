/*
Nome: André Meneghelli Vale
NUSP: 4898948
Problema P12 - 1076. Trash
*/
#include<cstdio>
#include<cstring>
#include<algorithm>

using namespace std;

const int maxn=155;
const int inf=1000;
int w[maxn][maxn];
int lx[maxn],ly[maxn];
bool s[maxn],t[maxn];
int match[maxn];
int slack[maxn];
int n;

bool metodoHungaro(int u) {
	s[u] = true;
	for(int v=1;v<=n;v++)
	{
		if(!t[v] && lx[u]+ly[v] == w[u][v])
		{
			t[v] = true;
			if(match[v]==-1 || metodoHungaro(match[v]))
			{
				match[v]=u;
				return true;
			}
		}
		else if(slack[v] > lx[u]+ly[v]-w[u][v]) slack[v] = lx[u]+ly[v]-w[u][v];
	}
	return false;
}

int KM() {
	int sum=0;
	memset(match,-1,sizeof(match));
	memset(ly,0,sizeof(ly));
	for(int i=1;i<=n;i++)
	{
		lx[i]= -inf;
		for(int j=1;j<=n;j++)
			lx[i]=max(lx[i],w[i][j]);
	}
	for(int i=1;i<=n;i++)
	{
		while(true)
		{
			memset(s,false,sizeof(s));
			memset(t,false,sizeof(t));
			for(int j=1;j<=n;j++)
				slack[j]=inf;
			if(metodoHungaro(i)) break;
			else
			{
				int a=inf;
				for(int j=1;j<=n;j++)
					if(!t[j] && slack[j]<a)
						a=slack[j];
				for(int j=1;j<=n;j++)
				{
					if(s[j]) lx[j]-=a;
					if(t[j]) ly[j]+=a;
				}
			}
		}
	}
	for(int i=1;i<=n;i++) sum+=w[match[i]][i];
	return sum;
}

int main()
{
	int peso;
	while(scanf("%d",&n)!=EOF)
	{
		for(int i=1;i<=n;i++)
		{
			peso=0;
			for(int j=1;j<=n;j++)
			{
				scanf("%d",&w[i][j]);
				peso+=w[i][j];
			}
			for(int j=1;j<=n;j++)
				w[i][j]=peso-w[i][j];
		}
		for(int i=1;i<=n;i++)
		{
			for(int j=1;j<=n;j++)
				w[i][j] = -w[i][j];
		}
		printf("%d\n",-KM());
	}
	return 0;
}
