#include<bits/stdc++.h>
using namespace std;
/*��casper̰�� 
UID�����ƶ��û�����ݱ�ʶ�����ִ����û�1-1000 
CID����������������ײ㵥Ԫ���ţ�����100��ʱ�� ÿ��ʱ��ÿ���û��� 
location��ʾ�����ƶ��û��ĵ���λ����Ϣ����xy�������������ʹ�����xy ������Ҫ�������� 

profile��ʾ�ƶ��û�����˽�������ԣ� K��A 
K����һ���������С�����ȣ�����С�û�������
A��ʾ�����������С�����
1000ge �ֲ���120*120�� 14400��  14����1��  ��С����15*15�� 225 ��16�� K=0-300�� 

1.��ʼ�� 120*120xy����  100��ʱ��1000���ڵ�     ----100��xy����ͼ����ά���飩 
ÿ���ڵ���һ���ṹ�������һ���������CID��K��A����vector��������0-100ʱ�̵� CID
2.�������û���Ŀ��������������˳ʱ��������һ������     ��ͼ1234˳��С����4��������һ��CID 
  1 * 2
 *******
  3 * 4
*/
//quadrangΪ��������ĳ�ڵ����������ʾ��x y��Χ �����������½ǵ����꣩ 
struct quadrang{
	int x;
	int y;
};
//quadnodeΪ�Ĳ����ڵ㣬��ʾ64����С����16�� ��С����4��������
//quadrangΪÿ���������½�����
//userctΪ���������û�����
//layerΪ�����������ǵڼ�������
//child���������ʾ��Ҷ�ӽڵ�ĺ����±�Ϊ4��
//fa��ʾ�ýڵ�ĸ��ڵ��±� 
struct quadnode{
	quadrang rang;
	int userct;
	int layer;
	int child[5];
    int fa;
};
//���� 
struct cidtemp{
	int quadnodeid;
	int userct;
}cidTemp[4];
//cidtemp cidTemp[4];
//�½�450���Ĳ����ڵ㣬��Ϊÿ������ڵ�Ϊ
//0��ʾ�������� 
// 1 2 3 4 ��ʾ������� 
//11 12 13 14 21 22 23 24 31 32 33 34 41 41 43 44 ��ʾ�δ����� 
//111 112 113 114 121 122 123 124.....441 442 443 444��ʾ��С���� 
//�������ڵ��±�Ϊ444�������½�450������ 
quadnode Quadnode[450];
//nodeΪ�û��ڵ� 
//ÿ���û��и���Ҫ��ĸ����û���K��������С�����СA �ǲ����
//��С��������CID��ÿ���û��������귶Χ�����vector���Ϊÿ���ڵ���100��ʱ�̵���Ϣ 
struct node{
	vector<int>CID[110];//3���͹� 
	int nowK[110]; 
	int nowA[110];
	vector<quadrang>rang; 
	int K;
	int A; 
}Node[1010];
//t��ʾĳʱ�� 
int t;
//1gridΪ00��ʱ�̵�120*120�����ڵĽڵ�ֲ� 
int grid[110][130][130];
//cid����adjust_cid��������CID�� 
vector<int>cid; 
//random����ĳ����Χ������ĺ��� 
int random(int low,int high){
	int result=0;
	result= low+rand()%(high-low+1);
	return result;
}
//init_user��ʼ���û� 
void init_user(){
	//100��ʱ�̣�1000���û���ÿ���û�ÿ��ʱ����120*120�����������ýڵ�λ�� 
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
	//cout<<"����"<<"\n"; 
	//1-1000���û��ڵ�ֻ��Ҫ�ʼ����һ��K��A 
	for(int i=1;i<=1000;++i){
		Node[i].K=random(1,35);
		Node[i].A=random(1,5);
	} 
}
//init_quadtree_userct��ʼ���Ĳ��� 
//����ÿ���û�����������û���������ʾ��Χ�����������ڶ�Ӧ�Ĳ�������� 
void  init_quadtree_userct(){
	//   60  60Ϊ�������� ���·ֱ�Ϊÿ��������������  ���һ��-15������ײ��������½����� 
	//  x-30,y+30  x+30,y+30  x-30,y-30  x+30,y-30
	//  x-15��y+15   x+15��y+15   x-15��y-15   x+15��y-15 
	//  x-15��y   x��y   x-15,y-15    x,y-15 
	//���������ﶯ̬�ı��������ķ�Χ���� 
	int X1[4]={-30,30,-30,30};  //1234
	int Y1[4]={30,30,-30,-30};
	
	int X2[4]={-15,15,-15,15};//1234
	int Y2[4]={15,15,-15,-15};
	
	int X3[4]={-15,0,-15,0};//1234
	int Y3[4]={0,0,-15,-15};
	//x y��¼ÿ������Χ 
	int x,y;
	//gridNum������Ŀ   ++�� 
//	int gridNum=1;
	//������±�Ϊ0 �����ڵ��޸��ڵ㣬��������Ϊ-1 
	int root=0;
	Quadnode[root].fa=-1;
	//����ѭ��ֱ���������Ữ�ֵ���ײ㣬ÿ��ѭ����Ҫ�ָ����꣬���Լ��ϵ�Ҫ��ȥ 
	//���ѭ��������ķ�Χ����������ڽڵ����Ĳ�����ĸ��ӹ�ϵ��������
	//id��ʾ���ӽڵ��±� 
	for(int i=0;i<4;++i){
		x=60+X1[i];
		y=60+Y1[i];
		//��ʼ����idΪ0��4��ѭ�������ĸ����� 
		int id=0;
		id=id*10+i+1;
        //�ʼ�����ø��ڵ�Ķ��Ӻ����������ӵĸ��ڵ����� 
		Quadnode[root].child[i+1]=id;
		Quadnode[id].fa=root;
		//ÿ�����ӽڵ����������Ķ��Ӻ����Ĺ�ϵ���Լ���ʾ��Χ����������nowΪid��ʾ����Ҫ�����ĵ�ǰ�ڵ�
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
				//nowx nowy��ʾ��С�������½����꣬����ͳ��������ÿ��С�����꣬��Ҫ�ı� 
				int nowx=x,nowy=y;
				// ÿ�������ڵ��û��� 
				int userct=0;  
				for(int m=0;m<15;++m){
					for(int n=0;n<15;++n){
						//���ĳʱ�̸�������ĳλ�ò�Ϊ0����ʾ��λ�����û�����Ŀ++ 
						if(grid[t][nowx+m][nowy+n]!=0){
							userct++;
						}
					}
				} 
				//������ӽڵ�id 
				id=id*10+k+1;
				Quadnode[now].child[k+1]=id;
				Quadnode[id].fa=now;
				//�Ѹղż���ĸ�����ķ�Χxy���û��������Ӧ���Ĳ����ڵ� 
				Quadnode[id].userct=userct;
				Quadnode[id].rang.x=x;
				Quadnode[id].rang.y=y; 
				//�ָ�id 
				id=(id-k-1)/10; 
				//cout<<ct<<" "; 
				x=x-X3[k];
				y=y-Y3[k];
			}
			x=x-X2[j];
			y=y-Y2[j]; 
			//�ָ�id 
			id=(id-j-1)/10; 
			//�ָ����ڵ� �������� 
			now=now/10; 
		}
		x=x-X1[i];
		y=y-Y1[i]; 
		//�ָ�id 
		id=(id-i-1)/10; 
	}
}
//init_layer��ʼ���Ĳ����ڵ�Ĳ�Σ�������ǵײ�ڵ���û����ͷ�Χ 
void init_layer(){
	//�����Ĳ������ӽڵ��û��� 
	int childuserct;
	//����ѭ������ǵײ�ڵ���û������������Ķ��ӽڵ��û������ 
	//ÿ���ڵ���� ���Ӵ�С�������С����ʼ 
	//��1  2��ڵ㸳ֵ�û���Ŀ ,������Ū����Ϊ����ײ㿪ʼ�ڵ�Ŵ���λ�� 
	for(int i=444;i>0;--i){
	    childuserct=0;
	    //��С�����ĸ�һ�� 
		for(int j=1;j<=4;++j){
			childuserct=childuserct+Quadnode[Quadnode[i].child[j]].userct;
		}
		//���ĳ�ڵ�û��¼�û��������������û��������ʾ��Ϊ�ϲ�ڵ�
		//�Ѷ����û����ĺͣ����������ӽڵ����귶Χ���ƹ��� 
		if(Quadnode[i].userct==0&&childuserct!=0){
			Quadnode[i].rang=Quadnode[Quadnode[i].child[2]].rang;
			Quadnode[i].userct=childuserct;
		}
		//�������ڵ��¼���ǵڼ�������
		//3λ���Ľڵ�Ŵ��������
		//2λ���ڶ���
		//1λ����һ�� 
		if(i>=111){
			Quadnode[i].layer=3; 
		}else if(i>=11){
			Quadnode[i].layer=2;
		}else{
			Quadnode[i].layer=1;
		}
	} 
	
} 
//judge����ĳ��ʱ��ĳ���ڵ� ��CID 
//���� 
bool cmp(cidtemp a,cidtemp b){
	if(a.userct!=b.userct){
		return a.userct>b.userct;
	}else{
		//����û���Ŀһ���Ļ���ѡ����С�ķ�ǰ�� 
		return a.quadnodeid<b.quadnodeid;
	}
} 
//���� 
void judge(int nownode, int nowquadnode){
	//nowAΪ��ǰ�ڵ��Ѽ������ڵ������С 
	//nowKΪ��ǰ�ڵ��Ѽ������ڵ������ȣ���Χ�û�����
	//nowtempΪ��ǰ�Ľڵ㣬Ҫ�ı� 
	int nowA=0;
	int nowK=0;
	int nowtemp=0;
	//�����ǰ�ڵ����������С��4 
	if(Node[nownode].A<=4){
		//���С��4�������û����������ǾͶ������� 
		if(Quadnode[Quadnode[nowquadnode].fa].userct<=Node[nownode].K){
			nowK=nowK+Quadnode[Quadnode[nowquadnode].fa].userct;
			nowA=nowA+4;
			//�������� 
			//�����ĸ��ڵ�ĸ��׵�CID���뵽���û��ڵ��� 
			Node[nownode].CID[t].push_back(Quadnode[nowquadnode].fa);
			//��Ϊ���Ĳ������Ǵ������ң������4������ 
			nowquadnode=Quadnode[nowquadnode].fa; 
	    //���С��4�������û��������Ǿ�һ��һ����  �����ǲ��Ƕ��ò���4�� 
		}else if(Quadnode[Quadnode[nowquadnode].fa].userct>Node[nownode].K){
			//���� 
			//�ȼӵ�ǰ�� 
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
			   //������˾ͷ��أ���������ֱ������4�� 
			   if(nowK>=Node[nownode].K&&nowA>=Node[nownode].A){
			     	Node[nownode].nowK[t]=nowK;
			     	Node[nownode].nowA[t]=nowA;
			      	return;
		    	}
			}
		}
	//��������������4���ǾͰѵ�ǰ����С����ĸ��ڵ�CID���� 
	}else{
		nowK=nowK+Quadnode[Quadnode[nowquadnode].fa].userct;
		nowA+=4;
		Node[nownode].CID[t].push_back(Quadnode[nowquadnode].fa);
		nowquadnode=Quadnode[nowquadnode].fa; 
		//Ȼ�������ֵ� ��ֱ��A��K������Ҫ�� 
		nowtemp=Quadnode[Quadnode[nowquadnode].fa].child[1];
		for(int i=0;i<4;++i){
			cidTemp[i].quadnodeid=nowtemp;
			cidTemp[i].userct=Quadnode[nowtemp].userct;  
			nowtemp++;
		} 
    	sort(cidTemp,cidTemp+4,cmp); 
    	//��������ĸ����μ��룬����ǸղżӵľͲ����� 
    	for(int i=0;i<4;++i){
			if(cidTemp[i].quadnodeid!=nowquadnode){
				Node[nownode].CID[t].push_back(cidTemp[i].quadnodeid);
			    nowK=nowK+cidTemp[i].userct;
			   	nowA+=4; 
		   	}
			//������˾ͷ��أ���������ֱ������4�� 
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
   	//��������ĸ����μ��룬����ǸղżӵľͲ����� 
   	for(int i=0;i<4;++i){
		if(cidTemp[i].quadnodeid!=nowquadnode){
			Node[nownode].CID[t].push_back(cidTemp[i].quadnodeid);
		    nowK=nowK+cidTemp[i].userct;
		   	nowA+=4; 
	  	}
		//������˾ͷ��أ���������ֱ������4�� 
		if(nowK>=Node[nownode].K&&nowA>=Node[nownode].A){
			Node[nownode].nowK[t]=nowK;
			Node[nownode].nowA[t]=nowA; 
		   	return;
	   	}
	}
}
//find_rangѰ��ĳ�û��ڵ�������������� 
quadrang find_rang(int x,int y){
	quadrang range;
	int rx=0;
	int ry=0;
	//ȡ������Ϊ��������Ϊ���½����� 
	rx=x/15*15;
	ry=y/15*15;
	//��ֵ��range 
	range.x=rx;
	range.y=ry;
	return range;
}
//find_quadnode��ĳ���Ĳ����ڵ�����귶Χ 
int find_quadnode(quadrang range){
	//����ײ�ġ�������͵�ǰ�û��ڵ����ͬ���򷵻�����Ĳ����ڵ� 
	//!!
	for(int i=111;i<=444;++i){
		if(Quadnode[i].rang.x==range.x&&Quadnode[i].rang.y==range.y&&Quadnode[i].userct!=0){
			return i;
		}
	}
	//���x����y����120��Ϊ����߽� -15����һ���ڴ�����һ�� 
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
//adjust_cid����cid
//��Ϊ�ʼ�����ÿ��ʱ��ÿ���û���cidΪ��ײ��
//4���������һ���ڶ���ڵ㣬����16�����1����һ��ڵ㣬����߲�Ĵ��� 
void  adjust_cid(int nownode){
	int merge=0;
	if(Node[nownode].nowA[t]==4){
		merge=Node[nownode].CID[t][1];
		merge=merge/10;
		cid.push_back(merge);
		//cidΪ������ģ����û��ڵ�ԭ���Ľ������� 
    	Node[nownode].CID[t].swap(cid);  
    	//���cid����Ϊ��һʱ�̵�t����Ҫ�øɾ���cid 
	    cid.clear();
	} else if(Node[nownode].nowA[t]==16){
		merge=Node[nownode].CID[t][1];
		merge=merge/10;
		cid.push_back(merge);
		//cidΪ������ģ����û��ڵ�ԭ���Ľ������� 
    	Node[nownode].CID[t].swap(cid);  
    	//���cid����Ϊ��һʱ�̵�t����Ҫ�øɾ���cid 
	    cid.clear();
	}
} 
//compute_cid����cid 
void compute_cid(){
	//nownode��ǰ�û��ڵ� 
	//nowQuadnode��ǰ�Ĳ����ڵ� 
	//range��ǰxy��Ӧ������������Χ 
	int nownode;
	int nowQuadnode;
	quadrang range;
     //int kk=1;
	//������ÿ��������������Ƿ����û� 
	for(int x=1;x<=120;++x){
		for(int y=1;y<=120;++y){
			//���û��ͼ�����������Ӧ����Χ 
			//�ڶ�Ӧ��Χ���Ĳ����ڵ�
			//�ڼ���ýڵ�cid 
			if(grid[t][x][y]!=0){
				//���ĳ�����û����Ͱ����ı�Ÿ�ֵ����ǰ�û���nownode 
				nownode=grid[t][x][y];
				//���ݵ�ǰ�û���λ��x��y�ҵ���������������½����꣨����������������� 
				range=find_rang(x,y); 
				//�ڸ��ݸղ��ҵĵ�ǰ�û����������x��y���ҵ�����Ӧ���Ĳ����ڵ㣨��ͼ�ϵı�ţ� 
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
	//����ÿ���û��ڵ�cid 
	for(int i=1;i<=1000;++i){
		adjust_cid(i);
	}
	
} 
int main(){
	double resk=0.0,resA=0.0;
    init_user();
	//100��ʱ�� 
	for(t=1;t<=100;++t){
		//��ʼ���û� 
        //��ʼ���Ĳ��� 
    	init_quadtree_userct();
    	//��ʼ���Ĳ����ڵ��� 
    	init_layer(); 
    	//�����ʼ�û��ڵ�Node�ı�š�K��A��δ����CID�� 
    /*	if(t==1){
    		char strnode[100]="F:\\casper\\node\\t=%dnode%d.txt";
            for(int n=1; n<=1000; ++n){
                sprintf(strnode,"F:\\casper\\node\\t=%dnode%d.txt",t,n);
                ofstream out(strnode);
                out<<"n="<<n<<" K="<<Node[n].K<<" A="<<Node[n].A<<" "; 
                out.close();
            }
            //�����ǰʱ�� �Ĳ����ڵ�Quadnode�ı�ź͸ñ�������ڵ��û��� 
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
    	//����cid 
    	compute_cid(); 
    	//���ÿ��ʱ��1000���ڵ�ƽ����nowk/k��A/nowa 
    	cout<<"t="<<t<<":"<<"\n"; 
    	double everyk=0,lastk=0;
     	for(int i=1;i<=1000;++i){
   	    	everyk=(double)Node[i].nowK[t]/(double)Node[i].K;
   	    	lastk+=everyk;
        } 
        //resk��100��ʱ�̵� nowk/
        //double resk;
		resk+= lastk/1000;
        cout<<"nowk/k="<<lastk/1000<<endl;
        
        double everyA=0,lastA=0;
     	for(int i=1;i<=1000;++i){
   	    	everyA=(double)Node[i].A/(double)Node[i].nowA[t];
   	    	lastA+=everyA;
        } 
        //resA��100��ʱ�̵� A/nowA
      //  double resA;
		resA+= lastA/1000;
        cout<<"A/nowA="<<lastA/1000<<endl;
    	//	���������CID���û��ڵ�Node�ı�š�K��A������֮���CID 
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
        	cout<<"100��ʱ��ƽ��nowk/k="<<resk/100<<"\n"; 
			cout<<"100��ʱ��ƽ��A/nowA="<<resA/100; 
		}
	}
	return 0;
} 
