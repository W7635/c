//世界各国专利申请量排名管理系统 
//双重索引，type一层，index_va（申请量排名）一层，index_gr是增幅排名的索引 
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#define LISTINCREMENT 10          	   
#define ERROR   0
#define OK   1
#define TRUE  1
#define FALSE 0
#define MAXSIZE  150   				
int k1=0,k2=0,k3=0,k4=0;
typedef int Status;
typedef int KeyType;   				
typedef  struct { 
	char     country[30];        	//国家名称 
	int      country_type;       	//国家类型，0代表低收入国家，1代表中低等收入国家，2代表中高等收入国家，3代表高收入国家 
	KeyType  value[25];  	        //历年专利申请量 
	int 	 year[25];	         	//年度，时间跨度为25年,样例数据中为1999到2019年数据 
	int      growth[25];  	 	    //专利申请量增幅 
	int 	 index_va[25];          //专利申请量历年排名中的索引 
	int 	 index_gr[25]; 			//专利申请量增幅历年排名中的索引  
}RecType;
typedef  struct{
    RecType   r[MAXSIZE+1];   		//r[0]闲置
    int       index_l[MAXSIZE+1];   //存储低收入国家索引值 
    int  	  index_ml[MAXSIZE+1];	//存储中低收入国家索引值 
    int       index_mh[MAXSIZE+1];	//存储中高收入国家索引值 
    int       index_h[MAXSIZE+1];   //存储高收入国家索引值 
    int       length;        		//顺序表长度     
}SqList;   

int NPA_Menu_Show()
//显示菜单选项，读取菜单选择输入，判断其输入值是否在0-7之间，NPA：Number of Patent Applications
{	int key,flag=1;
	printf("\n===============世界各国专利申请量排名管理系统=================\n");
	printf("*                                                            *\n");
	printf("*  1:原始数据导入     2:原始数据查询      3:原始数据修改     *\n");
	printf("*  4:申请量增幅计算   5:申请量排名        6:增幅排名         *\n");
	printf("*  7:排名查询	      8:排名分析          9:排名结果保存     *\n");
	printf("*  0：退出                                                   *\n");
	printf("*                                                            *\n");	
	printf("==============================================================\n");	
	printf("根据菜单提示进行输入：");	
	while(flag)
	{ scanf("%d",&key);
	  if(key>=0&&key<=9)
	  {	flag=0;
	  	return key;
	  }
	  else
	  	printf("菜单选择输入错误，请重新输入：");
	}
} 
 
int NPA_SqList_Read(SqList &L,char fileName[]) 
//原始数据导入 
{   FILE *fp = NULL;
	int i,k,n=0;														//从L.r[n] 从n=1开始记录数据 
	int flag=1;   
	char  temp_country_type[30];
	if ((fp=fopen(fileName,"r"))==NULL)
			{ printf("文件打开失败!"); 			  
			}
	else
			{ while(flag&&!feof(fp))			 
			  {	n++;
				fscanf(fp,"%s %s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", L.r[n].country,temp_country_type,\
				    &L.r[n].value[0],&L.r[n].value[1],&L.r[n].value[2],&L.r[n].value[3],&L.r[n].value[4],\
					&L.r[n].value[5],&L.r[n].value[6],&L.r[n].value[7],&L.r[n].value[8],&L.r[n].value[9],\
					&L.r[n].value[10],&L.r[n].value[11],&L.r[n].value[12],&L.r[n].value[13],&L.r[n].value[14],\
					&L.r[n].value[15],&L.r[n].value[16],&L.r[n].value[17],&L.r[n].value[18],&L.r[n].value[19],\
					&L.r[n].value[20]);					
				if(strcmp(temp_country_type,"低收入国家")==0)
				 	{	L.r[n].country_type=0;
				 		k1++;
			 			L.index_l[k1]=n;   								//根据收入水平将国家进行分类,用于存储不同类别国家的索引
					} 
				else if(strcmp(temp_country_type,"中低等收入国家")==0)
				 	{	L.r[n].country_type=1;
						k2++;
			 			L.index_ml[k2]=n;							
					}
				else if(strcmp(temp_country_type,"中高等收入国家")==0)
				 	{	L.r[n].country_type=2;
				 		k3++;
			 			L.index_mh[k3]=n;								
					}
				else if(strcmp(temp_country_type,"高收入国家")==0)
				 	{	L.r[n].country_type=3;
				 		k4++;
		  	 			L.index_h[k4]=n;								
					}	
				for(i=0;i<=20;i++)
				 	L.r[n].year[i]=1999+i;    							//从1999年到2019年 					  	   				 									 							 								 							
				if(n>150)   											//超出数据条数的最大值 
					flag=0;		 			 		 
			 	} 			   
			   fclose(fp);
			   L.length=n-1;  											//去除第一个r[0]，最后一个n++ 
			   return OK;		
           }
}

