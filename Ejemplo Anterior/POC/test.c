#include<stdio.h>
//#include<conio.h>
#define MAX 30
void create(int set[]);
void print(int set[]);
void Union(int set1[],int set2[],int set3[]);
void intersection(int set1[],int set2[],int set4[]);
void difference(int set1[],int set2[],int set5[]);
void symmetric(int set1[],int set2[],int set6[]);
int member(int set[],int x);

int main()
{ int set1[MAX],set2[MAX],set3[MAX];
  int x,op;
  set1[0]=set2[0]=set3[0]=0;
  printf("-------------------------------------------------------------\n");
  printf("----------------made by C code champ ------------------------\n");
  printf("-------------------------------------------------------------\n");
  printf("\n\n\tMENU BASED C PROGRAM OF SET OPERATIONS \n");  
  do
   { 
     printf("\n1)Create two sets.");
     printf("\n2)Print the set.");
     printf("\n3)Union of two sets.");
     printf("\n4)Intersection of two sets A intesection B.");
     printf("\n5)Difference between two sets A - B");
     printf("\n6)Symmetrec Difference between two sets i.e (A-B)U(B-A).");
     printf("\n7)Exit from program\n\n");
     printf("\nEnter Your Choice : ");
     scanf("%d",&op);
     printf("\n\n");
     switch(op)
      {
    case 1: printf("\nCreate Set A: ");
        create(set1);
        printf("\nCreate Set B: ");
        create(set2);
        break;
    case 2: printf("\nSet A:\n");
        print(set1);
        printf("\n\nSet B:\n");
        print(set2);
        break;
    case 3: Union(set1,set2,set3);
            print(set3);
            break;
    case 4: intersection(set1,set2,set3);
            print(set3);
            break;
    case 5: difference(set1,set2,set3);
            print(set3);
            break;
    case 6: symmetric(set1,set2,set3);
            print(set3);
            break;
    case 7: break;
    default:
            printf("\nPlease Enter a valid choice\n");
            break;
     }
  printf("\n\nPress a key to continue......\n\n");
  //getch();
  }while(op!=7);
}

 void create(int set[])
   {   int n,i,x;
       set[0]=0;
       printf("\nNo. of elements in the set : ");
       scanf("%d",&n);
       printf("\nEnter set elements : \n");
       for(i=1;i<=n;i++)
       scanf("%d",&set[i]);
       set[0]=n;

   }

 void  print(int set[])
 { 
    int i,n;
    n=set[0];
    printf("\nMembers of the set :--> ");
    for(i=1;i<=n;i++)
       printf("%d  ",set[i]);
  }

void Union(int set1[],int set2[],int set3[])
  { int i,n;
    set3[0]=0;
    n=set1[0];
    for(i=0;i<=n;i++)
    set3[i]=set1[i];

    n=set2[0];
    for(i=1;i<=n;i++)
       if(!member(set3,set2[i]))
        set3[++set3[0]]=set2[i];
   }

int member(int set[],int x)
{   
    int i,n;
    n=set[0];
    for(i=1;i<=n;i++)
    if(x==set[i])
    return(1);
    return(0);
  }

void intersection(int set1[],int set2[],int set3[])
     {
    int i,n;
    set3[0]=0; 
    n=set1[0];
    for(i=1;i<=n;i++)
      if(member(set2,set1[i])) 
           set3[++set3[0]]=set1[i];  
     }

void difference(int set1[],int set2[],int set3[])
{   
    int i,n;
    n=set1[0];
    set3[0]=0;
    for(i=1;i<=n;i++)
       if(!member(set2,set1[i]))
         set3[++set3[0]]=set1[i];  
      }

void symmetric(int set1[],int set2[],int set3[])
{   
    int i,n;
    n=set1[0];
    set3[0]=0;
    //Calculate set1-set2
    for(i=1;i<=n;i++)
       if(!member(set2,set1[i]))
         set3[++set3[0]]=set1[i];  
    //Calculate set2-set1
    n=set2[0];
    for(i=1;i<=n;i++)
       if(!member(set1,set2[i]))
         set3[++set3[0]]=set2[i];  

      }