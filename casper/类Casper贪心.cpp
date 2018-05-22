#include<bits/stdc++.h>
using namespace std;
/*类casper贪心 
UID代表移动用户的身份标识；数字代表用户1-1000 
CID代表网格区域中最底层单元格编号；所求（100个时刻 每个时刻每个用户） 
location表示的是移动用户的地理位置信息；（xy）坐标上有数就代表有xy ，不需要单独定义 

profile表示移动用户的隐私需求属性： K和A 
K代表一名区域的最小匿名度（即最小用户个数）
A表示匿名区域的最小面积。
1000ge 分布在120*120例 14400例  14个有1个  最小方阵15*15有 225 有16个 K=0-300？ 

1.初始化 120*120xy网格  100个时刻1000个节点     ----100个xy坐标图（二维数组） 
每个节点是一个结构体数组的一个，面包含CID，K和A；用vector（）包括0-100时刻的 CID
2.网格内用户数目不够按照坐标轴顺时针增加下一个网格     下图1234顺序，小的满4个存入上一层CID 
  1 * 2
 *******
  3 * 4
*/
//quadrang为坐标轴上某节点锁在网格表示的x y范围 （该网格左下角的坐标） 
struct quadrang{
	int x;
	int y;
};
//quadnode为四叉树节点，表示64个最小网格，16个 次小网格，4个大网格
//quadrang为每个网格左下角坐标
//userct为该网格内用户总数
//layer为层数，网格是第几及网格
//child【】数组表示非叶子节点的孩子下标为4个
//fa表示该节点的父节点下标 
struct quadnode{
	quadrang rang;
	int userct;
	int layer;
	int child[5];
    int fa;
};
//！！ 
struct cidtemp{
	int quadnodeid;
	int userct;
}cidTemp[4];
//cidtemp cidTemp[4];
//新建450个四叉树节点，因为每层所需节点为
//0表示整个区域 
// 1 2 3 4 表示最大网格 
//11 12 13 14 21 22 23 24 31 32 33 34 41 41 43 44 表示次大网格 
//111 112 113 114 121 122 123 124.....441 442 443 444表示最小网格 
//所需最大节点下标为444，所以新建450个够了 
quadnode Quadnode[450];
//node为用户节点 
//每个用户有个所要求的附近用户数K和所需最小区域大小A 是不变的
//最小匿名区域CID和每个用户所在坐标范围存放在vector里，因为每个节点有100个时刻的信息 
struct node{
	vector<int>CID[110];//3个就够 
	int nowK[110]; 
	int nowA[110];
	vector<quadrang>rang; 
	int K;
	int A; 
}Node[1010];
//t表示某时刻 
int t;
//1grid为00个时刻的120*120坐标内的节点分布 
int grid[110][130][130];
//cid用于adjust_cid函数整理CID用 
vector<int>cid; 
//random生成某个范围随机数的函数 
int random(int low,int high){
	int result=0;
	result= low+rand()%(high-low+1);
	return result;
}
//init_user初始化用户 
void init_user(){
	//100个时刻，1000个用户，每个用户每个时刻在120*120的网格上设置节点位置 
	for(int t=1;t<=100;++t){
		for(int i=1;i<=1000;++i){
	    	while(1){
		    	int a=random(1,120);
		    	int b=random(1,120);
		     	if(grid[t][a][b]==0){
			    	grid[t][a][b]=i;
			    	break;
		    	}
			//cout<<"haha"<<"\n";
	    	}
		//cout<<"heihei"<<"\n";
    	}
	}
	//cout<<"！！"<<"\n"; 
	//1-1000个用户节点只需要最开始设置一次K和A 
	for(int i=1;i<=1000;++i){
		Node[i].K=random(1,35);
		Node[i].A=random(1,5);
	} 
}
//init_quadtree_userct初始化四叉树 
//计算每个用户所在网格的用户数和所表示范围，并把他放在对应四叉树里，建树 
void  init_quadtree_userct(){
	//   60  60为坐标中心 以下分别为每层网格中心坐标  最后一个-15的是最底层网格左下角坐标 
	//  x-30,y+30  x+30,y+30  x-30,y-30  x+30,y-30
	//  x-15，y+15   x+15，y+15   x-15，y-15   x+15，y-15 
	//  x-15，y   x，y   x-15,y-15    x,y-15 
	//放在数组里动态改变网格代表的范围坐标 
	int X1[4]={-30,30,-30,30};  //1234
	int Y1[4]={30,30,-30,-30};
	
	int X2[4]={-15,15,-15,15};//1234
	int Y2[4]={15,15,-15,-15};
	
	int X3[4]={-15,0,-15,0};//1234
	int Y3[4]={0,0,-15,-15};
	//x y记录每个网格范围 
	int x,y;
	//gridNum网格数目   ++用 
//	int gridNum=1;
	//根结点下标为0 ，根节点无父节点，所以设置为-1 
	int root=0;
	Quadnode[root].fa=-1;
	//三层循环直到把坐标轴划分到最底层，每层循环后要恢复坐标，所以加上的要减去 
	//这个循环把网格的范围计算和网格内节点在四叉树里的父子关系都计算了
	//id表示儿子节点下标 
	for(int i=0;i<4;++i){
		x=60+X1[i];
		y=60+Y1[i];
		//开始设置id为0，4次循环他有四个儿子 
		int id=0;
		id=id*10+i+1;
        //最开始是设置根节点的儿子和设置他儿子的父节点是他 
		Quadnode[root].child[i+1]=id;
		Quadnode[id].fa=root;
		//每个儿子节点在设置他的儿子和他的关系，以及表示范围，所以设置now为id表示现在要遍历的当前节点
		int now=id;
		for(int j=0;j<4;++j){
			x=x+X2[j];
			y=y+Y2[j];
			
			id=id*10+j+1;
			Quadnode[now].child[j+1]=id;
			Quadnode[id].fa=now;
			now=id;
			for(int k=0;k<4;++k){
				x=x+X3[k];
				y=y+Y3[k];
				//cout<<x<<" "<<y<<"\n"; 
				//nowx nowy表示最小网格左下角坐标，用于统计网格内每个小点坐标，需要改变 
				int nowx=x,nowy=y;
				// 每个网格内的用户数 
				int userct=0;  
				for(int m=0;m<15;++m){
					for(int n=0;n<15;++n){
						//如果某时刻该网格内某位置不为0，表示该位置有用户，数目++ 
						if(grid[t][nowx+m][nowy+n]!=0){
							userct++;
						}
					}
				} 
				//计算儿子节点id 
				id=id*10+k+1;
				Quadnode[now].child[k+1]=id;
				Quadnode[id].fa=now;
				//把刚才计算的该网格的范围xy和用户数存入对应的四叉树节点 
				Quadnode[id].userct=userct;
				Quadnode[id].rang.x=x;
				Quadnode[id].rang.y=y; 
				//恢复id 
				id=(id-k-1)/10; 
				//cout<<ct<<" "; 
				x=x-X3[k];
				y=y-Y3[k];
			}
			x=x-X2[j];
			y=y-Y2[j]; 
			//恢复id 
			id=(id-j-1)/10; 
			//恢复父节点 ！！！！ 
			now=now/10; 
		}
		x=x-X1[i];
		y=y-Y1[i]; 
		//恢复id 
		id=(id-i-1)/10; 
	}
}
//init_layer初始化四叉树节点的层次，并计算非底层节点的用户数和范围 
void init_layer(){
	//设置四叉树儿子节点用户数 
	int childuserct;
	//以下循环计算非底层节点的用户数，即把他的儿子节点用户数相加 
	//每个节点遍历 ，从大到小方便从最小网格开始 
	//把1  2层节点赋值用户数目 ,倒着来弄，因为从最底层开始节点号大（三位） 
	for(int i=444;i>0;--i){
	    childuserct=0;
	    //最小网格四个一组 
		for(int j=1;j<=4;++j){
			childuserct=childuserct+Quadnode[Quadnode[i].child[j]].userct;
		}
		//如果某节点没记录用户数且他儿子有用户数，则表示他为上层节点
		//把儿子用户数的和，第三个儿子节点坐标范围复制过来 
		if(Quadnode[i].userct==0&&childuserct!=0){
			Quadnode[i].rang=Quadnode[Quadnode[i].child[2]].rang;
			Quadnode[i].userct=childuserct;
		}
		//给各个节点记录上是第几层网格
		//3位数的节点号代表第三层
		//2位数第二层
		//1位数第一层 
		if(i>=111){
			Quadnode[i].layer=3; 
		}else if(i>=11){
			Quadnode[i].layer=2;
		}else{
			Quadnode[i].layer=1;
		}
	} 
	
} 
//judge计算某个时刻某个节点 的CID 
//！！ 
bool cmp(cidtemp a,cidtemp b){
	if(a.userct!=b.userct){
		return a.userct>b.userct;
	}else{
		//如果用户数目一样的话就选号码小的放前面 
		return a.quadnodeid<b.quadnodeid;
	}
} 
//！！ 
void judge(int nownode, int nowquadnode){
	//nowA为当前节点已计算在内的区域大小 
	//nowK为当前节点已计算在内的匿名度（周围用户数）
	//nowtemp为当前的节点，要改变 
	int nowA=0;
	int nowK=0;
	int nowtemp=0;
	//如果当前节点的所需区域小于4 
	if(Node[nownode].A<=4){
		//如果小于4的区域用户数不够，那就都加上先 
		if(Quadnode[Quadnode[nowquadnode].fa].userct<=Node[nownode].K){
			nowK=nowK+Quadnode[Quadnode[nowquadnode].fa].userct;
			nowA=nowA+4;
			//！！！！ 
			//把这四个节点的父亲的CID加入到该用户节点里 
			Node[nownode].CID[t].push_back(Quadnode[nowquadnode].fa);
			//因为在四叉树上是从左往右，加完加4，好算 
			nowquadnode=Quadnode[nowquadnode].fa; 
	    //如果小于4的区域用户数够，那就一个一个加  ，看是不是都用不了4个 
		}else if(Quadnode[Quadnode[nowquadnode].fa].userct>Node[nownode].K){
			//！！ 
			//先加当前的 
			Node[nownode].CID[t].push_back(nowquadnode);
			nowK=nowK+Quadnode[nowquadnode].userct; 
			nowA++;
			nowtemp=Quadnode[Quadnode[nowquadnode].fa].child[1];
			for(int i=0;i<4;++i){
				cidTemp[i].quadnodeid=nowtemp;
				cidTemp[i].userct=Quadnode[nowtemp].userct;  
				nowtemp++;
			} 
			sort(cidTemp,cidTemp+4,cmp); 
			for(int i=0;i<4;++i){
				if(cidTemp[i].quadnodeid!=nowquadnode){
					Node[nownode].CID[t].push_back(cidTemp[i].quadnodeid);
				    nowK=nowK+cidTemp[i].userct;
			    	nowA++; 
		    	}
			   //如果够了就返回，不够继续直到加完4个 
			   if(nowK>=Node[nownode].K&&nowA>=Node[nownode].A){
			     	Node[nownode].nowK[t]=nowK;
			     	Node[nownode].nowA[t]=nowA;
			      	return;
		    	}
			}
		}
	//如果所需区域大于4，那就把当前的最小网格的父节点CID算里 
	}else{
		nowK=nowK+Quadnode[Quadnode[nowquadnode].fa].userct;
		nowA+=4;
		Node[nownode].CID[t].push_back(Quadnode[nowquadnode].fa);
		nowquadnode=Quadnode[nowquadnode].fa; 
		//然后再找兄弟 ，直到A和K都满足要求 
		nowtemp=Quadnode[Quadnode[nowquadnode].fa].child[1];
		for(int i=0;i<4;++i){
			cidTemp[i].quadnodeid=nowtemp;
			cidTemp[i].userct=Quadnode[nowtemp].userct;  
			nowtemp++;
		} 
    	sort(cidTemp,cidTemp+4,cmp); 
    	//排序完的四个依次加入，如果是刚才加的就不加了 
    	for(int i=0;i<4;++i){
			if(cidTemp[i].quadnodeid!=nowquadnode){
				Node[nownode].CID[t].push_back(cidTemp[i].quadnodeid);
			    nowK=nowK+cidTemp[i].userct;
			   	nowA+=4; 
		   	}
			//如果够了就返回，不够继续直到加完4个 
			if(nowK>=Node[nownode].K&&nowA>=Node[nownode].A){
				Node[nownode].nowK[t]=nowK;
			    Node[nownode].nowA[t]=nowA;
			   	return;
		   	}
		}
	} 
	nowtemp=Quadnode[Quadnode[nowquadnode].fa].child[1];
	for(int i=0;i<4;++i){
		cidTemp[i].quadnodeid=nowtemp;
		cidTemp[i].userct=Quadnode[nowtemp].userct;  
		nowtemp++;
	} 
   	sort(cidTemp,cidTemp+4,cmp); 
   	//排序完的四个依次加入，如果是刚才加的就不加了 
   	for(int i=0;i<4;++i){
		if(cidTemp[i].quadnodeid!=nowquadnode){
			Node[nownode].CID[t].push_back(cidTemp[i].quadnodeid);
		    nowK=nowK+cidTemp[i].userct;
		   	nowA+=4; 
	  	}
		//如果够了就返回，不够继续直到加完4个 
		if(nowK>=Node[nownode].K&&nowA>=Node[nownode].A){
			Node[nownode].nowK[t]=nowK;
			Node[nownode].nowA[t]=nowA; 
		   	return;
	   	}
	}
}
//find_rang寻找某用户节点所在网格的坐标 
quadrang find_rang(int x,int y){
	quadrang range;
	int rx=0;
	int ry=0;
	//取整，因为网格坐标为左下角坐标 
	rx=x/15*15;
	ry=y/15*15;
	//赋值给range 
	range.x=rx;
	range.y=ry;
	return range;
}
//find_quadnode找某个四叉树节点的坐标范围 
int find_quadnode(quadrang range){
	//找最底层的。如果他和当前用户节点的相同，则返回这个四叉树节点 
	//!!
	for(int i=111;i<=444;++i){
		if(Quadnode[i].rang.x==range.x&&Quadnode[i].rang.y==range.y&&Quadnode[i].userct!=0){
			return i;
		}
	}
	//如果x或者y等于120，为区域边界 -15处理一下在从新找一遍 
	if(range.x==120){
		range.x=105;
	} 
	if(range.y==120){
		range.y=105;
	}
	for(int i=111;i<=444;++i){
		if(Quadnode[i].rang.x==range.x&&Quadnode[i].rang.y==range.y){
			return i;
		}
	}
} 
//adjust_cid调整cid
//因为最开始存入的每个时刻每个用户的cid为最底层的
//4个可能组成一个第二层节点，或者16个组成1个第一层节点，用最高层的代替 
void  adjust_cid(int nownode){
	int merge=0;
	if(Node[nownode].nowA[t]==4){
		merge=Node[nownode].CID[t][1];
		merge=merge/10;
		cid.push_back(merge);
		//cid为处理过的，和用户节点原来的交换数据 
    	Node[nownode].CID[t].swap(cid);  
    	//清空cid，因为下一时刻的t还需要用干净的cid 
	    cid.clear();
	} else if(Node[nownode].nowA[t]==16){
		merge=Node[nownode].CID[t][1];
		merge=merge/10;
		cid.push_back(merge);
		//cid为处理过的，和用户节点原来的交换数据 
    	Node[nownode].CID[t].swap(cid);  
    	//清空cid，因为下一时刻的t还需要用干净的cid 
	    cid.clear();
	}
} 
//compute_cid计算cid 
void compute_cid(){
	//nownode当前用户节点 
	//nowQuadnode当前四叉树节点 
	//range当前xy对应的网格所代表范围 
	int nownode;
	int nowQuadnode;
	quadrang range;
     //int kk=1;
	//坐标轴每个坐标遍历，看是否有用户 
	for(int x=1;x<=120;++x){
		for(int y=1;y<=120;++y){
			//有用户就计算这个坐标对应网格范围 
			//在对应范围找四叉树节点
			//在计算该节点cid 
			if(grid[t][x][y]!=0){
				//如果某点有用户，就把它的编号赋值给当前用户号nownode 
				nownode=grid[t][x][y];
				//根据当前用户的位置x，y找到它所在网格的左下角坐标（这个坐标代表这个网格） 
				range=find_rang(x,y); 
				//在根据刚才找的当前用户所在网格的x、y，找到它对应的四叉树节点（地图上的编号） 
				nowQuadnode=find_quadnode(range);
			/*	if(nownode==8){
					cout<<"x="<<x<<" y="<<y<<" range x="<<range.x<<" range y="<<range.y
					<<" nowQuadnode= "<<nowQuadnode<<"\n";
				} */
				judge(nownode,nowQuadnode);
			    
			   // cout<<kk++<<" rx="<<x<<" ry="<<y<<" x="<<range.x<<" y="<<range.y<<" nownode="<<nownode<<" nowQuadnode="<<nowQuadnode<<"\n";
			}
		}
	}
	//调整每个用户节点cid 
	for(int i=1;i<=1000;++i){
		adjust_cid(i);
	}
	
} 
int main(){
	double resk=0.0,resA=0.0;
    init_user();
	//100个时刻 
	for(t=1;t<=100;++t){
		//初始化用户 
        //初始化四叉树 
    	init_quadtree_userct();
    	//初始化四叉树节点层次 
    	init_layer(); 
    	//输出初始用户节点Node的编号、K和A（未计算CID） 
    /*	if(t==1){
    		char strnode[100]="F:\\casper\\node\\t=%dnode%d.txt";
            for(int n=1; n<=1000; ++n){
                sprintf(strnode,"F:\\casper\\node\\t=%dnode%d.txt",t,n);
                ofstream out(strnode);
                out<<"n="<<n<<" K="<<Node[n].K<<" A="<<Node[n].A<<" "; 
                out.close();
            }
            //输出当前时刻 四叉树节点Quadnode的编号和该编号网格内的用户数 
            char strquadnode[100]="F:\\casper\\quadnode\\t=%dquadnode%d.txt";
            for(int n=1; n<=444; ++n){
            	if(Quadnode[Quadnode[n].fa].userct!=0||Quadnode[n].userct!=0){
                    sprintf(strquadnode,"F:\\casper\\quadnode\\t=%dquadnode%d.txt",t,n);
                    ofstream out(strquadnode);
                    out<<"n="<<n<<" userct="<<Quadnode[n].userct<<" layer="<<Quadnode[n].layer<<"\n"; 
                    out.close();
		    	}
            } 
		}*/
    	//计算cid 
    	compute_cid(); 
    	//输出每个时刻1000个节点平均的nowk/k和A/nowa 
    	cout<<"t="<<t<<":"<<"\n"; 
    	double everyk=0,lastk=0;
     	for(int i=1;i<=1000;++i){
   	    	everyk=(double)Node[i].nowK[t]/(double)Node[i].K;
   	    	lastk+=everyk;
        } 
        //resk是100个时刻的 nowk/
        //double resk;
		resk+= lastk/1000;
        cout<<"nowk/k="<<lastk/1000<<endl;
        
        double everyA=0,lastA=0;
     	for(int i=1;i<=1000;++i){
   	    	everyA=(double)Node[i].A/(double)Node[i].nowA[t];
   	    	lastA+=everyA;
        } 
        //resA是100个时刻的 A/nowA
      //  double resA;
		resA+= lastA/1000;
        cout<<"A/nowA="<<lastA/1000<<endl;
    	//	输出计算完CID的用户节点Node的编号、K、A和整理之后的CID 
    /*	char strcid[100]="F:\\casper\\cid\\t=%dcid%d.txt";
        for(int n=1; n<=1000; ++n){
        
            sprintf(strcid,"F:\\casper\\cid\\t=%dcid%d.txt",t,n);
            ofstream out(strcid);
            out<<"n="<<n<<" A="<<Node[n].A<<" K="<<Node[n].K<<" nowk="<<Node[n].nowK[t]
	    	<<" nowA="<<Node[n].nowA[t]<<" nowk/k="
			<<setprecision(2)<<fixed<<(double)Node[n].K/(double)Node[n].nowK[t]<<" CID=";
			for(int i=0;i<Node[n].CID[t].size();++i){
				out<<Node[n].CID[t][i]<<" ";
			} 
			out<<"\n";
            out.close();
        }*/
        if(t==100){
        	cout<<"\n"; 
        	cout<<"100个时刻平均nowk/k="<<resk/100<<"\n"; 
			cout<<"100个时刻平均A/nowA="<<resA/100; 
		}
	}
	return 0;
} 