int NPA_SqList_Search(SqList &L)
//原始数据查询，根据国家名称和年度进行查询 
{	char country_name[30];
	int i,year; 
	printf("输入要查询的国家名称\n");
 	scanf("%s/n",country_name);
 	printf("输入要查询的年度(1999--2019)\n");
 	scanf("%d/n",&year);
 	if(year<1999||year>2019)
 		printf("输入查询的年度错误,范围为1999--2019\n");
	else
	{for(i=1;i<=L.length;i++) 	  
	   if(strcmp(country_name,L.r[i].country)==0)
 			break;  
	if(i>L.length)
		printf("输入要查询的国家名称错误\n");	 
	else 	  
		{	printf("%s%d年,专利申请量为%d件,增幅为%d件\n",country_name,year,L.r[i].value[year-1999],L.r[i].growth[year-1999]);
	     	return OK; 
		} 
	 }	 
} 

int NPA_SqList_Revise(SqList &L)
//原始数据修改，根据国家名称和年度进行查询 
{	char country_name[30];
	int i,j,year,temp,loc,k,max; 
	printf("输入要修改的国家名称\n");
 	scanf("%s/n",country_name);
 	printf("输入要修改的年度(1999--2019)\n");
 	scanf("%d/n",&year);
 	if(year<1999||year>2019)
 		printf("输入修改的年度错误,范围为1999--2019\n");
	else
	{for(i=1;i<=L.length;i++) 	            											//找到国家索引 
	   if(strcmp(country_name,L.r[i].country)==0)
 			break; 
	loc=i;
	k= year-1999;
	if(loc>L.length)
		printf("输入要修改的国家名称错误\n");	 
	else 	  
		{	printf("输入修改后专利申请量值：\n");
		 	scanf("%d",&temp); 
		 	L.r[loc].value[k]=temp;     												//更新当年专利申请量 
		 	if(k==0)        	       													//更新当年和专利申请量增幅 
          	 	L.r[loc].growth[0]=0;    
        	else
		  	 	L.r[loc].growth[k]= L.r[loc].value[k]-L.r[loc].value[k-1];	 			//计算增幅
		 	//更新下一年专利申请量增幅
		 	L.r[loc].growth[k+1]= L.r[loc].value[k+1]-L.r[loc].value[k];	    		
		 	printf("%s%d年,专利申请量更改为%d,增幅为%d\n",country_name,year,L.r[loc].value[k],L.r[loc].growth[k]);	      
		} 
	 } 
	 //更新该年度专利申请量排名，冒泡排序 
	 {for(i=1;i<=L.length;i++)     														
            { //每趟排序开始时设置交换标志变量值为假
             for(j=1;j<=L.length - i;j++)                   
         		if (L.r[L.r[j].index_va[k]].value[k]<L.r[L.r[j+1].index_va[k]].value[k])//如果小于后者，则交换（索引） 
           			{	temp=L.r[j].index_va[k];    
             			L.r[j].index_va[k]=L.r[j+1].index_va[k];
             			L.r[j+1].index_va[k]=temp;                      		  
           			}
     	    }    	   	 	
 	}		 
	 //更新该年和下一年度专利申请量增幅排名	 
	 for(;k<=year-1999+1;k++)	
	 {  //查看该国家类型 
	    if(L.r[loc].country_type==0)
	    //选择排序 
			{	for(i=1; i<k1; i++)   
      				{   max=i;                											//假设无序子表中的第一条记录的关键字最小
         				for(j=i+1; j<=k1; ++j)
     	     				if(L.r[L.r[L.index_l[j]].index_gr[k]].growth[k] > L.r[L.r[L.index_l[max]].index_gr[k]].growth[k])  //比较关键字值		    
     	         				max=j;
         				if(max!=i)          											//如果最小关键字记录不在无序子表的第一个位置，则交换
        					{	temp=L.r[L.index_l[i]].index_gr[k];    
            					L.r[L.index_l[i]].index_gr[k]=L.r[L.index_l[max]].index_gr[k];
            					L.r[L.index_l[max]].index_gr[k]=temp;       								        
       						} 
     				}			  				 										//低收入国家排名更新			 
			}
		else if(L.r[loc].country_type==1)	
			{	for(i=1; i<k2; i++)   
      				{   max=i;                											//假设无序子表中的第一条记录的关键字最小
         				for(j=i+1; j<=k2; ++j)
     	     				if(L.r[L.r[L.index_ml[j]].index_gr[k]].growth[k] > L.r[L.r[L.index_ml[max]].index_gr[k]].growth[k])  //比较关键字值		    
     	         				max=j;
        				 if(max!=i)          											//如果最小关键字记录不在无序子表的第一个位置，则交换
        					{	temp=L.r[L.index_ml[i]].index_gr[k];    
            				 	L.r[L.index_ml[i]].index_gr[k]=L.r[L.index_ml[max]].index_gr[k];
             					L.r[L.index_ml[max]].index_gr[k]=temp;       								        
       						} 
     				}			 														//中低收入国家排名更新 			 
			}
		else if(L.r[loc].country_type==2)
			{	for(i=1; i<k3; i++)   
      				{  max=i;               										 	//假设无序子表中的第一条记录的关键字最小
         				for(j=i+1; j<=k3; ++j)
     	     				if(L.r[L.r[L.index_mh[j]].index_gr[k]].growth[k] > L.r[L.r[L.index_mh[max]].index_gr[k]].growth[k])  //比较关键字值		    
     	        				max=j;
         				if(max!=i)          											//如果最小关键字记录不在无序子表的第一个位置，则交换
        					{	temp=L.r[L.index_mh[i]].index_gr[k];    
             			  	 	L.r[L.index_mh[i]].index_gr[k]=L.r[L.index_mh[max]].index_gr[k];
            				 	L.r[L.index_mh[max]].index_gr[k]=temp;       								        
       						} 
      				}   																//中高收入国家排名更新      	              	 	   						 
			}
		else if(L.r[loc].country_type==3)
			{	for(i=1; i<k4; i++)   
     				 {  max=i;                											//假设无序子表中的第一条记录的关键字最小
        				for(j=i+1; j<=k4; ++j)
     	     				if(L.r[L.r[L.index_h[j]].index_gr[k]].growth[k] > L.r[L.r[L.index_h[max]].index_gr[k]].growth[k])  //比较关键字值		    
     	         				max=j;
         				if(max!=i)          											//如果最小关键字记录不在无序子表的第一个位置，则交换
        					{	temp=L.r[L.index_h[i]].index_gr[k];    
             					L.r[L.index_h[i]].index_gr[k]=L.r[L.index_h[max]].index_gr[k];
             					L.r[L.index_h[max]].index_gr[k]=temp;       								        
       						} 
      				}																	//高收入国家排名更新			 
			}	 		
	 }	 
} 

