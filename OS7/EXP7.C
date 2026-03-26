#include<stdio.h>
#include<string.h>
int main()
{
int alloc[10][10],max[10][10];
int avail[10],work[10],total[10];
int i,j,k,n,need[10][10];
int m;
int count=0,c=0;
char finish[10];
printf("Enter the no. of processes and resources:");
scanf("%d%d",&n,&m);
for(i=0;i<n;i++)  // Changed from <=n to <n
finish[i]='n';
printf("Enter the claim matrix:\n");
for(i=0;i<n;i++)
for(j=0;j<m;j++)
scanf("%d",&max[i][j]);
printf("Enter the allocation matrix:\n");
for(i=0;i<n;i++)
for(j=0;j<m;j++)
scanf("%d",&alloc[i][j]);
printf("Resource vector:");
for(i=0;i<m;i++)
scanf("%d",&total[i]);

// Calculate available resources
for(i=0;i<m;i++)
avail[i]=0; 
for(i=0;i<n;i++)
for(j=0;j<m;j++)
avail[j]+=alloc[i][j];

// Calculate work as total - allocated (available resources)
for(i=0;i<m;i++)
work[i]=total[i]-avail[i];  // Fixed calculation

// Calculate need matrix
for(i=0;i<n;i++)
for(j=0;j<m;j++)
need[i][j]=max[i][j]-alloc[i][j];

A:
for(i=0;i<n;i++)
{
if(finish[i]=='n')  // Only check unfinished processes
{
c=0;
for(j=0;j<m;j++)
if(need[i][j]<=work[j])
c++;
if(c==m)
{
printf("All the resources can be allocated to Process %d\n", i+1);
printf("Available resources are:");
for(k=0;k<m;k++)
{
work[k]+=alloc[i][k];
printf("%4d",work[k]);
}
printf("\n");
finish[i]='y';
printf("Process %d executed?:%c \n",i+1,finish[i]);
count++;
break;  // Break to restart from beginning after finding a process
}
}
}
if(count<n)
{
int allBlocked=1;
for(i=0;i<n;i++)
{
if(finish[i]=='n')
{
allBlocked=0;
break;
}
}
if(!allBlocked)
goto A;
}
if(count==n)
{
printf("\n System is in safe mode");
printf("\n The given state is safe state");
}
else
{
printf("\n System is not in safe mode");
}
return 0;
}