int  NPA_SqList_Calculate(SqList &L)
//计算专利申请量增幅，相比于上一年 
{	int i,k;  																			//k为年度变量 
	for(k=0;k<=20;k++)  																//从2000年到2019年，共20年 
 	{	for(i=1;i<=L.length;i++)     	
          if(k==0)            															//上一年为0 
          	 L.r[i].growth[k]=0;    
          else
		  	 L.r[i].growth[k]= L.r[i].value[k]-L.r[i].value[k-1];	    				//计算增幅                                 	      	   	 	
 	} 
	return OK; 	 	
} 

void NPA_SqList_Sort_V(SqList &L)
//采用冒泡法排序，索引方式实现，默认对所有年度各个国家专利申请量进行排序 
{	int i,j,k,temp; 
    for(k = 0; k <= 20; k++) 
		{for(i = 1; i <= L.length; i++)
	    	L.r[i].index_va[k] = i;  													//遍历给索引数组赋值为原数组的下标
		}      
 	for(k = 0; k <=20;k++)
 		{for(i=1;i<=L.length;i++)     													
            { //每趟排序开始时设置交换标志变量值为假
             for(j=1;j<=L.length - i;j++)                   
         		if (L.r[L.r[j].index_va[k]].value[k]<L.r[L.r[j+1].index_va[k]].value[k]) //如果小于后者，则交换
           			{ 	temp=L.r[j].index_va[k];    
             		  	L.r[j].index_va[k]=L.r[j+1].index_va[k];
             		  	L.r[j+1].index_va[k]=temp;                      		  
           			}
     	    }    	   	 	
 	    } 
	//显示排名结果
	for(k=0;k<=20;k++)
	{printf("\n%d年世界各国专利申请量排名\n",L.r[1].year[k]); 
	 printf("序号.            国家名称     专利申请量(件)\n");
	 for(i=1;i<=L.length;i++)         	 
		printf("%2d.  %20s    %6d\n",i,L.r[L.r[i].index_va[k]].country, L.r[L.r[i].index_va[k]].value[k]);			
	}        	
}

void NPA_SqList_Sort_G(SqList &L)
//采用直接选择排序，索引方式实现，默认对所有年度各个国家专利申请量增幅分类进行排序 
{	int i,j,k,temp,max;      	 		
	for(k = 0; k <= 20; k++) 		      
	{  for(i=1;i<=L.length;i++)
		  L.r[i].index_gr[k]=i;      													//对每个分类的国家排序序列进行初始化（整个序列中的顺序 ）									 
	}  	 	  	
 	for(k = 0; k<=20; k++)   
 	{ //对低收入国家历年进行排序 	
	  for(i=1; i<k1; i++)   
      {  max=i;                															//假设无序子表中的第一条记录的关键字最小
         for(j=i+1; j<=k1; ++j)
     	     if(L.r[L.r[L.index_l[j]].index_gr[k]].growth[k] > L.r[L.r[L.index_l[max]].index_gr[k]].growth[k])  //比较关键字值		    
     	        max=j;
         if(max!=i)          															//如果最小关键字记录不在无序子表的第一个位置，则交换
        {	temp=L.r[L.index_l[i]].index_gr[k];    
            L.r[L.index_l[i]].index_gr[k]=L.r[L.index_l[max]].index_gr[k];
            L.r[L.index_l[max]].index_gr[k]=temp;       								        
       	} 
     }
     //对中低收入国家历年进行排序
	 for(i=1; i<k2; i++)   
      {  max=i;                															//假设无序子表中的第一条记录的关键字最小
         for(j=i+1; j<=k2; ++j)
     	     if(L.r[L.r[L.index_ml[j]].index_gr[k]].growth[k] > L.r[L.r[L.index_ml[max]].index_gr[k]].growth[k])  //比较关键字值		    
     	         max=j;
         if(max!=i)          															//如果最小关键字记录不在无序子表的第一个位置，则交换
         {	temp=L.r[L.index_ml[i]].index_gr[k];    
            L.r[L.index_ml[i]].index_gr[k]=L.r[L.index_ml[max]].index_gr[k];
            L.r[L.index_ml[max]].index_gr[k]=temp;       								        
       	 } 
     }		 			    		 	
     //对中高收入国家历年进行排序 
     for(i=1; i<k3; i++)   		
      {  max=i;                 														//假设无序子表中的第一条记录的关键字最小
         for(j=i+1; j<=k3; ++j)
     	     if(L.r[L.r[L.index_mh[j]].index_gr[k]].growth[k] > L.r[L.r[L.index_mh[max]].index_gr[k]].growth[k])  //比较关键字值		    
     	         max=j;
         if(max!=i)            															//如果最小关键字记录不在无序子表的第一个位置，则交换
         {	temp=L.r[L.index_mh[i]].index_gr[k];    
            L.r[L.index_mh[i]].index_gr[k]=L.r[L.index_mh[max]].index_gr[k];
            L.r[L.index_mh[max]].index_gr[k]=temp;       								        
       	 } 
      }        	              	 
	  //对高收入国家历年进行排序
	  for(i=1; i<k4; i++)   	
      {  max=i;                 														//假设无序子表中的第一条记录的关键字最小
         for(j=i+1; j<=k4; ++j)
     	     if(L.r[L.r[L.index_h[j]].index_gr[k]].growth[k] > L.r[L.r[L.index_h[max]].index_gr[k]].growth[k])  //比较关键字值		    
     	         max=j;
         if(max!=i)          															//如果最小关键字记录不在无序子表的第一个位置，则交换
         {	temp=L.r[L.index_h[i]].index_gr[k];    
            L.r[L.index_h[i]].index_gr[k]=L.r[L.index_h[max]].index_gr[k];
            L.r[L.index_h[max]].index_gr[k]=temp;       								        
       	 } 
      }        		  		   	   	 	
 	} 
	//显示增速排名结果
	for(k=0;k<=20;k++)
	{	printf("\n%d年低收入国家专利申请量增幅排名\n",L.r[L.index_l[1]].year[k]); 
	 	printf("序号.            国家名称    专利申请量增幅（件）\n");
	 	for(i=1;i<=k1;i++)         	 
			printf("%2d.  %20s    %6d \n",i,L.r[L.r[L.index_l[i]].index_gr[k]].country,L.r[L.r[L.index_l[i]].index_gr[k]].growth[k]);				
	}  
	printf("*************************************************************************\n"); 
	for(k=0;k<=20;k++)
	{	printf("\n%d年中低收入国家专利申请量增幅排名\n",L.r[L.index_ml[1]].year[k]); 
	 	printf("序号.            国家名称    专利申请量增幅（件）\n");
	 	for(i=1;i<=k2;i++)         	 
			printf("%2d.  %20s    %6d \n",i,L.r[L.r[L.index_ml[i]].index_gr[k]].country,L.r[L.r[L.index_ml[i]].index_gr[k]].growth[k]);					
	}
	printf("*************************************************************************\n");
	for(k=0;k<=20;k++)
	{	printf("\n%d年中高收入国家专利申请量增幅排名\n",L.r[L.index_mh[1]].year[k]); 
	 	printf("序号.            国家名称    专利申请量增幅（件）\n");
	 	for(i=1;i<=k3;i++)         	 
			printf("%2d.  %20s    %6d \n",i,L.r[L.r[L.index_mh[i]].index_gr[k]].country,L.r[L.r[L.index_mh[i]].index_gr[k]].growth[k]);				
	}
	printf("*************************************************************************\n");
	for(k=0;k<=20;k++)
	{	printf("\n%d年高收入国家专利申请量增幅排名\n",L.r[L.index_h[1]].year[k]); 
	 	printf("序号.            国家名称    专利申请量增幅（件）\n");
	 	for(i=1;i<=k4;i++)         	 
			printf("%2d.  %20s    %6d \n",i,L.r[L.r[L.index_h[i]].index_gr[k]].country,L.r[L.r[L.index_h[i]].index_gr[k]].growth[k]);					
	}      	
}

void NPA_SqList_Query(SqList &L)
//根据国家名称查询排名
{	char country_name[30];
	int i,loc,sort_va[21],sort_gr[21],k; 
	printf("输入要查询的国家名称\n");
 	scanf("%s/n",country_name);
 	for(i=1;i<=L.length;i++) 	  
	   if(strcmp(country_name,L.r[i].country)==0)
 			break;
 	if(i>L.length)
 		printf("输入要查询的国家名称错误！\n");
	else
	{ 		
 		loc = i;
 		for(k=0;k<=20;k++)   																//查找历年排名，记录到sort_va和sort_gr中 
 			for(i=1;i<=L.length;i++)
 		 	{	if(L.r[i].index_va[k]==loc)
 		 			sort_va[k]=i;
 		 		if(L.r[i].index_gr[k]==loc)
 		 			sort_gr[k]=i;			
	 	 	}
 		for(i=0;i<=20;i++)																	//输出专利申请量各年度排名 
 	    	printf("%s%d年专利申请量全世界排名为%d,对应收入水平国家中增幅排名为%d \n",country_name,L.r[loc].year[i],sort_va[i],sort_gr[i]); 
 	} 
} 

int NPA_SqList_Analyze(SqList &L)
//排名分析 
{	char country_name[30];
	int i,j,loc;
	float sum_gr=0,sum_va=0,var_gr,var_va,min_gr,max_gr,min_va,max_va; 
	printf("输入要查询的国家名称\n");
 	scanf("%s/n",country_name);	
	for(i=1;i<=L.length;i++) 																//匹配对应的国家  
	   if(strcmp(country_name,L.r[i].country)==0)
 			break;  
 	loc=i;		
	if(loc>L.length)
		printf("输入要查询的国家名称错误\n");	 
	else 	  
		{//计算专利申请量和增幅的最值、均值
		 min_gr=max_gr=L.r[loc].growth[0];
		 min_va=max_va=L.r[loc].value[0];
		 for(i=0;i<=20;i++)
		 {	sum_gr=L.r[loc].growth[i]+sum_gr;
		  	sum_va=L.r[loc].value[i]+sum_va;		  
		  	if(L.r[loc].growth[i]<min_gr)
		  	  	min_gr=L.r[loc].growth[i];
		  	if(L.r[loc].growth[i]>max_gr)
		  	  	max_gr=L.r[loc].growth[i];	
		  	if(L.r[loc].value[i]<min_va)
		  	 	min_va=L.r[loc].value[i];
		  	if(L.r[loc].value[i]>max_va)
		  	  	max_va=L.r[loc].value[i];	
		 }
		 sum_gr=1.0*sum_gr/21;
		 sum_va=1.0*sum_va/21;
		 for(i=0;i<=20;i++)
		 {	var_gr=var_gr+(L.r[loc].growth[i]-sum_gr)*(L.r[loc].growth[i]-sum_gr);
		  	var_va=var_va+(L.r[loc].value[i]-sum_va)*(L.r[loc].value[i]-sum_va);	
		 }
		 var_gr=var_gr/21;
		 var_va=var_va/21;
		 printf("%s,1999-2019年专利申请量最小值为：%.2f,最大值为：%.2f,均值为：%.2f,方差为：%.2f,增幅最小值为：%.5f,最大值为：%.5f,均值为：%.5f,方差为：%.8f\n",country_name,\
		        min_va,max_va,sum_va,var_va,min_gr,max_gr,sum_gr,var_gr);
	     return OK; 
		}  	
} 

void NPA_SqList_Save(SqList &L,char fileName[],char fileName_saved_va[],char fileName_saved_gr[])
//保存排序结果到本地磁盘 
{ 	FILE *fp = NULL;
	int i,num_char,k,num_year;	  	
	num_char=strlen(fileName);
	strncpy(fileName_saved_va,fileName,num_char-4);
	strncpy(fileName_saved_gr,fileName,num_char-4);
	strcat(fileName_saved_va,"_Sorted.txt");  		 	 									//保存增加值排名结果到磁盘txt文本文件中，名称为当前txt文本文档增加后缀 _Sorted
	strcat(fileName_saved_gr,"_Grouped _Sorted.txt");  	 									//保存增速排名结果到磁盘txt文本文件中，名称为当前txt文本文档增加_Grouped _Sorted	
	if ((fp=fopen(fileName_saved_va,"w"))==NULL)
			printf("文件打开失败!"); 			
	else
	{   //保存结果到磁盘txt文本文件中 
	    for(k=0;k<=20;k++)
		{	fprintf(fp,"\n%d年世界各国专利申请量排名\n",L.r[1].year[k]); 
	 	 	fprintf(fp,"序号.            国家名称    专利申请量(件数)\n");
	 	 	for(i=1;i<=L.length;i++)         	 
				fprintf(fp,"%2d.  %20s    %6d\n",i,L.r[L.r[i].index_va[k]].country, L.r[L.r[i].index_va[k]].value[k]);			
		}  					
		fclose(fp);
	}
	if ((fp=fopen(fileName_saved_gr,"w"))==NULL)
			printf("文件打开失败!"); 			
	else
	{   //保存结果到磁盘txt文本文件中 
	    for(k=0;k<=20;k++)
		{	fprintf(fp,"\n%d年低收入国家专利申请量增幅排名\n",L.r[L.index_l[1]].year[k]); 
	 	 	fprintf(fp,"序号.            国家名称    专利申请量增幅\n");
	 		for(i=1;i<=k1;i++)         	 
				fprintf(fp,"%2d.  %20s    %6d \n",i,L.r[L.r[L.index_l[i]].index_gr[k]].country,L.r[L.r[L.index_l[i]].index_gr[k]].growth[k]);				
		}  
		fprintf(fp,"*************************************************************************\n"); 
		for(k=0;k<=20;k++)
		{	fprintf(fp,"\n%d年中低收入国家专利申请量增幅排名\n",L.r[L.index_ml[1]].year[k]); 
	 	 	fprintf(fp,"序号.            国家名称    专利申请量增幅\n");
	 	 	for(i=1;i<=k2;i++)         	 
				fprintf(fp,"%2d.  %20s    %6d \n",i,L.r[L.r[L.index_ml[i]].index_gr[k]].country,L.r[L.r[L.index_ml[i]].index_gr[k]].growth[k]);					
		}
		fprintf(fp,"*************************************************************************\n");
		for(k=0;k<=20;k++)
		{	fprintf(fp,"\n%d年中高收入国家专利申请量增幅排名\n",L.r[L.index_mh[1]].year[k]); 
	 	 	fprintf(fp,"序号.            国家名称    专利申请量增幅\n");
	 	 	for(i=1;i<=k3;i++)         	 
				fprintf(fp,"%2d.  %20s    %6d \n",i,L.r[L.r[L.index_mh[i]].index_gr[k]].country,L.r[L.r[L.index_mh[i]].index_gr[k]].growth[k]);				
		}
		fprintf(fp,"*************************************************************************\n");
		for(k=0;k<=20;k++)
		{	fprintf(fp,"\n%d年高收入国家专利申请量增幅排名\n",L.r[L.index_h[1]].year[k]); 
	 	 	fprintf(fp,"序号.            国家名称    专利申请量增幅\n");
	 	 	for(i=1;i<=k4;i++)         	 
				fprintf(fp,"%2d.  %20s    %6d \n",i,L.r[L.r[L.index_h[i]].index_gr[k]].country,L.r[L.r[L.index_h[i]].index_gr[k]].growth[k]);					
		} 
		fclose(fp);
	}						
}

int main(){      	
   	char fileName[300],fileName_saved_va[310],fileName_saved_gr[310];
   	int status,year,flag=1,key;
   	SqList L; 																						//定义线性表，分配预定义大小的空间     	    
 	while(flag)
	  {	key=NPA_Menu_Show(); 																		//读取键盘的输入 
		switch(key){
			case 1:   printf("请输入读取文件路径和名称：\n");
					  scanf("%s",fileName);					  
					  getchar(); 																	//去掉输入流中的回车符
					  status = NPA_SqList_Read(L,fileName);   										//读取数据
					  if(status==OK)
					  	printf("数据读取完成，根据提示进行操作。\n");
					  break;
			case 2:   if(L.r[1].year[0]==0)
						 printf("请先读入数据！\n");	
					  else
					  	 NPA_SqList_Search(L);
					  break;	  					  	  					  			 					     					  		
			case 3:   if(L.r[1].year[0]==0)
						 printf("请先读入数据！\n");	
					  else
						 {	NPA_SqList_Revise(L);	
						    printf("专利申请量修改完成，根据提示进行操作。\n");									 
						 } 
					  break;
			case 4:   if(L.r[1].year[0]==0)
						 printf("请先读入数据！\n");	
					  else
						{status = NPA_SqList_Calculate(L);   
					 	 if(status==OK)
					  		printf("专利申请量增幅计算完成，根据提示进行操作。\n");
						}	
					  break;
			case 5:   if(L.r[1].year[0]==0)
						 printf("请先读入数据！\n");	
					  else
						{NPA_SqList_Sort_V(L);					       	
					  	 printf("专利申请量排名完成，根据提示进行操作。\n");	
					    }	
					  break;		  		  
			case 6:   if(L.r[1].year[0]==0)
						 printf("请先读入数据！\n");	
					  else
						{NPA_SqList_Sort_G(L);						 	
				         printf("专利申请量增幅排名完成，根据提示进行操作。\n");	
				        } 
				      break;	
			case 7:   if(L.r[1].year[0]==0)
						 printf("请先读入数据！\n");	
					  else
						{ NPA_SqList_Query(L);
						  printf("排名查询完成，根据提示进行操作。\n");
					    } 
				      break;
			case 8:   if(L.r[1].year[0]==0)
						 printf("请先读入数据！\n");	
					  else
						{ NPA_SqList_Analyze(L);
						  printf("排名分析完成，根据提示进行操作。\n");
						  
					    }
					  break;  
			case 9:   if(L.r[1].year[0]==0)
						 printf("请先读入数据！\n");	
					  else
						{ NPA_SqList_Save(L,fileName,fileName_saved_va,fileName_saved_gr);
						  printf("排名分析完成，根据提示进行操作。\n");						  
			              printf("排名结果保存完成，申请量排名保存路径为：\n");	
			              printf("%s\n",fileName_saved_va);	
			              printf("增幅排名保存路径为：\n");	
			              printf("%s\n",fileName_saved_gr);
					    }
					  break;			    
			case 0:   flag=0;
					  printf("退出系统。");
					  break;	  	  			  						  	 					  
			default:  printf("菜单选择输入错误，请重新输入");
		}   			 	  	  
	 }	 
	return OK; 	  		     	
}



